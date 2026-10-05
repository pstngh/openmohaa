/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMoHAA source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMoHAA source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// humanbot_adapter.cpp: engine glue of the human-imitation bots.
//
// Every bot owns one HumanBotAdapter. Each server frame all bots first perceive
// and decide on the same world snapshot (Prepare), then send their usercmds
// (Commit): K sub-step commands per frame with digital keys, like a human
// client. The brain (code/humanbot) only ever sees the noisy observations built
// by the perception glue; stock bot code runs only for doors and unstuck
// recovery, and ladders are climbed by a fixed rule (Ladder()).

#include "humanbot_internal.h"
#include "playerbot.h"
#include "movement_telemetry.h"
#include "navigation_path.h"
#include "weapon.h"
#include "doors.h"
#include "misc.h"

#include <chrono>
#include <cmath>
#include <cstring>
#include <map>

cvar_t *g_humanbot_substeps;
cvar_t *g_humanbot_seed;
cvar_t *g_humanbot_disguise;
cvar_t *g_humanbot_model_dir;
cvar_t *g_humanbot_families;
cvar_t *g_humanbot_debug;
cvar_t *g_humanbot_wall_steer;
cvar_t *g_humanbot_skill;
static cvar_t *g_humanbot_door_ahead;
static cvar_t *g_humanbot_door_hold;
static cvar_t *g_humanbot_door_go;

//
// Model
//
static hb::ModelBundle s_bundle;
static bool            s_bundleOk = false;
static std::string     s_modelError;

const hb::ModelBundle *HB_Bundle()
{
    return s_bundleOk ? &s_bundle : nullptr;
}

static bool ReadText(const std::string& path, std::string& out)
{
    void *buffer = NULL;
    long  len    = gi.FS_ReadFile(path.c_str(), &buffer, qtrue);
    if (len <= 0 || !buffer) {
        return false;
    }
    out.assign(static_cast<const char *>(buffer), static_cast<size_t>(len));
    gi.FS_FreeFile(buffer);
    return true;
}

bool HB_LoadModel(std::string& error)
{
    std::map<std::string, std::string> overrides;
    if (g_humanbot_model_dir && g_humanbot_model_dir->string[0]) {
        const std::string dir = g_humanbot_model_dir->string;
        for (const char *name : {"shared.json", "styles.json", "calibration.json"}) {
            std::string text;
            if (ReadText(dir + "/" + name, text)) {
                overrides[name] = text;
            }
        }
    }
    const bool ok = hb::LoadBundle(overrides, s_bundle, error);
    s_bundleOk    = !s_bundle.sha256.empty();
    s_modelError  = error;
    return ok;
}

//
// Style draws for new bots (per server, seeded from g_humanbot_seed)
//
static hb::Rng s_styleRng;
static bool    s_styleSeeded  = false;
static int     s_pendingFamily = -1;

int HB_PendingFamily()
{
    return s_pendingFamily;
}

void HB_SetPendingFamily(int family)
{
    s_pendingFamily = family;
}

static uint64_t BaseSeed()
{
    if (g_humanbot_seed && g_humanbot_seed->string[0] && Q_stricmp(g_humanbot_seed->string, "0")) {
        return hb::HashString(g_humanbot_seed->string);
    }
    return static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count())
         ^ (static_cast<uint64_t>(time(NULL)) << 20);
}

static bool FamilyWeights(float w[hb::FAMILY_COUNT])
{
    if (!g_humanbot_families || !g_humanbot_families->string[0]) {
        return false;
    }
    float sum = 0.0f;
    if (sscanf(g_humanbot_families->string, "%f %f %f", &w[0], &w[1], &w[2]) != 3) {
        return false;
    }
    for (int i = 0; i < hb::FAMILY_COUNT; i++) {
        w[i] = std::max(0.0f, w[i]);
        sum += w[i];
    }
    return sum > 0.0f;
}

//
// Event buses: what happened during the last frame, delivered to every bot at
// the start of the next one
//
static std::vector<HbSoundEvent>  s_soundsPending, s_soundsTick;
static std::vector<HbDamageEvent> s_damagePending, s_damageTick;
static std::vector<int>           s_deathsPending, s_deathsTick;

static const float SOUND_MAX_RANGE = 4000.0f;
static const int   MAX_FALL_HEIGHT = 400;   // like the stock bots' paths

//
// The per-bot adapter
//
class HumanBotAdapter
{
public:
    HumanBotAdapter(BotController *controller, Player *player);
    ~HumanBotAdapter();

    void Prepare();
    void Commit();
    void Spawned();
    void Killed();
    void GotKill(Entity *victim);
    void HeadHit() { m_headHit = true; }

    void Reinit();

    Player             *GetPlayer() const { return m_player; }
    const hb::Diag&      GetDiag() const { return m_diag; }
    bool                 DiagValid() const { return m_diagValid; }
    const hb::StyleDials& Dials() const { return m_dials; }
    int                  Owner() const { return m_owner; }
    float                MeanThinkUs() const { return m_thinkCount ? static_cast<float>(m_thinkSum / m_thinkCount) : 0.0f; }
    int                  MaxThinkUs() const { return m_thinkMax; }
    // the frame's usercmds through G_ClientThink (Pmove and the rest of the player's move), per frame
    float                MeanCommitUs() const { return m_commitCount ? static_cast<float>(m_commitSum / m_commitCount) : 0.0f; }
    int                  KbdViolations() const { return m_kbdViolations; }
    int                  StuckBouts() const { return m_stuckBouts; }
    int                  PressureBouts() const { return m_pressureBouts; }
    void                 ResetCounters();
    int                  Ping() const { return m_pingMs; }

private:
    void ChooseStyle();
    void Join(Player *p);
    void Track(Player *p, const hb::SelfState& self);
    void Steering(Player *p, const hb::TickPlan& plan, const hb::SelfState& self);
    void Owners(Player *p, const HbView& view, const hb::SelfState& self, hb::TickPlan& plan);
    void Ladder(Player *p, const HbView& view, const Vector& goal, hb::TickPlan& plan);
    void StartDoorPass(Player *p, Door *d);
    bool DoorPass(Player *p, const HbView& view, const hb::SelfState& self, hb::TickPlan& plan);
    bool Opponent(Player *self, Player *other) const;
    bool TeammateInCrosshair(Player *p, const HbView& view) const;
    void SetOwner(Player *p, int owner);

    BotController  *m_controller;
    SafePtr<Player> m_player;

    hb::StyleDials  m_dials;
    hb::Brain       m_brain;
    hb::Perceiver   m_perceiver;
    hb::Substepper  m_sub;
    hb::Rng         m_rngSub;
    hb::Rng         m_rngPresent;
    hb::PingModel   m_ping;
    hb::EyeState    m_eye;
    const hb::MapPrior *m_prior = nullptr;

    // this frame's decisions
    std::vector<hb::SubCmd> m_cmds;
    std::vector<Vector>     m_eyes;
    int                     m_command = hb::CMD_NONE;
    hb::Diag                m_diag;
    bool                    m_diagValid = false;
    int                     m_tick      = 0;
    int                     m_substeps  = 4;

    // joining
    bool m_joinPosted = false;

    // own movement bookkeeping
    Vector m_lastOrigin;
    int    m_lastChord  = hb::CHORD_NEUTRAL;
    float  m_lastClear  = 128.0f;
    float  m_stuckMs    = 0.0f;
    float  m_wallMs     = 0.0f;
    float  m_drop[hb::NUM_CHORDS] = {};

    // navigation mesh steering toward the brain's goal
    IPather *m_pather      = nullptr;
    Vector   m_pathGoal;
    int      m_pathTime    = -100000;
    bool     m_steerValid  = false;
    float    m_steerYaw    = 0.0f;
    float    m_pathLen     = 0.0f;
    int      m_nCorners    = 0;   // the straightened path's next corners (the view leads along them)
    bool     m_doorValid   = false;   // a closed door across the way ahead (DoorAhead)
    Vector   m_door;
    Vector   m_corners[hb::MAX_NAV_CORNERS];

