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
#include "hb_substep.h"
#include "hb_test.h"
#include "hb_trigger.h"
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
    // pooled human targets (REPORT section 7): holds ~300 ms, 72% direct reversal, 58% pure strafe, 31% diagonal
    HB_CHECK(med >= 200.0 && med <= 400.0);
    HB_CHECK(rev > 0.55 && rev < 0.9);
    HB_CHECK(pure > 0.4 && pure < 0.75);
    HB_CHECK(diag > 0.15 && diag < 0.45);

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
    int still = 0;
    for (int t = 0; t < 20000; t++) {
        vc.Step(self, h, r, out);
        still += out.still;
        self.viewYaw   = hb::Wrap180(self.viewYaw + out.yawDelta);
        self.viewPitch = hb::Clamp(self.viewPitch + out.pitchDelta, -85.0f, 85.0f);
        self.timeMs += 50;
    }
    HB_REPORT("hidden: still share %.2f", still / 20000.0);
    HB_CHECK(still / 20000.0 > 0.15 && still / 20000.0 < 0.5);
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
    TestView(b);
    TestSubsteps();
    TestEye();
    return hbtest::Finish("test_hb_modules");
}
