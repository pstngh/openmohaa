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
// by the perception glue; stock bot code runs only for ladders, doors and
// unstuck recovery.

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

    // special owners
    int m_owner          = hb::OWNER_BRAIN;
    int m_recoveryUntil  = 0;
    int m_useUntil       = 0;
    int m_ladderGoalTime = -100000;

    int m_gotKillOf = -1;
    bool m_headHit  = false;
    int m_focusId   = -1;
    int m_pingMs    = 0;

    // self-test counters
    double m_thinkSum      = 0.0;
    int    m_thinkCount    = 0;
    int    m_thinkMax      = 0;
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
    m_substeps = g_humanbot_substeps ? hb::ClampI(g_humanbot_substeps->integer, 1, hb::MAX_SUBSTEPS) : 4;
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
// g_humanbot_wall_steer scales the push (0 = off).
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

// Navigation mesh path toward the brain's goal: the direction of its next corner, kept off the walls.
void HumanBotAdapter::Steering(Player *p, const hb::TickPlan& plan, const hb::SelfState& self)
{
    if (!plan.navTargetValid || plan.owner == hb::OWNER_DEAD) {
        m_steerValid = false;
        return;
    }
    const Vector goal(plan.navTarget.x, plan.navTarget.y, plan.navTarget.z);
    if (!m_pather) {
        m_pather = IPather::CreatePather();
    }
    if (!m_pather) {
        m_steerValid = false;
        return;
    }
    if ((goal - m_pathGoal).lengthSquared() > Square(64.0f) || level.inttime - m_pathTime > 1000 || !m_pather->GetNodeCount()) {
        if (!m_pather->IsQuerying()) {
            PathSearchParameter parameters;
            parameters.entity     = p;
            parameters.fallHeight = MAX_FALL_HEIGHT;
            parameters.leashDist  = 0.0f;
            m_pather->FindPath(p->origin, goal, parameters);
            m_pathGoal = goal;
            m_pathTime = level.inttime;
        }
    } else {
        m_pather->UpdatePos(p->origin);
    }
    if (m_pather->GetNodeCount() > 0) {
        Vector dir = m_pather->GetCurrentDirection();
        dir.z      = 0.0f;
        if (dir.normalize() > 0.0f) {
            m_steerValid = true;
            m_steerYaw   = WallSteerYaw(dir.toYaw(), self.viewYaw, self.clearance);
            m_pathLen    = (goal - p->origin).length();
            return;
        }
    }
    m_steerValid = false;
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

// The only times stock code writes movement: ladders, doors and recovery.
void HumanBotAdapter::Owners(Player *p, const HbView& view, const hb::SelfState& self, hb::TickPlan& plan)
{
    if (plan.owner == hb::OWNER_DEAD || !self.alive) {
        SetOwner(p, hb::OWNER_DEAD);
        m_recoveryUntil = 0;
        return;
    }
    const Vector goal = plan.navTargetValid ? Vector(plan.navTarget.x, plan.navTarget.y, plan.navTarget.z) : p->origin;

    // ladder: the stock movement and rotation, snapped to digital keys
    if (self.onLadder) {
        BotMovement& movement = m_controller->GetMovement();
        if (level.inttime - m_ladderGoalTime > 1000 || !movement.IsMoving()) {
            movement.MoveTo(goal);
            m_ladderGoalTime = level.inttime;
        }
        usercmd_t cmd;
        memset(&cmd, 0, sizeof(cmd));
        movement.MoveThink(cmd);
        Vector want = movement.GetCurrentPathDirection().toAngles();
        want.x      = Q_clamp_float(AngleNormalize180(want.x), -80.0f, 80.0f);
        plan.chord      = hb::MakeChord(SignKey(cmd.forwardmove), SignKey(cmd.rightmove));
        plan.jump       = cmd.upmove > 0;
        plan.crouch     = false;
        plan.lean       = 0;
        plan.attack     = false;
        plan.yawDelta   = Q_clamp_float(AngleNormalize180(want.y - view.yaw), -30.0f, 30.0f);
        plan.pitchDelta = Q_clamp_float(want.x - view.pitch, -20.0f, 20.0f);
        plan.viewStill  = false;
        plan.flickShaped = false;
        SetOwner(p, hb::OWNER_LADDER);
        return;
    }

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

    // doors: the stock use logic when a closed door or a ladder is right ahead
    if (level.inttime < m_useUntil) {
        plan.use = true;
    } else if (hb::ChordFwd(plan.chord) > 0 || m_wallMs > 0.0f) {
        Vector fwd;
        Vector(0, view.yaw, 0).AngleVectors(&fwd);
        const Vector  start = p->origin + Vector(0, 0, p->viewheight);
        const trace_t tr    = G_Trace(start, vec_zero, vec_zero, start + fwd * 64.0f, p, MASK_USABLE | MASK_LADDER, qfalse, "HumanBot use");
        if (tr.ent && tr.ent->entity && tr.ent->entity != world) {
            Entity *e = tr.ent->entity;
            if ((e->IsSubclassOfDoor() && !static_cast<Door *>(e)->isOpen()) || e->isSubclassOf(FuncLadder)) {
                plan.use   = true;
                m_useUntil = level.inttime + 100;
                SetOwner(p, hb::OWNER_DOOR);
                return;
            }
        }
    }
    SetOwner(p, hb::OWNER_BRAIN);
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
    Owners(p, view, raw.self, plan);
    m_command = plan.command;

    //
    // Sub-step usercmds and the eye each of them carries
    //
    m_sub.Build(plan, view.yaw, view.pitch, m_rngSub, m_cmds);
    const bool kbdOk = hb::Substepper::CheckContract(m_cmds);
    if (!kbdOk) {
        m_kbdViolations++;
    }
    const float frameMs = static_cast<float>(hb::TICK_MS) / static_cast<float>(m_cmds.size());
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
            for (int i = 1; i <= inventory.NumObjects(); i++) {
                Entity *item = G_GetEntity(inventory.ObjectAt(i));
                if (!item || !item->IsSubclassOfWeapon()) {
                    continue;
                }
                Weapon   *w    = static_cast<Weapon *>(item);
                const int wc   = w->GetWeaponClass();
                const bool want = m_command == hb::CMD_PISTOL ? (wc & ::WEAPON_CLASS_PISTOL) != 0
                                                              : (wc & (::WEAPON_CLASS_SMG | ::WEAPON_CLASS_RIFLE | ::WEAPON_CLASS_MG | ::WEAPON_CLASS_HEAVY)) != 0;
                if (want && w != p->GetActiveWeapon(WEAPON_MAIN) && w->HasAmmo(FIRE_PRIMARY)) {
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
        if (u.forwardmove || u.rightmove || u.upmove || c.attack || c.use || c.lean || c.walk) {
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

    // a realistic ping only while disguised; bots otherwise show as bots
    if (HB_DisguiseActive()) {
        m_pingMs = m_ping.Step();
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
    g_humanbot_substeps  = gi.Cvar_Get("g_humanbot_substeps", "4", 0);
    g_humanbot_fov       = gi.Cvar_Get("g_humanbot_fov", "80", 0);
    g_humanbot_aspect    = gi.Cvar_Get("g_humanbot_aspect", "1.778", 0);
    g_humanbot_seed      = gi.Cvar_Get("g_humanbot_seed", "0", 0);
    g_humanbot_disguise  = gi.Cvar_Get("g_humanbot_disguise", "0", 0);
    g_humanbot_model_dir = gi.Cvar_Get("g_humanbot_model_dir", "", 0);
    g_humanbot_families  = gi.Cvar_Get("g_humanbot_families", "", 0);
    g_humanbot_debug     = gi.Cvar_Get("g_humanbot_debug", "0", 0);
    g_humanbot_wall_steer = gi.Cvar_Get("g_humanbot_wall_steer", "1", 0);
    g_humanbot_skill     = gi.Cvar_Get("g_humanbot_skill", "0", 0);

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
            "  %-20s %s  stuck>2s %d  pressure>500ms %.2f/min  kbd %d  think %.0f us (max %d)  %s\n",
            p && p->client ? p->client->pers.netname : "?",
            hb::StyleKey(a->Dials()).c_str(),
            a->StuckBouts(),
            pressurePerMin,
            a->KbdViolations(),
            a->MeanThinkUs(),
            a->MaxThinkUs(),
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
