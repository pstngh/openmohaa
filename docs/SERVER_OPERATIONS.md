# Bot-tuning server operations

This runbook describes the intended process without storing infrastructure secrets. The current host, login method, service unit, install root, and game-data paths must be supplied through an approved external channel and verified read-only before mutation.

## Build contract

The authoritative CI recipe is `.github/workflows/branches-build.yml`.

```sh
python3 code/tools/demoroutes/test_build_bot_route_data.py
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DBUILD_CLIENT=OFF \
  -DBUILD_SERVER=ON \
  -DBUILD_GAME_LIBRARIES=ON \
  -DBUILD_GAME_QVMS=OFF \
  -DBUILD_RENDERER_GL1=OFF \
  -DBUILD_RENDERER_GL2=OFF \
  -DBUILD_STANDALONE=OFF \
  -DBUILD_MACOS_BUNDLE=OFF \
  -DBUILD_TESTING=OFF \
  -DUSE_OPENAL=OFF \
  -DUSE_HTTP=OFF \
  -DUSE_CODEC_VORBIS=OFF \
  -DUSE_CODEC_OPUS=OFF \
  -DUSE_CODEC_MAD=OFF \
  -DUSE_VOIP=OFF \
  -DUSE_MUMBLE=OFF \
  -DUSE_FREETYPE=OFF
cmake --build build --parallel --target omohaaded game
```

Accept only an artifact containing x86-64 `omohaaded` and `game.so`.

## Deployment prerequisites

1. Confirm the requested Git commit and passing CI run.
2. Obtain the target and authentication outside Git. Never paste credentials into tracked commands, logs, or documentation.
3. Read-only inspect the remote OS, service unit, `ExecStart`, service user, working directory, current binary paths, and checksums.
4. Confirm the service is systemd-managed. Do not introduce `screen` or CPU pinning; the owner selected systemd and default OS scheduling.
5. Preserve recoverable copies/checksums of the currently deployed binaries before replacement.

## Tuning deployment sequence

The current deployment policy for an accepted server build is: preserve the exact prior binary pair, stop the systemd service, replace only `omohaaded` and `game.so` as one verified pair, and start the service again. The configured service start hook resets exactly the active telemetry triplet for a clean schema-compatible run. Do not reboot the VPS unless the owner requests it or a verified deployment requirement makes it necessary. Before the reset, download/archive telemetry when it is still needed as evidence.

Use the exact verified service and paths from the prerequisite inspection; this document deliberately contains no guessed unit or directory names.

After the service restart, verify:

- the systemd unit is active and enabled;
- the process command line and working directory are expected;
- deployed file checksums match the selected artifact;
- the listening server is reachable;
- the server reports expected bot cvars and `g_movelog` state;
- a new telemetry triplet uses the expected schema before beginning the test.

## Configuration invariants

- Cheats are intentionally enabled on this private tuning server while tuning continues.
- Use `dmflags 0` as the neutral baseline. Infinite ammo is bit 14 (`bitset dmflags 14`), not `dmflags -1`; `-1` also enables weapon-disabling bits.
- `g_bot_difficulty 100` means maximum combat capability, not a navigation/strategy multiplier.
- Keep the compared map, bot count, teams, cvars, and random conditions stable when evaluating a movement change.

## Telemetry handling

- Server telemetry is the three-file `telemetry` triplet documented in `docs/markdown/03-configuration/03-configuration-bots.md`.
- Do not mix schemas. Archive or remove all three together before a clean run.
- Stop logging cleanly when practical so buffers flush.
- Store downloaded captures outside Git and label them with deployed commit, map, cvars, and session window.
- Owner chat annotations are delayed observations; correlate them with nearby frames rather than assuming the message timestamp is the exact event frame.

## Rollback

If the server fails to start or gameplay regresses, stop the unit, restore the preserved binary pair from the same known-good build, reboot or restart according to the approved operation, and verify checksums/service status again. Then revert the focused Git commit rather than rewriting shared history.
