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
#include "navigate.h"
#include "navigation_path.h"

#define MAX_BOT_FUNCTIONS 5

typedef struct nodeAttract_s {
    float             m_fRespawnTime;
    AttractiveNodePtr m_pNode;
} nodeAttract_t;

class BotController;

enum bot_fire_decision_t {
    BOT_FIRE_NONE,
    BOT_FIRE_NO_TARGET,
    BOT_FIRE_NO_SIGHT,
    BOT_FIRE_REACTION_DELAY,
    BOT_FIRE_NO_WEAPON,
    BOT_FIRE_NO_AMMO,
    BOT_FIRE_OUT_OF_RANGE,
    BOT_FIRE_SEMIAUTO_BUSY,
    BOT_FIRE_SEMIAUTO_SPREAD,
    BOT_FIRE_FIRING,
    BOT_FIRE_RELOADING
};

enum bot_contact_source_t {
    BOT_CONTACT_NONE,
    BOT_CONTACT_SOUND,
    BOT_CONTACT_DAMAGE,
    BOT_CONTACT_VISUAL
};

enum bot_objective_state_t {
    BOT_OBJECTIVE_NONE,
    BOT_OBJECTIVE_ROUTE,
    BOT_OBJECTIVE_ADVANCE,
    BOT_OBJECTIVE_HOLD,
    BOT_OBJECTIVE_PATROL,
    BOT_OBJECTIVE_COVER,
    BOT_OBJECTIVE_PLANT,
    BOT_OBJECTIVE_DEFUSE
};

enum bot_objective_use_phase_t {
    BOT_OBJECTIVE_USE_AIM,
    BOT_OBJECTIVE_USE_RELEASE,
    BOT_OBJECTIVE_USE_HOLD
};

enum bot_objective_site_state_t {
    BOT_OBJECTIVE_SITE_AVAILABLE,
    BOT_OBJECTIVE_SITE_PLANTED,
    BOT_OBJECTIVE_SITE_DESTROYED
};

struct bot_team_contact_t {
    bool            valid;
    bot_contact_source_t source;
    int             reportTime;
    int             availableTime;
    int             expireTime;
    int             lastLogTime;
    Vector          position;
    SafePtr<Player> reporter;
    SafePtr<Player> enemy;

    bot_team_contact_t();
    void Clear();
};

struct bot_movement_telemetry_t {
    bool  hasCombatTarget;
    bool  pathing;
    bool  blockedRecovery;
    bool  pathCollisionAvoidance;
    bool  movementSuppressed;
    int   strafeDirection;
    int   strafeChangeMsec;
    bool  isLeaning;
    bool  strafeApplied;
    bool  strafeClearanceFlip;
    float strafeClearance;
    float strafeOtherClearance;
    float strafeProbeFraction;
    float strafeIntensity;
    int   radialDirection;
    int   radialChangeMsec;
    bool  radialActive;
    bool  radialForcedCloseRetreat;
    float radialDistance;
    float radialDesiredMove;
    float radialBeforeMove;
    bool  guardTriggered;
    bool  guardHitSentient;
    bool  guardHitWorld;
    bool  guardRemovedComponent;
    float guardFraction;
    int   guardEntity;

    bot_movement_telemetry_t();
    void Reset();
};

struct bot_controller_telemetry_t {
    unsigned int        stateFlags;
    int                 enemyEntity;
    bool                enemyVisible;
    bool                canAttack;
    bool                wantsFire;
    bool                noMove;
    bot_fire_decision_t fireDecision;
    int                 reactionRemainingMsec;
    float               enemyDistance;
    int                 aimAcquireMsec;
    float               aimHeightFraction;
    float               aimErrorFraction;
    float               aimErrorUnits;
    int                 aimLatencyMsec;
    Vector              aimTarget;
    Vector              aimPoint;
    Vector              aimErrorDirection;
    Vector              targetAngles;
    bot_contact_source_t contactSource;
    int                 contactEnemy;
    int                 contactReporter;
    int                 contactAgeMsec;
    bool                contactResponder;
    Vector              contactPosition;
    bot_objective_state_t     objectiveState;
    int                       objectiveSite;
    int                       objectiveRound;
    bot_objective_use_phase_t objectiveUsePhase;
    bool                      objectiveAttacker;
    bool                      objectiveCritical;
    Vector                    objectivePosition;

