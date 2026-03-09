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

// We assume that we have limited access to the server-side
// and that most logic come from the playerstate_s structure

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

    m_iCuriousTime        = 0;
    m_iAttackTime         = 0;
    m_iEnemyEyesTag       = -1;
    m_iContinuousFireTime = 0;
    m_iLastSeenTime       = 0;
    m_iLastUnseenTime     = 0;
    m_iLastBurstTime      = 0;

    m_iNextTauntTime = 0;

    m_StateFlags = 0;

    // Roomba — randomize directions per bot, fixed until respawn
    m_iRoombaTurnDir        = (rand() % 2) ? 1 : -1;
    m_fRoombaYaw            = 0;
    m_fRoombaTurnSpeed      = 4.0f + G_Random(4.0f);
    m_bAimOverride          = false;
    m_iStrafeDir            = (rand() % 2) ? 1 : -1;
    m_iNextStrafeSwitchTime = 0;

    m_bJump          = false;
    m_iJumpCheckTime = 0;
    m_vJumpLocation  = vec_zero;
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
        event->AddString(g_bot_primary_weapon->string[0] ? g_bot_primary_weapon->string : "auto");

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

    // Reset aim override flag — states set it if they need to aim at an enemy
    m_bAimOverride = false;

    CheckStates();

    // Pure roomba: always run forward, always strafe+lean, no pathfinding
    m_botCmd.forwardmove = 127;

    // Alternate strafe+lean direction periodically
    if (level.inttime >= m_iNextStrafeSwitchTime) {
        m_iStrafeDir            = -m_iStrafeDir;
        m_iNextStrafeSwitchTime = level.inttime + 2000 + (int)G_Random(3000);
    }

    m_botCmd.rightmove = (signed char)(m_iStrafeDir * 127);

    m_botCmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);
    if (m_iStrafeDir < 0) {
        m_botCmd.buttons |= BUTTON_LEAN_LEFT;
    } else {
        m_botCmd.buttons |= BUTTON_LEAN_RIGHT;
    }

    // Roomba yaw: ALWAYS turning one direction, never flipped.
    // The constant turning naturally rotates the bot out of corners.
    m_fRoombaYaw += m_iRoombaTurnDir * m_fRoombaTurnSpeed;
    m_fRoombaYaw = AngleMod(m_fRoombaYaw);

    if (!m_bAimOverride) {
        Vector angles;
        angles[PITCH] = 0;
        angles[YAW]   = m_fRoombaYaw;
        angles[ROLL]  = 0;
        rotation.SetTargetAngles(angles);
    }

    rotation.TurnThink(m_botCmd, m_botEyes);
    CheckUse();
    CheckObstacleJump();

    CheckValidWeapon();
}

