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
    // Since the fit on all five people (09-28 added) this open loop presses the diagonals 14% of the time; since the
    // refit of 2026-10-04 it strafes 0.80 of it (0.80 before too, under the old bound of 0.8 by a hair); since the fit
    // on the duels of every dm map (2026-10-08) 0.83 and the diagonal 0.10, while bot against bot in the engine the
    // fight's plain strafe and diagonal shares stayed where they were (49-58% and 32-43%; people 58% and 31%).
    HB_CHECK(med >= 200.0 && med <= 400.0);
    HB_CHECK(rev > 0.55 && rev < 0.9);
    HB_CHECK(pure > 0.4 && pure < 0.86);
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
    // running to a far fight (the enemy hidden 900 u or more away) a forward diagonal at a wall that blocks forward's own
    // direction too keeps forward, and with both key directions blocked lets go of the strafe key only: people running a
    // diagonal at a wall keep forward and steer with the mouse (they let go of it 49 times per 1000 ticks there, the
    // reflex made the bots' 273 and their running a weave). Near the enemy the reflex lets go of forward as before
    hb::MoveInput diagWall = corridor;
    diagWall.enemyDist     = 1000.0f;
    diagWall.clearance[6]  = 30.0f;   // forward-left: reached in 125 ms
    diagWall.clearance[7]  = 30.0f;   // forward too
    diagWall.clearance[3]  = 128.0f;  // left open
    hb::MoveInput diagBoxed = diagWall;
    diagBoxed.clearance[3]  = 30.0f;
    hb::MoveInput diagNear  = diagWall;
    diagNear.enemyDist      = 350.0f;
    int fwdKept = 0, fwdKeptBoxed = 0, sideKeptBoxed = 0, fwdKeptNear = 0;
    for (int i = 0; i < 2000; i++) {
        hb::Mover a, x, n;
        a.Init(&reflex);
        x.Init(&reflex);
        n.Init(&reflex);
        a.SetKeys(1, -1);
        x.SetKeys(1, -1);
        n.SetKeys(1, -1);
        a.Step(diagWall, style, rm, rs, out);
        fwdKept += hb::ChordFwd(out.chord) == 1;
        x.Step(diagBoxed, style, rm, rs, out);
        fwdKeptBoxed += hb::ChordFwd(out.chord) == 1;
        sideKeptBoxed += hb::ChordSide(out.chord) == -1;
        n.Step(diagNear, style, rm, rs, out);
        fwdKeptNear += hb::ChordFwd(out.chord) == 1;
    }
    HB_REPORT("diagonal at a wall ahead, running to a far fight: forward kept one more tick %.2f (near the enemy %.2f); with "
              "both key directions blocked forward %.2f, the strafe %.2f",
              fwdKept / 2000.0, fwdKeptNear / 2000.0, fwdKeptBoxed / 2000.0, sideKeptBoxed / 2000.0);
    HB_CHECK(fwdKept > 1800);
    HB_CHECK(fwdKeptNear < 1000);
    HB_CHECK(fwdKeptBoxed > 1800);
    HB_CHECK(sideKeptBoxed < 1000);
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

    // people crouch three times as readily in the half second after a shot is heard, and hardly when nothing has been
    // seen, heard or felt for a second; jumps likewise (their quiet jumps are mostly up onto something)
    auto dipsPerMin = [&](bool fire, bool quietNow, bool dead = false) {
        hb::Mover m;
        m.Init(&b.shared.movement);
        hb::MoveInput ci = in;
        ci.fireHeard     = fire;
        ci.quiet         = quietNow;
        ci.enemyDead     = dead;
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
    const double quiet = dipsPerMin(false, true), recent = dipsPerMin(false, false), fired = dipsPerMin(true, false);
    HB_REPORT("crouch dips a minute hidden: %.1f quiet, %.1f in a fight's lulls, %.1f after a shot", quiet, recent, fired);
    HB_CHECK(fired > 3.0 * quiet);
    HB_CHECK(recent > 1.5 * quiet);
    // the owner's rule (2026-10-08), as the model ships: no crouch with no reason for it, quiet or with the enemy dead,
    // unless a shot was just heard
    const double deadQuiet = dipsPerMin(false, true, true), deadFired = dipsPerMin(true, false, true);
    HB_REPORT("crouch dips a minute with the enemy dead: %.1f, after a shot %.1f", deadQuiet, deadFired);
    HB_CHECK(b.shared.movement.crouch.noReasonScale == 0.0f);
    HB_CHECK(quiet == 0.0 && deadQuiet == 0.0 && recent > 1.0 && deadFired > 1.0);
    auto jumpsPerMin = [&](bool quietNow) {
        hb::Mover m;
        m.Init(&b.shared.movement);
        hb::MoveInput ci = in;
        ci.quiet         = quietNow;
        hb::Rng r1(33), r2(34);
        bool    pk = false;
        int     n  = 0;
        for (int i = 0; i < 200000; i++) {
            m.Step(ci, style, r1, r2, out);
            n += out.jump && !pk;
            pk = out.jump;
        }
        return n / (200000.0 / 1200.0);
    };
    const double jq = jumpsPerMin(true), jr = jumpsPerMin(false);
    HB_REPORT("jumps a minute hidden: %.1f quiet, %.1f in a fight's lulls", jq, jr);
    HB_CHECK(jr > 1.5 * jq);
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

// With the enemy hidden, the farther away it is believed to be, the longer people keep forward held and the less readily
// they start a strafe (on every practice map: within 300 u they let go of forward at 9% a tick, beyond 700 u under 3%).
static void TestHiddenDistance(const hb::ModelBundle& b)
{
    HB_CHECK(!b.shared.movement.fwd.hidDistLogit.v.empty() && !b.shared.movement.side.hidDistLogit.v.empty());
    hb::StyleOffsets style;
    auto shares = [&](float dist, double& fwd, double& strafe) {
        hb::Mover mv;
        mv.Init(&b.shared.movement);
        hb::MoveInput in;
        in.ctx        = hb::CTX_HIDDEN_NOFIRE;
        in.enemyKnown = true;
        in.enemyDist  = dist;
        in.onGround   = true;
        hb::Rng        rm(61), rs(62);
        hb::MoveOutput out;
        int            f = 0, st = 0;
        const int      n = 300000;
        for (int i = 0; i < n; i++) {
            mv.Step(in, style, rm, rs, out);
            f += hb::ChordFwd(out.chord) == 1;
            st += hb::ChordSide(out.chord) != 0;
        }
        fwd    = f / double(n);
        strafe = st / double(n);
    };
    double fNear, sNear, fFar, sFar;
    shares(200.0f, fNear, sNear);
    shares(900.0f, fFar, sFar);
    HB_REPORT("enemy hidden 200 u away: forward held %.2f, strafing %.2f; 900 u away: %.2f, %.2f", fNear, sNear, fFar, sFar);
    HB_CHECK(fFar > fNear + 0.05);
    HB_CHECK(sFar < sNear);
    // the diagonal habit, a style measured in fights, fades running to a far fight (whole within nav_far_near of a hidden
    // enemy, gone from nav_far_dist): people of every style run to it (the strafer people hold a plain strafe 10% of
    // the time they move 800-1200 u from a hidden enemy on dm/brownffa and dm/flag, 29% within 500 u)
    auto plain = [&](const hb::StyleOffsets& st, float dist) {
        hb::Mover mv;
        mv.Init(&b.shared.movement);
        hb::MoveInput in;
        in.ctx        = hb::CTX_HIDDEN_NOFIRE;
        in.enemyKnown = true;
        in.enemyDist  = dist;
        in.onGround   = true;
        hb::Rng        rm(63), rs(64);
        hb::MoveOutput out;
        int            n = 0;
        for (int i = 0; i < 300000; i++) {
            mv.Step(in, st, rm, rs, out);
            n += hb::ChordFwd(out.chord) == 0 && hb::ChordSide(out.chord) != 0;
        }
        return n / 300000.0;
    };
    hb::StyleOffsets lowDiag;
    lowDiag.diagLogit = -2.7f;
    const double lowNear = plain(lowDiag, 300.0f), lowFar = plain(lowDiag, 1000.0f), pooledFar = plain(style, 1000.0f);
    HB_REPORT("plain strafe, enemy hidden: a low diagonal habit %.3f at 300 u, %.3f at 1000 u (no habit %.3f)", lowNear,
              lowFar, pooledFar);
    HB_CHECK(lowFar < lowNear - 0.03);
    HB_CHECK(std::fabs(lowFar - pooledFar) < 0.02);
}

// A route that turns: every second it takes a new bearing within 90 deg of the view. A key changes for the route once
// that improves the chord's alignment by more than nav_deadband; at 0.3 the bots kept forward alone, a diagonal or a
// strafe that went the route's way only partly (a stable route in the open was within 22 deg of the held chord on 43% of
// ticks on the maps), zig-zagged and ran back and forth.
static void TestRouteTurns(const hb::ModelBundle& b)
{
    hb::StyleOffsets style;
    auto along = [&](float deadband) {
        hb::MovementModel mm = b.shared.movement;
        mm.navDeadband       = deadband;
        hb::Mover mv;
        mv.Init(&mm);
        hb::MoveInput in;
        in.ctx        = hb::CTX_HIDDEN_NOFIRE;
        in.enemyKnown = true;
        in.enemyDist  = 600.0f;
        in.onGround   = true;
        in.navValid   = true;
        in.travelling = true;
        in.urgency    = 0.8f;
        hb::Rng        rm(81), rs(82), rr(83);
        hb::MoveOutput out;
        double         sum = 0.0;
        const int      n   = 200000;
        for (int i = 0; i < n; i++) {
            if (i % 20 == 0) {
                in.navBearing = static_cast<float>(rr.Uniform(-90.0, 90.0));
            }
            mv.Step(in, style, rm, rs, out);
            if (out.chord != hb::CHORD_NEUTRAL) {
                sum += std::cos((hb::Mover::ChordAngle(out.chord) - in.navBearing) * hb::DEG2RAD);
            }
        }
        return sum / n;
    };
    const double now = along(b.shared.movement.navDeadband), before = along(0.3f);
    HB_REPORT("a route turning every second: the keys go its way %.2f (cosine, dead band %.2f), %.2f at 0.3", now,
              b.shared.movement.navDeadband, before);
    HB_CHECK(b.shared.movement.navDeadband < 0.3f);
    HB_CHECK(now > before + 0.02);
}

// Travelling with no enemy expected soon, the view leads along the way: it follows a point ahead on the path round a
// turn, gives way to a corner once the enemy is expected out of it soon, and turns to a closed door across the way; and
// while it leads, the keys hold forward or a forward diagonal more.
static void TestTravelLead(const hb::ModelBundle& b)
{
    hb::ViewModel vm    = b.shared.view;
    vm.lookaroundPerMin = 0.0f;
    HB_CHECK(vm.travelLead > 0.0f);
    // the bot runs east at 250 u/s, the path turns north at x = 300; the point travelLead ahead along it
    auto ahead = [&](float x) {
        const float left = vm.travelLead - std::max(0.0f, 300.0f - x);
        return left <= 0.0f ? hb::Vec3(x + vm.travelLead, 0, 0) : hb::Vec3(300.0f, left, 0);
    };
    hb::BeliefEstimate be;   // the enemy far: its corner 3 s away
    be.valid            = true;
    be.spread           = 200.0f;
    be.mode             = hb::Vec3(-1500, 500, 0);
    be.nExposure        = 1;
    be.exposure[0]      = hb::Vec3(-900, 600, 0);
    be.exposureCell[0]  = 7;
    be.exposureMass[0]  = 1.0f;
    be.exposureEtaMs[0] = 3000.0f;
    be.cornerValid[0]   = true;
    be.corner[0]        = hb::Vec3(-900, 600, 82);
    be.cornerOpen[0]    = 1.0f;
    hb::SelfState self;
    self.alive    = true;
    self.velocity = hb::Vec3(250, 0, 0);
    hb::ViewInput h;
    h.ctx        = hb::CTX_HIDDEN_NOFIRE;
    h.belief     = &be;
    h.moving     = true;
    h.navValid   = true;
    h.travelling = true;
    h.aheadValid = true;
    hb::ViewControl vc;
    vc.Init(&vm);
    vc.Reset(self);
    hb::Rng        r(23);
    hb::ViewOutput out;
    double         errSum = 0.0;
    int            n = 0, travel = 0;
    float          x = 0.0f;
    for (int t = 0; t < 60; t++) {   // 3 s: 1.2 s east, then north
        const hb::Vec3 pos = x < 300.0f ? hb::Vec3(x, 0, 0) : hb::Vec3(300.0f, x - 300.0f, 0);
        self.origin   = pos;
        self.eye      = pos + hb::Vec3(0, 0, 82);
        self.velocity = x < 300.0f ? hb::Vec3(250, 0, 0) : hb::Vec3(0, 250, 0);
        h.ahead       = x < 300.0f ? ahead(x) : hb::Vec3(300.0f, x - 300.0f + vm.travelLead, 0);
        self.timeMs += 50;
        vc.Step(self, h, r, out);
        self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
        x += 12.5f;
        if (t >= 10) {
            errSum += std::fabs(hb::Wrap180(hb::YawOf(h.ahead - pos) - self.viewYaw));
            n++;
            travel += out.travel && out.mode == hb::VIEW_TRAVEL ? 1 : 0;
        }
    }
    HB_REPORT("travel round a path turn: the view %.1f deg off the point %.0f u ahead on average, travel mode %d of %d ticks, "
              "view at %.0f deg at the end (the way north: 90)", errSum / n, vm.travelLead, travel, n, self.viewYaw);
    HB_CHECK(travel == n);
    HB_CHECK(errSum / n < 15.0);
    HB_CHECK(std::fabs(hb::Wrap180(self.viewYaw - 90.0f)) < 15.0f);

    // the enemy expected out of the corner soon: the corner wins
    be.corner[0]        = hb::Vec3(700, 2000, 82);
    be.exposure[0]      = hb::Vec3(700, 2000, 0);
    be.exposureEtaMs[0] = 100.0f;
    bool preaim = false;
    for (int t = 0; t < 40; t++) {
        self.timeMs += 50;
        vc.Step(self, h, r, out);
        self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
        preaim       = preaim || out.mode == hb::VIEW_PREAIM;
    }
    HB_REPORT("the enemy expected out of a corner soon: travel %d, pre-aim seen %d", out.travel ? 1 : 0, preaim ? 1 : 0);
    HB_CHECK(!out.travel);
    HB_CHECK(preaim);

    // a closed door across the way, 40 deg right, the bot at it: the view turns to it
    be.exposureEtaMs[0] = 3000.0f;
    self.viewYaw        = 90.0f;
    self.velocity       = hb::Vec3();
    h.doorValid         = true;
    h.door              = self.eye + hb::Vec3(std::cos(50.0f * hb::DEG2RAD), std::sin(50.0f * hb::DEG2RAD), 0) * 60.0f;
    for (int t = 0; t < 10; t++) {
        self.timeMs += 50;
        vc.Step(self, h, r, out);
        self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
    }
    HB_REPORT("a closed door 40 deg right across the way: view at %.0f deg after 0.5 s (the door at 50)", self.viewYaw);
    HB_CHECK(std::fabs(hb::Wrap180(self.viewYaw - 50.0f)) < 10.0f);

    // the keys while the view leads: the way within 45 deg of the view, forward held more
    hb::StyleOffsets style;
    auto fwdShare = [&](bool travelView) {
        hb::Mover mv;
        mv.Init(&b.shared.movement);
        hb::MoveInput in;
        in.ctx        = hb::CTX_HIDDEN_NOFIRE;
        in.enemyKnown = true;
        in.enemyDist  = 250.0f;   // near: the fitted keys strafe most
        in.onGround   = true;
        in.navValid   = true;
        in.travelling = true;
        in.urgency    = 0.8f;
        in.travelView = travelView;
        hb::Rng        rm(91), rs(92), rr(93);
        hb::MoveOutput mo;
        int            fwd = 0;
        const int      steps = 100000;
        for (int i = 0; i < steps; i++) {
            if (i % 20 == 0) {
                in.navBearing = static_cast<float>(rr.Uniform(-45.0, 45.0));
            }
            mv.Step(in, style, rm, rs, mo);
            fwd += hb::ChordFwd(mo.chord) == 1 ? 1 : 0;
        }
        return static_cast<double>(fwd) / steps;
    };
    const double led = fwdShare(true), free = fwdShare(false);
    HB_REPORT("the way within 45 deg, the enemy 250 u away: forward held %.2f of the time with the view leading (pull x%.1f), "
              "%.2f without", led,
              b.shared.movement.travelPull, free);
    HB_CHECK(led > free + 0.03);
}

// The enemy believed near (ViewModel::travelFar): on the way out of a spawn the view watches the corner on his side, not
// the way ahead (on dm/flag the bots came through the doors to the owner, who held the flag room, looking down the
// corridor); believed far, the view leads along the way. A closed door across the way is looked at by its direction: a
// bot strafing past it no longer swings its view off the way. And with the view on his side the keys follow the way
// more firmly (a forward diagonal rather than a plain strafe).
static void TestNearSide(const hb::ModelBundle& b)
{
    hb::ViewModel vm    = b.shared.view;
    vm.lookaroundPerMin = 0.0f;
    HB_CHECK(vm.travelFar > 0.0f);
    // the bot runs east; the believed enemy north-west of it, round a corner to the north
    hb::BeliefEstimate be;
    be.valid            = true;
    be.spread           = 150.0f;
    be.nExposure        = 2;
    be.exposureCell[0]  = 7;
    be.exposureMass[0]  = 0.5f;
    be.exposureEtaMs[0] = 2500.0f;
    be.cornerValid[0]   = true;
    be.cornerOpen[0]    = 1.0f;
    be.exposureCell[1]  = 8;   // a corner on the far side, as likely
    be.exposureMass[1]  = 0.5f;
    be.exposureEtaMs[1] = 2500.0f;
    be.cornerValid[1]   = true;
    be.cornerOpen[1]    = -1.0f;
    auto run = [&](float enemyDist, int& travel, float& offSide, float& offWay) {
        be.mode        = hb::Vec3(-enemyDist * 0.6f, enemyDist * 0.8f, 0);
        be.corner[0]   = hb::Vec3(-150, 250, 82);
        be.exposure[0] = hb::Vec3(-150, 250, 0);
        be.corner[1]   = hb::Vec3(250, -250, 82);
        be.exposure[1] = hb::Vec3(250, -250, 0);
        hb::SelfState self;
        self.alive = true;
        hb::ViewInput h;
        h.ctx        = hb::CTX_HIDDEN_NOFIRE;
        h.belief     = &be;
        h.moving     = true;
        h.navValid   = true;
        h.navYaw     = 0.0f;
        h.travelling = true;
        h.aheadValid = true;
        hb::ViewControl vc;
        vc.Init(&vm);
        vc.Reset(self);
        hb::Rng        r(29);
        hb::ViewOutput out;
        travel  = 0;
        offSide = offWay = 0.0f;
        for (int t = 0; t < 40; t++) {   // 2 s
            const hb::Vec3 pos(t * 2.0f, 0, 0);
            self.origin   = pos;
            self.eye      = pos + hb::Vec3(0, 0, 82);
            self.velocity = hb::Vec3(40, 0, 0);
            h.ahead       = pos + hb::Vec3(vm.travelLead, 0, 0);
            self.timeMs += 50;
            vc.Step(self, h, r, out);
            self.viewYaw   = hb::Wrap180(self.viewYaw + out.yawDelta);
            self.viewPitch = hb::Clamp(self.viewPitch + out.pitchDelta, -85.0f, 85.0f);
            travel += out.travel ? 1 : 0;
        }
        offSide = std::fabs(hb::Wrap180(hb::YawOf(be.corner[0] - self.eye) - self.viewYaw));
        offWay  = std::fabs(self.viewYaw);
    };
    int   tNear, tFar;
    float sNear, wNear, sFar, wFar;
    run(450.0f, tNear, sNear, wNear);
    run(1600.0f, tFar, sFar, wFar);
    HB_REPORT("the enemy believed 450 u away round a corner: travel %d of 40 ticks, the view %.0f deg off the corner on his "
              "side after 2 s (the way %.0f deg off); 1600 u away: travel %d, the way %.0f deg off", tNear, sNear, wNear, tFar, wFar);
    HB_CHECK(tNear == 0);
    HB_CHECK(sNear < 15.0f);
    HB_CHECK(tFar > 30);
    HB_CHECK(wFar < 15.0f);

    // a closed door 40 u ahead across the way (north), found afresh along the way from the eye each tick, the bot
    // strafing west past it at 150 u/s: the view stays on the way
    {
        hb::SelfState self;
        self.alive   = true;
        self.viewYaw = 90.0f;
        hb::BeliefEstimate far = be;
        far.mode               = hb::Vec3(0, 3000, 0);
        hb::ViewInput h;
        h.ctx        = hb::CTX_HIDDEN_NOFIRE;
        h.belief     = &far;
        h.moving     = true;
        h.navValid   = true;
        h.navYaw     = 90.0f;
        h.travelling = true;
        h.aheadValid = true;
        h.doorValid  = true;
        hb::ViewControl vc;
        vc.Init(&vm);
        vc.Reset(self);
        hb::Rng        r(31);
        hb::ViewOutput out;
        float          worst = 0.0f;
        for (int t = 0; t < 12; t++) {
            const hb::Vec3 pos(-7.5f * t, 0, 0);
            self.origin   = pos;
            self.eye      = pos + hb::Vec3(0, 0, 82);
            self.velocity = hb::Vec3(-150, 0, 0);
            h.ahead       = pos + hb::Vec3(0, vm.travelLead, 0);
            h.door        = self.eye + hb::Vec3(0, 40, 0);
            self.timeMs += 50;
            vc.Step(self, h, r, out);
            self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
            worst        = std::max(worst, std::fabs(hb::Wrap180(self.viewYaw - 90.0f)));
        }
        HB_REPORT("strafing past a closed door across the way: the view at most %.0f deg off it in 0.6 s", worst);
        HB_CHECK(worst < 10.0f);
    }

    // the keys with the view on his side and the way 20-70 deg off it: forward held more, plain strafes fewer
    hb::StyleOffsets style;
    auto shares = [&](bool nearView, double& fwd, double& strafe) {
        hb::Mover mv;
        mv.Init(&b.shared.movement);
        hb::MoveInput in;
        in.ctx        = hb::CTX_HIDDEN_NOFIRE;
        in.enemyKnown = true;
        in.enemyDist  = 450.0f;
        in.onGround   = true;
        in.navValid   = true;
        in.travelling = true;
        in.urgency    = 0.8f;
        in.nearView   = nearView;
        hb::Rng        rm(95), rs(96), rr(97);
        hb::MoveOutput mo;
        int            f = 0, st = 0;
        const int      steps = 100000;
        for (int i = 0; i < steps; i++) {
            if (i % 20 == 0) {
                in.navBearing = static_cast<float>(rr.Uniform(20.0, 70.0)) * (rr.Bernoulli(0.5) ? 1.0f : -1.0f);
            }
            mv.Step(in, style, rm, rs, mo);
            f += hb::ChordFwd(mo.chord) == 1 ? 1 : 0;
            st += hb::ChordFwd(mo.chord) == 0 && hb::ChordSide(mo.chord) != 0 ? 1 : 0;
        }
        fwd    = static_cast<double>(f) / steps;
        strafe = static_cast<double>(st) / steps;
    };
    double fN, sN, f0, s0;
    shares(true, fN, sN);
    shares(false, f0, s0);
    HB_REPORT("the way 20-70 deg off the view, the enemy 450 u away: forward held %.2f, plain strafe %.2f with the view on his "
              "side (pull x%.1f); %.2f, %.2f without", fN, sN, b.shared.movement.nearPull, f0, s0);
    HB_CHECK(fN > f0 + 0.03);
    HB_CHECK(sN < s0);
}

// A bot that wants to go somewhere, stands with no key held and whose route runs into a wall it touches takes the open
// chord nearest the route within a second instead of standing there.
static void TestUnstick(const hb::ModelBundle& b)
{
    hb::Mover mv;
    mv.Init(&b.shared.movement);
    hb::StyleOffsets style;
    hb::MoveInput    in;
    in.ctx        = hb::CTX_HIDDEN_NOFIRE;
    in.onGround   = true;
    in.navValid   = true;
    in.travelling = true;
    in.urgency    = 0.8f;
    in.navBearing = 90.0f;   // the route goes left, into a wall the bot touches on its left
    for (int c = 0; c < hb::NUM_CHORDS; c++) {
        in.clearance[c] = hb::ChordSide(c) == -1 ? 0.0f : 128.0f;
    }
    hb::Rng        rm(71), rs(72);
    hb::MoveOutput out;
    int            run = 0, longest = 0, pressed = 0, intoWall = 0;
    for (int i = 0; i < 100000; i++) {
        if (i % 200 == 0) {
            mv.SetKeys(0, 0);   // back to standing: a new stand starts
            run = 0;
        }
        mv.Step(in, style, rm, rs, out);
        if (out.chord == hb::CHORD_NEUTRAL) {
            longest = std::max(longest, ++run);
        } else {
            run = 0;
            pressed++;
            intoWall += hb::ChordSide(out.chord) == -1;
        }
    }
    HB_REPORT("route into a touched wall: longest stand %d ticks, presses into the wall %d of %d ticks", longest, intoWall, pressed);
    HB_CHECK(longest <= 21);
    HB_CHECK(intoWall == 0);
}

// The far route pull gives way while the bot presses into a wall: holding forward into a wall it touches, the route
// straight on and the enemy believed far, it lets go as readily as without the far pull (and less readily off the wall).
static void TestFarPullWall(const hb::ModelBundle& b)
{
    hb::MovementModel far = b.shared.movement, flat = b.shared.movement;
    far.navFarMult        = 3.0f;
    flat.navFarMult       = 1.0f;
    hb::StyleOffsets style;
    auto pChange = [&](const hb::MovementModel& mm, float pressMs, bool travelView = false) {
        hb::Mover mv;
        mv.Init(&mm);
        mv.SetKeys(1, 0);
        hb::MoveInput in;
        in.travelView  = travelView;
        in.ctx         = hb::CTX_HIDDEN_NOFIRE;
        in.enemyKnown  = true;
        in.enemyDist   = 1200.0f;
        in.onGround    = true;
        in.navValid    = true;
        in.travelling  = true;
        in.urgency     = 0.8f;
        in.navBearing  = 0.0f;   // the route runs straight on, into the wall ahead
        in.wallPressMs = pressMs;
        for (int c = 0; c < hb::NUM_CHORDS; c++) {
            in.clearance[c] = hb::ChordFwd(c) == 1 ? 2.0f : 128.0f;
        }
        hb::Rng        rm(81), rs(82);
        hb::MoveOutput out;
        mv.Step(in, style, rm, rs, out);
        return out.pSwitch;
    };
    const float pressFar = pChange(far, 200.0f), pressFlat = pChange(flat, 200.0f);
    const float offFar = pChange(far, 0.0f), offFlat = pChange(flat, 0.0f);
    HB_REPORT("forward held into a wall, enemy far: a key changes %.3f (no far pull %.3f); before pressing %.3f (%.3f)",
              pressFar, pressFlat, offFar, offFlat);
    HB_CHECK(std::fabs(pressFar - pressFlat) < 1e-6f);
    HB_CHECK(offFar < offFlat);
    // nor the travel mode's pull (in the arena a bot pushed against a pillar for 2.1 s with it)
    const float pressLed = pChange(far, 200.0f, true), offLed = pChange(far, 0.0f, true);
    HB_REPORT("... with the view leading along the way: %.3f; before pressing %.3f", pressLed, offLed);
    HB_CHECK(std::fabs(pressLed - pressFar) < 1e-6f);
    HB_CHECK(offLed < offFar);
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

// People lean to see past what hides the enemy: away from his side while he is hidden, toward him on screen.
static void TestLeanEnemy(const hb::ModelBundle& b)
{
    hb::MovementModel mm = b.shared.movement;
    mm.leanEnemyLogit    = {-1.0f, -1.0f, 1.0f, 1.0f, 0.0f};
    hb::StyleOffsets style;
    auto toRight = [&](int ctx, float bearing) {
        hb::Mover m;
        m.Init(&mm);
        hb::MoveInput in;
        in.ctx          = ctx;
        in.enemyKnown   = true;
        in.enemyBearing = bearing;
        in.enemyDist    = 600.0f;
        in.onGround     = true;
        hb::Rng        r1(51), r2(52);
        hb::MoveOutput out;
        int            right = 0, leaned = 0;
        for (int i = 0; i < 200000; i++) {
            m.Step(in, style, r1, r2, out);
            leaned += out.lean != 0;
            right += out.lean == 1;
        }
        return right / double(std::max(leaned, 1));
    };
    // the enemy 45 deg to the right of the view (the bearing is + to the left)
    const double hid = toRight(hb::CTX_HIDDEN_NOFIRE, -45.0f), los = toRight(hb::CTX_LOS_NOFIRE, -45.0f);
    const double hidLeft = toRight(hb::CTX_HIDDEN_NOFIRE, 45.0f), ahead = toRight(hb::CTX_HIDDEN_NOFIRE, -1.0f);
    mm.leanEnemyLogit.clear();
    const double hid0 = toRight(hb::CTX_HIDDEN_NOFIRE, -45.0f), los0 = toRight(hb::CTX_LOS_NOFIRE, -45.0f);
    HB_REPORT("the enemy 45 deg to the right, leans to the right: hidden %.2f (no term %.2f; him on the left %.2f, "
              "straight ahead %.2f), in sight %.2f (no term %.2f)", hid, hid0, hidLeft, ahead, los, los0);
    HB_CHECK(hid < hid0 - 0.1);
    HB_CHECK(hidLeft > hid0 + 0.1);
    HB_CHECK(std::fabs(ahead - hid0) < 0.03);
    HB_CHECK(los > los0 + 0.1);
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
    // 25-38% per tick with the target near the crosshair (people, over all gaps since the last release; with no
    // prefire the calibrated shift puts this point, 0.5 s after a release, at about 0.5)
    HB_CHECK(pNear > 0.2f && pNear < 0.6f);
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
    in.canFire = true;
    // the owner's rule (2026-10-07), as the model ships: no press with no part of the enemy on screen, whatever the
    // fitted hidden press says (an exposure expected in the crosshair, a hit, the view on the belief), and a burst begun
    // in sight lets go within hidden_hold_ms of the last part leaving the screen
    {
        const hb::TriggerModel& m = b.shared.trigger;
        HB_CHECK(m.hiddenPress <= 0.0f && m.hiddenHoldMs > 0.0f && m.hiddenHoldMs <= 500.0f);
        hb::TriggerInput q;
        q.canFire      = true;
        q.los          = false;
        q.hiddenYawErr = 1.0f;
        q.anticipate   = true;
        q.damaged      = true;
        int presses    = 0;
        for (const int ms : {0, 100, 300, 1000, 3000, 20000}) {
            q.lageMs = ms;
            HB_CHECK(tr.PressProb(q, 20) == 0.0f);
            hb::Trigger t0;
            t0.Init(&m);
            hb::Rng rr(11);
            for (int i = 0; i < 2000; i++) {
                presses += t0.Step(q, rr) ? 1 : 0;
            }
        }
        HB_CHECK(presses == 0);
        // a burst begun in sight: the enemy leaves the screen and the trigger is let go of soon after
        int longest = 0;
        for (int k = 0; k < 400; k++) {
            hb::Trigger t1;
            t1.Init(&m);
            hb::Rng          rr(100 + k);
            hb::TriggerInput s = in;
            s.errHalfWidths    = 1.0f;
            s.lageMs           = 600;
            bool held          = false;
            for (int i = 0; i < 200 && !held; i++) {
                held = t1.Step(s, rr);
            }
            HB_CHECK(held);
            q.anticipate = false;
            q.damaged    = false;
            int ticks    = 0;
            for (q.lageMs = 0; q.lageMs < 5000 && t1.Step(q, rr); q.lageMs += 50) {
                ticks++;
            }
            longest = std::max(longest, ticks * 50);
        }
        HB_REPORT("bursts begun in sight run on at most %d ms past the last visible part (hidden_hold_ms %.0f)", longest,
                  m.hiddenHoldMs);
        HB_CHECK(longest <= static_cast<int>(m.hiddenHoldMs));
    }
    // the fitted hidden press (the rule off): firing into cover only where the enemy is believed to be
    hb::TriggerModel fitted = b.shared.trigger;
    fitted.hiddenPress      = 1.0f;
    fitted.hiddenHoldMs     = 0.0f;
    tr.Init(&fitted);
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
    hb::TriggerModel plain = fitted, faded = fitted;
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
    // after a kill: a reload within two seconds with no enemy left, none while another one is on screen, and a
    // planned one held when another comes on screen before it starts
    auto afterKill = [&](bool otherSeen, bool seenLater) {
        int n = 0;
        for (int k = 0; k < 400; k++) {
            w.Reset();
            s.weaponClass = hb::WEAPON_CLASS_SMG;
            s.weaponState = 0;
            s.clipSize    = 32;
            s.clipAmmo    = 12;
            s.reserveAmmo = 64;
            s.timeMs += 50;
            int cmd = w.Step(s, true, otherSeen, otherSeen, false, otherSeen ? 0 : 5000, r);
            for (int t = 0; t < 40 && cmd == hb::CMD_NONE; t++) {
                s.timeMs += 50;
                const bool seen = otherSeen || (seenLater && t >= 4);
                cmd = w.Step(s, false, otherSeen || seenLater, seen, false, seen ? 0 : 5000, r);
            }
            n += cmd == hb::CMD_RELOAD;
        }
        return n / 400.0;
    };
    const double alone = afterKill(false, false), other = afterKill(true, false), comes = afterKill(false, true);
    HB_REPORT("reload within 2 s of a kill (12 of 32 rounds left): %.2f with no enemy left, %.2f with another on "
              "screen, %.2f with another coming on screen 250 ms later", alone, other, comes);
    HB_CHECK(alone > 0.9);
    HB_CHECK(other < 0.02);
    HB_CHECK(comes < 0.02);
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

// The bot hears which side footsteps come from, as people do: with an unseen enemy running in front of them they turn
// around within a second 2.4% of the time, no more than with a quiet one (the bots, which heard a quarter of footsteps
// on the mirrored side, 11%). A footstep heard just outside the view is turned to like one behind.
static void TestHearingSide(const hb::ModelBundle& b)
{
    hb::Perceiver pc;
    pc.Init(&b.shared.perception, hb::Rng(13));
    int wrong = 0, n = 0;
    for (int i = 0; i < 2000; i++) {
        hb::RawInput raw;
        raw.self.alive   = true;
        raw.self.timeMs  = 50 * i;
        raw.self.viewYaw = 0.0f;
        const bool front = i % 2 == 0;
        raw.sounds.push_back(hb::RawSound{hb::SOUND_FOOTSTEP, 3, hb::Vec3(front ? 400.0f : -400.0f, 60.0f, 0.0f)});
        hb::Observation obs;
        pc.Process(raw, 96.4f, 64.4f, 1.0f, obs);
        for (const hb::SoundObs& o : obs.sounds) {
            n++;
            wrong += (std::fabs(o.yaw) > 90.0f) == front;
        }
    }
    HB_REPORT("footsteps 8.5 deg off straight ahead or behind: heard on the wrong side %d of %d", wrong, n);
    HB_CHECK(n == 2000 && wrong == 0);

    hb::ViewModel vm    = b.shared.view;
    vm.lookaroundPerMin = 0.0f;
    hb::SelfState self;
    self.alive   = true;
    self.eye     = hb::Vec3(0, 0, 82);
    self.viewYaw = 0.0f;
    hb::ViewInput h;
    h.ctx = hb::CTX_HIDDEN_NOFIRE;
    std::vector<hb::SoundObs> step(1), none;
    step[0].type = hb::SOUND_FOOTSTEP;
    step[0].yaw  = 65.0f;   // just outside the view (48 deg to each side)
    hb::ViewControl vc;
    vc.Init(&vm);
    vc.Reset(self);
    hb::Rng        r(9);
    hb::ViewOutput out;
    for (int t = 0; t < 22; t++) {
        h.sounds = t == 2 ? &step : &none;
        self.timeMs += 50;
        vc.Step(self, h, r, out);
        self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
    }
    HB_REPORT("a footstep 65 deg off: view at %.0f deg after 1 s", self.viewYaw);
    HB_CHECK(std::fabs(hb::Wrap180(self.viewYaw - 65.0f)) < 20.0f);
}

// A held direction gives way once the enemy is believed out of view of it: after running past a corner the bots kept
// its direction for its dwell (about 2 s), 60 deg away from where they rightly believed the enemy to be.
static void TestHoldGivesWay(const hb::ModelBundle& b)
{
    for (const float give : {0.0f, 48.0f}) {
        hb::ViewModel vm      = b.shared.view;
        vm.beliefLookShare    = 1.0f;
        vm.beliefLookCorner   = 0.0f;
        vm.preaimShare        = 0.0f;
        vm.preaimWeight       = 0.0f;
        vm.preaimHazard       = 0.0f;
        vm.travelShare        = 0.0f;
        vm.lookaroundPerMin   = 0.0f;
        vm.routeTurnHazard    = 0.0f;
        vm.hiddenReaimHazard  = 0.0f;
        vm.lookDwellMedianMs  = 5000.0f;
        vm.holdBeliefDeg      = give;
        vm.travelFar          = 0.0f;   // (the near rules, TestNearSide, would break the hold off as well)
        hb::SelfState self;
        self.alive   = true;
        self.eye     = hb::Vec3(0, 0, 82);
        self.viewYaw = 0.0f;
        hb::BeliefEstimate be;
        be.valid  = true;
        be.spread = 150.0f;
        be.mode   = hb::Vec3(0, 600, 0);   // 90 deg left
        hb::ViewInput h;
        h.ctx = hb::CTX_HIDDEN_NOFIRE;
        hb::ViewControl vc;
        vc.Init(&vm);
        vc.Reset(self);
        hb::Rng        r(17);
        hb::ViewOutput out;
        int            firstMode = -1;
        for (int t = 0; t < 30; t++) {
            h.belief = t < 5 ? nullptr : &be;   // no belief: the look holds the view's direction
            self.timeMs += 50;
            vc.Step(self, h, r, out);
            self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
            if (t == 0) {
                firstMode = out.mode;
            }
        }
        HB_REPORT("held look, enemy then believed 90 deg left, give way at %.0f deg: view at %.0f deg 1.25 s later", give, self.viewYaw);
        HB_CHECK(firstMode == hb::VIEW_HOLD);
        if (give > 0.0f) {
            HB_CHECK(std::fabs(hb::Wrap180(self.viewYaw - 90.0f)) < 20.0f);
        } else {
            HB_CHECK(std::fabs(self.viewYaw) < 10.0f);
        }
    }
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

// A look that begins while the view is still turning to the last one is turned to once that turn is done: a quarter
// of the bots' hidden looks began so and were never turned to (the view 21 deg off the new target when the old turn
// ended, then left to the idle controller and the still mouse).
static void TestLookChain(const hb::ModelBundle& b)
{
    float med[2];
    for (const int chain : {0, 1}) {
        hb::ViewModel vm    = b.shared.view;
        vm.lookaroundPerMin = 0.0f;
        vm.lookChain        = static_cast<float>(chain);
        std::vector<float> offs;
        for (int k = 0; k < 200; k++) {
            hb::SelfState self;
            self.alive   = true;
            self.eye     = hb::Vec3(0, 0, 82);
            self.viewYaw = 0.0f;
            hb::ViewInput h;
            h.ctx = hb::CTX_HIDDEN_NOFIRE;
            std::vector<hb::DamageObs> first(1), second(1), none;
            first[0].yaw  = 100.0f;   // a big turn, under way for about 300 ms
            second[0].yaw = -40.0f;   // felt while it is under way
            hb::ViewControl vc;
            vc.Init(&vm);
            vc.Reset(self);
            hb::Rng        r(100 + k);
            hb::ViewOutput out;
            for (int t = 0; t < 24; t++) {   // 1.2 s
                h.damage = t == 2 ? &first : t == 5 ? &second : &none;
                self.timeMs += 50;
                vc.Step(self, h, r, out);
                self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
            }
            offs.push_back(std::fabs(hb::Wrap180(self.viewYaw + 40.0f)));
        }
        std::sort(offs.begin(), offs.end());
        med[chain] = offs[offs.size() / 2];
    }
    HB_REPORT("a hit felt at -40 deg while turning to one at 100: the view %.0f deg off it 1.2 s later (%.0f without the "
              "chained turn)", med[1], med[0]);
    HB_CHECK(med[1] < 10.0f);
    HB_CHECK(med[0] > 2.0f * med[1]);
}

// With the enemy believed near, a sound's direction (10-20 deg off where he is) gives way to where he is believed to be
// once it has been watched 600 ms and lies 20 deg off him: the bots held it for 3 s while the belief went on refining.
static void TestNearSoundGivesWay(const hb::ModelBundle& b)
{
    float at[2];
    for (const int give : {0, 1}) {
        hb::ViewModel vm      = b.shared.view;
        vm.lookaroundPerMin   = 0.0f;
        vm.beliefLookShare    = 1.0f;
        vm.preaimShare        = 0.0f;
        vm.preaimWeight       = 0.0f;
        vm.nearSoundDeg       = give ? 20.0f : 0.0f;
        HB_CHECK(vm.travelFar > 400.0f && vm.nearAwayDeg > 70.0f);
        hb::BeliefEstimate be;
        be.valid  = true;
        be.spread = 150.0f;
        be.mode   = hb::Vec3(400, 0, 0);   // straight ahead, near
        hb::SelfState self;
        self.alive   = true;
        self.eye     = hb::Vec3(0, 0, 82);
        self.viewYaw = 0.0f;
        hb::ViewInput h;
        h.ctx    = hb::CTX_HIDDEN_NOFIRE;
        h.belief = &be;
        std::vector<hb::SoundObs> step(1), none;
        step[0].type = hb::SOUND_FOOTSTEP;
        step[0].yaw  = 70.0f;   // heard 70 deg left, outside the view
        hb::ViewControl vc;
        vc.Init(&vm);
        vc.Reset(self);
        hb::Rng        r(23);
        hb::ViewOutput out;
        for (int t = 0; t < 36; t++) {   // 1.8 s
            h.sounds = t == 2 ? &step : &none;
            self.timeMs += 50;
            vc.Step(self, h, r, out);
            self.viewYaw = hb::Wrap180(self.viewYaw + out.yawDelta);
        }
        at[give] = self.viewYaw;
    }
    HB_REPORT("the enemy believed near ahead, a footstep heard 70 deg left: the view at %.0f deg 1.8 s later (%.0f when the "
              "sound's direction is kept)", at[1], at[0]);
    HB_CHECK(std::fabs(at[1]) < 15.0f);
    HB_CHECK(std::fabs(hb::Wrap180(at[0] - 70.0f)) < 20.0f);
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

// The soft wallhack (the owner's, 2026-10-05): a hidden enemy's true position reaches the belief only as an occasional,
// noisy hunch (hunch_per_min a minute, hunch_sigma_deg off), never while it is seen, and the view does not turn to it
// (only footsteps and gunfire turn it).
static void TestHunch(const hb::ModelBundle& b)
{
    const hb::PerceptionModel& pm = b.shared.perception;
    HB_CHECK(pm.hunchPerMin > 0.0f);
    hb::Perceiver pc;
    pc.Init(&pm, hb::Rng(11));
    hb::RawInput raw;
    raw.self.alive  = true;
    raw.self.origin = hb::Vec3(0, 0, 0);
    raw.self.eye    = hb::Vec3(0, 0, 82);
    hb::RawEnemy e;
    e.id         = 3;
    e.alive      = true;
    e.hunchValid = true;
    e.hunchPos   = hb::Vec3(0, 600, 0);   // north, behind a wall
    raw.enemies.push_back(e);
    hb::Observation obs;
    int             n = 0;
    double          s2 = 0.0;
    const int       ticks = 20 * 600;     // 10 minutes
    for (int t = 0; t < ticks; t++) {
        pc.Process(raw, 96.4f, 64.4f, 1.0f, obs);
        for (const hb::SoundObs& so : obs.sounds) {
            if (so.type == hb::SOUND_HUNCH && so.sourceId == 3) {
                n++;
                const double d = hb::Wrap180(so.yaw - 90.0f);
                s2 += d * d;
            }
        }
    }
    const double perMin = n / 10.0, sd = n ? std::sqrt(s2 / n) : 0.0;
    HB_REPORT("a hidden enemy 600 u away: %.1f hunches a minute (%.0f set), their direction %.1f deg off (sd; %.0f set)", perMin,
              pm.hunchPerMin, sd, pm.hunchSigmaDeg);
    HB_CHECK(std::fabs(perMin - pm.hunchPerMin) < 0.15 * pm.hunchPerMin);
    HB_CHECK(std::fabs(sd - pm.hunchSigmaDeg) < 0.2 * pm.hunchSigmaDeg);
    // none while it is seen
    raw.enemies[0].inFov    = true;
    raw.enemies[0].partMask = 0x3f;
    raw.enemies[0].centroid = hb::Vec3(0, 600, 50);
    int seen = 0;
    for (int t = 0; t < 2000; t++) {
        pc.Process(raw, 96.4f, 64.4f, 1.0f, obs);
        for (const hb::SoundObs& so : obs.sounds) {
            seen += !obs.enemies.empty() && obs.enemies[0].detected && so.type == hb::SOUND_HUNCH ? 1 : 0;
        }
    }
    HB_CHECK(seen == 0);
    // the view does not turn to a hunch behind it
    hb::ViewModel vm    = b.shared.view;
    vm.lookaroundPerMin = 0.0f;
    hb::ViewControl vc;
    vc.Init(&vm);
    hb::SelfState self;
    self.alive = true;
    self.eye   = hb::Vec3(0, 0, 82);
    vc.Reset(self);
    std::vector<hb::SoundObs> hs(1);
    hs[0].type = hb::SOUND_HUNCH;
    hs[0].yaw  = 180.0f;
    hb::ViewInput h;
    h.sounds = &hs;
    hb::Rng        r(13);
    hb::ViewOutput out;
    bool           turned = false;
    for (int t = 0; t < 40; t++) {
        self.timeMs += 50;
        vc.Step(self, h, r, out);
        turned = turned || out.mode == hb::VIEW_SOUND;
    }
    HB_CHECK(!turned);
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
    TestHearingSide(b);
    TestHoldGivesWay(b);
    TestDamageTurn(b);
    TestLookChain(b);
    TestNearSoundGivesWay(b);
    TestLeanWall(b);
    TestLeanEnemy(b);
    TestLeanSwitch(b);
    TestStrafeMemory(b);
    TestHiddenDistance(b);
    TestRouteTurns(b);
    TestTravelLead(b);
    TestNearSide(b);
    TestUnstick(b);
    TestFarPullWall(b);
    TestSubsteps();
    TestEye();
    TestPerceiverDead(b);
    TestHunch(b);
    return hbtest::Finish("test_hb_modules");
}
