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

// A "bot_style" event for `bot`: the style dials go in the chat_message column.
void G_MoveLogBotStyle(Player *bot, const char *dials);
