/*
===========================================================================
Copyright (C) 2025 the OpenMoHAA team

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
// g_bot.cpp

#include "g_local.h"
#include "entity.h"
#include "playerbot.h"
#include "g_bot.h"

static saved_bot_t *saved_bots     = NULL;
static unsigned int num_saved_bots = 0;
static unsigned int botId          = 0;

Container<str> alliedModelList;
Container<str> germanModelList;

saved_bot_t::saved_bot_t()
    : userinfo {0}
{}

static unsigned int G_GetNumBotsToSpawn();
static bool G_IsBot(gentity_t *ent);

/*
===========
IsAlliedPlayerModel

Return whether or not the specified filename is for allies
============
*/
bool IsAlliedPlayerModel(const char *filename)
{
    return !Q_stricmpn(filename, "allied_", 7) || !Q_stricmpn(filename, "american_", 9);
}

/*
===========
IsGermanPlayerModel

Return whether or not the specified filename is for axis
============
*/
bool IsGermanPlayerModel(const char *filename)
{
    return !Q_stricmpn(filename, "german_", 7) || !Q_stricmpn(filename, "IT_", 3) || !Q_stricmpn(filename, "SC_", 3);
}

/*
===========
IsPlayerModel

Return whether or not the specified filename
is a player model that can be chosen
============
*/
bool IsPlayerModel(const char *filename)
{
    size_t len = strlen(filename);

    if (len >= 8 && !Q_stricmp(&filename[len - 8], "_fps.tik")) {
        return false;
    }

    if (!IsAlliedPlayerModel(filename) && !IsGermanPlayerModel(filename)) {
        return false;
    }

    return true;
}

/*
===========
ClearModelList

Clear the allied and axis model list
============
*/
void ClearModelList()
{
    alliedModelList.FreeObjectList();
    germanModelList.FreeObjectList();
}

/*
===========
InitModelList

Initialize the list of allied and axis player models
that bots can use
============
*/
void InitModelList()
{
    char **fileList;
    int    numFiles;
    int    i;
    size_t numAlliedModels = 0, numGermanModels = 0;

    ClearModelList();

    fileList = gi.FS_ListFiles("models/player", ".tik", qfalse, &numFiles);

    for (i = 0; i < numFiles; i++) {
        const char *filename = fileList[i];

        if (!IsPlayerModel(filename)) {
            continue;
        }

        if (IsAlliedPlayerModel(filename)) {
            numAlliedModels++;
        } else {
            numGermanModels++;
        }
    }

    alliedModelList.Resize(numAlliedModels);
    germanModelList.Resize(numGermanModels);

    for (i = 0; i < numFiles; i++) {
        const char *filename = fileList[i];
        size_t      len      = strlen(filename);

        if (!IsPlayerModel(filename)) {
            continue;
        }

        if (IsAlliedPlayerModel(filename)) {
            alliedModelList.AddObject(str(filename, 0, len - 4));
        } else {
            germanModelList.AddObject(str(filename, 0, len - 4));
        }
    }

    gi.FS_FreeFileList(fileList);
}

/*
===========
G_BotBegin

Begin spawning a new bot entity
============
*/
void G_BotBegin(gentity_t *ent)
{
    Player *player;

    level.spawn_entnum = ent->s.number;
    player             = new Player;

    G_ClientBegin(ent, NULL);

    botManager.getControllerManager().createController(player);
}

/*
===========
G_BotThink

Called each server frame to make bots think
============
*/
void G_BotThink(gentity_t *ent, int msec)
{
    // Controllers are advanced together by G_BotFrame.
    (void)ent;
    (void)msec;
}

/*
===========
G_FindFreeEntityForBot

Find a free client slot
============
*/
gentity_t *G_FindFreeEntityForBot()
{
    gentity_t *ent;
    int        i;

    // Bots permanently occupy real client slots and are reported as players.
    // Prefer high slots so bots do not consume low private-client reservations.
    for (i = maxclients->integer - 1; i >= 0; i--) {
        ent = &g_entities[i];
        if (!ent->inuse && ent->client && !ent->client->pers.userinfo[0]) {
            return ent;
        }
    }

    return NULL;
}

/*
===========
G_GetFirstBot

Return the first bot
============
*/
static gentity_t *G_GetFirstBot()
{
    gentity_t   *ent;
    unsigned int n;

    for (n = 0; n < game.maxclients; n++) {
        ent = &g_entities[n];
        if (G_IsBot(ent)) {
            return ent;
        }
    }

    return NULL;
}

/*
===========
G_IsBot

Return whether or not the gentity is a bot
============
*/
static bool G_IsBot(gentity_t *ent)
{
    return ent->inuse && ent->client && (ent->r.svFlags & SVF_BOT);
}

/*
===========
G_GetRandomAlliedPlayerModel
============
*/
const char *G_GetRandomAlliedPlayerModel()
{
    if (g_bot_allied_skin->string[0]) {
        return g_bot_allied_skin->string;
    }

    if (!alliedModelList.NumObjects()) {
        return "";
    }

    const unsigned int index = rand() % alliedModelList.NumObjects();
    return alliedModelList[index];
}

