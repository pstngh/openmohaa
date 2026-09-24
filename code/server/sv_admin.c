/*
===========================================================================
Copyright (C) 2024 the OpenMoHAA team

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

// sv_admin.c -- server-side admin subsystem

#include "server.h"
#include "sv_chatban.h"

// ---- Static storage ----
static adminAccount_t   adminAccounts[MAX_ADMIN_ACCOUNTS];
static int              adminAccountCount = 0;

#define ADMIN_BAN_LIST_PAGE_SIZE 40
#define ADMIN_ACCOUNT_LIST_PAGE_SIZE 20

static qboolean SV_AdminIsValidMapName(const char *mapname)
{
    const unsigned char *p = (const unsigned char *)mapname;

    if (!mapname[0] || mapname[0] == '/' || strstr(mapname, "..")) {
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

static qboolean SV_AdminIsValidAddressToken(const char *address)
{
    const unsigned char *p = (const unsigned char *)address;

    if (!address[0]) {
        return qfalse;
    }

    while (*p) {
        if ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f')
            || (*p >= 'A' && *p <= 'F') || *p == '.' || *p == ':'
            || *p == '/') {
            p++;
            continue;
        }

        return qfalse;
    }

    return qtrue;
}

static qboolean SV_AdminIsValidReason(const char *reason)
{
    const unsigned char *p = (const unsigned char *)reason;

    while (*p) {
        // Reasons are embedded in quoted engine commands and server messages.
        // The tokenizer has no escaped-quote syntax, so reject delimiters.
        if (*p < ' ' || *p == '"' || *p == ';') {
            return qfalse;
        }
        p++;
    }

    return qtrue;
}

static void SV_AdminSanitizeCommandText(char *text)
{
    for (; *text; text++) {
        if (*text == '"' || *text == '\\' || *text == ';'
            || *text == '\n' || *text == '\r') {
            *text = ' ';
        }
    }
}

// ---- Helper: extract a value after "key=" up to the next whitespace ----
static const char *SV_AdminExtractValue(const char *haystack, const char *key, char *out, int outSize)
{
    const char *start;
    const char *end;
    int len;

    start = strstr(haystack, key);
    if (!start) {
        return NULL;
    }

    start += strlen(key);

    // Find end of value (next whitespace or end of string)
    end = start;
    while (*end && *end != ' ' && *end != '\t' && *end != '\n' && *end != '\r') {
        end++;
    }

    len = (int)(end - start);
    if (len >= outSize) {
        len = outSize - 1;
    }

    memcpy(out, start, len);
    out[len] = '\0';

    return start;
}

/*
==================
SV_AdminInit

Load admin accounts from admins.ini in the game directory.
Format: login=<user> password=<pass> rights=<bitmask>
Lines starting with // are comments.
==================
*/
void SV_AdminInit(void)
{
    int             filelen;
    fileHandle_t    readfrom;
    char            *textbuf, *curpos, *endpos, *lineEnd;
    char            filepath[MAX_QPATH];
    char            rightsStr[32];

    adminAccountCount = 0;

    Com_sprintf(filepath, sizeof(filepath), "%s/admins.ini", FS_GetCurrentGameDir());

    filelen = FS_BaseDir_FOpenFileRead(filepath, &readfrom);
    if (filelen < 0) {
        Com_Printf("sv_admin: No admins.ini found at %s\n", filepath);
        return;
    }

    if (filelen < 2) {
        FS_FCloseFile(readfrom);
        return;
    }

    curpos = textbuf = Z_Malloc(filelen + 1);
    FS_Read(textbuf, filelen, readfrom);
    FS_FCloseFile(readfrom);

    textbuf[filelen] = '\0';
    endpos = textbuf + filelen;

    while (curpos < endpos && adminAccountCount < MAX_ADMIN_ACCOUNTS) {
        const char *p;

        // Find end of line
        lineEnd = curpos;
        while (lineEnd < endpos && *lineEnd != '\n') {
            lineEnd++;
        }

        // Null-terminate this line
        if (lineEnd < endpos) {
            *lineEnd = '\0';
        }

        // Trim leading whitespace
        p = curpos;
        while (*p == ' ' || *p == '\t') {
            p++;
        }

        // Skip blank or comment lines
        if (*p == '\0' || *p == '\r') {
            curpos = lineEnd + 1;
            continue;
        }
        if (p[0] == '/' && p[1] == '/') {
            curpos = lineEnd + 1;
            continue;
        }

        // Parse key=value tokens
        if (SV_AdminExtractValue(p, "login=", adminAccounts[adminAccountCount].login, sizeof(adminAccounts[adminAccountCount].login))
            && SV_AdminExtractValue(p, "password=", adminAccounts[adminAccountCount].password, sizeof(adminAccounts[adminAccountCount].password))
            && SV_AdminExtractValue(p, "rights=", rightsStr, sizeof(rightsStr)))
        {
            adminAccounts[adminAccountCount].rights = atoi(rightsStr);
            Com_Printf("sv_admin: Loaded admin account '%s' with rights %d\n",
                adminAccounts[adminAccountCount].login,
                adminAccounts[adminAccountCount].rights);
            adminAccountCount++;
        } else {
            Com_Printf("sv_admin: Malformed line in admins.ini: %s\n", p);
        }

        curpos = lineEnd + 1;
    }

    Z_Free(textbuf);
    Com_Printf("sv_admin: Loaded %d admin account(s)\n", adminAccountCount);
}

