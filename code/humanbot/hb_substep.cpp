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
// hb_substep.cpp: sub-step schedule of a tick.

#include "hb_substep.h"
#include "hb_math.h"
#include "hb_model.h"

#include <cmath>
#include <algorithm>

namespace hb
{

void Substepper::Init(int substeps)
{
    m_K     = ClampI(substeps, 1, MAX_SUBSTEPS);
    m_valid = false;
}

void Substepper::Reset(float yaw, float pitch)
{
    m_prev       = SubCmd();
    m_prev.yaw   = yaw;
    m_prev.pitch = pitch;
    m_valid      = false;
}

// Sub-step index at which a key that changes this tick is pressed/released.
static int ChangeAt(bool changed, int K, double u)
{
    if (!changed) {
        return 0;
    }
    return ClampI(static_cast<int>(u * K), 0, K - 1);
}

void Substepper::Build(const TickPlan& plan, float curYaw, float curPitch, Rng& rng, std::vector<SubCmd>& out)
{
    const int K = m_K;
    out.assign(K, SubCmd());
    if (!m_valid) {
        m_prev.chord  = plan.chord;
        m_prev.attack = plan.attack;
        m_prev.bash   = plan.bash;
        m_prev.lean   = plan.lean;
        m_prev.crouch = plan.crouch;
        m_prev.jump   = plan.jump;
        m_prev.walk   = plan.walk;
        m_prev.use    = plan.use;
        m_valid       = true;
    }
    // fixed draws: timing jitter, one per key group, view split
    double ut[MAX_SUBSTEPS];
    for (int k = 0; k < MAX_SUBSTEPS; k++) {
        ut[k] = rng.Uniform();
    }
    const double uFwd = rng.Uniform(), uSide = rng.Uniform(), uAtt = rng.Uniform(), uLean = rng.Uniform();
    const double uCr = rng.Uniform(), uJmp = rng.Uniform(), uWalk = rng.Uniform(), uUse = rng.Uniform();
    double wv[MAX_SUBSTEPS];
    for (int k = 0; k < MAX_SUBSTEPS; k++) {
        wv[k] = rng.Normal();
    }

    // serverTime offsets: evenly spread with +-2 ms jitter, strictly increasing, last = 0
    int prevOfs = -TICK_MS;
    for (int k = 0; k < K; k++) {
        int ofs;
        if (k == K - 1) {
            ofs = 0;
        } else {
            const double base = -TICK_MS + TICK_MS * (k + 1) / static_cast<double>(K);
            ofs               = static_cast<int>(std::floor(base + (ut[k] - 0.5) * 4.0 + 0.5));
            const int maxOfs  = -(K - 1 - k);  // leave room for the remaining sub-steps
            ofs               = ClampI(ofs, prevOfs + 1, maxOfs);
        }
        out[k].serverTimeOffset = ofs;
        prevOfs                 = ofs;
    }

    const int pf = ChordFwd(m_prev.chord), ps = ChordSide(m_prev.chord);
    const int nf = ChordFwd(plan.chord), ns = ChordSide(plan.chord);
    const int atFwd  = ChangeAt(pf != nf, K, uFwd);
    const int atSide = ChangeAt(ps != ns, K, uSide);
    const int atAtt  = ChangeAt(m_prev.attack != plan.attack, K, uAtt);
    // the bash (out of ammunition only, never with the trigger) shares the trigger's timing draw
    const int atBash = ChangeAt(m_prev.bash != plan.bash, K, uAtt);
    const int atLean = ChangeAt(m_prev.lean != plan.lean, K, uLean);
    const int atCr   = ChangeAt(m_prev.crouch != plan.crouch, K, uCr);
    const int atJmp  = ChangeAt(m_prev.jump != plan.jump, K, uJmp);
    const int atWalk = ChangeAt(m_prev.walk != plan.walk, K, uWalk);
    const int atUse  = ChangeAt(m_prev.use != plan.use, K, uUse);

    // view split
    float frac[MAX_SUBSTEPS];
    if (plan.viewStill) {
        for (int k = 0; k < K; k++) {
            frac[k] = 0.0f;
        }
    } else if (plan.flickShaped) {
        float s = 0.0f;
        for (int k = 0; k < K; k++) {
            frac[k] = std::max(0.0f, plan.flickFrac[k]);
            s += frac[k];
        }
        for (int k = 0; k < K; k++) {
            frac[k] = s > 0.0f ? frac[k] / s : 1.0f / K;
        }
    } else {
        float s = 0.0f;
        for (int k = 0; k < K; k++) {
            frac[k] = std::max(0.2f, 1.0f + 0.2f * static_cast<float>(wv[k]));
            s += frac[k];
        }
        for (int k = 0; k < K; k++) {
            frac[k] /= s;
        }
    }

    float cum = 0.0f;
    for (int k = 0; k < K; k++) {
        SubCmd& c = out[k];
        c.chord   = MakeChord(k >= atFwd ? nf : pf, k >= atSide ? ns : ps);
        c.attack  = k >= atAtt ? plan.attack : m_prev.attack;
        c.bash    = k >= atBash ? plan.bash : m_prev.bash;
        c.lean    = k >= atLean ? plan.lean : m_prev.lean;
        c.crouch  = k >= atCr ? plan.crouch : m_prev.crouch;
        c.jump    = k >= atJmp ? plan.jump : m_prev.jump;
        c.walk    = k >= atWalk ? plan.walk : m_prev.walk;
        c.use     = k >= atUse ? plan.use : m_prev.use;
        if (c.crouch && c.jump) {
            c.jump = false;  // one upmove at a time
        }
        cum += frac[k];
        if (k == K - 1) {
            cum = plan.viewStill ? 0.0f : 1.0f;
        }
        c.yaw   = curYaw + plan.yawDelta * cum;
        c.pitch = curPitch + plan.pitchDelta * cum;
    }
    m_prev = out[K - 1];
    // g_humanbot_skill's fire gate: skip the rounds the crosshair would send off the body (the
    // intended trigger state above carries over; draws only while the gate is on)
    if (plan.fireGate > 0.0f) {
        for (int k = 0; k < K; k++) {
            SubCmd& c = out[k];
            if (!c.attack && !plan.gateMayPress) {
                continue;
            }
            const float tYaw = plan.gateYaw + plan.gateYawRate * static_cast<float>(k + 1) / static_cast<float>(K);
            const bool  off  = std::fabs(Wrap180(c.yaw - tYaw)) > plan.gateHalfW || std::fabs(c.pitch - plan.gatePitch) > plan.gateHalfH;
            // fire when on the body, hold off when off it (people time their rounds this way)
            if (c.attack == off && rng.Uniform() < plan.fireGate) {
                c.attack = !off;
            }
        }
    }
}

bool Substepper::CheckContract(const std::vector<SubCmd>& cmds)
{
    if (cmds.empty() || cmds.back().serverTimeOffset != 0) {
        return false;
    }
    for (size_t i = 0; i < cmds.size(); i++) {
        const SubCmd& c = cmds[i];
        if (c.chord < 0 || c.chord >= NUM_CHORDS) {
            return false;
        }
        if (c.lean < -1 || c.lean > 1) {
            return false;
        }
        if (c.crouch && c.jump) {
            return false;
        }
        if (c.serverTimeOffset <= -TICK_MS || c.serverTimeOffset > 0) {
            return false;
        }
        if (i > 0 && c.serverTimeOffset <= cmds[i - 1].serverTimeOffset) {
            return false;
        }
        if (!std::isfinite(c.yaw) || !std::isfinite(c.pitch)) {
            return false;
        }
    }
    return true;
}

} // namespace hb
