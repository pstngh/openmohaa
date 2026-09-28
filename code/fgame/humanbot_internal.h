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
// humanbot_internal.h: shared declarations of the human-bot engine glue
// (humanbot_*.cpp). Nothing outside those files includes this header.

#pragma once

#include "g_local.h"
#include "player.h"
#include "humanbot_adapter.h"

#include "../humanbot/hb_brain.h"
#include "../humanbot/hb_bundle.h"
#include "../humanbot/hb_diag.h"
#include "../humanbot/hb_eye_math.h"
#include "../humanbot/hb_map.h"
#include "../humanbot/hb_perception_model.h"
#include "../humanbot/hb_presentation.h"
#include "../humanbot/hb_style.h"
#include "../humanbot/hb_substep.h"

#include <string>
#include <vector>

class BotController;
class IPather;

//
// Cvars (humanbot_adapter.cpp)
//
extern cvar_t *g_humanbot_substeps;
extern cvar_t *g_humanbot_fov;
extern cvar_t *g_humanbot_aspect;
extern cvar_t *g_humanbot_seed;
extern cvar_t *g_humanbot_disguise;
extern cvar_t *g_humanbot_model_dir;
extern cvar_t *g_humanbot_families;
extern cvar_t *g_humanbot_debug;
extern cvar_t *g_humanbot_wall_steer;

// The active model, or nullptr when it failed to load (the stock bots then run).
const hb::ModelBundle *HB_Bundle();
// Loads (or reloads) the model: the embedded bundle merge-patched with the JSON
// files found in g_humanbot_model_dir. Returns false and fills `error` when the
// overrides were rejected (the embedded model is then used).
bool HB_LoadModel(std::string& error);

//
// Map context (humanbot_world.cpp)
//
struct HbWorldStatus {
    bool        navmesh     = false;  // the engine navigation mesh is valid
    bool        prior       = false;  // a map prior is in use
    bool        embedded    = false;  // it is a recorded human prior (else derived from the navmesh)
    bool        recorded    = false;  // a recorded human prior exists for this map
    bool        checksumOk  = false;  // and its checksum matches this map file
    int         cells       = 0;
    int         snapped     = 0;      // cells whose centre lies on the navmesh
    int         spawns      = 0;
    bool        visReady    = false;  // the trace-built visibility table is complete
    float       visProgress = 0.0f;
    int         visMs       = 0;      // time spent building it
    bool        visCached   = false;  // loaded from the cache file
    std::string mapName;
    std::string note;
};

void                HB_WorldReset();         // new level
void                HB_WorldFrame();         // lazy set-up and time-sliced visibility build
const hb::MapPrior *HB_WorldPrior();         // nullptr until the context is ready
HbWorldStatus       HB_WorldGetStatus();

//
// Perception glue (humanbot_perception.cpp)
//
struct HbView {
    Vector eye;        // leaned eye (EyePosition) used for sight
    Vector logEye;     // unleaned eye (GetPlayerView), for the logger-equivalent centroid ray
    float  yaw   = 0.0f;
    float  pitch = 0.0f;
};

// Horizontal and vertical field of view (degrees) of the bot screen: Hor+ from
// g_humanbot_fov at 4:3 to g_humanbot_aspect, like CG_CalcFov.
void HB_Fov(float& hfov, float& vfov);
HbView HB_ViewOf(Player *player);

// Body-part positions of a player this server frame (cached per frame).
// Returns false when the player has no usable model.
bool HB_PartPositions(Player *target, Vector parts[hb::NUM_PARTS]);

// Visibility of `target` from `view`: parts inside the frustum and unoccluded,
// whether any part is inside the frustum, and the logger's centroid ray.
// No randomness, no side effects on the game state.
struct HbSight {
    bool inFov       = false;
    int  partMask    = 0;
    bool centroidLos = false;
    int  traces      = 0;
};
HbSight HB_SightOf(Player *viewer, const HbView& view, Player *target, bool fullCheck);

// Own state for the brain.
void HB_FillSelf(Player *player, const HbView& view, hb::SelfState& self);
// Clearance (the logger's box probe, up to 128 u) in the 8 chord directions relative to
// the view yaw, and the depth of the floor just ahead for half of them (dropPhase 0 or
// 1: odd or even chord indices; -1 none), so each direction is probed every other tick.
void HB_FillClearance(Player *player, float viewYaw, float clearance[hb::NUM_CHORDS], float drop[hb::NUM_CHORDS], int dropPhase);

//
// Event buses (humanbot_adapter.cpp)
//
struct HbSoundEvent {
    int    type     = hb::SOUND_OTHER;
    int    sourceId = -1;
    Vector origin;
    float  radius   = 0.0f;
    int    areanum  = -1;
};

struct HbDamageEvent {
    int    victimId   = -1;
    int    attackerId = -1;
    Vector attackerPos;
    float  damage     = 0.0f;
};

//
// Leaned eye (humanbot_eye.cpp)
//

// Advances the client's first-person eye model (CG_OffsetFirstPersonView) by one
// client frame of `frameMs` at the given view angles; returns the desired eye
// before the clip traces.
Vector HB_EyeStep(Player *player, hb::EyeState& state, float yaw, float pitch, float frameMs);
// The client's two clip traces (height, then lateral) from the unleaned eye.
Vector HB_EyeClip(Player *player, const Vector& desired);
// usereyes offset from the origin, rounded like CL_EyeInfo.
void HB_EyeOffset(Player *player, const Vector& eye, signed char ofs[3]);

//
// Presentation (humanbot_presentation.cpp)
//
std::string HB_PickDisguiseName(int clientNum);
bool        HB_DisguiseActive();

//
// Adapter bookkeeping (humanbot_adapter.cpp)
//
class HumanBotAdapter;
HumanBotAdapter *HB_AdapterFor(const Player *player);
void             HB_ListBots();
int              HB_PendingFamily();          // family requested by addbotstyle for the next bot, -1 if none
void             HB_SetPendingFamily(int family);

//
// Self test (humanbot_commands.cpp)
//
void HB_SelfTestFrame();
