/*
===========================================================================
Copyright (C) 2024 the OpenMoHAA team

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
// playerbot_rotation.cpp: Manages bot rotation

#include "playerbot.h"

BotRotation::BotRotation()
{
    m_vTargetAng = vec_zero;
}

void BotRotation::SetControlledEntity(Player *newEntity)
{
    controlledEntity = newEntity;
}

void BotRotation::TurnThink(usercmd_t& botcmd, usereyes_t& eyeinfo)
{
    float pitch = AngleMod(m_vTargetAng[PITCH]);
    const float yaw = AngleMod(m_vTargetAng[YAW]);

    if (pitch > 180) {
        pitch -= 360;
    }

    eyeinfo.angles[0] = pitch;
    eyeinfo.angles[1] = yaw;
    botcmd.angles[0]  = ANGLE2SHORT(pitch) - controlledEntity->client->ps.delta_angles[0];
    botcmd.angles[1]  = ANGLE2SHORT(yaw) - controlledEntity->client->ps.delta_angles[1];
    botcmd.angles[2]  = -controlledEntity->client->ps.delta_angles[2];
}

/*
====================
SetTargetAngles

Set the bot's angle
====================
*/
void BotRotation::SetTargetAngles(Vector vAngles)
{
    m_vTargetAng = vAngles;
}

/*
====================
AimAt

Make the bot face to the specified direction
====================
*/
void BotRotation::AimAt(Vector vPos)
{
    Vector vDelta = vPos - controlledEntity->EyePosition();
    Vector vTarget;

    VectorNormalize(vDelta);
    vectoangles(vDelta, vTarget);

    SetTargetAngles(vTarget);
}