    // special owners
    int m_owner          = hb::OWNER_BRAIN;
    int m_recoveryUntil  = 0;
    int m_useUntil       = 0;

    // the door the bot opened, while it goes through (DoorPass())
    int    m_passDoor  = -1;
    int    m_passStart = 0;
    float  m_passYaw   = 0.0f;   // the door's yaw closed
    Vector m_passCenter;          // the doorway's middle
    Vector m_passHinge;           // the door's origin, where it turns
    Vector m_passNormal;          // across the doorway, from the bot's side
    int    m_passLean  = 0;       // the lean toward the way through (-1 left, 1 right)
    bool   m_engaged   = false;   // the focus enemy seen this tick

    // the climb in progress (Ladder())
    int   m_ladderSince    = 0;
    int   m_ladderDir      = 1;
    bool  m_ladderReversed = false;
    float m_ladderPitch    = 0.0f;
    float m_ladderMoveZ    = 0.0f;
    int   m_ladderMoveTime = 0;
    int   m_ladderPatience = 0;
    bool  m_ladderJumped   = false;

    int m_gotKillOf = -1;
    bool m_headHit  = false;
    int m_focusId   = -1;
    int m_pingMs    = 0;

    // self-test counters
    double m_thinkSum      = 0.0;
    int    m_thinkCount    = 0;
    int    m_thinkMax      = 0;
    double m_commitSum     = 0.0;
    int    m_commitCount   = 0;
    int    m_kbdViolations = 0;
    int    m_stuckBouts    = 0;
    int    m_pressureBouts = 0;
};

static Container<HumanBotAdapter *> s_adapters;

// The map's geometry for the brain's own questions (the corners of believed paths). Players never block:
// CONTENTS_BODY is left out of the mask, so no answer depends on where an enemy stands.
class HbWorldQuery : public hb::WorldQuery
{
public:
    bool Clear(const hb::Vec3& a, const hb::Vec3& b) const override
    {
        const Vector s(a.x, a.y, a.z), e(b.x, b.y, b.z);
        return G_SightTrace(
            s, vec_zero, vec_zero, e, (Entity *)NULL, (Entity *)NULL, (MASK_SHOT & ~CONTENTS_TRIGGER) & ~CONTENTS_BODY,
            qfalse, "HumanBot corner"
        );
    }
};

static HbWorldQuery s_worldQuery;

HumanBotAdapter *HB_AdapterFor(const Player *player)
{
    for (int i = 1; i <= s_adapters.NumObjects(); i++) {
        HumanBotAdapter *a = s_adapters.ObjectAt(i);
        if (a->GetPlayer() == player) {
            return a;
        }
    }
    return nullptr;
}

HumanBotAdapter::HumanBotAdapter(BotController *controller, Player *player)
    : m_controller(controller)
    , m_player(player)
{
    const hb::ModelBundle& b = *HB_Bundle();
    ChooseStyle();
    m_substeps = g_humanbot_substeps ? hb::ClampI(g_humanbot_substeps->integer, 1, hb::MAX_SUBSTEPS) : 12;
    // every bot has its own generator; the seed comes from the server seed and the style
    const std::string seedText = std::string(g_humanbot_seed ? g_humanbot_seed->string : "") + "|" + hb::StyleKey(m_dials)
                               + "|" + std::to_string(player->entnum);
    const uint64_t    seed     = hb::HashString(seedText.c_str());
    const hb::Rng root(seed);
    m_brain.Init(&b, HB_WorldPrior(), m_dials, seed, m_substeps);
    m_brain.SetWorld(&s_worldQuery);
    m_perceiver.Init(&b.shared.perception, root.Derive(hb::STREAM_PERCEPTION));
    m_sub.Init(m_substeps);
    m_rngSub     = root.Derive(hb::STREAM_SUBSTEP);
    m_rngPresent = root.Derive(hb::STREAM_PRESENT);
    m_ping.Init(&b.shared.presentation, m_rngPresent);
    m_prior      = HB_WorldPrior();
    m_lastOrigin = player->origin;
    s_adapters.AddObject(this);
    G_MoveLogBotStyle(player, hb::DialsJson(m_dials).c_str());
}

// After a model reload: the same style, fresh modules on the new parameters.
void HumanBotAdapter::Reinit()
{
    Player *p = m_player;
    if (!p || !HB_Bundle()) {
        return;
    }
    const hb::ModelBundle& b    = *HB_Bundle();
    const std::string      seedText = std::string(g_humanbot_seed ? g_humanbot_seed->string : "") + "|"
                               + hb::StyleKey(m_dials) + "|" + std::to_string(p->entnum) + "|reload";
    const uint64_t         seed     = hb::HashString(seedText.c_str());
    const hb::Rng root(seed);
    m_dials = hb::SampleStyle(b.style, m_dials.family, m_dials.seed);
    m_brain.Init(&b, HB_WorldPrior(), m_dials, seed, m_substeps);
    m_brain.SetWorld(&s_worldQuery);
    m_perceiver.Init(&b.shared.perception, root.Derive(hb::STREAM_PERCEPTION));
    m_ping.Init(&b.shared.presentation, m_rngPresent);
    m_prior = HB_WorldPrior();
}

HumanBotAdapter::~HumanBotAdapter()
{
    s_adapters.RemoveObject(this);
    delete m_pather;
}

void HumanBotAdapter::ChooseStyle()
{
    const hb::ModelBundle& b      = *HB_Bundle();
    Player                *p      = m_player;
    int                    family = -1;
    uint32_t               seed   = 0;
    const char            *key    = Info_ValueForKey(p->client->pers.userinfo, "hb_style");
    if (!key || !key[0] || !hb::ParseStyleKey(key, family, seed)) {
        if (!s_styleSeeded) {
            s_styleRng    = hb::Rng(BaseSeed()).Derive(hb::STREAM_STYLE);
            s_styleSeeded = true;
        }
        family          = s_pendingFamily;
        s_pendingFamily = -1;
        seed            = static_cast<uint32_t>(s_styleRng.Next() >> 32);
    }
    float        w[hb::FAMILY_COUNT];
    const bool   weighted = family < 0 && FamilyWeights(w);
    m_dials               = hb::SampleStyle(b.style, family, seed, weighted ? w : nullptr);
    // keep it with the bot across map changes (G_SaveBots copies the userinfo)
    Info_SetValueForKey(p->client->pers.userinfo, "hb_style", hb::StyleKey(m_dials).c_str());
}

void HumanBotAdapter::ResetCounters()
{
    m_thinkSum      = 0.0;
    m_thinkCount    = 0;
    m_thinkMax      = 0;
    m_commitSum     = 0.0;
    m_commitCount   = 0;
    m_kbdViolations = 0;
    m_stuckBouts    = 0;
    m_pressureBouts = 0;
}

bool HumanBotAdapter::Opponent(Player *self, Player *other) const
{
    if (!other || other == self || !other->client) {
        return false;
    }
    if (other->GetTeam() == TEAM_NONE || other->GetTeam() == TEAM_SPECTATOR) {
        return false;
    }
    if (g_gametype->integer <= GT_FFA) {
        return true;
    }
    return other->GetTeam() != self->GetTeam();
}

bool HumanBotAdapter::TeammateInCrosshair(Player *p, const HbView& view) const
{
    if (g_gametype->integer <= GT_FFA) {
        return false;
    }
    Vector fwd;
    Vector(view.pitch, view.yaw, 0).AngleVectors(&fwd);
    const trace_t tr = G_Trace(view.eye, vec_zero, vec_zero, view.eye + fwd * 2048.0f, p, MASK_SHOT, qfalse, "HumanBot ff");
    if (tr.entityNum < 0 || tr.entityNum >= game.maxclients) {
        return false;
    }
    Entity *e = G_GetEntity(tr.entityNum);
    return e && e->IsSubclassOfPlayer() && static_cast<Player *>(e)->GetTeam() == p->GetTeam();
}

