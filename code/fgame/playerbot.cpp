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
// playerbot.cpp: Multiplayer bot system.
//
// FIXME: Refactor code and use OOP-based state system

#include "g_local.h"
#include "actor.h"
#include "playerbot.h"
#include "consoleevent.h"
#include "debuglines.h"
#include "scriptexception.h"
#include "vehicleturret.h"
#include "weaputils.h"
#include "windows.h"
#include "g_bot.h"
#include "movement_telemetry.h"

// We assume that we have limited access to the server-side
// and that most logic come from the playerstate_s structure

CLASS_DECLARATION(Listener, BotController, NULL) {
    {NULL, NULL}
};

BotController::botfunc_t BotController::botfuncs[MAX_BOT_FUNCTIONS];

// Human reference shots retained substantial tracking error after target
// acquisition. Keeping 60% of the configured error prevents the bot from
// settling into near-perfect lock while preserving the difficulty presets.
static const float BOT_AIM_RESIDUAL_FRACTION = 0.60f;

static const float BOT_MAX_VISION_DISTANCE = 4096.0f;

static const float BOT_GRENADE_SAFE_DISTANCE      = 384.0f;
static const int   BOT_GRENADE_REPATH_MSEC        = 250;
static const float BOT_GRENADE_DIRECT_ESCAPE_STEP = 192.0f;
static const float BOT_RELOAD_SAFE_DISTANCE       = 384.0f;
static const int   BOT_IDLE_PROGRESS_MSEC          = 10000;
static const float BOT_IDLE_PROGRESS_UNITS         = 512.0f;
static const int   BOT_POST_KILL_AIM_MSEC          = 150;
static const int   BOT_LOS_AIM_HOLD_MSEC           = 300;
static const int   BOT_LADDER_AIM_HOLD_MSEC        = 750;
static const float BOT_LADDER_AIM_DISTANCE         = 96.0f;

// Body heights sampled when checking whether an enemy is partially visible,
// as fractions of the bounding-box height, ordered top-down. A single
// eye-to-eye trace declares an enemy invisible whenever anything clips that
// one line - a railing bar, a ramp edge, the top of a cover wall - even
// though most of the body is exposed and shootable. Cover hides the low
// samples first; railings and ramp edges typically leave gaps around the
// middle ones.
static const float BOT_VISIBILITY_SAMPLES[]    = {0.9f, 0.7f, 0.5f, 0.3f, 0.1f};
static const int   BOT_NUM_VISIBILITY_SAMPLES  = ARRAY_LEN(BOT_VISIBILITY_SAMPLES);

bot_team_contact_t::bot_team_contact_t()
{
    Clear();
}

void bot_team_contact_t::Clear()
{
    valid         = false;
    source        = BOT_CONTACT_NONE;
    reportTime    = 0;
    availableTime = 0;
    expireTime    = 0;
    lastLogTime   = 0;
    position      = vec_zero;
    reporter      = NULL;
    enemy         = NULL;
}

bot_controller_telemetry_t::bot_controller_telemetry_t()
{
    Reset();
}

void bot_controller_telemetry_t::Reset()
{
    stateFlags            = 0;
    enemyEntity           = -1;
    enemyVisible          = false;
    canAttack             = false;
    wantsFire             = false;
    noMove                = false;
    fireDecision          = BOT_FIRE_NONE;
    reactionRemainingMsec = 0;
    enemyDistance        = -1.0f;
    aimAcquireMsec       = -1;
    aimHeightFraction    = 0.0f;
    aimErrorFraction     = 0.0f;
    aimErrorUnits        = 0.0f;
    aimLatencyMsec       = 0;
    aimTarget            = vec_zero;
    aimPoint             = vec_zero;
    aimErrorDirection    = vec_zero;
    targetAngles         = vec_zero;
    contactSource        = BOT_CONTACT_NONE;
    contactEnemy         = -1;
    contactReporter      = -1;
    contactAgeMsec       = -1;
    contactResponder     = false;
    contactPosition      = vec_zero;
    objectiveState       = BOT_OBJECTIVE_NONE;
    objectiveSite        = -1;
    objectiveRound       = -1;
    objectiveUsePhase    = BOT_OBJECTIVE_USE_AIM;
    objectiveAttacker    = false;
    objectiveCritical    = false;
    objectivePosition    = vec_zero;
}

static int BotSoundPriority(int eventType)
{
    switch (eventType) {
    case AI_EVENT_WEAPON_FIRE:
        return 7;
    case AI_EVENT_EXPLOSION:
        return 6;
    case AI_EVENT_WEAPON_IMPACT:
        return 5;
    case AI_EVENT_AMERICAN_URGENT:
    case AI_EVENT_GERMAN_URGENT:
        return 4;
    case AI_EVENT_AMERICAN_VOICE:
    case AI_EVENT_GERMAN_VOICE:
        return 3;
    case AI_EVENT_MISC_LOUD:
    case AI_EVENT_FOOTSTEP:
        return 2;
    case AI_EVENT_MISC:
        return 1;
    default:
        return 0;
    }
}

static int BotSoundInterestDuration(int eventType)
{
    switch (eventType) {
    case AI_EVENT_WEAPON_FIRE:
    case AI_EVENT_EXPLOSION:
        return 10000;
    case AI_EVENT_WEAPON_IMPACT:
        return 7000;
    case AI_EVENT_AMERICAN_URGENT:
    case AI_EVENT_GERMAN_URGENT:
        return 6000;
    case AI_EVENT_AMERICAN_VOICE:
    case AI_EVENT_GERMAN_VOICE:
    case AI_EVENT_MISC_LOUD:
    case AI_EVENT_FOOTSTEP:
        return 4000;
    default:
        return 2500;
    }
}

static Vector RandomBotAimErrorDirection()
{
    // Keep persistent error away from the head and neck: horizontal error is
    // symmetric, while its smaller vertical component can only pull down.
    Vector direction(G_CRandom(1.0f), G_CRandom(1.0f), 0);
    if (direction.length() < 0.01f) {
        direction = Vector(1, 0, 0);
    }
    direction.normalize();
    direction.z = -G_Random(0.35f);
    direction.normalize();
    return direction;
}

static int RandomBotAimErrorInterval()
{
    return 1000 + (int)G_Random(1000);
}

static int BotGrenadeReactionDelay()
{
    const float difficulty =
        g_bot_difficulty && g_bot_difficulty->integer >= 0
        ? Q_clamp_float(g_bot_difficulty->value, 0.0f, 100.0f)
        : 50.0f;
    return 350 - static_cast<int>(difficulty * 2.0f);
}

BotController::BotController()
{
    if (LoadingSavegame) {
        return;
    }

    m_botCmd.serverTime = 0;
    m_botCmd.msec       = 0;
    m_botCmd.buttons    = 0;
    m_botCmd.angles[0]  = ANGLE2SHORT(0);
    m_botCmd.angles[1]  = ANGLE2SHORT(0);
    m_botCmd.angles[2]  = ANGLE2SHORT(0);

    m_botCmd.forwardmove = 0;
    m_botCmd.rightmove   = 0;
    m_botCmd.upmove      = 0;

    m_botEyes.angles[0] = 0;
    m_botEyes.angles[1] = 0;
    m_botEyes.ofs[0]    = 0;
    m_botEyes.ofs[1]    = 0;
    m_botEyes.ofs[2]    = DEFAULT_VIEWHEIGHT;

    m_iCuriousTime              = 0;
    m_iAttackTime               = 0;
    m_iAttackStopAimTime        = 0;
    m_iEnemyEyesTag             = -1;
    m_iLastSeenTime             = 0;
    m_iLastUnseenTime           = 0;
    m_iAimAcquireTime           = -1;
    m_fAimHeightFraction        = 0.57f;
    m_vAimErrorDirection        = vec_zero;
    m_vAimErrorTargetDirection = vec_zero;
    m_iNextAimErrorChangeTime   = 0;
    m_iAimHistoryHead           = 0;
    m_iAimHistoryCount          = 0;
    m_iPostKillAimUntil         = 0;
    m_vPostKillAimAngles        = vec_zero;
    m_iLadderAimUntil           = 0;
    m_vLadderAimAngles          = vec_zero;
    m_iCuriousEventType         = AI_EVENT_NONE;
    ResetGrenadeAvoidance();
    m_bReloadRetreating         = false;
    m_pCombatPrimaryWeapon      = NULL;
    m_bTeamResponding           = false;
    m_iTeamContactSource        = BOT_CONTACT_NONE;
    m_iTeamContactEnemy         = -1;
    m_iTeamContactReporter      = -1;
    m_iTeamContactReportTime    = 0;
    m_iTeamContactExpireTime    = 0;
    m_iNextTeamSearchMoveTime   = 0;
    m_vTeamContactPos           = vec_zero;
    m_iObjectiveState            = BOT_OBJECTIVE_NONE;
    m_iObjectiveUsePhase         = BOT_OBJECTIVE_USE_AIM;
    m_iObjectiveRound            = -1;
    m_iObjectiveSite             = -1;
    m_iObjectiveRouteVariant     = 0;
    m_iObjectiveUseStartTime     = 0;
    m_iObjectiveReapproachUntil  = 0;
    m_iObjectiveNextMoveTime     = 0;
    m_iObjectiveLastProgressTime = 0;
    m_iObjectiveLastStallLogTime = 0;
    m_bObjectiveAttacker         = false;
    m_bObjectiveHasDestination   = false;
    m_bObjectiveOwnsMovement     = false;
    m_bObjectiveOwnsUse          = false;
    m_bObjectiveCritical             = false;
    m_bObjectiveRouteActive          = false;
    m_vObjectiveDestination          = vec_zero;
    m_vObjectiveLastProgressPos      = vec_zero;
    m_vIdleProgressPos               = vec_zero;
    m_iIdleProgressTime              = 0;

    m_StateFlags = 0;
}

