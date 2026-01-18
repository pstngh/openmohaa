/*
===========================================================================
Copyright (C) 2026 OpenMOHAA Contributors

This file is part of OpenMOHAA source code.

OpenMOHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMOHAA source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMOHAA source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// sv_admin.c - Server Administration System Implementation

#include "server.h"
#include "sv_admin.h"

//=============================================================================
// GLOBAL ADMIN DATA
//=============================================================================

adminEntry_t adminList[MAX_ADMINS];
adminSession_t adminSessions[MAX_ADMIN_SESSIONS];
muteEntry_t muteList[MAX_MUTES];

//=============================================================================
// UTILITY FUNCTIONS
//=============================================================================

/*
==================
SV_GetCallingClient

Gets the client_t pointer for the client executing the current command.
This is used to identify who is calling admin commands.
==================
*/
client_t* SV_GetCallingClient(void)
{
    // When a client executes a command via console, we need to find which client it is
    // In the engine, commands can be executed from different contexts:
    // 1. From server console (no client)
    // 2. From a client's console
    // 3. From RCON (redirectAddress is set)

    // For now, check if this is from RCON redirect
    if (svs.redirectAddress.type != NA_BAD) {
        // Search for client with matching IP
        int i;
        client_t *cl;
        for (i = 0, cl = svs.clients; i < sv_maxclients->integer; i++, cl++) {
            if (cl->state >= CS_CONNECTED) {
                if (NET_CompareAdr(cl->netchan.remoteAddress, svs.redirectAddress)) {
                    return cl;
                }
            }
        }
    }

    return NULL;
}

/*
==================
SV_FindAdminByUsername

Finds an admin entry by username
==================
*/
adminEntry_t* SV_FindAdminByUsername(const char *username)
{
    int i;

    for (i = 0; i < MAX_ADMINS; i++) {
        if (adminList[i].active && !Q_stricmp(adminList[i].username, username)) {
            return &adminList[i];
        }
    }

    return NULL;
}

/*
==================
SV_ComparePasswords

Compares two passwords (plain text for now)
==================
*/
qboolean SV_ComparePasswords(const char *input, const char *stored)
{
    return (strcmp(input, stored) == 0);
}

