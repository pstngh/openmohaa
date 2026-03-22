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
#include "../corepp/tiki.h"
#include "scriptexception.h"
#include "vehicleturret.h"
#include "weaputils.h"
#include "windows.h"
#include "g_bot.h"

// Debug logging helper — writes to both console and bot_obj_debug.log
static void BotObjDebug(const char *fmt, ...)
{
    if (!g_bot_debug_obj->integer) {
        return;
    }

    char    buf[1024];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    gi.Printf("%s", buf);

    fileHandle_t f = gi.FS_FOpenFileAppend("bot_obj_debug.log");
    if (f) {
        char line[1100];
        int  len = snprintf(line, sizeof(line), "[%.2f] %s", level.time, buf);
        gi.FS_Write(line, len, f);
        gi.FS_FCloseFile(f);
    }
}

// We assume that we have limited access to the server-side
// and that most logic come from the playerstate_s structure

// Objective state constants
#define BOT_OBJ_STATE_NONE      0
#define BOT_OBJ_STATE_MOVING    1
#define BOT_OBJ_STATE_PLANTING  2
#define BOT_OBJ_STATE_DEFENDING 3
#define BOT_OBJ_STATE_DEFUSING  4

// Sub-states for plant/defuse USE key sequencing:
// AIMING  = rotating to face the bomb, USE is released
// EDGE    = facing bomb, release USE for 1 frame to create a rising edge
// HOLDING = hold USE continuously (DoUse fires on the rising edge frame)
#define BOT_USE_AIMING   0
#define BOT_USE_EDGE     1
#define BOT_USE_HOLDING  2

#define BOT_PLANT_DEFUSE_TIME   5.0f
#define BOT_OBJ_PROXIMITY       96.0f
#define BOT_OBJ_ENEMY_RANGE     512.0f
#define BOT_OBJ_DEFEND_RADIUS   384.0f

CLASS_DECLARATION(Listener, BotController, NULL) {
    {NULL, NULL}
};

BotController::botfunc_t BotController::botfuncs[MAX_BOT_FUNCTIONS];

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

    m_iCuriousTime    = 0;
    m_iAttackTime     = 0;
    m_iEnemyEyesTag   = -1;
    m_iLastSeenTime   = 0;
    m_iLastUnseenTime = 0;

    m_iObjectiveState   = 0;
    m_iUseState         = 0;
    m_fPlantDefuseStart = 0;
    m_fPlantHealthStart = 0;
    m_bIsOnBombTeam     = false;
    m_iBombSiteIndex    = -1;
    m_vMyObjective      = vec_zero;

    m_StateFlags = 0;

    // Strafe and lean - always start with a direction (never neutral)
    m_iStrafeDirection      = (rand() % 2) ? 1 : -1;
    m_iLeanDirection        = m_iStrafeDirection;
    m_fNextStrafeChangeTime = 0;
    m_iShootMoveMode        = 0;
    m_fNextShootMoveTime    = 0;
    m_iShootMoveDirection   = 1;
}