    bot_controller_telemetry_t();
    void Reset();
};

struct bot_objective_site_t {
    SafePtr<Entity> explosive;
    SafePtr<Entity> trigger;
    SafePtr<Player> user;

    void Clear();
};

class BotMovement
{
public:
    BotMovement();
    ~BotMovement();

    void SetControlledEntity(Player *newEntity);
    void SetCombatTarget(
        const Vector& target,
        bool forceRetreat = false,
        bool forceAdvance = false
    );
    void ClearCombatTarget();

    void MoveThink(usercmd_t& botcmd);

    void AvoidPath(
        Vector vPos,
        float  fAvoidRadius,
        Vector vPreferredDir = vec_zero,
        float *vLeashHome    = NULL,
        float  fLeashRadius  = 0.0f
    );
    void MoveNear(Vector vNear, float fRadius, float *vLeashHome = NULL, float fLeashRadius = 0.0f);
    void MoveTo(Vector vPos, float *vLeashHome = NULL, float fLeashRadius = 0.0f);
    void MoveDirect(Vector vPos, float fRadius);
    bool MoveToBestAttractivePoint(int iMinPriority = 0);
    void AbandonAttractivePoint();

    bool CanMoveTo(Vector vPos);
    bool MoveDone();
    bool IsMoving(void);
    bool IsMovingTo(const Vector& position, float tolerance = 64.0f) const;
    void ClearMove(void);

    Vector GetCurrentGoal() const;
    Vector GetCurrentMoveDirection() const;
    void   ResetTelemetry();
    void   GetTelemetry(bot_movement_telemetry_t& telemetry) const;

private:
    Vector CalculateDir(const Vector& delta) const;
    Vector CalculateRelativeWishDirection(const Vector& dir) const;
    Vector GetCommandMoveVector(const usercmd_t& botcmd) const;
    void   SetCommandMoveVector(usercmd_t& botcmd, const Vector& move) const;
    void   CheckAttractiveNodes();
    void   CheckEndPos(Entity *entity);
    bool   ContinueJump(usercmd_t& botcmd);
    void   CheckJump(usercmd_t& botcmd);
    void   CheckJumpOverEdge(usercmd_t& botcmd);
    void   DirectMoveThink(usercmd_t& botcmd);
    void   NewMove();
    Vector FixDeltaFromCollision(const Vector& delta);
    void   SteerTowardPathHealth(Vector& direction) const;
    void   CalculateBestFrontAvoidance(
          const Vector& targetOrg,
          float         maxDist,
          const Vector& forward,
          const Vector& right,
          float&        bestFrac,
          Vector&       bestPos
      );

private:
    SafePtr<Player>            controlledEntity;
    AttractiveNodePtr          m_pPrimaryAttract;
    Container<nodeAttract_t *> m_attractList;
    IPather                   *m_pPath;
    int                        m_iLastMoveTime;

    Vector m_vCurrentOrigin;
    Vector m_vTargetPos;
    Vector m_vCurrentGoal;
    Vector m_vCurrentDir;
    Vector m_vLastCheckPos[2];
    float  m_fAttractTime;
    int    m_iTempAwayTime;
    int    m_iNumBlocks;
    int    m_iCheckPathTime;
    int    m_iLastBlockTime;
    int    m_iTempAwayState;
    bool   m_bPathing;
    bool   m_bDirectMove;
    float  m_fDirectMoveRadius;

    ///
    /// Collision detection
    ///

    bool   m_bAvoidCollision;
    int    m_iCollisionCheckTime;
    Vector m_vTempCollisionAvoidance;
    int    m_iCollisionProgressTime;
    Vector m_vCollisionProgressOrigin;

    ///
    /// Jump detection
    ///

    bool   m_bJump;
    int    m_iJumpCheckTime;
    int    m_iJumpCommitTime;
    int    m_iJumpLandingTime;
    int    m_iJumpRetryTime;
    bool   m_bJumpWasAirborne;
    Vector m_vJumpLocation;
    Vector m_vJumpDirection;

    ///
    /// Aggressive movement (strafe + lean + enemy-relative radial movement)
    ///