/*
==================
SV_TrimWhitespace

Trims leading and trailing whitespace from a string in-place
==================
*/
static void SV_TrimWhitespace(char *str)
{
    char *start = str;
    char *end;

    // Trim leading space
    while (*start && (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n')) {
        start++;
    }

    // All spaces?
    if (*start == 0) {
        *str = 0;
        return;
    }

    // Trim trailing space
    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        end--;
    }

    // Write new null terminator
    *(end + 1) = 0;

    // Move trimmed string to beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

/*
==================
SV_ParseINILine

Parses a single line from admins.ini
Format: login=username password=password level=2
Returns qtrue if successful
==================
*/
static qboolean SV_ParseINILine(const char *line, char *username, char *password, int *level)
{
    char buffer[256];
    char *token;
    char key[64];
    char value[128];
    int i;

    Q_strncpyz(buffer, line, sizeof(buffer));

    // Initialize output
    username[0] = '\0';
    password[0] = '\0';
    *level = 0;

    // Parse space-separated key=value pairs
    token = strtok(buffer, " \t");
    while (token != NULL) {
        // Find the '=' character
        char *equals = strchr(token, '=');
        if (equals) {
            // Split into key and value
            *equals = '\0';
            Q_strncpyz(key, token, sizeof(key));
            Q_strncpyz(value, equals + 1, sizeof(value));

            // Process the key-value pair
            if (!Q_stricmp(key, "login")) {
                Q_strncpyz(username, value, MAX_ADMIN_NAME);
            } else if (!Q_stricmp(key, "password")) {
                Q_strncpyz(password, value, MAX_ADMIN_PASSWORD);
            } else if (!Q_stricmp(key, "level")) {
                *level = atoi(value);
            }
        }

        token = strtok(NULL, " \t");
    }

    // Validate we got all required fields
    if (username[0] && password[0] && (*level == 1 || *level == 2)) {
        return qtrue;
    }

    return qfalse;
}

//=============================================================================
// ADMIN LIST MANAGEMENT
//=============================================================================

/*
==================
SV_InitAdminSystem

Initializes the admin system
Called on server startup
==================
*/
void SV_InitAdminSystem(void)
{
    int i;

    Com_Printf("Initializing admin system...\n");

    // Clear all admin data
    memset(adminList, 0, sizeof(adminList));
    memset(adminSessions, 0, sizeof(adminSessions));
    memset(muteList, 0, sizeof(muteList));

    svs.numAdmins = 0;
    svs.numMutes = 0;

    // Load admin list (ban list is managed by official OpenMOHAA ban system)
    SV_LoadAdminList();

    Com_Printf("Admin system initialized.\n");
}

/*
==================
SV_ShutdownAdminSystem

Shuts down the admin system
==================
*/
void SV_ShutdownAdminSystem(void)
{
    Com_Printf("Shutting down admin system...\n");

    // Clear all sessions
    memset(adminSessions, 0, sizeof(adminSessions));

    Com_Printf("Admin system shutdown complete.\n");
}

/*
==================
SV_LoadAdminList

Loads the admin list from admins.ini
Format: login=username password=password level=2
==================
*/
void SV_LoadAdminList(void)
{
    fileHandle_t f;
    int len;
    char *buffer;
    char *p;
    char line[256];
    int lineNum = 0;
    char username[MAX_ADMIN_NAME];
    char password[MAX_ADMIN_PASSWORD];
    int level;

    Com_Printf("Loading admin list from admins.ini...\n");

    // Clear existing admin list
    memset(adminList, 0, sizeof(adminList));
    svs.numAdmins = 0;

    // Try to open the file (reads from game directory)
    len = FS_FOpenFileRead("admins.ini", &f, qtrue, qtrue);
    if (!f) {
        Com_Printf("admins.ini not found. No admins loaded.\n");
        return;
    }

    // Allocate buffer for file contents
    buffer = (char *)Z_Malloc(len + 1);
    FS_Read(buffer, len, f);
    buffer[len] = '\0';
    FS_FCloseFile(f);

    // Parse line by line
    p = buffer;
    while (*p) {
        // Extract one line
        int i = 0;
        while (*p && *p != '\n' && *p != '\r' && i < sizeof(line) - 1) {
            line[i++] = *p++;
        }
        line[i] = '\0';

        // Skip newline characters
        while (*p == '\n' || *p == '\r') {
            p++;
        }

        lineNum++;

        // Trim whitespace
        SV_TrimWhitespace(line);

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';') {
            continue;
        }

        // Parse the line
        if (SV_ParseINILine(line, username, password, &level)) {
            if (svs.numAdmins >= MAX_ADMINS) {
                Com_Printf("Warning: Maximum number of admins (%d) reached. Ignoring remaining entries.\n", MAX_ADMINS);
                break;
            }

            // Add to admin list
            Q_strncpyz(adminList[svs.numAdmins].username, username, sizeof(adminList[svs.numAdmins].username));
            Q_strncpyz(adminList[svs.numAdmins].password, password, sizeof(adminList[svs.numAdmins].password));
            adminList[svs.numAdmins].level = level;
            adminList[svs.numAdmins].active = qtrue;
            svs.numAdmins++;

            Com_Printf("  Loaded admin: %s (Level %d)\n", username, level);
        } else {
            Com_Printf("Warning: Failed to parse line %d in admins.ini: %s\n", lineNum, line);
        }
    }

    Z_Free(buffer);
    Com_Printf("Loaded %d admin(s) from admins.ini\n", svs.numAdmins);
}

//=============================================================================
// SESSION MANAGEMENT
//=============================================================================

/*
==================
SV_FindAdminSession

Finds an admin session by IP address
==================
*/
adminSession_t* SV_FindAdminSession(netadr_t ip)
{
    int i;

    for (i = 0; i < MAX_ADMIN_SESSIONS; i++) {
        if (adminSessions[i].active && NET_CompareAdr(adminSessions[i].ip, ip)) {
            // Check if session has expired
            if (svs.time - adminSessions[i].lastActivity > ADMIN_SESSION_TIMEOUT) {
                // Session expired
                adminSessions[i].active = qfalse;
                return NULL;
            }
            return &adminSessions[i];
        }
    }

    return NULL;
}

