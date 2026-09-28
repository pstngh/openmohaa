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
// pm_world.cpp: axis-aligned box world with CM_BoxTrace / SV_Trace semantics.
//
// The brush code below is CM_TraceThroughBrush / CM_TestBoxInBrush from
// cm_trace.c restricted to six-sided axial brushes (no bevels, fences, patches
// or terrain), with the same float/double promotions, so fractions, end
// positions and plane choices match the engine's for the same geometry.

#include "pm_world.h"

#include <cstring>

namespace
{

// cm_local.h defines SURFACE_CLIP_EPSILON as the double literal (0.125). The
// expressions below promote exactly like cm_trace.c does.
const double SURFACE_CLIP_EPSILON = 0.125;

// traceWork_t, reduced to what axial brushes need.
struct TraceWork {
    vec3_t  start;        // centre of the moving box at the start
    vec3_t  end;          // centre at the end
    vec3_t  size[2];      // symmetric extents around the centre
    vec3_t  offsets[8];   // corner for each plane signbits value
    vec3_t  bounds[2];    // bounds of the whole move
    bool    sphere;       // capsule plane expansion (sphere.use)
    float   radius;       // sphere.radius
    vec3_t  sphereOffset; // sphere.offset
    int     contents;     // brush mask
    trace_t trace;
};

// CM_InitBoxHull + CM_TempBoxModel: brush sides +x, -x, +y, -y, +z, -z.
void SetBoxPlanes(PmWorld::Box& box)
{
    int i;

    for (i = 0; i < 6; i++) {
        cplane_t *p    = &box.planes[i];
        const int axis = i >> 1;

        memset(p, 0, sizeof(*p));
        if (!(i & 1)) {
            p->normal[axis] = 1;
            p->dist         = box.maxs[axis];
            p->type         = axis;
        } else {
            p->normal[axis] = -1;
            p->dist         = -box.mins[axis];
            p->type         = 3 + axis;
        }
        SetPlaneSignbits(p);
    }
}

// The set-up part of CM_BoxTrace.
void InitTraceWork(
    TraceWork&   tw,
    const vec3_t start,
    const vec3_t end,
    const vec3_t mins,
    const vec3_t maxs,
    int          contentMask,
    bool         capsule
)
{
    vec3_t offset;
    int    i;

    memset(&tw, 0, sizeof(tw));
    tw.trace.fraction = 1;
    tw.trace.location = -1;
    tw.contents       = contentMask;

    // adjust so that mins and maxs are always symmetric
    for (i = 0; i < 3; i++) {
        offset[i]     = (mins[i] + maxs[i]) * 0.5;
        tw.size[0][i] = mins[i] - offset[i];
        tw.size[1][i] = maxs[i] - offset[i];
        tw.start[i]   = start[i] + offset[i];
        tw.end[i]     = end[i] + offset[i];
    }

    // Pmove's "cylinder": planes are pushed out by a vertical capsule of radius
    // min(half width, half height) instead of by the box corners
    if (capsule) {
        tw.sphere          = true;
        tw.radius          = (tw.size[1][0] > tw.size[1][2]) ? tw.size[1][2] : tw.size[1][0];
        tw.sphereOffset[0] = 0;
        tw.sphereOffset[1] = 0;
        tw.sphereOffset[2] = tw.size[1][2] - tw.radius;
    }

    // offsets[signbits] = vector to the appropriate corner from the centre
    for (i = 0; i < 8; i++) {
        tw.offsets[i][0] = tw.size[(i & 1) ? 1 : 0][0];
        tw.offsets[i][1] = tw.size[(i & 2) ? 1 : 0][1];
        tw.offsets[i][2] = tw.size[(i & 4) ? 1 : 0][2];
    }

    for (i = 0; i < 3; i++) {
        if (tw.start[i] < tw.end[i]) {
            tw.bounds[0][i] = tw.start[i] + tw.size[0][i];
            tw.bounds[1][i] = tw.end[i] + tw.size[1][i];
        } else {
            tw.bounds[0][i] = tw.end[i] + tw.size[0][i];
            tw.bounds[1][i] = tw.start[i] + tw.size[1][i];
        }
    }
}

// CM_TestBoxInBrush for a brush made of its six axial planes only: the box
// is inside when the bounds overlap, faces included.
void TestBoxInBox(TraceWork& tw, const PmWorld::Box& box)
{
    if (tw.bounds[0][0] > box.maxs[0] || tw.bounds[0][1] > box.maxs[1] || tw.bounds[0][2] > box.maxs[2]
        || tw.bounds[1][0] < box.mins[0] || tw.bounds[1][1] < box.mins[1] || tw.bounds[1][2] < box.mins[2]) {
        return;
    }

    tw.trace.startsolid = tw.trace.allsolid = qtrue;
    tw.trace.fraction                       = 0;
    tw.trace.contents                       = box.contents;
}

// CM_TraceThroughBrush (non-fence path) for an axial box brush.
void TraceThroughBox(TraceWork& tw, const PmWorld::Box& box)
{
    int             i;
    const cplane_t *plane;
    const cplane_t *clipplane;
    float           dist;
    float           enterFrac, leaveFrac;
    float           d1, d2;
    qboolean        getout, startout;
    float           f;
    float           t;

    enterFrac = -1.0;
    leaveFrac = 1.0;
    clipplane = NULL;

    getout   = qfalse;
    startout = qfalse;

    //
    // compare the trace against all planes of the brush
    // find the latest time the trace crosses a plane towards the interior
    // and the earliest time the trace crosses a plane towards the exterior
    //
    for (i = 0; i < 6; i++) {
        plane = &box.planes[i];

        if (tw.sphere) {
            // find the closest point on the capsule to the plane
            t = DotProduct(plane->normal, tw.sphereOffset);
            if (t < 0) {
                t = -t;
            }

            // adjust the plane distance appropriately for radius
            dist = t + plane->dist + tw.radius;
        } else {
            // adjust the plane distance appropriately for mins/maxs
            dist = plane->dist - DotProduct(tw.offsets[plane->signbits], plane->normal);
        }

        d1 = DotProduct(tw.start, plane->normal) - dist;
        d2 = DotProduct(tw.end, plane->normal) - dist;

        // if it doesn't cross the plane, the plane isn't relevant
        if (d1 <= 0 && d2 <= 0) {
            continue;
        }

        if (d2 > 0) {
            getout = qtrue; // endpoint is not in solid
        }
        if (d1 > 0) {
            startout = qtrue;

            // if completely in front of face, no intersection with the entire brush
            if (d2 >= SURFACE_CLIP_EPSILON || d2 >= d1) {
                return;
            }
        }

        // crosses face
        if (d1 > d2) { // enter
            f = (d1 - SURFACE_CLIP_EPSILON) / (d1 - d2);
            if (f < 0) {
                f = 0;
            }
            if (f > enterFrac) {
                enterFrac = f;
                clipplane = plane;
            }
        } else { // leave
            f = (d1 + SURFACE_CLIP_EPSILON) / (d1 - d2);
            if (f > 1) {
                f = 1;
            }
            if (f < leaveFrac) {
                leaveFrac = f;
            }
        }
    }

    //
    // all planes have been checked, and the trace was not
    // completely outside the brush
    //
    if (!startout) { // original point was inside brush
        tw.trace.startsolid = qtrue;
        if (!getout) {
            tw.trace.fraction = 0;
            tw.trace.allsolid = qtrue;
        }
        return;
    }

    if (enterFrac <= leaveFrac) {
        if (enterFrac > -1 && enterFrac < tw.trace.fraction) {
            if (enterFrac < 0) {
                enterFrac = 0;
            }
            tw.trace.fraction     = enterFrac;
            tw.trace.plane        = *clipplane;
            tw.trace.surfaceFlags = 0;
            tw.trace.shaderNum    = 0;
            tw.trace.contents     = box.contents;
        }
    }
}

bool SamePoint(const vec3_t a, const vec3_t b)
{
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

// CM_TransformedBoxTrace against one body (a CM_TempBoxModel, never rotated).
void TraceBody(
    trace_t            *results,
    const vec3_t        start,
    const vec3_t        end,
    const vec3_t        mins,
    const vec3_t        maxs,
    const PmWorld::Box& body,
    int                 contentMask,
    bool                capsule
)
{
    TraceWork tw;
    vec3_t    start_l, end_l;
    vec3_t    offset;
    vec3_t    symetricSize[2];
    int       i;

    // adjust so that mins and maxs are always symmetric
    for (i = 0; i < 3; i++) {
        offset[i]          = (mins[i] + maxs[i]) * 0.5;
        symetricSize[0][i] = mins[i] - offset[i];
        symetricSize[1][i] = maxs[i] - offset[i];
        start_l[i]         = start[i] + offset[i];
        end_l[i]           = end[i] + offset[i];
    }

    // subtract origin offset
    VectorSubtract(start_l, body.origin, start_l);
    VectorSubtract(end_l, body.origin, end_l);

    // sweep the box through the model
    InitTraceWork(tw, start_l, end_l, symetricSize[0], symetricSize[1], contentMask, capsule);
    if (body.contents & tw.contents) {
        if (SamePoint(start_l, end_l)) {
            TestBoxInBox(tw, body);
        } else {
            TraceThroughBox(tw, body);
        }
    }

    // the end position is recalculated in world space
    tw.trace.endpos[0] = start[0] + tw.trace.fraction * (end[0] - start[0]);
    tw.trace.endpos[1] = start[1] + tw.trace.fraction * (end[1] - start[1]);
    tw.trace.endpos[2] = start[2] + tw.trace.fraction * (end[2] - start[2]);

    *results = tw.trace;
}

bool PointInBox(const vec3_t p, const vec3_t mins, const vec3_t maxs)
{
    return p[0] >= mins[0] && p[0] <= maxs[0] && p[1] >= mins[1] && p[1] <= maxs[1] && p[2] >= mins[2]
        && p[2] <= maxs[2];
}

} // namespace

const PmWorld *PmWorld::s_bound = NULL;

PmWorld::PmWorld() {}

int PmWorld::AddBox(const float mins[3], const float maxs[3], int contents)
{
    Box box;

    if (mins[0] > maxs[0] || mins[1] > maxs[1] || mins[2] > maxs[2]) {
        Com_Error(
            ERR_DROP,
            "PmWorld::AddBox: inverted box (%g %g %g) (%g %g %g)",
            mins[0],
            mins[1],
            mins[2],
            maxs[0],
            maxs[1],
            maxs[2]
        );
    }

    memset(&box, 0, sizeof(box));
    VectorCopy(mins, box.mins);
    VectorCopy(maxs, box.maxs);
    box.contents  = contents;
    box.entityNum = ENTITYNUM_WORLD;
    SetBoxPlanes(box);

    m_boxes.push_back(box);
    return (int)m_boxes.size() - 1;
}

int PmWorld::AddRoom(const float mins[3], const float maxs[3], float thickness, int contents)
{
    const float t = thickness;
    int         first;

    // floor and ceiling cover the walls' footprint
    const float floorMins[3] = {mins[0] - t, mins[1] - t, mins[2] - t};
    const float floorMaxs[3] = {maxs[0] + t, maxs[1] + t, mins[2]};
    const float ceilMins[3]  = {mins[0] - t, mins[1] - t, maxs[2]};
    const float ceilMaxs[3]  = {maxs[0] + t, maxs[1] + t, maxs[2] + t};
    // walls along y (at -x and +x), then along x (at -y and +y)
    const float westMins[3]  = {mins[0] - t, mins[1] - t, mins[2]};
    const float westMaxs[3]  = {mins[0], maxs[1] + t, maxs[2]};
    const float eastMins[3]  = {maxs[0], mins[1] - t, mins[2]};
    const float eastMaxs[3]  = {maxs[0] + t, maxs[1] + t, maxs[2]};
    const float southMins[3] = {mins[0], mins[1] - t, mins[2]};
    const float southMaxs[3] = {maxs[0], mins[1], maxs[2]};
    const float northMins[3] = {mins[0], maxs[1], mins[2]};
    const float northMaxs[3] = {maxs[0], maxs[1] + t, maxs[2]};

    first = AddBox(floorMins, floorMaxs, contents);
    AddBox(ceilMins, ceilMaxs, contents);
    AddBox(westMins, westMaxs, contents);
    AddBox(eastMins, eastMaxs, contents);
    AddBox(southMins, southMaxs, contents);
    AddBox(northMins, northMaxs, contents);
    return first;
}

int PmWorld::AddPillar(float centerX, float centerY, float halfWidth, float bottomZ, float topZ, int contents)
{
    const float mins[3] = {centerX - halfWidth, centerY - halfWidth, bottomZ};
    const float maxs[3] = {centerX + halfWidth, centerY + halfWidth, topZ};

    return AddBox(mins, maxs, contents);
}

void PmWorld::ClearBoxes()
{
    m_boxes.clear();
}

int PmWorld::NumBoxes() const
{
    return (int)m_boxes.size();
}

const PmWorld::Box& PmWorld::GetBox(int index) const
{
    if (index < 0 || index >= (int)m_boxes.size()) {
        Com_Error(ERR_DROP, "PmWorld::GetBox: bad index %d", index);
    }
    return m_boxes[index];
}

void PmWorld::SetBody(int entityNum, const float origin[3], const float mins[3], const float maxs[3])
{
    Box   *body = NULL;
    size_t i;

    if (entityNum < 0 || entityNum >= ENTITYNUM_MAX_NORMAL) {
        Com_Error(ERR_DROP, "PmWorld::SetBody: bad entity number %d", entityNum);
    }

    for (i = 0; i < m_bodies.size(); i++) {
        if (m_bodies[i].entityNum == entityNum) {
            body = &m_bodies[i];
            break;
        }
    }

    if (!body) {
        m_bodies.push_back(Box());
        body = &m_bodies.back();
    }

    memset(body, 0, sizeof(*body));
    VectorCopy(origin, body->origin);
    VectorCopy(mins, body->mins);
    VectorCopy(maxs, body->maxs);
    body->contents  = CONTENTS_BODY;
    body->entityNum = entityNum;
    SetBoxPlanes(*body);
}

void PmWorld::ClearBody(int entityNum)
{
    size_t i;

    for (i = 0; i < m_bodies.size(); i++) {
        if (m_bodies[i].entityNum == entityNum) {
            m_bodies.erase(m_bodies.begin() + i);
            return;
        }
    }
}

void PmWorld::ClearBodies()
{
    m_bodies.clear();
}

bool PmWorld::HasBody(int entityNum) const
{
    return GetBody(entityNum) != NULL;
}

const PmWorld::Box *PmWorld::GetBody(int entityNum) const
{
    size_t i;

    for (i = 0; i < m_bodies.size(); i++) {
        if (m_bodies[i].entityNum == entityNum) {
            return &m_bodies[i];
        }
    }
    return NULL;
}

void PmWorld::Trace(
    trace_t     *results,
    const vec3_t start,
    const vec3_t mins,
    const vec3_t maxs,
    const vec3_t end,
    int          passEntityNum,
    int          contentMask,
    bool         capsule
) const
{
    TraceWork tw;
    trace_t   clip;
    trace_t   trace;
    size_t    i;
    int       j;

    //
    // clip to world (CM_BoxTrace with model 0)
    //
    InitTraceWork(tw, start, end, mins, maxs, contentMask, capsule);

    if (SamePoint(start, end)) {
        // position test special case
        for (i = 0; i < m_boxes.size(); i++) {
            if (!(m_boxes[i].contents & tw.contents)) {
                continue;
            }
            TestBoxInBox(tw, m_boxes[i]);
            if (tw.trace.allsolid) {
                break;
            }
        }
    } else {
        for (i = 0; i < m_boxes.size(); i++) {
            if (!(m_boxes[i].contents & tw.contents)) {
                continue;
            }
            TraceThroughBox(tw, m_boxes[i]);
            if (!tw.trace.fraction) {
                break;
            }
        }
    }

    // generate endpos from the original, unmodified start/end
    if (tw.trace.fraction == 1) {
        VectorCopy(end, tw.trace.endpos);
    } else {
        for (j = 0; j < 3; j++) {
            tw.trace.endpos[j] = start[j] + tw.trace.fraction * (end[j] - start[j]);
        }
    }

    //
    // SV_Trace
    //
    clip           = tw.trace;
    clip.entityNum = clip.fraction != 1.0 ? ENTITYNUM_WORLD : ENTITYNUM_NONE;
    if (clip.fraction == 0) {
        *results = clip;
        return; // blocked immediately by the world
    }

    //
    // clip to other solid entities (SV_ClipMoveToEntities)
    //
    for (i = 0; i < m_bodies.size(); i++) {
        const Box& body = m_bodies[i];

        if (clip.allsolid) {
            break;
        }
        if (body.entityNum == passEntityNum) {
            continue; // don't clip against the pass entity
        }
        if (!(contentMask & body.contents)) {
            continue;
        }

        TraceBody(&trace, start, end, mins, maxs, body, contentMask, capsule);

        if (trace.allsolid) {
            clip.allsolid   = qtrue;
            clip.entityNum  = body.entityNum;
            trace.entityNum = body.entityNum;
        } else if (trace.startsolid) {
            clip.startsolid = qtrue;
            clip.entityNum  = body.entityNum;
            trace.entityNum = body.entityNum;
        }

        if (trace.fraction < clip.fraction) {
            // make sure we keep a startsolid from a previous trace
            const qboolean oldStart = clip.startsolid;

            trace.entityNum = body.entityNum;
            clip            = trace;
            clip.startsolid |= oldStart;
        }
    }

    *results = clip;
}

int PmWorld::PointContents(const vec3_t point, int passEntityNum) const
{
    int    contents = 0;
    size_t i;

    for (i = 0; i < m_boxes.size(); i++) {
        if (PointInBox(point, m_boxes[i].mins, m_boxes[i].maxs)) {
            contents |= m_boxes[i].contents;
        }
    }

    for (i = 0; i < m_bodies.size(); i++) {
        const Box& body = m_bodies[i];
        vec3_t     local;

        if (body.entityNum == passEntityNum) {
            continue;
        }
        VectorSubtract(point, body.origin, local);
        if (PointInBox(local, body.mins, body.maxs)) {
            contents |= body.contents;
        }
    }

    return contents;
}

bool PmWorld::LineOfSight(const vec3_t start, const vec3_t end, int ignoreEntity, int targetEntity, int contentMask)
    const
{
    trace_t trace;

    Trace(&trace, start, vec3_origin, vec3_origin, end, ignoreEntity, contentMask, false);

    if (trace.fraction >= 0.999f) {
        return true;
    }
    return targetEntity != ENTITYNUM_NONE && trace.entityNum == targetEntity;
}

void PmWorld::PmoveTrace(
    trace_t     *results,
    const vec3_t start,
    const vec3_t mins,
    const vec3_t maxs,
    const vec3_t end,
    int          passEntityNum,
    int          contentMask,
    int          capsule,
    qboolean     traceDeep
)
{
    // traceDeep only adds hit locations on character models
    (void)traceDeep;

    if (!s_bound) {
        Com_Error(ERR_FATAL, "PmWorld::PmoveTrace: no world bound (use PmWorldScope)");
    }
    s_bound->Trace(results, start, mins, maxs, end, passEntityNum, contentMask, capsule != 0);
}

int PmWorld::PmovePointContents(const vec3_t point, int passEntityNum)
{
    if (!s_bound) {
        Com_Error(ERR_FATAL, "PmWorld::PmovePointContents: no world bound (use PmWorldScope)");
    }
    return s_bound->PointContents(point, passEntityNum);
}

const PmWorld *PmWorld::Bound()
{
    return s_bound;
}

PmWorldScope::PmWorldScope(const PmWorld& world)
    : m_previous(PmWorld::s_bound)
{
    PmWorld::s_bound = &world;
}

PmWorldScope::~PmWorldScope()
{
    PmWorld::s_bound = m_previous;
}
