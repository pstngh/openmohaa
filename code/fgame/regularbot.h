/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.
===========================================================================
*/
// regularbot.h: Isolated, game-only SMG bots.

#pragma once

#include "player.h"
#include "navigate.h"
#include "navigation_path.h"

// This is the stock OpenMoHAA navigation mover, kept separate from the
// real-slot Roomba controller.
class RegularBotMovement
{
public:
    RegularBotMovement();
    ~RegularBotMovement();

    void SetControlledEntity(Player *newEntity);
    void MoveThink(usercmd_t& botcmd);

    bool AvoidPath(
        Vector vPos,
        float  fAvoidRadius,
        Vector vPreferredDir = vec_zero,
        float *vLeashHome    = NULL,
        float  fLeashRadius  = 0.0f,
        float  fMinTravelDistance = 0.0f
    );
    void MoveNear(Vector vNear, float fRadius, float *vLeashHome = NULL, float fLeashRadius = 0.0f);
    void MoveTo(Vector vPos, float *vLeashHome = NULL, float fLeashRadius = 0.0f);

    bool MoveDone();
    bool IsMoving(void);
    void ClearMove(void);

    Vector GetCurrentGoal() const;
    Vector GetCurrentPathDirection() const;

private:
    Vector CalculateDir(const Vector& delta) const;
    Vector CalculateRelativeWishDirection(const Vector& dir) const;
    void   CheckEndPos(Entity *entity);
    bool   CheckJump(usercmd_t& botcmd, trace_t& forwardTrace);
    void   CheckJumpOverEdge(usercmd_t& botcmd, const trace_t& forwardTrace);
    void   NewMove();
    Vector FixDeltaFromCollision(const Vector& delta);
    void   CalculateBestFrontAvoidance(
          const Vector& targetOrg,
          float         maxDist,
          const Vector& forward,
          const Vector& right,
          float&        bestFrac,
          Vector&       bestPos
      );

private:
    SafePtr<Player> controlledEntity;
    IPather        *m_pPath;
    int             m_iLastMoveTime;

    Vector m_vCurrentOrigin;
    Vector m_vTargetPos;
    Vector m_vCurrentGoal;
    Vector m_vCurrentDir;
    Vector m_vLastCheckPos[2];
    int    m_iTempAwayTime;
    int    m_iNumBlocks;
    int    m_iCheckPathTime;
    int    m_iLastBlockTime;
    int    m_iTempAwayState;
    bool   m_bPathing;

    bool   m_bAvoidCollision;
    bool   m_bCollisionStalled;
    int    m_iCollisionCheckTime;
    Vector m_vTempCollisionAvoidance;

    bool   m_bJump;
    int    m_iJumpCheckTime;
    Vector m_vJumpLocation;
};

class RegularBotRotation
{
public:
    RegularBotRotation();

    void SetControlledEntity(Player *newEntity);
    void Reset();
    void TurnThink(usercmd_t& botcmd, usereyes_t& eyeinfo, float maxDegreesPerSecond);
    void SetTargetAngles(const Vector& angles);

private:
    SafePtr<Player> controlledEntity;
    Vector          m_vTargetAngles;
    Vector          m_vCurrentAngles;
};

class RegularBotController
{
public:
    RegularBotController();
    ~RegularBotController();

    void Think();
    void Spawned();

    void    setControlledEntity(Player *player);
    Player *getControlledEntity() const;

private:
    bool    IsValidEnemy(Sentient *sent) const;
    void    ClearEnemy();
    void    UpdateEnemy();
    void    UpdateNavigation();
    void    UpdateMovement();
    void    UpdateAimAndFire();
    void    UpdateLean();
    void    CheckSmgWeapon();
    Weapon *FindSmgWeapon() const;
    void    ResetAimTracking(const Vector& targetPosition);
    void    RefreshAimErrorTarget();
    void    UpdateAimError();
    Vector  GetLaggedAimPosition(const Vector& targetPosition);

private:
    RegularBotMovement movement;
    RegularBotRotation rotation;

    SafePtr<Player>   controlledEnt;
    SafePtr<Sentient> m_pEnemy;
    DelegateHandle    delegateHandle_spawned;

    usercmd_t  m_botCmd;
    usereyes_t m_botEyes;

    int    m_iEnemyScanCursor;
    int    m_iNextEnemyScanTime;
    int    m_iReactionReadyTime;
    int    m_iStopAimTime;
    int    m_iStopFireTime;
    bool   m_bEnemyVisible;
    Vector m_vLastSeenPosition;
    Vector m_vTrackedAimPosition;
    bool   m_bTrackedAimPosition;
    Vector m_vAimError;
    Vector m_vAimErrorTarget;
    int    m_iNextAimErrorTime;

    int m_iNextNavigationTime;
    int m_iNextNavigationRetryTime;
    int m_iStrafeDirection;

    int m_iNextReloadTime;
};

class RegularBotControllerManager
{
public:
    ~RegularBotControllerManager();

    RegularBotController *createController(Player *player);
    void                  removeController(RegularBotController *controller);
    RegularBotController *findController(Entity *ent);

    const Container<RegularBotController *>& getControllers() const;

    void Cleanup();
    void ThinkControllers();

private:
    Container<RegularBotController *> controllers;
};

extern RegularBotControllerManager regularBotControllerManager;
