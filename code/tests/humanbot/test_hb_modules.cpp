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
// test_hb_modules.cpp: statistical behaviour of the movement, trigger and view
// modules in fixed situations, the sub-step schedule and the eye math.

#include "hb_bundle.h"
#include "hb_eye_math.h"
#include "hb_movement.h"
#include "hb_perception_model.h"
#include "hb_substep.h"
#include "hb_test.h"
#include "hb_trigger.h"
#include "hb_weapon.h"
#include "hb_view.h"

#include <algorithm>
#include <cmath>
#include <vector>

// The two key processes in a steady LOS firefight reproduce strafe holds of
// about 300 ms, mostly direct reversals and the human chord shares.
static void TestMovement(const hb::ModelBundle& b)
{
    hb::Mover mv;
    mv.Init(&b.shared.movement);
    hb::StyleOffsets style;
    hb::MoveInput    in;
    in.ctx          = hb::CTX_LOS_FIRE;
    in.enemyKnown   = true;
    in.enemyBearing = 0.0f;
    in.enemyDist    = 350.0f;
    hb::Rng          rm(1), rs(2);
    hb::MoveOutput   out;
    const int        n = 400000;
    int              counts[hb::NUM_CHORDS] = {};
    std::vector<int> holds;
    int              lastSide = 0, runLen = 0, ends = 0, reversals = 0;
    for (int i = 0; i < n; i++) {
        mv.Step(in, style, rm, rs, out);
        counts[out.chord]++;
        const int side = hb::ChordSide(out.chord);
        if (side == lastSide) {
            runLen++;
        } else {
            if (lastSide != 0) {
                holds.push_back(runLen * 50);
                ends++;
                reversals += side == -lastSide;
            }
            lastSide = side;
            runLen   = 1;
        }
    }
    std::sort(holds.begin(), holds.end());
    const double med = holds[holds.size() / 2];
    const double rev = reversals / double(ends);
    const double pure = (counts[3] + counts[5]) / double(n);
    const double diag = (counts[6] + counts[8]) / double(n);
    HB_REPORT("LOS fight: side hold median %.0f ms, direct reverse %.2f, pure strafe %.2f, fwd-diag %.2f", med, rev, pure, diag);
    // pooled human targets (REPORT section 7): holds ~300 ms, 72% direct reversal, 58% pure strafe, 31% diagonal.
    // This open loop never leaves the fight context, so it strafes more than the closed loop, whose
    // shares calibrate.py matches to the humans (hb_arena: 61% pure strafe, 26% diagonal; people 58% and 31%).
    // Since the fit on all five people (09-28 added) this open loop presses the diagonals 14% of the time.
    HB_CHECK(med >= 200.0 && med <= 400.0);
    HB_CHECK(rev > 0.55 && rev < 0.9);
    HB_CHECK(pure > 0.4 && pure < 0.8);
    HB_CHECK(diag > 0.1 && diag < 0.45);

    // style dials move the realised shares the right way
    hb::StyleOffsets diagStyle;
    diagStyle.diagLogit = 1.5f;
    hb::Mover mv2;
    mv2.Init(&b.shared.movement);
    int diag2 = 0;
    for (int i = 0; i < 100000; i++) {
        mv2.Step(in, diagStyle, rm, rs, out);
        diag2 += out.chord == 6 || out.chord == 8;
    }
    HB_CHECK(diag2 / 100000.0 > diag + 0.05);
    // a low diagonal habit presses forward less while strafing, not back more (as a tilt of the forward key's
    // choice it made such styles back off twice as often as people of those styles: back key 2.25x the pooled bot's).
    // Less forward leaves more time for the fitted presses from none, back among them: 1.3x with the tuning of
    // 2026-10-02, 1.5x since the forward habit was recalibrated with the strafe slide at walls (2026-10-03)
    hb::StyleOffsets lowDiag;
    lowDiag.diagLogit = -2.7f;
    hb::Mover mvBase, mvLow;
    mvBase.Init(&b.shared.movement);
    mvLow.Init(&b.shared.movement);
    int backBase = 0, backLow = 0, diagLow = 0;
    for (int i = 0; i < 100000; i++) {
        mvBase.Step(in, style, rm, rs, out);
        backBase += hb::ChordFwd(out.chord) == -1;
        mvLow.Step(in, lowDiag, rm, rs, out);
        backLow += hb::ChordFwd(out.chord) == -1;
        diagLow += out.chord == 6 || out.chord == 8;
    }
    HB_REPORT("low diagonal habit: diagonal %.3f (pooled %.3f), back key %.3f (pooled %.3f)", diagLow / 100000.0, diag,
              backLow / 100000.0, backBase / 100000.0);
    HB_CHECK(diagLow / 100000.0 < diag - 0.03);
    HB_CHECK(backLow < backBase * 1.6 + 200);

    // people keep pressing toward walls they touch, so the fitted model vetoes none; with a
    // veto clearance set, a key is never pressed toward a wall that close
    hb::MovementModel vetoed = b.shared.movement;
    vetoed.vetoClearance     = 16.0f;
    hb::MoveInput wall       = in;
    wall.clearance[5]        = 4.0f;   // right
    wall.clearance[8]        = 4.0f;   // forward-right
    hb::Mover mv3;
    mv3.Init(&vetoed);
    int into = 0, sideMoves = 0;
    for (int i = 0; i < 20000; i++) {
        mv3.Step(wall, style, rm, rs, out);
        into += out.chord == 5 || out.chord == 8;
        sideMoves += hb::ChordSide(out.chord) == -1;
    }
    HB_CHECK(into == 0);
    HB_CHECK(sideMoves > 2000);   // it still strafes the other way
    // without a veto, the fitted wall terms still tilt new keys away from a wall that close (people strafe into a
    // touching wall about half as often as into open space). Since a strafe after a stop goes the other way (95-98%
    // after one tick of none, the side's memory) the tilt is small: a wall the bot touches is what the reflex vetoes
    hb::Mover mvOpen, mvWall, mvTouch;
    mvOpen.Init(&b.shared.movement);
    mvWall.Init(&b.shared.movement);
    mvTouch.Init(&b.shared.movement);
    hb::MoveInput touch = wall;
    touch.clearance[5]  = 1.0f;
    touch.clearance[8]  = 1.0f;
    int openRight = 0, openLeft = 0, wallRight = 0, wallLeft = 0, touchRight = 0;
    for (int i = 0; i < 200000; i++) {
        mvOpen.Step(in, style, rm, rs, out);
        openRight += hb::ChordSide(out.chord) == 1;
        openLeft += hb::ChordSide(out.chord) == -1;
        mvWall.Step(wall, style, rm, rs, out);
        wallRight += hb::ChordSide(out.chord) == 1;
        wallLeft += hb::ChordSide(out.chord) == -1;
        mvTouch.Step(touch, style, rm, rs, out);
        touchRight += hb::ChordSide(out.chord) == 1;
    }
    const double openShare = openRight / double(std::max(1, openRight + openLeft));
    const double wallShare = wallRight / double(std::max(1, wallRight + wallLeft));
    HB_REPORT("strafe right: %.2f of strafe time in the open, %.2f with a wall 4 u to the right, %d ticks touching it",
              openShare, wallShare, touchRight);
    HB_CHECK(wallShare < openShare - 0.02);
    HB_CHECK(touchRight == 0);
    // the wall reflex: a strafe toward a wall reached within the reflex time is let go of at once
    hb::MovementModel reflex = b.shared.movement;
    reflex.wallReflexMs      = 200.0f;
    reflex.wallReflexLogit   = 4.0f;
    hb::MovementModel noReflex = b.shared.movement;
    noReflex.wallReflexMs      = 0.0f;
    hb::MoveInput coming     = in;
    coming.clearance[5]      = 30.0f;   // right: 30 u away ...
    coming.clearance[2]      = 30.0f;
    coming.clearance[8]      = 30.0f;
    coming.velRight          = 240.0f;  // ... reached in 125 ms
    int heldOn = 0, heldOff = 0;
    for (int i = 0; i < 2000; i++) {
        hb::Mover on, off;
        on.Init(&reflex);
        off.Init(&noReflex);
        on.SetKeys(0, 1);
        off.SetKeys(0, 1);
        on.Step(coming, style, rm, rs, out);
        heldOn += hb::ChordSide(out.chord) == 1;
        off.Step(coming, style, rm, rs, out);
        heldOff += hb::ChordSide(out.chord) == 1;
    }
    HB_REPORT("strafe held one more tick toward a wall 125 ms away: %.2f with the reflex, %.2f without", heldOn / 2000.0,
              heldOff / 2000.0);
    HB_CHECK(heldOn < heldOff - 500);
    // only the direction the bot goes counts: a diagonal down a corridor, its forward and strafe directions facing
    // the walls 40 u away but the diagonal open, keeps both keys (people walk corridors so)
    hb::MoveInput corridor = in;
    corridor.ctx           = hb::CTX_HIDDEN_NOFIRE;
    corridor.clearance[7]  = 40.0f;   // forward
    corridor.clearance[3]  = 40.0f;   // left
    corridor.velFwd        = 170.0f;  // forward-left at 240 u/s
    corridor.velRight      = -170.0f;
    hb::MoveInput open = corridor;
    open.clearance[7]  = 128.0f;
    open.clearance[3]  = 128.0f;
    int keptCorridor = 0, keptOpen = 0;
    for (int i = 0; i < 2000; i++) {
        hb::Mover a, o;
        a.Init(&reflex);
        o.Init(&reflex);
        a.SetKeys(1, -1);
        o.SetKeys(1, -1);
        a.Step(corridor, style, rm, rs, out);
        keptCorridor += out.chord == 6;
        o.Step(open, style, rm, rs, out);
        keptOpen += out.chord == 6;
    }
    HB_REPORT("diagonal kept one more tick down a corridor: %.2f (in the open %.2f)", keptCorridor / 2000.0, keptOpen / 2000.0);
    HB_CHECK(keptCorridor > keptOpen - 100);
    // forward into a wall with the front-left diagonal open: the bot slides along it (adds the strafe) and keeps
    // forward, where people turn forward into a diagonal; with no diagonal open it lets go
    hb::MoveInput ahead = corridor;
    ahead.clearance[7]  = 20.0f;   // forward: reached in 83 ms
    ahead.clearance[8]  = 20.0f;   // forward-right blocked too
    ahead.clearance[3]  = 128.0f;
    ahead.clearance[6]  = 128.0f;  // forward-left open
    ahead.velFwd        = 240.0f;
    ahead.velRight      = 0.0f;
    hb::MoveInput boxed = ahead;
    boxed.clearance[6]  = 20.0f;
    int slid = 0, stopped = 0, boxedFwd = 0;
    for (int i = 0; i < 2000; i++) {
        hb::Mover a, x;
        a.Init(&reflex);
        x.Init(&reflex);
        a.SetKeys(1, 0);
        x.SetKeys(1, 0);
        a.Step(ahead, style, rm, rs, out);
        slid += out.chord == 6;
        stopped += hb::ChordFwd(out.chord) != 1;
        x.Step(boxed, style, rm, rs, out);
        boxedFwd += hb::ChordFwd(out.chord) == 1;
    }
    HB_REPORT("forward into a wall: slid %.2f, let go %.2f; with no diagonal open forward kept %.2f", slid / 2000.0,
              stopped / 2000.0, boxedFwd / 2000.0);
    HB_CHECK(slid > 1000);
    HB_CHECK(stopped < 400);
    HB_CHECK(boxedFwd < 1000);
    // a strafe into a wall with the front diagonal on its side open: the bot adds forward and runs along the wall
    // (people add forward there more often than the bots stopped), where with that diagonal blocked it lets go
    hb::MoveInput side = coming;
    side.clearance[8]  = 128.0f;   // forward-right open
    side.clearance[7]  = 128.0f;
    int sideSlid = 0, sideOff = 0, comingSlid = 0;
    for (int i = 0; i < 4000; i++) {
        hb::Mover a, o, x;
        a.Init(&reflex);
        o.Init(&noReflex);
        x.Init(&reflex);
        a.SetKeys(0, 1);
        o.SetKeys(0, 1);
        x.SetKeys(0, 1);
        a.Step(side, style, rm, rs, out);
        sideSlid += out.chord == 8;
        o.Step(side, style, rm, rs, out);
        sideOff += out.chord == 8;
        x.Step(coming, style, rm, rs, out);
        comingSlid += out.chord == 8;
    }
    HB_REPORT("strafe into a wall: forward added %.3f with the front diagonal open (%.3f without the reflex), %.3f with it blocked",
              sideSlid / 4000.0, sideOff / 4000.0, comingSlid / 4000.0);
    HB_CHECK(sideSlid > 4 * sideOff + 100);
    HB_CHECK(comingSlid * 4 < sideSlid);
    // every probe blocked (the box starts in solid): the reflex stands down and the bot still moves
    hb::MoveInput blind = in;
    for (int c = 0; c < hb::NUM_CHORDS; c++) {
        blind.clearance[c] = 0.0f;
    }
    hb::Mover mvBlind;
    mvBlind.Init(&reflex);
    int moving = 0;
    for (int i = 0; i < 20000; i++) {
        mvBlind.Step(blind, style, rm, rs, out);
        moving += out.chord != hb::CHORD_NEUTRAL;
    }
    HB_CHECK(moving > 10000);
    // a ledge deep enough to hurt is let go of at once, even when the key was already held
    hb::MoveInput ledge = in;
    ledge.drop[7]       = 400.0f;  // forward
    ledge.drop[6]       = 400.0f;
    ledge.drop[8]       = 400.0f;
    hb::Mover mv5;
    mv5.Init(&b.shared.movement);
    mv5.SetKeys(1, 0);
    int overLedge = 0;
    for (int i = 0; i < 20000; i++) {
        mv5.Step(ledge, style, rm, rs, out);
        overLedge += hb::ChordFwd(out.chord) == 1;
    }
    HB_CHECK(overLedge == 0);
    // lean is held most of a firefight and mostly agrees with the strafe side
    hb::Mover mv4;
    mv4.Init(&b.shared.movement);
    int lean = 0, agree = 0, both = 0;
    for (int i = 0; i < 200000; i++) {
        mv4.Step(in, style, rm, rs, out);
        lean += out.lean != 0;
        const int side = hb::ChordSide(out.chord);
        if (out.lean != 0 && side != 0) {
            both++;
            agree += out.lean == side;
        }
    }
    HB_REPORT("LOS fight: lean held %.2f, lean/strafe agreement %.2f", lean / 200000.0, agree / double(both));
    HB_CHECK(lean / 200000.0 > 0.5);
    HB_CHECK(agree / double(both) > 0.6);
}

