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

#include "../qcommon/qcommon.h"

#define SERVER_MAXCHATBANS       1024
#define MAX_CHATBAN_REASON_LENGTH 128

typedef struct serverChatBan_s {
    netadr_t ip;
    int      subnet;
    char     reason[MAX_CHATBAN_REASON_LENGTH];
} serverChatBan_t;

typedef enum chatBanCommandType_e {
    CHATBAN_COMMAND_NONE,
    CHATBAN_COMMAND_TEXT,
    CHATBAN_COMMAND_TAUNT
} chatBanCommandType_t;

typedef enum chatBanParseResult_e {
    CHATBAN_PARSE_INVALID = -1,
    CHATBAN_PARSE_SKIP,
    CHATBAN_PARSE_OK
} chatBanParseResult_t;

qboolean SV_ChatBanReasonIsSafe(const char *reason);
qboolean SV_ChatBanAddressTokenIsSafe(const char *address);
qboolean SV_ChatBanParseInteger(const char *text, int minimum, int maximum, int *value);
qboolean SV_ChatBanCanonicalizeRelativePath(
    const char *input,
    char *output,
    size_t outputSize
);
chatBanCommandType_t SV_ChatBanClassifyCommand(const char *command, const char *messageToken);

chatBanParseResult_t SV_ChatBanParseRecord(
    char *line,
    char **address,
    int *subnet,
    char **reason
);
qboolean SV_ChatBanFormatRecord(
    const char *address,
    int subnet,
    const char *reason,
    char *record,
    size_t recordSize
);

qboolean SV_ChatBanEntryMatches(const serverChatBan_t *entry, netadr_t address);
int SV_ChatBanFindExactEntry(
    const serverChatBan_t *entries,
    int count,
    netadr_t address,
    int subnet
);
int SV_ChatBanFindCoveringEntry(
    const serverChatBan_t *entries,
    int count,
    netadr_t address,
    int subnet
);
int SV_ChatBanDeleteCoveredEntries(
    serverChatBan_t *entries,
    int *count,
    netadr_t address,
    int subnet
);
