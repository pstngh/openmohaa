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
// hb_nav.cpp: intents and goals.

#include "hb_nav.h"

#include <cmath>
#include <vector>

namespace hb
{

static constexpr int ENGAGE_MEMORY_MS = 1000;
static constexpr int POST_KILL_MS     = 1500;

void Navigator::Init(const NavModel *params, const MapPrior *map)
{
    m_p   = params;
    m_map = map;
    Reset();
}

void Navigator::SetMap(const MapPrior *map)
{
    m_map = map;
    Reset();
}

void Navigator::Reset()
{
    m_intent      = INTENT_HUNT;
    m_holdUntilMs = 0;
    m_goalUntilMs = 0;
    m_goalValid   = false;
}

Vec3 Navigator::PickHuntGoal(const SelfState& self, const NavInput& in, Rng& rng)
{
    const BeliefEstimate *b = in.focus;
    if (b && b->valid && !b->dead && b->spread < 600.0f) {
        return b->mode;
    }
    if (m_map && m_map->NumCells() > 0) {
        // somewhere people spend time while hunting, not right here
        std::vector<double> w(m_map->NumCells());
        for (int c = 0; c < m_map->NumCells(); c++) {
            const MapCell& mc = m_map->cells[c];
            const float    d  = (mc.center - self.origin).lengthXY();
            w[c]              = (mc.occ[CTX_HIDDEN_NOFIRE] + mc.occ[CTX_LOS_FIRE] + 1.0) * (d > 256.0f ? 1.0 : 0.05);
        }
        return m_map->cells[rng.Categorical(w)].center;
    }
    if (b && b->valid) {
        return b->mode;
    }
    const float a = static_cast<float>(rng.Uniform(0.0, 360.0));
    return self.origin + YawDir(a) * 512.0f;
}

Vec3 Navigator::PickCover(const SelfState& self, const Vec3& threat) const
{
    if (!m_map || m_map->NumCells() == 0) {
        Vec3 away = self.origin - threat;
        away.z    = 0.0f;
        const float l = away.lengthXY();
        return l > 1.0f ? self.origin + away * (160.0f / l) : self.origin;
    }
    const int here   = m_map->CellAt(self.origin);
    const int threatCell = m_map->CellAt(threat);
    if (here < 0) {
        return self.origin;
    }
    // a nearby cell (two hops) that the threat cannot see, preferring the closest
    int   best  = -1;
    float bestS = 1e30f;
    for (int k = m_map->rStart[here]; k < m_map->rStart[here + 1]; k++) {
        const int a = m_map->rTo[k];
        for (int k2 = m_map->rStart[a]; k2 < m_map->rStart[a + 1]; k2++) {
            const int   c    = m_map->rTo[k2];
            const float vis  = threatCell >= 0 ? m_map->Visibility(threatCell, c) : 0.5f;
            const float dist = (m_map->cells[c].center - self.origin).lengthXY();
            const float s    = vis * 1000.0f + dist;
            if (s < bestS) {
                bestS = s;
                best  = c;
            }
        }
    }
    return best >= 0 ? m_map->cells[best].center : self.origin;
}

void Navigator::Step(const SelfState& self, const NavInput& in, Rng& rng, NavOutput& out)
{
    const NavModel& p   = *m_p;
    const int       now = self.timeMs;
    const double    uHold = rng.Uniform();
    out                   = NavOutput();

    const BeliefEstimate *b = in.focus;
    const bool recentlySeen = b && b->valid && !b->dead && b->msSinceSeen < ENGAGE_MEMORY_MS;

    if (in.detected || recentlySeen) {
        m_intent    = INTENT_ENGAGE;
        out.target  = in.detected ? in.enemyPos : b->mode;
        out.urgency = p.engageUrgency;
        m_goalValid = false;
    } else if (in.reloading && b && b->valid && !b->dead) {
        m_intent    = INTENT_RELOAD_COVER;
        out.target  = PickCover(self, b->mode);
        out.urgency = p.reloadUrgency;
    } else if (in.msSinceSpawn < p.spawnPushMs) {
        m_intent = INTENT_SPAWN_PUSH;
        if (!m_goalValid || now >= m_goalUntilMs) {
            m_goal        = PickHuntGoal(self, in, rng);
            m_goalValid   = true;
            m_goalUntilMs = now + static_cast<int>(p.repathMs);
        }
        out.target  = m_goal;
        out.urgency = 1.0f;
    } else if (in.msSinceKill < POST_KILL_MS) {
        m_intent    = INTENT_POST_KILL;
        out.target  = self.origin;
        out.urgency = 0.15f;
    } else if (now < m_holdUntilMs) {
        m_intent    = INTENT_HOLD;
        out.target  = self.origin;
        out.urgency = 0.0f;
    } else {
        // hold where people hold, more readily when the enemy is expected soon
        float hz = p.holdHazard;
        if (b && b->valid && b->visibleSoon > 0.2f) {
            hz *= 3.0f;
        }
        if (m_map) {
            const int c = m_map->CellAt(self.origin);
            if (c >= 0) {
                hz *= 0.5f + 2.0f * m_map->cells[c].still;
            }
        }
        if (uHold < hz) {
            m_intent      = INTENT_HOLD;
            m_holdUntilMs = now + static_cast<int>(rng.LogNormal(p.holdMedianMs, p.holdSigma));
            out.target    = self.origin;
            out.urgency   = 0.0f;
        } else {
            m_intent = INTENT_HUNT;
            const bool reached = m_goalValid && (m_goal - self.origin).lengthXY() < p.waypointReach;
            if (!m_goalValid || now >= m_goalUntilMs || reached || self.stuckMs > 800.0f) {
                m_goal        = PickHuntGoal(self, in, rng);
                m_goalValid   = true;
                m_goalUntilMs = now + static_cast<int>(3.0f * p.repathMs);
            }
            out.target  = m_goal;
            out.urgency = p.huntUrgency;
        }
    }
    out.intent = m_intent;
    out.valid  = true;
    if (self.navSteerValid) {
        out.desiredYaw = self.navSteerYaw;
    } else {
        out.desiredYaw = YawOf(out.target - self.origin);
    }
    if ((out.target - self.origin).lengthXY() < 24.0f) {
        out.urgency = 0.0f;
    }
}

} // namespace hb
