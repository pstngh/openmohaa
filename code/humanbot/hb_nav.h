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
// hb_nav.h: where the bot wants to go.
//
// Intents (spawn push, hunt, hold, engage, reload cover, post-kill) pick a goal
// from the belief and the human occupancy of the map. The output is only a
// desired world direction and an urgency; the movement module turns it into
// keys relative to wherever the bot is looking.

#pragma once

#include "hb_belief.h"
#include "hb_map.h"
#include "hb_model.h"
#include "hb_rng.h"
#include "hb_types.h"

namespace hb
{

struct NavInput {
    const BeliefEstimate *focus    = nullptr;
    bool                  detected = false;   // focus enemy perceived now
    Vec3                  enemyPos;           // perceived feet position when detected
    bool                  reloading = false;
    int                   msSinceSpawn = 1000000;
    int                   msSinceKill  = 1000000;
};

struct NavOutput {
    int   intent     = INTENT_HUNT;
    bool  valid      = false;
    Vec3  target;
    float urgency    = 0.0f;
    float desiredYaw = 0.0f;   // world yaw to travel
};

class Navigator
{
public:
    void Init(const NavModel *params, const MapPrior *map);
    void SetMap(const MapPrior *map);
    void Reset();

    void Step(const SelfState& self, const NavInput& in, Rng& rng, NavOutput& out);

private:
    Vec3 PickHuntGoal(const SelfState& self, const NavInput& in, Rng& rng);
    Vec3 PickCover(const SelfState& self, const Vec3& threat) const;

    const NavModel *m_p   = nullptr;
    const MapPrior *m_map = nullptr;
    int             m_intent        = INTENT_HUNT;
    int             m_holdUntilMs   = 0;
    int             m_goalUntilMs   = 0;
    Vec3            m_goal;
    bool            m_goalValid     = false;
};

} // namespace hb
