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
// playerbot_movement.cpp: Manages bot movements

#include "playerbot.h"
#include "debuglines.h"

static int maxFallHeight = 400;

bot_movement_telemetry_t::bot_movement_telemetry_t()
{
    Reset();
}

void bot_movement_telemetry_t::Reset()
{
    hasCombatTarget        = false;
    pathing                = false;
    blockedRecovery        = false;
    pathCollisionAvoidance = false;
    movementSuppressed     = false;
    strafeDirection        = 0;
    strafeChangeMsec       = -1;
    isLeaning              = false;
    strafeApplied          = false;
    strafeClearanceFlip    = false;
    strafeClearance        = -1.0f;
    strafeOtherClearance   = -1.0f;
    strafeProbeFraction    = -1.0f;
    strafeIntensity        = 0.0f;
    radialDirection        = 0;
    radialChangeMsec       = -1;
    radialActive           = false;
    radialForcedCloseRetreat = false;
    radialDistance         = -1.0f;
    radialDesiredMove      = 0.0f;
    radialBeforeMove       = 0.0f;
    guardTriggered         = false;
    guardHitSentient       = false;
    guardHitWorld          = false;
    guardRemovedComponent = false;
    guardFraction          = 1.0f;
    guardEntity            = ENTITYNUM_NONE;
}

BotMovement::BotMovement()
{
    controlledEntity = NULL;

    m_pPath         = NULL;
    m_iLastMoveTime = 0;

    m_bPathing       = false;
    m_iTempAwayState = 0;
    m_fAttractTime   = 0;

    m_iCheckPathTime = 0;
    m_iTempAwayTime  = 0;
    m_iNumBlocks     = 0;

    m_bAvoidCollision     = false;
    m_iCollisionCheckTime = 0;
    m_bJump               = false;
    m_iJumpCheckTime      = 0;

    // Aggressive movement
    m_iStrafeDirection      = 1;
    m_iNextStrafeChangeTime = 0;
    m_iRadialDirection      = 1;
    m_iNextRadialChangeTime = 0;
    m_bIsLeaning            = false;
    m_bHasCombatTarget      = false;
    m_vCombatTarget         = vec_zero;
}

BotMovement::~BotMovement()
{
    delete m_pPath;
}

void BotMovement::SetControlledEntity(Player *newEntity)
{
    controlledEntity = newEntity;
}

void BotMovement::SetCombatTarget(const Vector& target)
{
    m_bHasCombatTarget = true;
    m_vCombatTarget    = target;
}

void BotMovement::ClearCombatTarget()
{
    m_bHasCombatTarget      = false;
    m_vCombatTarget         = vec_zero;
    m_iRadialDirection      = 1;
    m_iNextRadialChangeTime = 0;
}

