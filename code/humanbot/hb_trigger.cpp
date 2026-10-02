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
// hb_trigger.cpp: press and release hazards.

#include "hb_trigger.h"
#include "hb_math.h"

#include <algorithm>
#include <cmath>

namespace hb
{

float HiddenLateRamp(int lageMs)
{
    const float ms = static_cast<float>(lageMs);
    if (ms <= HIDDEN_LATE_FROM_MS) {
        return 0.0f;
    }
    return std::min(1.0f, std::log(ms / HIDDEN_LATE_FROM_MS) / std::log(HIDDEN_LATE_FULL_MS / HIDDEN_LATE_FROM_MS));
}

void Trigger::Init(const TriggerModel *params)
{
    m_p = params;
    Reset();
}

void Trigger::Reset()
{
    m_attack     = false;
    m_age        = 40;
    m_burstShots = 0;
    m_pPress     = 0.0f;
    m_pRelease   = 0.0f;
}

float Trigger::PressProb(const TriggerInput& in, int gapTicks) const
{
    const TriggerModel& t = *m_p;
    float               z;
    if (in.los) {
        const TriggerSide& s = t.pressLos;
        z = s.bias + s.err[BinIndex(in.errHalfWidths, t.enEdges)] + s.lage[BinIndex(static_cast<float>(in.lageMs), t.lageLosEdges)]
          + s.age[BinIndex(gapTicks, t.gapEdges)] + t.pressLosLogit + in.pressLogit;
        if (in.damaged) {
            z += s.damaged;
        }
    } else {
        const TriggerSide& s = t.pressHidden;
        z = s.bias + s.err[BinIndex(in.hiddenYawErr, t.yawEdges)]
          + s.lage[BinIndex(static_cast<float>(in.lageMs), t.lageHiddenEdges)] + s.age[BinIndex(gapTicks, t.gapEdges)]
          + t.hiddenFireLogit;
        if (in.damaged) {
            z += s.damaged;
        }
        // an enemy expected in the crosshair right now has not been lost track of: no fade then
        if (in.anticipate) {
            z += t.anticipationLogit;
        } else {
            z += t.hiddenLateLogit * HiddenLateRamp(in.lageMs);
        }
    }
    return Sigmoid(z);
}

float Trigger::ReleaseProb(const TriggerInput& in, int holdTicks) const
{
    const TriggerModel& t = *m_p;
    float               z;
    if (in.los) {
        const TriggerSide& s = t.releaseLos;
        z = s.bias + s.err[BinIndex(in.errHalfWidths, t.enEdges)] + s.lage[BinIndex(static_cast<float>(in.lageMs), t.lageLosEdges)]
          + s.age[BinIndex(holdTicks, t.holdEdges)] + t.releaseLosLogit
          + (in.errHalfWidths >= RELEASE_FAR_HALF_WIDTHS ? t.releaseFarLogit : t.releaseNearLogit)
          + (holdTicks <= 2 ? t.releaseTapLogit : 0.0f) + in.releaseLogit;
    } else {
        // the style's burst length is a firefight habit: fire at an enemy out of sight is let go of as
        // people let go of it (with it, the bots held such fire 2.3x as long and ran out of ammunition)
        const TriggerSide& s = t.releaseHidden;
        z = s.bias + s.err[BinIndex(in.hiddenYawErr, t.yawEdges)]
          + s.lage[BinIndex(static_cast<float>(in.lageMs), t.lageHiddenEdges)] + s.age[BinIndex(holdTicks, t.holdEdges)];
    }
    return Sigmoid(z);
}

bool Trigger::Step(const TriggerInput& in, Rng& rng)
{
    // one draw per tick whatever happens, so the stream does not depend on the state
    const double u = rng.Uniform();
    m_pPress       = 0.0f;
    m_pRelease     = 0.0f;
    bool next      = m_attack;
    if (!in.canFire || in.blocked) {
        next = false;
    } else if (m_attack) {
        m_pRelease = ReleaseProb(in, m_age);
        next       = !(u < m_pRelease);
    } else {
        m_pPress = PressProb(in, m_age);
        next     = u < m_pPress;
    }
    if (next != m_attack) {
        m_attack = next;
        m_age    = 1;
        if (next) {
            m_burstShots = 0;
        }
    } else if (m_age < 100000) {
        m_age++;
    }
    return m_attack;
}

} // namespace hb