// ---- Helper: end an admin session ----
// Chat and taunt restrictions belong to the connection, not to the session,
// so logging out must not lift them.
static void SV_AdminClearSession(client_t *cl)
{
    cl->adminAuthenticated = qfalse;
    cl->adminRights = 0;
    cl->adminUsername[0] = '\0';
}

/*
==================
SV_AdminOnClientDisconnect

Clear admin session state when a client disconnects.
==================
*/
void SV_AdminOnClientDisconnect(client_t *cl)
{
    SV_AdminClearSession(cl);
    cl->adminChatDisabled = qfalse;
    cl->adminTauntDisabled = qfalse;
    cl->chatBanActive = qfalse;
    cl->chatBanNotified = qfalse;
}

// ---- Helper: check access level ----
static qboolean SV_AdminCheckAccess(client_t *cl, int required, const char *cmdName)
{
    if (!(cl->adminRights & required)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Insufficient permissions for %s.\n\"", cmdName);
        return qfalse;
    }
    return qtrue;
}

// ---- Helper: print a line to a specific client's console via stufftext echo ----
static Q_PRINTF_FUNC(2, 3) void SV_AdminConsoleEcho(client_t *cl, const char *fmt, ...)
{
    char raw[256];
    va_list argptr;

    va_start(argptr, fmt);
    Q_vsnprintf(raw, sizeof(raw), fmt, argptr);
    va_end(argptr);

    SV_AdminSanitizeCommandText(raw);
    SV_SendServerCommand(cl, "stufftext \"echo %s\"", raw);
}

// ---- Helper: find client by name ----
static client_t *SV_AdminFindClientByName(const char *name)
{
    int i;
    for (i = 0; i < sv_maxclients->integer; i++) {
        if (svs.clients[i].state >= CS_CONNECTED && !Q_stricmp(svs.clients[i].name, name)) {
            return &svs.clients[i];
        }
    }
    return NULL;
}

// ---- Helper: find client by number ----
static client_t *SV_AdminFindClientByNum(int num)
{
    if (num < 0 || num >= sv_maxclients->integer) {
        return NULL;
    }
    if (svs.clients[num].state < CS_CONNECTED) {
        return NULL;
    }
    return &svs.clients[num];
}

// ---- Helper: find client by a slot argument made only of digits ----
// atoi() would turn a mistyped name into slot 0.
static client_t *SV_AdminFindClientBySlotArg(const char *arg)
{
    int i;

    for (i = 0; arg[i] >= '0' && arg[i] <= '9'; i++) {
    }

    if (!i || arg[i] || i > 3) {
        return NULL;
    }

    return SV_AdminFindClientByNum(atoi(arg));
}

// ---- Helper: find client by name or numeric slot ----
static client_t *SV_AdminFindClientByHandle(const char *handle)
{
    int i;

    if (!handle || !*handle) {
        return NULL;
    }

    // Numeric handle
    for (i = 0; handle[i] >= '0' && handle[i] <= '9'; i++) {
    }

    if (!handle[i]) {
        return SV_AdminFindClientBySlotArg(handle);
    }

    return SV_AdminFindClientByName(handle);
}

// ---- Command: ad_login <user> <pass> ----
static void SV_Admin_Login(client_t *cl)
{
    const char *user;
    const char *pass;
    int i;

    if (Cmd_Argc() < 3) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_login <username> <password>\n\"");
        return;
    }

    user = Cmd_Argv(1);
    pass = Cmd_Argv(2);

    for (i = 0; i < adminAccountCount; i++) {
        if (!strcmp(adminAccounts[i].login, user) && !strcmp(adminAccounts[i].password, pass)) {
            cl->adminAuthenticated = qtrue;
            cl->adminRights = adminAccounts[i].rights;
            Q_strncpyz(cl->adminUsername, user, sizeof(cl->adminUsername));

            SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Admin login successful. Welcome, %s.\n\"", user);
            Com_Printf("sv_admin: %s logged in as admin '%s'\n", cl->name, user);
            return;
        }
    }

    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Admin login failed: invalid credentials.\n\"");
    // Authentication remains intentionally unlimited. Keep failed-attempt
    // diagnostics off the production console to avoid an I/O amplification path.
    Com_DPrintf("sv_admin: Failed admin login attempt by %s\n", cl->name);
}

// ---- Command: ad_logout ----
static void SV_Admin_Logout(client_t *cl)
{
    if (!cl->adminAuthenticated) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "You are not logged in.\n\"");
        return;
    }

    Com_Printf("sv_admin: %s logged out from admin '%s'\n", cl->name, cl->adminUsername);
    SV_AdminClearSession(cl);
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Admin logout successful.\n\"");
}

// ---- Command: ad_map <mapname> ----
static void SV_Admin_Map(client_t *cl)
{
    char mapname[MAX_QPATH];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_MAP, "ad_map")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_map <mapname>\n\"");
        return;
    }

    // Extract before Cbuf clobbers Cmd_Argv
    Q_strncpyz(mapname, Cmd_Argv(1), sizeof(mapname));

    if (!SV_AdminIsValidMapName(mapname)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid map name.\n\"");
        return;
    }

    Com_Printf("sv_admin: %s (%s) changing map to %s\n", cl->name, cl->adminUsername, mapname);
    SV_SendServerCommand(NULL, "print \"" HUD_MESSAGE_WHITE "Admin %s is changing map to %s\n\"", cl->adminUsername, mapname);
    Cbuf_ExecuteText(EXEC_APPEND, va("map \"%s\"\n", mapname));
}

