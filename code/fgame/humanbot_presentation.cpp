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
// humanbot_presentation.cpp: how the bots show up to players.
//
// By default bots are labelled as bots (stock-style names, "bot" in the ping
// column). With g_humanbot_disguise 1 (a test mode) they take names from
// humanbot/names.txt, show a realistic ping and are listed like players in
// the server status queries. Bots never chat either way.

#include "humanbot_internal.h"
#include "playerbot.h"
#include "g_bot.h"

// Used when humanbot/names.txt is missing: generic handles, none of them a real person.
static const char *const DEFAULT_NAMES[] = {
    "Kestrel",   "Mortimer", "sgt.Rook",  "Havoc",     "Juniper",  "Tallboy",  "Brisk",     "Cobalt",   "Fennec",
    "Grinder",   "Halcyon",  "IronLung",  "Jackdaw",   "Kilroy",   "Lamplight", "Marauder", "Nettle",   "Outrider",
    "Pilgrim",   "Quartz",   "Ranger_7",  "Sawtooth",  "Thistle",  "Umber",    "Vesper",    "Wardog",   "Yarrow",
    "Zephyr",    "Ashcan",   "Bramble",   "Cinder",    "Dustoff",  "Ember",    "Flintlock", "Gunnar",   "Hollow",
    "Ivory",     "Jolt",     "Kettle",    "Lowkey",    "Muddy",    "Nimbus",   "Oakley",    "Pepper",   "Quill",
    "Rascal",    "Spud",     "Tinder",    "Upshot",    "Violet",   "Wicket",   "Yonder",    "Zulu_2",   "Bighorn",
};

bool HB_DisguiseActive()
{
    return g_humanbot_disguise && g_humanbot_disguise->integer != 0;
}

static std::vector<std::string> LoadNames()
{
    std::vector<std::string> names;
    std::string              text;
    void                    *buffer = NULL;
    const std::string        paths[] = {
        g_humanbot_model_dir && g_humanbot_model_dir->string[0] ? std::string(g_humanbot_model_dir->string) + "/names.txt"
                                                                 : std::string(),
        "humanbot/names.txt",
    };
    for (const std::string& path : paths) {
        if (path.empty()) {
            continue;
        }
        const long len = gi.FS_ReadFile(path.c_str(), &buffer, qtrue);
        if (len > 0 && buffer) {
            text.assign(static_cast<const char *>(buffer), static_cast<size_t>(len));
            gi.FS_FreeFile(buffer);
            names = hb::ParseNames(text);
            if (!names.empty()) {
                return names;
            }
        }
    }
    for (const char *n : DEFAULT_NAMES) {
        names.push_back(n);
    }
    return names;
}

std::string HB_PickDisguiseName(int clientNum)
{
    static std::vector<std::string> names;
    if (names.empty()) {
        names = LoadNames();
    }
    std::vector<std::string> used;
    for (int i = 0; i < game.maxclients; i++) {
        const gentity_t *ent = &g_entities[i];
        if (ent->inuse && ent->client && ent->client->pers.netname[0]) {
            used.push_back(ent->client->pers.netname);
        }
    }
    static hb::Rng rng(hb::HashString(g_humanbot_seed ? g_humanbot_seed->string : "") ^ 0x6e616d6573ULL);
    (void)clientNum;
    return hb::PickName(names, used, rng);
}

// Ping shown for a client in the server status queries: the disguised ping of a
// bot, or -1 for anything else (humans keep the server's measured ping).
int G_HumanBotDisplayPing(int clientNum)
{
    if (!HB_DisguiseActive() || clientNum < 0 || clientNum >= game.maxclients) {
        return -1;
    }
    gentity_t *ent = &g_entities[clientNum];
    if (!ent->inuse || !ent->client || !(ent->r.svFlags & SVF_BOT)) {
        return -1;
    }
    return ent->client->ps.ping > 0 ? ent->client->ps.ping : 45;
}

unsigned int G_HumanBotNumSimulatedPlayers()
{
    // disguised bots are presented as players
    return HB_DisguiseActive() ? 0 : G_GetNumBots();
}

bool G_HumanBotDisguised(void)
{
    return HB_DisguiseActive() && HB_Bundle() != nullptr;
}

const char *G_HumanBotDisguiseName(int clientNum)
{
    static std::string name;
    if (!G_HumanBotDisguised()) {
        return NULL;
    }
    name = HB_PickDisguiseName(clientNum);
    return name.c_str();
}
