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
// humanbot_adapter.cpp: engine glue of the human-imitation bots.
//
// Interim stub: the telemetry logger links against these entry points while
// the brain library is being built.

#include "g_local.h"
#include "humanbot_adapter.h"
#include "../humanbot/hb_diag.h"

void G_HumanBotInit(void) {}

void G_HumanBotShutdown(void) {}

void G_HumanBotFrame(void) {}

bool G_HumanBotGetDiag(Player *player, hb::Diag *out)
{
    if (out) {
        *out = hb::Diag();
    }
    return false;
}

const char *G_HumanBotModelSha256(void)
{
    return "";
}

const char *G_HumanBotMetaLines(void)
{
    return "";
}

void G_HumanBotObserveParts(Player *viewer, Player *target, int *visibleParts, int *inFov)
{
    if (visibleParts) {
        *visibleParts = 0;
    }
    if (inFov) {
        *inFov = 0;
    }
}

void G_HumanBotEmitSound(Entity *source, const Vector& origin, int soundType, float radius) {}

void G_HumanBotDamage(
    Sentient *victim, Entity *attacker, float damage, const Vector& position, const Vector& direction, int meansOfDeath
)
{}