// ---- Command: ad_restart ----
static void SV_Admin_Restart(client_t *cl)
{
    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_RESTART, "ad_restart")) {
        return;
    }

    Com_Printf("sv_admin: %s (%s) restarting server\n", cl->name, cl->adminUsername);
    SV_SendServerCommand(NULL, "print \"" HUD_MESSAGE_WHITE "Admin %s is restarting the server\n\"", cl->adminUsername);
    Cbuf_ExecuteText(EXEC_APPEND, "restart\n");
}

// ---- Command: ad_kick <name|clientnum> ----
static void SV_Admin_Kick(client_t *cl)
{
    client_t *target;
    char      targetHandle[64];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_KICK, "ad_kick")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_kick <playername|clientnum>\n\"");
        return;
    }

    Q_strncpyz(targetHandle, Cmd_Argv(1), sizeof(targetHandle));

    target = SV_AdminFindClientByHandle(targetHandle);
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Player not found: %s\n\"", targetHandle);
        return;
    }

    Com_Printf("sv_admin: %s (%s) kicked %s\n", cl->name, cl->adminUsername, target->name);
    SV_KickClientForReason(target, NULL, qfalse);
}

// ---- Command: ad_kickr <name|clientnum> <reason> ----
static void SV_Admin_KickReason(client_t *cl)
{
    client_t *target;
    char reason[256];
    char targetName[64];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_KICK, "ad_kickr")) {
        return;
    }

    if (Cmd_Argc() < 3) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_kickr <playername|clientnum> <reason>\n\"");
        return;
    }

    Q_strncpyz(targetName, Cmd_Argv(1), sizeof(targetName));
    Q_strncpyz(reason, Cmd_ArgsFrom(2), sizeof(reason));

    if (!SV_AdminIsValidReason(reason)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Reason contains unsupported characters.\n\"");
        return;
    }

    target = SV_AdminFindClientByHandle(targetName);
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Player not found: %s\n\"", targetName);
        return;
    }

    Com_Printf("sv_admin: %s (%s) kicked %s for: %s\n", cl->name, cl->adminUsername, target->name, reason);
    SV_KickClientForReason(target, reason, qfalse);
}

// ---- Command: ad_clientkick <clientnum> ----
static void SV_Admin_ClientKick(client_t *cl)
{
    client_t *target;
    int clientNum;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_KICK, "ad_clientkick")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_clientkick <clientnum>\n\"");
        return;
    }

    target = SV_AdminFindClientBySlotArg(Cmd_Argv(1));
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid client number.\n\"");
        return;
    }
    clientNum = target - svs.clients;

    Com_Printf("sv_admin: %s (%s) kicked client %d (%s)\n", cl->name, cl->adminUsername, clientNum, target->name);
    SV_KickClientForReason(target, NULL, qfalse);
}

// ---- Command: ad_clientkickr <clientnum> <reason> ----
static void SV_Admin_ClientKickReason(client_t *cl)
{
    client_t *target;
    int clientNum;
    char reason[256];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_KICK, "ad_clientkickr")) {
        return;
    }

    if (Cmd_Argc() < 3) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_clientkickr <clientnum> <reason>\n\"");
        return;
    }

    Q_strncpyz(reason, Cmd_ArgsFrom(2), sizeof(reason));

    if (!SV_AdminIsValidReason(reason)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Reason contains unsupported characters.\n\"");
        return;
    }

    target = SV_AdminFindClientBySlotArg(Cmd_Argv(1));
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid client number.\n\"");
        return;
    }
    clientNum = target - svs.clients;

    Com_Printf("sv_admin: %s (%s) kicked client %d (%s) for: %s\n", cl->name, cl->adminUsername, clientNum, target->name, reason);
    SV_KickClientForReason(target, reason, qfalse);
}

// ---- Command: ad_kicksilent <name|clientnum> [reason] ----
// Like ad_kick, but the other players are not told that anyone was kicked.
static void SV_Admin_KickSilent(client_t *cl)
{
    client_t *target;
    const char *reason = NULL;
    char      reasonBuffer[256];
    char      targetHandle[64];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_KICK, "ad_kicksilent")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_kicksilent <playername|clientnum> [reason]\n\"");
        return;
    }

    Q_strncpyz(targetHandle, Cmd_Argv(1), sizeof(targetHandle));
    if (Cmd_Argc() >= 3) {
        Q_strncpyz(reasonBuffer, Cmd_ArgsFrom(2), sizeof(reasonBuffer));
        if (!SV_AdminIsValidReason(reasonBuffer)) {
            SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Reason contains unsupported characters.\n\"");
            return;
        }
        reason = reasonBuffer;
    }

    target = SV_AdminFindClientByHandle(targetHandle);
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Player not found: %s\n\"", targetHandle);
        return;
    }

    Com_Printf("sv_admin: %s (%s) silently kicked %s\n", cl->name, cl->adminUsername, target->name);
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "%s was silently kicked\n\"", target->name);
    SV_KickClientForReason(target, reason, qtrue);
}

