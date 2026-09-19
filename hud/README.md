# Compact HUD override

The files under `hud/ui/` are editable OpenMoHAA URC layouts. They override
the stock Allied Assault HUD layouts without modifying the original game PK3s.

## Build the PK3

From the repository root:

```sh
python3 hud/build_pk3.py --output zzz_openmohaa-hud.pk3
```

The builder sorts every entry and fixes ZIP metadata, so identical layout
sources produce a byte-for-byte identical PK3. Entries are stored
uncompressed, so the bytes don't depend on the zlib build. Every release
archive includes it as `zzz_openmohaa-hud.pk3` next to the binaries.

## Install

Copy `zzz_openmohaa-hud.pk3` into the game's `main/` directory. Keep the
filename late in PK3 sort order if renaming it, so it wins over `Pak0.pk3`.
Remove it to go back to the stock layouts.

Only the Allied Assault layouts are overridden. Spearhead and Breakthrough add
`mainta/` or `maintt/` after `main/`, so any HUD layouts in their paks win over
this PK3 whatever its name.

The timer and score changes live in the client-game module, which every
client build already includes.

## Validate

```sh
python3 hud/tests/test_hud_layouts.py
python3 hud/tests/test_build_pk3.py
```

Every populated ammo menu reserves 24 or 25 virtual units beneath the bullet
graphic or animation. The count ends one unit above the bottom edge, centered
under the graphic
with at least 5 units of vertical clearance. The empty ammo menu is unchanged.
