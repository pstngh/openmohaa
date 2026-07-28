#include "playerbot.h"
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
// playerbot_master.cpp: Multiplayer bot system.

#include "g_local.h"
#include "actor.h"
#include "playerbot.h"
#include "consoleevent.h"
#include "debuglines.h"
#include "scriptexception.h"
#include "vehicleturret.h"
#include "weaputils.h"
#include "movement_telemetry.h"

BotManager botManager;

CLASS_DECLARATION(Listener, BotControllerManager, NULL) {
    {NULL, NULL}
};

CLASS_DECLARATION(Listener, BotManager, NULL) {
    {NULL, NULL}
};

void BotManager::Init()
{
    ClearTeamContacts();
    ClearObjectiveSites();
    objectiveRound        = 0;
    objectiveSeed         = 0;
    objectiveRoundActive  = false;
    nextObjectiveScanTime = 0;
    botControllerManager.Init();
}

void BotManager::Cleanup()
{
    ClearTeamContacts();
    ClearObjectiveSites();
    botControllerManager.Cleanup();
}

void BotManager::Frame()
{
    UpdateObjectiveRound();
    botControllerManager.ThinkControllers();
}

void BotManager::BroadcastEvent(Entity *originator, Vector origin, int iType, float radius)
{
    Sentient      *ent;
    Actor         *act;
    Vector         delta;
    str            name;
    float          r2;
    float          dist2;
    int            i;
    int            iNumSentients;
    int            iAreaNum;
    BotController *controller;

    if (radius <= 0.0f) {
        radius = G_AIEventRadius(iType);
    }
    if (iType == AI_EVENT_WEAPON_FIRE) {
        radius = Q_max(radius, 4096.0f);
    }

    assert(originator);

    r2 = Square(radius);

    const Container<BotController *>& controllers = getControllerManager().getControllers();
    for (i = 1; i <= controllers.NumObjects(); i++) {
        controller = controllers.ObjectAt(i);
        ent        = controller->getControlledEntity();
        if (!ent || ent == originator || ent->deadflag) {
            continue;
        }

        delta = origin - ent->centroid;

        // dot product returns length squared
        dist2 = Square(delta);

        if (originator) {
            iAreaNum = originator->edict->r.areanum;
        } else {
            iAreaNum = gi.AreaForPoint(origin);
        }

        if (dist2 > r2) {
            continue;
        }

        if (iAreaNum != ent->edict->r.areanum && !gi.AreasConnected(iAreaNum, ent->edict->r.areanum)) {
            continue;
        }

        controller->NoticeEvent(origin, iType, originator, dist2, r2);
    }
}

int BotManager::TeamContactIndex(teamtype_t team)
{
    if (team == TEAM_ALLIES) {
        return 0;
    }
    if (team == TEAM_AXIS) {
        return 1;
    }
    return -1;
}

void BotManager::ClearTeamContacts()
{
    for (int team = 0; team < 2; ++team) {
        for (int enemy = 0; enemy < MAX_CLIENTS; ++enemy) {
            teamContacts[team][enemy].Clear();
        }
    }
}

