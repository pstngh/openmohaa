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

#include <vector>

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
    float                 angleHold    = 1.0f;     // style (hold or clear an angle): x the hold hazard with an exposure coming up
    bool                  outOfAmmo    = false;    // nothing left to fire: close in on the enemy (to bash)
    float                 clipFill     = 1.0f;     // rounds left in the clip, a share of it
};

struct NavOutput {
    int   intent     = INTENT_HUNT;
    bool  valid      = false;
    Vec3  target;
    float urgency    = 0.0f;
    float desiredYaw = 0.0f;   // world yaw to travel
    bool  viaValid   = false;  // the engine's path goes to `via`, a point along the way people go (NavModel::viaDist)
    Vec3  via;
};

class Navigator
{
public:
    void Init(const NavModel *params, const MapPrior *map);
    void SetMap(const MapPrior *map);
    void Reset();

    void Step(const SelfState& self, const NavInput& in, Rng& rng, NavOutput& out);
    // Pick a new hunt goal at the next step (the believed position moved: an enemy was heard behind).
    void Replan() { m_goalValid = false; }
    // The point `dist` along the way to the last step's target: on the straightened navmesh path when the engine
    // gives its corners, else along the route cells. False without a way.
    bool PointAhead(const SelfState& self, float dist, Vec3& out);

private:
    Vec3 PickHuntGoal(const SelfState& self, const NavInput& in, Rng& rng);
    Vec3 PickCover(const SelfState& self, const Vec3& threat) const;
    // Direction of the next route-graph cell toward target (when the engine gives no steering).
    bool RouteYaw(const SelfState& self, const Vec3& target, float& yaw);
    // The route cell the bot is in (or the nearest within hereCells cells), with the shortest-path tree toward target's
    // cell built (-1: no route).
    int  RouteFrom(const SelfState& self, const Vec3& target, float hereCells = 2.0f);
    // The point the engine's path goes to: viaDist along the route people take toward target (false: the target itself).
    bool Via(const SelfState& self, const Vec3& target, Vec3& via);

    const NavModel *m_p   = nullptr;
    const MapPrior *m_map = nullptr;
    int             m_intent        = INTENT_HUNT;
    int             m_holdUntilMs   = 0;
    int             m_goalUntilMs   = 0;
    Vec3            m_goal;
    bool            m_goalValid     = false;
    Vec3            m_target;                  // the last step's target
    bool            m_targetValid   = false;
    // shortest-path tree toward the last route target
    int                m_treeTarget = -1;
    std::vector<int>   m_next;
    std::vector<float> m_dist;
    // the via point's cell and the tree it was taken on
    int                m_viaCell    = -1;
    int                m_viaTree    = -1;
};

} // namespace hb
