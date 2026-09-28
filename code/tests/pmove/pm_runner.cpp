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
// pm_runner.cpp: Player::ClientThink / ClientMove / SetMoveInfo / GetMoveInfo
// (fgame/player.cpp) around the real Pmove, plus a model of the legs state
// machine for crouching and jumping. See pm_runner.h.

#include "pm_runner.h"

#include <cmath>
#include <cstring>

namespace
{

// Player::ModifyHeight
void SetHeight(PmPlayer& player, float maxsZ, int viewheight)
{
    player.maxs[2]    = maxsZ;
    player.viewheight = viewheight;
}

void EnterStance(PmPlayer& player, const PmSettings& settings, int stance)
{
    switch (stance) {
    case PM_STANCE_STAND:
        SetHeight(player, MAXS_Z, DEFAULT_VIEWHEIGHT);
        player.crouching = false;
        break;
    case PM_STANCE_CROUCH:
    case PM_STANCE_TUCKED:
        SetHeight(player, CROUCH_MAXS_Z, CROUCH_VIEWHEIGHT);
        player.crouching = true;
        break;
    case PM_STANCE_JUMP_START:
        // modheight jumpstart only lowers the view before protocol 15
        if (settings.protocol < PROTOCOL_MOHTA_MIN) {
            player.viewheight = JUMP_START_VIEWHEIGHT;
        }
        player.maxs[2]   = MAXS_Z;
        player.crouching = true;
        break;
    case PM_STANCE_AIRBORNE:
        // keeps the jump-start height until the tuck
        player.crouching = false;
        break;
    }

    player.stance       = stance;
    player.stanceThinks = 0;
}

// Player::CondCanStand: room for the standing box at the current origin.
bool CanStand(const PmPlayer& player, const PmWorld& world)
{
    vec3_t  mins, maxs;
    trace_t trace;

    VectorCopy(player.mins, mins);
    VectorCopy(player.maxs, maxs);
    mins[2] = MINS_Z;
    maxs[2] = MAXS_Z;

    world.Trace(&trace, player.ps.origin, mins, maxs, player.ps.origin, player.entityNum, MASK_PLAYERSOLID, true);
    return !trace.startsolid;
}

// Player::Jump
void Jump(PmPlayer& player, const PmSettings& settings)
{
    const int gravity = (int)settings.gravity; // sv_gravity->integer

    if (settings.jumpHeight > 16) {
        // v^2 = 2ad
        player.ps.velocity[2] += sqrtf(2 * gravity * settings.jumpHeight);

        // make sure the player leaves the ground
        player.ps.walking = qfalse;
    }
}

// The move flags part of Player::ClientMove: pm_type and the height flags
// that PM_CheckDuck turns back into mins/maxs/viewheight.
void SetMoveFlags(PmPlayer& player, const PmSettings& settings)
{
    playerState_t *ps = &player.ps;

    ps->pm_type = PM_NORMAL;
    ps->pm_flags &=
        ~(PMF_FROZEN | PMF_NO_PREDICTION | PMF_NO_MOVE | PMF_DUCKED | PMF_TURRET | PMF_VIEW_PRONE | PMF_VIEW_DUCK_RUN
          | PMF_VIEW_JUMP_START);

    if (settings.protocol >= PROTOCOL_MOHTA_MIN) {
        // only crouch or jump start is possible
        if (player.maxs[2] == CROUCH_MAXS_Z) {
            ps->pm_flags |= PMF_DUCKED;
        } else if (player.viewheight == (int)JUMP_START_VIEWHEIGHT) {
            ps->pm_flags |= PMF_VIEW_JUMP_START;
        }
    } else {
        if (player.maxs[2] == CROUCH_RUN_MAXS_Z) {
            ps->pm_flags |= PMF_DUCKED;
        } else if (player.maxs[2] == CROUCH_MAXS_Z) {
            ps->pm_flags |= PMF_DUCKED | PMF_VIEW_PRONE;
        } else if (player.maxs[2] == PRONE_MAXS_Z) {
            ps->pm_flags |= PMF_VIEW_PRONE;
        } else if (player.maxs[2] == (CROUCH_MAXS_Z - 1)) {
            ps->pm_flags |= PMF_VIEW_DUCK_RUN;
        } else if (player.viewheight == (int)JUMP_START_VIEWHEIGHT) {
            ps->pm_flags |= PMF_VIEW_JUMP_START;
        }
    }
}

// The speed part of Player::ClientMove; `cmd` is last_ucmd.
int ComputeSpeed(const PmPlayer& player, const PmSettings& settings, const usercmd_t& cmd)
{
    int speed;

    if (!player.onGround) {
        speed = (int)player.airSpeed;
    } else {
        if (cmd.buttons & BUTTON_RUN) {
            // Player::GetRunSpeed(): sprint is off in Allied Assault
            speed = (int)settings.runSpeed;
        } else {
            speed = (int)(settings.runSpeed * settings.walkSpeedMult);
        }

        if (player.crouching) {
            speed = (int)((float)speed * settings.crouchSpeedMult);
        }

        // also use the weapon movement speed
        speed = (int)((float)speed * player.weaponSpeedMult);
    }

    if (settings.gametype != GT_SINGLE_PLAYER) {
        speed = (int)((float)speed * settings.dmSpeedMult);
    }

    return speed;
}

// Player::SetMoveInfo (the entity origin/velocity are ps.origin/ps.velocity here).
void SetMoveInfo(pmove_t& pm, PmPlayer& player, const PmSettings& settings, const usercmd_t *ucmd)
{
    memset(&pm, 0, sizeof(pm));

    pm.ps = &player.ps;
    if (ucmd) {
        pm.cmd = *ucmd;
    }

    pm.trace         = PmWorld::PmoveTrace;
    pm.tracemask     = MASK_PLAYERSOLID;
    pm.pointcontents = PmWorld::PmovePointContents;

    pm.pmove_fixed = settings.pmoveFixed;
    pm.pmove_msec  = settings.pmoveMsec;
    if (settings.pmoveMsec < 8) {
        pm.pmove_msec = 8;
    } else if (settings.pmoveMsec > 33) {
        pm.pmove_msec = 33;
    }

    if (settings.protocol >= PROTOCOL_MOHTA_MIN) {
        // lean while moving only with DF_ALLOW_LEAN_MOVEMENT, which is off by default
        pm.alwaysAllowLean  = qfalse;
        pm.leanMax          = 45.f;
        pm.leanAdd          = 6.f;
        pm.leanRecoverSpeed = 8.5f;
        pm.leanSpeed        = 2.f;
    } else {
        pm.alwaysAllowLean = qtrue;
        if (settings.gametype != GT_SINGLE_PLAYER) {
            pm.leanMax = 40.f;
        } else {
            pm.leanMax = 0;
        }
        pm.leanAdd          = 10.f;
        pm.leanRecoverSpeed = 15.f;
        pm.leanSpeed        = 4.f;
    }

    pm.protocol = settings.protocol;

    pm.ps->groundEntityNum = ENTITYNUM_NONE;
}

// Player::GetMoveInfo
void GetMoveInfo(const pmove_t& pm, PmPlayer& player, const PmSettings& settings)
{
    playerState_t *ps = &player.ps;
    int            i;

    player.moveresult = pm.moveresult;

    if (ps->groundEntityNum != ENTITYNUM_NONE) {
        float backoff;
        float change;

        backoff = DotProduct(ps->groundTrace.plane.normal, ps->velocity);

        for (i = 0; i < 3; i++) {
            change = ps->groundTrace.plane.normal[i] * backoff;
            ps->velocity[i] -= change;
        }
    }

    player.onGround = ps->groundEntityNum != ENTITYNUM_NONE;
    if (player.onGround) {
        player.airSpeed = settings.airSpeed;
    }

    if ((ps->pm_flags & PMF_FROZEN) || (ps->pm_flags & PMF_NO_MOVE)) {
        VectorClear(ps->velocity);
    } else {
        VectorCopy(pm.mins, player.mins);
        VectorCopy(pm.maxs, player.maxs);
        player.viewheight = ps->viewheight;
    }
}

} // namespace

