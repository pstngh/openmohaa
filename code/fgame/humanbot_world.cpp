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
// humanbot_world.cpp: the map context of the human bots.
//
// On the recorded maps the bots use the human prior (route graph, occupancy,
// spawns, empirical line of sight), checked against the map checksum and
// snapped to the navigation mesh. Other maps get a coarse graph derived from
// the navigation mesh. A cell-to-cell visibility table is then traced a few
// milliseconds per frame and cached on disk; until it is complete the prior's
// empirical visibility is used.

#include "humanbot_internal.h"
#include "navigation_recast_load.h"
#include "navigation_recast_helpers.h"
#include "playerstart.h"

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

#include <cmath>
#include <cstring>
#include <map>

static const float  SNAP_EXTENT_XY      = 24.0f;
static const float  SNAP_EXTENT_Z       = 64.0f;
static const float  VIS_EYE_HEIGHT      = 64.0f;   // between a crouched and a standing eye
static const int    VIS_BUDGET_MS       = 2;       // tracing time per server frame
static const int    INIT_WAIT_FRAMES    = 40;      // wait this long for the navigation mesh before going without
// A map without a recorded prior gets cells of 32 u, or coarser ones on a big map: the visibility table grows with the
// square of the cells (dm/brownffa has 229,322 cells of 32 u, a 13 GB table: the test server died of it) and the
// route queries with the cells. The practice maps' recorded priors have a few hundred.
static const int    MAX_NAV_CELLS       = 3000;
static const float  NAV_CELL_SIZES[]    = {32.0f, 48.0f, 64.0f, 96.0f, 128.0f, 192.0f, 256.0f, 384.0f, 512.0f, 768.0f};
static const char  *VIS_CACHE_MAGIC     = "HBV1";

struct WorldContext {
    bool          started    = false;
    bool          ready      = false;
    int           frames     = 0;
    hb::MapPrior  prior;
    HbWorldStatus status;
    // visibility build
    int           visA       = 0;
    int           visB       = 1;
    long long     visPairs   = 0;
    long long     visDone    = 0;
    std::string   cacheName;
};

static WorldContext s_world;

void HB_WorldReset()
{
    s_world = WorldContext();
}

const hb::MapPrior *HB_WorldPrior()
{
    return s_world.ready ? &s_world.prior : nullptr;
}

HbWorldStatus HB_WorldGetStatus()
{
    return s_world.status;
}

static std::string MapKey(const char *mapname)
{
    std::string key = mapname ? mapname : "";
    for (char& c : key) {
        if (c == '/' || c == '\\') {
            c = '_';
        } else {
            c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        }
    }
    return key;
}

static bool ReadGameFile(const std::string& path, std::string& out)
{
    void *buffer = NULL;
    long  len    = gi.FS_ReadFile(path.c_str(), &buffer, qtrue);
    if (len <= 0 || !buffer) {
        return false;
    }
    out.assign(static_cast<const char *>(buffer), static_cast<size_t>(len));
    gi.FS_FreeFile(buffer);
    return true;
}

//
// Navigation mesh helpers
//
static bool NavPoint(const hb::Vec3& p, hb::Vec3& onMesh)
{
    if (!navigationMap.IsValid() || !navigationMap.GetNavMeshQuery()) {
        return false;
    }
    const vec3_t gamePos = {p.x, p.y, p.z + 8.0f};
    float        center[3];
    ConvertGameToRecastCoord(gamePos, center);
    const float extents[3] = {SNAP_EXTENT_XY, SNAP_EXTENT_Z, SNAP_EXTENT_XY};
    dtPolyRef   ref         = 0;
    float       nearest[3];
    const dtStatus st =
        navigationMap.GetNavMeshQuery()->findNearestPoly(center, extents, navigationMap.GetQueryFilter(), &ref, nearest);
    if (dtStatusFailed(st) || !ref) {
        return false;
    }
    vec3_t out;
    ConvertRecastToGameCoord(nearest, out);
    onMesh = hb::Vec3(out[0], out[1], out[2]);
    return std::fabs(onMesh.x - p.x) <= SNAP_EXTENT_XY && std::fabs(onMesh.y - p.y) <= SNAP_EXTENT_XY;
}

