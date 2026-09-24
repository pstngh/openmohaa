/*
===========================================================================
Copyright (C) 2024 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMoHAA source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
OpenMoHAA source code; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#pragma once

#include "sv_chatban_core.h"

struct client_s;

void SV_ChatBanAddOperatorCommands(void);

qboolean SV_ChatBanResolveTarget(
    const char *token,
    netadr_t *address,
    int *subnet,
    struct client_s **target,
    char *error,
    int errorSize
);
qboolean SV_ChatBanParseAddress(
    const char *token,
    netadr_t *address,
    int *subnet,
    char *error,
    int errorSize
);

qboolean SV_ChatBanAdd(
    netadr_t address,
    int subnet,
    const char *reason,
    serverChatBan_t *added,
    char *error,
    int errorSize
);
qboolean SV_ChatBanRemove(
    netadr_t address,
    int subnet,
    serverChatBan_t *removed,
    char *error,
    int errorSize
);
qboolean SV_ChatBanRemoveNumber(
    int number,
    serverChatBan_t *removed,
    char *error,
    int errorSize
);

int SV_ChatBanGetCount(void);
const serverChatBan_t *SV_ChatBanGetEntry(int index);

void SV_ChatBanRefreshClient(struct client_s *client, qboolean notifyChange);
void SV_ChatBanNotifyClient(struct client_s *client);
void SV_ChatBanAnnounceMatches(
    netadr_t address,
    int subnet,
    qboolean added,
    const char *adminUsername
);