/*
===========
SV_AdminConvertWildcardToCIDR

Converts wildcard IP notation (e.g. "188.241.144.*") to CIDR notation
(e.g. "188.241.144.0/24") so admins can use the more intuitive wildcard
format in ban commands.
============
*/
static void SV_AdminConvertWildcardToCIDR(const char *input, char *output, size_t outputSize)
{
    int octets[4] = {0, 0, 0, 0};
    int numOctets = 0;
    const char *p = input;
    char token[16];
    int tokenLen;

    // Parse up to 4 dot-separated octets, stopping at wildcard
    while (numOctets < 4 && *p) {
        tokenLen = 0;
        while (*p && *p != '.' && tokenLen < (int)(sizeof(token) - 1)) {
            token[tokenLen++] = *p++;
        }
        token[tokenLen] = '\0';

        // Check for wildcard
        if (strchr(token, '*')) {
            break;
        }

        octets[numOctets] = atoi(token);
        numOctets++;

        if (*p == '.') {
            p++;
        }
    }

    // If no wildcard was found, pass through unchanged
    if (!strchr(input, '*')) {
        Q_strncpyz(output, input, outputSize);
        return;
    }

    switch (numOctets) {
    case 0:
        Com_sprintf(output, outputSize, "0.0.0.0/0");
        break;
    case 1:
        Com_sprintf(output, outputSize, "%d.0.0.0/8", octets[0]);
        break;
    case 2:
        Com_sprintf(output, outputSize, "%d.%d.0.0/16", octets[0], octets[1]);
        break;
    case 3:
        Com_sprintf(output, outputSize, "%d.%d.%d.0/24", octets[0], octets[1], octets[2]);
        break;
    default:
        Q_strncpyz(output, input, outputSize);
        break;
    }
}

// ---- Command: ad_banip <ip> ----
static void SV_Admin_BanIP(client_t *cl)
{
    char ip[64];
    char cidr[64];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_BAN, "ad_banip")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_banip <ip or wildcard, e.g. 192.168.1.*>\n\"");
        return;
    }

    // Extract before Cbuf clobbers Cmd_Argv
    Q_strncpyz(ip, Cmd_Argv(1), sizeof(ip));
    SV_AdminConvertWildcardToCIDR(ip, cidr, sizeof(cidr));

    if (!SV_AdminIsValidAddressToken(cidr)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid IP address.\n\"");
        return;
    }

    Com_Printf("sv_admin: %s (%s) banning IP %s\n", cl->name, cl->adminUsername, cidr);
    Cbuf_ExecuteText(EXEC_NOW, va("banaddr %s\n", cidr));
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "IP %s has been banned.\n\"", cidr);
}

// ---- Command: ad_banipr <ip> <reason> ----
static void SV_Admin_BanIPReason(client_t *cl)
{
    char ip[64];
    char cidr[64];
    char reason[256];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_BAN, "ad_banipr")) {
        return;
    }

    if (Cmd_Argc() < 3) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_banipr <ip or wildcard, e.g. 192.168.1.*> <reason>\n\"");
        return;
    }

    // Extract before Cbuf clobbers Cmd_Argv
    Q_strncpyz(ip, Cmd_Argv(1), sizeof(ip));
    SV_AdminConvertWildcardToCIDR(ip, cidr, sizeof(cidr));
    Q_strncpyz(reason, Cmd_ArgsFrom(2), sizeof(reason));

    if (!SV_AdminIsValidAddressToken(cidr)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid IP address.\n\"");
        return;
    }
    if (!SV_AdminIsValidReason(reason)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Reason contains unsupported characters.\n\"");
        return;
    }

    Com_Printf("sv_admin: %s (%s) banning IP %s for: %s\n", cl->name, cl->adminUsername, cidr, reason);
    Cbuf_ExecuteText(EXEC_NOW, va("banaddr %s \"%s\"\n", cidr, reason));
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "IP %s has been banned for: %s\n\"", cidr, reason);
}

// ---- Command: ad_banipsilent <ip> [reason] ----
// Like ad_banip, but the other players are not told that anyone was banned.
static void SV_Admin_BanIPSilent(client_t *cl)
{
    char ip[64];
    char cidr[64];
    char reason[256];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_BAN, "ad_banipsilent")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_banipsilent <ip or wildcard, e.g. 192.168.1.*> [reason]\n\"");
        return;
    }

    // Extract before Cbuf clobbers Cmd_Argv
    Q_strncpyz(ip, Cmd_Argv(1), sizeof(ip));
    SV_AdminConvertWildcardToCIDR(ip, cidr, sizeof(cidr));

    if (!SV_AdminIsValidAddressToken(cidr)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid IP address.\n\"");
        return;
    }

    if (Cmd_Argc() >= 3) {
        Q_strncpyz(reason, Cmd_ArgsFrom(2), sizeof(reason));
        if (!SV_AdminIsValidReason(reason)) {
            SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Reason contains unsupported characters.\n\"");
            return;
        }
        Com_Printf("sv_admin: %s (%s) silently banning IP %s for: %s\n", cl->name, cl->adminUsername, cidr, reason);
        Cbuf_ExecuteText(EXEC_NOW, va("banaddrsilent %s \"%s\"\n", cidr, reason));
    } else {
        Com_Printf("sv_admin: %s (%s) silently banning IP %s\n", cl->name, cl->adminUsername, cidr);
        Cbuf_ExecuteText(EXEC_NOW, va("banaddrsilent %s\n", cidr));
    }

    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "IP %s has been silently banned.\n\"", cidr);
}

