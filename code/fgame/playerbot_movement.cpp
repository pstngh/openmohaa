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
#include "movement_telemetry.h"
#include "debuglines.h"
#include "health.h"
#include "misc.h"

static int       maxFallHeight                   = 400;
static const int   BOT_LADDER_STALL_MSEC           = 1000;
static const float BOT_LADDER_PROGRESS_UNITS       = 16.0f;
static const int   BOT_LADDER_EXIT_MAX_MSEC        = 1000;
static const float BOT_LADDER_EXIT_DISTANCE        = 64.0f;
static const int   BOT_COLLISION_AVOID_COMMIT_MSEC = 750;
static const float BOT_GUARD_RECOVERY_COMMAND_MAX   = 24.0f;
static const float BOT_COLLISION_AVOID_REACHED_UNITS = 16.0f;
static const int   BOT_COLLISION_SIDE_COMMIT_MSEC  = 1200;
static const int   BOT_COLLISION_STALL_MSEC        = 350;
static const float BOT_COLLISION_LOOKAHEAD_FRAMES = 2.0f;
static const float BOT_COLLISION_SAFETY_MARGIN    = 2.0f;
static const float BOT_COLLISION_PROGRESS_UNITS    = 24.0f;
static const float BOT_COLLISION_PATH_PROBE_DISTANCE = 64.0f;
static const int   BOT_REDUCED_STANCE_RECOVERY_MSEC = 1500;
static const int BOT_JUMP_TAKEOFF_MSEC            = 250;
static const int BOT_JUMP_COMMIT_MAX_MSEC         = 1000;
static const int BOT_JUMP_LANDING_COMMIT_MSEC      = 250;
static const int BOT_JUMP_RETRY_MSEC              = 500;
static const float BOT_HEALTH_PATH_LOOKAHEAD       = 192.0f;
static const float BOT_HEALTH_PATH_CORRIDOR        = 48.0f;
static const int   BOT_STRAFE_GEOMETRY_LOCK_MSEC   = 1200;
static const int   BOT_MOVEMENT_OVERLAY_SUPPRESS_MSEC = 350;
static const int   BOT_DOOR_PUSH_STALL_MSEC            = 350;
static const int   BOT_DOOR_PUSH_RECONTACT_MSEC        = 1500;
static const int   BOT_DOOR_EXIT_COMMIT_MSEC           = 750;
static const float BOT_DOOR_PUSH_PROBE_DISTANCE        = 64.0f;
static const float BOT_DOOR_PUSH_PROBE_EPSILON         = 0.05f;
static const float BOT_DOOR_OPEN_EDGE_BLOCKED_FRACTION = 0.25f;
static const float BOT_DOOR_OPEN_EDGE_CLEARANCE_MARGIN = 0.25f;
static const float BOT_DOOR_PUSH_SIDE_COMMAND          = 127.0f;
static const float BOT_STRAFE_PROBE_DISTANCE          = 56.0f;
static const float BOT_ROAM_STRAFE_VETO_DISTANCE      = 112.0f;
static const float BOT_ROAM_STRAFE_CLEARANCE_EPSILON  = 0.05f;
static const int   BOT_DOOR_PUSH_LOG_MSEC              = 1000;
static const int   BOT_LOCAL_LOOP_SAMPLE_MSEC         = 500;
static const int   BOT_LOCAL_LOOP_MIN_AGE_MSEC        = 3000;
static const int   BOT_LOCAL_LOOP_MAX_AGE_MSEC        = 8000;
static const int   BOT_LOCAL_LOOP_COOLDOWN_MSEC       = 8000;
static const float BOT_LOCAL_LOOP_RETURN_UNITS        = 64.0f;
static const float BOT_LOCAL_LOOP_TARGET_UNITS        = 32.0f;
static const float BOT_LOCAL_LOOP_MIN_TRAVEL_UNITS    = 320.0f;
static const float BOT_LOCAL_LOOP_TELEPORT_UNITS      = 256.0f;
static const float BOT_BLOCKED_RECOVERY_SIDE_UNITS    = 96.0f;
static const float BOT_BLOCKED_RECOVERY_FORWARD_UNITS = 32.0f;
static const float BOT_BLOCKED_RECOVERY_BACK_UNITS    = 96.0f;
static const float BOT_ROUTE_TURN_LIMIT_SPEED          = 80.0f;
static const float BOT_ROUTE_TURN_LIMIT_MIN_DEGREES    = 45.0f;
static const float BOT_ROUTE_TURN_RATE_DEGREES         = 720.0f;
static const float BOT_ROUTE_TURN_PROBE_DISTANCE       = 64.0f;
static const float BOT_ROUTE_TURN_CLEARANCE_EPSILON    = 0.05f;
static const int   BOT_ROUTE_TURN_RESET_MSEC           = 250;
static const int   BOT_LEAN_RELEASE_GRACE_MSEC         = 100;
static const int   BOT_LEAN_SWITCH_NEUTRAL_MSEC        = 250;

static Door *BotTraceDoor(const trace_t& trace)
{
    if (!trace.ent || !trace.ent->entity
        || !trace.ent->entity->IsSubclassOfDoor()) {
        return nullptr;
    }

    return static_cast<Door *>(trace.ent->entity);
}

static Door *BotTraceOpenableDoor(const trace_t& trace, Player *player)
{
    Door *door = BotTraceDoor(trace);
    if (!door) {
        return nullptr;
    }

    // A fully open panel is ordinary collision at its displaced position.
    // Recast and the final movement guard already account for that geometry;
    // treating it as still openable starts another push episode against a
    // door that has nowhere left to move.
    if (door->isOpen()) {
        return nullptr;
    }

    return door->CanBeOpenedBy(player) ? door : nullptr;
}

