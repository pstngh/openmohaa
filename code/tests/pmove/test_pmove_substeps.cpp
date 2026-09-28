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
// test_pmove_substeps.cpp: the real Pmove on a flat floor, driven with K = 1,
// 2 or 4 usercmds per 50 ms server frame (pm_harness).
//
// Checks the box world's trace semantics, then movement invariants: no NaN,
// ground contact after every usercmd, run/walk/crouch speeds, strafe onset,
// reversal and stop timing for every K and key-change sub-step, a mid-frame
// key change bracketed by the whole-frame changes, and the jump. K = 4 is
// compared with the pooled human response of the movement telemetry.
//
// usage: test_pmove_substeps [-v]   (-v prints every simulated curve)

#include "pm_runner.h"

#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <vector>

namespace
{

//
// Pooled human targets, measured by humanbot/fit/fit_velocity_response.py
// (writes humanbot/cache/velocity_response.json) from the duel-mask telemetry:
// unbroken segments, MP40/Thompson, on the ground, not ducked, BUTTON_RUN held.
// Tick 1 is the first 50 ms row whose last usercmd has the new key state.
//
const float HUMAN_RUN_SPEED        = 240.0f; // run_speed_forward_held_median (raw median 233.7)
const float HUMAN_WALK_SPEED       = 144.0f; // held_speeds.walk_forward.held.q50
const float HUMAN_CROUCH_SPEED     = 144.0f; // held_speeds.crouch_forward_key_released.mode
const float HUMAN_CROUCH_KEY_SPEED = 101.8f; // held_speeds.crouch_forward_key_held.all.q50
// strafe_onset.lat_speed_median: |vel_right| after 0 -> strafe key, from standing still (n 324)
const float HUMAN_ONSET[] = {35.0f, 97.3f, 141.5f, 174.0f, 198.3f, 202.2f};
// strafe_reversal_full_speed: ticks until vel_right takes the new sign (n 3400), and the
// lateral speed in the new key's direction (lat_speed_new_dir_median)
const float HUMAN_REVERSAL_TICKS = 3.0f;
const float HUMAN_REVERSAL[]     = {-130.7f, -24.5f, 49.5f, 106.4f, 148.8f, 179.7f};
// stop_from_full_strafe: Kaplan-Meier median ticks until speed_xy < 5 (keys released, n 2779)
// and the median speed of the stops still released (speed_median_while_released)
const float HUMAN_STOP_TICKS = 7.0f;
const float HUMAN_STOP[]     = {178.0f, 134.6f, 85.9f, 62.0f, 47.3f, 33.1f};
// jump: take-off row, first airborne row, apex above the take-off height (n 400)
const float HUMAN_JUMP_TAKEOFF_VZ = 299.33f; // takeoff_vz_median
const float HUMAN_JUMP_FIRST_VZ   = 255.1f;  // first_air_vz.q50
const float HUMAN_JUMP_FIRST_RISE = 13.7f;   // first_air_rise.q50
const float HUMAN_JUMP_APEX_Q50   = 50.9f;   // apex_rise.q50
const float HUMAN_JUMP_APEX_Q90   = 55.6f;   // apex_rise.q90

// Tolerances of K = 4 against the human targets
const float TOL_HELD_SPEED     = 1.0f;  // held speeds are exact physics values
const float TOL_CURVE          = 15.0f; // u/s per tick (sub-step timing of human usercmds)
const float TOL_REVERSAL_TICKS = 0.5f;
const float TOL_STOP_TICKS     = 1.0f;
const float TOL_JUMP_FIRST_VZ  = 10.0f;
const float TOL_JUMP_RISE      = 2.0f;
// A mid-frame change must lie between the frame-start and frame-end changes of K = 1,
// up to the K-dependence of the friction/acceleration integration
const float TOL_BRACKET = 5.0f;

// Physics of an SMG carrier (PM_CmdScale, pm_accelerate 8, pm_friction 6, stopspeed 50)
const float SMG_RUN_SPEED    = 240.0f;
const float SMG_STRAFE_SPEED = 240.0f * 107.0f / 127.0f; // 202.205: rightmove 127 * 0.85 truncates to 107
const float SMG_BACK_SPEED   = 240.0f * 101.0f / 127.0f; // 190.866: forwardmove -127 * 0.8 truncates to 101
const float SMG_SLOW_SPEED   = 144.0f;                   // walk or crouch: (int)(250 * 0.6) * 0.96
const float JUMP_SPEED       = 299.3326f;                // sqrt(2 * 800 * 56)
const float FLOOR_Z          = 0.125f;                   // resting height over a floor at 0 (SURFACE_CLIP_EPSILON)
const float STOPPED          = 5.0f;

const int   START_TIME = 100000;
const int   TICKS      = 6;
const int   SUBSTEPS[] = {1, 2, 4};
const float EPS        = 0.01f;

int  checks   = 0;
int  failures = 0;
bool verbose  = false;

void Check(bool ok, const char *fmt, ...) Q_PRINTF_FUNC(2, 3);

void Check(bool ok, const char *fmt, ...)
{
    va_list args;

    checks++;
    if (ok) {
        return;
    }

    failures++;
    fprintf(stderr, "FAIL: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

bool Near(float a, float b, float tol)
{
    return fabsf(a - b) <= tol;
}

typedef std::vector<float> Curve;

void PrintCurve(const char *label, const Curve& curve)
{
    size_t i;

    printf("  %-30s", label);
    for (i = 0; i < curve.size(); i++) {
        printf(" %7.1f", curve[i]);
    }
    printf("\n");
}

// Median of the curves at every tick
Curve MedianCurve(const std::vector<Curve>& curves)
{
    Curve  out;
    size_t t, i;

    for (t = 0; t < curves[0].size(); t++) {
        std::vector<float> v;

        for (i = 0; i < curves.size(); i++) {
            v.push_back(curves[i][t]);
        }
        std::sort(v.begin(), v.end());
        out.push_back(v.size() % 2 ? v[v.size() / 2] : 0.5f * (v[v.size() / 2 - 1] + v[v.size() / 2]));
    }
    return out;
}

float Median(std::vector<float> v)
{
    std::sort(v.begin(), v.end());
    return v.size() % 2 ? v[v.size() / 2] : 0.5f * (v[v.size() / 2 - 1] + v[v.size() / 2]);
}

// First tick (1-based) where pred holds, 0 if never
template<typename Pred>
int FirstTick(const Curve& curve, Pred pred)
{
    size_t t;

    for (t = 0; t < curve.size(); t++) {
        if (pred(curve[t])) {
            return (int)t + 1;
        }
    }
    return 0;
}

//
// Simulation: a big closed room, player 0 standing at the centre facing +x
//
struct Sim {
    PmWorld     world;
    PmSettings  settings;
    PmPlayer    player;
    int         time;
    bool        checkGround; // assert ground contact after every usercmd
    const char *name;
};

usercmd_t Keys(int forward, int right, int up, int buttons = BUTTON_RUN)
{
    return PmMakeUsercmd(0, 0, forward, right, up, buttons);
}

void Spawn(Sim& sim, const char *name, bool checkGround = true)
{
    const float  mins[3] = {-4096, -4096, 0};
    const float  maxs[3] = {4096, 4096, 1024};
    const vec3_t origin  = {0, 0, 8};

    sim.world.AddRoom(mins, maxs);
    sim.time        = START_TIME;
    sim.checkGround = checkGround;
    sim.name        = name;

    PmInitPlayer(sim.player, 0, origin, 0, sim.time);
    Check(PmDropToFloor(sim.player, sim.world, sim.settings), "%s: no floor under the spawn", name);
    Check(Near(sim.player.ps.origin[2], FLOOR_Z, 1e-4f), "%s: rests at z %f", name, sim.player.ps.origin[2]);
}

void CheckState(Sim& sim, int cmdIndex)
{
    const PmPlayer& p    = sim.player;
    bool            good = true;
    int             i;

    for (i = 0; i < 3; i++) {
        good = good && std::isfinite(p.ps.origin[i]) && std::isfinite(p.ps.velocity[i]);
    }
    Check(good, "%s: non-finite state at t %d cmd %d", sim.name, sim.time, cmdIndex);

    if (sim.checkGround) {
        Check(
            p.onGround && p.ps.walking && p.ps.groundEntityNum == ENTITYNUM_WORLD
                && Near(p.ps.origin[2], FLOOR_Z, 1e-4f) && p.ps.velocity[2] == 0,
            "%s: left the ground at t %d cmd %d (z %f vz %f)",
            sim.name,
            sim.time,
            cmdIndex,
            p.ps.origin[2],
            p.ps.velocity[2]
        );
    }
}

// One server frame: numCmds usercmds, `after` from sub-step changeIndex on, then
// the think. Same as PmRunServerFrame, with the state checked after each usercmd.
void Frame(Sim& sim, const usercmd_t& before, const usercmd_t& after, int numCmds, int changeIndex)
{
    int i;

    sim.time += sim.settings.frameMsec;
    for (i = 0; i < numCmds; i++) {
        usercmd_t cmd = i < changeIndex ? before : after;

        cmd.serverTime = PmSubstepTime(sim.time, sim.settings.frameMsec, numCmds, i);
        Check(
            PmRunUsercmd(sim.player, sim.world, sim.settings, cmd, sim.time),
            "%s: usercmd at %d ignored",
            sim.name,
            cmd.serverTime
        );
        CheckState(sim, i);
    }
    Check(
        sim.player.ps.commandTime == sim.time,
        "%s: commandTime %d != frame time %d",
        sim.name,
        sim.player.ps.commandTime,
        sim.time
    );

    PmRunThink(sim.player, sim.world, sim.settings);
}

void Frames(Sim& sim, const usercmd_t& cmd, int numCmds, int count)
{
    int i;

    for (i = 0; i < count; i++) {
        Frame(sim, cmd, cmd, numCmds, 0);
    }
}

//
// Movement responses. `change` is the sub-step of frame 1 where the new keys start;
// change == K means "at the end of the frame" (from frame 2 on). Curves hold one
// sample per server frame after the think, like telemetry rows, tick 1 = frame 1.
//

// |vel_right| after pressing the right strafe key from standing still
Curve StrafeOnset(int K, int change)
{
    Sim   sim;
    Curve curve;
    int   t;

    Spawn(sim, "strafe onset");
    Frames(sim, Keys(0, 0, 0), K, 4);

    Frame(sim, Keys(0, 0, 0), Keys(0, 127, 0), K, change);
    curve.push_back(fabsf(PmVelRight(sim.player)));
    for (t = 1; t < TICKS + 2; t++) {
        Frames(sim, Keys(0, 127, 0), K, 1);
        curve.push_back(fabsf(PmVelRight(sim.player)));
    }
    return curve;
}

// Lateral velocity in the new key's direction after flipping right -> left at full speed
Curve StrafeReversal(int K, int change)
{
    Sim   sim;
    Curve curve;
    int   t;

    Spawn(sim, "strafe reversal");
    Frames(sim, Keys(0, 127, 0), K, 12);
    Check(Near(PmVelRight(sim.player), SMG_STRAFE_SPEED, EPS), "K=%d: held strafe speed %f", K, PmVelRight(sim.player));

    Frame(sim, Keys(0, 127, 0), Keys(0, -127, 0), K, change);
    curve.push_back(-PmVelRight(sim.player));
    for (t = 1; t < TICKS + 2; t++) {
        Frames(sim, Keys(0, -127, 0), K, 1);
        curve.push_back(-PmVelRight(sim.player));
    }
    return curve;
}

// speed_xy after releasing every key from a full-speed strafe
Curve Stop(int K, int change)
{
    Sim   sim;
    Curve curve;
    int   t;

    Spawn(sim, "stop");
    Frames(sim, Keys(0, 127, 0), K, 12);

    Frame(sim, Keys(0, 127, 0), Keys(0, 0, 0), K, change);
    curve.push_back(PmSpeedXY(sim.player));
    for (t = 1; t < TICKS + 6; t++) {
        Frames(sim, Keys(0, 0, 0), K, 1);
        curve.push_back(PmSpeedXY(sim.player));
    }
    return curve;
}

// speed_xy holding `cmd` from standing still
Curve HeldSpeed(int K, const usercmd_t& cmd, int frames, const usercmd_t *firstFrame = NULL)
{
    Sim   sim;
    Curve curve;
    int   t;

    Spawn(sim, "held speed");
    if (firstFrame) {
        Frames(sim, *firstFrame, K, 1);
    }
    for (t = 0; t < frames; t++) {
        Frames(sim, cmd, K, 1);
        curve.push_back(PmSpeedXY(sim.player));
    }
    return curve;
}

//
// Tests
//

void TestWorld()
{
    PmWorld      world;
    trace_t      tr;
    const float  mins[3] = {-512, -512, 0};
    const float  maxs[3] = {512, 512, 256};
    const vec3_t bmins   = {MINS_X, MINS_Y, MINS_Z};
    const vec3_t bmaxs   = {MAXS_X, MAXS_Y, MAXS_Z};
    const vec3_t zero    = {0, 0, 0};
    const int    pillar  = 6; // index after the six room boxes
    vec3_t       start, end;

    world.AddRoom(mins, maxs);
    Check(world.AddPillar(200, 0, 32, 0, 256) == pillar, "world: pillar index");
    Check(world.NumBoxes() == 7, "world: %d boxes", world.NumBoxes());

    // sweep into a wall: stops SURFACE_CLIP_EPSILON short, wall normal, world entity
    VectorSet(start, 0, 300, FLOOR_Z);
    VectorSet(end, 0, 600, FLOOR_Z);
    world.Trace(&tr, start, bmins, bmaxs, end, 0, MASK_PLAYERSOLID, true);
    Check(!tr.startsolid && !tr.allsolid && tr.fraction < 1, "world: wall sweep hit");
    Check(Near(tr.endpos[1], 512 - 15 - 0.125f, 1e-3f), "world: wall sweep stops at y %f", tr.endpos[1]);
    Check(tr.plane.normal[1] == -1 && tr.plane.normal[0] == 0 && tr.plane.normal[2] == 0, "world: wall normal");
    Check(tr.entityNum == ENTITYNUM_WORLD && tr.contents == CONTENTS_SOLID, "world: wall entity %d", tr.entityNum);

    // ground trace from rest: fraction 0 on the floor plane
    VectorSet(start, 0, 0, FLOOR_Z);
    VectorSet(end, 0, 0, FLOOR_Z - 0.25f);
    world.Trace(&tr, start, bmins, bmaxs, end, 0, MASK_PLAYERSOLID, true);
    Check(
        tr.fraction == 0 && !tr.startsolid && tr.plane.normal[2] == 1, "world: ground trace fraction %f", tr.fraction
    );

    // position tests: touching the floor counts as inside, 0.125 above does not
    VectorSet(start, 0, 0, 0);
    world.Trace(&tr, start, bmins, bmaxs, start, 0, MASK_PLAYERSOLID, true);
    Check(tr.startsolid && tr.allsolid && tr.fraction == 0, "world: touching position test");
    VectorSet(start, 0, 0, FLOOR_Z);
    world.Trace(&tr, start, bmins, bmaxs, start, 0, MASK_PLAYERSOLID, true);
    Check(!tr.startsolid && tr.fraction == 1 && tr.entityNum == ENTITYNUM_NONE, "world: free position test");

    // inside the pillar: allsolid; leaving it: startsolid only, the move completes
    VectorSet(start, 200, 0, 64);
    world.Trace(&tr, start, zero, zero, start, 0, MASK_PLAYERSOLID, false);
    Check(tr.allsolid && tr.startsolid, "world: point inside pillar");
    VectorSet(end, 200, 100, 64);
    world.Trace(&tr, start, zero, zero, end, 0, MASK_PLAYERSOLID, false);
    Check(tr.startsolid && !tr.allsolid && tr.fraction == 1, "world: leaving the pillar %f", tr.fraction);

    // zero-extent ray and line of sight around the pillar
    VectorSet(start, 0, 0, 64);
    VectorSet(end, 400, 0, 64);
    world.Trace(&tr, start, zero, zero, end, 0, MASK_PLAYERSOLID, false);
    Check(
        Near(tr.endpos[0], 168 - 0.125f, 1e-3f) && tr.plane.normal[0] == -1, "world: ray stops at x %f", tr.endpos[0]
    );
    Check(!world.LineOfSight(start, end, 0), "world: pillar blocks the sight line");
    VectorSet(end, 400, 100, 64);
    Check(world.LineOfSight(start, end, 0), "world: clear sight line");

    // bodies: players collide, the pass entity is skipped, the target rule of the logger
    {
        const vec3_t other = {100, 0, FLOOR_Z};
        vec3_t       centroid;

        world.SetBody(1, other, bmins, bmaxs);
        VectorSet(start, 0, 0, FLOOR_Z);
        VectorSet(end, 150, 0, FLOOR_Z);
        world.Trace(&tr, start, bmins, bmaxs, end, 0, MASK_PLAYERSOLID, true);
        Check(
            tr.entityNum == 1 && Near(tr.endpos[0], 100 - 30 - 0.125f, 1e-3f),
            "world: body sweep %d x %f",
            tr.entityNum,
            tr.endpos[0]
        );
        world.Trace(&tr, start, bmins, bmaxs, end, 1, MASK_PLAYERSOLID, true);
        Check(tr.fraction == 1, "world: pass entity skipped");

        VectorSet(start, 0, 0, 82);
        VectorSet(centroid, 100, 0, 47);
        Check(!world.LineOfSight(start, centroid, 0), "world: body blocks a ray to its own centroid");
        Check(world.LineOfSight(start, centroid, 0, 1), "world: target body counts as seen");
        Check(world.PointContents(centroid, 0) == CONTENTS_BODY, "world: body contents");
        Check(world.PointContents(centroid, 1) == 0, "world: pass entity contents");
        VectorSet(centroid, 0, 0, -1);
        Check(world.PointContents(centroid) == CONTENTS_SOLID, "world: floor contents");
        world.ClearBody(1);
        Check(!world.HasBody(1), "world: body cleared");
    }
}

// Deterministic pseudo-random numbers (LCG) for the property tests
unsigned int rngState = 0x2468aceu;

unsigned int NextRandom()
{
    rngState = rngState * 1664525u + 1013904223u;
    return rngState >> 8;
}

float RandRange(float lo, float hi)
{
    return lo + (hi - lo) * (float)(NextRandom() & 0xffff) / 65535.0f;
}

int RandInt(int count)
{
    return (int)(NextRandom() % (unsigned int)count);
}

// Random sweeps through random boxes: fractions in [0, 1], hit planes are unit axial
// normals facing the move, and a sweep that did not start in solid never ends in solid
// (position test at endpos, where touching counts as inside).
void TestTraceProperties()
{
    PmWorld     world;
    const float mins[3] = {-512, -512, 0};
    const float maxs[3] = {512, 512, 256};
    // moving boxes whose capsule expansion equals the box (half width <= half height)
    const float extents[][6] = {
        {0,      0,      0,  0,      0,      0            },
        {MINS_X, MINS_Y, 0,  MAXS_X, MAXS_Y, MAXS_Z       },
        {MINS_X, MINS_Y, 0,  MAXS_X, MAXS_Y, CROUCH_MAXS_Z},
        {-4,     -4,     -4, 4,      4,      4            },
    };
    int bad[4] = {0, 0, 0, 0};
    int hits   = 0;
    int i;

    world.AddRoom(mins, maxs);
    for (i = 0; i < 12; i++) {
        // integer faces, so that snapped starts below touch them exactly
        const float cx = roundf(RandRange(-450, 450)), cy = roundf(RandRange(-450, 450)),
                    cz       = roundf(RandRange(0, 200));
        const float bmins[3] = {cx - roundf(RandRange(4, 60)), cy - roundf(RandRange(4, 60)), cz};
        const float bmaxs[3] = {
            cx + roundf(RandRange(4, 60)), cy + roundf(RandRange(4, 60)), cz + roundf(RandRange(2, 90))
        };

        world.AddBox(bmins, bmaxs);
    }

    for (i = 0; i < 20000; i++) {
        const float *e       = extents[RandInt(4)];
        const bool   capsule = RandInt(2) != 0;
        vec3_t       start, end, delta;
        trace_t      tr, pos;

        VectorSet(start, RandRange(-500, 500), RandRange(-500, 500), RandRange(0, 200));
        if (RandInt(2) == 0) {
            // on the 1/8 grid: exactly touching or SURFACE_CLIP_EPSILON away from faces
            VectorSet(start, roundf(start[0] * 8) / 8, roundf(start[1] * 8) / 8, roundf(start[2] * 8) / 8);
        }
        if (RandInt(4) == 0) {
            // short moves, the common Pmove case
            VectorSet(end, start[0] + RandRange(-4, 4), start[1] + RandRange(-4, 4), start[2] + RandRange(-4, 4));
        } else {
            VectorSet(end, RandRange(-500, 500), RandRange(-500, 500), RandRange(0, 200));
        }
        VectorSubtract(end, start, delta);

        world.Trace(&tr, start, e, e + 3, end, ENTITYNUM_NONE, MASK_PLAYERSOLID, capsule);
        if (!(tr.fraction >= 0 && tr.fraction <= 1)) {
            bad[0]++;
        }
        if (tr.startsolid) {
            continue;
        }
        if (tr.fraction < 1) {
            const float *n = tr.plane.normal;

            hits++;
            if (fabsf(n[0]) + fabsf(n[1]) + fabsf(n[2]) != 1 || DotProduct(n, delta) >= 0) {
                bad[1]++;
            }
        }
        world.Trace(&pos, tr.endpos, e, e + 3, tr.endpos, ENTITYNUM_NONE, MASK_PLAYERSOLID, capsule);
        if (pos.startsolid) {
            bad[2]++;
        }
        if (tr.fraction == 1 && !VectorCompare(tr.endpos, end)) {
            bad[3]++;
        }
    }

    printf("trace properties: 20000 sweeps, %d hits\n", hits);
    Check(!bad[0], "trace: %d fractions outside [0, 1]", bad[0]);
    Check(!bad[1], "trace: %d hit planes not unit axial against the move", bad[1]);
    Check(!bad[2], "trace: %d sweeps ended in solid", bad[2]);
    Check(!bad[3], "trace: %d complete sweeps not ending at end", bad[3]);
    Check(hits > 1000, "trace: only %d hits", hits);
}

// Two players with random keys, turns, jumps and crouches among pillars, a step, a
// crate and a low ceiling, K random per frame: never non-finite, never inside solid
// or inside each other, always inside the room.
void TestRandomWalk()
{
    PmWorld     world;
    PmSettings  settings;
    PmPlayer    players[2];
    usercmd_t   cmds[2];
    const float mins[3]        = {-768, -768, 0};
    const float maxs[3]        = {768, 768, 512};
    const float stepMins[3]    = {-300, 200, 0};
    const float stepMaxs[3]    = {-100, 400, 12};
    const float crateMins[3]   = {100, -400, 0};
    const float crateMaxs[3]   = {180, -320, 40};
    const float ceilingMins[3] = {-200, -250, 70};
    const float ceilingMaxs[3] = {0, -50, 90};
    int         time           = START_TIME;
    int         airborne = 0, crouched = 0, stuck = 0, outside = 0, nonFinite = 0;
    int         frame, p, i;

    world.AddRoom(mins, maxs);
    world.AddPillar(0, 0, 32, 0, 512);
    world.AddPillar(300, 300, 24, 0, 512);
    world.AddPillar(-400, -400, 40, 0, 512);
    world.AddBox(stepMins, stepMaxs);
    world.AddBox(crateMins, crateMaxs);
    world.AddBox(ceilingMins, ceilingMaxs);

    for (p = 0; p < 2; p++) {
        const vec3_t origin = {p ? 200.0f : -200.0f, 0, 8};

        PmInitPlayer(players[p], p, origin, p ? 180.0f : 0.0f, time);
        PmDropToFloor(players[p], world, settings);
        PmLinkPlayer(world, players[p]);
        cmds[p] = Keys(0, 0, 0);
    }

    for (frame = 0; frame < 4000; frame++) {
        time += settings.frameMsec;
        for (p = 0; p < 2; p++) {
            PmPlayer&    pl  = players[p];
            usercmd_t&   cmd = cmds[p];
            PmFrameInput input;
            trace_t      tr;
            float        yaw = SHORT2ANGLE(cmd.angles[YAW]);

            input.before = cmd;
            if (RandInt(6) == 0) {
                const int keys[3] = {-127, 0, 127};

                cmd.forwardmove = (signed char)keys[RandInt(3)];
                cmd.rightmove   = (signed char)keys[RandInt(3)];
                cmd.upmove      = RandInt(8) == 0 ? (signed char)keys[RandInt(3)] : 0;
                cmd.buttons     = RandInt(10) ? BUTTON_RUN : 0;
                yaw += RandRange(-60, 60);
            }
            yaw += RandRange(-8, 8);
            cmd.angles[YAW]   = ANGLE2SHORT(yaw);
            input.after       = cmd;
            input.numCmds     = 1 + RandInt(4);
            input.changeIndex = RandInt(input.numCmds + 1);
            PmRunServerFrame(pl, world, settings, time, input);

            for (i = 0; i < 3; i++) {
                if (!std::isfinite(pl.ps.origin[i]) || !std::isfinite(pl.ps.velocity[i])) {
                    nonFinite++;
                }
            }
            world.Trace(&tr, pl.ps.origin, pl.mins, pl.maxs, pl.ps.origin, pl.entityNum, MASK_PLAYERSOLID, true);
            if (tr.startsolid) {
                stuck++;
            }
            if (fabsf(pl.ps.origin[0]) > 768 - 15 || fabsf(pl.ps.origin[1]) > 768 - 15 || pl.ps.origin[2] < 0) {
                outside++;
            }
            airborne += !pl.onGround;
            crouched += pl.crouching && pl.onGround;
        }
    }

    printf(
        "random walk: 2 players x 4000 frames, %d airborne and %d crouched player-frames, %d usercmds\n",
        airborne,
        crouched,
        players[0].numUsercmds + players[1].numUsercmds
    );
    Check(!nonFinite, "random walk: %d non-finite values", nonFinite);
    Check(!stuck, "random walk: %d player-frames inside solid", stuck);
    Check(!outside, "random walk: %d player-frames outside the room", outside);
    Check(airborne > 100 && crouched > 100, "random walk: too few jumps (%d) or crouches (%d)", airborne, crouched);
}

// Pmove keeps working against geometry: slide along a wall, bump another player
void TestCollisions()
{
    Sim          sim;
    const vec3_t other = {160, 0, FLOOR_Z};
    const vec3_t bmins = {MINS_X, MINS_Y, MINS_Z};
    const vec3_t bmaxs = {MAXS_X, MAXS_Y, MAXS_Z};
    int          t;

    // run at 45 degrees into the +y wall of the room: pressed against it and sliding
    // along it, slower than 240 * cos(45) because the push into the wall is wasted
    Spawn(sim, "wall slide");
    sim.player.ps.origin[1] = 4096 - 40;
    for (t = 0; t < 40; t++) {
        Frame(sim, PmMakeUsercmd(0, 45, 127, 0, 0, BUTTON_RUN), PmMakeUsercmd(0, 45, 127, 0, 0, BUTTON_RUN), 4, 0);
    }
    printf("wall slide (K=4, 45 degrees into a wall): %.1f u/s along it\n", sim.player.ps.velocity[0]);
    Check(Near(sim.player.ps.origin[1], 4096 - 15 - 0.125f, 1e-2f), "wall slide: y %f", sim.player.ps.origin[1]);
    Check(
        sim.player.ps.velocity[0] > 100 && sim.player.ps.velocity[0] < SMG_RUN_SPEED * sqrtf(0.5f)
            && fabsf(sim.player.ps.velocity[1]) < 0.5f,
        "wall slide: velocity %f %f",
        sim.player.ps.velocity[0],
        sim.player.ps.velocity[1]
    );

    // run into another player's body: stopped at the bodies' gap, never through it
    Sim bump;
    Spawn(bump, "body bump");
    bump.world.SetBody(1, other, bmins, bmaxs);
    PmLinkPlayer(bump.world, bump.player);
    for (t = 0; t < 20; t++) {
        Frames(bump, Keys(127, 0, 0), 4, 1);
    }
    Check(Near(bump.player.ps.origin[0], 160 - 30 - 0.125f, 0.05f), "body bump: x %f", bump.player.ps.origin[0]);
    Check(
        bump.world.GetBody(0) && bump.world.GetBody(0)->origin[0] == bump.player.ps.origin[0],
        "body bump: own body follows"
    );
}

// PmRunServerFrame is the Frame() loop of this test
void TestServerFrameHelper()
{
    Sim          a, b;
    PmFrameInput input;
    int          t;

    Spawn(a, "helper a");
    Spawn(b, "helper b");
    input.before      = Keys(0, 0, 0);
    input.after       = Keys(127, -127, 0);
    input.numCmds     = 4;
    input.changeIndex = 2;

    for (t = 0; t < 6; t++) {
        a.time += a.settings.frameMsec;
        Check(PmRunServerFrame(a.player, a.world, a.settings, a.time, input) == 4, "helper: 4 usercmds");
        Frame(b, input.before, input.after, input.numCmds, input.changeIndex);
    }
    Check(!memcmp(&a.player.ps, &b.player.ps, sizeof(a.player.ps)), "helper: PmRunServerFrame differs from the loop");

    // sub-step times: strictly increasing, the last one at the frame time
    Check(
        PmSubstepTime(1000, 50, 4, 0) == 962 && PmSubstepTime(1000, 50, 4, 1) == 975
            && PmSubstepTime(1000, 50, 4, 2) == 987 && PmSubstepTime(1000, 50, 4, 3) == 1000,
        "helper: sub-step times"
    );

    // ClientThink drops commands from the future and repeats
    usercmd_t cmd  = Keys(0, 0, 0);
    cmd.serverTime = a.time + 1;
    Check(!PmRunUsercmd(a.player, a.world, a.settings, cmd, a.time), "helper: future usercmd accepted");
    cmd.serverTime = a.time;
    Check(!PmRunUsercmd(a.player, a.world, a.settings, cmd, a.time), "helper: repeated usercmd accepted");
}

// Strafe onset, reversal and stop for K = 1, 2, 4 and every change sub-step
void TestStrafeResponses()
{
    std::vector<Curve> onset[5], reversal[5], stop[5];
    size_t             k;
    int                K, change, t;
    char               label[64];

    for (k = 0; k < sizeof(SUBSTEPS) / sizeof(SUBSTEPS[0]); k++) {
        K = SUBSTEPS[k];
        for (change = 0; change <= K; change++) {
            onset[K].push_back(StrafeOnset(K, change));
            reversal[K].push_back(StrafeReversal(K, change));
            stop[K].push_back(Stop(K, change));
        }
    }

    // Integration sanity for one whole frame: v = accel * dt * wishspeed, friction 1 - 6 dt
    Check(Near(onset[1][0][0], 8 * 0.05f * SMG_STRAFE_SPEED, EPS), "K=1 onset tick 1: %f", onset[1][0][0]);
    Check(Near(stop[1][0][0], SMG_STRAFE_SPEED * (1 - 6 * 0.05f), EPS), "K=1 stop tick 1: %f", stop[1][0][0]);

    for (k = 0; k < sizeof(SUBSTEPS) / sizeof(SUBSTEPS[0]); k++) {
        K = SUBSTEPS[k];
        for (change = 0; change < K; change++) {
            const Curve& on   = onset[K][change];
            const Curve& rev  = reversal[K][change];
            const Curve& st   = stop[K][change];
            const int    full = FirstTick(on, [](float v) { return v >= SMG_STRAFE_SPEED - EPS; });
            const int    sign = FirstTick(rev, [](float v) { return v > 0; });
            const int    halt = FirstTick(st, [](float v) { return v < STOPPED; });

            Check(full >= 4 && full <= 6, "K=%d change %d: full strafe speed at tick %d", K, change, full);
            Check(sign >= 2 && sign <= 3, "K=%d change %d: reversal at tick %d", K, change, sign);
            Check(halt >= 7 && halt <= 9, "K=%d change %d: stop at tick %d", K, change, halt);
        }

        // an earlier key change always gives a response at least as far along
        for (change = 0; change < K; change++) {
            for (t = 0; t < TICKS + 2; t++) {
                Check(onset[K][change][t] >= onset[K][change + 1][t] - EPS, "K=%d onset not ordered by sub-step", K);
                Check(reversal[K][change][t] >= reversal[K][change + 1][t] - EPS, "K=%d reversal not ordered", K);
                Check(stop[K][change][t] <= stop[K][change + 1][t] + EPS, "K=%d stop not ordered", K);
            }
        }
    }

    // A mid-frame change lies between K = 1 with the change at the frame start and at its end
    {
        const struct {
            int K, change;
        } mids[] = {
            {2, 1},
            {4, 2}
        };

        size_t m;

        for (m = 0; m < sizeof(mids) / sizeof(mids[0]); m++) {
            const std::vector<Curve> *sets[3]  = {onset, reversal, stop};
            const char               *names[3] = {"onset", "reversal", "stop"};
            int                       s;

            for (s = 0; s < 3; s++) {
                const Curve& mid   = sets[s][mids[m].K][mids[m].change];
                const Curve& first = sets[s][1][0];
                const Curve& last  = sets[s][1][1];
                float        worst = 0;

                for (t = 0; t < TICKS + 2; t++) {
                    const float lo = std::min(first[t], last[t]);
                    const float hi = std::max(first[t], last[t]);

                    worst = std::max(worst, std::max(lo - mid[t], mid[t] - hi));
                }
                printf(
                    "bracket K=%d change %d %-8s: max excursion outside K=1 [start, end] %.2f u/s\n",
                    mids[m].K,
                    mids[m].change,
                    names[s],
                    std::max(worst, 0.0f)
                );
                Check(
                    worst <= TOL_BRACKET,
                    "K=%d change %d %s leaves the K=1 bracket by %.2f",
                    mids[m].K,
                    mids[m].change,
                    names[s],
                    worst
                );
            }
        }
    }

    // Tables
    printf("\nstrafe responses per 50 ms tick (median over the key-change sub-step, uniform timing)\n");
    for (k = 0; k < sizeof(SUBSTEPS) / sizeof(SUBSTEPS[0]); k++) {
        K = SUBSTEPS[k];
        std::vector<Curve> on(onset[K].begin(), onset[K].end() - 1);
        std::vector<Curve> rev(reversal[K].begin(), reversal[K].end() - 1);
        std::vector<Curve> st(stop[K].begin(), stop[K].end() - 1);

        snprintf(label, sizeof(label), "K=%d onset |vel_right|", K);
        PrintCurve(label, MedianCurve(on));
        snprintf(label, sizeof(label), "K=%d reversal (new dir)", K);
        PrintCurve(label, MedianCurve(rev));
        snprintf(label, sizeof(label), "K=%d stop speed", K);
        PrintCurve(label, MedianCurve(st));
        if (verbose) {
            for (change = 0; change <= K; change++) {
                snprintf(label, sizeof(label), "  onset change %d", change);
                PrintCurve(label, onset[K][change]);
                snprintf(label, sizeof(label), "  reversal change %d", change);
                PrintCurve(label, reversal[K][change]);
                snprintf(label, sizeof(label), "  stop change %d", change);
                PrintCurve(label, stop[K][change]);
            }
        }
    }
    PrintCurve("human onset |vel_right|", Curve(HUMAN_ONSET, HUMAN_ONSET + TICKS));
    PrintCurve("human reversal (new dir)", Curve(HUMAN_REVERSAL, HUMAN_REVERSAL + TICKS));
    PrintCurve("human stop speed", Curve(HUMAN_STOP, HUMAN_STOP + TICKS));

    // K = 4 against the humans: median over the four key-change sub-steps
    {
        const std::vector<Curve> on(onset[4].begin(), onset[4].end() - 1);
        const std::vector<Curve> rev(reversal[4].begin(), reversal[4].end() - 1);
        const std::vector<Curve> st(stop[4].begin(), stop[4].end() - 1);
        const Curve              onMed  = MedianCurve(on);
        const Curve              revMed = MedianCurve(rev);
        const Curve              stMed  = MedianCurve(st);
        std::vector<float>       revTicks, stopTicks;
        size_t                   i;

        for (i = 0; i < rev.size(); i++) {
            revTicks.push_back((float)FirstTick(rev[i], [](float v) { return v > 0; }));
            stopTicks.push_back((float)FirstTick(st[i], [](float v) { return v < STOPPED; }));
        }

        for (t = 0; t < TICKS; t++) {
            Check(
                Near(onMed[t], HUMAN_ONSET[t], TOL_CURVE),
                "K=4 onset tick %d: %.1f vs human %.1f",
                t + 1,
                onMed[t],
                HUMAN_ONSET[t]
            );
            Check(
                Near(revMed[t], HUMAN_REVERSAL[t], TOL_CURVE),
                "K=4 reversal tick %d: %.1f vs human %.1f",
                t + 1,
                revMed[t],
                HUMAN_REVERSAL[t]
            );
            Check(
                Near(stMed[t], HUMAN_STOP[t], TOL_CURVE),
                "K=4 stop tick %d: %.1f vs human %.1f",
                t + 1,
                stMed[t],
                HUMAN_STOP[t]
            );
        }
        printf(
            "K=4 ticks to reversal %.1f (human %.1f), ticks to stop %.1f (human %.1f)\n",
            Median(revTicks),
            HUMAN_REVERSAL_TICKS,
            Median(stopTicks),
            HUMAN_STOP_TICKS
        );
        Check(
            Near(Median(revTicks), HUMAN_REVERSAL_TICKS, TOL_REVERSAL_TICKS),
            "K=4 ticks to reversal %.1f",
            Median(revTicks)
        );
        Check(Near(Median(stopTicks), HUMAN_STOP_TICKS, TOL_STOP_TICKS), "K=4 ticks to stop %.1f", Median(stopTicks));
    }
}

// Held speeds: run, back, strafe, walk, crouch
void TestHeldSpeeds()
{
    const usercmd_t crouchPress = Keys(0, 0, -127);
    size_t          k;

    printf("\nheld speeds after 12 ticks (run, back, strafe, walk, crouch, crouch key held)\n");
    for (k = 0; k < sizeof(SUBSTEPS) / sizeof(SUBSTEPS[0]); k++) {
        const int   K      = SUBSTEPS[k];
        const Curve run    = HeldSpeed(K, Keys(127, 0, 0), 12);
        const Curve back   = HeldSpeed(K, Keys(-127, 0, 0), 12);
        const Curve strafe = HeldSpeed(K, Keys(0, -127, 0), 12);
        const Curve walk   = HeldSpeed(K, Keys(127, 0, 0, 0), 12);
        const Curve crouch = HeldSpeed(K, Keys(127, 0, 0), 12, &crouchPress);
        const Curve ckey   = HeldSpeed(K, Keys(127, 0, -127), 12, &crouchPress);
        const int   reach  = FirstTick(run, [](float v) { return v >= SMG_RUN_SPEED - EPS; });

        printf(
            "  K=%d  %6.1f %6.1f %6.1f %6.1f %6.1f %6.1f   run reaches 240 at tick %d\n",
            K,
            run.back(),
            back.back(),
            strafe.back(),
            walk.back(),
            crouch.back(),
            ckey.back(),
            reach
        );

        Check(Near(run.back(), SMG_RUN_SPEED, EPS), "K=%d run speed %f", K, run.back());
        Check(reach == (K == 1 ? 4 : 5), "K=%d run speed reached at tick %d", K, reach);
        Check(Near(back.back(), SMG_BACK_SPEED, EPS), "K=%d back speed %f", K, back.back());
        Check(Near(strafe.back(), SMG_STRAFE_SPEED, EPS), "K=%d strafe speed %f", K, strafe.back());
        Check(Near(walk.back(), SMG_SLOW_SPEED, EPS) && walk.back() < run.back(), "K=%d walk speed %f", K, walk.back());
        Check(
            Near(crouch.back(), SMG_SLOW_SPEED, EPS) && crouch.back() < run.back(),
            "K=%d crouch speed %f",
            K,
            crouch.back()
        );
        // the upmove key also enters PM_CmdScale: 144 / sqrt(2)
        Check(Near(ckey.back(), SMG_SLOW_SPEED * sqrtf(0.5f), 0.05f), "K=%d crouch-key speed %f", K, ckey.back());

        if (K == 4) {
            Check(
                Near(run.back(), HUMAN_RUN_SPEED, TOL_HELD_SPEED), "K=4 run %f vs human %f", run.back(), HUMAN_RUN_SPEED
            );
            Check(
                Near(walk.back(), HUMAN_WALK_SPEED, TOL_HELD_SPEED),
                "K=4 walk %f vs human %f",
                walk.back(),
                HUMAN_WALK_SPEED
            );
            Check(
                Near(crouch.back(), HUMAN_CROUCH_SPEED, TOL_HELD_SPEED),
                "K=4 crouch %f vs human %f",
                crouch.back(),
                HUMAN_CROUCH_SPEED
            );
            Check(
                Near(ckey.back(), HUMAN_CROUCH_KEY_SPEED, TOL_HELD_SPEED),
                "K=4 crouch key %f vs human %f",
                ckey.back(),
                HUMAN_CROUCH_KEY_SPEED
            );
        }
    }
    printf(
        "  human   %6.1f      -      -  %6.1f %6.1f %6.1f\n",
        HUMAN_RUN_SPEED,
        HUMAN_WALK_SPEED,
        HUMAN_CROUCH_SPEED,
        HUMAN_CROUCH_KEY_SPEED
    );
}

// Crouch toggling: a press ducks (pm_flags 3, bbox 54), another press stands up,
// no standing up under a low ceiling
void TestCrouchToggle()
{
    Sim         sim;
    const float lowMins[3] = {-64, -64, 70};
    const float lowMaxs[3] = {64, 64, 80};

    Spawn(sim, "crouch toggle");
    Frames(sim, Keys(0, 0, -127), 4, 1);
    Check(sim.player.stance == PM_STANCE_CROUCH && sim.player.maxs[2] == CROUCH_MAXS_Z, "crouch: press ducks");
    Frames(sim, Keys(0, 0, 0), 4, 2);
    Check(
        sim.player.ps.pm_flags == (PMF_DUCKED | PMF_VIEW_PRONE) && sim.player.viewheight == (int)CROUCH_VIEWHEIGHT,
        "crouch: flags %d",
        sim.player.ps.pm_flags
    );
    Frames(sim, Keys(0, 0, -127), 4, 1);
    Check(sim.player.stance == PM_STANCE_STAND && sim.player.maxs[2] == MAXS_Z, "crouch: second press stands up");

    Frames(sim, Keys(0, 0, 0), 4, 1);
    Frames(sim, Keys(0, 0, -127), 4, 1);
    sim.world.AddBox(lowMins, lowMaxs);
    Frames(sim, Keys(0, 0, 0), 4, 1);
    Frames(sim, Keys(0, 0, -127), 4, 1);
    Check(sim.player.stance == PM_STANCE_CROUCH, "crouch: stood up under a ceiling");
}

// Jump from standing still: take-off at the think after the press's think
void TestJump()
{
    size_t k;

    printf("\njump (z above the floor, vz) per tick after the take-off think\n");
    for (k = 0; k < sizeof(SUBSTEPS) / sizeof(SUBSTEPS[0]); k++) {
        const int K = SUBSTEPS[k];
        Sim       sim;
        float     apex   = 0;
        int       landed = 0;
        int       t;

        Spawn(sim, "jump", false);
        Frames(sim, Keys(0, 0, 127), K, 1); // the think sees the press: jump start
        Check(
            sim.player.stance == PM_STANCE_JUMP_START && sim.player.viewheight == (int)JUMP_START_VIEWHEIGHT,
            "K=%d jump start",
            K
        );

        Frames(sim, Keys(0, 0, 0), K, 1); // usercmds with PMF_VIEW_JUMP_START, then the impulse
        Check(sim.player.ps.pm_flags == PMF_VIEW_JUMP_START, "K=%d take-off flags %d", K, sim.player.ps.pm_flags);
        Check(Near(sim.player.ps.velocity[2], JUMP_SPEED, EPS), "K=%d impulse %f", K, sim.player.ps.velocity[2]);
        Check(Near(sim.player.ps.velocity[2], HUMAN_JUMP_TAKEOFF_VZ, 0.05f), "K=%d impulse vs human", K);

        Frames(sim, Keys(0, 0, 0), K, 1);
        const float z1  = sim.player.ps.origin[2] - FLOOR_Z;
        const float vz1 = sim.player.ps.velocity[2];
        printf("  K=%d  first airborne tick z %.2f vz %.1f", K, z1, vz1);
        Check(!sim.player.onGround, "K=%d still on the ground after take-off", K);
        Check(
            Near(vz1, HUMAN_JUMP_FIRST_VZ, TOL_JUMP_FIRST_VZ),
            "K=%d first airborne vz %.1f vs human %.1f",
            K,
            vz1,
            HUMAN_JUMP_FIRST_VZ
        );
        Check(
            Near(z1, HUMAN_JUMP_FIRST_RISE, TOL_JUMP_RISE),
            "K=%d first rise %.2f vs human %.2f",
            K,
            z1,
            HUMAN_JUMP_FIRST_RISE
        );
        apex = z1;

        for (t = 2; t < 30 && !landed; t++) {
            Frames(sim, Keys(0, 0, 0), K, 1);
            apex = std::max(apex, sim.player.ps.origin[2] - FLOOR_Z);
            if (t == 3) {
                Check(
                    sim.player.stance == PM_STANCE_TUCKED && sim.player.maxs[2] == CROUCH_MAXS_Z, "K=%d mid-air tuck", K
                );
            }
            if (sim.player.onGround) {
                landed = t;
            }
        }
        printf(", apex %.2f, lands at tick %d\n", apex, landed);

        // ballistic: the sampled apex is at most v0^2 / 2g, and from the first airborne
        // tick on the height is z1 + vz1^2 / 2g at most
        Check(
            apex <= JUMP_SPEED * JUMP_SPEED / 1600 + EPS && apex >= z1 + vz1 * vz1 / 1600 - 0.5f,
            "K=%d apex %.2f",
            K,
            apex
        );
        Check(
            apex >= HUMAN_JUMP_APEX_Q50 && apex <= HUMAN_JUMP_APEX_Q90 + 1,
            "K=%d apex %.2f outside the human [q50, q90 + 1]",
            K,
            apex
        );
        Check(landed == 15, "K=%d landed at tick %d", K, landed);
        Check(
            Near(sim.player.ps.origin[2], FLOOR_Z, 1e-3f) && sim.player.stance == PM_STANCE_STAND,
            "K=%d landing z %f",
            K,
            sim.player.ps.origin[2]
        );
    }
    printf(
        "  human first airborne tick z %.2f vz %.1f, apex q50 %.1f q90 %.1f\n",
        HUMAN_JUMP_FIRST_RISE,
        HUMAN_JUMP_FIRST_VZ,
        HUMAN_JUMP_APEX_Q50,
        HUMAN_JUMP_APEX_Q90
    );
}

} // namespace

int main(int argc, char **argv)
{
    int i;

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-v")) {
            verbose = true;
        }
    }

    TestWorld();
    TestTraceProperties();
    TestRandomWalk();
    TestCollisions();
    TestServerFrameHelper();
    TestStrafeResponses();
    TestHeldSpeeds();
    TestCrouchToggle();
    TestJump();

    printf("\n%d checks, %d failed\n", checks, failures);
    return failures ? 1 : 0;
}
