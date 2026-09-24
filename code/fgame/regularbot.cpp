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
// regularbot.cpp: Lightweight human-like SMG bot controller.

#include "g_local.h"
#include "regularbot.h"
#include "scriptexception.h"

RegularBotControllerManager regularBotControllerManager;

namespace {

signed char QuantizeMovement(signed char value)
{
    if (value > 24) {
        return 127;
    }
    if (value < -24) {
        return -127;
    }
    return 0;
}

int AimReactionDelay()
{
    return Q_clamp(sv_regularbot_aim_reaction_ms->integer, 0, 5000);
}

float AimLatency()
{
    return Q_clamp_float(sv_regularbot_aim_latency_ms->value, 0.0f, 2000.0f);
}

float AimTurnSpeed()
{
    return Q_clamp_float(sv_regularbot_aim_turnspeed->value, 0.0f, 1080.0f);
}

float AimErrorRadius()
{
    return Q_clamp_float(sv_regularbot_aim_error_deg->value, 0.0f, 45.0f);
}

} // namespace

RegularBotRotation::RegularBotRotation()
    : m_vTargetAngles(vec_zero)
    , m_vCurrentAngles(vec_zero)
{}

void RegularBotRotation::SetControlledEntity(Player *newEntity)
{
    controlledEntity = newEntity;
    Reset();
}

void RegularBotRotation::Reset()
{
    if (!controlledEntity) {
        m_vTargetAngles  = vec_zero;
        m_vCurrentAngles = vec_zero;
        return;
    }

    m_vCurrentAngles = controlledEntity->GetViewAngles();
    m_vTargetAngles  = m_vCurrentAngles;
}

void RegularBotRotation::TurnThink(
    usercmd_t& botcmd, usereyes_t& eyeinfo, float maxDegreesPerSecond
)
{
    if (!controlledEntity) {
        return;
    }

    m_vTargetAngles[PITCH] = Q_clamp_float(AngleNormalize180(m_vTargetAngles[PITCH]), -85.0f, 85.0f);
    m_vTargetAngles[YAW]   = AngleMod(m_vTargetAngles[YAW]);

    for (int axis = PITCH; axis <= YAW; axis++) {
        const float difference = AngleDelta(m_vTargetAngles[axis], m_vCurrentAngles[axis]);
        const float axisSpeed  = axis == PITCH ? maxDegreesPerSecond * 0.75f : maxDegreesPerSecond;
        const float maxStep    = axisSpeed * level.frametime;
        const float step       = Q_clamp_float(difference * 8.0f * level.frametime, -maxStep, maxStep);

        m_vCurrentAngles[axis] = AngleMod(m_vCurrentAngles[axis] + step);
    }

    float pitch = AngleNormalize180(m_vCurrentAngles[PITCH]);
    pitch       = Q_clamp_float(pitch, -85.0f, 85.0f);

    eyeinfo.angles[PITCH] = pitch;
    eyeinfo.angles[YAW]   = AngleMod(m_vCurrentAngles[YAW]);
    botcmd.angles[PITCH]  = ANGLE2SHORT(pitch) - controlledEntity->client->ps.delta_angles[PITCH];
    botcmd.angles[YAW]    = ANGLE2SHORT(eyeinfo.angles[YAW]) - controlledEntity->client->ps.delta_angles[YAW];
    botcmd.angles[ROLL]   = -controlledEntity->client->ps.delta_angles[ROLL];
}

void RegularBotRotation::SetTargetAngles(const Vector& angles)
{
    m_vTargetAngles = angles;
}

