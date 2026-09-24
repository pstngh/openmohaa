# Configuration and commands

## General configuration

This documentation currently only lists new changes that were introduced in OpenMoHAA. For a list of known settings, see [Server configuration](02-configuration-server.md).

If you want to use containers, see [Creating a Docker image](../02-running/04-docker.md).

### Home directory

OpenMoHAA uses a dedicated home directory by default for user data and mods. This behavior can be customized:

- `set fs_homepath Z:\openmohaa_data`: User data will be read and written in the directory located in `Z:\openmohaa_data`
- `set fs_homepath homedata`: The subdirectory `homedata` in the game directory will be used to read and store user data
- `set fs_homepath .`: Not recommended, the game directory will be used for storing user data, just like the original MOH:AA

#### Default paths by OS:

- Windows: `%APPDATA%\openmohaa`
- Linux: `~/.openmohaa`
- macOS: `~/Library/Application Support/openmohaa`

### Configure the network components

Network settings can be adjusted to use either IPv4, IPv6, or both. By default, IPv6 is disabled on dedicated servers.

- `set net_enabled 0`: Disable networking.
- `set net_enabled 1`: Enable IPv4 only (the default for dedicated servers).
- `set net_enabled 2`: Enable IPv6 only.
- `set net_enabled 3`: Enable both IPv4 and IPv6 (the default when running the standalone game).

> [!WARNING]
> The master server (using the GameSpy protocol) does not support IPv6. If IPv4 is disabled, the server won't show up in the public server list.

### Flood protection differences with MOH: Spearhead

Flood protection is turned off by default in OpenMoHAA (`sv_floodProtection 0`).

- In MOH: Allied Assault and OpenMoHAA, it monitors all commands.
- In MOH: Spearhead 2.0 and later, it monitors only text messages.

Flood protection prevents spam but can sometimes interfere with rapid actions like reloading and checking scores within a short period of time. It can be disabled with `set sv_floodProtection 0`.

