# Current State

Reconciled: 2026-08-20

## Active task

None. The implementation series is complete; await the next user request.

## Why this serves the goal

The branch contains the narrow server behavior described in `GOAL.md` while
keeping deployment and live status explicit rather than assumed.

## Status

Complete; no code or deployment work is currently authorized.

## Completed

- The implementation series through `4838a21` contains 13 commits after `main`:
  real-slot administration/bots, lean melee behavior, GameSpy reporting, a lean
  Linux server build, native rotation/announcements, spectator and name rules,
  SP-map FFA packages, and SP-map taunt audio.
- The name-change policy adds a per-connection human rename limit: the first
  exact name change is logged and allowed, while the second is logged and
  kicked. Bots are exempt, map changes preserve the count, and reconnecting
  resets it.
- The same change adds a shared log-only print path and routes only the
  server-side client-name messages through it. Existing game-module contextual
  console messages retain their prior behavior.
- Tracked server packages: `misc/server/zzz_m6l2a_ffa.pk3` and
  `misc/server/zzz_sp_ffa.pk3`.

## Remaining

Nothing until the user selects another development or operational task.

## Blockers / unknowns

- The previously managed VPS is now a one-slot redirect, not the current public
  game server.
- The current server's binaries, config, five-bot state, PK3s, and taunt behavior
  are unverified.
- Live-server behavior of the name-change limit is unverified.
- Live stock-client testing of SP-map taunts is unrecorded.
- Runtime rotation is operator-owned and not tracked in Git.

## Assumptions

- `GOAL.md` remains current until the user explicitly changes it.
- Committed or CI-passing code is not assumed deployed or live-tested.

## Verified

- The name-change policy passed a focused harness covering initial, identical,
  non-name, first/second rename, reason, and bot cases. Debug `omohaaded` and
  `game` targets built successfully; the dedicated-server startup smoke test
  stopped at the expected missing proprietary game assets.
- The log-only update builds successfully for Debug `omohaaded` and `game`.
  A debugger-driven runtime check against the real server binary verified that
  log-only output appears in `qconsole.log` but not captured console output,
  while ordinary output continues to appear in both.
- Linux build and unit-test workflows for `4838a21` passed on 2026-07-30.
- `server_opm.cfg` and external `z_forteffa.pk3` are not tracked.
- No earlier continuity/task system existed; source TODOs are not active tasks.

## Relevant locations

- Server: `code/server/`
- Game module: `code/fgame/`
- Server docs: `docs/markdown/03-configuration/`
- Server map packages: `misc/server/`

## Exact next action

For the next request, inspect Git and the affected files first. If it concerns a
live server, identify the authoritative server before inspecting or deploying;
do not assume the legacy redirect is production.
