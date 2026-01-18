# OpenMOHAA Admin System

A comprehensive, native admin system for OpenMOHAA game servers with IP-based authentication, two-tier permissions, and persistent ban management.

## Features

- **IP-based Authentication**: Secure login system tied to client IP addresses
- **Two-tier Permission System**: Junior and Senior admin levels
- **Session Management**: 1-hour automatic session timeout with activity tracking
- **Integrated Ban System**: Uses official OpenMOHAA ban system (`serverbans.dat`) with full CIDR notation support
- **Chat & Taunt Mutes**: Temporary mutes that clear on map change
- **Comprehensive Logging**: All admin actions logged to `main/admin.log`
- **Cross-platform**: Works on all platforms supported by OpenMOHAA

## Installation

### 1. Build the Server

The admin system is compiled into the server binary. Build the server as normal:

```bash
mkdir build
cd build
cmake ..
make
```

### 2. Configure Admins

Create `main/admins.ini` in your server directory with the following format:

```ini
login=username1 password=password1 level=2
login=username2 password=password2 level=1
```

**Permission Levels:**
- **Level 1 (Junior Admin)**: Can kick and mute players
- **Level 2 (Senior Admin)**: Full permissions including bans

**Example:**
```ini
login=admin password=SecurePass123 level=2
login=moderator password=ModPass456 level=1
```

### 3. Start the Server

The admin system will automatically:
- Load the admin list from `main/admins.ini`
- Use the official OpenMOHAA ban system (`main/serverbans.dat`)
- Log all admin actions to `main/admin.log`

**File Locations:**
All files are stored in the server's `main/` directory for easy access and backup:
- `main/admins.ini` - Admin credentials configuration
- `main/serverbans.dat` - Banned IPs (managed by OpenMOHAA ban system)
- `main/admin.log` - Admin action audit log

## Admin Commands

All commands use the `ad_` prefix and are executed from the in-game console.

### Authentication

#### `ad_login <username> <password>`
Authenticate as an admin. Session lasts 1 hour with automatic activity tracking.

```
ad_login admin SecurePass123
```

**Response:**
- `Login successful! Welcome, admin (Level 2)` - Success
- `Invalid username or password` - Failed authentication
- `You are already logged in` - Already authenticated

---

### Player Management

#### `ad_kick <client_id>`
**Required Level:** 1 (Junior Admin)

Kick a player from the server by their client ID.

```
ad_kick 3
```

**Example:**
```
ad_kick 5
> Admin moderator kicked PlayerName (192.168.1.100)
```

#### `ad_ban <client_id>`
**Required Level:** 2 (Senior Admin)

Permanently ban a player by their IP address.

```
ad_ban 3
```

**Notes:**
- Adds the player's IP to `main/banlist.txt`
- Ban persists across server restarts
- Use `ad_unbanip` to remove the ban

---

### Ban Management

#### `ad_banip <ip_address>`
**Required Level:** 2 (Senior Admin)

Manually add an IP address to the ban list. Supports CIDR notation.

```
ad_banip 192.168.1.100
ad_banip 10.0.0.0/24
```

#### `ad_unbanip <ip_address>`
**Required Level:** 2 (Senior Admin)

Remove an IP address from the ban list.

```
ad_unbanip 192.168.1.100
```

#### `ad_listips`
**Required Level:** 1 (Junior Admin)

Display all banned IP addresses.

```
ad_listips
```

**Example Output:**
```
Banned IP Addresses:
192.168.1.100
10.0.0.0/24
Total: 2 banned IPs
```

---

### Mute System

Mutes are IP-based and automatically clear when:
- The map changes
- The server restarts

#### `ad_dischat <client_id>`
**Required Level:** 1 (Junior Admin)

Disable text chat for a player.

```
ad_dischat 3
```

**Notes:**
- Player can still see chat from others
- Muted player sees: "You have been muted by an admin"
- Clears on map change

