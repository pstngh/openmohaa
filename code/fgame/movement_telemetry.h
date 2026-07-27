/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

Opt-in, server-side movement and aim telemetry used to compare human and bot
play.  These hooks must remain observational: they must not alter simulation
state or consume random numbers.
===========================================================================
*/

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
void G_MoveLogBotEvent(
    const char *eventName, Player *actor, Player *target, int detail, const Vector& position
);