/*
==================
SV_CreateAdminSession

Creates a new admin session or updates an existing one
==================
*/
adminSession_t* SV_CreateAdminSession(netadr_t ip, const char *username, int level)
{
    int i;
    adminSession_t *session;

    // Check if session already exists for this IP
    session = SV_FindAdminSession(ip);
    if (session) {
        // Update existing session
        Q_strncpyz(session->username, username, sizeof(session->username));
        session->level = level;
        session->lastActivity = svs.time;
        return session;
    }

    // Find a free slot
    for (i = 0; i < MAX_ADMIN_SESSIONS; i++) {
        if (!adminSessions[i].active) {
            // Initialize new session
            adminSessions[i].ip = ip;
            Q_strncpyz(adminSessions[i].username, username, sizeof(adminSessions[i].username));
            adminSessions[i].level = level;
            adminSessions[i].lastActivity = svs.time;
            adminSessions[i].active = qtrue;
            return &adminSessions[i];
        }
    }

    // No free slots - this shouldn't happen often
    Com_Printf("Warning: Admin session limit reached\n");
    return NULL;
}

/*
==================
SV_DestroyAdminSession

Destroys an admin session
==================
*/
void SV_DestroyAdminSession(netadr_t ip)
{
    adminSession_t *session = SV_FindAdminSession(ip);
    if (session) {
        session->active = qfalse;
    }
}

/*
==================
SV_UpdateSessionActivity

Updates the last activity time for a session
==================
*/
void SV_UpdateSessionActivity(adminSession_t *session)
{
    if (session) {
        session->lastActivity = svs.time;
    }
}

/*
==================
SV_CleanupExpiredSessions

Removes expired admin sessions
Called periodically from SV_Frame
==================
*/
void SV_CleanupExpiredSessions(void)
{
    int i;
    int cleaned = 0;

    for (i = 0; i < MAX_ADMIN_SESSIONS; i++) {
        if (adminSessions[i].active) {
            if (svs.time - adminSessions[i].lastActivity > ADMIN_SESSION_TIMEOUT) {
                adminSessions[i].active = qfalse;
                cleaned++;
            }
        }
    }

    if (cleaned > 0) {
        Com_DPrintf("Cleaned up %d expired admin session(s)\n", cleaned);
    }
}

/*
==================
SV_IsClientAdmin

Checks if a client has admin privileges of the required level
==================
*/
qboolean SV_IsClientAdmin(client_t *cl, int requiredLevel)
{
    adminSession_t *session;

    if (!cl) {
        return qfalse;
    }

    session = SV_FindAdminSession(cl->netchan.remoteAddress);
    if (!session) {
        return qfalse;
    }

    return (session->level >= requiredLevel);
}

/*
==================
SV_GetClientAdminSession

Gets the admin session for a client (or NULL if not logged in)
==================
*/
adminSession_t* SV_GetClientAdminSession(client_t *cl)
{
    if (!cl) {
        return NULL;
    }

    return SV_FindAdminSession(cl->netchan.remoteAddress);
}

//=============================================================================
// MUTE SYSTEM
//=============================================================================

/*
==================
SV_FindMuteEntry

Finds or creates a mute entry for an IP address
==================
*/
static muteEntry_t* SV_FindMuteEntry(netadr_t ip, qboolean create)
{
    int i;
    int firstFree = -1;

    // Search for existing entry
    for (i = 0; i < MAX_MUTES; i++) {
        if (muteList[i].active && NET_CompareAdr(muteList[i].ip, ip)) {
            return &muteList[i];
        }
        if (!muteList[i].active && firstFree == -1) {
            firstFree = i;
        }
    }

    // Create new entry if requested
    if (create && firstFree != -1) {
        muteList[firstFree].ip = ip;
        muteList[firstFree].active = qtrue;
        muteList[firstFree].chatMuted = qfalse;
        muteList[firstFree].tauntMuted = qfalse;
        muteList[firstFree].chatMuteTime = 0;
        muteList[firstFree].tauntMuteTime = 0;
        svs.numMutes++;
        return &muteList[firstFree];
    }

    return NULL;
}

/*
==================
SV_MutePlayerChat

Mutes a player's chat
==================
*/
void SV_MutePlayerChat(netadr_t ip)
{
    muteEntry_t *mute = SV_FindMuteEntry(ip, qtrue);
    if (mute) {
        mute->chatMuted = qtrue;
        mute->chatMuteTime = svs.time;
        Com_Printf("DEBUG: Muted chat for IP %s\n", NET_AdrToString(ip));
    } else {
        Com_Printf("DEBUG: Failed to find/create mute entry for IP %s\n", NET_AdrToString(ip));
    }
}

/*
==================
SV_MutePlayerTaunt

Mutes a player's taunts
==================
*/
void SV_MutePlayerTaunt(netadr_t ip)
{
    muteEntry_t *mute = SV_FindMuteEntry(ip, qtrue);
    if (mute) {
        mute->tauntMuted = qtrue;
        mute->tauntMuteTime = svs.time;
    }
}

