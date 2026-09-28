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
// hb_arena.cpp: closed-loop games of human bots in a box world, moved by the
// real player movement code (pm_harness).
//
// Every bot runs the same pipeline as in the game: human-fair raw percepts
// (frustum, body parts, sight rays, sounds, damage) -> Perceiver -> Brain ->
// Substepper -> K usercmds -> Pmove. An SMG model resolves the shots. The
// arena reports the closed-loop checks of the plan (stuck and wall-pressure
// bouts, yaw speed, hidden stillness, firing, belief error, think time) and,
// with --test, fails when one is out of range.
//
//   hb_arena [--bots N] [--seconds S] [--seed N] [--substeps K] [--layout pillars|open]
//            [--override shared.json] [--reference human_reference.json] [--pooled] [--test] [--load]

#include "hb_brain.h"
#include "hb_bundle.h"
#include "hb_eye_math.h"
#include "hb_perception_model.h"
#include "hb_style.h"
#include "hb_substep.h"

#include "pm_runner.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace
{

const float ARENA_PI       = 3.14159265358979f;
const float TO_RAD         = ARENA_PI / 180.0f;
const float TO_DEG         = 180.0f / ARENA_PI;
const int   FRAME_MS       = 50;
const float EYE_VIS_HEIGHT = 64.0f;
const float CELL           = 48.0f;

struct Options {
    int         bots     = 2;
    int         seconds  = 300;
    uint64_t    seed     = 1;
    int         substeps = 4;
    std::string layout   = "pillars";
    std::string overrideFile;
    std::string reference;   // human_reference.json: print bot and human side by side
    bool        pooled = false;  // every bot plays the pooled (average human) style
    bool        test = false;
    bool        load = false;
};

//
// World: a closed room with pillars and cover
//
void BuildWorld(PmWorld& w, const std::string& layout, std::vector<std::vector<float>>& spawns)
{
    const float mins[3] = {-768.0f, -768.0f, 0.0f};
    const float maxs[3] = {768.0f, 768.0f, 256.0f};
    w.AddRoom(mins, maxs);
    if (layout != "open") {
        const float px[4] = {-384.0f, 384.0f, -384.0f, 384.0f};
        const float py[4] = {-384.0f, -384.0f, 384.0f, 384.0f};
        for (int i = 0; i < 4; i++) {
            w.AddPillar(px[i], py[i], 48.0f, 0.0f, 256.0f);
        }
        const float wallA0[3] = {-192.0f, -16.0f, 0.0f}, wallA1[3] = {192.0f, 16.0f, 120.0f};
        const float wallB0[3] = {-16.0f, 256.0f, 0.0f}, wallB1[3] = {16.0f, 640.0f, 256.0f};
        const float wallC0[3] = {-16.0f, -640.0f, 0.0f}, wallC1[3] = {16.0f, -256.0f, 256.0f};
        const float crate0[3] = {480.0f, -80.0f, 0.0f}, crate1[3] = {560.0f, 0.0f, 64.0f};
        const float crate2[3] = {-560.0f, 0.0f, 0.0f}, crate3[3] = {-480.0f, 80.0f, 64.0f};
        w.AddBox(wallA0, wallA1);
        w.AddBox(wallB0, wallB1);
        w.AddBox(wallC0, wallC1);
        w.AddBox(crate0, crate1);
        w.AddBox(crate2, crate3);
    }
    spawns = {{-700, -700, 225}, {700, 700, 45},   {-700, 700, 315}, {700, -700, 135},
              {0, -700, 90},     {0, 700, 270},    {-700, 0, 0},     {700, 0, 180}};
}

bool BoxFree(const PmWorld& w, float x, float y)
{
    const float  mins[3] = {-15.0f, -15.0f, 0.0f};
    const float  maxs[3] = {15.0f, 15.0f, 94.0f};
    const vec3_t p       = {x, y, 1.0f};
    trace_t      tr;
    w.Trace(&tr, p, mins, maxs, p, ENTITYNUM_NONE, MASK_PLAYERSOLID);
    return !tr.startsolid && !tr.allsolid;
}

bool BoxMoveClear(const PmWorld& w, const hb::Vec3& a, const hb::Vec3& b)
{
    const float  mins[3] = {-15.0f, -15.0f, 0.0f};
    const float  maxs[3] = {15.0f, 15.0f, 94.0f};
    const vec3_t s       = {a.x, a.y, 1.0f};
    const vec3_t e       = {b.x, b.y, 1.0f};
    trace_t      tr;
    w.Trace(&tr, s, mins, maxs, e, ENTITYNUM_NONE, MASK_PLAYERSOLID);
    return tr.fraction >= 1.0f && !tr.startsolid;
}

// A route-graph prior over the free floor (like the navmesh fallback of the game).
void BuildPrior(const PmWorld& w, const std::vector<std::vector<float>>& spawns, hb::MapPrior& m)
{
    m.name     = "arena";
    m.checksum = 1;
    m.cellSize = CELL;
    const int n0 = static_cast<int>(-768.0f / CELL), n1 = static_cast<int>(768.0f / CELL);
    for (int ix = n0; ix < n1; ix++) {
        for (int iy = n0; iy < n1; iy++) {
            const float x = (ix + 0.5f) * CELL, y = (iy + 0.5f) * CELL;
            if (!BoxFree(w, x, y)) {
                continue;
            }
            hb::MapCell c;
            c.ix       = ix;
            c.iy       = iy;
            c.z        = 0.0f;
            c.center   = hb::Vec3(x, y, 0.0f);
            c.occTotal = 0.0f;
            for (int k = 0; k < hb::CTX_COUNT; k++) {
                c.occ[k] = 1.0f;
                c.occTotal += 1.0f;
            }
            c.leave   = 0.3f;
            c.still   = 0.2f;
            c.snapped = true;
            m.cells.push_back(c);
        }
    }
    m.AddCellIndex();
    const int n = m.NumCells();
    m.kStart.assign(n + 1, 0);
    for (int a = 0; a < n; a++) {
        m.kStart[a] = static_cast<int>(m.kTo.size());
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (!dx && !dy) {
                    continue;
                }
                const hb::Vec3 p((m.cells[a].ix + dx + 0.5f) * CELL, (m.cells[a].iy + dy + 0.5f) * CELL, 0.0f);
                const int      b = m.CellAt(p);
                if (b >= 0 && BoxMoveClear(w, m.cells[a].center, m.cells[b].center)) {
                    m.kTo.push_back(b);
                    m.kCount.push_back(1.0f);
                }
            }
        }
    }
    m.kStart[n]         = static_cast<int>(m.kTo.size());
    m.kernelDistEdges   = {0.0f};
    m.kernelBeta        = {0.0f};
    m.fromNavmesh       = true;
    m.BuildRouteGraph();
    for (const std::vector<float>& s : spawns) {
        hb::MapSpawn sp;
        sp.pos   = hb::Vec3(s[0], s[1], 0.0f);
        sp.yaw   = s[2];
        sp.count = 1.0f;
        m.spawns.push_back(sp);
    }
    m.InitRuntimeVisibility();
    for (int a = 0; a < n; a++) {
        for (int b = a + 1; b < n; b++) {
            const vec3_t pa = {m.cells[a].center.x, m.cells[a].center.y, EYE_VIS_HEIGHT};
            const vec3_t pb = {m.cells[b].center.x, m.cells[b].center.y, EYE_VIS_HEIGHT};
            m.SetRuntimeVisibility(a, b, w.LineOfSight(pa, pb, ENTITYNUM_NONE));
        }
    }
    m.FinishRuntimeVisibility();
}