BotController::~BotController()
{
    botManager.ReleaseObjectiveClaim(controlledEnt);

    if (controlledEnt) {
        controlledEnt->delegate_gotKill.Remove(delegateHandle_gotKill);
        controlledEnt->delegate_killed.Remove(delegateHandle_killed);
        controlledEnt->delegate_stufftext.Remove(delegateHandle_stufftext);
        controlledEnt->delegate_spawned.Remove(delegateHandle_spawned);
    }
}

BotMovement& BotController::GetMovement()
{
    return movement;
}

void BotController::GetTelemetry(bot_controller_telemetry_t& telemetry) const
{
    telemetry                  = m_telemetry;
    telemetry.stateFlags       = m_StateFlags;
    telemetry.enemyEntity      = m_pEnemy ? m_pEnemy->entnum : -1;
    telemetry.aimAcquireMsec   = m_iAimAcquireTime >= 0 ? Q_max(0, level.inttime - m_iAimAcquireTime) : -1;
    telemetry.aimHeightFraction = m_fAimHeightFraction;
    telemetry.aimLatencyMsec    = g_bot_aim_latency ? g_bot_aim_latency->integer : 0;
    telemetry.targetAngles      = rotation.GetTargetAngles();
    telemetry.contactSource     = m_iTeamContactSource;
    telemetry.contactEnemy      = m_iTeamContactEnemy;
    telemetry.contactReporter   = m_iTeamContactReporter;
    telemetry.contactAgeMsec =
        m_bTeamResponding ? Q_max(0, level.inttime - m_iTeamContactReportTime) : -1;
    telemetry.contactResponder = m_bTeamResponding;
    telemetry.contactPosition  = m_vTeamContactPos;
    telemetry.objectiveState    = m_iObjectiveState;
    telemetry.objectiveSite     = m_iObjectiveSite;
    telemetry.objectiveRound    = m_iObjectiveRound;
    telemetry.objectiveUsePhase = m_iObjectiveUsePhase;
    telemetry.objectiveAttacker = m_bObjectiveAttacker;
    telemetry.objectiveCritical = m_bObjectiveCritical;
    telemetry.objectivePosition = m_vObjectiveDestination;
}

void BotController::Init(void)
{
    for (int i = 0; i < MAX_BOT_FUNCTIONS; i++) {
        botfuncs[i].BeginState = &BotController::State_DefaultBegin;
        botfuncs[i].EndState   = &BotController::State_DefaultEnd;
    }

    InitState_Attack(&botfuncs[0]);
    InitState_Curious(&botfuncs[1]);
    InitState_Grenade(&botfuncs[2]);
    InitState_Idle(&botfuncs[3]);
    //InitState_Weapon(&botfuncs[4]);
}

void BotController::GetUsercmd(usercmd_t *ucmd)
{
    *ucmd = m_botCmd;
}

void BotController::GetEyeInfo(usereyes_t *eyeinfo)
{
    *eyeinfo = m_botEyes;
}

void BotController::UpdateBotStates(void)
{
    m_telemetry.Reset();
    movement.ResetTelemetry();
    m_botCmd.serverTime = level.svsTime;

    if (g_bot_manualmove->integer) {
        m_botCmd.buttons = 0;
        m_botCmd.forwardmove = m_botCmd.rightmove = m_botCmd.upmove = 0;
        return;
    }

    if (!controlledEnt->client->pers.dm_primary[0]) {
        Event *event;

        //
        // Primary weapon
        //
        event = new Event(EV_Player_PrimaryDMWeapon);
        event->AddString("auto");

        controlledEnt->ProcessEvent(event);
    }

    if (controlledEnt->GetTeam() == TEAM_NONE || controlledEnt->GetTeam() == TEAM_SPECTATOR) {
        float time;

        // Add some delay to avoid telefragging
        time = controlledEnt->entnum / 20.0;

        if (controlledEnt->EventPending(EV_Player_AutoJoinDMTeam)) {
            return;
        }

        //
        // Team
        //
        controlledEnt->PostEvent(EV_Player_AutoJoinDMTeam, time);
        return;
    }

    if (controlledEnt->IsDead() || controlledEnt->IsSpectator()) {
        // The bot should respawn
        m_botCmd.buttons ^= BUTTON_ATTACKLEFT;
        return;
    }

    m_botCmd.buttons |= BUTTON_RUN;

    m_botEyes.ofs[0]    = 0;
    m_botEyes.ofs[1]    = 0;
    m_botEyes.ofs[2]    = controlledEnt->viewheight;
    m_botEyes.angles[0] = 0;
    m_botEyes.angles[1] = 0;

    UpdateTeamContact();
    CheckStates();
    UpdateObjectiveBehavior();

    movement.MoveThink(m_botCmd);
    FinalizeObjectiveCommand();

    if (m_iPostKillAimUntil) {
        if ((m_pEnemy && IsValidEnemy(m_pEnemy)) || level.inttime >= m_iPostKillAimUntil) {
            m_iPostKillAimUntil = 0;
        } else {
            rotation.SetTargetAngles(m_vPostKillAimAngles);
        }
    }

    rotation.TurnThink(m_botCmd, m_botEyes);
    CheckUse();

    CheckValidWeapon();
}

void BotController::CheckUse(void)
{
    Vector  dir;
    Vector  start;
    Vector  end;
    trace_t trace;

    if (m_bObjectiveOwnsUse || controlledEnt->GetLadder()) {
        return;
    }

    controlledEnt->angles.AngleVectorsLeft(&dir);

    start = controlledEnt->origin + Vector(0, 0, controlledEnt->viewheight);
    end   = controlledEnt->origin + Vector(0, 0, controlledEnt->viewheight) + dir * 64;

    trace = G_Trace(
        start, vec_zero, vec_zero, end, controlledEnt, MASK_USABLE | MASK_LADDER, false, "BotController::CheckUse"
    );

    if (!trace.ent || trace.ent->entity == world) {
        m_botCmd.buttons &= ~BUTTON_USE;
        return;
    }

    if (trace.ent->entity->IsSubclassOfDoor()) {
        Door *door = static_cast<Door *>(trace.ent->entity);
        if (door->isOpen()) {
            // Don't use an open door
            m_botCmd.buttons &= ~BUTTON_USE;
            return;
        }
    } else if (!trace.ent->entity->isSubclassOf(FuncLadder)) {
        m_botCmd.buttons &= ~BUTTON_USE;
        return;
    }

    //
    // Toggle the use button
    //
    m_botCmd.buttons ^= BUTTON_USE;

#if 0
    Vector  forward;
    Vector  start, end;

    AngleVectors(controlledEnt->GetViewAngles(), forward, NULL, NULL);

    start = (controlledEnt->m_vViewPos - forward * 12.0f);
    end   = (controlledEnt->m_vViewPos + forward * 128.0f);

    trace = G_Trace(start, vec_zero, vec_zero, end, controlledEnt, MASK_LADDER, qfalse, "checkladder");
    if (trace.ent->entity && trace.ent->entity->isSubclassOf(FuncLadder)) {
        return;
    }

    m_botCmd.buttons ^= BUTTON_USE;
#endif
}

bool BotController::CheckWindows(void)
{
    trace_t trace;
    Vector  start, end;
    Vector  dir;

    controlledEnt->angles.AngleVectorsLeft(&dir);
    start = controlledEnt->origin + Vector(0, 0, controlledEnt->viewheight);
    end   = controlledEnt->origin + Vector(0, 0, controlledEnt->viewheight) + dir * 64;

    trace = G_Trace(start, vec_zero, vec_zero, end, controlledEnt, MASK_PLAYERSOLID, false, "BotController::CheckUse");

    if (trace.fraction != 1 && trace.ent) {
        if (trace.ent->entity->isSubclassOf(WindowObject)) {
            return true;
        }
    }

    return false;
}

void BotController::CheckValidWeapon()
{
    Weapon *weapon  = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    Weapon *pending = controlledEnt->GetNewActiveWeapon();

    if (m_pCombatPrimaryWeapon) {
        if (weapon == m_pCombatPrimaryWeapon) {
            m_pCombatPrimaryWeapon = NULL;
        } else if (!m_pCombatPrimaryWeapon->HasAmmo(FIRE_PRIMARY)) {
            m_pCombatPrimaryWeapon = NULL;
        } else if (!pending && (!m_pEnemy || !IsValidEnemy(m_pEnemy))) {
            Weapon *primary = m_pCombatPrimaryWeapon;
            controlledEnt->useWeapon(primary, WEAPON_MAIN);
            G_MoveLogBotEvent("bot_reload_primary", controlledEnt, NULL, primary->entnum, controlledEnt->origin);
            return;
        }
    }

    // Do not replace a weapon switch while the current weapon is lowering.
    if ((!weapon || !weapon->HasAmmo(FIRE_PRIMARY)) && !pending) {
        UseWeaponWithAmmo();
    }
}

void BotController::SendCommand(const char *text)
{
    char        *buffer;
    char        *data;
    size_t       len;
    ConsoleEvent ev;

    len = strlen(text) + 1;

    buffer = (char *)gi.Malloc(len);
    data   = buffer;
    Q_strncpyz(data, text, len);

    const char *com_token = COM_Parse(&data);

    if (!com_token) {
        return;
    }

    controlledEnt->m_lastcommand = com_token;

    if (!Event::GetEvent(com_token)) {
        return;
    }

    ev = ConsoleEvent(com_token);

    if (!(ev.GetEventFlags(ev.eventnum) & EV_CONSOLE)) {
        gi.Free(buffer);
        return;
    }

    ev.SetConsoleEdict(controlledEnt->edict);

    while (1) {
        com_token = COM_Parse(&data);

        if (!com_token || !*com_token) {
            break;
        }

        ev.AddString(com_token);
    }

    gi.Free(buffer);

    try {
        controlledEnt->ProcessEvent(ev);
    } catch (ScriptException& exc) {
        gi.DPrintf("*** Bot Command Exception *** %s\n", exc.string.c_str());
    }
}

