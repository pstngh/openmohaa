/*
===========================================================================
Copyright (C) 2025 the OpenMoHAA team

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

/**
 * @file navigation_recast_path.cpp
 * @brief Modern navigation system using Recast and Detour
 *
 * Detour is used for path-finding using a mesh that was automatically generated when parsing the BSP file.
 */

#include "navigation_recast_path.h"
#include "navigation_recast_load.h"
#include "navigation_recast_helpers.h"
#include "level.h"

#include "DetourPathCorridor.h"
#include "DetourCommon.h"
#include "entity.h"
#include "bg_local.h"

#define MAX_NPOLYS 256

static const float RECAST_COMFORT_ENTER_CLEARANCE  = 12.0f;
static const float RECAST_COMFORT_EXIT_CLEARANCE   = 18.0f;
static const float RECAST_COMFORT_TARGET_CLEARANCE = 24.0f;
static const float RECAST_COMFORT_LOOKAHEAD        = 96.0f;
static const float RECAST_COMFORT_MIN_FORWARD      = 32.0f;
static const int   RECAST_COMFORT_MAX_VISITED      = 16;

RecastPathMaster pathMaster;

static const vec3_t DETOUR_EXTENT = {(MAXS_X - MINS_X) / 2, (MAXS_Z - MINS_Z) / 2, (MAXS_Y - MINS_Y) / 2};

struct DetourData {
public:
    dtPathCorridor corridor;
    vec3_t         corners[4];
    unsigned char  cornerFlags[4];
    dtPolyRef      cornerPolys[4];
    int            ncorners;
};

RecastPather::RecastPather()
    : detourData(NULL)
    , moving(false)
    , lastCheckTime(0)
    , traversingOffMeshLink(false)
    , comfortInsetActive(false)
    , comfortInsetEnabled(false)
{
    detourData = new DetourData();
    detourData->corridor.init(256);
    comfortInsetNormal = vec_zero;
}

RecastPather::~RecastPather()
{
    if (detourData) {
        delete detourData;
        detourData = NULL;
    }
}

void RecastPather::FindPath(const Vector& start, const Vector& end, const PathSearchParameter& parameters)
{
    Vector               recastStart, recastEnd;
    dtPolyRef            startRef, endRef;
    vec3_t               startPt, endPt;
    const dtQueryFilter *filter = navigationMap.GetQueryFilter();

    lastorg = start;
    ResetPosition(start);

    ConvertGameToRecastCoord(start, recastStart);
    ConvertGameToRecastCoord(end, recastEnd);

    endRef   = 0;
    startRef = detourData->corridor.getFirstPoly();
    dtVcopy(startPt, detourData->corridor.getPos());
    navigationMap.GetNavMeshQuery()->findNearestPoly(recastEnd, DETOUR_EXTENT, filter, &endRef, endPt);

    if (!startRef || !endRef) {
        return;
    }

    dtPolyRef polys[MAX_NPOLYS];
    int       nPolys = 0;
    navigationMap.GetNavMeshQuery()->findPath(startRef, endRef, startPt, endPt, filter, polys, &nPolys, ARRAY_LEN(polys) - 1);

    if (nPolys) {
        vec3_t   closestPos;
        dtStatus status;

        if (polys[nPolys - 1] != endRef) {
            status = navigationMap.GetNavMeshQuery()->closestPointOnPoly(polys[nPolys - 1], endPt, closestPos, 0);
            if (dtStatusFailed(status)) {
                VectorCopy(endPt, closestPos);
            }
        } else {
            VectorCopy(endPt, closestPos);
        }

        moving = true;
        detourData->corridor.setCorridor(closestPos, polys, nPolys);
    }
}

