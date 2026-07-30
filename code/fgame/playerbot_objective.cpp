/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

Objective-mode bot coordination. Bomb state remains owned by the map script:
bots discover the script model/trigger relationship, read its live/exploded
variables, and interact through the same use command as a human player.
===========================================================================
*/

#include "g_local.h"
#include "dm_manager.h"
#include "movement_telemetry.h"
#include "playerbot.h"
#include "playerstart.h"
#include "trigger.h"
#include "../script/scriptvariable.h"

static const int   BOT_OBJECTIVE_USE_TIMEOUT_MSEC = 10000;
static const int   BOT_OBJECTIVE_REAPPROACH_MSEC  = 2500;
static const int   BOT_OBJECTIVE_STALL_MSEC       = 5000;
static const float BOT_OBJECTIVE_PROGRESS_UNITS   = 64.0f;
// Match the player's 64-unit use trace plus its 16-unit endpoint scan.
static const float BOT_OBJECTIVE_USE_RANGE        = 80.0f;
// Bomb models can occupy the center nav point, so stop at a reachable point nearby.
static const float BOT_OBJECTIVE_APPROACH_RADIUS  = 64.0f;
// After a rejected interaction, move close enough to clear nearby geometry.
static const float BOT_OBJECTIVE_RETRY_APPROACH_RADIUS = 24.0f;
static const float BOT_OBJECTIVE_DEFAULT_USE_FOV        = 30.0f;
static const float BOT_OBJECTIVE_DEFAULT_USE_DISTANCE   = 128.0f;
// Generated hold/patrol points are approximate; accept nearby navigation.
static const float BOT_OBJECTIVE_PATROL_SEARCH_RADIUS = 128.0f;
// The bomb bridge is scripted and absent from the static navigation graph.
static const float BOT_OBJECTIVE_TEAM4_DIRECT_MAX_DISTANCE = 384.0f;
static const float BOT_OBJECTIVE_TEAM4_DIRECT_RADIUS       = 48.0f;

void bot_objective_site_t::Clear()
{
    explosive = NULL;
    trigger   = NULL;
    user      = NULL;
}

static int BotObjectiveVariable(Entity *entity, const char *name)
{
    if (!entity) {
        return 0;
    }

    ScriptVariable *variable = entity->Vars()->GetVariable(name);
    return variable ? variable->intValue() : 0;
}

static float BotObjectiveLevelVariable(const char *name, float fallback)
{
    ScriptVariable *variable = level.vars->GetVariable(name);
    return variable ? variable->floatValue() : fallback;
}

static float BotObjectiveUseFov()
{
    return BotObjectiveLevelVariable("bombusefov", BOT_OBJECTIVE_DEFAULT_USE_FOV);
}

static float BotObjectiveUseDistance()
{
    return BotObjectiveLevelVariable("bomb_use_distance", BOT_OBJECTIVE_DEFAULT_USE_DISTANCE);
}

static bool BotObjectiveCanSee(Player *player, Entity *explosive, float fov)
{
    return player && explosive
        && player->CanSee(explosive, fov, BotObjectiveUseDistance(), false);
}

static bool BotObjectiveInUseRange(Player *player, Entity *trigger)
{
    if (!player || !trigger) {
        return false;
    }

    const Vector eye             = player->EyePosition();
    float        distanceSquared = 0.0f;

    for (int axis = 0; axis < 3; ++axis) {
        float distance = 0.0f;
        if (eye[axis] < trigger->absmin[axis]) {
            distance = trigger->absmin[axis] - eye[axis];
        } else if (eye[axis] > trigger->absmax[axis]) {
            distance = eye[axis] - trigger->absmax[axis];
        }
        distanceSquared += distance * distance;
    }

    return distanceSquared <= Square(BOT_OBJECTIVE_USE_RANGE);
}

static bool BotObjectiveIsTeam4()
{
    return !Q_stricmp(level.mapname.c_str(), "obj/obj_team4");
}

bool BotManager::ObjectiveModeActive() const
{
    return g_gametype->integer == GT_OBJECTIVE && dmManager.RoundActive()
        && GetObjectivePlantTeam() != TEAM_NONE;
}

teamtype_t BotManager::GetObjectivePlantTeam() const
{
    if (dmManager.GetBombPlantTeam() == STRING_ALLIES) {
        return TEAM_ALLIES;
    }
    if (dmManager.GetBombPlantTeam() == STRING_AXIS) {
        return TEAM_AXIS;
    }
    return TEAM_NONE;
}

void BotManager::ClearObjectiveSites()
{
    for (int i = 0; i < MAX_BOT_OBJECTIVE_SITES; ++i) {
        objectiveSites[i].Clear();
    }

    objectiveSiteCount    = 0;
    objectiveSitesScanned = false;
}

