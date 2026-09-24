# Bot settings

## Global settings

### `sv_bots`

- **Default**: 0
- **Type**: integer

The engine clamps this value to `0` through `MAX_CLIENTS - sv_maxclients`
before allocating game clients. The macOS launcher keeps `sv_maxclients` at
`2` and therefore accepts `1` through `62` bots.

With the default `sv_sharedbots 0`, bot capacity is allocated when the map
loads. Bots can be removed and added back live within that capacity. A higher
`sv_bots` is kept and takes effect the next time a map is loaded with `map`;
`map_restart` keeps the current capacity.

### Launcher deathmatch rules

The macOS launcher enables cheats only for its private LAN bot session and
disables grenades, rockets, and landmines. For Spearhead and Breakthrough it
also enables classic Allied Assault sniper behavior and replaces the Kar98
mortar with the shotgun, and bot team deathmatch respawns immediately through
`g_instant_team_respawn`. The launcher's saved **Infinite ammo** option
controls the infinite ammo flag independently and defaults to on. Normal
engine and multiplayer launches retain their standard defaults.

With the infinite ammo flag (`dmflags` bit 14, value 16384) set, weapons in
this build never use ammunition, so they never need reloading. This applies
to every server running this build, not only to launcher matches.

### `g_no_grenades`

- **Default**: 0
- **Type**: boolean

Set this to `1` before loading a map to omit frag and smoke grenades from
multiplayer spawn loadouts. The bot launcher sets it automatically.

### `g_instant_team_respawn`

- **Default**: 0
- **Type**: boolean

Set this to `1` to respawn team players immediately, ignoring
`sv_team_spawn_interval`. Unlike that cvar, it is not saved to the config, so
it lasts only for the current session. The bot launcher sets it for bot team
deathmatch.

### `g_aalean`

- **Default**: 0
- **Type**: boolean

In Spearhead and Breakthrough, set this to `1` to use Allied Assault's lean
limit, input speed, return-to-center speed, and first-person camera roll. It
does not control pain animations. The server publishes the value to clients,
which predict the same lean behavior.

Regardless of this setting, this build allows leaning in every game mode,
including single-player, and allows leaning while moving in Spearhead and
Breakthrough without their lean-movement `dmflags` bit.

### `g_painanims`

- **Default**: 1
- **Type**: boolean

Controls Spearhead/Breakthrough player hit-reaction animations independently
of `g_aalean`. Set it to `0` to suppress those animation overlays. The two
cvars provide all four combinations of AA or SH/BT lean behavior with pain
animations enabled or disabled.

| Behavior | `g_aalean` | `g_painanims` |
| --- | ---: | ---: |
| SH/BT lean + SH/BT pain | 0 | 1 |
| AA lean without SH/BT pain (previous AA style) | 1 | 0 |
| AA lean + SH/BT pain | 1 | 1 |
| SH/BT lean without pain | 0 | 0 |

### `g_godmode`

- **Default**: 0
- **Type**: boolean

Set this to `1` to apply god mode to every hit a human player takes, so it
survives death and map changes. Bots are never protected, and it only takes
effect while cheats are enabled. The launcher's **God mode** option sets it.

### `g_accuracy`

- **Default**: 0
- **Type**: integer

Weapon accuracy for human players. Bots use `g_bot_spread` instead.

- `0`: Normal. Spread grows while firing, up to the amount a full clip produces.
- `1`: High. Spread does not grow, so every shot uses the weapon's base spread.
- `2`: Perfect. Shots have no spread at all.

### `g_bot_initial_spawn_delay`

- **Default**: 0
- **Type**: float (seconds)

#### Description

This sets how long the game should wait before spawning bots after loading a new map.

#### Usage

- `0`: Bots spawn instantly at map start (default).
- `5`: Bots spawn 5 seconds after the map begins.

#### Notes

- Applies only once when a new map has finished loading. It is not triggered on restarts or between rounds.
- Doesn't affect individual bot respawns during gameplay.

## Tuning behavior

The launcher derives these settings from its difficulty slider. They can also
be set directly in a config or on the command line. Out-of-range values are
clamped by the game, and reversed minimum/maximum pairs are interpreted in
ascending order.

### Aim and reaction