//
// Bots
//
struct Shot {
    int      shooter;
    hb::Vec3 pos;
};

struct Bot {
    int            id = 0;
    PmPlayer       pm;
    hb::StyleDials dials;
    hb::Brain      brain;
    hb::Perceiver  perceiver;
    hb::Substepper sub;
    hb::Rng        rngSub;
    hb::Rng        rngArena;
    hb::EyeState   eye;

    bool  alive     = false;
    float health    = 100.0f;
    int   diedAt    = -100000;
    int   clip      = 32;
    int   reserve   = 160;
    int   reloadEnd = 0;
    int   nextShot  = 0;
    bool  firing    = false;
    int   gotKillOf = -1;
    int   lastStep  = 0;

    std::vector<hb::RawDamage> damageIn;
    std::vector<hb::SubCmd>    cmds;
    hb::TickPlan               plan;
    hb::Diag                   diag;

    // own movement bookkeeping (like the engine adapter)
    float lastX = 0.0f, lastY = 0.0f;
    int   lastChord = hb::CHORD_NEUTRAL;
    float lastClear = 128.0f;
    float stuckMs = 0.0f, wallMs = 0.0f;
    float prevYaw = 0.0f;
    bool  prevYawValid = false;

    // metrics (duel mask: alive with a living opponent; contexts from the logger's true centroid ray)
    long long ticksAlive = 0, ticksHidden = 0, stillHidden = 0, ticksFight = 0, stillFight = 0;
    long long ctxTicks[hb::CTX_COUNT] = {};
    long long chordTicks[hb::CTX_COUNT][hb::NUM_CHORDS] = {};
    long long leanTicks[hb::CTX_COUNT] = {};
    long long mouseStill[hb::CTX_COUNT] = {};
    long long standStill[hb::CTX_COUNT] = {};
    long long revN[hb::CTX_COUNT] = {}, revY[hb::CTX_COUNT] = {};
    std::vector<float> yawByCtx[hb::CTX_COUNT];
    int       prevSide = 0, prevCtx = 0;
    bool      prevDuel = false;
    int       shots = 0, hits = 0, kills = 0, deaths = 0;
    int       stuckBouts = 0, pressureBouts = 0, maxStuckMs = 0;
    std::vector<float> yawSpeed;
    std::vector<float> beliefErr;
    double    thinkUs = 0.0;
    long long thinkN  = 0;
    int       thinkMax = 0;
    int       kbdBad = 0;
};

float WeaponState(const Bot& b, int now)
{
    if (now < b.reloadEnd) {
        return 5;
    }
    return b.firing ? 1 : 0;
}

hb::Vec3 Origin(const Bot& b)
{
    return hb::Vec3(b.pm.ps.origin[0], b.pm.ps.origin[1], b.pm.ps.origin[2]);
}

float BodyHeight(const Bot& b)
{
    return b.pm.maxs[2] - b.pm.mins[2];
}

// Six points of the body from its box (head, chest, belly, pelvis, feet).
void Parts(const Bot& b, hb::Vec3 out[hb::NUM_PARTS])
{
    const hb::Vec3 o      = Origin(b);
    const float    h      = BodyHeight(b);
    const float    yaw    = b.pm.ps.viewangles[YAW] * TO_RAD;
    const hb::Vec3 side(-std::sin(yaw), std::cos(yaw), 0.0f);
    out[hb::PART_HEAD]   = o + hb::Vec3(0, 0, 0.89f * h);
    out[hb::PART_CHEST]  = o + hb::Vec3(0, 0, 0.72f * h);
    out[hb::PART_BELLY]  = o + hb::Vec3(0, 0, 0.60f * h);
    out[hb::PART_PELVIS] = o + hb::Vec3(0, 0, 0.48f * h);
    out[hb::PART_LFOOT]  = o + side * 6.0f + hb::Vec3(0, 0, 4.0f);
    out[hb::PART_RFOOT]  = o - side * 6.0f + hb::Vec3(0, 0, 4.0f);
}

