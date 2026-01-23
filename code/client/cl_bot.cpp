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
// cl_bot.cpp: Client-side bot system - allows client to play autonomously

#include "client.h"
#include "cl_bot.h"
#include "../fgame/bg_public.h"

// Bot instance
clientBot_t clBot;

// Store the last eye info for CL_EyeInfo to use
static usereyes_t clBotEyeInfo;
static qboolean clBotEyeInfoValid = qfalse;

// CVars
cvar_t *cl_bot           = NULL;
cvar_t *cl_bot_movespeed = NULL;
cvar_t *cl_bot_aimspeed  = NULL;
cvar_t *cl_bot_attackdist = NULL;
cvar_t *cl_bot_firedelay = NULL;
cvar_t *cl_bot_roamtime  = NULL;
cvar_t *cl_bot_debug     = NULL;

// Constants
#define CLBOT_ROAM_CHANGE_TIME      3000    // Change roam direction every 3 seconds
#define CLBOT_ATTACK_TIMEOUT        5000    // Stop attacking if no enemy seen for 5 seconds
#define CLBOT_RESPAWN_DELAY         500     // Wait between respawn attempts
#define CLBOT_TEAM_JOIN_DELAY       2000    // Wait 2 seconds before joining team
#define CLBOT_JUMP_COOLDOWN         1000    // Minimum time between jumps
#define CLBOT_MIN_FIRE_DELAY        100     // Minimum time between shots
#define CLBOT_SPAWN_GRACE_TIME      500     // Grace period after spawning

//
// Forward declarations
//
static void     CL_Bot_Think(void);
static void     CL_Bot_ThinkIdle(void);
static void     CL_Bot_ThinkRoaming(void);
static void     CL_Bot_ThinkAttacking(void);
static void     CL_Bot_ThinkDead(void);
static void     CL_Bot_UpdateMovement(usercmd_t *cmd);
static void     CL_Bot_UpdateAiming(usercmd_t *cmd);
static void     CL_Bot_UpdateButtons(usercmd_t *cmd);
static void     CL_Bot_CheckForEnemies(void);
static void     CL_Bot_HandleTeamJoin(void);
static void     CL_Bot_HandleRespawn(usercmd_t *cmd);
static float    CL_Bot_AngleDiff(float ang1, float ang2);
static void     CL_Bot_PickNewRoamTarget(void);
static entityState_t* CL_Bot_FindEntityByNumber(int entityNum);
static int      CL_Bot_CheckObstacles(void);

/*
====================
CL_Bot_Init

Initialize the client bot system
====================
*/
void CL_Bot_Init(void)
{
    Com_Printf("Initializing client bot system...\n");

    // Register cvars
    cl_bot           = Cvar_Get("cl_bot", "0", CVAR_ARCHIVE);
    cl_bot_movespeed = Cvar_Get("cl_bot_movespeed", "127", CVAR_ARCHIVE);
    cl_bot_aimspeed  = Cvar_Get("cl_bot_aimspeed", "360", CVAR_ARCHIVE);
    cl_bot_attackdist = Cvar_Get("cl_bot_attackdist", "2048", CVAR_ARCHIVE);
    cl_bot_firedelay = Cvar_Get("cl_bot_firedelay", "150", CVAR_ARCHIVE);
    cl_bot_roamtime  = Cvar_Get("cl_bot_roamtime", "3000", CVAR_ARCHIVE);
    cl_bot_debug     = Cvar_Get("cl_bot_debug", "0", 0);

    // Register commands
    Cmd_AddCommand("bot_enable", CL_Bot_Enable_f);
    Cmd_AddCommand("bot_disable", CL_Bot_Disable_f);

    // Initialize bot state
    CL_Bot_Reset();

    Com_Printf("Client bot system initialized.\n");
}

/*
====================
CL_Bot_Shutdown

Shutdown the client bot system
====================
*/
void CL_Bot_Shutdown(void)
{
    Cmd_RemoveCommand("bot_enable");
    Cmd_RemoveCommand("bot_disable");

    CL_Bot_Reset();
}

/*
====================
CL_Bot_Reset

Reset bot state
====================
*/
void CL_Bot_Reset(void)
{
    memset(&clBot, 0, sizeof(clBot));
    clBot.enemyEntityNum = -1;
    clBot.state = CLBOT_STATE_IDLE;
    clBotEyeInfoValid = qfalse;
}

