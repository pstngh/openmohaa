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
// sv_admin.h - Server Administration System

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// ADMIN SYSTEM CONSTANTS
//=============================================================================

#define MAX_ADMIN_NAME              32
#define MAX_ADMIN_PASSWORD          64
#define MAX_ADMINS                  128
#define MAX_ADMIN_SESSIONS          32
#define MAX_MUTES                   256

#define ADMIN_SESSION_TIMEOUT       3600000  // 1 hour in milliseconds
#define MUTE_DURATION               86400000 // 24 hours in milliseconds

#define ADMIN_LEVEL_NONE            0
#define ADMIN_LEVEL_JUNIOR          1        // Can kick, mute, message
#define ADMIN_LEVEL_SENIOR          2        // All Level 1 + ban/unban

//=============================================================================
// ADMIN DATA STRUCTURES
//=============================================================================

// Admin entry loaded from admins.ini
typedef struct adminEntry_s {
    char        username[MAX_ADMIN_NAME];
    char        password[MAX_ADMIN_PASSWORD];
    int         level;                      // 1 = Junior, 2 = Senior
    qboolean    active;                     // Is this slot in use?
} adminEntry_t;

// Active admin session (IP-based, survives reconnects)
typedef struct adminSession_s {
    netadr_t    ip;                         // Admin's IP address
    char        username[MAX_ADMIN_NAME];   // Logged-in username
    int         level;                      // Permission level
    int         lastActivity;               // Last activity timestamp (svs.time)
    qboolean    active;                     // Is this session active?
} adminSession_t;

// Mute entry (IP-based, persists across reconnects)
typedef struct muteEntry_s {
    netadr_t    ip;                         // Muted player's IP
    qboolean    chatMuted;                  // Is chat disabled?
    qboolean    tauntMuted;                 // Are taunts disabled?
    int         chatMuteTime;               // When chat mute was applied
    int         tauntMuteTime;              // When taunt mute was applied
    qboolean    active;                     // Is this slot in use?
} muteEntry_t;

//=============================================================================
// ADMIN SYSTEM INITIALIZATION
//=============================================================================

void SV_InitAdminSystem(void);
void SV_ShutdownAdminSystem(void);
void SV_LoadAdminList(void);
void SV_LoadBanListTxt(void);

//=============================================================================
// SESSION MANAGEMENT
//=============================================================================

adminSession_t* SV_FindAdminSession(netadr_t ip);
adminSession_t* SV_CreateAdminSession(netadr_t ip, const char *username, int level);
void SV_DestroyAdminSession(netadr_t ip);
void SV_UpdateSessionActivity(adminSession_t *session);
void SV_CleanupExpiredSessions(void);
qboolean SV_IsClientAdmin(client_t *cl, int requiredLevel);
adminSession_t* SV_GetClientAdminSession(client_t *cl);

//=============================================================================
// MUTE SYSTEM
//=============================================================================

void SV_MutePlayerChat(netadr_t ip);
void SV_MutePlayerTaunt(netadr_t ip);
void SV_UnmutePlayerChat(netadr_t ip);
void SV_UnmutePlayerTaunt(netadr_t ip);
qboolean SV_IsPlayerChatMuted(netadr_t ip);
qboolean SV_IsPlayerTauntMuted(netadr_t ip);
void SV_CleanupExpiredMutes(void);
void SV_RestoreMuteStatus(client_t *cl, netadr_t from);

//=============================================================================
// BAN SYSTEM
//=============================================================================

qboolean SV_AdminBanList_Add(const char *ipMask);
qboolean SV_AdminBanList_Remove(const char *ipMask);
qboolean SV_AdminBanList_Check(netadr_t from);
void SV_SaveBanList(void);
void SV_ListBans(void);

//=============================================================================
// LOGGING SYSTEM
//=============================================================================

void SV_LogAdminAction(adminSession_t *session, const char *command, const char *target, const char *targetIP);

//=============================================================================
// ADMIN COMMANDS
//=============================================================================

// Command implementations (client commands - receive client_t* parameter)
// NOTE: These are registered in the ucmds[] array in sv_client.c
void SV_AdminLogin_f(client_t *cl);
void SV_AdminKick_f(client_t *cl);
void SV_AdminClientKick_f(client_t *cl);
void SV_AdminBanIP_f(client_t *cl);
void SV_AdminBan_f(client_t *cl);
void SV_AdminUnbanIP_f(client_t *cl);
void SV_AdminDisableChat_f(client_t *cl);
void SV_AdminDisableTaunt_f(client_t *cl);
void SV_AdminSay_f(client_t *cl);
void SV_AdminStatus_f(client_t *cl);
void SV_AdminListAdmins_f(client_t *cl);
void SV_AdminListIPs_f(client_t *cl);
void SV_AdminMap_f(client_t *cl);

//=============================================================================
// UTILITY FUNCTIONS
//=============================================================================

client_t* SV_GetCallingClient(void);
adminEntry_t* SV_FindAdminByUsername(const char *username);
qboolean SV_ComparePasswords(const char *input, const char *stored);

#ifdef __cplusplus
}
#endif
