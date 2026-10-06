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
// hb_perception_model.h: raw percepts -> noisy, human-like observations.
//
// Detection is a per-tick hazard that grows with the visible body area and
// falls with eccentricity and distance. Sounds give a noisy bearing and
// distance (footsteps with front/back confusion); damage gives a noisy
// direction. The number of random draws per tick depends only on the number
// of enemies and sound events, never on where a hidden enemy is.

#pragma once

#include "hb_model.h"
#include "hb_rng.h"
#include "hb_types.h"

#include <vector>

namespace hb
{

class Perceiver
{
public:
    void Init(const PerceptionModel *params, const Rng& rng);
    void Reset();

    // hfov/vfov in degrees; detectMult scales the detection rate (1 for every style today).
    void Process(const RawInput& raw, float hfovDeg, float vfovDeg, float detectMult, Observation& out);

    float LastDetectP() const { return m_lastDetectP; }

    // Per-tick detection probability of a target (also used by the replay harness).
    static float DetectProb(const PerceptionModel& p, float eccDeg, float dist, int parts, float detectMult, bool insideFov);

private:
    struct Track {
        int  id        = -1;
        bool detected  = false;
        bool hadTarget = false;   // was detected when it was last lost
        int  lostTicks = 1000;
        Vec3 lastPos;
        int  lastTime  = 0;
        bool onScreen  = false;   // a part was on screen last tick
        int  onScreenSince = 0;   // when the parts came on screen
    };

    Track& TrackFor(int id);

    const PerceptionModel *m_p = nullptr;
    Rng                    m_rng;
    std::vector<Track>     m_tracks;
    float                  m_lastDetectP = 0.0f;
    bool                   m_wasAlive    = true;   // alive last tick (a respawn brings a hunch at once)
};

} // namespace hb
