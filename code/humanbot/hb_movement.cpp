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
// hb_movement.cpp: side key, forward key, lean and stance processes.

#include "hb_movement.h"
#include "hb_math.h"

#include <algorithm>
#include <cmath>

namespace hb
{

static constexpr float LEDGE_VETO_DEPTH = 240.0f;
// Out of a nook (set by hand on practice-map captures, 2026-10-04): a bot travelling somewhere (hunting or out of the
// spawn, route urgency at least UNSTICK_URGENCY), that has held no key for UNSTICK_TICKS and whose route runs into a wall
// it touches takes the open chord nearest the route. Standing with walls around, the route pull leans away from every
// open direction and the fitted keys start ever more slowly the longer one stands: on dm/brownffa the bots stood like
// that for seconds (frozen over 2 s 1.8 times a minute, people 0.3). Not after an enemy just lost: people hold the
// corner it went out of sight at.
static constexpr int   UNSTICK_TICKS    = 20;
static constexpr float UNSTICK_URGENCY  = 0.5f;
static constexpr float VETO_LOGIT       = 4.0f;
static constexpr float WALL_TOUCH       = 4.0f;    // units: the box is touching the wall (the wall reflex)

static int Bit(int chord)
{
    return 1 << chord;
}

void Mover::Init(const MovementModel *params, const SpawnModel *spawn)
{
    m_p     = params;
    m_spawn = spawn;
    Reset();
}

void Mover::Reset()
{
    m_side      = 0;
    m_lastSide  = 0;
    m_sideAge   = 1;
    m_fwd       = 0;
    m_fwdAge    = 1;
    m_lean      = 0;
    m_leanAge   = 1;
    m_ctx       = -1;
    m_ctxAge    = 10000;
    m_crouchAge = 0;
    m_crouchedTicks = 0;
    m_jumpAge   = 0;
    m_walkAge   = 0;
    m_sideSpawn = false;
    m_fwdSpawn  = false;
}

void Mover::Spawn(int chord, MoveOutput& out)
{
    Reset();
    m_fwd       = ChordFwd(chord);
    m_side      = ChordSide(chord);
    m_lastSide  = m_side;
    m_sideSpawn = m_spawn != nullptr;
    m_fwdSpawn  = m_spawn != nullptr;
    out         = MoveOutput();
    out.chord   = MakeChord(m_fwd, m_side);
}

// Switch logit of a key still in its first run after the respawn, or NAN once the spawn run is over.
float Mover::SpawnLogit(const Table& t, int row, int age) const
{
    if (!m_spawn || age >= m_spawn->ageEdges.back()) {
        return NAN;
    }
    return Logit(t.At(row, BinIndex(age, m_spawn->ageEdges)));
}

void Mover::SetKeys(int fwd, int side)
{
    m_fwd     = ClampI(fwd, -1, 1);
    m_side    = ClampI(side, -1, 1);
    if (m_side != 0) {
        m_lastSide = m_side;
    }
    m_fwdAge  = 1;
    m_sideAge = 1;
}

float Mover::ChordAngle(int chord)
{
    const int f = ChordFwd(chord);
    const int s = ChordSide(chord);
    if (!f && !s) {
        return 0.0f;
    }
    return std::atan2(static_cast<float>(-s), static_cast<float>(f)) * RAD2DEG;
}

// Chords that lead over a ledge deep enough to hurt (fall damage starts near
// 250 units): never pressed, and let go of at once.
int Mover::LedgeMask(const MoveInput& in) const
{
    int mask = 0;
    for (int c = 0; c < NUM_CHORDS; c++) {
        if (c != CHORD_NEUTRAL && in.drop[c] > LEDGE_VETO_DEPTH) {
            mask |= Bit(c);
        }
    }
    return mask;
}

// Chords a new key press may not go toward: ledges and walls closer than the
// veto clearance. People do keep holding a key into a wall (they slide along
// it), so walls only shift fitted odds: of letting go (in the key's direction,
// and along the diagonal) and of what a key changes to (ChordWall).
// Every probe blocked: the box starts in solid, and the probes say nothing about where the walls
// are. The wall reflex then stands down (it would freeze the bot); the stuck recovery frees it.
static bool ProbesBlind(const MoveInput& in)
{
    for (int c = 0; c < NUM_CHORDS; c++) {
        if (c != CHORD_NEUTRAL && in.clearance[c] >= WALL_TOUCH) {
            return false;
        }
    }
    return true;
}

int Mover::VetoMask(const MoveInput& in) const
{
    int         mask  = LedgeMask(in);
    const bool  reflex = m_p->wallReflexMs > 0.0f && !ProbesBlind(in);
    const float touch  = reflex ? std::max(m_p->vetoClearance, WALL_TOUCH) : m_p->vetoClearance;
    for (int c = 0; c < NUM_CHORDS; c++) {
        if (c != CHORD_NEUTRAL && in.clearance[c] < touch) {
            mask |= Bit(c);
        }
    }
    return mask;
}

// The wall reflex. People see a wall coming and do not run into it; the key processes, fitted on
// people who steer along walls with the mouse, barely react to walls (the bots touched walls 7x as
// often). A chord's wall counts when it is reached within wallReflexMs at the current speed along the
// chord's direction, or touched.
bool Mover::WallAhead(const MoveInput& in, int chord) const
{
    if (m_p->wallReflexMs <= 0.0f || chord == CHORD_NEUTRAL || ProbesBlind(in)) {
        return false;
    }
    return WallMargin(in, chord) < 0.0f;
}

// How far beyond the reflex's reach a chord's wall lies (negative: reached within wallReflexMs, or touched).
float Mover::WallMargin(const MoveInput& in, int chord) const
{
    const float a     = ChordAngle(chord) * DEG2RAD;   // + = left
    const float v     = in.velFwd * std::cos(a) - in.velRight * std::sin(a);
    const float reach = std::max(v, 0.0f) * m_p->wallReflexMs * 0.001f + WALL_TOUCH;
    return in.clearance[chord] - reach;
}

// What the wall reflex does this tick, from last tick's keys. Only the direction the bot goes counts: a
// diagonal down a corridor has walls ahead of both its keys' own directions (people walk corridors so, the view
// about 40 deg off the path), and letting go of a key for those made the bots zig-zag and stop. People slide
// along a wall instead of stopping at it: a diagonal into a wall lets go of the key whose own direction is the
// more open, forward into a wall adds a strafe toward the more open front diagonal (people turn forward
// into a diagonal there, 217 per 1000 ticks, and stop 32), and a strafe into a wall adds forward when the front
// diagonal on its side is open (people add forward in 22% of their key changes there and stop in 28%; letting go,
// the bots stopped in 49%, and they stood at walls 5% of their hidden time against people's 3%).
void Mover::Reflex(const MoveInput& in, int veto)
{
    m_reflexSide = false;
    m_reflexFwd  = false;
    m_slideSide  = 0;
    m_slideFwd   = false;
    if (!WallAhead(in, MakeChord(m_fwd, m_side))) {
        return;
    }
    if (m_fwd != 0 && m_side != 0) {
        const float mf = WallMargin(in, MakeChord(m_fwd, 0));
        const float ms = WallMargin(in, MakeChord(0, m_side));
        if (mf < 0.0f && ms < 0.0f) {
            m_reflexSide = true;
            m_reflexFwd  = true;
        } else if (mf >= ms) {
            m_reflexSide = true;
        } else {
            m_reflexFwd = true;
        }
    } else if (m_fwd == 1) {
        float best = 0.0f;
        for (int s = -1; s <= 1; s += 2) {
            const int c = MakeChord(1, s);
            if (veto & Bit(c)) {
                continue;
            }
            const float mg = WallMargin(in, c);
            if (mg >= best) {
                best        = mg;
                m_slideSide = s;
            }
        }
        m_reflexFwd = m_slideSide == 0;
    } else if (m_fwd != 0) {
        m_reflexFwd = true;
    } else if (!(veto & Bit(MakeChord(1, m_side))) && WallMargin(in, MakeChord(1, m_side)) >= 0.0f) {
        m_slideFwd = true;
    } else {
        m_reflexSide = true;
    }
}

// How well a chord goes where the bot wants to travel: 1 straight there, -1 straight away, 0 standing.
float Mover::NavAlign(const MoveInput& in, int chord) const
{
    if (chord == CHORD_NEUTRAL) {
        return 0.0f;
    }
    return std::cos((ChordAngle(chord) - in.navBearing) * DEG2RAD);
}

// How much changing one key (to its best allowed other state) would improve the alignment;
// `side` is the side key the change combines with.
float Mover::NavGain(const MoveInput& in, bool sideKey, int veto, int side) const
{
    const float cur  = NavAlign(in, MakeChord(m_fwd, side));
    float       best = -2.0f;
    for (int v = -1; v <= 1; v++) {
        if (v == (sideKey ? side : m_fwd)) {
            continue;
        }
        const int c = sideKey ? MakeChord(m_fwd, v) : MakeChord(v, side);
        if (veto & Bit(c)) {
            continue;
        }
        best = std::max(best, NavAlign(in, c));
    }
    return best > -1.5f ? best - cur : 0.0f;
}

// A fitted wall term by the clearance of a chord's direction (0 for neutral, or when the model has none).
static float ChordWall(const std::vector<float>& logit, const MovementModel& m, const MoveInput& in, int chord)
{
    if (logit.empty() || chord == CHORD_NEUTRAL) {
        return 0.0f;
    }
    return logit[BinIndex(in.clearance[chord], m.clearEdges)];
}

float Mover::CtxChange(const KeyModel& k, int row, int ctx) const
{
    const int cb = BinIndex(m_ctxAge, m_p->ctxAgeEdges);
    if (cb >= static_cast<int>(m_p->ctxAgeEdges.size()) - 1) {
        return 0.0f;
    }
    return k.ctxChangeLogit.At(ctx, row, cb);
}

// With the enemy hidden, how readily a key changes by how far away it is believed to be: the farther, the longer people
// keep forward held and the less readily they start a strafe (row: side key strafing or not, -1 for the forward key).
float Mover::HidDist(const KeyModel& k, int row, int fwd, const MoveInput& in) const
{
    if (k.hidDistLogit.v.empty() || !in.enemyKnown || (in.ctx != CTX_HIDDEN_NOFIRE && in.ctx != CTX_HIDDEN_FIRE)) {
        return 0.0f;
    }
    const int b = BinIndex(in.enemyDist, m_p->hidDistEdges);
    return row < 0 ? k.hidDistLogit.At(fwd + 1, b) : k.hidDistLogit.At(row, fwd + 1, b);
}

// How firmly the keys follow the route: the urgency, more so the farther away the enemy is believed to be while it is
// hidden (people run to a far fight and strafe and peek near one). Not while pressing into a wall: three times the
// pull outweighed letting go of a key held into it (in the arena bots pushed against pillars for seconds).
float Mover::NavPull(const MoveInput& in) const
{
    const MovementModel& m = *m_p;
    float                k = in.urgency;
    if (m.navFarMult != 1.0f && m.navFarDist > m.navFarNear && in.enemyKnown && in.wallPressMs <= 0.0f
        && (in.ctx == CTX_HIDDEN_NOFIRE || in.ctx == CTX_HIDDEN_FIRE)) {
        k *= 1.0f + (m.navFarMult - 1.0f) * Clamp((in.enemyDist - m.navFarNear) / (m.navFarDist - m.navFarNear), 0.0f, 1.0f);
    }
    return k;
}

void Mover::StepSide(const MoveInput& in, const StyleOffsets& style, int ledge, int veto, double u1, double u2, int& side, float& p)
{
    const MovementModel& m     = *m_p;
    const float          scale = style.holdScale > 0.05f ? style.holdScale : 1.0f;
    const int            ab    = BinIndex(std::max(1.0f, m_sideAge / scale), m.ageEdges);

    float z = m_sideSpawn ? SpawnLogit(m_spawn->sideSwitchP, m_side != 0 ? 1 : 0, m_sideAge) : NAN;
    if (std::isnan(z)) {
        z = m.side.switchLogit.At(in.ctx, m_side + 1, m_fwd + 1, ab);
        if (m_side == 0 && m_sideAge == 1 && m_lastSide != 0) {
            // the tick after a strafe was let go: how readily a strafe follows at once is a habit (people 25-73%;
            // the stoppers turn this way rather than reverse directly)
            z += style.counterLogit;
        }
    }
    if (m_side != 0) {
        z += m.side.wallLogit[BinIndex(in.clearance[MakeChord(0, m_side)], m.clearEdges)];
        if (m_fwd != 0) {
            // a diagonal into a wall reads open in both key directions
            z += ChordWall(m.side.diagWallLogit, m, in, MakeChord(m_fwd, m_side));
        }
        if (m_reflexSide) {
            z += m.wallReflexLogit;
        }
        if (in.wallPressMs > 0.0f) {
            z += m.wallPressureLogit * Clamp(in.wallPressMs / 350.0f, 0.0f, 1.0f);
        }
        if (ledge & Bit(MakeChord(0, m_side))) {
            z += VETO_LOGIT;
        }
    }
    if (m_side == 0 && m_slideSide != 0) {
        // forward into a wall: slide along it
        z += m.wallReflexLogit;
    }
    if (in.losChanged) {
        z += m.side.losChangeLogit;
    }
    z += CtxChange(m.side, m_side != 0 ? 1 : 0, in.ctx);
    z += HidDist(m.side, m_side != 0 ? 1 : 0, m_fwd, in);
    // the calibrated strafe habit only shortens or stretches the pauses between strafes: how strafes
    // end (hold length, reverse or let go) stays as fitted
    if (m_side == 0) {
        z += m.sideCtxLogit[in.ctx];
    }
    if (in.navValid && in.urgency > 0.0f) {
        z += m.navSwitchLogit * NavPull(in) * (NavGain(in, true, veto, m_side) - m.navDeadband);
    }
    p    = Sigmoid(z);
    side = m_side;
    if (u1 >= p) {
        return;
    }

    // the two states the key can change to, and the logit of the first
    int   a, b;
    float za;
    if (m_side != 0) {
        a  = -m_side; // reverse
        b  = 0;       // let go
        za = Logit(m.reverseP.At(in.ctx, m_fwd + 1, BinIndex(m_sideAge, m.ageEdges))) + m.reverseLogit + style.reverseLogit;
    } else {
        a = 1;        // right
        b = -1;       // left
        if (m_lastSide != 0 && !m.oppositeP.v.empty()) {
            // after a stop people press the other side: at once 95-98% of the time
            const float opp = m.oppositeP.At(in.ctx, BinIndex(m_sideAge, m.oppositeGapEdges));
            za              = Logit(m_lastSide < 0 ? opp : 1.0f - opp);
        } else {
            za = Logit(m.rightP.At(in.ctx, m_fwd + 1));
        }
    }
    if (in.navValid && in.urgency > 0.0f) {
        za += m.navChoiceLogit * NavPull(in) * (NavAlign(in, MakeChord(m_fwd, a)) - NavAlign(in, MakeChord(m_fwd, b)));
    }
    // people do not start a key into a wall they are touching
    za += ChordWall(m.side.choiceWallLogit, m, in, MakeChord(m_fwd, a)) - ChordWall(m.side.choiceWallLogit, m, in, MakeChord(m_fwd, b));
    const bool vetoA = a != 0 && (veto & Bit(MakeChord(0, a))) != 0;
    const bool vetoB = b != 0 && (veto & Bit(MakeChord(0, b))) != 0;
    int        pick  = u2 < Sigmoid(za) ? a : b;
    if (m_side == 0 && m_slideSide != 0) {
        pick = (veto & Bit(MakeChord(0, m_slideSide))) != 0 ? 0 : m_slideSide;
    } else if (vetoA && vetoB) {
        pick = 0;
    } else if (pick == a && vetoA) {
        pick = b;
    } else if (pick == b && vetoB) {
        pick = a;
    }
    side = pick;
}

void Mover::StepFwd(const MoveInput& in, const StyleOffsets& style, int ledge, int veto, int side, double u1, Rng& rng,
                    int& fwd, float& p)
{
    const MovementModel& m  = *m_p;
    const int            ab = BinIndex(m_fwdAge, m.ageEdges);
    // a wall ahead: let go of the key (a person stops pressing into it, and does not back off)
    const bool wallAhead = m_reflexFwd;

    float z = m_fwdSpawn ? SpawnLogit(m_spawn->fwdSwitchP, m_fwd + 1, m_fwdAge) : NAN;
    if (std::isnan(z)) {
        z = m.fwd.switchLogit.At(in.ctx, m_fwd + 1, side + 1, ab);
    }
    if (m_fwd != 0) {
        z += m.fwd.wallLogit[BinIndex(in.clearance[MakeChord(m_fwd, 0)], m.clearEdges)];
        if (side != 0) {
            z += ChordWall(m.fwd.diagWallLogit, m, in, MakeChord(m_fwd, side));
        }
        if (wallAhead) {
            z += m.wallReflexLogit;
        }
        if (in.wallPressMs > 0.0f) {
            z += m.wallPressureLogit * Clamp(in.wallPressMs / 350.0f, 0.0f, 1.0f);
        }
        if (ledge & Bit(MakeChord(m_fwd, 0))) {
            z += VETO_LOGIT;
        }
    }
    if (m_slideFwd) {
        // a strafe into a wall: run along it on the diagonal
        z += m.wallReflexLogit;
    }
    if (in.losChanged) {
        z += m.fwd.losChangeLogit;
    }
    z += CtxChange(m.fwd, m_fwd + 1, in.ctx);
    z += HidDist(m.fwd, -1, m_fwd, in);
    // the calibrated forward habit pulls toward forward; letting go of forward stays as fitted
    if (m_fwd != 1) {
        z += m.fwdCtxLogit[in.ctx];
    }
    // the diagonal habit keeps forward held while strafing
    if (m_fwd == 1 && side != 0) {
        z -= 0.5f * style.diagLogit;
    }
    if (in.navValid && in.urgency > 0.0f) {
        z += m.navSwitchLogit * NavPull(in) * (NavGain(in, false, veto, side) - m.navDeadband);
    }

    // the next state: a categorical over the other two, tilted toward or away from the enemy by distance
    float k = m.approach.At(in.ctx, BinIndex(in.enemyDist, m.distEdges));
    if (in.enemyReloading) {
        k += m.approachEnemyReload;
    }
    // the diagonal habit as a rate: how readily a strafe gains the forward key (from none or from back), the other
    // way keeping its fitted rate. As a tilt of the choice it made a low-diagonal style press back instead of
    // forward (from none while strafing: back 57-62% of the presses, people of those styles 22-34%)
    const bool  rateHabit = side != 0 && m_fwd != 1;
    const float cosb = in.enemyKnown ? std::cos(in.enemyBearing * DEG2RAD) : 0.0f;
    double      zz[3];
    double      zmax = -1e30;
    for (int to = -1; to <= 1; to++) {
        if (to == m_fwd || (to != 0 && (veto & Bit(MakeChord(to, 0)))) || (wallAhead && to == -m_fwd)) {
            zz[to + 1] = -1e30;
            continue;
        }
        double v = m.fwdNext.At(in.ctx, m_fwd + 1, side + 1, to + 1) + k * static_cast<float>(to - m_fwd) * cosb;
        if (to == 1) {
            v += m.fwdCtxLogit[in.ctx];
        }
        if (in.navValid && in.urgency > 0.0f) {
            v += m.navChoiceLogit * NavPull(in) * NavAlign(in, MakeChord(to, side));
        }
        v += ChordWall(m.fwd.choiceWallLogit, m, in, MakeChord(to, side));
        zz[to + 1] = v;
        zmax       = std::max(zmax, v);
    }
    double w[3];
    double wsum = 0.0;
    for (int i = 0; i < 3; i++) {
        w[i] = zz[i] <= -1e29 ? 0.0 : std::exp(zz[i] - zmax);
        wsum += w[i];
    }
    p = Sigmoid(z);
    if (rateHabit && wsum > 0.0 && w[2] > 0.0) {
        const double q  = w[2] / wsum;
        const double kh = std::exp(0.5 * style.diagLogit);
        p               = static_cast<float>(std::min(0.999, p * (q * kh + (1.0 - q))));
        w[2] *= kh;
    }
    fwd = m_fwd;
    if (u1 >= p) {
        return;
    }
    if (zmax <= -1e29) {
        fwd = 0;
        return;
    }
    if (m_slideFwd && w[2] > 0.0) {
        fwd = 1;
        return;
    }
    fwd = rng.Categorical(w, 3) - 1;
}

// People lean away from a flat wall at their side (in sight a third of their leans there go into it, the bots'
// half) and around an edge just ahead on one side (77%, the bots' 60%).
float Mover::LeanWallLogit(const MoveInput& in, int side) const
{
    const MovementModel& m = *m_p;
    const float          c = in.clearance[MakeChord(0, side)];
    const float          w = Clamp(1.0f - c / m.leanWallRange, 0.0f, 1.0f);
    if (w <= 0.0f) {
        return 0.0f;
    }
    const bool edge = in.clearance[MakeChord(1, side)] >= m.leanEdgeOpen;
    return w * (m.leanWallLogit + (edge ? m.leanEdgeLogit : 0.0f));
}

void Mover::StepLean(const MoveInput& in, const StyleOffsets& style, int side, Rng& rng)
{
    const MovementModel& m  = *m_p;
    const int            ab = BinIndex(m_leanAge, m.leanAgeEdges);
    // the opponent-dead time has lean habits of its own
    const int lctx = in.enemyDead ? LEAN_CTX_DEAD : in.ctx;
    // a context change shifts the chance of leaving the current lean state
    float     change = 0.0f;
    const int cb     = BinIndex(m_ctxAge, m.leanCtxAgeEdges);
    if (!in.enemyDead && cb < static_cast<int>(m.leanCtxAgeEdges.size()) - 1) {
        change = m.leanCtxChangeLogit.At(in.ctx, m_lean != 0 ? 1 : 0, cb);
    }
    // style and the calibrated per-context habit: lean on (+) / off (-)
    const float habit = style.leanLogit + (in.enemyDead ? 0.0f : m.leanCtxLogit[in.ctx]);
    // the walls beside move the side a lean takes, not whether there is one: right minus left
    const float wall = LeanWallLogit(in, 1) - LeanWallLogit(in, -1);
    double      p[3];
    if (m_lean == 0) {
        const int rel = side + 1;
        for (int k = 0; k < 3; k++) {
            p[k] = m.leanNext.At(0, lctx, ab, rel, k);
        }
        const double on = 1.0 - p[1];
        if (on > 1e-6 && on < 1.0 - 1e-6) {
            const double on2 = Sigmoid(Logit(static_cast<float>(on)) + habit + change);
            const double s   = on2 / on;
            p[0] *= s;
            p[2] *= s;
            p[1] = 1.0 - on2;
        }
        const double lr = p[0] + p[2];
        if (wall != 0.0f && lr > 1e-9 && p[0] > 1e-9 && p[2] > 1e-9) {
            const double right = Sigmoid(Logit(static_cast<float>(p[2] / lr)) + wall);
            p[2]               = lr * right;
            p[0]               = lr * (1.0 - right);
        }
        const int o = rng.Categorical(p, 3);
        if (o != 1) {
            m_lean    = o - 1;
            m_leanAge = 1;
        } else {
            m_leanAge++;
        }
    } else {
        // the strafe against the lean: the tick it turns so (the side changed this tick) is where people decide
        const bool against = side != 0 && side != m_lean;
        const int  rel     = side == 0 ? 0 : (!against ? 1 : (m.leanRels == 4 && m_sideAge == 1 ? 3 : 2));
        for (int k = 0; k < 3; k++) {
            p[k] = m.leanNext.At(1, lctx, ab, rel, k);
        }
        // the habit and a context change govern letting go only. What a lean does with the strafe against it is a
        // style of its own (the presser people flip it across with the strafe, the strafers let go of it, most of
        // all while firing, the stoppers mostly keep it): there the fitted chain's own odds by context hold, shifted
        // by the style's let-go and switch. The habit no longer holds a lean against the strafe: its calibrated
        // per-context part (+2.2 hidden firing) cancelled the strafers' let-go in fights
        const float letGo   = against ? -style.leanDropLogit : habit;
        double      release = p[1], swap = p[2];
        if (release > 1e-6 && release < 1.0 - 1e-6) {
            release = Sigmoid(Logit(static_cast<float>(release)) - letGo + change);
        }
        if (against && swap > 1e-6 && swap < 1.0 - 1e-6) {
            swap = Sigmoid(Logit(static_cast<float>(swap)) + style.leanSwitchLogit);
        }
        if (release + swap > 1.0) {
            const double s = 1.0 / (release + swap);
            release *= s;
            swap *= s;
        }
        p[0] = 1.0 - release - swap;
        p[1] = release;
        p[2] = swap;
        // stay on this side or switch to the other
        const double ss = p[0] + p[2];
        if (wall != 0.0f && ss > 1e-9 && p[0] > 1e-9 && p[2] > 1e-9) {
            const double here = Sigmoid(Logit(static_cast<float>(p[0] / ss)) + static_cast<float>(m_lean) * wall);
            p[0]              = ss * here;
            p[2]              = ss * (1.0 - here);
        }
        const int o = rng.Categorical(p, 3);
        if (o == 0) {
            m_leanAge++;
        } else if (o == 1) {
            m_lean    = 0;
            m_leanAge = 1;
        } else {
            m_lean    = -m_lean;
            m_leanAge = 1;
        }
    }
}

// One key held down for a while: pressed with a per-context hazard, released
// with a hazard that depends on how long it has been held.
static bool StepKey(const StanceKeyModel& k, int ctx, float mult, bool allowPress, double uPress, double uRelease, int& age)
{
    if (age > 0) {
        const float h = k.releaseHazard[BinIndex(age, k.releaseAgeEdges)];
        if (uRelease < h) {
            age = 0;
            return false;
        }
        age++;
        return true;
    }
    if (allowPress && uPress < k.pressHazard[ctx] * mult) {
        age = 1;
        return true;
    }
    return false;
}

// Crouch toggles the stance: a press while standing ducks, the next press stands up.
// People dip for about 350 ms; the stand-up press comes by ticks crouched.
static bool StepToggle(const StanceKeyModel& k, const std::vector<float>& press, int ctx, float mult, bool crouched,
                       bool onGround, double uPress, double uRelease, int& age, int& crouchedTicks)
{
    crouchedTicks = crouched && onGround ? crouchedTicks + 1 : 0;
    if (age > 0) {
        return StepKey(k, ctx, mult, false, uPress, uRelease, age);
    }
    const float h = crouchedTicks > 0 ? k.upHazard[BinIndex(crouchedTicks, k.upAgeEdges)]
                                      : (onGround ? press[ctx] * mult : 0.0f);
    if (uPress < h) {
        age = 1;
        return true;
    }
    return false;
}

void Mover::StepStance(const MoveInput& in, const StyleOffsets& style, Rng& rng, MoveOutput& out)
{
    const MovementModel& m = *m_p;
    // fixed draws per tick
    const double uc = rng.Uniform(), ucr = rng.Uniform();
    const double uj = rng.Uniform(), ujr = rng.Uniform();
    const double uw = rng.Uniform(), uwr = rng.Uniform();

    if (m.crouch.upHazard.empty()) {
        out.crouch = StepKey(m.crouch, in.ctx, style.crouchMult, true, uc, ucr, m_crouchAge);
    } else {
        // people crouch three times as readily in the half second after a shot is heard
        const std::vector<float>& press =
            in.fireHeard && !m.crouch.pressHazardFire.empty() ? m.crouch.pressHazardFire : m.crouch.pressHazard;
        out.crouch = StepToggle(m.crouch, press, in.ctx, style.crouchMult, in.ducked, in.onGround, uc, ucr, m_crouchAge,
                                m_crouchedTicks);
    }
    out.jump   = StepKey(m.jump, in.ctx, style.jumpMult, in.allowJump && in.onGround && !out.crouch, uj, ujr, m_jumpAge);
    if (out.crouch && out.jump) {
        out.jump  = false;
        m_jumpAge = 0;
    }
    out.walk = StepKey(m.walk, in.ctx, m.walkMult * style.walkMult, true, uw, uwr, m_walkAge);
    // people let go of walk when a fight starts
    if (in.ctx == CTX_LOS_FIRE || in.ctx == CTX_HIDDEN_FIRE) {
        m_walkAge = 0;
        out.walk  = false;
    }
}

void Mover::Step(const MoveInput& in, const StyleOffsets& style, Rng& moveRng, Rng& stanceRng, MoveOutput& out)
{
    out = MoveOutput();
    // fixed draws per tick
    const double us1 = moveRng.Uniform();
    const double us2 = moveRng.Uniform();
    const double uf1 = moveRng.Uniform();

    if (m_ctx < 0) {
        m_ctx    = in.ctx;  // the context a life starts in is not a change
        m_ctxAge = 10000;
    } else if (in.ctx != m_ctx) {
        m_ctx    = in.ctx;
        m_ctxAge = 1;
    } else if (m_ctxAge < 10000) {
        m_ctxAge++;
    }
    const int ledge = LedgeMask(in);
    const int veto  = VetoMask(in);
    out.vetoMask    = veto;
    Reflex(in, veto);

    // both keys decide on last tick's state, like two fingers
    int   side, fwd;
    float ps, pf;
    StepSide(in, style, ledge, veto, us1, us2, side, ps);
    StepFwd(in, style, ledge, veto, m_side, uf1, moveRng, fwd, pf);
    // out of a nook (UNSTICK_*): standing, wanting to go, the route into a touched wall: the open chord nearest the route
    if (side == 0 && fwd == 0 && m_side == 0 && m_fwd == 0 && in.travelling && in.navValid && in.urgency >= UNSTICK_URGENCY
        && std::min(m_sideAge, m_fwdAge) >= UNSTICK_TICKS) {
        int bestAll = -1, bestOpen = -1;
        for (int c = 0; c < NUM_CHORDS; c++) {
            if (c == CHORD_NEUTRAL) {
                continue;
            }
            if (bestAll < 0 || NavAlign(in, c) > NavAlign(in, bestAll)) {
                bestAll = c;
            }
            if (!(veto & Bit(c)) && (bestOpen < 0 || NavAlign(in, c) > NavAlign(in, bestOpen))) {
                bestOpen = c;
            }
        }
        if (bestAll >= 0 && (veto & ~ledge & Bit(bestAll)) && bestOpen >= 0) {
            fwd  = bestOpen / 3 - 1;
            side = bestOpen % 3 - 1;
        }
    }
    // a diagonal both keys allow may still lead over a ledge: let go of forward first
    if (ledge & Bit(MakeChord(fwd, side))) {
        if (!(ledge & Bit(MakeChord(0, side)))) {
            fwd = 0;
        } else if (!(ledge & Bit(MakeChord(fwd, 0)))) {
            side = 0;
        } else {
            fwd  = 0;
            side = 0;
        }
    }
    if (side != m_side) {
        m_side      = side;
        m_sideAge   = 1;
        m_sideSpawn = false;
        if (side != 0) {
            m_lastSide = side;
        }
    } else if (m_sideAge < 100000) {
        m_sideAge++;
    }
    if (fwd != m_fwd) {
        m_fwd      = fwd;
        m_fwdAge   = 1;
        m_fwdSpawn = false;
    } else if (m_fwdAge < 100000) {
        m_fwdAge++;
    }
    out.pSwitch = 1.0f - (1.0f - ps) * (1.0f - pf);
    // people switch lean and strafe together: the lean follows this tick's side key (as fitted)
    StepLean(in, style, m_side, stanceRng);
    StepStance(in, style, stanceRng, out);
    out.chord = MakeChord(m_fwd, m_side);
    out.lean  = m_lean;
}

} // namespace hb