#### `ad_distaunt <client_id>`
**Required Level:** 1 (Junior Admin)

Disable voice taunts for a player.

```
ad_distaunt 3
```

**Notes:**
- Blocks all voice chat/taunts
- Muted player sees: "Your taunts have been disabled by an admin"
- Clears on map change

---

### Communication

#### `ad_say <message>`
**Required Level:** 1 (Junior Admin)

Broadcast a message to all players with the `[ADMIN]` prefix.

```
ad_say Server restart in 5 minutes!
```

**Output:**
```
[ADMIN] Server restart in 5 minutes!
```

**Notes:**
- Message appears in-game chat
- Visible to all connected players
- Logged to admin_log.txt

---

### Server Management

#### `ad_map <mapname>`
**Required Level:** 2 (Senior Admin)

Change to a different map.

```
ad_map dm/mohdm1
ad_map obj/obj_team1
```

**Notes:**
- Clears all mutes (they reset on map change)
- Kicks players if the map fails to load

---

### Information Commands

#### `ad_status`
**Required Level:** 1 (Junior Admin)

Display all connected players with their client IDs and IP addresses.

```
ad_status
```

**Example Output:**
```
ID  Name                 IP Address
0   PlayerOne            192.168.1.100
1   PlayerTwo            192.168.1.101
2   AdminUser            38.133.36.82
```

#### `ad_listadmins`
**Required Level:** 1 (Junior Admin)

Show all configured admin accounts.

```
ad_listadmins
```

**Example Output:**
```
Configured Admins:
admin (Level 2)
moderator (Level 1)
Total: 2 admins
```

---

## File Formats

### admins.ini

Located in: `main/admins.ini`

Format:
```ini
login=username password=password level=level
```

**Rules:**
- One admin per line
- Usernames: Max 32 characters
- Passwords: Max 32 characters
- Levels: 1 (Junior) or 2 (Senior)
- Comments not supported

**Example:**
```ini
login=admin password=SecurePass123 level=2
login=moderator password=ModPass456 level=1
login=helper password=HelperPass789 level=1
```

---

### serverbans.dat

Located in: `main/serverbans.dat` (configurable with `sv_banFile` cvar)

**Format:** Managed by the official OpenMOHAA ban system

**Example:**
```
0 192.168.1.100 32
0 10.0.0.0 24:Banned by admin test: Cheating
0 172.16.5.50 32:Banned by admin moderator
```

**Format Details:**
- First field: 0 = ban, 1 = exception
- Second field: IP address
- Third field: Subnet mask (32 for single IP, 24 for /24 network, etc.)
- Optional: Reason after colon

**Notes:**
- Uses the same ban system as console commands (`banaddr`, `bandel`, `listbans`, etc.)
- Modified by admin commands (`ad_ban`, `ad_banip`, `ad_unbanip`) and console commands
- Full CIDR notation support (e.g., `10.0.0.0/24` bans 256 addresses)
- Supports ban exceptions for whitelisting
- Checked on every client connection

**Related Console Commands:**
- `rehashbans` - Reload bans from file
- `listbans` - List all bans
- `banaddr <ip>` - Ban an IP address
- `bandel <ip>` - Remove a ban
- `flushbans` - Remove all bans

---

### admin.log

Located in: `main/admin.log` (in server directory, not AppData)

**Format:** Automatic logging of all admin actions

**Example:**
```
[2026-01-18 15:30:45] admin (38.133.36.82) ad_login
[2026-01-18 15:31:12] admin (38.133.36.82) ad_kick PlayerName (192.168.1.100)
[2026-01-18 15:32:05] admin (38.133.36.82) ad_say Server restart in 5 minutes!
[2026-01-18 15:33:20] admin (38.133.36.82) ad_ban PlayerTwo (192.168.1.101)
```

