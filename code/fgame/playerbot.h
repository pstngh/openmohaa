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
// playerbot.h: Multiplayer bot system.

#pragma once

#include "player.h"

class BotRotation
{
public:
    BotRotation();

    void SetControlledEntity(Player *newEntity);

    void TurnThink(usercmd_t& botcmd, usereyes_t& eyeinfo);
    void SetTargetAngles(Vector vAngles);
    void AimAt(Vector vPos);

private:
    SafePtr<Player> controlledEntity;

    Vector m_vTargetAng;
};

class BotController : public Listener
{
    BotRotation rotation;

    // Targeting
    SafePtr<Sentient> m_pEnemy;
    int               m_iStopAimTime;
    int               m_iNextEnemyScanTime;
    int               m_iEnemyScanCursor;
    bool              m_bEnemyVisible;

    // Input
    usercmd_t  m_botCmd;
    usereyes_t m_botEyes;

    // Melee
    int m_iNextMeleeTime;

    // Roomba movement (all states)
    int    m_iRoombaTurnDir;            // -1 = left, 1 = right (fixed at spawn)
    float  m_fRoombaYaw;                // accumulated yaw angle
    float  m_fRoombaTurnSpeed;          // turn speed (degrees/second)
    bool   m_bAimOverride;             // true = attack is aiming, skip roomba yaw this frame
    int    m_iStrafeDir;                // -1 = left, 1 = right
    int    m_iNextStrafeSwitchTime;     // when to flip strafe direction
    int    m_iNextMovementCheckTime;    // rate-limit collision/use traces

    // Jump detection
    bool   m_bJump;
    int    m_iJumpCheckTime;
    Vector m_vJumpLocation;
private:
    DelegateHandle delegateHandle_spawned;

private:
    Weapon *FindMeleeWeapon(void);

    void CheckUse(void);
    void CheckValidWeapon(void);
    void CheckObstacleJump(void);

    void UpdateEnemy(void);
    void UpdateAimAndMelee(void);
    bool IsValidEnemy(Sentient *sent) const;

public:
    CLASS_PROTOTYPE(BotController);

    BotController();
    ~BotController();

    void GetEyeInfo(usereyes_t *eyeinfo);
    void GetUsercmd(usercmd_t *ucmd);

    void UpdateBotStates(void);
    void ClearEnemy(void);

    void Think();

    void Spawned(void);

public:
    void    setControlledEntity(Player *player);
    Player *getControlledEntity() const;

private:
    SafePtr<Player> controlledEnt;
};

class BotControllerManager : public Listener
{
public:
    CLASS_PROTOTYPE(BotControllerManager);

public:
    ~BotControllerManager();

    BotController                    *createController(Player *player);
    void                              removeController(BotController *controller);
    BotController                    *findController(Entity *ent);
    const Container<BotController *>& getControllers() const;

    void Cleanup();
    void ThinkControllers();

private:
    Container<BotController *> controllers;
};

class BotManager : public Listener
{
public:
    CLASS_PROTOTYPE(BotManager);

public:
    BotControllerManager& getControllerManager();

    void Cleanup();
    void Frame();

private:
    BotControllerManager botControllerManager;
};

extern BotManager botManager;