/*
===========
G_GetRandomGermanPlayerModel
============
*/
const char *G_GetRandomGermanPlayerModel()
{
    if (g_bot_axis_skin->string[0]) {
        return g_bot_axis_skin->string;
    }

    if (!germanModelList.NumObjects()) {
        return "";
    }

    const unsigned int index = rand() % germanModelList.NumObjects();
    return germanModelList[index];
}

/*
===========
G_AddBot

Add the specified bot, optionally its saved state
============
*/
gentity_t *G_AddBot(const bot_info_t *info)
{
    int        clientNum;
    gentity_t *e;
    char       botName[MAX_NETNAME];
    char       userinfo[MAX_INFO_STRING] {0};

    e = G_FindFreeEntityForBot();
    if (!e) {
        gi.DPrintf("BOT: no free slot for a new bot\n");
        return NULL;
    }

    clientNum = e - g_entities;

    // increase the unique ID
    botId++;

    if (info && info->name) {
        Q_strncpyz(botName, info->name, sizeof(botName));
        gi.DPrintf("BOT: using custom name '%s'\n", botName);
    } else {
        // Bot names remain sequential even if real player slots are sparse.
        const unsigned int num =
            botManager.getControllerManager().getControllers().NumObjects();

        cvar_t *v = gi.Cvar_Find(va("g_bot%d_name", num));
        if (v && *v->string) {
            Q_strncpyz(botName, v->string, sizeof(botName));
            gi.DPrintf("BOT: using cvar name '%s' from g_bot%d_name\n", botName, num);
        } else {
            Com_sprintf(botName, sizeof(botName), "bot%d", botId);
            gi.DPrintf("BOT: generated name '%s' (botId=%d)\n", botName, botId);
        }
    }

    Info_SetValueForKey(userinfo, "name", botName);

    //
    // Choose a random model
    //
    Info_SetValueForKey(userinfo, "dm_playermodel", G_GetRandomAlliedPlayerModel());
    Info_SetValueForKey(userinfo, "dm_playergermanmodel", G_GetRandomGermanPlayerModel());

    Info_SetValueForKey(userinfo, "fov", "80");
    Info_SetValueForKey(userinfo, "ip", "localhost");

    // Connect the bot for the first time
    // setup user info and stuff
    G_BotConnect(clientNum, qtrue, userinfo);
    G_BotBegin(e);

    gi.DPrintf("BOT: added '%s' in real client slot %d\n", botName, clientNum);

    return e;
}

/*
===========
G_RestoreBot

Restore the specified bot
============
*/
static gentity_t *G_RestoreBot(const saved_bot_t& saved)
{
    gentity_t *e;

    e = G_FindFreeEntityForBot();
    if (!e) {
        gi.DPrintf("BOT: no free slot for restoring bot\n");
        return NULL;
    }

    gi.DPrintf("BOT: restoring bot in slot %d\n", (int)(e - g_entities));

    G_BotConnect(e - g_entities, qfalse, saved.userinfo);
    G_BotBegin(e);

    return e;
}

/*
===========
G_AddBots

Add the specified number of bots
============
*/
static void G_AddBots(unsigned int num)
{
    int n;

    for (n = 0; n < num; n++) {
        G_AddBot();
    }
}

/*
===========
G_RemoveBot

Remove the specified bot
============
*/
static void G_RemoveBot(gentity_t *ent)
{
    int clientNum = ent - g_entities;

    gi.DPrintf(
        "BOT: removing '%s' from real client slot %d\n",
        ent->client ? ent->client->pers.netname : "?",
        clientNum
    );

    // Controller cleanup is now handled inside G_ClientDisconnect so that
    // both the explicit G_RemoveBot path and the server-side SV_DropClient
    // path (which bypasses G_RemoveBot) are covered.  No need to remove
    // the controller here – G_ClientDisconnect will take care of it.

    // All bots own real server slots, so always use the server drop path.
    gi.DropClient(clientNum, "removed");
}

/*
===========
G_RemoveBots

Remove the specified number of bots
============
*/
static void G_RemoveBots(unsigned int num)
{
    unsigned int removed = 0;
    unsigned int n;
    unsigned int teamCount[2] {0};
    bool         bNoMoreToRemove = false;

    teamCount[0] = dmManager.GetTeamAllies()->m_players.NumObjects();
    teamCount[1] = dmManager.GetTeamAxis()->m_players.NumObjects();

    while (!bNoMoreToRemove) {
        bNoMoreToRemove = true;

        // First remove bots that are in the team
        // with the higest player count
        for (n = 0; n < game.maxclients && removed < num; n++) {
            gentity_t *e = &g_entities[n];
            if (!G_IsBot(e)) {
                continue;
            }

            Player *player = static_cast<Player *>(e->entity);
            if (player->GetTeam() == TEAM_ALLIES || player->GetTeam() == TEAM_AXIS) {
                unsigned int teamIndex = (player->GetTeam() - TEAM_ALLIES);
                if (teamCount[teamIndex] < teamCount[1 - teamIndex]) {
                    // Not enough players in that team, don't remove the bot
                    continue;
                }

                teamCount[teamIndex]--;
                bNoMoreToRemove = false;
            }

            G_RemoveBot(e);
            removed++;
        }
    }

    //
    // Remove all bots that haven't been removed earlier
    //
    for (n = 0; n < game.maxclients && removed < num; n++) {
        gentity_t *e = &g_entities[n];
        if (!G_IsBot(e)) {
            continue;
        }

        G_RemoveBot(e);
        removed++;
    }
}