hb::Vec3 LogEye(const Bot& b)
{
    return Origin(b) + hb::Vec3(0, 0, static_cast<float>(b.pm.ps.viewheight));
}

bool Visible(const PmWorld& w, const hb::Vec3& from, const hb::Vec3& to, int viewer, int target)
{
    const vec3_t s = {from.x, from.y, from.z};
    const vec3_t e = {to.x, to.y, to.z};
    return w.LineOfSight(s, e, viewer, target);
}

class Arena
{
public:
    Arena(const Options& o, const hb::ModelBundle& b);
    void Run();
    bool Report();

private:
    void Respawn(Bot& b);
    void Decide(Bot& b, float hfov, float vfov);
    void Move(Bot& b);
    void Fire(Bot& b);
    void Measure(Bot& b);
    std::map<std::string, double> Metrics() const;
    void CompareReference(const std::map<std::string, double>& M) const;

    Options                        m_o;
    const hb::ModelBundle&         m_b;
    PmWorld                        m_world;
    PmSettings                     m_settings;
    hb::MapPrior                   m_prior;
    std::vector<std::vector<float>> m_spawns;
    std::vector<Bot>               m_bots;
    std::vector<hb::RawSound>      m_sounds, m_soundsNext;
    std::vector<int>               m_deaths, m_deathsNext;
    int                            m_now = 0;
    hb::Rng                        m_rng;
};

Arena::Arena(const Options& o, const hb::ModelBundle& b)
    : m_o(o)
    , m_b(b)
    , m_bots(o.bots)
    , m_rng(o.seed)
{
    BuildWorld(m_world, o.layout, m_spawns);
    BuildPrior(m_world, m_spawns, m_prior);
    for (int i = 0; i < o.bots; i++) {
        Bot& bot  = m_bots[i];
        bot.id    = i;
        bot.dials = o.pooled ? hb::PooledStyle(b.style)
                             : hb::SampleStyle(b.style, -1, static_cast<uint32_t>(o.seed * 7919u + i * 104729u));
        const uint64_t seed = o.seed * 1000003ull + static_cast<uint64_t>(i);
        const hb::Rng  root(seed);
        bot.brain.Init(&b, &m_prior, bot.dials, seed, o.substeps);
        bot.perceiver.Init(&b.shared.perception, root.Derive(hb::STREAM_PERCEPTION));
        bot.sub.Init(o.substeps);
        bot.rngSub   = root.Derive(hb::STREAM_SUBSTEP);
        bot.rngArena = root.Derive(99);
        bot.alive    = false;
        bot.diedAt   = -100000;
        // the first spawn needs no click
        Respawn(bot);
    }
}

void Arena::Respawn(Bot& b)
{
    // farthest start from the living others, like the FFA spawn metric
    int   best = 0;
    float bestD = -1.0f;
    for (size_t s = 0; s < m_spawns.size(); s++) {
        float d = 1e9f;
        for (const Bot& o : m_bots) {
            if (&o != &b && o.alive) {
                const float dx = o.pm.ps.origin[0] - m_spawns[s][0], dy = o.pm.ps.origin[1] - m_spawns[s][1];
                d              = std::min(d, std::sqrt(dx * dx + dy * dy));
            }
        }
        d *= 0.8f + 0.4f * static_cast<float>(m_rng.Uniform());
        if (d > bestD) {
            bestD = d;
            best  = static_cast<int>(s);
        }
    }
    const vec3_t origin = {m_spawns[best][0], m_spawns[best][1], 16.0f};
    PmInitPlayer(b.pm, b.id, origin, m_spawns[best][2], m_now, PM_SPEEDMULT_SMG);
    PmDropToFloor(b.pm, m_world, m_settings);
    PmLinkPlayer(m_world, b.pm);
    b.alive     = true;
    b.health    = 100.0f;
    b.clip      = 32;
    b.reserve   = 160;
    b.reloadEnd = 0;
    b.firing    = false;
    b.eye       = hb::EyeState();
    b.lastX     = b.pm.ps.origin[0];
    b.lastY     = b.pm.ps.origin[1];
    b.stuckMs   = 0.0f;
    b.wallMs    = 0.0f;
    b.prevYawValid = false;
    b.sub.Reset(b.pm.ps.viewangles[YAW], b.pm.ps.viewangles[PITCH]);
}