static void SnapCells(hb::MapPrior& prior, int& snapped)
{
    snapped = 0;
    for (hb::MapCell& c : prior.cells) {
        hb::Vec3 p;
        if (NavPoint(c.center, p)) {
            c.center.z = p.z;
            c.snapped  = true;
            snapped++;
        }
    }
}

// A coarse route graph from the navigation mesh: polygons are sampled on a grid of
// the prior's cell size and joined inside a polygon and across polygon links. Gives up
// (false, tooMany set) as soon as the map has more than maxCells cells.
static bool BuildNavmeshPrior(hb::MapPrior& prior, float size, int maxCells, bool& tooMany)
{
    tooMany               = false;
    const dtNavMesh *mesh = navigationMap.IsValid() ? navigationMap.GetNavMesh() : NULL;
    if (!mesh) {
        return false;
    }
    std::map<std::pair<int, int>, std::vector<int>> columns;
    std::map<dtPolyRef, std::vector<int>>           polyCells;
    std::vector<std::vector<std::pair<int, float>>> adj;

    auto cellFor = [&](const vec3_t p) {
        const int ix = static_cast<int>(std::floor(p[0] / size));
        const int iy = static_cast<int>(std::floor(p[1] / size));
        std::vector<int>& col = columns[std::make_pair(ix, iy)];
        for (int c : col) {
            if (std::fabs(prior.cells[c].z - p[2]) < 48.0f) {
                return c;
            }
        }
        hb::MapCell cell;
        cell.ix       = ix;
        cell.iy       = iy;
        cell.z        = p[2];
        cell.center   = hb::Vec3((ix + 0.5f) * size, (iy + 0.5f) * size, p[2]);
        cell.occTotal = 0.0f;
        for (int k = 0; k < hb::CTX_COUNT; k++) {
            cell.occ[k] = 1.0f;
            cell.occTotal += 1.0f;
        }
        cell.leave   = 0.3f;
        cell.still   = 0.2f;
        cell.snapped = true;
        prior.cells.push_back(cell);
        adj.emplace_back();
        col.push_back(static_cast<int>(prior.cells.size()) - 1);
        return static_cast<int>(prior.cells.size()) - 1;
    };
    auto link = [&](int a, int b) {
        if (a == b) {
            return;
        }
        for (const auto& e : adj[a]) {
            if (e.first == b) {
                return;
            }
        }
        adj[a].push_back(std::make_pair(b, 1.0f));
        adj[b].push_back(std::make_pair(a, 1.0f));
    };

    for (int t = 0; t < mesh->getMaxTiles(); t++) {
        const dtMeshTile *tile = mesh->getTile(t);
        if (!tile || !tile->header) {
            continue;
        }
        const dtPolyRef base = mesh->getPolyRefBase(tile);
        for (int i = 0; i < tile->header->polyCount; i++) {
            const dtPoly *poly = &tile->polys[i];
            if (poly->getType() == DT_POLYTYPE_OFFMESH_CONNECTION || poly->vertCount < 3) {
                continue;
            }
            // game-space vertices of the polygon
            vec3_t verts[DT_VERTS_PER_POLYGON];
            float  minX = 1e9f, maxX = -1e9f, minY = 1e9f, maxY = -1e9f, sumZ = 0.0f;
            for (int k = 0; k < poly->vertCount; k++) {
                ConvertRecastToGameCoord(&tile->verts[poly->verts[k] * 3], verts[k]);
                minX = std::min(minX, verts[k][0]);
                maxX = std::max(maxX, verts[k][0]);
                minY = std::min(minY, verts[k][1]);
                maxY = std::max(maxY, verts[k][1]);
                sumZ += verts[k][2];
            }
            const float      meanZ = sumZ / poly->vertCount;
            std::vector<int> cells;
            // grid samples inside the (convex) polygon, plus its centre
            vec3_t centre = {0.5f * (minX + maxX), 0.5f * (minY + maxY), meanZ};
            cells.push_back(cellFor(centre));
            for (float x = std::floor(minX / size) * size + 0.5f * size; x < maxX; x += size) {
                for (float y = std::floor(minY / size) * size + 0.5f * size; y < maxY; y += size) {
                    bool inside = true;
                    for (int k = 0; k < poly->vertCount && inside; k++) {
                        const float *a = verts[k];
                        const float *b = verts[(k + 1) % poly->vertCount];
                        const float  cross = (b[0] - a[0]) * (y - a[1]) - (b[1] - a[1]) * (x - a[0]);
                        inside             = cross <= 0.5f;   // clockwise in game space
                    }
                    if (!inside) {
                        // try the other winding
                        inside = true;
                        for (int k = 0; k < poly->vertCount && inside; k++) {
                            const float *a     = verts[k];
                            const float *b     = verts[(k + 1) % poly->vertCount];
                            const float  cross = (b[0] - a[0]) * (y - a[1]) - (b[1] - a[1]) * (x - a[0]);
                            inside             = cross >= -0.5f;
                        }
                    }
                    if (inside) {
                        vec3_t p = {x, y, meanZ};
                        cells.push_back(cellFor(p));
                    }
                }
            }
            // cells of one polygon are joined to their grid neighbours
            for (size_t a = 0; a < cells.size(); a++) {
                for (size_t b = a + 1; b < cells.size(); b++) {
                    const hb::MapCell& ca = prior.cells[cells[a]];
                    const hb::MapCell& cb = prior.cells[cells[b]];
                    if (std::abs(ca.ix - cb.ix) <= 1 && std::abs(ca.iy - cb.iy) <= 1) {
                        link(cells[a], cells[b]);
                    }
                }
            }
            polyCells[base | static_cast<dtPolyRef>(i)] = cells;
            if (static_cast<int>(prior.cells.size()) > maxCells) {
                tooMany = true;
                return false;
            }
        }
    }
    if (prior.cells.empty()) {
        return false;
    }
    // polygon links: join the closest cells of neighbouring polygons
    for (int t = 0; t < mesh->getMaxTiles(); t++) {
        const dtMeshTile *tile = mesh->getTile(t);
        if (!tile || !tile->header) {
            continue;
        }
        const dtPolyRef base = mesh->getPolyRefBase(tile);
        for (int i = 0; i < tile->header->polyCount; i++) {
            const dtPoly *poly = &tile->polys[i];
            auto          from = polyCells.find(base | static_cast<dtPolyRef>(i));
            if (from == polyCells.end()) {
                continue;
            }
            for (unsigned int l = poly->firstLink; l != DT_NULL_LINK; l = tile->links[l].next) {
                auto to = polyCells.find(tile->links[l].ref);
                if (to == polyCells.end()) {
                    continue;
                }
                int   bestA = -1, bestB = -1;
                float best  = 1e18f;
                for (int a : from->second) {
                    for (int b : to->second) {
                        const float d = (prior.cells[a].center - prior.cells[b].center).length();
                        if (d < best) {
                            best  = d;
                            bestA = a;
                            bestB = b;
                        }
                    }
                }
                if (bestA >= 0) {
                    link(bestA, bestB);
                }
            }
        }
    }
    const size_t n = prior.cells.size();
    prior.kStart.assign(n + 1, 0);
    prior.kTo.clear();
    prior.kCount.clear();
    for (size_t a = 0; a < n; a++) {
        prior.kStart[a] = static_cast<int>(prior.kTo.size());
        for (const auto& e : adj[a]) {
            prior.kTo.push_back(e.first);
            prior.kCount.push_back(e.second);
        }
    }
    prior.kStart[n]       = static_cast<int>(prior.kTo.size());
    prior.kernelDistEdges = {0.0f};
    prior.kernelBeta      = {0.0f};
    prior.cellSize        = size;
    prior.fromNavmesh     = true;
    prior.AddCellIndex();
    prior.BuildRouteGraph();
    return true;
}