    int  m_iStrafeDirection;       // -1 = left, 1 = right
    int  m_iNextStrafeChangeTime;  // When to flip strafe direction
    int  m_iRadialDirection;       // -1 = retreat, 0 = orbit, 1 = advance
    int  m_iNextRadialChangeTime;  // When to choose a new radial state
    bool m_bIsLeaning;             // Hysteresis: currently strafing
    bool m_bLeanCommandActive;     // Current usercmd contains a lean input
    bool m_bHasCombatTarget;
    bool m_bForceCombatRetreat;
    bool m_bForceCombatAdvance;

    void   UpdateAggressiveMovement(usercmd_t& botcmd);
    void   UpdateCombatRadialMovement(usercmd_t& botcmd, bool suppressMovement);
    void   PreventImminentBodyContact(usercmd_t& botcmd);
    float  CalculateLateralClearance(int direction);
    int    ChooseRadialDirection(float distance) const;
    int    RadialPhaseDuration() const;

    Vector m_vCombatTarget;
    bot_movement_telemetry_t m_telemetry;
};

class BotRotation
{
public:
    BotRotation();

    void SetControlledEntity(Player *newEntity);

    void          TurnThink(usercmd_t& botcmd, usereyes_t& eyeinfo);
    const Vector& GetTargetAngles() const;
    void          SetTargetAngles(Vector vAngles);
    void          AimAt(Vector vPos);

private:
    SafePtr<Player> controlledEntity;

    Vector m_vTargetAng;
    Vector m_vCurrentAng;
    Vector m_vAngDelta;
    Vector m_vAngSpeed;
};

class BotState
{
public:
    virtual bool CheckCondition() const = 0;
    virtual void Begin()                = 0;
    virtual void End()                  = 0;
    virtual void Think()                = 0;
};

class BotController : public Listener
{
public:
    struct botfunc_t {
        bool (BotController::*CheckCondition)(void);
        void (BotController::*BeginState)(void);
        void (BotController::*EndState)(void);
        void (BotController::*ThinkState)(void);
    };

private:
    static botfunc_t botfuncs[];

    BotMovement movement;
    BotRotation rotation;

    // States
    int    m_iCuriousTime;
    int    m_iAttackTime;
    int    m_iAttackStopAimTime;
    int    m_iLastSeenTime;
    int    m_iLastUnseenTime;
    float  m_fAimHeightFraction;
    int    m_iAimAcquireTime;
    Vector m_vAimErrorDirection;
    Vector m_vAimErrorTargetDirection;
    int    m_iNextAimErrorChangeTime;

    enum { MAX_AIM_HISTORY_SAMPLES = 256 };
    struct aim_sample_t {
        int    time;
        Vector position;
    };
    aim_sample_t m_AimHistory[MAX_AIM_HISTORY_SAMPLES];
    int          m_iAimHistoryHead;
    int          m_iAimHistoryCount;
    int          m_iPostKillAimUntil;
    Vector       m_vPostKillAimAngles;
    int          m_iLadderAimUntil;
    Vector       m_vLadderAimAngles;

