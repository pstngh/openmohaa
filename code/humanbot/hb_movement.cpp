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
// hb_movement.cpp: chord, lean and stance processes.

#include "hb_movement.h"
#include "hb_math.h"

#include <cmath>
#include <algorithm>

namespace hb
{

static constexpr float LEDGE_VETO_DEPTH = 64.0f;

void Mover::Init(const MovementModel *params)
{
    m_p = params;
    Reset();
}

void Mover::Reset()
{
    m_chord      = CHORD_NEUTRAL;
    m_age        = 1;
    m_lean       = 0;
    m_leanAge    = 1;
    m_crouchLeft = 0;
    m_jumpLeft   = 0;
    m_walkLeft   = 0;
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

static bool Moving(int chord)
{
    return chord != CHORD_NEUTRAL;
}

int Mover::VetoMask(const MoveInput& in) const
{
    int mask = 0;
    for (int c = 0; c < NUM_CHORDS; c++) {
        if (!Moving(c)) {
            continue;
        }
        if (in.clearance[c] < m_p->vetoClearance || in.drop[c] > LEDGE_VETO_DEPTH) {
            mask |= 1 << c;
        }
    }
    return mask;
}

float Mover::SwitchProb(const MoveInput& in, const StyleOffsets& style, int chord, int age, int vetoMask) const
{
    const MovementModel& m     = *m_p;
    const float          scale = style.holdScale > 0.05f ? style.holdScale : 1.0f;
    const float          sage  = std::max(1.0f, age / scale);
    const int            ab    = BinIndex(sage, m.ageEdges);
    float                z     = m.switchLogit.At(in.ctx, chord, ab);
    if (Moving(chord)) {
        z += m.wallLogit[BinIndex(in.clearance[chord], m.clearEdges)];
    }
    if (in.losChanged) {
        z += m.losChangeLogit;
    }
    if (in.navValid && in.urgency > 0.0f) {
        float mis;
        if (Moving(chord)) {
            mis = 0.5f * (1.0f - std::cos((ChordAngle(chord) - in.navBearing) * DEG2RAD));
        } else {
            mis = 1.0f;
        }
        z += m.navSwitchLogit * in.urgency * (mis - 0.3f);
    }
    if (in.wallPressMs > 0.0f) {
        z += m.wallPressureLogit * Clamp(in.wallPressMs / 350.0f, 0.0f, 1.0f);
    }
    if (vetoMask & (1 << chord)) {
        z += 4.0f;
    }
    return Sigmoid(z);
}

void Mover::NextChordWeights(const MoveInput& in, const StyleOffsets& style, int chord, int vetoMask, double w[NUM_CHORDS]) const
{
    const MovementModel& m  = *m_p;
    const int            db = BinIndex(in.enemyDist, m.distEdges);
    const int            fromSide = ChordSide(chord);
    double               zmax = -1e30;
    double               z[NUM_CHORDS];
    for (int c = 0; c < NUM_CHORDS; c++) {
        if (c == chord || (vetoMask & (1 << c))) {
            z[c] = -1e30;
            continue;
        }
        double v = m.transLogit.At(in.ctx, chord, c);
        if (Moving(c)) {
            const float ang = ChordAngle(c);
            if (in.enemyKnown) {
                const float r = std::cos((ang - in.enemyBearing) * DEG2RAD);
                float       k = m.radial.At(in.ctx, db);
                if (in.enemyReloading) {
                    k += m.radialEnemyReload;
                }
                v += k * r;
            }
            if (in.navValid && in.urgency > 0.0f) {
                v += m.navChoiceLogit * in.urgency * std::cos((ang - in.navBearing) * DEG2RAD);
            }
            if (ChordFwd(c) == 1 && ChordSide(c) != 0) {
                v += style.diagLogit;
            }
            if (fromSide != 0 && ChordSide(c) == -fromSide) {
                v += style.reverseLogit;
            }
        } else if (in.navValid) {
            v += m.neutralNavLogit * in.urgency;
        }
        z[c] = v;
        if (v > zmax) {
            zmax = v;
        }
    }
    for (int c = 0; c < NUM_CHORDS; c++) {
        w[c] = z[c] <= -1e29 ? 0.0 : std::exp(z[c] - zmax);
    }
}

void Mover::StepLean(const MoveInput& in, const StyleOffsets& style, Rng& rng)
{
    const MovementModel& m    = *m_p;
    const int            ab   = BinIndex(m_leanAge, m.leanAgeEdges);
    const int            side = ChordSide(m_chord);
    double               p[3];
    if (m_lean == 0) {
        const int rel = side + 1;
        for (int k = 0; k < 3; k++) {
            p[k] = m.leanNext.At(0, in.ctx, ab, rel, k);
        }
        // style: shift the chance of starting a lean
        const double on = 1.0 - p[1];
        if (on > 1e-6 && on < 1.0 - 1e-6) {
            const double on2 = Sigmoid(Logit(static_cast<float>(on)) + style.leanLogit);
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
        const int rel = side == 0 ? 0 : (side == m_lean ? 1 : 2);
        for (int k = 0; k < 3; k++) {
            p[k] = m.leanNext.At(1, in.ctx, ab, rel, k);
        }
        const double stay = p[0];
        if (stay > 1e-6 && stay < 1.0 - 1e-6) {
            const double stay2 = Sigmoid(Logit(static_cast<float>(stay)) + style.leanLogit);
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

static int DrawHold(const std::vector<float>& pmf, Rng& rng)
{
    std::vector<double> w(pmf.begin(), pmf.end());
    return rng.Categorical(w) + 1;
}

void Mover::StepStance(const MoveInput& in, const StyleOffsets& style, Rng& rng, MoveOutput& out)
{
    const MovementModel& m = *m_p;
    // fixed draws per tick: crouch, jump, walk presses and their hold lengths
    const double uc = rng.Uniform();
    const double uj = rng.Uniform();
    const double uw = rng.Uniform();

    if (m_crouchLeft > 0) {
        m_crouchLeft--;
    } else if (uc < m.crouch.pressHazard[in.ctx] * style.crouchMult) {
        m_crouchLeft = DrawHold(m.crouch.holdPmf, rng) - 1;
        out.crouch   = true;
    }
    if (m_crouchLeft > 0) {
        out.crouch = true;
    }

    if (m_jumpLeft > 0) {
        m_jumpLeft--;
    } else if (in.allowJump && in.onGround && !out.crouch && uj < m.jump.pressHazard[in.ctx] * style.jumpMult) {
        m_jumpLeft = DrawHold(m.jump.holdPmf, rng) - 1;
        out.jump   = true;
    }
    if (m_jumpLeft > 0 && !out.crouch) {
        out.jump = true;
    }

    if (m_walkLeft > 0) {
        m_walkLeft--;
    } else if (uw < m.walk.pressHazard[in.ctx] * style.walkMult) {
        m_walkLeft = DrawHold(m.walk.holdPmf, rng) - 1;
        out.walk   = true;
    }
    if (m_walkLeft > 0) {
        out.walk = true;
    }
    // people let go of walk when a fight starts
    if (in.ctx == CTX_LOS_FIRE || in.ctx == CTX_HIDDEN_FIRE) {
        m_walkLeft = 0;
        out.walk   = false;
    }
}

void Mover::Step(const MoveInput& in, const StyleOffsets& style, Rng& moveRng, Rng& stanceRng, MoveOutput& out)
{
    out          = MoveOutput();
    const double u = moveRng.Uniform();
    const int    veto = VetoMask(in);
    out.vetoMask      = veto;
    out.pSwitch       = SwitchProb(in, style, m_chord, m_age, veto);
    if (u < out.pSwitch) {
        double w[NUM_CHORDS];
        NextChordWeights(in, style, m_chord, veto, w);
        double tot = 0.0;
        for (double x : w) {
            tot += x;
        }
        const int next = tot > 0.0 ? moveRng.Categorical(w, NUM_CHORDS) : CHORD_NEUTRAL;
        if (next != m_chord) {
            m_chord = next;
            m_age   = 1;
        } else {
            m_age++;
        }
    } else {
        m_age++;
    }
    StepLean(in, style, stanceRng);
    StepStance(in, style, stanceRng, out);
    out.chord = m_chord;
    out.lean  = m_lean;
}

} // namespace hb