// Deathmatch starts of the level (for maps without a recorded prior).
static void AddEngineSpawns(hb::MapPrior& prior)
{
    for (int i = 1; i <= level.m_SimpleArchivedEntities.NumObjects(); i++) {
        SimpleArchivedEntity *ent       = level.m_SimpleArchivedEntities.ObjectAt(i);
        const char           *classname = ent->getClassID();
        if (Q_stricmp(classname, "info_player_deathmatch") && Q_stricmp(classname, "info_player_allied")
            && Q_stricmp(classname, "info_player_axis")) {
            continue;
        }
        hb::MapSpawn sp;
        sp.pos   = hb::Vec3(ent->origin.x, ent->origin.y, ent->origin.z);
        sp.yaw   = ent->angles.y;
        sp.count = 1.0f;
        prior.spawns.push_back(sp);
    }
}

//
// Visibility table: time-sliced traces between cell centres, cached on disk
//
static bool LoadVisCache()
{
    std::string data;
    if (!ReadGameFile(s_world.cacheName, data) || data.size() < 12 || data.compare(0, 4, VIS_CACHE_MAGIC)) {
        return false;
    }
    int32_t cells = 0;
    int32_t sum   = 0;
    memcpy(&cells, data.data() + 4, 4);
    memcpy(&sum, data.data() + 8, 4);
    cells = LittleLong(cells);
    sum   = LittleLong(sum);
    if (cells != s_world.prior.NumCells() || sum != s_world.prior.checksum) {
        return false;
    }
    std::vector<uint8_t> bits(data.begin() + 12, data.end());
    return s_world.prior.LoadRuntimeBits(bits);
}