static bool BotDoorOpeningEdgeDirection(Door *door, Vector& direction)
{
    if (!door || !door->isSubclassOf(RotatingDoor)) {
        direction = vec_zero;
        return false;
    }

    // Rotating door brushes use their origin as the hinge. The center of the
    // current world-space bounds therefore points from the hinge toward the
    // free edge that a human naturally pushes while the panel swings open.
    const Vector doorCenter = (door->absmin + door->absmax) * 0.5f;
    direction = doorCenter - door->origin;
    direction.z = 0.0f;
    return VectorNormalize2D(direction) > 4.0f;
}

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
    m_fLadderProgressHeight = 0.0f;
    m_iLadderProgressTime   = 0;
    m_iLadderExitUntil  = 0;
    m_vLadderExitOrigin = vec_zero;
    m_vLadderExitDirection = vec_zero;
    m_fDirectMoveRadius = 0.0f;
    m_iTempAwayState    = 0;
    m_fAttractTime      = 0;

    m_iCheckPathTime = 0;
    m_iTempAwayTime  = 0;
    m_iNumBlocks     = 0;
    m_vLocalLoopLastOrigin     = vec_zero;
    m_fLocalLoopTravelTotal    = 0.0f;
    m_iLocalLoopSampleCount    = 0;
    m_iLocalLoopNextSampleTime = 0;
    m_iLocalLoopCooldownUntil  = 0;
    m_bLocalOscillation        = false;

    m_bAvoidCollision          = false;
    m_iCollisionCheckTime      = 0;
    m_iCollisionProgressTime   = 0;
    m_vCollisionProgressOrigin = vec_zero;
    m_iCollisionAvoidDirection = 0;
    m_iCollisionAvoidDirectionUntil = 0;
    m_iReducedStanceStartTime  = 0;
    m_iDoorPushLogTime         = 0;
    m_iDoorPushEntity          = ENTITYNUM_NONE;
    m_iDoorPushStartTime       = 0;
    m_iDoorPushLastContactTime = 0;
    m_iDoorPushBlockedLogTime  = 0;
    m_iOpenDoorPanelLogTime    = 0;
    m_iOpenDoorPanelEntity     = ENTITYNUM_NONE;
    m_vDoorPushDirection       = vec_zero;
    m_vDoorPushApproachDirection = vec_zero;
    m_vRouteCommandDirection   = vec_zero;
    m_iRouteCommandTime        = 0;
    m_iRouteTurnLogTime        = 0;
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
    m_bRoamStrafeActive     = false;
    m_bIsLeaning            = false;
    m_bLeanCommandActive    = false;
    m_iLeanDirection        = 0;
    m_iLeanLastAppliedTime  = 0;
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

    UpdateLocalLoopDetection();

    botcmd.forwardmove = 0;
    botcmd.rightmove   = 0;
    // The bot usercmd persists across frames: start each movement frame
    // lean-neutral so a lean can't stay latched after strafing stops.
    botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);

    RecoverStandingStance();

    Entity *ladder = controlledEntity->GetLadder();
    if (ladder && m_iLadderExitUntil) {
        // Attachment detection can reacquire the same volume on the frame
        // after a detach. The exit commitment remains the movement owner
        // until it reaches clearance or expires, so reject that attachment
        // instead of canceling the commitment and snapping back to the ladder.
        controlledEntity->UnattachFromLadder(NULL);
        ladder = NULL;
    }
    if (ladder) {
        if (!m_bWasOnLadder) {
            m_fLadderProgressHeight = controlledEntity->origin.z;
            m_iLadderProgressTime   = level.inttime;
        } else if (fabs(controlledEntity->origin.z - m_fLadderProgressHeight)
                   >= BOT_LADDER_PROGRESS_UNITS) {
            m_fLadderProgressHeight = controlledEntity->origin.z;
            m_iLadderProgressTime   = level.inttime;
        }

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
        m_fLadderProgressHeight = 0.0f;
        m_iLadderProgressTime   = 0;

        const bool groundedLadderExit =
            controlledEntity->groundentity
            || controlledEntity->client->ps.walking;
        const bool reachedLadderTop =
            controlledEntity->origin.z >= m_fLadderTop;
        if ((reachedLadderTop || groundedLadderExit)
            && m_vLadderExitDirection.lengthXYSquared() > 0.0f) {
            // FuncLadder places a bottom user behind the panel
            // (-facingDir), while a completed top-off leaves in +facingDir.
            // Move farther from the panel at the bottom instead of crossing
            // back through the ladder volume.
            if (groundedLadderExit && !reachedLadderTop) {
                m_vLadderExitDirection *= -1.0f;
            }
            m_vLadderExitOrigin = controlledEntity->origin;
            m_iLadderExitUntil =
                level.inttime + BOT_LADDER_EXIT_MAX_MSEC;
            m_vCurrentDir = m_vLadderExitDirection;

            if (groundedLadderExit && !reachedLadderTop) {
                G_MoveLogBotEvent(
                    "bot_ladder_ground_exit",
                    controlledEntity,
                    NULL,
                    0,
                    controlledEntity->origin
                        + m_vLadderExitDirection
                            * BOT_LADDER_EXIT_DISTANCE
                );
            }

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
        if (controlledEntity->GetLadder()) {
            // CheckJump owns ladder-release input. Keep it reachable when a
            // path search fails during a ladder transition.
            CheckJump(botcmd);
        }
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
        m_pPath->SetRouteComfortInsetEnabled(AllowRouteComfortInset());
        m_pPath->UpdatePos(controlledEntity->origin);
    }

    if (!m_pPath->GetNodeCount() && m_iTempAwayState != 2) {
        ClearMove();
        FinalizeMovement(botcmd);
        return;
    }

    if (m_pPath->GetNodeCount()) {
        vDelta = m_pPath->GetCurrentDelta();
        if (m_pPath->UsesLegacyCollisionAvoidance()) {
            vDelta = FixDeltaFromCollision(vDelta);
        }

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

            m_iTempAwayState = 2;
            m_iTempAwayTime  = level.inttime;
            m_iNumBlocks++;
            m_bAvoidCollision = false;

            // Prefer a committed lateral escape around the obstruction. The
            // old recovery target mixed the remaining path delta with a
            // reverse vector, so it repeatedly backed out and re-entered the
            // same doorway or corner when the path was issued again.
            if (m_pPath->GetNodeCount()) {
                delta = m_pPath->GetCurrentDelta();
            } else {
                delta = m_vTargetPos - controlledEntity->origin;
            }

            m_pPath->Clear();
            m_vCurrentGoal = ChooseBlockedRecoveryGoal(delta);
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
    ApplyRouteDirectionContinuity(m_vCurrentDir);

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

void BotMovement::ApplyRouteDirectionContinuity(Vector& direction)
{
    direction.z = 0.0f;
    if (VectorNormalize2D(direction) <= 0.0f) {
        m_vRouteCommandDirection = vec_zero;
        m_iRouteCommandTime      = level.inttime;
        return;
    }

    const bool explicitMovementOwner = !controlledEntity
        || m_bDirectMove || m_bHasCombatTarget
        || controlledEntity->GetLadder() || m_bJump
        || m_iTempAwayState == 2 || m_bAvoidCollision
        || (m_vDoorPushApproachDirection.lengthXYSquared() > 0.01f
            && level.inttime <= m_iDoorPushLastContactTime
                + BOT_DOOR_EXIT_COMMIT_MSEC);
    const bool resetContinuity = explicitMovementOwner
        || !m_iRouteCommandTime
        || level.inttime > m_iRouteCommandTime + BOT_ROUTE_TURN_RESET_MSEC
        || controlledEntity->velocity.lengthXYSquared()
            < Square(BOT_ROUTE_TURN_LIMIT_SPEED)
        || m_vRouteCommandDirection.lengthXYSquared() <= 0.01f;
    if (resetContinuity) {
        m_vRouteCommandDirection = direction;
        m_iRouteCommandTime      = level.inttime;
        return;
    }

    const float yawDelta = AngleNormalize180(
        direction.toYaw() - m_vRouteCommandDirection.toYaw()
    );
    const int elapsedMsec = Q_max(1, level.inttime - m_iRouteCommandTime);
    const float maxTurn = BOT_ROUTE_TURN_RATE_DEGREES
        * elapsedMsec / 1000.0f;
    if (fabs(yawDelta) > BOT_ROUTE_TURN_LIMIT_MIN_DEGREES
        && fabs(yawDelta) > maxTurn) {
        const float limitedYaw = m_vRouteCommandDirection.toYaw()
            + (yawDelta < 0.0f ? -maxTurn : maxTurn);
        const Vector limitedDirection(
            cos(DEG2RAD(limitedYaw)),
            sin(DEG2RAD(limitedYaw)),
            0.0f
        );
        usercmd_t limitedCommand;
        usercmd_t desiredCommand;
        SetCommandMoveVector(
            limitedCommand, limitedDirection * 127.0f
        );
        SetCommandMoveVector(desiredCommand, direction * 127.0f);
        const float limitedClearance = CalculateMoveProbeFraction(
            limitedCommand, BOT_ROUTE_TURN_PROBE_DISTANCE
        );
        const float desiredClearance = CalculateMoveProbeFraction(
            desiredCommand, BOT_ROUTE_TURN_PROBE_DISTANCE
        );
        if (limitedClearance + BOT_ROUTE_TURN_CLEARANCE_EPSILON
            < desiredClearance) {
            m_vRouteCommandDirection = direction;
            m_iRouteCommandTime      = level.inttime;
            return;
        }
        direction = limitedDirection;

        if (level.inttime >= m_iRouteTurnLogTime) {
            m_iRouteTurnLogTime = level.inttime + 1000;
            G_MoveLogBotEvent(
                "bot_route_turn_limited", controlledEntity, NULL,
                (int)fabs(yawDelta),
                controlledEntity->origin + direction * 64.0f
            );
        }
    }

    m_vRouteCommandDirection = direction;
    m_iRouteCommandTime      = level.inttime;
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
        const bool stalled = m_iLadderProgressTime
            && level.inttime - m_iLadderProgressTime
                >= BOT_LADDER_STALL_MSEC;

        if (g_navigation_legacy->integer || stalled) {
            botcmd.upmove = botcmd.upmove ? 0 : 127;
        } else if (!m_pPath || !m_pPath->GetNodeCount()) {
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
AllowRouteComfortInset

Return whether ordinary route travel owns the current movement command.
====================
*/
bool BotMovement::AllowRouteComfortInset() const
{
    if (!controlledEntity) {
        return false;
    }

    const bool doorCommitActive =
        m_vDoorPushApproachDirection.lengthXYSquared() > 0.01f
        && level.inttime <= m_iDoorPushLastContactTime
            + BOT_DOOR_EXIT_COMMIT_MSEC;
    return m_bPathing && !m_bDirectMove && !m_bHasCombatTarget
        && !controlledEntity->GetLadder() && !m_bJump
        && m_iTempAwayState != 2 && !m_bAvoidCollision
        && !doorCommitActive;
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
        m_pPath->SetRouteComfortInsetEnabled(AllowRouteComfortInset());
        m_pPath->UpdatePos(controlledEntity->origin);
        m_vCurrentDir = CalculateDir(m_pPath->GetCurrentDelta());
    } else {
        m_vCurrentDir = vec_zero;
    }
    m_vRouteCommandDirection = m_vCurrentDir;
    m_iRouteCommandTime      = level.inttime;
}

void BotMovement::ResetLocalLoopHistory()
{
    m_iLocalLoopSampleCount    = 0;
    m_fLocalLoopTravelTotal    = 0.0f;
    m_iLocalLoopNextSampleTime = level.inttime;
    m_vLocalLoopLastOrigin = controlledEntity
        ? controlledEntity->origin : vec_zero;
}

void BotMovement::UpdateLocalLoopDetection()
{
    if (!controlledEntity || !m_bPathing || m_bDirectMove || !m_pPath
        || (!m_pPath->GetNodeCount() && m_iTempAwayState == 0)
        || m_bHasCombatTarget
        || controlledEntity->GetLadder() || m_bJump) {
        ResetLocalLoopHistory();
        return;
    }

    const Vector origin = controlledEntity->origin;
    if (!m_iLocalLoopSampleCount) {
        m_vLocalLoopOrigins[0] = origin;
        m_vLocalLoopTargets[0] = m_vTargetPos;
        m_fLocalLoopTravel[0]  = 0.0f;
        m_iLocalLoopTimes[0]   = level.inttime;
        m_iLocalLoopSampleCount = 1;
        m_iLocalLoopNextSampleTime =
            level.inttime + BOT_LOCAL_LOOP_SAMPLE_MSEC;
        m_vLocalLoopLastOrigin = origin;
        return;
    }

    const Vector frameDelta = origin - m_vLocalLoopLastOrigin;
    const float frameTravel = frameDelta.length();
    m_vLocalLoopLastOrigin = origin;
    if (frameTravel > BOT_LOCAL_LOOP_TELEPORT_UNITS) {
        ResetLocalLoopHistory();
        return;
    }
    m_fLocalLoopTravelTotal += frameTravel;

    if (level.inttime < m_iLocalLoopNextSampleTime) {
        return;
    }

    if (level.inttime >= m_iLocalLoopCooldownUntil) {
        for (int i = 0; i < m_iLocalLoopSampleCount; ++i) {
            const int age = level.inttime - m_iLocalLoopTimes[i];
            if (age < BOT_LOCAL_LOOP_MIN_AGE_MSEC
                || age > BOT_LOCAL_LOOP_MAX_AGE_MSEC) {
                continue;
            }
            if ((origin - m_vLocalLoopOrigins[i]).lengthSquared()
                    > Square(BOT_LOCAL_LOOP_RETURN_UNITS)
                || (m_vTargetPos - m_vLocalLoopTargets[i]).lengthSquared()
                    > Square(BOT_LOCAL_LOOP_TARGET_UNITS)
                || m_fLocalLoopTravelTotal - m_fLocalLoopTravel[i]
                    < BOT_LOCAL_LOOP_MIN_TRAVEL_UNITS) {
                continue;
            }

            m_bLocalOscillation       = true;
            m_iLocalLoopCooldownUntil =
                level.inttime + BOT_LOCAL_LOOP_COOLDOWN_MSEC;
            ResetLocalLoopHistory();
            return;
        }
    }

    if (m_iLocalLoopSampleCount == MAX_LOCAL_LOOP_SAMPLES) {
        for (int i = 1; i < MAX_LOCAL_LOOP_SAMPLES; ++i) {
            m_vLocalLoopOrigins[i - 1] = m_vLocalLoopOrigins[i];
            m_vLocalLoopTargets[i - 1] = m_vLocalLoopTargets[i];
            m_fLocalLoopTravel[i - 1]  = m_fLocalLoopTravel[i];
            m_iLocalLoopTimes[i - 1]   = m_iLocalLoopTimes[i];
        }
        --m_iLocalLoopSampleCount;
    }

    const int sample = m_iLocalLoopSampleCount++;
    m_vLocalLoopOrigins[sample] = origin;
    m_vLocalLoopTargets[sample] = m_vTargetPos;
    m_fLocalLoopTravel[sample]  = m_fLocalLoopTravelTotal;
    m_iLocalLoopTimes[sample]   = level.inttime;
    m_iLocalLoopNextSampleTime =
        level.inttime + BOT_LOCAL_LOOP_SAMPLE_MSEC;
}

bool BotMovement::ConsumeLocalOscillation(void)
{
    const bool detected = m_bLocalOscillation;
    m_bLocalOscillation = false;
    return detected;
}

Vector BotMovement::ChooseBlockedRecoveryGoal(const Vector& pathDelta)
{
    Vector forward = pathDelta;
    forward.z = 0.0f;
    if (VectorNormalize2D(forward) <= 0.0f) {
        forward = Vector(controlledEntity->orientation[0]);
        forward.z = 0.0f;
        VectorNormalize2D(forward);
    }

    const Vector right(-forward.y, forward.x, 0.0f);
    int preferredDirection = 0;
    if (level.inttime < m_iCollisionAvoidDirectionUntil) {
        preferredDirection = m_iCollisionAvoidDirection;
    }
    if (!preferredDirection) {
        preferredDirection =
            ((controlledEntity->entnum + m_iNumBlocks) & 1) ? 1 : -1;
    }

    const int directions[2] = {
        preferredDirection, -preferredDirection
    };
    for (int i = 0; i < 2; ++i) {
        const int direction = directions[i];
        const Vector side = right
            * (BOT_BLOCKED_RECOVERY_SIDE_UNITS * direction);
        const Vector candidates[2] = {
            controlledEntity->origin + side
                + forward * BOT_BLOCKED_RECOVERY_FORWARD_UNITS,
            controlledEntity->origin + side
        };
        for (int j = 0; j < 2; ++j) {
            if (!CollisionAvoidanceTargetClear(candidates[j])) {
                continue;
            }

            m_iCollisionAvoidDirection = direction;
            m_iCollisionAvoidDirectionUntil =
                level.inttime + BOT_COLLISION_SIDE_COMMIT_MSEC;
            G_MoveLogBotEvent(
                "bot_blocked_lateral_recovery",
                controlledEntity,
                NULL,
                direction,
                candidates[j]
            );
            return candidates[j];
        }
    }

    m_iCollisionAvoidDirection      = 0;
    m_iCollisionAvoidDirectionUntil = 0;
    const Vector fallback = controlledEntity->origin
        - forward * BOT_BLOCKED_RECOVERY_BACK_UNITS;
    G_MoveLogBotEvent(
        "bot_blocked_reverse_recovery",
        controlledEntity,
        NULL,
        m_iNumBlocks,
        fallback
    );
    return fallback;
}

void BotMovement::PushThroughOpenableDoor(
    usercmd_t& botcmd, Door *door, const trace_t& trace
)
{
    if (!door || !controlledEntity) {
        return;
    }

    const bool continuingContact = door->entnum == m_iDoorPushEntity
        && level.inttime <= m_iDoorPushLastContactTime
            + BOT_DOOR_PUSH_RECONTACT_MSEC;
    RecordDoorPushThrough(door);
    if (!continuingContact) {
        m_iDoorPushStartTime = level.inttime;
        m_vDoorPushDirection = vec_zero;
        m_vDoorPushApproachDirection = vec_zero;
    }
    m_iDoorPushLastContactTime = level.inttime;

    // Retain the route's original through-door approach. The path can flip
    // sides while the moving panel intermittently stops tracing, so the
    // first stable command is a better exit direction than later samples.
    if (m_vDoorPushApproachDirection.lengthXYSquared() <= 0.01f) {
        m_vDoorPushApproachDirection = GetCommandMoveVector(botcmd);
        m_vDoorPushApproachDirection.z = 0.0f;
        if (VectorNormalize2D(m_vDoorPushApproachDirection) <= 0.0f) {
            m_vDoorPushApproachDirection = m_vCurrentDir;
            m_vDoorPushApproachDirection.z = 0.0f;
            VectorNormalize2D(m_vDoorPushApproachDirection);
        }
    }

    Vector openingEdge;
    const bool preferOpeningEdge =
        BotDoorOpeningEdgeDirection(door, openingEdge);

    // A human presses the free edge of a hinged panel as soon as it starts
    // moving, rather than drifting toward the hinge. Sliding and unusual
    // scripted doors retain the proven square-on grace period.
    if (!preferOpeningEdge
        && level.inttime < m_iDoorPushStartTime
            + BOT_DOOR_PUSH_STALL_MSEC) {
        return;
    }

    if (m_vDoorPushDirection.lengthXYSquared() <= 0.01f) {
        Vector normal = trace.plane.normal;
        normal.z      = 0.0f;
        if (VectorNormalize2D(normal) <= 0.0f) {
            return;
        }

        Vector tangent(-normal.y, normal.x, 0.0f);
        const Vector doorCenter = (door->absmin + door->absmax) * 0.5f;
        const Vector fromCenter = controlledEntity->origin - doorCenter;
        const Vector pathDelta  = m_vCurrentGoal - controlledEntity->origin;
        const float edgeDot     = DotProduct(openingEdge, tangent);
        const float awayDot     = DotProduct(fromCenter, tangent);
        const float pathDot     = DotProduct(pathDelta, tangent);
        int preferredDirection;
        if (preferOpeningEdge && fabs(edgeDot) > 0.1f) {
            preferredDirection = edgeDot > 0.0f ? 1 : -1;
        } else if (fabs(awayDot) > 4.0f) {
            preferredDirection = awayDot > 0.0f ? 1 : -1;
        } else if (fabs(pathDot) > 1.0f) {
            preferredDirection = pathDot > 0.0f ? 1 : -1;
        } else {
            preferredDirection = door->entnum & 1 ? 1 : -1;
        }

        usercmd_t preferredProbe = botcmd;
        usercmd_t otherProbe     = botcmd;
        SetCommandMoveVector(
            preferredProbe,
            tangent * (preferredDirection * BOT_DOOR_PUSH_SIDE_COMMAND)
        );
        SetCommandMoveVector(
            otherProbe,
            tangent * (-preferredDirection * BOT_DOOR_PUSH_SIDE_COMMAND)
        );
        const float preferredClearance = CalculateMoveProbeFraction(
            preferredProbe, BOT_DOOR_PUSH_PROBE_DISTANCE
        );
        const float otherClearance = CalculateMoveProbeFraction(
            otherProbe, BOT_DOOR_PUSH_PROBE_DISTANCE
        );
        const bool openingEdgeBlocked = preferOpeningEdge
            && preferredClearance < BOT_DOOR_OPEN_EDGE_BLOCKED_FRACTION
            && otherClearance > preferredClearance
                + BOT_DOOR_OPEN_EDGE_CLEARANCE_MARGIN;
        if (openingEdgeBlocked
            || (!preferOpeningEdge
                && otherClearance > preferredClearance
                    + BOT_DOOR_PUSH_PROBE_EPSILON)) {
            preferredDirection = -preferredDirection;
        }

        m_vDoorPushDirection = tangent * (float)preferredDirection;
        G_MoveLogBotEvent(
            preferOpeningEdge
                ? "bot_door_push_open_edge"
                : "bot_door_push_slide",
            controlledEntity,
            NULL,
            door->entnum,
            controlledEntity->origin
                + m_vDoorPushDirection * BOT_DOOR_PUSH_PROBE_DISTANCE
        );
    }

    ApplyDoorPushThrough(botcmd);
}

void BotMovement::ApplyDoorPushThrough(usercmd_t& botcmd) const
{
    if (!controlledEntity
        || controlledEntity->GetLadder() || m_bJump
        || m_iTempAwayState == 2
        || m_vDoorPushDirection.lengthXYSquared() <= 0.01f) {
        return;
    }

    // Build contact movement from the stable through-door approach, then add
    // the chosen edge tangent. This prevents a route update during panel
    // movement from turning the bot back across the same doorway.
    Vector move = m_vDoorPushApproachDirection.lengthXYSquared() > 0.01f
        ? m_vDoorPushApproachDirection * BOT_DOOR_PUSH_SIDE_COMMAND
        : GetCommandMoveVector(botcmd);
    const float sideMove = DotProduct(move, m_vDoorPushDirection);
    if (sideMove < BOT_DOOR_PUSH_SIDE_COMMAND) {
        move += m_vDoorPushDirection
            * (BOT_DOOR_PUSH_SIDE_COMMAND - sideMove);
    }

    // Preserve the selected world-space edge direction when converting the
    // blended vector back to signed command axes. Independent axis clipping
    // can rotate an over-range vector toward the opposite door-frame corner.
    Vector angles = controlledEntity->angles;
    Vector forward, left, up;
    angles.x = 0.0f;
    angles.z = 0.0f;
    angles.AngleVectorsLeft(&forward, &left, &up);
    const float commandMax = Q_max(
        fabs(DotProduct(move, forward)),
        fabs(DotProduct(move, left))
    );
    if (commandMax > BOT_DOOR_PUSH_SIDE_COMMAND) {
        move *= BOT_DOOR_PUSH_SIDE_COMMAND / commandMax;
    }
    SetCommandMoveVector(botcmd, move);
}

void BotMovement::ContinueDoorExit(usercmd_t& botcmd) const
{
    if (!controlledEntity
        || controlledEntity->GetLadder() || m_bJump
        || m_iTempAwayState == 2
        || m_vDoorPushDirection.lengthXYSquared() <= 0.01f
        || m_vDoorPushApproachDirection.lengthXYSquared() <= 0.01f
        || level.inttime > m_iDoorPushLastContactTime
            + BOT_DOOR_EXIT_COMMIT_MSEC) {
        return;
    }

    // Once contact with the panel clears, keep pushing through the doorway
    // instead of carrying the lateral edge tangent along the frame.
    SetCommandMoveVector(
        botcmd,
        m_vDoorPushApproachDirection * BOT_DOOR_PUSH_SIDE_COMMAND
    );
}

void BotMovement::RecordDoorPushThrough(Door *door)
{
    if (!door) {
        return;
    }
    if (door->entnum == m_iDoorPushEntity
        && level.inttime < m_iDoorPushLogTime) {
        return;
    }

    m_iDoorPushEntity  = door->entnum;
    m_iDoorPushLogTime = level.inttime + BOT_DOOR_PUSH_LOG_MSEC;

    G_MoveLogBotEvent(
        "bot_door_pushthrough",
        controlledEntity,
        NULL,
        door->entnum,
        door->origin
    );
}

void BotMovement::RecordOpenDoorPanelContact(
    Door *door, const trace_t& trace
)
{
    if (!door || !door->isOpen()) {
        return;
    }
    if (door->entnum == m_iOpenDoorPanelEntity
        && level.inttime < m_iOpenDoorPanelLogTime) {
        return;
    }

    m_iOpenDoorPanelEntity  = door->entnum;
    m_iOpenDoorPanelLogTime = level.inttime + BOT_DOOR_PUSH_LOG_MSEC;
    G_MoveLogBotEvent(
        "bot_open_door_panel_contact", controlledEntity, NULL,
        door->entnum, trace.endpos
    );
}

bool BotMovement::EscapeOpenDoorPanel(
    usercmd_t& botcmd, Door *door, const trace_t& trace
)
{
    if (!door || !door->isOpen() || !controlledEntity) {
        return false;
    }

    const bool continuingContact = door->entnum == m_iDoorPushEntity
        && level.inttime <= m_iDoorPushLastContactTime
            + BOT_DOOR_PUSH_RECONTACT_MSEC;
    if (!continuingContact) {
        m_iDoorPushStartTime = level.inttime;
        m_vDoorPushDirection = vec_zero;
        m_vDoorPushApproachDirection = GetCommandMoveVector(botcmd);
        m_vDoorPushApproachDirection.z = 0.0f;
        if (VectorNormalize2D(m_vDoorPushApproachDirection) <= 0.0f) {
            m_vDoorPushApproachDirection = m_vCurrentDir;
            m_vDoorPushApproachDirection.z = 0.0f;
            VectorNormalize2D(m_vDoorPushApproachDirection);
        }
    }
    m_iDoorPushEntity          = door->entnum;
    m_iDoorPushLastContactTime = level.inttime;

    if (m_vDoorPushDirection.lengthXYSquared() <= 0.01f) {
        Vector normal = trace.plane.normal;
        normal.z      = 0.0f;
        if (VectorNormalize2D(normal) <= 0.0f) {
            return false;
        }

        const Vector tangent(-normal.y, normal.x, 0.0f);
        const Vector doorCenter = (door->absmin + door->absmax) * 0.5f;
        const Vector fromCenter = controlledEntity->origin - doorCenter;
        const Vector pathDelta  = m_vCurrentGoal - controlledEntity->origin;
        const float awayDot     = DotProduct(fromCenter, tangent);
        const float pathDot     = DotProduct(pathDelta, tangent);
        int preferredDirection;
        if (fabs(awayDot) > 4.0f) {
            // Slide toward the nearest end of the displaced panel.
            preferredDirection = awayDot > 0.0f ? 1 : -1;
        } else if (fabs(pathDot) > 1.0f) {
            preferredDirection = pathDot > 0.0f ? 1 : -1;
        } else {
            preferredDirection = door->entnum & 1 ? 1 : -1;
        }

        usercmd_t preferredProbe = botcmd;
        usercmd_t otherProbe     = botcmd;
        SetCommandMoveVector(
            preferredProbe,
            tangent * (preferredDirection * BOT_DOOR_PUSH_SIDE_COMMAND)
        );
        SetCommandMoveVector(
            otherProbe,
            tangent * (-preferredDirection * BOT_DOOR_PUSH_SIDE_COMMAND)
        );
        const float preferredClearance = CalculateMoveProbeFraction(
            preferredProbe, BOT_DOOR_PUSH_PROBE_DISTANCE
        );
        const float otherClearance = CalculateMoveProbeFraction(
            otherProbe, BOT_DOOR_PUSH_PROBE_DISTANCE
        );
        if (otherClearance > preferredClearance
            + BOT_DOOR_PUSH_PROBE_EPSILON) {
            preferredDirection = -preferredDirection;
        }

        m_vDoorPushDirection = tangent * (float)preferredDirection;
        G_MoveLogBotEvent(
            "bot_open_door_panel_escape", controlledEntity, NULL,
            door->entnum,
            controlledEntity->origin
                + m_vDoorPushDirection * BOT_DOOR_PUSH_PROBE_DISTANCE
        );
    }

    // A fully open panel cannot yield to forward pressure. Give its chosen
    // tangent full command ownership until collision clears, then the existing
    // short door-exit commitment resumes the retained through-door approach.
    SetCommandMoveVector(
        botcmd, m_vDoorPushDirection * BOT_DOOR_PUSH_SIDE_COMMAND
    );
    botcmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);
    m_bIsLeaning                   = false;
    m_bLeanCommandActive           = false;
    m_telemetry.movementSuppressed = true;
    m_telemetry.guardTriggered        = true;
    m_telemetry.guardHitSentient      = false;
    m_telemetry.guardHitWorld         = false;
    m_telemetry.guardRemovedComponent = true;
    m_telemetry.guardFraction         = trace.fraction;
    m_telemetry.guardEntity           = trace.entityNum;
    return true;
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

    maxDist = Q_min(dist, BOT_COLLISION_PATH_PROBE_DISTANCE);

    stepOrg       = controlledEntity->origin + Vector(0, 0, STEPSIZE);
    target        = controlledEntity->origin + forward * maxDist;
    targetStepOrg = target + Vector(0, 0, STEPSIZE);

    trace = G_Trace(stepOrg, mins, maxs, targetStepOrg, controlledEntity, MASK_PLAYERSOLID, qtrue, "GetCurrentDelta");
    Door *openableDoor = BotTraceOpenableDoor(trace, controlledEntity);
    if (openableDoor && !openableDoor->isOpen()) {
        // Keep pushing into a closed or moving unlocked door so CheckUse can
        // open it. A fully open panel falls through to local obstacle
        // avoidance instead of being ground against every frame.
        m_iCollisionAvoidDirection      = 0;
        m_iCollisionAvoidDirectionUntil = 0;
        return delta;
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

        openableDoor = BotTraceOpenableDoor(trace, controlledEntity);
        if (openableDoor && !openableDoor->isOpen()) {
            m_iCollisionAvoidDirection      = 0;
            m_iCollisionAvoidDirectionUntil = 0;
            return delta;
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
    if (!controlledEntity) {
        return false;
    }
    if (!m_pPath) {
        m_pPath = IPather::CreatePather();
    }

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

bool BotMovement::IsBlockedRecoveryActive(void) const
{
    return m_iTempAwayState != 0;
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
    m_bIsLeaning        = false;
    m_bLeanCommandActive = false;
    m_iLeanDirection    = 0;
    m_iLeanLastAppliedTime = 0;
    m_bDirectMove       = false;
    if (!controlledEntity || controlledEntity->IsDead()
        || (!controlledEntity->GetLadder() && !m_iLadderExitUntil)) {
        m_bWasOnLadder         = false;
        m_fLadderTop           = 0.0f;
        m_fLadderProgressHeight = 0.0f;
        m_iLadderProgressTime   = 0;
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
    m_vRouteCommandDirection  = vec_zero;
    m_iRouteCommandTime       = 0;
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

/*
====================
GetRouteLookAheadTarget

Return an eye-height point ahead on an ordinary path for noncombat view
placement.  Movement continues to use the current steering direction; this
preview must never become another locomotion owner.
====================
*/
bool BotMovement::GetRouteLookAheadTarget(float distance, Vector& target) const
{
    if (!controlledEntity || distance <= 0.0f
        || !AllowRouteComfortInset() || m_iTempAwayState != 0
        || m_iLadderExitUntil || !m_pPath || !m_pPath->GetNodeCount()) {
        return false;
    }

    target = m_pPath->GetLookAheadPoint(controlledEntity->origin, distance);
    Vector delta = target - controlledEntity->origin;
    delta.z      = 0.0f;
    if (delta.lengthXYSquared() < Square(16.0f)) {
        return false;
    }

    // Path points sit on the floor.  Raise the preview to the bot's current
    // eye height so level travel keeps a level crosshair while stairs and
    // ramps naturally contribute measured vertical placement.
    target.z += controlledEntity->viewheight;
    return true;
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

float BotMovement::CalculateMoveProbeFraction(
    const usercmd_t& botcmd,
    float distance
) const
{
    if (!controlledEntity || distance <= 0.0f) {
        return 0.0f;
    }

    Vector probeDirection = GetCommandMoveVector(botcmd);
    if (VectorNormalize2D(probeDirection) <= 0.0f) {
        return 0.0f;
    }

    Vector mins = controlledEntity->mins;
    Vector maxs = controlledEntity->maxs;
    maxs.z -= STEPSIZE;
    const Vector start =
        controlledEntity->origin + Vector(0, 0, STEPSIZE);
    const Vector end = start + probeDirection * distance;
    const trace_t trace = G_Trace(
        start,
        mins,
        maxs,
        end,
        controlledEntity,
        MASK_PLAYERSOLID | CONTENTS_BOTCLIP,
        false,
        "BotMovement::CalculateMoveProbeFraction"
    );

    return trace.startsolid ? 0.0f : trace.fraction;
}

float BotMovement::CalculateStrafeProbeFraction(
    const usercmd_t& botcmd,
    int direction,
    float distance
) const
{
    if (direction == 0) {
        return 0.0f;
    }

    usercmd_t probeCommand = botcmd;
    const int offset =
        (int)(direction * g_bot_strafe_intensity->value * 127.0f);
    int probeRight = (int)probeCommand.rightmove + offset;
    probeCommand.rightmove =
        (signed char)Q_clamp(probeRight, -127, 127);
    return CalculateMoveProbeFraction(probeCommand, distance);
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
    usercmd_t baseCommand = botcmd;
    UpdateAggressiveMovement(botcmd);
    ContinueDoorExit(baseCommand);
    ContinueDoorExit(botcmd);
    ResolveImminentCollision(botcmd, baseCommand);
}

/*
====================
UpdateAggressiveMovement

Movement layer calibrated from human-versus-bot telemetry: active and neutral
roaming strafe phases with matching lean, plus continuous combat strafe and
slower movement toward and away from nearby enemies. Lean follows actual lateral
movement instead of running as an independent animation.
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

    const bool nonCombatTravel = !m_bHasCombatTarget;

    // Human roaming contained substantial neutral lateral time. During
    // non-combat travel, alternate full-strength strafe phases with neutral
    // phases. Combat retains the continuous strafe cadence.
    if (level.inttime >= m_iNextStrafeChangeTime
        && level.inttime >= m_iStrafeGeometryLockTime) {
        if (nonCombatTravel) {
            m_bRoamStrafeActive = !m_bRoamStrafeActive;
            if (m_bRoamStrafeActive) {
                m_iStrafeDirection = -m_iStrafeDirection;
            }
        } else {
            m_iStrafeDirection = -m_iStrafeDirection;
        }
        m_iNextStrafeChangeTime = level.inttime
            + RandomInterval(
                g_bot_strafe_min_interval,
                g_bot_strafe_max_interval
            );
    }

    m_bIsLeaning = false;
    const bool baseTravelCommand = botcmd.forwardmove < -8
        || botcmd.forwardmove > 8 || botcmd.rightmove < -8
        || botcmd.rightmove > 8;
    const bool strafePhaseActive = !nonCombatTravel
        || (m_bRoamStrafeActive && baseTravelCommand);
    if (!suppressMovement && strafePhaseActive) {
        const float probeDistance = nonCombatTravel
            ? BOT_ROAM_STRAFE_VETO_DISTANCE
            : BOT_STRAFE_PROBE_DISTANCE;
        const float probeFraction = CalculateStrafeProbeFraction(
            botcmd,
            m_iStrafeDirection,
            probeDistance
        );

        // Optional roaming style must never make navigation clearance worse.
        // Cancel it for this frame instead of reversing it or adding a second
        // steering owner. The active phase can resume as soon as it is safe.
        bool vetoRoamStrafe = false;
        if (nonCombatTravel) {
            const float baseProbeFraction =
                CalculateMoveProbeFraction(botcmd, probeDistance);
            vetoRoamStrafe = probeFraction
                + BOT_ROAM_STRAFE_CLEARANCE_EPSILON
                < baseProbeFraction;
            m_telemetry.strafeOtherClearance =
                baseProbeFraction * probeDistance;
        }

        m_telemetry.strafeProbeFraction = probeFraction;
        m_telemetry.strafeClearance = probeFraction * probeDistance;

        if (!vetoRoamStrafe) {
            const float intensity =
                g_bot_strafe_intensity->value * probeFraction;
            const int offset =
                (int)(m_iStrafeDirection * intensity * 127.0f);
            m_telemetry.strafeIntensity = intensity;

            if (offset) {
                int newRight = (int)botcmd.rightmove + offset;
                botcmd.rightmove =
                    (signed char)Q_clamp(newRight, -127, 127);
                m_telemetry.strafeApplied = true;
                m_bIsLeaning = true;
            }
        }
    }

    if (m_bHasCombatTarget) {
        UpdateCombatRadialMovement(botcmd, suppressMovement);
    }

    // Lean follows the final lateral command, after the combat radial layer.
    // Preserve very short neutral gaps, but require a neutral beat before an
    // opposite lean. This removes one-frame on/off and left/right animation
    // flicker without shortening the underlying strafe movement.
    const int requestedLeanDirection =
        m_bIsLeaning ? m_iStrafeDirection : 0;
    bool leanReleaseGrace = false;
    if (requestedLeanDirection) {
        const bool switchingTooSoon = m_iLeanDirection
            && requestedLeanDirection != m_iLeanDirection
            && level.inttime < m_iLeanLastAppliedTime
                + BOT_LEAN_SWITCH_NEUTRAL_MSEC;
        if (switchingTooSoon) {
            m_bIsLeaning = false;
        } else {
            m_iLeanDirection       = requestedLeanDirection;
            m_iLeanLastAppliedTime = level.inttime;
        }
    } else if (!suppressMovement && m_iLeanDirection
        && level.inttime <= m_iLeanLastAppliedTime
            + BOT_LEAN_RELEASE_GRACE_MSEC) {
        m_bIsLeaning = true;
        leanReleaseGrace = true;
    }

    // The collision resolver performs the same direction check if it removes
    // a movement component.
    if (m_bIsLeaning) {
        const bool matchesDirection = leanReleaseGrace
            || (m_iLeanDirection < 0
            ? botcmd.rightmove < -8
            : botcmd.rightmove > 8);
        if (matchesDirection) {
            if (m_iLeanDirection < 0) {
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

    if (m_pPath->UsesLegacyCollisionAvoidance()) {
        delta = FixDeltaFromCollision(delta);
    }
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
    // Blocked recovery has already selected an escape direction. Let normal
    // player collision constrain it instead of having this predictive guard
    // erase the command that is meant to break the stall.
    if (!controlledEntity || controlledEntity->GetLadder() || m_bJump
        || m_iTempAwayState == 2) {
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

    Door *tracedDoor = BotTraceDoor(trace);
    if (tracedDoor && tracedDoor->isOpen()) {
        RecordOpenDoorPanelContact(tracedDoor, trace);
        if (EscapeOpenDoorPanel(botcmd, tracedDoor, trace)) {
            return;
        }
    }
    Door *openableDoor = BotTraceOpenableDoor(trace, controlledEntity);
    if (openableDoor) {
        // Preserve forward pressure and commit toward one panel edge.
        PushThroughOpenableDoor(botcmd, openableDoor, trace);
        return;
    }

    // A committed door approach can meet the adjacent frame or world corner.
    // Drop both vectors immediately so the next contact captures the route
    // again and probes a fresh panel edge instead of repeating that collision.
    if (trace.entityNum == ENTITYNUM_WORLD
        && m_vDoorPushDirection.lengthXYSquared() > 0.01f
        && level.inttime <= m_iDoorPushLastContactTime
            + BOT_DOOR_EXIT_COMMIT_MSEC) {
        m_vDoorPushDirection = vec_zero;
        m_vDoorPushApproachDirection = vec_zero;
        if (level.inttime >= m_iDoorPushBlockedLogTime) {
            m_iDoorPushBlockedLogTime =
                level.inttime + BOT_DOOR_PUSH_LOG_MSEC;
            G_MoveLogBotEvent(
                "bot_door_push_blocked", controlledEntity, NULL,
                m_iDoorPushEntity, trace.endpos
            );
        }
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

    // The final guard can reduce a valid route command to nearly zero. Start
    // the existing recovery grace period now instead of waiting for the
    // periodic blocked check to discover it. A transient contact is still
    // discarded by that normal movement check.
    if (m_bPathing && !m_bHasCombatTarget
        && resolvedCommand <= BOT_GUARD_RECOVERY_COMMAND_MAX
        && m_iTempAwayState == 0) {
        m_iTempAwayState = 1;
        m_iLastBlockTime = level.inttime;
        m_iCheckPathTime = level.inttime;
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
