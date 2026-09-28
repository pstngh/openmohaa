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
// hb_view.h: where the bot looks and how its mouse moves.
//
// A delayed controller fitted on the recordings tracks the aim point:
//   rate[n] = rho*rate[n-1] + Kp*err[n-2] + Kself*wself[n-1] + Kopp*wopp[n-4] + b + eps[n]
// (degrees per 50 ms tick, separate parameters firing and idle), with AR(1)
// Student-t noise sized as a lateral miss at the target. Large errors are
// closed by main-sequence flicks with a human overshoot rate; a still gate
// holds the mouse exactly still like people do. Without a visible enemy the
// view pre-aims where the belief says the enemy will appear, looks around, or
// looks ahead along the route.

#pragma once

#include "hb_belief.h"
#include "hb_model.h"
#include "hb_rng.h"
#include "hb_types.h"

#include <vector>

namespace hb
{

struct ViewInput {
    int   ctx       = CTX_HIDDEN_NOFIRE;
    bool  firing    = false;   // attack held this tick
    int   substeps  = 4;
    // tracking a perceived (or just lost) enemy
    bool  track       = false;
    bool  detected    = false;
    bool  acquisition = false; // first ticks of a sighting after occlusion
    Vec3  enemyFeet;           // perceived feet position of the tracked enemy
    Vec3  enemyVel;
    float bodyHeight  = 94.0f;
    // hidden look policy
    const BeliefEstimate *belief = nullptr;
    bool  moving      = false;
    bool  navValid    = false;
    float navYaw      = 0.0f;  // world yaw of travel
    const std::vector<SoundObs>  *sounds = nullptr;
    const std::vector<DamageObs> *damage = nullptr;
    float aimHeightFiring = 0.44f;  // style
    float noiseScale      = 1.0f;   // style (skill) x pooled tuning
};

struct ViewOutput {
    float yawDelta    = 0.0f;
    float pitchDelta  = 0.0f;
    bool  still       = false;
    bool  flick       = false;
    float flickFrac[8] = {};
    int   mode        = VIEW_TRACK;
    float targetYaw   = 0.0f;
    float targetPitch = 0.0f;
    float errYaw      = 0.0f;
    float errPitch    = 0.0f;
    float noiseYaw    = 0.0f;
    float aimHeight   = 0.0f;
    float flickAmp    = 0.0f;
};

class ViewControl
{
public:
    void Init(const ViewModel *params);
    void Reset(const SelfState& self);

    void Step(const SelfState& self, const ViewInput& in, Rng& rng, ViewOutput& out);

    // Main-sequence flick duration in ms for an amplitude (degrees), median.
    float FlickDurationMs(float amplitude) const;

private:
    struct Flick {
        bool  active   = false;
        float ampYaw   = 0.0f;
        float ampPitch = 0.0f;
        float durMs    = 100.0f;
        float tMs      = 0.0f;
    };

    void  ChooseLook(const SelfState& self, const ViewInput& in, Rng& rng);
    void  LookAround(const SelfState& self, Rng& rng);
    void  StartFlick(float errYaw, float errPitch, float gainMedian, float gainSigma, Rng& rng);
    float NoiseStep(const NoiseModel& nm, float dist, float& state, float scale, Rng& rng);

    const ViewModel *m_p = nullptr;

    // controller state (degrees per tick)
    float m_rate      = 0.0f;
    float m_prate     = 0.0f;
    float m_err[4]    = {};   // yaw error history: [0] = n-1, [1] = n-2, ...
    float m_perr[2]   = {};
    float m_wopp[5]   = {};
    float m_tpitch[4] = {};   // target pitch history for its rate
    float m_noise     = 0.0f;
    float m_pnoise    = 0.0f;
    bool  m_still     = false;
    bool  m_histValid = false;
    float m_aimH      = 0.66f;
    Flick m_flick;

    // hidden look policy
    int   m_lookMode    = VIEW_TRAVEL;
    Vec3  m_lookPoint;
    bool  m_lookPointValid = false;
    float m_lookYaw     = 0.0f;
    float m_lookPitch   = 0.0f;
    float m_dwellMs     = 0.0f;
    int   m_damageAtMs  = -100000;
    float m_damageYaw   = 0.0f;
    bool  m_damagePending = false;
    bool  m_wasTracking = false;
    int   m_refractory  = 0;   // ticks before another corrective flick may start
};

} // namespace hb