void Arena::Decide(Bot& b, float hfov, float vfov)
{
    const auto t0 = std::chrono::steady_clock::now();
    hb::RawInput raw;
    hb::SelfState& self = raw.self;
    self.timeMs    = m_now;
    self.entnum    = b.id;
    self.alive     = b.alive;
    self.spectator = !b.alive;
    self.origin    = Origin(b);
    self.velocity  = hb::Vec3(b.pm.ps.velocity[0], b.pm.ps.velocity[1], b.pm.ps.velocity[2]);
    self.viewYaw   = hb::Wrap180(b.pm.ps.viewangles[YAW]);
    self.viewPitch = hb::Wrap180(b.pm.ps.viewangles[PITCH]);
    hb::EyeInput ei;
    ei.origin      = self.origin;
    ei.velocity    = self.velocity;
    ei.viewheight  = static_cast<float>(b.pm.ps.viewheight);
    ei.pitch       = self.viewPitch;
    ei.yaw         = self.viewYaw;
    ei.leanAngle   = b.pm.ps.fLeanAngle;
    ei.walking     = b.pm.onGround;
    ei.frametimeMs = static_cast<float>(FRAME_MS);
    self.eye       = hb::ComputeEye(b.eye, ei);
    self.health    = b.health;
    self.onGround  = b.pm.onGround;
    self.ducked    = (b.pm.ps.pm_flags & PMF_DUCKED) != 0;
    self.weaponClass = hb::WEAPON_CLASS_SMG;
    self.weaponState = static_cast<int>(WeaponState(b, m_now));
    self.clipAmmo    = b.clip;
    self.clipSize    = 32;
    self.reserveAmmo = b.reserve;
    // clearance in the chord directions (the logger's probe)
    for (int c = 0; c < hb::NUM_CHORDS; c++) {
        if (c == hb::CHORD_NEUTRAL) {
            self.clearance[c] = 128.0f;
            continue;
        }
        const float a  = (self.viewYaw + hb::Mover::ChordAngle(c)) * TO_RAD;
        const vec3_t s = {self.origin.x, self.origin.y, self.origin.z};
        const vec3_t e = {self.origin.x + std::cos(a) * 128.0f, self.origin.y + std::sin(a) * 128.0f, self.origin.z};
        trace_t tr;
        m_world.Trace(&tr, s, b.pm.mins, b.pm.maxs, e, b.id, MASK_PLAYERSOLID & ~CONTENTS_BODY);
        self.clearance[c] = tr.startsolid ? 0.0f : tr.fraction * 128.0f;
    }
    // stuck and wall-pressure clocks
    const float dx = b.pm.ps.origin[0] - b.lastX, dy = b.pm.ps.origin[1] - b.lastY;
    const float moved = std::sqrt(dx * dx + dy * dy);
    const bool  pressing = b.alive && b.lastChord != hb::CHORD_NEUTRAL;
    if (pressing && moved < 2.0f) {
        b.stuckMs += FRAME_MS;
        b.maxStuckMs = std::max(b.maxStuckMs, static_cast<int>(b.stuckMs));
    } else {
        b.stuckBouts += b.stuckMs >= 2000.0f;
        b.stuckMs = 0.0f;
    }
    if (pressing && b.lastClear < 8.0f && moved < 5.0f) {
        b.wallMs += FRAME_MS;
    } else {
        b.pressureBouts += b.wallMs >= 500.0f;
        b.wallMs = 0.0f;
    }
    b.lastX          = b.pm.ps.origin[0];
    b.lastY          = b.pm.ps.origin[1];
    self.stuckMs     = b.stuckMs;
    self.wallPressMs = b.wallMs;

    if (b.alive) {
        const float tanH = std::tan(0.5f * hfov * TO_RAD), tanV = std::tan(0.5f * vfov * TO_RAD);
        hb::Vec3    fwd, left;
        hb::ForwardLeft(self.viewPitch, self.viewYaw, fwd, left);
        const hb::Vec3 up(left.y * fwd.z - left.z * fwd.y, left.z * fwd.x - left.x * fwd.z, left.x * fwd.y - left.y * fwd.x);
        for (Bot& o : m_bots) {
            if (&o == &b) {
                continue;
            }
            hb::RawEnemy e;
            e.id    = o.id;
            e.alive = o.alive;
            if (o.alive) {
                hb::Vec3 parts[hb::NUM_PARTS];
                Parts(o, parts);
                int inside = 0;
                for (int k = 0; k < hb::NUM_PARTS; k++) {
                    const hb::Vec3 d = parts[k] - self.eye;
                    const float    x = d.dot(fwd);
                    if (x > 1.0f && std::fabs(d.dot(left)) <= x * tanH && std::fabs(d.dot(up)) <= x * tanV) {
                        inside |= 1 << k;
                    }
                }
                int mask = 0;
                for (int k = 0; k < hb::NUM_PARTS; k++) {
                    if ((inside & (1 << k)) && Visible(m_world, self.eye, parts[k], b.id, o.id)) {
                        mask |= 1 << k;
                    }
                }
                if (mask) {
                    e.inFov    = true;
                    e.partMask = mask;
                    for (int k = 0; k < hb::NUM_PARTS; k++) {
                        if (mask & (1 << k)) {
                            e.partPos[k] = parts[k];
                        }
                    }
                    const hb::Vec3 cen = Origin(o) + hb::Vec3(0, 0, 0.5f * BodyHeight(o));
                    e.centroid         = cen;
                    e.velocity         = hb::Vec3(o.pm.ps.velocity[0], o.pm.ps.velocity[1], o.pm.ps.velocity[2]);
                    e.bodyHeight       = BodyHeight(o);
                    e.centroidLos      = Visible(m_world, LogEye(b), cen, b.id, o.id);
                    e.reloading        = m_now < o.reloadEnd;
                    e.firing           = o.firing;
                }
            }
            raw.enemies.push_back(e);
        }
        for (const hb::RawSound& s : m_sounds) {
            if (s.sourceId != b.id) {
                raw.sounds.push_back(s);
            }
        }
        raw.damage = b.damageIn;
    }
    b.damageIn.clear();
    raw.deaths    = m_deaths;
    raw.gotKillOf = b.gotKillOf;
    b.gotKillOf   = -1;

    hb::Observation obs;
    b.perceiver.Process(raw, hfov, vfov, b.brain.Offsets().detectMult, obs);
    b.brain.SetFov(hfov, vfov);
    b.brain.Think(obs, b.plan, &b.diag);
    b.sub.Build(b.plan, self.viewYaw, self.viewPitch, b.rngSub, b.cmds);
    if (!hb::Substepper::CheckContract(b.cmds)) {
        b.kbdBad++;
    }
    const int us = static_cast<int>(
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count()
    );
    b.thinkUs += us;
    b.thinkN++;
    b.thinkMax = std::max(b.thinkMax, us);
    b.lastChord = b.cmds.empty() ? hb::CHORD_NEUTRAL : b.cmds.back().chord;
    b.lastClear = b.lastChord == hb::CHORD_NEUTRAL ? 128.0f : self.clearance[b.lastChord];
}