static void SaveVisCache()
{
    const std::vector<uint8_t>& bits = s_world.prior.RuntimeBits();
    std::string                 data(VIS_CACHE_MAGIC);
    const int32_t               cells = LittleLong(s_world.prior.NumCells());
    const int32_t               sum   = LittleLong(s_world.prior.checksum);
    data.append(reinterpret_cast<const char *>(&cells), 4);
    data.append(reinterpret_cast<const char *>(&sum), 4);
    data.append(reinterpret_cast<const char *>(bits.data()), bits.size());
    gi.FS_WriteFile(s_world.cacheName.c_str(), data.data(), static_cast<int>(data.size()));
}

static void BuildVisibilitySlice()
{
    hb::MapPrior& m     = s_world.prior;
    const int     n     = m.NumCells();
    const int     start = gi.Milliseconds();
    const int     mask  = MASK_SOLID & ~CONTENTS_TRIGGER;
    int           count = 0;
    while (s_world.visA < n - 1) {
        const hb::MapCell& a  = m.cells[s_world.visA];
        const hb::MapCell& b  = m.cells[s_world.visB];
        const Vector       pa(a.center.x, a.center.y, a.center.z + VIS_EYE_HEIGHT);
        const Vector       pb(b.center.x, b.center.y, b.center.z + VIS_EYE_HEIGHT);
        const bool         visible =
            G_SightTrace(pa, vec_zero, vec_zero, pb, (Entity *)NULL, (Entity *)NULL, mask, qfalse, "HumanBot visibility");
        m.SetRuntimeVisibility(s_world.visA, s_world.visB, visible);
        s_world.visDone++;
        if (++s_world.visB >= n) {
            s_world.visA++;
            s_world.visB = s_world.visA + 1;
        }
        if ((++count & 63) == 0 && gi.Milliseconds() - start >= VIS_BUDGET_MS) {
            break;
        }
    }
    s_world.status.visMs += gi.Milliseconds() - start;
    s_world.status.visProgress = s_world.visPairs > 0 ? static_cast<float>(s_world.visDone) / s_world.visPairs : 1.0f;
    if (s_world.visA >= n - 1) {
        m.FinishRuntimeVisibility();
        s_world.status.visReady    = true;
        s_world.status.visProgress = 1.0f;
        SaveVisCache();
        gi.DPrintf(
            "humanbot: visibility of %s built (%d cells, %d ms)\n", s_world.status.mapName.c_str(), n, s_world.status.visMs
        );
    }
}