void BotManager::ReportTeamContact(
    Player *reporter, Player *enemy, bot_contact_source_t source, const Vector& position, float uncertainty
)
{
    if (g_gametype->integer < GT_TEAM || !reporter || !enemy || reporter == enemy || enemy->entnum < 0
        || enemy->entnum >= MAX_CLIENTS || reporter->GetTeam() == enemy->GetTeam()) {
        return;
    }

    const int teamIndex = TeamContactIndex(reporter->GetTeam());
    if (teamIndex < 0) {
        return;
    }

    bot_team_contact_t& contact = teamContacts[teamIndex][enemy->entnum];
    const bool active = contact.valid && contact.enemy == enemy && level.inttime < contact.expireTime;
    const bool stronger = !active || source > contact.source;

    if (active && !stronger && source < contact.source && level.inttime - contact.reportTime < 1500) {
        return;
    }
    if (active && !stronger && level.inttime - contact.reportTime < 350) {
        return;
    }

    Vector reportedPosition = position;
    if (uncertainty > 0.0f) {
        const float angle  = G_Random(6.28318530718f);
        const float radius = G_Random(uncertainty);
        reportedPosition.x += cos(angle) * radius;
        reportedPosition.y += sin(angle) * radius;
    }

    const bool shouldLog = !active || stronger || level.inttime - contact.lastLogTime >= 1000;
    const int previousAvailableTime = contact.availableTime;

    contact.valid         = true;
    contact.source        = source;
    contact.reportTime    = level.inttime;
    contact.availableTime =
        active ? previousAvailableTime : level.inttime + 300 + (int)G_Random(900.0f);
    contact.expireTime = level.inttime + 10000;
    contact.position   = reportedPosition;
    contact.reporter   = reporter;
    contact.enemy      = enemy;

    if (shouldLog) {
        const char *eventName = "bot_contact";
        if (source == BOT_CONTACT_VISUAL) {
            eventName = "bot_contact_visual";
        } else if (source == BOT_CONTACT_DAMAGE) {
            eventName = "bot_contact_damage";
        } else if (source == BOT_CONTACT_SOUND) {
            eventName = "bot_contact_sound";
        }

        G_MoveLogBotEvent(eventName, reporter, enemy, static_cast<int>(source), reportedPosition);
        contact.lastLogTime = level.inttime;
    }
}

bool BotManager::FindTeamContact(BotController *controller, bot_team_contact_t& result) const
{
    Player *player = controller ? controller->getControlledEntity() : NULL;
    if (!player || !controller->CanRespondToTeamContact()) {
        return false;
    }

    const int teamIndex = TeamContactIndex(player->GetTeam());
    if (teamIndex < 0) {
        return false;
    }

    bool  found     = false;
    float bestScore = 0.0f;

    for (int enemyNum = 0; enemyNum < MAX_CLIENTS; ++enemyNum) {
        const bot_team_contact_t& contact = teamContacts[teamIndex][enemyNum];
        if (!contact.valid || !contact.enemy || contact.enemy->IsDead() || contact.enemy->IsSpectator()
            || level.inttime < contact.availableTime || level.inttime >= contact.expireTime) {
            continue;
        }
        if (!controller->CanRespondToTeamContact(contact.position)) {
            continue;
        }

        const float playerDistance = (player->origin - contact.position).length();
        const float playerRankDistance =
            playerDistance - (controller->IsRespondingToTeamContact(enemyNum) ? 256.0f : 0.0f);
        int         closerBots     = 0;
        const Container<BotController *>& controllers = botControllerManager.getControllers();

        for (int i = 1; i <= controllers.NumObjects(); ++i) {
            BotController *candidateController = controllers.ObjectAt(i);
            Player        *candidate           = candidateController->getControlledEntity();
            if (candidateController == controller || !candidate || candidate->GetTeam() != player->GetTeam()
                || !candidateController->CanRespondToTeamContact(contact.position)) {
                continue;
            }

            const float candidateDistance = (candidate->origin - contact.position).length();
            const float candidateRankDistance =
                candidateDistance
                - (candidateController->IsRespondingToTeamContact(enemyNum) ? 256.0f : 0.0f);
            if (candidateRankDistance < playerRankDistance
                || (candidateRankDistance == playerRankDistance && candidate->entnum < player->entnum)) {
                ++closerBots;
            }
        }

        if (closerBots >= 2) {
            continue;
        }

        const float ageSeconds = (level.inttime - contact.reportTime) / 1000.0f;
        const float score =
            playerDistance + ageSeconds * 64.0f - static_cast<float>(contact.source) * 128.0f;
        if (!found || score < bestScore) {
            found     = true;
            bestScore = score;
            result    = contact;
        }
    }

    return found;
}

BotControllerManager& BotManager::getControllerManager()
{
    return botControllerManager;
}