/*
==================
SV_UnmutePlayerChat

Unmutes a player's chat
==================
*/
void SV_UnmutePlayerChat(netadr_t ip)
{
    muteEntry_t *mute = SV_FindMuteEntry(ip, qfalse);
    if (mute) {
        mute->chatMuted = qfalse;
    }
}

/*
==================
SV_UnmutePlayerTaunt

Unmutes a player's taunts
==================
*/
void SV_UnmutePlayerTaunt(netadr_t ip)
{
    muteEntry_t *mute = SV_FindMuteEntry(ip, qfalse);
    if (mute) {
        mute->tauntMuted = qfalse;
    }
}

/*
==================
SV_IsPlayerChatMuted

Checks if a player's chat is muted
==================
*/
qboolean SV_IsPlayerChatMuted(netadr_t ip)
{
    muteEntry_t *mute = SV_FindMuteEntry(ip, qfalse);
    qboolean result = (mute && mute->chatMuted);
    Com_Printf("DEBUG: IsPlayerChatMuted for %s: %s (mute=%p, chatMuted=%d)\n",
               NET_AdrToString(ip), result ? "YES" : "NO", mute, mute ? mute->chatMuted : 0);
    return result;
}

/*
==================
SV_IsPlayerTauntMuted

Checks if a player's taunts are muted
==================
*/
qboolean SV_IsPlayerTauntMuted(netadr_t ip)
{
    muteEntry_t *mute = SV_FindMuteEntry(ip, qfalse);
    return (mute && mute->tauntMuted);
}

/*
==================
SV_CleanupExpiredMutes

Clears all mutes (no longer time-based, cleared on map change)
Called from map change
==================
*/
void SV_CleanupExpiredMutes(void)
{
    // Clear all mutes
    memset(muteList, 0, sizeof(muteList));
    svs.numMutes = 0;
    Com_Printf("Cleared all mutes\n");
}

/*
==================
SV_RestoreMuteStatus

Restores mute status for a reconnecting player
Called from SV_DirectConnect
==================
*/
void SV_RestoreMuteStatus(client_t *cl, netadr_t from)
{
    // Mutes are IP-based and automatically persist
    // This function exists for potential future use
}

// LOGGING SYSTEM
//=============================================================================

/*
==================
SV_LogAdminAction

Logs an admin action to admin_log.txt
Format: [timestamp] admin_name (admin_ip) command target (target_ip)
==================
*/
void SV_LogAdminAction(adminSession_t *session, const char *command, const char *target, const char *targetIP)
{
    FILE *f;
    char timestamp[64];
    char logLine[512];
    char adminIP[64];
    char logPath[MAX_OSPATH];
    qtime_t time;

    if (!session) {
        return;
    }

    // Get current time
    Com_RealTime(&time);
    Com_sprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d",
                1900 + time.tm_year, time.tm_mon + 1, time.tm_mday,
                time.tm_hour, time.tm_min, time.tm_sec);

    // Get admin IP as string
    Q_strncpyz(adminIP, NET_AdrToString(session->ip), sizeof(adminIP));

    // Build log line
    if (targetIP) {
        Com_sprintf(logLine, sizeof(logLine), "[%s] %s (%s) %s %s (%s)\n",
                    timestamp, session->username, adminIP, command, target, targetIP);
    } else {
        Com_sprintf(logLine, sizeof(logLine), "[%s] %s (%s) %s %s\n",
                    timestamp, session->username, adminIP, command, target);
    }

    // Build path to server's main/admin.log
    Com_sprintf(logPath, sizeof(logPath), "%s/%s/admin.log",
                Cvar_VariableString("fs_basepath"),
                FS_GetCurrentGameDir());

    // Append to log file (writes to server directory, not AppData)
    f = Sys_FOpen(logPath, "ab");
    if (f) {
        fwrite(logLine, 1, strlen(logLine), f);
        fclose(f);
    } else {
        Com_Printf("Warning: Failed to open %s for writing\n", logPath);
    }
}

//=============================================================================
// ADMIN COMMANDS
//=============================================================================