// Joins a side like a human: in free-for-all the side follows the weapon dial
// (axis carries the MP40, allies the Thompson); team games use the auto team.
void HumanBotAdapter::Join(Player *p)
{
    if (!p->client->pers.dm_primary[0] || Q_stricmp(p->client->pers.dm_primary, "smg")) {
        Event *event = new Event(EV_Player_PrimaryDMWeapon);
        event->AddString("smg");
        p->ProcessEvent(event);
    }
    if (p->GetTeam() != TEAM_NONE && p->GetTeam() != TEAM_SPECTATOR) {
        m_joinPosted = false;
        return;
    }
    if (m_joinPosted || p->EventPending(EV_Player_JoinDMTeam) || p->EventPending(EV_Player_AutoJoinDMTeam)) {
        return;
    }
    // a short, human-looking delay (and no two bots spawning on the same frame)
    const float delay = 0.5f + static_cast<float>(m_rngPresent.Uniform()) * 1.5f + p->entnum / 20.0f;
    if (g_gametype->integer <= GT_FFA) {
        Event *event = new Event(EV_Player_JoinDMTeam);
        event->AddString(m_rngPresent.Uniform() < m_dials.mp40Share ? "axis" : "allies");
        p->PostEvent(event, delay);
    } else {
        p->PostEvent(EV_Player_AutoJoinDMTeam, delay);
    }
    m_joinPosted = true;
}

// Stuck and wall-pressure clocks from last tick's keys and this tick's motion.
void HumanBotAdapter::Track(Player *p, const hb::SelfState& self)
{
    Vector moved = p->origin - m_lastOrigin;
    moved.z      = 0.0f;
    const bool pressing = m_lastChord != hb::CHORD_NEUTRAL && self.alive && !self.spectator && !self.onLadder;
    if (pressing && moved.length() < 2.0f) {
        m_stuckMs += 50.0f;
    } else {
        if (m_stuckMs >= 2000.0f) {
            m_stuckBouts++;
        }
        m_stuckMs = 0.0f;
    }
    if (pressing && m_lastClear < 8.0f && moved.length() < 5.0f) {
        m_wallMs += 50.0f;
    } else {
        if (m_wallMs >= 500.0f) {
            m_pressureBouts++;
        }
        m_wallMs = 0.0f;
    }
    m_lastOrigin = p->origin;
}

// People walk down the middle of a corridor and take corners wide (26% of their time within 16 u
// of a wall). The navigation mesh is built for a 1 u agent, so its corners lie about 10 u from the
// walls, inside the player's 15 u box: a bot heading for them scrapes every wall on the way. Walls
// closer than this push the route direction sideways, away from them (only the part across the
// route, so they never turn the bot back).
// g_humanbot_wall_steer scales the push (0 = off). At full strength (1, until 2026-10-05) the pushes of the walls on
// either side of a corridor took turns tick by tick (the steered direction swung across 15 times a minute) and the keys
// that follow it zig-zagged; at 0.5 the bots touch walls as often as people (9.8% of the time on dm/brownffa and
// dm/flag, people 9.1%; 5.5% on the duel maps, people 7.3%).
static const float WALL_STEER_MARGIN = 32.0f;

static float WallSteerYaw(float routeYaw, float viewYaw, const float clearance[hb::NUM_CHORDS])
{
    const float gain = g_humanbot_wall_steer ? g_humanbot_wall_steer->value : 0.0f;
    if (gain <= 0.0f) {
        return routeYaw;
    }
    const float px = std::cos(DEG2RAD(routeYaw));
    const float py = std::sin(DEG2RAD(routeYaw));
    float       rx = 0.0f, ry = 0.0f;
    for (int c = 0; c < hb::NUM_CHORDS; c++) {
        if (c == hb::CHORD_NEUTRAL || clearance[c] >= WALL_STEER_MARGIN) {
            continue;
        }
        // the probes are box traces: clearance is the gap between the box and the wall
        const float w = (WALL_STEER_MARGIN - clearance[c]) / WALL_STEER_MARGIN;
        const float a = DEG2RAD(viewYaw + hb::Mover::ChordAngle(c));
        rx -= w * std::cos(a);
        ry -= w * std::sin(a);
    }
    const float along = rx * px + ry * py;
    rx -= along * px;
    ry -= along * py;
    return RAD2DEG(std::atan2(py + gain * ry, px + gain * rx));
}

// A closed door across the way within DOOR_AHEAD of the eye, along the yaw: where the way meets it. The doors of the
// practice maps open to the use key only, aimed at them (Player::getUseableEntities: 64 u along the view); the bots
// pressed use only for a door straight ahead in their view, and at dm/flag's junction west of the spawn they slid along
// a closed door for seconds while their view was on the corridor beyond it or on a corner (people open it and walk
// through).
// g_humanbot_door_ahead: how far ahead (96 u until 2026-10-05; with the view on the door's nearest part, 128 u has use
// reach it 53 u from its middle against 49, people 70).
static bool DoorAhead(Player *p, float yaw, Vector& at)
{
    Vector fwd;
    Vector(0, yaw, 0).AngleVectors(&fwd);
    const float   ahead = g_humanbot_door_ahead ? g_humanbot_door_ahead->value : 128.0f;
    const Vector  start = p->origin + Vector(0, 0, p->viewheight);
    const trace_t tr    = G_Trace(start, vec_zero, vec_zero, start + fwd * ahead, p, MASK_USABLE, qfalse, "HumanBot door");
    // (a door already opening is not looked at: it swings out of the way)
    if (tr.ent && tr.ent->entity && tr.ent->entity != world && tr.ent->entity->IsSubclassOfDoor()
        && static_cast<Door *>(tr.ent->entity)->isCompletelyClosed()) {
        // its nearest part, not where the way meets it: coming at a door on the slant, the bots looked along it and were
        // 42 u off it when use reached it (people press 60 u off, the view 12 deg off its nearest part)
        const Entity *d = tr.ent->entity;
        at              = Vector(
            Q_clamp_float(start.x, d->absmin.x, d->absmax.x), Q_clamp_float(start.y, d->absmin.y, d->absmax.y), start.z
        );
        return true;
    }
    return false;
}

// Navigation mesh path toward the brain's goal: the direction of its next corner, kept off the walls.
void HumanBotAdapter::Steering(Player *p, const hb::TickPlan& plan, const hb::SelfState& self)
{
    if (!plan.navTargetValid || plan.owner == hb::OWNER_DEAD) {
        m_steerValid = false;
        return;
    }
    const Vector goal(plan.navTarget.x, plan.navTarget.y, plan.navTarget.z);
    // the path goes to the brain's point along the way people go when it gives one (hb::Navigator::Via)
    const Vector pathTo = plan.navViaValid ? Vector(plan.navVia.x, plan.navVia.y, plan.navVia.z) : goal;
    if (!m_pather) {
        m_pather = IPather::CreatePather();
    }
    if (!m_pather) {
        m_steerValid = false;
        return;
    }
    if ((pathTo - m_pathGoal).lengthSquared() > Square(64.0f) || level.inttime - m_pathTime > 1000 || !m_pather->GetNodeCount()) {
        if (!m_pather->IsQuerying()) {
            PathSearchParameter parameters;
            parameters.entity     = p;
            parameters.fallHeight = MAX_FALL_HEIGHT;
            parameters.leashDist  = 0.0f;
            m_pather->FindPath(p->origin, pathTo, parameters);
            m_pathGoal = pathTo;
            m_pathTime = level.inttime;
        }
    } else {
        m_pather->UpdatePos(p->origin);
    }
    if (m_pather->GetNodeCount() > 0) {
        Vector dir = m_pather->GetCurrentDirection();
        dir.z      = 0.0f;
        if (dir.normalize() > 0.0f) {
            m_nCorners   = m_pather->GetCorners(m_corners, hb::MAX_NAV_CORNERS);
            m_steerValid = true;
            m_steerYaw   = WallSteerYaw(dir.toYaw(), self.viewYaw, self.clearance);
            m_doorValid  = DoorAhead(p, dir.toYaw(), m_door);
            m_pathLen    = (goal - p->origin).length();
            return;
        }
    }
    m_steerValid = false;
    m_nCorners   = 0;
    m_doorValid  = false;
}