// Crouch toggles the stance in this engine: the bot must stand up again, in short dips
// (people crouch 3.5% of the time, dips of about 350 ms). The engine side of the toggle
// is played here: each press edge flips the stance.
static void TestStance(const hb::ModelBundle& b)
{
    hb::Mover mv;
    mv.Init(&b.shared.movement);
    hb::StyleOffsets style;
    hb::MoveInput    in;
    in.ctx      = hb::CTX_HIDDEN_NOFIRE;
    in.onGround = true;
    hb::Rng          rm(11), rs(12);
    hb::MoveOutput   out;
    bool             ducked = false, prevKey = false;
    int              duckTicks = 0, run = 0;
    std::vector<int> dips;
    const int        n = 400000;
    for (int i = 0; i < n; i++) {
        in.ducked = ducked;
        mv.Step(in, style, rm, rs, out);
        if (out.crouch && !prevKey) {
            if (ducked) {
                dips.push_back(run * 50);
                run = 0;
            }
            ducked = !ducked;
        }
        prevKey = out.crouch;
        duckTicks += ducked;
        run += ducked;
    }
    std::sort(dips.begin(), dips.end());
    const double share = duckTicks / double(n);
    const double med   = dips.empty() ? 0.0 : dips[dips.size() / 2];
    HB_REPORT("crouch toggle: crouched %.3f of the time, %zu dips, median %.0f ms", share, dips.size(), med);
    HB_CHECK(share > 0.005 && share < 0.12);
    HB_CHECK(dips.size() > 100);
    HB_CHECK(med >= 150.0 && med <= 800.0);

    // lean with no living enemy (after a kill) is far rarer than while hunting one
    auto leanShare = [&](bool dead) {
        hb::Mover m;
        m.Init(&b.shared.movement);
        hb::MoveInput li = in;
        li.enemyDead     = dead;
        hb::Rng r1(21), r2(22);
        int     lean = 0;
        for (int i = 0; i < 200000; i++) {
            m.Step(li, style, r1, r2, out);
            lean += out.lean != 0;
        }
        return lean / 200000.0;
    };
    const double hunting = leanShare(false), deadOpp = leanShare(true);
    HB_REPORT("lean hidden: %.2f with a living enemy, %.2f with none", hunting, deadOpp);
    HB_CHECK(deadOpp < hunting - 0.1);

    // people crouch three times as readily in the half second after a shot is heard
    auto dipsPerMin = [&](bool fire) {
        hb::Mover m;
        m.Init(&b.shared.movement);
        hb::MoveInput ci = in;
        ci.fireHeard     = fire;
        hb::Rng r1(31), r2(32);
        bool    d = false, pk = false;
        int     dipsN = 0;
        for (int i = 0; i < 200000; i++) {
            ci.ducked = d;
            m.Step(ci, style, r1, r2, out);
            if (out.crouch && !pk) {
                dipsN += !d;
                d = !d;
            }
            pk = out.crouch;
        }
        return dipsN / (200000.0 / 1200.0);
    };
    const double quiet = dipsPerMin(false), fired = dipsPerMin(true);
    HB_REPORT("crouch dips a minute hidden: %.1f quiet, %.1f after a shot", quiet, fired);
    HB_CHECK(fired > 2.0 * quiet);
}