/*
====================
CL_Bot_Enable_f

Console command to enable bot mode
====================
*/
void CL_Bot_Enable_f(void)
{
    Cvar_Set("cl_bot", "1");
    CL_Bot_Reset();
    clBot.active = qtrue;
    clBot.initialized = qtrue;
    Com_Printf("Client bot mode enabled.\n");
}

/*
====================
CL_Bot_Disable_f

Console command to disable bot mode
====================
*/
void CL_Bot_Disable_f(void)
{
    Cvar_Set("cl_bot", "0");
    clBot.active = qfalse;
    clBotEyeInfoValid = qfalse;
    Com_Printf("Client bot mode disabled.\n");
}

/*
====================
CL_Bot_SetState

Change bot state
====================
*/
void CL_Bot_SetState(clBotState_t newState)
{
    if (clBot.state != newState) {
        if (cl_bot_debug && cl_bot_debug->integer) {
            const char *stateNames[] = {"IDLE", "ROAMING", "ATTACKING", "DEAD"};
            Com_Printf("Bot state: %s -> %s\n", stateNames[clBot.state], stateNames[newState]);
        }

        clBot.state = newState;
        clBot.stateTime = cls.realtime;

        // State entry logic
        switch (newState) {
        case CLBOT_STATE_ROAMING:
            CL_Bot_PickNewRoamTarget();
            break;
        case CLBOT_STATE_ATTACKING:
            clBot.attackStartTime = cls.realtime;
            break;
        case CLBOT_STATE_DEAD:
            clBot.enemyEntityNum = -1;
            break;
        default:
            break;
        }
    }
}

/*
====================
CL_Bot_IsActive

Check if bot mode is currently active
====================
*/
qboolean CL_Bot_IsActive(void)
{
    return cl_bot && cl_bot->integer && clc.state == CA_ACTIVE && cl.snap.valid;
}

/*
====================
CL_Bot_GetEyeInfo

Get the bot's eye info for CL_EyeInfo
====================
*/
qboolean CL_Bot_GetEyeInfo(usereyes_t *eyeinfo)
{
    if (!CL_Bot_IsActive() || !clBotEyeInfoValid) {
        return qfalse;
    }

    *eyeinfo = clBotEyeInfo;
    return qtrue;
}

/*
====================
CL_Bot_Frame

Called every frame when bot mode is active
====================
*/
void CL_Bot_Frame(void)
{
    if (!CL_Bot_IsActive()) {
        clBot.active = qfalse;
        return;
    }

    clBot.active = qtrue;

    // Initialize angles from player state if not done
    if (!clBot.initialized) {
        VectorCopy(cl.snap.ps.viewangles, clBot.currentAngles);
        VectorCopy(cl.snap.ps.viewangles, clBot.targetAngles);
        clBot.initialized = qtrue;
    }

    CL_Bot_Think();
}

/*
====================
CL_Bot_CheckStuck

Check if bot is stuck and try to get unstuck
====================
*/
#define STUCK_CHECK_INTERVAL 500  // Check every 500ms
#define STUCK_DISTANCE_THRESHOLD 10.0f  // Must move at least 10 units

static void CL_Bot_CheckStuck(void)
{
    // Only check periodically
    if (cls.realtime - clBot.lastStuckCheckTime < STUCK_CHECK_INTERVAL) {
        return;
    }

    // Don't check if dead or just spawned
    if (cl.snap.ps.pm_type == PM_DEAD || cls.realtime - clBot.spawnedTime < 1000) {
        VectorCopy(cl.snap.ps.origin, clBot.lastPosition);
        clBot.lastStuckCheckTime = cls.realtime;
        return;
    }

    // Calculate how far we've moved
    vec3_t delta;
    VectorSubtract(cl.snap.ps.origin, clBot.lastPosition, delta);
    float distMoved = VectorLength(delta);

    // If we haven't moved much and we're trying to move, we're stuck
    if (distMoved < STUCK_DISTANCE_THRESHOLD && clBot.isMoving) {
        clBot.stuckCount++;

        if (cl_bot_debug && cl_bot_debug->integer) {
            Com_Printf("Bot stuck! (count=%d, moved=%.1f)\n", clBot.stuckCount, distMoved);
        }

        // Try to get unstuck
        if (clBot.stuckCount >= 2) {
            // Jump to try to get over obstacles
            clBot.shouldJump = qtrue;
        }
        if (clBot.stuckCount >= 3) {
            // Turn around and pick new direction
            clBot.targetAngles[YAW] = AngleMod(clBot.currentAngles[YAW] + 90 + (rand() % 180));
            CL_Bot_PickNewRoamTarget();
            clBot.stuckCount = 0;
        }
    } else {
        // We moved, reset stuck counter
        if (clBot.stuckCount > 0) {
            clBot.stuckCount = 0;
        }
    }

    // Update position for next check
    VectorCopy(cl.snap.ps.origin, clBot.lastPosition);
    clBot.lastStuckCheckTime = cls.realtime;
}