RegularBotController::RegularBotController()
    : m_iEnemyScanCursor(1)
    , m_iNextEnemyScanTime(0)
    , m_iReactionReadyTime(0)
    , m_iStopAimTime(0)
    , m_iStopFireTime(0)
    , m_bEnemyVisible(false)
    , m_vLastSeenPosition(vec_zero)
    , m_vTrackedAimPosition(vec_zero)
    , m_bTrackedAimPosition(false)
    , m_vAimError(vec_zero)
    , m_vAimErrorTarget(vec_zero)
    , m_iNextAimErrorTime(0)
    , m_iNextNavigationTime(0)
    , m_iNextNavigationRetryTime(0)
    , m_iStrafeDirection(0)
    , m_iNextReloadTime(0)
{
    memset(&m_botCmd, 0, sizeof(m_botCmd));
    memset(&m_botEyes, 0, sizeof(m_botEyes));
    m_botEyes.ofs[2] = DEFAULT_VIEWHEIGHT;
}

RegularBotController::~RegularBotController()
{
    if (controlledEnt) {
        controlledEnt->delegate_spawned.Remove(delegateHandle_spawned);
    }
}

void RegularBotController::ResetAimTracking(const Vector& targetPosition)
{
    m_vTrackedAimPosition = targetPosition;
    m_bTrackedAimPosition = true;
    RefreshAimErrorTarget();
    m_vAimError = m_vAimErrorTarget;
}

void RegularBotController::ClearEnemy()
{
    m_pEnemy              = NULL;
    m_bEnemyVisible       = false;
    m_iReactionReadyTime  = 0;
    m_iStopAimTime        = 0;
    m_iStopFireTime       = 0;
    m_iNextAimErrorTime   = 0;
    m_bTrackedAimPosition = false;
    m_vTrackedAimPosition = vec_zero;
    m_vAimError           = vec_zero;
    m_vAimErrorTarget     = vec_zero;
    m_vLastSeenPosition   = vec_zero;
}

bool RegularBotController::IsValidEnemy(Sentient *sent) const
{
    if (!sent || sent == controlledEnt || sent->hidden() || (sent->flags & FL_NOTARGET)) {
        return false;
    }
    if (sent->IsDead() || sent->getSolidType() == SOLID_NOT) {
        return false;
    }

    if (sent->IsSubclassOfPlayer()) {
        Player *player = static_cast<Player *>(sent);
        if (player->IsSpectator()) {
            return false;
        }
        if (g_gametype->integer >= GT_TEAM && player->GetTeam() == controlledEnt->GetTeam()) {
            return false;
        }
    } else if (sent->m_Team == controlledEnt->m_Team) {
        return false;
    }

    return true;
}

void RegularBotController::RefreshAimErrorTarget()
{
    const float yawRadius   = AimErrorRadius();
    const float pitchRadius = yawRadius * 0.7f;

    // Two samples favor small errors while preserving occasional clear misses.
    m_vAimErrorTarget[PITCH] = (G_CRandom(pitchRadius) + G_CRandom(pitchRadius)) * 0.5f;
    m_vAimErrorTarget[YAW]   = (G_CRandom(yawRadius) + G_CRandom(yawRadius)) * 0.5f;
    m_vAimErrorTarget[ROLL]  = 0.0f;

    m_iNextAimErrorTime = level.inttime + 800 + (int)G_Random(400.0f);
}

void RegularBotController::UpdateAimError()
{
    const float yawRadius   = AimErrorRadius();
    const float pitchRadius = yawRadius * 0.7f;

    if (yawRadius <= 0.0f) {
        m_vAimError           = vec_zero;
        m_vAimErrorTarget     = vec_zero;
        m_iNextAimErrorTime   = level.inttime + 1000;
        return;
    }

    m_vAimError[PITCH]       = Q_clamp_float(m_vAimError[PITCH], -pitchRadius, pitchRadius);
    m_vAimError[YAW]         = Q_clamp_float(m_vAimError[YAW], -yawRadius, yawRadius);
    m_vAimErrorTarget[PITCH] = Q_clamp_float(m_vAimErrorTarget[PITCH], -pitchRadius, pitchRadius);
    m_vAimErrorTarget[YAW]   = Q_clamp_float(m_vAimErrorTarget[YAW], -yawRadius, yawRadius);

    if (level.inttime >= m_iNextAimErrorTime) {
        RefreshAimErrorTarget();
    }

    const float frameTime = Q_max((float)level.intframetime, 1.0f);
    const float blend     = frameTime / (600.0f + frameTime);
    m_vAimError += (m_vAimErrorTarget - m_vAimError) * blend;
    m_vAimError[ROLL] = 0.0f;
}