void BotMovement::MoveThink(usercmd_t& botcmd)
{
    Vector vAngles;
    Vector vWishDir;
    Vector vDelta;

    m_telemetry.Reset();

    botcmd.forwardmove = 0;
    botcmd.rightmove   = 0;
    // The bot usercmd persists across frames: start each movement frame
    // lean-neutral so a lean can't stay latched after strafing stops
    botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);

    CheckAttractiveNodes();

    if (!IsMoving() || !m_pPath) {
        // No path to follow, but the bot should still juke and lean in place
        UpdateAggressiveMovement(botcmd);
        PreventImminentBodyContact(botcmd);
        return;
    }

    if (m_pPath->GetNodeCount()) {
        m_vTargetPos = m_pPath->GetDestination();
    }

    if (m_pPath->IsQuerying()) {
        m_iLastMoveTime = level.inttime;
    }

    if (level.inttime >= m_iLastMoveTime + 5000 && m_vCurrentOrigin != controlledEntity->origin) {
        m_vCurrentOrigin = controlledEntity->origin;

        if (m_pPath->GetNodeCount() && !controlledEntity->GetLadder()) {
            // recalculate paths because of a new origin

            PathSearchParameter parameters;
            parameters.entity     = controlledEntity;
            parameters.fallHeight = maxFallHeight;
            m_pPath->FindPath(controlledEntity->origin, m_pPath->GetDestination(), parameters);
        }

        m_iLastMoveTime = level.inttime;
    }

    if (m_iTempAwayState == 2 && level.inttime >= m_iTempAwayTime + 750) {
        m_iTempAwayState = 0;

        PathSearchParameter parameters;
        parameters.entity     = controlledEntity;
        parameters.fallHeight = maxFallHeight;
        m_pPath->FindPath(controlledEntity->origin, m_vTargetPos, parameters);

        m_iLastMoveTime  = level.inttime;
        m_iCheckPathTime = level.inttime;
    }

    vDelta = m_pPath->GetCurrentDelta();
    vDelta = FixDeltaFromCollision(vDelta);

    if (m_pPath->GetNodeCount()) {
        m_pPath->UpdatePos(controlledEntity->origin);

        m_vCurrentGoal = controlledEntity->origin;
        VectorAdd2D(m_vCurrentGoal, vDelta, m_vCurrentGoal);

        if (MoveDone()) {
            // Clear the path
            m_pPath->Clear();
        }
    }

    if (ai_debugpath->integer) {
        G_DebugLine(controlledEntity->centroid, m_vCurrentGoal + Vector(0, 0, 36), 1, 1, 0, 1);
    }

    // Check if we're blocked
    if (level.inttime >= m_iCheckPathTime + 1000 && m_iTempAwayState != 2) {
        bool blocked = false;

        m_iCheckPathTime = level.inttime;

        if (m_iNumBlocks >= 5) {
            // Give up
            ClearMove();
        }

        if (!m_pPath->IsQuerying() && !controlledEntity->GetLadder()) {
            if (controlledEntity->GetMoveResult() >= MOVERESULT_BLOCKED
                || controlledEntity->velocity.lengthSquared() <= Square(8)) {
                blocked = true;
            } else if ((controlledEntity->origin - m_vLastCheckPos[0]).lengthSquared() <= Square(64)
                       && (controlledEntity->origin - m_vLastCheckPos[1]).lengthSquared() <= Square(64)) {
                blocked = true;
            }
        }

        if (!blocked) {
            m_iTempAwayState = 0;
            m_iNumBlocks     = 0;

            if (!m_pPath->GetNodeCount()) {
                m_vTargetPos   = controlledEntity->origin + Vector(G_CRandom(512), G_CRandom(512), G_CRandom(512));
                m_vCurrentGoal = m_vTargetPos;
            }
        } else if (m_iTempAwayState == 0) {
            m_iLastBlockTime = level.inttime;
            m_iTempAwayState = 1;
        }

        if (m_iTempAwayState && level.inttime >= m_iLastBlockTime + 1000) {
            Vector delta;
            Vector dir;

            m_iTempAwayState = 2;
            m_iTempAwayTime  = level.inttime;
            m_iNumBlocks++;

            // Try to backward a little
            if (m_pPath->GetNodeCount()) {
                delta = m_pPath->GetCurrentDelta();
            } else {
                delta = m_vTargetPos - controlledEntity->origin;
            }

            m_pPath->Clear();

            if (m_iNumBlocks < 2) {
                dir   = -delta;
                dir.z = 0;
                dir.normalize();

                if (dir.x < -0.5 || dir.x > 0.5) {
                    dir.x *= 4;
                    dir.y /= 4;
                } else if (dir.y < -0.5 || dir.y > 0.5) {
                    dir.x /= 4;
                    dir.y *= 4;
                } else {
                    dir.x = G_CRandom(2);
                    dir.y = G_CRandom(2);
                }

                m_vCurrentGoal = controlledEntity->origin + delta + dir * 128;
            } else {
                m_vCurrentGoal = controlledEntity->origin + Vector(G_CRandom(512), G_CRandom(512), G_CRandom(512));
            }
        }

        m_vLastCheckPos[1] = m_vLastCheckPos[0];
        m_vLastCheckPos[0] = controlledEntity->origin;
    }

    if (ai_debugpath->integer) {
        int i;
        int nodecount = m_pPath->GetNodeCount();

        for (i = 0; i < nodecount - 1; i++) {
            PathNav      node1  = m_pPath->GetNode(i);
            PathNav      node2  = m_pPath->GetNode(i + 1);
            const Vector vStart = node1.origin + Vector(0, 0, 32);
            const Vector vEnd   = node2.origin + Vector(0, 0, 32);

            G_DebugLine(vStart, vEnd, 1, 0, 0, 1);
        }
    }

    if (m_pPath->GetNodeCount() || m_iTempAwayState != 0) {
        if ((m_vTargetPos - controlledEntity->origin).lengthSquared() <= Square(16)) {
            ClearMove();
        }
    } else {
        //if ((m_vTargetPos - controlledEntity->origin).lengthXYSquared() <= Square(16)) {
        ClearMove();
        //}
    }

    // Rotate the dir
    if (m_pPath->GetNodeCount()) {
        m_vCurrentDir = CalculateDir(vDelta);
    } else {
        m_vCurrentDir = CalculateDir(m_vCurrentGoal - controlledEntity->origin);
    }

    vWishDir = CalculateRelativeWishDirection(m_vCurrentDir);

    // Forward to the specified direction
    float x = vWishDir.x * 127;
    float y = -vWishDir.y * 127;

    botcmd.forwardmove = (signed char)Q_clamp(x, -127, 127);
    botcmd.rightmove   = (signed char)Q_clamp(y, -127, 127);
    botcmd.upmove      = 0;

    // Apply aggressive evasive movement (strafe + lean)
    UpdateAggressiveMovement(botcmd);
    PreventImminentBodyContact(botcmd);

    CheckJump(botcmd);

    if (!m_bJump) {
        CheckJumpOverEdge(botcmd);
    }
}

Vector BotMovement::CalculateDir(const Vector& delta) const
{
    Vector dir;

    dir    = delta;
    dir[2] = 0;
    VectorNormalize2D(dir);

    return dir;
}

Vector BotMovement::CalculateRelativeWishDirection(const Vector& dir) const
{
    Vector angles;
    Vector wishdir;

    angles = dir.toAngles() - controlledEntity->angles;
    angles.AngleVectorsLeft(&wishdir);

    return wishdir;
}

Vector BotMovement::GetCommandMoveVector(const usercmd_t& botcmd) const
{
    Vector angles = controlledEntity->angles;
    Vector forward, left, up;

    angles.x = 0;
    angles.z = 0;
    angles.AngleVectorsLeft(&forward, &left, &up);

    Vector move = forward * (float)botcmd.forwardmove - left * (float)botcmd.rightmove;
    move.z      = 0;
    return move;
}

void BotMovement::SetCommandMoveVector(usercmd_t& botcmd, const Vector& move) const
{
    Vector angles = controlledEntity->angles;
    Vector forward, left, up;

    angles.x = 0;
    angles.z = 0;
    angles.AngleVectorsLeft(&forward, &left, &up);

    botcmd.forwardmove = (signed char)Q_clamp_float(DotProduct(move, forward), -127, 127);
    botcmd.rightmove   = (signed char)Q_clamp_float(-DotProduct(move, left), -127, 127);
}