// After a strafe is let go, the next strafe goes the other way (people: 95-98% after one tick of none).
static void TestStrafeMemory(const hb::ModelBundle& b)
{
    hb::Mover mv;
    mv.Init(&b.shared.movement);
    hb::StyleOffsets style;
    hb::MoveInput    in;
    in.ctx          = hb::CTX_LOS_FIRE;
    in.enemyKnown   = true;
    in.enemyDist    = 350.0f;
    hb::Rng        rm(51), rs(52);
    hb::MoveOutput out;
    int            last = 0, prevSide = 0, gap = 0, quickN = 0, quickOpp = 0;
    for (int i = 0; i < 400000; i++) {
        mv.Step(in, style, rm, rs, out);
        const int side = hb::ChordSide(out.chord);
        if (side == 0) {
            gap += prevSide == 0;
        } else if (prevSide == 0 && last != 0) {
            if (gap <= 1) {
                quickN++;
                quickOpp += side == -last;
            }
        }
        if (side != 0) {
            last = side;
            gap  = 0;
        }
        prevSide = side;
    }
    const double opp = quickOpp / double(std::max(quickN, 1));
    HB_REPORT("a strafe pressed right after a stop goes the other way %.2f (%d)", opp, quickN);
    HB_CHECK(quickN > 100 && opp > 0.85);
}