/*
==================
SV_AdminLogin_f

Command: ad_login <username> <password>
Authenticates an admin and creates a session
==================
*/
void SV_AdminLogin_f(client_t *cl)
{
    const char *username;
    const char *password;
    adminEntry_t *admin;
    adminSession_t *session;

    if (!cl) {
        return;
    }

    if (Cmd_Argc() != 3) {
        SV_SendServerCommand(cl, "print \"Usage: ad_login <username> <password>\n\"");
        return;
    }

    username = Cmd_Argv(1);
    password = Cmd_Argv(2);

    // Check if any admins are loaded
    if (svs.numAdmins == 0) {
        SV_SendServerCommand(cl, "print \"No admins loaded. Check server console for errors.\n\"");
        return;
    }

    // Find admin by username
    admin = SV_FindAdminByUsername(username);
    if (!admin) {
        SV_SendServerCommand(cl, "print \"Login failed: Invalid username or password\n\"");
        return;
    }

    // Check password
    if (!SV_ComparePasswords(password, admin->password)) {
        SV_SendServerCommand(cl, "print \"Login failed: Invalid username or password\n\"");
        return;
    }

    // Create or update session
    session = SV_CreateAdminSession(cl->netchan.remoteAddress, admin->username, admin->level);
    if (!session) {
        SV_SendServerCommand(cl, "print \"Login failed: Server error\n\"");
        return;
    }

    SV_SendServerCommand(cl, "print \"Successfully logged in as %s (Level %d)\n\"", admin->username, admin->level);
    Com_Printf("Admin login: %s (%s) - Level %d\n", admin->username, NET_AdrToString(cl->netchan.remoteAddress), admin->level);
}

/*
==================
SV_AdminKick_f

Command: ad_kick <client_id>
Kicks a player by client ID
==================
*/
void SV_AdminKick_f(client_t *cl)
{
    adminSession_t *session;
    int clientId;
    client_t *target;
    char targetIP[64];
    char targetName[MAX_NAME_LENGTH];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() != 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_kick <client_id>\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    clientId = atoi(Cmd_Argv(1));

    if (clientId < 0 || clientId >= sv_maxclients->integer) {
        SV_SendServerCommand(cl, "print \"Invalid client ID: %d\n\"", clientId);
        return;
    }

    target = &svs.clients[clientId];

    if (target->state < CS_CONNECTED) {
        SV_SendServerCommand(cl, "print \"Client %d is not connected\n\"", clientId);
        return;
    }

    // Save target name and IP BEFORE kicking (kicking clears the name)
    Q_strncpyz(targetName, target->name, sizeof(targetName));
    Q_strncpyz(targetIP, NET_AdrToString(target->netchan.remoteAddress), sizeof(targetIP));

    // Announce the kick to all players (in-game)
    SV_SendServerCommand(NULL, "print \"[ADMIN] %s has been kicked\n\"", targetName);

    // Send kick message to the client
    SV_NET_OutOfBandPrint(&svs.netprofile, target->netchan.remoteAddress,
        "droperror\nYou have been kicked by an admin");

    // Kick the player (empty reason to avoid duplicate broadcast)
    SV_DropClient(target, "");

    // Log the action
    SV_LogAdminAction(session, "ad_kick", targetName, targetIP);

    Com_Printf("Admin %s kicked player %s (%s)\n", session->username, targetName, targetIP);
}

/*
==================
SV_AdminClientKick_f

Command: ad_clientkick <client_id>
Kicks a player by client ID
==================
*/
void SV_AdminClientKick_f(client_t *cl)
{
    adminSession_t *session;
    int clientId;
    client_t *target;
    char targetIP[64];
    char targetName[MAX_NAME_LENGTH];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() != 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_clientkick <client_id>\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    clientId = atoi(Cmd_Argv(1));

    if (clientId < 0 || clientId >= sv_maxclients->integer) {
        SV_SendServerCommand(cl, "print \"Invalid client ID\n\"");
        return;
    }

    target = &svs.clients[clientId];

    if (target->state < CS_CONNECTED) {
        SV_SendServerCommand(cl, "print \"Client %d is not connected\n\"", clientId);
        return;
    }

    // Save target name and IP BEFORE kicking (kicking clears the name)
    Q_strncpyz(targetName, target->name, sizeof(targetName));
    Q_strncpyz(targetIP, NET_AdrToString(target->netchan.remoteAddress), sizeof(targetIP));

    // Announce the kick to all players (in-game)
    SV_SendServerCommand(NULL, "print \"[ADMIN] %s has been kicked\n\"", targetName);

    // Send kick message to the client
    SV_NET_OutOfBandPrint(&svs.netprofile, target->netchan.remoteAddress,
        "droperror\nYou have been kicked by an admin");

    // Kick the player (empty reason to avoid duplicate broadcast)
    SV_DropClient(target, "");

    // Log the action
    SV_LogAdminAction(session, "ad_clientkick", targetName, targetIP);

    Com_Printf("Admin %s kicked client %d %s (%s)\n", session->username, clientId, targetName, targetIP);
}