// ---- Command: ad_banid <clientnum> ----
static void SV_Admin_BanID(client_t *cl)
{
    client_t *target;
    int clientNum;
    char ip[64];
    char targetName[64];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_BAN, "ad_banid")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_banid <clientnum>\n\"");
        return;
    }

    target = SV_AdminFindClientBySlotArg(Cmd_Argv(1));
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid client number.\n\"");
        return;
    }
    clientNum = target - svs.clients;

    Q_strncpyz(ip, NET_AdrToString(target->netchan.remoteAddress), sizeof(ip));
    Q_strncpyz(targetName, target->name, sizeof(targetName));
    Com_Printf("sv_admin: %s (%s) banning client %d (%s) IP %s\n", cl->name, cl->adminUsername, clientNum, targetName, ip);
    Cbuf_ExecuteText(EXEC_NOW, va("banaddr %s\n", ip));
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Client %d (%s) IP %s has been banned.\n\"", clientNum, targetName, ip);
}

// ---- Command: ad_banidr <clientnum> <reason> ----
static void SV_Admin_BanIDReason(client_t *cl)
{
    client_t *target;
    int clientNum;
    char ip[64];
    char targetName[64];
    char reason[256];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_BAN, "ad_banidr")) {
        return;
    }

    if (Cmd_Argc() < 3) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_banidr <clientnum> <reason>\n\"");
        return;
    }

    // Extract reason before Cbuf clobbers Cmd_Argv
    Q_strncpyz(reason, Cmd_ArgsFrom(2), sizeof(reason));

    if (!SV_AdminIsValidReason(reason)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Reason contains unsupported characters.\n\"");
        return;
    }

    target = SV_AdminFindClientBySlotArg(Cmd_Argv(1));
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid client number.\n\"");
        return;
    }
    clientNum = target - svs.clients;

    Q_strncpyz(ip, NET_AdrToString(target->netchan.remoteAddress), sizeof(ip));
    Q_strncpyz(targetName, target->name, sizeof(targetName));
    Com_Printf("sv_admin: %s (%s) banning client %d (%s) IP %s for: %s\n", cl->name, cl->adminUsername, clientNum, targetName, ip, reason);
    Cbuf_ExecuteText(EXEC_NOW, va("banaddr %s \"%s\"\n", ip, reason));
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Client %d (%s) IP %s has been banned for: %s\n\"", clientNum, targetName, ip, reason);
}

// ---- Command: ad_unbanip <ip> ----
static void SV_Admin_UnbanIP(client_t *cl)
{
    char ip[64];
    char cidr[64];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_BAN, "ad_unbanip")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_unbanip <ip or wildcard, e.g. 192.168.1.*>\n\"");
        return;
    }

    // Extract before Cbuf clobbers Cmd_Argv
    Q_strncpyz(ip, Cmd_Argv(1), sizeof(ip));
    SV_AdminConvertWildcardToCIDR(ip, cidr, sizeof(cidr));

    if (!SV_AdminIsValidAddressToken(cidr)) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid IP address.\n\"");
        return;
    }

    Com_Printf("sv_admin: %s (%s) unbanning IP %s\n", cl->name, cl->adminUsername, cidr);
    Cbuf_ExecuteText(EXEC_NOW, va("bandel %s\n", cidr));
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "IP %s has been unbanned.\n\"", cidr);
}

// ---- Command: ad_listips ----
static void SV_Admin_ListIPs(client_t *cl)
{
    int i;
    int first;
    int end;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_BAN, "ad_listips")) {
        return;
    }

    first = Cmd_Argc() > 1 ? atoi(Cmd_Argv(1)) : 0;
    if (first < 0) {
        first = 0;
    }

    if (!serverBansCount) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "No banned IPs.\n\"");
        return;
    }

    if (first >= serverBansCount) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Invalid offset %d; valid range is 0-%d.\n\"",
            first,
            serverBansCount - 1
        );
        return;
    }

    end = Q_min(first + ADMIN_BAN_LIST_PAGE_SIZE, serverBansCount);

    SV_SendServerCommand(
        cl,
        "print \"" HUD_MESSAGE_WHITE "--- Banned IPs %d-%d of %d ---\n\"",
        first,
        end > first ? end - 1 : 0,
        serverBansCount
    );

    for (i = first; i < end; i++) {
        const char *ip = NET_AdrToString(serverBans[i].ip);
        if (serverBans[i].reason[0]) {
            SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "%d: %s/%d %s(%s)\n\"",
                i, ip, serverBans[i].subnet,
                serverBans[i].isexception ? "[exception] " : "",
                serverBans[i].reason);
        } else {
            SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "%d: %s/%d %s\n\"",
                i, ip, serverBans[i].subnet,
                serverBans[i].isexception ? "[exception]" : "");
        }
    }

    if (end < serverBansCount) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "More entries: ad_listips %d\n\"",
            end
        );
    } else {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "--- End of ban list ---\n\"");
    }
}

// ---- Command: ad_rcon <command> ----
static void SV_Admin_Rcon(client_t *cl)
{
    char cmdString[1024];
    const char *raw;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_RCON, "ad_rcon")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_rcon <command>\n\"");
        return;
    }

    if (Cmd_Argc() == 2) {
        // A single argument may be the whole command wrapped in quotes.
        Q_strncpyz(cmdString, Cmd_Argv(1), sizeof(cmdString));
    } else {
        // Take the text after "ad_rcon" untokenized so quoted arguments such
        // as player names survive, as the engine rcon does.
        raw = Cmd_Cmd();
        while (*raw && *raw <= ' ') {
            raw++;
        }
        while (*raw > ' ') {
            raw++;
        }
        while (*raw && *raw <= ' ') {
            raw++;
        }
        Q_strncpyz(cmdString, raw, sizeof(cmdString));
    }

    Com_Printf("sv_admin: %s (%s) executing rcon: %s\n", cl->name, cl->adminUsername, cmdString);
    // Queue rather than run now: this runs while the admin's packet is being
    // parsed, and commands such as killserver free svs.clients under it.
    Cbuf_ExecuteText(EXEC_APPEND, va("%s\n", cmdString));
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Command queued.\n\"");
}