void BotMovement::CheckAttractiveNodes()
{
    for (int i = m_attractList.NumObjects(); i > 0; i--) {
        nodeAttract_t *a = m_attractList.ObjectAt(i);

        if (a->m_pNode == NULL || !a->m_pNode->CheckTeam(controlledEntity) || level.time > a->m_fRespawnTime) {
            delete a;
            m_attractList.RemoveObjectAt(i);
        }
    }
}

void BotMovement::CheckEndPos(Entity *entity)
{
    Vector  start;
    Vector  end;
    trace_t trace;

    if (!m_pPath->GetNodeCount()) {
        return;
    }

    start = m_pPath->GetDestination();
    end   = m_vTargetPos;

    trace =
        G_Trace(start, entity->mins, entity->maxs, end, entity, MASK_TARGETPATH, true, "BotController::CheckEndPos");

    if (trace.fraction < 0.95f) {
        m_vTargetPos = trace.endpos;
    }
}

void BotMovement::CheckJump(usercmd_t& botcmd)
{
    Vector  start;
    Vector  end;
    Vector  dir;
    Vector  delta;
    trace_t trace;

    if (controlledEntity->GetLadder()) {
        if (g_navigation_legacy->integer) {
            botcmd.upmove = botcmd.upmove ? 0 : 127;
        } else if (!m_pPath->GetNodeCount()) {
            // If the bot is not moving, cancel it
            botcmd.upmove = botcmd.upmove ? 0 : 127;
        }
        return;
    }

    if (!controlledEntity->groundentity && !controlledEntity->client->ps.walking) {
        // Falling
        m_bJump = false;
        return;
    }

    dir = m_vCurrentDir;

    start = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    end =
        controlledEntity->origin + Vector(0, 0, STEPSIZE) + dir * (controlledEntity->maxs.y - controlledEntity->mins.y);

    if (ai_debugpath->integer) {
        G_DebugLine(start, end, 1, 0, 1, 1);
    }

    // Check if the bot needs to jump
    trace = G_Trace(
        start,
        controlledEntity->mins,
        controlledEntity->maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID,
        false,
        "BotController::CheckJump"
    );

    // No need to jump
    if (!trace.startsolid && trace.fraction > 0.5f) {
        m_bJump = false;
        return;
    }

    start = controlledEntity->origin;
    end   = controlledEntity->origin;
    end.z += STEPSIZE * 3;
    end.z += STEPSIZE / 1.5;

    if (ai_debugpath->integer) {
        G_DebugLine(start, end, 1, 0, 1, 1);
    }

    // Check if the bot can jump up
    trace = G_Trace(
        start,
        controlledEntity->mins,
        controlledEntity->maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID,
        true,
        "BotController::CheckJump"
    );

    start = trace.endpos;
    end   = trace.endpos + dir * (controlledEntity->maxs.y - controlledEntity->mins.y);

    if (ai_debugpath->integer) {
        G_DebugLine(start, end, 1, 0, 1, 1);
    }

    Vector bounds[2];
    bounds[0] = Vector(controlledEntity->mins[0], controlledEntity->mins[1], 0);
    bounds[1] = Vector(
        controlledEntity->maxs[0],
        controlledEntity->maxs[1],
        (controlledEntity->maxs[0] + controlledEntity->maxs[1]) * 0.5
    );

    // Check if the bot can jump at the location
    trace = G_Trace(
        start, bounds[0], bounds[1], end, controlledEntity, MASK_PLAYERSOLID, false, "BotController::CheckJump"
    );

    if (trace.plane.normal[2] <= MIN_WALK_NORMAL && trace.fraction < 1) {
        m_bJump = false;
        return;
    }

    if (!m_bJump) {
        m_bJump          = true;
        m_iJumpCheckTime = level.inttime;
        m_vJumpLocation  = controlledEntity->origin;
    } else if (level.inttime > m_iJumpCheckTime + 100) {
        m_bJump = false;

        delta = m_vJumpLocation - controlledEntity->origin;
        if (delta.lengthSquared() < Square(32)) {
            botcmd.upmove = 127;
        }
    }
}

void BotMovement::CheckJumpOverEdge(usercmd_t& botcmd)
{
    Vector  start;
    Vector  end;
    Vector  dir;
    trace_t trace;

    if (!controlledEntity->groundentity && !controlledEntity->client->ps.walking) {
        // Falling
        return;
    }

    dir = m_vCurrentDir;

    start = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    end =
        controlledEntity->origin + Vector(0, 0, STEPSIZE) + dir * (controlledEntity->maxs.y - controlledEntity->mins.y);

    if (ai_debugpath->integer) {
        G_DebugLine(start, end, 1, 0, 1, 1);
    }

    // Check if the bot needs to jump
    trace = G_Trace(
        start,
        controlledEntity->mins,
        controlledEntity->maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID,
        false,
        "BotController::CheckJumpOverEdge"
    );

    if (trace.fraction < 1) {
        // Blocked
        return;
    }

    //
    // Check if falling
    //

    start = trace.endpos;
    end   = start - Vector(0, 0, STEPSIZE * 2);

    trace = G_Trace(
        start,
        controlledEntity->mins,
        controlledEntity->maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID,
        false,
        "BotController::CheckJumpOverEdge"
    );

    if (trace.fraction != 1.0) {
        // Blocked
        return;
    }

    //
    // Check if there is an edge at the end
    //

    end = start + dir * controlledEntity->GetRunSpeed() / 2.0;
    end -= Vector(0, 0, STEPSIZE * 2);

    trace = G_Trace(
        start,
        controlledEntity->mins,
        controlledEntity->maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID,
        false,
        "BotController::CheckJumpOverEdge"
    );

    if (trace.fraction == 1) {
        return;
    }

    if (!botcmd.upmove) {
        botcmd.upmove = 127;
    } else {
        botcmd.upmove = 0;
    }
}

