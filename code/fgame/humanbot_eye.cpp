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
// humanbot_eye.cpp: the eye position a human client would send (usereyes_t),
// so bots peek and shoot from a leaned eye exactly like people do.

#include "humanbot_internal.h"

Vector HB_EyeStep(Player *player, hb::EyeState& state, float yaw, float pitch, float frameMs)
{
    const playerState_t& ps = player->client->ps;
    hb::EyeInput         in;
    in.origin      = hb::Vec3(ps.origin[0], ps.origin[1], ps.origin[2]);
    in.velocity    = hb::Vec3(ps.velocity[0], ps.velocity[1], ps.velocity[2]);
    in.viewheight  = static_cast<float>(ps.viewheight);
    in.pitch       = pitch;
    in.yaw         = yaw;
    in.leanAngle   = ps.fLeanAngle;
    in.walking     = ps.walking != 0;
    in.climbWall   = ps.pm_type == PM_CLIMBWALL;
    in.frozen      = ps.walking && ((ps.pm_flags & PMF_FROZEN) || (ps.pm_flags & PMF_NO_MOVE));
    in.frametimeMs = frameMs;
    const hb::Vec3 eye = hb::ComputeEye(state, in);
    return Vector(eye.x, eye.y, eye.z);
}

Vector HB_EyeClip(Player *player, const Vector& desired)
{
    const playerState_t& ps = player->client->ps;
    const Vector         mins(-6, -6, -6);
    const Vector         maxs(6, 6, 6);
    const Vector         start(ps.origin[0], ps.origin[1], ps.origin[2] + ps.viewheight);
    const Vector         up(ps.origin[0], ps.origin[1], desired.z);
    const trace_t        th = G_Trace(start, mins, maxs, up, player, MASK_PLAYERSOLID, qfalse, "HumanBot eye height");
    const Vector         mid(th.endpos);
    const Vector         side(desired.x, desired.y, mid.z);
    const trace_t        tl = G_Trace(mid, mins, maxs, side, player, MASK_PLAYERSOLID, qfalse, "HumanBot eye lateral");
    return Vector(tl.endpos);
}

void HB_EyeOffset(Player *player, const Vector& eye, signed char ofs[3])
{
    const playerState_t& ps = player->client->ps;
    for (int i = 0; i < 3; i++) {
        ofs[i] = hb::EyeOffsetByte(eye[i] - ps.origin[i]);
    }
}
