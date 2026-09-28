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
static constexpr float VETO_LOGIT       = 4.0f;

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
    m_sideAge   = 1;
    m_fwd       = 0;
    m_fwdAge    = 1;
    m_lean      = 0;
    m_leanAge   = 1;
    m_ctx       = -1;
    m_ctxAge    = 10000;
    m_crouchAge = 0;
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
// it), so walls only raise the fitted hazard of letting go.
int Mover::VetoMask(const MoveInput& in) const
{
    int mask = LedgeMask(in);
    for (int c = 0; c < NUM_CHORDS; c++) {
        if (c != CHORD_NEUTRAL && in.clearance[c] < m_p->vetoClearance) {
            mask |= Bit(c);
        }
    }
    return mask;
}

// How well a chord goes where the bot wants to travel: 1 straight there, -1 straight away, 0 standing.
float Mover::NavAlign(const MoveInput& in, int chord) const
{
    if (chord == CHORD_NEUTRAL) {
        return 0.0f;
    }
    return std::cos((ChordAngle(chord) - in.navBearing) * DEG2RAD);
}

// How much changing one key (to its best allowed other state) would improve the alignment.
float Mover::NavGain(const MoveInput& in, bool sideKey, int veto) const
{
    const float cur  = NavAlign(in, MakeChord(m_fwd, m_side));
    float       best = -2.0f;
    for (int v = -1; v <= 1; v++) {
        if (v == (sideKey ? m_side : m_fwd)) {
            continue;
        }
        const int c = sideKey ? MakeChord(m_fwd, v) : MakeChord(v, m_side);
        if (veto & Bit(c)) {
            continue;
        }
        best = std::max(best, NavAlign(in, c));
    }
    return best > -1.5f ? best - cur : 0.0f;
}

float Mover::CtxChange(const KeyModel& k, int row, int ctx) const
{
    const int cb = BinIndex(m_ctxAge, m_p->ctxAgeEdges);
    if (cb >= static_cast<int>(m_p->ctxAgeEdges.size()) - 1) {
        return 0.0f;
    }
    return k.ctxChangeLogit.At(ctx, row, cb);
}

void Mover::StepSide(const MoveInput& in, const StyleOffsets& style, int ledge, int veto, double u1, double u2, int& side, float& p)
{
    const MovementModel& m     = *m_p;
    const float          scale = style.holdScale > 0.05f ? style.holdScale : 1.0f;
    const int            ab    = BinIndex(std::max(1.0f, m_sideAge / scale), m.ageEdges);

    float z = m_sideSpawn ? SpawnLogit(m_spawn->sideSwitchP, m_side != 0 ? 1 : 0, m_sideAge) : NAN;
    if (std::isnan(z)) {
        z = m.side.switchLogit.At(in.ctx, m_side + 1, m_fwd + 1, ab);
    }
    if (m_side != 0) {
        z += m.side.wallLogit[BinIndex(in.clearance[MakeChord(0, m_side)], m.clearEdges)];
        if (in.wallPressMs > 0.0f) {
            z += m.wallPressureLogit * Clamp(in.wallPressMs / 350.0f, 0.0f, 1.0f);
        }
        if (ledge & Bit(MakeChord(0, m_side))) {
            z += VETO_LOGIT;
        }
    }
    if (in.losChanged) {
        z += m.side.losChangeLogit;
    }
    z += CtxChange(m.side, m_side != 0 ? 1 : 0, in.ctx);
    if (in.navValid && in.urgency > 0.0f) {
        z += m.navSwitchLogit * in.urgency * (NavGain(in, true, veto) - 0.3f);
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
        za = Logit(m.reverseP.At(in.ctx, m_fwd + 1, BinIndex(m_sideAge, m.ageEdges))) + style.reverseLogit;
    } else {
        a  = 1;       // right
        b  = -1;      // left
        za = Logit(m.rightP.At(in.ctx, m_fwd + 1));
    }
    if (in.navValid && in.urgency > 0.0f) {
        za += m.navChoiceLogit * in.urgency * (NavAlign(in, MakeChord(m_fwd, a)) - NavAlign(in, MakeChord(m_fwd, b)));
    }
    const bool vetoA = a != 0 && (veto & Bit(MakeChord(0, a))) != 0;
    const bool vetoB = b != 0 && (veto & Bit(MakeChord(0, b))) != 0;
    int        pick  = u2 < Sigmoid(za) ? a : b;
    if (vetoA && vetoB) {
        pick = 0;
    } else if (pick == a && vetoA) {
        pick = b;
    } else if (pick == b && vetoB) {
        pick = a;
    }
    side = pick;
}