/*
====================
AvoidPath

Avoid the specified position within the radius and start from a direction
====================
*/
void BotMovement::AvoidPath(
    Vector vAvoid, float fAvoidRadius, Vector vPreferredDir, float *vLeashHome, float fLeashRadius
)
{
    Vector vDir;

    if (vPreferredDir == vec_zero) {
        vDir = controlledEntity->origin - vAvoid;
        VectorNormalizeFast(vDir);
    } else {
        vDir = vPreferredDir;
    }

    PathSearchParameter parameters;
    parameters.entity     = controlledEntity;
    parameters.fallHeight = maxFallHeight;
    parameters.leashDist  = fLeashRadius;
    if (vLeashHome) {
        parameters.leashHome = vLeashHome;
    }

    if (!m_pPath) {
        m_pPath = IPather::CreatePather();
    }

    m_pPath->FindPathAway(controlledEntity->origin, vAvoid, vDir, fAvoidRadius, parameters);

    NewMove();

    if (!m_pPath->GetNodeCount()) {
        // Random movements
        m_vTargetPos = controlledEntity->origin + Vector(G_Random(256) - 128, G_Random(256) - 128, G_Random(256) - 128);
        m_vCurrentGoal = m_vTargetPos;
        return;
    }

    m_iLastMoveTime = level.inttime;
    m_vTargetPos    = m_pPath->GetDestination();
}

/*
====================
MoveNear

Move near the specified position within the radius
====================
*/
void BotMovement::MoveNear(Vector vNear, float fRadius, float *vLeashHome, float fLeashRadius)
{
    PathSearchParameter parameters;
    parameters.entity     = controlledEntity;
    parameters.fallHeight = maxFallHeight;
    parameters.leashDist  = fLeashRadius;
    if (vLeashHome) {
        parameters.leashHome = vLeashHome;
    }

    if (!m_pPath) {
        m_pPath = IPather::CreatePather();
    }

    m_pPath->FindPathNear(controlledEntity->origin, vNear, fRadius, parameters);
    NewMove();

    if (!m_pPath->GetNodeCount()) {
        m_bPathing = false;
        return;
    }

    m_iLastMoveTime = level.inttime;
    m_vTargetPos    = m_pPath->GetDestination();
}

/*
====================
MoveTo

Move to the specified position
====================
*/
void BotMovement::MoveTo(Vector vPos, float *vLeashHome, float fLeashRadius)
{
    m_vTargetPos = vPos;

    PathSearchParameter parameters;
    parameters.entity     = controlledEntity;
    parameters.fallHeight = maxFallHeight;
    parameters.leashDist  = fLeashRadius;
    if (vLeashHome) {
        parameters.leashHome = vLeashHome;
    }

    if (!m_pPath) {
        m_pPath = IPather::CreatePather();
    }

    m_pPath->FindPath(controlledEntity->origin, vPos, parameters);

    NewMove();

    if (!m_pPath->GetNodeCount()) {
        m_bPathing = false;
        return;
    }

    m_iLastMoveTime = level.inttime;
    CheckEndPos(controlledEntity);
}

/*
====================
MoveToBestAttractivePoint

Move to the nearest attractive point with a minimum priority
Returns true if no attractive point was found
====================
*/
bool BotMovement::MoveToBestAttractivePoint(int iMinPriority)
{
    Container<AttractiveNode *> list;
    AttractiveNode             *bestNode;
    float                       bestDistanceSquared;
    int                         bestPriority;

    if (m_pPrimaryAttract) {
        MoveTo(m_pPrimaryAttract->origin);

        if (!IsMoving()) {
            m_pPrimaryAttract = NULL;
        } else {
            if (MoveDone()) {
                if (!m_fAttractTime) {
                    m_fAttractTime = level.time + m_pPrimaryAttract->m_fMaxStayTime;
                }
                if (level.time > m_fAttractTime) {
                    nodeAttract_t *a  = new nodeAttract_t;
                    a->m_fRespawnTime = level.time + m_pPrimaryAttract->m_fRespawnTime;
                    a->m_pNode        = m_pPrimaryAttract;

                    m_pPrimaryAttract = NULL;
                }
            }

            return true;
        }
    }

    if (!attractiveNodes.NumObjects()) {
        return false;
    }

    bestNode            = NULL;
    bestDistanceSquared = 99999999.0f;
    bestPriority        = iMinPriority;

    for (int i = attractiveNodes.NumObjects(); i > 0; i--) {
        AttractiveNode *node = attractiveNodes.ObjectAt(i);
        float           distSquared;
        bool            m_bRespawning = false;

        for (int j = m_attractList.NumObjects(); j > 0; j--) {
            AttractiveNode *node2 = m_attractList.ObjectAt(j)->m_pNode;

            if (node2 == node) {
                m_bRespawning = true;
                break;
            }
        }

        if (m_bRespawning) {
            continue;
        }

        if (node->m_iPriority < bestPriority) {
            continue;
        }

        if (!node->CheckTeam(controlledEntity)) {
            continue;
        }

        distSquared = VectorLengthSquared(controlledEntity->origin - node->origin);

        if (node->m_fMaxDistanceSquared >= 0 && distSquared > node->m_fMaxDistanceSquared) {
            continue;
        }

        if (!CanMoveTo(node->origin)) {
            continue;
        }

        if (distSquared < bestDistanceSquared) {
            bestDistanceSquared = distSquared;
            bestNode            = node;
            bestPriority        = node->m_iPriority;
        }
    }

    if (bestNode) {
        m_pPrimaryAttract = bestNode;
        m_fAttractTime    = 0;
        MoveTo(bestNode->origin);
        return true;
    } else {
        // No attractive point found
        return false;
    }
}

