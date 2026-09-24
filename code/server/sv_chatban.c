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

#include "server.h"
#include "sv_chatban.h"

#include <string.h>

#define CHATBAN_LIST_PAGE_SIZE 40
#define CHATBAN_MAX_FILE_SIZE  (256 * 1024)

static serverChatBan_t serverChatBans[SERVER_MAXCHATBANS];
static int             serverChatBansCount;
// File the in-memory list was last loaded from, empty after a failed load.
// Saves rewrite the whole file, so they are refused for any other file.
static char            serverChatBansLoadedPath[MAX_QPATH];

static Q_PRINTF_FUNC(3, 4) void QDECL SV_ChatBanSetError(
    char *error,
    int errorSize,
    const char *format,
    ...
)
{
    va_list args;

    if (!error || errorSize <= 0) {
        return;
    }

    va_start(args, format);
    Q_vsnprintf(error, errorSize, format, args);
    va_end(args);
}

static qboolean SV_ChatBanIsPositiveInteger(const char *text)
{
    const unsigned char *p = (const unsigned char *)text;

    if (!text || !text[0]) {
        return qfalse;
    }

    while (*p) {
        if (*p < '0' || *p > '9') {
            return qfalse;
        }
        p++;
    }

    return qtrue;
}

static qboolean SV_ChatBanValidateSubnet(netadr_t address, int subnet)
{
    if (address.type == NA_IP) {
        return subnet >= 0 && subnet <= 32;
    }

    if (address.type == NA_IP6) {
        return subnet >= 0 && subnet <= 128;
    }

    return qfalse;
}

static void SV_ChatBanClearPort(netadr_t *address)
{
    address->port = 0;
    address->scope_id = 0;
}

static void SV_ChatBanCanonicalizeAddress(netadr_t *address, int subnet)
{
    byte *bytes;
    int byteCount;
    int fullBytes;
    int remainingBits;
    int index;

    if (address->type == NA_IP) {
        bytes = address->ip;
        byteCount = 4;
    } else {
        bytes = address->ip6;
        byteCount = 16;
    }

    fullBytes = subnet / 8;
    remainingBits = subnet % 8;
    if (remainingBits && fullBytes < byteCount) {
        bytes[fullBytes] &= (byte)(0xffu << (8 - remainingBits));
        fullBytes++;
    }
    for (index = fullBytes; index < byteCount; index++) {
        bytes[index] = 0;
    }

    SV_ChatBanClearPort(address);
}

static qboolean SV_ChatBanValidateFile(char *filepath, int filepathSize, char *error, int errorSize)
{
    char chatName[MAX_QPATH];
    char banName[MAX_QPATH];
    const char *gameDirectory;

    if (!sv_chatBanFile || !sv_chatBanFile->string || !sv_chatBanFile->string[0]) {
        SV_ChatBanSetError(error, errorSize, "sv_chatBanFile is empty");
        return qfalse;
    }

    if (!SV_ChatBanCanonicalizeRelativePath(
            sv_chatBanFile->string, chatName, sizeof(chatName))) {
        SV_ChatBanSetError(
            error,
            errorSize,
            "sv_chatBanFile must be a safe relative path without parent traversal"
        );
        return qfalse;
    }

    // Writing a file with one of these extensions is a fatal error.
    if (Sys_DllExtension(chatName) || COM_CompareExtension(chatName, ".qvm")
        || COM_CompareExtension(chatName, ".pk3")) {
        SV_ChatBanSetError(
            error,
            errorSize,
            "sv_chatBanFile must not use a .pk3, .qvm or library extension"
        );
        return qfalse;
    }

    if (sv_banFile && sv_banFile->string && sv_banFile->string[0]) {
        const char *banFile = sv_banFile->string;

        // The ban module prefixes the game directory, so leading separators
        // name the same file.
        while (*banFile == '/' || *banFile == '\\') {
            banFile++;
        }

        if (!SV_ChatBanCanonicalizeRelativePath(banFile, banName, sizeof(banName))) {
            SV_ChatBanSetError(
                error,
                errorSize,
                "sv_banFile (%s) must be a plain relative path so chat bans cannot overwrite it",
                sv_banFile->string
            );
            return qfalse;
        }

        if (!Q_stricmp(chatName, banName)) {
            SV_ChatBanSetError(
                error,
                errorSize,
                "sv_chatBanFile must not point to sv_banFile (%s)",
                sv_banFile->string
            );
            return qfalse;
        }
    }

    gameDirectory = FS_GetCurrentGameDir();
    if (strlen(gameDirectory) + 1 + strlen(chatName) >= (size_t)filepathSize) {
        SV_ChatBanSetError(error, errorSize, "sv_chatBanFile path is too long");
        return qfalse;
    }

    Com_sprintf(
        filepath,
        filepathSize,
        "%s/%s",
        gameDirectory,
        chatName
    );
    return qtrue;
}

