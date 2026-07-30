/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

Human-demo route selection. The graph provides strategic waypoints; the
existing navigation backend still owns the path between them.
===========================================================================
*/

#include "g_local.h"
#include "movement_telemetry.h"
#include "playerbot.h"
#include "playerbot_route.h"

static const unsigned int BOT_DEMO_ROUTE_MAX_NODES = 1024;
static const unsigned int BOT_DEMO_ROUTE_MIN_SMG_SUPPORT = 3;
static const unsigned int BOT_DEMO_ROUTE_MIN_NORMAL_SUPPORT = 5;
static const float        BOT_DEMO_ROUTE_DETOUR_ALLOWANCE = 768.0f;
static const float        BOT_DEMO_ROUTE_INFINITY = 1.0e30f;
static const float BOT_OBJECTIVE_DEMO_ROUTE_SEARCH_RADIUS  = 1024.0f;
static const float BOT_OBJECTIVE_DEMO_ROUTE_FIT_RADIUS     = 384.0f;
static const float BOT_OBJECTIVE_DEMO_ROUTE_REACHED_RADIUS = 192.0f;
static const float BOT_OBJECTIVE_DEMO_ROUTE_REACHED_HEIGHT = 96.0f;
static const float BOT_OBJECTIVE_DEMO_ROUTE_ROAM_DISTANCE  = 1024.0f;
static const float BOT_OBJECTIVE_DEMO_ROUTE_COVER_RADIUS   = 2048.0f;
static const int   BOT_OBJECTIVE_DEMO_ROUTE_RETRY_MSEC     = 2000;

enum bot_demo_route_support_t {
    BOT_DEMO_ROUTE_GEOMETRY,
    BOT_DEMO_ROUTE_NORMAL,
    BOT_DEMO_ROUTE_SMG
};

static Vector BotDemoRouteNearestPathNode(
    const Vector& desired, float maxDistance
)
{
    PathNode *best = NULL;
    float     bestDistanceSquared = 0.0f;

    for (int i = 0; i < PathSearch::nodecount; ++i) {
        PathNode *node = PathSearch::pathnodes[i];
        if (!node || node->virtualNumChildren <= 0
            || (node->origin - desired).lengthXYSquared()
                > Square(maxDistance)) {
            continue;
        }

        const float distanceSquared =
            (node->origin - desired).lengthSquared();
        if (!best || distanceSquared < bestDistanceSquared) {
            best                = node;
            bestDistanceSquared = distanceSquared;
        }
    }
    return best ? best->origin : desired;
}

static unsigned int BotDemoRouteHash(unsigned int value)
{
    value ^= value >> 16;
    value *= 0x7feb352du;
    value ^= value >> 15;
    value *= 0x846ca68bu;
    value ^= value >> 16;
    return value;
}

static float BotDemoRouteRandomFraction(unsigned int seed)
{
    return static_cast<float>(BotDemoRouteHash(seed) & 0x00ffffffu)
        / static_cast<float>(0x01000000u);
}

static bool BotDemoRouteValidNode(
    const bot_demo_route_graph_t *graph, int node
)
{
    return graph && node >= 0
        && static_cast<unsigned int>(node) < graph->nodeCount;
}

static const bot_demo_route_graph_t *BotDemoRouteFindGraph(
    const char *mapName, bool attacker, bool postPlant
)
{
    if (!mapName) {
        return NULL;
    }

    for (unsigned int i = 0; i < g_botDemoRouteGraphCount; ++i) {
        const bot_demo_route_graph_t& graph = g_botDemoRouteGraphs[i];
        if (graph.attacker == attacker && graph.postPlant == postPlant
            && !Q_stricmp(graph.mapName, mapName)) {
            return &graph;
        }
    }
    return NULL;
}

static Vector BotDemoRouteNodePosition(
    const bot_demo_route_graph_t *graph, int node
)
{
    if (!BotDemoRouteValidNode(graph, node)) {
        return vec_zero;
    }

    const bot_demo_route_node_t& routeNode = graph->nodes[node];
    return Vector(routeNode.x, routeNode.y, routeNode.z);
}

