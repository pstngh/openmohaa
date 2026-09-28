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
// pm_runner.h: runs the real Pmove() for a player the way the server does.
//
// PmRunUsercmd() is Player::ClientThink + ClientMove + SetMoveInfo/GetMoveInfo
// for a living deathmatch player (PM_NORMAL, user movecontrol, no ladder,
// turret or vehicle):
//
//   * ps.speed as in ClientMove: on the ground sv_runspeed with BUTTON_RUN,
//     sv_runspeed * sv_walkspeedmult without it, times sv_crouchspeedmult
//     while crouching, times the held weapon's movement speed; in the air
//     Player::airspeed (200); then times sv_dmspeedmult, truncated to int at
//     each step like the game (SMG: 240 run, 144 walk or crouch, 86 both);
//   * ps.gravity = sv_gravity, pm_type PM_NORMAL, pm_flags DUCKED/VIEW_PRONE/
//     VIEW_DUCK_RUN/VIEW_JUMP_START from the entity height and viewheight;
//   * pmove_t: MASK_PLAYERSOLID, the PmWorld callbacks, pmove_fixed and
//     pmove_msec (clamped to [8, 33]), the lean limits of the protocol
//     (8: leanMax 40, leanAdd 10, leanRecoverSpeed 15, leanSpeed 4,
//     alwaysAllowLean) and ps.groundEntityNum reset to ENTITYNUM_NONE;
//   * after Pmove: GetMoveInfo's ground-plane velocity clip, the entity bbox
//     and viewheight from pm->mins/maxs/viewheight, groundentity/airspeed, and
//     ClientMove's "blocked" rule (origin restored when a key is held, the
//     player walks and moved less than 0.005).
//
// PmRunThink() runs once per server frame after the frame's usercmds, like
// Player::Think evaluating the legs state machine. The state machine scripts
// are game data, so this is a model of what the movement telemetry shows:
//
//   * a crouch-key press (upmove < 0 now, not at the previous think) toggles
//     crouch: modheight duck (maxs[2] 54, viewheight 48) with crouch speed;
//     standing up needs CAN_STAND room; a jump-key press also stands up;
//   * a jump-key press on the ground enters jump start: viewheight 52 (the
//     usercmds get PMF_VIEW_JUMP_START) with crouch speed (the telemetry shows
//     the 0.6 speed factor in that frame); the next think adds
//     sqrt(2 * sv_gravity * 56) = 299.33 u/s upward (Player::Jump) and clears
//     ps.walking; the third think after that tucks the legs (modheight duck)
//     until the player lands;
//   * no jump from another player's head (Player::Jump in multiplayer).
//
// Not modelled: sprint (off in Allied Assault), ladders, water, knockback,
// the random push off another player's head, weapon zoom speed.
//
// PmRunServerFrame() is one 50 ms server frame: K usercmds with strictly
// increasing serverTimes ending at the frame time, then the think. A key
// change lands on a chosen sub-step (PmFrameInput::changeIndex).
//
// Pmove uses globals, so none of this is thread-safe.

#pragma once

#include "pm_world.h"

// Weapon::GetMovementSpeed() of the held weapon (TIKI "dmmovementspeed").
#define PM_SPEEDMULT_NONE   1.00f // nothing, pistol or grenade: 250 u/s
#define PM_SPEEDMULT_SMG    0.96f // MP40, Thompson: 240 u/s
#define PM_SPEEDMULT_RIFLE  0.94f // Kar98, M1 Garand: 235 u/s
#define PM_SPEEDMULT_SNIPER 0.80f // sniper rifles: 200 u/s

// Server settings (cvars) that shape movement. The defaults are the recorded
// practice server: sv_fps 20, runspeed 250, dmspeedmult 1, gravity 800,
// protocol 8, free-for-all.
struct PmSettings {
    float runSpeed        = 250.0f;       // sv_runspeed
    float walkSpeedMult   = 0.6f;         // sv_walkspeedmult (BUTTON_RUN released)
    float crouchSpeedMult = 0.6f;         // sv_crouchspeedmult
    float dmSpeedMult     = 1.0f;         // sv_dmspeedmult
    float gravity         = 800.0f;       // sv_gravity
    float airSpeed        = 200.0f;       // Player::airspeed: ps.speed while airborne
    float jumpHeight      = 56.0f;        // legs state machine "jump 56": 299.33 u/s at gravity 800
    int   pmoveFixed      = 0;            // pmove_fixed
    int   pmoveMsec       = 8;            // pmove_msec (the game clamps it to [8, 33])
    int   protocol        = PROTOCOL_MOH; // g_protocol (8 = Allied Assault)
    int   gametype        = GT_FFA;       // g_gametype
    int   frameMsec       = 50;           // server frame length (sv_fps 20)
    int   tuckThinks      = 3;            // thinks after the take-off think before the mid-air duck
};