void BotManager::UpdateObjectiveRound()
{
    if (g_gametype->integer != GT_OBJECTIVE) {
        objectiveRoundActive = false;
        return;
    }

    const bool roundActive = dmManager.RoundActive();
    if (roundActive && !objectiveRoundActive) {
        ++objectiveRound;
        objectiveSeed         = static_cast<int>(G_Random(32768.0f));
        nextObjectiveScanTime = 0;
        ClearObjectiveSites();
        ClearTeamContacts();
    } else if (!roundActive && objectiveRoundActive) {
        for (int i = 0; i < objectiveSiteCount; ++i) {
            objectiveSites[i].user = NULL;
        }
    }

    objectiveRoundActive = roundActive;
}

void BotManager::DiscoverObjectiveSites()
{
    ClearObjectiveSites();
    objectiveSitesScanned = true;
    nextObjectiveScanTime = level.inttime + 2000;

    Entity *explosive = NULL;
    while ((explosive = G_FindClass(explosive, "script_model")) != NULL) {
        ScriptVariable *triggerNameVariable = explosive->Vars()->GetVariable("trigger_name");
        if (!triggerNameVariable) {
            continue;
        }

        const str triggerName = triggerNameVariable->stringValue();
        if (!triggerName.length()) {
            continue;
        }

        Entity *trigger = NULL;
        while ((trigger = G_FindTarget(trigger, triggerName.c_str())) != NULL) {
            if (!trigger->isSubclassOf(TriggerUse)) {
                continue;
            }

            bool duplicate = false;
            for (int i = 0; i < objectiveSiteCount; ++i) {
                if (objectiveSites[i].trigger == trigger) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                break;
            }

            if (objectiveSiteCount >= MAX_BOT_OBJECTIVE_SITES) {
                gi.DPrintf(
                    "Bot objective discovery: ignoring bomb sites beyond the %d-site limit\n",
                    MAX_BOT_OBJECTIVE_SITES
                );
                return;
            }

            bot_objective_site_t& site = objectiveSites[objectiveSiteCount++];
            site.explosive             = explosive;
            site.trigger               = trigger;
            site.user                  = NULL;
            break;
        }
    }
}

int BotManager::GetObjectiveSiteCount()
{
    if (!objectiveSitesScanned || (!objectiveSiteCount && level.inttime >= nextObjectiveScanTime)) {
        DiscoverObjectiveSites();
    }
    return objectiveSiteCount;
}

bot_objective_site_state_t BotManager::GetObjectiveSiteState(int site)
{
    if (site < 0 || site >= GetObjectiveSiteCount() || !objectiveSites[site].explosive) {
        return BOT_OBJECTIVE_SITE_DESTROYED;
    }

    Entity *explosive = objectiveSites[site].explosive;
    if (BotObjectiveVariable(explosive, "exploded")) {
        return BOT_OBJECTIVE_SITE_DESTROYED;
    }
    if (BotObjectiveVariable(explosive, "live")) {
        return BOT_OBJECTIVE_SITE_PLANTED;
    }
    return BOT_OBJECTIVE_SITE_AVAILABLE;
}

Vector BotManager::GetObjectiveSitePosition(int site) const
{
    if (site < 0 || site >= objectiveSiteCount) {
        return vec_zero;
    }

    Entity *trigger = objectiveSites[site].trigger;
    if (trigger) {
        return (trigger->absmin + trigger->absmax) * 0.5f;
    }

    Entity *explosive = objectiveSites[site].explosive;
    return explosive ? explosive->origin : vec_zero;
}

Entity *BotManager::GetObjectiveSiteExplosive(int site) const
{
    if (site < 0 || site >= objectiveSiteCount) {
        return NULL;
    }
    return objectiveSites[site].explosive;
}

Entity *BotManager::GetObjectiveSiteTrigger(int site) const
{
    if (site < 0 || site >= objectiveSiteCount) {
        return NULL;
    }
    return objectiveSites[site].trigger;
}

bool BotManager::ClaimObjectiveSite(int site, Player *player)
{
    if (!player || site < 0 || site >= objectiveSiteCount) {
        return false;
    }

    SafePtr<Player>& user = objectiveSites[site].user;
    if (user && user != player && !user->IsDead() && !user->IsSpectator()) {
        return false;
    }

    user = player;
    return true;
}

void BotManager::ReleaseObjectiveClaim(Player *player)
{
    if (!player) {
        return;
    }

    for (int i = 0; i < objectiveSiteCount; ++i) {
        if (objectiveSites[i].user == player) {
            objectiveSites[i].user = NULL;
        }
    }
}

int BotManager::GetObjectiveRound() const
{
    return objectiveRound;
}

int BotManager::GetObjectiveSeed() const
{
    return objectiveSeed;
}

int BotManager::GetObjectiveBotRank(Player *player) const
{
    if (!player) {
        return 0;
    }

    int rank = 0;
    const Container<BotController *>& controllers = botControllerManager.getControllers();
    for (int i = 1; i <= controllers.NumObjects(); ++i) {
        Player *candidate = controllers.ObjectAt(i)->getControlledEntity();
        if (candidate && candidate->GetTeam() == player->GetTeam() && candidate->entnum < player->entnum) {
            ++rank;
        }
    }
    return rank;
}

