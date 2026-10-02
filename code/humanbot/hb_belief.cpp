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
// hb_belief.cpp: particle filter over the route graph.

#include "hb_belief.h"

#include <algorithm>
#include <cmath>

namespace hb
{

static constexpr int   RESPAWN_MIN_MS    = 1600;   // earliest recorded respawn after a death
static constexpr int   DEAD_RECKON_TICKS = 8;      // follow the last seen velocity this long
static constexpr float CHEST_HEIGHT      = 56.0f;
// Corners are traced at the height of a standing enemy's head (0.9 of 94 u): people's first visible part is
// the head in 87% of sightings. A corner is reused while the eye stays within CORNER_REUSE_DIST of where it
// was traced; at most CORNERS_PER_TICK new ones are traced per tick (about 20 sight traces each).
static constexpr float CORNER_HEAD_Z     = 84.0f;
static constexpr float CORNER_REUSE_DIST = 24.0f;
static constexpr int   CORNERS_PER_TICK  = 2;
static constexpr int   CORNER_CACHE      = 8;
static constexpr int   PATH_FWD          = 2;      // cells past the exposure tried when it is hidden from the true eye
static constexpr float CORNER_MIN_DIST   = 96.0f;  // nearer corners are not watched (their direction swings)

void BeliefFilter::Init(const BeliefModel *params, const PerceptionModel *perception, const MapPrior *map, const Rng& rng)
{
    m_p    = params;
    m_perc = perception;
    m_map  = map;
    m_rng  = rng;
    Reset();
}

void BeliefFilter::SetMap(const MapPrior *map)
{
    m_map      = map;
    m_treeCell = -2;
    m_corners.clear();
    for (TrackState& t : m_tracks) {
        t.initialised = false;
    }
}

void BeliefFilter::Reset()
{
    m_tracks.clear();
    m_focus    = -1;
    m_treeCell = -2;
    m_corners.clear();
}

bool BeliefFilter::FindCorner(const Vec3& eye, const int *path, int n, Vec3& point, float& open) const
{
    // head points along the path, from the enemy's side toward the bot
    Vec3 pts[PATH_BACK + 1 + PATH_FWD];
    int  m = 0, j = -1;
    for (int k = 0; k < n; k++) {
        if (path[k] >= 0) {
            if (k == PATH_BACK) {
                j = m;   // the exposure cell
            }
            pts[m++] = m_map->cells[path[k]].center + Vec3(0.0f, 0.0f, CORNER_HEAD_Z);
        }
    }
    if (m < 2 || j < 0) {
        return false;
    }
    // the step where the path crosses out of cover, as seen from this eye: start at the exposure cell and
    // walk back while it is visible, or on toward the bot while it is hidden
    const bool vj  = m_world->Clear(eye, pts[j]);
    int        hid = -1;
    if (vj) {
        for (int k = j - 1; k >= 0; k--) {
            if (!m_world->Clear(eye, pts[k])) {
                hid = k;
                break;
            }
        }
    } else {
        for (int k = j + 1; k < m; k++) {
            if (m_world->Clear(eye, pts[k])) {
                hid = k - 1;
                break;
            }
        }
    }
    if (hid < 0 || hid + 1 >= m) {
        return false;
    }
    const Vec3 a = pts[hid], b = pts[hid + 1];
    float      lo = 0.0f, hi = 1.0f;
    for (int it = 0; it < 7; it++) {
        const float mid = 0.5f * (lo + hi);
        if (m_world->Clear(eye, a + (b - a) * mid)) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    const Vec3 h = a + (b - a) * lo;   // the last hidden point of the head
    // where the line to it first meets the map: the edge of the cover
    float flo = 0.0f, fhi = 1.0f;
    for (int it = 0; it < 9; it++) {
        const float mid = 0.5f * (flo + fhi);
        if (m_world->Clear(eye, eye + (h - eye) * mid)) {
            flo = mid;
        } else {
            fhi = mid;
        }
    }
    point = eye + (h - eye) * fhi;
    const float side = Wrap180(YawOf(b - eye) - YawOf(point - eye));
    open             = side >= 0.0f ? 1.0f : -1.0f;
    // a corner right beside the bot (it walks along the wall it would watch) swings with every step: none
    return (point - eye).lengthXY() > CORNER_MIN_DIST;
}

void BeliefFilter::Corners(BeliefEstimate& e, const Observation& obs, const int (*paths)[PATH_BACK + 1])
{
    const int now    = obs.self.timeMs;
    int       traced = 0;
    for (int i = 0; i < e.nExposure; i++) {
        e.cornerValid[i] = false;
        // one corner per exposure cell from where the eye is (the heaviest path into it decides which, when traced)
        const int key = paths[i][PATH_BACK];
        CornerCache *hit = nullptr;
        for (CornerCache& c : m_corners) {
            if (c.cell == key && (c.eye - obs.self.eye).length() < CORNER_REUSE_DIST) {
                hit = &c;
                break;
            }
        }
        if (!hit) {
            if (traced >= CORNERS_PER_TICK) {
                continue;
            }
            traced++;
            // the path into the exposure, and on toward the bot when the exposure is hidden from the true eye
            int path[PATH_BACK + 1 + PATH_FWD];
            for (int k = 0; k <= PATH_BACK; k++) {
                path[k] = paths[i][k];
            }
            int c = key;
            for (int k = 0; k < PATH_FWD; k++) {
                c                     = c >= 0 ? m_next[c] : -1;
                path[PATH_BACK + 1 + k] = c;
            }
            CornerCache entry;
            entry.cell   = key;
            entry.eye    = obs.self.eye;
            entry.valid  = FindCorner(obs.self.eye, path, PATH_BACK + 1 + PATH_FWD, entry.point, entry.open);
            if (static_cast<int>(m_corners.size()) < CORNER_CACHE) {
                m_corners.push_back(entry);
                hit = &m_corners.back();
            } else {
                hit = &m_corners[0];
                for (CornerCache& cc : m_corners) {
                    if (cc.usedMs < hit->usedMs) {
                        hit = &cc;
                    }
                }
                *hit = entry;
            }
        }
        hit->usedMs      = now;
        e.cornerValid[i] = hit->valid;
        e.corner[i]      = hit->point;
        e.cornerOpen[i]  = hit->open;
    }
}

int BeliefFilter::FindTrack(int enemyId) const
{
    for (size_t i = 0; i < m_tracks.size(); i++) {
        if (m_tracks[i].est.enemyId == enemyId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void BeliefFilter::RefreshPathTree(int botCell)
{
    if (!m_map || botCell == m_treeCell) {
        return;
    }
    m_treeCell = botCell;
    m_map->PathTreeTo(botCell, m_next, m_dist);
}

// A duel's single track gets the full particle count; with several enemies listed each track gets
// half (three tracks cost 1.5 duel tracks, the 16-bot budget).
int BeliefFilter::ParticlesPerTrack(const Observation& obs) const
{
    return obs.enemies.size() > 1 ? std::max(64, m_p->particles / 2) : m_p->particles;
}

void BeliefFilter::SeedFromPrior(TrackState& t, const Observation& obs, bool avoidVisible)
{
    const int n = ParticlesPerTrack(obs);
    t.parts.assign(n, Particle());
    if (!m_map || m_map->NumCells() == 0) {
        // no map knowledge: a broad ring around the bot
        for (Particle& p : t.parts) {
            const float a = static_cast<float>(m_rng.Uniform(0.0, 360.0));
            const float r = static_cast<float>(m_rng.Uniform(256.0, 1024.0));
            p.pos         = obs.self.origin + YawDir(a) * r;
            p.cell        = -1;
            p.w           = 1.0f / n;
        }
        return;
    }
    const int botCell = m_map->CellAt(obs.self.origin);
    // cumulative weights, so each particle is one binary search (256 draws over thousands of cells)
    std::vector<double> cdf(m_map->NumCells());
    double              sum = 0.0;
    for (int c = 0; c < m_map->NumCells(); c++) {
        const MapCell& mc = m_map->cells[c];
        double         v  = mc.occTotal + 1.0;
        if (avoidVisible && botCell >= 0 && m_map->Visibility(botCell, c) > 0.5f) {
            v *= 0.05;
        }
        if ((mc.center - obs.self.origin).lengthXY() < m_p->spawnMinDist) {
            v *= 0.05;
        }
        sum += v;
        cdf[c] = sum;
    }
    const float half = 0.45f * m_map->cellSize;
    for (Particle& p : t.parts) {
        p.cell = m_rng.CategoricalCdf(cdf);
        p.pos  = m_map->cells[p.cell].center
              + Vec3(static_cast<float>(m_rng.Uniform(-half, half)), static_cast<float>(m_rng.Uniform(-half, half)), 0.0f);
        p.w = 1.0f / n;
    }
}

void BeliefFilter::SeedFromSpawns(TrackState& t, const Observation& obs)
{
    if (!m_map || m_map->spawns.empty()) {
        SeedFromPrior(t, obs, true);
        return;
    }
    // FFA spawn rule (dm_manager.cpp): metric = min squared distance to living
    // players - (1..1.25) * 1024^2; the best <= 5 spots are drawn by rank.
    struct Spot {
        int   idx;
        float metric;
    };
    std::vector<Spot> spots;
    for (size_t i = 0; i < m_map->spawns.size(); i++) {
        const float d2 = obs.self.alive ? (m_map->spawns[i].pos - obs.self.origin).lengthSq() : 23170.0f * 23170.0f;
        spots.push_back(Spot{static_cast<int>(i), d2 - 1.125f * 1024.0f * 1024.0f});
    }
    std::sort(spots.begin(), spots.end(), [](const Spot& a, const Spot& b) { return a.metric > b.metric; });
    std::vector<double> w(spots.size(), 0.0);
    if (spots[0].metric <= 0.0f) {
        w[0] = 0.8;
        for (size_t i = 1; i < spots.size() && i < 3; i++) {
            w[i] = 0.1;
        }
    } else {
        const size_t top = std::min<size_t>(5, spots.size());
        for (size_t i = 0; i < top; i++) {
            if (spots[i].metric > 0.0f) {
                w[i] = static_cast<double>(top - i);
            }
        }
    }
    const int n = ParticlesPerTrack(obs);
    t.parts.assign(n, Particle());
    for (Particle& p : t.parts) {
        const MapSpawn& s = m_map->spawns[spots[m_rng.Categorical(w)].idx];
        p.pos             = s.pos + Vec3(static_cast<float>(m_rng.Normal(0.0, 12.0)), static_cast<float>(m_rng.Normal(0.0, 12.0)), 0.0f);
        p.cell            = m_map->NearestCell(p.pos, 128.0f);
        p.w               = 1.0f / n;
    }
}

void BeliefFilter::InitTrack(TrackState& t, const Observation& obs)
{
    SeedFromPrior(t, obs, true);
    t.initialised = true;
}

void BeliefFilter::Predict(TrackState& t, const Observation& obs)
{
    const float dt   = TICK_S;
    const float half = m_map ? 0.45f * m_map->cellSize : 16.0f;
    const bool  reckon = t.est.seenThisLife && t.lostTick < DEAD_RECKON_TICKS;
    for (Particle& p : t.parts) {
        if (reckon) {
            // right after losing sight people keep going the way they went
            const float s = static_cast<float>(m_rng.Normal(1.0, 0.35));
            p.pos += t.est.lastSeenVel * (dt * s);
            p.pos.x += static_cast<float>(m_rng.Normal(0.0, 4.0));
            p.pos.y += static_cast<float>(m_rng.Normal(0.0, 4.0));
            if (m_map) {
                const int c = m_map->CellAt(p.pos);
                if (c >= 0) {
                    if (c != p.cell) {
                        p.prev = p.cell;
                    }
                    p.cell = c;
                }
            }
            continue;
        }
        if (!m_map || p.cell < 0) {
            // free space: a random walk at the hidden move speed
            const float a = static_cast<float>(m_rng.Uniform(0.0, 360.0));
            const double go = m_rng.Uniform();
            if (go < 0.5) {
                p.pos += YawDir(a) * (195.0f * dt);
            }
            continue;
        }
        const MapCell& mc = m_map->cells[p.cell];
        const double   u  = m_rng.Uniform();
        if (u < Clamp(mc.leave * m_p->moveBoost, 0.0f, 0.95f)) {
            const Vec3 toBot = obs.self.origin - mc.center;
            m_map->KernelWeights(p.cell, toBot, toBot.lengthXY(), m_kTo, m_kW);
            if (!m_kTo.empty()) {
                std::vector<double>& w = m_kWd;
                w.assign(m_kW.begin(), m_kW.end());
                if (p.prev >= 0 && p.prev != p.cell && m_p->momentum != 0.0f) {
                    // keep going the way it was going
                    const Vec3  h  = mc.center - m_map->cells[p.prev].center;
                    const float hl = h.lengthXY();
                    for (size_t k = 0; k < w.size(); k++) {
                        const Vec3  dv = m_map->cells[m_kTo[k]].center - mc.center;
                        const float dl = dv.lengthXY();
                        if (hl > 1.0f && dl > 1.0f) {
                            w[k] *= std::exp(m_p->momentum * (h.x * dv.x + h.y * dv.y) / (hl * dl));
                        }
                    }
                }
                const int to = m_kTo[m_rng.Categorical(w)];
                p.prev       = p.cell;
                p.cell       = to;
                const MapCell& nc = m_map->cells[to];
                p.pos = nc.center
                      + Vec3(static_cast<float>(m_rng.Uniform(-half, half)), static_cast<float>(m_rng.Uniform(-half, half)), 0.0f);
            }
        }
    }
}

void BeliefFilter::NegativeInfo(TrackState& t, const Observation& obs, float hfovDeg, float vfovDeg)
{
    if (!obs.self.alive) {
        return;
    }
    const Vec3& eye     = obs.self.eye;
    const int   botCell = m_map ? m_map->CellAt(obs.self.origin) : -1;
    for (Particle& p : t.parts) {
        const Vec3  target = p.pos + Vec3(0.0f, 0.0f, CHEST_HEIGHT);
        const Vec3  d      = target - eye;
        const float dist   = d.length();
        if (dist < 48.0f) {
            p.w *= 0.2f;  // it would be touching us
            continue;
        }
        // most particles are off screen sideways: the pitch only for those within the yaw
        if (std::fabs(Wrap180(YawOf(d) - obs.self.viewYaw)) > 0.5f * hfovDeg
            || std::fabs(PitchOf(d) - obs.self.viewPitch) > 0.5f * vfovDeg) {
            continue;
        }
        float vis;
        if (m_map && botCell >= 0 && p.cell >= 0) {
            vis = m_map->Visibility(botCell, p.cell);
        } else {
            vis = dist < 512.0f ? 0.5f : 0.2f;
        }
        if (vis > 0.0f) {
            p.w *= 1.0f - m_p->negDetect * vis;
        }
    }
}

float BeliefFilter::SoundLikelihood(const Particle& p, const SoundObs& s, const Observation& obs) const
{
    const Vec3  d     = p.pos - obs.self.origin;
    const float yaw   = YawOf(d);
    const float sig   = std::max(3.0f, s.yawSigma * m_p->soundSigmaScale);
    const float e1    = Wrap180(yaw - s.yaw) / sig;
    float       ly    = std::exp(-0.5f * e1 * e1);
    if (s.frontBack) {
        const float c  = m_perc ? m_perc->frontBackConfusion : 0.25f;
        const float e2 = Wrap180(yaw - s.mirrorYaw) / sig;
        ly             = (1.0f - c) * ly + c * std::exp(-0.5f * e2 * e2);
    }
    const float dist = std::max(16.0f, d.length());
    const float lr   = std::log(dist / std::max(16.0f, s.dist)) / std::max(0.1f, s.distLogSd * 1.5f);
    const float ld   = std::exp(-0.5f * lr * lr);
    return ly * ld;
}

void BeliefFilter::Inject(TrackState& t, float yaw, float yawSigma, float mirrorYaw, float mirrorP, float dist, float distLogSd,
                          float share, bool needVisible, const Observation& obs)
{
    if (share <= 0.0f || t.parts.empty()) {
        return;
    }
    const int n       = static_cast<int>(t.parts.size());
    const int k       = std::min(n, static_cast<int>(share * n + 0.5f));
    const int botCell = m_map ? m_map->CellAt(obs.self.origin) : -1;
    // replace the least likely particles
    std::vector<int>& order = m_order;
    order.resize(n);
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }
    std::sort(order.begin(), order.end(), [&](int a, int b) { return t.parts[a].w < t.parts[b].w; });
    float wsum = 0.0f;
    for (const Particle& p : t.parts) {
        wsum += p.w;
    }
    const float wnew = wsum / n;
    for (int j = 0; j < k; j++) {
        Particle& p = t.parts[order[j]];
        for (int tries = 0; tries < 4; tries++) {
            const bool  mir = m_rng.Uniform() < mirrorP;
            const float y   = (mir ? mirrorYaw : yaw) + static_cast<float>(m_rng.Normal(0.0, yawSigma));
            const float d   = std::max(32.0f, dist * static_cast<float>(std::exp(m_rng.Normal(0.0, distLogSd))));
            const Vec3  pos = obs.self.origin + YawDir(y) * d;
            if (m_map) {
                const int c = m_map->NearestCell(pos, 96.0f);
                if (c < 0) {
                    continue;
                }
                if (needVisible && botCell >= 0 && m_map->Visibility(botCell, c) < 0.3f) {
                    continue;
                }
                p.cell = c;
                p.prev = -1;
                p.pos  = m_map->cells[c].center;
            } else {
                p.pos = pos;
            }
            p.w = wnew;
            break;
        }
    }
}

void BeliefFilter::ApplySound(TrackState& t, const SoundObs& s, const Observation& obs)
{
    // how well does the current cloud explain the sound?
    double lbar = 0.0, wsum = 0.0;
    for (const Particle& p : t.parts) {
        lbar += p.w * SoundLikelihood(p, s, obs);
        wsum += p.w;
    }
    lbar = wsum > 0.0 ? lbar / wsum : 0.0;
    if (lbar < 0.25) {
        const float c = s.frontBack ? (m_perc ? m_perc->frontBackConfusion : 0.25f) : 0.0f;
        Inject(t, s.yaw, s.yawSigma * m_p->soundSigmaScale, s.mirrorYaw, c, s.dist, s.distLogSd,
               m_p->injectMax * static_cast<float>(1.0 - lbar / 0.25), false, obs);
    }
    for (Particle& p : t.parts) {
        p.w *= 0.01f + SoundLikelihood(p, s, obs);
    }
    t.est.msSinceHeard = 0;
    t.est.lastThreatMs = obs.self.timeMs;
}

void BeliefFilter::ApplyDamage(TrackState& t, const DamageObs& dm, const Observation& obs)
{
    const int botCell = m_map ? m_map->CellAt(obs.self.origin) : -1;
    double    lbar = 0.0, wsum = 0.0;
    for (const Particle& p : t.parts) {
        const float e = Wrap180(YawOf(p.pos - obs.self.origin) - dm.yaw) / std::max(3.0f, dm.yawSigma);
        lbar += p.w * std::exp(-0.5f * e * e);
        wsum += p.w;
    }
    lbar = wsum > 0.0 ? lbar / wsum : 0.0;
    if (lbar < 0.25) {
        // the shooter is somewhere along that bearing, in a cell that sees us
        Inject(t, dm.yaw, dm.yawSigma, dm.yaw, 0.0f, 500.0f, 0.6f, m_p->injectMax * static_cast<float>(1.0 - lbar / 0.25),
               true, obs);
    }
    for (Particle& p : t.parts) {
        const Vec3  d  = p.pos - obs.self.origin;
        const float e  = Wrap180(YawOf(d) - dm.yaw) / std::max(3.0f, dm.yawSigma);
        float       lv = 1.0f;
        if (m_map && botCell >= 0 && p.cell >= 0) {
            lv = 0.1f + m_map->Visibility(botCell, p.cell);  // the shooter had to see us
        }
        p.w *= (0.05f + std::exp(-0.5f * e * e)) * lv;
    }
    t.est.lastThreatMs = obs.self.timeMs;
}

void BeliefFilter::Normalise(TrackState& t, const Observation& obs)
{
    double sum = 0.0;
    for (const Particle& p : t.parts) {
        sum += p.w;
    }
    if (!(sum > 1e-12)) {
        // every hypothesis was ruled out: start again from where people usually are
        SeedFromPrior(t, obs, true);
        return;
    }
    double sq = 0.0;
    for (Particle& p : t.parts) {
        p.w = static_cast<float>(p.w / sum);
        sq += static_cast<double>(p.w) * p.w;
    }
    const double ess = 1.0 / sq;
    t.est.ess        = static_cast<float>(ess / t.parts.size());
    if (ess < m_p->essResample * t.parts.size()) {
        // systematic resampling with a little roughening
        const int n  = static_cast<int>(t.parts.size());
        m_scratch.resize(n);
        const double step = 1.0 / n;
        double       u    = m_rng.Uniform() * step;
        double       c    = t.parts[0].w;
        int          i    = 0;
        for (int k = 0; k < n; k++) {
            while (u > c && i < n - 1) {
                i++;
                c += t.parts[i].w;
            }
            m_scratch[k] = t.parts[i];
            m_scratch[k].pos.x += static_cast<float>(m_rng.Normal(0.0, m_p->jitter));
            m_scratch[k].pos.y += static_cast<float>(m_rng.Normal(0.0, m_p->jitter));
            m_scratch[k].w = static_cast<float>(step);
            u += step;
        }
        t.parts.swap(m_scratch);
        if (m_map) {
            for (Particle& p : t.parts) {
                const int cell = m_map->CellAt(p.pos);
                if (cell >= 0) {
                    p.cell = cell;
                }
            }
        }
    }
}

void BeliefFilter::Summarise(TrackState& t, const Observation& obs, bool exposures)
{
    BeliefEstimate& e = t.est;
    // weighted mean and spread
    Vec3   mean;
    double wsum = 0.0;
    for (const Particle& p : t.parts) {
        mean += p.pos * p.w;
        wsum += p.w;
    }
    if (wsum > 0.0) {
        mean = mean * static_cast<float>(1.0 / wsum);
    }
    double var = 0.0;
    for (const Particle& p : t.parts) {
        var += p.w * (p.pos - mean).lengthSq();
    }
    e.spread = static_cast<float>(std::sqrt(std::max(0.0, var / std::max(wsum, 1e-9))));

    // mode: the densest cell neighbourhood
    if (m_map && m_map->NumCells() > 0) {
        if (m_cellMass.size() != static_cast<size_t>(m_map->NumCells())) {
            m_cellMass.assign(m_map->NumCells(), 0.0f);
        }
        for (int c : m_touched) {
            m_cellMass[c] = 0.0f;
        }
        m_touched.clear();
        for (const Particle& p : t.parts) {
            if (p.cell >= 0) {
                if (m_cellMass[p.cell] == 0.0f) {
                    m_touched.push_back(p.cell);
                }
                m_cellMass[p.cell] += p.w;
            }
        }
        int   best  = -1;
        float bestM = -1.0f;
        for (int c : m_touched) {
            float m = m_cellMass[c];
            for (int k = m_map->rStart[c]; k < m_map->rStart[c + 1]; k++) {
                m += m_cellMass[m_map->rTo[k]];
            }
            if (m > bestM) {
                bestM = m;
                best  = c;
            }
        }
        if (best >= 0) {
            const Vec3& bc = m_map->cells[best].center;
            Vec3        mm;
            double      mw = 0.0;
            for (const Particle& p : t.parts) {
                if ((p.pos - bc).lengthXY() < 96.0f) {
                    mm += p.pos * p.w;
                    mw += p.w;
                }
            }
            e.mode = mw > 0.0 ? mm * static_cast<float>(1.0 / mw) : bc;
        } else {
            e.mode = mean;
        }
    } else {
        e.mode = mean;
    }

    // exposure points: first cell visible from the bot on each particle's way toward it
    e.nExposure   = 0;
    e.visibleSoon = 0.0f;
    if (!exposures || !m_map || !obs.self.alive) {
        return;
    }
    const int botCell = m_map->CellAt(obs.self.origin);
    if (botCell < 0) {
        return;
    }
    RefreshPathTree(botCell);
    struct Exp {
        int   cell;
        float mass;
        float eta;
        float bestW;
        int   back[PATH_BACK + 1];   // the heaviest particle's last cells before the exposure, and the exposure
    };
    std::vector<Exp> ex;
    const float      speed = std::max(100.0f, m_map->hiddenMoveSpeed);
    for (const Particle& p : t.parts) {
        if (p.cell < 0) {
            continue;
        }
        int   c    = p.cell;
        float path = 0.0f;
        int   hit  = -1;
        int   back[PATH_BACK + 1];
        for (int k = 0; k <= PATH_BACK; k++) {
            back[k] = -1;
        }
        for (int step = 0; step < 48 && c >= 0; step++) {
            for (int k = 0; k < PATH_BACK; k++) {
                back[k] = back[k + 1];
            }
            back[PATH_BACK] = c;
            if (m_map->Visibility(botCell, c) >= 0.5f) {
                hit = c;
                break;
            }
            const int nx = m_next[c];
            if (nx < 0) {
                break;
            }
            path += (m_map->cells[nx].center - m_map->cells[c].center).length();
            c = nx;
        }
        if (hit < 0) {
            continue;
        }
        const float eta = 1000.0f * path / speed;
        bool        found = false;
        for (Exp& x : ex) {
            if (x.cell == hit) {
                x.eta = (x.eta * x.mass + eta * p.w) / (x.mass + p.w);
                x.mass += p.w;
                if (p.w > x.bestW) {
                    x.bestW = p.w;
                    for (int k = 0; k <= PATH_BACK; k++) {
                        x.back[k] = back[k];
                    }
                }
                found = true;
                break;
            }
        }
        if (!found) {
            Exp x{hit, p.w, eta, p.w, {}};
            for (int k = 0; k <= PATH_BACK; k++) {
                x.back[k] = back[k];
            }
            ex.push_back(x);
        }
        if (eta < 300.0f) {
            e.visibleSoon += p.w;
        }
    }
    std::sort(ex.begin(), ex.end(), [](const Exp& a, const Exp& b) { return a.mass > b.mass; });
    int paths[MAX_EXPOSURE][PATH_BACK + 1];
    for (size_t i = 0; i < ex.size() && e.nExposure < MAX_EXPOSURE; i++) {
        e.exposure[e.nExposure]      = m_map->cells[ex[i].cell].center;
        e.exposureMass[e.nExposure]  = ex[i].mass;
        e.exposureEtaMs[e.nExposure] = ex[i].eta;
        e.exposureCell[e.nExposure]  = ex[i].cell;
        e.cornerValid[e.nExposure]   = false;
        for (int k = 0; k <= PATH_BACK; k++) {
            paths[e.nExposure][k] = ex[i].back[k];
        }
        e.nExposure++;
    }
    if (m_world && e.nExposure > 0) {
        Corners(e, obs, paths);
    }
}

void BeliefFilter::Update(const Observation& obs, float hfovDeg, float vfovDeg)
{
    const int now = obs.self.timeMs;

    // tracks for the listed enemies (at most MAX_TRACKS, most recent threats first)
    for (const EnemyObs& eo : obs.enemies) {
        if (FindTrack(eo.id) >= 0) {
            continue;
        }
        if (static_cast<int>(m_tracks.size()) < MAX_TRACKS) {
            TrackState t;
            t.est.enemyId = eo.id;
            m_tracks.push_back(t);
        } else if (eo.detected) {
            // replace the stalest track
            int stale = 0;
            for (int i = 1; i < static_cast<int>(m_tracks.size()); i++) {
                if (m_tracks[i].est.lastThreatMs < m_tracks[stale].est.lastThreatMs) {
                    stale = i;
                }
            }
            m_tracks[stale]           = TrackState();
            m_tracks[stale].est.enemyId = eo.id;
        }
    }
    // drop tracks of enemies that are no longer listed (left the game)
    for (int i = static_cast<int>(m_tracks.size()) - 1; i >= 0; i--) {
        bool listed = false;
        for (const EnemyObs& eo : obs.enemies) {
            listed |= eo.id == m_tracks[i].est.enemyId;
        }
        if (!listed) {
            m_tracks.erase(m_tracks.begin() + i);
        }
    }

    for (TrackState& t : m_tracks) {
        BeliefEstimate& e = t.est;
        if (!t.initialised) {
            InitTrack(t, obs);
        }
        // kill feed
        bool died = obs.gotKillOf == e.enemyId;
        for (int id : obs.deaths) {
            died |= id == e.enemyId;
        }
        if (died) {
            e.dead         = true;
            e.seenThisLife = false;
            t.deadUntilMs  = now + RESPAWN_MIN_MS;
        }
        if (e.dead && now >= t.deadUntilMs) {
            e.dead = false;
            SeedFromSpawns(t, obs);
        }
        const EnemyObs *seen = nullptr;
        for (const EnemyObs& eo : obs.enemies) {
            if (eo.id == e.enemyId && eo.detected) {
                seen = &eo;
            }
        }
        e.valid    = true;
        e.detected = seen != nullptr;
        if (e.msSinceHeard < 1000000) {
            e.msSinceHeard += TICK_MS;
        }
        if (seen) {
            // a sighting collapses the cloud onto what is seen
            e.dead         = false;
            e.seenThisLife = true;
            e.msSinceSeen  = 0;
            e.lastSeenPos  = seen->pos - Vec3(0.0f, 0.0f, 0.5f * seen->bodyHeight);
            e.lastSeenVel  = seen->vel;
            e.lastThreatMs = now;
            t.lostTick     = 0;
            const int   n  = static_cast<int>(t.parts.size());
            const int   c  = m_map ? m_map->CellAt(e.lastSeenPos) : -1;
            for (Particle& p : t.parts) {
                p.pos  = e.lastSeenPos + Vec3(static_cast<float>(m_rng.Normal(0.0, 6.0)), static_cast<float>(m_rng.Normal(0.0, 6.0)), 0.0f);
                p.cell = c;
                p.prev = -1;
                p.w    = 1.0f / n;
            }
            e.mode    = e.lastSeenPos;
            e.modeVel = e.lastSeenVel;
            e.spread  = 8.0f;
            e.ess     = 1.0f;
        } else if (!e.dead) {
            if (e.msSinceSeen < 1000000) {
                e.msSinceSeen += TICK_MS;
            }
            t.lostTick++;
            Predict(t, obs);
            NegativeInfo(t, obs, hfovDeg, vfovDeg);
            Normalise(t, obs);
        }
    }

    // sounds and damage have no identity: give each to the track that explains it best. People
    // follow a few sounds at a time, not a battle's worth: the nearest MAX_HEARD_SOUNDS of a frame count
    // (a duel never has more), and the track is chosen on every 4th particle (an unbiased estimate).
    const SoundObs *heard[MAX_HEARD_SOUNDS];
    int             nHeard = 0;
    for (const SoundObs& s : obs.sounds) {
        if (nHeard < MAX_HEARD_SOUNDS) {
            heard[nHeard++] = &s;
        } else {
            int far = 0;
            for (int k = 1; k < nHeard; k++) {
                far = heard[k]->dist > heard[far]->dist ? k : far;
            }
            if (s.dist < heard[far]->dist) {
                heard[far] = &s;
            }
        }
    }
    for (int h = 0; h < nHeard; h++) {
        const SoundObs& s    = *heard[h];
        int             best = -1;
        double          bl   = 0.0;
        for (size_t i = 0; i < m_tracks.size(); i++) {
            TrackState& t = m_tracks[i];
            if (t.est.dead || t.est.detected) {
                continue;
            }
            double l = 0.0;
            for (size_t j = 0; j < t.parts.size(); j += 4) {
                l += t.parts[j].w * SoundLikelihood(t.parts[j], s, obs);
            }
            if (l > bl) {
                bl   = l;
                best = static_cast<int>(i);
            }
        }
        if (best >= 0) {
            ApplySound(m_tracks[best], s, obs);
            Normalise(m_tracks[best], obs);
        }
    }
    for (const DamageObs& d : obs.damage) {
        int best = -1;
        for (size_t i = 0; i < m_tracks.size(); i++) {
            if (m_tracks[i].est.detected) {
                best = -2;  // a visible enemy is shooting us: no update needed
                break;
            }
            if (!m_tracks[i].est.dead && (best < 0 || m_tracks[i].est.lastThreatMs > m_tracks[best].est.lastThreatMs)) {
                best = static_cast<int>(i);
            }
        }
        if (best >= 0) {
            ApplyDamage(m_tracks[best], d, obs);
            Normalise(m_tracks[best], obs);
        }
    }

    // focus: whoever is visible, else the most recent threat
    m_focus = -1;
    int bestT = -2000000000;
    for (size_t i = 0; i < m_tracks.size(); i++) {
        const BeliefEstimate& e = m_tracks[i].est;
        int key = e.lastThreatMs;
        if (e.detected) {
            key = 2000000000;
        }
        if (e.dead) {
            key -= 1000000;
        }
        if (key > bestT) {
            bestT   = key;
            m_focus = static_cast<int>(i);
        }
    }

    // only the focus track's exposure points are used (view, trigger anticipation): the others
    // skip the path walks
    for (size_t i = 0; i < m_tracks.size(); i++) {
        TrackState& t = m_tracks[i];
        if (!t.est.detected && !t.est.dead) {
            Summarise(t, obs, static_cast<int>(i) == m_focus);
        } else if (t.est.detected) {
            Summarise(t, obs, static_cast<int>(i) == m_focus);
            t.est.mode = t.est.lastSeenPos;
        }
    }
}

} // namespace hb
