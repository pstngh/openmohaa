#pragma once

#include "../qcommon/q_shared.h"

// ---- Permission bitmask constants ----
#define ACCESSLEVEL_MAP         (1 << 0)   // ad_map
#define ACCESSLEVEL_RESTART     (1 << 1)   // ad_restart
#define ACCESSLEVEL_KICK        (1 << 2)   // ad_kick, ad_kickr, ad_clientkick, ad_clientkickr
#define ACCESSLEVEL_BAN         (1 << 3)   // ad_banip, ad_banipr, ad_banid, ad_banidr, ad_unbanip, ad_listips
#define ACCESSLEVEL_RCON        (1 << 4)   // ad_rcon
#define ACCESSLEVEL_LISTADMINS  (1 << 5)   // ad_listadmins
#define ACCESSLEVEL_DISCHAT     (1 << 6)   // ad_dischat
#define ACCESSLEVEL_DISTAUNT    (1 << 7)   // ad_distaunt

// Maximum number of admin accounts loadable from admins.ini
#define MAX_ADMIN_ACCOUNTS  64

// ---- Admin account entry (loaded from file) ----
typedef struct {
    char    login[64];
    char    password[64];
    int     rights;
} adminAccount_t;

// Forward declaration (defined in server.h as client_t)
struct client_s;

// ---- Public API ----
void        SV_AdminInit(void);
void        SV_AdminOnClientDisconnect(struct client_s *cl);
qboolean    SV_AdminHandleClientCommand(struct client_s *cl);
qboolean    SV_AdminShouldBlockClientCommand(struct client_s *cl, const char *cmdName);