static float BotDemoRouteDistance(
    const bot_demo_route_graph_t *graph, int first, int second
)
{
    return (
        BotDemoRouteNodePosition(graph, first)
        - BotDemoRouteNodePosition(graph, second)
    ).length();
}

static int BotDemoRouteFindNearestNode(
    const bot_demo_route_graph_t *graph,
    const Vector&                 position,
    float                         maxDistance
)
{
    if (!graph || !graph->nodeCount) {
        return -1;
    }

    int   nearest = -1;
    float bestDistanceSquared = 0.0f;
    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        const float distanceSquared =
            (BotDemoRouteNodePosition(graph, i) - position).lengthSquared();
        if ((maxDistance <= 0.0f
             || distanceSquared <= Square(maxDistance))
            && (nearest < 0 || distanceSquared < bestDistanceSquared)) {
            nearest             = static_cast<int>(i);
            bestDistanceSquared = distanceSquared;
        }
    }
    return nearest;
}

static bot_demo_route_support_t BotDemoRouteNodeEndSupport(
    const bot_demo_route_graph_t *graph,
    const Vector&                 origin,
    const Vector&                 center,
    float                         maxCenterDistance,
    float                         minOriginDistance
)
{
    unsigned int smgSupport = 0;
    unsigned int normalSupport = 0;

    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        const Vector position = BotDemoRouteNodePosition(graph, i);
        if (maxCenterDistance > 0.0f
            && (position - center).lengthXYSquared()
                > Square(maxCenterDistance)) {
            continue;
        }
        if (minOriginDistance > 0.0f
            && (position - origin).lengthXYSquared()
                < Square(minOriginDistance)) {
            continue;
        }

        smgSupport += graph->nodes[i].smgEnds;
        normalSupport += graph->nodes[i].normalEnds;
    }

    if (smgSupport >= BOT_DEMO_ROUTE_MIN_SMG_SUPPORT) {
        return BOT_DEMO_ROUTE_SMG;
    }
    if (normalSupport >= BOT_DEMO_ROUTE_MIN_NORMAL_SUPPORT) {
        return BOT_DEMO_ROUTE_NORMAL;
    }
    return BOT_DEMO_ROUTE_GEOMETRY;
}

static unsigned int BotDemoRouteNodeEndWeight(
    const bot_demo_route_node_t& node, bot_demo_route_support_t support
)
{
    if (support == BOT_DEMO_ROUTE_SMG) {
        return node.smgEnds;
    }
    if (support == BOT_DEMO_ROUTE_NORMAL) {
        return node.normalEnds;
    }
    return node.geometryEnds;
}

static int BotDemoRouteChooseGoal(
    const bot_demo_route_graph_t *graph,
    const Vector&                 origin,
    const Vector&                 center,
    float                         maxCenterDistance,
    float                         minOriginDistance,
    unsigned int                  seed
)
{
    if (!graph || !graph->nodeCount) {
        return -1;
    }

    const bot_demo_route_support_t support =
        BotDemoRouteNodeEndSupport(
            graph,
            origin,
            center,
            maxCenterDistance,
            minOriginDistance
        );
    unsigned int totalWeight = 0;

    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        const Vector position = BotDemoRouteNodePosition(graph, i);
        if (maxCenterDistance > 0.0f
            && (position - center).lengthXYSquared()
                > Square(maxCenterDistance)) {
            continue;
        }
        if (minOriginDistance > 0.0f
            && (position - origin).lengthXYSquared()
                < Square(minOriginDistance)) {
            continue;
        }

        totalWeight +=
            BotDemoRouteNodeEndWeight(graph->nodes[i], support);
    }
    if (!totalWeight) {
        return -1;
    }

    float choice = BotDemoRouteRandomFraction(seed) * totalWeight;
    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        const Vector position = BotDemoRouteNodePosition(graph, i);
        if (maxCenterDistance > 0.0f
            && (position - center).lengthXYSquared()
                > Square(maxCenterDistance)) {
            continue;
        }
        if (minOriginDistance > 0.0f
            && (position - origin).lengthXYSquared()
                < Square(minOriginDistance)) {
            continue;
        }

        const unsigned int weight =
            BotDemoRouteNodeEndWeight(graph->nodes[i], support);
        if (choice < weight) {
            return static_cast<int>(i);
        }
        choice -= weight;
    }
    return -1;
}