void PmInitPlayer(PmPlayer& player, int entityNum, const vec3_t origin, float yaw, int time, float weaponSpeedMult)
{
    memset(&player, 0, sizeof(player));

    player.entityNum = entityNum;

    player.ps.clientNum             = entityNum;
    player.ps.commandTime           = time;
    player.ps.pm_type               = PM_NORMAL;
    player.ps.stats[STAT_HEALTH]    = 100;
    player.ps.stats[STAT_MAXHEALTH] = 100;
    player.ps.groundEntityNum       = ENTITYNUM_NONE;
    player.ps.viewheight            = DEFAULT_VIEWHEIGHT;
    player.ps.viewangles[YAW]       = yaw;
    VectorCopy(origin, player.ps.origin);

    VectorSet(player.mins, MINS_X, MINS_Y, MINS_Z);
    VectorSet(player.maxs, MAXS_X, MAXS_Y, MAXS_Z);
    player.viewheight = DEFAULT_VIEWHEIGHT;

    player.weaponSpeedMult = weaponSpeedMult;
    player.airSpeed        = 200.0f; // Player::Init
    player.stance          = PM_STANCE_STAND;
    player.moveresult      = MOVERESULT_NONE;
}

bool PmDropToFloor(PmPlayer& player, const PmWorld& world, const PmSettings& settings, float maxDrop)
{
    trace_t trace;
    vec3_t  end;
    pmove_t pm;

    VectorCopy(player.ps.origin, end);
    end[2] -= maxDrop;

    world.Trace(&trace, player.ps.origin, player.mins, player.maxs, end, player.entityNum, MASK_PLAYERSOLID, true);
    if (trace.allsolid || trace.startsolid || trace.fraction == 1.0f) {
        return false;
    }

    VectorCopy(trace.endpos, player.ps.origin);
    VectorClear(player.ps.velocity);

    // Player::CheckGround
    SetMoveFlags(player, settings);
    SetMoveInfo(pm, player, settings, &player.lastCmd);
    {
        PmWorldScope scope(world);
        Pmove_GroundTrace(&pm);
    }
    GetMoveInfo(pm, player, settings);

    return player.onGround;
}

