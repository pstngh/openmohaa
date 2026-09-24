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
// playerbot.cpp: Multiplayer roomba bot system.

#include "g_local.h"
#include "playerbot.h"
#include "scriptexception.h"
#include "g_bot.h"

// We assume that we have limited access to the server-side
// and that most logic come from the playerstate_s structure

CLASS_DECLARATION(Listener, BotController, NULL) {
    {NULL, NULL}
};

BotController::BotController()
{
    memset(&m_botCmd, 0, sizeof(m_botCmd));
    memset(&m_botEyes, 0, sizeof(m_botEyes));

    m_botEyes.ofs[0]    = 0;
    m_botEyes.ofs[1]    = 0;
    m_botEyes.ofs[2]    = DEFAULT_VIEWHEIGHT;

    m_iStopAimTime       = 0;
    m_iNextEnemyScanTime = 0;
    m_iEnemyScanCursor   = 1;
    m_bEnemyVisible      = false;
    m_iNextMeleeTime     = 0;

    // Roomba — randomize directions per bot, fixed until respawn
    m_iRoombaTurnDir        = (rand() % 2) ? 1 : -1;
    m_fRoombaYaw            = 0;
    m_fRoombaTurnSpeed      = 80.0f + G_Random(80.0f);
    m_bAimOverride          = false;
    m_iStrafeDir            = (rand() % 2) ? 1 : -1;
    m_iNextStrafeSwitchTime = 0;
    m_iNextMovementCheckTime = 0;

    m_bJump          = false;
    m_iJumpCheckTime = 0;
    m_vJumpLocation  = vec_zero;
}

BotController::~BotController()
{
    if (controlledEnt) {
        controlledEnt->delegate_spawned.Remove(delegateHandle_spawned);
    }
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
    m_botCmd.serverTime  = level.svsTime;
    m_botCmd.forwardmove = 0;
    m_botCmd.rightmove   = 0;
    m_botCmd.upmove      = 0;

    if (!controlledEnt->client->pers.dm_primary[0]) {
        Event *event;

        //
        // Primary weapon
        //
        event = new Event(EV_Player_PrimaryDMWeapon);
        event->AddString("auto");

        controlledEnt->ProcessEvent(event);

        // The entity may have been removed by the event
        if (!controlledEnt || !controlledEnt->edict->inuse) {
            return;
        }
    }

    if (controlledEnt->GetTeam() == TEAM_NONE || controlledEnt->GetTeam() == TEAM_SPECTATOR) {
        float time;

        m_botCmd.buttons = 0;

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
        m_botCmd.buttons = (m_botCmd.buttons & BUTTON_ATTACKLEFT) ? 0 : BUTTON_ATTACKLEFT;
        return;
    }

    m_botCmd.buttons |= BUTTON_RUN;
    m_botCmd.buttons &= ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT | BUTTON_USE);

    m_botEyes.ofs[0]    = 0;
    m_botEyes.ofs[1]    = 0;
    m_botEyes.ofs[2]    = controlledEnt->viewheight;
    m_botEyes.angles[0] = 0;
    m_botEyes.angles[1] = 0;

    // Reset aim override flag — states set it if they need to aim at an enemy
    m_bAimOverride = false;

    UpdateEnemy();
    UpdateAimAndMelee();

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
    m_fRoombaYaw += m_iRoombaTurnDir * m_fRoombaTurnSpeed * level.frametime;
    m_fRoombaYaw = AngleMod(m_fRoombaYaw);

    if (!m_bAimOverride) {
        Vector angles;
        angles[PITCH] = 0;
        angles[YAW]   = m_fRoombaYaw;
        angles[ROLL]  = 0;
        rotation.SetTargetAngles(angles);
    }

    rotation.TurnThink(m_botCmd, m_botEyes);

    if (controlledEnt->GetLadder()) {
        m_botCmd.upmove = 127;
    }

    // Door, ladder, obstacle and edge traces do not need frame-rate cadence.
    // Staggering by client slot keeps the five bots from tracing together.
    if (level.inttime >= m_iNextMovementCheckTime) {
        m_iNextMovementCheckTime = level.inttime + 100;
        CheckUse();
        CheckObstacleJump();
    }

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