qboolean SV_ChatBanParseAddress(
    const char *token,
    netadr_t *address,
    int *subnet,
    char *error,
    int errorSize
)
{
    char  buffer[NET_ADDRSTRMAXLEN + 8];
    char *suffix;
    int parsedSubnet;
    qboolean expectsIPv6;

    if (!token || !token[0] || strlen(token) >= sizeof(buffer)) {
        SV_ChatBanSetError(error, errorSize, "Invalid address");
        return qfalse;
    }

    Q_strncpyz(buffer, token, sizeof(buffer));
    suffix = strrchr(buffer, '/');
    if (suffix) {
        if (strchr(buffer, '/') != suffix || !suffix[1]) {
            SV_ChatBanSetError(error, errorSize, "Invalid CIDR address: %s", token);
            return qfalse;
        }

        *suffix++ = '\0';
        if (!SV_ChatBanParseInteger(suffix, 0, 128, &parsedSubnet)) {
            SV_ChatBanSetError(error, errorSize, "Invalid CIDR prefix: %s", suffix);
            return qfalse;
        }
    } else {
        parsedSubnet = -1;
    }

    if (!SV_ChatBanAddressTokenIsSafe(buffer)) {
        SV_ChatBanSetError(error, errorSize, "Invalid address: %s", buffer);
        return qfalse;
    }

    expectsIPv6 = strchr(buffer, ':') != NULL;
    if (!NET_StringToAdr(buffer, address, expectsIPv6 ? NA_IP6 : NA_IP)
        || (expectsIPv6 && address->type != NA_IP6)
        || (!expectsIPv6 && address->type != NA_IP)) {
        SV_ChatBanSetError(error, errorSize, "Invalid address: %s", buffer);
        return qfalse;
    }

    SV_ChatBanClearPort(address);
    if (parsedSubnet < 0) {
        *subnet = address->type == NA_IP6 ? 128 : 32;
    } else {
        *subnet = (int)parsedSubnet;
    }

    if (!SV_ChatBanValidateSubnet(*address, *subnet)) {
        SV_ChatBanSetError(error, errorSize, "Invalid prefix %d for %s", *subnet, buffer);
        return qfalse;
    }

    SV_ChatBanCanonicalizeAddress(address, *subnet);
    return qtrue;
}

qboolean SV_ChatBanResolveTarget(
    const char *token,
    netadr_t *address,
    int *subnet,
    client_t **target,
    char *error,
    int errorSize
)
{
    int clientNum;

    if (target) {
        *target = NULL;
    }

    if (SV_ChatBanIsPositiveInteger(token)) {
        if (!SV_ChatBanParseInteger(token, 0, MAX_CLIENTS - 1, &clientNum)
            || !sv_maxclients || !svs.clients
            || clientNum < 0 || clientNum >= sv_maxclients->integer
            || svs.clients[clientNum].state < CS_CONNECTED) {
            SV_ChatBanSetError(error, errorSize, "Invalid client number: %s", token);
            return qfalse;
        }

        if (svs.clients[clientNum].netchan.remoteAddress.type != NA_IP
            && svs.clients[clientNum].netchan.remoteAddress.type != NA_IP6) {
            SV_ChatBanSetError(error, errorSize, "Client %d is not connected through IP", clientNum);
            return qfalse;
        }

        *address = svs.clients[clientNum].netchan.remoteAddress;
        SV_ChatBanClearPort(address);
        *subnet = address->type == NA_IP6 ? 128 : 32;
        if (target) {
            *target = &svs.clients[clientNum];
        }
        return qtrue;
    }

    return SV_ChatBanParseAddress(token, address, subnet, error, errorSize);
}

static int SV_ChatBanFindMatchingEntry(netadr_t address)
{
    int index;

    if (address.type != NA_IP && address.type != NA_IP6) {
        return -1;
    }

    for (index = 0; index < serverChatBansCount; index++) {
        if (SV_ChatBanEntryMatches(&serverChatBans[index], address)) {
            return index;
        }
    }

    return -1;
}