void PmLinkPlayer(PmWorld& world, const PmPlayer& player)
{
    world.SetBody(player.entityNum, player.ps.origin, player.mins, player.maxs);
}

usercmd_t PmMakeUsercmd(float pitch, float yaw, int forward, int right, int up, int buttons)
{
    usercmd_t cmd;

    memset(&cmd, 0, sizeof(cmd));
    cmd.angles[PITCH] = ANGLE2SHORT(pitch);
    cmd.angles[YAW]   = ANGLE2SHORT(yaw);
    cmd.forwardmove   = (signed char)forward;
    cmd.rightmove     = (signed char)right;
    cmd.upmove        = (signed char)up;
    cmd.buttons       = (unsigned short)buttons;
    return cmd;
}

int PmSubstepTime(int frameTime, int frameMsec, int numCmds, int index)
{
    return frameTime - frameMsec + ((index + 1) * frameMsec) / numCmds;
}

bool PmRunUsercmd(PmPlayer& player, PmWorld& world, const PmSettings& settings, const usercmd_t& ucmd, int levelTime)
{
    playerState_t *ps  = &player.ps;
    usercmd_t      cmd = ucmd;
    pmove_t        pm;
    vec3_t         oldpos;
    vec3_t         delta;

    //
    // Player::ClientThink
    //

    // sanity check the command time to prevent speedup cheating
    if (cmd.serverTime > levelTime) {
        return false; // commands from the future are ignored
    }
    if (cmd.serverTime < levelTime - 1000) {
        cmd.serverTime = levelTime - 1000;
    }
    if (cmd.serverTime - ps->commandTime < 1) {
        return false;
    }

    player.lastCmd    = cmd;
    player.moveresult = MOVERESULT_NONE;

    //
    // Player::ClientMove
    //
    SetMoveFlags(player, settings);
    ps->speed   = ComputeSpeed(player, settings, cmd);
    ps->gravity = (int)(settings.gravity * 1.0f); // sv_gravity * entity gravity

    VectorCopy(ps->origin, oldpos);

    SetMoveInfo(pm, player, settings, &cmd);
    {
        PmWorldScope scope(world);
        Pmove(&pm);
    }
    GetMoveInfo(pm, player, settings);

    // if we're not moving, set the blocked flag in case the user is trying to move
    VectorSubtract(oldpos, ps->origin, delta);
    if ((cmd.forwardmove || cmd.rightmove) && VectorLength(delta) < 0.005f) {
        player.moveresult = MOVERESULT_BLOCKED;
    }
    if (ps->walking && player.moveresult >= MOVERESULT_BLOCKED) {
        VectorCopy(oldpos, ps->origin);
    }

    // setOrigin/setSize relink the entity
    if (world.HasBody(player.entityNum)) {
        PmLinkPlayer(world, player);
    }

    player.numUsercmds++;
    return true;
}