Vector BotManager::GetObjectiveEnemySpawnCenter(teamtype_t team) const
{
    const teamtype_t enemyTeam = team == TEAM_ALLIES ? TEAM_AXIS : TEAM_ALLIES;
    DM_Team         *dmTeam    = dmManager.GetTeam(enemyTeam);
    if (!dmTeam || !dmTeam->m_spawnpoints.NumObjects()) {
        return vec_zero;
    }

    Vector center = vec_zero;
    for (int i = 1; i <= dmTeam->m_spawnpoints.NumObjects(); ++i) {
        center += dmTeam->m_spawnpoints.ObjectAt(i)->origin;
    }
    return center / static_cast<float>(dmTeam->m_spawnpoints.NumObjects());
}

static Vector BotObjectiveDirection(const Vector& from, const Vector& to)
{
    Vector direction = to - from;
    direction.z      = 0.0f;
    if (direction.lengthSquared() < 1.0f) {
        return Vector(1.0f, 0.0f, 0.0f);
    }
    direction.normalize();
    return direction;
}

static Vector BotObjectiveNearestNode(const Vector& desired, const Vector& center, float maxCenterDistance)
{
    PathNode *best      = NULL;
    float     bestScore = 0.0f;

    for (int i = 0; i < PathSearch::nodecount; ++i) {
        PathNode *node = PathSearch::pathnodes[i];
        if (!node || node->virtualNumChildren <= 0) {
            continue;
        }
        if (maxCenterDistance > 0.0f
            && (node->origin - center).lengthXYSquared() > Square(maxCenterDistance)) {
            continue;
        }

        const float score = (node->origin - desired).lengthSquared();
        if (!best || score < bestScore) {
            best      = node;
            bestScore = score;
        }
    }

    return best ? best->origin : desired;
}

static Vector BotObjectivePatrolPoint(const Vector& center, const Vector& toward, float radius, int variant)
{
    static const float angles[] = {-1.10f, 1.10f, -0.45f, 0.45f};

    Vector      direction = BotObjectiveDirection(center, toward);
    const float angle     = angles[variant & 3];
    const float cosine = cos(angle);
    const float sine   = sin(angle);
    Vector rotated(
        direction.x * cosine - direction.y * sine,
        direction.x * sine + direction.y * cosine,
        0.0f
    );
    const Vector desired = center + rotated * radius;
    return BotObjectiveNearestNode(desired, center, radius * 1.75f);
}

static int BotObjectiveFindSite(bot_objective_site_state_t state, int start, const Vector& origin)
{
    const int count = botManager.GetObjectiveSiteCount();
    int       best         = -1;
    float     bestDistance = 0.0f;

    for (int offset = 0; offset < count; ++offset) {
        const int site = (start + offset + count) % count;
        if (botManager.GetObjectiveSiteState(site) != state) {
            continue;
        }

        const float distance = (botManager.GetObjectiveSitePosition(site) - origin).lengthSquared();
        if (best < 0 || distance < bestDistance) {
            best         = site;
            bestDistance = distance;
        }
    }
    return best;
}

void BotController::ResetObjectiveBehavior()
{
    botManager.ReleaseObjectiveClaim(controlledEnt);

    m_iObjectiveState             = BOT_OBJECTIVE_NONE;
    m_iObjectiveUsePhase          = BOT_OBJECTIVE_USE_AIM;
    m_iObjectiveRound             = -1;
    m_iObjectiveSite              = -1;
    m_iObjectiveRouteVariant          = 0;
    ResetDemoRoute(m_ObjectiveDemoRoute);
    m_iObjectiveUseStartTime      = 0;
    m_iObjectiveReapproachUntil   = 0;
    m_iObjectiveNextMoveTime     = 0;
    m_iObjectiveLastProgressTime = 0;
    m_iObjectiveLastStallLogTime = 0;
    m_bObjectiveAttacker         = false;
    m_bObjectiveHasDestination   = false;
    m_bObjectiveOwnsMovement     = false;
    m_bObjectiveOwnsUse          = false;
    m_bObjectiveCritical        = false;
    m_bObjectiveRoutePostPlant  = false;
    m_vObjectiveDestination     = vec_zero;
    m_vObjectiveLastProgressPos = vec_zero;
    m_fObjectiveBestDistance    = 0.0f;

    m_botCmd.buttons &= ~BUTTON_USE;
    movement.ClearMove();
}