/*
====================
CL_Bot_Think

Main bot thinking logic
====================
*/
static void CL_Bot_Think(void)
{
    // Handle team joining if needed
    CL_Bot_HandleTeamJoin();

    // Check for enemies
    CL_Bot_CheckForEnemies();

    // Check if stuck
    CL_Bot_CheckStuck();

    // Update state based on conditions
    if (cl.snap.ps.pm_type == PM_DEAD) {
        CL_Bot_SetState(CLBOT_STATE_DEAD);
    } else if (clBot.enemyEntityNum >= 0) {
        CL_Bot_SetState(CLBOT_STATE_ATTACKING);
    } else if (clBot.state == CLBOT_STATE_DEAD || clBot.state == CLBOT_STATE_IDLE) {
        CL_Bot_SetState(CLBOT_STATE_ROAMING);
    }

    // State-specific thinking
    switch (clBot.state) {
    case CLBOT_STATE_IDLE:
        CL_Bot_ThinkIdle();
        break;
    case CLBOT_STATE_ROAMING:
        CL_Bot_ThinkRoaming();
        break;
    case CLBOT_STATE_ATTACKING:
        CL_Bot_ThinkAttacking();
        break;
    case CLBOT_STATE_DEAD:
        CL_Bot_ThinkDead();
        break;
    }
}

/*
====================
CL_Bot_ThinkIdle

Idle state - waiting to do something
====================
*/
static void CL_Bot_ThinkIdle(void)
{
    // Just transition to roaming after a short delay
    if (cls.realtime - clBot.stateTime > 500) {
        CL_Bot_SetState(CLBOT_STATE_ROAMING);
    }
}

/*
====================
CL_Bot_ThinkRoaming

Roaming state - wandering around looking for enemies
====================
*/
static void CL_Bot_ThinkRoaming(void)
{
    int roamTime = cl_bot_roamtime ? cl_bot_roamtime->integer : CLBOT_ROAM_CHANGE_TIME;

    // Change direction periodically
    if (cls.realtime - clBot.lastMoveChangeTime > roamTime) {
        CL_Bot_PickNewRoamTarget();
    }

    // Check for obstacles and adjust direction
    int obstacleDir = CL_Bot_CheckObstacles();
    if (obstacleDir != 0) {
        // Turn to avoid obstacle (45 degrees per detection)
        clBot.targetAngles[YAW] = AngleMod(clBot.currentAngles[YAW] + (obstacleDir * 45.0f));
        clBot.lastMoveChangeTime = cls.realtime; // Reset roam timer
    }

    // Keep moving in the current direction
    clBot.isMoving = qtrue;

    // Always keep pitch level when roaming (look straight ahead)
    clBot.targetAngles[PITCH] = 0;

    // In roaming, we move forward relative to where we're looking
    // The target yaw was set by PickNewRoamTarget, and we're turning towards it
    // Update moveDir to match our current view so movement is always forward
    vec3_t angles;
    angles[PITCH] = 0;
    angles[YAW] = clBot.currentAngles[YAW];
    angles[ROLL] = 0;

    vec3_t forward;
    AngleVectors(angles, forward, NULL, NULL);
    VectorCopy(forward, clBot.moveDir);
}

/*
====================
CL_Bot_FindEntityByNumber

Find an entity in the snapshot by entity number
====================
*/
static entityState_t* CL_Bot_FindEntityByNumber(int entityNum)
{
    int i;

    for (i = 0; i < cl.snap.numEntities; i++) {
        int entityIndex = (cl.snap.parseEntitiesNum + i) & (MAX_PARSE_ENTITIES - 1);
        entityState_t *ent = &cl.parseEntities[entityIndex];

        if (ent->number == entityNum) {
            return ent;
        }
    }

    return NULL;
}

