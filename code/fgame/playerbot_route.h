/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

Human-demo route graph interface for objective bots.
===========================================================================
*/

#pragma once

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
    bool                              attacker;
    bool                              postPlant;
    const bot_demo_route_node_t      *nodes;
    unsigned int                      nodeCount;
    const bot_demo_route_edge_t      *edges;
    unsigned int                      edgeCount;
};

extern const bot_demo_route_graph_t g_botDemoRouteGraphs[];
extern const unsigned int           g_botDemoRouteGraphCount;