// ---- Command: ad_say <message> ----
static void SV_Admin_Say(client_t *cl)
{
    char text[1024];
    char *p;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_RCON, "ad_say")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_say <message>\n\"");
        return;
    }

    Q_strncpyz(text, Cmd_ArgsFrom(1), sizeof(text));
    p = text;

    if (*p == '"') {
        size_t len;

        p++;
        len = strlen(p);
        if (len > 0 && p[len - 1] == '"') {
            p[len - 1] = '\0';
        }
    }

    SV_AdminSanitizeCommandText(p);

    Com_Printf("sv_admin: %s (%s) say: %s\n", cl->name, cl->adminUsername, p);
    SV_SendServerCommand(NULL, "print \"" HUD_MESSAGE_CHAT_WHITE "console: %s\n\"", p);
}

// ---- Command: ad_listadmins ----
static void SV_Admin_ListAdmins(client_t *cl)
{
    int i;
    int first;
    int end;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_LISTADMINS, "ad_listadmins")) {
        return;
    }

    first = Cmd_Argc() > 1 ? atoi(Cmd_Argv(1)) : 0;
    first = Q_clamp_int(first, 0, Q_max(adminAccountCount - 1, 0));
    end   = Q_min(first + ADMIN_ACCOUNT_LIST_PAGE_SIZE, adminAccountCount);

    SV_SendServerCommand(
        cl,
        "print \"" HUD_MESSAGE_WHITE "--- Admin Accounts %d-%d of %d ---\n\"",
        adminAccountCount ? first : 0,
        end > first ? end - 1 : 0,
        adminAccountCount
    );

    for (i = first; i < end; i++) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "%d: %s (rights: %d)\n\"",
            i, adminAccounts[i].login, adminAccounts[i].rights);
    }

    if (end < adminAccountCount) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "More entries: ad_listadmins %d\n\"",
            end
        );
    }

    // List currently logged-in admins
    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "--- Online Admins ---\n\"");
    for (i = 0; i < sv_maxclients->integer; i++) {
        if (svs.clients[i].state >= CS_CONNECTED && svs.clients[i].adminAuthenticated) {
            SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "  %s (logged in as %s)\n\"",
                svs.clients[i].name, svs.clients[i].adminUsername);
        }
    }

    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "--- End of admin list ---\n\"");
}

// ---- Command: ad_status ----
static void SV_Admin_Status(client_t *cl)
{
    int i;
    int botPortIndex = 0;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_LISTADMINS, "ad_status")) {
        return;
    }

    SV_AdminConsoleEcho(cl, "slot score ping address name");
    SV_AdminConsoleEcho(cl, "---- ----- ---- ---------------------- ------------------------------");

    for (i = 0; i < sv_maxclients->integer; i++) {
        client_t *target = &svs.clients[i];
        const char *name;
        const char *address;
        int score;
        int ping;

        if (target->state < CS_CONNECTED) {
            continue;
        }

        name = target->name[0] ? target->name : "<unnamed>";
        address = SV_GetClientStatusAddress(target, &botPortIndex);
        score = target->gentity ? target->gentity->client->ps.stats[STAT_KILLS] : 0;
        ping = target->ping;

        SV_AdminConsoleEcho(cl, "%4d %5d %4d %-22s %s", i, score, ping, address, name);
    }
}

// ---- Command: ad_chatban <clientnum|ip[/prefix]> [reason] ----
static void SV_Admin_ChatBan(client_t *cl)
{
    netadr_t address;
    int subnet;
    serverChatBan_t added;
    const char *reason;
    char addressText[NET_ADDRSTRMAXLEN];
    char error[256];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_DISCHAT, "ad_chatban")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Usage: ad_chatban <clientnum|ip[/prefix]> [reason]\n\""
        );
        return;
    }

    reason = Cmd_Argc() >= 3 ? Cmd_ArgsFrom(2) : "";
    if (!SV_ChatBanReasonIsSafe(reason)) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Reason is too long or contains unsafe characters.\n\""
        );
        return;
    }

    if (!SV_ChatBanResolveTarget(
            Cmd_Argv(1), &address, &subnet, NULL, error, sizeof(error))
        || !SV_ChatBanAdd(address, subnet, reason, &added, error, sizeof(error))) {
        SV_AdminSanitizeCommandText(error);
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "%s.\n\"", error);
        return;
    }

    Q_strncpyz(addressText, NET_AdrToString(added.ip), sizeof(addressText));
    if (reason[0]) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Persistent chat ban added for %s/%d. Reason: %s\n\"",
            addressText,
            added.subnet,
            reason
        );
    } else {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Persistent chat ban added for %s/%d.\n\"",
            addressText,
            added.subnet
        );
    }

    Com_Printf(
        "sv_admin: %s (%s) added persistent chat ban %s/%d%s%s\n",
        cl->name,
        cl->adminUsername,
        addressText,
        added.subnet,
        reason[0] ? " for: " : "",
        reason
    );
    SV_ChatBanAnnounceMatches(added.ip, added.subnet, qtrue, cl->adminUsername);
}