/*
====================
CL_Bot_ThinkAttacking

Attacking state - engaging an enemy
====================
*/
static void CL_Bot_ThinkAttacking(void)
{
    entityState_t *enemy;
    vec3_t delta;
    float dist;

    // Check if enemy is still valid
    if (clBot.enemyEntityNum < 0) {
        CL_Bot_SetState(CLBOT_STATE_ROAMING);
        return;
    }

    // Check if we've lost sight of enemy for too long
    if (cls.realtime - clBot.lastEnemySeenTime > CLBOT_ATTACK_TIMEOUT) {
        clBot.enemyEntityNum = -1;
        CL_Bot_SetState(CLBOT_STATE_ROAMING);
        return;
    }

    // Find the enemy entity in the snapshot
    enemy = CL_Bot_FindEntityByNumber(clBot.enemyEntityNum);

    if (!enemy) {
        // Enemy not in snapshot anymore, use last known position for a bit
        if (cls.realtime - clBot.lastEnemySeenTime > 2000) {
            clBot.enemyEntityNum = -1;
            CL_Bot_SetState(CLBOT_STATE_ROAMING);
        }
        return;
    }

    // Check if enemy is dead
    if (enemy->eFlags & EF_DEAD) {
        clBot.enemyEntityNum = -1;
        CL_Bot_SetState(CLBOT_STATE_ROAMING);
        return;
    }

    // Update last seen time
    clBot.lastEnemySeenTime = cls.realtime;

    // Calculate distance to enemy
    VectorSubtract(enemy->origin, cl.snap.ps.origin, delta);
    dist = VectorLength(delta);

    // Update aim target
    VectorCopy(enemy->origin, clBot.enemyLastPos);

    // Calculate aim angles - aim at center mass
    vec3_t aimDir;
    vec3_t aimTarget;
    VectorCopy(enemy->origin, aimTarget);
    aimTarget[2] += 40; // Aim at upper body

    VectorSubtract(aimTarget, cl.snap.ps.origin, aimDir);
    aimDir[2] -= cl.snap.ps.viewheight; // Account for our eye height
    VectorNormalize(aimDir);
    vectoangles(aimDir, clBot.targetAngles);

    if (cl_bot_debug && cl_bot_debug->integer > 1) {
        Com_Printf("Enemy at %.0f,%.0f,%.0f dist=%.0f aim=%.1f,%.1f\n",
            enemy->origin[0], enemy->origin[1], enemy->origin[2],
            dist, clBot.targetAngles[PITCH], clBot.targetAngles[YAW]);
    }

    // Move towards enemy while attacking, with some strafing mixed in
    float attackDist = cl_bot_attackdist ? cl_bot_attackdist->value : 2048;

    if (dist < 150) {
        // Too close, back up
        VectorCopy(delta, clBot.moveDir);
        VectorNormalize(clBot.moveDir);
        VectorNegate(clBot.moveDir, clBot.moveDir);
        clBot.isMoving = qtrue;
    } else if (dist > 400) {
        // Far away - move towards enemy with slight strafe
        vec3_t forward, right;
        VectorCopy(delta, forward);
        forward[2] = 0;
        VectorNormalize(forward);

        // Add slight strafe to make movement less predictable
        AngleVectors(clBot.currentAngles, NULL, right, NULL);
        float strafeAmount = ((cls.realtime / 2000) % 2 == 0) ? 0.2f : -0.2f;

        clBot.moveDir[0] = forward[0] + right[0] * strafeAmount;
        clBot.moveDir[1] = forward[1] + right[1] * strafeAmount;
        clBot.moveDir[2] = 0;
        VectorNormalize(clBot.moveDir);
        clBot.isMoving = qtrue;
    } else {
        // Medium range - strafe while slowly advancing
        vec3_t forward, right;
        VectorCopy(delta, forward);
        forward[2] = 0;
        VectorNormalize(forward);

        AngleVectors(clBot.currentAngles, NULL, right, NULL);
        float strafeDir = ((cls.realtime / 1500) % 2 == 0) ? 1.0f : -1.0f;

        // 70% strafe, 30% forward
        clBot.moveDir[0] = forward[0] * 0.3f + right[0] * strafeDir * 0.7f;
        clBot.moveDir[1] = forward[1] * 0.3f + right[1] * strafeDir * 0.7f;
        clBot.moveDir[2] = 0;
        VectorNormalize(clBot.moveDir);
        clBot.isMoving = qtrue;
    }
}

/*
====================
CL_Bot_ThinkDead

Dead state - waiting to respawn
====================
*/
static void CL_Bot_ThinkDead(void)
{
    clBot.isMoving = qfalse;
    clBot.enemyEntityNum = -1;
}