/*
====================
NewMove

Called when there is a new move
====================
*/
void BotMovement::NewMove()
{
    m_bPathing         = true;
    m_vLastCheckPos[0] = controlledEntity->origin;
    m_vLastCheckPos[1] = controlledEntity->origin;
}

void BotMovement::CalculateBestFrontAvoidance(
    const Vector& targetOrg, float maxDist, const Vector& forward, const Vector& right, float& bestFrac, Vector& bestPos
)
{
    Vector  mins, maxs;
    bool    wasOnGround = true;
    Vector  start, step;
    Vector  entityStepOrg;
    trace_t trace;
    int     i;

    bestFrac = 0;
    bestPos  = vec_zero;

    mins = controlledEntity->mins;
    maxs = controlledEntity->maxs;
    maxs.z -= STEPSIZE;
    entityStepOrg = controlledEntity->origin + Vector(0, 0, STEPSIZE);

    for (i = 1; i < 5; i++) {
        start = entityStepOrg - forward + right * (32 * i);
        if (i == 1) {
            step = start;
        }

        //
        // Trace to the right
        //
        trace = G_Trace(entityStepOrg, mins, maxs, start, controlledEntity, MASK_PLAYERSOLID, qtrue, "GetCurrentDelta");

        if (trace.startsolid || trace.fraction <= 0) {
            break;
        }

        start   = trace.endpos;
        start.z = step.z;

        // Make sure the bot can jump after falling
        trace = G_Trace(
            start,
            mins,
            maxs,
            start - Vector(0, 0, STEPSIZE + STEPSIZE * 3),
            controlledEntity,
            MASK_PLAYERSOLID,
            qtrue,
            "GetCurrentDelta"
        );
        if (trace.fraction == 1) {
            if (!wasOnGround) {
                break;
            }

            wasOnGround = false;
            continue;
        }

        wasOnGround = true;
        step        = trace.endpos;

        //
        // Trace from the right to the node
        //
        trace = G_Trace(start, mins, maxs, targetOrg, controlledEntity, MASK_PLAYERSOLID, qtrue, "GetCurrentDelta");
        if (trace.fraction == 0) {
            trace = G_Trace(
                start,
                mins,
                maxs,
                start + forward * Q_min(maxDist, 64),
                controlledEntity,
                MASK_PLAYERSOLID,
                qtrue,
                "GetCurrentDelta"
            );
            trace = G_Trace(
                trace.endpos, mins, maxs, targetOrg, controlledEntity, MASK_PLAYERSOLID, qtrue, "GetCurrentDelta"
            );
        }

        if (trace.fraction > bestFrac) {
            bestFrac = trace.fraction;
            bestPos  = start;
        }
        if (trace.fraction >= 0.999) {
            break;
        }
    }
}

Vector BotMovement::FixDeltaFromCollision(const Vector& delta)
{
    trace_t trace;
    Vector  stepOrg;
    Vector  mins;
    Vector  maxs;
    Vector  newDelta;
    Vector  angles;
    Vector  forward, right, up;
    Vector  target;
    Vector  targetStepOrg;
    Vector  dest;
    Vector  front;
    float   dist;
    float   maxDist;

    if (controlledEntity->GetLadder()) {
        return delta;
    }

    if (level.inttime < m_iCollisionCheckTime + 250 || m_bJump) {
        if (m_bAvoidCollision) {
            newDelta = m_vTempCollisionAvoidance - controlledEntity->origin;
            if (newDelta.lengthSquared() > Square(16)) {
                // Not reached
                return newDelta;
            }

            // Path has been reached so clear the collision
            m_bAvoidCollision = false;
        }

        return delta;
    }

    m_iCollisionCheckTime = level.inttime;
    m_bAvoidCollision     = false;

    dest     = controlledEntity->origin + delta;
    newDelta = delta;
    dist     = VectorNormalize2(newDelta, forward);
    VectorToAngles(forward, angles);
    AngleVectors(angles, forward, right, up);

    mins = controlledEntity->mins;
    maxs = controlledEntity->maxs;
    maxs.z -= STEPSIZE;

    maxDist = Q_min(dist, 32);

    stepOrg       = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    target        = controlledEntity->origin + forward * maxDist;
    targetStepOrg = target + Vector(0, 0, STEPSIZE);

    trace = G_Trace(stepOrg, mins, maxs, targetStepOrg, controlledEntity, MASK_PLAYERSOLID, qtrue, "GetCurrentDelta");
    if (trace.fraction < 1.0) {
        //
        // Try to use a flat plane instead
        //

        trace_t tmpTrace;
        Vector  forwardXY, rightXY, upXY;
        Vector  targetXY, targetStepOrgXY;

        angles.x = 0;
        AngleVectors(angles, forwardXY, rightXY, upXY);
        targetXY        = controlledEntity->origin + forwardXY * maxDist + Vector(0, 0, STEPSIZE);
        targetStepOrgXY = targetXY + Vector(0, 0, STEPSIZE);

        tmpTrace =
            G_Trace(stepOrg, mins, maxs, targetStepOrgXY, controlledEntity, MASK_PLAYERSOLID, qtrue, "GetCurrentDelta");

        if (tmpTrace.fraction > trace.fraction) {
            trace   = tmpTrace;
            forward = forwardXY;
            right   = rightXY;
            up      = upXY;
            target  = targetXY;
        }
    }

    if (trace.fraction < 1.0) {
        Vector start, step;
        float  bestLeftFrac = 0, bestRightFrac = 0;
        Vector bestLeftPos, bestRightPos;

        // 0 = parallel
        // -1 = perpendicular
        // If it's near parallel use the trace normal
        if (DotProduct(trace.plane.normal, forward) < -0.75) {
            VectorCopy(trace.plane.normal, forward);
            VectorNegate(forward, forward);
            VectorToAngles(forward, angles);
            AngleVectors(angles, forward, right, up);
        }

        //
        // Try to resolve following situation (schema from top):
        //
        // ┌───┐
        // ↑   │
        // p▌  t   ← Must be able to avoid the obstacle in front and move left or right, to target
        // ↓   ↑
        // └─→─┘
        //
        CalculateBestFrontAvoidance(target, 64, forward, right, bestRightFrac, bestRightPos);

        if (bestRightFrac != 1) {
            CalculateBestFrontAvoidance(target, 64, forward, -right, bestLeftFrac, bestLeftPos);
        }

        if (bestLeftFrac != 0 || bestRightFrac != 0) {
            m_bAvoidCollision = true;

            //
            // By default use the one with higher fraction
            //
            if (bestLeftFrac > bestRightFrac) {
                m_vTempCollisionAvoidance = bestLeftPos + forward * 64;
            } else if (bestLeftFrac < bestRightFrac) {
                m_vTempCollisionAvoidance = bestRightPos + forward * 64;
            } else {
                // Randomly choose direction if both are the same
                if (Vector::DistanceSquared(bestLeftPos, dest) > Vector::DistanceSquared(bestRightPos, dest)) {
                    m_vTempCollisionAvoidance = bestRightPos + forward * 64;
                } else {
                    m_vTempCollisionAvoidance = bestLeftPos + forward * 64;
                }
            }

            //
            // If falling, make sure to use the one that won't fall
            //
#if 0
            if (leftFallTrace.fraction != rightFallTrace.fraction
                && (leftFallTrace.fraction != 1 || rightFallTrace.fraction != 1)) {
                if (leftFallTrace.fraction == 1 && bestRightFrac) {
                    m_vTempCollisionAvoidance = bestRightPos + forward * 64;
                } else if (rightFallTrace.fraction == 1 && bestLeftFrac) {
                    m_vTempCollisionAvoidance = bestLeftPos + forward * 64;
                }
            }
#endif

            return m_vTempCollisionAvoidance - controlledEntity->origin;
        }
    }

    return delta;
}