// People lean away from a flat wall at their side and around an edge just ahead on one side.
static void TestLeanWall(const hb::ModelBundle& b)
{
    hb::MovementModel mm = b.shared.movement;
    mm.leanWallLogit     = -1.0f;
    mm.leanEdgeLogit     = 2.0f;
    hb::StyleOffsets style;
    auto intoWall = [&](bool edge) {
        hb::Mover m;
        m.Init(&mm);
        hb::MoveInput in;
        in.ctx        = hb::CTX_LOS_FIRE;
        in.enemyKnown = true;
        in.enemyDist  = 350.0f;
        in.onGround   = true;
        in.clearance[hb::MakeChord(0, 1)] = 10.0f;   // a wall at the right
        in.clearance[hb::MakeChord(1, 1)] = edge ? 128.0f : 20.0f;
        hb::Rng        r1(41), r2(42);
        hb::MoveOutput out;
        int            into = 0, leaned = 0;
        for (int i = 0; i < 200000; i++) {
            m.Step(in, style, r1, r2, out);
            leaned += out.lean != 0;
            into += out.lean == 1;
        }
        return into / double(std::max(leaned, 1));
    };
    const double flat = intoWall(false), edge = intoWall(true);
    mm.leanWallLogit = mm.leanEdgeLogit = 0.0f;
    const double flat0 = intoWall(false);
    HB_REPORT("leans into a wall at the right: flat %.2f (no wall term %.2f), with an edge ahead %.2f", flat, flat0, edge);
    HB_CHECK(flat < flat0 - 0.1);
    HB_CHECK(edge > flat0 + 0.1);
}

// Leaning, with the strafe turned against the lean: the style sets how readily the lean follows it across and how
// readily it is let go of (the presser people flip it with the strafe, the strafers let go of it, the stoppers mostly
// keep it); the lean habit no longer acts there.
static void TestLeanSwitch(const hb::ModelBundle& b)
{
    auto rates = [&](float switchLogit, float dropLogit, float leanLogit, double& release) {
        hb::Mover m;
        m.Init(&b.shared.movement);
        hb::StyleOffsets style;
        style.leanSwitchLogit = switchLogit;
        style.leanDropLogit   = dropLogit;
        style.leanLogit       = leanLogit;
        hb::MoveInput in;
        in.ctx        = hb::CTX_LOS_FIRE;
        in.enemyKnown = true;
        in.enemyDist  = 350.0f;
        in.onGround   = true;
        hb::Rng        r1(61), r2(62);
        hb::MoveOutput out;
        int            prevLean = 0, against = 0, switched = 0, let = 0;
        for (int i = 0; i < 400000; i++) {
            m.Step(in, style, r1, r2, out);
            if (prevLean != 0 && hb::ChordSide(out.chord) == -prevLean) {
                against++;
                switched += out.lean == -prevLean;
                let += out.lean == 0;
            }
            prevLean = out.lean;
        }
        release = let / double(std::max(against, 1));
        return switched / double(std::max(against, 1));
    };
    double       rel0, relP, relS, relD, relH;
    const double neutral = rates(0.0f, 0.0f, 0.0f, rel0), presser = rates(2.0f, 0.0f, 0.0f, relP);
    const double strafer = rates(-5.0f, 0.0f, 0.0f, relS), habit = rates(0.0f, 0.0f, 2.0f, relH);
    rates(0.0f, 1.5f, 0.0f, relD);
    HB_REPORT("strafe turned against the lean, next tick: switch %.3f neutral, %.3f at +2, %.3f at -5, %.3f with a +2 lean "
              "habit; let go %.3f neutral, %.3f with a +1.5 let-go, %.3f with a +2 lean habit",
              neutral, presser, strafer, habit, rel0, relD, relH);
    HB_CHECK(presser > 1.8 * neutral);
    HB_CHECK(strafer < 0.1 * neutral);
    HB_CHECK(relD > 2.0 * rel0);
    // the habit holds a lean on elsewhere, not against the strafe
    HB_CHECK(std::fabs(habit - neutral) < 0.25 * neutral);
    HB_CHECK(std::fabs(relH - rel0) < 0.25 * rel0);
}

