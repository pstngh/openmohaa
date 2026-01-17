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

## Awareness and detection

### Field of View (FOV)

Bots have **360-degree awareness** at all times. They can detect enemies in any direction without needing to face them, simulating perfect spatial awareness.

### Visual Range

Bots can see enemies up to **4096 units** away, regardless of fog settings. Unlike regular AI, bots are not affected by the map's far plane distance or fog - they maintain full vision range at all times.

### Sound Detection

Bots react to all AI sound events within their detection radius:
- **Footsteps** (512 units): Movement sounds from enemies
- **Weapon Fire** (2048 units): Gunshots and weapon discharge
- **Weapon Impact** (384 units): Bullet impacts and hits
- **Explosions** (4096 units): Grenade and explosive detonations
- **Grenades** (384 units): Thrown grenades
- **Voice Lines** (1024-1536 units): Team voice commands
- **Miscellaneous Sounds** (1500-2250 units): Doors, objects, and environment

When a sound is detected, bots enter a "curious" state for 20 seconds and actively move toward the sound source to investigate.

### Aggressive Seeking

Bots proactively move toward any detected enemy sounds or visual contacts. They do not wait passively but immediately pursue detected threats, making them highly responsive to player activity.

## Combat and targeting

### `g_bot_aim_legal_zones_only`

- **Default**: 1
- **Type**: integer (boolean)

#### Description

Enables legal hit zone targeting, restricting bots to aim only at middle/lower torso, pelvis, upper/lower arms, hands, and upper/lower legs.

#### Usage

- `0`: Disable legal zones (bots use original aiming that targets head/eyes).
- `1`: Enable legal zones only (default, excludes head, helmet, neck, upper torso, and feet).

#### Notes

When enabled, bots will never directly target the head, helmet, neck, upper torso, or feet unless those are the only parts visible. This creates more realistic and fair combat where bots don't consistently land headshots.

### `g_bot_aim_zone_change_time`

- **Default**: 0.15
- **Type**: float (seconds)

#### Description

Time between aim zone changes during combat, creating a "dancing" aim effect.

#### Usage

- Lower values (e.g., `0.05`): Very rapid aim dancing, more erratic spray patterns.
- Higher values (e.g., `0.3`): Slower aim changes, more predictable targeting.
- `0.15`: Default, creates natural-looking aim adjustments during spray.

#### Notes

The bot dynamically switches between 16 different legal hit zones (various parts of middle/lower torso, pelvis, upper/lower arms, hands, and upper/lower legs) at this interval. This prevents bots from locking onto a single spot and creates more realistic spray patterns.

### `g_bot_reload_after_kill`

- **Default**: 1
- **Type**: integer (boolean)

#### Description

Automatically reload immediately after every kill when no other enemy is in sight.

#### Usage

- `0`: Disable auto-reload after kills.
- `1`: Enable auto-reload after kills (default).

#### Notes

This ensures bots maintain a full magazine when safe, preventing them from being caught with low ammo in the next engagement. Bots only reload if they have no current enemy target.

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