/*
====================
CL_Bot_CreateCmd

Create the usercmd for this frame
Returns qtrue if bot handled the command, qfalse to use normal input
====================
*/
qboolean CL_Bot_CreateCmd(usercmd_t *cmd, usereyes_t *eyeinfo)
{
    if (!cl_bot || !cl_bot->integer) {
        clBotEyeInfoValid = qfalse;
        return qfalse;
    }

    if (clc.state != CA_ACTIVE || !cl.snap.valid) {
        clBotEyeInfoValid = qfalse;
        return qfalse;
    }

    // Initialize command
    memset(cmd, 0, sizeof(*cmd));
    cmd->serverTime = cl.serverTime;

    // Run bot frame logic first
    CL_Bot_Frame();

    // Handle respawning
    if (cl.snap.ps.pm_type == PM_DEAD) {
        CL_Bot_HandleRespawn(cmd);
        CL_Bot_UpdateAiming(cmd);

        // Store eye info
        clBotEyeInfo.ofs[0] = 0;
        clBotEyeInfo.ofs[1] = 0;
        clBotEyeInfo.ofs[2] = 0;
        clBotEyeInfo.angles[0] = clBot.currentAngles[PITCH];
        clBotEyeInfo.angles[1] = clBot.currentAngles[YAW];
        clBotEyeInfoValid = qtrue;

        if (eyeinfo) {
            *eyeinfo = clBotEyeInfo;
        }
        return qtrue;
    }

    // Track spawn time
    if (clBot.state == CLBOT_STATE_DEAD) {
        clBot.spawnedTime = cls.realtime;
    }

    // Generate movement
    CL_Bot_UpdateMovement(cmd);

    // Generate aiming
    CL_Bot_UpdateAiming(cmd);

    // Generate buttons (attack, use, etc)
    CL_Bot_UpdateButtons(cmd);

    // Always run
    cmd->buttons |= BUTTON_RUN;

    // Store eye info for CL_EyeInfo
    clBotEyeInfo.ofs[0] = 0;
    clBotEyeInfo.ofs[1] = 0;
    clBotEyeInfo.ofs[2] = (signed char)cl.snap.ps.viewheight;
    clBotEyeInfo.angles[0] = clBot.currentAngles[PITCH];
    clBotEyeInfo.angles[1] = clBot.currentAngles[YAW];
    clBotEyeInfoValid = qtrue;

    if (eyeinfo) {
        *eyeinfo = clBotEyeInfo;
    }

    if (cl_bot_debug && cl_bot_debug->integer > 2) {
        Com_Printf("Bot cmd: fwd=%d right=%d btn=%x ang=%.1f,%.1f\n",
            cmd->forwardmove, cmd->rightmove, cmd->buttons,
            clBot.currentAngles[PITCH], clBot.currentAngles[YAW]);
    }

    return qtrue;
}

/*
====================
CL_Bot_UpdateMovement

Update movement portion of usercmd
====================
*/
static void CL_Bot_UpdateMovement(usercmd_t *cmd)
{
    if (!clBot.isMoving) {
        return;
    }

    // Convert move direction to forward/right relative to view
    vec3_t forward, right, up;
    vec3_t flatAngles;

    flatAngles[PITCH] = 0;
    flatAngles[YAW] = clBot.currentAngles[YAW];
    flatAngles[ROLL] = 0;

    AngleVectors(flatAngles, forward, right, up);

    // Project move direction onto horizontal plane
    vec3_t moveDir;
    VectorCopy(clBot.moveDir, moveDir);
    moveDir[2] = 0;
    VectorNormalize(moveDir);

    // Calculate forward and right components
    float forwardMove = DotProduct(moveDir, forward);
    float rightMove = DotProduct(moveDir, right);

    // Apply movement speed
    int moveSpeed = cl_bot_movespeed ? cl_bot_movespeed->integer : 127;
    if (moveSpeed > 127) moveSpeed = 127;
    if (moveSpeed < 0) moveSpeed = 0;

    cmd->forwardmove = (signed char)(forwardMove * moveSpeed);
    cmd->rightmove = (signed char)(rightMove * moveSpeed);

    // Check if we should jump (if we've been stuck)
    if (clBot.shouldJump && cls.realtime - clBot.lastJumpTime > CLBOT_JUMP_COOLDOWN) {
        cmd->upmove = 127;
        clBot.lastJumpTime = cls.realtime;
        clBot.shouldJump = qfalse;
    }
}