void HumanBotAdapter::SetOwner(Player *p, int owner)
{
    static const char *const OWNER_NAMES[] = {"brain", "ladder", "door", "recovery", "dead", "manual", "spectator"};
    if (owner != m_owner) {
        G_MoveLogBotEvent("bot_owner", p, NULL, owner, p->origin);
        if (g_humanbot_debug && g_humanbot_debug->integer && owner != hb::OWNER_DEAD && m_owner != hb::OWNER_DEAD) {
            // the stock code taking over (or handing back) is what to watch in a live test
            gi.Printf(
                "humanbot: %s %s -> %s at (%.0f %.0f %.0f)\n",
                p->client ? p->client->pers.netname : "?",
                m_owner >= 0 && m_owner <= hb::OWNER_SPECTATOR ? OWNER_NAMES[m_owner] : "?",
                owner >= 0 && owner <= hb::OWNER_SPECTATOR ? OWNER_NAMES[owner] : "?",
                p->origin.x,
                p->origin.y,
                p->origin.z
            );
        }
        m_owner = owner;
    }
}

static int SignKey(int v)
{
    return v > 20 ? 1 : (v < -20 ? -1 : 0);
}

// A climb, as people climb (obj/obj_team2, 81 climbs up and 7 down; nobody took a ladder in the dm/vents
// duels): the forward key held, the view up the ladder (pitch p50 -54 deg) or down it (+70), toward the
// end nearer the brain's goal. On forward the ladder's state machine climbs up unless the view is more
// than 30 deg down, and down when it is; it gets off at either end the same way.
// Nothing in the recordings says what to do when the climb stalls, so this is set by hand: a bot that
// has not moved 8 u for 1.5-3 s turns back once (two bots meeting on the ladder, or one standing on the
// climber's head, blocked each other for minutes), and jumps off the second time or after 30 s.
static const float LADDER_PROGRESS_Z   = 8.0f;
static const int   LADDER_PATIENCE_MS  = 1500;
static const int   LADDER_GIVE_UP_MS   = 30000;

void HumanBotAdapter::Ladder(Player *p, const HbView& view, const Vector& goal, hb::TickPlan& plan)
{
    const int now = level.inttime;
    if (!m_ladderSince) {
        m_ladderSince    = std::max(now, 1);
        m_ladderDir      = goal.z >= p->origin.z ? 1 : -1;
        m_ladderReversed = false;
        m_ladderJumped   = false;
        m_ladderMoveZ    = p->origin.z;
        m_ladderMoveTime = now;
        m_ladderPatience = LADDER_PATIENCE_MS + static_cast<int>(m_rngPresent.Uniform() * LADDER_PATIENCE_MS);
        m_ladderPitch    = 0.0f;
    }
    if (std::fabs(p->origin.z - m_ladderMoveZ) > LADDER_PROGRESS_Z) {
        m_ladderMoveZ    = p->origin.z;
        m_ladderMoveTime = now;
    }
    bool jump = false;
    if (now - m_ladderMoveTime > m_ladderPatience || now - m_ladderSince > LADDER_GIVE_UP_MS) {
        if (!m_ladderReversed && now - m_ladderSince <= LADDER_GIVE_UP_MS) {
            m_ladderDir      = -m_ladderDir;
            m_ladderReversed = true;
            m_ladderMoveTime = now;
            m_ladderPitch    = 0.0f;
        } else {
            // the state machine leaves on a jump press, not a held jump
            jump = !m_ladderJumped;
        }
    }
    m_ladderJumped = jump;
    if (m_ladderPitch == 0.0f) {
        m_ladderPitch = m_ladderDir > 0 ? Q_clamp_float(static_cast<float>(m_rngPresent.Normal(-54.0, 8.0)), -70.0f, -35.0f)
                                        : Q_clamp_float(static_cast<float>(m_rngPresent.Normal(66.0, 6.0)), 45.0f, 80.0f);
    }

    float   wantYaw = view.yaw;
    Entity *ladder  = p->GetLadder();
    if (ladder && ladder->isSubclassOf(FuncLadder)) {
        wantYaw = static_cast<FuncLadder *>(ladder)->getFacingAngles()[YAW];
    }
    plan.chord       = hb::MakeChord(1, 0);
    plan.jump        = jump;
    plan.crouch      = false;
    plan.lean        = 0;
    plan.use         = false;
    plan.attack      = false;
    plan.bash        = false;
    plan.yawDelta    = Q_clamp_float(AngleNormalize180(wantYaw - view.yaw), -30.0f, 30.0f);
    plan.pitchDelta  = Q_clamp_float(m_ladderPitch - view.pitch, -20.0f, 20.0f);
    plan.viewStill   = false;
    plan.flickShaped = false;
}

// The only times the brain's movement is overridden: ladders (Ladder()), and the stock code for doors and
// recovery.
void HumanBotAdapter::Owners(Player *p, const HbView& view, const hb::SelfState& self, hb::TickPlan& plan)
{
    if (plan.owner == hb::OWNER_DEAD || !self.alive) {
        SetOwner(p, hb::OWNER_DEAD);
        m_recoveryUntil = 0;
        return;
    }
    const Vector goal = plan.navTargetValid ? Vector(plan.navTarget.x, plan.navTarget.y, plan.navTarget.z) : p->origin;

    if (self.onLadder) {
        Ladder(p, view, goal, plan);
        SetOwner(p, hb::OWNER_LADDER);
        return;
    }
    m_ladderSince = 0;

    // recovery: after 1 s of wall pressure or 1.5 s without progress, stock
    // AvoidPath moves the bot for 750 ms (the brain keeps the view)
    if (m_recoveryUntil == 0 && (m_stuckMs >= 1500.0f || m_wallMs >= 1000.0f)) {
        m_controller->GetMovement().AvoidPath(p->origin, 192.0f);
        m_recoveryUntil = level.inttime + 750;
    }
    if (m_recoveryUntil) {
        if (level.inttime < m_recoveryUntil) {
            usercmd_t cmd;
            memset(&cmd, 0, sizeof(cmd));
            m_controller->GetMovement().MoveThink(cmd);
            plan.chord  = hb::MakeChord(SignKey(cmd.forwardmove), SignKey(cmd.rightmove));
            plan.jump   = cmd.upmove > 0 && self.onGround;
            plan.crouch = false;
            SetOwner(p, hb::OWNER_RECOVERY);
            return;
        }
        m_recoveryUntil = 0;
        m_stuckMs       = 0.0f;
        m_wallMs        = 0.0f;
        m_controller->GetMovement().ClearMove();
    }

    // doors: the stock use logic when a closed door or a ladder is right ahead (in the view, the bot going forward or
    // pressing a wall), or a closed door lies across its way: with the view on the door but a strafe held the bots slid
    // to and fro beside dm/flag's doors with the enemy behind them and never opened them
    if (level.inttime < m_useUntil) {
        plan.use = true;
    } else if (hb::ChordFwd(plan.chord) > 0 || m_wallMs > 0.0f || (m_steerValid && m_doorValid)) {
        Vector fwd;
        Vector(0, view.yaw, 0).AngleVectors(&fwd);
        const Vector  start = p->origin + Vector(0, 0, p->viewheight);
        const trace_t tr    = G_Trace(start, vec_zero, vec_zero, start + fwd * 64.0f, p, MASK_USABLE | MASK_LADDER, qfalse, "HumanBot use");
        if (tr.ent && tr.ent->entity && tr.ent->entity != world) {
            Entity *e = tr.ent->entity;
            // (use does nothing to a door on the move, and shuts an open one)
            if ((e->IsSubclassOfDoor() && static_cast<Door *>(e)->isCompletelyClosed()) || e->isSubclassOf(FuncLadder)) {
                plan.use   = true;
                m_useUntil = level.inttime + 100;
                if (e->IsSubclassOfDoor()) {
                    StartDoorPass(p, static_cast<Door *>(e));
                }
                DoorPass(p, view, self, plan);
                SetOwner(p, hb::OWNER_DOOR);
                return;
            }
        }
    }
    if (DoorPass(p, view, self, plan)) {
        SetOwner(p, hb::OWNER_DOOR);
        return;
    }
    SetOwner(p, hb::OWNER_BRAIN);
}