void Arena::Move(Bot& b)
{
    if (!b.alive) {
        // the respawn click (the engine needs a fresh press after the respawn delay)
        bool click = false;
        for (const hb::SubCmd& c : b.cmds) {
            click = click || c.attack;
        }
        if (click && m_now - b.diedAt >= 1000) {
            Respawn(b);
        }
        return;
    }
    const int K = static_cast<int>(b.cmds.size());
    for (int k = 0; k < K; k++) {
        const hb::SubCmd& c = b.cmds[k];
        int buttons = 0;
        if (c.attack) {
            buttons |= BUTTON_ATTACKLEFT;
        }
        if (!c.walk) {
            buttons |= BUTTON_RUN;
        }
        if (c.lean < 0) {
            buttons |= BUTTON_LEAN_LEFT;
        } else if (c.lean > 0) {
            buttons |= BUTTON_LEAN_RIGHT;
        }
        usercmd_t u = PmMakeUsercmd(c.pitch, c.yaw, hb::ChordFwd(c.chord) * 127, hb::ChordSide(c.chord) * 127,
                                    c.jump ? 127 : (c.crouch ? -127 : 0), buttons);
        u.serverTime = m_now + c.serverTimeOffset;
        PmRunUsercmd(b.pm, m_world, m_settings, u, m_now);
    }
    PmRunThink(b.pm, m_world, m_settings);
    // footsteps while running on the ground (walking and crouching are silent)
    const float speed = PmSpeedXY(b.pm);
    const bool  run   = !b.cmds.empty() && !b.cmds.back().walk && !b.cmds.back().crouch;
    if (b.pm.onGround && run && speed > 150.0f && m_now - b.lastStep >= 350) {
        b.lastStep = m_now;
        hb::RawSound s;
        s.type     = hb::SOUND_FOOTSTEP;
        s.sourceId = b.id;
        s.origin   = Origin(b);
        m_soundsNext.push_back(s);
    }
}

void Arena::Fire(Bot& b)
{
    b.firing = false;
    if (!b.alive || b.cmds.empty()) {
        return;
    }
    if (b.plan.command == hb::CMD_RELOAD && m_now >= b.reloadEnd && b.clip < 32 && b.reserve > 0) {
        b.reloadEnd = m_now + 2350;
        hb::RawSound s;
        s.type     = hb::SOUND_RELOAD;
        s.sourceId = b.id;
        s.origin   = Origin(b);
        m_soundsNext.push_back(s);
    }
    if (b.reloadEnd && m_now >= b.reloadEnd) {
        const int take = std::min(32 - b.clip, b.reserve);
        b.clip += take;
        b.reserve -= take;
        b.reloadEnd = 0;
    }
    const bool attack = b.cmds.back().attack;
    if (!attack || m_now < b.reloadEnd || b.clip <= 0 || m_now < b.nextShot) {
        return;
    }
    // one SMG round per 100 ms, resolved once per frame like Player::Think
    b.firing   = true;
    b.nextShot = m_now + 100;
    b.clip--;
    b.shots++;
    const float    speed  = PmSpeedXY(b.pm);
    const float    spread = (speed > 100.0f ? 2.4f : 1.2f) * (b.pm.onGround ? 1.0f : 3.0f);
    const float    yaw    = b.pm.ps.viewangles[YAW] + static_cast<float>(b.rngArena.Normal()) * spread;
    const float    pitch  = b.pm.ps.viewangles[PITCH] + static_cast<float>(b.rngArena.Normal()) * spread;
    hb::EyeInput   ei;
    const hb::Vec3 eye = LogEye(b);
    (void)ei;
    const float    cp = std::cos(pitch * TO_RAD);
    const hb::Vec3 dir(cp * std::cos(yaw * TO_RAD), cp * std::sin(yaw * TO_RAD), -std::sin(pitch * TO_RAD));
    const vec3_t   s = {eye.x, eye.y, eye.z};
    const vec3_t   e = {eye.x + dir.x * 4096.0f, eye.y + dir.y * 4096.0f, eye.z + dir.z * 4096.0f};
    const vec3_t   zero = {0, 0, 0};
    trace_t        tr;
    m_world.Trace(&tr, s, zero, zero, e, b.id, MASK_SHOT);
    hb::RawSound snd;
    snd.type     = hb::SOUND_GUNFIRE;
    snd.sourceId = b.id;
    snd.origin   = eye;
    m_soundsNext.push_back(snd);
    if (tr.entityNum < 0 || tr.entityNum >= static_cast<int>(m_bots.size())) {
        return;
    }
    Bot& t = m_bots[tr.entityNum];
    if (!t.alive) {
        return;
    }
    const float hitZ   = tr.endpos[2] - t.pm.ps.origin[2];
    const float damage = hitZ > 0.83f * BodyHeight(t) ? 60.0f : 20.0f;
    b.hits++;
    t.health -= damage;
    hb::RawDamage d;
    d.attackerId  = b.id;
    d.attackerPos = Origin(b);
    d.damage      = damage;
    t.damageIn.push_back(d);
    if (t.health <= 0.0f) {
        t.alive  = false;
        t.diedAt = m_now;
        t.deaths++;
        b.kills++;
        b.gotKillOf = t.id;
        m_deathsNext.push_back(t.id);
        m_world.ClearBody(t.id);
    }
}

