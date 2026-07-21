# Bot settings

## Global settings

### `sv_bots`

- **Default**: 0
- **Type**: integer

The engine clamps this value to `0` through `MAX_CLIENTS - sv_maxclients`
before allocating game clients. The macOS launcher keeps `sv_maxclients` at
`2` and therefore accepts `1` through `62` bots.

With the default `sv_sharedbots 0`, bot capacity is allocated when the map
starts. Bots can be removed and added back live, but increasing `sv_bots`
beyond that map's startup capacity requires a map restart.

### Launcher deathmatch rules

The macOS launcher enables cheats only for its private LAN bot session and
disables grenades, rockets, and landmines. For Spearhead and Breakthrough it
also enables classic Allied Assault sniper behavior and replaces the Kar98
mortar with the shotgun. The launcher's saved **Infinite ammo** option controls
that flag independently and defaults to on. Normal engine and multiplayer
launches retain their standard defaults.

### `g_no_grenades`

- **Default**: 0
- **Type**: boolean

Set this to `1` before loading a map to omit frag and smoke grenades from
multiplayer spawn loadouts. The bot launcher sets it automatically.

### `g_aalean`

- **Default**: 0
- **Type**: boolean

In Spearhead and Breakthrough, set this to `1` to use Allied Assault's lean
limit, input speed, return-to-center speed, and first-person camera roll. It
does not control pain animations.

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
| `g_bot_attack_react_min_delay` | `0.2` | `0`-`10` seconds | Delay before a bot starts shooting a newly seen enemy. |
| `g_bot_aim_height_min` | `0.32` | `0`-`1` | Lowest aim height as a fraction of the enemy bounding box. The wider stock range reflects human vertical shot placement while retaining a torso-biased ceiling. |
| `g_bot_aim_height_max` | `0.55` | `0`-`1` | Highest aim height as a fraction of the enemy bounding box. The default keeps the selected aim point in the torso. |
| `g_bot_aim_error` | `40` | `0`-`400` units | Off-target error on acquisition; after settling, 60% persists as a smooth horizontal/downward drift. |
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
master enable/disable cvar. Strafing and matching lean are applied generally.
Enemy-relative advance/retreat movement engages only while an enemy is within
`g_bot_peek_distance`: advance phases last longer than retreats outside 96
units, the phases become even near the opponent, and the bot always backs away
inside body-contact range. The radial layer shapes direction only and preserves
the full running-speed command while the bot is moving, including through
doorways and narrow spaces. Imminent
player collisions and backward wall impacts remove only the entering movement
component; they do not impose a wall buffer or disable leaning.

| Cvar | Default | Valid range | Description |
| --- | ---: | ---: | --- |
| `g_bot_strafe_intensity` | `0.7` | `0`-`1` | Sideways movement intensity. |
| `g_bot_strafe_min_interval` | `400` | `50`-`10000` ms | Minimum time before changing strafe direction. |
| `g_bot_strafe_max_interval` | `900` | `50`-`10000` ms | Maximum time before changing strafe direction. |
| `g_bot_peek_min_interval` | `1600` | `50`-`10000` ms | Minimum base time before changing advance/retreat direction. Outside 96 units, advance phases use 2x and retreat phases use 0.4x this randomized base. |
| `g_bot_peek_max_interval` | `3200` | `50`-`10000` ms | Maximum base time before changing advance/retreat direction. |
| `g_bot_peek_distance` | `384` | `0`-`4096` units | Range inside which bots start peeking and retreating. |