static void InitWorld()
{
    s_world.started = true;
    HbWorldStatus& st = s_world.status;
    st.mapName        = level.mapname.c_str();
    st.navmesh        = navigationMap.IsValid();

    const std::string key      = MapKey(level.mapname.c_str());
    const cvar_t     *sumCvar  = gi.Cvar_Get("sv_mapChecksum", "", 0);
    const int32_t     checksum = sumCvar ? static_cast<int32_t>(strtol(sumCvar->string, NULL, 10)) : 0;

    // the recorded human prior, overridable from g_humanbot_model_dir
    std::string text = hb::EmbeddedText("maps/" + key + ".json");
    std::string overrideText;
    if (g_humanbot_model_dir && g_humanbot_model_dir->string[0]) {
        std::string full;
        if (ReadGameFile(std::string(g_humanbot_model_dir->string) + "/maps/" + key + ".json", full)) {
            if (text.empty()) {
                text = full;
            } else {
                overrideText = full;
            }
        }
    }
    bool loaded = false;
    if (!text.empty()) {
        std::string  error;
        hb::MapPrior prior;
        if (hb::LoadMapPrior(text, overrideText, prior, error)) {
            st.recorded   = true;
            st.checksumOk = checksum == 0 || prior.checksum == checksum;
            if (st.checksumOk) {
                s_world.prior = std::move(prior);
                loaded        = true;
                st.embedded   = true;
            } else {
                st.note = "map checksum differs from the recorded map; using the navigation mesh";
            }
        } else {
            st.note = "map prior rejected: " + error;
        }
    }
    if (!loaded) {
        for (const float size : NAV_CELL_SIZES) {
            bool tooMany  = false;
            s_world.prior = hb::MapPrior();
            if (BuildNavmeshPrior(s_world.prior, size, MAX_NAV_CELLS, tooMany)) {
                s_world.prior.name     = level.mapname.c_str();
                s_world.prior.checksum = checksum;
                AddEngineSpawns(s_world.prior);
                loaded = true;
                if (size > NAV_CELL_SIZES[0]) {
                    st.note += (st.note.empty() ? "" : "; ") + std::string("a big map: cells of ")
                             + std::to_string(static_cast<int>(size)) + " u (at most " + std::to_string(MAX_NAV_CELLS)
                             + " cells)";
                    gi.Printf("humanbot: %s is a big map: cells of %d u\n", level.mapname.c_str(), static_cast<int>(size));
                }
                break;
            }
            if (!tooMany) {
                break;
            }
        }
        if (!loaded) {
            s_world.prior = hb::MapPrior();
            if (st.note.empty()) {
                st.note = navigationMap.IsValid() ? "the map is too big for the bots' map context: bots hunt without a map"
                                                  : "no recorded prior and no navigation mesh: bots hunt without a map";
            }
        }
    }
    if (!loaded) {
        return;
    }
    if (st.navmesh) {
        SnapCells(s_world.prior, st.snapped);
    }
    st.prior  = true;
    st.cells  = s_world.prior.NumCells();
    st.spawns = static_cast<int>(s_world.prior.spawns.size());

    s_world.prior.InitRuntimeVisibility();
    s_world.cacheName = "humanbot/vis/" + key + "_" + std::to_string(static_cast<uint32_t>(s_world.prior.checksum)) + "_"
                      + std::to_string(s_world.prior.NumCells()) + ".bin";
    const long long n = s_world.prior.NumCells();
    s_world.visPairs  = n * (n - 1) / 2;
    if (LoadVisCache()) {
        st.visReady    = true;
        st.visCached   = true;
        st.visProgress = 1.0f;
    } else {
        s_world.prior.InitRuntimeVisibility();
        s_world.visA = 0;
        s_world.visB = 1;
    }
    s_world.ready = true;
    gi.DPrintf(
        "humanbot: %s map context: %s prior, %d cells (%d on the navmesh), %d spawns%s\n",
        st.mapName.c_str(),
        st.embedded ? "recorded" : "navmesh",
        st.cells,
        st.snapped,
        st.spawns,
        st.visCached ? ", cached visibility" : ""
    );
}

void HB_WorldFrame()
{
    if (!s_world.started) {
        // the navigation mesh is built when the level spawns (only with sv_maxbots > 0)
        if (!navigationMap.IsValid() && ++s_world.frames < INIT_WAIT_FRAMES) {
            return;
        }
        InitWorld();
        return;
    }
    if (s_world.ready && !s_world.status.visReady && s_world.prior.NumCells() > 1) {
        BuildVisibilitySlice();
    }
}