/*
====================
CL_Bot_UpdateAiming

Update aiming portion of usercmd
====================
*/
static void CL_Bot_UpdateAiming(usercmd_t *cmd)
{
    float aimSpeed = cl_bot_aimspeed ? cl_bot_aimspeed->value : 360.0f;
    float frameTime = cls.frametime / 1000.0f;
    if (frameTime <= 0) frameTime = 0.016f; // Default to ~60fps
    float maxChange = aimSpeed * frameTime;

    // Deadzone: don't make tiny corrections that cause shaking
    // Use larger deadzone when roaming, smaller when attacking
    float deadzone = (clBot.state == CLBOT_STATE_ATTACKING) ? 1.0f : 3.0f;

    // Smoothly interpolate current angles towards target angles
    for (int i = 0; i < 2; i++) {
        float diff = CL_Bot_AngleDiff(clBot.targetAngles[i], clBot.currentAngles[i]);

        // Skip tiny corrections within deadzone
        if (fabs(diff) <= deadzone) {
            continue;
        }

        if (fabs(diff) <= maxChange) {
            clBot.currentAngles[i] = clBot.targetAngles[i];
        } else if (diff > 0) {
            clBot.currentAngles[i] += maxChange;
        } else {
            clBot.currentAngles[i] -= maxChange;
        }
    }

    // Normalize yaw to 0-360, but keep pitch in -89 to 89
    clBot.currentAngles[YAW] = AngleMod(clBot.currentAngles[YAW]);

    // Clamp pitch (don't use AngleMod on pitch - it breaks negative angles)
    if (clBot.currentAngles[PITCH] > 89) {
        clBot.currentAngles[PITCH] = 89;
    } else if (clBot.currentAngles[PITCH] < -89) {
        clBot.currentAngles[PITCH] = -89;
    }

    // Set command angles (these are delta from delta_angles)
    for (int i = 0; i < 3; i++) {
        cmd->angles[i] = ANGLE2SHORT(clBot.currentAngles[i]) - cl.snap.ps.delta_angles[i];
    }

    // Update client viewangles for prediction
    VectorCopy(clBot.currentAngles, cl.viewangles);
}

/*
====================
CL_Bot_UpdateButtons

Update button portion of usercmd (attack, use, etc)
====================
*/
static void CL_Bot_UpdateButtons(usercmd_t *cmd)
{
    // Only attack if we have an enemy and we're not in spawn protection
    if (clBot.state == CLBOT_STATE_ATTACKING && clBot.enemyEntityNum >= 0) {
        // Don't fire immediately after spawning
        if (cls.realtime - clBot.spawnedTime < CLBOT_SPAWN_GRACE_TIME) {
            return;
        }

        // Hold fire button continuously while attacking
        // Most weapons need the button held, not just pressed once
        cmd->buttons |= BUTTON_ATTACKLEFT;
    }
}

/*
====================
CL_Bot_CanSeePoint

Check if there's a clear line of sight from bot's eye to a point
====================
*/
static qboolean CL_Bot_CanSeePoint(vec3_t targetPos)
{
    trace_t trace;
    vec3_t start;
    vec3_t mins = {0, 0, 0};
    vec3_t maxs = {0, 0, 0};

    // Start from our eye position
    VectorCopy(cl.snap.ps.origin, start);
    start[2] += cl.snap.ps.viewheight;

    // Trace against world geometry only (clipHandle 0 = world)
    CM_BoxTrace(&trace, start, targetPos, mins, maxs, 0, CONTENTS_SOLID, qfalse);

    // If trace completed without hitting anything, we can see the point
    return (trace.fraction >= 0.98f);
}

/*
====================
CL_Bot_CheckObstacles

Check for obstacles ahead and return a turn direction if blocked
Returns: 0 = clear, 1 = turn right, -1 = turn left
====================
*/
#define OBSTACLE_CHECK_DIST 100.0f
#define OBSTACLE_CHECK_INTERVAL 100  // Check every 100ms