// ---- Command: ad_chatunban <clientnum|ip[/prefix]> ----
static void SV_Admin_ChatUnban(client_t *cl)
{
    netadr_t address;
    int subnet;
    serverChatBan_t removed;
    char addressText[NET_ADDRSTRMAXLEN];
    char error[256];

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_DISCHAT, "ad_chatunban")) {
        return;
    }

    if (Cmd_Argc() != 2) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Usage: ad_chatunban <clientnum|ip[/prefix]>\n\""
        );
        return;
    }

    if (!SV_ChatBanResolveTarget(
            Cmd_Argv(1), &address, &subnet, NULL, error, sizeof(error))
        || !SV_ChatBanRemove(address, subnet, &removed, error, sizeof(error))) {
        SV_AdminSanitizeCommandText(error);
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "%s.\n\"", error);
        return;
    }

    Q_strncpyz(addressText, NET_AdrToString(removed.ip), sizeof(addressText));
    SV_SendServerCommand(
        cl,
        "print \"" HUD_MESSAGE_WHITE "Persistent chat ban removed for %s/%d.\n\"",
        addressText,
        removed.subnet
    );
    Com_Printf(
        "sv_admin: %s (%s) removed persistent chat ban %s/%d%s%s\n",
        cl->name,
        cl->adminUsername,
        addressText,
        removed.subnet,
        removed.reason[0] ? " originally recorded for: " : "",
        removed.reason
    );
    SV_ChatBanAnnounceMatches(removed.ip, removed.subnet, qfalse, cl->adminUsername);
}

// ---- Command: ad_listchatbans [page] ----
static void SV_Admin_ListChatBans(client_t *cl)
{
    int count;
    int page = 1;
    int pageCount;
    int first;
    int end;
    int index;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_DISCHAT, "ad_listchatbans")) {
        return;
    }

    if (Cmd_Argc() > 2) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Usage: ad_listchatbans [page]\n\""
        );
        return;
    }

    if (Cmd_Argc() == 2) {
        if (!SV_ChatBanParseInteger(Cmd_Argv(1), 1, SERVER_MAXCHATBANS, &page)) {
            SV_SendServerCommand(
                cl,
                "print \"" HUD_MESSAGE_WHITE "Usage: ad_listchatbans [page]\n\""
            );
            return;
        }
    }

    count = SV_ChatBanGetCount();
    if (!count) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "No persistent chat bans.\n\"");
        return;
    }

    pageCount = (count + ADMIN_BAN_LIST_PAGE_SIZE - 1) / ADMIN_BAN_LIST_PAGE_SIZE;
    if (page > pageCount) {
        SV_SendServerCommand(
            cl,
            "print \"" HUD_MESSAGE_WHITE "Invalid page %d; valid pages are 1-%d.\n\"",
            page,
            pageCount
        );
        return;
    }

    first = (page - 1) * ADMIN_BAN_LIST_PAGE_SIZE;
    end = Q_min(first + ADMIN_BAN_LIST_PAGE_SIZE, count);
    SV_AdminConsoleEcho(cl, "--- Persistent chat bans: page %d/%d ---", page, pageCount);

    for (index = first; index < end; index++) {
        const serverChatBan_t *entry = SV_ChatBanGetEntry(index);

        if (entry->reason[0]) {
            SV_AdminConsoleEcho(
                cl,
                "Chat ban #%d: %s/%d - Reason: %s",
                index + 1,
                NET_AdrToString(entry->ip),
                entry->subnet,
                entry->reason
            );
        } else {
            SV_AdminConsoleEcho(
                cl,
                "Chat ban #%d: %s/%d",
                index + 1,
                NET_AdrToString(entry->ip),
                entry->subnet
            );
        }
    }
}

// ---- Command: ad_dischat <clientnum> ----
static void SV_Admin_DisChat(client_t *cl)
{
    client_t *target;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_DISCHAT, "ad_dischat")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_dischat <clientnum>\n\"");
        return;
    }

    target = SV_AdminFindClientBySlotArg(Cmd_Argv(1));
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid client number.\n\"");
        return;
    }

    target->adminChatDisabled = !target->adminChatDisabled;

    if (target->adminChatDisabled) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Chat disabled for %s\n\"", target->name);
        SV_SendServerCommand(target, "print \"" HUD_MESSAGE_WHITE "Your chat has been disabled by an admin\n\"");
        SV_SendServerCommand(NULL, "print \"" HUD_MESSAGE_WHITE "Admin %s disabled chat for %s\n\"", cl->adminUsername, target->name);
        Com_Printf("sv_admin: %s (%s) disabled chat for %s\n", cl->name, cl->adminUsername, target->name);
    } else {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Chat re-enabled for %s\n\"", target->name);
        SV_SendServerCommand(target, "print \"" HUD_MESSAGE_WHITE "Your chat has been re-enabled by an admin\n\"");
        SV_SendServerCommand(NULL, "print \"" HUD_MESSAGE_WHITE "Admin %s re-enabled chat for %s\n\"", cl->adminUsername, target->name);
        Com_Printf("sv_admin: %s (%s) re-enabled chat for %s\n", cl->name, cl->adminUsername, target->name);
    }
}

