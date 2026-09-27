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
// hb_belief.h: where the bot thinks a hidden enemy is.
//
// A particle filter per tracked enemy (at most three), living on the human
// route graph of the map. Particles move with the recorded human move kernel
// (conditioned on where the bot is); cells the bot is looking at and would see
// lose weight; sightings collapse the cloud; sounds and damage reweight it; a
// kill re-seeds it on the spawn points the FFA spawn rule would pick.

#pragma once

#include "hb_map.h"
#include "hb_model.h"
#include "hb_rng.h"
#include "hb_types.h"

#include <vector>

namespace hb
{

constexpr int MAX_TRACKS   = 3;
constexpr int MAX_EXPOSURE = 4;

struct BeliefEstimate {
    bool  valid        = false;
    int   enemyId      = -1;
    bool  detected     = false;   // enemy perceived this tick
    bool  dead         = false;   // killed, not yet expected back
    bool  seenThisLife = false;
    int   msSinceSeen  = 1000000;
    int   msSinceHeard = 1000000;
    Vec3  mode;                   // most likely position (feet)
    Vec3  modeVel;                // expected velocity right after sight loss
    float spread       = 0.0f;
    float ess          = 0.0f;
    float visibleSoon  = 0.0f;    // mass expected in view within ~300 ms
    int   nExposure    = 0;
    Vec3  exposure[MAX_EXPOSURE]; // where the enemy would first become visible (feet)
    float exposureMass[MAX_EXPOSURE] = {};
    float exposureEtaMs[MAX_EXPOSURE] = {};
    Vec3  lastSeenPos;
    Vec3  lastSeenVel;
    int   lastThreatMs = -1000000;  // last time this enemy was seen, heard or hurt us
};

class BeliefFilter
{
public:
    void Init(const BeliefModel *params, const PerceptionModel *perception, const MapPrior *map, const Rng& rng);
    void SetMap(const MapPrior *map);
    void Reset();

    // Runs one tick. `enemyIds` is the list of enemies that exist (scoreboard).
    void Update(const Observation& obs, float hfovDeg, float vfovDeg);

    const BeliefEstimate& Track(int i) const { return m_tracks[i].est; }
    int                   NumTracks() const { return static_cast<int>(m_tracks.size()); }
    int                   FindTrack(int enemyId) const;

    // Track index of the focus enemy: the most recent threat (-1 when none).
    int Focus() const { return m_focus; }

private:
    struct Particle {
        int   cell = -1;
        Vec3  pos;
        float w    = 1.0f;
    };

    struct TrackState {
        BeliefEstimate        est;
        std::vector<Particle> parts;
        int                   deadUntilMs   = 0;
        int                   lostTick      = 0;     // ticks since the last sighting
        bool                  initialised   = false;
    };

    void InitTrack(TrackState& t, const Observation& obs);
    void SeedFromPrior(TrackState& t, const Observation& obs, bool avoidVisible);
    void SeedFromSpawns(TrackState& t, const Observation& obs);
    void Predict(TrackState& t, const Observation& obs);
    void NegativeInfo(TrackState& t, const Observation& obs, float hfovDeg, float vfovDeg);
    void ApplySound(TrackState& t, const SoundObs& s, const Observation& obs);
    void ApplyDamage(TrackState& t, const DamageObs& d, const Observation& obs);
    void Normalise(TrackState& t, const Observation& obs);
    void Summarise(TrackState& t, const Observation& obs);
    void RefreshPathTree(int botCell);
    float SoundLikelihood(const Particle& p, const SoundObs& s, const Observation& obs) const;

    const BeliefModel     *m_p   = nullptr;
    const PerceptionModel *m_perc = nullptr;
    const MapPrior        *m_map = nullptr;
    Rng                    m_rng;
    std::vector<TrackState> m_tracks;
    int                    m_focus = -1;

    int                m_treeCell = -2;
    std::vector<int>   m_next;
    std::vector<float> m_dist;
    std::vector<float> m_cellMass;
    std::vector<int>   m_touched;
    std::vector<int>   m_kTo;
    std::vector<float> m_kW;
    std::vector<Particle> m_scratch;
};

} // namespace hb