**Notes:**
- Timestamp format: `[YYYY-MM-DD HH:MM:SS]`
- Includes admin username and IP address
- Records command used and target/message
- Automatically appends (never truncated)
- Written to server directory for easy access and backup

---

## Technical Details

### Session Management

- **Duration**: 1 hour from last activity
- **Activity Tracking**: Any admin command extends the session
- **Cleanup**: Expired sessions removed every 60 seconds
- **Storage**: In-memory, cleared on server restart

### Mute System

- **Storage**: In-memory mute list
- **Scope**: IP-based (survives reconnects)
- **Duration**: Until map change or server restart
- **Enforcement**: Server-side interception of `dmmessage` command
- **Types**:
  - Chat mute: Blocks text messages
  - Taunt mute: Blocks voice taunts (messages starting with `*XX`)

### Ban System

- **Storage**: `main/serverbans.dat` (persistent, configurable with `sv_banFile`)
- **Integration**: Uses official OpenMOHAA ban system
- **Format**: Binary format managed by engine (0/1 exception IP subnet:reason)
- **CIDR Support**: Full support (e.g., `192.168.1.0/24`, IPv6)
- **Features**: Ban exceptions, conflict detection, reason tracking
- **Checking**: On client connection via `SV_CheckDRDoS()`
- **Action**: Immediate disconnect with ban reason
- **Commands**: Works with both `ad_*` admin commands and console `ban*` commands

### Authentication

- **Method**: IP-based login with username/password
- **Security**: Passwords stored in plain text (use strong passwords and secure file permissions)
- **Validation**: Username and password must match exactly
- **Multi-login**: One active session per IP address

---

## Security Considerations

### File Permissions

Protect your admin configuration files:

```bash
chmod 600 main/admins.ini
chmod 644 main/banlist.txt
chmod 644 main/admin_log.txt
```

### Password Security

- Use strong, unique passwords for each admin
- Do **not** share admin credentials
- Passwords are stored in **plain text** - protect `admins.ini`
- Change passwords regularly

### Network Security

- Admin authentication is IP-based
- Sessions are tied to client IP addresses
- VPN/proxy changes will require re-authentication
- Consider firewall rules to restrict admin access

---

## Troubleshooting

### "main/admins.ini not found"

**Cause:** The file doesn't exist or is in the wrong location

**Solution:**
1. Create `main/admins.ini` in your server directory
2. Ensure the file has at least one admin entry
3. Check file permissions (must be readable by the server)

### "Invalid username or password"

**Cause:** Credentials don't match `admins.ini`

**Solution:**
1. Check username and password spelling (case-sensitive)
2. Verify the format in `admins.ini`: `login=user password=pass level=2`
3. Ensure no extra spaces or special characters
4. Reload the admin list by restarting the server

### "You must be logged in as admin to use this command"

**Cause:** Not authenticated or session expired

**Solution:**
1. Use `ad_login <username> <password>` to authenticate
2. If already logged in, your session may have expired (1-hour timeout)
3. Re-login if your IP address changed (VPN/reconnect)

### Mutes not working

**Cause:** Outdated build or server not restarted

**Solution:**
1. Rebuild the server with the latest code
2. Restart the server completely
3. Verify with debug output (if enabled) that mutes are being applied

### Ban list not persisting

**Cause:** Permission issues or incorrect file path

**Solution:**
1. Ensure `main/banlist.txt` is writable by the server
2. Check that the `main/` directory exists
3. Verify file permissions: `chmod 644 main/banlist.txt`

---

## Command Reference Quick Guide

| Command | Level | Description |
|---------|-------|-------------|
| `ad_login <user> <pass>` | - | Authenticate as admin |
| `ad_kick <id>` | 1 | Kick a player |
| `ad_ban <id>` | 2 | Ban a player |
| `ad_banip <ip>` | 2 | Ban an IP address |
| `ad_unbanip <ip>` | 2 | Remove IP ban |
| `ad_dischat <id>` | 1 | Mute player chat |
| `ad_distaunt <id>` | 1 | Mute player taunts |
| `ad_say <message>` | 1 | Broadcast message |
| `ad_map <mapname>` | 2 | Change map |
| `ad_status` | 1 | List players |
| `ad_listadmins` | 1 | List admin accounts |
| `ad_listips` | 1 | List banned IPs |