static void BotDemoRouteDistancesToGoal(
    const bot_demo_route_graph_t *graph, int goalNode, float *distances
)
{
    bool visited[BOT_DEMO_ROUTE_MAX_NODES] = {};
    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        distances[i] = BOT_DEMO_ROUTE_INFINITY;
    }
    distances[goalNode] = 0.0f;

    for (unsigned int iteration = 0;
         iteration < graph->nodeCount;
         ++iteration) {
        int   nearest = -1;
        float nearestDistance = BOT_DEMO_ROUTE_INFINITY;
        for (unsigned int i = 0; i < graph->nodeCount; ++i) {
            if (!visited[i] && distances[i] < nearestDistance) {
                nearest         = static_cast<int>(i);
                nearestDistance = distances[i];
            }
        }
        if (nearest < 0) {
            break;
        }

        visited[nearest] = true;
        for (unsigned int i = 0; i < graph->edgeCount; ++i) {
            const bot_demo_route_edge_t& edge = graph->edges[i];
            if (edge.to != static_cast<unsigned int>(nearest)
                || edge.from >= graph->nodeCount) {
                continue;
            }
            const float candidate =
                nearestDistance
                + BotDemoRouteDistance(
                    graph, static_cast<int>(edge.from), nearest
                );
            if (candidate < distances[edge.from]) {
                distances[edge.from] = candidate;
            }
        }
    }
}

static unsigned int BotDemoRouteEdgeWeight(
    const bot_demo_route_edge_t& edge, bot_demo_route_support_t support
)
{
    if (support == BOT_DEMO_ROUTE_SMG) {
        return edge.smgRoutes;
    }
    if (support == BOT_DEMO_ROUTE_NORMAL) {
        return edge.normalRoutes;
    }
    return edge.geometryRoutes;
}

