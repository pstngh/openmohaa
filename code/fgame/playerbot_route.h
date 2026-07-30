/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

Human-demo route graph interface for multiplayer bots.
===========================================================================
*/

#pragma once

enum bot_demo_route_mode_t {
    BOT_DEMO_ROUTE_OBJECTIVE,
    BOT_DEMO_ROUTE_FREE_FOR_ALL
};

struct bot_demo_route_node_t {
    float        x;
    float        y;
    float        z;
    unsigned int geometryEnds;
    unsigned int normalEnds;
    unsigned int smgEnds;
};

struct bot_demo_route_edge_t {
    unsigned int from;
    unsigned int to;
    unsigned int geometryRoutes;
    unsigned int normalRoutes;
    unsigned int smgRoutes;
};

struct bot_demo_route_graph_t {
    const char                       *mapName;
    bot_demo_route_mode_t             mode;
    bool                              attacker;
    bool                              postPlant;
    const bot_demo_route_node_t      *nodes;
    unsigned int                      nodeCount;
    const bot_demo_route_edge_t      *edges;
    unsigned int                      edgeCount;
};

struct bot_demo_route_cursor_t {
    int currentNode;
    int previousNode;
    int nextNode;
    int goalNode;
    int hop;
    int retryTime;
    int retryAttempt;
};

extern const bot_demo_route_graph_t g_botDemoRouteGraphs[];
extern const unsigned int           g_botDemoRouteGraphCount;
