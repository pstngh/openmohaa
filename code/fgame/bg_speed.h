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
// bg_speed.h: one step of a player's top speed (Player::ClientMove).
//
// Added in OPM
//  The speed times a factor (the crouch, the held weapon's movement speed,
//  sv_dmspeedmult), cut to a whole number as Allied Assault 1.11 does, always.
//  The original multiplies at the x87's full precision: 250 x 0.96f is
//  239.99999 -> 239, then x 1.1f -> 262 with an SMG. In float, 250 x 0.96f
//  rounds to 240 (x 1.1f -> 264). A float factor times a whole speed is exact
//  in a double, so the double gives the original's numbers

#pragma once

static inline int BG_SpeedTimes(int speed, float mult)
{
    return (int)((double)speed * (double)mult);
}
