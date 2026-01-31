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
// cl_bot.h: Client-side bot system - allows client to play autonomously

#pragma once

#include "../qcommon/q_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// Bot states
//
typedef enum {
    CLBOT_STATE_IDLE,
    CLBOT_STATE_ROAMING,
    CLBOT_STATE_ATTACKING,
    CLBOT_STATE_DEAD
} clBotState_t;

//
// Client bot structure
//
typedef struct {
    qboolean    active;             // Whether bot mode is enabled
    qboolean    initialized;        // Whether the bot has been initialized

    // State machine
    clBotState_t state;
    int         stateTime;          // Time when state started

    // Movement
    vec3_t      moveTarget;         // Current movement target
    vec3_t      moveDir;            // Current movement direction
    int         moveTime;           // Time when movement target was set
    int         lastMoveChangeTime; // Time when we last changed movement direction
    qboolean    isMoving;

    // Combat
    int         enemyEntityNum;     // Entity number of current enemy (-1 if none)
    vec3_t      enemyLastPos;       // Last known enemy position
    vec3_t      enemyVelocity;      // Enemy velocity for lead targeting
    int         enemyAcquiredTime;  // When enemy was first acquired
    int         lastEnemySeenTime;  // Time when enemy was last seen
    int         lastFireTime;       // Time of last fire
    int         lastMeleeTime;      // Time of last melee attack button toggle
    int         attackStartTime;    // When attack started

    // Aiming
    vec3_t      targetAngles;       // Angles we want to aim at
    vec3_t      currentAngles;      // Current view angles
    float       aimSpeed;           // Current aim speed factor

    // Jumping
    int         lastJumpTime;       // Time of last jump
    qboolean    shouldJump;         // Whether we should jump this frame

    // Stuck detection
    vec3_t      lastPosition;       // Position at last stuck check
    int         lastStuckCheckTime; // When we last checked if stuck
    int         stuckCount;         // How many times we've been stuck recently

    // Position history (to avoid going in circles)
    #define CLBOT_POS_HISTORY_SIZE 16
    vec3_t      posHistory[CLBOT_POS_HISTORY_SIZE];  // Recent positions
    int         posHistoryIndex;    // Current index in circular buffer
    int         lastPosRecordTime;  // When we last recorded position

    // Team/spawn handling
    qboolean    hasJoinedTeam;      // Whether we've sent team join command
    int         joinTeamTime;       // When we requested to join a team
    int         weaponSelectTime;   // When we last sent weapon select command
    int         weaponSelectAttempts; // Number of weapon select attempts
    int         weaponCommandSendCount; // Weapon command send counter (like cg.iWeaponCommandSend)
    int         lastRespawnTime;    // When we last tried to respawn
    int         spawnedTime;        // When we spawned
    qboolean    wasConnected;       // Whether we were connected last frame (for auto-reconnect)
    int         reconnectFrameCount; // Frame counter for auto-reconnect check
} clientBot_t;

extern clientBot_t  clBot;
extern cvar_t       *cl_bot;
extern cvar_t       *cl_bot_movespeed;
extern cvar_t       *cl_bot_aimspeed;
extern cvar_t       *cl_bot_attackdist;
extern cvar_t       *bot_server;

//
// Initialization
//
void        CL_Bot_Init(void);
void        CL_Bot_Shutdown(void);

//
// Frame processing
//
void        CL_Bot_Frame(void);
qboolean    CL_Bot_CreateCmd(usercmd_t *cmd, usereyes_t *eyeinfo);
qboolean    CL_Bot_IsActive(void);
qboolean    CL_Bot_GetEyeInfo(usereyes_t *eyeinfo);

//
// State management
//
void        CL_Bot_SetState(clBotState_t newState);
void        CL_Bot_Reset(void);

//
// Commands
//
void        CL_Bot_Enable_f(void);
void        CL_Bot_Disable_f(void);

#ifdef __cplusplus
}
#endif
