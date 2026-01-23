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

// CVars
cvar_t *cl_bot           = NULL;
cvar_t *cl_bot_movespeed = NULL;
cvar_t *cl_bot_aimspeed  = NULL;
cvar_t *cl_bot_attackdist = NULL;
cvar_t *cl_bot_firedelay = NULL;
cvar_t *cl_bot_roamtime  = NULL;

// Constants
#define CLBOT_ROAM_CHANGE_TIME      3000    // Change roam direction every 3 seconds
#define CLBOT_ATTACK_TIMEOUT        5000    // Stop attacking if no enemy seen for 5 seconds
#define CLBOT_RESPAWN_DELAY         1000    // Wait 1 second between respawn attempts
#define CLBOT_TEAM_JOIN_DELAY       2000    // Wait 2 seconds before joining team
#define CLBOT_JUMP_COOLDOWN         1000    // Minimum time between jumps
#define CLBOT_MIN_FIRE_DELAY        100     // Minimum time between shots
#define CLBOT_SPAWN_GRACE_TIME      1000    // Grace period after spawning

//
// Forward declarations
//
static void     CL_Bot_Think(void);
static void     CL_Bot_ThinkIdle(void);
static void     CL_Bot_ThinkRoaming(void);
static void     CL_Bot_ThinkAttacking(void);
static void     CL_Bot_ThinkDead(void);
static void     CL_Bot_UpdateMovement(usercmd_t *cmd);
static void     CL_Bot_UpdateAiming(usercmd_t *cmd, usereyes_t *eyeinfo);
static void     CL_Bot_UpdateButtons(usercmd_t *cmd);
static void     CL_Bot_CheckForEnemies(void);
static void     CL_Bot_HandleTeamJoin(void);
static void     CL_Bot_HandleRespawn(usercmd_t *cmd);
static qboolean CL_Bot_CanSeeEntity(int entityNum);
static float    CL_Bot_AngleDiff(float ang1, float ang2);
static void     CL_Bot_PickNewRoamTarget(void);
static void     CL_Bot_SetRandomMoveDir(void);

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
    cl_bot_aimspeed  = Cvar_Get("cl_bot_aimspeed", "180", CVAR_ARCHIVE);
    cl_bot_attackdist = Cvar_Get("cl_bot_attackdist", "2048", CVAR_ARCHIVE);
    cl_bot_firedelay = Cvar_Get("cl_bot_firedelay", "200", CVAR_ARCHIVE);
    cl_bot_roamtime  = Cvar_Get("cl_bot_roamtime", "3000", CVAR_ARCHIVE);

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
        default:
            break;
        }
    }
}