Vector RegularBotController::GetLaggedAimPosition(const Vector& targetPosition)
{
    if (!m_bTrackedAimPosition) {
        m_vTrackedAimPosition = targetPosition;
        m_bTrackedAimPosition = true;
    }

    const float latency = AimLatency();
    if (latency <= 0.0f) {
        m_vTrackedAimPosition = targetPosition;
        return m_vTrackedAimPosition;
    }

    const float frameTime = Q_max((float)level.intframetime, 1.0f);
    const float blend     = frameTime / (latency + frameTime);
    m_vTrackedAimPosition += (targetPosition - m_vTrackedAimPosition) * blend;
    return m_vTrackedAimPosition;
}

void RegularBotController::UpdateEnemy()
{
    if (level.inttime < m_iNextEnemyScanTime) {
        return;
    }

    m_iNextEnemyScanTime = level.inttime + 100;
    const bool wasVisible = m_bEnemyVisible;
    m_bEnemyVisible       = false;

    float maxDistance = world->m_fAIVisionDistance;
    if (world->farplane_distance > 0.0f) {
        maxDistance = Q_min(maxDistance, world->farplane_distance * 0.828f);
    }
    const float maxDistanceSquared = Square(maxDistance);

    if (m_pEnemy && IsValidEnemy(m_pEnemy)
        && controlledEnt->CanSee(m_pEnemy, 360.0f, maxDistance, false)) {
        m_bEnemyVisible     = true;
        m_iStopAimTime      = level.inttime + 700;
        m_vLastSeenPosition = m_pEnemy->centroid;

        // A brief obstruction must not restart the new-target reaction delay.
        return;
    }

    if (wasVisible) {
        m_iStopFireTime = level.inttime + 150 + (int)G_Random(400.0f);
    }

    if (m_pEnemy && !IsValidEnemy(m_pEnemy)) {
        ClearEnemy();
    }

    // Inspect at most one viable candidate per update, including while a lost
    // target is remembered. This keeps acquisition to no more than one new
    // visibility trace per bot per 100 ms.
    const int numSentients = SentientList.NumObjects();
    for (int checked = 0; checked < numSentients; checked++) {
        if (m_iEnemyScanCursor > numSentients) {
            m_iEnemyScanCursor = 1;
        }

        Sentient *sent = SentientList.ObjectAt(m_iEnemyScanCursor++);
        if (sent == m_pEnemy || !IsValidEnemy(sent)) {
            continue;
        }
        if ((sent->origin - controlledEnt->origin).lengthSquared() > maxDistanceSquared) {
            continue;
        }
        if (!controlledEnt->CanSee(sent, 360.0f, maxDistance, false)) {
            break;
        }

        m_pEnemy              = sent;
        m_bEnemyVisible       = true;
        m_iStopAimTime        = level.inttime + 700;
        m_iReactionReadyTime  = level.inttime + AimReactionDelay();
        m_vLastSeenPosition   = sent->centroid;
        ResetAimTracking(sent->centroid);
        return;
    }

    if (m_pEnemy && level.inttime >= m_iStopAimTime) {
        ClearEnemy();
    }
}