void SV_ChatBanNotifyClient(client_t *client)
{
    int match;

    if (!client || !client->chatBanActive || client->chatBanNotified
        || client->state < CS_ACTIVE) {
        return;
    }

    match = SV_ChatBanFindMatchingEntry(client->netchan.remoteAddress);
    if (match >= 0 && serverChatBans[match].reason[0]) {
        SV_SendServerCommand(
            client,
            "print \"" HUD_MESSAGE_WHITE
            "You are banned from chat and taunts.\nReason: %s\n\"",
            serverChatBans[match].reason
        );
    } else {
        SV_SendServerCommand(
            client,
            "print \"" HUD_MESSAGE_WHITE "You are banned from chat and taunts.\n\""
        );
    }

    client->chatBanNotified = qtrue;
}

void SV_ChatBanRefreshClient(client_t *client, qboolean notifyChange)
{
    qboolean wasActive;
    qboolean isActive;

    if (!client) {
        return;
    }

    wasActive = client->chatBanActive;
    isActive  = SV_ChatBanFindMatchingEntry(client->netchan.remoteAddress) >= 0;

    client->chatBanActive = isActive;
    if (isActive != wasActive) {
        client->chatBanNotified = qfalse;
    }

    if (!notifyChange || client->state < CS_ACTIVE) {
        return;
    }

    if (isActive && !wasActive) {
        SV_ChatBanNotifyClient(client);
    } else if (!isActive && wasActive) {
        SV_SendServerCommand(
            client,
            "print \"" HUD_MESSAGE_WHITE "Your persistent chat ban has been removed.\n\""
        );
    }
}

static void SV_ChatBanRefreshAll(qboolean notifyChange)
{
    int index;

    if (!sv_maxclients || !svs.clients) {
        return;
    }

    for (index = 0; index < sv_maxclients->integer; index++) {
        if (svs.clients[index].state >= CS_CONNECTED) {
            SV_ChatBanRefreshClient(&svs.clients[index], notifyChange);
        }
    }
}

static void SV_ChatBanInstallList(const serverChatBan_t *entries, int count, qboolean notifyChange)
{
    memset(serverChatBans, 0, sizeof(serverChatBans));
    if (count > 0) {
        memcpy(serverChatBans, entries, (size_t)count * sizeof(*entries));
    }
    serverChatBansCount = count;
    SV_ChatBanRefreshAll(notifyChange);
}

static qboolean SV_ChatBanWriteList(
    const serverChatBan_t *entries,
    int count,
    char *error,
    int errorSize
)
{
    char filepath[MAX_QPATH];
    char address[NET_ADDRSTRMAXLEN];
    char record[NET_ADDRSTRMAXLEN + MAX_CHATBAN_REASON_LENGTH + 32];
    fileHandle_t file;
    int index;
    size_t length;

    if (!SV_ChatBanValidateFile(filepath, sizeof(filepath), error, errorSize)) {
        return qfalse;
    }

    // After a failed load, or once sv_chatBanFile names another file, the
    // list in memory does not reflect that file; writing would replace it.
    if (Q_stricmp(filepath, serverChatBansLoadedPath)) {
        SV_ChatBanSetError(
            error,
            errorSize,
            "%s is not loaded; fix it if needed and run rehashchatbans",
            filepath
        );
        return qfalse;
    }

    file = FS_BaseDir_FOpenFileWrite_HomeState(filepath);
    if (!file) {
        SV_ChatBanSetError(error, errorSize, "Could not write %s", filepath);
        return qfalse;
    }

    for (index = 0; index < count; index++) {
        Q_strncpyz(address, NET_AdrToString(entries[index].ip), sizeof(address));
        if (!SV_ChatBanFormatRecord(
                address,
                entries[index].subnet,
                entries[index].reason,
                record,
                sizeof(record))) {
            FS_FCloseFile(file);
            SV_ChatBanSetError(error, errorSize, "Could not format chat-ban entry %d", index + 1);
            return qfalse;
        }

        length = strlen(record);
        if (FS_Write(record, length, file) != length) {
            FS_FCloseFile(file);
            SV_ChatBanSetError(error, errorSize, "Could not finish writing %s", filepath);
            return qfalse;
        }
    }

    FS_FCloseFile(file);
    return qtrue;
}