/*
====================
AimAtAimNode

Make the bot face toward the current path
====================
*/
void BotController::AimAtAimNode(void)
{
    if (!movement.IsMoving()) {
        m_iLadderAimUntil = 0;
        return;
    }

    if (controlledEnt->GetLadder()) {
        m_iLadderAimUntil = 0;
        Vector vAngles = movement.GetCurrentMoveDirection().toAngles();
        vAngles.x      = Q_clamp_float(vAngles.x, -80, 80);

        rotation.SetTargetAngles(vAngles);
        return;
    }

    // The path direction can change sharply while collision steering aligns a
    // bot with a ladder entrance. Once the ladder is detected ahead, face its
    // fixed surface point until attachment instead of following those
    // short-lived steering corrections.
    Vector moveDirection = movement.GetCurrentMoveDirection();
    moveDirection.z      = 0.0f;
    if (VectorNormalize2D(moveDirection) > 0.0f) {
        const Vector start =
            controlledEnt->origin + Vector(0, 0, controlledEnt->viewheight);
        const Vector end = start + moveDirection * BOT_LADDER_AIM_DISTANCE;
        trace_t trace     = G_Trace(
            start,
            vec_zero,
            vec_zero,
            end,
            controlledEnt,
            MASK_LADDER,
            false,
            "BotController::AimAtAimNode"
        );

        if (trace.ent && trace.ent->entity
            && trace.ent->entity->isSubclassOf(FuncLadder)) {
            Vector ladderDirection = trace.endpos - controlledEnt->origin;
            ladderDirection.z      = 0.0f;
            if (VectorNormalize2D(ladderDirection) > 0.0f) {
                m_vLadderAimAngles   = ladderDirection.toAngles();
                m_vLadderAimAngles.x = 0.0f;
                m_iLadderAimUntil    =
                    level.inttime + BOT_LADDER_AIM_HOLD_MSEC;
            }
        }
    }

    if (level.inttime < m_iLadderAimUntil) {
        rotation.SetTargetAngles(m_vLadderAimAngles);
        return;
    }
    m_iLadderAimUntil = 0;

    Vector targetAngles = movement.GetCurrentMoveDirection().toAngles();
    targetAngles.x      = 0;
    rotation.SetTargetAngles(targetAngles);
}

/*
====================
CheckReload

Make the bot reload if necessary
====================
*/
void BotController::CheckReload(void)
{
    Weapon *weap;

    if (level.inttime < m_iLastFireTime + 2000) {
        // Don't reload while attacking
        return;
    }

    weap = controlledEnt->GetActiveWeapon(WEAPON_MAIN);

    if (weap && weap->CheckReload(FIRE_PRIMARY)) {
        SendCommand("reload");
    }
}

/*
====================
NoticeEvent

Warn the bot of an event
====================
*/
void BotController::NoticeEvent(Vector vPos, int iType, Entity *pEnt, float fDistanceSquared, float fRadiusSquared)
{
    Sentient *pSentOwner = NULL;
    float     fRangeFactor;
    Vector    delta1, delta2;

    if (iType == AI_EVENT_GRENADE) {
        // Grenades are hazards, not curiosity targets. The broadcast already
        // limits this to connected areas; retain a short human hearing range
        // and track the live projectile after it is noticed.
        if (!g_bot_grenade_avoid || !g_bot_grenade_avoid->integer || !pEnt
            || !pEnt->IsSubclassOfProjectile()
            || fDistanceSquared > Square(BOT_GRENADE_SAFE_DISTANCE)) {
            return;
        }

        if (m_pAvoidGrenade == pEnt) {
            m_vAvoidGrenadePosition = pEnt->origin;
            return;
        }

        if (m_pAvoidGrenade) {
            const float currentDistance =
                (m_pAvoidGrenade->origin - controlledEnt->origin).lengthSquared();
            if (currentDistance <= fDistanceSquared) {
                return;
            }
        }

        const bool alreadyFleeing = m_bGrenadeFleeing;
        m_pAvoidGrenade           = pEnt;
        m_vAvoidGrenadePosition   = pEnt->origin;
        m_iGrenadeReactTime =
            alreadyFleeing ? level.inttime : level.inttime + BotGrenadeReactionDelay();
        m_iGrenadeNextPathTime = 0;
        m_bGrenadePathFailed   = false;

        G_MoveLogBotEvent(
            "bot_grenade_notice",
            controlledEnt,
            NULL,
            pEnt->entnum,
            m_vAvoidGrenadePosition
        );
        return;
    }

    if (pEnt->IsSubclassOfSentient()) {
        pSentOwner = static_cast<Sentient *>(pEnt);
    } else if (pEnt->IsSubclassOfVehicleTurretGun()) {
        VehicleTurretGun *pVTG = static_cast<VehicleTurretGun *>(pEnt);
        pSentOwner             = pVTG->GetSentientOwner();
    } else if (pEnt->IsSubclassOfItem()) {
        Item *pItem = static_cast<Item *>(pEnt);
        pSentOwner  = pItem->GetOwner();
    } else if (pEnt->IsSubclassOfProjectile()) {
        Projectile *pProj = static_cast<Projectile *>(pEnt);
        pSentOwner        = pProj->GetOwner();
    }

    if (pSentOwner) {
        if (pSentOwner == controlledEnt) {
            return;
        }

        if ((pSentOwner->flags & FL_NOTARGET) || pSentOwner->getSolidType() == SOLID_NOT) {
            return;
        }

        if (pSentOwner->IsSubclassOfPlayer()) {
            Player *player = static_cast<Player *>(pSentOwner);

            if (g_gametype->integer >= GT_TEAM && player->GetTeam() == controlledEnt->GetTeam()) {
                return;
            }

            if (g_gametype->integer >= GT_TEAM && iType == AI_EVENT_WEAPON_FIRE) {
                fRangeFactor = Q_clamp_float(1.0f - (fDistanceSquared / fRadiusSquared), 0.0f, 1.0f);
                const float uncertainty = 128.0f + (1.0f - fRangeFactor) * 256.0f;
                botManager.ReportTeamContact(
                    controlledEnt, player, BOT_CONTACT_SOUND, vPos, uncertainty
                );
                return;
            }
        }
    }

    if (m_iCuriousTime > level.inttime) {
        delta1 = vPos - controlledEnt->origin;
        delta2 = m_vNewCuriousPos - controlledEnt->origin;

        const int newPriority     = BotSoundPriority(iType);
        const int currentPriority = BotSoundPriority(m_iCuriousEventType);

        // A more important sound always wins. At equal priority, keep the
        // closer one instead of abandoning it for a farther event.
        if (newPriority < currentPriority
            || (newPriority == currentPriority && delta1.lengthSquared() >= delta2.lengthSquared())) {
            return;
        }
    }

    fRangeFactor = 1.0 - (fDistanceSquared / fRadiusSquared);

    if (iType != AI_EVENT_WEAPON_FIRE && fRangeFactor < random()) {
        return;
    }

    m_iCuriousEventType = iType;
    m_iCuriousTime      = level.inttime + BotSoundInterestDuration(iType);
    m_vNewCuriousPos    = vPos;
}

void BotController::ClearTeamResponse(void)
{
    m_bTeamResponding         = false;
    m_iTeamContactSource      = BOT_CONTACT_NONE;
    m_iTeamContactEnemy       = -1;
    m_iTeamContactReporter    = -1;
    m_iTeamContactReportTime  = 0;
    m_iTeamContactExpireTime  = 0;
    m_iNextTeamSearchMoveTime = 0;
    m_vTeamContactPos         = vec_zero;
}

void BotController::ResetGrenadeAvoidance(void)
{
    m_pAvoidGrenade         = NULL;
    m_vAvoidGrenadePosition = vec_zero;
    m_iGrenadeReactTime     = 0;
    m_iGrenadeNextPathTime  = 0;
    m_bGrenadeFleeing       = false;
    m_bGrenadePathFailed    = false;
}

void BotController::UpdateTeamContact(void)
{
    bot_team_contact_t contact;

    if (g_gametype->integer < GT_TEAM || m_iAttackTime || !botManager.FindTeamContact(this, contact)) {
        if (m_bTeamResponding) {
            m_iCuriousTime      = 0;
            m_iCuriousEventType = AI_EVENT_NONE;
            movement.ClearMove();
            ClearTeamResponse();
        }
        return;
    }

    const bool newResponse =
        !m_bTeamResponding || m_iTeamContactEnemy != contact.enemy->entnum
        || m_iTeamContactSource != contact.source;

    m_bTeamResponding        = true;
    m_iTeamContactSource     = contact.source;
    m_iTeamContactEnemy      = contact.enemy->entnum;
    m_iTeamContactReporter   = contact.reporter ? contact.reporter->entnum : -1;
    m_iTeamContactReportTime = contact.reportTime;
    m_iTeamContactExpireTime = contact.expireTime;
    m_vTeamContactPos        = contact.position;

    if (newResponse) {
        m_iNextTeamSearchMoveTime = 0;
        G_MoveLogBotEvent(
            "bot_contact_response",
            controlledEnt,
            contact.enemy,
            static_cast<int>(contact.source),
            contact.position
        );
    }

    m_iCuriousEventType = contact.source == BOT_CONTACT_SOUND ? AI_EVENT_WEAPON_FIRE : AI_EVENT_MISC_LOUD;
    m_iCuriousTime      = contact.expireTime;
    m_vNewCuriousPos    = contact.position;
}