/*
====================
CanMoveTo

Returns true if the bot has done moving
====================
*/
bool BotMovement::CanMoveTo(Vector vPos)
{
    PathSearchParameter parameters;
    parameters.fallHeight = maxFallHeight;
    parameters.entity     = controlledEntity;
    return m_pPath->TestPath(controlledEntity->origin, vPos, parameters);
}

/*
====================
MoveDone

Returns true if the bot has done moving
====================
*/
bool BotMovement::MoveDone()
{
    if (!m_bPathing) {
        return true;
    }

    if (m_iTempAwayState != 0) {
        return false;
    }

    if (!m_pPath->GetNodeCount()) {
        return true;
    }

    Vector delta = m_pPath->GetDestination() - controlledEntity->origin;
    if (delta.lengthXYSquared() < Square(16) && (m_pPath->GetNodeCount() == 1 || delta.z < controlledEntity->maxs.z)) {
        return true;
    }

    return false;
}

/*
====================
IsMoving

Returns true if the bot has a current path
====================
*/
bool BotMovement::IsMoving(void)
{
    return m_bPathing;
}

/*
====================
ClearMove

Stop the bot from moving
====================
*/
void BotMovement::ClearMove(void)
{
    m_bPathing   = false;
    m_iNumBlocks = 0;

    if (m_pPath) {
        m_pPath->Clear();
    }
}

/*
====================
GetCurrentGoal

Return the current goal, usually the nearest node the player should look at
====================
*/
Vector BotMovement::GetCurrentGoal() const
{
    if (!m_pPath->GetNodeCount()) {
        return m_vCurrentGoal;
    }

    if (!m_pPath->HasReachedGoal(controlledEntity->origin) && m_pPath->GetNodeCount()) {
        const Vector delta = m_pPath->GetCurrentDelta();
        return controlledEntity->origin + Vector(delta[0], delta[1], 0);
    }

    return controlledEntity->origin;
}

Vector BotMovement::GetCurrentPathDirection() const
{
    return m_pPath->GetCurrentDirection();
}

void BotMovement::ResetTelemetry()
{
    m_telemetry.Reset();
}

void BotMovement::GetTelemetry(bot_movement_telemetry_t& telemetry) const
{
    telemetry                        = m_telemetry;
    telemetry.hasCombatTarget        = m_bHasCombatTarget;
    telemetry.pathing                = m_bPathing;
    telemetry.blockedRecovery        = m_iTempAwayState == 2;
    telemetry.pathCollisionAvoidance = m_bAvoidCollision;
    telemetry.strafeDirection        = m_iStrafeDirection;
    telemetry.strafeChangeMsec       = Q_max(0, m_iNextStrafeChangeTime - level.inttime);
    telemetry.isLeaning              = m_bIsLeaning;
    telemetry.radialDirection        = m_iRadialDirection;
    telemetry.radialChangeMsec  =
        m_iNextRadialChangeTime ? Q_max(0, m_iNextRadialChangeTime - level.inttime) : -1;
}