void BotController::CheckUse(void)
{
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

void BotController::CheckObstacleJump(void)
{
    Vector  start;
    Vector  end;
    Vector  dir;
    Vector  delta;
    trace_t trace;

    // Toggle use on ladders so bots climb
    if (controlledEnt->GetLadder()) {
        m_botCmd.upmove = m_botCmd.upmove ? 0 : 127;
        return;
    }

    // Don't jump while airborne
    if (!controlledEnt->groundentity && !controlledEnt->client->ps.walking) {
        m_bJump = false;
        return;
    }

    // Use the bot's facing direction
    controlledEnt->angles.AngleVectorsLeft(&dir);
    dir[2] = 0;
    VectorNormalize2D(dir);

    // Trace forward at step height to detect obstacles
    start = controlledEnt->origin + Vector(0, 0, STEPSIZE);
    end   = start + dir * (controlledEnt->maxs.y - controlledEnt->mins.y);

    trace = G_Trace(
        start, controlledEnt->mins, controlledEnt->maxs, end,
        controlledEnt, MASK_PLAYERSOLID, false, "BotController::CheckObstacleJump"
    );

    if (!trace.startsolid && trace.fraction > 0.5f) {
        // Path is clear — check for edge jumps instead
        m_bJump = false;

        // Edge detection: is there a void ahead?
        start = controlledEnt->origin + Vector(0, 0, STEPSIZE)
              + dir * (controlledEnt->maxs.y - controlledEnt->mins.y);
        end   = start - Vector(0, 0, STEPSIZE * 2);

        trace = G_Trace(
            start, controlledEnt->mins, controlledEnt->maxs, end,
            controlledEnt, MASK_PLAYERSOLID, false, "BotController::CheckObstacleJump"
        );

        if (trace.fraction != 1.0f) {
            // Ground exists, no edge
            return;
        }

        // Void below — check if there's a landing spot ahead
        end = start + dir * controlledEnt->GetRunSpeed() / 2.0f;
        end -= Vector(0, 0, STEPSIZE * 2);

        trace = G_Trace(
            start, controlledEnt->mins, controlledEnt->maxs, end,
            controlledEnt, MASK_PLAYERSOLID, false, "BotController::CheckObstacleJump"
        );

        if (trace.fraction < 1.0f) {
            // Edge with landing — jump over it
            m_botCmd.upmove = m_botCmd.upmove ? 0 : 127;
        }
        return;
    }

    // Obstacle detected — check if bot can jump over it
    start = controlledEnt->origin;
    end   = controlledEnt->origin;
    end.z += STEPSIZE * 3;
    end.z += STEPSIZE / 1.5f;

    trace = G_Trace(
        start, controlledEnt->mins, controlledEnt->maxs, end,
        controlledEnt, MASK_PLAYERSOLID, true, "BotController::CheckObstacleJump"
    );

    // Check if bot can move forward at jump height
    start = trace.endpos;
    end   = trace.endpos + dir * (controlledEnt->maxs.y - controlledEnt->mins.y);

    Vector bounds[2];
    bounds[0] = Vector(controlledEnt->mins[0], controlledEnt->mins[1], 0);
    bounds[1] = Vector(
        controlledEnt->maxs[0], controlledEnt->maxs[1],
        (controlledEnt->maxs[0] + controlledEnt->maxs[1]) * 0.5f
    );

    trace = G_Trace(
        start, bounds[0], bounds[1], end,
        controlledEnt, MASK_PLAYERSOLID, false, "BotController::CheckObstacleJump"
    );

    if (trace.plane.normal[2] <= MIN_WALK_NORMAL && trace.fraction < 1) {
        m_bJump = false;
        return;
    }

    // State machine: wait 100ms before executing jump
    if (!m_bJump) {
        m_bJump          = true;
        m_iJumpCheckTime = level.inttime;
        m_vJumpLocation  = controlledEnt->origin;
    } else if (level.inttime > m_iJumpCheckTime + 100) {
        m_bJump = false;

        delta = m_vJumpLocation - controlledEnt->origin;
        if (delta.lengthSquared() < Square(32)) {
            m_botCmd.upmove = 127;
        }
    }
}

void BotController::CheckValidWeapon()
{
    Weapon *weapon = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    if (!weapon) {
        // If holstered, use the best weapon available
        UseWeaponWithAmmo();
    } else if (!(weapon->GetWeaponClass() & WEAPON_CLASS_PISTOL)) {
        // Added in OPM
        //  Force bots to switch to pistol (melee-only behavior)
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

    switch (iType) {
    case AI_EVENT_MISC:
    case AI_EVENT_MISC_LOUD:
        break;
    case AI_EVENT_WEAPON_FIRE:
    case AI_EVENT_WEAPON_IMPACT:
    case AI_EVENT_EXPLOSION:
    case AI_EVENT_AMERICAN_VOICE:
    case AI_EVENT_GERMAN_VOICE:
    case AI_EVENT_AMERICAN_URGENT:
    case AI_EVENT_GERMAN_URGENT:
    case AI_EVENT_FOOTSTEP:
    case AI_EVENT_GRENADE:
    default:
        m_iCuriousTime   = level.inttime + 20000;
        m_vNewCuriousPos = vPos;
        break;
    }
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
    m_iCuriousTime  = 0;
    m_iAttackTime   = 0;
    m_vOldEnemyPos  = vec_zero;
    m_vLastEnemyPos = vec_zero;
    m_pEnemy        = NULL;
    m_iEnemyEyesTag = -1;
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
    if (CheckWindows()) {
        m_botCmd.buttons ^= BUTTON_ATTACKLEFT;
        m_iLastFireTime = level.inttime;
    } else {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
        CheckReload();
    }

    // Roomba yaw is applied globally in UpdateBotStates
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
    if (CheckWindows()) {
        m_botCmd.buttons ^= BUTTON_ATTACKLEFT;
        m_iLastFireTime = level.inttime;
    } else {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
    }

    // Roomba yaw is applied globally in UpdateBotStates
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
    float maxDistance = Q_min(world->m_fAIVisionDistance, world->farplane_distance * 0.828);

    // Keep current target if still valid and visible — prevents oscillation
    if (m_pEnemy && IsValidEnemy(m_pEnemy)
        && controlledEnt->CanSee(m_pEnemy, 360, maxDistance, false)) {
        m_vLastEnemyPos = m_pEnemy->origin;
        m_iAttackTime   = level.inttime + 1000;
        return true;
    }

    // Current target lost — scan for a new one (closest first)
    Container<Sentient *> sents = SentientList;

    bot_origin = controlledEnt->origin;
    sents.Sort(sentients_compare);

    for (int i = 1; i <= sents.NumObjects(); i++) {
        Sentient *sent = sents.ObjectAt(i);

        if (!IsValidEnemy(sent)) {
            continue;
        }

        if (controlledEnt->CanSee(sent, 360, maxDistance, false)) {
            if (!m_pEnemy) {
                m_iLastUnseenTime = level.inttime;
            }

            m_pEnemy        = sent;
            m_iEnemyEyesTag = -1;
            m_vLastEnemyPos = m_pEnemy->origin;
            m_iAttackTime   = level.inttime + 1000;
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
    bool    bMelee  = false;
    bool    bCanSee = false;
    Weapon *pWeap   = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    bool    bFiring = false;

    if (!m_pEnemy || !IsValidEnemy(m_pEnemy)) {
        // Ignore dead enemies
        m_iAttackTime = 0;
        return;
    }
    float fDistanceSquared = (m_pEnemy->origin - controlledEnt->origin).lengthSquared();

    m_vOldEnemyPos = m_vLastEnemyPos;

    bCanSee =
        controlledEnt->CanSee(m_pEnemy, 360, Q_min(world->m_fAIVisionDistance, world->farplane_distance * 0.828), false);

    if (bCanSee) {
        if (!pWeap) {
            return;
        }

        // No reaction delay - bots attack instantly
        m_iLastUnseenTime = 0;

        float fPrimaryBulletRange          = pWeap->GetBulletRange(FIRE_PRIMARY) / 1.25f;
        float fPrimaryBulletRangeSquared   = fPrimaryBulletRange * fPrimaryBulletRange;
        float fSecondaryBulletRange        = pWeap->GetBulletRange(FIRE_SECONDARY);
        float fSecondaryBulletRangeSquared = fSecondaryBulletRange * fSecondaryBulletRange;

        if (controlledEnt->client->ps.stats[STAT_AMMO] <= 0
            && controlledEnt->client->ps.stats[STAT_CLIPAMMO] <= 0) {
            m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
            controlledEnt->ZoomOff();
        } else if (fDistanceSquared > fPrimaryBulletRangeSquared) {
            m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);
            controlledEnt->ZoomOff();
        } else {
            float     fSpreadFactor = pWeap->GetSpreadFactor(FIRE_PRIMARY);

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
                }
            } else {
                bFiring = true;
                m_botCmd.buttons |= BUTTON_ATTACKLEFT;
            }
        }

        // Burst fire
        const int fireDelay            = pWeap->FireDelay(FIRE_PRIMARY) * 1000;
        const int maxcontinuousFireTime = fireDelay + g_bot_attack_continuousfire_min_firetime->value * 1000
                                       + G_Random(g_bot_attack_continuousfire_random_firetime->value * 1000);
        const int maxBurstTime = fireDelay + g_bot_attack_burst_min_time->value * 1000
                               + G_Random(g_bot_attack_burst_random_delay->value * 1000);

        if (m_iLastBurstTime) {
            if (level.inttime > m_iLastBurstTime + maxBurstTime) {
                m_iLastBurstTime      = 0;
                m_iContinuousFireTime = 0;
            } else {
                m_botCmd.buttons &= ~BUTTON_ATTACKLEFT;
            }
        } else {
            if (bFiring) {
                m_iContinuousFireTime += level.intframetime;
            } else {
                m_iContinuousFireTime = 0;
            }

            if (!m_iLastBurstTime && m_iContinuousFireTime > maxcontinuousFireTime) {
                m_iLastBurstTime      = level.inttime;
                m_iContinuousFireTime = 0;
            }
        }

        m_iLastFireTime = level.inttime;

        if (pWeap->GetFireType(FIRE_SECONDARY) == FT_MELEE) {
            if (controlledEnt->client->ps.stats[STAT_AMMO] <= 0
                && controlledEnt->client->ps.stats[STAT_CLIPAMMO] <= 0) {
                bMelee = true;
            } else if (fDistanceSquared <= fSecondaryBulletRangeSquared) {
                bMelee = true;
            }
        }

        // Force melee-only: bots never shoot, only bash/melee
        m_botCmd.buttons &= ~BUTTON_ATTACKLEFT;
        if (pWeap->GetFireType(FIRE_SECONDARY) == FT_MELEE
            && fDistanceSquared <= fSecondaryBulletRangeSquared) {
            m_botCmd.buttons ^= BUTTON_ATTACKRIGHT;
        } else {
            m_botCmd.buttons &= ~BUTTON_ATTACKRIGHT;
        }

        m_iAttackTime        = level.inttime + 1000;
        m_iAttackStopAimTime = level.inttime + 3000;
        m_iLastSeenTime      = level.inttime;
        m_vLastEnemyPos      = m_pEnemy->origin;
    } else {
        m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT);

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
            vTarget = eyes_or.origin;
        } else {
            vTarget = m_pEnemy->origin;
        }

        // Instant lock — tell UpdateBotStates to skip roomba yaw this frame
        rotation.AimAt(vTarget);
        m_bAimOverride = true;

        // Keep roomba yaw synced to current facing so there's no snap when aim releases
        m_fRoombaYaw = controlledEnt->angles[YAW];
    } else {
        // Can't see enemy and aim timer expired — clear enemy, roomba yaw
        // continues from UpdateBotStates automatically
        ClearEnemy();
        return;
    }

    // Keep the attack timer alive while we have an enemy
    m_iAttackTime = level.inttime + 1000;
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
    func->ThinkState     = &BotController::State_Grenade;
}

