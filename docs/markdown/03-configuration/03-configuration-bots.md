# Bot settings

The Roomba and regular-SMG systems are independent. Changing one population
does not convert, replace, or retune the other.

## Permanent Roomba bots

Roombas occupy real `sv_maxclients` slots and are reported as normal players to
the master server. Reserve the desired human capacity plus the Roomba count in
`sv_maxclients`; Roombas are never displaced by connecting humans.

Set `sv_numbots 5` for five permanent bots. No separate maximum or
human-population floor is used; `sv_numbots` is the exact target and is capped
only by `sv_maxclients`.

They retain the fixed, melee-only Roomba controller. They do not use firearm,
burst-fire, accuracy, reaction-delay, or pathfinding tuning. The regular-bot
settings below have no effect on them.

## Regular SMG bots

Regular bots are represented as players inside the game module but do not own
server-network client slots. As a result:

- Humans and Roombas still use slots `0` through `sv_maxclients - 1`.
- Regular bots use the reserved slots immediately above `sv_maxclients`.
- Humans see regular bots in the in-game scoreboard with a `bot` ping label.
- GameSpy/master-server player queries do not include regular bots.
- `sv_maxclients + sv_maxregularbots` must be no greater than 64.

### Capacity and population fill

Configure capacity before loading a map, then adjust the population floor live:

```cpp
set sv_maxclients 32
set sv_numbots 5

set sv_maxregularbots 7
set sv_regularbot_minplayers 12
```

In this example, the 32 real slots can hold the five permanent Roombas and 27
humans. With no humans connected, no regular bots run: nobody can see them, and
they are hidden from the master server. When the first human connects, six
regular bots join the five Roombas and that human for a total of 12. Each
further human removes one regular bot; at seven humans, none remain.
`sv_maxregularbots` is the latched capacity and maximum.
`sv_regularbot_minplayers` is the live total-population target and counts humans
plus Roombas before calculating the regular-bot count. Humans count from the
moment they connect, including while they load a map or spectate, so a map
change does not add bots only to remove them as players finish loading.

Capacity defaults to zero. At zero, the regular controller has no instances and
Recast navigation is neither loaded nor updated. With nonzero capacity, the
navigation mesh is built once at map load. Per-frame navigation updates are
skipped whenever no regular bots exist. Set `sv_regularbot_minplayers 0` to
remove all regular bots without changing the reserved capacity.

### Aim controls

Aim behavior is split into four live settings so that delay, tracking, turning,
and error can be tuned independently:

```cpp
set sv_regularbot_aim_reaction_ms 350
set sv_regularbot_aim_latency_ms 120
set sv_regularbot_aim_turnspeed 220
set sv_regularbot_aim_error_deg 2.5
```

`sv_regularbot_aim_reaction_ms` is the initial delay before the bot starts
turning toward and firing at a newly selected opponent. It defaults to 350 ms
and is clamped from 0 through 5000 ms. A brief obstruction does not restart this
delay.

`sv_regularbot_aim_latency_ms` controls the continuous lag while the crosshair
follows a moving enemy. It defaults to 120 ms and is clamped from 0 through
2000 ms. This is a lightweight smooth follow delay, separate from initial
reaction time; zero follows the current target position directly.

`sv_regularbot_aim_turnspeed` is the maximum tracking turn rate in degrees per
second. It defaults to 220 and is clamped from 0 through 1080.

`sv_regularbot_aim_error_deg` is the maximum horizontal angular error. It
defaults to 2.5 degrees and is clamped from 0 through 45 degrees; vertical error
uses 70 percent of that value. The error moves gradually between random offsets
instead of jumping to a new offset. Normal weapon spread remains active.

All four settings take effect immediately and do not alter navigation,
strafing, leaning, or continuous SMG fire.

### Damage control

Regular-bot bullet damage can be scaled live without changing the weapon files:

```cpp
set sv_regularbot_damage 100
```

The value is a percentage clamped from 0 through 100 and defaults to 100.
Setting it to 50 halves all bullet damage from regular bots; setting it to 0
removes their bullet health damage while retaining normal firing, ammo use,
spread, tracers, and impacts. It does not affect humans, real-slot Roomba bots,
or the SMG's secondary melee bash.

### Movement and weapons

Regular bots use stock OpenMoHAA pathfinding and collision recovery. The
navigator's movement is converted to full digital key values, and any actual
left/right movement receives matched lean input. Combat destinations favor
lateral movement around a visible opponent, so bots can strafe and lean without
overwriting the safe path chosen by navigation. Roaming rejects failed or very
short routes instead of steering directly toward an unpathed random point.

The primary selection is strictly `smg`, producing the faction-appropriate
Allied or Axis SMG. They reload and continuously hold primary fire while an
engageable target remains in their imperfect aim. Accuracy comes from initial
reaction time, continuous tracking latency, limited turning speed, angular
error, and normal weapon spread rather than artificial trigger pauses. They
never switch to a pistol to attack; if the SMG is completely empty, it remains
equipped and the bot may use that weapon's normal secondary bash at melee range.

### Names

Names are optional zero-based cvars:

```cpp
set g_regularbot0_name "Charlie"
set g_regularbot1_name "Baker"
```

Unconfigured bots use `smgbot1`, `smgbot2`, and so on.
