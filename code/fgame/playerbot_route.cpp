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
static const float BOT_DEMO_ROUTE_SEARCH_RADIUS            = 1024.0f;
static const float BOT_DEMO_ROUTE_FIT_RADIUS               = 384.0f;
static const float BOT_DEMO_ROUTE_REACHED_RADIUS           = 192.0f;
static const float BOT_DEMO_ROUTE_REACHED_HEIGHT           = 96.0f;
static const float BOT_DEMO_ROUTE_ROAM_DISTANCE            = 1024.0f;
static const float BOT_OBJECTIVE_DEMO_ROUTE_COVER_RADIUS   = 2048.0f;
static const float BOT_OBJECTIVE_OPENING_ROLE_HEIGHT       = 192.0f;
static const float BOT_OBJECTIVE_OPENING_PATROL_DISTANCE   = 512.0f;
static const int   BOT_DEMO_ROUTE_RETRY_MSEC               = 2000;

struct bot_demo_opening_role_t {
    float x;
    float y;
    float z;
    float radius;
};

// Complete normal-mode SMG lives form four lower and four upper defender
// opening areas on obj_team2. These centers only constrain goal selection;
// the human route graph and navigation backend still choose every path.
static const bot_demo_opening_role_t kObjTeam2DefenderOpeningRoles[] = {
    {-6.0f, 2158.0f, -451.0f, 540.0f},
    {286.0f, 1361.0f, -434.0f, 584.0f},
    {1032.0f, 2386.0f, -453.0f, 542.0f},
    {1907.0f, 1382.0f, -470.0f, 650.0f},
    {333.0f, 324.0f, -108.0f, 497.0f},
    {935.0f, 2183.0f, 9.0f, 590.0f},
    {943.0f, 1043.0f, -18.0f, 650.0f},
    {2538.0f, 2685.0f, -11.0f, 650.0f}
};

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

static Vector BotDemoRouteOpeningRolePosition(int role)
{
    if (role < 0
        || static_cast<unsigned int>(role)
            >= sizeof(kObjTeam2DefenderOpeningRoles)
                / sizeof(kObjTeam2DefenderOpeningRoles[0])) {
        return vec_zero;
    }

    const bot_demo_opening_role_t& openingRole =
        kObjTeam2DefenderOpeningRoles[role];
    return Vector(openingRole.x, openingRole.y, openingRole.z);
}

static int BotDemoRouteChooseOpeningRole(Player *player)
{
    if (!player) {
        return -1;
    }

    static const unsigned int roleCount =
        sizeof(kObjTeam2DefenderOpeningRoles)
        / sizeof(kObjTeam2DefenderOpeningRoles[0]);
    int roleUseCount[roleCount] = {};

    // Simulate one deterministic team assignment in entity-number order. A
    // bot takes its closest unused area, so up to eight defenders spread out
    // while still favoring the area nearest their actual round spawn.
    const Container<BotController *>& controllers =
        botManager.getControllerManager().getControllers();
    for (int entityNumber = 0; entityNumber < MAX_CLIENTS; ++entityNumber) {
        Player *candidate = NULL;
        for (int i = 1; i <= controllers.NumObjects(); ++i) {
            Player *controllerPlayer =
                controllers.ObjectAt(i)->getControlledEntity();
            if (controllerPlayer
                && controllerPlayer->entnum == entityNumber
                && controllerPlayer->GetTeam() == player->GetTeam()) {
                candidate = controllerPlayer;
                break;
            }
        }
        if (!candidate) {
            continue;
        }

        int   bestRole  = -1;
        float bestScore = 0.0f;
        for (unsigned int role = 0; role < roleCount; ++role) {
            const unsigned int jitterSeed =
                static_cast<unsigned int>(botManager.GetObjectiveSeed())
                ^ static_cast<unsigned int>(entityNumber * 0x9e3779b9u)
                ^ static_cast<unsigned int>(role * 0x85ebca6bu);
            const float score =
                (BotDemoRouteOpeningRolePosition(role) - candidate->origin)
                    .lengthSquared()
                + roleUseCount[role] * Square(8192.0f)
                + BotDemoRouteRandomFraction(jitterSeed) * Square(64.0f);
            if (bestRole < 0 || score < bestScore) {
                bestRole  = static_cast<int>(role);
                bestScore = score;
            }
        }

        if (bestRole < 0) {
            continue;
        }
        ++roleUseCount[bestRole];
        if (candidate == player) {
            return bestRole;
        }
    }

    return (botManager.GetObjectiveBotRank(player)
            + botManager.GetObjectiveSeed())
        % static_cast<int>(roleCount);
}

