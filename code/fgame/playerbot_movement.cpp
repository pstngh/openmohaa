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
#include "health.h"
#include "misc.h"

static int       maxFallHeight                   = 400;
static const int   BOT_LADDER_EXIT_MAX_MSEC        = 1000;
static const float BOT_LADDER_EXIT_DISTANCE        = 64.0f;
static const int   BOT_COLLISION_AVOID_COMMIT_MSEC = 750;
static const float BOT_COLLISION_AVOID_REACHED_UNITS = 16.0f;
static const int   BOT_COLLISION_SIDE_COMMIT_MSEC  = 1200;
static const int   BOT_COLLISION_STALL_MSEC        = 350;
static const float BOT_COLLISION_LOOKAHEAD_FRAMES = 2.0f;
static const float BOT_COLLISION_SAFETY_MARGIN    = 2.0f;
static const float BOT_COLLISION_PROGRESS_UNITS    = 24.0f;
static const int   BOT_REDUCED_STANCE_RECOVERY_MSEC = 1500;
static const int BOT_JUMP_TAKEOFF_MSEC            = 250;
static const int BOT_JUMP_COMMIT_MAX_MSEC         = 1000;
static const int BOT_JUMP_LANDING_COMMIT_MSEC      = 250;
static const int BOT_JUMP_RETRY_MSEC              = 500;
static const float BOT_HEALTH_PATH_LOOKAHEAD       = 192.0f;
static const float BOT_HEALTH_PATH_CORRIDOR        = 48.0f;
static const int   BOT_STRAFE_GEOMETRY_LOCK_MSEC   = 1200;
static const int   BOT_MOVEMENT_OVERLAY_SUPPRESS_MSEC = 350;
static const float BOT_STRAFE_PROBE_DISTANCE        = 56.0f;

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

    m_bPathing          = false;
    m_bDirectMove       = false;
    m_bWasOnLadder      = false;
    m_fLadderTop        = 0.0f;
    m_iLadderExitUntil  = 0;
    m_vLadderExitOrigin = vec_zero;
    m_vLadderExitDirection = vec_zero;
    m_fDirectMoveRadius = 0.0f;
    m_iTempAwayState    = 0;
    m_fAttractTime      = 0;

    m_iCheckPathTime = 0;
    m_iTempAwayTime  = 0;
    m_iNumBlocks     = 0;

    m_bAvoidCollision          = false;
    m_iCollisionCheckTime      = 0;
    m_iCollisionProgressTime   = 0;
    m_vCollisionProgressOrigin = vec_zero;
    m_iCollisionAvoidDirection = 0;
    m_iCollisionAvoidDirectionUntil = 0;
    m_iReducedStanceStartTime  = 0;
    m_bJump               = false;
    m_iJumpCheckTime      = 0;
    m_iJumpCommitTime     = -1;
    m_iJumpLandingTime    = 0;
    m_iJumpRetryTime      = 0;
    m_bJumpWasAirborne    = false;

    // Aggressive movement
    m_iStrafeDirection        = 1;
    m_iNextStrafeChangeTime   = 0;
    m_iStrafeGeometryLockTime = 0;
    m_iMovementOverlaySuppressUntil = 0;
    m_iRadialDirection      = 0;
    m_iNextRadialChangeTime = 0;
    m_bIsLeaning            = false;
    m_bLeanCommandActive    = false;
    m_bHasCombatTarget      = false;
    m_bForceCombatRetreat   = false;
    m_bForceCombatAdvance   = false;
    m_vCombatTarget         = vec_zero;
}

BotMovement::~BotMovement()
{
    delete m_pPath;

    for (int i = m_attractList.NumObjects(); i > 0; --i) {
        delete m_attractList.ObjectAt(i);
    }
    m_attractList.ClearObjectList();
}

void BotMovement::SetControlledEntity(Player *newEntity)
{
    controlledEntity = newEntity;
}

void BotMovement::SetCombatTarget(
    const Vector& target, bool forceRetreat, bool forceAdvance
)
{
    m_bHasCombatTarget    = true;
    m_bForceCombatRetreat = forceRetreat;
    m_bForceCombatAdvance = forceAdvance && !forceRetreat;
    m_vCombatTarget       = target;
}

void BotMovement::ClearCombatTarget()
{
    m_bHasCombatTarget      = false;
    m_bForceCombatRetreat   = false;
    m_bForceCombatAdvance   = false;
    m_vCombatTarget         = vec_zero;
    m_iRadialDirection      = 0;
    m_iNextRadialChangeTime = 0;
}