bool BotController::CanRespondToTeamContact(void) const
{
    return controlledEnt && !controlledEnt->IsDead() && !controlledEnt->IsSpectator() && !m_iAttackTime
        && !m_bObjectiveCritical && !m_bObjectiveOwnsUse;
}

bool BotController::CanRespondToTeamContact(const Vector& position) const
{
    if (!CanRespondToTeamContact()) {
        return false;
    }

    return CanInvestigatePosition(position);
}

bool BotController::CanInvestigatePosition(const Vector& position) const
{
    if (!botManager.ObjectiveModeActive() || !m_bObjectiveAttacker
        || m_iObjectiveState != BOT_OBJECTIVE_ADVANCE || !m_bObjectiveHasDestination) {
        return true;
    }

    Vector objectiveDelta = m_vObjectiveDestination - controlledEnt->origin;
    objectiveDelta.z      = 0.0f;
    if (objectiveDelta.lengthSquared() <= Square(512.0f)) {
        return false;
    }

    Vector investigationDelta = position - controlledEnt->origin;
    investigationDelta.z      = 0.0f;
    if (DotProduct(investigationDelta, objectiveDelta) <= 0.0f) {
        return false;
    }

    Vector contactDelta = m_vObjectiveDestination - position;
    contactDelta.z      = 0.0f;

    // Attackers still react to callouts that help clear the route, but they
    // do not surrender movement to a sound behind them or to a report that
    // leaves them farther from the objective.
    return contactDelta.lengthSquared() <= objectiveDelta.lengthSquared();
}

bool BotController::IsRespondingToTeamContact(int enemyNum) const
{
    return m_bTeamResponding && m_iTeamContactEnemy == enemyNum;
}

/*
====================
ClearEnemy

Clear the bot's enemy
====================
*/
void BotController::ClearEnemy(void)
{
    m_iAttackTime               = 0;
    m_iAttackStopAimTime        = 0;
    m_iLadderAimUntil           = 0;
    m_vLadderAimAngles          = vec_zero;
    m_iAimAcquireTime           = -1;
    m_iAimHistoryHead           = 0;
    m_iAimHistoryCount          = 0;
    m_vAimErrorDirection        = vec_zero;
    m_vAimErrorTargetDirection = vec_zero;
    m_iNextAimErrorChangeTime   = 0;
    m_pEnemy                    = NULL;
    m_iEnemyEyesTag             = -1;
    m_vOldEnemyPos              = vec_zero;
    m_vLastEnemyPos             = vec_zero;
    movement.ClearCombatTarget();
}

/*
====================
Bot states
--------------------
____________________
--------------------
____________________
--------------------
____________________
--------------------
____________________
====================
*/

void BotController::CheckStates(void)
{
    m_StateCount = 0;

    for (int i = 0; i < MAX_BOT_FUNCTIONS; i++) {
        botfunc_t *func = &botfuncs[i];

        if (func->CheckCondition) {
            if ((this->*func->CheckCondition)()) {
                if (!(m_StateFlags & (1 << i))) {
                    m_StateFlags |= 1 << i;

                    if (func->BeginState) {
                        (this->*func->BeginState)();
                    }
                }

                if (func->ThinkState) {
                    m_StateCount++;
                    (this->*func->ThinkState)();
                }
            } else {
                if ((m_StateFlags & (1 << i))) {
                    m_StateFlags &= ~(1 << i);

                    if (func->EndState) {
                        (this->*func->EndState)();
                    }
                }
            }
        } else {
            if (func->ThinkState) {
                m_StateCount++;
                (this->*func->ThinkState)();
            }
        }
    }

    assert(m_StateCount);
    if (!m_StateCount) {
        gi.DPrintf("*** WARNING *** %s was stuck with no states !!!", controlledEnt->client->pers.netname);
        State_Reset();
    }
}

/*
====================
Default state


====================
*/
void BotController::State_DefaultBegin(void)
{
    movement.ClearMove();
}

void BotController::State_DefaultEnd(void) {}

void BotController::State_Reset(void)
{
    m_iCuriousTime              = 0;
    m_iAttackTime               = 0;
    m_iAttackStopAimTime        = 0;
    m_iAimAcquireTime           = -1;
    m_iAimHistoryHead           = 0;
    m_iAimHistoryCount          = 0;
    m_iPostKillAimUntil         = 0;
    m_vPostKillAimAngles        = vec_zero;
    m_iLadderAimUntil           = 0;
    m_vLadderAimAngles          = vec_zero;
    m_vAimErrorDirection        = vec_zero;
    m_vAimErrorTargetDirection = vec_zero;
    m_iNextAimErrorChangeTime   = 0;
    m_vLastCuriousPos           = vec_zero;
    m_iCuriousEventType         = AI_EVENT_NONE;
    m_vOldEnemyPos              = vec_zero;
    m_vLastEnemyPos             = vec_zero;
    m_vLastDeathPos             = vec_zero;
    m_vIdleProgressPos          = vec_zero;
    m_iIdleProgressTime         = 0;
    ResetGrenadeAvoidance();
    m_bReloadRetreating         = false;
    m_pCombatPrimaryWeapon      = NULL;
    m_pEnemy                    = NULL;
    m_iEnemyEyesTag             = -1;
    movement.ClearCombatTarget();
    ClearTeamResponse();
    ResetObjectiveBehavior();
}

/*
====================
Idle state

Make the bot move to random directions
====================
*/
void BotController::InitState_Idle(botfunc_t *func)
{
    func->CheckCondition = &BotController::CheckCondition_Idle;
    func->BeginState     = &BotController::State_BeginIdle;
    func->EndState       = &BotController::State_EndIdle;
    func->ThinkState     = &BotController::State_Idle;
}

bool BotController::CheckCondition_Idle(void)
{
    if (m_bGrenadeFleeing) {
        return false;
    }

    if (m_iCuriousTime) {
        return false;
    }

    if (m_iAttackTime) {
        return false;
    }

    return true;
}

void BotController::State_BeginIdle(void)
{
    State_DefaultBegin();
    m_vIdleProgressPos  = controlledEnt->origin;
    m_iIdleProgressTime = level.inttime;
}

void BotController::State_EndIdle(void)
{
    m_iIdleProgressTime = 0;
}

void BotController::State_Idle(void)
{
    if (CheckWindows()) {
        m_botCmd.buttons ^= BUTTON_ATTACKLEFT;
        m_iLastFireTime = level.inttime;
    } else {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
        CheckReload();
    }

    AimAtAimNode();

    if (m_bObjectiveOwnsMovement) {
        m_vIdleProgressPos  = controlledEnt->origin;
        m_iIdleProgressTime = level.inttime;
        return;
    }

    // A valid path can still circle through the same room indefinitely, so
    // physical "blocked" checks alone cannot recognize the failure. If an
    // idle bot has not left a modest area in ten seconds, discard that roam
    // and choose a new heading below. Objective movement has its own progress
    // handling and is deliberately excluded above.
    if (!m_iIdleProgressTime) {
        m_vIdleProgressPos  = controlledEnt->origin;
        m_iIdleProgressTime = level.inttime;
    } else if (level.inttime >= m_iIdleProgressTime + BOT_IDLE_PROGRESS_MSEC) {
        if ((controlledEnt->origin - m_vIdleProgressPos).lengthXYSquared()
            < Square(BOT_IDLE_PROGRESS_UNITS)) {
            movement.ClearMove();
            movement.AbandonAttractivePoint();
            m_vLastDeathPos = vec_zero;
        }

        m_vIdleProgressPos  = controlledEnt->origin;
        m_iIdleProgressTime = level.inttime;
    }

    if (!movement.MoveToBestAttractivePoint() && !movement.IsMoving()) {
        if (m_vLastDeathPos != vec_zero) {
            movement.MoveTo(m_vLastDeathPos);

            if (movement.MoveDone()) {
                m_vLastDeathPos = vec_zero;
            }
        } else {
            Vector randomDir(G_CRandom(16), G_CRandom(16), 0);
            Vector preferredDir(G_CRandom(1.0f), G_CRandom(1.0f), 0);
            float  radius = 512 + G_Random(2048);

            if (VectorNormalize2D(preferredDir) <= 0) {
                preferredDir = Vector(controlledEnt->orientation[0]);
            }
            preferredDir *= 1024.0f;

            movement.AvoidPath(controlledEnt->origin + randomDir, radius, preferredDir);
        }
    }
}

/*
====================
Curious state

Forward to the last event position
====================
*/
void BotController::InitState_Curious(botfunc_t *func)
{
    func->CheckCondition = &BotController::CheckCondition_Curious;
    func->ThinkState     = &BotController::State_Curious;
}

bool BotController::CheckCondition_Curious(void)
{
    if (m_iAttackTime) {
        m_iCuriousTime      = 0;
        m_iCuriousEventType = AI_EVENT_NONE;
        ClearTeamResponse();
        return false;
    }

    if (m_iCuriousTime && !CanInvestigatePosition(m_vNewCuriousPos)) {
        m_iCuriousTime      = 0;
        m_iCuriousEventType = AI_EVENT_NONE;
        ClearTeamResponse();
        movement.ClearMove();
        return false;
    }

    if (level.inttime > m_iCuriousTime) {
        if (m_iCuriousTime) {
            movement.ClearMove();
            m_iCuriousTime      = 0;
            m_iCuriousEventType = AI_EVENT_NONE;
            ClearTeamResponse();
        }

        return false;
    }

    return true;
}