void Arena::Measure(Bot& b)
{
    if (!b.alive) {
        b.prevYawValid = false;
        b.prevDuel     = false;
        return;
    }
    // the nearest living enemy, like the logger's opponent columns
    const Bot *opp  = nullptr;
    float      best = 1e18f;
    for (const Bot& o : m_bots) {
        if (&o == &b || !o.alive) {
            continue;
        }
        const float d = (Origin(o) - Origin(b)).lengthSq();
        if (d < best) {
            best = d;
            opp  = &o;
        }
    }
    const float yaw    = b.pm.ps.viewangles[YAW];
    const float yawD   = b.prevYawValid ? std::fabs(hb::Wrap180(yaw - b.prevYaw)) : -1.0f;
    b.prevYaw          = yaw;
    b.prevYawValid     = true;
    if (!opp) {
        b.prevDuel = false;
        return;
    }
    b.ticksAlive++;
    const hb::Vec3 cen    = Origin(*opp) + hb::Vec3(0, 0, 0.5f * BodyHeight(*opp));
    const bool     los    = Visible(m_world, LogEye(b), cen, b.id, opp->id);
    const bool     attack = !b.cmds.empty() && b.cmds.back().attack;
    const int      ctx    = m_now < b.reloadEnd ? hb::CTX_RELOAD
                          : (los ? (attack ? hb::CTX_LOS_FIRE : hb::CTX_LOS_NOFIRE) : (attack ? hb::CTX_HIDDEN_FIRE : hb::CTX_HIDDEN_NOFIRE));
    const int      chord  = b.cmds.empty() ? hb::CHORD_NEUTRAL : b.cmds.back().chord;
    b.ctxTicks[ctx]++;
    b.chordTicks[ctx][chord]++;
    b.leanTicks[ctx] += !b.cmds.empty() && b.cmds.back().lean != 0;
    b.standStill[ctx] += PmSpeedXY(b.pm) < 5.0f;
    if (yawD >= 0.0f) {
        b.yawSpeed.push_back(yawD * (1000.0f / FRAME_MS));
        b.yawByCtx[ctx].push_back(yawD * (1000.0f / FRAME_MS));
        b.mouseStill[ctx] += yawD < 0.01f;
    }
    // how strafes end, in the context of the tick before the change
    const int side = hb::ChordSide(chord);
    if (b.prevDuel && b.prevSide != 0 && side != b.prevSide) {
        b.revN[b.prevCtx]++;
        b.revY[b.prevCtx] += side == -b.prevSide;
    }
    b.prevSide = side;
    b.prevCtx  = ctx;
    b.prevDuel = true;
    if (!b.diag.detected && !attack) {
        b.ticksHidden++;
        b.stillHidden += b.plan.viewStill;
        if (!los && b.diag.belief_spread > 0.0f) {
            const float dx = opp->pm.ps.origin[0] - b.diag.belief_x, dy = opp->pm.ps.origin[1] - b.diag.belief_y;
            b.beliefErr.push_back(std::sqrt(dx * dx + dy * dy));
        }
    } else if (los) {
        b.ticksFight++;
        b.stillFight += b.plan.viewStill;
    }
}

void Arena::Run()
{
    float hfov, vfov;
    {
        const float tv = std::tan(40.0f * TO_RAD) * 0.75f;
        vfov           = 2.0f * std::atan(tv) * TO_DEG;
        hfov           = 2.0f * std::atan(tv * (16.0f / 9.0f)) * TO_DEG;
    }
    PmWorldScope scope(m_world);
    const int    frames = m_o.seconds * 1000 / FRAME_MS;
    for (int f = 0; f < frames; f++) {
        m_now += FRAME_MS;
        m_sounds.swap(m_soundsNext);
        m_soundsNext.clear();
        m_deaths.swap(m_deathsNext);
        m_deathsNext.clear();
        // every bot decides on the same snapshot, then all move
        for (Bot& b : m_bots) {
            Decide(b, hfov, vfov);
        }
        for (Bot& b : m_bots) {
            Move(b);
        }
        for (Bot& b : m_bots) {
            Fire(b);
        }
        for (Bot& b : m_bots) {
            Measure(b);
        }
    }
}

float Quantile(std::vector<float> v, double q)
{
    if (v.empty()) {
        return NAN;
    }
    std::sort(v.begin(), v.end());
    return v[std::min(v.size() - 1, static_cast<size_t>(q * static_cast<double>(v.size())))];
}

static const char *const CTX_NAMES[hb::CTX_COUNT] = {"hidden_nofire", "hidden_fire", "los_nofire", "los_fire", "reload"};