static int BotDemoRouteChooseNextPass(
    const bot_demo_route_graph_t *graph,
    int                           currentNode,
    int                           previousNode,
    const float                  *distances,
    bool                          allowPrevious,
    unsigned int                  seed
)
{
    float bestCost = BOT_DEMO_ROUTE_INFINITY;
    unsigned int smgSupport = 0;
    unsigned int normalSupport = 0;

    for (unsigned int i = 0; i < graph->edgeCount; ++i) {
        const bot_demo_route_edge_t& edge = graph->edges[i];
        if (edge.from != static_cast<unsigned int>(currentNode)
            || edge.to >= graph->nodeCount
            || (!allowPrevious
                && static_cast<int>(edge.to) == previousNode)
            || distances[edge.to] >= distances[currentNode]
            || distances[edge.to] >= BOT_DEMO_ROUTE_INFINITY) {
            continue;
        }

        const float cost =
            BotDemoRouteDistance(
                graph, currentNode, static_cast<int>(edge.to)
            ) + distances[edge.to];
        bestCost = Q_min(bestCost, cost);
    }
    if (bestCost >= BOT_DEMO_ROUTE_INFINITY) {
        return -1;
    }

    for (unsigned int i = 0; i < graph->edgeCount; ++i) {
        const bot_demo_route_edge_t& edge = graph->edges[i];
        if (edge.from != static_cast<unsigned int>(currentNode)
            || edge.to >= graph->nodeCount
            || (!allowPrevious
                && static_cast<int>(edge.to) == previousNode)
            || distances[edge.to] >= distances[currentNode]
            || distances[edge.to] >= BOT_DEMO_ROUTE_INFINITY) {
            continue;
        }
        const float cost =
            BotDemoRouteDistance(
                graph, currentNode, static_cast<int>(edge.to)
            ) + distances[edge.to];
        if (cost <= bestCost + BOT_DEMO_ROUTE_DETOUR_ALLOWANCE) {
            smgSupport += edge.smgRoutes;
            normalSupport += edge.normalRoutes;
        }
    }

    bot_demo_route_support_t support = BOT_DEMO_ROUTE_GEOMETRY;
    if (smgSupport >= BOT_DEMO_ROUTE_MIN_SMG_SUPPORT) {
        support = BOT_DEMO_ROUTE_SMG;
    } else if (normalSupport >= BOT_DEMO_ROUTE_MIN_NORMAL_SUPPORT) {
        support = BOT_DEMO_ROUTE_NORMAL;
    }

    float totalWeight = 0.0f;
    for (unsigned int i = 0; i < graph->edgeCount; ++i) {
        const bot_demo_route_edge_t& edge = graph->edges[i];
        if (edge.from != static_cast<unsigned int>(currentNode)
            || edge.to >= graph->nodeCount
            || (!allowPrevious
                && static_cast<int>(edge.to) == previousNode)
            || distances[edge.to] >= distances[currentNode]
            || distances[edge.to] >= BOT_DEMO_ROUTE_INFINITY) {
            continue;
        }

        const float cost =
            BotDemoRouteDistance(
                graph, currentNode, static_cast<int>(edge.to)
            ) + distances[edge.to];
        if (cost > bestCost + BOT_DEMO_ROUTE_DETOUR_ALLOWANCE) {
            continue;
        }

        const unsigned int routeSupport =
            BotDemoRouteEdgeWeight(edge, support);
        if (!routeSupport) {
            continue;
        }
        totalWeight += routeSupport
            / (1.0f + (cost - bestCost) / 512.0f);
    }
    if (totalWeight <= 0.0f) {
        return -1;
    }

    float choice = BotDemoRouteRandomFraction(seed) * totalWeight;
    for (unsigned int i = 0; i < graph->edgeCount; ++i) {
        const bot_demo_route_edge_t& edge = graph->edges[i];
        if (edge.from != static_cast<unsigned int>(currentNode)
            || edge.to >= graph->nodeCount
            || (!allowPrevious
                && static_cast<int>(edge.to) == previousNode)
            || distances[edge.to] >= distances[currentNode]
            || distances[edge.to] >= BOT_DEMO_ROUTE_INFINITY) {
            continue;
        }

        const float cost =
            BotDemoRouteDistance(
                graph, currentNode, static_cast<int>(edge.to)
            ) + distances[edge.to];
        if (cost > bestCost + BOT_DEMO_ROUTE_DETOUR_ALLOWANCE) {
            continue;
        }

        const unsigned int routeSupport =
            BotDemoRouteEdgeWeight(edge, support);
        const float weight = routeSupport
            / (1.0f + (cost - bestCost) / 512.0f);
        if (choice < weight) {
            return static_cast<int>(edge.to);
        }
        choice -= weight;
    }
    return -1;
}

static int BotDemoRouteChooseNext(
    const bot_demo_route_graph_t *graph,
    int                           currentNode,
    int                           previousNode,
    const float                  *distances,
    unsigned int                  seed
)
{
    if (!BotDemoRouteValidNode(graph, currentNode)
        || distances[currentNode] >= BOT_DEMO_ROUTE_INFINITY) {
        return -1;
    }

    int next = BotDemoRouteChooseNextPass(
        graph,
        currentNode,
        previousNode,
        distances,
        false,
        seed
    );
    if (next < 0) {
        next = BotDemoRouteChooseNextPass(
            graph,
            currentNode,
            previousNode,
            distances,
            true,
            seed
        );
    }
    return next;
}

static bool BotDemoRoutePointReached(
    const Vector& origin, const Vector& destination
)
{
    const Vector delta = destination - origin;
    return delta.lengthXYSquared()
            <= Square(BOT_OBJECTIVE_DEMO_ROUTE_REACHED_RADIUS)
        && fabs(delta.z) <= BOT_OBJECTIVE_DEMO_ROUTE_REACHED_HEIGHT;
}