static void TestTrigger(const hb::ModelBundle& b)
{
    hb::Trigger tr;
    tr.Init(&b.shared.trigger);
    hb::TriggerInput in;
    in.canFire       = true;
    in.los           = true;
    in.lageMs        = 200;
    in.errHalfWidths = 1.0f;
    const float pNear = tr.PressProb(in, 10);
    in.errHalfWidths  = 15.0f;
    const float pFar  = tr.PressProb(in, 10);
    HB_CHECK(pNear > 0.2f && pNear < 0.5f);   // 25-38% per tick with the target near the crosshair
    HB_CHECK(pFar < 0.05f);
    in.errHalfWidths = 1.0f;
    in.lageMs        = 0;
    const float pFirst = tr.PressProb(in, 10);
    HB_CHECK(pFirst < pNear);                  // reaction: the first tick is less likely than 200 ms in
    // release: sprays are committed while on target, and let go of far off target
    // (11-13% per tick beyond 10 half-widths in the recordings, from about a hundred ticks)
    const float relOn = tr.ReleaseProb(in, 4);
    HB_CHECK(relOn < 0.06f);
    in.errHalfWidths = 15.0f;
    HB_CHECK(tr.ReleaseProb(in, 4) > 0.06f && tr.ReleaseProb(in, 4) > 3.0f * relOn);
    // bursts in a steady on-target fight: median of a few shots, heavy tail
    in.errHalfWidths = 1.0f;
    in.lageMs        = 600;
    hb::Rng          r(5);
    std::vector<int> bursts;
    int              run = 0;
    for (int i = 0; i < 200000; i++) {
        const bool a = tr.Step(in, r);
        if (a) {
            run++;
        } else if (run) {
            bursts.push_back(run);
            run = 0;
        }
    }
    std::sort(bursts.begin(), bursts.end());
    HB_REPORT("on-target holds: median %d ticks, p90 %d ticks", bursts[bursts.size() / 2], bursts[bursts.size() * 9 / 10]);
    HB_CHECK(bursts[bursts.size() / 2] >= 3);
    // cannot fire: the button is released at once
    in.canFire = false;
    HB_CHECK(!tr.Step(in, r));
    // hidden: firing into cover only where the enemy is believed to be
    hb::TriggerInput h;
    h.canFire      = true;
    h.los          = false;
    h.lageMs       = 800;
    h.hiddenYawErr = 3.0f;
    const float pAligned = tr.PressProb(h, 20);
    h.hiddenYawErr       = 120.0f;
    HB_CHECK(pAligned > 4.0f * tr.PressProb(h, 20));
    // the style's burst length shifts the release in sight only: fire into cover is let go of as people let go of it
    // (shifted too, the bots held it 2.3x as long as people and ran out of ammunition)
    h.hiddenYawErr        = 3.0f;
    const float relHidden = tr.ReleaseProb(h, 4);
    h.releaseLogit        = -1.5f;
    HB_CHECK(tr.ReleaseProb(h, 4) == relHidden);
    in.releaseLogit      = 0.0f;
    const float relSight = tr.ReleaseProb(in, 4);
    in.releaseLogit      = -1.5f;
    HB_CHECK(tr.ReleaseProb(in, 4) < 0.5f * relSight);
    // fire into cover fades with the time since sight (people lose track of the enemy), unless the enemy is
    // expected in the crosshair right now
    HB_CHECK(hb::HiddenLateRamp(0) == 0.0f && hb::HiddenLateRamp(500) == 0.0f);
    HB_CHECK(hb::HiddenLateRamp(2000) > 0.4f && hb::HiddenLateRamp(2000) < 0.6f);
    HB_CHECK(hb::HiddenLateRamp(8000) == 1.0f && hb::HiddenLateRamp(100000) == 1.0f);
    hb::TriggerModel plain = b.shared.trigger, faded = b.shared.trigger;
    plain.hiddenLateLogit  = 0.0f;
    faded.hiddenLateLogit  = -2.0f;
    hb::Trigger tp, tf;
    tp.Init(&plain);
    tf.Init(&faded);
    h.hiddenYawErr = 3.0f;
    for (const int ms : {300, 10000}) {
        h.lageMs     = ms;
        h.anticipate = false;
        if (ms < 500) {
            HB_CHECK(tf.PressProb(h, 20) == tp.PressProb(h, 20));
        } else {
            HB_CHECK(tf.PressProb(h, 20) < 0.3f * tp.PressProb(h, 20));
        }
        h.anticipate = true;
        HB_CHECK(tf.PressProb(h, 20) == tp.PressProb(h, 20));
    }
}

// Running out: the pistol (even an empty one: it bashes), back to the primary when it has rounds again, and a weapon
// drawn when nothing is in hand (never on a ladder, which puts it away).
static void TestWeapon(const hb::ModelBundle& b)
{
    hb::WeaponLogic w;
    w.Init(&b.shared.weapon);
    hb::Rng       r(3);
    hb::SelfState s;
    s.alive       = true;
    s.timeMs      = 10000;
    s.hasPistol   = true;
    s.weaponState = 0;
    auto run = [&](int ticks, bool detected) {
        int cmd = hb::CMD_NONE;
        for (int t = 0; t < ticks && cmd == hb::CMD_NONE; t++) {
            s.timeMs += 50;
            cmd = w.Step(s, false, true, detected, false, detected ? 0 : 5000, r);
        }
        return cmd;
    };
    // nothing in hand: after a second the primary, or the pistol when the primary is empty; never on a ladder
    s.weaponClass = hb::WEAPON_CLASS_NONE;
    s.primaryAmmo = 200;
    s.pistolAmmo  = 7;
    s.onLadder    = true;
    HB_CHECK(run(60, false) == hb::CMD_NONE);
    s.onLadder = false;
    HB_CHECK(run(10, false) == hb::CMD_NONE);   // not at once: a respawn passes through no weapon too
    HB_CHECK(run(30, false) == hb::CMD_PRIMARY);
    w.Reset();
    s.primaryAmmo = 0;
    s.pistolAmmo  = 0;
    HB_CHECK(run(40, false) == hb::CMD_PISTOL);
    // the primary is out of rounds and so is the pistol: out of ammunition, draw the pistol anyway
    w.Reset();
    s.weaponClass = hb::WEAPON_CLASS_SMG;
    s.clipAmmo    = 0;
    s.reserveAmmo = 0;
    HB_CHECK(hb::OutOfAmmo(s));
    HB_CHECK(run(1, true) == hb::CMD_PISTOL);
    // the empty pistol in hand and rounds for the primary again: back to it, fight or not
    w.Reset();
    s.weaponClass = hb::WEAPON_CLASS_PISTOL;
    s.primaryAmmo = 60;
    HB_CHECK(!hb::OutOfAmmo(s));
    HB_CHECK(run(1, true) == hb::CMD_PRIMARY);
    // a loaded pistol stays in a fight, and goes back to the primary out of it
    w.Reset();
    s.clipAmmo    = 5;
    s.reserveAmmo = 20;
    s.pistolAmmo  = 25;
    HB_CHECK(run(20, true) == hb::CMD_NONE);
    HB_CHECK(run(1, false) == hb::CMD_PRIMARY);
    // out of sight, people reload early with little left in the clip, seldom with most of it
    auto earlyReloads = [&](int clip) {
        int n = 0;
        for (int k = 0; k < 400; k++) {
            w.Reset();
            s.weaponClass = hb::WEAPON_CLASS_SMG;
            s.weaponState = 0;
            s.clipSize    = 32;
            s.clipAmmo    = clip;
            s.reserveAmmo = 64;
            n += run(20, false) == hb::CMD_RELOAD;   // within a second
        }
        return n / 400.0;
    };
    const double low = earlyReloads(3), full = earlyReloads(28);
    HB_REPORT("early reload within a second, enemy out of sight: %.2f with 3 rounds left, %.2f with 28", low, full);
    HB_CHECK(low > 0.05 && low > 4.0 * full);
}

