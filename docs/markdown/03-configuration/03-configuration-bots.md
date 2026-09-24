# Bot settings

Bots always occupy real `sv_maxclients` slots and are reported as normal
players to the master server. Reserve the desired human capacity plus the bot
count in `sv_maxclients`; bots are never displaced by connecting humans.

Set `sv_numbots 5` for five permanent bots. No separate maximum or
human-population floor is used; `sv_numbots` is the exact target and is capped
only by `sv_maxclients`.

## Behavior

Bots use a fixed, melee-only roomba controller. They do not use firearm,
burst-fire, accuracy, reaction-delay, pathfinding, or instant-message tuning.
