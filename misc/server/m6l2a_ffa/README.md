# m6l2a FFA support

`zzz_m6l2a_ffa.pk3` converts the stock Allied Assault single-player map
`m6l2a` for public multiplayer use. It contains server scripts only; clients
use their original map and sound files.

The matching OpenMoHAA server build is required. It:

- inhibits the map's rotating doors and single-player start in multiplayer,
- registers the normal deathmatch sound aliases on single-player maps, and
- forwards only the weapon sounds that stock clients do not register on
  `m6l2a`.

Remove `zzzzzzzzzzzzzzz_Reborn_pak8_0.6.pk3`; keeping it would restore the
overridden BSP, weapon balance changes, global sound replacements, and
per-animation scripts this package removes.

Copy `zzz_m6l2a_ffa.pk3` into the server's `main` directory. The map can then
be started with:

```
map m6l2a
```

The map is not added to the automatic rotation.