void RegularBotController::UpdateNavigation()
{
    const bool routeActive = movement.IsMoving() && !movement.MoveDone();

    if (routeActive) {
        if (level.inttime < m_iNextNavigationTime) {
            return;
        }
    } else if (level.inttime < m_iNextNavigationRetryTime) {
        return;
    }

    m_iNextNavigationTime = level.inttime + 750 + (int)G_Random(250.0f);

    if (m_pEnemy && (m_bEnemyVisible || level.inttime < m_iStopAimTime)) {
        const Vector enemyPosition = m_bEnemyVisible ? m_pEnemy->origin : m_vLastSeenPosition;
        const float  distance      = (enemyPosition - controlledEnt->origin).length();

        if (m_bEnemyVisible && distance > 420.0f) {
            movement.MoveNear(enemyPosition, 256.0f);
        } else if (m_bEnemyVisible) {
            const Vector enemyAngles = (enemyPosition - controlledEnt->origin).toAngles();
            Vector forward;
            Vector left;
            enemyAngles.AngleVectorsLeft(&forward, &left);

            m_iStrafeDirection = -m_iStrafeDirection;
            const Vector preferred = m_iStrafeDirection < 0 ? left : -left;
            movement.AvoidPath(enemyPosition, Q_max(288.0f, distance + 96.0f), preferred);
        } else if (!m_bEnemyVisible) {
            movement.MoveTo(m_vLastSeenPosition);
        }

        m_iNextNavigationRetryTime = movement.IsMoving() && !movement.MoveDone()
            ? 0
            : level.inttime + 100;
        return;
    }

    if (routeActive) {
        return;
    }

    const float firstYaw = G_Random(360.0f);
    for (int attempt = 0; attempt < 4; attempt++) {
        Vector direction;
        Vector(0.0f, firstYaw + attempt * 90.0f, 0.0f).AngleVectorsLeft(&direction);

        if (movement.AvoidPath(
                controlledEnt->origin - direction * 64.0f,
                768.0f,
                direction,
                NULL,
                0.0f,
                320.0f
            )) {
            m_iNextNavigationRetryTime = 0;
            return;
        }
    }

    // Avoid retrying four failed path searches every server frame while still
    // keeping an isolated bot's visible stop short.
    m_iNextNavigationRetryTime = level.inttime + 200;
}

void RegularBotController::UpdateMovement()
{
    movement.MoveThink(m_botCmd);
    m_botCmd.forwardmove = QuantizeMovement(m_botCmd.forwardmove);
    m_botCmd.rightmove   = QuantizeMovement(m_botCmd.rightmove);
}

void RegularBotController::UpdateAimAndFire()
{
    const bool tracking = m_pEnemy && (m_bEnemyVisible || level.inttime < m_iStopAimTime);
    Vector targetPosition;

    if (tracking) {
        targetPosition = m_bEnemyVisible ? m_pEnemy->centroid : m_vLastSeenPosition;

        // Reaction delay holds both the previous view direction and fire.
        // Keep the lagged target state current so this delay remains separate
        // from the ongoing target-following latency.
        if (level.inttime < m_iReactionReadyTime) {
            GetLaggedAimPosition(targetPosition);
            UpdateAimError();
            rotation.TurnThink(m_botCmd, m_botEyes, 240.0f);
            return;
        }

        targetPosition = GetLaggedAimPosition(targetPosition);
        UpdateAimError();

        Vector direction = targetPosition - controlledEnt->EyePosition();
        if (direction.lengthSquared() > 0.001f) {
            direction.normalize();
            Vector targetAngles = direction.toAngles() + m_vAimError;
            rotation.SetTargetAngles(targetAngles);
        }
        rotation.TurnThink(m_botCmd, m_botEyes, AimTurnSpeed());
    } else {
        if (movement.IsMoving()) {
            Vector pathDirection = movement.GetCurrentPathDirection();
            if (pathDirection.lengthSquared() > 0.001f) {
                Vector pathAngles = pathDirection.toAngles();
                pathAngles[PITCH] = 0.0f;
                rotation.SetTargetAngles(pathAngles);
            }
        }
        rotation.TurnThink(m_botCmd, m_botEyes, 240.0f);
        return;
    }

    Weapon *weapon = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    if (!weapon || !(weapon->GetWeaponClass() & WEAPON_CLASS_SMG)) {
        return;
    }

    if (!weapon->HasAmmoInClip(FIRE_PRIMARY)) {
        if (weapon->HasAmmo(FIRE_PRIMARY)) {
            if (level.inttime >= m_iNextReloadTime) {
                controlledEnt->PlayerReload(NULL);
                m_iNextReloadTime = level.inttime + 250;
            }
            return;
        }

        // Empty SMGs stay equipped. Use only their normal secondary bash.
        if (m_bEnemyVisible && weapon->GetFireType(FIRE_SECONDARY) == FT_MELEE) {
            const float meleeRange = weapon->GetBulletRange(FIRE_SECONDARY);
            if ((m_pEnemy->origin - controlledEnt->origin).lengthSquared() <= Square(meleeRange)) {
                m_botCmd.buttons |= BUTTON_ATTACKRIGHT;
            }
        }
        return;
    }

    const bool canFire = m_bEnemyVisible || level.inttime < m_iStopFireTime;

    if (!canFire || level.inttime < m_iReactionReadyTime) {
        return;
    }

    m_botCmd.buttons |= BUTTON_ATTACKLEFT;
}