// Posture of the legs state machine model (PmRunThink).
enum PmStance {
    PM_STANCE_STAND,      // maxs[2] 94, viewheight 82
    PM_STANCE_CROUCH,     // modheight duck: maxs[2] 54, viewheight 48, crouch speed
    PM_STANCE_JUMP_START, // modheight jumpstart: viewheight 52, crouch speed; jumps at the next think
    PM_STANCE_AIRBORNE,   // jumped, still at the jump-start height
    PM_STANCE_TUCKED      // mid-air duck: maxs[2] 54, viewheight 48, until landing
};

// A player as the server keeps it: the playerState plus the entity fields
// that feed Pmove.
struct PmPlayer {
    playerState_t ps;   // client->ps: origin, velocity, commandTime, viewangles, pm_flags ...
    vec3_t        mins; // entity bbox (setSize from pm->mins/maxs, or modheight)
    vec3_t        maxs;
    int           viewheight; // entity viewheight (from pm->ps->viewheight, or modheight)
    int           entityNum;  // entity and client number (== ps.clientNum)

    float     weaponSpeedMult; // Weapon::GetMovementSpeed() of the held weapon (PM_SPEEDMULT_*)
    bool      onGround;        // Player::groundentity != NULL after the last Pmove
    float     airSpeed;        // Player::airspeed
    bool      crouching;       // m_iMovePosFlags & MPF_POSITION_CROUCHING
    int       stance;          // PmStance
    int       stanceThinks;    // thinks since the stance was entered
    int       moveresult;      // Player::moveresult after the last usercmd (MOVERESULT_*)
    usercmd_t lastCmd;         // last accepted usercmd (Player::last_ucmd)
    int       thinkUpmove;     // lastCmd.upmove at the previous think (key presses are edges)
    int       numUsercmds;     // usercmds accepted so far
};

// Resets `player` to a living standing player (health 100) at origin, facing
// yaw degrees, with ps.commandTime = time. Does not touch the world.
void PmInitPlayer(
    PmPlayer& player, int entityNum, const vec3_t origin, float yaw, int time, float weaponSpeedMult = PM_SPEEDMULT_SMG
);

// Moves the player straight down onto the floor below (at most maxDrop units)
// and runs the ground check (Player::CheckGround), like a spawn settling.
// Returns false when it starts in solid or finds no floor.
bool PmDropToFloor(PmPlayer& player, const PmWorld& world, const PmSettings& settings, float maxDrop = 256.0f);

// Links the player's current box into the world as its body, so other players
// collide with it. PmRunUsercmd keeps an existing body in sync.
void PmLinkPlayer(PmWorld& world, const PmPlayer& player);

// A usercmd with view angles in degrees and digital keys (-127, 0, 127).
// serverTime is left 0; PmRunServerFrame fills it.
usercmd_t PmMakeUsercmd(float pitch, float yaw, int forward, int right, int up, int buttons);

// serverTime of sub-step index (0 .. numCmds - 1) of the frame ending at
// frameTime: frameTime - frameMsec + (index + 1) * frameMsec / numCmds.
int PmSubstepTime(int frameTime, int frameMsec, int numCmds, int index);

// Runs one usercmd like Player::ClientThink/ClientMove. levelTime is the
// server time (level.svsTime) when the command is executed: later commands are
// dropped, older ones clamped to levelTime - 1000, and a command that does not
// advance ps.commandTime is ignored. Returns true when Pmove ran. A body the
// world holds for the player is moved with it.
bool PmRunUsercmd(PmPlayer& player, PmWorld& world, const PmSettings& settings, const usercmd_t& cmd, int levelTime);

// The per-frame think (legs state machine model): crouch, stand, jump.
void PmRunThink(PmPlayer& player, PmWorld& world, const PmSettings& settings);

// Input of one server frame.
struct PmFrameInput {
    usercmd_t before;      // angles, keys and buttons of sub-steps [0, changeIndex)
    usercmd_t after;       // ... of sub-steps [changeIndex, numCmds)
    int       numCmds;     // K usercmds this frame, 1 .. frameMsec
    int       changeIndex; // first sub-step using `after`: 0 = the whole frame, numCmds = none
};

// Runs the server frame ending at frameTime: input.numCmds usercmds at
// PmSubstepTime(frameTime, settings.frameMsec, numCmds, i), then PmRunThink.
// Returns the number of usercmds that ran.
int PmRunServerFrame(
    PmPlayer& player, PmWorld& world, const PmSettings& settings, int frameTime, const PmFrameInput& input
);

// Same keys for the whole frame.
int PmRunServerFrame(
    PmPlayer& player, PmWorld& world, const PmSettings& settings, int frameTime, const usercmd_t& cmd, int numCmds
);

// Telemetry-style kinematics: horizontal speed, and the horizontal velocity
// along the view's forward and right (vel_fwd / vel_right of the analysis).
float PmSpeedXY(const PmPlayer& player);
float PmVelForward(const PmPlayer& player);
float PmVelRight(const PmPlayer& player);
