/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

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
// humanbot_commands.cpp: server commands of the human bots.
//
//   addbotstyle <presser|strafer|stopper|random> [name]   add a bot of a style family
//   humanbot_list                                          every bot's family and dials
//   humanbot_reload                                        reload the model (g_humanbot_model_dir)
//   humanbot_selftest [seconds]                            map context report, then a timed bot game

#include "humanbot_internal.h"
#include "playerbot.h"
#include "g_bot.h"

qboolean G_AddBotStyleCommand(gentity_t *ent)
{
    if (gi.Argc() <= 1) {
        gi.Printf("Usage: addbotstyle <presser|strafer|stopper|random> [name]\n");
        return qfalse;
    }
    if (!HB_Bundle()) {
        gi.Printf("addbotstyle: no human-bot model is loaded\n");
        return qfalse;
    }
    const char *family = gi.Argv(1);
    const int   f      = hb::FamilyFromName(family);
    if (f < 0 && Q_stricmp(family, "random")) {
        gi.Printf("addbotstyle: unknown family '%s' (presser, strafer, stopper or random)\n", family);
        return qfalse;
    }
    const unsigned int total = Q_min(sv_numbots->integer + 1, sv_maxbots->integer);
    if (static_cast<int>(total) <= sv_numbots->integer) {
        gi.Printf("addbotstyle: sv_maxbots (%d) reached\n", sv_maxbots->integer);
        return qfalse;
    }
    gi.cvar_set("sv_numbots", va("%d", total));

    HB_SetPendingFamily(f);
    bot_info_t  info;
    const bool  named = gi.Argc() > 2;
    if (named) {
        info.name = gi.Argv(2);
    }
    gentity_t *e = G_AddBot(named ? &info : NULL);
    HB_SetPendingFamily(-1);
    if (e && named) {
        gi.cvar_set(va("g_bot%d_name", G_GetBotId(e)), e->client->pers.netname);
    }
    return qtrue;
}

qboolean G_HumanBotListCommand(gentity_t *ent)
{
    HB_ListBots();
    return qtrue;
}

qboolean G_HumanBotReloadCommand(gentity_t *ent)
{
    std::string error;
    const bool  ok = HB_LoadModel(error);
    if (!HB_Bundle()) {
        gi.Printf("humanbot_reload: no usable model\n");
        return qfalse;
    }
    gi.Printf(
        "humanbot_reload: %s model %.12s (%s)%s%s\n",
        ok ? "loaded" : "overrides rejected, embedded",
        HB_Bundle()->sha256.c_str(),
        HB_Bundle()->source.c_str(),
        error.empty() ? "" : ": ",
        error.c_str()
    );
    G_HumanBotReinitAll();
    return qtrue;
}

//
// Self test
//
static int  s_testEnd   = 0;
static int  s_testStart = 0;
static bool s_testing   = false;

static void PrintWorld()
{
    const HbWorldStatus st = HB_WorldGetStatus();
    gi.Printf("humanbot_selftest: map %s\n", st.mapName.c_str());
    gi.Printf("  navigation mesh   %s\n", st.navmesh ? "valid" : "MISSING (set sv_maxbots > 0 before the map loads)");
    gi.Printf(
        "  map prior         %s%s, %d cells, %d on the navmesh, %d spawns\n",
        st.prior ? (st.embedded ? "recorded human prior" : "derived from the navmesh") : "NONE",
        st.recorded ? (st.checksumOk ? ", checksum ok" : ", checksum MISMATCH") : "",
        st.cells,
        st.snapped,
        st.spawns
    );
    gi.Printf(
        "  visibility        %s (%.0f%%, %d ms)\n",
        st.visReady ? (st.visCached ? "ready (cache)" : "ready") : "building",
        st.visProgress * 100.0f,
        st.visMs
    );
    if (!st.note.empty()) {
        gi.Printf("  note              %s\n", st.note.c_str());
    }
}

qboolean G_HumanBotSelfTestCommand(gentity_t *ent)
{
    if (!HB_Bundle()) {
        gi.Printf("humanbot_selftest: FAIL no human-bot model is loaded\n");
        return qfalse;
    }
    PrintWorld();
    const int seconds = gi.Argc() > 1 ? hb::ClampI(atoi(gi.Argv(1)), 10, 3600) : 60;
    if (G_GetNumBots() < 2) {
        gi.Printf("  fewer than 2 bots: raising sv_numbots to 2 for the test\n");
        gi.cvar_set("sv_numbots", va("%d", Q_min(2, sv_maxbots->integer)));
    }
    G_HumanBotResetCounters();
    s_testStart = level.inttime;
    s_testEnd   = level.inttime + seconds * 1000;
    s_testing   = true;
    gi.Printf("  running a %d s bot game...\n", seconds);
    return qtrue;
}

void HB_SelfTestFrame()
{
    if (!s_testing || level.inttime < s_testEnd) {
        return;
    }
    s_testing = false;
    const float minutes = (s_testEnd - s_testStart) / 60000.0f;
    PrintWorld();
    const bool pass = G_HumanBotReportCounters(minutes);
    gi.Printf("humanbot_selftest: %s\n", pass ? "PASS" : "FAIL");
}