std::map<std::string, double> Arena::Metrics() const
{
    std::map<std::string, double> M;
    long long ctx[hb::CTX_COUNT] = {}, chords[hb::CTX_COUNT][hb::NUM_CHORDS] = {}, lean[hb::CTX_COUNT] = {};
    long long mouse[hb::CTX_COUNT] = {}, stand[hb::CTX_COUNT] = {}, revN[hb::CTX_COUNT] = {}, revY[hb::CTX_COUNT] = {};
    std::vector<float> yaw[hb::CTX_COUNT];
    for (const Bot& b : m_bots) {
        for (int c = 0; c < hb::CTX_COUNT; c++) {
            ctx[c] += b.ctxTicks[c];
            lean[c] += b.leanTicks[c];
            mouse[c] += b.mouseStill[c];
            stand[c] += b.standStill[c];
            revN[c] += b.revN[c];
            revY[c] += b.revY[c];
            yaw[c].insert(yaw[c].end(), b.yawByCtx[c].begin(), b.yawByCtx[c].end());
            for (int k = 0; k < hb::NUM_CHORDS; k++) {
                chords[c][k] += b.chordTicks[c][k];
            }
        }
    }
    long long tot = 0, leanAll = 0, standAll = 0;
    for (int c = 0; c < hb::CTX_COUNT; c++) {
        tot += ctx[c];
        leanAll += lean[c];
        standAll += stand[c];
    }
    auto share = [](long long a, long long n) { return n > 0 ? static_cast<double>(a) / n : NAN; };
    for (int c = 0; c < hb::CTX_COUNT; c++) {
        const std::string n = CTX_NAMES[c];
        const long long*  k = chords[c];
        M["movement.context_share." + n]      = share(ctx[c], tot);
        M["movement.chord." + n + ".pure_strafe"] = share(k[3] + k[5], ctx[c]);
        M["movement.chord." + n + ".fwd_diag"]    = share(k[6] + k[8], ctx[c]);
        M["movement.chord." + n + ".forward"]     = share(k[7], ctx[c]);
        M["movement.chord." + n + ".neutral"]     = share(k[4], ctx[c]);
        M["movement.chord." + n + ".back_any"]    = share(k[0] + k[1] + k[2], ctx[c]);
        M["movement.strafe_reverse." + n]     = share(revY[c], revN[c]);
        M["movement.still." + n]              = share(stand[c], ctx[c]);
        M["view.mouse_still." + n]            = share(mouse[c], static_cast<long long>(yaw[c].size()));
        M["view.yaw_speed." + n + ".p50"]     = Quantile(yaw[c], 0.5);
        M["view.yaw_speed." + n + ".p99"]     = Quantile(yaw[c], 0.99);
    }
    M["movement.lean.all"]      = share(leanAll, tot);
    M["movement.lean.los_fire"] = share(lean[hb::CTX_LOS_FIRE], ctx[hb::CTX_LOS_FIRE]);
    M["movement.still.all"]     = share(standAll, tot);
    return M;
}

// Bot and human side by side for every statistic both have.
void Arena::CompareReference(const std::map<std::string, double>& M) const
{
    std::ifstream     f(m_o.reference);
    std::stringstream ss;
    ss << f.rdbuf();
    const std::string text = ss.str();
    std::printf("%-44s %9s %9s\n", "statistic", "bot", "human");
    for (const auto& kv : M) {
        const std::string key = "\"" + kv.first + "\"";
        const size_t      at  = text.find(key);
        if (at == std::string::npos) {
            continue;
        }
        const size_t v = text.find("\"value\":", at);
        if (v == std::string::npos) {
            continue;
        }
        const double human = std::atof(text.c_str() + v + 8);
        std::printf("%-44s %9.3f %9.3f\n", kv.first.c_str(), kv.second, human);
    }
}

