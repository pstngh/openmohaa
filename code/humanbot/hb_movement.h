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
// The 9 digital (forward, right) chords are a semi-Markov chain: the chance of
// letting go of the current chord depends on the context, the chord and how
// long it has been held (a refractory ramp, then a flat hazard), plus nearby
// walls, a fresh LOS change and how badly the chord disagrees with where the
// bot wants to go. The next chord follows the recorded transitions, tilted
// toward or away from the enemy by distance and toward the travel direction.
// Chords are relative to the view, so looking and moving stay independent.

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
    float clearance[9]   = {128, 128, 128, 128, 128, 128, 128, 128, 128};
    float drop[9]        = {};
    float wallPressMs    = 0.0f;
    bool  ducked         = false;
    bool  onGround       = true;
    bool  allowJump      = true;
};

struct MoveOutput {
    int   chord    = CHORD_NEUTRAL;
    int   lean     = 0;
    bool  crouch   = false;
    bool  jump     = false;
    bool  walk     = false;
    float pSwitch  = 0.0f;
    int   vetoMask = 0;
};

class Mover
{
public:
    void Init(const MovementModel *params);
    void Reset();

    void Step(const MoveInput& in, const StyleOffsets& style, Rng& moveRng, Rng& stanceRng, MoveOutput& out);

    int Chord() const { return m_chord; }
    int ChordAgeTicks() const { return m_age; }
    int Lean() const { return m_lean; }
    int LeanAgeTicks() const { return m_leanAge; }

    // Chord direction in the view frame (degrees, + = left); 0 for neutral.
    static float ChordAngle(int chord);

    // Probability of leaving the current chord this tick (exposed for tests and replay).
    float SwitchProb(const MoveInput& in, const StyleOffsets& style, int chord, int age, int vetoMask) const;
    void  NextChordWeights(const MoveInput& in, const StyleOffsets& style, int chord, int vetoMask, double w[NUM_CHORDS]) const;
    int   VetoMask(const MoveInput& in) const;

private:
    void StepLean(const MoveInput& in, const StyleOffsets& style, Rng& rng);
    void StepStance(const MoveInput& in, const StyleOffsets& style, Rng& rng, MoveOutput& out);

    const MovementModel *m_p = nullptr;
    int                  m_chord   = CHORD_NEUTRAL;
    int                  m_age     = 1;
    int                  m_lean    = 0;
    int                  m_leanAge = 1;
    int                  m_crouchLeft = 0;
    int                  m_jumpLeft   = 0;
    int                  m_walkLeft   = 0;
};

} // namespace hb