void BotController::BeginObjectivePlan()
{
    const int siteCount = botManager.GetObjectiveSiteCount();
    if (!controlledEnt || !siteCount) {
        return;
    }

    const int rank               = botManager.GetObjectiveBotRank(controlledEnt);
    const int seed               = botManager.GetObjectiveSeed();
    m_iObjectiveRound            = botManager.GetObjectiveRound();
    m_bObjectiveAttacker         = controlledEnt->GetTeam() == botManager.GetObjectivePlantTeam();
    m_iObjectiveSite             = (rank + seed) % siteCount;
    m_iObjectiveRouteVariant     = (rank + seed / Q_max(siteCount, 1)) & 3;
    m_iObjectiveState            = BOT_OBJECTIVE_NONE;
    m_bObjectiveHasDestination   = false;
    m_vObjectiveDestination      = vec_zero;
    m_vObjectiveLastProgressPos  = controlledEnt->origin;
    m_iObjectiveLastProgressTime = level.inttime;
    m_fObjectiveBestDistance     = 0.0f;

    G_MoveLogBotEvent(
        "bot_objective_plan",
        controlledEnt,
        NULL,
        m_iObjectiveSite,
        botManager.GetObjectiveSitePosition(m_iObjectiveSite)
    );
}

void BotController::UpdateObjectiveAdvance(const Vector& sitePosition)
{
    if (UpdateObjectiveDemoRoute(sitePosition, false, false)) {
        return;
    }

    const float siteDistance = (sitePosition - controlledEnt->origin).lengthXYSquared();
    if (BotObjectiveIsTeam4()
        && siteDistance <= Square(BOT_OBJECTIVE_TEAM4_DIRECT_MAX_DISTANCE)
        && (!movement.IsMoving() || movement.MoveDone())) {
        const bool changed =
            !m_bObjectiveHasDestination || m_iObjectiveState != BOT_OBJECTIVE_ADVANCE
            || m_vObjectiveDestination != sitePosition;

        m_iObjectiveState          = BOT_OBJECTIVE_ADVANCE;
        m_bObjectiveHasDestination = true;
        m_vObjectiveDestination    = sitePosition;
        m_bObjectiveOwnsMovement   = true;

        if (changed) {
            m_vObjectiveLastProgressPos  = controlledEnt->origin;
            m_iObjectiveLastProgressTime = level.inttime;
        }
        movement.MoveDirect(sitePosition, BOT_OBJECTIVE_TEAM4_DIRECT_RADIUS);
        G_MoveLogBotEvent(
            "bot_objective_direct_approach",
            controlledEnt,
            NULL,
            m_iObjectiveSite,
            sitePosition
        );
        if (!m_iAttackTime) {
            AimAtAimNode();
        }
        return;
    }

    SetObjectiveDestination(
        sitePosition,
        BOT_OBJECTIVE_ADVANCE,
        BOT_OBJECTIVE_APPROACH_RADIUS
    );
}

void BotController::SetObjectiveDestination(
    const Vector& destination, bot_objective_state_t state, float radius
)
{
    const bool changed =
        !m_bObjectiveHasDestination || m_iObjectiveState != state
        || m_vObjectiveDestination != destination;

    m_iObjectiveState          = state;
    m_bObjectiveHasDestination = true;
    m_vObjectiveDestination    = destination;
    m_bObjectiveOwnsMovement   = true;

    if (changed) {
        m_vObjectiveLastProgressPos  = controlledEnt->origin;
        m_iObjectiveLastProgressTime = level.inttime;
        m_fObjectiveBestDistance =
            (destination - controlledEnt->origin).length();
    }

    if (changed || !movement.IsMoving() || movement.MoveDone()) {
        if (radius > 0.0f) {
            movement.MoveNear(destination, radius);
        } else {
            movement.MoveTo(destination);
        }

        // Do not claim an objective path that the navigation backend rejected.
        // Patrol planning can then try another point, while the normal idle
        // fallback keeps the bot active for this frame.
        if (!movement.IsMoving()) {
            m_bObjectiveHasDestination = false;
            m_bObjectiveOwnsMovement   = false;
        }
    }

    if (!m_iAttackTime) {
        AimAtAimNode();
    }
}

void BotController::UpdateObjectivePatrol(
    const Vector& center, const Vector& toward, float radius, bot_objective_state_t state
)
{
    const bool sameDestination =
        m_iObjectiveState == state && m_bObjectiveHasDestination;
    const bool moveFinished = sameDestination && movement.MoveDone();
    const bool destinationReached =
        moveFinished
        && (m_vObjectiveDestination - controlledEnt->origin).lengthXYSquared()
            <= Square(BOT_OBJECTIVE_PATROL_SEARCH_RADIUS);

    if (destinationReached && !m_iObjectiveNextMoveTime) {
        m_iObjectiveNextMoveTime =
            level.inttime + 3000 + static_cast<int>(G_Random(3000.0f));
    }

    const bool choosePoint =
        m_iObjectiveState != state || !m_bObjectiveHasDestination
        || (moveFinished && !destinationReached)
        || (destinationReached && level.inttime >= m_iObjectiveNextMoveTime);

    if (choosePoint) {
        if (m_iObjectiveState == state) {
            m_iObjectiveRouteVariant = (m_iObjectiveRouteVariant + 1 + static_cast<int>(G_Random(3.0f))) & 3;
        }
        const Vector destination = BotObjectivePatrolPoint(center, toward, radius, m_iObjectiveRouteVariant);
        SetObjectiveDestination(destination, state, BOT_OBJECTIVE_PATROL_SEARCH_RADIUS);
        m_iObjectiveNextMoveTime = 0;
        return;
    }

    if (destinationReached) {
        // Stay put until it is time to choose a genuinely new patrol point.
        // Start the dwell on arrival so travel time cannot consume it and
        // cause an immediate U-turn at a doorway or corridor threshold.
        m_iObjectiveState            = state;
        m_bObjectiveOwnsMovement     = true;
        m_vObjectiveLastProgressPos  = controlledEnt->origin;
        m_iObjectiveLastProgressTime = level.inttime;
        movement.ClearMove();
        return;
    }

    SetObjectiveDestination(
        m_vObjectiveDestination,
        state,
        BOT_OBJECTIVE_PATROL_SEARCH_RADIUS
    );
}