void BotController::ResetObjectiveDemoRoute(bool retry)
{
    m_iObjectiveRouteCurrentNode      = -1;
    m_iObjectiveRoutePreviousNode     = -1;
    m_iObjectiveRouteNextNode         = -1;
    m_iObjectiveRouteGoalNode         = -1;
    m_iObjectiveRouteHop              = 0;
    if (retry) {
        m_iObjectiveRouteRetryTime =
            level.inttime + BOT_OBJECTIVE_DEMO_ROUTE_RETRY_MSEC;
        ++m_iObjectiveRouteRetryAttempt;
    } else {
        m_iObjectiveRouteRetryTime    = 0;
        m_iObjectiveRouteRetryAttempt = 0;
    }

    if (m_iObjectiveState == BOT_OBJECTIVE_ROUTE) {
        m_iObjectiveState          = BOT_OBJECTIVE_NONE;
        m_bObjectiveHasDestination = false;
        m_bObjectiveOwnsMovement   = false;
        m_vObjectiveDestination    = vec_zero;
    }
}

bool BotController::UpdateObjectiveDemoRoute(
    const Vector& destination, bool postPlant, bool roam
)
{
    if (m_bObjectiveRoutePostPlant != postPlant) {
        ResetObjectiveDemoRoute();
        m_bObjectiveRoutePostPlant = postPlant;
    }
    if (level.inttime < m_iObjectiveRouteRetryTime) {
        return false;
    }

    const bot_demo_route_graph_t *graph = BotDemoRouteFindGraph(
        level.mapname.c_str(), m_bObjectiveAttacker, postPlant
    );
    if (!graph) {
        return false;
    }

    if (m_iObjectiveRouteCurrentNode < 0) {
        m_iObjectiveRouteCurrentNode = BotDemoRouteFindNearestNode(
            graph,
            controlledEnt->origin,
            BOT_OBJECTIVE_DEMO_ROUTE_SEARCH_RADIUS
        );
        if (m_iObjectiveRouteCurrentNode < 0) {
            ResetObjectiveDemoRoute(true);
            return false;
        }

        G_MoveLogBotEvent(
            "bot_objective_route_plan",
            controlledEnt,
            NULL,
            postPlant ? 2 : 1,
            BotDemoRouteNodePosition(
                graph, m_iObjectiveRouteCurrentNode
            )
        );
    }

    if (m_iObjectiveRouteNextNode >= 0) {
        const Vector routePoint = BotDemoRouteNodePosition(
            graph, m_iObjectiveRouteNextNode
        );
        const Vector routeDestination = BotDemoRouteNearestPathNode(
            routePoint, BOT_OBJECTIVE_DEMO_ROUTE_FIT_RADIUS
        );
        if (BotDemoRoutePointReached(
                controlledEnt->origin, routeDestination
            )) {
            m_iObjectiveRoutePreviousNode =
                m_iObjectiveRouteCurrentNode;
            m_iObjectiveRouteCurrentNode = m_iObjectiveRouteNextNode;
            m_iObjectiveRouteNextNode     = -1;
            m_bObjectiveHasDestination    = false;
            movement.ClearMove();
        } else {
            SetObjectiveDestination(
                routeDestination, BOT_OBJECTIVE_ROUTE
            );
            if (m_bObjectiveHasDestination) {
                return true;
            }

            // Let normal navigation keep the bot active briefly, then rebuild
            // the strategic route from its new position with a different seed.
            G_MoveLogBotEvent(
                "bot_objective_route_fallback",
                controlledEnt,
                NULL,
                m_iObjectiveRouteNextNode,
                routePoint
            );
            ResetObjectiveDemoRoute(true);
            return false;
        }
    }

    const unsigned int seed =
        static_cast<unsigned int>(botManager.GetObjectiveSeed())
        ^ static_cast<unsigned int>(controlledEnt->entnum * 0x9e3779b9u)
        ^ static_cast<unsigned int>(m_iObjectiveRouteHop * 0x85ebca6bu)
        ^ static_cast<unsigned int>(
            m_iObjectiveRouteRetryAttempt * 0xc2b2ae35u
        );

    if (!roam) {
        const int goalNode = BotDemoRouteFindNearestNode(
            graph, destination, BOT_OBJECTIVE_DEMO_ROUTE_SEARCH_RADIUS
        );
        if (goalNode < 0) {
            ResetObjectiveDemoRoute(true);
            return false;
        }
        m_iObjectiveRouteGoalNode = goalNode;
    } else if (m_iObjectiveRouteGoalNode < 0
               || m_iObjectiveRouteGoalNode
                   == m_iObjectiveRouteCurrentNode) {
        m_iObjectiveRouteGoalNode = BotDemoRouteChooseGoal(
            graph,
            controlledEnt->origin,
            destination,
            postPlant ? BOT_OBJECTIVE_DEMO_ROUTE_COVER_RADIUS : 0.0f,
            BOT_OBJECTIVE_DEMO_ROUTE_ROAM_DISTANCE,
            seed
        );
    }

    if (m_iObjectiveRouteGoalNode < 0
        || m_iObjectiveRouteGoalNode
            == m_iObjectiveRouteCurrentNode) {
        if (roam) {
            ResetObjectiveDemoRoute(true);
        } else {
            m_iObjectiveRouteNextNode = -1;
        }
        return false;
    }

    if (graph->nodeCount > BOT_DEMO_ROUTE_MAX_NODES) {
        ResetObjectiveDemoRoute(true);
        return false;
    }

    float distances[BOT_DEMO_ROUTE_MAX_NODES];
    BotDemoRouteDistancesToGoal(
        graph, m_iObjectiveRouteGoalNode, distances
    );

    // Several graph nodes can snap to one navigation node. Consume those
    // internally, but issue at most one movement destination per update.
    for (unsigned int skipped = 0; skipped < graph->nodeCount; ++skipped) {
        m_iObjectiveRouteNextNode = BotDemoRouteChooseNext(
            graph,
            m_iObjectiveRouteCurrentNode,
            m_iObjectiveRoutePreviousNode,
            distances,
            seed + skipped
        );
        if (m_iObjectiveRouteNextNode < 0) {
            break;
        }

        const Vector routePoint = BotDemoRouteNodePosition(
            graph, m_iObjectiveRouteNextNode
        );
        const Vector routeDestination = BotDemoRouteNearestPathNode(
            routePoint, BOT_OBJECTIVE_DEMO_ROUTE_FIT_RADIUS
        );
        if (BotDemoRoutePointReached(
                controlledEnt->origin, routeDestination
            )) {
            m_iObjectiveRoutePreviousNode =
                m_iObjectiveRouteCurrentNode;
            m_iObjectiveRouteCurrentNode =
                m_iObjectiveRouteNextNode;
            m_iObjectiveRouteNextNode = -1;
            ++m_iObjectiveRouteHop;

            if (m_iObjectiveRouteCurrentNode
                == m_iObjectiveRouteGoalNode) {
                if (roam) {
                    m_iObjectiveRouteGoalNode = -1;
                }
                return false;
            }
            continue;
        }

        SetObjectiveDestination(
            routeDestination, BOT_OBJECTIVE_ROUTE
        );
        if (!m_bObjectiveHasDestination) {
            G_MoveLogBotEvent(
                "bot_objective_route_fallback",
                controlledEnt,
                NULL,
                m_iObjectiveRouteNextNode,
                routePoint
            );
            ResetObjectiveDemoRoute(true);
            return false;
        }

        ++m_iObjectiveRouteHop;
        G_MoveLogBotEvent(
            "bot_objective_route_waypoint",
            controlledEnt,
            NULL,
            m_iObjectiveRouteNextNode,
            routeDestination
        );
        return true;
    }

    G_MoveLogBotEvent(
        "bot_objective_route_fallback",
        controlledEnt,
        NULL,
        m_iObjectiveRouteGoalNode,
        controlledEnt->origin
    );
    ResetObjectiveDemoRoute(true);
    return false;
}
