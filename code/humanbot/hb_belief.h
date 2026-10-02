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
//
// Exposures are where a particle's way toward the bot first enters the bot's
// view (the cell visibility table). With the map's geometry (WorldQuery) each
// one gets its corner: the edge of cover the enemy would come out from, found
// like the data repo's corner check, by tracing the believed path at head
// height from the bot's eye to where it crosses out of cover, then the line to
// its last hidden point to where it first meets the map.

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
constexpr int PATH_BACK    = 3;   // cells of the believed path kept before an exposure
constexpr int MAX_HEARD_SOUNDS   = 3;   // sounds followed per frame (the nearest)

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
    int   exposureCell[MAX_EXPOSURE] = {};
    bool  cornerValid[MAX_EXPOSURE] = {};
    Vec3  corner[MAX_EXPOSURE];          // the edge of cover it would come out from, on the line to its head
    float cornerOpen[MAX_EXPOSURE] = {}; // +1: it comes out on the corner's left as the bot sees it, -1: right
    Vec3  lastSeenPos;
    Vec3  lastSeenVel;
    int   lastThreatMs = -1000000;  // last time this enemy was seen, heard or hurt us
};

class BeliefFilter
{
public:
    void Init(const BeliefModel *params, const PerceptionModel *perception, const MapPrior *map, const Rng& rng);
    void SetMap(const MapPrior *map);
    void SetWorld(const WorldQuery *world) { m_world = world; }
    void Reset();

    // Runs one tick. `enemyIds` is the list of enemies that exist (scoreboard).
    void Update(const Observation& obs, float hfovDeg, float vfovDeg);

    const BeliefEstimate& Track(int i) const { return m_tracks[i].est; }
    int                   NumTracks() const { return static_cast<int>(m_tracks.size()); }
    int                   FindTrack(int enemyId) const;

    // Track index of the focus enemy: the most recent threat (-1 when none).
    int Focus() const { return m_focus; }

    // The corner of a believed path seen from eye: path holds map cells from the enemy's side toward the bot
    // (-1 = none), the exposure at index PATH_BACK; point is where the line to the path's last hidden point (at
    // head height) first meets the map, open the side it comes out on (+1 left). Public for the unit tests.
    bool FindCorner(const Vec3& eye, const int *path, int n, Vec3& point, float& open) const;

private:
    struct Particle {
        int   cell = -1;
        int   prev = -1;     // previous cell: people keep moving the way they were going
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
    // Re-draws part of the cloud along a bearing when the cloud cannot explain an observation.
    void Inject(TrackState& t, float yaw, float yawSigma, float mirrorYaw, float mirrorP, float dist, float distLogSd,
                float share, bool needVisible, const Observation& obs);
    void Normalise(TrackState& t, const Observation& obs);
    void Summarise(TrackState& t, const Observation& obs, bool exposures);
    int  ParticlesPerTrack(const Observation& obs) const;
    void RefreshPathTree(int botCell);
    float SoundLikelihood(const Particle& p, const SoundObs& s, const Observation& obs) const;
    void Corners(BeliefEstimate& e, const Observation& obs, const int (*paths)[PATH_BACK + 1]);

    struct CornerCache {
        int   cell   = -1;   // the exposure cell
        Vec3  eye;
        bool  valid  = false;
        Vec3  point;
        float open   = 0.0f;
        int   usedMs = 0;
    };

    const BeliefModel     *m_p   = nullptr;
    const PerceptionModel *m_perc = nullptr;
    const MapPrior        *m_map = nullptr;
    const WorldQuery      *m_world = nullptr;
    std::vector<CornerCache> m_corners;
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
    std::vector<double> m_kWd;     // Predict's move weights (reused: no allocation per particle)
    std::vector<int>    m_order;   // Inject's particle order
    std::vector<Particle> m_scratch;
};

} // namespace hb