static int CL_Bot_CheckObstacles(void)
{
    static int lastCheckTime = 0;
    static int lastResult = 0;

    // Don't check too frequently
    if (cls.realtime - lastCheckTime < OBSTACLE_CHECK_INTERVAL) {
        return lastResult;
    }
    lastCheckTime = cls.realtime;

    trace_t trace;
    vec3_t start, end;
    vec3_t forward, right;
    vec3_t mins = {-16, -16, 0};
    vec3_t maxs = {16, 16, 32};

    // Get forward direction from current yaw
    vec3_t angles;
    angles[PITCH] = 0;
    angles[YAW] = clBot.currentAngles[YAW];
    angles[ROLL] = 0;
    AngleVectors(angles, forward, right, NULL);

    // Start from player position (at knee height to detect low obstacles)
    VectorCopy(cl.snap.ps.origin, start);
    start[2] += 20;

    // Trace forward
    VectorMA(start, OBSTACLE_CHECK_DIST, forward, end);
    CM_BoxTrace(&trace, start, end, mins, maxs, 0, CONTENTS_SOLID, qfalse);

    if (trace.fraction < 0.8f) {
        // Obstacle ahead, check left and right to decide which way to turn
        vec3_t leftEnd, rightEnd;
        trace_t leftTrace, rightTrace;

        // Check left
        VectorMA(start, OBSTACLE_CHECK_DIST, forward, leftEnd);
        VectorMA(leftEnd, -50, right, leftEnd);  // 50 units to the left
        CM_BoxTrace(&leftTrace, start, leftEnd, mins, maxs, 0, CONTENTS_SOLID, qfalse);

        // Check right
        VectorMA(start, OBSTACLE_CHECK_DIST, forward, rightEnd);
        VectorMA(rightEnd, 50, right, rightEnd);  // 50 units to the right
        CM_BoxTrace(&rightTrace, start, rightEnd, mins, maxs, 0, CONTENTS_SOLID, qfalse);

        // Turn towards the clearer direction
        if (rightTrace.fraction > leftTrace.fraction) {
            lastResult = 1;  // Turn right
        } else {
            lastResult = -1; // Turn left
        }

        if (cl_bot_debug && cl_bot_debug->integer) {
            Com_Printf("Obstacle ahead! Turn %s (L:%.2f R:%.2f)\n",
                lastResult > 0 ? "right" : "left",
                leftTrace.fraction, rightTrace.fraction);
        }

        return lastResult;
    }

    lastResult = 0;
    return 0;  // Clear ahead
}

/*
====================
CL_Bot_CheckForEnemies

Scan visible entities for enemies
====================
*/
static void CL_Bot_CheckForEnemies(void)
{
    int i;
    entityState_t *ent;
    float bestDist = cl_bot_attackdist ? cl_bot_attackdist->value : 2048;
    int bestEnemy = -1;
    int myTeam = cl.snap.ps.stats[STAT_TEAM];

    // Go through all entities in the snapshot
    for (i = 0; i < cl.snap.numEntities; i++) {
        int entityIndex = (cl.snap.parseEntitiesNum + i) & (MAX_PARSE_ENTITIES - 1);
        ent = &cl.parseEntities[entityIndex];

        // Skip non-players
        if (ent->eType != ET_PLAYER) {
            continue;
        }

        // Skip self
        if (ent->number == cl.snap.ps.clientNum) {
            continue;
        }

        // Skip dead players
        if (ent->eFlags & EF_DEAD) {
            continue;
        }

        // Skip teammates in team games
        if (myTeam == TEAM_ALLIES || myTeam == TEAM_AXIS) {
            qboolean sameTeam = qfalse;
            if ((myTeam == TEAM_ALLIES && (ent->eFlags & EF_ALLIES)) ||
                (myTeam == TEAM_AXIS && (ent->eFlags & EF_AXIS))) {
                sameTeam = qtrue;
            }
            if (sameTeam) {
                continue;
            }
        }

        // Calculate distance
        vec3_t delta;
        VectorSubtract(ent->origin, cl.snap.ps.origin, delta);
        float dist = VectorLength(delta);

        // Check if this is closer than current best
        if (dist < bestDist) {
            // Check line of sight - can we actually see this enemy?
            vec3_t enemyCenter;
            VectorCopy(ent->origin, enemyCenter);
            enemyCenter[2] += 40; // Aim at chest level

            if (!CL_Bot_CanSeePoint(enemyCenter)) {
                if (cl_bot_debug && cl_bot_debug->integer > 1) {
                    Com_Printf("Enemy %d blocked by wall\n", ent->number);
                }
                continue;
            }

            bestDist = dist;
            bestEnemy = ent->number;

            if (cl_bot_debug && cl_bot_debug->integer) {
                Com_Printf("Found enemy %d at distance %.0f\n", ent->number, dist);
            }
        }
    }

    // Update enemy tracking
    if (bestEnemy >= 0) {
        clBot.enemyEntityNum = bestEnemy;
        clBot.lastEnemySeenTime = cls.realtime;
    }
}