static qboolean SV_ChatBanLoadFile(char *error, int errorSize)
{
    char filepath[MAX_QPATH];
    char parseError[256];
    fileHandle_t file;
    long fileLength;
    size_t bytesRead;
    int lineNumber = 0;
    char *text;
    char *cursor;
    serverChatBan_t *loaded;
    int loadedCount = 0;

    if (!SV_ChatBanValidateFile(filepath, sizeof(filepath), error, errorSize)) {
        return qfalse;
    }

    serverChatBansLoadedPath[0] = '\0';

    loaded = Z_Malloc(sizeof(serverChatBans));
    memset(loaded, 0, sizeof(serverChatBans));

    fileLength = FS_BaseDir_FOpenFileRead(filepath, &file);
    if (fileLength < 0) {
        SV_ChatBanInstallList(loaded, 0, qtrue);
        Z_Free(loaded);
        Q_strncpyz(serverChatBansLoadedPath, filepath, sizeof(serverChatBansLoadedPath));
        Com_Printf("No chat-ban file found at %s; loaded an empty list.\n", filepath);
        return qtrue;
    }

    if (fileLength > CHATBAN_MAX_FILE_SIZE) {
        FS_FCloseFile(file);
        Z_Free(loaded);
        SV_ChatBanSetError(error, errorSize, "%s is too large", filepath);
        return qfalse;
    }

    text = Z_Malloc((size_t)fileLength + 1);
    bytesRead = FS_Read(text, (size_t)fileLength, file);
    FS_FCloseFile(file);
    if (bytesRead != (size_t)fileLength) {
        Z_Free(text);
        Z_Free(loaded);
        SV_ChatBanSetError(error, errorSize, "Could not read all of %s", filepath);
        return qfalse;
    }
    if (memchr(text, '\0', (size_t)fileLength)) {
        Z_Free(text);
        Z_Free(loaded);
        SV_ChatBanSetError(error, errorSize, "%s contains an embedded NUL byte", filepath);
        return qfalse;
    }
    text[fileLength] = '\0';

    cursor = text;
    while (*cursor) {
        char *line = cursor;
        char *newline = strchr(cursor, '\n');
        char *addressText;
        char *reason;
        int subnet;
        int parsedDefaultSubnet;
        netadr_t address;
        chatBanParseResult_t result;
        int covering;

        lineNumber++;
        if (newline) {
            *newline = '\0';
            cursor = newline + 1;
        } else {
            cursor += strlen(cursor);
        }

        result = SV_ChatBanParseRecord(line, &addressText, &subnet, &reason);
        if (result == CHATBAN_PARSE_SKIP) {
            continue;
        }
        if (result == CHATBAN_PARSE_INVALID
            || !SV_ChatBanParseAddress(
                addressText,
                &address,
                &parsedDefaultSubnet,
                parseError,
                sizeof(parseError))
            || !SV_ChatBanValidateSubnet(address, subnet)) {
            Com_Printf("Ignoring malformed chat-ban record at %s:%d\n", filepath, lineNumber);
            continue;
        }

        SV_ChatBanCanonicalizeAddress(&address, subnet);

        covering = SV_ChatBanFindCoveringEntry(loaded, loadedCount, address, subnet);
        if (covering >= 0) {
            Com_Printf(
                "Ignoring chat-ban record at %s:%d because entry %d already covers it.\n",
                filepath,
                lineNumber,
                covering + 1
            );
            continue;
        }

        SV_ChatBanDeleteCoveredEntries(loaded, &loadedCount, address, subnet);
        if (loadedCount >= SERVER_MAXCHATBANS) {
            Com_Printf("Ignoring excess chat-ban record at %s:%d\n", filepath, lineNumber);
            continue;
        }

        loaded[loadedCount].ip = address;
        loaded[loadedCount].subnet = subnet;
        Q_strncpyz(loaded[loadedCount].reason, reason, sizeof(loaded[loadedCount].reason));
        loadedCount++;
    }

    Z_Free(text);
    SV_ChatBanInstallList(loaded, loadedCount, qtrue);
    Z_Free(loaded);
    Q_strncpyz(serverChatBansLoadedPath, filepath, sizeof(serverChatBansLoadedPath));
    Com_Printf("Loaded %d persistent chat ban(s) from %s.\n", loadedCount, filepath);
    return qtrue;
}