void RecastPather::FindPathNear(
    const Vector& start, const Vector& end, float radius, const PathSearchParameter& parameters
)
{
    Vector               recastStart, recastEnd;
    dtPolyRef            startRef, endRef;
    vec3_t               endExtent;
    vec3_t               startPt, endPt;
    const dtQueryFilter *filter = navigationMap.GetQueryFilter();

    lastorg = start;
    ResetPosition(start);

    ConvertGameToRecastCoord(start, recastStart);
    ConvertGameToRecastCoord(end, recastEnd);

    endRef   = 0;
    startRef = detourData->corridor.getFirstPoly();
    dtVcopy(startPt, detourData->corridor.getPos());
    VectorCopy(DETOUR_EXTENT, endExtent);
    endExtent[0] = Q_max(endExtent[0], radius);
    endExtent[1] = Q_max(endExtent[1], static_cast<float>(MAXS_Z));
    endExtent[2] = Q_max(endExtent[2], radius);
    navigationMap.GetNavMeshQuery()->findNearestPoly(recastEnd, endExtent, filter, &endRef, endPt);

    if (!startRef || !endRef || dtVdist2DSqr(recastEnd, endPt) > Square(radius)) {
        return;
    }

    dtPolyRef polys[MAX_NPOLYS];
    int       nPolys = 0;
    navigationMap.GetNavMeshQuery()->findPath(startRef, endRef, startPt, endPt, filter, polys, &nPolys, ARRAY_LEN(polys) - 1);

    if (nPolys) {
        vec3_t   closestPos;
        dtStatus status;

        if (polys[nPolys - 1] != endRef) {
            status = navigationMap.GetNavMeshQuery()->closestPointOnPoly(polys[nPolys - 1], endPt, closestPos, 0);
            if (dtStatusFailed(status)) {
                VectorCopy(endPt, closestPos);
            }
        } else {
            VectorCopy(endPt, closestPos);
        }

        moving = true;
        detourData->corridor.setCorridor(closestPos, polys, nPolys);
    }
}

void RecastPather::FindPathAway(
    const Vector&              start,
    const Vector&              avoid,
    const Vector&              preferredDir,
    float                      radius,
    const PathSearchParameter& parameters
)
{
    Vector               recastStart, recastAvoid, recastEnd;
    Vector               dirNormalized;
    dtPolyRef            startRef, endRef;
    vec3_t               startPt, endPt;
    const dtQueryFilter *filter = navigationMap.GetQueryFilter();
    float                startAngle;
    float                startPitch;
    int                  i, j;

    lastorg = start;
    ResetPosition(start);

    startAngle = DEG2RAD(preferredDir.toYaw());
    startPitch = DEG2RAD(preferredDir.toPitch());

    dirNormalized = preferredDir;
    dirNormalized.normalize();

    ConvertGameToRecastCoord(start, recastStart);
    ConvertGameToRecastCoord(avoid, recastAvoid);
    ConvertGameToRecastCoord(avoid + dirNormalized * radius, recastEnd);

    startRef = detourData->corridor.getFirstPoly();
    dtVcopy(startPt, detourData->corridor.getPos());

    if (!startRef) {
        return;
    }

    for (i = 0; i < 36; i++) {
        float angle = startAngle + ((2 * M_PI) * (float)i / (float)36);
        float dx    = cos(angle) * radius;
        float dz    = sin(angle) * radius;

        for (j = 0; j < 4; j++) {
            float  pitch = startPitch + (M_PI * (float)j / 4);
            float  dy    = sin(pitch) * radius;
            Vector point(recastAvoid[0] + dx, recastAvoid[1] + dy, recastAvoid[2] + dz);

            if (navigationMap.GetNavMeshQuery()->findNearestPoly(point, DETOUR_EXTENT, filter, &endRef, endPt)
                    == DT_SUCCESS
                && endRef) {
                dtPolyRef polys[MAX_NPOLYS];
                int       nPolys = 0;
                navigationMap.GetNavMeshQuery()->findPath(
                    startRef, endRef, startPt, endPt, filter, polys, &nPolys, ARRAY_LEN(polys) - 1
                );

                if (nPolys) {
                    vec3_t   closestPos;
                    dtStatus status;

                    if (polys[nPolys - 1] != endRef) {
                        status = navigationMap.GetNavMeshQuery()->closestPointOnPoly(
                            polys[nPolys - 1], endPt, closestPos, 0
                        );
                        if (dtStatusFailed(status)) {
                            VectorCopy(endPt, closestPos);
                        }
                    } else {
                        VectorCopy(endPt, closestPos);
                    }

                    moving = true;
                    detourData->corridor.setCorridor(closestPos, polys, nPolys);
                }

                return;
            }
        }
    }
}