Weapon *RegularBotController::FindSmgWeapon() const
{
    Weapon               *bestWeapon = NULL;
    int                   bestRank   = -999999;
    const Container<int>& inventory  = controlledEnt->getInventory();

    for (int i = 1; i <= inventory.NumObjects(); i++) {
        Weapon *weapon = static_cast<Weapon *>(G_GetEntity(inventory.ObjectAt(i)));
        if (!weapon || !weapon->IsSubclassOfWeapon() || weapon->IsSubclassOfInventoryItem()) {
            continue;
        }
        if (!(weapon->GetWeaponClass() & WEAPON_CLASS_SMG) || weapon->GetRank() < bestRank) {
            continue;
        }

        bestWeapon = weapon;
        bestRank   = weapon->GetRank();
    }

    return bestWeapon;
}

void RegularBotController::CheckSmgWeapon()
{
    if (controlledEnt->GetNewActiveWeapon()) {
        return;
    }

    Weapon *active = controlledEnt->GetActiveWeapon(WEAPON_MAIN);
    if (active && (active->GetWeaponClass() & WEAPON_CLASS_SMG)) {
        return;
    }

    Weapon *smg = FindSmgWeapon();
    if (smg) {
        controlledEnt->useWeapon(smg, WEAPON_MAIN);
    }
}

void RegularBotController::UpdateLean()
{
    m_botCmd.buttons &= ~(BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);

    if (controlledEnt->GetLadder() || m_botCmd.upmove) {
        return;
    }

    if (m_botCmd.rightmove < 0) {
        m_botCmd.buttons |= BUTTON_LEAN_LEFT;
    } else if (m_botCmd.rightmove > 0) {
        m_botCmd.buttons |= BUTTON_LEAN_RIGHT;
    }
}

void RegularBotController::Think()
{
    if (!controlledEnt || !controlledEnt->edict->inuse) {
        return;
    }

    m_botCmd.serverTime  = level.svsTime;
    m_botCmd.forwardmove = 0;
    m_botCmd.rightmove   = 0;
    m_botCmd.upmove      = 0;

    if (!controlledEnt->client->pers.dm_primary[0]) {
        Event *event = new Event(EV_Player_PrimaryDMWeapon);
        event->AddString("smg");
        controlledEnt->ProcessEvent(event);

        if (!controlledEnt || !controlledEnt->edict->inuse) {
            return;
        }
    }

    if (controlledEnt->GetTeam() == TEAM_NONE || controlledEnt->GetTeam() == TEAM_SPECTATOR) {
        m_botCmd.buttons = 0;

        if (!controlledEnt->EventPending(EV_Player_AutoJoinDMTeam)) {
            controlledEnt->PostEvent(EV_Player_AutoJoinDMTeam, controlledEnt->entnum / 20.0f);
        }

        rotation.TurnThink(m_botCmd, m_botEyes, 240.0f);
        G_ClientThink(controlledEnt->edict, &m_botCmd, &m_botEyes);
        return;
    }

    if (controlledEnt->IsDead() || controlledEnt->IsSpectator()) {
        m_botCmd.buttons = (m_botCmd.buttons & BUTTON_ATTACKLEFT) ? 0 : BUTTON_ATTACKLEFT;
        rotation.TurnThink(m_botCmd, m_botEyes, 240.0f);
        G_ClientThink(controlledEnt->edict, &m_botCmd, &m_botEyes);
        return;
    }

    m_botCmd.buttons = BUTTON_RUN;
    m_botEyes.ofs[0] = 0.0f;
    m_botEyes.ofs[1] = 0.0f;
    m_botEyes.ofs[2] = controlledEnt->viewheight;

    CheckSmgWeapon();
    UpdateEnemy();
    UpdateNavigation();
    UpdateMovement();
    UpdateAimAndFire();
    UpdateLean();

    G_ClientThink(controlledEnt->edict, &m_botCmd, &m_botEyes);
}

