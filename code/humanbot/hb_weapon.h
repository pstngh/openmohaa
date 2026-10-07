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
// hb_weapon.h: reloads, weapon switches and the respawn click.
//
// People reload after a kill (more often the fewer rounds are left, about
// 650 ms later), when the clip runs dry, and rarely tactically. They click to
// respawn about 2.5 s after dying.

#pragma once

#include "hb_model.h"
#include "hb_rng.h"
#include "hb_types.h"

namespace hb
{

// Out of ammunition: nothing in hand can fire and neither the primary nor the pistol has a round left.
bool OutOfAmmo(const SelfState& self);

class WeaponLogic
{
public:
    void Init(const WeaponModel *params);
    void Reset();

    // Returns a Command for this tick.
    // msSinceSeen: since a part of the enemy was last on screen (0 while one is)
    int Step(const SelfState& self, bool gotKill, bool enemyAlive, bool enemyDetected, bool attackHeld, int msSinceSeen,
             Rng& rng);

    // Delay of the respawn click after death, ms.
    int  RespawnDelayMs(Rng& rng) const;
    bool ReloadPlanned() const { return m_reloadAtMs >= 0; }

private:
    const WeaponModel *m_p         = nullptr;
    int                m_reloadAtMs = -1;
    bool               m_reloadAfterKill = false;   // the planned reload is the one after a kill
    int                m_lastCmdMs  = -100000;
    int                m_unarmedSince = -1;   // since when nothing is in hand (-1: armed)
};

} // namespace hb
