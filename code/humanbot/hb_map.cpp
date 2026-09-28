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
// hb_map.cpp: route graph, cell lookup, visibility and path trees.

#include "hb_map.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <queue>
#include <utility>

namespace hb
{

void MapPrior::AddCellIndex()
{
    m_colStart.clear();
    m_colCells.clear();
    m_nx = m_ny = 0;
    if (cells.empty()) {
        return;
    }
    int ix1 = cells[0].ix, iy1 = cells[0].iy;
    m_ix0 = ix1;
    m_iy0 = iy1;
    for (const MapCell& c : cells) {
        m_ix0 = std::min(m_ix0, c.ix);
        m_iy0 = std::min(m_iy0, c.iy);
        ix1   = std::max(ix1, c.ix);
        iy1   = std::max(iy1, c.iy);
    }
    m_nx = ix1 - m_ix0 + 1;
    m_ny = iy1 - m_iy0 + 1;
    // counting sort of the cells by column, in cell order within a column
    m_colStart.assign(static_cast<size_t>(m_nx) * m_ny + 1, 0);
    for (const MapCell& c : cells) {
        m_colStart[static_cast<size_t>(c.ix - m_ix0) * m_ny + (c.iy - m_iy0) + 1]++;
    }
    for (size_t k = 1; k < m_colStart.size(); k++) {
        m_colStart[k] += m_colStart[k - 1];
    }
    m_colCells.assign(cells.size(), 0);
    std::vector<int> fill(m_colStart.begin(), m_colStart.end() - 1);
    for (int i = 0; i < NumCells(); i++) {
        m_colCells[fill[static_cast<size_t>(cells[i].ix - m_ix0) * m_ny + (cells[i].iy - m_iy0)]++] = i;
    }
}

bool MapPrior::Column(int ix, int iy, const int *&b, const int *&e) const
{
    const int x = ix - m_ix0, y = iy - m_iy0;
    if (x < 0 || y < 0 || x >= m_nx || y >= m_ny) {
        return false;
    }
    const size_t k = static_cast<size_t>(x) * m_ny + y;
    b              = m_colCells.data() + m_colStart[k];
    e              = m_colCells.data() + m_colStart[k + 1];
    return b != e;
}

void MapPrior::BuildRouteGraph()
{
    const int n = NumCells();
    std::vector<std::vector<std::pair<int, float>>> adj(n);
    auto add = [&](int a, int b) {
        const float len = (cells[a].center - cells[b].center).length();
        for (const auto& e : adj[a]) {
            if (e.first == b) {
                return;
            }
        }
        adj[a].push_back(std::make_pair(b, len));
    };
    for (int a = 0; a < n; a++) {
        for (int k = kStart[a]; k < kStart[a + 1]; k++) {
            const int b = kTo[k];
            if (b < 0 || b >= n || b == a) {
                continue;
            }
            add(a, b);
            // walkable both ways unless it is a drop (humans can fall, not climb)
            if (cells[b].z >= cells[a].z - 40.0f) {
                add(b, a);
            }
        }
    }
    rStart.assign(n + 1, 0);
    rTo.clear();
    rLen.clear();
    for (int a = 0; a < n; a++) {
        rStart[a] = static_cast<int>(rTo.size());
        for (const auto& e : adj[a]) {
            rTo.push_back(e.first);
            rLen.push_back(e.second);
        }
    }
    rStart[n] = static_cast<int>(rTo.size());
    // reverse graph for path trees toward a target
    std::vector<std::vector<std::pair<int, float>>> radj(n);
    for (int a = 0; a < n; a++) {
        for (const auto& e : adj[a]) {
            radj[e.first].push_back(std::make_pair(a, e.second));
        }
    }
    m_revStart.assign(n + 1, 0);
    m_revTo.clear();
    m_revLen.clear();
    for (int a = 0; a < n; a++) {
        m_revStart[a] = static_cast<int>(m_revTo.size());
        for (const auto& e : radj[a]) {
            m_revTo.push_back(e.first);
            m_revLen.push_back(e.second);
        }
    }
    m_revStart[n] = static_cast<int>(m_revTo.size());
}

int MapPrior::CellAt(const Vec3& p) const
{
    const int ix = static_cast<int>(std::floor(p.x / cellSize));
    const int iy = static_cast<int>(std::floor(p.y / cellSize));
    int       best = -1;
    float     bestD = 1e30f;
    for (int dx = 0; dx <= 1 && best < 0; dx++) {
        // exact column first, then the ring around it
        for (int ox = -dx; ox <= dx; ox++) {
            for (int oy = -dx; oy <= dx; oy++) {
                if (dx && ox > -dx && ox < dx && oy > -dx && oy < dx) {
                    continue;
                }
                const int *cb, *ce;
                if (!Column(ix + ox, iy + oy, cb, ce)) {
                    continue;
                }
                for (const int *it = cb; it != ce; ++it) {
                    const int   c  = *it;
                    const float dz = std::fabs(cells[c].z - p.z);
                    const float dxy = dx ? (cells[c].center - p).lengthXY() : 0.0f;
                    const float d  = dz + dxy;
                    if (dz < 72.0f && d < bestD) {
                        bestD = d;
                        best  = c;
                    }
                }
            }
        }
    }
    return best;
}

int MapPrior::NearestCell(const Vec3& p, float maxDist) const
{
    int   best  = CellAt(p);
    if (best >= 0) {
        return best;
    }
    float     bestD = maxDist * maxDist;
    auto      visit = [&](int i) {
        const Vec3  d  = cells[i].center - p;
        const float d2 = d.x * d.x + d.y * d.y + 4.0f * d.z * d.z;
        if (d2 < bestD || (d2 == bestD && best >= 0 && i < best)) {
            bestD = d2;
            best  = i;
        }
    };
    // only the columns within maxDist can hold a closer cell (a hot path: every injected particle)
    const int r = static_cast<int>(std::ceil(maxDist / cellSize)) + 1;
    if (m_nx > 0 && static_cast<double>(2 * r + 1) * (2 * r + 1) < NumCells()) {
        const int ix = static_cast<int>(std::floor(p.x / cellSize));
        const int iy = static_cast<int>(std::floor(p.y / cellSize));
        for (int ox = -r; ox <= r; ox++) {
            for (int oy = -r; oy <= r; oy++) {
                const int *cb, *ce;
                if (Column(ix + ox, iy + oy, cb, ce)) {
                    for (const int *it = cb; it != ce; ++it) {
                        visit(*it);
                    }
                }
            }
        }
        return best;
    }
    for (int i = 0; i < NumCells(); i++) {
        visit(i);
    }
    return best;
}

size_t MapPrior::PairIndex(int a, int b) const
{
    if (a > b) {
        std::swap(a, b);
    }
    return static_cast<size_t>(a) * static_cast<size_t>(NumCells()) + static_cast<size_t>(b);
}

void MapPrior::SetEmpiricalVisibility(int a, int b, float frac)
{
    const size_t n = static_cast<size_t>(NumCells());
    if (m_empirical.size() != n * n) {
        m_empirical.assign(n * n, 255);
    }
    if (a < 0 || b < 0 || a >= NumCells() || b >= NumCells()) {
        return;
    }
    m_empirical[PairIndex(a, b)] = static_cast<uint8_t>(std::lround(Clamp(frac, 0.0f, 1.0f) * 254.0f));
}

void MapPrior::InitRuntimeVisibility()
{
    const size_t n = static_cast<size_t>(NumCells());
    m_runtimeBits.assign((n * n + 3) / 4, 0);
    m_runtimeReady = false;
}

void MapPrior::SetRuntimeVisibility(int a, int b, bool visible)
{
    if (m_runtimeBits.empty() || a < 0 || b < 0 || a >= NumCells() || b >= NumCells()) {
        return;
    }
    const size_t idx   = PairIndex(a, b);
    const size_t byte  = idx >> 2;
    const int    shift = static_cast<int>(idx & 3) * 2;
    m_runtimeBits[byte] = static_cast<uint8_t>((m_runtimeBits[byte] & ~(3 << shift)) | ((visible ? 2 : 1) << shift));
}

void MapPrior::FinishRuntimeVisibility()
{
    m_runtimeReady = !m_runtimeBits.empty();
}

bool MapPrior::LoadRuntimeBits(const std::vector<uint8_t>& bits)
{
    const size_t n = static_cast<size_t>(NumCells());
    if (bits.size() != (n * n + 3) / 4) {
        return false;
    }
    m_runtimeBits  = bits;
    m_runtimeReady = true;
    return true;
}

float MapPrior::Visibility(int a, int b) const
{
    if (a < 0 || b < 0 || a >= NumCells() || b >= NumCells()) {
        return 0.0f;
    }
    if (a == b) {
        return 1.0f;
    }
    const size_t idx = PairIndex(a, b);
    if (m_runtimeReady) {
        const int v = (m_runtimeBits[idx >> 2] >> (static_cast<int>(idx & 3) * 2)) & 3;
        if (v == 2) {
            return 1.0f;
        }
        if (v == 1) {
            return 0.0f;
        }
    }
    if (!m_empirical.empty()) {
        const uint8_t e = m_empirical[idx];
        if (e != 255) {
            return e / 254.0f;
        }
    }
    return 0.0f;
}

void MapPrior::PathTreeTo(int target, std::vector<int>& next, std::vector<float>& dist) const
{
    const int n = NumCells();
    next.assign(n, -1);
    dist.assign(n, 1e30f);
    if (target < 0 || target >= n || m_revStart.size() != static_cast<size_t>(n + 1)) {
        return;
    }
    typedef std::pair<float, int> QE;
    std::priority_queue<QE, std::vector<QE>, std::greater<QE>> q;
    dist[target] = 0.0f;
    q.push(QE(0.0f, target));
    while (!q.empty()) {
        const QE e = q.top();
        q.pop();
        const int u = e.second;
        if (e.first > dist[u]) {
            continue;
        }
        // cells v with an edge v -> u move one step closer by going to u
        for (int k = m_revStart[u]; k < m_revStart[u + 1]; k++) {
            const int   v  = m_revTo[k];
            const float nd = dist[u] + m_revLen[k];
            if (nd < dist[v]) {
                dist[v] = nd;
                next[v] = u;
                q.push(QE(nd, v));
            }
        }
    }
}

void MapPrior::KernelWeights(int cell, const Vec3& towardEnemy, float enemyDist, std::vector<int>& to, std::vector<float>& w) const
{
    to.clear();
    w.clear();
    if (cell < 0 || cell >= NumCells()) {
        return;
    }
    float beta = 0.0f;
    if (!kernelBeta.empty()) {
        beta = kernelBeta[BinIndex(enemyDist, kernelDistEdges)];
    }
    const float el = towardEnemy.lengthXY();
    const Vec3& c  = cells[cell].center;
    auto push = [&](int b, float count) {
        const Vec3  d  = cells[b].center - c;
        const float dl = d.lengthXY();
        float       cs = 0.0f;
        if (dl > 1.0f && el > 1.0f) {
            cs = (d.x * towardEnemy.x + d.y * towardEnemy.y) / (dl * el);
        }
        to.push_back(b);
        w.push_back(count * std::exp(beta * cs));
    };
    if (kStart[cell] < kStart[cell + 1]) {
        for (int k = kStart[cell]; k < kStart[cell + 1]; k++) {
            push(kTo[k], kCount[k]);
        }
    } else {
        for (int k = rStart[cell]; k < rStart[cell + 1]; k++) {
            push(rTo[k], 1.0f);
        }
    }
}

} // namespace hb
