/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code; you can redistribute it
and/or modify it under the terms of the GNU General Public License.
===========================================================================
*/

// sv_rotation.c -- timed, population-aware dedicated-server map rotation

#include "server.h"

#define MAX_ROTATION_ENTRIES 64

typedef struct {
    char map[MAX_QPATH];
    int  minutes;
} mapRotationEntry_t;

static mapRotationEntry_t rotationEntries[MAX_ROTATION_ENTRIES];
static int                rotationEntryCount;
static int                rotationNextIndex;
static qboolean           rotationStarted;
static qboolean           rotationPending;
static char               rotationPendingMap[MAX_QPATH];
static int                rotationPendingMinutes;

static qboolean SV_MapRotationValidMapName(const char *map)
{
    const unsigned char *p = (const unsigned char *)map;

    if (!map[0] || map[0] == '/' || strstr(map, "..")) {
        return qfalse;
    }

    while (*p) {
        if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')
            || (*p >= '0' && *p <= '9') || *p == '_' || *p == '-'
            || *p == '.' || *p == '/') {
            p++;
            continue;
        }

        return qfalse;
    }

    return qtrue;
}

static qboolean SV_MapRotationMapExists(const char *map)
{
    char path[MAX_QPATH];

    Com_sprintf(path, sizeof(path), "maps/%s.bsp", map);
    if (FS_ReadFile(path, NULL) >= 0) {
        return qtrue;
    }

    Com_sprintf(path, sizeof(path), "maps/%s_sml.bsp", map);
    return FS_ReadFile(path, NULL) >= 0;
}

static qboolean SV_MapRotationParseEntry(const char *text, mapRotationEntry_t *entry)
{
    char  buffer[MAX_QPATH + 16];
    char *separator;
    char *end;
    long  minutes;

    if (!text || strlen(text) >= sizeof(buffer)) {
        return qfalse;
    }

    Q_strncpyz(buffer, text, sizeof(buffer));
    separator = strrchr(buffer, ':');
    if (!separator || separator == buffer || !separator[1]) {
        return qfalse;
    }

    *separator = '\0';
    minutes    = strtol(separator + 1, &end, 10);
    if (*end || minutes < 1 || minutes > 10800
        || !SV_MapRotationValidMapName(buffer)) {
        return qfalse;
    }

    Q_strncpyz(entry->map, buffer, sizeof(entry->map));
    entry->minutes = (int)minutes;
    return qtrue;
}

static int SV_MapRotationConnectedClients(void)
{
    int count = 0;
    int i;

    for (i = 0; i < svs.iNumClients; i++) {
        if (svs.clients[i].state >= CS_CONNECTED) {
            count++;
        }
    }

    return count;
}

static void SV_MapRotationSetPending(const mapRotationEntry_t *entry)
{
    rotationPending        = qtrue;
    rotationPendingMinutes = entry->minutes;
    Q_strncpyz(rotationPendingMap, entry->map, sizeof(rotationPendingMap));
}

void SV_MapRotationClear_f(void)
{
    rotationEntryCount    = 0;
    rotationNextIndex     = 0;
    rotationStarted       = qfalse;
    rotationPending       = qfalse;
    rotationPendingMap[0] = '\0';
    Com_Printf("Map rotation cleared.\n");
}

void SV_MapRotationAdd_f(void)
{
    mapRotationEntry_t entry;
    int                oldCount;
    int                i;

    if (Cmd_Argc() < 2) {
        Com_Printf("Usage: maprotation_add <map:minutes> [map:minutes ...]\n");
        return;
    }

    oldCount = rotationEntryCount;
    for (i = 1; i < Cmd_Argc(); i++) {
        if (rotationEntryCount >= MAX_ROTATION_ENTRIES) {
            Com_Printf("Map rotation is limited to %d entries.\n", MAX_ROTATION_ENTRIES);
            rotationEntryCount = oldCount;
            return;
        }

        if (!SV_MapRotationParseEntry(Cmd_Argv(i), &entry)) {
            Com_Printf("Invalid map rotation entry '%s' (expected map:minutes).\n", Cmd_Argv(i));
            rotationEntryCount = oldCount;
            return;
        }

        if (!SV_MapRotationMapExists(entry.map)) {
            Com_Printf("Map rotation map '%s' was not found.\n", entry.map);
            rotationEntryCount = oldCount;
            return;
        }

        rotationEntries[rotationEntryCount++] = entry;
    }

    Com_Printf("Map rotation now contains %d entr%s.\n",
        rotationEntryCount, rotationEntryCount == 1 ? "y" : "ies");
}

void SV_MapRotationList_f(void)
{
    int i;

    if (!rotationEntryCount) {
        Com_Printf("Map rotation is empty.\n");
        return;
    }

    for (i = 0; i < rotationEntryCount; i++) {
        Com_Printf("%2d: %s:%d%s\n", i + 1,
            rotationEntries[i].map, rotationEntries[i].minutes,
            rotationStarted && i == rotationNextIndex ? " (next)" : "");
    }
}

// Pick the map that follows the current one: the next entry, or the lock
// map while too few clients are connected. Only a played entry advances.
static const char *SV_MapRotationSelectNext(void)
{
    mapRotationEntry_t        lockEntry;
    const mapRotationEntry_t *nextEntry;
    int                       minPlayers;
    int                       playerCount;

    nextEntry  = &rotationEntries[rotationNextIndex];
    minPlayers = sv_maprotation_minplayers->integer;
    playerCount = SV_MapRotationConnectedClients();

    if (minPlayers > 0 && playerCount < minPlayers) {
        if (!SV_MapRotationParseEntry(sv_maprotation_lockmap->string, &lockEntry)
            || !SV_MapRotationMapExists(lockEntry.map)) {
            Com_Printf("Invalid sv_maprotation_lockmap '%s'; continuing the normal rotation.\n",
                sv_maprotation_lockmap->string);
        } else if (Q_stricmp(nextEntry->map, lockEntry.map)) {
            SV_MapRotationSetPending(&lockEntry);
            Com_Printf("Map rotation holding %s:%d: %d/%d connected; using %s:%d.\n",
                nextEntry->map, nextEntry->minutes, playerCount, minPlayers,
                lockEntry.map, lockEntry.minutes);
            return rotationPendingMap;
        }
    }

    SV_MapRotationSetPending(nextEntry);
    rotationNextIndex = (rotationNextIndex + 1) % rotationEntryCount;
    return rotationPendingMap;
}

void SV_MapRotationStart_f(void)
{
    const char *map;

    if (!rotationEntryCount) {
        Com_Printf("Cannot start an empty map rotation.\n");
        return;
    }

    rotationStarted   = qtrue;
    rotationNextIndex = 0;
    // The first entry obeys the population lock like every later one.
    map = SV_MapRotationSelectNext();

    Cvar_Set("sv_maplist", "");
    Cvar_Set("nextmap", "");
    Cbuf_AddText(va("map \"%s\"\n", map));
}

void SV_MapRotationMapStarted(const char *map)
{
    if (!rotationStarted) {
        return;
    }

    if (rotationPending && !Q_stricmp(map, rotationPendingMap)) {
        Cvar_Set("timelimit", va("%d", rotationPendingMinutes));
        rotationPending       = qfalse;
        rotationPendingMap[0] = '\0';
    }
}

void SV_MapRotationAdvance_f(void)
{
    if (!rotationStarted || !rotationEntryCount) {
        return;
    }

    Cvar_Set("nextmap", SV_MapRotationSelectNext());
}