// Through a door the bot opened (2026-10-05). The practice maps' doors swing away from whoever opens them and take 1-1.5 s
// to open (wood, metal). On dm/flag people pressed use as soon as the door was in reach (60 u off it) and held in front of
// the doorway while it swung, 30-55 u off it, lined up with its middle; they were through 1.25 s after the press, 70%
// within 3 s. The bots ran on into the swinging door, the route slid them along it to the frame and back (the path flips
// while the door moves), and they were through after 1.65 s, 57% within 3 s: pinned at the Flag room's doors, the one
// holding the room saw a shoulder through the gap before the bot could see them. So the keys are the door's for that
// moment: in front of the doorway's middle, no nearer than PASS_NEAR and no further than g_humanbot_door_hold, while
// the door has turned less than g_humanbot_door_go degrees, then through it, until PASS_BEYOND past it (go 0 = off).
// The brain keeps the view, and gets the keys back the moment its enemy is in sight. Going at 25 deg the bots were through
// after 1.35 s, 80% within 3 s; at 10-15 deg they ran into the door again, at 35-45 deg they stood waiting.
static const float PASS_BEYOND  = 24.0f;    // past the doorway: done
static const float PASS_FAR     = 160.0f;   // this far back or aside: given up
static const float PASS_NEAR    = 28.0f;    // waiting no nearer than this to the closed door (the box is 16 u wide)
static const float PASS_LATCH   = 0.25f;    // going: this share of the half-leaf toward the side away from the hinge
static const float PASS_LEAN_DEG = 20.0f;   // the view this far off the way through: lean toward the way
static const int   PASS_MS      = 2500;

void HumanBotAdapter::StartDoorPass(Player *p, Door *d)
{
    m_passDoor = -1;
    if (!d->isSubclassOf(RotatingDoor)) {
        return;
    }
    // the leaf from the hinge (the door's origin) to its middle, closed
    Vector c = (d->absmin + d->absmax) * 0.5f;
    Vector u = c - d->origin;
    u.z      = 0.0f;
    if (u.normalize() < 8.0f) {
        return;
    }
    Vector n(-u.y, u.x, 0.0f);
    Vector toC = c - p->origin;
    toC.z      = 0.0f;
    if (DotProduct(n, toC) < 0.0f) {
        n = n * -1.0f;
    }
    c.z          = p->origin.z;
    m_passDoor   = d->entnum;
    m_passStart  = level.inttime;
    m_passYaw    = d->angles.yaw();
    m_passCenter = c;
    m_passNormal = n;
    m_passHinge  = Vector(d->origin.x, d->origin.y, c.z);
    m_passLean   = 0;
}

// The chord nearest a direction off the view, among those not pressing into a wall right there (any if all are).
static int ChordToward(float relYaw, const float clearance[hb::NUM_CHORDS])
{
    int   best = hb::CHORD_NEUTRAL;
    float bestErr = 1e9f;
    for (int c = 0; c < hb::NUM_CHORDS; c++) {
        if (c == hb::CHORD_NEUTRAL) {
            continue;
        }
        const float err = std::fabs(AngleSubtract(relYaw, hb::Mover::ChordAngle(c))) + (clearance[c] < 2.0f ? 360.0f : 0.0f);
        if (err < bestErr) {
            bestErr = err;
            best    = c;
        }
    }
    return best;
}

bool HumanBotAdapter::DoorPass(Player *p, const HbView& view, const hb::SelfState& self, hb::TickPlan& plan)
{
    if (m_passDoor < 0) {
        return false;
    }
    const float goDeg = g_humanbot_door_go ? g_humanbot_door_go->value : 0.0f;
    Entity     *e     = G_GetEntity(m_passDoor);
    if (!e || !e->IsSubclassOfDoor() || goDeg <= 0.0f || m_engaged || level.inttime - m_passStart > PASS_MS) {
        m_passDoor = -1;
        return false;
    }
    Door  *d   = static_cast<Door *>(e);
    Vector rel = p->origin - m_passCenter;
    rel.z      = 0.0f;
    const float depth  = DotProduct(rel, m_passNormal);
    const float across = std::fabs(rel.x * m_passNormal.y - rel.y * m_passNormal.x);
    // through, too far off, or the door shut again (it is used afresh)
    if (depth > PASS_BEYOND || depth < -PASS_FAR || across > PASS_FAR
        || (d->isCompletelyClosed() && level.inttime - m_passStart > 200)) {
        m_passDoor = -1;
        return false;
    }
    const bool  wait = !d->isOpen() && std::fabs(AngleSubtract(d->angles.yaw(), m_passYaw)) < goDeg;
    // waiting: lined up with the doorway's middle where the bot stands, no nearer than PASS_NEAR and no further than
    // the hold distance; going: onto a line through it on the side that opens first (away from the hinge), 48 u ahead
    const float hold   = g_humanbot_door_hold ? g_humanbot_door_hold->value : 48.0f;
    const float along  = wait ? -Q_clamp_float(-depth, PASS_NEAR, std::max(PASS_NEAR, hold)) : std::min(depth + 48.0f, 64.0f);
    const Vector latch = m_passCenter - m_passHinge;
    Vector      to     = m_passCenter + latch * (wait ? 0.0f : PASS_LATCH) + m_passNormal * along - p->origin;
    to.z               = 0.0f;
    plan.chord         = wait && to.length() < 12.0f ? hb::CHORD_NEUTRAL
                                                     : ChordToward(AngleSubtract(to.toYaw(), view.yaw), self.clearance);
    plan.jump          = false;
    plan.crouch        = false;
    // Looking off the way through (at where the enemy is believed), lean toward the side the way goes, so the eye comes
    // out of the doorway ahead of the body. The lean chain follows the keys the brain chose, not these: at the moment
    // the one holding the room and the bot first had each other on screen, its lean led its motion 18-23% of the time
    // and went against it 21% (the owner's games); people coming in leaned with their motion 56% (against 17%), and the
    // holder saw them first 29% of the time, the bot 76-83%. The lean chain goes on from this lean when the pass ends.
    const float through = AngleSubtract(m_passNormal.toYaw(), view.yaw);
    if (std::fabs(through) > PASS_LEAN_DEG && std::fabs(through) < 180.0f - PASS_LEAN_DEG) {
        m_passLean = through > 0.0f ? -1 : 1;
    }
    if (m_passLean) {
        plan.lean = m_passLean;
        m_brain.SetLean(m_passLean);
    }
    return true;
}