void PmRunThink(PmPlayer& player, PmWorld& world, const PmSettings& settings)
{
    const int  upmove      = player.lastCmd.upmove;
    const bool crouchPress = upmove < 0 && player.thinkUpmove >= 0;
    const bool jumpPress   = upmove > 0 && player.thinkUpmove <= 0;
    const bool onBody = player.ps.groundEntityNum != ENTITYNUM_NONE && player.ps.groundEntityNum != ENTITYNUM_WORLD;

    player.thinkUpmove = upmove;
    player.stanceThinks++;

    switch (player.stance) {
    case PM_STANCE_STAND:
        if (player.onGround) {
            if (crouchPress) {
                EnterStance(player, settings, PM_STANCE_CROUCH);
            } else if (jumpPress) {
                EnterStance(player, settings, PM_STANCE_JUMP_START);
            }
        }
        break;

    case PM_STANCE_CROUCH:
        if ((crouchPress || jumpPress) && CanStand(player, world)) {
            EnterStance(player, settings, PM_STANCE_STAND);
        }
        break;

    case PM_STANCE_JUMP_START:
        if (!player.onGround) {
            // left the ground before the take-off
            EnterStance(player, settings, PM_STANCE_AIRBORNE);
        } else if (onBody && settings.gametype != GT_SINGLE_PLAYER) {
            // no jumping off another player's head
            EnterStance(player, settings, PM_STANCE_STAND);
        } else {
            Jump(player, settings);
            EnterStance(player, settings, PM_STANCE_AIRBORNE);
        }
        break;

    case PM_STANCE_AIRBORNE:
    case PM_STANCE_TUCKED:
        if (player.onGround) {
            // landed
            if (CanStand(player, world)) {
                EnterStance(player, settings, PM_STANCE_STAND);
            } else {
                EnterStance(player, settings, PM_STANCE_CROUCH);
            }
        } else if (player.stance == PM_STANCE_AIRBORNE && player.stanceThinks >= settings.tuckThinks) {
            EnterStance(player, settings, PM_STANCE_TUCKED);
        }
        break;
    }

    // the entity moved or changed size: relink it
    if (world.HasBody(player.entityNum)) {
        PmLinkPlayer(world, player);
    }
}

int PmRunServerFrame(
    PmPlayer& player, PmWorld& world, const PmSettings& settings, int frameTime, const PmFrameInput& input
)
{
    int previous = frameTime - settings.frameMsec;
    int ran      = 0;
    int i;

    if (input.numCmds < 1 || input.numCmds > settings.frameMsec) {
        Com_Error(ERR_DROP, "PmRunServerFrame: bad usercmd count %d", input.numCmds);
    }

    for (i = 0; i < input.numCmds; i++) {
        usercmd_t cmd = (i < input.changeIndex) ? input.before : input.after;

        cmd.serverTime = PmSubstepTime(frameTime, settings.frameMsec, input.numCmds, i);
        cmd.msec       = (byte)(cmd.serverTime - previous);
        previous       = cmd.serverTime;

        if (PmRunUsercmd(player, world, settings, cmd, frameTime)) {
            ran++;
        }
    }

    PmRunThink(player, world, settings);
    return ran;
}

int PmRunServerFrame(
    PmPlayer& player, PmWorld& world, const PmSettings& settings, int frameTime, const usercmd_t& cmd, int numCmds
)
{
    PmFrameInput input;

    input.before      = cmd;
    input.after       = cmd;
    input.numCmds     = numCmds;
    input.changeIndex = 0;
    return PmRunServerFrame(player, world, settings, frameTime, input);
}

float PmSpeedXY(const PmPlayer& player)
{
    return sqrtf(player.ps.velocity[0] * player.ps.velocity[0] + player.ps.velocity[1] * player.ps.velocity[1]);
}

float PmVelForward(const PmPlayer& player)
{
    const double yaw = DEG2RAD(player.ps.viewangles[YAW]);

    return (float)(player.ps.velocity[0] * cos(yaw) + player.ps.velocity[1] * sin(yaw));
}

float PmVelRight(const PmPlayer& player)
{
    const double yaw = DEG2RAD(player.ps.viewangles[YAW]);

    return (float)(player.ps.velocity[0] * sin(yaw) - player.ps.velocity[1] * cos(yaw));
}
