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
// hb_map.h: map knowledge shared by all bots on a map.
//
// A route graph of 32-unit cells split into floor levels, with the human
// occupancy, dwell and move kernel from the recordings (humanbot/maps), or a
// graph derived from the navmesh on maps without a prior. Cell-to-cell
// visibility comes from engine traces once built, from the recorded LOS before.

#pragma once

#include "hb_math.h"
#include "hb_model.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace hb
{

struct MapCell {
    int   ix     = 0;
    int   iy     = 0;
    float z      = 0.0f;      // floor height (player origin z)
    Vec3  center;             // (ix + 0.5, iy + 0.5) * size, z; snapped to the navmesh when possible
    float occ[CTX_COUNT] = {};
    float occTotal = 0.0f;
    float leave    = 0.3f;    // per-tick probability of moving on while the enemy is hidden
    float still    = 0.2f;
    bool  snapped  = false;
};

struct MapSpawn {
    Vec3  pos;
    float yaw   = 0.0f;
    float count = 1.0f;
};

class MapPrior
{
public:
    std::string            name;
    int32_t                checksum  = 0;
    float                  cellSize  = 32.0f;
    bool                   fromNavmesh = false;
    std::vector<MapCell>   cells;
    std::vector<MapSpawn>  spawns;
    std::vector<float>     kernelDistEdges;
    std::vector<float>     kernelBeta;
    float                  hiddenMoveSpeed = 195.0f;

    // Directed move kernel (CSR over cells): counts of observed moves.
    std::vector<int>   kStart;
    std::vector<int>   kTo;
    std::vector<float> kCount;

    // Undirected route graph (CSR) with edge lengths, for path planning.
    std::vector<int>   rStart;
    std::vector<int>   rTo;
    std::vector<float> rLen;

    void AddCellIndex();                          // (re)build the (ix, iy) -> cells lookup
    void BuildRouteGraph();                       // symmetrise the kernel into rStart/rTo/rLen
    int  NumCells() const { return static_cast<int>(cells.size()); }

    int  CellAt(const Vec3& p) const;             // cell containing p (nearest level), -1 if none
    int  NearestCell(const Vec3& p, float maxDist) const;

    // Probability that cells a and b see each other (0..1); runtime table first.
    float Visibility(int a, int b) const;
    bool  HasRuntimeVisibility() const { return m_runtimeReady; }
    void  InitRuntimeVisibility();                // allocate the trace-built table (all unknown)
    void  SetRuntimeVisibility(int a, int b, bool visible);
    void  FinishRuntimeVisibility();              // switch lookups over to the trace table
    void  SetEmpiricalVisibility(int a, int b, float frac);
    const std::vector<uint8_t>& RuntimeBits() const { return m_runtimeBits; }
    bool  LoadRuntimeBits(const std::vector<uint8_t>& bits);

    // Shortest-path tree toward `target` (Dijkstra on the route graph).
    // next[c] = neighbour of c one step closer to target (-1 when unreachable), dist[c] = path length.
    void PathTreeTo(int target, std::vector<int>& next, std::vector<float>& dist) const;

    // Enemy-direction-conditioned move weights out of `cell` (belief prediction).
    // Fills (to, weight) pairs; weights are unnormalised.
    void KernelWeights(int cell, const Vec3& towardEnemy, float enemyDist, std::vector<int>& to, std::vector<float>& w) const;

private:
    std::unordered_map<int64_t, std::vector<int>> m_columns;
    std::vector<uint8_t>                           m_empirical;   // 255 unknown, else round(frac * 254)
    std::vector<uint8_t>                           m_runtimeBits; // 2 bits per pair: 0 unknown, 1 hidden, 2 visible
    bool                                           m_runtimeReady = false;
    std::vector<int>                               m_revStart;    // reverse route graph (edges into a cell)
    std::vector<int>                               m_revTo;
    std::vector<float>                             m_revLen;

    size_t PairIndex(int a, int b) const;
};

// Derives a coarse route graph from sampled walkable points (navmesh fallback):
// points are binned into cells of `cellSize` and joined when the engine says
// two neighbouring cells are connected.
struct NavSample {
    Vec3 pos;
};

} // namespace hb