void HumanBotAdapter::Prepare()
{
    Player *p = m_player;
    m_cmds.clear();
    m_eyes.clear();
    m_command = hb::CMD_NONE;
    if (!p || !p->client) {
        return;
    }
    const auto t0 = std::chrono::steady_clock::now();
    m_tick++;

    const hb::MapPrior *prior = HB_WorldPrior();
    if (prior != m_prior) {
        m_prior = prior;
        m_brain.SetMap(prior);
    }
    float hfov, vfov;
    HB_Fov(hfov, vfov);
    m_brain.SetFov(hfov, vfov);

    //
    // Perception: everything a human at this screen and these speakers gets
    //
    const HbView view = HB_ViewOf(p);
    hb::RawInput raw;
    HB_FillSelf(p, view, raw.self);
    HB_FillClearance(p, view.yaw, raw.self.clearance, m_drop, m_tick & 1);
    for (int c = 0; c < hb::NUM_CHORDS; c++) {
        raw.self.drop[c] = m_drop[c];
    }
    Track(p, raw.self);
    raw.self.stuckMs       = m_stuckMs;
    raw.self.wallPressMs   = m_wallMs;
    raw.self.navSteerValid = m_steerValid;
    raw.self.navSteerYaw   = m_steerYaw;
    raw.self.navPathLen    = m_pathLen;
    raw.self.navCorners    = m_steerValid ? m_nCorners : 0;
    raw.self.doorAheadValid = m_steerValid && m_doorValid;
    raw.self.doorAhead      = hb::Vec3(m_door.x, m_door.y, m_door.z);
    for (int i = 0; i < raw.self.navCorners; i++) {
        raw.self.navCorner[i] = hb::Vec3(m_corners[i].x, m_corners[i].y, m_corners[i].z);
    }

    if (raw.self.alive && !raw.self.spectator) {
        for (int i = 0; i < game.maxclients; i++) {
            gentity_t *ent = &g_entities[i];
            if (!ent->inuse || !ent->client || !ent->entity || !ent->entity->IsSubclassOfPlayer()) {
                continue;
            }
            Player *o = static_cast<Player *>(ent->entity);
            if (!Opponent(p, o)) {
                continue;
            }
            hb::RawEnemy e;
            e.id    = o->entnum;
            e.alive = !o->IsDead() && !o->IsSpectator() && o->health > 0.0f;
            if (e.alive) {
                // the focus enemy and half of the others get the full part check each tick
                const bool     full = o->entnum == m_focusId || ((m_tick + o->entnum) & 1) == 0;
                const HbSight  s    = HB_SightOf(p, view, o, full);
                // nothing about an enemy with no visible part crosses into the brain
                e.inFov             = s.partMask != 0 && s.inFov;
                e.partMask          = s.partMask;
                // the soft wallhack: the brain's perception turns this into an occasional noisy hunch
                e.hunchValid        = true;
                e.hunchPos          = hb::Vec3(o->origin.x, o->origin.y, o->origin.z);
                e.centroidLos       = s.partMask != 0 && s.centroidLos;
                if (s.partMask) {
                    Vector parts[hb::NUM_PARTS];
                    HB_PartPositions(o, parts);
                    for (int k = 0; k < hb::NUM_PARTS; k++) {
                        if (s.partMask & (1 << k)) {
                            e.partPos[k] = hb::Vec3(parts[k].x, parts[k].y, parts[k].z);
                        }
                    }
                    e.centroid   = hb::Vec3(o->centroid.x, o->centroid.y, o->centroid.z);
                    e.velocity   = hb::Vec3(o->velocity.x, o->velocity.y, o->velocity.z);
                    e.bodyHeight = o->maxs.z - o->mins.z;
                    Weapon *w    = o->GetActiveWeapon(WEAPON_MAIN);
                    e.reloading  = w && w->GetState() == WEAPON_RELOADING;
                    e.firing     = w && w->GetState() == WEAPON_FIRING;
                }
            }
            raw.enemies.push_back(e);
        }
        const int myArea = p->edict->r.areanum;
        for (const HbSoundEvent& s : s_soundsTick) {
            if (s.sourceId == p->entnum || (s.origin - p->origin).lengthSquared() > Square(SOUND_MAX_RANGE)) {
                continue;
            }
            if (s.areanum >= 0 && myArea >= 0 && s.areanum != myArea && !gi.AreasConnected(s.areanum, myArea)) {
                continue;
            }
            hb::RawSound rs;
            rs.type     = s.type;
            rs.sourceId = s.sourceId;
            rs.origin   = hb::Vec3(s.origin.x, s.origin.y, s.origin.z);
            raw.sounds.push_back(rs);
        }
        for (const HbDamageEvent& d : s_damageTick) {
            if (d.victimId != p->entnum) {
                continue;
            }
            hb::RawDamage rd;
            rd.attackerId  = d.attackerId;
            rd.attackerPos = hb::Vec3(d.attackerPos.x, d.attackerPos.y, d.attackerPos.z);
            rd.damage      = d.damage;
            raw.damage.push_back(rd);
        }
        raw.teammateInCrosshair = TeammateInCrosshair(p, view);
    }
    raw.deaths    = s_deathsTick;
    raw.gotKillOf = m_gotKillOf;
    m_gotKillOf   = -1;
    raw.headHit   = m_headHit;
    m_headHit     = false;

    hb::Observation obs;
    m_brain.SetSkillBoost(g_humanbot_skill ? g_humanbot_skill->value : 0.0f);
    m_perceiver.Process(raw, hfov, vfov, m_brain.DetectMult(), obs);

    //
    // Decisions
    //
    hb::TickPlan plan;
    hb::Diag     diag;
    m_brain.Think(obs, plan, &diag);
    m_focusId = diag.focus_id;

    // dm/main west stairs: a jump off the rail there leaves the level flow
    if (plan.jump && !Q_stricmp(level.mapname.c_str(), "dm/main")) {
        const Vector& o = p->origin;
        const float   yaw = DEG2RAD(view.yaw + hb::Mover::ChordAngle(plan.chord));
        if (o.x >= 104.0f && o.x <= 124.0f && o.y >= 1200.0f && o.y <= 1312.0f && o.z >= -120.0f && o.z <= -104.0f
            && plan.chord != hb::CHORD_NEUTRAL && std::cos(yaw) > 0.35f) {
            plan.jump = false;
        }
    }
    m_engaged = diag.detected != 0;
    Owners(p, view, raw.self, plan);
    m_command = plan.command;

    //
    // Sub-step usercmds and the eye each of them carries
    //
    m_sub.Build(plan, view.yaw, view.pitch, m_rngSub, m_cmds);
    if (!plan.send) {
        // the dead ticks after a respawn: the server takes no usercmd from a client that has not seen it yet
        m_cmds.clear();
    }
    const bool kbdOk = m_cmds.empty() || hb::Substepper::CheckContract(m_cmds);
    if (!kbdOk) {
        m_kbdViolations++;
    }
    const float frameMs = static_cast<float>(hb::TICK_MS) / static_cast<float>(std::max<size_t>(1, m_cmds.size()));
    for (size_t k = 0; k < m_cmds.size(); k++) {
        m_eyes.push_back(HB_EyeStep(p, m_eye, m_cmds[k].yaw, m_cmds[k].pitch, frameMs));
    }
    if (!m_eyes.empty()) {
        // the client's clip traces, once per tick, applied to every sub-step
        const Vector clipped = HB_EyeClip(p, m_eyes.back());
        const Vector fix     = clipped - m_eyes.back();
        for (Vector& e : m_eyes) {
            e += fix;
        }
    }

    Steering(p, plan, raw.self);
    m_lastChord = m_cmds.empty() ? hb::CHORD_NEUTRAL : m_cmds.back().chord;
    m_lastClear = m_lastChord == hb::CHORD_NEUTRAL ? 128.0f : raw.self.clearance[m_lastChord];

    const int us = static_cast<int>(
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count()
    );
    m_thinkSum += us;
    m_thinkCount++;
    m_thinkMax = std::max(m_thinkMax, us);

    diag.owner            = m_owner;
    diag.substeps         = static_cast<int>(m_cmds.size());
    diag.kbd_ok           = kbdOk ? 1 : 0;
    diag.think_us         = us;
    diag.stuck_ms         = static_cast<int>(m_stuckMs);
    diag.wall_pressure_ms = static_cast<int>(m_wallMs);
    m_diag                = diag;
    m_diagValid           = true;
}