static void TestView(const hb::ModelBundle& b)
{
    hb::ViewControl vc;
    vc.Init(&b.shared.view);
    // main sequence: bigger flicks take longer
    HB_CHECK(vc.FlickDurationMs(15.0f) < vc.FlickDurationMs(100.0f));
    HB_CHECK_NEAR(vc.FlickDurationMs(15.0f), 175.0, 40.0);

    // tracking a static target from 20 degrees off converges near it
    hb::SelfState self;
    self.alive  = true;
    self.eye    = hb::Vec3(0, 0, 82);
    self.viewYaw = 20.0f;
    vc.Reset(self);
    hb::ViewInput in;
    in.ctx       = hb::CTX_LOS_FIRE;
    in.firing    = true;
    in.track     = true;
    in.detected  = true;
    in.acquisition = true;
    in.enemyFeet = hb::Vec3(400, 0, 0);
    hb::Rng        r(3);
    hb::ViewOutput out;
    std::vector<float> errs;
    for (int t = 0; t < 400; t++) {
        in.acquisition = t < 10;
        vc.Step(self, in, r, out);
        HB_CHECK(std::isfinite(out.yawDelta) && std::isfinite(out.pitchDelta));
        self.viewYaw   = hb::Wrap180(self.viewYaw + out.yawDelta);
        self.viewPitch = self.viewPitch + out.pitchDelta;
        if (t > 40) {
            errs.push_back(std::fabs(hb::Wrap180(0.0f - self.viewYaw)));
        }
    }
    std::sort(errs.begin(), errs.end());
    const float med = errs[errs.size() / 2];
    HB_REPORT("static target at 400 u: median |yaw error| %.2f deg", med);
    HB_CHECK(med < 5.0f);

    // the owner's rule: right after a head hit the aim is at the chest at once, even for a style
    // that aims high
    in.aimHeightFiring = 0.9f;
    for (int t = 0; t < 20; t++) {
        vc.Step(self, in, r, out);
    }
    HB_CHECK(out.aimHeight > 0.8f);
    in.chestOnly = true;
    vc.Step(self, in, r, out);
    HB_CHECK(out.aimHeight <= 0.5f);
    in.chestOnly = false;

    // hidden and idle: the mouse is often exactly still
    self.viewYaw = 0.0f;
    vc.Reset(self);
    // a belief with two likely exposure points to pre-aim at
    hb::BeliefEstimate be;
    be.valid           = true;
    be.nExposure       = 2;
    be.exposure[0]     = hb::Vec3(500, 100, 0);
    be.exposure[1]     = hb::Vec3(300, -400, 0);
    be.exposureMass[0] = 0.6f;
    be.exposureMass[1] = 0.3f;
    hb::ViewInput h;
    h.ctx    = hb::CTX_HIDDEN_NOFIRE;
    h.firing = false;
    h.belief = &be;
    // people who stop moving mostly stop turning too (hidden: 75% of standing ticks, about 20% on the move)
    for (const float speed : {0.0f, 200.0f}) {
        self.velocity = hb::Vec3(speed, 0.0f, 0.0f);
        int still     = 0;
        for (int t = 0; t < 20000; t++) {
            vc.Step(self, h, r, out);
            still += out.still;
            self.viewYaw   = hb::Wrap180(self.viewYaw + out.yawDelta);
            self.viewPitch = hb::Clamp(self.viewPitch + out.pitchDelta, -85.0f, 85.0f);
            self.timeMs += 50;
        }
        HB_REPORT("hidden at %.0f u/s: still share %.2f", speed, still / 20000.0);
        if (speed > 0.0f) {
            HB_CHECK(still / 20000.0 > 0.1 && still / 20000.0 < 0.5);
        } else {
            HB_CHECK(still / 20000.0 > 0.5 && still / 20000.0 < 0.9);
        }
    }
    self.velocity = hb::Vec3();
}