/*
==================
SV_AdminBanIP_f

Command: ad_banip <ip_address>
Bans an IP address
==================
*/
void SV_AdminBanIP_f(client_t *cl)
{
    adminSession_t *session;
    const char *ipMask;
    char reason[256];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_SENIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as Level 2 admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_banip <ip_address> [reason]\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    ipMask = Cmd_Argv(1);

    // Build reason if provided
    if (Cmd_Argc() > 2) {
        Com_sprintf(reason, sizeof(reason), "Banned by %s: %s", session->username, Cmd_ArgsFrom(2));
    } else {
        Com_sprintf(reason, sizeof(reason), "Banned by admin %s", session->username);
    }

    // Use official ban system
    if (SV_AddBan_IP(ipMask, reason)) {
        SV_SendServerCommand(cl, "print \"Added ban: %s\n\"", ipMask);
        SV_LogAdminAction(session, "ad_banip", ipMask, NULL);
        Com_Printf("Admin %s banned IP: %s\n", session->username, ipMask);
    } else {
        SV_SendServerCommand(cl, "print \"Failed to add ban (already exists or list full)\n\"");
    }
}

/*
==================
SV_AdminBan_f

Command: ad_ban <client_id>
Bans a player by client ID (resolves their IP and adds to ban list)
==================
*/
void SV_AdminBan_f(client_t *cl)
{
    adminSession_t *session;
    int clientId;
    client_t *target;
    char targetIP[64];
    char targetName[MAX_NAME_LENGTH];
    char reason[256];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_SENIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as Level 2 admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_ban <client_id> [reason]\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    clientId = atoi(Cmd_Argv(1));

    if (clientId < 0 || clientId >= sv_maxclients->integer) {
        SV_SendServerCommand(cl, "print \"Invalid client ID\n\"");
        return;
    }

    target = &svs.clients[clientId];

    if (target->state < CS_CONNECTED) {
        SV_SendServerCommand(cl, "print \"Client %d is not connected\n\"", clientId);
        return;
    }

    // Save target name and IP BEFORE banning (banning clears the name)
    Q_strncpyz(targetName, target->name, sizeof(targetName));
    Q_strncpyz(targetIP, NET_AdrToString(target->netchan.remoteAddress), sizeof(targetIP));

    // Build reason for ban record
    if (Cmd_Argc() > 2) {
        Com_sprintf(reason, sizeof(reason), "Banned by %s: %s", session->username, Cmd_ArgsFrom(2));
    } else {
        Com_sprintf(reason, sizeof(reason), "Banned by %s", session->username);
    }

    // Announce BEFORE banning
    SV_SendServerCommand(NULL, "print \"[ADMIN] %s has been banned\n\"", targetName);

    // Use official ban system (this will drop the client)
    if (SV_AddBan_Client(target, reason)) {
        SV_LogAdminAction(session, "ad_ban", targetName, targetIP);
        Com_Printf("Admin %s banned client %d %s (%s)\n", session->username, clientId, targetName, targetIP);
    } else {
        SV_SendServerCommand(cl, "print \"Failed to add ban\n\"");
    }
}

/*
==================
SV_AdminUnbanIP_f

Command: ad_unbanip <ip_address>
Removes an IP address from the ban list
==================
*/
void SV_AdminUnbanIP_f(client_t *cl)
{
    adminSession_t *session;
    const char *ipMask;

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_SENIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as Level 2 admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() != 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_unbanip <ip_address>\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    ipMask = Cmd_Argv(1);

    // Use official ban system
    if (SV_RemoveBan_IP(ipMask)) {
        SV_SendServerCommand(cl, "print \"Removed ban: %s\n\"", ipMask);
        SV_LogAdminAction(session, "ad_unbanip", ipMask, NULL);
        Com_Printf("Admin %s unbanned IP: %s\n", session->username, ipMask);
    } else {
        SV_SendServerCommand(cl, "print \"Ban not found: %s\n\"", ipMask);
    }
}