void BotController::UpdateObjectiveUse(bool planting)
{
    const bot_objective_site_state_t siteState = botManager.GetObjectiveSiteState(m_iObjectiveSite);
    const bool completed =
        planting ? siteState == BOT_OBJECTIVE_SITE_PLANTED : siteState == BOT_OBJECTIVE_SITE_AVAILABLE;

    if (completed) {
        G_MoveLogBotEvent(
            planting ? "bot_objective_plant_complete" : "bot_objective_defuse_complete",
            controlledEnt,
            NULL,
            m_iObjectiveSite,
            botManager.GetObjectiveSitePosition(m_iObjectiveSite)
        );
        botManager.ReleaseObjectiveClaim(controlledEnt);
        m_iObjectiveState           = planting ? BOT_OBJECTIVE_COVER : BOT_OBJECTIVE_HOLD;
        m_iObjectiveUsePhase        = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime    = 0;
        m_iObjectiveReapproachUntil = 0;
        m_botCmd.buttons &= ~BUTTON_USE;
        return;
    }

    if (siteState == BOT_OBJECTIVE_SITE_DESTROYED) {
        botManager.ReleaseObjectiveClaim(controlledEnt);
        m_iObjectiveState           = BOT_OBJECTIVE_NONE;
        m_iObjectiveUsePhase        = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime    = 0;
        m_iObjectiveReapproachUntil = 0;
        return;
    }

    Entity *explosive = botManager.GetObjectiveSiteExplosive(m_iObjectiveSite);
    Entity *trigger   = botManager.GetObjectiveSiteTrigger(m_iObjectiveSite);
    if (!explosive || !trigger
        || (m_iObjectiveUsePhase != BOT_OBJECTIVE_USE_HOLD
            && !BotObjectiveInUseRange(controlledEnt, trigger))) {
        botManager.ReleaseObjectiveClaim(controlledEnt);
        m_iObjectiveUsePhase     = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime = 0;
        SetObjectiveDestination(
            botManager.GetObjectiveSitePosition(m_iObjectiveSite),
            BOT_OBJECTIVE_ADVANCE,
            BOT_OBJECTIVE_APPROACH_RADIUS
        );
        return;
    }

    m_iObjectiveState        = planting ? BOT_OBJECTIVE_PLANT : BOT_OBJECTIVE_DEFUSE;
    m_bObjectiveOwnsMovement = true;
    m_bObjectiveOwnsUse      = true;
    m_bObjectiveCritical     = true;
    movement.ClearMove();
    rotation.AimAt(explosive->centroid);

    // canUse follows the player's view, while the map script checks CanSee
    // against the bomb model using body orientation.
    Vector bodyAngles = controlledEnt->angles;
    bodyAngles[YAW]    = rotation.GetTargetAngles()[YAW];
    controlledEnt->setAngles(bodyAngles);

    if (!m_iObjectiveUseStartTime) {
        m_iObjectiveUseStartTime = level.inttime;
    }

    if (m_bTeamResponding) {
        m_iCuriousTime      = 0;
        m_iCuriousEventType = AI_EVENT_NONE;
        ClearTeamResponse();
    }

    if (m_iObjectiveUsePhase == BOT_OBJECTIVE_USE_AIM) {
        if (controlledEnt->canUse(trigger, true)
            && BotObjectiveCanSee(controlledEnt, explosive, BotObjectiveUseFov())) {
            m_iObjectiveUsePhase = BOT_OBJECTIVE_USE_RELEASE;
        }
    } else if (m_iObjectiveUsePhase == BOT_OBJECTIVE_USE_RELEASE) {
        m_iObjectiveUsePhase = BOT_OBJECTIVE_USE_HOLD;
        // Give the script a full hold timeout; time spent turning toward the
        // model must not consume the interaction window.
        m_iObjectiveUseStartTime = level.inttime;
        G_MoveLogBotEvent(
            planting ? "bot_objective_plant_start" : "bot_objective_defuse_start",
            controlledEnt,
            NULL,
            m_iObjectiveSite,
            botManager.GetObjectiveSitePosition(m_iObjectiveSite)
        );
    }

    if (level.inttime - m_iObjectiveUseStartTime >= BOT_OBJECTIVE_USE_TIMEOUT_MSEC) {
        G_MoveLogBotEvent(
            planting ? "bot_objective_plant_timeout" : "bot_objective_defuse_timeout",
            controlledEnt,
            NULL,
            m_iObjectiveSite,
            botManager.GetObjectiveSitePosition(m_iObjectiveSite)
        );
        botManager.ReleaseObjectiveClaim(controlledEnt);
        m_iObjectiveState           = BOT_OBJECTIVE_ADVANCE;
        m_iObjectiveUsePhase        = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime    = 0;
        m_bObjectiveOwnsUse         = false;
        m_bObjectiveCritical        = false;
        m_botCmd.buttons &= ~BUTTON_USE;
        m_iObjectiveReapproachUntil = level.inttime + BOT_OBJECTIVE_REAPPROACH_MSEC;
        m_bObjectiveHasDestination = false;
        SetObjectiveDestination(
            botManager.GetObjectiveSitePosition(m_iObjectiveSite),
            BOT_OBJECTIVE_ADVANCE,
            BOT_OBJECTIVE_RETRY_APPROACH_RADIUS
        );
    }
}

