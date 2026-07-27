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
static const int   BOT_OBJECTIVE_STALL_MSEC       = 5000;
static const float BOT_OBJECTIVE_PROGRESS_UNITS   = 64.0f;

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

int BotManager::GetObjectiveDistanceRank(Player *player, const Vector& position) const
{
    if (!player) {
        return 0;
    }

    const float playerDistance = (player->origin - position).lengthSquared();
    int         rank           = 0;
    const Container<BotController *>& controllers = botControllerManager.getControllers();

    for (int i = 1; i <= controllers.NumObjects(); ++i) {
        Player *candidate = controllers.ObjectAt(i)->getControlledEntity();
        if (!candidate || candidate == player || candidate->GetTeam() != player->GetTeam()
            || candidate->IsDead() || candidate->IsSpectator()) {
            continue;
        }

        const float candidateDistance = (candidate->origin - position).lengthSquared();
        if (candidateDistance < playerDistance
            || (candidateDistance == playerDistance && candidate->entnum < player->entnum)) {
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

static Vector BotObjectiveRouteAnchor(const Vector& start, const Vector& goal, int variant, int stage)
{
    static const float lanes[] = {-1.0f, 1.0f, -0.45f, 0.45f};

    Vector forward = BotObjectiveDirection(start, goal);
    Vector left(-forward.y, forward.x, 0.0f);
    Vector delta   = goal - start;
    delta.z        = 0.0f;

    const float distance = Q_max(delta.length(), 1.0f);
    const float fraction = stage == 0 ? 0.25f : 0.58f;
    const float progress = Q_clamp_float(distance * fraction, 192.0f, Q_max(192.0f, distance - 192.0f));
    const float  laneWidth = Q_clamp_float(distance * 0.18f, 128.0f, 512.0f);
    const Vector desired   = start + forward * progress + left * (lanes[variant & 3] * laneWidth);

    return BotObjectiveNearestNode(desired, desired, Q_max(384.0f, laneWidth * 1.5f));
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

    m_iObjectiveState            = BOT_OBJECTIVE_NONE;
    m_iObjectiveUsePhase         = BOT_OBJECTIVE_USE_AIM;
    m_iObjectiveRound            = -1;
    m_iObjectiveSite             = -1;
    m_iObjectiveRouteVariant     = 0;
    m_iObjectiveRouteStage       = 0;
    m_iObjectiveUseStartTime     = 0;
    m_iObjectiveNextMoveTime     = 0;
    m_iObjectiveLastProgressTime = 0;
    m_iObjectiveLastStallLogTime = 0;
    m_bObjectiveAttacker         = false;
    m_bObjectiveHasDestination   = false;
    m_bObjectiveOwnsMovement     = false;
    m_bObjectiveOwnsUse          = false;
    m_bObjectiveCritical         = false;
    m_vObjectiveStart            = vec_zero;
    m_vObjectiveDestination      = vec_zero;
    m_vObjectiveLastProgressPos  = vec_zero;

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
    m_iObjectiveRouteStage       = 0;
    m_iObjectiveState            = BOT_OBJECTIVE_NONE;
    m_bObjectiveHasDestination   = false;
    m_vObjectiveStart            = controlledEnt->origin;
    m_vObjectiveDestination      = vec_zero;
    m_vObjectiveLastProgressPos  = controlledEnt->origin;
    m_iObjectiveLastProgressTime = level.inttime;

    G_MoveLogBotEvent(
        "bot_objective_plan",
        controlledEnt,
        NULL,
        m_iObjectiveSite,
        botManager.GetObjectiveSitePosition(m_iObjectiveSite)
    );
}

void BotController::SetObjectiveDestination(const Vector& destination, bot_objective_state_t state)
{
    const bool changed =
        !m_bObjectiveHasDestination || m_iObjectiveState != state
        || m_vObjectiveDestination != destination;

    m_iObjectiveState          = state;
    m_bObjectiveHasDestination = true;
    m_vObjectiveDestination    = destination;
    m_bObjectiveOwnsMovement   = true;

    if (changed || ((!movement.IsMoving() || movement.MoveDone()) && level.inttime >= m_iObjectiveNextMoveTime)) {
        movement.MoveTo(destination);
        m_iObjectiveNextMoveTime = level.inttime + 1000;
    }

    AimAtAimNode();
}

void BotController::UpdateObjectivePatrol(
    const Vector& center, const Vector& toward, float radius, bot_objective_state_t state
)
{
    const bool choosePoint =
        m_iObjectiveState != state || !m_bObjectiveHasDestination
        || (movement.MoveDone() && level.inttime >= m_iObjectiveNextMoveTime);

    if (choosePoint) {
        if (m_iObjectiveState == state) {
            m_iObjectiveRouteVariant = (m_iObjectiveRouteVariant + 1 + static_cast<int>(G_Random(3.0f))) & 3;
        }
        const Vector destination = BotObjectivePatrolPoint(center, toward, radius, m_iObjectiveRouteVariant);
        SetObjectiveDestination(destination, state);
        m_iObjectiveNextMoveTime = level.inttime + 3000 + static_cast<int>(G_Random(3000.0f));
    } else {
        SetObjectiveDestination(m_vObjectiveDestination, state);
        if (movement.MoveDone()) {
            m_vObjectiveLastProgressPos  = controlledEnt->origin;
            m_iObjectiveLastProgressTime = level.inttime;
        }
    }
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
        m_iObjectiveState        = planting ? BOT_OBJECTIVE_COVER : BOT_OBJECTIVE_HOLD;
        m_iObjectiveUsePhase     = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime = 0;
        m_botCmd.buttons &= ~BUTTON_USE;
        return;
    }

    if (siteState == BOT_OBJECTIVE_SITE_DESTROYED) {
        botManager.ReleaseObjectiveClaim(controlledEnt);
        m_iObjectiveState        = BOT_OBJECTIVE_NONE;
        m_iObjectiveUsePhase     = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime = 0;
        return;
    }

    Entity *trigger = botManager.GetObjectiveSiteTrigger(m_iObjectiveSite);
    if (!trigger || !controlledEnt->canUse(trigger, true)) {
        botManager.ReleaseObjectiveClaim(controlledEnt);
        m_iObjectiveUsePhase     = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime = 0;
        SetObjectiveDestination(
            botManager.GetObjectiveSitePosition(m_iObjectiveSite),
            BOT_OBJECTIVE_ADVANCE
        );
        return;
    }

    m_iObjectiveState        = planting ? BOT_OBJECTIVE_PLANT : BOT_OBJECTIVE_DEFUSE;
    m_bObjectiveOwnsMovement = true;
    m_bObjectiveOwnsUse      = true;
    m_bObjectiveCritical     = true;
    movement.ClearMove();
    rotation.AimAt(botManager.GetObjectiveSitePosition(m_iObjectiveSite));

    if (m_bTeamResponding) {
        m_iCuriousTime      = 0;
        m_iCuriousEventType = AI_EVENT_NONE;
        ClearTeamResponse();
    }

    if (m_iObjectiveUsePhase == BOT_OBJECTIVE_USE_AIM) {
        m_iObjectiveUsePhase = BOT_OBJECTIVE_USE_RELEASE;
    } else if (m_iObjectiveUsePhase == BOT_OBJECTIVE_USE_RELEASE) {
        m_iObjectiveUsePhase     = BOT_OBJECTIVE_USE_HOLD;
        m_iObjectiveUseStartTime = level.inttime;
        G_MoveLogBotEvent(
            planting ? "bot_objective_plant_start" : "bot_objective_defuse_start",
            controlledEnt,
            NULL,
            m_iObjectiveSite,
            botManager.GetObjectiveSitePosition(m_iObjectiveSite)
        );
    } else if (level.inttime - m_iObjectiveUseStartTime >= BOT_OBJECTIVE_USE_TIMEOUT_MSEC) {
        G_MoveLogBotEvent(
            planting ? "bot_objective_plant_timeout" : "bot_objective_defuse_timeout",
            controlledEnt,
            NULL,
            m_iObjectiveSite,
            botManager.GetObjectiveSitePosition(m_iObjectiveSite)
        );
        botManager.ReleaseObjectiveClaim(controlledEnt);
        m_iObjectiveState        = BOT_OBJECTIVE_ADVANCE;
        m_iObjectiveUsePhase     = BOT_OBJECTIVE_USE_AIM;
        m_iObjectiveUseStartTime = 0;
        m_bObjectiveOwnsUse      = false;
        m_bObjectiveCritical     = false;
        m_iObjectiveNextMoveTime = 0;
    }
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

    if ((controlledEnt->origin - m_vObjectiveLastProgressPos).lengthSquared()
        >= Square(BOT_OBJECTIVE_PROGRESS_UNITS)) {
        m_vObjectiveLastProgressPos  = controlledEnt->origin;
        m_iObjectiveLastProgressTime = level.inttime;
        return;
    }

    if (level.inttime - m_iObjectiveLastProgressTime >= BOT_OBJECTIVE_STALL_MSEC
        && level.inttime - m_iObjectiveLastStallLogTime >= BOT_OBJECTIVE_STALL_MSEC) {
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
    }
}

void BotController::UpdateObjectiveBehavior()
{
    m_bObjectiveOwnsMovement = false;
    m_bObjectiveOwnsUse      = false;
    m_bObjectiveCritical     = false;

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

    if (m_iAttackTime) {
        m_vObjectiveLastProgressPos  = controlledEnt->origin;
        m_iObjectiveLastProgressTime = level.inttime;
        return;
    }

    bot_objective_site_state_t siteState = botManager.GetObjectiveSiteState(m_iObjectiveSite);
    Vector enemySpawn = botManager.GetObjectiveEnemySpawnCenter(controlledEnt->GetTeam());
    if (enemySpawn == vec_zero) {
        enemySpawn = controlledEnt->origin + Vector(controlledEnt->orientation[0]) * 1024.0f;
    }

    if (m_bObjectiveAttacker) {
        if (m_bTeamResponding) {
            m_vObjectiveLastProgressPos  = controlledEnt->origin;
            m_iObjectiveLastProgressTime = level.inttime;
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
                m_iObjectiveRouteStage     = 0;
                m_iObjectiveState          = BOT_OBJECTIVE_NONE;
                m_bObjectiveHasDestination = false;
                m_vObjectiveStart          = controlledEnt->origin;
                m_vObjectiveDestination    = vec_zero;
                siteState                  = BOT_OBJECTIVE_SITE_AVAILABLE;
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
            UpdateObjectivePatrol(sitePosition, enemySpawn, 256.0f, BOT_OBJECTIVE_COVER);
            UpdateObjectiveProgress();
            return;
        }

        if ((sitePosition - m_vObjectiveStart).lengthXYSquared() < Square(512.0f)) {
            m_iObjectiveRouteStage = 2;
        }

        if (m_iObjectiveRouteStage < 2) {
            if (m_iObjectiveState == BOT_OBJECTIVE_ROUTE && movement.MoveDone()) {
                ++m_iObjectiveRouteStage;
                m_bObjectiveHasDestination = false;
            }

            if (m_iObjectiveRouteStage < 2) {
                if (m_iObjectiveState != BOT_OBJECTIVE_ROUTE || !m_bObjectiveHasDestination) {
                    const Vector anchor = BotObjectiveRouteAnchor(
                        m_vObjectiveStart,
                        sitePosition,
                        m_iObjectiveRouteVariant,
                        m_iObjectiveRouteStage
                    );
                    SetObjectiveDestination(anchor, BOT_OBJECTIVE_ROUTE);
                    G_MoveLogBotEvent(
                        "bot_objective_route",
                        controlledEnt,
                        NULL,
                        m_iObjectiveRouteStage,
                        anchor
                    );
                } else {
                    SetObjectiveDestination(m_vObjectiveDestination, BOT_OBJECTIVE_ROUTE);
                }
                UpdateObjectiveProgress();
                return;
            }
        }

        Entity *trigger = botManager.GetObjectiveSiteTrigger(m_iObjectiveSite);
        if (trigger && controlledEnt->canUse(trigger, true)) {
            if (botManager.ClaimObjectiveSite(m_iObjectiveSite, controlledEnt)) {
                m_iObjectiveState    = BOT_OBJECTIVE_PLANT;
                m_iObjectiveUsePhase = BOT_OBJECTIVE_USE_AIM;
                UpdateObjectiveUse(true);
            } else {
                UpdateObjectivePatrol(sitePosition, enemySpawn, 224.0f, BOT_OBJECTIVE_COVER);
            }
        } else {
            SetObjectiveDestination(sitePosition, BOT_OBJECTIVE_ADVANCE);
        }
    } else {
        const int plantedSite = BotObjectiveFindSite(
            BOT_OBJECTIVE_SITE_PLANTED,
            m_iObjectiveSite,
            controlledEnt->origin
        );

        if (plantedSite >= 0) {
            m_iObjectiveSite = plantedSite;
            const Vector sitePosition = botManager.GetObjectiveSitePosition(plantedSite);
            const int responseRank = botManager.GetObjectiveDistanceRank(controlledEnt, sitePosition);

            if (responseRank < 2) {
                m_bObjectiveCritical = true;
                if (m_bTeamResponding) {
                    m_iCuriousTime      = 0;
                    m_iCuriousEventType = AI_EVENT_NONE;
                    ClearTeamResponse();
                }

                Entity *trigger = botManager.GetObjectiveSiteTrigger(plantedSite);
                if (trigger && controlledEnt->canUse(trigger, true)) {
                    if (botManager.ClaimObjectiveSite(plantedSite, controlledEnt)) {
                        m_iObjectiveState    = BOT_OBJECTIVE_DEFUSE;
                        m_iObjectiveUsePhase = BOT_OBJECTIVE_USE_AIM;
                        UpdateObjectiveUse(false);
                    } else {
                        UpdateObjectivePatrol(sitePosition, enemySpawn, 224.0f, BOT_OBJECTIVE_COVER);
                    }
                } else {
                    SetObjectiveDestination(sitePosition, BOT_OBJECTIVE_ADVANCE);
                    m_bObjectiveCritical = true;
                }
            } else {
                if (m_bTeamResponding) {
                    m_vObjectiveLastProgressPos  = controlledEnt->origin;
                    m_iObjectiveLastProgressTime = level.inttime;
                    return;
                }
                UpdateObjectivePatrol(sitePosition, enemySpawn, 320.0f, BOT_OBJECTIVE_HOLD);
            }
        } else {
            if (m_bTeamResponding) {
                m_vObjectiveLastProgressPos  = controlledEnt->origin;
                m_iObjectiveLastProgressTime = level.inttime;
                return;
            }

            if (botManager.GetObjectiveSiteState(m_iObjectiveSite)
                == BOT_OBJECTIVE_SITE_DESTROYED) {
                const int available = BotObjectiveFindSite(
                    BOT_OBJECTIVE_SITE_AVAILABLE,
                    m_iObjectiveSite + 1,
                    controlledEnt->origin
                );
                if (available >= 0) {
                    m_iObjectiveSite = available;
                }
            }

            const int    rank         = botManager.GetObjectiveBotRank(controlledEnt);
            const int    siteCount    = Q_max(botManager.GetObjectiveSiteCount(), 1);
            const Vector sitePosition = botManager.GetObjectiveSitePosition(m_iObjectiveSite);

            if (((rank / siteCount) & 1) == 0) {
                UpdateObjectivePatrol(sitePosition, enemySpawn, 320.0f, BOT_OBJECTIVE_HOLD);
            } else {
                Vector forward = BotObjectiveDirection(sitePosition, enemySpawn);
                Vector delta   = enemySpawn - sitePosition;
                delta.z        = 0.0f;
                const float  distance     = Q_min(1024.0f, delta.length() * 0.45f);
                const Vector patrolCenter = sitePosition + forward * distance;
                UpdateObjectivePatrol(patrolCenter, enemySpawn, 384.0f, BOT_OBJECTIVE_PATROL);
            }
        }
    }

    UpdateObjectiveProgress();
}

void BotController::FinalizeObjectiveCommand()
{
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