/*
====================
CalculateLateralClearance

Trace laterally to determine how much space is available for strafing
Returns distance in units (0 to maxCheckDist)
====================
*/
float BotMovement::CalculateLateralClearance(int direction)
{
    const float maxCheckDist = 96.0f;

    if (direction == 0 || !controlledEntity) {
        return 0;
    }

    Vector forward, right, up;
    controlledEntity->angles.AngleVectors(&forward, &right, &up);

    Vector start = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    Vector end   = start + right * direction * maxCheckDist;

    trace_t trace = G_Trace(
        start,
        controlledEntity->mins,
        controlledEntity->maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID,
        false,
        "BotMovement::CalculateLateralClearance"
    );

    return trace.fraction * maxCheckDist;
}

// Pick a random dwell time from a min/max interval cvar pair (milliseconds).
static int RandomInterval(cvar_t *lo, cvar_t *hi)
{
    const int lower = Q_min(lo->integer, hi->integer);
    const int upper = Q_max(lo->integer, hi->integer);
    return lower + (int)G_Random(upper - lower);
}

/*
====================
UpdateAggressiveMovement

Combat movement layer, calibrated from human-versus-human telemetry:
continuous side-to-side strafing with matching lean, plus slower movement
toward and away from the enemy during close-range engagements. Jumps and
crouches are intentionally not added here.
====================
*/
void BotMovement::UpdateAggressiveMovement(usercmd_t& botcmd)
{
    // Ladders: leave pathing untouched
    if (controlledEntity->GetLadder()) {
        return;
    }

    // While the blocked-recovery logic is actively backing the bot out of an
    // obstruction, suppress the strafe/radial movement injection so its escape
    // vector isn't fought - but keep leaning; only movement steers
    const bool bSuppressMovement = (m_iTempAwayState == 2);
    m_telemetry.movementSuppressed = bSuppressMovement;

    // --- Strafe oscillator: flip left/right on a short randomized timer ---
    if (level.inttime >= m_iNextStrafeChangeTime) {
        m_iStrafeDirection      = -m_iStrafeDirection;
        m_iNextStrafeChangeTime = level.inttime + RandomInterval(g_bot_strafe_min_interval, g_bot_strafe_max_interval);
    }

    // Hysteresis: need 20 units to start strafing, but only drop out below 12.
    // Kept low so bots strafe close to walls and use the full side-to-side
    // room; the stop floor (~one frame of lateral travel) keeps the hull off
    // the wall rather than grinding against it.
    const float startThreshold = 20.0f;
    const float stopThreshold  = 12.0f;
    const float threshold      = m_bIsLeaning ? stopThreshold : startThreshold;

    float clearance = CalculateLateralClearance(m_iStrafeDirection);
    m_telemetry.strafeClearance = clearance;
    if (clearance < threshold) {
        // Preferred side is blocked. Switch only if the other side is clearly
        // open, and commit the flip to the oscillator (with a fresh dwell
        // time) so the side can't bounce back next frame — recomputing the
        // side per frame turns marginal clearance into frame-rate left/right
        // lean flapping against walls.
        float otherClearance = CalculateLateralClearance(-m_iStrafeDirection);
        m_telemetry.strafeOtherClearance = otherClearance;
        if (otherClearance >= startThreshold) {
            m_iStrafeDirection      = -m_iStrafeDirection;
            m_iNextStrafeChangeTime = level.inttime + RandomInterval(g_bot_strafe_min_interval, g_bot_strafe_max_interval);
            clearance               = otherClearance;
            m_telemetry.strafeClearanceFlip = true;
            m_telemetry.strafeClearance     = clearance;
        }
    }

    if (clearance >= threshold && !bSuppressMovement) {
        m_bIsLeaning = true;

        // Doorway / gap damping: probe forward along the side the strafe is
        // about to push toward. If that diagonal is obstructed soon - a
        // doorframe, corner, or cover edge - scale the lateral push down so the
        // bot straightens and threads the gap instead of clipping its edge. The
        // lateral clearance traces above only see straight out to the sides, so
        // they cannot tell "sliding along an open wall" from "drifting into a
        // doorframe"; this forward-biased probe is what distinguishes them. Open
        // ground leaves the probe clear, so open-field strafing keeps full
        // intensity and the aggressive feel is unchanged.
        Vector fwd, rgt, up;
        controlledEntity->angles.AngleVectors(&fwd, &rgt, &up);

        Vector probeDir = fwd + rgt * (float)m_iStrafeDirection;
        probeDir.z      = 0;
        probeDir.normalize();

        const float probeDist  = 56.0f;
        Vector      probeStart = controlledEntity->origin + Vector(0, 0, STEPSIZE);
        Vector      probeEnd   = probeStart + probeDir * probeDist;

        trace_t probe = G_Trace(
            probeStart,
            controlledEntity->mins,
            controlledEntity->maxs,
            probeEnd,
            controlledEntity,
            MASK_PLAYERSOLID,
            false,
            "BotMovement::StrafeProbe"
        );

        float intensity  = g_bot_strafe_intensity->value * probe.fraction;
        m_telemetry.strafeApplied       = true;
        m_telemetry.strafeProbeFraction = probe.fraction;
        m_telemetry.strafeIntensity     = intensity;
        int   offset     = (int)(m_iStrafeDirection * intensity * 127.0f);
        int   newRight   = botcmd.rightmove + offset;
        botcmd.rightmove = (signed char)Q_clamp(newRight, -127, 127);

    } else {
        m_bIsLeaning = false;
    }

    // Lean follows the intended strafe side even when a wall prevents the
    // body from moving laterally. Lean itself is not wall-limited, and keeping
    // it separate from clearance avoids making cramped bots look timid.
    if (m_iStrafeDirection < 0) {
        botcmd.buttons |= BUTTON_LEAN_LEFT;
    } else {
        botcmd.buttons |= BUTTON_LEAN_RIGHT;
    }

    UpdateCombatRadialMovement(botcmd, bSuppressMovement);
}