/*
====================
CL_Bot_Frame

Called every frame when bot mode is active
====================
*/
void CL_Bot_Frame(void)
{
    if (!cl_bot || !cl_bot->integer) {
        clBot.active = qfalse;
        return;
    }

    clBot.active = qtrue;

    // Only think if we're connected and have a valid snapshot
    if (clc.state != CA_ACTIVE || !cl.snap.valid) {
        return;
    }

    CL_Bot_Think();
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

    clBot.isMoving = qtrue;
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
    if (clBot.enemyEntityNum < 0 || clBot.enemyEntityNum >= MAX_GENTITIES) {
        CL_Bot_SetState(CLBOT_STATE_ROAMING);
        return;
    }

    // Check if we've lost sight of enemy for too long
    if (cls.realtime - clBot.lastEnemySeenTime > CLBOT_ATTACK_TIMEOUT) {
        clBot.enemyEntityNum = -1;
        CL_Bot_SetState(CLBOT_STATE_ROAMING);
        return;
    }

    // Get enemy entity
    enemy = &cl.parseEntities[(cl.snap.parseEntitiesNum + clBot.enemyEntityNum) & (MAX_PARSE_ENTITIES - 1)];

    // Calculate distance to enemy
    VectorSubtract(enemy->origin, cl.snap.ps.origin, delta);
    dist = VectorLength(delta);

    // Update aim target
    VectorCopy(enemy->origin, clBot.enemyLastPos);

    // Calculate aim angles
    vec3_t aimDir;
    VectorCopy(delta, aimDir);
    aimDir[2] += 40; // Aim at upper body
    VectorNormalize(aimDir);
    vectoangles(aimDir, clBot.targetAngles);

    // Move towards or away from enemy based on distance
    float attackDist = cl_bot_attackdist ? cl_bot_attackdist->value : 2048;

    if (dist > attackDist * 0.8f) {
        // Move towards enemy
        VectorNormalize(delta);
        VectorCopy(delta, clBot.moveDir);
        clBot.isMoving = qtrue;
    } else if (dist < 256) {
        // Too close, back up
        VectorNormalize(delta);
        VectorNegate(delta, clBot.moveDir);
        clBot.isMoving = qtrue;
    } else {
        // Good distance, strafe
        vec3_t right;
        AngleVectors(clBot.currentAngles, NULL, right, NULL);
        if ((cls.realtime / 1000) % 2 == 0) {
            VectorCopy(right, clBot.moveDir);
        } else {
            VectorNegate(right, clBot.moveDir);
        }
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
    if (!cl_bot || !cl_bot->integer || !clBot.active) {
        return qfalse;
    }

    if (clc.state != CA_ACTIVE || !cl.snap.valid) {
        return qfalse;
    }

    // Initialize command
    memset(cmd, 0, sizeof(*cmd));
    cmd->serverTime = cl.serverTime;

    // Handle respawning
    if (cl.snap.ps.pm_type == PM_DEAD) {
        CL_Bot_HandleRespawn(cmd);
        CL_Bot_UpdateAiming(cmd, eyeinfo);
        return qtrue;
    }

    // Track spawn time
    if (clBot.state == CLBOT_STATE_DEAD) {
        clBot.spawnedTime = cls.realtime;
    }

    // Run bot frame logic
    CL_Bot_Frame();

    // Generate movement
    CL_Bot_UpdateMovement(cmd);

    // Generate aiming
    CL_Bot_UpdateAiming(cmd, eyeinfo);

    // Generate buttons (attack, use, etc)
    CL_Bot_UpdateButtons(cmd);

    // Always run
    cmd->buttons |= BUTTON_RUN;

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
    AngleVectors(clBot.currentAngles, forward, right, up);

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
    moveSpeed = Q_clamp(moveSpeed, 0, 127);

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
static void CL_Bot_UpdateAiming(usercmd_t *cmd, usereyes_t *eyeinfo)
{
    float aimSpeed = cl_bot_aimspeed ? cl_bot_aimspeed->value : 180.0f;
    float frameTime = cls.frametime / 1000.0f;
    float maxChange = aimSpeed * frameTime;
    int i;

    // Smoothly interpolate current angles towards target angles
    for (i = 0; i < 2; i++) {
        float diff = CL_Bot_AngleDiff(clBot.currentAngles[i], clBot.targetAngles[i]);
        float change = Q_clamp(diff, -maxChange, maxChange);
        clBot.currentAngles[i] = AngleMod(clBot.currentAngles[i] - change);
    }

    // Clamp pitch
    if (clBot.currentAngles[PITCH] > 89) {
        clBot.currentAngles[PITCH] = 89;
    } else if (clBot.currentAngles[PITCH] < -89) {
        clBot.currentAngles[PITCH] = -89;
    }

    // Set command angles (relative to delta_angles)
    cmd->angles[PITCH] = ANGLE2SHORT(clBot.currentAngles[PITCH]) - cl.snap.ps.delta_angles[PITCH];
    cmd->angles[YAW] = ANGLE2SHORT(clBot.currentAngles[YAW]) - cl.snap.ps.delta_angles[YAW];
    cmd->angles[ROLL] = 0;

    // Set eye info
    eyeinfo->ofs[0] = 0;
    eyeinfo->ofs[1] = 0;
    eyeinfo->ofs[2] = cl.snap.ps.viewheight;
    eyeinfo->angles[0] = clBot.currentAngles[PITCH];
    eyeinfo->angles[1] = clBot.currentAngles[YAW];

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
        // Check fire delay
        int fireDelay = cl_bot_firedelay ? cl_bot_firedelay->integer : CLBOT_MIN_FIRE_DELAY;

        // Don't fire immediately after spawning
        if (cls.realtime - clBot.spawnedTime < CLBOT_SPAWN_GRACE_TIME) {
            return;
        }

        if (cls.realtime - clBot.lastFireTime >= fireDelay) {
            cmd->buttons |= BUTTON_ATTACKLEFT;
            clBot.lastFireTime = cls.realtime;
        }
    }
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
            // Check if same team
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
            // Check if we can "see" this entity (basic visibility)
            if (CL_Bot_CanSeeEntity(ent->number)) {
                bestDist = dist;
                bestEnemy = ent->number;
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
CL_Bot_CanSeeEntity

Basic visibility check using trace
Note: This is a simplified check - we don't have full trace access on client
====================
*/
static qboolean CL_Bot_CanSeeEntity(int entityNum)
{
    // For now, just assume we can see entities in our snapshot
    // A more sophisticated implementation could use CG_Trace
    return qtrue;
}

/*
====================
CL_Bot_HandleTeamJoin

Handle automatic team joining
====================
*/
static void CL_Bot_HandleTeamJoin(void)
{
    // Check if we need to join a team
    int team = cl.snap.ps.stats[STAT_TEAM];

    if (team == TEAM_NONE || team == TEAM_SPECTATOR) {
        // Wait a bit before joining
        if (!clBot.hasJoinedTeam) {
            if (clBot.joinTeamTime == 0) {
                clBot.joinTeamTime = cls.realtime;
            } else if (cls.realtime - clBot.joinTeamTime > CLBOT_TEAM_JOIN_DELAY) {
                // Send auto-join command
                CL_AddReliableCommand("auto_join_team", qfalse);
                clBot.hasJoinedTeam = qtrue;
            }
        }
    } else {
        clBot.hasJoinedTeam = qtrue;
    }

    // Handle primary weapon selection
    if (!clBot.hasPrimaryWeapon && clBot.hasJoinedTeam && team != TEAM_SPECTATOR) {
        CL_AddReliableCommand("primarydmweapon auto", qfalse);
        clBot.hasPrimaryWeapon = qtrue;
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
        cmd->buttons ^= BUTTON_ATTACKLEFT;
        clBot.lastRespawnTime = cls.realtime;
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
    // Random direction based on current position
    float yaw = (rand() % 360);
    float pitch = 0;

    clBot.targetAngles[YAW] = yaw;
    clBot.targetAngles[PITCH] = pitch;

    // Set move direction based on yaw
    vec3_t forward;
    AngleVectors(clBot.targetAngles, forward, NULL, NULL);
    VectorCopy(forward, clBot.moveDir);

    clBot.lastMoveChangeTime = cls.realtime;
}

/*
====================
CL_Bot_SetRandomMoveDir

Set a random movement direction
====================
*/
static void CL_Bot_SetRandomMoveDir(void)
{
    float yaw = (rand() % 360);

    vec3_t angles;
    angles[PITCH] = 0;
    angles[YAW] = yaw;
    angles[ROLL] = 0;

    vec3_t forward;
    AngleVectors(angles, forward, NULL, NULL);
    VectorCopy(forward, clBot.moveDir);
}

/*
====================
CL_Bot_AngleDiff

Calculate difference between two angles
====================
*/
static float CL_Bot_AngleDiff(float ang1, float ang2)
{
    float diff = ang1 - ang2;

    if (ang1 > ang2) {
        if (diff > 180.0f) {
            diff -= 360.0f;
        }
    } else {
        if (diff < -180.0f) {
            diff += 360.0f;
        }
    }

    return diff;
}