/*
===========
G_GetNumBots
============
*/
unsigned int G_GetNumBots()
{
    return botManager.getControllerManager().getControllers().NumObjects();
}

/*
===========
G_GetBotSkill
============
*/
const char *G_GetBotSkill()
{
    return "Average";
}

/*
===========
G_SaveBots

Save bot persistent data
============
*/
void G_SaveBots()
{
    unsigned int count;
    unsigned int n;

    if (saved_bots) {
        delete[] saved_bots;
        saved_bots = NULL;
    }

    const BotControllerManager& manager        = botManager.getControllerManager();
    unsigned int                numSpawnedBots = manager.getControllers().NumObjects();

    if (!numSpawnedBots) {
        return;
    }

    saved_bots     = new saved_bot_t[numSpawnedBots];
    num_saved_bots = 0;

    count = manager.getControllers().NumObjects();
    assert(count <= numSpawnedBots);

    for (n = 1; n <= count; n++) {
        const BotController *controller = manager.getControllers().ObjectAt(n);
        Player              *player     = controller->getControlledEntity();
        if (!player) {
            // this shouldn't happen
            continue;
        }

        saved_bot_t& saved = saved_bots[num_saved_bots++];
        memcpy(saved.userinfo, player->client->pers.userinfo, sizeof(saved.userinfo));
    }
}

/*
===========
G_RestoreBots

Restore bot persistent data, such as their team
============
*/
void G_RestoreBots()
{
    unsigned int n;

    if (saved_bots) {
        if (g_gametype->integer != GT_SINGLE_PLAYER) {
            const unsigned int numToSpawn = G_GetNumBotsToSpawn();

            if (num_saved_bots > numToSpawn) {
                num_saved_bots = numToSpawn;
            }

            for (n = 0; n < num_saved_bots; n++) {
                const saved_bot_t& saved = saved_bots[n];

                G_RestoreBot(saved);
            }
        }

        delete[] saved_bots;
        saved_bots = NULL;
    }
}

static unsigned int G_GetNumBotsToSpawn()
{
    const int requested = Q_max(sv_numbots->integer, 0);

    // Bots use real client slots. Human population never changes this target.
    return Q_min(requested, maxclients->integer);
}

/*
===========
G_RestartBots

Save bots
============
*/
void G_RestartBots()
{
    gi.DPrintf("BOT: restarting bots, resetting botId\n");
    G_SaveBots();

    // Map restarts keep the game module loaded. Remove all active bot entities
    // before restore to avoid duplicate bots or stale pre-spawn entities.
    while (true) {
        gentity_t *bot = G_GetFirstBot();
        if (!bot) {
            break;
        }

        G_RemoveBot(bot);
    }

    // Defensive cleanup in case a controller wasn't linked to an entity.
    botManager.Cleanup();

    botId = 0;
}

/*
===========
G_ResetBots

Save and reset the bot count
============
*/
void G_ResetBots()
{
    gi.DPrintf("BOT: resetting bots, cleaning up and resetting botId\n");

    botManager.Cleanup();

    botId = 0;
}

/*
===========
G_BotInit

Called to initialize bots
============
*/
void G_BotInit()
{
    InitModelList();
}

/*
===========
G_BotFrame

Called each frame to manage bots
============
*/
void G_BotFrame()
{
    botManager.Frame();
}

/*
===========
G_BotPostInit

Called after the server has spawned
============
*/
void G_BotPostInit()
{
    G_RestoreBots();
    G_SpawnBots();
}

/*
===========
G_SpawnBots

Called each frame to manage bot spawning
============
*/
void G_SpawnBots()
{
    unsigned int numBotsToSpawn;
    unsigned int numSpawnedBots;

    if (g_gametype->integer == GT_SINGLE_PLAYER) {
        // No bot on single-player
        return;
    }

    numBotsToSpawn = G_GetNumBotsToSpawn();
    numSpawnedBots = botManager.getControllerManager().getControllers().NumObjects();

    //
    // Spawn bots
    //
    if (numBotsToSpawn > numSpawnedBots) {
        gi.DPrintf("BOT: spawning %d bot(s) (target=%d, current=%d)\n", numBotsToSpawn - numSpawnedBots, numBotsToSpawn, numSpawnedBots);
        G_AddBots(numBotsToSpawn - numSpawnedBots);
    } else if (numBotsToSpawn < numSpawnedBots) {
        gi.DPrintf("BOT: removing %d bot(s) (target=%d, current=%d)\n", numSpawnedBots - numBotsToSpawn, numBotsToSpawn, numSpawnedBots);
        G_RemoveBots(numSpawnedBots - numBotsToSpawn);
    } else {
        sv_numbots->modified = false;
    }
}