qboolean SV_ChatBanAdd(
    netadr_t address,
    int subnet,
    const char *reason,
    serverChatBan_t *added,
    char *error,
    int errorSize
)
{
    serverChatBan_t *candidate;
    int candidateCount;
    int covering;
    int coveringSubnet;
    char existingAddress[NET_ADDRSTRMAXLEN];
    char addedAddress[NET_ADDRSTRMAXLEN];

    if (!reason) {
        reason = "";
    }

    SV_ChatBanClearPort(&address);
    if (!SV_ChatBanValidateSubnet(address, subnet)) {
        SV_ChatBanSetError(error, errorSize, "Invalid chat-ban address or prefix");
        return qfalse;
    }
    SV_ChatBanCanonicalizeAddress(&address, subnet);
    if (!SV_ChatBanReasonIsSafe(reason)) {
        SV_ChatBanSetError(error, errorSize, "Reason is too long or contains unsafe characters");
        return qfalse;
    }

    candidate = Z_Malloc(sizeof(serverChatBans));
    memcpy(candidate, serverChatBans, sizeof(serverChatBans));
    candidateCount = serverChatBansCount;

    covering = SV_ChatBanFindCoveringEntry(candidate, candidateCount, address, subnet);
    if (covering >= 0) {
        Q_strncpyz(existingAddress, NET_AdrToString(candidate[covering].ip), sizeof(existingAddress));
        coveringSubnet = candidate[covering].subnet;
        Z_Free(candidate);
        SV_ChatBanSetError(
            error,
            errorSize,
            "Chat ban #%d (%s/%d) already covers this address",
            covering + 1,
            existingAddress,
            coveringSubnet
        );
        return qfalse;
    }

    SV_ChatBanDeleteCoveredEntries(candidate, &candidateCount, address, subnet);
    if (candidateCount >= SERVER_MAXCHATBANS) {
        Z_Free(candidate);
        SV_ChatBanSetError(error, errorSize, "Maximum number of chat bans reached");
        return qfalse;
    }

    candidate[candidateCount].ip = address;
    candidate[candidateCount].subnet = subnet;
    Q_strncpyz(candidate[candidateCount].reason, reason, sizeof(candidate[candidateCount].reason));
    if (added) {
        *added = candidate[candidateCount];
    }
    candidateCount++;

    if (!SV_ChatBanWriteList(candidate, candidateCount, error, errorSize)) {
        Z_Free(candidate);
        return qfalse;
    }

    SV_ChatBanInstallList(candidate, candidateCount, qtrue);
    Z_Free(candidate);

    Q_strncpyz(addedAddress, NET_AdrToString(address), sizeof(addedAddress));
    Com_Printf(
        "Added persistent chat ban: %s/%d%s%s\n",
        addedAddress,
        subnet,
        reason[0] ? " - Reason: " : "",
        reason
    );
    return qtrue;
}

qboolean SV_ChatBanRemove(
    netadr_t address,
    int subnet,
    serverChatBan_t *removed,
    char *error,
    int errorSize
)
{
    serverChatBan_t *candidate;
    int candidateCount;
    int exact;
    int covering;
    char coveringAddress[NET_ADDRSTRMAXLEN];

    SV_ChatBanClearPort(&address);
    if (!SV_ChatBanValidateSubnet(address, subnet)) {
        SV_ChatBanSetError(error, errorSize, "Invalid chat-ban address or prefix");
        return qfalse;
    }
    SV_ChatBanCanonicalizeAddress(&address, subnet);

    exact = SV_ChatBanFindExactEntry(serverChatBans, serverChatBansCount, address, subnet);
    if (exact < 0) {
        covering = SV_ChatBanFindCoveringEntry(serverChatBans, serverChatBansCount, address, subnet);
        if (covering >= 0) {
            Q_strncpyz(
                coveringAddress,
                NET_AdrToString(serverChatBans[covering].ip),
                sizeof(coveringAddress)
            );
            SV_ChatBanSetError(
                error,
                errorSize,
                "Address is covered by chat ban #%d (%s/%d); remove that exact CIDR",
                covering + 1,
                coveringAddress,
                serverChatBans[covering].subnet
            );
        } else {
            SV_ChatBanSetError(error, errorSize, "No exact chat-ban entry matches that address");
        }
        return qfalse;
    }

    candidate = Z_Malloc(sizeof(serverChatBans));
    memcpy(candidate, serverChatBans, sizeof(serverChatBans));
    candidateCount = serverChatBansCount;
    if (removed) {
        *removed = candidate[exact];
    }
    memmove(
        &candidate[exact],
        &candidate[exact + 1],
        (size_t)(candidateCount - exact - 1) * sizeof(*candidate)
    );
    candidateCount--;

    if (!SV_ChatBanWriteList(candidate, candidateCount, error, errorSize)) {
        Z_Free(candidate);
        return qfalse;
    }

    SV_ChatBanInstallList(candidate, candidateCount, qtrue);
    Z_Free(candidate);
    return qtrue;
}