void HumanBotAdapter::Commit()
{
    Player *p = m_player;
    if (!p || !p->client) {
        return;
    }
    Join(p);

    switch (m_command) {
    case hb::CMD_RELOAD:
        m_controller->SendCommand("reload");
        break;
    case hb::CMD_PISTOL:
    case hb::CMD_PRIMARY:
        {
            const Container<int>& inventory = p->getInventory();
            Weapon *const         active    = p->GetActiveWeapon(WEAPON_MAIN);
            // with nothing loaded in hand, the pistol is drawn even when it is empty: it still bashes
            const bool forBash = m_command == hb::CMD_PISTOL && (!active || !active->HasAmmo(FIRE_PRIMARY));
            for (int i = 1; i <= inventory.NumObjects(); i++) {
                Entity *item = G_GetEntity(inventory.ObjectAt(i));
                if (!item || !item->IsSubclassOfWeapon()) {
                    continue;
                }
                Weapon   *w    = static_cast<Weapon *>(item);
                const int wc   = w->GetWeaponClass();
                const bool want = m_command == hb::CMD_PISTOL ? (wc & ::WEAPON_CLASS_PISTOL) != 0
                                                              : (wc & (::WEAPON_CLASS_SMG | ::WEAPON_CLASS_RIFLE | ::WEAPON_CLASS_MG | ::WEAPON_CLASS_HEAVY)) != 0;
                if (want && w != active && (w->HasAmmo(FIRE_PRIMARY) || forBash)) {
                    p->useWeapon(w, WEAPON_MAIN);
                    break;
                }
            }
            break;
        }
    default:
        break;
    }

    // the usercmds of this frame, oldest first; the last one is stamped with the frame time
    const auto t0       = std::chrono::steady_clock::now();
    int        prevTime = p->client->ps.commandTime;
    const bool wasDead  = p->IsDead() || p->IsSpectator();
    for (size_t k = 0; k < m_cmds.size(); k++) {
        const hb::SubCmd& c = m_cmds[k];
        usercmd_t         u;
        usereyes_t        eyes;
        memset(&u, 0, sizeof(u));
        memset(&eyes, 0, sizeof(eyes));
        u.serverTime  = level.svsTime + c.serverTimeOffset;
        u.msec        = static_cast<byte>(hb::ClampI(u.serverTime - prevTime, 0, 255));
        prevTime      = u.serverTime;
        u.forwardmove = static_cast<signed char>(hb::ChordFwd(c.chord) * 127);
        u.rightmove   = static_cast<signed char>(hb::ChordSide(c.chord) * 127);
        u.upmove      = static_cast<signed char>(c.jump ? 127 : (c.crouch ? -127 : 0));
        unsigned int buttons = 0;
        if (c.attack) {
            buttons |= BUTTON_ATTACKLEFT;
        }
        if (c.bash) {
            buttons |= BUTTON_ATTACKRIGHT;
        }
        if (!c.walk) {
            buttons |= BUTTON_RUN;
        }
        if (c.use) {
            buttons |= BUTTON_USE;
        }
        if (c.lean < 0) {
            buttons |= BUTTON_LEAN_LEFT;
        } else if (c.lean > 0) {
            buttons |= BUTTON_LEAN_RIGHT;
        }
        // CL_CmdButtons: any key down (the walk key included) sets both bits
        if (u.forwardmove || u.rightmove || u.upmove || c.attack || c.bash || c.use || c.lean || c.walk) {
            buttons |= BUTTON_ANY | BUTTON_MOUSE;
        }
        u.buttons   = static_cast<unsigned short>(buttons);
        u.angles[0] = static_cast<short>(ANGLE2SHORT(c.pitch) - p->client->ps.delta_angles[0]);
        u.angles[1] = static_cast<short>(ANGLE2SHORT(c.yaw) - p->client->ps.delta_angles[1]);
        u.angles[2] = static_cast<short>(-p->client->ps.delta_angles[2]);
        if (k < m_eyes.size()) {
            HB_EyeOffset(p, m_eyes[k], eyes.ofs);
        }
        eyes.angles[0] = c.pitch;
        eyes.angles[1] = c.yaw;
        G_ClientThink(p->edict, &u, &eyes);
        if (!m_player) {
            return;  // removed by a script during the move
        }
        if ((p->IsDead() || p->IsSpectator()) != wasDead) {
            // (re)spawned or killed mid-frame: the rest of this tick was planned for the old view
            break;
        }
    }
    if (!m_cmds.empty()) {
        m_commitSum += static_cast<double>(
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count()
        );
        m_commitCount++;
    }

    // a realistic ping only while disguised; bots otherwise show as bots. The brain times the respawn by it either way
    const int ping = m_ping.Step();
    m_brain.SetPingMs(ping);
    if (HB_DisguiseActive()) {
        m_pingMs = ping;
        p->client->ps.ping = m_pingMs;
    } else {
        m_pingMs = 0;
    }
}

void HumanBotAdapter::Spawned()
{
    m_stuckMs       = 0.0f;
    m_wallMs        = 0.0f;
    m_recoveryUntil = 0;
    m_useUntil      = 0;
    m_ladderSince   = 0;
    m_eye           = hb::EyeState();
    m_steerValid    = false;
    if (m_player) {
        m_lastOrigin = m_player->origin;
        m_sub.Reset(m_player->GetViewAngles()[YAW], m_player->GetViewAngles()[PITCH]);
        G_MoveLogBotStyle(m_player, hb::DialsJson(m_dials).c_str());
    }
    m_controller->GetMovement().ClearMove();
}

void HumanBotAdapter::Killed()
{
    m_recoveryUntil = 0;
    m_steerValid    = false;
}

void HumanBotAdapter::GotKill(Entity *victim)
{
    m_gotKillOf = victim ? victim->entnum : -1;
}

//
// Public entry points
//
static bool s_inited = false;

void G_HumanBotInit(void)
{
    g_humanbot_substeps  = gi.Cvar_Get("g_humanbot_substeps", "12", 0);
    g_humanbot_fov       = gi.Cvar_Get("g_humanbot_fov", "80", 0);
    g_humanbot_aspect    = gi.Cvar_Get("g_humanbot_aspect", "1.778", 0);
    g_humanbot_seed      = gi.Cvar_Get("g_humanbot_seed", "0", 0);
    g_humanbot_disguise  = gi.Cvar_Get("g_humanbot_disguise", "0", 0);
    g_humanbot_model_dir = gi.Cvar_Get("g_humanbot_model_dir", "", 0);
    g_humanbot_families  = gi.Cvar_Get("g_humanbot_families", "", 0);
    g_humanbot_debug     = gi.Cvar_Get("g_humanbot_debug", "0", 0);
    g_humanbot_wall_steer = gi.Cvar_Get("g_humanbot_wall_steer", "0.5", 0);
    g_humanbot_skill     = gi.Cvar_Get("g_humanbot_skill", "0", 0);
    g_humanbot_door_ahead = gi.Cvar_Get("g_humanbot_door_ahead", "128", 0);
    g_humanbot_door_hold  = gi.Cvar_Get("g_humanbot_door_hold", "48", 0);
    g_humanbot_door_go    = gi.Cvar_Get("g_humanbot_door_go", "25", 0);

    if (!s_inited) {
        std::string error;
        if (!HB_LoadModel(error)) {
            gi.Printf("humanbot: model overrides rejected (%s); using the embedded model\n", error.c_str());
        }
        if (HB_Bundle()) {
            gi.Printf("humanbot: model %.12s (%s)\n", s_bundle.sha256.c_str(), s_bundle.source.c_str());
        } else {
            gi.Printf("humanbot: no usable model; the stock bots are used\n");
        }
        s_inited = true;
    }
    HB_WorldReset();
    s_soundsPending.clear();
    s_soundsTick.clear();
    s_damagePending.clear();
    s_damageTick.clear();
    s_deathsPending.clear();
    s_deathsTick.clear();
}

void G_HumanBotShutdown(void)
{
    HB_WorldReset();
    s_soundsPending.clear();
    s_soundsTick.clear();
    s_damagePending.clear();
    s_damageTick.clear();
}

void G_HumanBotFrame(void)
{
    HB_WorldFrame();
    HB_SelfTestFrame();
}

HumanBotAdapter *G_HumanBotCreate(BotController *controller, Player *player)
{
    if (!HB_Bundle() || !controller || !player || !player->client) {
        return nullptr;
    }
    return new HumanBotAdapter(controller, player);
}

void G_HumanBotDestroy(HumanBotAdapter *adapter)
{
    delete adapter;
}

void G_HumanBotBeginFrame(void)
{
    s_soundsTick.swap(s_soundsPending);
    s_soundsPending.clear();
    s_damageTick.swap(s_damagePending);
    s_damagePending.clear();
    s_deathsTick.swap(s_deathsPending);
    s_deathsPending.clear();
}

void G_HumanBotPrepare(HumanBotAdapter *adapter)
{
    if (adapter) {
        adapter->Prepare();
    }
}

void G_HumanBotCommit(HumanBotAdapter *adapter)
{
    if (adapter) {
        adapter->Commit();
    }
}