| Cvar | Default | Valid range | Description |
| --- | ---: | ---: | --- |
| `g_bot_attack_react_min_delay` | `0.2` | `0`-`10` seconds | Delay before a bot may fire at a newly acquired target, including a switch to another target. |
| `g_bot_aim_height_min` | `0.32` | `0`-`1` | Lowest aim height as a fraction of the enemy bounding box. The wider stock range reflects human vertical shot placement while retaining a torso-biased ceiling. |
| `g_bot_aim_height_max` | `0.55` | `0`-`1` | Highest aim height as a fraction of the enemy bounding box. The default keeps the selected aim point in the torso. |
| `g_bot_aim_error` | `20` | `0`-`400` units | Off-target error at the target on acquisition, applied across the line of sight so all of it becomes a miss; after settling, 60% persists as a smooth sideways/downward drift. The launcher presets use `30` at casual, `20` at default, and `10` at esports. |
| `g_bot_aim_settle_time` | `0.4` | `0`-`10` seconds | Time for acquisition error to decay smoothly to its persistent floor. |
| `g_bot_aim_latency` | `0` | `0`-`2000` ms | Makes the bot aim at a timestamped past target position. |

Enemy visibility is checked at several heights of the body, not only eye to
eye, so an enemy peeking over cover or partially hidden behind a railing or
ramp edge is still detected and engaged. The humanized aim height is kept
while the body around it is visible; when it falls behind cover, the aim
snaps to the closest visible part of the body - for an enemy firing over a
wall, the lowest visible point above the cover.

### Turning and weapon accuracy

| Cvar | Default | Valid range | Description |
| --- | ---: | ---: | --- |
| `g_bot_turn_speed` | `360` | `1`-`1080` degrees/second | Maximum turn rate. |
| `g_bot_turn_accel` | `15` | `0.1`-`100` | Rate at which the bot ramps up to its maximum turn rate. |
| `g_bot_spread` | `1` | `0`-`10` | Static multiple of each bullet's base spread. `0` is pinpoint; values above `1` are less accurate. Bot weapon bloom does not accumulate. Horizontal spread remains symmetric; vertical bot spread is mirrored downward at full magnitude to avoid accidental head/neck shots without concentrating bullets at zero deviation. The launcher presets use `6` at casual, `3.5` at default, and `2` at esports. |
| `g_bot_sniper` | `25` | `0`-`100` percent | Chance that a bot receives a sniper rifle. |
| `g_bot_stg` | `5` | `0`-`100` percent | Chance that an Axis bot receives an STG. Snipers take priority, so the effective STG percentage is capped by the remaining non-sniper percentage. |

Bot SMG loadouts are team-based rather than skin-based: Allied bots receive a
Thompson and Axis bots receive an MP40. Human loadouts remain nationality-based.

### Player health and high-damage weapons

In multiplayer, sniper rifles, the Mauser KAR98 rifle, and the shotgun scale
their bullet damage with `g_playerdmhealth`, using 100 health as the baseline.
For example, `g_playerdmhealth 200` gives those weapons twice their normal
damage. Other weapons, melee attacks, knockback, and single-player damage are
unchanged. Values are clamped to `1` through `1000`.

### Aggressive movement

This movement is intentionally part of the default bot behavior and has no
master enable/disable cvar. Strafing and matching lean are applied generally,
except while a bot stands still to fire a weapon that cannot be fired
accurately on the move. Enemy-relative radial movement engages only while an
enemy is within `g_bot_peek_distance`. Short randomized phases choose between
advancing, orbiting with no radial input, and retreating. Retreat becomes less
likely with distance, while the bot always backs away inside body-contact
range. The radial layer shapes direction only and preserves the full
running-speed command while the bot is moving, including through doorways and
narrow spaces. Imminent player collisions and backward wall impacts remove only
the entering movement component; they do not impose a wall buffer or disable
leaning.

| Cvar | Default | Valid range | Description |
| --- | ---: | ---: | --- |
| `g_bot_strafe_intensity` | `0.7` | `0`-`1` | Sideways movement intensity. |
| `g_bot_strafe_min_interval` | `400` | `50`-`10000` ms | Minimum time before changing strafe direction. |
| `g_bot_strafe_max_interval` | `900` | `50`-`10000` ms | Maximum time before changing strafe direction. |
| `g_bot_peek_min_interval` | `400` | `50`-`10000` ms | Minimum time before choosing a new advance/orbit/retreat state. |
| `g_bot_peek_max_interval` | `900` | `50`-`10000` ms | Maximum time before choosing a new advance/orbit/retreat state. |
| `g_bot_peek_distance` | `384` | `0`-`4096` units | Range inside which bots use enemy-relative radial movement. |

## Recording movement and aim reference sessions

### `g_movelog`

- **Default**: 0
- **Type**: boolean