For more details on preventing message spamming, check out the [Chat](#chat) section below.

### Updates

The game periodically checks for new versions in the GitHub project page in the background. Updates are not applied automatically, they must be downloaded and installed manually.

Update checking is enabled by default, but can be disabled with:
- `set net_enabled 0`, disables networking as mentioned aboe
- `set com_updatechecker_enabled 0`
- Compiling the project without libcurl support

If disabled, remember to check the project page for new versions. Updates can improve security and provide important fixes against exploits.

## Server configuration

### Optimization / Antichams

A new variable, `sv_netoptimize`, enables a feature that optimizes network bandwidth by not sending players information about others they can't see. For each client, the server optimizes by only transmitting data about players within their view. Clients will not receive information about players they can't see. This feature also helps protect against cheaters:

- `set sv_netoptimize 0`: Disable optimization - the default
- `set sv_netoptimize 1`: Enable optimization for entities that are moving
- `set sv_netoptimize 2`: Enable optimization, always

This option exists since **Medal of Honor: Allied Assault Breakthrough** 2.30, however it was improved in OpenMoHAA: sounds like footsteps will be sent so players don't get confused.

### Managing bans

Thanks to the [ioquake3](https://ioquake3.org/) project, IP bans are supported. Bans are saved in `serverbans.dat` by default, (modifiable with `sv_banFile` varaiable):

|Name       |Parameters                                      |Description
|-----------|------------------------------------------------|-----------
|rehashbans |                                                |Loads saved bans from the banlist file
|listbans   |                                                |Lists all banned IP addresses
|banaddr    |ip[*/subnet*] \| clientnum [*subnet*] [reason]  |Bans an IP through its address or through a client number, a subnet can be specified to ban a network range
|exceptaddr |ip[*/subnet*] \| clientnum [*subnet*]           |Adds an IP as an exception, for example IP ranges can be banned but one or more exceptions can be added
|bandel     |ip[*/subnet*] \| num                            |Unbans an IP address or a subnet, the entry number can be specified as an alternative
|exceptdel  |ip[*/subnet*] \| num                            |Removes a ban exception
|flushbans  |                                                |Removes all bans

Examples:

- `banaddr 192.168.5.2` bans IP address **192.168.5.2**.
- `banaddr 192.168.1.0/24` bans all **192.168.1.x** IP addresses (in the range **192.168.1.0**-**192.168.1.255**).
- `banaddr 2` bans the IP address of the client **#2**.
- `banaddr 4 24` bans the subnet of client **#4** - i.e if client .**#4** has IP **192.168.8.4**, then it bans all IPs ranging from **192.168.8.0**-**192.168.8.255**.
- `exceptaddr 3` ads the IP of client **#3** as an exception.
- `bandel 192.168.8.4` unbans **192.168.8.4**.
- `bandel 192.168.1.0/24` unbans the entire **192.168.1.0** subnet (IP ranging from **192.168.1.0**-**192.168.1.255**).

To calculate IP subnets, search for `IP subnet calculator` on Internet.

### Persistent chat bans

Persistent chat bans prevent selected IPv4 or IPv6 addresses and CIDR ranges
from sending text or taunts. They do not reject connections, disconnect
players, or restrict joining, movement, gameplay, downloads, userinfo, or
admin commands.

Chat bans are stored independently in `chatbans.dat`. Set another relative
filename with the archived `sv_chatBanFile` cvar. For safety, the server refuses
to load or write the file if its normalized path is the same as `sv_banFile`,
if `sv_banFile` is not a plain relative path, or if the name ends in `.pk3`,
`.qvm` or a library extension. Changes are saved only to the file the list was
last loaded from: after a failed load or a new `sv_chatBanFile`, run
`rehashchatbans` before adding or removing entries.

Each record uses `<address> <prefix>[:reason]`:

```text
203.0.113.7 32:repeated spam
2001:db8:1234:: 48
```

Blank lines and lines beginning with `//` are ignored. The list accepts exact
addresses and explicit CIDR ranges, has a maximum of 1024 entries, and has no
exceptions, expiration times, or temporary entries. Malformed addresses,
prefixes, paths, and unsafe reasons are rejected.

| Name | Parameters | Description |
|------|------------|-------------|
| `chatbanaddr` | `clientnum \| ip[/prefix] [reason]` | Adds and saves an exact client address or explicit CIDR range |
| `chatbandel` | `entry-number \| ip[/prefix]` | Removes one exact entry by list number or exact address/prefix |
| `listchatbans` | `[page]` | Lists saved entries and optional reasons |
| `rehashchatbans` | | Reloads `chatbans.dat` and refreshes connected clients |
| `flushchatbans` | | Removes only persistent chat bans |

Authenticated admins with access right `64` can use the corresponding client
commands:

| Name | Parameters | Description |
|------|------------|-------------|
| `ad_chatban` | `clientnum \| ip[/prefix] [reason]` | Adds and saves a persistent chat ban |
| `ad_chatunban` | `clientnum \| ip[/prefix]` | Removes an exact persistent chat-ban entry |
| `ad_listchatbans` | `[page]` | Lists entries and reasons privately to the admin |

Adding an exact address affects every connected client sharing that address.
A broader existing range prevents redundant narrower additions; adding a new
broader range replaces entries it contains. Removing an address never silently
removes a broader range: specify that exact CIDR or its list number.

Affected players receive one private notice, including the configured reason,
when the restriction becomes active or when they reconnect. Later blocked
attempts are silently discarded. Explicit additions and removals may announce
the names of affected connected players publicly, but never expose an address,
CIDR, or reason. Full details remain in the server log.

The persistent restriction covers `say`, `sayteam`, `tell`, ordinary
`dmmessage`, `vsay`, `vosay`, `vtell`, `instamsg`, and taunt-form `dmmessage`.
The existing `ad_dischat` and `ad_distaunt` commands remain independent,
temporary per-connection toggles.

## Game settings

### Chat

Chat messages are logged to console and in the logfile by default, without requiring to set the `developer` variable.

The in-game chat behavior can be adjusted:

- `set g_instamsg_allowed 0`: Disable voice instant messages.
- `set g_instamsg_minDelay x`: Minimum delay (ms) between voice messages (default 1000)
- `set g_textmsg_allowed 0`: Disable all text messages. `All`, `team` and `private` messages will be disabled.
- `set g_textmsg_minDelay x`: Minimum delay (ms) between text messages (default 1000)

Temporarily disabling text messages can be useful in situations where tensions arise in the chat. Otherwise, it's best to keep them enabled under normal circumstances.

### Balancing teams

This prevents players from joining teams with more players than others. Disabled by default.

It can be enabled with: `set g_teambalance 1`.

This feature is passive: it only checks the team sizes when someone tries to join, so it won't automatically balance teams during the game.

> [!NOTE]
> This check doesn't apply in server scripts; it only works when clients join teams directly.

### Bots

This server variant has two independent bot populations. Existing Roomba bots
keep their original behavior, slots, names, and public-player reporting. The
optional regular bots use a separate controller and separate game-only slots.

#### Permanent Roomba bots

Set `sv_numbots` to the exact permanent bot count. The value is capped at
`sv_maxclients`; connecting humans never reduce it. Reserve enough client slots
for both humans and Roombas. Roombas are reported as normal players to the
master server and have synthetic player-like pings.

Set Roomba names with `g_botx_name`, where `x` is the zero-based bot number:

```cpp
set g_bot0_name customname // The first bot spawned will be named customname
set g_bot1_name "Fast beat" // The second bot spawned will be named Fast beat
```

Bots will keep their name between restarts and new maps.

Example with five permanent bots and capacity for 16 humans:
```cpp
set sv_maxclients 21
set sv_numbots 5
```

> [!NOTE]
> These melee-only roomba bots do not use the navigation mesh, so no Recast
> build or navigation package is required.

#### Regular SMG bots

Regular bots navigate, strafe, lean in the strafe direction, and use only the
Allied or Axis SMG. They appear in the in-game scoreboard with `bot` instead of
a numeric ping. Their game-only slots are above `sv_maxclients`, so they do not
consume human slots and do not appear in the master-server player roster.

```cpp
set sv_maxregularbots 7 // Reserved capacity and maximum; set before loading the map
set sv_regularbot_minplayers 12 // Fill total population to 12 when capacity permits
set sv_regularbot_damage 100 // Bullet damage percentage, from 0 through 100
set sv_regularbot_aim_reaction_ms 350 // Delay before reacting to a newly acquired enemy
set sv_regularbot_aim_latency_ms 120 // Continuous lag while following enemy movement
set sv_regularbot_aim_turnspeed 220 // Maximum tracking turn rate in degrees per second
set sv_regularbot_aim_error_deg 2.5 // Maximum slowly drifting angular error
```

`sv_maxregularbots` is latched. The sum of `sv_maxclients` and
`sv_maxregularbots` cannot exceed the engine limit of 64.
`sv_regularbot_minplayers` counts humans and real-slot Roombas, then adds only
enough regular bots to reach the requested total, capped by
`sv_maxregularbots`. Humans count from the moment they connect, and no regular
bots run while no human is connected. The population, damage, and all four aim
settings can be changed while the map is running. Aim settings do not disable
strafing or matched lean behavior.

Set optional names with `g_regularbotx_name`:

```cpp
set g_regularbot0_name "Charlie"
set g_regularbot1_name "Baker"
```

The default `sv_maxregularbots 0` avoids loading or updating Recast navigation.
Enabling capacity builds the navigation mesh once when the map loads; frame
updates run only while at least one regular bot exists.

For the complete distinction and tuning details, see
[Bot settings](./03-configuration-bots.md).

#### Known issues with bots

- Bots may not properly detect or avoid minefields.
- Bots won't complete objectives. They only navigate the map and attack other players.
- Minefields may fully block bot paths. For example, on Omaha Beach, bots spawning at the West axis spawn may get stuck in the spawn area.
- Some obstacles might completely block bot paths.