bool BotController::CheckCondition_Grenade(void)
{
    // FIXME: TODO
    return false;
}

void BotController::State_Grenade(void)
{
    // FIXME: TODO
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

        if (!next) {
            continue;
        }
        if (!next->IsSubclassOfWeapon() || next->IsSubclassOfInventoryItem()) {
            continue;
        }

        if (next->GetWeaponClass() & WEAPON_CLASS_THROWABLE) {
            continue;
        }

        // Added in OPM
        //  Bots only use pistol-class weapons
        if (!(next->GetWeaponClass() & WEAPON_CLASS_PISTOL)) {
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

Weapon *BotController::FindMeleeWeapon()
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

        if (!next) {
            continue;
        }
        if (!next->IsSubclassOfWeapon() || next->IsSubclassOfInventoryItem()) {
            continue;
        }

        // Added in OPM
        //  Bots only use pistol-class weapons for melee
        if (!(next->GetWeaponClass() & WEAPON_CLASS_PISTOL)) {
            continue;
        }

        if (next->GetRank() < bestrank) {
            continue;
        }

        if (next->GetFireType(FIRE_SECONDARY) != FT_MELEE) {
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
    if (!bestWeapon) {
        //
        // If there is no weapon with ammo, fallback to a weapon that can melee
        //
        bestWeapon = FindMeleeWeapon();
    }

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

    // Initialize roomba yaw from current facing so we don't snap
    if (controlledEnt) {
        m_fRoombaYaw = controlledEnt->angles[YAW];
    }
    // Re-randomize directions on each spawn, fixed until next respawn
    m_iRoombaTurnDir   = (rand() % 2) ? 1 : -1;
    m_fRoombaTurnSpeed = 4.0f + G_Random(4.0f);
    m_iStrafeDir            = (rand() % 2) ? 1 : -1;
    m_iNextStrafeSwitchTime = 0;
}

void BotController::Think()
{
    if (!controlledEnt) {
        return;
    }

    usercmd_t  ucmd;
    usereyes_t eyeinfo;

    UpdateBotStates();
    GetUsercmd(&ucmd);
    GetEyeInfo(&eyeinfo);

    G_ClientThink(controlledEnt->edict, &ucmd, &eyeinfo);
}

void BotController::Killed(const Event& ev)
{
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

    // Choose a new random primary weapon
    Event event(EV_Player_PrimaryDMWeapon);
    event.AddString(g_bot_primary_weapon->string[0] ? g_bot_primary_weapon->string : "auto");

    controlledEnt->ProcessEvent(event);

    //
    // This is useful to change nationality in Spearhead and Breakthrough
    // this allows the AI to use more weapons
    //
    Info_SetValueForKey(controlledEnt->client->pers.userinfo, "dm_playermodel", G_GetRandomAlliedPlayerModel());
    Info_SetValueForKey(controlledEnt->client->pers.userinfo, "dm_playergermanmodel", G_GetRandomGermanPlayerModel());

    G_ClientUserinfoChanged(controlledEnt->edict, controlledEnt->client->pers.userinfo);
}

void BotController::GotKill(const Event& ev)
{
    ClearEnemy();
    m_iCuriousTime = 0;

    // Disabled in OPM: bots should never send taunts/instamsg chatter.
    // Keep timer state reset so re-enabling in the future won't burst.
    m_iNextTauntTime = level.inttime + g_bot_instamsg_delay->integer;
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
        try {
            controller->Think();
        } catch (ScriptException& exc) {
            gi.DPrintf("*** Bot Think Exception *** bot %d: %s\n", i, exc.string.c_str());
        }
    }
}