`g_movelog 1` enables opt-in, server-side telemetry for comparing human and
bot play. The host needs the instrumented OpenMoHAA build; remote human
players can connect with an unmodified client. The recorder does not change
movement, aiming, weapon damage, or random-number state, and it does not log
IP addresses or passwords.

The server samples every connected player's authoritative state at 20 Hz. It
records raw movement/buttons, final position and velocity, view angles,
weapon/ammunition state, nearest-opponent distance and relative motion,
full-body wall clearance in eight directions, line of sight, angular aim
error, target-relative aim height, and the entity beneath the crosshair. Schema
3 additionally traces along and opposite the actual movement command, toward
and away from the opponent, including the hit entity, surface normal, and
start-solid state. Sight blockers, visibility transitions, crosshair impact
distance, client entry/exit, and the BSP map checksum are also recorded. Shots,
reloads, damage, deaths, and spawns are recorded separately at the exact frame
in which they happen.

The test-only schema 4 profile adds bot decision state to each frame without
performing additional traces or consuming random numbers. It records the
selected enemy, sight/attack/fire decisions, reaction time remaining, intended
and error-adjusted aim points, aim acquisition state, path and blocked-recovery
state, strafe clearance and doorway damping, radial phase and forced close
retreat, plus the result of the existing imminent-contact guard. Human rows use
neutral or sentinel values in these `bot_*` columns.

`bot_fire_decision` values are:

| Value | Meaning |
| ---: | --- |
| `0` | No attack decision this frame. |
| `1` | No valid target. |
| `2` | Target is out of sight. |
| `3` | Waiting for the reaction delay. |
| `4` | No active weapon. |
| `5` | No ammunition. |
| `6` | Target is outside weapon range. |
| `7` | Semi-automatic weapon animation is busy. |
| `8` | Waiting for semi-automatic spread to settle. |
| `9` | Bot intends to fire. |

To capture a reference match, enter these commands in the host console:

```text
set g_movelog 1
map dm/mapname
```

The recorder can also be enabled after a map has already loaded. At the end
of the session, close the files cleanly with:

```text
set g_movelog 0
```

The recorder keeps three persistent files under `telemetry` in the active game
directory (`main`, `mainta`, or `maintt`):

- `movement_frames.csv`: synchronized 20 Hz movement and aim rows.
- `movement_events.csv`: exact combat and lifecycle events.
- `movement_meta.txt`: map, game, movement, health, accuracy, and bot-tuning
  settings needed to reproduce each session.

Nothing is overwritten: new recordings append to these same three files.
Every period between enabling and disabling `g_movelog` has a unique
`session_id`, as does a recording continued after a map change or server
restart. The CSV rows and metadata blocks carry that ID so individual matches
can still be separated during analysis.

Recording stops with a console message when a file cannot be written or would
grow past 1 GiB. Archive or delete the three files to record again.

All three files must use the same telemetry schema. Before the first recording
with this schema 4 test build, archive or delete schema 3 copies of all three files.
The recorder refuses to mix schemas or append when only part of the triplet is
present. Once fresh schema 4 files exist, subsequent sessions append normally.

For bot tuning, record several rounds of human versus human play and several
rounds against bots on the same maps and settings. A normal client demo
(`record <name>` / `stoprecord`) or video is useful visual context, but the
three telemetry files contain the server-side data needed for quantitative
movement and aim comparison.

### `cl_movelog`

- **Default**: 0
- **Type**: boolean

`cl_movelog 1` records the local player's movement on any server without
requiring changes to that server. The macOS launcher exposes it as **Record my
movement**. It is passive: no commands are changed and no telemetry is sent
over the network.

Each recording creates three uniquely named files under `client_telemetry` in
the active game directory. A new set starts with every map, video restart, or
re-enable, and nothing is recorded during demo playback:

- `*_frames.csv`: predicted movement, view, weapon, target, and collision state
  at 50 Hz, or once per frame when the frame rate is lower. Full-player-hull
  traces measure 256-unit clearance in eight directions, along the movement
  command, and along actual velocity. Crosshair and enemy fields identify
  direct or aim-aligned visible targets with an explicit confidence value.
- `*_inputs.csv`: every discrete movement/button transition, including short
  strafe and lean taps that may fall between state samples.
- `*_meta.txt`: map, schema, trace ranges, and interpretation notes.

The client deliberately omits names, chat, and network addresses. Enemy
information is limited to entities sent by the server to the client, and local
positions are predicted rather than authoritative server state. Use
`cl_movelog 0` to flush and close an active recording cleanly.