void BotController::State_Curious(void)
{
    if (CheckWindows()) {
        m_botCmd.buttons ^= BUTTON_ATTACKLEFT;
        m_iLastFireTime = level.inttime;
    } else {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
    }

    AimAtAimNode();

    if (!movement.MoveToBestAttractivePoint(3) && (!movement.IsMoving() || m_vLastCuriousPos != m_vNewCuriousPos)) {
        movement.MoveTo(m_vNewCuriousPos);
        m_vLastCuriousPos = m_vNewCuriousPos;

        // A sound or team report can be connected acoustically while still
        // being unreachable by the navigation mesh. Do not spend the rest of
        // the report lifetime searching around a destination we cannot path
        // to; resume normal roaming instead.
        if (!movement.IsMoving()) {
            m_iCuriousTime      = 0;
            m_iCuriousEventType = AI_EVENT_NONE;
            ClearTeamResponse();
            return;
        }
    }

    if (movement.MoveDone() && m_bTeamResponding && level.inttime < m_iTeamContactExpireTime) {
        if (!m_iNextTeamSearchMoveTime) {
            m_iNextTeamSearchMoveTime = level.inttime + 500 + (int)G_Random(1000);
        } else if (level.inttime >= m_iNextTeamSearchMoveTime) {
            Vector searchOffset(G_CRandom(192.0f), G_CRandom(192.0f), 0.0f);
            m_vNewCuriousPos          = m_vTeamContactPos + searchOffset;
            m_iNextTeamSearchMoveTime = level.inttime + 1000 + (int)G_Random(1500);
            movement.MoveTo(m_vNewCuriousPos);
            m_vLastCuriousPos = m_vNewCuriousPos;
        }
        return;
    }

    if (movement.MoveDone()) {
        m_iCuriousTime      = 0;
        m_iCuriousEventType = AI_EVENT_NONE;
        ClearTeamResponse();
    }
}

/*
====================
Attack state

Attack the enemy
====================
*/
void BotController::InitState_Attack(botfunc_t *func)
{
    func->CheckCondition = &BotController::CheckCondition_Attack;
    func->EndState       = &BotController::State_EndAttack;
    func->ThinkState     = &BotController::State_Attack;
}

static Vector bot_origin;

static int sentients_compare(const void *elem1, const void *elem2)
{
    Entity *e1, *e2;
    float   delta[3];
    float   d1, d2;

    e1 = *(Entity **)elem1;
    e2 = *(Entity **)elem2;

    VectorSubtract(bot_origin, e1->origin, delta);
    d1 = VectorLengthSquared(delta);

    VectorSubtract(bot_origin, e2->origin, delta);
    d2 = VectorLengthSquared(delta);

    if (d2 <= d1) {
        return d1 > d2;
    } else {
        return -1;
    }
}

bool BotController::IsValidEnemy(Sentient *sent) const
{
    if (sent == controlledEnt) {
        return false;
    }

    if (sent->hidden() || (sent->flags & FL_NOTARGET)) {
        // Ignore hidden / non-target enemies
        return false;
    }

    if (sent->IsDead()) {
        // Ignore dead enemies
        return false;
    }

    if (sent->getSolidType() == SOLID_NOT) {
        // Ignore non-solid, like spectators
        return false;
    }

    if (sent->IsSubclassOfPlayer()) {
        Player *player = static_cast<Player *>(sent);

        if (g_gametype->integer >= GT_TEAM && player->GetTeam() == controlledEnt->GetTeam()) {
            return false;
        }
    } else {
        if (sent->m_Team == controlledEnt->m_Team) {
            return false;
        }
    }

    return true;
}

bool BotController::IsEngagedByAnotherBot(Sentient *enemy) const
{
    const Container<BotController *>& controllers = botManager.getControllerManager().getControllers();

    for (int i = 1; i <= controllers.NumObjects(); i++) {
        BotController *other = controllers.ObjectAt(i);
        if (other != this && other->m_pEnemy == enemy) {
            return true;
        }
    }

    return false;
}

bool BotController::CanSeeEnemyPoint(Sentient *enemy, const Vector& point)
{
    return G_SightTrace(
        controlledEnt->EyePosition(),
        vec_zero,
        vec_zero,
        point,
        controlledEnt,
        enemy,
        MASK_CANSEE,
        qfalse,
        "BotController::CanSeeEnemyPoint"
    );
}

bool BotController::IsEnemyWithinVision(Sentient *enemy) const
{
    vec2_t delta;

    VectorSub2D(enemy->centroid, controlledEnt->centroid, delta);
    if (VectorLength2DSquared(delta) > Square(BOT_MAX_VISION_DISTANCE)) {
        return false;
    }

    return controlledEnt->AreasConnected(enemy);
}

static float BotEnemyEyeFraction(Sentient *enemy)
{
    if (enemy->maxs.z <= 0) {
        return 1.0f;
    }

    return Q_clamp_float((enemy->EyePosition().z - enemy->origin.z) / enemy->maxs.z, 0.0f, 1.0f);
}

//
// Any-part visibility used for target acquisition. Unlike a single
// eye-to-eye trace, an enemy peeking over cover or standing behind a railing
// still counts as visible when any sampled body height can be seen.
//
bool BotController::IsEnemyPartVisible(Sentient *enemy)
{
    if (!IsEnemyWithinVision(enemy)) {
        return false;
    }

    if (CanSeeEnemyPoint(enemy, enemy->EyePosition())) {
        return true;
    }

    for (int i = 0; i < BOT_NUM_VISIBILITY_SAMPLES; i++) {
        Vector point = enemy->origin;
        point.z += enemy->maxs.z * BOT_VISIBILITY_SAMPLES[i];

        if (CanSeeEnemyPoint(enemy, point)) {
            return true;
        }
    }

    return false;
}

//
// Full visibility scan for the current combat target. Returns whether any
// part of the enemy is visible and adjusts the aim height: the humanized
// height is kept while the body around it is visible, otherwise the aim
// falls back to the visible sampled height closest to it - for an enemy
// firing over a wall that is the lowest visible point of the body, so the
// bot shoots back instead of burying its aim in the cover.
//
bool BotController::CheckEnemyVisibility(Sentient *enemy, float desiredAimFraction, float& aimFraction)
{
    struct {
        float fraction;
        bool  visible;
    } samples[BOT_NUM_VISIBILITY_SAMPLES + 1];

    aimFraction = desiredAimFraction;

    if (!IsEnemyWithinVision(enemy)) {
        return false;
    }

    samples[0].fraction = BotEnemyEyeFraction(enemy);
    samples[0].visible  = CanSeeEnemyPoint(enemy, enemy->EyePosition());

    for (int i = 0; i < BOT_NUM_VISIBILITY_SAMPLES; i++) {
        Vector point = enemy->origin;
        point.z += enemy->maxs.z * BOT_VISIBILITY_SAMPLES[i];

        samples[i + 1].fraction = BOT_VISIBILITY_SAMPLES[i];
        samples[i + 1].visible  = CanSeeEnemyPoint(enemy, point);
    }

    bool anyVisible = false;
    for (int i = 0; i < BOT_NUM_VISIBILITY_SAMPLES + 1; i++) {
        if (samples[i].visible) {
            anyVisible = true;
            break;
        }
    }

    if (!anyVisible) {
        return false;
    }

    // Keep the humanized height when the sampled heights bracketing it are
    // both visible.
    float belowFraction = -1.0f, aboveFraction = -1.0f;
    bool  belowVisible = false, aboveVisible = false;
    for (int i = 0; i < BOT_NUM_VISIBILITY_SAMPLES + 1; i++) {
        if (samples[i].fraction <= desiredAimFraction
            && (belowFraction < 0 || samples[i].fraction > belowFraction)) {
            belowFraction = samples[i].fraction;
            belowVisible  = samples[i].visible;
        }
        if (samples[i].fraction >= desiredAimFraction
            && (aboveFraction < 0 || samples[i].fraction < aboveFraction)) {
            aboveFraction = samples[i].fraction;
            aboveVisible  = samples[i].visible;
        }
    }

    if ((belowFraction < 0 || belowVisible) && (aboveFraction < 0 || aboveVisible)) {
        return true;
    }

    // Fall back to the visible height nearest the desired one, preferring
    // the lower point on ties.
    float bestDistance = 2.0f;
    for (int i = 0; i < BOT_NUM_VISIBILITY_SAMPLES + 1; i++) {
        if (!samples[i].visible) {
            continue;
        }

        const float distance = fabs(samples[i].fraction - desiredAimFraction);
        if (distance < bestDistance || (distance == bestDistance && samples[i].fraction < aimFraction)) {
            bestDistance = distance;
            aimFraction  = samples[i].fraction;
        }
    }

    return true;
}

void BotController::BeginAimAcquisition(void)
{
    m_iAimAcquireTime = level.inttime;

    // Keep live console edits safe even when the two cvars are changed in
    // reverse order after CVAR_Init has already run.
    const float minHeight = Q_min(g_bot_aim_height_min->value, g_bot_aim_height_max->value);
    const float maxHeight = Q_max(g_bot_aim_height_min->value, g_bot_aim_height_max->value);
    m_fAimHeightFraction  = minHeight + G_Random(maxHeight - minHeight);

    m_vAimErrorDirection       = RandomBotAimErrorDirection();
    m_vAimErrorTargetDirection = m_vAimErrorDirection;
    m_iNextAimErrorChangeTime  = level.inttime + RandomBotAimErrorInterval();

    m_iAimHistoryHead  = 0;
    m_iAimHistoryCount = 0;
}

void BotController::UpdateAimErrorDirection(void)
{
    if (level.inttime >= m_iNextAimErrorChangeTime) {
        m_vAimErrorTargetDirection = RandomBotAimErrorDirection();
        m_iNextAimErrorChangeTime  = level.inttime + RandomBotAimErrorInterval();
    }

    // Exponential-style smoothing keeps the miss direction moving without
    // per-frame jitter and makes the behavior independent of server FPS.
    const float blend = Q_clamp_float(level.frametime * 1.5f, 0, 1);
    m_vAimErrorDirection += (m_vAimErrorTargetDirection - m_vAimErrorDirection) * blend;
}

