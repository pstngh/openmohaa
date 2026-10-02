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
// hb_trigger.h: when the bot holds the attack button.
//
// Press and release are per-tick hazards fitted on the recordings: with LOS
// they depend on the aim error (in body half-widths), the time since the LOS
// changed and how long the button has been held or released; while hidden,
// on how close the view is to where the enemy is believed to be. There is no
// hard aim threshold, so bots spray and pre-fire like people do.

#pragma once

#include "hb_model.h"
#include "hb_rng.h"

namespace hb
{

struct TriggerInput {
    bool  canFire        = false;  // weapon ready, rounds in the clip, alive
    bool  los            = false;  // in sight: a body part of the focus enemy perceived
    int   lageMs         = 0;      // since `los` last changed (a sighting counts from when the parts came on screen)
    float errHalfWidths  = 100.0f; // with LOS: aim error / opponent body half-width
    float hiddenYawErr   = 180.0f; // hidden: |yaw error| to the believed enemy position
    bool  damaged        = false;  // hit this tick
    bool  anticipate     = false;  // an exposure is expected in the crosshair right now
    bool  blocked        = false;  // a teammate is in the crosshair
    float releaseLogit   = 0.0f;   // style: burst length (in sight only)
    float pressLogit     = 0.0f;   // style: reaction (press hazard with LOS)
};

class Trigger
{
public:
    void Init(const TriggerModel *params);
    void Reset();

    // Decides the attack button for this tick. Returns the new state.
    bool Step(const TriggerInput& in, Rng& rng);

    bool  Attack() const { return m_attack; }
    int   HoldTicks() const { return m_attack ? m_age : 0; }
    int   BurstShots() const { return m_burstShots; }
    float LastPress() const { return m_pPress; }
    float LastRelease() const { return m_pRelease; }

    // Hazards for a given state (used by the replay harness too).
    float PressProb(const TriggerInput& in, int gapTicks) const;
    float ReleaseProb(const TriggerInput& in, int holdTicks) const;

private:
    const TriggerModel *m_p = nullptr;
    bool                m_attack     = false;
    int                 m_age        = 1;   // ticks in the current state (1 = first tick)
    int                 m_burstShots = 0;
    float               m_pPress     = 0.0f;
    float               m_pRelease   = 0.0f;
};

} // namespace hb
