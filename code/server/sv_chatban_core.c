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

#include "sv_chatban_core.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *SV_ChatBanSkipWhitespace(char *text)
{
    while (*text && isspace((unsigned char)*text)) {
        text++;
    }

    return text;
}

static void SV_ChatBanTrimTrailingWhitespace(char *text)
{
    char *end = text + strlen(text);

    while (end > text && isspace((unsigned char)end[-1])) {
        end--;
    }

    *end = '\0';
}

static int SV_ChatBanMaximumPrefix(const char *address)
{
    return strchr(address, ':') ? 128 : 32;
}

qboolean SV_ChatBanReasonIsSafe(const char *reason)
{
    const unsigned char *p = (const unsigned char *)reason;

    if (!reason || strlen(reason) >= MAX_CHATBAN_REASON_LENGTH) {
        return qfalse;
    }

    while (*p) {
        if (*p < ' ' || *p == 127 || *p == '"' || *p == '\\' || *p == ';') {
            return qfalse;
        }
        p++;
    }

    return qtrue;
}

static qboolean SV_ChatBanIPv4AddressIsSafe(const char *address)
{
    const char *p = address;
    int segment;

    for (segment = 0; segment < 4; segment++) {
        int digits = 0;
        int value = 0;

        if (*p < '0' || *p > '9') {
            return qfalse;
        }
        if (*p == '0' && p[1] >= '0' && p[1] <= '9') {
            return qfalse;
        }

        while (*p >= '0' && *p <= '9') {
            value = value * 10 + (*p - '0');
            digits++;
            if (digits > 3 || value > 255) {
                return qfalse;
            }
            p++;
        }

        if (segment < 3) {
            if (*p++ != '.') {
                return qfalse;
            }
        } else if (*p) {
            return qfalse;
        }
    }

    return qtrue;
}

qboolean SV_ChatBanAddressTokenIsSafe(const char *address)
{
    const unsigned char *p = (const unsigned char *)address;
    const char          *firstColon;
    qboolean             hasColon;

    if (!address || !address[0]) {
        return qfalse;
    }

    firstColon = strchr(address, ':');
    hasColon = firstColon != NULL;

    // An IPv6 literal has at least two colons. With one, the address parser
    // reads host:port and resolves the host through DNS.
    if (hasColon && !strchr(firstColon + 1, ':')) {
        return qfalse;
    }

    while (*p) {
        if (hasColon) {
            if (!(isxdigit(*p) || *p == ':' || *p == '.')) {
                return qfalse;
            }
        } else if (!(isdigit(*p) || *p == '.')) {
            return qfalse;
        }
        p++;
    }

    return hasColon || SV_ChatBanIPv4AddressIsSafe(address);
}

qboolean SV_ChatBanParseInteger(const char *text, int minimum, int maximum, int *value)
{
    const unsigned char *p = (const unsigned char *)text;
    char *end;
    long parsed;

    if (!text || !text[0] || !value || minimum > maximum) {
        return qfalse;
    }

    while (*p) {
        if (*p < '0' || *p > '9') {
            return qfalse;
        }
        p++;
    }

    errno = 0;
    parsed = strtol(text, &end, 10);
    if (errno == ERANGE || *end || parsed < minimum || parsed > maximum) {
        return qfalse;
    }

    *value = (int)parsed;
    return qtrue;
}

qboolean SV_ChatBanCanonicalizeRelativePath(
    const char *input,
    char *output,
    size_t outputSize
)
{
    const unsigned char *source = (const unsigned char *)input;
    char *destination = output;
    char *segmentStart = output;
    char *outputEnd;

    if (!input || !input[0] || !output || outputSize < 2
        || input[0] == '/' || input[0] == '\\') {
        return qfalse;
    }

    outputEnd = output + outputSize - 1;
    while (*source) {
        unsigned char c = *source++;

        if (c == '/' || c == '\\') {
            if (destination == segmentStart) {
                continue;
            }
            if (destination - segmentStart == 1 && segmentStart[0] == '.') {
                destination = segmentStart;
                continue;
            }
            if (destination - segmentStart == 2
                && segmentStart[0] == '.' && segmentStart[1] == '.') {
                return qfalse;
            }
            if (destination >= outputEnd) {
                return qfalse;
            }
            *destination++ = '/';
            segmentStart = destination;
            continue;
        }

        if (c >= 'A' && c <= 'Z') {
            c = (unsigned char)(c - 'A' + 'a');
        } else if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
                     || c == '_' || c == '-' || c == '.')) {
            return qfalse;
        }

        if (destination >= outputEnd) {
            return qfalse;
        }
        *destination++ = (char)c;
    }

    if (destination - segmentStart == 1 && segmentStart[0] == '.') {
        destination = segmentStart;
    } else if (destination - segmentStart == 2
               && segmentStart[0] == '.' && segmentStart[1] == '.') {
        return qfalse;
    }

    if (destination > output && destination[-1] == '/') {
        destination--;
    }
    *destination = '\0';
    return output[0] != '\0';
}

static qboolean SV_ChatBanIsTauntToken(const char *token)
{
    return token && token[0] == '*'
           && token[1] >= '1' && token[1] <= '9'
           && token[2] >= '1' && token[2] <= '9'
           && token[3] == '\0';
}