void BotMovement::MoveThink(usercmd_t& botcmd)
{
    Vector vAngles;
    Vector vWishDir;
    Vector vDelta;

    m_telemetry.Reset();
    m_bLeanCommandActive = false;

    botcmd.forwardmove = 0;
    botcmd.rightmove   = 0;
    // The bot usercmd persists across frames: start each movement frame
    // lean-neutral so a lean can't stay latched after strafing stops.
    botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);

    RecoverStandingStance();

    Entity *ladder = controlledEntity->GetLadder();
    if (ladder) {
        m_bWasOnLadder     = true;
        m_iLadderExitUntil = 0;

        if (ladder->isSubclassOf(FuncLadder)) {
            const FuncLadder *funcLadder =
                static_cast<const FuncLadder *>(ladder);
            m_fLadderTop           = ladder->absmax.z;
            m_vLadderExitDirection = funcLadder->getFacingDir();
            m_vLadderExitDirection.z = 0.0f;
            VectorNormalize2D(m_vLadderExitDirection);
        } else {
            m_vLadderExitDirection = vec_zero;
        }
    } else if (m_bWasOnLadder) {
        m_bWasOnLadder = false;

        if (controlledEntity->origin.z >= m_fLadderTop
            && m_vLadderExitDirection.lengthXYSquared() > 0.0f) {
            // The top-off animation only clears the ladder lip. Keep moving
            // in its prescribed forward direction before returning control to
            // the path, or a newly projected route can turn back into the
            // opening and attach to the same ladder again.
            m_vLadderExitOrigin = controlledEntity->origin;
            m_iLadderExitUntil =
                level.inttime + BOT_LADDER_EXIT_MAX_MSEC;
            m_vCurrentDir = m_vLadderExitDirection;

            m_bJump            = false;
            m_iJumpCommitTime  = -1;
            m_iJumpLandingTime = 0;
            m_bJumpWasAirborne = false;
            m_bAvoidCollision  = false;
            m_iTempAwayState   = 0;
            m_iNumBlocks       = 0;
        }
    }

    if (ContinueLadderExit(botcmd)) {
        return;
    }

    if (ContinueJump(botcmd)) {
        return;
    }
    // Preserve the ladder's deliberate alternating vertical command, but
    // clear jump input as soon as its movement state is finished.
    if (!controlledEntity->GetLadder()) {
        botcmd.upmove = 0;
    }

    CheckAttractiveNodes();

    if (m_bDirectMove) {
        DirectMoveThink(botcmd);
        return;
    }

    if (!IsMoving() || !m_pPath) {
        // No path to follow. Active combat can still juke in place; deliberate
        // holds and genuine idle leave the movement command neutral.
        FinalizeMovement(botcmd);
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

    if (m_pPath->GetNodeCount()) {
        // Advance the corridor from the bot's current position before reading
        // its steering direction. Reading first used the previous frame's
        // corner and could alternate the command at walls and tight doorways.
        m_pPath->UpdatePos(controlledEntity->origin);
    }

    if (!m_pPath->GetNodeCount() && m_iTempAwayState != 2) {
        ClearMove();
        FinalizeMovement(botcmd);
        return;
    }

    if (m_pPath->GetNodeCount()) {
        vDelta = m_pPath->GetCurrentDelta();
        vDelta = FixDeltaFromCollision(vDelta);

        m_vCurrentGoal = controlledEntity->origin;
        VectorAdd2D(m_vCurrentGoal, vDelta, m_vCurrentGoal);
    } else {
        // Blocked recovery deliberately clears the path while it backs away.
        vDelta = m_vCurrentGoal - controlledEntity->origin;
    }

    if (MoveDone()) {
        ClearMove();
        FinalizeMovement(botcmd);
        return;
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
            FinalizeMovement(botcmd);
            return;
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
            m_bAvoidCollision = false;

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
            FinalizeMovement(botcmd);
            return;
        }
    } else {
        ClearMove();
        FinalizeMovement(botcmd);
        return;
    }

    // Rotate the dir
    if (m_pPath->GetNodeCount()) {
        m_vCurrentDir = CalculateDir(vDelta);
    } else {
        m_vCurrentDir = CalculateDir(m_vCurrentGoal - controlledEntity->origin);
    }
    SteerTowardPathHealth(m_vCurrentDir);

    vWishDir = CalculateRelativeWishDirection(m_vCurrentDir);

    // Forward to the specified direction
    float x = vWishDir.x * 127;
    float y = -vWishDir.y * 127;

    botcmd.forwardmove = (signed char)Q_clamp(x, -127, 127);
    botcmd.rightmove   = (signed char)Q_clamp(y, -127, 127);
    botcmd.upmove      = 0;

    // Apply aggressive evasive movement (strafe + lean)
    FinalizeMovement(botcmd);

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

bool BotMovement::ContinueLadderExit(usercmd_t& botcmd)
{
    if (!m_iLadderExitUntil) {
        return false;
    }

    Vector displacement = controlledEntity->origin - m_vLadderExitOrigin;
    displacement.z      = 0.0f;

    const bool onGround =
        controlledEntity->groundentity || controlledEntity->client->ps.walking;
    const bool clearOfLadder =
        DotProduct(displacement, m_vLadderExitDirection)
        >= BOT_LADDER_EXIT_DISTANCE;
    if ((onGround && clearOfLadder)
        || level.inttime >= m_iLadderExitUntil) {
        m_iLadderExitUntil = 0;

        if (m_bPathing && !m_bDirectMove && m_pPath) {
            PathSearchParameter parameters;
            parameters.entity     = controlledEntity;
            parameters.fallHeight = maxFallHeight;
            m_pPath->FindPath(
                controlledEntity->origin, m_vTargetPos, parameters
            );
        }

        m_iLastMoveTime   = level.inttime;
        m_iCheckPathTime  = level.inttime;
        m_iTempAwayState  = 0;
        m_iNumBlocks      = 0;
        m_bAvoidCollision = false;
        return false;
    }

    m_vCurrentDir = m_vLadderExitDirection;
    SetCommandMoveVector(
        botcmd, m_vLadderExitDirection * 127.0f
    );
    botcmd.upmove = 0;
    botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);

    m_bIsLeaning                   = false;
    m_bLeanCommandActive           = false;
    m_telemetry.movementSuppressed = true;
    return true;
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

void BotMovement::RecoverStandingStance()
{
    const bool onGround =
        controlledEntity->groundentity || controlledEntity->client->ps.walking;
    if (controlledEntity->maxs.z >= MAXS_Z || controlledEntity->GetLadder()
        || m_iJumpCommitTime >= 0 || !onGround) {
        m_iReducedStanceStartTime = 0;
        return;
    }

    if (!m_iReducedStanceStartTime) {
        m_iReducedStanceStartTime = level.inttime;
        return;
    }
    if (level.inttime < m_iReducedStanceStartTime + BOT_REDUCED_STANCE_RECOVERY_MSEC) {
        return;
    }

    Vector standMaxs = controlledEntity->maxs;
    standMaxs.z      = MAXS_Z;
    trace_t trace    = G_Trace(
        controlledEntity->origin,
        controlledEntity->mins,
        standMaxs,
        controlledEntity->origin,
        controlledEntity,
        MASK_PLAYERSOLID,
        true,
        "BotMovement::RecoverStandingStance"
    );
    if (trace.startsolid) {
        return;
    }

    Event *height = new Event("modheight", 1);
    height->AddString("stand");
    controlledEntity->ProcessEvent(height);

    Event *position = new Event("moveposflags", 1);
    position->AddString("standing");
    controlledEntity->ProcessEvent(position);

    m_iReducedStanceStartTime = 0;
}