bool BotController::TryStartObjectiveUse(bool planting, int site)
{
    Entity *explosive = botManager.GetObjectiveSiteExplosive(site);
    Entity *trigger   = botManager.GetObjectiveSiteTrigger(site);
    if (!explosive
        || !BotObjectiveInUseRange(controlledEnt, trigger)
        // Ignore facing here: UpdateObjectiveUse turns the bot before applying
        // the map script's exact FOV check.
        || !BotObjectiveCanSee(controlledEnt, explosive, 360.0f)
        || !botManager.ClaimObjectiveSite(site, controlledEnt)) {
        return false;
    }

    m_iObjectiveSite            = site;
    m_iObjectiveState           = planting ? BOT_OBJECTIVE_PLANT : BOT_OBJECTIVE_DEFUSE;
    m_iObjectiveUsePhase        = BOT_OBJECTIVE_USE_AIM;
    m_iObjectiveUseStartTime    = 0;
    m_iObjectiveReapproachUntil = 0;
    UpdateObjectiveUse(planting);
    return true;
}

void BotController::UpdateObjectiveProgress()
{
    if (m_bObjectiveOwnsUse) {
        m_vObjectiveLastProgressPos  = controlledEnt->origin;
        m_iObjectiveLastProgressTime = level.inttime;
        return;
    }

    if (!m_bObjectiveOwnsMovement || m_iObjectiveState == BOT_OBJECTIVE_NONE) {
        return;
    }

    if (m_iObjectiveState == BOT_OBJECTIVE_ROUTE) {
        const float distance =
            (m_vObjectiveDestination - controlledEnt->origin).length();
        if (distance <= m_fObjectiveBestDistance - BOT_OBJECTIVE_PROGRESS_UNITS) {
            m_fObjectiveBestDistance     = distance;
            m_vObjectiveLastProgressPos  = controlledEnt->origin;
            m_iObjectiveLastProgressTime = level.inttime;
            return;
        }
    } else if ((controlledEnt->origin - m_vObjectiveLastProgressPos).lengthSquared()
               >= Square(BOT_OBJECTIVE_PROGRESS_UNITS)) {
        m_vObjectiveLastProgressPos  = controlledEnt->origin;
        m_iObjectiveLastProgressTime = level.inttime;
        return;
    }

    if (level.inttime - m_iObjectiveLastProgressTime
            >= BOT_OBJECTIVE_STALL_MSEC
        && level.inttime - m_iObjectiveLastStallLogTime
            >= BOT_OBJECTIVE_STALL_MSEC) {
        G_MoveLogBotEvent(
            "bot_objective_stalled",
            controlledEnt,
            NULL,
            static_cast<int>(m_iObjectiveState),
            controlledEnt->origin
        );
        m_iObjectiveLastStallLogTime = level.inttime;
        m_iObjectiveLastProgressTime = level.inttime;
        movement.ClearMove();
        m_iObjectiveNextMoveTime = 0;

        if (m_iObjectiveState == BOT_OBJECTIVE_ROUTE) {
            ResetObjectiveDemoRoute(true);
            G_MoveLogBotEvent(
                "bot_objective_route_fallback",
                controlledEnt,
                NULL,
                1,
                controlledEnt->origin
            );
        }
    }
}

