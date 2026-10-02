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
// hb_types.h: what the bot brain perceives and what it decides.
//
// Two layers of input exist. Raw* structs are filled by the engine glue and
// hold only what a human could see or hear: body-part positions are set only
// for parts that are actually visible, and sounds and damage carry their true
// source, which hb::PerceptionModel turns into noisy human-like observations.
// hb::Brain receives only the noisy Observation, never a hidden position. The
// one other thing it may ask is whether the map's geometry blocks a line of
// sight (WorldQuery): players are never traced, so no answer depends on where
// an enemy is.

#pragma once

#include "hb_math.h"

#include <cstdint>
#include <string>
#include <vector>

namespace hb
{

enum Owner {
    OWNER_BRAIN,
    OWNER_LADDER,
    OWNER_DOOR,
    OWNER_RECOVERY,
    OWNER_DEAD,
    OWNER_MANUAL,
    OWNER_SPECTATOR,
};

enum ViewMode {
    VIEW_TRACK,
    VIEW_FLICK,
    VIEW_PREAIM,
    VIEW_LOOKAROUND,
    VIEW_TRAVEL,
    VIEW_DAMAGE,
    VIEW_SOUND,
    VIEW_HOLD,
    VIEW_BELIEF,   // watching where the enemy is believed to be (through walls)
};

enum Intent {
    INTENT_SPAWN_PUSH,
    INTENT_HUNT,
    INTENT_HOLD,
    INTENT_ENGAGE,
    INTENT_RELOAD_COVER,
    INTENT_POST_KILL,
};

enum WeaponClass {
    WEAPON_CLASS_NONE,
    WEAPON_CLASS_PISTOL,
    WEAPON_CLASS_SMG,
    WEAPON_CLASS_RIFLE,
    WEAPON_CLASS_SNIPER,
    WEAPON_CLASS_HEAVY,
    WEAPON_CLASS_GRENADE,
    WEAPON_CLASS_OTHER,
};

enum SoundType {
    SOUND_FOOTSTEP,
    SOUND_GUNFIRE,
    SOUND_RELOAD,
    SOUND_IMPACT,
    SOUND_DOOR,
    SOUND_OTHER,
};

enum BodyPart {
    PART_HEAD,
    PART_CHEST,
    PART_BELLY,
    PART_PELVIS,
    PART_LFOOT,
    PART_RFOOT,
    NUM_PARTS
};

enum Command {
    CMD_NONE,
    CMD_RELOAD,
    CMD_PRIMARY,  // switch to the primary (SMG) weapon
    CMD_PISTOL,   // switch to the pistol
};

//
// The map's static geometry, for the brain's own line-of-sight questions (where would an enemy on
// a believed path come out of cover). Implemented by the engine glue and the test harnesses. Bodies
// are never traced: a trace stopped by a hidden enemy would give its position away.
//
class WorldQuery
{
public:
    virtual ~WorldQuery() = default;
    // True when nothing of the map blocks sight from a to b.
    virtual bool Clear(const Vec3& a, const Vec3& b) const = 0;
};

//
// Own state, read from the engine every tick
//
struct SelfState {
    int   timeMs    = 0;
    int   entnum    = -1;
    int   team      = 0;
    bool  alive     = false;
    bool  spectator = true;
    Vec3  origin;
    Vec3  velocity;
    float viewYaw   = 0.0f;
    float viewPitch = 0.0f;
    Vec3  eye;                    // leaned eye used for perception and firing
    Vec3  aimEye;                 // unleaned eye (origin + viewheight): the frame of the recorded aim heights
    bool  aimEyeValid = false;
    float health    = 0.0f;
    float maxHealth = 100.0f;
    bool  onGround  = true;
    bool  ducked    = false;
    bool  onLadder  = false;
    int   weaponClass   = WEAPON_CLASS_NONE;
    int   weaponState   = -1;     // telemetry convention: -1 none, 0 ready, 1 firing, 5 reloading
    int   clipAmmo      = 0;
    int   clipSize      = 0;
    int   reserveAmmo   = 0;
    bool  hasPistol     = false;
    bool  switching     = false;  // weapon change in progress
    float clearance[9]  = {128, 128, 128, 128, 128, 128, 128, 128, 128};  // per chord direction, <= 128
    float drop[9]       = {};     // floor drop 32 u ahead per chord direction (0 = level)
    bool  navSteerValid = false;  // navmesh steering toward the last nav target
    float navSteerYaw   = 0.0f;   // world yaw of the next path corner
    float navPathLen    = 0.0f;
    float stuckMs       = 0.0f;   // pressing movement keys without progress
    float wallPressMs   = 0.0f;   // pressing into geometry
};

//
// Raw percepts (engine glue -> PerceptionModel). Human-fair by construction.
//
struct RawEnemy {
    int   id          = -1;
    bool  alive       = false;
    bool  inFov       = false;    // some part inside the 16:9 frustum
    int   partMask    = 0;        // visible parts: inside the frustum and unoccluded
    Vec3  partPos[NUM_PARTS];     // world positions, only for parts in partMask
    float bodyHeight  = 94.0f;
    bool  centroidLos = false;    // logger-equivalent centroid ray, frustum gated
    Vec3  centroid;               // only when partMask != 0
    Vec3  velocity;               // only when partMask != 0
    bool  reloading   = false;    // only when partMask != 0
    bool  firing      = false;    // only when partMask != 0
};

struct RawSound {
    int  type     = SOUND_OTHER;
    int  sourceId = -1;
    Vec3 origin;                  // true source; PerceptionModel adds the noise
};

struct RawDamage {
    int   attackerId = -1;
    Vec3  attackerPos;            // true; PerceptionModel keeps only a noisy direction
    float damage     = 0.0f;
};

struct RawInput {
    SelfState              self;
    std::vector<RawEnemy>  enemies;
    std::vector<RawSound>  sounds;
    std::vector<RawDamage> damage;
    std::vector<int>       deaths;    // players who died this tick (kill feed)
    int                    gotKillOf  = -1;
    bool                   headHit    = false;   // a bullet of ours hit an enemy's head this tick
    bool                   teammateInCrosshair = false;
};

//
// Noisy, human-like observations (PerceptionModel -> Brain)
//
struct EnemyObs {
    int   id          = -1;
    bool  detected    = false;   // perceived this tick
    int   visParts    = 0;
    int   partMask    = 0;
    Vec3  partPos[NUM_PARTS];
    Vec3  pos;                   // perceived centroid (valid while detected)
    Vec3  vel;
    float bodyHeight  = 94.0f;
    bool  centroidLos = false;
    bool  reloading   = false;
    bool  firing      = false;
    float detectP     = 0.0f;
    int   visibleMs   = 0;       // since a body part of it came on screen (while detected): the reaction clock
};

struct SoundObs {
    int   type      = SOUND_OTHER;
    int   sourceId  = -1;        // identity is only known for visible sources; -1 otherwise
    float yaw       = 0.0f;      // world yaw from the listener
    float yawSigma  = 20.0f;
    float dist      = 0.0f;      // noisy distance estimate
    float distLogSd = 0.35f;
    bool  frontBack = false;     // front/back ambiguous (mirror across the listener's lateral axis)
    float mirrorYaw = 0.0f;
};

struct DamageObs {
    float yaw      = 0.0f;       // world yaw toward the attacker (noisy)
    float yawSigma = 20.0f;
    float damage   = 0.0f;
    int   attackerId = -1;
};

struct Observation {
    SelfState              self;
    std::vector<EnemyObs>  enemies;
    std::vector<SoundObs>  sounds;
    std::vector<DamageObs> damage;
    std::vector<int>       deaths;
    int                    gotKillOf = -1;
    bool                   headHit   = false;
    bool                   teammateInCrosshair = false;
};

//
// Decisions for one 50 ms tick
//
struct TickPlan {
    int   owner       = OWNER_BRAIN;
    int   chord       = 4;
    bool  attack      = false;
    int   lean        = 0;       // -1 left, 0 none, 1 right
    bool  crouch      = false;
    bool  jump        = false;
    bool  walk        = false;
    bool  use         = false;
    float yawDelta    = 0.0f;    // view change over this tick, degrees
    float pitchDelta  = 0.0f;
    bool  viewStill   = false;   // mouse not moved at all this tick
    float flickFrac[8] = {};     // optional per-substep share of yawDelta during flicks (sums to 1)
    bool  flickShaped = false;
    int   command     = CMD_NONE;
    bool  navTargetValid = false;
    Vec3  navTarget;
    // the fire gate of g_humanbot_skill: a round with the crosshair off the body is skipped with
    // fireGate (0 = off); the target direction at the tick's start and its change over the tick
    float fireGate     = 0.0f;
    float gateYaw      = 0.0f;
    float gateYawRate  = 0.0f;
    float gatePitch    = 0.0f;
    float gateHalfW    = 0.0f;
    float gateHalfH    = 0.0f;
    bool  gateMayPress = false;   // the gate may also start a round with the crosshair on the body
};

// One usercmd of a tick (hb_substep).
struct SubCmd {
    int   serverTimeOffset = 0;  // ms relative to the tick's server time (<= 0, last = 0)
    int   chord   = 4;
    bool  attack  = false;
    int   lean    = 0;
    bool  crouch  = false;
    bool  jump    = false;
    bool  walk    = false;
    bool  use     = false;
    float yaw     = 0.0f;        // absolute view angles at this sub-step
    float pitch   = 0.0f;
};

} // namespace hb