    Vector                    m_vLastCuriousPos;
    Vector                    m_vNewCuriousPos;
    int                       m_iCuriousEventType;
    SafePtr<Entity>           m_pAvoidGrenade;
    Vector                    m_vAvoidGrenadePosition;
    int                       m_iGrenadeReactTime;
    int                       m_iGrenadeNextPathTime;
    bool                      m_bGrenadeFleeing;
    bool                      m_bGrenadePathFailed;
    bool                      m_bReloadRetreating;
    SafePtr<Weapon>           m_pCombatPrimaryWeapon;
    bool                      m_bTeamResponding;
    bot_contact_source_t      m_iTeamContactSource;
    int                       m_iTeamContactEnemy;
    int                       m_iTeamContactReporter;
    int                       m_iTeamContactReportTime;
    int                       m_iTeamContactExpireTime;
    int                       m_iNextTeamSearchMoveTime;
    Vector                    m_vTeamContactPos;
    bot_contact_source_t      m_iRejectedTeamContactSource;
    int                       m_iRejectedTeamContactEnemy;
    int                       m_iRejectedTeamContactUntil;
    bot_objective_state_t     m_iObjectiveState;
    bot_objective_use_phase_t m_iObjectiveUsePhase;
    int                       m_iObjectiveRound;
    int                       m_iObjectiveSite;
    int                       m_iObjectiveRouteVariant;
    int                       m_iObjectiveRouteCurrentNode;
    int                       m_iObjectiveRoutePreviousNode;
    int                       m_iObjectiveRouteNextNode;
    int                       m_iObjectiveRouteGoalNode;
    int                       m_iObjectiveRouteHop;
    int                       m_iObjectiveUseStartTime;
    int                       m_iObjectiveReapproachUntil;
    int                       m_iObjectiveNextMoveTime;
    int                       m_iObjectiveLastProgressTime;
    int                       m_iObjectiveLastStallLogTime;
    bool                      m_bObjectiveAttacker;
    bool                      m_bObjectiveHasDestination;
    bool                      m_bObjectiveOwnsMovement;
    bool                      m_bObjectiveOwnsUse;
    bool                      m_bObjectiveCritical;
    bool                      m_bObjectiveRoutePostPlant;
    bool                      m_bObjectiveRouteDisabled;
    Vector                    m_vObjectiveDestination;
    Vector                    m_vObjectiveLastProgressPos;
    Vector                    m_vIdleProgressPos;
    int                       m_iIdleProgressTime;
    Vector                    m_vOldEnemyPos;
    Vector                    m_vLastEnemyPos;
    Vector                    m_vLastDeathPos;
    SafePtr<Sentient>         m_pEnemy;
    int                       m_iEnemyEyesTag;

    // Input
    usercmd_t  m_botCmd;
    usereyes_t m_botEyes;

    // States
    int               m_StateCount;
    unsigned int      m_StateFlags;
    ScriptThreadLabel m_RunLabel;

    int m_iLastFireTime;

    // Strafe and lean (controller-level, applied on top of movement-level strafe)
    bot_controller_telemetry_t m_telemetry;

private:
    DelegateHandle delegateHandle_gotKill;
    DelegateHandle delegateHandle_killed;
    DelegateHandle delegateHandle_stufftext;
    DelegateHandle delegateHandle_spawned;

private:
    Weapon *FindWeaponWithAmmo(int requiredClass = 0);
    bool    UseCombatPistol(void);
    void    UseWeaponWithAmmo(void);

    void CheckUse(void);
    bool CheckWindows(void);
    void CheckValidWeapon(void);
    void UpdateTeamContact(void);
    void ClearTeamResponse(void);
    void RejectTeamContact(void);
    void ResetTeamContactRejection(void);
    void ResetGrenadeAvoidance(void);
    void UpdateObjectiveBehavior(void);
    void FinalizeObjectiveCommand(void);
    void ResetObjectiveBehavior(void);
    void ResetObjectiveDemoRoute(bool disable = false);
    void BeginObjectivePlan(void);
    bool UpdateObjectiveDemoRoute(
        const Vector& destination, bool postPlant, bool roam
    );
    void UpdateObjectiveAdvance(const Vector& sitePosition);
    void UpdateObjectiveUse(bool planting);
    bool TryStartObjectiveUse(bool planting, int site);
    void SetObjectiveDestination(
        const Vector& destination, bot_objective_state_t state, float radius = 0.0f
    );
    void UpdateObjectivePatrol(
        const Vector& center, const Vector& toward, float radius, bot_objective_state_t state
    );
    void UpdateObjectiveProgress(void);

    void State_DefaultBegin(void);
    void State_DefaultEnd(void);
    void State_Reset(void);

    static void InitState_Idle(botfunc_t *func);
    bool        CheckCondition_Idle(void);
    void        State_BeginIdle(void);
    void        State_EndIdle(void);
    void        State_Idle(void);

    static void InitState_Curious(botfunc_t *func);
    bool        CheckCondition_Curious(void);
    void        State_BeginCurious(void);
    void        State_EndCurious(void);
    void        State_Curious(void);