void G_HumanBotSpawned(HumanBotAdapter *adapter)
{
    if (adapter) {
        adapter->Spawned();
    }
}

void G_HumanBotKilled(HumanBotAdapter *adapter)
{
    if (adapter) {
        adapter->Killed();
    }
}

void G_HumanBotGotKill(HumanBotAdapter *adapter, Entity *victim)
{
    if (adapter) {
        adapter->GotKill(victim);
    }
}

void G_HumanBotDeath(Player *victim)
{
    if (victim) {
        s_deathsPending.push_back(victim->entnum);
    }
}

bool G_HumanBotGetDiag(Player *player, hb::Diag *out)
{
    const HumanBotAdapter *a = HB_AdapterFor(player);
    if (!a || !a->DiagValid()) {
        return false;
    }
    if (out) {
        *out = a->GetDiag();
    }
    return true;
}

const char *G_HumanBotModelSha256(void)
{
    return HB_Bundle() ? s_bundle.sha256.c_str() : "";
}

const char *G_HumanBotMetaLines(void)
{
    static std::string lines;
    lines.clear();
    for (cvar_t *cv : {g_humanbot_substeps, g_humanbot_fov, g_humanbot_aspect, g_humanbot_seed, g_humanbot_disguise,
                       g_humanbot_model_dir, g_humanbot_families}) {
        if (cv) {
            lines += std::string(cv->name) + "=" + cv->string + "\n";
        }
    }
    return lines.c_str();
}

void G_HumanBotObserveParts(Player *viewer, Player *target, int *visibleParts, int *inFov)
{
    int parts = 0;
    int fov   = 0;
    if (viewer && target && viewer != target && viewer->client && !target->IsDead()) {
        const HbView  view = HB_ViewOf(viewer);
        const HbSight s    = HB_SightOf(viewer, view, target, true);
        for (int i = 0; i < hb::NUM_PARTS; i++) {
            parts += (s.partMask >> i) & 1;
        }
        fov = s.inFov ? 1 : 0;
    }
    if (visibleParts) {
        *visibleParts = parts;
    }
    if (inFov) {
        *inFov = fov;
    }
}

static int SoundTypeFromAIEvent(int iType)
{
    switch (iType) {
    case AI_EVENT_FOOTSTEP:
        return hb::SOUND_FOOTSTEP;
    case AI_EVENT_WEAPON_IMPACT:
        return hb::SOUND_IMPACT;
    case AI_EVENT_MISC:
    case AI_EVENT_MISC_LOUD:
        return hb::SOUND_DOOR;
    default:
        return hb::SOUND_OTHER;
    }
}

void G_HumanBotEmitSound(Entity *source, const Vector& origin, int soundType, float radius)
{
    if (!s_inited || !HB_Bundle()) {
        return;
    }
    HbSoundEvent s;
    s.type     = soundType;
    s.sourceId = source ? source->entnum : -1;
    s.origin   = origin;
    s.radius   = radius;
    vec3_t o   = {origin.x, origin.y, origin.z};
    s.areanum  = gi.AreaForPoint(o);
    s_soundsPending.push_back(s);
}

void G_HumanBotAIEvent(Entity *source, const Vector& origin, int aiEventType, float radius)
{
    // gunfire comes from every Weapon::Shoot instead (the AI fire event is throttled)
    if (aiEventType == AI_EVENT_WEAPON_FIRE || aiEventType == AI_EVENT_NONE || aiEventType >= AI_EVENT_MAX) {
        return;
    }
    if (aiEventType >= AI_EVENT_AMERICAN_VOICE && aiEventType <= AI_EVENT_GERMAN_URGENT) {
        return;
    }
    G_HumanBotEmitSound(source, origin, SoundTypeFromAIEvent(aiEventType), radius);
}

void G_HumanBotDamage(
    Sentient *victim, Entity *attacker, float damage, const Vector& position, const Vector& direction, int meansOfDeath,
    int location
)
{
    // our bullet in an enemy's head: the bot sees the hit like a player does (the owner's rule acts on it)
    if (s_inited && attacker && attacker != victim && victim && victim->IsSubclassOfPlayer()
        && (location == HITLOC_HEAD || location == HITLOC_HELMET)) {
        for (int i = 1; i <= s_adapters.NumObjects(); i++) {
            HumanBotAdapter *a = s_adapters.ObjectAt(i);
            if (a->GetPlayer() == attacker) {
                a->HeadHit();
            }
        }
    }
    // falls and the world are no threat to turn toward
    if (!s_inited || !victim || !victim->IsSubclassOfPlayer() || !attacker || attacker == victim
        || !attacker->IsSubclassOfSentient()) {
        return;
    }
    HbDamageEvent d;
    d.victimId    = victim->entnum;
    d.attackerId  = attacker->entnum;
    d.attackerPos = attacker->origin;
    d.damage      = damage;
    s_damagePending.push_back(d);
}

void G_HumanBotReinitAll()
{
    for (int i = 1; i <= s_adapters.NumObjects(); i++) {
        s_adapters.ObjectAt(i)->Reinit();
    }
}

void G_HumanBotResetCounters()
{
    for (int i = 1; i <= s_adapters.NumObjects(); i++) {
        s_adapters.ObjectAt(i)->ResetCounters();
    }
}

// Self-test verdict: no stuck bout over 2 s, no keyboard violation and a mean think time
// within budget for every bot, and fewer than one 500 ms wall-pressure bout per bot-minute
// over all bots (as hb_arena counts it: one bout of one bot in a one-minute test is no verdict).
bool G_HumanBotReportCounters(float minutes)
{
    bool pass     = s_adapters.NumObjects() >= 1;
    int  pressure = 0;
    for (int i = 1; i <= s_adapters.NumObjects(); i++) {
        HumanBotAdapter *a = s_adapters.ObjectAt(i);
        Player          *p = a->GetPlayer();
        const float      pressurePerMin = minutes > 0.0f ? a->PressureBouts() / minutes : 0.0f;
        const bool       ok = a->StuckBouts() == 0 && a->KbdViolations() == 0 && a->MeanThinkUs() <= 150.0f;
        pressure += a->PressureBouts();
        gi.Printf(
            "  %-20s %s  stuck>2s %d  pressure>500ms %.2f/min  kbd %d  think %.0f us (max %d)  usercmds %.0f us  %s\n",
            p && p->client ? p->client->pers.netname : "?",
            hb::StyleKey(a->Dials()).c_str(),
            a->StuckBouts(),
            pressurePerMin,
            a->KbdViolations(),
            a->MeanThinkUs(),
            a->MaxThinkUs(),
            a->MeanCommitUs(),
            ok ? "ok" : "FAIL"
        );
        pass = pass && ok;
    }
    if (s_adapters.NumObjects() < 1) {
        gi.Printf("  no human bots were running\n");
        return false;
    }
    const float botMinutes = minutes * s_adapters.NumObjects();
    const float perBotMin  = botMinutes > 0.0f ? pressure / botMinutes : 0.0f;
    const bool  pressureOk = perBotMin < 1.0f;
    gi.Printf("  all bots             pressure>500ms %.2f/bot-min  %s\n", perBotMin, pressureOk ? "ok" : "FAIL");
    return pass && pressureOk;
}

void HB_ListBots()
{
    gi.Printf("humanbot: %d bot(s), model %.12s\n", s_adapters.NumObjects(), s_bundle.sha256.c_str());
    for (int i = 1; i <= s_adapters.NumObjects(); i++) {
        HumanBotAdapter *a = s_adapters.ObjectAt(i);
        Player          *p = a->GetPlayer();
        gi.Printf(
            "  %2d %-20s %s  think %.0f us (max %d)  owner %d\n    %s\n",
            p ? p->entnum : -1,
            p && p->client ? p->client->pers.netname : "?",
            hb::StyleKey(a->Dials()).c_str(),
            a->MeanThinkUs(),
            a->MaxThinkUs(),
            a->Owner(),
            hb::DialsJson(a->Dials()).c_str()
        );
    }
}