void RegularBotController::Spawned()
{
    ClearEnemy();
    movement.ClearMove();
    rotation.Reset();

    m_botCmd.buttons           = 0;
    m_iEnemyScanCursor         = 1;
    m_iNextNavigationTime      = 0;
    m_iNextNavigationRetryTime = 0;
    m_iNextReloadTime          = 0;
    m_iStrafeDirection         = (rand() % 2) ? 1 : -1;

    const int stagger = controlledEnt ? (controlledEnt->entnum % 5) * 20 : 0;
    m_iNextEnemyScanTime = level.inttime + stagger;
}

void RegularBotController::setControlledEntity(Player *player)
{
    controlledEnt = player;
    movement.SetControlledEntity(player);
    rotation.SetControlledEntity(player);

    delegateHandle_spawned = player->delegate_spawned.Add(std::bind(&RegularBotController::Spawned, this));
    Spawned();
}

Player *RegularBotController::getControlledEntity() const
{
    return controlledEnt;
}

RegularBotControllerManager::~RegularBotControllerManager()
{
    Cleanup();
}

RegularBotController *RegularBotControllerManager::createController(Player *player)
{
    RegularBotController *controller = new RegularBotController();
    controller->setControlledEntity(player);
    controllers.AddObject(controller);
    return controller;
}

void RegularBotControllerManager::removeController(RegularBotController *controller)
{
    if (!controller) {
        return;
    }
    controllers.RemoveObject(controller);
    delete controller;
}

RegularBotController *RegularBotControllerManager::findController(Entity *ent)
{
    for (int i = 1; i <= controllers.NumObjects(); i++) {
        RegularBotController *controller = controllers.ObjectAt(i);
        if (controller->getControlledEntity() == ent) {
            return controller;
        }
    }
    return NULL;
}

const Container<RegularBotController *>& RegularBotControllerManager::getControllers() const
{
    return controllers;
}

void RegularBotControllerManager::Cleanup()
{
    for (int i = 1; i <= controllers.NumObjects(); i++) {
        delete controllers.ObjectAt(i);
    }
    controllers.FreeObjectList();
}

void RegularBotControllerManager::ThinkControllers()
{
    for (int i = controllers.NumObjects(); i > 0; i--) {
        RegularBotController *controller = controllers.ObjectAt(i);
        if (!controller->getControlledEntity()) {
            delete controller;
            controllers.RemoveObjectAt(i);
        }
    }

    for (int i = 1; i <= controllers.NumObjects(); i++) {
        RegularBotController *controller = controllers.ObjectAt(i);
        try {
            controller->Think();
        } catch (ScriptException& exc) {
            gi.DPrintf("REGULARBOT: think exception for bot %d: %s\n", i, exc.string.c_str());
        }

        if (!controller->getControlledEntity()) {
            delete controller;
            controllers.RemoveObjectAt(i--);
        }
    }
}