void BotController::CheckObstacleJump(void)
{
    Vector  start;
    Vector  end;
    Vector  dir;
    Vector  delta;
    trace_t trace;

    // Keep moving upward on ladders.
    if (controlledEnt->GetLadder()) {
        m_botCmd.upmove = 127;
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
            m_botCmd.upmove = 127;
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

    // Do not rescan the inventory while a previously requested switch is pending.
    if (controlledEnt->GetNewActiveWeapon()) {
        return;
    }

    if (weapon && (weapon->GetWeaponClass() & WEAPON_CLASS_PISTOL)
        && weapon->GetFireType(FIRE_SECONDARY) == FT_MELEE) {
        return;
    }

    Weapon *meleeWeapon = FindMeleeWeapon();
    if (meleeWeapon && meleeWeapon != weapon) {
        controlledEnt->useWeapon(meleeWeapon, WEAPON_MAIN);
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
    m_pEnemy         = NULL;
    m_iStopAimTime   = 0;
    m_bEnemyVisible  = false;
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

void BotController::UpdateEnemy(void)
{
    // Visibility does not need frame-rate cadence. Each bot's initial deadline
    // is staggered by client slot, avoiding synchronized trace bursts.
    if (level.inttime < m_iNextEnemyScanTime) {
        return;
    }

    m_iNextEnemyScanTime = level.inttime + 100;
    m_bEnemyVisible      = false;

    float maxDistance = world->m_fAIVisionDistance;
    if (world->farplane_distance > 0.0f) {
        maxDistance = Q_min(maxDistance, world->farplane_distance * 0.828f);
    }
    const float maxDistanceSquared = maxDistance * maxDistance;

    if (m_pEnemy && IsValidEnemy(m_pEnemy)
        && controlledEnt->CanSee(m_pEnemy, 360, maxDistance, false)) {
        m_bEnemyVisible = true;
        m_iStopAimTime  = level.inttime + 3000;
        return;
    }

    if (m_pEnemy && !IsValidEnemy(m_pEnemy)) {
        ClearEnemy();
    }

    // Inspect at most one viable candidate per update. This bounds each bot to
    // one reacquisition trace instead of scanning and sorting every sentient.
    const int numSentients = SentientList.NumObjects();
    for (int checked = 0; checked < numSentients; checked++) {
        if (m_iEnemyScanCursor > numSentients) {
            m_iEnemyScanCursor = 1;
        }

        Sentient *sent = SentientList.ObjectAt(m_iEnemyScanCursor++);
        if (sent == m_pEnemy || !IsValidEnemy(sent)) {
            continue;
        }

        if ((sent->origin - controlledEnt->origin).lengthSquared()
            > maxDistanceSquared) {
            continue;
        }

        if (!controlledEnt->CanSee(sent, 360, maxDistance, false)) {
            break;
        }

        m_pEnemy         = sent;
        m_bEnemyVisible  = true;
        m_iStopAimTime   = level.inttime + 3000;
        return;
    }

    if (!m_pEnemy || level.inttime >= m_iStopAimTime) {
        ClearEnemy();
    }
}

void BotController::UpdateAimAndMelee(void)
{
    if (!m_pEnemy || !IsValidEnemy(m_pEnemy)) {
        ClearEnemy();
        return;
    }

    if (m_bEnemyVisible) {
        Weapon *weapon = controlledEnt->GetActiveWeapon(WEAPON_MAIN);

        if (weapon && weapon->GetFireType(FIRE_SECONDARY) == FT_MELEE) {
            const float meleeRange = weapon->GetBulletRange(FIRE_SECONDARY);
            const float distanceSquared =
                (m_pEnemy->origin - controlledEnt->origin).lengthSquared();

            if (distanceSquared <= meleeRange * meleeRange
                && level.inttime >= m_iNextMeleeTime) {
                m_botCmd.buttons |= BUTTON_ATTACKRIGHT;
                m_iNextMeleeTime = level.inttime + 1000;
            }
        }
    }

    // Preserve the existing short aim hold after line of sight is lost.
    if (!m_bEnemyVisible && level.inttime >= m_iStopAimTime) {
        ClearEnemy();
        return;
    }

    rotation.AimAt(m_pEnemy->centroid);
    m_bAimOverride = true;

    // Resume the roomba turn from the current view when target tracking ends.
    m_fRoombaYaw = controlledEnt->angles[YAW];
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

    // Select the highest-ranked pistol with a secondary melee attack.
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

void BotController::Spawned(void)
{
    ClearEnemy();
    m_iEnemyScanCursor   = 1;
    m_iNextMeleeTime     = 0;
    m_botCmd.buttons     = 0;
    m_bJump              = false;
    m_iJumpCheckTime     = 0;
    m_vJumpLocation      = vec_zero;

    // Initialize roomba yaw from current facing so we don't snap
    if (controlledEnt) {
        m_fRoombaYaw = controlledEnt->angles[YAW];
    }
    // Re-randomize directions on each spawn, fixed until next respawn
    m_iRoombaTurnDir   = (rand() % 2) ? 1 : -1;
    m_fRoombaTurnSpeed = 80.0f + G_Random(80.0f);
    m_iStrafeDir            = (rand() % 2) ? 1 : -1;
    m_iNextStrafeSwitchTime = 0;

    const int stagger = controlledEnt ? (controlledEnt->entnum % 5) * 20 : 0;
    m_iNextEnemyScanTime    = level.inttime + stagger;
    m_iNextMovementCheckTime = level.inttime + stagger;
}

void BotController::Think()
{
    if (!controlledEnt) {
        return;
    }

    usercmd_t  ucmd;
    usereyes_t eyeinfo;

    UpdateBotStates();

    // The entity may have been invalidated during UpdateBotStates
    if (!controlledEnt || !controlledEnt->edict->inuse) {
        return;
    }

    GetUsercmd(&ucmd);
    GetEyeInfo(&eyeinfo);

    G_ClientThink(controlledEnt->edict, &ucmd, &eyeinfo);
}

void BotController::setControlledEntity(Player *player)
{
    controlledEnt = player;
    rotation.SetControlledEntity(player);
    m_fRoombaYaw             = player->angles[YAW];
    const int stagger        = (player->entnum % 5) * 20;
    m_iNextEnemyScanTime     = level.inttime + stagger;
    m_iNextMovementCheckTime = level.inttime + stagger;

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

void BotControllerManager::Cleanup()
{
    int i;

    for (i = 1; i <= controllers.NumObjects(); i++) {
        BotController *controller = controllers.ObjectAt(i);
        delete controller;
    }

    controllers.FreeObjectList();
}

void BotControllerManager::ThinkControllers()
{
    int i;

    // Delete controllers that don't have associated player entity.
    // This can happen if a script removes the entity or if a ProcessEvent
    // invalidated it during the previous frame.
    for (i = controllers.NumObjects(); i > 0; i--) {
        BotController *controller = controllers.ObjectAt(i);
        if (!controller->getControlledEntity()) {
            gi.DPrintf("BOT: orphan controller %d has no player entity, removing\n", i);

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
            gi.DPrintf("BOT: *** Think Exception *** bot %d: %s\n", i, exc.string.c_str());
        }

        // If the entity was invalidated during Think, clean up immediately
        // rather than waiting for the orphan pass on the next frame.
        if (!controller->getControlledEntity()) {
            gi.DPrintf("BOT: bot %d entity invalidated during Think, removing controller\n", i);
            delete controller;
            controllers.RemoveObjectAt(i);
            i--;
        }
    }
}