bool Arena::Report()
{
    const double minutes = m_o.seconds / 60.0;
    std::vector<float> yaws, errs;
    long long hidden = 0, still = 0, fight = 0, stillF = 0, shots = 0, hits = 0, kills = 0, kbd = 0;
    long long ctx[hb::CTX_COUNT] = {}, chords[hb::CTX_COUNT][hb::NUM_CHORDS] = {}, lean[hb::CTX_COUNT] = {};
    int       stuck = 0, pressure = 0, maxStuck = 0;
    double    us = 0.0;
    long long usN = 0;
    int       usMax = 0;
    for (const Bot& b : m_bots) {
        yaws.insert(yaws.end(), b.yawSpeed.begin(), b.yawSpeed.end());
        errs.insert(errs.end(), b.beliefErr.begin(), b.beliefErr.end());
        hidden += b.ticksHidden;
        still += b.stillHidden;
        fight += b.ticksFight;
        stillF += b.stillFight;
        shots += b.shots;
        hits += b.hits;
        kills += b.kills;
        kbd += b.kbdBad;
        stuck += b.stuckBouts;
        pressure += b.pressureBouts;
        maxStuck = std::max(maxStuck, b.maxStuckMs);
        us += b.thinkUs;
        usN += b.thinkN;
        usMax = std::max(usMax, b.thinkMax);
        for (int c = 0; c < hb::CTX_COUNT; c++) {
            ctx[c] += b.ctxTicks[c];
            lean[c] += b.leanTicks[c];
            for (int k = 0; k < hb::NUM_CHORDS; k++) {
                chords[c][k] += b.chordTicks[c][k];
            }
        }
    }
    const double botMinutes    = minutes * static_cast<double>(m_bots.size());
    const float  yawP99        = Quantile(yaws, 0.99);
    const float  stillHidden   = hidden ? static_cast<float>(still) / hidden : NAN;
    const float  pressurePerMin = static_cast<float>(pressure / std::max(botMinutes, 1e-9));
    const float  meanUs        = usN ? static_cast<float>(us / usN) : 0.0f;

    std::ostringstream j;
    j << "{\"bots\":" << m_bots.size() << ",\"seconds\":" << m_o.seconds << ",\"layout\":\"" << m_o.layout << "\"";
    j << ",\"stuck_bouts_over_2s\":" << stuck << ",\"max_stuck_ms\":" << maxStuck;
    j << ",\"pressure_bouts_per_bot_min\":" << pressurePerMin;
    j << ",\"yaw_speed_p50\":" << Quantile(yaws, 0.5) << ",\"yaw_speed_p99\":" << yawP99;
    j << ",\"still_hidden\":" << stillHidden << ",\"still_fight\":" << (fight ? static_cast<float>(stillF) / fight : NAN);
    j << ",\"shots_per_bot_min\":" << shots / botMinutes << ",\"hit_share\":" << (shots ? static_cast<double>(hits) / shots : 0.0);
    j << ",\"kills_per_bot_min\":" << kills / botMinutes;
    j << ",\"belief_err_p50\":" << Quantile(errs, 0.5) << ",\"belief_err_p90\":" << Quantile(errs, 0.9);
    j << ",\"think_us_mean\":" << meanUs << ",\"think_us_max\":" << usMax << ",\"kbd_violations\":" << kbd;
    j << ",\"ctx_share\":[";
    long long ctxTot = 0;
    for (long long c : ctx) {
        ctxTot += c;
    }
    for (int c = 0; c < hb::CTX_COUNT; c++) {
        j << (c ? "," : "") << (ctxTot ? static_cast<double>(ctx[c]) / ctxTot : 0.0);
    }
    j << "],\"fwd_share_by_ctx\":[";
    for (int c = 0; c < hb::CTX_COUNT; c++) {
        const long long f = chords[c][6] + chords[c][7] + chords[c][8];
        j << (c ? "," : "") << (ctx[c] ? static_cast<double>(f) / ctx[c] : 0.0);
    }
    j << "],\"neutral_by_ctx\":[";
    for (int c = 0; c < hb::CTX_COUNT; c++) {
        j << (c ? "," : "") << (ctx[c] ? static_cast<double>(chords[c][4]) / ctx[c] : 0.0);
    }
    j << "],\"lean_by_ctx\":[";
    for (int c = 0; c < hb::CTX_COUNT; c++) {
        j << (c ? "," : "") << (ctx[c] ? static_cast<double>(lean[c]) / ctx[c] : 0.0);
    }
    j << "]";
    // the same statistics as humanbot/eval (human_reference.json keys)
    std::map<std::string, double> M = Metrics();
    j << ",\"metrics\":{";
    bool first = true;
    for (const auto& kv : M) {
        j << (first ? "" : ",") << "\"" << kv.first << "\":" << (std::isfinite(kv.second) ? kv.second : -1.0);
        first = false;
    }
    j << "}}";
    std::printf("%s\n", j.str().c_str());
    if (!m_o.reference.empty()) {
        CompareReference(M);
    }

    if (!m_o.test && !m_o.load) {
        return true;
    }
    bool ok = true;
    auto check = [&](bool cond, const char *what) {
        if (!cond) {
            std::printf("FAIL %s\n", what);
            ok = false;
        }
    };
    if (m_o.test) {
        check(stuck == 0, "a stuck bout over 2 s");
        check(pressurePerMin < 1.0f, "wall-pressure bouts of 500 ms or more: at least 1 per bot-minute");
        check(kbd == 0, "keyboard contract violations");
        check(shots > 0 && kills > 0, "bots never fire or never kill");
        check(yawP99 > 300.0f && yawP99 < 1400.0f, "yaw speed p99 far outside the human 450-700 deg/s");
        check(stillHidden > 0.08f && stillHidden < 0.7f, "hidden stillness far outside the human 20-40%");
    }
#ifdef NDEBUG
    check(meanUs <= 150.0f, "mean think time over 150 us per bot per tick");
#endif
    std::printf("hb_arena: %s\n", ok ? "PASS" : "FAIL");
    return ok;
}

} // namespace

int main(int argc, char **argv)
{
    Options o;
    for (int i = 1; i < argc; i++) {
        const std::string a    = argv[i];
        auto              next = [&]() { return i + 1 < argc ? std::string(argv[++i]) : std::string(); };
        if (a == "--bots") {
            o.bots = std::max(2, std::atoi(next().c_str()));
        } else if (a == "--seconds") {
            o.seconds = std::max(1, std::atoi(next().c_str()));
        } else if (a == "--seed") {
            o.seed = std::strtoull(next().c_str(), nullptr, 10);
        } else if (a == "--substeps") {
            o.substeps = std::max(1, std::min(8, std::atoi(next().c_str())));
        } else if (a == "--layout") {
            o.layout = next();
        } else if (a == "--override") {
            o.overrideFile = next();
        } else if (a == "--reference") {
            o.reference = next();
        } else if (a == "--pooled") {
            o.pooled = true;
        } else if (a == "--test") {
            o.test = true;
        } else if (a == "--load") {
            o.load = true;
        } else {
            std::fprintf(stderr, "unknown option %s\n", a.c_str());
            return 2;
        }
    }
    std::map<std::string, std::string> overrides;
    if (!o.overrideFile.empty()) {
        std::ifstream     f(o.overrideFile);
        std::stringstream ss;
        ss << f.rdbuf();
        overrides["shared.json"] = ss.str();
    }
    hb::ModelBundle b;
    std::string     err;
    if (!hb::LoadBundle(overrides, b, err)) {
        std::fprintf(stderr, "model: %s\n", err.c_str());
        if (b.sha256.empty()) {
            return 1;
        }
    }
    Arena arena(o, b);
    arena.Run();
    return arena.Report() ? 0 : 1;
}
