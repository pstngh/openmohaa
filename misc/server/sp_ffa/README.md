# Stock single-player FFA maps

`zzz_sp_ffa.pk3` provides server-side FFA adapters for these stock Allied
Assault maps:

- `m1l2a` - The Rescue Mission
- `m3l2` - Battle in the Bocage
- `m3l3` - The Nebelwerfer Hunt
- `m4l3` - The Command Post
- `m5l2a` - The Hunt for the King Tiger - Destroyed Village
- `m5l3` - The Bridge

Clients use the map data already shipped with Allied Assault. The package
replaces only the map scripts, adds distributed deathmatch spawns, removes
single-player actors, pickups, vehicles, and mission triggers, and opens
mission progression barriers. It starts no timers or polling loops.

`m4l3` removes its rotating doors during map initialization so their
doorways remain open in multiplayer. This uses the existing script API and
does not require another server binary change.

Copy `zzz_sp_ffa.pk3` into the server's `main` directory, then start a map
with its bare name, for example:

```
map m4l3
```

The package does not add any map to the automatic rotation.
