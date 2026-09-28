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
// pm_world.h: a static world of axis-aligned boxes for running the real Pmove
// outside the server.
//
// PmWorld stands in for the collision model (cm_trace.c) and the server's
// entity clipping (sv_world.c):
//
//   * static boxes are world brushes (trace entityNum ENTITYNUM_WORLD);
//   * bodies are linked player entities (CONTENTS_BODY, entityNum = client
//     number). The moving player passes its own number as passEntityNum, so
//     players collide with each other like in the game.
//
// Traces reproduce CM_BoxTrace + SV_Trace for axial boxes, operation for
// operation: brush planes in the box-hull order (+x -x +y -y +z -z), the
// SURFACE_CLIP_EPSILON (0.125) back-off, startsolid/allsolid rules, position
// tests for zero-length moves (touching counts as inside) and the capsule
// ("cylinder") plane expansion Pmove asks for. The world is first, bodies
// after it, and a body replaces the result only with a smaller fraction.
//
// Pmove only knows plain function pointers, so a world is bound with
// PmWorldScope while Pmove runs (pm_runner.cpp does this). Like Pmove itself
// (globals pm/pml) this is single-threaded.

#pragma once

// q_shared.h defines two unused static functions that warn in every C++ unit.
#if defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "../../qcommon/q_shared.h"
#include "../../fgame/bg_public.h"
#if defined(__GNUC__)
#    pragma GCC diagnostic pop
#endif

#include <vector>

// Content mask of the telemetry logger's sight ray (MASK_SHOT without triggers).
#define PM_MASK_SIGHT (MASK_SHOT & ~CONTENTS_TRIGGER)

// Default thickness of the slabs AddRoom() builds.
#define PM_ROOM_THICKNESS 16.0f

class PmWorld
{
public:
    // One axial box. For bodies, mins/maxs/planes are relative to origin.
    struct Box {
        vec3_t   origin; // (0,0,0) for static boxes
        vec3_t   mins;
        vec3_t   maxs;
        int      contents;
        int      entityNum; // ENTITYNUM_WORLD, or the client number of a body
        cplane_t planes[6]; // +x, -x, +y, -y, +z, -z (CM_InitBoxHull order)
    };

    PmWorld();

    //
    // Static geometry (world brushes)
    //

    // Adds a box brush; returns its index. mins must not exceed maxs.
    int AddBox(const float mins[3], const float maxs[3], int contents = CONTENTS_SOLID);

    // Adds a closed room whose open inside is [mins, maxs]: a floor slab whose top
    // face is at mins[2], a ceiling slab whose bottom face is at maxs[2] and four
    // walls, each `thickness` thick. Returns the index of the floor box (the other
    // five follow it).
    int AddRoom(
        const float mins[3], const float maxs[3], float thickness = PM_ROOM_THICKNESS, int contents = CONTENTS_SOLID
    );

    // Adds a square pillar of half width `halfWidth` centred on (centerX, centerY),
    // from bottomZ to topZ. Returns its index.
    int
    AddPillar(float centerX, float centerY, float halfWidth, float bottomZ, float topZ, int contents = CONTENTS_SOLID);

    void       ClearBoxes();
    int        NumBoxes() const;
    const Box& GetBox(int index) const;

    //
    // Player bodies (linked player entities, CONTENTS_BODY)
    //

    // Links (or moves) the body of entity `entityNum` with the entity-relative
    // bounds mins/maxs at origin, like gi.linkentity after setOrigin/setSize.
    void SetBody(int entityNum, const float origin[3], const float mins[3], const float maxs[3]);
    void ClearBody(int entityNum);
    void ClearBodies();
    bool HasBody(int entityNum) const;
    // Returns the linked body of entityNum, or NULL.
    const Box *GetBody(int entityNum) const;

    //
    // Queries (engine semantics)
    //

    // SV_Trace: sweeps the box mins/maxs from start to end. passEntityNum is
    // skipped (usually the moving player). capsule is Pmove's "cylinder" flag.
    void Trace(
        trace_t     *results,
        const vec3_t start,
        const vec3_t mins,
        const vec3_t maxs,
        const vec3_t end,
        int          passEntityNum,
        int          contentMask,
        bool         capsule = false
    ) const;

    // SV_PointContents: OR of the contents of every box and body (except
    // passEntityNum) containing the point, faces included.
    int PointContents(const vec3_t point, int passEntityNum = ENTITYNUM_NONE) const;

    // Zero-extent ray against boxes and bodies, ignoring ignoreEntity. True when
    // the ray gets through (fraction >= 0.999) or when the first thing it hits is
    // targetEntity: the rule of the telemetry logger's centroid sight ray
    // (eye -> opponent centroid, ignoring the viewer, target = the opponent).
    bool LineOfSight(
        const vec3_t start,
        const vec3_t end,
        int          ignoreEntity,
        int          targetEntity = ENTITYNUM_NONE,
        int          contentMask  = PM_MASK_SIGHT
    ) const;

    //
    // Pmove callbacks (pmove_t::trace / pmove_t::pointcontents). They query the
    // world bound by the innermost live PmWorldScope and abort without one.
    //
    static void PmoveTrace(
        trace_t     *results,
        const vec3_t start,
        const vec3_t mins,
        const vec3_t maxs,
        const vec3_t end,
        int          passEntityNum,
        int          contentMask,
        int          capsule,
        qboolean     traceDeep
    );
    static int PmovePointContents(const vec3_t point, int passEntityNum);

    // World the callbacks currently use (NULL outside a PmWorldScope).
    static const PmWorld *Bound();

private:
    friend class PmWorldScope;

    std::vector<Box> m_boxes;
    std::vector<Box> m_bodies;

    static const PmWorld *s_bound;
};

// Binds a world to the Pmove callbacks for the lifetime of the object and
// restores the previous binding afterwards (scopes nest).
class PmWorldScope
{
public:
    explicit PmWorldScope(const PmWorld& world);
    ~PmWorldScope();

    PmWorldScope(const PmWorldScope&)            = delete;
    PmWorldScope& operator=(const PmWorldScope&) = delete;

private:
    const PmWorld *m_previous;
};