// An enemy heard behind is turned to after the reaction delay, and the view keeps that way for the hold.
static void TestSoundTurn(const hb::ModelBundle& b)
{
    hb::ViewModel vm     = b.shared.view;
    vm.soundTurnP        = 1.0f;
    vm.soundTurnDelayMs  = 150.0f;
    vm.soundTurnHoldMs   = 3000.0f;
    vm.lookaroundPerMin  = 0.0f;
    hb::SelfState self;
    self.alive   = true;
    self.eye     = hb::Vec3(0, 0, 82);
    self.viewYaw = 0.0f;
    hb::ViewInput h;
    h.ctx = hb::CTX_HIDDEN_NOFIRE;
    std::vector<hb::SoundObs> sounds(1);
    sounds[0].type = hb::SOUND_FOOTSTEP;
    sounds[0].yaw  = 170.0f;   // behind
    std::vector<hb::SoundObs> none;
    hb::ViewControl vc;
    vc.Init(&vm);
    vc.Reset(self);
    hb::Rng        r(9);
    hb::ViewOutput out;
    float          yawAt1s = 0.0f, yawAt3s = 0.0f;
    for (int t = 0; t < 60; t++) {
        h.sounds = t == 2 ? &sounds : &none;
        self.timeMs += 50;
        vc.Step(self, h, r, out);
        self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
        if (t == 21) {
            yawAt1s = self.viewYaw;
        }
        if (t == 59) {
            yawAt3s = self.viewYaw;
        }
    }
    HB_REPORT("a footstep behind (170 deg): view at %.0f deg after 1 s, %.0f after 3 s", yawAt1s, yawAt3s);
    HB_CHECK(std::fabs(hb::Wrap180(yawAt1s - 170.0f)) < 30.0f);
    HB_CHECK(std::fabs(hb::Wrap180(yawAt3s - 170.0f)) < 30.0f);
}

// A hit from an enemy not in sight turns the view toward it, also from just outside the view (the owner shot a bot
// from 61 deg off and its view did not move: only hits felt more than 60 deg off used to turn it).
static void TestDamageTurn(const hb::ModelBundle& b)
{
    hb::ViewModel vm    = b.shared.view;
    vm.lookaroundPerMin = 0.0f;
    for (const float off : {30.0f, 55.0f, 120.0f}) {
        hb::SelfState self;
        self.alive   = true;
        self.eye     = hb::Vec3(0, 0, 82);
        self.viewYaw = 0.0f;
        hb::ViewInput h;
        h.ctx = hb::CTX_HIDDEN_NOFIRE;
        std::vector<hb::DamageObs> hit(1), none;
        hit[0].yaw = off;
        hb::ViewControl vc;
        vc.Init(&vm);
        vc.Reset(self);
        hb::Rng        r(11);
        hb::ViewOutput out;
        bool           damageMode = false;
        for (int t = 0; t < 12; t++) {   // 600 ms
            h.damage = t == 2 ? &hit : &none;
            self.timeMs += 50;
            vc.Step(self, h, r, out);
            self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
            damageMode   = damageMode || out.mode == hb::VIEW_DAMAGE;
        }
        HB_REPORT("hit felt %.0f deg off the view: view at %.0f deg 500 ms later", off, self.viewYaw);
        HB_CHECK(damageMode);
        HB_CHECK(std::fabs(hb::Wrap180(self.viewYaw - off)) < 15.0f);
    }
}

// A belief look with a corner watches the corner nearest the believed position, and a watched corner is followed by
// where it is when another exposure cell comes to offer it.
static void TestViewCorners(const hb::ModelBundle& b)
{
    hb::ViewModel vm      = b.shared.view;
    vm.beliefLookShare    = 1.0f;   // every look decision is a belief look
    vm.preaimShare        = 0.0f;
    vm.preaimWeight       = 0.0f;
    vm.preaimHazard       = 0.0f;
    vm.travelShare        = 0.0f;
    vm.lookaroundPerMin   = 0.0f;
    vm.routeTurnHazard    = 0.0f;
    vm.hiddenReaimHazard  = 0.0f;
    hb::SelfState self;
    self.alive = true;
    self.eye   = hb::Vec3(0, 0, 82);
    hb::BeliefEstimate be;
    be.valid           = true;
    be.spread          = 50.0f;
    be.mode            = hb::Vec3(600, 300, 0);   // behind cover, 26.6 deg left
    be.nExposure       = 2;
    be.exposure[0]     = hb::Vec3(400, 200, 0);
    be.exposure[1]     = hb::Vec3(300, -300, 0);
    be.exposureCell[0] = 10;
    be.exposureCell[1] = 20;
    be.exposureMass[0] = 0.4f;   // the lighter one lies toward the believed position
    be.exposureMass[1] = 0.6f;
    be.exposureEtaMs[0] = be.exposureEtaMs[1] = 3000.0f;
    be.cornerValid[0] = be.cornerValid[1] = true;
    be.corner[0]       = hb::Vec3(400, 200, 82);
    be.corner[1]       = hb::Vec3(300, -300, 82);
    be.cornerOpen[0]   = 1.0f;
    be.cornerOpen[1]   = -1.0f;
    hb::ViewInput h;
    h.ctx    = hb::CTX_HIDDEN_NOFIRE;
    h.belief = &be;
    for (const float corner : {0.0f, 1.0f}) {
        vm.beliefLookCorner = corner;
        hb::ViewControl vc;
        vc.Init(&vm);
        self.viewYaw = 0.0f;
        vc.Reset(self);
        hb::Rng        r(5);
        hb::ViewOutput out;
        for (int t = 0; t < 20; t++) {
            vc.Step(self, h, r, out);
            self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
        }
        const float cornerYaw = hb::YawOf(be.corner[0] - self.eye) - vm.preaimCoverDeg;
        HB_REPORT("belief look, corner %.0f: mode %d, target yaw %.1f (corner %.1f, believed position %.1f)", corner, out.mode,
                  out.targetYaw, cornerYaw, hb::YawOf(be.mode - self.eye));
        if (corner > 0.5f) {
            HB_CHECK(out.mode == hb::VIEW_PREAIM);
            HB_CHECK_NEAR(out.targetYaw, cornerYaw, 0.5);
        } else {
            HB_CHECK(out.mode == hb::VIEW_BELIEF);
        }
    }
    // the watched edge is offered by another exposure cell, a little moved: followed by geometry, kept by cell
    vm.beliefLookCorner = 1.0f;
    for (const float geom : {0.0f, 1.0f}) {
        vm.preaimFollowGeom = geom;
        vm.lookDwellMedianMs = 1e6f;   // no new look decision
        hb::ViewControl vc;
        vc.Init(&vm);
        self.viewYaw = 0.0f;
        vc.Reset(self);
        hb::Rng            r(5);
        hb::ViewOutput     out;
        hb::BeliefEstimate b2 = be;
        for (int t = 0; t < 5; t++) {
            vc.Step(self, h, r, out);
        }
        b2.exposureCell[0] = 11;
        b2.corner[0]       = hb::Vec3(400, 210, 82);   // 1.1 deg further left
        h.belief           = &b2;
        vc.Step(self, h, r, out);
        h.belief = &be;
        const float moved = hb::YawOf(b2.corner[0] - self.eye) - vm.preaimCoverDeg;
        HB_REPORT("follow by geometry %.0f: target yaw %.2f (the moved corner %.2f)", geom, out.targetYaw, moved);
        if (geom > 0.5f) {
            HB_CHECK_NEAR(out.targetYaw, moved, 0.05);
        } else {
            HB_CHECK(std::fabs(out.targetYaw - moved) > 0.5f);
        }
    }
}