Vector BotController::GetDelayedAimTarget(const Vector& currentTarget)
{
    if (g_bot_aim_latency->integer <= 0) {
        m_iAimHistoryHead  = 0;
        m_iAimHistoryCount = 0;
        return currentTarget;
    }

    const int newestIndex = (m_iAimHistoryHead + MAX_AIM_HISTORY_SAMPLES - 1) % MAX_AIM_HISTORY_SAMPLES;
    if (m_iAimHistoryCount && m_AimHistory[newestIndex].time == level.inttime) {
        m_AimHistory[newestIndex].position = currentTarget;
    } else {
        m_AimHistory[m_iAimHistoryHead].time     = level.inttime;
        m_AimHistory[m_iAimHistoryHead].position = currentTarget;
        m_iAimHistoryHead = (m_iAimHistoryHead + 1) % MAX_AIM_HISTORY_SAMPLES;
        if (m_iAimHistoryCount < MAX_AIM_HISTORY_SAMPLES) {
            m_iAimHistoryCount++;
        }
    }

    const int targetTime  = level.inttime - g_bot_aim_latency->integer;
    const int oldestIndex =
        (m_iAimHistoryHead + MAX_AIM_HISTORY_SAMPLES - m_iAimHistoryCount) % MAX_AIM_HISTORY_SAMPLES;
    const aim_sample_t *previous = &m_AimHistory[oldestIndex];

    if (targetTime <= previous->time) {
        return previous->position;
    }

    for (int i = 1; i < m_iAimHistoryCount; i++) {
        const int           index = (oldestIndex + i) % MAX_AIM_HISTORY_SAMPLES;
        const aim_sample_t *next  = &m_AimHistory[index];

        if (targetTime <= next->time) {
            const int sampleDuration = next->time - previous->time;
            if (sampleDuration <= 0) {
                return next->position;
            }

            const float fraction = (float)(targetTime - previous->time) / sampleDuration;
            return previous->position + (next->position - previous->position) * fraction;
        }

        previous = next;
    }

    return currentTarget;
}

bool BotController::CheckCondition_Attack(void)
{
    bot_origin = controlledEnt->origin;

    // Target focus: stay locked on the current enemy as long as it is a valid
    // target and in sight, instead of re-picking the nearest one each think.
    // The one exception is a dogpile: if another bot has also started engaging
    // this target, fall through to reacquire and peel off onto a free enemy
    // (the acquisition below keeps us here anyway when there is none). Also
    // reacquire once the target dies, changes state, or breaks line of sight.
    if (m_pEnemy && IsValidEnemy(m_pEnemy) && IsEnemyPartVisible(m_pEnemy) && !IsEngagedByAnotherBot(m_pEnemy)) {
        m_vLastEnemyPos = m_pEnemy->origin;
        m_iAttackTime   = level.inttime + 1000;
        return true;
    }

    Container<Sentient *> sents = SentientList;
    sents.Sort(sentients_compare);

    // Acquire a target. Prefer the nearest visible enemy that no other bot is
    // already engaging, so bots spread their fire instead of all piling onto
    // one target (for example the player). Only fall back to an already-engaged
    // enemy when it is the only one in sight.
    Sentient *pFallback = NULL; // nearest visible enemy, engaged by someone or not
    Sentient *pChosen   = NULL; // nearest visible enemy no other bot is on

    for (int i = 1; i <= sents.NumObjects(); i++) {
        Sentient *sent = sents.ObjectAt(i);

        if (!IsValidEnemy(sent)) {
            continue;
        }

        if (!IsEnemyPartVisible(sent)) {
            continue;
        }

        if (!pFallback) {
            pFallback = sent;
        }

        if (!IsEngagedByAnotherBot(sent)) {
            pChosen = sent;
            break;
        }
    }

    if (!pChosen) {
        pChosen = pFallback;
    }

    if (pChosen) {
        if (m_pEnemy != pChosen) {
            m_iEnemyEyesTag = -1;
            BeginAimAcquisition();
        }

        if (!m_pEnemy) {
            m_iLastUnseenTime = level.inttime;
        }

        m_pEnemy        = pChosen;
        m_vLastEnemyPos = m_pEnemy->origin;
        m_iAttackTime   = level.inttime + 1000;
        return true;
    }

    // Nothing new in sight: retain the current enemy only until the deadline
    // established by the last visible observation.
    if (m_pEnemy && IsValidEnemy(m_pEnemy) && level.inttime <= m_iAttackTime) {
        return true;
    }

    if (level.inttime > m_iAttackTime) {
        if (m_iAttackTime) {
            movement.ClearMove();
            m_iAttackTime = 0;
        }

        return false;
    }

    return true;
}

void BotController::State_EndAttack(void)
{
    m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
    movement.ClearCombatTarget();
    m_bReloadRetreating = false;
    m_iAttackStopAimTime = 0;
    controlledEnt->ZoomOff();
    m_iAimAcquireTime           = -1;
    m_iAimHistoryHead           = 0;
    m_iAimHistoryCount          = 0;
    m_vAimErrorDirection        = vec_zero;
    m_vAimErrorTargetDirection = vec_zero;
    m_iNextAimErrorChangeTime   = 0;
}