int BotMovement::RadialPhaseDuration(float distance) const
{
    float duration = (float)RandomInterval(g_bot_peek_min_interval, g_bot_peek_max_interval);

    // From 96-384 units the recording strongly favored closing distance over
    // retreating. Use long advances and short retreats there. Inside 96 units
    // human advance/retreat timing was much closer to even.
    if (distance >= 96.0f) {
        duration *= m_iRadialDirection > 0 ? 2.0f : 0.4f;
    }

    return Q_max(50, (int)duration);
}

void BotMovement::UpdateCombatRadialMovement(usercmd_t& botcmd, bool suppressMovement)
{
    const float maxDistance = g_bot_peek_distance->value;
    if (suppressMovement || !m_bHasCombatTarget || maxDistance <= 0) {
        m_iRadialDirection      = 1;
        m_iNextRadialChangeTime = 0;
        return;
    }

    Vector      towardEnemy = m_vCombatTarget - controlledEntity->origin;
    const float distance    = VectorNormalize2D(towardEnemy);
    m_telemetry.radialDistance = distance;
    if (distance <= 0 || distance >= maxDistance) {
        m_iRadialDirection      = 1;
        m_iNextRadialChangeTime = 0;
        return;
    }

    if (!m_iNextRadialChangeTime) {
        // Do not immediately flip into retreat on first contact. Human combat
        // movement opens with a clear forward bias.
        m_iRadialDirection      = 1;
        m_iNextRadialChangeTime = level.inttime + RadialPhaseDuration(distance);
    } else if (level.inttime >= m_iNextRadialChangeTime) {
        m_iRadialDirection      = -m_iRadialDirection;
        m_iNextRadialChangeTime = level.inttime + RadialPhaseDuration(distance);
    }

    Vector move = GetCommandMoveVector(botcmd);
    m_telemetry.radialActive = true;

    // Keep the radial component deliberately slower than the lateral strafe,
    // producing broad arcs instead of straight charges. At body-contact range
    // always move outward; otherwise alternate according to the distance-
    // weighted phase above.
    float desiredRadialMove;
    if (distance < 56.0f) {
        desiredRadialMove = -48.0f;
        m_telemetry.radialForcedCloseRetreat = true;
    } else if (m_iRadialDirection < 0) {
        desiredRadialMove = distance < 96.0f ? -40.0f : -28.0f;
    } else {
        desiredRadialMove = distance < 96.0f ? 32.0f : 36.0f;
    }

    const float currentRadialMove = DotProduct(move, towardEnemy);
    m_telemetry.radialDesiredMove = desiredRadialMove;
    m_telemetry.radialBeforeMove  = currentRadialMove;
    move += towardEnemy * (desiredRadialMove - currentRadialMove);
    SetCommandMoveVector(botcmd, move);
}

void BotMovement::PreventImminentBodyContact(usercmd_t& botcmd)
{
    if (!controlledEntity || controlledEntity->GetLadder() || m_bJump) {
        return;
    }

    Vector move = GetCommandMoveVector(botcmd);
    if (move.lengthXYSquared() <= 1.0f) {
        return;
    }

    Vector direction = move;
    VectorNormalize2D(direction);

    const float commandFraction =
        Q_max(fabs((float)botcmd.forwardmove), fabs((float)botcmd.rightmove)) / 127.0f;
    const float lookAheadDistance = controlledEntity->GetRunSpeed() * commandFraction * level.frametime + 1.0f;
    if (lookAheadDistance <= 1.0f) {
        return;
    }

    Vector mins = controlledEntity->mins;
    Vector maxs = controlledEntity->maxs;
    maxs.z -= STEPSIZE;

    const Vector start = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    const Vector end   = start + direction * lookAheadDistance;
    trace_t trace      = G_Trace(
        start,
        mins,
        maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID | CONTENTS_BOTCLIP,
        true,
        "BotMovement::PreventImminentBodyContact"
    );

    if (!trace.startsolid && trace.fraction >= 1.0f) {
        return;
    }

    const bool hitSentient = trace.ent && trace.ent->entity && trace.ent->entity->IsSubclassOfSentient();
    m_telemetry.guardTriggered   = true;
    m_telemetry.guardHitSentient = hitSentient;
    m_telemetry.guardHitWorld    = trace.entityNum == ENTITYNUM_WORLD;
    m_telemetry.guardFraction    = trace.fraction;
    m_telemetry.guardEntity      = trace.entityNum;

    // Always prevent actual player/body contact. For world geometry, intervene
    // only while backing up; normal forward pathing already handles walls and
    // should remain free to hug corners and doorways.
    if (!hitSentient && botcmd.forwardmove >= 0) {
        return;
    }

    Vector collisionNormal = trace.plane.normal;
    collisionNormal.z      = 0;
    if (collisionNormal.lengthXYSquared() < 0.01f && hitSentient) {
        collisionNormal   = controlledEntity->origin - trace.ent->entity->origin;
        collisionNormal.z = 0;
    }
    if (VectorNormalize2D(collisionNormal) <= 0) {
        return;
    }

    const float intoObstacle = DotProduct(move, collisionNormal);
    if (intoObstacle < 0) {
        // Remove only the component entering the obstacle. Tangential strafe
        // and the lean buttons are deliberately preserved.
        move += collisionNormal * -intoObstacle;
        SetCommandMoveVector(botcmd, move);
        m_telemetry.guardRemovedComponent = true;
    }
}