/*
==================
SV_AdminDisableChat_f

Command: ad_dischat <client_id>
Disables chat for a player (24 hour duration)
==================
*/
void SV_AdminDisableChat_f(client_t *cl)
{
    adminSession_t *session;
    int clientId;
    client_t *target;
    char targetIP[64];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() != 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_dischat <client_id>\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    clientId = atoi(Cmd_Argv(1));

    if (clientId < 0 || clientId >= sv_maxclients->integer) {
        SV_SendServerCommand(cl, "print \"Invalid client ID\n\"");
        return;
    }

    target = &svs.clients[clientId];

    if (target->state < CS_CONNECTED) {
        SV_SendServerCommand(cl, "print \"Client %d is not connected\n\"", clientId);
        return;
    }

    // Mute the player's chat
    SV_MutePlayerChat(target->netchan.remoteAddress);

    // Get target IP for logging
    Q_strncpyz(targetIP, NET_AdrToString(target->netchan.remoteAddress), sizeof(targetIP));

    // Log the action
    SV_LogAdminAction(session, "ad_dischat", target->name, targetIP);

    SV_SendServerCommand(cl, "print \"Disabled chat for %s\n\"", target->name);
    SV_SendServerCommand(target, "print \"Your chat has been disabled by an admin\n\"");
    Com_Printf("Admin %s disabled chat for %s (%s)\n", session->username, target->name, targetIP);
}

/*
==================
SV_AdminDisableTaunt_f

Command: ad_distaunt <client_id>
Disables taunts for a player (24 hour duration)
==================
*/
void SV_AdminDisableTaunt_f(client_t *cl)
{
    adminSession_t *session;
    int clientId;
    client_t *target;
    char targetIP[64];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() != 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_distaunt <client_id>\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    clientId = atoi(Cmd_Argv(1));

    if (clientId < 0 || clientId >= sv_maxclients->integer) {
        SV_SendServerCommand(cl, "print \"Invalid client ID\n\"");
        return;
    }

    target = &svs.clients[clientId];

    if (target->state < CS_CONNECTED) {
        SV_SendServerCommand(cl, "print \"Client %d is not connected\n\"", clientId);
        return;
    }

    // Mute the player's taunts
    SV_MutePlayerTaunt(target->netchan.remoteAddress);

    // Get target IP for logging
    Q_strncpyz(targetIP, NET_AdrToString(target->netchan.remoteAddress), sizeof(targetIP));

    // Log the action
    SV_LogAdminAction(session, "ad_distaunt", target->name, targetIP);

    SV_SendServerCommand(cl, "print \"Disabled taunts for %s\n\"", target->name);
    SV_SendServerCommand(target, "print \"Your taunts have been disabled by an admin\n\"");
    Com_Printf("Admin %s disabled taunts for %s (%s)\n", session->username, target->name, targetIP);
}

/*
==================
SV_AdminSay_f

Command: ad_say <message>
Sends a message from admin to all players
==================
*/
void SV_AdminSay_f(client_t *cl)
{
    adminSession_t *session;
    const char *message;
    char fullMessage[256];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() < 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_say <message>\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    message = Cmd_ArgsFrom(1);

    // Build the message with admin prefix
    Com_sprintf(fullMessage, sizeof(fullMessage), "print \"[ADMIN] %s\n\"", message);

    // Send to all clients
    SV_SendServerCommand(NULL, "%s", fullMessage);

    // Log the action
    SV_LogAdminAction(session, "ad_say", message, NULL);

    Com_Printf("Admin %s: %s\n", session->username, message);
}

/*
==================
SV_AdminStatus_f

Command: ad_status
Shows all connected players with their IPs (Level 1+)
==================
*/
void SV_AdminStatus_f(client_t *cl)
{
    adminSession_t *session;
    int i;
    client_t *target;
    char buffer[2048];
    char *p = buffer;
    int remaining = sizeof(buffer);
    int written;

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    // Build status text (without "print" command yet)
    written = Com_sprintf(p, remaining, "ID  Name                 IP Address\n");
    p += written;
    remaining -= written;

    written = Com_sprintf(p, remaining, "--- -------------------- ---------------\n");
    p += written;
    remaining -= written;

    for (i = 0; i < sv_maxclients->integer; i++) {
        target = &svs.clients[i];
        if (target->state >= CS_CONNECTED) {
            written = Com_sprintf(p, remaining, "%3d %-20s %s\n",
                                  i, target->name, NET_AdrToString(target->netchan.remoteAddress));
            p += written;
            remaining -= written;

            if (remaining <= 100) {
                break;
            }
        }
    }

    // Send as single print command
    SV_SendServerCommand(cl, "print \"%s\"", buffer);
}

