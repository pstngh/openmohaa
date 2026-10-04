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
// hb_substep.h: one tick's decisions as K usercmds, like a human client.
//
// A human at 85 fps sends about four usercmds per 50 ms server frame. The
// bot does the same: strictly increasing serverTime ending at the frame time,
// each key change landing on its own random sub-step and the view motion
// spread across the sub-steps (following the flick profile during flicks).

#pragma once

#include "hb_rng.h"
#include "hb_types.h"

#include <vector>

namespace hb
{


class Substepper
{
public:
    void Init(int substeps);
    void Reset(float yaw, float pitch);
    int  Count() const { return m_K; }

    // curYaw/curPitch: the view at the start of the tick.
    void Build(const TickPlan& plan, float curYaw, float curPitch, Rng& rng, std::vector<SubCmd>& out);

    // Keyboard contract: forward/right only -127, 0, 127, never both leans,
    // serverTime strictly increasing, last offset 0.
    static bool CheckContract(const std::vector<SubCmd>& cmds);

private:
    int    m_K     = 4;
    bool   m_valid = false;
    SubCmd m_prev;
};

} // namespace hb
