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
// movement_telemetry.h: opt-in (g_movelog), server-side movement and aim
// telemetry used to compare human and bot play. The column contract lives in
// movement_telemetry_schema.h.
//
// These hooks must remain observational: they must not alter simulation state
// or consume random numbers.

#pragma once

class Entity;
class Player;
class Sentient;
class Vector;
class Weapon;

void G_MoveLogFrame();
void G_MoveLogShutdown();

void G_MoveLogShot(Sentient *owner, Weapon *weapon, int mode, const Vector& position, const Vector& forward);
void G_MoveLogReload(Sentient *owner, Weapon *weapon);
void G_MoveLogDamage(
    Sentient     *victim,
    Sentient     *attacker,
    float         damage,
    float         healthBefore,
    float         healthAfter,
    int           meansOfDeath,
    int           location,
    const Vector& position,
    const Vector& direction
);
void G_MoveLogDeath(Player *victim, Entity *attacker, int meansOfDeath, int location);
void G_MoveLogSpawn(Player *player);
void G_MoveLogClientBegin(Player *player);
void G_MoveLogClientDisconnect(Player *player);
void G_MoveLogChat(Player *player, int mode, const char *message);
void G_MoveLogBotEvent(const char *eventName, Player *actor, Entity *target, int eventDetail, const Vector& position);

// The wall probe of the ext_wall_* columns, shared with the human bots (g_humanbot_step_walls): how far the player's box
// goes in `direction` (flat, unit length) stepping up what the player steps up (STEPSIZE, as PM_StepSlideMove does: a
// curb, a stair or rubble is no wall to someone walking), at most `range`. `footClear` is the box's clearance from the
// feet in that direction (the clear_* probe); `start` is G_WallProbeStart(player), the box raised STEPSIZE above the
// origin, once per player and frame.
Vector G_WallProbeStart(Player *player);
float  G_WallClearance(Player *player, const Vector& start, const Vector& direction, float footClear, float range);

// A "bot_style" event for `bot`: the style dials go in the chat_message column.
void G_MoveLogBotStyle(Player *bot, const char *dials);