/*
====================
CL_Bot_HandleTeamJoin

Handle automatic team joining
====================
*/
static void CL_Bot_HandleTeamJoin(void)
{
    int team = cl.snap.ps.stats[STAT_TEAM];

    if (team == TEAM_NONE || team == TEAM_SPECTATOR) {
        // Wait a bit before joining
        if (!clBot.hasJoinedTeam) {
            if (clBot.joinTeamTime == 0) {
                clBot.joinTeamTime = cls.realtime;
                if (cl_bot_debug && cl_bot_debug->integer) {
                    Com_Printf("Bot waiting to join team...\n");
                }
            } else if (cls.realtime - clBot.joinTeamTime > CLBOT_TEAM_JOIN_DELAY) {
                // Send auto-join command
                if (cl_bot_debug && cl_bot_debug->integer) {
                    Com_Printf("Bot sending auto_join_team command\n");
                }
                CL_AddReliableCommand("auto_join_team", qfalse);
                clBot.hasJoinedTeam = qtrue;
                clBot.weaponSelectTime = cls.realtime; // Start weapon select timer
            }
        }
    } else {
        if (!clBot.hasJoinedTeam) {
            clBot.hasJoinedTeam = qtrue;
            clBot.weaponSelectTime = cls.realtime; // Start weapon select timer
        }
    }

    // Handle primary weapon selection - retry multiple times with delay
    if (clBot.hasJoinedTeam && team != TEAM_SPECTATOR && team != TEAM_NONE) {
        // Send weapon select command after a short delay, retry a few times
        if (clBot.weaponSelectAttempts < 3) {
            int delay = 500 + (clBot.weaponSelectAttempts * 1000); // 500ms, 1500ms, 2500ms
            if (cls.realtime - clBot.weaponSelectTime > delay) {
                if (cl_bot_debug && cl_bot_debug->integer) {
                    Com_Printf("Bot selecting primary weapon (attempt %d)\n", clBot.weaponSelectAttempts + 1);
                }
                CL_AddReliableCommand("primarydmweapon smg", qfalse);
                clBot.weaponSelectAttempts++;
                clBot.weaponSelectTime = cls.realtime;
            }
        }
    }
}

/*
====================
CL_Bot_HandleRespawn

Handle respawning when dead
====================
*/
static void CL_Bot_HandleRespawn(usercmd_t *cmd)
{
    // Toggle attack button to respawn
    if (cls.realtime - clBot.lastRespawnTime > CLBOT_RESPAWN_DELAY) {
        cmd->buttons |= BUTTON_ATTACKLEFT;
        clBot.lastRespawnTime = cls.realtime;

        if (cl_bot_debug && cl_bot_debug->integer) {
            Com_Printf("Bot attempting respawn\n");
        }
    }
}

/*
====================
CL_Bot_PickNewRoamTarget

Select a new roaming target/direction
====================
*/
static void CL_Bot_PickNewRoamTarget(void)
{
    // Pick a mostly forward direction with small random deviation
    // This prevents the erratic zig-zagging behavior
    // Occasionally make a bigger turn (20% chance)
    float deviation;
    if (rand() % 100 < 20) {
        // Bigger turn: ±45 to ±90 degrees
        deviation = (float)(45 + (rand() % 45));
        if (rand() % 2) {
            deviation = -deviation;
        }
    } else {
        // Small adjustment: ±30 degrees max
        deviation = (float)((rand() % 60) - 30);
    }

    float yaw = clBot.currentAngles[YAW] + deviation;

    clBot.targetAngles[YAW] = AngleMod(yaw);
    clBot.targetAngles[PITCH] = 0;

    // Set move direction based on the NEW target yaw
    vec3_t angles;
    angles[PITCH] = 0;
    angles[YAW] = clBot.targetAngles[YAW];
    angles[ROLL] = 0;

    vec3_t forward;
    AngleVectors(angles, forward, NULL, NULL);
    VectorCopy(forward, clBot.moveDir);

    clBot.lastMoveChangeTime = cls.realtime;

    if (cl_bot_debug && cl_bot_debug->integer) {
        Com_Printf("Bot new roam direction: yaw=%.1f (deviation=%.1f)\n", clBot.targetAngles[YAW], deviation);
    }
}

/*
====================
CL_Bot_AngleDiff

Calculate shortest difference between two angles
====================
*/
static float CL_Bot_AngleDiff(float ang1, float ang2)
{
    float diff = ang1 - ang2;

    while (diff > 180.0f) {
        diff -= 360.0f;
    }
    while (diff < -180.0f) {
        diff += 360.0f;
    }

    return diff;
}