qboolean SV_ChatBanRemoveNumber(
    int number,
    serverChatBan_t *removed,
    char *error,
    int errorSize
)
{
    if (number < 1 || number > serverChatBansCount) {
        SV_ChatBanSetError(error, errorSize, "Invalid chat-ban entry number: %d", number);
        return qfalse;
    }

    return SV_ChatBanRemove(
        serverChatBans[number - 1].ip,
        serverChatBans[number - 1].subnet,
        removed,
        error,
        errorSize
    );
}

int SV_ChatBanGetCount(void)
{
    return serverChatBansCount;
}

const serverChatBan_t *SV_ChatBanGetEntry(int index)
{
    if (index < 0 || index >= serverChatBansCount) {
        return NULL;
    }

    return &serverChatBans[index];
}

static void SV_ChatBanSanitizeAnnouncement(char *text)
{
    for (; *text; text++) {
        if (*text == '"' || *text == '\\' || *text == ';' || *text == '\n' || *text == '\r') {
            *text = ' ';
        }
    }
}

void SV_ChatBanAnnounceMatches(
    netadr_t address,
    int subnet,
    qboolean added,
    const char *adminUsername
)
{
    int index;
    char safeName[MAX_NAME_LENGTH];
    char safeAdmin[64];

    if (!sv_maxclients || !svs.clients) {
        return;
    }

    if (adminUsername && adminUsername[0]) {
        Q_strncpyz(safeAdmin, adminUsername, sizeof(safeAdmin));
        SV_ChatBanSanitizeAnnouncement(safeAdmin);
    } else {
        safeAdmin[0] = '\0';
    }

    for (index = 0; index < sv_maxclients->integer; index++) {
        client_t *client = &svs.clients[index];

        if (client->state < CS_CONNECTED || !client->name[0]
            || !NET_CompareBaseAdrMask(client->netchan.remoteAddress, address, subnet)
            || (added && !client->chatBanActive)
            || (!added && client->chatBanActive)) {
            continue;
        }

        Q_strncpyz(safeName, client->name, sizeof(safeName));
        SV_ChatBanSanitizeAnnouncement(safeName);

        if (safeAdmin[0]) {
            SV_SendServerCommand(
                NULL,
                "print \"" HUD_MESSAGE_WHITE "Admin %s %s %s.\n\"",
                safeAdmin,
                added ? "chat-banned" : "removed the persistent chat ban for",
                safeName
            );
        } else {
            SV_SendServerCommand(
                NULL,
                "print \"" HUD_MESSAGE_WHITE "Server %s %s.\n\"",
                added ? "chat-banned" : "removed the persistent chat ban for",
                safeName
            );
        }
    }
}

static qboolean SV_ChatBanServerRunning(void)
{
    if (!com_sv_running || !com_sv_running->integer) {
        Com_Printf("Server is not running.\n");
        return qfalse;
    }

    return qtrue;
}

static void SV_ChatBanAddr_f(void)
{
    netadr_t address;
    int subnet;
    serverChatBan_t added;
    const char *reason;
    char error[256];

    if (!SV_ChatBanServerRunning()) {
        return;
    }
    if (Cmd_Argc() < 2) {
        Com_Printf("Usage: chatbanaddr <clientnum | ip[/prefix]> [reason]\n");
        return;
    }

    reason = Cmd_Argc() >= 3 ? Cmd_ArgsFrom(2) : "";
    if (!SV_ChatBanResolveTarget(
            Cmd_Argv(1), &address, &subnet, NULL, error, sizeof(error))
        || !SV_ChatBanAdd(address, subnet, reason, &added, error, sizeof(error))) {
        Com_Printf("Error: %s\n", error);
        return;
    }

    SV_ChatBanAnnounceMatches(added.ip, added.subnet, qtrue, NULL);
}