// ---- Command: ad_distaunt <clientnum> ----
static void SV_Admin_DisTaunt(client_t *cl)
{
    client_t *target;

    if (!SV_AdminCheckAccess(cl, ACCESSLEVEL_DISTAUNT, "ad_distaunt")) {
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Usage: ad_distaunt <clientnum>\n\"");
        return;
    }

    target = SV_AdminFindClientBySlotArg(Cmd_Argv(1));
    if (!target) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Invalid client number.\n\"");
        return;
    }

    target->adminTauntDisabled = !target->adminTauntDisabled;

    if (target->adminTauntDisabled) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Taunts disabled for %s\n\"", target->name);
        SV_SendServerCommand(target, "print \"" HUD_MESSAGE_WHITE "Your taunts have been disabled by an admin\n\"");
        SV_SendServerCommand(NULL, "print \"" HUD_MESSAGE_WHITE "Admin %s disabled taunts for %s\n\"", cl->adminUsername, target->name);
        Com_Printf("sv_admin: %s (%s) disabled taunts for %s\n", cl->name, cl->adminUsername, target->name);
    } else {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Taunts re-enabled for %s\n\"", target->name);
        SV_SendServerCommand(target, "print \"" HUD_MESSAGE_WHITE "Your taunts have been re-enabled by an admin\n\"");
        SV_SendServerCommand(NULL, "print \"" HUD_MESSAGE_WHITE "Admin %s re-enabled taunts for %s\n\"", cl->adminUsername, target->name);
        Com_Printf("sv_admin: %s (%s) re-enabled taunts for %s\n", cl->name, cl->adminUsername, target->name);
    }
}

/*
==================
SV_AdminShouldBlockClientCommand

Check if a client command should be blocked due to admin restrictions.
Returns qtrue if the command is blocked.
==================
*/
qboolean SV_AdminShouldBlockClientCommand(client_t *cl, const char *cmdName)
{
    const char *messageToken = NULL;
    chatBanCommandType_t commandType;

    if (!cl->adminChatDisabled && !cl->adminTauntDisabled && !cl->chatBanActive) {
        return qfalse;
    }

    // dmmessage <mode> <text...>: the game treats the message as a taunt
    // when its first word (argument 2) is a taunt code.
    if (Cmd_Argc() > 2) {
        messageToken = Cmd_Argv(2);
    }
    commandType = SV_ChatBanClassifyCommand(cmdName, messageToken);

    if (commandType == CHATBAN_COMMAND_NONE) {
        return qfalse;
    }

    if (cl->chatBanActive) {
        SV_ChatBanNotifyClient(cl);
        return qtrue;
    }

    if (commandType == CHATBAN_COMMAND_TEXT && cl->adminChatDisabled) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Your chat has been disabled by an admin\n\"");
        return qtrue;
    }

    if (commandType == CHATBAN_COMMAND_TAUNT && cl->adminTauntDisabled) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Your taunts have been disabled by an admin\n\"");
        return qtrue;
    }

    return qfalse;
}

/*
==================
SV_AdminHandleClientCommand

Handle admin commands from a client.
Returns qtrue if the command was handled (should not be forwarded to game DLL).
==================
*/
qboolean SV_AdminHandleClientCommand(client_t *cl)
{
    const char *cmd = Cmd_Argv(0);

    // Normal gameplay commands take one three-character comparison and leave.
    if (Q_stricmpn(cmd, "ad_", 3)) {
        return qfalse;
    }

    // ad_login and ad_logout are always intercepted to prevent credentials leaking to game DLL
    if (!Q_stricmp(cmd, "ad_login")) {
        SV_Admin_Login(cl);
        return qtrue;
    }
    if (!Q_stricmp(cmd, "ad_logout")) {
        SV_Admin_Logout(cl);
        return qtrue;
    }

    if (!cl->adminAuthenticated) {
        SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "You are not logged in as admin. Use ad_login first.\n\"");
        return qtrue;
    }

    // Dispatch authenticated commands
    if (!Q_stricmp(cmd, "ad_map"))           { SV_Admin_Map(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_restart"))       { SV_Admin_Restart(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_kick"))          { SV_Admin_Kick(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_kickr"))         { SV_Admin_KickReason(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_kicksilent"))    { SV_Admin_KickSilent(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_clientkick"))    { SV_Admin_ClientKick(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_clientkickr"))   { SV_Admin_ClientKickReason(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_banip"))         { SV_Admin_BanIP(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_banipr"))        { SV_Admin_BanIPReason(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_banipsilent"))   { SV_Admin_BanIPSilent(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_banid"))         { SV_Admin_BanID(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_banidr"))        { SV_Admin_BanIDReason(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_unbanip"))       { SV_Admin_UnbanIP(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_listips"))       { SV_Admin_ListIPs(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_rcon"))          { SV_Admin_Rcon(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_say"))           { SV_Admin_Say(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_listadmins"))    { SV_Admin_ListAdmins(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_status"))        { SV_Admin_Status(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_chatban"))       { SV_Admin_ChatBan(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_chatunban"))     { SV_Admin_ChatUnban(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_listchatbans"))  { SV_Admin_ListChatBans(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_dischat"))       { SV_Admin_DisChat(cl); return qtrue; }
    if (!Q_stricmp(cmd, "ad_distaunt"))      { SV_Admin_DisTaunt(cl); return qtrue; }

    SV_SendServerCommand(cl, "print \"" HUD_MESSAGE_WHITE "Unknown admin command: %s\n\"", cmd);
    return qtrue;
}
