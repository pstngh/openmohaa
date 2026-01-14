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
#include "gamecvars.h"

BotRotation::BotRotation()
{
    m_vAngDelta   = vec_zero;
    m_vAngSpeed   = vec_zero;
    m_vTargetAng  = vec_zero;
    m_vCurrentAng = vec_zero;

    // Humanized aim init
    m_vLastAimTarget   = vec_zero;
    m_vOvershootOffset = vec_zero;
    m_iAimStartTime    = 0;
    m_bNewTarget       = false;
}

void BotRotation::SetControlledEntity(Player *newEntity)
{
    controlledEntity = newEntity;
}

float AngleDifference(float ang1, float ang2)
{
    float diff;

    diff = ang1 - ang2;
    if (ang1 > ang2) {
        if (diff > 180.0) {
            diff -= 360.0;
        }
    } else {
        if (diff < -180.0) {
            diff += 360.0;
        }
    }
    return diff;
}

void BotRotation::TurnThink(usercmd_t& botcmd, usereyes_t& eyeinfo)
{
    float diff;
    float deltaDiff;
    float factor;
    float maxChange;
    float maxChangeDelta;
    float minChange;
    float changeSpeed;
    float speed;
    int   i;

    factor      = 1.0;
    maxChange   = Q_max(360, g_bot_turn_speed->integer);
    minChange   = 20;
    changeSpeed = g_bot_turn_speed->integer;

    if (m_vTargetAng[PITCH] > 180) {
        m_vTargetAng[PITCH] -= 360;
    }

    for (i = 0; i < 2; i++) {
        m_vCurrentAng[i] = AngleMod(m_vCurrentAng[i]);
        m_vTargetAng[i]  = AngleMod(m_vTargetAng[i]);

        diff      = AngleDifference(m_vCurrentAng[i], m_vTargetAng[i]);
        deltaDiff = fabs(diff);

        maxChangeDelta = maxChange * level.frametime;
        if (maxChangeDelta > deltaDiff) {
            maxChangeDelta = deltaDiff;
        }

        if (deltaDiff >= minChange) {
            m_vAngSpeed[i] = Q_min(1.0, m_vAngSpeed[i] + changeSpeed * level.frametime);
            maxChangeDelta *= m_vAngSpeed[i];
        } else {
            m_vAngSpeed[i] = Q_max(0.0, m_vAngSpeed[i] - changeSpeed * level.frametime);
        }

        speed = diff * level.frametime * 10 * factor;

        m_vAngDelta[i]   = Q_clamp_float(speed, -maxChangeDelta, maxChangeDelta);
        m_vCurrentAng[i] = AngleMod(m_vCurrentAng[i] - m_vAngDelta[i]);
    }

    if (m_vCurrentAng[PITCH] > 180) {
        m_vCurrentAng[PITCH] -= 360;
    }

    eyeinfo.angles[0] = m_vCurrentAng[0];
    eyeinfo.angles[1] = m_vCurrentAng[1];
    botcmd.angles[0]  = ANGLE2SHORT(m_vCurrentAng[0]) - controlledEntity->client->ps.delta_angles[0];
    botcmd.angles[1]  = ANGLE2SHORT(m_vCurrentAng[1]) - controlledEntity->client->ps.delta_angles[1];
    botcmd.angles[2]  = ANGLE2SHORT(m_vCurrentAng[2]) - controlledEntity->client->ps.delta_angles[2];
}

/*
====================
GetTargetAngles

Return the target angle
====================
*/
const Vector& BotRotation::GetTargetAngles() const
{
    return m_vTargetAng;
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
With humanized aim: slight overshoot on new target, two-phase acquisition
====================
*/
void BotRotation::AimAt(Vector vPos)
{
    Vector vDelta = vPos - controlledEntity->EyePosition();
    Vector vTarget;

    VectorNormalize(vDelta);
    vectoangles(vDelta, vTarget);

    // If humanized aim is disabled, use instant aim
    if (!g_bot_aim_human->integer) {
        SetTargetAngles(vTarget);
        return;
    }

    // Check if this is a new target (position changed significantly)
    float targetDist = (vPos - m_vLastAimTarget).length();
    
    if (targetDist > 50.0f) {
        // New target acquired - set up overshoot
        m_bNewTarget = true;
        m_iAimStartTime = level.inttime;
        m_vLastAimTarget = vPos;
        
        // Calculate small overshoot (5-15% past target in the aim direction)
        float overshootAmount = 0.05f + G_Random(0.10f);
        Vector aimDir = vTarget - m_vCurrentAng;
        
        // Normalize angle differences
        for (int i = 0; i < 2; i++) {
            if (aimDir[i] > 180.0f) aimDir[i] -= 360.0f;
            if (aimDir[i] < -180.0f) aimDir[i] += 360.0f;
        }
        
        m_vOvershootOffset[0] = aimDir[0] * overshootAmount;
        m_vOvershootOffset[1] = aimDir[1] * overshootAmount;
        m_vOvershootOffset[2] = 0;
    } else {
        // Same target - update position for tracking
        m_vLastAimTarget = vPos;
    }

    // Two-phase aiming
    int aimTime = level.inttime - m_iAimStartTime;
    
    if (m_bNewTarget && aimTime < 80) {
        // Phase 1 (0-80ms): Fast snap WITH overshoot
        // Aim past the target slightly
        Vector overshootTarget = vTarget + m_vOvershootOffset;
        SetTargetAngles(overshootTarget);
    } else if (m_bNewTarget && aimTime < 150) {
        // Phase 2 (80-150ms): Correct back to actual target
        // Lerp from overshoot back to real target
        float correction = (float)(aimTime - 80) / 70.0f; // 0 to 1 over 70ms
        Vector correctedTarget = vTarget + m_vOvershootOffset * (1.0f - correction);
        SetTargetAngles(correctedTarget);
    } else {
        // Phase 3: Normal tracking (no overshoot)
        m_bNewTarget = false;
        SetTargetAngles(vTarget);
    }
}

/*
====================
ResetAimState

Reset humanized aim state (call when enemy changes)
====================
*/
void BotRotation::ResetAimState(void)
{
    m_vLastAimTarget   = vec_zero;
    m_vOvershootOffset = vec_zero;
    m_iAimStartTime    = 0;
    m_bNewTarget       = false;
}