static void TestSubsteps()
{
    hb::Substepper ss;
    ss.Init(4);
    ss.Reset(0.0f, 0.0f);
    hb::Rng             r(11);
    std::vector<hb::SubCmd> cmds;
    hb::TickPlan        plan;
    int                 at[4] = {0, 0, 0, 0};
    float               yaw   = 0.0f;
    for (int t = 0; t < 4000; t++) {
        plan.chord    = (t % 2) ? 3 : 5;   // flip every tick
        plan.yawDelta = 4.0f;
        ss.Build(plan, yaw, 0.0f, r, cmds);
        HB_CHECK(hb::Substepper::CheckContract(cmds));
        HB_CHECK(cmds.size() == 4);
        HB_CHECK_NEAR(cmds.back().yaw, yaw + 4.0f, 1e-3);
        for (size_t k = 1; k < cmds.size(); k++) {
            HB_CHECK(cmds[k].yaw >= cmds[k - 1].yaw - 1e-4f);
        }
        for (int k = 0; k < 4; k++) {
            if (cmds[k].chord == plan.chord) {
                at[k]++;
                break;
            }
        }
        yaw = cmds.back().yaw;
    }
    // the key change lands on each sub-step about equally often
    for (int k = 0; k < 4; k++) {
        HB_CHECK_NEAR(at[k] / 4000.0, 0.25, 0.04);
    }
    // a still tick keeps every sub-step on the same angles
    plan.viewStill = true;
    ss.Build(plan, 10.0f, 5.0f, r, cmds);
    for (const hb::SubCmd& c : cmds) {
        HB_CHECK(c.yaw == 10.0f && c.pitch == 5.0f);
    }
    // contract violations are caught
    std::vector<hb::SubCmd> bad = cmds;
    bad[1].serverTimeOffset     = bad[0].serverTimeOffset;
    HB_CHECK(!hb::Substepper::CheckContract(bad));
}

static void TestEye()
{
    hb::EyeState st;
    hb::EyeInput in;
    in.origin     = hb::Vec3(100, 200, 0);
    in.viewheight = 82.0f;
    in.walking    = true;
    hb::Vec3 e;
    for (int i = 0; i < 50; i++) {
        e = hb::ComputeEye(st, in);
    }
    // standing still, level view, no lean: straight above the origin
    HB_CHECK_NEAR(e.x, 100.0, 1e-3);
    HB_CHECK_NEAR(e.y, 200.0, 1e-3);
    HB_CHECK_NEAR(e.z, 82.0, 1e-3);
    // lean right by 40 degrees rolls the eye around a pivot 28.7 below it
    in.leanAngle = 40.0f;
    e            = hb::ComputeEye(st, in);
    HB_CHECK_NEAR(std::fabs(e.y - 200.0), 28.7 * std::sin(40.0 * hb::DEG2RAD), 0.05);
    HB_CHECK_NEAR(e.z, 82.0 - 28.7 * (1.0 - std::cos(40.0 * hb::DEG2RAD)), 0.05);
    // looking down tilts the head forward around a lower pivot
    in.leanAngle = 0.0f;
    in.pitch     = 45.0f;
    e            = hb::ComputeEye(st, in);
    HB_CHECK(e.x > 100.5f);
    HB_CHECK(e.z < 82.0f);
    // crouching lowers the eye smoothly, not instantly
    in.pitch      = 0.0f;
    in.viewheight = 48.0f;
    e             = hb::ComputeEye(st, in);
    HB_CHECK(e.z > 49.0f && e.z < 82.0f);
    // CL_EyeInfo rounding
    HB_CHECK(hb::EyeOffsetByte(81.6f) == 82);
    HB_CHECK(hb::EyeOffsetByte(-3.4f) == -2);
    HB_CHECK(hb::EyeOffsetByte(500.0f) == 127);
}

// A dead enemy stays listed, unseen, so the belief keeps its track through the death (and draws it back at the spawns).
static void TestPerceiverDead(const hb::ModelBundle& b)
{
    hb::Perceiver pc;
    pc.Init(&b.shared.perception, hb::Rng(7));
    hb::RawInput raw;
    raw.self.alive = true;
    hb::RawEnemy e;
    e.id    = 3;
    e.alive = false;
    raw.enemies.push_back(e);
    hb::Observation obs;
    pc.Process(raw, 96.4f, 64.4f, 1.0f, obs);
    HB_CHECK(obs.enemies.size() == 1);
    HB_CHECK(!obs.enemies.empty() && obs.enemies[0].id == 3 && !obs.enemies[0].detected);
}

int main()
{
    hb::ModelBundle b;
    std::string     err;
    if (!hb::LoadBundle({}, b, err)) {
        std::printf("bundle failed: %s\n", err.c_str());
        return 1;
    }
    TestMovement(b);
    TestStance(b);
    TestTrigger(b);
    TestWeapon(b);
    TestView(b);
    TestViewCorners(b);
    TestSoundTurn(b);
    TestDamageTurn(b);
    TestLeanWall(b);
    TestLeanSwitch(b);
    TestStrafeMemory(b);
    TestSubsteps();
    TestEye();
    TestPerceiverDead(b);
    return hbtest::Finish("test_hb_modules");
}