    static void InitState_Attack(botfunc_t *func);
    bool        CheckCondition_Attack(void);
    void        State_BeginAttack(void);
    void        State_EndAttack(void);
    void        State_Attack(void);
    bool        IsValidEnemy(Sentient *sent) const;
    bool        IsEngagedByAnotherBot(Sentient *enemy) const;
    bool        CanSeeEnemyPoint(Sentient *enemy, const Vector& point);
    bool        IsEnemyWithinVision(Sentient *enemy) const;
    bool        IsEnemyPartVisible(Sentient *enemy);
    bool        CheckEnemyVisibility(Sentient *enemy, float desiredAimFraction, float& aimFraction);
    void        BeginAimAcquisition(void);
    void        UpdateAimErrorDirection(void);
    Vector      GetDelayedAimTarget(const Vector& currentTarget);

    static void InitState_Grenade(botfunc_t *func);
    bool        CheckCondition_Grenade(void);
    void        State_BeginGrenade(void);
    void        State_EndGrenade(void);
    void        State_Grenade(void);

    static void InitState_Weapon(botfunc_t *func);
    bool        CheckCondition_Weapon(void);
    void        State_BeginWeapon(void);
    void        State_EndWeapon(void);
    void        State_Weapon(void);

    void CheckStates(void);

public:
    CLASS_PROTOTYPE(BotController);

    BotController();
    ~BotController();

    static void Init(void);

    void GetEyeInfo(usereyes_t *eyeinfo);
    void GetUsercmd(usercmd_t *ucmd);

    void UpdateBotStates(void);
    void CheckReload(void);

    void AimAtAimNode(void);

    void NoticeEvent(Vector vPos, int iType, Entity *pEnt, float fDistanceSquared, float fRadiusSquared);
    void ClearEnemy(void);

    void SendCommand(const char *text);

    void Think();

    void Spawned(void);

    void Killed(const Event& ev);
    void GotKill(const Event& ev);
    void EventStuffText(const str& text);

    BotMovement& GetMovement();
    void GetTelemetry(bot_controller_telemetry_t& telemetry) const;
    bool CanRespondToTeamContact(void) const;
    bool CanRespondToTeamContact(const Vector& position) const;
    bool IsRespondingToTeamContact(int enemyNum) const;
    bool IsTeamContactRejected(int enemyNum, bot_contact_source_t source) const;
    bool CanInvestigatePosition(const Vector& position) const;

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

    void Init();
    void Cleanup();
    void ThinkControllers();

private:
    Container<BotController *> controllers;
};

class BotManager : public Listener
{
    enum { MAX_BOT_OBJECTIVE_SITES = 16 };

public:
    CLASS_PROTOTYPE(BotManager);

public:
    BotControllerManager& getControllerManager();

    void Init();
    void Cleanup();
    void Frame();
    void BroadcastEvent(Entity *originator, Vector origin, int iType, float radius);
    void ReportTeamContact(
        Player *reporter, Player *enemy, bot_contact_source_t source, const Vector& position, float uncertainty
    );
    bool FindTeamContact(BotController *controller, bot_team_contact_t& result) const;
    bool ObjectiveModeActive() const;
    int  GetObjectiveRound() const;
    int  GetObjectiveSeed() const;
    int  GetObjectiveBotRank(Player *player) const;
    int  GetObjectiveSiteCount();
    bot_objective_site_state_t GetObjectiveSiteState(int site);
    Vector                     GetObjectiveSitePosition(int site) const;
    Entity                    *GetObjectiveSiteExplosive(int site) const;
    Entity                    *GetObjectiveSiteTrigger(int site) const;
    bool                       ClaimObjectiveSite(int site, Player *player);
    void                       ReleaseObjectiveClaim(Player *player);
    teamtype_t                 GetObjectivePlantTeam() const;
    Vector                     GetObjectiveEnemySpawnCenter(teamtype_t team) const;

private:
    static int TeamContactIndex(teamtype_t team);
    void       ClearTeamContacts();
    void       UpdateObjectiveRound();
    void       ClearObjectiveSites();
    void       DiscoverObjectiveSites();

    BotControllerManager botControllerManager;
    bot_team_contact_t    teamContacts[2][MAX_CLIENTS];
    bot_objective_site_t  objectiveSites[MAX_BOT_OBJECTIVE_SITES];
    int                   objectiveSiteCount;
    int                   objectiveRound;
    int                   objectiveSeed;
    int                   nextObjectiveScanTime;
    bool                  objectiveSitesScanned;
    bool                  objectiveRoundActive;
};

extern BotManager botManager;
