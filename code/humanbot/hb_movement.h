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
// hb_movement.h: movement keys, lean and stance as human-like hazard processes.
//
// The side key (left/none/right) and the forward key (back/none/forward) are
// two coupled semi-Markov processes, like the two fingers pressing them. Each
// has its own hold age: the chance of changing it follows a refractory ramp
// and then a flat hazard, shifted by walls in its direction, a fresh LOS
// change, a context change (a fight starting or ending) and how badly the
// resulting chord disagrees with where the bot wants to go. A strafe that ends
// either reverses or lets go; the forward key leans toward or away from the
// enemy by distance. The 9 chords are relative to the view, so looking and
// moving stay independent.

#pragma once

#include "hb_model.h"
#include "hb_rng.h"
#include "hb_style.h"

namespace hb
{

struct MoveInput {
    int   ctx            = CTX_HIDDEN_NOFIRE;
    bool  losChanged     = false;   // perceived LOS changed this tick or the last
    bool  enemyKnown     = false;   // bearing/distance below are meaningful
    float enemyBearing   = 0.0f;    // relative to the view, degrees, + = left
    float enemyDist      = 1000.0f;
    bool  enemyReloading = false;
    bool  navValid       = false;
    float navBearing     = 0.0f;    // desired travel direction relative to the view, + = left
    float urgency        = 0.0f;    // 0 = no preference, 1 = must travel
    bool  travelling     = false;   // going somewhere (hunting, out of the spawn), not after an enemy just lost
    bool  travelView     = false;   // the view leads along the way (its travel mode): the pull x travelPull
    bool  nearView       = false;   // the view watches the side of an enemy believed near: the pull x nearPull
    float clearance[9]   = {128, 128, 128, 128, 128, 128, 128, 128, 128};
    float drop[9]        = {};      // depth of a ledge in each chord direction (0 = none)
    float wallPressMs    = 0.0f;
    float velFwd         = 0.0f;    // own velocity in the view frame, units/s
    float velRight       = 0.0f;
    bool  ducked         = false;
    bool  enemyDead      = false;   // no living enemy known (after a kill): lean context LEAN_CTX_DEAD
    bool  fireHeard      = false;   // another player's gunfire heard in the last 500 ms (people crouch then)
    bool  onGround       = true;
    bool  allowJump      = true;
};

struct MoveOutput {
    int   chord    = CHORD_NEUTRAL;
    int   lean     = 0;
    bool  crouch   = false;
    bool  jump     = false;
    bool  walk     = false;
    float pSwitch  = 0.0f;   // chance that some movement key changed this tick
    int   vetoMask = 0;
};

class Mover
{
public:
    void Init(const MovementModel *params, const SpawnModel *spawn = nullptr);
    void Reset();

    // The first live tick of a life: the keys already held (drawn from the spawn model by the
    // caller). The first run of each key then follows the spawn hazards until it changes.
    void Spawn(int chord, MoveOutput& out);
    void Step(const MoveInput& in, const StyleOffsets& style, Rng& moveRng, Rng& stanceRng, MoveOutput& out);
    // Starts from given key states with fresh ages (replays start from what the human held).
    void SetKeys(int fwd, int side);

    int Chord() const { return MakeChord(m_fwd, m_side); }
    int ChordAgeTicks() const { return m_side != 0 ? m_sideAge : m_fwdAge; }
    int SideAgeTicks() const { return m_sideAge; }
    int Lean() const { return m_lean; }
    int LeanAgeTicks() const { return m_leanAge; }

    // Chord direction in the view frame (degrees, + = left); 0 for neutral.
    static float ChordAngle(int chord);
    // Chords a new key press may not go toward (walls closer than the veto clearance, ledges).
    int          VetoMask(const MoveInput& in) const;
    bool         WallAhead(const MoveInput& in, int chord) const;

private:
    float WallMargin(const MoveInput& in, int chord) const;
    void  Reflex(const MoveInput& in, int veto);
    float NavAlign(const MoveInput& in, int chord) const;
    float NavGain(const MoveInput& in, bool sideKey, int veto, int side) const;
    float CtxChange(const KeyModel& k, int row, int ctx) const;
    float HidDist(const KeyModel& k, int row, int fwd, const MoveInput& in) const;
    float NavPull(const MoveInput& in) const;
    int   LedgeMask(const MoveInput& in) const;
    void  StepSide(const MoveInput& in, const StyleOffsets& style, int ledge, int veto, double u1, double u2, int& side, float& p);
    void  StepFwd(const MoveInput& in, const StyleOffsets& style, int ledge, int veto, int side, double u1, Rng& rng, int& fwd,
                  float& p);
    void  StepLean(const MoveInput& in, const StyleOffsets& style, int side, Rng& rng);
    // How strongly the walls on one side (-1 left, +1 right) draw a lean to it (logit; see MovementModel).
    float LeanWallLogit(const MoveInput& in, int side) const;
    void  StepStance(const MoveInput& in, const StyleOffsets& style, Rng& rng, MoveOutput& out);

    float SpawnLogit(const Table& t, int row, int age) const;

    const MovementModel *m_p     = nullptr;
    const SpawnModel    *m_spawn = nullptr;
    bool                 m_sideSpawn = false;  // the side key has not changed since the first live tick
    bool                 m_fwdSpawn  = false;
    int                  m_side    = 0;
    int                  m_sideAge = 1;
    int                  m_fwd     = 0;
    int                  m_fwdAge  = 1;
    int                  m_lean    = 0;
    int                  m_leanAge = 1;
    int                  m_ctx        = -1;
    int                  m_ctxAge     = 10000;
    int                  m_crouchAge  = 0;   // ticks held, 0 = released
    int                  m_crouchedTicks = 0;  // ticks crouched on the ground (crouch toggles)
    int                  m_jumpAge    = 0;
    int                  m_walkAge    = 0;
    // this tick's wall reflex (Reflex): let go of the side key, of the forward key, or slide along the
    // wall with a strafe toward this side (0 = none)
    bool                 m_reflexSide = false;
    bool                 m_reflexFwd  = false;
    int                  m_slideSide  = 0;
    int                  m_lastSide   = 0;      // the side of the last strafe (kept while no strafe key is held)
    bool                 m_slideFwd   = false;  // a strafe into a wall: add forward and run along it
};

} // namespace hb
