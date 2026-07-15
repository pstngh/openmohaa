# Bot settings

## Global settings

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
| `g_bot_aim_height_min` | `0.49` | `0`-`1` | Lowest aim height as a fraction of the enemy bounding box. |
| `g_bot_aim_height_max` | `0.65` | `0`-`1` | Highest aim height as a fraction of the enemy bounding box. |
| `g_bot_aim_error` | `40` | `0`-`1024` units | Initial off-target error chosen once per target acquisition. |
| `g_bot_aim_settle_time` | `0.4` | `0`-`10` seconds | Time for acquisition error to decay smoothly to zero. |
| `g_bot_aim_latency` | `0` | `0`-`2000` ms | Makes the bot aim at a timestamped past target position. |

### Turning and weapon accuracy

| Cvar | Default | Valid range | Description |
| --- | ---: | ---: | --- |
| `g_bot_turn_speed` | `360` | `1`-`1080` degrees/second | Maximum turn rate. |
| `g_bot_turn_accel` | `15` | `0.1`-`100` | Rate at which the bot ramps up to its maximum turn rate. |
| `g_bot_spread` | `1` | `0`-`10` | Static multiple of each bullet's base spread. `0` is pinpoint; values above `1` are less accurate. Bot weapon bloom does not accumulate. |
| `g_bot_sniper` | `25` | `0`-`100` percent | Chance that a bot receives a sniper rifle. |

### Combat movement

| Cvar | Default | Valid range | Description |
| --- | ---: | ---: | --- |
| `g_bot_strafe_intensity` | `0.7` | `0`-`1` | Sideways movement intensity. |
| `g_bot_strafe_min_interval` | `400` | `50`-`10000` ms | Minimum time before changing strafe direction. |
| `g_bot_strafe_max_interval` | `800` | `50`-`10000` ms | Maximum time before changing strafe direction. |
| `g_bot_peek_min_interval` | `600` | `50`-`10000` ms | Minimum time before changing peek/retreat direction. |
| `g_bot_peek_max_interval` | `1200` | `50`-`10000` ms | Maximum time before changing peek/retreat direction. |
| `g_bot_peek_distance` | `384` | `0`-`4096` units | Range inside which bots start peeking and retreating. |