bool BotMovement::ContinueJump(usercmd_t& botcmd)
{
    if (m_iJumpCommitTime < 0) {
        return false;
    }

    const bool onGround =
        controlledEntity->groundentity || controlledEntity->client->ps.walking;
    const int elapsed = level.inttime - m_iJumpCommitTime;

    if (!onGround) {
        m_bJumpWasAirborne = true;
    }

    const Vector displacement = controlledEntity->origin - m_vJumpLocation;
    const bool landed = m_bJumpWasAirborne && onGround;

    if (landed && !m_iJumpLandingTime
        && displacement.z >= STEPSIZE
        && displacement.lengthXYSquared() < Square(32)) {
        // Keep moving across a newly reached ledge instead of handing control
        // back on its first narrow grounded frame and stepping off again.
        m_iJumpLandingTime = level.inttime;
    }

    const bool landingCommit =
        m_iJumpLandingTime
        && level.inttime < m_iJumpLandingTime + BOT_JUMP_LANDING_COMMIT_MSEC
        && displacement.lengthXYSquared() < Square(32);
    const bool landingComplete = m_iJumpLandingTime && !landingCommit;
    const bool stalled = !m_bJumpWasAirborne && elapsed >= BOT_JUMP_TAKEOFF_MSEC;
    const bool timedOut = !m_iJumpLandingTime && elapsed >= BOT_JUMP_COMMIT_MAX_MSEC;
    if ((landed && !m_iJumpLandingTime) || landingComplete || stalled || timedOut) {
        const bool failed =
            !m_bJumpWasAirborne
            || (displacement.lengthXYSquared() < Square(32) && displacement.z < STEPSIZE);

        m_bJump            = false;
        m_iJumpCommitTime  = -1;
        m_iJumpLandingTime = 0;
        m_iJumpRetryTime   = level.inttime + (failed ? BOT_JUMP_RETRY_MSEC : 0);
        m_bJumpWasAirborne = false;

        m_iCheckPathTime   = level.inttime;
        m_vLastCheckPos[0] = controlledEntity->origin;
        m_vLastCheckPos[1] = controlledEntity->origin;

        if (failed && !m_bDirectMove && m_pPath && m_pPath->GetNodeCount()) {
            PathSearchParameter parameters;
            parameters.entity     = controlledEntity;
            parameters.fallHeight = maxFallHeight;
            m_pPath->FindPath(controlledEntity->origin, m_vTargetPos, parameters);
            m_iLastMoveTime = level.inttime;
        }

        return false;
    }

    const Vector wishDirection = CalculateRelativeWishDirection(m_vJumpDirection);
    botcmd.forwardmove = (signed char)Q_clamp_float(wishDirection.x * 127.0f, -127.0f, 127.0f);
    botcmd.rightmove   = (signed char)Q_clamp_float(-wishDirection.y * 127.0f, -127.0f, 127.0f);
    botcmd.upmove      = !m_bJumpWasAirborne ? 127 : 0;
    botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);

    m_bIsLeaning                  = false;
    m_bLeanCommandActive          = false;
    m_telemetry.movementSuppressed = true;
    return true;
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

    if (level.inttime < m_iJumpRetryTime) {
        m_bJump = false;
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
            m_bJump              = true;
            m_iJumpCommitTime    = level.inttime;
            m_bJumpWasAirborne   = false;
            m_iJumpLandingTime   = 0;
            m_vJumpLocation      = controlledEntity->origin;
            m_vJumpDirection     = dir;
            m_vJumpDirection.z   = 0;
            VectorNormalize2D(m_vJumpDirection);

            // The jump now owns movement until takeoff and landing. Clear
            // escape states that would otherwise steer sideways or backward.
            m_bAvoidCollision = false;
            m_iTempAwayState  = 0;
            m_iNumBlocks      = 0;
            m_iCheckPathTime  = level.inttime;
            ContinueJump(botcmd);
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

void BotMovement::MoveDirect(Vector vPos, float fRadius)
{
    if (m_pPath) {
        m_pPath->Clear();
    }

    m_vTargetPos        = vPos;
    m_vCurrentGoal      = vPos;
    m_vCurrentDir       = CalculateDir(vPos - controlledEntity->origin);
    m_bPathing          = true;
    m_bDirectMove       = true;
    m_fDirectMoveRadius = Q_max(0.0f, fRadius);
    m_bAvoidCollision               = false;
    m_iCollisionAvoidDirection      = 0;
    m_iCollisionAvoidDirectionUntil = 0;
    m_iTempAwayState                = 0;
    m_iNumBlocks                    = 0;
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
        if (m_fAttractTime) {
            if (level.time > m_fAttractTime) {
                AbandonAttractivePoint();
            }
            return true;
        }

        if (!IsMoving()) {
            MoveTo(m_pPrimaryAttract->origin);
        }

        if (!IsMoving()) {
            AbandonAttractivePoint();
        } else if (MoveDone()) {
            m_fAttractTime =
                level.time + m_pPrimaryAttract->m_fMaxStayTime;
            ClearMove();
        }

        return true;
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

void BotMovement::AbandonAttractivePoint()
{
    if (!m_pPrimaryAttract) {
        return;
    }

    nodeAttract_t *attract  = new nodeAttract_t;
    attract->m_fRespawnTime = level.time + m_pPrimaryAttract->m_fRespawnTime;
    attract->m_pNode        = m_pPrimaryAttract;
    m_attractList.AddObject(attract);

    m_pPrimaryAttract = NULL;
    m_fAttractTime    = 0;
}

/*
====================
NewMove

Called when there is a new move
====================
*/
void BotMovement::NewMove()
{
    m_bDirectMove       = false;
    m_bPathing          = true;
    m_bAvoidCollision               = false;
    m_iCollisionAvoidDirection      = 0;
    m_iCollisionAvoidDirectionUntil = 0;
    m_iTempAwayState                = 0;
    m_iNumBlocks                    = 0;
    m_vLastCheckPos[0]  = controlledEntity->origin;
    m_vLastCheckPos[1]  = controlledEntity->origin;

    if (m_pPath && m_pPath->GetNodeCount()) {
        m_pPath->UpdatePos(controlledEntity->origin);
        m_vCurrentDir = CalculateDir(m_pPath->GetCurrentDelta());
    } else {
        m_vCurrentDir = vec_zero;
    }
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

void BotMovement::AbandonCollisionAvoidance()
{
    m_bAvoidCollision        = false;
    m_iCollisionCheckTime    = 0;
    m_iCollisionProgressTime = 0;

    // Give the other side first choice when this detour is evaluated again.
    if (m_iCollisionAvoidDirection) {
        m_iCollisionAvoidDirection = -m_iCollisionAvoidDirection;
        m_iCollisionAvoidDirectionUntil =
            level.inttime + BOT_COLLISION_SIDE_COMMIT_MSEC;
    }
}

bool BotMovement::CollisionAvoidanceTargetClear(const Vector& target) const
{
    Vector mins = controlledEntity->mins;
    Vector maxs = controlledEntity->maxs;
    maxs.z -= STEPSIZE;

    const Vector start = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    Vector end = target;
    end.z = start.z;
    Vector direction = end - start;
    const float targetDistance = VectorNormalize2D(direction);
    if (targetDistance <= 0.0f) {
        return false;
    }

    // A committed detour stops within 16 units, while the final guard looks
    // two frames ahead. Include the possible overshoot in this validation.
    const float guardDistance =
        controlledEntity->GetRunSpeed() * level.frametime
            * BOT_COLLISION_LOOKAHEAD_FRAMES
        + BOT_COLLISION_SAFETY_MARGIN;
    const float endpointClearance =
        Q_max(0.0f, guardDistance - BOT_COLLISION_AVOID_REACHED_UNITS);
    end += direction * endpointClearance;

    const trace_t trace = G_Trace(
        start,
        mins,
        maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID | CONTENTS_BOTCLIP,
        true,
        "BotMovement::CollisionAvoidanceTargetClear"
    );
    if (trace.startsolid || trace.fraction < 1.0f) {
        return false;
    }

    const trace_t groundTrace = G_Trace(
        end,
        mins,
        maxs,
        end - Vector(0, 0, STEPSIZE * 4),
        controlledEntity,
        MASK_PLAYERSOLID | CONTENTS_BOTCLIP,
        true,
        "BotMovement::CollisionAvoidanceTargetClearGround"
    );
    return groundTrace.fraction < 1.0f;
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

    // Blocked recovery owns the escape direction. A stale collision detour
    // must not keep steering against it.
    if (m_iTempAwayState == 2) {
        m_bAvoidCollision               = false;
        m_iCollisionAvoidDirection      = 0;
        m_iCollisionAvoidDirectionUntil = 0;
        return delta;
    }

    // Commit to a chosen side long enough to clear the obstacle. Recomputing
    // both sides every 250 ms made equally open routes alternate at walls.
    if (m_bAvoidCollision) {
        if ((controlledEntity->origin - m_vCollisionProgressOrigin).lengthXYSquared()
            >= Square(BOT_COLLISION_PROGRESS_UNITS)) {
            m_vCollisionProgressOrigin = controlledEntity->origin;
            m_iCollisionProgressTime   = level.inttime;
        } else if (level.inttime
                   >= m_iCollisionProgressTime + BOT_COLLISION_STALL_MSEC) {
            // Re-evaluate the local obstacle instead of rebuilding the whole
            // route. A strategic repath here could turn a bot completely
            // around because one short collision detour stalled.
            AbandonCollisionAvoidance();
        }

        newDelta = m_vTempCollisionAvoidance - controlledEntity->origin;
        if (m_bAvoidCollision && !m_bJump
            && newDelta.lengthXYSquared() > Square(BOT_COLLISION_AVOID_REACHED_UNITS)
            && level.inttime < m_iCollisionCheckTime + BOT_COLLISION_AVOID_COMMIT_MSEC) {
            return newDelta;
        }

        m_bAvoidCollision        = false;
        m_iCollisionProgressTime = 0;
    }

    // This 32-unit path probe must run before the two-frame final guard. At
    // run speed a bot travels farther than that during the old 250 ms
    // throttle, so the guard visibly hit and slid along the obstacle before
    // the path layer had a chance to choose its committed detour.
    if (m_bJump) {
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
    if (trace.ent && trace.ent->entity
        && trace.ent->entity->IsSubclassOfDoor()) {
        Door *door = static_cast<Door *>(trace.ent->entity);
        if (door->CanBeOpenedBy(controlledEntity)) {
            // An unlocked door is part of the route, including while it is
            // opening. Do not treat its moving panel as a wall.
            m_iCollisionAvoidDirection      = 0;
            m_iCollisionAvoidDirectionUntil = 0;
            return delta;
        }
    }

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

        if (trace.ent && trace.ent->entity
            && trace.ent->entity->IsSubclassOfDoor()) {
            Door *door = static_cast<Door *>(trace.ent->entity);
            if (door->CanBeOpenedBy(controlledEntity)) {
                m_iCollisionAvoidDirection      = 0;
                m_iCollisionAvoidDirectionUntil = 0;
                return delta;
            }
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
            // Preserve the selected side through short re-evaluations. Without
            // this memory, near-equal traces can make a bot alternate sides at
            // walls and narrow doorframes.
            int preferredDirection = 0;
            if (level.inttime < m_iCollisionAvoidDirectionUntil) {
                if (m_iCollisionAvoidDirection < 0 && bestLeftFrac > 0.0f) {
                    preferredDirection = -1;
                } else if (m_iCollisionAvoidDirection > 0
                           && bestRightFrac > 0.0f) {
                    preferredDirection = 1;
                }
            }

            if (!preferredDirection && bestLeftFrac > bestRightFrac) {
                preferredDirection = -1;
            } else if (!preferredDirection && bestLeftFrac < bestRightFrac) {
                preferredDirection = 1;
            } else if (!preferredDirection
                       && Vector::DistanceSquared(bestLeftPos, dest)
                           > Vector::DistanceSquared(bestRightPos, dest)) {
                preferredDirection = 1;
            } else if (!preferredDirection) {
                preferredDirection = -1;
            }

            const int directions[2] = {preferredDirection, -preferredDirection};
            int selectedDirection = 0;
            Vector selectedTarget;
            for (int i = 0; i < 2 && !selectedDirection; ++i) {
                const int direction = directions[i];
                const float fraction = direction < 0 ? bestLeftFrac : bestRightFrac;
                if (fraction <= 0.0f) {
                    continue;
                }

                const Vector sideTarget = direction < 0 ? bestLeftPos : bestRightPos;
                const Vector forwardTarget = sideTarget + forward * 64;
                if (CollisionAvoidanceTargetClear(forwardTarget)) {
                    selectedTarget = forwardTarget;
                } else if (CollisionAvoidanceTargetClear(sideTarget)) {
                    selectedTarget = sideTarget;
                } else {
                    continue;
                }
                selectedDirection = direction;
            }

            if (selectedDirection) {
                m_bAvoidCollision          = true;
                m_iCollisionProgressTime   = level.inttime;
                m_vCollisionProgressOrigin = controlledEntity->origin;
                m_vTempCollisionAvoidance  = selectedTarget;
                m_iCollisionAvoidDirection = selectedDirection;
                m_iCollisionAvoidDirectionUntil = level.inttime + BOT_COLLISION_SIDE_COMMIT_MSEC;
                return m_vTempCollisionAvoidance - controlledEntity->origin;
            }
        }
    }

    if (level.inttime >= m_iCollisionAvoidDirectionUntil) {
        m_iCollisionAvoidDirection = 0;
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

    if (m_bDirectMove) {
        return (m_vTargetPos - controlledEntity->origin).lengthXYSquared()
            <= Square(m_fDirectMoveRadius);
    }

    if (!m_pPath) {
        return true;
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

bool BotMovement::IsMovingTo(const Vector& position, float tolerance) const
{
    if (!m_bPathing) {
        return false;
    }
    return (m_vTargetPos - position).lengthSquared() <= Square(tolerance);
}

/*
====================
ClearMove

Stop the bot from moving
====================
*/
void BotMovement::ClearMove(void)
{
    m_bPathing          = false;
    m_bDirectMove       = false;
    if (!controlledEntity || controlledEntity->IsDead()
        || (!controlledEntity->GetLadder() && !m_iLadderExitUntil)) {
        m_bWasOnLadder         = false;
        m_fLadderTop           = 0.0f;
        m_iLadderExitUntil     = 0;
        m_vLadderExitOrigin    = vec_zero;
        m_vLadderExitDirection = vec_zero;
    }
    m_bAvoidCollision               = false;
    m_iCollisionProgressTime        = 0;
    m_vCollisionProgressOrigin      = vec_zero;
    m_iCollisionAvoidDirection      = 0;
    m_iCollisionAvoidDirectionUntil = 0;
    m_iTempAwayState                = 0;
    m_iNumBlocks                    = 0;
    m_bJump             = false;
    m_iJumpCommitTime   = -1;
    m_iJumpLandingTime  = 0;
    m_iJumpRetryTime    = 0;
    m_bJumpWasAirborne  = false;
    m_vCurrentDir              = vec_zero;
    m_iStrafeGeometryLockTime  = 0;
    m_iMovementOverlaySuppressUntil = 0;
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
    if (m_bDirectMove || !m_pPath || !m_pPath->GetNodeCount()) {
        return m_vCurrentGoal;
    }

    if (!m_pPath->HasReachedGoal(controlledEntity->origin) && m_pPath->GetNodeCount()) {
        const Vector delta = m_pPath->GetCurrentDelta();
        return controlledEntity->origin + Vector(delta[0], delta[1], 0);
    }

    return controlledEntity->origin;
}

Vector BotMovement::GetCurrentMoveDirection() const
{
    return m_vCurrentDir;
}

void BotMovement::ResetTelemetry()
{
    m_telemetry.Reset();
    m_bLeanCommandActive = false;
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
    telemetry.isLeaning              = m_bLeanCommandActive;
    telemetry.radialDirection        = m_iRadialDirection;
    telemetry.radialChangeMsec  =
        m_iNextRadialChangeTime ? Q_max(0, m_iNextRadialChangeTime - level.inttime) : -1;
}

float BotMovement::CalculateStrafeProbeFraction(const usercmd_t& botcmd, int direction)
{
    if (direction == 0 || !controlledEntity) {
        return 0.0f;
    }

    usercmd_t probeCommand = botcmd;
    const int offset =
        (int)(direction * g_bot_strafe_intensity->value * 127.0f);
    int probeRight = (int)probeCommand.rightmove + offset;
    probeCommand.rightmove =
        (signed char)Q_clamp(probeRight, -127, 127);

    Vector probeDirection = GetCommandMoveVector(probeCommand);
    if (VectorNormalize2D(probeDirection) <= 0.0f) {
        return 0.0f;
    }

    const float probeDistance = BOT_STRAFE_PROBE_DISTANCE;
    Vector mins = controlledEntity->mins;
    Vector maxs = controlledEntity->maxs;
    maxs.z -= STEPSIZE;
    const Vector start =
        controlledEntity->origin + Vector(0, 0, STEPSIZE);
    const Vector end = start + probeDirection * probeDistance;
    const trace_t trace = G_Trace(
        start,
        mins,
        maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID | CONTENTS_BOTCLIP,
        false,
        "BotMovement::CalculateStrafeProbeFraction"
    );

    return trace.fraction;
}

// Pick a random dwell time from a min/max interval cvar pair (milliseconds).
static int RandomInterval(cvar_t *lo, cvar_t *hi)
{
    const int lower = Q_min(lo->integer, hi->integer);
    const int upper = Q_max(lo->integer, hi->integer);
    return lower + (int)G_Random(upper - lower);
}

void BotMovement::FinalizeMovement(usercmd_t& botcmd)
{
    const usercmd_t baseCommand = botcmd;
    UpdateAggressiveMovement(botcmd);
    ResolveImminentCollision(botcmd, baseCommand);
}

/*
====================
UpdateAggressiveMovement

Movement layer calibrated from human-versus-human telemetry: continuous
side-to-side strafing with matching lean, plus slower movement toward and away
from nearby enemies. Lean follows actual lateral movement instead of running as
an independent animation.
====================
*/
void BotMovement::UpdateAggressiveMovement(usercmd_t& botcmd)
{
    // Ladders: leave pathing untouched.
    if (controlledEntity->GetLadder()) {
        m_bIsLeaning = false;
        m_iStrafeGeometryLockTime = 0;
        return;
    }

    // Strafe and lean are an overlay for purposeful travel and combat, not a
    // source of movement by themselves. A completed cover route remains a
    // genuine hold until another route is assigned.
    if (!IsMoving() && !m_bHasCombatTarget) {
        m_bIsLeaning = false;
        m_iStrafeGeometryLockTime = 0;
        return;
    }

    // Recovery and collision detours own the movement vector; lateral input
    // here would fight their selected escape side.
    const bool suppressMovement = m_iTempAwayState == 2 || m_bAvoidCollision
        || level.inttime < m_iMovementOverlaySuppressUntil;
    m_telemetry.movementSuppressed = suppressMovement;

    if (suppressMovement) {
        if (botcmd.rightmove < -8) {
            m_iStrafeDirection = -1;
        } else if (botcmd.rightmove > 8) {
            m_iStrafeDirection = 1;
        }
        m_iStrafeGeometryLockTime =
            level.inttime + BOT_STRAFE_GEOMETRY_LOCK_MSEC;
        m_iNextStrafeChangeTime =
            Q_max(m_iNextStrafeChangeTime, m_iStrafeGeometryLockTime);
    }

    // Select the side before probing it. Probing first used the clearance for
    // the old side on the exact frame the oscillator changed direction.
    if (level.inttime >= m_iNextStrafeChangeTime
        && level.inttime >= m_iStrafeGeometryLockTime) {
        m_iStrafeDirection = -m_iStrafeDirection;
        m_iNextStrafeChangeTime =
            level.inttime + RandomInterval(g_bot_strafe_min_interval, g_bot_strafe_max_interval);
    }

    m_bIsLeaning = false;
    if (!suppressMovement) {
        // Probe the actual path-plus-strafe command. Human telemetry retains
        // substantial strafe in narrow spaces, so geometry scales this overlay
        // instead of reversing it or choosing a second movement direction.
        const float probeFraction =
            CalculateStrafeProbeFraction(botcmd, m_iStrafeDirection);
        const float intensity =
            g_bot_strafe_intensity->value * probeFraction;
        const int offset =
            (int)(m_iStrafeDirection * intensity * 127.0f);

        m_telemetry.strafeProbeFraction = probeFraction;
        m_telemetry.strafeClearance =
            probeFraction * BOT_STRAFE_PROBE_DISTANCE;
        m_telemetry.strafeIntensity = intensity;

        if (offset) {
            int newRight = (int)botcmd.rightmove + offset;
            botcmd.rightmove =
                (signed char)Q_clamp(newRight, -127, 127);
            m_telemetry.strafeApplied = true;
            m_bIsLeaning = true;
        }
    }

    if (m_bHasCombatTarget) {
        UpdateCombatRadialMovement(botcmd, suppressMovement);
    }

    // Lean follows the final lateral command, after the combat radial layer.
    // The collision resolver performs the same check if it removes a component.
    if (m_bIsLeaning) {
        const bool matchesDirection = m_iStrafeDirection < 0
            ? botcmd.rightmove < -8
            : botcmd.rightmove > 8;
        if (matchesDirection) {
            if (m_iStrafeDirection < 0) {
                botcmd.buttons |= BUTTON_LEAN_LEFT;
            } else {
                botcmd.buttons |= BUTTON_LEAN_RIGHT;
            }
            m_bLeanCommandActive = true;
        } else {
            m_bIsLeaning = false;
        }
    }
}

void BotMovement::DirectMoveThink(usercmd_t& botcmd)
{
    Vector delta = m_vTargetPos - controlledEntity->origin;
    delta.z      = 0.0f;

    if (delta.lengthXYSquared() <= Square(m_fDirectMoveRadius)) {
        ClearMove();
        FinalizeMovement(botcmd);
        return;
    }

    delta          = FixDeltaFromCollision(delta);
    m_vCurrentGoal = controlledEntity->origin + delta;
    m_vCurrentDir  = CalculateDir(delta);
    SteerTowardPathHealth(m_vCurrentDir);

    const Vector wishDirection = CalculateRelativeWishDirection(m_vCurrentDir);
    botcmd.forwardmove = (signed char)Q_clamp_float(wishDirection.x * 127.0f, -127.0f, 127.0f);
    botcmd.rightmove   = (signed char)Q_clamp_float(-wishDirection.y * 127.0f, -127.0f, 127.0f);
    botcmd.upmove      = 0;

    FinalizeMovement(botcmd);
    CheckJump(botcmd);
    if (!m_bJump) {
        CheckJumpOverEdge(botcmd);
    }
}

void BotMovement::SteerTowardPathHealth(Vector& direction) const
{
    if (!controlledEntity
        || controlledEntity->health + controlledEntity->m_fHealRate
            >= controlledEntity->max_health) {
        return;
    }

    Vector pathDirection = direction;
    pathDirection.z      = 0.0f;
    if (VectorNormalize2D(pathDirection) <= 0.0f) {
        return;
    }

    Entity *bestHealth  = NULL;
    float   bestForward = BOT_HEALTH_PATH_LOOKAHEAD + 1.0f;

    for (Entity *entity = findradius(
             NULL, controlledEntity->origin, BOT_HEALTH_PATH_LOOKAHEAD
         );
         entity;
         entity = findradius(
             entity, controlledEntity->origin, BOT_HEALTH_PATH_LOOKAHEAD
         )) {
        if (!entity->isSubclassOf(Health) || entity->hidden()
            || entity->getSolidType() == SOLID_NOT) {
            continue;
        }

        Vector offset = entity->origin - controlledEntity->origin;
        if (fabs(offset.z) > STEPSIZE * 2.0f) {
            continue;
        }
        offset.z = 0.0f;

        const float forward = DotProduct(offset, pathDirection);
        if (forward <= 0.0f || forward >= bestForward) {
            continue;
        }

        const Vector lateral = offset - pathDirection * forward;
        if (lateral.lengthXYSquared() > Square(BOT_HEALTH_PATH_CORRIDOR)) {
            continue;
        }

        Vector mins = controlledEntity->mins;
        Vector maxs = controlledEntity->maxs;
        maxs.z -= STEPSIZE;
        const Vector start =
            controlledEntity->origin + Vector(0, 0, STEPSIZE);
        const Vector end = entity->origin + Vector(0, 0, STEPSIZE);
        const trace_t trace = G_Trace(
            start,
            mins,
            maxs,
            end,
            controlledEntity,
            MASK_PLAYERSOLID | CONTENTS_BOTCLIP,
            true,
            "BotMovement::SteerTowardPathHealth"
        );
        if (trace.startsolid || trace.fraction < 1.0f) {
            continue;
        }

        bestHealth  = entity;
        bestForward = forward;
    }

    if (bestHealth) {
        direction = CalculateDir(bestHealth->origin - controlledEntity->origin);
    }
}

int BotMovement::ChooseRadialDirection(float distance) const
{
    // SMG telemetry from two human matches showed three distinct radial
    // states: closing, orbiting with no radial input, and retreating. Humans
    // were balanced near each other and increasingly favored advance/orbit
    // over retreat as distance grew.
    int advanceChance;
    int orbitChance;

    if (distance < 96.0f) {
        advanceChance = 35;
        orbitChance   = 30;
    } else if (distance < 128.0f) {
        advanceChance = 40;
        orbitChance   = 40;
    } else {
        advanceChance = 40;
        orbitChance   = 50;
    }

    const float roll = G_Random(100.0f);
    if (roll < advanceChance) {
        return 1;
    }
    if (roll < advanceChance + orbitChance) {
        return 0;
    }
    return -1;
}

int BotMovement::RadialPhaseDuration() const
{
    return Q_max(50, RandomInterval(g_bot_peek_min_interval, g_bot_peek_max_interval));
}

void BotMovement::UpdateCombatRadialMovement(usercmd_t& botcmd, bool suppressMovement)
{
    const float maxDistance = g_bot_peek_distance->value;
    if ((suppressMovement && !m_bForceCombatRetreat) || !m_bHasCombatTarget
        || (!m_bForceCombatRetreat && maxDistance <= 0)) {
        m_iRadialDirection      = 0;
        m_iNextRadialChangeTime = 0;
        return;
    }

    Vector      towardEnemy = m_vCombatTarget - controlledEntity->origin;
    const float distance    = VectorNormalize2D(towardEnemy);
    m_telemetry.radialDistance = distance;
    if (distance <= 0 || (!m_bForceCombatRetreat && distance >= maxDistance)) {
        m_iRadialDirection      = 0;
        m_iNextRadialChangeTime = 0;
        return;
    }

    if (m_bForceCombatRetreat) {
        m_iRadialDirection      = -1;
        m_iNextRadialChangeTime = 0;
        m_telemetry.radialForcedCloseRetreat = true;
    } else if (m_bForceCombatAdvance) {
        m_iRadialDirection      = 1;
        m_iNextRadialChangeTime = 0;
    } else if (distance < 56.0f) {
        // Resolve unsafe spacing immediately rather than waiting for the
        // current phase to expire. Leaving this range starts a fresh phase.
        m_iRadialDirection      = -1;
        m_iNextRadialChangeTime = 0;
        m_telemetry.radialForcedCloseRetreat = true;
    } else if (!m_iNextRadialChangeTime || level.inttime >= m_iNextRadialChangeTime) {
        m_iRadialDirection      = ChooseRadialDirection(distance);
        m_iNextRadialChangeTime = level.inttime + RadialPhaseDuration();
    }

    const float preRadialCommandMax =
        Q_max(fabs((float)botcmd.forwardmove), fabs((float)botcmd.rightmove));
    Vector move = GetCommandMoveVector(botcmd);
    m_telemetry.radialActive = true;

    // Keep the radial component deliberately slower than the lateral strafe,
    // producing broad arcs instead of straight charges.
    float desiredRadialMove;
    if (m_bForceCombatRetreat) {
        desiredRadialMove = -127.0f;
    } else if (m_bForceCombatAdvance) {
        desiredRadialMove = 127.0f;
    } else if (distance < 56.0f) {
        desiredRadialMove = -48.0f;
    } else if (m_iRadialDirection < 0) {
        desiredRadialMove = distance < 96.0f ? -40.0f : -28.0f;
    } else if (m_iRadialDirection > 0) {
        desiredRadialMove = distance < 96.0f ? 32.0f : 36.0f;
    } else {
        desiredRadialMove = 0.0f;
    }

    const float currentRadialMove = DotProduct(move, towardEnemy);
    m_telemetry.radialDesiredMove = desiredRadialMove;
    m_telemetry.radialBeforeMove  = currentRadialMove;
    move += towardEnemy * (desiredRadialMove - currentRadialMove);

    // The radial values above bias direction; they are not a speed target.
    // Replacing a large path component with the deliberately small radial
    // component can collapse the whole command to walking speed, which reads
    // as the bot skating while firing. PM_CmdScale keys speed off the largest
    // command component, so scale the shaped move back up until its largest
    // component regains the pre-radial command magnitude. The direction blend
    // is preserved and no clearance gate is involved, so doorways and narrow
    // spaces keep full run speed. Scale up only: with no path and no strafe
    // room the radial component is the entire intended movement and keeps its
    // deliberate slow pacing. The collision guard still runs after this layer
    // and removes unsafe movement components.
    Vector angles = controlledEntity->angles;
    Vector forward, left, up;

    angles.x = 0;
    angles.z = 0;
    angles.AngleVectorsLeft(&forward, &left, &up);

    const float postRadialCommandMax =
        Q_max(fabs(DotProduct(move, forward)), fabs(DotProduct(move, left)));
    if (postRadialCommandMax > 0.0f && postRadialCommandMax < preRadialCommandMax) {
        move *= preRadialCommandMax / postRadialCommandMax;
    }

    SetCommandMoveVector(botcmd, move);
}

bool BotMovement::TraceImminentMove(const usercmd_t& botcmd, trace_t& trace) const
{
    if (!controlledEntity) {
        return false;
    }

    Vector move = GetCommandMoveVector(botcmd);
    if (move.lengthXYSquared() <= 1.0f) {
        return false;
    }

    const float commandFraction =
        Q_max(fabs((float)botcmd.forwardmove), fabs((float)botcmd.rightmove)) / 127.0f;
    const float frameDistance =
        controlledEntity->GetRunSpeed() * commandFraction * level.frametime;
    if (frameDistance <= 0.0f) {
        return false;
    }

    Vector direction = move;
    VectorNormalize2D(direction);

    // Resolve the fully shaped command two frames ahead. This is the single
    // final authority for path, strafe, lean, and combat-radial movement.
    const float lookAheadDistance =
        frameDistance * BOT_COLLISION_LOOKAHEAD_FRAMES
        + BOT_COLLISION_SAFETY_MARGIN;
    Vector mins = controlledEntity->mins;
    Vector maxs = controlledEntity->maxs;
    maxs.z -= STEPSIZE;

    const Vector start = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    const Vector end   = start + direction * lookAheadDistance;
    trace = G_Trace(
        start,
        mins,
        maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID | CONTENTS_BOTCLIP,
        true,
        "BotMovement::TraceImminentMove"
    );
    return trace.startsolid || trace.fraction < 1.0f;
}

void BotMovement::ResolveImminentCollision(usercmd_t& botcmd, const usercmd_t& baseCommand)
{
    if (!controlledEntity || controlledEntity->GetLadder() || m_bJump) {
        return;
    }

    Vector move = GetCommandMoveVector(botcmd);
    trace_t trace;
    if (!TraceImminentMove(botcmd, trace)) {
        return;
    }

    const bool overlayChangedCommand =
        botcmd.forwardmove != baseCommand.forwardmove
        || botcmd.rightmove != baseCommand.rightmove;
    if ((m_telemetry.strafeApplied || m_telemetry.radialActive)
        && overlayChangedCommand) {
        trace_t baseTrace;
        const bool baseBlocked = TraceImminentMove(baseCommand, baseTrace);

        // Navigation remains authoritative when an optional combat overlay
        // causes a collision. If the base path also collides, resolve that
        // path vector instead of sliding the unrelated overlay vector.
        botcmd.forwardmove = baseCommand.forwardmove;
        botcmd.rightmove   = baseCommand.rightmove;
        botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);
        m_bIsLeaning         = false;
        m_bLeanCommandActive = false;
        m_iMovementOverlaySuppressUntil =
            level.inttime + BOT_MOVEMENT_OVERLAY_SUPPRESS_MSEC;
        m_iNextStrafeChangeTime =
            Q_max(m_iNextStrafeChangeTime, m_iMovementOverlaySuppressUntil);
        m_iRadialDirection      = 0;
        m_iNextRadialChangeTime = 0;
        m_telemetry.movementSuppressed = true;
        m_telemetry.strafeApplied      = false;
        m_telemetry.strafeIntensity    = 0.0f;
        m_telemetry.radialActive       = false;
        m_telemetry.radialDesiredMove  = 0.0f;

        if (!baseBlocked) {
            const bool hitSentient = trace.ent && trace.ent->entity
                && trace.ent->entity->IsSubclassOfSentient();
            m_telemetry.guardTriggered   = true;
            m_telemetry.guardHitSentient = hitSentient;
            m_telemetry.guardHitWorld    = trace.entityNum == ENTITYNUM_WORLD;
            m_telemetry.guardFraction    = trace.fraction;
            m_telemetry.guardEntity      = trace.entityNum;
            m_telemetry.guardRemovedComponent = true;
            return;
        }

        move  = GetCommandMoveVector(botcmd);
        trace = baseTrace;
    }

    const bool hitSentient = trace.ent && trace.ent->entity
        && trace.ent->entity->IsSubclassOfSentient();
    m_telemetry.guardTriggered   = true;
    m_telemetry.guardHitSentient = hitSentient;
    m_telemetry.guardHitWorld    = trace.entityNum == ENTITYNUM_WORLD;
    m_telemetry.guardFraction    = trace.fraction;
    m_telemetry.guardEntity      = trace.entityNum;

    Vector collisionNormal = trace.plane.normal;
    collisionNormal.z      = 0.0f;
    if (collisionNormal.lengthXYSquared() < 0.01f && hitSentient) {
        collisionNormal   = controlledEntity->origin - trace.ent->entity->origin;
        collisionNormal.z = 0.0f;
    }
    if (VectorNormalize2D(collisionNormal) > 0.0f) {
        const float intoObstacle = DotProduct(move, collisionNormal);
        if (intoObstacle < 0.0f) {
            // Project onto the collision plane. Tangential movement survives,
            // matching human wall-sliding without entering the first obstacle.
            move += collisionNormal * -intoObstacle;
            SetCommandMoveVector(botcmd, move);

            // A tight corner can put that tangent into a second plane.
            trace_t resolvedTrace;
            if (TraceImminentMove(botcmd, resolvedTrace)) {
                botcmd.forwardmove = 0;
                botcmd.rightmove   = 0;
            }
        } else {
            // This is only expected while already touching an obstacle.
            botcmd.forwardmove = 0;
            botcmd.rightmove   = 0;
        }
    } else {
        // An indeterminate collision normal is not safe to move through.
        botcmd.forwardmove = 0;
        botcmd.rightmove   = 0;
    }

    const float resolvedCommand = Q_max(
        fabs((float)botcmd.forwardmove),
        fabs((float)botcmd.rightmove)
    );
    if (m_bAvoidCollision && !hitSentient
        && resolvedCommand <= 8.0f) {
        AbandonCollisionAvoidance();
    }

    m_telemetry.guardRemovedComponent = true;

    if (m_bLeanCommandActive) {
        const bool leanMatchesMovement =
            ((botcmd.buttons & BUTTON_LEAN_LEFT) && botcmd.rightmove < -8)
            || ((botcmd.buttons & BUTTON_LEAN_RIGHT) && botcmd.rightmove > 8);
        if (!leanMatchesMovement) {
            botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);
            m_bLeanCommandActive = false;
            m_bIsLeaning         = false;
        }
    }
}