void BotController::State_Attack(void)
{
    bool    bCanSee             = false;
    bool    bCanAttack          = false;
    float   fAimHeightFraction  = m_fAimHeightFraction;
    float   fMinDistance        = 128;
    float   fMinDistanceSquared = fMinDistance * fMinDistance;
    float   fEnemyDistanceSquared;
    Weapon *pWeap   = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    bool    bNoMove = false;
    bool    bFiring = false;

    if (!m_pEnemy || !IsValidEnemy(m_pEnemy)) {
        // Ignore dead enemies
        m_telemetry.fireDecision = BOT_FIRE_NO_TARGET;
        m_iAttackTime = 0;
        movement.ClearCombatTarget();
        return;
    }
    float fDistanceSquared = (m_pEnemy->origin - controlledEnt->origin).lengthSquared();
    m_telemetry.enemyDistance = sqrt(fDistanceSquared);
    const bool bReloading = pWeap && pWeap->GetState() == WEAPON_RELOADING;

    // Feed the aggressive-movement layer the enemy itself, not only a
    // distance. This lets forward/back phases remain enemy-relative while the
    // bot turns and follows a path around a cramped room.
    movement.SetCombatTarget(m_pEnemy->origin);

    m_vOldEnemyPos = m_vLastEnemyPos;

    bCanSee = CheckEnemyVisibility(m_pEnemy, m_fAimHeightFraction, fAimHeightFraction);
    m_telemetry.enemyVisible = bCanSee;

    if (bCanSee) {
        m_iAttackStopAimTime = Q_max(
            m_iAttackStopAimTime,
            level.inttime + BOT_LOS_AIM_HOLD_MSEC
        );
        if (m_pEnemy->IsSubclassOfPlayer()) {
            botManager.ReportTeamContact(
                controlledEnt,
                static_cast<Player *>(m_pEnemy.Pointer()),
                BOT_CONTACT_VISUAL,
                m_pEnemy->origin,
                0.0f
            );
        }

        if (!pWeap) {
            m_telemetry.fireDecision = BOT_FIRE_NO_WEAPON;
            return;
        }

        bCanAttack = !bReloading;
        if (bReloading) {
            m_telemetry.fireDecision = BOT_FIRE_RELOADING;
            m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
            controlledEnt->ZoomOff();
        } else if (m_iLastUnseenTime) {
            const unsigned int minDelay = g_bot_attack_react_min_delay->value * 1000;
            if (level.inttime <= m_iLastUnseenTime + minDelay) {
                bCanAttack = false;
                m_telemetry.fireDecision = BOT_FIRE_REACTION_DELAY;
                m_telemetry.reactionRemainingMsec =
                    static_cast<int>(m_iLastUnseenTime + minDelay - level.inttime);
            } else {
                m_iLastUnseenTime = 0;
                // Fresh acquisition: choose one error vector and settle it
                // smoothly instead of picking a new direction every frame.
                BeginAimAcquisition();
            }
        }

        if (bCanAttack) {
            m_telemetry.canAttack = true;
            float fPrimaryBulletRange          = pWeap->GetBulletRange(FIRE_PRIMARY) / 1.25f;
            float fPrimaryBulletRangeSquared   = fPrimaryBulletRange * fPrimaryBulletRange;
            float fSpreadFactor                = pWeap->GetSpreadFactor(FIRE_PRIMARY);

            //
            // check the fire movement speed if the weapon has a max fire movement
            //
            if (pWeap->GetMaxFireMovement() < 1 && pWeap->HasAmmoInClip(FIRE_PRIMARY)) {
                float length;

                length = controlledEnt->velocity.length();
                if ((length / sv_runspeed->value) > (pWeap->GetMaxFireMovementMult())) {
                    bNoMove = true;
                    m_telemetry.noMove = true;
                    movement.ClearMove();
                }
            }

            fMinDistance = fPrimaryBulletRange;

            // Human players did not try to maintain a broad 256-unit buffer.
            // Retreat behavior rose sharply only at body-contact range; the
            // radial movement layer handles the wider 64-384 unit rhythm.
            if (fMinDistance > 64) {
                fMinDistance = 64;
            }

            fMinDistanceSquared = fMinDistance * fMinDistance;

            if (controlledEnt->client->ps.stats[STAT_AMMO] <= 0
                && controlledEnt->client->ps.stats[STAT_CLIPAMMO] <= 0) {
                m_telemetry.fireDecision = BOT_FIRE_NO_AMMO;
                m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
                controlledEnt->ZoomOff();
            } else if (fDistanceSquared > fPrimaryBulletRangeSquared) {
                m_telemetry.fireDecision = BOT_FIRE_OUT_OF_RANGE;
                m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
                controlledEnt->ZoomOff();
            } else {
                //
                // Attacking
                //

                if (pWeap->IsSemiAuto()) {
                    if (controlledEnt->client->ps.iViewModelAnim != VM_ANIM_IDLE
                        && (controlledEnt->client->ps.iViewModelAnim < VM_ANIM_IDLE_0
                            || controlledEnt->client->ps.iViewModelAnim > VM_ANIM_IDLE_2)) {
                        m_telemetry.fireDecision = BOT_FIRE_SEMIAUTO_BUSY;
                        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
                        controlledEnt->ZoomOff();
                    } else if (fSpreadFactor < 0.25) {
                        bFiring = true;
                        m_telemetry.fireDecision = BOT_FIRE_FIRING;
                        m_telemetry.wantsFire    = true;
                        m_botCmd.buttons ^= BUTTON_ATTACKLEFT;
                        if (pWeap->GetZoom()) {
                            if (!controlledEnt->IsZoomed()) {
                                m_botCmd.buttons |= BUTTON_ATTACKRIGHT;
                            } else {
                                m_botCmd.buttons &= ~BUTTON_ATTACKRIGHT;
                            }
                        }
                    } else {
                        bNoMove = true;
                        m_telemetry.noMove       = true;
                        m_telemetry.fireDecision = BOT_FIRE_SEMIAUTO_SPREAD;
                        movement.ClearMove();
                    }
                } else {
                    bFiring = true;
                    m_telemetry.fireDecision = BOT_FIRE_FIRING;
                    m_telemetry.wantsFire    = true;
                    m_botCmd.buttons |= BUTTON_ATTACKLEFT;
                }
            }

            m_iLastFireTime = level.inttime;

            // Bots do not use the secondary-fire melee bash (Spearhead /
            // Breakthrough): they keep firing their primary at point-blank
            // range instead of lunging in to butt-strike an enemy.

            m_iAttackTime        = level.inttime + 1000;
            m_iAttackStopAimTime = level.inttime + 3000;
            m_iLastSeenTime      = level.inttime;
            m_vLastEnemyPos      = m_pEnemy->origin;
        }
    } else {
        m_telemetry.fireDecision = bReloading ? BOT_FIRE_RELOADING : BOT_FIRE_NO_SIGHT;
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
        fMinDistanceSquared = 0;

        if (level.inttime > m_iLastSeenTime + 2000) {
            m_iLastUnseenTime = level.inttime;
        }
    }

    if (bCanSee || level.inttime < m_iAttackStopAimTime) {
        Vector        vTarget;
        orientation_t eyes_or;

        if (m_iEnemyEyesTag == -1) {
            // Cache the tag
            m_iEnemyEyesTag = gi.Tag_NumForName(m_pEnemy->edict->tiki, "eyes bone");
        }

        if (m_iEnemyEyesTag != -1) {
            // Use the enemy's eyes bone
            m_pEnemy->GetTag(m_iEnemyEyesTag, &eyes_or);

            //vRandomOffset = Vector(G_CRandom(8), G_CRandom(8), -G_Random(32));
            vTarget = eyes_or.origin;
        } else {
            //vRandomOffset = Vector(G_CRandom(8), G_CRandom(8), 16 + G_Random(m_pEnemy->viewheight - 16));
            vTarget = m_pEnemy->origin;
        }

        if (m_iAimAcquireTime < 0) {
            BeginAimAcquisition();
        }

        // Build the complete aim point before recording it so latency also
        // delays vertical movement and stance changes. This makes 120 ms mean
        // 120 ms, independent of frame rate.
        vTarget.z = m_pEnemy->origin.z + m_pEnemy->maxs.z * fAimHeightFraction;
        vTarget   = GetDelayedAimTarget(vTarget);
        m_telemetry.aimTarget = vTarget;

        // Acquisition error settles into a small, drifting floor instead of
        // reaching perfect tracking. The existing aim-error value still
        // controls both the initial miss and the residual, so no extra tuning
        // control is needed.
        float       errorFraction = BOT_AIM_RESIDUAL_FRACTION;
        const float settleMs = g_bot_aim_settle_time->value * 1000;
        if (settleMs > 0) {
            const float acquisitionFraction =
                Q_clamp_float(1.0f - (level.inttime - m_iAimAcquireTime) / settleMs, 0, 1);
            errorFraction += (1.0f - BOT_AIM_RESIDUAL_FRACTION) * acquisitionFraction;
        }

        UpdateAimErrorDirection();

        Vector vAimPoint = vTarget;
        vAimPoint += m_vAimErrorDirection * (g_bot_aim_error->value * errorFraction);

        m_telemetry.aimErrorFraction  = errorFraction;
        m_telemetry.aimErrorUnits     = g_bot_aim_error->value * errorFraction;
        m_telemetry.aimErrorDirection = m_vAimErrorDirection;
        m_telemetry.aimPoint          = vAimPoint;

        rotation.AimAt(vAimPoint);
    } else {
        AimAtAimNode();
    }

    if (bReloading) {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);

        if (!m_bReloadRetreating) {
            m_bReloadRetreating = true;
            G_MoveLogBotEvent(
                "bot_reload_retreat",
                controlledEnt,
                NULL,
                m_pEnemy->entnum,
                m_pEnemy->origin
            );
        }

        if (g_bot_reload_pistol->integer) {
            UseCombatPistol();
        }

        // A navigation path can begin by moving toward the threat or curve
        // back as geometry changes. Remove that path and shape the final
        // combat command away from the live enemy every frame instead.
        movement.ClearMove();
        if (fDistanceSquared < Square(BOT_RELOAD_SAFE_DISTANCE)) {
            movement.SetCombatTarget(m_pEnemy->origin, true);
        } else {
            movement.ClearCombatTarget();
        }
        return;
    }
    m_bReloadRetreating = false;

    if (bNoMove) {
        m_telemetry.noMove = true;
        return;
    }

    fEnemyDistanceSquared = (controlledEnt->origin - m_vLastEnemyPos).lengthSquared();

    if ((!movement.MoveToBestAttractivePoint(5) && !movement.IsMoving())
        || (m_vOldEnemyPos != m_vLastEnemyPos && !movement.MoveDone()) || fEnemyDistanceSquared < fMinDistanceSquared) {
        if (fEnemyDistanceSquared < fMinDistanceSquared) {
            Vector vDir = controlledEnt->origin - m_vLastEnemyPos;
            VectorNormalizeFast(vDir);

            movement.AvoidPath(m_vLastEnemyPos, fMinDistance, Vector(controlledEnt->orientation[1]) * 512);
        } else {
            movement.MoveTo(m_vLastEnemyPos);
        }

        if (!bCanSee && movement.MoveDone()) {
            // Lost track of the enemy
            ClearEnemy();
            return;
        }
    }

    if (bCanSee && movement.IsMoving()) {
        m_iAttackTime = level.inttime + 1000;
    }
}

/*
====================
Grenade state

Avoid any grenades
====================
*/
void BotController::InitState_Grenade(botfunc_t *func)
{
    func->CheckCondition = &BotController::CheckCondition_Grenade;
    func->BeginState     = &BotController::State_BeginGrenade;
    func->EndState       = &BotController::State_EndGrenade;
    func->ThinkState     = &BotController::State_Grenade;
}

bool BotController::CheckCondition_Grenade(void)
{
    if (!g_bot_grenade_avoid || !g_bot_grenade_avoid->integer) {
        ResetGrenadeAvoidance();
        return false;
    }

    if (!m_pAvoidGrenade) {
        if (m_iGrenadeReactTime) {
            G_MoveLogBotEvent(
                "bot_grenade_gone",
                controlledEnt,
                NULL,
                -1,
                m_vAvoidGrenadePosition
            );
        }
        m_iGrenadeReactTime    = 0;
        m_iGrenadeNextPathTime = 0;
        return false;
    }

    m_vAvoidGrenadePosition = m_pAvoidGrenade->origin;
    if ((m_vAvoidGrenadePosition - controlledEnt->origin).lengthSquared()
        >= Square(BOT_GRENADE_SAFE_DISTANCE)) {
        G_MoveLogBotEvent(
            "bot_grenade_safe",
            controlledEnt,
            NULL,
            m_pAvoidGrenade->entnum,
            m_vAvoidGrenadePosition
        );
        m_pAvoidGrenade         = NULL;
        m_iGrenadeReactTime    = 0;
        m_iGrenadeNextPathTime = 0;
        return false;
    }

    return level.inttime >= m_iGrenadeReactTime;
}

void BotController::State_BeginGrenade(void)
{
    m_bGrenadeFleeing      = true;
    m_iGrenadeNextPathTime = 0;
    movement.ClearMove();
    m_botCmd.buttons &= ~BUTTON_USE;

    G_MoveLogBotEvent(
        "bot_grenade_flee",
        controlledEnt,
        NULL,
        m_pAvoidGrenade ? m_pAvoidGrenade->entnum : -1,
        m_vAvoidGrenadePosition
    );
}

void BotController::State_EndGrenade(void)
{
    m_bGrenadeFleeing      = false;
    m_iGrenadeNextPathTime = 0;
    movement.ClearMove();
}