void Mover::StepFwd(const MoveInput& in, const StyleOffsets& style, int ledge, int veto, double u1, Rng& rng, int& fwd, float& p)
{
    const MovementModel& m  = *m_p;
    const int            ab = BinIndex(m_fwdAge, m.ageEdges);

    float z = m_fwdSpawn ? SpawnLogit(m_spawn->fwdSwitchP, m_fwd + 1, m_fwdAge) : NAN;
    if (std::isnan(z)) {
        z = m.fwd.switchLogit.At(in.ctx, m_fwd + 1, m_side + 1, ab);
    }
    if (m_fwd != 0) {
        z += m.fwd.wallLogit[BinIndex(in.clearance[MakeChord(m_fwd, 0)], m.clearEdges)];
        if (in.wallPressMs > 0.0f) {
            z += m.wallPressureLogit * Clamp(in.wallPressMs / 350.0f, 0.0f, 1.0f);
        }
        if (ledge & Bit(MakeChord(m_fwd, 0))) {
            z += VETO_LOGIT;
        }
    }
    if (in.losChanged) {
        z += m.fwd.losChangeLogit;
    }
    z += CtxChange(m.fwd, m_fwd + 1, in.ctx);
    // the diagonal habit keeps forward held while strafing
    if (m_fwd == 1 && m_side != 0) {
        z -= 0.5f * style.diagLogit;
    }
    if (in.navValid && in.urgency > 0.0f) {
        z += m.navSwitchLogit * in.urgency * (NavGain(in, false, veto) - 0.3f);
    }
    p   = Sigmoid(z);
    fwd = m_fwd;
    if (u1 >= p) {
        return;
    }

    // the next state: a categorical over the other two, tilted toward or away from the enemy by distance
    float k = m.approach.At(in.ctx, BinIndex(in.enemyDist, m.distEdges));
    if (in.enemyReloading) {
        k += m.approachEnemyReload;
    }
    const float cosb = in.enemyKnown ? std::cos(in.enemyBearing * DEG2RAD) : 0.0f;
    double      zz[3];
    double      zmax = -1e30;
    for (int to = -1; to <= 1; to++) {
        if (to == m_fwd || (to != 0 && (veto & Bit(MakeChord(to, 0))))) {
            zz[to + 1] = -1e30;
            continue;
        }
        double v = m.fwdNext.At(in.ctx, m_fwd + 1, m_side + 1, to + 1) + k * static_cast<float>(to - m_fwd) * cosb;
        if (to == 1 && m_side != 0) {
            v += 0.5f * style.diagLogit;
        }
        if (in.navValid && in.urgency > 0.0f) {
            v += m.navChoiceLogit * in.urgency * NavAlign(in, MakeChord(to, m_side));
        }
        zz[to + 1] = v;
        zmax       = std::max(zmax, v);
    }
    if (zmax <= -1e29) {
        fwd = 0;
        return;
    }
    double w[3];
    for (int i = 0; i < 3; i++) {
        w[i] = zz[i] <= -1e29 ? 0.0 : std::exp(zz[i] - zmax);
    }
    fwd = rng.Categorical(w, 3) - 1;
}

void Mover::StepLean(const MoveInput& in, const StyleOffsets& style, Rng& rng)
{
    const MovementModel& m  = *m_p;
    const int            ab = BinIndex(m_leanAge, m.leanAgeEdges);
    // a context change shifts the chance of leaving the current lean state
    float     change = 0.0f;
    const int cb     = BinIndex(m_ctxAge, m.leanCtxAgeEdges);
    if (cb < static_cast<int>(m.leanCtxAgeEdges.size()) - 1) {
        change = m.leanCtxChangeLogit.At(in.ctx, m_lean != 0 ? 1 : 0, cb);
    }
    // style and the calibrated per-context habit: lean on (+) / off (-)
    const float habit = style.leanLogit + m.leanCtxLogit[in.ctx];
    double      p[3];
    if (m_lean == 0) {
        const int rel = m_side + 1;
        for (int k = 0; k < 3; k++) {
            p[k] = m.leanNext.At(0, in.ctx, ab, rel, k);
        }
        const double on = 1.0 - p[1];
        if (on > 1e-6 && on < 1.0 - 1e-6) {
            const double on2 = Sigmoid(Logit(static_cast<float>(on)) + habit + change);
            const double s   = on2 / on;
            p[0] *= s;
            p[2] *= s;
            p[1] = 1.0 - on2;
        }
        const int o = rng.Categorical(p, 3);
        if (o != 1) {
            m_lean    = o - 1;
            m_leanAge = 1;
        } else {
            m_leanAge++;
        }
    } else {
        const int rel = m_side == 0 ? 0 : (m_side == m_lean ? 1 : 2);
        for (int k = 0; k < 3; k++) {
            p[k] = m.leanNext.At(1, in.ctx, ab, rel, k);
        }
        const double stay = p[0];
        if (stay > 1e-6 && stay < 1.0 - 1e-6) {
            const double stay2 = Sigmoid(Logit(static_cast<float>(stay)) + habit - change);
            const double s     = (1.0 - stay2) / (1.0 - stay);
            p[1] *= s;
            p[2] *= s;
            p[0] = stay2;
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

void Mover::StepStance(const MoveInput& in, const StyleOffsets& style, Rng& rng, MoveOutput& out)
{
    const MovementModel& m = *m_p;
    // fixed draws per tick
    const double uc = rng.Uniform(), ucr = rng.Uniform();
    const double uj = rng.Uniform(), ujr = rng.Uniform();
    const double uw = rng.Uniform(), uwr = rng.Uniform();

    out.crouch = StepKey(m.crouch, in.ctx, style.crouchMult, true, uc, ucr, m_crouchAge);
    out.jump   = StepKey(m.jump, in.ctx, style.jumpMult, in.allowJump && in.onGround && !out.crouch, uj, ujr, m_jumpAge);
    if (out.crouch && out.jump) {
        out.jump  = false;
        m_jumpAge = 0;
    }
    out.walk = StepKey(m.walk, in.ctx, style.walkMult, true, uw, uwr, m_walkAge);
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

    // both keys decide on last tick's state, like two fingers
    int   side, fwd;
    float ps, pf;
    StepSide(in, style, ledge, veto, us1, us2, side, ps);
    StepFwd(in, style, ledge, veto, uf1, moveRng, fwd, pf);
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
    StepLean(in, style, stanceRng);
    StepStance(in, style, stanceRng, out);
    out.chord = MakeChord(m_fwd, m_side);
    out.lean  = m_lean;
}

} // namespace hb