**Level Key:**
- **Level 1**: Junior Admin (kick, mute, info commands)
- **Level 2**: Senior Admin (all commands including ban)

---

## Development Notes

### Code Structure

- **sv_admin.h**: Header file with data structures and function declarations
- **sv_admin.c**: Core admin system implementation (~1700 lines)
- **sv_client.c**: Client command registration and mute enforcement
- **sv_init.c**: Initialization and cleanup on map change
- **sv_main.c**: Session cleanup timer

### Key Functions

- `SV_LoadAdminList()`: Loads admins from `admins.ini`
- `SV_LoadBanList()`: Loads banned IPs from `banlist.txt`
- `SV_AdminLogin_f()`: Handles authentication
- `SV_MutePlayerChat()`: Applies chat mute
- `SV_MutePlayerTaunt()`: Applies taunt mute
- `SV_IsPlayerChatMuted()`: Checks if player is chat-muted
- `SV_IsPlayerTauntMuted()`: Checks if player is taunt-muted
- `SV_CleanupExpiredMutes()`: Clears mutes on map change

### Client Command Flow

1. Player types command in console (e.g., `ad_kick 3`)
2. `SV_ClientCommand()` receives the command
3. `SV_ExecuteClientCommand()` looks up command in `ucmds[]` array
4. Matching function called with `client_t *cl` parameter
5. Function validates permissions and executes action

### Mute Enforcement

1. Player sends chat/taunt via `dmmessage` command
2. `SV_ExecuteClientCommand()` intercepts before game processing
3. Checks second argument:
   - Starts with `*XX` (2 digits) → Taunt
   - Otherwise → Chat
4. Calls `SV_IsPlayerChatMuted()` or `SV_IsPlayerTauntMuted()`
5. If muted, sends error message and returns (blocks execution)
6. If not muted, passes to game engine via `ge->ClientCommand()`

---

## Changelog

### Version 1.0 (2026-01-18)

**Initial Release**
- IP-based authentication system
- Two-tier permission hierarchy
- Session management with 1-hour timeout
- Integrated with official OpenMOHAA ban system
- Temporary mute system (chat and taunts)
- Comprehensive admin logging to server directory
- 13 admin commands
- File-based configuration

**Major Features**
- **Integrated Ban System**: Admin ban commands use official OpenMOHAA ban system
  - Shared ban list with console commands (`banaddr`, `bandel`, `listbans`)
  - Full CIDR notation support for IPv4 and IPv6
  - Ban reasons tracked and displayed
  - Ban exceptions/whitelist support
  - Automatic conflict detection
  - Single source of truth: `serverbans.dat`
- **Server Directory Storage**: All files stored in `main/` for easy access
  - `main/admins.ini` - Admin credentials
  - `main/serverbans.dat` - Banned IPs (official format)
  - `main/admin.log` - Action audit log

**Bug Fixes & Improvements**
- Fixed file paths to write to server directory instead of AppData
- Fixed mute enforcement (intercepts `dmmessage` command for both chat and taunts)
- Fixed output buffering for `ad_status` and `ad_listadmins`
- Changed `ad_say` prefix from "CONSOLE" to "ADMIN"
- Renamed `ad_banid` to `ad_ban` for consistency
- Changed file extensions to match OpenMOHAA conventions (.log instead of .txt)
- Removed 240+ lines of duplicate ban code

---

## License

This admin system is part of the OpenMOHAA project. See the main project license for details.

## Support

For issues, questions, or contributions, please visit the OpenMOHAA repository.