void BotController::UpdateObjectiveBehavior()
{
    // Combat, team response, and survival movement can replace the active
    // navigation path. Keep the strategic hop, but force it to be reissued
    // once objective movement regains control.
    if (!m_bObjectiveOwnsMovement
        && m_ObjectiveDemoRoute.nextNode >= 0) {
        m_bObjectiveHasDestination = false;
    }

    m_bObjectiveOwnsMovement = false;
    m_bObjectiveOwnsUse      = false;
    m_bObjectiveCritical     = false;

    if (m_bGrenadeFleeing || m_bReloadRetreating) {
        // Survival temporarily owns movement. Preserve the objective plan, but
        // restart an interrupted plant/defuse attempt after reaching safety.
        if (m_iObjectiveState == BOT_OBJECTIVE_PLANT
            || m_iObjectiveState == BOT_OBJECTIVE_DEFUSE) {
            m_iObjectiveUsePhase     = BOT_OBJECTIVE_USE_AIM;
            m_iObjectiveUseStartTime = 0;
            m_iObjectiveNextMoveTime = 0;
        }
        if (m_bObjectiveHasDestination) {
            m_vObjectiveLastProgressPos  = controlledEnt->origin;
            m_iObjectiveLastProgressTime = level.inttime;
            m_fObjectiveBestDistance =
                (m_vObjectiveDestination - controlledEnt->origin).length();
        }
        m_botCmd.buttons &= ~BUTTON_USE;
        return;
    }

    if (!botManager.ObjectiveModeActive()) {
        if (m_iObjectiveState != BOT_OBJECTIVE_NONE || m_iObjectiveRound >= 0) {
            ResetObjectiveBehavior();
        }
        return;
    }

    if (m_iObjectiveRound != botManager.GetObjectiveRound() || m_iObjectiveSite < 0) {
        ResetObjectiveBehavior();
        BeginObjectivePlan();
    }
    if (m_iObjectiveSite < 0) {
        return;
    }

    if (m_iObjectiveState == BOT_OBJECTIVE_PLANT) {
        UpdateObjectiveUse(true);
        UpdateObjectiveProgress();
        return;
    }
    if (m_iObjectiveState == BOT_OBJECTIVE_DEFUSE) {
        UpdateObjectiveUse(false);
        UpdateObjectiveProgress();
        return;
    }

    bot_objective_site_state_t siteState = botManager.GetObjectiveSiteState(m_iObjectiveSite);
    Vector enemySpawn = botManager.GetObjectiveEnemySpawnCenter(controlledEnt->GetTeam());
    if (enemySpawn == vec_zero) {
        enemySpawn = controlledEnt->origin + Vector(controlledEnt->orientation[0]) * 1024.0f;
    }

    if (m_bObjectiveAttacker && siteState == BOT_OBJECTIVE_SITE_AVAILABLE
        && level.inttime >= m_iObjectiveReapproachUntil
        && TryStartObjectiveUse(true, m_iObjectiveSite)) {
        UpdateObjectiveProgress();
        return;
    }

    if (m_bObjectiveAttacker) {
        if (m_iAttackTime) {
            m_vObjectiveLastProgressPos  = controlledEnt->origin;
            m_iObjectiveLastProgressTime = level.inttime;
            m_fObjectiveBestDistance =
                (m_vObjectiveDestination - controlledEnt->origin).length();
            return;
        }

        if (m_bTeamResponding) {
            m_vObjectiveLastProgressPos  = controlledEnt->origin;
            m_iObjectiveLastProgressTime = level.inttime;
            m_fObjectiveBestDistance =
                (m_vObjectiveDestination - controlledEnt->origin).length();
            return;
        }

        if (siteState == BOT_OBJECTIVE_SITE_DESTROYED) {
            const int available = BotObjectiveFindSite(
                BOT_OBJECTIVE_SITE_AVAILABLE,
                m_iObjectiveSite + 1,
                controlledEnt->origin
            );
            if (available >= 0) {
                botManager.ReleaseObjectiveClaim(controlledEnt);
                m_iObjectiveSite           = available;
                m_iObjectiveState          = BOT_OBJECTIVE_NONE;
                m_bObjectiveHasDestination = false;
                m_vObjectiveDestination    = vec_zero;
                ResetObjectiveDemoRoute();
                siteState = BOT_OBJECTIVE_SITE_AVAILABLE;
                G_MoveLogBotEvent(
                    "bot_objective_replan",
                    controlledEnt,
                    NULL,
                    m_iObjectiveSite,
                    botManager.GetObjectiveSitePosition(m_iObjectiveSite)
                );
            } else {
                const int planted = BotObjectiveFindSite(
                    BOT_OBJECTIVE_SITE_PLANTED,
                    m_iObjectiveSite,
                    controlledEnt->origin
                );
                if (planted < 0) {
                    return;
                }
                m_iObjectiveSite = planted;
                siteState        = BOT_OBJECTIVE_SITE_PLANTED;
            }
        }

        const Vector sitePosition = botManager.GetObjectiveSitePosition(m_iObjectiveSite);
        if (siteState == BOT_OBJECTIVE_SITE_PLANTED) {
            if (!UpdateObjectiveDemoRoute(sitePosition, true, true)) {
                UpdateObjectivePatrol(
                    sitePosition, enemySpawn, 256.0f, BOT_OBJECTIVE_COVER
                );
            }
            UpdateObjectiveProgress();
            return;
        }

        Entity *explosive = botManager.GetObjectiveSiteExplosive(m_iObjectiveSite);
        Entity *trigger   = botManager.GetObjectiveSiteTrigger(m_iObjectiveSite);
        const bool inUseRange = BotObjectiveInUseRange(controlledEnt, trigger);
        if (inUseRange
            && (level.inttime < m_iObjectiveReapproachUntil
                || !BotObjectiveCanSee(controlledEnt, explosive, 360.0f))) {
            SetObjectiveDestination(
                sitePosition,
                BOT_OBJECTIVE_ADVANCE,
                BOT_OBJECTIVE_RETRY_APPROACH_RADIUS
            );
        } else if (inUseRange) {
            UpdateObjectivePatrol(sitePosition, enemySpawn, 224.0f, BOT_OBJECTIVE_COVER);
        } else {
            UpdateObjectiveAdvance(sitePosition);
        }
    } else {
        // Stay committed to the first live bomb. It has less time remaining,
        // and selecting the nearest site every frame can continually replace
        // the path when two planted bombs are a similar distance away.
        int plantedSite = m_iObjectiveSite;
        if (botManager.GetObjectiveSiteState(plantedSite) != BOT_OBJECTIVE_SITE_PLANTED) {
            plantedSite = BotObjectiveFindSite(
                BOT_OBJECTIVE_SITE_PLANTED,
                m_iObjectiveSite,
                controlledEnt->origin
            );
        }

        if (plantedSite >= 0) {
            m_iObjectiveSite          = plantedSite;
            m_bObjectiveCritical      = true;
            const Vector sitePosition = botManager.GetObjectiveSitePosition(plantedSite);

            if (m_bTeamResponding) {
                m_iCuriousTime      = 0;
                m_iCuriousEventType = AI_EVENT_NONE;
                ClearTeamResponse();
            }

            if (level.inttime >= m_iObjectiveReapproachUntil
                && TryStartObjectiveUse(false, plantedSite)) {
                UpdateObjectiveProgress();
                return;
            }

            Entity *explosive = botManager.GetObjectiveSiteExplosive(plantedSite);
            Entity *trigger   = botManager.GetObjectiveSiteTrigger(plantedSite);
            const bool inUseRange = BotObjectiveInUseRange(controlledEnt, trigger);
            if (inUseRange
                && (level.inttime < m_iObjectiveReapproachUntil
                    || !BotObjectiveCanSee(controlledEnt, explosive, 360.0f))) {
                SetObjectiveDestination(
                    sitePosition,
                    BOT_OBJECTIVE_ADVANCE,
                    BOT_OBJECTIVE_RETRY_APPROACH_RADIUS
                );
            } else if (inUseRange) {
                UpdateObjectivePatrol(sitePosition, enemySpawn, 224.0f, BOT_OBJECTIVE_COVER);
            } else {
                // Once a bomb is live, go directly to it. A demo route can
                // keep reaching waypoints without reducing the fuse time.
                ResetObjectiveDemoRoute();
                SetObjectiveDestination(
                    sitePosition,
                    BOT_OBJECTIVE_ADVANCE,
                    BOT_OBJECTIVE_APPROACH_RADIUS
                );
            }
        } else {
            // Before a plant, defenders follow the human pre-plant graph.
            // Combat and team callouts continue to own movement temporarily.
            if (!m_iAttackTime && !m_bTeamResponding
                && UpdateObjectiveDemoRoute(
                    botManager.GetObjectiveSitePosition(m_iObjectiveSite),
                    false,
                    true
                )) {
                UpdateObjectiveProgress();
                return;
            }

            if (m_iObjectiveState != BOT_OBJECTIVE_NONE
                || m_bObjectiveHasDestination) {
                botManager.ReleaseObjectiveClaim(controlledEnt);
                m_iObjectiveState          = BOT_OBJECTIVE_NONE;
                m_bObjectiveHasDestination = false;
                m_vObjectiveDestination    = vec_zero;
                movement.ClearMove();
            }
            return;
        }
    }

    UpdateObjectiveProgress();
}

void BotController::FinalizeObjectiveCommand()
{
    if (m_bObjectiveOwnsMovement
        && m_iObjectiveState == BOT_OBJECTIVE_ROUTE
        && !movement.IsMoving()) {
        // Movement can finish a horizontally close path below or above the
        // strategic waypoint. Keep the route's height check and abandon this
        // hop instead of recreating a zero-input path until the watchdog fires.
        G_MoveLogBotEvent(
            "bot_objective_route_fallback",
            controlledEnt,
            NULL,
            2,
            m_vObjectiveDestination
        );
        ResetObjectiveDemoRoute(true);
    }

    if (!m_bObjectiveOwnsUse) {
        return;
    }

    m_botCmd.forwardmove = 0;
    m_botCmd.rightmove   = 0;
    m_botCmd.upmove      = 0;
    m_botCmd.buttons &=
        ~(BUTTON_ATTACKLEFT | BUTTON_ATTACKRIGHT | BUTTON_LEAN_LEFT | BUTTON_LEAN_RIGHT);

    if (m_iObjectiveUsePhase == BOT_OBJECTIVE_USE_HOLD) {
        m_botCmd.buttons |= BUTTON_USE;
    } else {
        m_botCmd.buttons &= ~BUTTON_USE;
    }
}