static bool BotDemoRouteValidNode(
    const bot_demo_route_graph_t *graph, int node
)
{
    return graph && node >= 0
        && static_cast<unsigned int>(node) < graph->nodeCount;
}

static const bot_demo_route_graph_t *BotDemoRouteFindGraph(
    const char            *mapName,
    bot_demo_route_mode_t  mode,
    bool                   attacker,
    bool                   postPlant
)
{
    if (!mapName) {
        return NULL;
    }

    for (unsigned int i = 0; i < g_botDemoRouteGraphCount; ++i) {
        const bot_demo_route_graph_t& graph = g_botDemoRouteGraphs[i];
        if (graph.mode == mode && graph.attacker == attacker
            && graph.postPlant == postPlant
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

static bool BotDemoRouteNodeHasOutgoingEdge(
    const bot_demo_route_graph_t *graph, int node
)
{
    for (unsigned int i = 0; i < graph->edgeCount; ++i) {
        if (graph->edges[i].from == static_cast<unsigned int>(node)) {
            return true;
        }
    }
    return false;
}

static int BotDemoRouteFindNearestNode(
    const bot_demo_route_graph_t *graph,
    const Vector&                 position,
    float                         maxDistance,
    bool                          requireOutgoing = false
)
{
    if (!graph || !graph->nodeCount) {
        return -1;
    }

    int   nearest = -1;
    float bestDistanceSquared = 0.0f;
    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        if (requireOutgoing
            && !BotDemoRouteNodeHasOutgoingEdge(graph, i)) {
            continue;
        }
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

static void BotDemoRouteReachableNodes(
    const bot_demo_route_graph_t *graph,
    int                           start,
    bool                         *reachable
)
{
    int queue[BOT_DEMO_ROUTE_MAX_NODES];
    int first = 0;
    int last  = 0;

    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        reachable[i] = false;
    }
    if (!BotDemoRouteValidNode(graph, start)) {
        return;
    }

    reachable[start] = true;
    queue[last++]     = start;
    while (first < last) {
        const int node = queue[first++];
        for (unsigned int i = 0; i < graph->edgeCount; ++i) {
            const bot_demo_route_edge_t& edge = graph->edges[i];
            if (edge.from != static_cast<unsigned int>(node)
                || edge.to >= graph->nodeCount
                || reachable[edge.to]) {
                continue;
            }
            reachable[edge.to] = true;
            queue[last++]       = static_cast<int>(edge.to);
        }
    }
}

static bot_demo_route_support_t BotDemoRouteNodeEndSupport(
    const bot_demo_route_graph_t *graph,
    const Vector&                 origin,
    const Vector&                 center,
    float                         maxCenterDistance,
    float                         maxCenterHeight,
    float                         minOriginDistance,
    const bool                   *reachable
)
{
    unsigned int smgSupport = 0;
    unsigned int normalSupport = 0;

    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        if (!reachable[i]) {
            continue;
        }
        const Vector position = BotDemoRouteNodePosition(graph, i);
        if (maxCenterDistance > 0.0f
            && (position - center).lengthXYSquared()
                > Square(maxCenterDistance)) {
            continue;
        }
        if (maxCenterHeight > 0.0f
            && fabs(position.z - center.z) > maxCenterHeight) {
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
    int                           currentNode,
    const Vector&                 origin,
    const Vector&                 center,
    float                         maxCenterDistance,
    float                         maxCenterHeight,
    float                         minOriginDistance,
    unsigned int                  seed
)
{
    if (!graph || !graph->nodeCount) {
        return -1;
    }

    bool reachable[BOT_DEMO_ROUTE_MAX_NODES];
    BotDemoRouteReachableNodes(graph, currentNode, reachable);
    const bot_demo_route_support_t support =
        BotDemoRouteNodeEndSupport(
            graph,
            origin,
            center,
            maxCenterDistance,
            maxCenterHeight,
            minOriginDistance,
            reachable
        );
    unsigned int totalWeight = 0;

    for (unsigned int i = 0; i < graph->nodeCount; ++i) {
        if (!reachable[i]) {
            continue;
        }
        const Vector position = BotDemoRouteNodePosition(graph, i);
        if (maxCenterDistance > 0.0f
            && (position - center).lengthXYSquared()
                > Square(maxCenterDistance)) {
            continue;
        }
        if (maxCenterHeight > 0.0f
            && fabs(position.z - center.z) > maxCenterHeight) {
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
        if (!reachable[i]) {
            continue;
        }
        const Vector position = BotDemoRouteNodePosition(graph, i);
        if (maxCenterDistance > 0.0f
            && (position - center).lengthXYSquared()
                > Square(maxCenterDistance)) {
            continue;
        }
        if (maxCenterHeight > 0.0f
            && fabs(position.z - center.z) > maxCenterHeight) {
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
            <= Square(BOT_DEMO_ROUTE_REACHED_RADIUS)
        && fabs(delta.z) <= BOT_DEMO_ROUTE_REACHED_HEIGHT;
}

void BotController::ResetDemoRoute(
    bot_demo_route_cursor_t& route, bool retry
)
{
    route.currentNode  = -1;
    route.previousNode = -1;
    route.nextNode     = -1;
    route.goalNode     = -1;
    route.hop          = 0;

    if (retry) {
        route.retryTime = level.inttime + BOT_DEMO_ROUTE_RETRY_MSEC;
        ++route.retryAttempt;
    } else {
        route.retryTime    = 0;
        route.retryAttempt = 0;
    }

    if (&route == &m_ObjectiveDemoRoute
        && m_iObjectiveState == BOT_OBJECTIVE_ROUTE) {
        m_iObjectiveState          = BOT_OBJECTIVE_NONE;
        m_bObjectiveHasDestination = false;
        m_bObjectiveOwnsMovement   = false;
        m_vObjectiveDestination    = vec_zero;
    }
}

void BotController::ResetObjectiveDemoRoute(bool retry)
{
    ResetDemoRoute(m_ObjectiveDemoRoute, retry);
}

void BotController::ResetFreeForAllDemoRoute(bool retry)
{
    ResetDemoRoute(m_FreeForAllDemoRoute, retry);
}

bool BotController::UpdateObjectiveDemoRoute(
    const Vector& destination, bool postPlant, bool roam
)
{
    Vector goalCenter = destination;
    float  maxGoalCenterDistance =
        postPlant ? BOT_OBJECTIVE_DEMO_ROUTE_COVER_RADIUS : 0.0f;
    float maxGoalCenterHeight   = 0.0f;
    float minGoalOriginDistance = BOT_DEMO_ROUTE_ROAM_DISTANCE;

    const bool usesOpeningRole =
        !postPlant && !m_bObjectiveAttacker
        && !Q_stricmp(level.mapname.c_str(), "obj/obj_team2");
    if (usesOpeningRole && m_iObjectiveOpeningRole < 0) {
        m_iObjectiveOpeningRole =
            BotDemoRouteChooseOpeningRole(controlledEnt);
        G_MoveLogBotEvent(
            "bot_objective_opening_role",
            controlledEnt,
            NULL,
            m_iObjectiveOpeningRole,
            BotDemoRouteOpeningRolePosition(m_iObjectiveOpeningRole)
        );
    }

    const Vector openingRolePosition =
        BotDemoRouteOpeningRolePosition(m_iObjectiveOpeningRole);
    if (usesOpeningRole && openingRolePosition != vec_zero) {
        const bot_demo_opening_role_t& openingRole =
            kObjTeam2DefenderOpeningRoles[m_iObjectiveOpeningRole];
        goalCenter             = openingRolePosition;
        maxGoalCenterDistance  = openingRole.radius;
        maxGoalCenterHeight    = BOT_OBJECTIVE_OPENING_ROLE_HEIGHT;
        minGoalOriginDistance  = BOT_OBJECTIVE_OPENING_PATROL_DISTANCE;
    }
    if (m_bObjectiveRoutePostPlant != postPlant) {
        ResetObjectiveDemoRoute();
        m_bObjectiveRoutePostPlant = postPlant;
    }

    const bot_demo_route_graph_t *graph = BotDemoRouteFindGraph(
        level.mapname.c_str(),
        BOT_DEMO_ROUTE_OBJECTIVE,
        m_bObjectiveAttacker,
        postPlant
    );
    const unsigned int seed =
        static_cast<unsigned int>(botManager.GetObjectiveSeed())
        ^ static_cast<unsigned int>(
            controlledEnt->entnum * 0x9e3779b9u
        ) ^ static_cast<unsigned int>(
            (m_iObjectiveOpeningRole + 1) * 0x85ebca6bu
        );
    return UpdateDemoRoute(
        graph,
        m_ObjectiveDemoRoute,
        goalCenter,
        roam,
        maxGoalCenterDistance,
        maxGoalCenterHeight,
        minGoalOriginDistance,
        seed
    );
}

bool BotController::UpdateFreeForAllDemoRoute()
{
    if (g_gametype->integer != GT_FFA) {
        return false;
    }

    const bot_demo_route_graph_t *graph = BotDemoRouteFindGraph(
        level.mapname.c_str(),
        BOT_DEMO_ROUTE_FREE_FOR_ALL,
        false,
        false
    );
    const unsigned int seed =
        static_cast<unsigned int>(level.inttime)
        ^ static_cast<unsigned int>(
            controlledEnt->entnum * 0x9e3779b9u
        );
    return UpdateDemoRoute(
        graph,
        m_FreeForAllDemoRoute,
        vec_zero,
        true,
        0.0f,
        0.0f,
        BOT_DEMO_ROUTE_ROAM_DISTANCE,
        seed
    );
}

bool BotController::UpdateDemoRoute(
    const bot_demo_route_graph_t *graph,
    bot_demo_route_cursor_t&      route,
    const Vector&                 destination,
    bool                          roam,
    float                         maxGoalCenterDistance,
    float                         maxGoalCenterHeight,
    float                         minGoalOriginDistance,
    unsigned int                  seed
)
{
    if (!graph) {
        return false;
    }

    if (graph->nodeCount > BOT_DEMO_ROUTE_MAX_NODES) {
        ResetDemoRoute(route, true);
        return false;
    }

    const bool objective = graph->mode == BOT_DEMO_ROUTE_OBJECTIVE;

    // Replacing a path from a mid-ladder origin can fail and send the bot back
    // to the same entrance. Finish or abandon the current ladder transition
    // before the strategic route layer issues another hop.
    if (controlledEnt->GetLadder()) {
        if (objective) {
            m_bObjectiveOwnsMovement = true;
        }
        return true;
    }

    if (level.inttime < route.retryTime) {
        return false;
    }

    const char *planEvent = objective
        ? "bot_objective_route_plan" : "bot_ffa_route_plan";
    const char *waypointEvent = objective
        ? "bot_objective_route_waypoint" : "bot_ffa_route_waypoint";
    const char *fallbackEvent = objective
        ? "bot_objective_route_fallback" : "bot_ffa_route_fallback";

    if (route.currentNode < 0) {
        route.currentNode = BotDemoRouteFindNearestNode(
            graph,
            controlledEnt->origin,
            BOT_DEMO_ROUTE_SEARCH_RADIUS,
            roam
        );
        if (route.currentNode < 0) {
            ResetDemoRoute(route, true);
            return false;
        }

        G_MoveLogBotEvent(
            planEvent,
            controlledEnt,
            NULL,
            objective ? (m_bObjectiveRoutePostPlant ? 2 : 1) : 0,
            BotDemoRouteNodePosition(graph, route.currentNode)
        );
    }

    if (route.nextNode >= 0) {
        const Vector routePoint = BotDemoRouteNodePosition(
            graph, route.nextNode
        );
        const Vector routeDestination = BotDemoRouteNearestPathNode(
            routePoint, BOT_DEMO_ROUTE_FIT_RADIUS
        );
        if (BotDemoRoutePointReached(
                controlledEnt->origin, routeDestination
            )) {
            route.previousNode = route.currentNode;
            route.currentNode  = route.nextNode;
            route.nextNode     = -1;
            if (objective) {
                m_bObjectiveHasDestination = false;
            }
            movement.ClearMove();
        } else {
            if (objective) {
                SetObjectiveDestination(
                    routeDestination, BOT_OBJECTIVE_ROUTE
                );
            } else if (!movement.IsMoving() || movement.MoveDone()) {
                movement.MoveTo(routeDestination);
            }

            if (objective
                    ? m_bObjectiveHasDestination
                    : movement.IsMoving()) {
                return true;
            }

            // Let normal navigation keep the bot active briefly, then rebuild
            // the strategic route from its new position with a different seed.
            G_MoveLogBotEvent(
                fallbackEvent,
                controlledEnt,
                NULL,
                route.nextNode,
                routePoint
            );
            ResetDemoRoute(route, true);
            return false;
        }
    }

    seed ^= static_cast<unsigned int>(route.hop * 0x85ebca6bu)
        ^ static_cast<unsigned int>(
            route.retryAttempt * 0xc2b2ae35u
        );

    if (!roam) {
        const int goalNode = BotDemoRouteFindNearestNode(
            graph, destination, BOT_DEMO_ROUTE_SEARCH_RADIUS
        );
        if (goalNode < 0) {
            ResetDemoRoute(route, true);
            return false;
        }
        route.goalNode = goalNode;
    } else if (route.goalNode < 0
               || route.goalNode == route.currentNode) {
        route.goalNode = BotDemoRouteChooseGoal(
            graph,
            route.currentNode,
            controlledEnt->origin,
            destination,
            maxGoalCenterDistance,
            maxGoalCenterHeight,
            minGoalOriginDistance,
            seed
        );
    }

    if (route.goalNode < 0 || route.goalNode == route.currentNode) {
        if (roam) {
            ResetDemoRoute(route, true);
        } else {
            route.nextNode = -1;
        }
        return false;
    }

    float distances[BOT_DEMO_ROUTE_MAX_NODES];
    BotDemoRouteDistancesToGoal(graph, route.goalNode, distances);

    // Several graph nodes can snap to one navigation node. Consume those
    // internally, but issue at most one movement destination per update.
    for (unsigned int skipped = 0; skipped < graph->nodeCount; ++skipped) {
        route.nextNode = BotDemoRouteChooseNext(
            graph,
            route.currentNode,
            route.previousNode,
            distances,
            seed + skipped
        );
        if (route.nextNode < 0) {
            break;
        }

        const Vector routePoint = BotDemoRouteNodePosition(
            graph, route.nextNode
        );
        const Vector routeDestination = BotDemoRouteNearestPathNode(
            routePoint, BOT_DEMO_ROUTE_FIT_RADIUS
        );
        if (BotDemoRoutePointReached(
                controlledEnt->origin, routeDestination
            )) {
            route.previousNode = route.currentNode;
            route.currentNode  = route.nextNode;
            route.nextNode     = -1;
            ++route.hop;

            if (route.currentNode == route.goalNode) {
                if (roam) {
                    route.goalNode = -1;
                }
                return !objective;
            }
            continue;
        }

        if (objective) {
            SetObjectiveDestination(
                routeDestination, BOT_OBJECTIVE_ROUTE
            );
        } else {
            movement.MoveTo(routeDestination);
        }
        if (objective
                ? !m_bObjectiveHasDestination
                : !movement.IsMoving()) {
            G_MoveLogBotEvent(
                fallbackEvent,
                controlledEnt,
                NULL,
                route.nextNode,
                routePoint
            );
            ResetDemoRoute(route, true);
            return false;
        }

        ++route.hop;
        G_MoveLogBotEvent(
            waypointEvent,
            controlledEnt,
            NULL,
            route.nextNode,
            routeDestination
        );
        return true;
    }

    G_MoveLogBotEvent(
        fallbackEvent,
        controlledEnt,
        NULL,
        route.goalNode,
        controlledEnt->origin
    );
    ResetDemoRoute(route, true);
    return false;
}