static void SV_ChatBanDel_f(void)
{
    serverChatBan_t removed;
    netadr_t address;
    int subnet;
    int number;
    char error[256];
    qboolean success;

    if (!SV_ChatBanServerRunning()) {
        return;
    }
    if (Cmd_Argc() != 2) {
        Com_Printf("Usage: chatbandel <entry-number | ip[/prefix]>\n");
        return;
    }

    if (SV_ChatBanIsPositiveInteger(Cmd_Argv(1))) {
        if (!SV_ChatBanParseInteger(
                Cmd_Argv(1), 1, SERVER_MAXCHATBANS, &number)) {
            Com_Printf("Error: Invalid chat-ban entry number: %s\n", Cmd_Argv(1));
            return;
        }
        success = SV_ChatBanRemoveNumber(number, &removed, error, sizeof(error));
    } else {
        success = SV_ChatBanParseAddress(Cmd_Argv(1), &address, &subnet, error, sizeof(error))
                  && SV_ChatBanRemove(address, subnet, &removed, error, sizeof(error));
    }

    if (!success) {
        Com_Printf("Error: %s\n", error);
        return;
    }

    Com_Printf(
        "Removed persistent chat ban: %s/%d%s%s\n",
        NET_AdrToString(removed.ip),
        removed.subnet,
        removed.reason[0] ? " - Reason: " : "",
        removed.reason
    );
    SV_ChatBanAnnounceMatches(removed.ip, removed.subnet, qfalse, NULL);
}

static void SV_ListChatBans_f(void)
{
    int page = 1;
    int pageCount;
    int first;
    int end;
    int index;

    if (!SV_ChatBanServerRunning()) {
        return;
    }
    if (Cmd_Argc() > 2) {
        Com_Printf("Usage: listchatbans [page]\n");
        return;
    }
    if (Cmd_Argc() == 2) {
        if (!SV_ChatBanParseInteger(Cmd_Argv(1), 1, SERVER_MAXCHATBANS, &page)) {
            Com_Printf("Usage: listchatbans [page]\n");
            return;
        }
    }

    if (!serverChatBansCount) {
        Com_Printf("No persistent chat bans.\n");
        return;
    }

    pageCount = (serverChatBansCount + CHATBAN_LIST_PAGE_SIZE - 1) / CHATBAN_LIST_PAGE_SIZE;
    if (page > pageCount) {
        Com_Printf("Invalid page %d; valid pages are 1-%d.\n", page, pageCount);
        return;
    }

    first = (page - 1) * CHATBAN_LIST_PAGE_SIZE;
    end = Q_min(first + CHATBAN_LIST_PAGE_SIZE, serverChatBansCount);
    Com_Printf("--- Persistent chat bans: page %d/%d ---\n", page, pageCount);

    for (index = first; index < end; index++) {
        Com_Printf(
            "Chat ban #%d: %s/%d%s%s\n",
            index + 1,
            NET_AdrToString(serverChatBans[index].ip),
            serverChatBans[index].subnet,
            serverChatBans[index].reason[0] ? " - Reason: " : "",
            serverChatBans[index].reason
        );
    }
}

static void SV_RehashChatBans_f(void)
{
    char error[256];

    if (!SV_ChatBanLoadFile(error, sizeof(error))) {
        Com_Printf("Error: %s\n", error);
    }
}

static void SV_FlushChatBans_f(void)
{
    char error[256];

    if (!SV_ChatBanServerRunning()) {
        return;
    }
    if (!serverChatBansCount) {
        Com_Printf("No persistent chat bans to remove.\n");
        return;
    }

    if (!SV_ChatBanWriteList(serverChatBans, 0, error, sizeof(error))) {
        Com_Printf("Error: %s\n", error);
        return;
    }

    SV_ChatBanInstallList(serverChatBans, 0, qtrue);
    Com_Printf("All persistent chat bans have been removed.\n");
    SV_SendServerCommand(
        NULL,
        "print \"" HUD_MESSAGE_WHITE "Server cleared all persistent chat bans.\n\""
    );
}

void SV_ChatBanAddOperatorCommands(void)
{
    Cmd_AddCommand("chatbanaddr", SV_ChatBanAddr_f);
    Cmd_AddCommand("chatbandel", SV_ChatBanDel_f);
    Cmd_AddCommand("listchatbans", SV_ListChatBans_f);
    Cmd_AddCommand("rehashchatbans", SV_RehashChatBans_f);
    Cmd_AddCommand("flushchatbans", SV_FlushChatBans_f);
}
