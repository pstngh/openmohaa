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

## Altering behavior

There is no skill system yet, however some settings can be modified to alter bot difficulty:

### `g_bot_attack_burst_min_time`

- **Default**: 0.1
- **Type**: float (seconds)

#### Description

Minimum time to pause firing (burst).

### `g_bot_attack_burst_random_delay`

- **Default**: 0.5
- **Type**: float (seconds)

#### Description

Random time added to pause firing (burst).

### `g_bot_attack_continuousfire_min_firetime`

- **Default**: 0.5
- **Type**: float (seconds)

#### Description

Minimum duration of continuous firing.

### `g_bot_attack_continuousfire_random_firetime`

- **Default**: 1.5
- **Type**: float (seconds)

#### Description

Random time added to the continuous firing duration.

### `g_bot_attack_react_min_delay`

- **Default**: 0.2
- **Type**: float (seconds)

#### Description

The minimum delay before shooting the enemy.

### `g_bot_attack_react_random_delay`

- **Default**: 1.0
- **Type**: float (seconds)

#### Description

Random delay added before shooting the enemy.

### `g_bot_attack_spreadmult`

- **Default**: 1.0
- **Type**: float

#### Description

Controls how accurate bots are when shooting.

#### Usage

- Lower values (< 1.0): More accurate, more likely to land headshots.
- Higher values (> 1.0): Less accurate, more likely to miss their target.

### `g_bot_turn_speed`

- **Default**: 15
- **Type**: float (degrees)

#### Description

The rate of degrees per second when turning.

### `g_bot_instamsg_chance`

- **Default**: 5
- **Type**: integer

#### Description

The chance at which the bot sends an instant message when shooting.

#### Usage

- 0: Disable.
- higher values: Less frequent messages.

### `g_bot_instamsg_delay`

- **Default**: 5.0
- **Type**: float (seconds)

#### Description

The minimum delay between instant messages.

## Movement behavior

### `g_bot_strafe_enabled`

- **Default**: 1
- **Type**: integer (boolean)

#### Description

Enables or disables persistent strafe and lean behavior for bots.

#### Usage

- `0`: Disable strafe and lean behavior (bots move normally).
- `1`: Enable strafe and lean behavior (default).

#### Notes

When enabled, bots will constantly strafe left/right and lean while moving, making them harder to hit. The movement is additive to their pathfinding, creating aggressive zigzag patterns toward their goals.

### `g_bot_strafe_intensity`

- **Default**: 1.0
- **Type**: float (0.0 to 1.0)

#### Description

Controls the intensity of bot strafing movement.

#### Usage

- `0.0`: No strafing (same as disabling).
- `0.5`: Moderate strafing movement.
- `1.0`: Full intensity strafing (default, most aggressive).

#### Notes

Lower values create more subtle movement, while higher values make bots more twitchy and harder to hit.

### `g_bot_strafe_min_change_time`

- **Default**: 0.05
- **Type**: float (seconds)

#### Description

Minimum time before bots change their strafe and lean direction.

#### Usage

- Lower values (e.g., `0.05`): Very frequent direction changes, creating erratic movement.
- Higher values (e.g., `0.2`): More predictable movement patterns.

#### Notes

Works together with `g_bot_strafe_max_change_time` to create randomized timing.

### `g_bot_strafe_max_change_time`

- **Default**: 0.5
- **Type**: float (seconds)

#### Description

Maximum time before bots change their strafe and lean direction.

#### Usage

- Must be greater than or equal to `g_bot_strafe_min_change_time`.
- `0.5`: Default maximum interval (never exceeds 0.5 seconds between changes).

#### Notes

Bots will randomly pick a time between the min and max values for each direction change, creating unpredictable movement patterns.

### `g_bot_lean_match_chance`

- **Default**: 85
- **Type**: integer (0 to 100)

#### Description

The percentage chance that a bot's lean direction will match their strafe direction.

#### Usage

- `0`: Lean and strafe are completely independent (can strafe left while leaning right).
- `50`: Equal chance of matching or being independent.
- `85`: Default, lean matches strafe 85% of the time for more natural movement.
- `100`: Lean always matches strafe direction.

#### Notes

Higher values create more synchronized, natural-looking movement. Lower values create more chaotic, unpredictable behavior.
