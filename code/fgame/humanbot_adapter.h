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
// humanbot_adapter.h: engine-side entry points of the human-imitation bots.
//
// The brain itself (code/humanbot) never sees engine types. Everything the
// engine tells a bot goes through the perception glue, and everything the
// bot does goes out as ordinary usercmds.

#pragma once

class BotController;
class Entity;
class HumanBotAdapter;
class Player;
class Sentient;
class Vector;
struct gentity_s;

namespace hb
{
struct Diag;
}

//
// Sound bus categories (see G_HumanBotEmitSound)
//
enum hb_sound_type_t {
    HB_SOUND_FOOTSTEP,
    HB_SOUND_GUNFIRE,
    HB_SOUND_RELOAD,
    HB_SOUND_IMPACT,
    HB_SOUND_DOOR,
    HB_SOUND_OTHER,
};

//
// Life cycle
//
void G_HumanBotInit(void);
void G_HumanBotShutdown(void);
void G_HumanBotFrame(void);  // map context and self test, once per server frame

//
// Bots (called by BotController / BotControllerManager)
//

// The adapter of a new bot, or nullptr when no model is loaded (the stock bot then runs).
HumanBotAdapter *G_HumanBotCreate(BotController *controller, Player *player);
void             G_HumanBotDestroy(HumanBotAdapter *adapter);
// Two-phase frame: every bot perceives and decides on the same world snapshot,
// then every bot sends its usercmds.
void G_HumanBotBeginFrame(void);
void G_HumanBotPrepare(HumanBotAdapter *adapter);
void G_HumanBotCommit(HumanBotAdapter *adapter);
void G_HumanBotSpawned(HumanBotAdapter *adapter);
void G_HumanBotKilled(HumanBotAdapter *adapter);
void G_HumanBotGotKill(HumanBotAdapter *adapter, Entity *victim);
void G_HumanBotReinitAll(void);
void G_HumanBotResetCounters(void);
bool G_HumanBotReportCounters(float minutes);

//
// Presentation
//

// A human-looking name for a new bot while g_humanbot_disguise is on, else NULL.
const char  *G_HumanBotDisguiseName(int clientNum);
bool         G_HumanBotDisguised(void);
// Ping to show for a client in server status queries: a disguised bot's ping, or -1.
int          G_HumanBotDisplayPing(int clientNum);
unsigned int G_HumanBotNumSimulatedPlayers(void);

//
// Server commands
//
int G_AddBotStyleCommand(struct gentity_s *ent);
int G_HumanBotListCommand(struct gentity_s *ent);
int G_HumanBotReloadCommand(struct gentity_s *ent);
int G_HumanBotSelfTestCommand(struct gentity_s *ent);

//
// Telemetry support
//

// Fills the per-tick brain diagnostics of a human bot. Returns false (and leaves
// the defaults) for humans and for bots without a brain.
bool G_HumanBotGetDiag(Player *player, hb::Diag *out);

// SHA-256 (hex) of the active model bundle, or an empty string.
const char *G_HumanBotModelSha256(void);

// "key=value\n" lines describing the bot configuration for the telemetry
// metadata: every g_humanbot_* cvar (the logger adds hb_model_sha256 itself).
const char *G_HumanBotMetaLines(void);

// Human-fair view of `target` from `viewer`'s eye: the number of visible body
// parts (head, upper and lower spine, pelvis, feet) and whether any part is
// inside the viewer's 16:9 frustum at the bot fov. Used by the logger for the
// ext_vis_parts / ext_in_fov columns, for humans and bots alike.
void G_HumanBotObserveParts(Player *viewer, Player *target, int *visibleParts, int *inFov);

//
// Event buses
//
void G_HumanBotEmitSound(Entity *source, const Vector& origin, int soundType, float radius);
// G_BroadcastAIEvent: footsteps, impacts, doors (weapon fire comes from Weapon::Shoot).
void G_HumanBotAIEvent(Entity *source, const Vector& origin, int aiEventType, float radius);
// Player::Killed: the kill feed every player sees.
void G_HumanBotDeath(Player *victim);
void G_HumanBotDamage(
    Sentient     *victim,
    Entity       *attacker,
    float         damage,
    const Vector& position,
    const Vector& direction,
    int           meansOfDeath
);