void BotController::State_Grenade(void)
{
    if (!m_pAvoidGrenade) {
        return;
    }

    m_vAvoidGrenadePosition = m_pAvoidGrenade->origin;

    // Attack still owns aim and firing, but not movement while escaping.
    movement.ClearCombatTarget();
    m_botCmd.buttons &= ~BUTTON_USE;

    if (level.inttime < m_iGrenadeNextPathTime && movement.IsMoving()
        && !movement.MoveDone()) {
        return;
    }

    Vector away = controlledEnt->origin - m_vAvoidGrenadePosition;
    away.z      = 0.0f;
    if (away.lengthXYSquared() < 1.0f) {
        away = -Vector(controlledEnt->orientation[0]);
        away.z = 0.0f;
    }
    away.normalize();

    movement.AvoidPath(
        m_vAvoidGrenadePosition,
        BOT_GRENADE_SAFE_DISTANCE,
        away * BOT_GRENADE_SAFE_DISTANCE
    );
    m_iGrenadeNextPathTime = level.inttime + BOT_GRENADE_REPATH_MSEC;

    if (!movement.MoveDone()) {
        m_bGrenadePathFailed = false;
        return;
    }

    if (!m_bGrenadePathFailed) {
        G_MoveLogBotEvent(
            "bot_grenade_path_fail",
            controlledEnt,
            NULL,
            m_pAvoidGrenade->entnum,
            m_vAvoidGrenadePosition
        );
        m_bGrenadePathFailed = true;
    }

    // A missing nav path should not leave the bot standing on the grenade.
    // Direct movement retains the normal collision and ledge guards.
    movement.MoveDirect(
        controlledEnt->origin + away * BOT_GRENADE_DIRECT_ESCAPE_STEP,
        32.0f
    );
}

/*
====================
Weapon state

Change weapon when necessary
====================
*/
void BotController::InitState_Weapon(botfunc_t *func)
{
    func->CheckCondition = &BotController::CheckCondition_Weapon;
    func->BeginState     = &BotController::State_BeginWeapon;
}

bool BotController::CheckCondition_Weapon(void)
{
    return controlledEnt->GetActiveWeapon(WEAPON_MAIN)
        != controlledEnt->BestWeapon(NULL, false, WEAPON_CLASS_THROWABLE);
}

void BotController::State_BeginWeapon(void)
{
    Weapon *weap = controlledEnt->BestWeapon(NULL, false, WEAPON_CLASS_THROWABLE);

    if (weap == NULL) {
        SendCommand("safeholster 1");
        return;
    }

    SendCommand(va("use \"%s\"", weap->model.c_str()));
}

Weapon *BotController::FindWeaponWithAmmo(int requiredClass)
{
    Weapon               *next;
    int                   n;
    int                   j;
    int                   bestrank;
    Weapon               *bestweapon;
    const Container<int>& inventory = controlledEnt->getInventory();

    n = inventory.NumObjects();

    // Search until we find the best weapon with ammo
    bestweapon = NULL;
    bestrank   = -999999;

    for (j = 1; j <= n; j++) {
        next = (Weapon *)G_GetEntity(inventory.ObjectAt(j));

        assert(next);
        if (!next->IsSubclassOfWeapon() || next->IsSubclassOfInventoryItem()) {
            continue;
        }

        if (next->GetWeaponClass() & WEAPON_CLASS_THROWABLE) {
            continue;
        }

        if (requiredClass && !(next->GetWeaponClass() & requiredClass)) {
            continue;
        }

        if (next->GetRank() < bestrank) {
            continue;
        }

        if (!next->HasAmmo(FIRE_PRIMARY)) {
            continue;
        }

        bestweapon = (Weapon *)next;
        bestrank   = bestweapon->GetRank();
    }

    return bestweapon;
}

bool BotController::UseCombatPistol()
{
    Weapon *active = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    if (active && (active->GetWeaponClass() & WEAPON_CLASS_PISTOL)) {
        return false;
    }

    Weapon *pending = controlledEnt->GetNewActiveWeapon();
    if (pending) {
        return (pending->GetWeaponClass() & WEAPON_CLASS_PISTOL) != 0;
    }

    Weapon *pistol = FindWeaponWithAmmo(WEAPON_CLASS_PISTOL);
    if (!pistol) {
        return false;
    }

    m_pCombatPrimaryWeapon = active;
    controlledEnt->useWeapon(pistol, WEAPON_MAIN);
    G_MoveLogBotEvent(
        "bot_reload_pistol",
        controlledEnt,
        m_pEnemy && m_pEnemy->IsSubclassOfPlayer() ? static_cast<Player *>(m_pEnemy.Pointer()) : NULL,
        pistol->entnum,
        controlledEnt->origin
    );
    return true;
}

void BotController::UseWeaponWithAmmo()
{
    Weapon *bestWeapon = FindWeaponWithAmmo();

    if (!bestWeapon || bestWeapon == controlledEnt->GetActiveWeapon(WEAPON_MAIN)) {
        return;
    }

    controlledEnt->useWeapon(bestWeapon, WEAPON_MAIN);
}

void BotController::Spawned(void)
{
    ClearEnemy();
    ResetGrenadeAvoidance();
    m_bReloadRetreating = false;
    m_pCombatPrimaryWeapon = NULL;
    m_iPostKillAimUntil    = 0;
    m_vPostKillAimAngles   = vec_zero;
    m_iLadderAimUntil      = 0;
    m_vLadderAimAngles     = vec_zero;
    m_iCuriousTime      = 0;
    m_iCuriousEventType = AI_EVENT_NONE;
    m_vIdleProgressPos  = vec_zero;
    m_iIdleProgressTime = 0;
    m_botCmd.buttons    = 0;
    m_StateFlags        = 0;
    ClearTeamResponse();
    ResetObjectiveBehavior();
}

void BotController::Think()
{
    usercmd_t  ucmd;
    usereyes_t eyeinfo;

    UpdateBotStates();
    GetUsercmd(&ucmd);
    GetEyeInfo(&eyeinfo);

    G_ClientThink(controlledEnt->edict, &ucmd, &eyeinfo);
}

void BotController::Killed(const Event& ev)
{
    Entity *attacker;

    ClearTeamResponse();
    ResetGrenadeAvoidance();
    ResetObjectiveBehavior();
    m_pCombatPrimaryWeapon = NULL;
    m_iPostKillAimUntil    = 0;
    m_vPostKillAimAngles   = vec_zero;
    m_iLadderAimUntil      = 0;
    m_vLadderAimAngles     = vec_zero;

    // send the respawn buttons
    if (!(m_botCmd.buttons & BUTTON_ATTACKLEFT)) {
        m_botCmd.buttons |= BUTTON_ATTACKLEFT;
    } else {
        m_botCmd.buttons &= ~BUTTON_ATTACKLEFT;
    }

    m_botEyes.ofs[0]    = 0;
    m_botEyes.ofs[1]    = 0;
    m_botEyes.ofs[2]    = 0;
    m_botEyes.angles[0] = 0;
    m_botEyes.angles[1] = 0;

    attacker = ev.GetEntity(1);

    if (attacker && rand() % 5 == 0) {
        // 1/5 chance to go back to the attacker position
        m_vLastDeathPos = attacker->origin;
    } else {
        m_vLastDeathPos = vec_zero;
    }

    // Choose a new random primary weapon
    Event event(EV_Player_PrimaryDMWeapon);
    event.AddString("auto");

    controlledEnt->ProcessEvent(event);
}

void BotController::GotKill(const Event& ev)
{
    m_vPostKillAimAngles = rotation.GetTargetAngles();
    m_iPostKillAimUntil  = level.inttime + BOT_POST_KILL_AIM_MSEC;
    ClearEnemy();
    m_iCuriousTime      = 0;
    m_iCuriousEventType = AI_EVENT_NONE;

    // Bot taunts disabled
}

void BotController::EventStuffText(const str& text)
{
    SendCommand(text);
}

void BotController::setControlledEntity(Player *player)
{
    controlledEnt = player;
    movement.SetControlledEntity(player);
    rotation.SetControlledEntity(player);

    delegateHandle_gotKill =
        player->delegate_gotKill.Add(std::bind(&BotController::GotKill, this, std::placeholders::_1));
    delegateHandle_killed = player->delegate_killed.Add(std::bind(&BotController::Killed, this, std::placeholders::_1));
    delegateHandle_stufftext =
        player->delegate_stufftext.Add(std::bind(&BotController::EventStuffText, this, std::placeholders::_1));
    delegateHandle_spawned = player->delegate_spawned.Add(std::bind(&BotController::Spawned, this));
}

Player *BotController::getControlledEntity() const
{
    return controlledEnt;
}

BotController *BotControllerManager::createController(Player *player)
{
    BotController *controller = new BotController();
    controller->setControlledEntity(player);

    controllers.AddObject(controller);

    return controller;
}

void BotControllerManager::removeController(BotController *controller)
{
    controllers.RemoveObject(controller);
    delete controller;
}

BotController *BotControllerManager::findController(Entity *ent)
{
    int i;

    for (i = 1; i <= controllers.NumObjects(); i++) {
        BotController *controller = controllers.ObjectAt(i);
        if (controller->getControlledEntity() == ent) {
            return controller;
        }
    }

    return nullptr;
}

const Container<BotController *>& BotControllerManager::getControllers() const
{
    return controllers;
}

BotControllerManager::~BotControllerManager()
{
    Cleanup();
}

void BotControllerManager::Init()
{
    BotController::Init();
}

void BotControllerManager::Cleanup()
{
    int i;

    BotController::Init();

    for (i = 1; i <= controllers.NumObjects(); i++) {
        BotController *controller = controllers.ObjectAt(i);
        delete controller;
    }

    controllers.FreeObjectList();
}

void BotControllerManager::ThinkControllers()
{
    int i;

    // Delete controllers that don't have associated player entity
    // This cannot happen unless some mods remove them
    for (i = controllers.NumObjects(); i > 0; i--) {
        BotController *controller = controllers.ObjectAt(i);
        if (!controller->getControlledEntity()) {
            gi.DPrintf(
                "Bot %d has no associated player entity. This shouldn't happen unless the entity has been removed by a "
                "script. The controller will be removed, please fix.\n",
                i
            );

            // Remove the controller, it will be recreated later to match `sv_bots`
            delete controller;
            controllers.RemoveObjectAt(i);
        }
    }

    for (i = 1; i <= controllers.NumObjects(); i++) {
        BotController *controller = controllers.ObjectAt(i);
        controller->Think();
    }
}