chatBanCommandType_t SV_ChatBanClassifyCommand(const char *command, const char *messageToken)
{
    if (!command || !command[0]) {
        return CHATBAN_COMMAND_NONE;
    }

    if (!Q_stricmp(command, "dmmessage")) {
        return SV_ChatBanIsTauntToken(messageToken) ? CHATBAN_COMMAND_TAUNT : CHATBAN_COMMAND_TEXT;
    }

    if (!Q_stricmp(command, "say") || !Q_stricmp(command, "sayteam") || !Q_stricmp(command, "tell")) {
        return CHATBAN_COMMAND_TEXT;
    }

    if (!Q_stricmp(command, "vsay") || !Q_stricmp(command, "vosay")
        || !Q_stricmp(command, "vtell") || !Q_stricmp(command, "instamsg")) {
        return CHATBAN_COMMAND_TAUNT;
    }

    return CHATBAN_COMMAND_NONE;
}

chatBanParseResult_t SV_ChatBanParseRecord(
    char *line,
    char **address,
    int *subnet,
    char **reason
)
{
    char *p;
    char *prefixEnd;
    long  parsedSubnet;

    if (!line || !address || !subnet || !reason) {
        return CHATBAN_PARSE_INVALID;
    }

    *address = NULL;
    *subnet  = 0;
    *reason  = NULL;

    p = SV_ChatBanSkipWhitespace(line);
    SV_ChatBanTrimTrailingWhitespace(p);

    if (!p[0] || (p[0] == '/' && p[1] == '/')) {
        return CHATBAN_PARSE_SKIP;
    }

    *address = p;
    while (*p && !isspace((unsigned char)*p)) {
        p++;
    }

    if (!*p) {
        return CHATBAN_PARSE_INVALID;
    }

    *p++ = '\0';
    if (!SV_ChatBanAddressTokenIsSafe(*address)) {
        return CHATBAN_PARSE_INVALID;
    }

    p = SV_ChatBanSkipWhitespace(p);
    if (*p < '0' || *p > '9') {
        return CHATBAN_PARSE_INVALID;
    }

    errno = 0;
    parsedSubnet = strtol(p, &prefixEnd, 10);
    if (errno == ERANGE || prefixEnd == p || parsedSubnet < 0
        || parsedSubnet > SV_ChatBanMaximumPrefix(*address)) {
        return CHATBAN_PARSE_INVALID;
    }

    p = SV_ChatBanSkipWhitespace(prefixEnd);
    if (*p == ':') {
        p++;
        p = SV_ChatBanSkipWhitespace(p);
        SV_ChatBanTrimTrailingWhitespace(p);
        *reason = p;
    } else if (*p == '\0') {
        *reason = p;
    } else {
        return CHATBAN_PARSE_INVALID;
    }

    if (!SV_ChatBanReasonIsSafe(*reason)) {
        return CHATBAN_PARSE_INVALID;
    }

    *subnet = (int)parsedSubnet;
    return CHATBAN_PARSE_OK;
}

qboolean SV_ChatBanFormatRecord(
    const char *address,
    int subnet,
    const char *reason,
    char *record,
    size_t recordSize
)
{
    int length;

    if (!record || !recordSize || !SV_ChatBanAddressTokenIsSafe(address)
        || subnet < 0 || subnet > SV_ChatBanMaximumPrefix(address)
        || !SV_ChatBanReasonIsSafe(reason)) {
        return qfalse;
    }

    if (reason[0]) {
        length = snprintf(record, recordSize, "%s %d:%s\n", address, subnet, reason);
    } else {
        length = snprintf(record, recordSize, "%s %d\n", address, subnet);
    }

    return length >= 0 && (size_t)length < recordSize;
}

qboolean SV_ChatBanEntryMatches(const serverChatBan_t *entry, netadr_t address)
{
    return entry && NET_CompareBaseAdrMask(entry->ip, address, entry->subnet);
}

int SV_ChatBanFindExactEntry(
    const serverChatBan_t *entries,
    int count,
    netadr_t address,
    int subnet
)
{
    int index;

    for (index = 0; index < count; index++) {
        if (entries[index].subnet == subnet
            && NET_CompareBaseAdrMask(entries[index].ip, address, subnet)) {
            return index;
        }
    }

    return -1;
}

int SV_ChatBanFindCoveringEntry(
    const serverChatBan_t *entries,
    int count,
    netadr_t address,
    int subnet
)
{
    int index;

    for (index = 0; index < count; index++) {
        if (entries[index].subnet <= subnet
            && NET_CompareBaseAdrMask(entries[index].ip, address, entries[index].subnet)) {
            return index;
        }
    }

    return -1;
}

int SV_ChatBanDeleteCoveredEntries(
    serverChatBan_t *entries,
    int *count,
    netadr_t address,
    int subnet
)
{
    int index = 0;
    int removed = 0;

    while (index < *count) {
        if (entries[index].subnet > subnet
            && NET_CompareBaseAdrMask(entries[index].ip, address, subnet)) {
            memmove(
                &entries[index],
                &entries[index + 1],
                (size_t)(*count - index - 1) * sizeof(*entries)
            );
            (*count)--;
            removed++;
        } else {
            index++;
        }
    }

    return removed;
}
