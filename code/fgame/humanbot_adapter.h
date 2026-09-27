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

class Entity;
class Player;
class Sentient;
class Vector;

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
void G_HumanBotFrame(void);

//
// Telemetry support
//

// Fills the per-tick brain diagnostics of a human bot. Returns false (and leaves
// the defaults) for humans and for bots without a brain.
bool G_HumanBotGetDiag(Player *player, hb::Diag *out);

// SHA-256 (hex) of the active model bundle, or an empty string.
const char *G_HumanBotModelSha256(void);

// Human-fair view of `target` from `viewer`'s eye: the number of visible body
// parts (head, upper and lower spine, pelvis, feet) and whether any part is
// inside the viewer's 16:9 frustum at the bot fov. Used by the logger for the
// ext_vis_parts / ext_in_fov columns, for humans and bots alike.
void G_HumanBotObserveParts(Player *viewer, Player *target, int *visibleParts, int *inFov);

//
// Event buses
//
void G_HumanBotEmitSound(Entity *source, const Vector& origin, int soundType, float radius);
void G_HumanBotDamage(
    Sentient     *victim,
    Entity       *attacker,
    float         damage,
    const Vector& position,
    const Vector& direction,
    int           meansOfDeath
);