bool RecastPather::TestPath(const Vector& start, const Vector& end, const PathSearchParameter& parameters)
{
    Vector               recastStart, recastEnd;
    const dtQueryFilter *filter = navigationMap.GetQueryFilter();
    dtStatus             status;

    ConvertGameToRecastCoord(start, recastStart);
    ConvertGameToRecastCoord(end, recastEnd);

    dtPolyRef nearestStartRef, nearestEndRef;
    vec3_t    nearestStartPt, nearestEndPt;
    navigationMap.GetNavMeshQuery()->findNearestPoly(
        recastStart, DETOUR_EXTENT, filter, &nearestStartRef, nearestStartPt
    );
    navigationMap.GetNavMeshQuery()->findNearestPoly(recastEnd, DETOUR_EXTENT, filter, &nearestEndRef, nearestEndPt);

    dtPolyRef polys[MAX_NPOLYS];
    int       nPolys;
    status = navigationMap.GetNavMeshQuery()->findPath(
        nearestStartRef, nearestEndRef, nearestStartPt, nearestEndPt, filter, polys, &nPolys, 256
    );

    if (!(status & DT_SUCCESS)) {
        // Invalid path
        return false;
    }

    return true;
}

static bool overOffmeshConnection(
    const vec3_t pos, const unsigned char *cornerFlags, const vec3_t cornerVerts, const float radius, const int ncorners
)
{
    if (!ncorners) {
        return false;
    }

    const bool offMeshConnection = (cornerFlags[ncorners - 1] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;
    if (offMeshConnection) {
        const float distSq = dtVdist2DSqr(&cornerVerts[(ncorners - 1) * 3], pos);
        if (distSq < radius * radius) {
            return true;
        }
    }

    return false;
}

bool RecastPather::BuildComfortInsetCorner(float *corner)
{
    dtNavMesh            *navMesh  = navigationMap.GetNavMesh();
    dtNavMeshQuery       *navQuery = navigationMap.GetNavMeshQuery();
    const dtQueryFilter  *filter   = navigationMap.GetQueryFilter();
    const dtPathCorridor& corridor = detourData->corridor;
    const dtPolyRef       startRef = corridor.getFirstPoly();
    const float          *startPos = corridor.getPos();

    if (!comfortInsetEnabled || !moving || !navMesh || !navQuery || !startRef
        || traversingOffMeshLink) {
        comfortInsetActive = false;
        return false;
    }

    for (int i = 0; i < detourData->ncorners; ++i) {
        if (detourData->cornerFlags[i] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) {
            // Ladder and jump links own their exact approach and departure.
            comfortInsetActive = false;
            return false;
        }
    }

    float routeDelta[3] = {
        corner[0] - startPos[0],
        0.0f,
        corner[2] - startPos[2]
    };
    const float routeDistance = sqrtf(
        routeDelta[0] * routeDelta[0] + routeDelta[2] * routeDelta[2]
    );
    if (routeDistance < RECAST_COMFORT_MIN_FORWARD
        || ((detourData->cornerFlags[0] & DT_STRAIGHTPATH_END)
            && routeDistance
                <= RECAST_COMFORT_LOOKAHEAD + RECAST_COMFORT_MIN_FORWARD)) {
        // Do not replace a precise short-range goal with a lateral waypoint.
        comfortInsetActive = false;
        return false;
    }

    float wallDistance = RECAST_COMFORT_TARGET_CLEARANCE;
    vec3_t wallPos;
    vec3_t wallNormal;
    const dtStatus wallStatus = navQuery->findDistanceToWall(
        startRef,
        startPos,
        RECAST_COMFORT_TARGET_CLEARANCE,
        filter,
        &wallDistance,
        wallPos,
        wallNormal
    );
    if (dtStatusFailed(wallStatus)
        || dtStatusDetail(wallStatus, DT_OUT_OF_NODES)
        || (!comfortInsetActive
            && wallDistance >= RECAST_COMFORT_ENTER_CLEARANCE)
        || (comfortInsetActive
            && wallDistance >= RECAST_COMFORT_EXIT_CLEARANCE)) {
        comfortInsetActive = false;
        return false;
    }

    wallNormal[1] = 0.0f;
    float normalLength = sqrtf(
        wallNormal[0] * wallNormal[0] + wallNormal[2] * wallNormal[2]
    );
    if (normalLength <= 0.001f) {
        // Detour documents the wall normal as undefined at zero distance.
        // The current polygon centroid supplies a deterministic fallback
        // direction toward walkable space.
        const dtMeshTile *tile = NULL;
        const dtPoly     *poly = NULL;
        if (dtStatusFailed(
                navMesh->getTileAndPolyByRef(
                    startRef, &tile, &poly
                )
            )
            || !tile || !poly || !poly->vertCount) {
            comfortInsetActive = false;
            return false;
        }

        wallNormal[0] = 0.0f;
        wallNormal[2] = 0.0f;
        for (int i = 0; i < poly->vertCount; ++i) {
            const float *vertex = &tile->verts[poly->verts[i] * 3];
            wallNormal[0] += vertex[0];
            wallNormal[2] += vertex[2];
        }
        wallNormal[0] = wallNormal[0] / poly->vertCount - startPos[0];
        wallNormal[2] = wallNormal[2] / poly->vertCount - startPos[2];
        normalLength = sqrtf(
            wallNormal[0] * wallNormal[0]
                + wallNormal[2] * wallNormal[2]
        );
        if (normalLength <= 0.001f) {
            comfortInsetActive = false;
            return false;
        }
    }

    wallNormal[0] /= normalLength;
    wallNormal[2] /= normalLength;

    const float savedNormalLength = sqrtf(
        comfortInsetNormal[0] * comfortInsetNormal[0]
            + comfortInsetNormal[2] * comfortInsetNormal[2]
    );
    if (comfortInsetActive && savedNormalLength > 0.001f
        && wallNormal[0] * comfortInsetNormal[0]
                + wallNormal[2] * comfortInsetNormal[2]
            < 0.5f) {
        wallNormal[0] = comfortInsetNormal[0] / savedNormalLength;
        wallNormal[2] = comfortInsetNormal[2] / savedNormalLength;
    } else {
        comfortInsetNormal = Vector(wallNormal[0], 0.0f, wallNormal[2]);
    }

    routeDelta[0] /= routeDistance;
    routeDelta[2] /= routeDistance;
    const float forwardDistance = Q_min(
        RECAST_COMFORT_LOOKAHEAD, routeDistance
    );
    const float lateralDistance = Q_max(
        0.0f, RECAST_COMFORT_TARGET_CLEARANCE - wallDistance
    );
    const float verticalFraction = forwardDistance / routeDistance;
    vec3_t desired = {
        startPos[0] + routeDelta[0] * forwardDistance
            + wallNormal[0] * lateralDistance,
        startPos[1] + (corner[1] - startPos[1]) * verticalFraction,
        startPos[2] + routeDelta[2] * forwardDistance
            + wallNormal[2] * lateralDistance
    };

    vec3_t result;
    dtPolyRef visited[RECAST_COMFORT_MAX_VISITED];
    int visitedCount = 0;
    const dtStatus moveStatus = navQuery->moveAlongSurface(
        startRef, startPos, desired, filter, result,
        visited, &visitedCount, ARRAY_LEN(visited)
    );
    if (dtStatusFailed(moveStatus)
        || dtStatusDetail(moveStatus, DT_BUFFER_TOO_SMALL) || !visitedCount
        || dtVdist2D(result, desired) > 1.0f) {
        comfortInsetActive = false;
        return false;
    }

    const dtPolyRef *path = corridor.getPath();
    const int pathCount = corridor.getPathCount();
    for (int i = 0; i < visitedCount; ++i) {
        bool inCorridor = false;
        for (int j = 0; j < pathCount; ++j) {
            if (visited[i] == path[j]) {
                inCorridor = true;
                break;
            }
        }
        if (!inCorridor) {
            comfortInsetActive = false;
            return false;
        }
    }

    float candidateClearance = RECAST_COMFORT_EXIT_CLEARANCE;
    vec3_t candidateWallPos;
    vec3_t candidateWallNormal;
    const dtStatus candidateStatus = navQuery->findDistanceToWall(
        visited[visitedCount - 1], result,
        RECAST_COMFORT_EXIT_CLEARANCE, filter,
        &candidateClearance, candidateWallPos, candidateWallNormal
    );
    if (dtStatusFailed(candidateStatus)
        || dtStatusDetail(candidateStatus, DT_OUT_OF_NODES)
        || candidateClearance < RECAST_COMFORT_EXIT_CLEARANCE - 0.1f) {
        // Width gate: tight doors and passages retain Detour's original
        // first corner instead of losing traversability.
        comfortInsetActive = false;
        return false;
    }

    dtVcopy(corner, result);
    comfortInsetActive = true;
    return true;
}

void RecastPather::UpdatePos(const Vector& origin)
{
    const dtQueryFilter *filter = navigationMap.GetQueryFilter();
    const Vector velocity = (origin - lastorg) * (1.0 / level.frametime);
    const float distSqr = Q_min(Square(64), velocity.lengthSquared());

    lastorg = origin;
    Vector recastOrigin;

    ConvertGameToRecastCoord(origin, recastOrigin);

    if (traversingOffMeshLink) {
        Vector agentPos;
        Vector delta;

        ConvertRecastToGameCoord(detourData->corridor.getPos(), agentPos);
        delta = agentPos - origin;
        if (delta.lengthSquared() < Square(24) + distSqr) {
            // traversed
            traversingOffMeshLink = false;
        }
    } else if (level.inttime >= lastCheckTime + 2000) {
        vec3_t delta;
        VectorSubtract(recastOrigin, detourData->corridor.getPos(), delta);

        if (VectorLengthSquared(delta) > Square(64)) {
            dtPolyRef startRef, endRef;
            vec3_t    startPt, endPt;

            //
            // Get the target position
            //
            endRef = detourData->corridor.getLastPoly();
            VectorCopy(detourData->corridor.getTarget(), endPt);

            ResetPosition(origin);

            startRef = 0;
            navigationMap.GetNavMeshQuery()->findNearestPoly(recastOrigin, DETOUR_EXTENT, filter, &startRef, startPt);

            if (startRef) {
                dtPolyRef polys[MAX_NPOLYS];
                int       nPolys = 0;
                navigationMap.GetNavMeshQuery()->findPath(
                    startRef, endRef, startPt, endPt, filter, polys, &nPolys, ARRAY_LEN(polys) - 1
                );

                if (nPolys) {
                    moving = true;
                    detourData->corridor.setCorridor(endPt, polys, nPolys);
                }
            }
        }

        lastCheckTime = level.inttime;
    } else {
        detourData->corridor.movePosition(recastOrigin, navigationMap.GetNavMeshQuery(), filter);
        ConvertRecastToGameCoord(detourData->corridor.getPos(), lastValidOrg);

        detourData->ncorners = detourData->corridor.findCorners(
            (float *)detourData->corners,
            detourData->cornerFlags,
            detourData->cornerPolys,
            4,
            navigationMap.GetNavMeshQuery(),
            filter
        );
        if (detourData->ncorners) {
            dtPolyRef refs[2];
            vec3_t    startOffPos, endOffPos;

            if (overOffmeshConnection(
                    recastOrigin, detourData->cornerFlags, (const vec_t *)detourData->corners, 16, detourData->ncorners
                )
                && detourData->corridor.moveOverOffmeshConnection(
                    detourData->cornerPolys[detourData->ncorners - 1],
                    refs,
                    startOffPos,
                    endOffPos,
                    navigationMap.GetNavMeshQuery()
                )) {
                ConvertRecastToGameCoord(detourData->corridor.getPos(), currentNodePos);

                traversingOffMeshLink = true;
            } else {
                vec3_t steeringCorner;
                VectorCopy(detourData->corners[0], steeringCorner);
                BuildComfortInsetCorner(steeringCorner);
                ConvertRecastToGameCoord(steeringCorner, currentNodePos);
            }
        } else {
            ConvertRecastToGameCoord(detourData->corridor.getPos(), currentNodePos);
        }
    }
}

void RecastPather::SetRouteComfortInsetEnabled(bool enabled)
{
    comfortInsetEnabled = enabled;
    if (!enabled) {
        comfortInsetActive = false;
        comfortInsetNormal = vec_zero;
    }
}

void RecastPather::Clear()
{
    ResetPosition(lastorg);
}

PathNav RecastPather::GetNode(unsigned int index) const
{
    PathNav               nav;
    Vector                target;
    Vector                delta;
    Vector                agentPos;
    const dtPathCorridor *inCorridor;
    const dtPolyRef      *path;
    int                   npath;

    inCorridor = &detourData->corridor;

    path  = inCorridor->getPath();
    npath = inCorridor->getPathCount();
    if (npath <= 0) {
        return {};
    }

    if (index + 1 == npath) {
        // Just return the target pos
        ConvertRecastToGameCoord(inCorridor->getTarget(), nav.origin);
        //ConvertRecastToGameCoord(agent->npos, agentPos);
        agentPos = lastorg;

        nav.dir[0] = nav.origin[0] - agentPos[0];
        nav.dir[1] = nav.origin[1] - agentPos[1];
        nav.dist   = VectorLength2D(nav.dir);
        VectorNormalize2D(nav.dir);

        return nav;
    }

    const dtMeshTile *tile;
    const dtPoly     *poly;
    if (navigationMap.GetNavMesh()->getTileAndPolyByRef(path[index], &tile, &poly) != DT_SUCCESS) {
        return {};
    }

    const unsigned int tileId = (unsigned int)(poly - tile->polys);

    if (poly->getType() == DT_POLYTYPE_OFFMESH_CONNECTION) {
        dtOffMeshConnection *con = &tile->offMeshCons[tileId - tile->header->offMeshBase];
        Vector               start, end;

        ConvertRecastToGameCoord(&con->pos[0], start);
        ConvertRecastToGameCoord(&con->pos[3], end);

        nav.origin = start;
        nav.dir[0] = end[0] - start[0];
        nav.dir[1] = end[1] - start[1];
        VectorNormalize2D(nav.dir);
        nav.dist = delta.length();
    } else {
        const dtPolyDetail *dm = &tile->detailMeshes[tileId];
        Vector              middle[1];

        for (int i = 0; i < 1; i++) {
            for (int j = 0; j < dm->triCount; ++j) {
                const unsigned char *t = &tile->detailTris[(dm->triBase + j) * 4];
                for (int k = 0; k < 3; ++k) {
                    if (t[k] < poly->vertCount) {
                        middle[i] += &tile->verts[poly->verts[t[k]] * 3];
                    } else {
                        middle[i] += &tile->detailVerts[(dm->vertBase + t[k] - poly->vertCount) * 3];
                    }
                }
            }

            middle[i] /= dm->triCount * 3;
        }

        ConvertRecastToGameCoord(middle[0], nav.origin);
        ConvertRecastToGameCoord(middle[1], target);

        delta      = target - nav.origin;
        nav.dir[0] = delta[0];
        nav.dir[1] = delta[1];
        VectorNormalize2D(nav.dir);
        nav.dist = delta.length();
    }

    return nav;
}

int RecastPather::GetNodeCount() const
{
    if (!moving) {
        return 0;
    }

    return detourData->corridor.getPathCount();
}

Vector RecastPather::GetCurrentDelta() const
{
    Vector delta;

    delta = currentNodePos - lastorg;

    return delta;
}

Vector RecastPather::GetCurrentDirection() const
{
    Vector delta;

    delta = currentNodePos - lastorg;
    delta.normalize();

    return delta;
}

Vector RecastPather::GetDestination() const
{
    Vector dest;

    ConvertRecastToGameCoord(detourData->corridor.getTarget(), dest);

    return dest;
}

bool RecastPather::HasReachedGoal(const Vector& origin) const
{
    Vector target;

    const dtPolyRef lastPoly = detourData->corridor.getLastPoly();
    ConvertRecastToGameCoord(detourData->corridor.getTarget(), target);

    if (fabs(origin[0] - target[0]) < 16.0f && fabs(origin[1] - target[1]) < 16.0f) {
        return true;
    }

    return false;
}

bool RecastPather::IsQuerying() const
{
    return false;
}

void RecastPather::ResetPosition(const Vector& origin)
{
    const dtQueryFilter *filter = navigationMap.GetQueryFilter();
    vec3_t               agentPos;

    traversingOffMeshLink = false;
    comfortInsetActive    = false;
    comfortInsetNormal    = vec_zero;
    lastCheckTime         = level.inttime;

    moving = false;

    ConvertGameToRecastCoord(origin, agentPos);

    // Find nearest position on navmesh and place the agent there.
    dtPolyRef ref = 0;
    float     nearest[3];
    dtStatus  status;

    status = navigationMap.GetNavMeshQuery()->findNearestPoly(agentPos, DETOUR_EXTENT, filter, &ref, nearest);

    if (dtStatusFailed(status) || !ref) {
        // Use the last valid position instead
        ConvertGameToRecastCoord(lastValidOrg, agentPos);
        status = navigationMap.GetNavMeshQuery()->findNearestPoly(agentPos, DETOUR_EXTENT, filter, &ref, nearest);
    }

    if (dtStatusSucceed(status) && ref) {
        detourData->corridor.reset(ref, nearest);
        ConvertRecastToGameCoord(agentPos, lastValidOrg);
    } else {
        detourData->corridor.reset(0, agentPos);
    }
}

void RecastPathMaster::PostLoadNavigation(const NavigationMap& map) {}

void RecastPathMaster::ClearNavigation() {}

void RecastPathMaster::Update() {}