/*
==================
SV_AdminListAdmins_f

Command: ad_listadmins
Lists all currently logged-in admins
==================
*/
void SV_AdminListAdmins_f(client_t *cl)
{
    adminSession_t *session;
    int i;
    int count = 0;
    char buffer[2048];
    char *p = buffer;
    int remaining = sizeof(buffer);
    int written;

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    // Build admin list text (without "print" command yet)
    written = Com_sprintf(p, remaining, "Currently logged-in admins:\n");
    p += written;
    remaining -= written;

    for (i = 0; i < MAX_ADMIN_SESSIONS; i++) {
        if (adminSessions[i].active) {
            written = Com_sprintf(p, remaining, "  %s (Level %d) - %s\n",
                                  adminSessions[i].username,
                                  adminSessions[i].level,
                                  NET_AdrToString(adminSessions[i].ip));
            p += written;
            remaining -= written;
            count++;

            if (remaining <= 100) {
                break;
            }
        }
    }

    if (count == 0) {
        written = Com_sprintf(p, remaining, "  (none)\n");
        p += written;
        remaining -= written;
    }

    // Send as single print command
    SV_SendServerCommand(cl, "print \"%s\"", buffer);
}

/*
==================
SV_AdminListIPs_f

Command: ad_listips
Lists all banned IPs (Level 2 only)
==================
*/
void SV_AdminListIPs_f(client_t *cl)
{
    adminSession_t *session;
    int i, banCount;
    char buffer[2048];
    char *p = buffer;
    int remaining = sizeof(buffer);
    int written;
    char ipString[128];
    char reason[128];

    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_SENIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as Level 2 admin to use this command\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    // Get ban count from official ban system
    banCount = SV_GetBanCount();

    if (banCount == 0) {
        SV_SendServerCommand(cl, "print \"No banned IPs\n\"");
        return;
    }

    // Build ban list text (without "print" command yet)
    written = Com_sprintf(p, remaining, "Banned IPs (%d total):\n", banCount);
    p += written;
    remaining -= written;

    for (i = 0; i < banCount && i < 50; i++) {  // Limit to first 50 to avoid overflow
        if (SV_GetBanEntry(i, ipString, sizeof(ipString), reason, sizeof(reason))) {
            if (reason[0]) {
                written = Com_sprintf(p, remaining, "  %3d: %s - %s\n", i + 1, ipString, reason);
            } else {
                written = Com_sprintf(p, remaining, "  %3d: %s\n", i + 1, ipString);
            }
            p += written;
            remaining -= written;

            if (remaining <= 100) {
                break;
            }
        }
    }

    if (banCount > 50) {
        written = Com_sprintf(p, remaining, "  ... and %d more\n", banCount - 50);
        p += written;
        remaining -= written;
    }

    // Send as single print command
    SV_SendServerCommand(cl, "print \"%s\"", buffer);
}

/*
==================
SV_AdminMap_f

Command: ad_map <mapname>
Changes the current map (Level 1+)
==================
*/
void SV_AdminMap_f(client_t *cl)
{
    adminSession_t *session;
    const char *mapname;


    session = SV_GetClientAdminSession(cl);
    if (!session || session->level < ADMIN_LEVEL_JUNIOR) {
        SV_SendServerCommand(cl, "print \"You must be logged in as admin to use this command\n\"");
        return;
    }

    if (Cmd_Argc() != 2) {
        SV_SendServerCommand(cl, "print \"Usage: ad_map <mapname>\n\"");
        return;
    }

    SV_UpdateSessionActivity(session);

    mapname = Cmd_Argv(1);

    // Log the action
    SV_LogAdminAction(session, "ad_map", mapname, NULL);

    // Announce map change (in-game)
    SV_SendServerCommand(NULL, "print \"[ADMIN] Changing map to: %s\n\"", mapname);
    Com_Printf("Admin %s changing map to: %s\n", session->username, mapname);

    // Execute the map change
    Cbuf_AddText(va("map %s\n", mapname));
}

//=============================================================================
// COMMAND REGISTRATION
//=============================================================================

// NOTE: Admin commands are registered as client commands in the ucmds[] array
// in sv_client.c, not as server console commands. This allows players to execute
// them from their in-game console.