BotController::~BotController()
{
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

void BotController::Init(void)
{
    for (int i = 0; i < MAX_BOT_FUNCTIONS; i++) {
        botfuncs[i].BeginState = &BotController::State_DefaultBegin;
        botfuncs[i].EndState   = &BotController::State_DefaultEnd;
    }

    InitState_Attack(&botfuncs[0]);
    InitState_Curious(&botfuncs[1]);
    InitState_Objective(&botfuncs[2]);
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

    CheckStates();

    movement.MoveThink(m_botCmd);
    rotation.TurnThink(m_botCmd, m_botEyes);
    CheckUse();

    CheckValidWeapon();
}

void BotController::CheckUse(void)
{
    // Don't toggle use button while planting/defusing
    if (m_iObjectiveState == BOT_OBJ_STATE_PLANTING || m_iObjectiveState == BOT_OBJ_STATE_DEFUSING) {
        return;
    }

    Vector  dir;
    Vector  start;
    Vector  end;
    trace_t trace;

    if (controlledEnt->GetLadder()) {
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
    Weapon *weapon = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    if (!weapon) {
        // If holstered, use the best weapon available
        UseWeaponWithAmmo();
    } else if (!weapon->HasAmmo(FIRE_PRIMARY) && !controlledEnt->GetNewActiveWeapon()) {
        // In case the current weapon has no ammo, use the best available weapon
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
    Vector goal;

    if (!movement.IsMoving()) {
        return;
    }

    //goal = movement.GetCurrentGoal();
    //if (goal != controlledEnt->origin) {
    //    rotation.AimAt(goal);
    //}

    if (controlledEnt->GetLadder()) {
        Vector vAngles = movement.GetCurrentPathDirection().toAngles();
        vAngles.x      = Q_clamp_float(vAngles.x, -80, 80);

        rotation.SetTargetAngles(vAngles);
        return;
    } else {
        Vector targetAngles;
        targetAngles   = movement.GetCurrentPathDirection().toAngles();
        targetAngles.x = 0;
        rotation.SetTargetAngles(targetAngles);
    }
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
    Sentient *pSentOwner;
    float     fRangeFactor;
    Vector    delta1, delta2;

    if (m_iCuriousTime) {
        delta1 = vPos - controlledEnt->origin;
        delta2 = m_vNewCuriousPos - controlledEnt->origin;
        if (delta1.lengthSquared() < delta2.lengthSquared()) {
            return;
        }
    }

    fRangeFactor = 1.0 - (fDistanceSquared / fRadiusSquared);

    if (fRangeFactor < random()) {
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
    } else {
        pSentOwner = NULL;
    }

    if (pSentOwner) {
        if (pSentOwner == controlledEnt) {
            // Ignore self
            return;
        }

        if ((pSentOwner->flags & FL_NOTARGET) || pSentOwner->getSolidType() == SOLID_NOT) {
            return;
        }

        // Ignore teammates
        if (pSentOwner->IsSubclassOfPlayer()) {
            Player *p = static_cast<Player *>(pSentOwner);

            if (g_gametype->integer >= GT_TEAM && p->GetTeam() == controlledEnt->GetTeam()) {
                return;
            }
        }
    }

    // React to all sound events
    m_iCuriousTime   = level.inttime + 20000;
    m_vNewCuriousPos = vPos;
}

/*
====================
ClearEnemy

Clear the bot's enemy
====================
*/
void BotController::ClearEnemy(void)
{
    m_iAttackTime   = 0;
    m_pEnemy        = NULL;
    m_iEnemyEyesTag = -1;
    m_vOldEnemyPos  = vec_zero;
    m_vLastEnemyPos = vec_zero;
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
    m_iCuriousTime    = 0;
    m_iAttackTime     = 0;
    m_vLastCuriousPos = vec_zero;
    m_vOldEnemyPos    = vec_zero;
    m_vLastEnemyPos   = vec_zero;
    m_vLastDeathPos   = vec_zero;
    m_pEnemy          = NULL;
    m_iEnemyEyesTag   = -1;
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
    func->ThinkState     = &BotController::State_Idle;
}

bool BotController::CheckCondition_Idle(void)
{
    if (m_iCuriousTime) {
        return false;
    }

    if (m_iAttackTime) {
        return false;
    }

    return true;
}

void BotController::State_Idle(void)
{
    // Don't interfere with planting/defusing at all
    if (m_iObjectiveState == BOT_OBJ_STATE_PLANTING || m_iObjectiveState == BOT_OBJ_STATE_DEFUSING) {
        return;
    }

    if (CheckWindows()) {
        m_botCmd.buttons ^= BUTTON_ATTACKLEFT;
        m_iLastFireTime = level.inttime;
    } else {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
        CheckReload();
    }

    AimAtAimNode();

    // If Objective state is active, let it control movement
    if (m_StateFlags & (1 << 2)) {
        return;
    }

    if (!movement.MoveToBestAttractivePoint() && !movement.IsMoving()) {
        if (m_vLastDeathPos != vec_zero) {
            movement.MoveTo(m_vLastDeathPos);

            if (movement.MoveDone()) {
                m_vLastDeathPos = vec_zero;
            }
        } else {
            Vector randomDir(G_CRandom(16), G_CRandom(16), G_CRandom(16));
            Vector preferredDir;
            float  radius = 512 + G_Random(2048);

            // In objective modes, bias roaming toward the objective
            // Only attackers should rush the objective; defenders roam
            // randomly until a bomb is planted (objective state handles
            // rushing to defuse).
            if (dmManager.IsBotObjectiveSet()
                && (g_gametype->integer == GT_OBJECTIVE || g_gametype->integer == GT_TEAM_ROUNDS)
                && (m_bIsOnBombTeam || dmManager.GetBombsPlanted() > 0)) {
                preferredDir = dmManager.GetBotObjectiveLocation() - controlledEnt->origin;
                preferredDir.normalize();
                preferredDir *= 1024;
            } else {
                preferredDir += Vector(controlledEnt->orientation[0]) * (rand() % 5 ? 1024 : -1024);
                preferredDir += Vector(controlledEnt->orientation[2]) * (rand() % 5 ? 1024 : -1024);
            }
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
        m_iCuriousTime = 0;
        return false;
    }

    if (level.inttime > m_iCuriousTime) {
        if (m_iCuriousTime) {
            movement.ClearMove();
            m_iCuriousTime = 0;
        }

        return false;
    }

    return true;
}

void BotController::State_Curious(void)
{
    // Don't interfere with planting/defusing
    if (m_iObjectiveState == BOT_OBJ_STATE_PLANTING || m_iObjectiveState == BOT_OBJ_STATE_DEFUSING) {
        return;
    }

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
    }

    if (movement.MoveDone()) {
        m_iCuriousTime = 0;
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

bool BotController::CheckCondition_Attack(void)
{
    Container<Sentient *> sents       = SentientList;
    float                 maxDistance = 0;

    bot_origin = controlledEnt->origin;
    sents.Sort(sentients_compare);

    for (int i = 1; i <= sents.NumObjects(); i++) {
        Sentient *sent = sents.ObjectAt(i);

        if (!IsValidEnemy(sent)) {
            continue;
        }

        maxDistance = 4096.0f;

        if (controlledEnt->CanSee(sent, 360, maxDistance, false)) {
            if (m_pEnemy != sent) {
                m_iEnemyEyesTag = -1;
            }

            if (!m_pEnemy) {
                m_iLastUnseenTime = level.inttime;
            }

            m_pEnemy        = sent;
            m_vLastEnemyPos = m_pEnemy->origin;
        }

        if (m_pEnemy) {
            m_iAttackTime = level.inttime + 1000;
            return true;
        }
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
    controlledEnt->ZoomOff();
}

void BotController::State_Attack(void)
{
    bool    bCanSee             = false;
    bool    bCanAttack          = false;
    float   fMinDistance        = 128;
    float   fMinDistanceSquared = fMinDistance * fMinDistance;
    float   fEnemyDistanceSquared;
    Weapon *pWeap   = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    bool    bNoMove = false;
    bool    bFiring = false;

    // While planting or defusing, don't aim/shoot/move at enemies —
    // objective state owns the bot completely in those states
    if (m_iObjectiveState == BOT_OBJ_STATE_PLANTING || m_iObjectiveState == BOT_OBJ_STATE_DEFUSING) {
        return;
    }

    if (!m_pEnemy || !IsValidEnemy(m_pEnemy)) {
        // Ignore dead enemies
        m_iAttackTime = 0;
        return;
    }
    float fDistanceSquared = (m_pEnemy->origin - controlledEnt->origin).lengthSquared();

    m_vOldEnemyPos = m_vLastEnemyPos;

    bCanSee =
        controlledEnt->CanSee(m_pEnemy, 360, 4096.0f, false);

    if (bCanSee) {
        if (!pWeap) {
            return;
        }

        bCanAttack = true;
        if (m_iLastUnseenTime) {
            const float reactionTime = Q_min(1000 * Q_min(1, fDistanceSquared / Square(2048)), 1000);
            const unsigned int minDelay = g_bot_attack_react_min_delay->value * 1000;
            const unsigned int randomDelay = g_bot_attack_react_random_delay->value * 1000;
            if (level.inttime <= m_iLastUnseenTime + minDelay + G_Random(randomDelay)) {
                bCanAttack = false;
            } else {
                m_iLastUnseenTime = 0;
            }
        }

        if (bCanAttack) {
            float fPrimaryBulletRange        = pWeap->GetBulletRange(FIRE_PRIMARY) / 1.25f;
            float fPrimaryBulletRangeSquared = fPrimaryBulletRange * fPrimaryBulletRange;
            float fSpreadFactor              = pWeap->GetSpreadFactor(FIRE_PRIMARY);

            //
            // check the fire movement speed if the weapon has a max fire movement
            // NOTE: We no longer stop movement - bots should always keep moving
            //
            if (pWeap->GetMaxFireMovement() < 1 && pWeap->HasAmmoInClip(FIRE_PRIMARY)) {
                float length;

                length = controlledEnt->velocity.length();
                if ((length / sv_runspeed->value) > (pWeap->GetMaxFireMovementMult())) {
                    bNoMove = true;
                    // movement.ClearMove(); - Removed: bots never stop moving
                }
            }

            fMinDistance = fPrimaryBulletRange;

            if (fMinDistance > 256) {
                fMinDistance = 256;
            }

            fMinDistanceSquared = fMinDistance * fMinDistance;

            if (controlledEnt->client->ps.stats[STAT_AMMO] <= 0
                && controlledEnt->client->ps.stats[STAT_CLIPAMMO] <= 0) {
                m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
                controlledEnt->ZoomOff();
            } else if (fDistanceSquared > fPrimaryBulletRangeSquared) {
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
                        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
                        controlledEnt->ZoomOff();
                    } else if (fSpreadFactor < 0.25) {
                        bFiring = true;
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
                        // movement.ClearMove(); - Removed: bots never stop moving
                    }
                } else {
                    bFiring = true;
                    m_botCmd.buttons |= BUTTON_ATTACKLEFT;
                }
            }

            m_iLastFireTime = level.inttime;

            m_iAttackTime        = level.inttime + 1000;
            m_iAttackStopAimTime = level.inttime + 3000;
            m_iLastSeenTime      = level.inttime;
            m_vLastEnemyPos      = m_pEnemy->origin;
        }
    } else {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
        fMinDistanceSquared = 0;

        if (level.inttime > m_iLastSeenTime + 2000) {
            m_iLastUnseenTime = level.inttime;
        }
    }

    if (bCanSee || level.inttime < m_iAttackStopAimTime) {
        Vector vTarget;

        // Aim at body using cvar-controlled height range
        // g_bot_aim_height_max = top of aim range (default 0.65 = chest)
        // g_bot_aim_height_min = bottom of aim range (default 0.49 = waist)
        float maxHeight = m_pEnemy->viewheight * g_bot_aim_height_max->value;
        float minHeight = m_pEnemy->viewheight * g_bot_aim_height_min->value;
        float spreadDown = maxHeight - minHeight;

        vTarget = m_pEnemy->origin;
        vTarget[2] += maxHeight;

        if (level.inttime >= m_iLastAimTime + 100) {
            // Spread only goes left/right or DOWN (never up toward head)
            m_vAimOffset[0] = G_CRandom((m_pEnemy->maxs.x - m_pEnemy->mins.x) * 0.5);
            m_vAimOffset[1] = G_CRandom((m_pEnemy->maxs.y - m_pEnemy->mins.y) * 0.5);
            m_vAimOffset[2] = -G_Random(spreadDown);  // Only negative (down toward min height)
            m_iLastAimTime = level.inttime;
        }

        rotation.AimAt(vTarget + m_vAimOffset * g_bot_attack_spreadmult->value);
    } else {
        AimAtAimNode();
    }

    if (bNoMove) {
        return;
    }

    // Don't chase enemies while actively planting or defusing —
    // objective state controls movement in those states
    if (m_iObjectiveState == BOT_OBJ_STATE_PLANTING || m_iObjectiveState == BOT_OBJ_STATE_DEFUSING) {
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

    if (movement.IsMoving()) {
        m_iAttackTime = level.inttime + 1000;
    }
}

/*
====================
Objective state

Handle bomb plant/defuse in objective game modes
====================
*/

void BotController::InitState_Objective(botfunc_t *func)
{
    func->CheckCondition = &BotController::CheckCondition_Objective;
    func->BeginState     = &BotController::State_BeginObjective;
    func->EndState       = &BotController::State_EndObjective;
    func->ThinkState     = &BotController::State_Objective;
}

bool BotController::CheckCondition_Objective(void)
{
    bool bDebug = g_bot_debug_obj->integer != 0;

    if (g_gametype->integer != GT_OBJECTIVE && g_gametype->integer != GT_TEAM_ROUNDS) {
        return false;
    }

    //
    // If no explicit bot objective was set via script, try to discover
    // a valid objective location from multiple sources.
    //
    if (!dmManager.IsBotObjectiveSet()) {
        Vector     vFallback = vec_zero;
        const char *source   = NULL;

        // 1. Try team-specific objective locations (used by TOW and
        //    maps with m_bForceTeamObjectiveLocation)
        if (g_gametype->integer >= GT_TOW || level.m_bForceTeamObjectiveLocation) {
            if (controlledEnt->GetTeam() == TEAM_AXIS && level.m_vAxisObjectiveLocation != vec_zero) {
                vFallback = level.m_vAxisObjectiveLocation;
                source    = "AxisObjectiveLocation";
            } else if (controlledEnt->GetTeam() == TEAM_ALLIES && level.m_vAlliedObjectiveLocation != vec_zero) {
                vFallback = level.m_vAlliedObjectiveLocation;
                source    = "AlliedObjectiveLocation";
            }
        }

        // 2. Try the generic objective location
        if (vFallback == vec_zero && level.m_vObjectiveLocation != vec_zero) {
            vFallback = level.m_vObjectiveLocation;
            source    = "ObjectiveLocation";
        }

        // 3. Scan CS_OBJECTIVES configstrings for any objective with a location
        if (vFallback == vec_zero) {
            for (int i = 0; i < MAX_OBJECTIVES; i++) {
                const char *s = gi.getConfigstring(CS_OBJECTIVES + i);
                if (!s || !s[0]) {
                    continue;
                }

                const char *loc = Info_ValueForKey(s, "loc");
                Vector      v;
                if (loc[0] && sscanf(loc, "%f %f %f", &v[0], &v[1], &v[2]) == 3 && v != vec_zero) {
                    vFallback = v;
                    source    = va("CS_OBJECTIVES[%d]", i);
                    break;
                }
            }
        }

        // 4. Auto-discover from func_objective entities in the map
        if (vFallback == vec_zero) {
            Entity *obj = G_FindClass(NULL, "func_objective");
            if (obj) {
                vFallback = obj->origin;
                source    = "func_objective entity";
            }
        }

        // One-time entity dump for diagnostics (written to bot_obj_dump.txt)
        if (bDebug) {
            static bool bDumped = false;
            if (!bDumped) {
                bDumped = true;
                fileHandle_t f = gi.FS_FOpenFileWrite("bot_obj_dump.txt");
                if (f) {
                    const char *header = "BOT_OBJ: Entity dump for objective discovery:\n";
                    gi.FS_Write(header, strlen(header), f);
                    for (gentity_t *ent = &g_entities[0]; ent < &g_entities[globals.num_entities]; ent++) {
                        if (!ent->inuse || !ent->entity) {
                            continue;
                        }
                        const char *cls  = ent->entity->getClassID();
                        const char *tnam = ent->entity->TargetName().c_str();
                        const char *mdl  = ent->entity->model.c_str();
                        const char *tiki = ent->tiki ? ent->tiki->name : "";
                        if (tnam[0] || (mdl[0] && strcmp(cls, "worldspawn") != 0) || tiki[0]) {
                            char line[512];
                            int len = snprintf(line, sizeof(line),
                                "  [%d] class='%s' tname='%s' model='%s' tiki='%s'"
                                " org=(%.0f %.0f %.0f)\n",
                                ent->entity->entnum, cls,
                                tnam, mdl, tiki,
                                ent->entity->origin[0], ent->entity->origin[1], ent->entity->origin[2]
                            );
                            if (len > 0) {
                                gi.FS_Write(line, len, f);
                            }
                        }
                    }
                    gi.FS_FCloseFile(f);
                    gi.Printf("BOT_OBJ: Entity dump written to bot_obj_dump.txt\n");
                }
            }
        }

        // 5. Find bomb explosive entities by tiki model name.
        //    In obj_ maps, bomb plant sites are script_model entities with
        //    the "pulse_explosive" tiki model.
        //    Collect ALL bomb sites so attacking bots can be split among them.
        if (vFallback == vec_zero) {
            dmManager.ClearBombSites();
            for (gentity_t *ent = &g_entities[0]; ent < &g_entities[globals.num_entities]; ent++) {
                if (!ent->inuse || !ent->entity) {
                    continue;
                }
                // Check both Entity::model and edict->tiki->name
                bool bFound = false;
                if (ent->entity->model.length() > 0
                    && strstr(ent->entity->model.c_str(), "pulse_explosive")) {
                    bFound = true;
                }
                if (!bFound && ent->tiki && ent->tiki->name
                    && strstr(ent->tiki->name, "pulse_explosive")) {
                    bFound = true;
                }
                if (bFound && ent->entity->origin != vec_zero) {
                    dmManager.AddBombSite(ent->entity->origin);
                    BotObjDebug(
                        "BOT_OBJ: Found bomb site #%d at (%g %g %g) entity '%s'\n",
                        dmManager.GetNumBombSites(),
                        ent->entity->origin[0], ent->entity->origin[1], ent->entity->origin[2],
                        ent->entity->TargetName().c_str()
                    );
                    if (vFallback == vec_zero) {
                        vFallback = ent->entity->origin;
                        source    = va("bomb model entity '%s'", ent->entity->TargetName().c_str());
                    }
                }
            }
        }

        // 6. Find trigger_use entities as a last resort — these are
        //    the interaction points for planting/defusing in obj_ maps.
        //    Brush entities often have origin at (0,0,0), so use the
        //    center of absmin/absmax instead.
        if (vFallback == vec_zero) {
            Entity *trig = G_FindClass(NULL, "trigger_use");
            if (trig) {
                Vector center = (trig->absmin + trig->absmax) * 0.5f;
                if (center != vec_zero) {
                    vFallback = center;
                    source    = "trigger_use entity";
                }
            }
        }

        if (vFallback == vec_zero) {
            BotObjDebug(
                "BOT_OBJ [%s]: No objective location found. gametype=%d, "
                "objLoc=(%g %g %g), alliedLoc=(%g %g %g), axisLoc=(%g %g %g)\n",
                controlledEnt->client->pers.netname,
                g_gametype->integer,
                level.m_vObjectiveLocation[0], level.m_vObjectiveLocation[1], level.m_vObjectiveLocation[2],
                level.m_vAlliedObjectiveLocation[0], level.m_vAlliedObjectiveLocation[1], level.m_vAlliedObjectiveLocation[2],
                level.m_vAxisObjectiveLocation[0], level.m_vAxisObjectiveLocation[1], level.m_vAxisObjectiveLocation[2]
            );
            return false;
        }

        dmManager.SetBotObjectiveLocation(vFallback);

        BotObjDebug(
            "BOT_OBJ [%s]: Objective set to (%g %g %g) from %s\n",
            controlledEnt->client->pers.netname,
            vFallback[0], vFallback[1], vFallback[2],
            source
        );
    }

    return true;
}

void BotController::State_BeginObjective(void)
{
    m_iObjectiveState   = BOT_OBJ_STATE_NONE;
    m_fPlantDefuseStart = 0;
    m_fPlantHealthStart = 0;

    // Determine if this bot is on the planting team
    m_bIsOnBombTeam = false;
    if (dmManager.GetBombPlantTeam() == STRING_AXIS && controlledEnt->GetTeam() == TEAM_AXIS) {
        m_bIsOnBombTeam = true;
    } else if (dmManager.GetBombPlantTeam() == STRING_ALLIES && controlledEnt->GetTeam() == TEAM_ALLIES) {
        m_bIsOnBombTeam = true;
    }

    // Assign this bot a bomb site. Split attacking bots across available
    // bomb sites using their entity number so they don't all rush the same one.
    m_vMyObjective   = dmManager.GetBotObjectiveLocation();
    m_iBombSiteIndex = -1;
    if (m_bIsOnBombTeam && dmManager.GetNumBombSites() > 0) {
        m_iBombSiteIndex = controlledEnt->entnum % dmManager.GetNumBombSites();
        m_vMyObjective   = dmManager.GetBombSite(m_iBombSiteIndex);
    }

    BotObjDebug(
        "BOT_OBJ [%s]: BeginObjective - team=%d, bombPlantTeam=%d, isOnBombTeam=%d, "
        "bombSites=%d, myObj=(%g %g %g)\n",
        controlledEnt->client->pers.netname,
        controlledEnt->GetTeam(),
        (int)dmManager.GetBombPlantTeam(),
        m_bIsOnBombTeam,
        dmManager.GetNumBombSites(),
        m_vMyObjective[0], m_vMyObjective[1], m_vMyObjective[2]
    );
}

void BotController::State_EndObjective(void)
{
    // Release planter slot if we were planting
    if (m_iBombSiteIndex >= 0) {
        dmManager.ReleaseBombSitePlanter(m_iBombSiteIndex, controlledEnt->entnum);
    }
    m_iObjectiveState = BOT_OBJ_STATE_NONE;
    m_iUseState       = BOT_USE_AIMING;
}

bool BotController::IsNearObjective(float fRadius) const
{
    Vector vObjPos = m_bIsOnBombTeam ? m_vMyObjective : dmManager.GetBotObjectiveLocation();
    Vector vDelta  = controlledEnt->origin - vObjPos;
    // Use XY distance only — bombs may be at a different Z height
    // (on tables, elevated platforms, etc.) which inflates 3D distance
    return vDelta.lengthXYSquared() <= fRadius * fRadius;
}

bool BotController::IsEnemyNearby(float fRadius) const
{
    float fRadiusSq = fRadius * fRadius;

    for (int i = 1; i <= SentientList.NumObjects(); i++) {
        Sentient *sent = SentientList.ObjectAt(i);

        if (!IsValidEnemy(sent)) {
            continue;
        }

        Vector vDelta = sent->origin - controlledEnt->origin;
        if (vDelta.lengthSquared() <= fRadiusSq) {
            if (controlledEnt->CanSee(sent, 360, fRadius, false)) {
                return true;
            }
        }
    }

    return false;
}

void BotController::State_Objective(void)
{
    // Attackers use their assigned bomb site, defenders use the shared location
    Vector vObjPos         = m_bIsOnBombTeam ? m_vMyObjective : dmManager.GetBotObjectiveLocation();
    bool   bAnyBombPlanted = dmManager.GetBombsPlanted() > 0;
    bool   bMySitePlanted  = m_iBombSiteIndex >= 0 && dmManager.IsBombSitePlanted(m_iBombSiteIndex);
    bool   bInCombat       = m_iAttackTime != 0;

    // Periodic debug logging (every 2 seconds)
    if (level.inttime % 2000 < 50) {
        Vector vDelta = controlledEnt->origin - vObjPos;
        BotObjDebug(
            "BOT_OBJ [%s]: state=%d useState=%d inCombat=%d bombTeam=%d anyPlanted=%d mySitePlanted=%d site=%d "
            "distXY=%.0f dist3D=%.0f pos=(%.0f %.0f %.0f) myObj=(%.0f %.0f %.0f) moving=%d\n",
            controlledEnt->client->pers.netname,
            m_iObjectiveState, m_iUseState, bInCombat, m_bIsOnBombTeam, bAnyBombPlanted, bMySitePlanted, m_iBombSiteIndex,
            vDelta.lengthXY(), vDelta.length(),
            controlledEnt->origin[0], controlledEnt->origin[1], controlledEnt->origin[2],
            vObjPos[0], vObjPos[1], vObjPos[2],
            movement.IsMoving()
        );
    }

    // Don't let combat status interfere with planting/defusing.
    // Once the bot reaches the bomb and enters PLANTING/DEFUSING,
    // it commits fully. The attack state already returns early
    // when planting/defusing, so there's no conflict.

    // Suppress fire when not in combat, or when actively planting/defusing
    // (can't shoot and hold USE at the same time)
    if (!bInCombat
        || m_iObjectiveState == BOT_OBJ_STATE_PLANTING
        || m_iObjectiveState == BOT_OBJ_STATE_DEFUSING) {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
        CheckReload();
    }

    if (m_bIsOnBombTeam) {
        //
        // Attacking team logic
        //
        if (!bMySitePlanted) {
            // Already in PLANTING state — stay in it regardless of distance
            // (don't let small drift reset the USE state machine)
            bool bInRange = IsNearObjective(BOT_OBJ_PROXIMITY);

            if (m_iObjectiveState == BOT_OBJ_STATE_PLANTING || bInRange) {
                // Try to claim the planter slot for this site
                bool bIsPlanter = dmManager.ClaimBombSitePlanter(m_iBombSiteIndex, controlledEnt->entnum);

                if (!bIsPlanter) {
                    // Someone else is planting — defend the area
                    m_iObjectiveState = BOT_OBJ_STATE_DEFENDING;
                    m_iUseState       = BOT_USE_AIMING;
                    if (!movement.IsMoving()) {
                        Vector randomDir(G_CRandom(8), G_CRandom(8), 0);
                        movement.MoveNear(vObjPos + randomDir, BOT_OBJ_DEFEND_RADIUS * 0.5f);
                    }
                    return;
                }

                if (m_iObjectiveState != BOT_OBJ_STATE_PLANTING) {
                    // Start planting
                    m_iObjectiveState   = BOT_OBJ_STATE_PLANTING;
                    m_iUseState         = BOT_USE_AIMING;
                    m_fPlantDefuseStart = level.time;
                    m_fPlantHealthStart = controlledEnt->health;
                }

                // Abort if we took damage — release the planter slot
                if (controlledEnt->health < m_fPlantHealthStart) {
                    dmManager.ReleaseBombSitePlanter(m_iBombSiteIndex, controlledEnt->entnum);
                    m_iObjectiveState = BOT_OBJ_STATE_MOVING;
                    m_iUseState       = BOT_USE_AIMING;
                    return;
                }

                // Stop moving and crouch while planting
                movement.ClearMove();
                m_botCmd.forwardmove = 0;
                m_botCmd.rightmove   = 0;
                m_botCmd.upmove      = -1;

                // Look at the bomb objective
                rotation.AimAt(vObjPos);

                // USE key state machine:
                // AIMING  -> face bomb, USE released
                // EDGE    -> release USE for 1 frame (ensures rising edge next frame)
                // HOLDING -> hold USE (DoUse fires on the first HOLDING frame)
                switch (m_iUseState) {
                case BOT_USE_AIMING:
                    m_botCmd.buttons &= ~BUTTON_USE;
                    if (rotation.IsNearTargetAngles(15.0f)) {
                        m_iUseState = BOT_USE_EDGE;
                        BotObjDebug("BOT_OBJ [%s]: facing bomb, releasing USE for edge\n",
                            controlledEnt->client->pers.netname);
                    }
                    break;

                case BOT_USE_EDGE:
                    m_botCmd.buttons &= ~BUTTON_USE;
                    m_iUseState         = BOT_USE_HOLDING;
                    m_fPlantDefuseStart = level.time;
                    BotObjDebug("BOT_OBJ [%s]: edge frame, will hold USE next frame\n",
                        controlledEnt->client->pers.netname);
                    break;

                case BOT_USE_HOLDING:
                    m_botCmd.buttons |= BUTTON_USE;

                    if (level.inttime % 1000 < 50) {
                        BotObjDebug("BOT_OBJ [%s]: holding USE, %.1fs elapsed\n",
                            controlledEnt->client->pers.netname,
                            level.time - m_fPlantDefuseStart);
                    }
                    break;
                }

                // Check if plant is complete
                if (level.time - m_fPlantDefuseStart >= BOT_PLANT_DEFUSE_TIME) {
                    dmManager.SetBombsPlanted(dmManager.GetBombsPlanted() + 1);
                    if (m_iBombSiteIndex >= 0) {
                        dmManager.SetBombSitePlanted(m_iBombSiteIndex, true);
                    }
                    dmManager.ReleaseBombSitePlanter(m_iBombSiteIndex, controlledEnt->entnum);
                    m_iObjectiveState = BOT_OBJ_STATE_DEFENDING;
                    m_iUseState       = BOT_USE_AIMING;
                    BotObjDebug("BOT_OBJ [%s]: plant complete on site %d!\n",
                        controlledEnt->client->pers.netname, m_iBombSiteIndex);
                }
            } else {
                // Move toward objective (even during combat — attack state
                // handles aiming/shooting, this overrides its chase movement)
                m_iObjectiveState = BOT_OBJ_STATE_MOVING;
                movement.MoveTo(vObjPos);
            }
        } else {
            // Bomb is planted, defend it
            m_iObjectiveState = BOT_OBJ_STATE_DEFENDING;
            if (!IsNearObjective(BOT_OBJ_DEFEND_RADIUS)) {
                movement.MoveNear(vObjPos, BOT_OBJ_DEFEND_RADIUS);
            } else if (!movement.IsMoving()) {
                // Patrol around the bomb site
                Vector randomDir(G_CRandom(8), G_CRandom(8), 0);
                movement.MoveNear(vObjPos + randomDir, BOT_OBJ_DEFEND_RADIUS * 0.5f);
            }
        }
    } else {
        //
        // Defending team logic
        //
        if (!bAnyBombPlanted) {
            // No bomb planted — defenders roam freely.
            // Clear any previous objective movement so bots don't rush a bomb site.
            if (m_iObjectiveState != BOT_OBJ_STATE_NONE) {
                movement.ClearMove();
                m_iObjectiveState = BOT_OBJ_STATE_NONE;
                m_iUseState       = BOT_USE_AIMING;
            }
            return;
        } else {
            // Bomb is planted! Find the nearest planted site to rush to
            int    iDefuseSite = -1;
            float  fBestDist   = 1e9f;
            Vector vDefusePos  = vObjPos;

            for (int s = 0; s < dmManager.GetNumBombSites(); s++) {
                if (dmManager.IsBombSitePlanted(s)) {
                    Vector vDelta = controlledEnt->origin - dmManager.GetBombSite(s);
                    float  fDist  = vDelta.length();
                    if (fDist < fBestDist) {
                        fBestDist  = fDist;
                        iDefuseSite = s;
                        vDefusePos  = dmManager.GetBombSite(s);
                    }
                }
            }

            if (m_iObjectiveState == BOT_OBJ_STATE_DEFUSING || fBestDist <= BOT_OBJ_PROXIMITY) {
                if (m_iObjectiveState != BOT_OBJ_STATE_DEFUSING) {
                    // Start defusing
                    m_iObjectiveState   = BOT_OBJ_STATE_DEFUSING;
                    m_iUseState         = BOT_USE_AIMING;
                    m_fPlantDefuseStart = level.time;
                    m_fPlantHealthStart = controlledEnt->health;
                }

                // Only abort defuse if we took damage — do NOT abort just
                // because enemies are nearby. Defusing under fire is critical.
                if (controlledEnt->health < m_fPlantHealthStart) {
                    m_iObjectiveState = BOT_OBJ_STATE_MOVING;
                    m_iUseState       = BOT_USE_AIMING;
                    return;
                }

                // Stop moving and crouch while defusing
                movement.ClearMove();
                m_botCmd.forwardmove = 0;
                m_botCmd.rightmove   = 0;
                m_botCmd.upmove      = -1;

                // Look at the bomb
                rotation.AimAt(vDefusePos);

                // USE key state machine (same as planting)
                switch (m_iUseState) {
                case BOT_USE_AIMING:
                    m_botCmd.buttons &= ~BUTTON_USE;
                    if (rotation.IsNearTargetAngles(15.0f)) {
                        m_iUseState = BOT_USE_EDGE;
                    }
                    break;

                case BOT_USE_EDGE:
                    m_botCmd.buttons &= ~BUTTON_USE;
                    m_iUseState         = BOT_USE_HOLDING;
                    m_fPlantDefuseStart = level.time;
                    break;

                case BOT_USE_HOLDING:
                    m_botCmd.buttons |= BUTTON_USE;
                    break;
                }

                // Check if defuse is complete
                if (level.time - m_fPlantDefuseStart >= BOT_PLANT_DEFUSE_TIME) {
                    dmManager.SetBombsPlanted(dmManager.GetBombsPlanted() - 1);
                    if (iDefuseSite >= 0) {
                        dmManager.SetBombSitePlanted(iDefuseSite, false);
                    }
                    m_iObjectiveState = BOT_OBJ_STATE_DEFENDING;
                    m_iUseState       = BOT_USE_AIMING;
                }
            } else {
                // Rush to nearest planted bomb (even during combat)
                m_iObjectiveState = BOT_OBJ_STATE_MOVING;
                movement.MoveTo(vDefusePos);
            }
        }
    }
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

Weapon *BotController::FindWeaponWithAmmo()
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
    m_iCuriousTime   = 0;
    m_botCmd.buttons = 0;

    // Recache bomb team assignment on respawn
    m_iObjectiveState = BOT_OBJ_STATE_NONE;
    m_bIsOnBombTeam   = false;
    if (dmManager.GetBombPlantTeam() == STRING_AXIS && controlledEnt->GetTeam() == TEAM_AXIS) {
        m_bIsOnBombTeam = true;
    } else if (dmManager.GetBombPlantTeam() == STRING_ALLIES && controlledEnt->GetTeam() == TEAM_ALLIES) {
        m_bIsOnBombTeam = true;
    }
}

void BotController::UpdateStrafeAndLean(void)
{
    if (!g_bot_strafe->integer) {
        return;
    }

    // Check if it's time to change strafe direction
    if (level.time >= m_fNextStrafeChangeTime) {
        // Flip strafe direction (always -1 or 1, never 0)
        m_iStrafeDirection = -m_iStrafeDirection;

        // Lean matches strafe 85% of the time
        if (rand() % 100 < g_bot_lean_match_chance->integer) {
            m_iLeanDirection = m_iStrafeDirection;
        } else {
            m_iLeanDirection = -m_iStrafeDirection;
        }

        // Calculate next change time
        float minTime = g_bot_strafe_min_time->value;
        float maxTime = g_bot_strafe_max_time->value;
        m_fNextStrafeChangeTime = level.time + minTime + G_Random(maxTime - minTime);
    }

    // Update shooting movement mode (varies between forward, forward/back, strafe-only)
    bool isAttacking = (m_StateFlags & 1);  // Attack is state 0
    if (isAttacking && level.time >= m_fNextShootMoveTime) {
        // Pick a random mode: 0=forward, 1=forward/back, 2=strafe only
        if (g_bot_shoot_bobbing->integer) {
            m_iShootMoveMode = rand() % 3;
        } else {
            // Skip mode 1 (forward/back) - only pick 0 or 2
            m_iShootMoveMode = (rand() % 2) * 2;
        }

        // For forward/back mode, pick initial direction
        if (m_iShootMoveMode == 1) {
            m_iShootMoveDirection = (rand() % 2) ? 1 : -1;
        }

        // Next mode change in 0.2-0.5s
        m_fNextShootMoveTime = level.time + 0.2f + G_Random(0.3f);
    }
}

void BotController::ApplyStrafeAndLean(void)
{
    if (!controlledEnt) {
        return;
    }

    // Skip on ladders
    if (controlledEnt->GetLadder()) {
        return;
    }

    if (!g_bot_strafe->integer) {
        return;
    }

    // ADD strafe to pathfinding movement (don't overwrite it)
    // Use reduced intensity so strafe doesn't overpower navigation
    int strafeAmount = m_iStrafeDirection * 64;
    int newRightMove = (int)m_botCmd.rightmove + strafeAmount;
    m_botCmd.rightmove = (signed char)Q_clamp(newRightMove, -127, 127);

    // Apply lean
    m_botCmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);
    if (m_iLeanDirection > 0) {
        m_botCmd.buttons |= BUTTON_LEAN_RIGHT;
    } else {
        m_botCmd.buttons |= BUTTON_LEAN_LEFT;
    }

    // Handle forward/backward movement when shooting
    bool isAttacking = (m_StateFlags & 1);
    if (isAttacking) {
        switch (m_iShootMoveMode) {
        case 0:
            // Forward only - keep existing forward movement
            break;
        case 1:
            // Forward/backward alternation
            m_botCmd.forwardmove = (signed char)(m_iShootMoveDirection * 127);
            // Flip direction for next frame
            m_iShootMoveDirection = -m_iShootMoveDirection;
            break;
        case 2:
            // Strafe only - no forward movement
            m_botCmd.forwardmove = 0;
            break;
        }
    }
}

void BotController::Think()
{
    usercmd_t  ucmd;
    usereyes_t eyeinfo;

    UpdateStrafeAndLean();
    UpdateBotStates();
    ApplyStrafeAndLean();
    GetUsercmd(&ucmd);
    GetEyeInfo(&eyeinfo);

    G_ClientThink(controlledEnt->edict, &ucmd, &eyeinfo);
}

void BotController::Killed(const Event& ev)
{
    Entity *attacker;

    // Release planter slot on death
    if (m_iBombSiteIndex >= 0) {
        dmManager.ReleaseBombSitePlanter(m_iBombSiteIndex, controlledEnt->entnum);
    }
    m_iObjectiveState = BOT_OBJ_STATE_NONE;
    m_iUseState       = BOT_USE_AIMING;

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

    Info_SetValueForKey(controlledEnt->client->pers.userinfo, "dm_playermodel", "allies_airborne");
    Info_SetValueForKey(controlledEnt->client->pers.userinfo, "dm_playergermanmodel", "german_winter_1");

    G_ClientUserinfoChanged(controlledEnt->edict, controlledEnt->client->pers.userinfo);
}

void BotController::GotKill(const Event& ev)
{
    ClearEnemy();
    m_iCuriousTime = 0;

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

            // Remove the controller, it will be recreated later to match `sv_numbots`
            delete controller;
            controllers.RemoveObjectAt(i);
        }
    }

    for (i = 1; i <= controllers.NumObjects(); i++) {
        BotController *controller = controllers.ObjectAt(i);
        controller->Think();
    }
}
