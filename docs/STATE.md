---
last_updated: 2026-09-24 UTC
branch: main
peer_branch: bots/server-ai
active_plan: .agent/plans/active/macos-client.md
---

# Current state — macOS client

## Active task

Smoke-test the merged follow-up audit on `main` on the Mac mini M4 before changing launcher, layout, rendering, or bot behavior further.

## Why this serves the goal

The code and its history are complete and build on Linux and in macOS CI; real-device evidence is now the shortest path to a reliable client and prevents speculative fixes.

## Status

On 2026-09-24 the history after the upstream base `a2f3401` was rewritten into a clean series with the fixes from the 2026-09-23 audit folded into the commits they belong to, and it replaced `main`. The previous history remains at the tag `dev`.

A second audit of the rewritten `main` (`fa50e60`) the same day left D012-D014 as they are and added six focused commits, which were reviewed and fast-forwarded onto `main`:

- A raised `sv_bots` is kept until the next map load instead of being lowered to the current map's capacity.
- Human bloom is capped at a clip's worth only with the infinite ammo dmflag, as it was meant for bottomless clips; normal games keep stock bloom.
- The server telemetry `model` column records the German model for Axis players.
- Duplicate bot aim, curious, and telemetry state and dead remarks are removed without changing the bots or the telemetry columns.
- Dead macOS packaging code is removed without changing the package.
- The launcher saves whenever a published setting changes, replacing 38 per-field save handlers, and unused pak-scanner and card code is gone.

Runtime testing on a Mac is still outstanding, and GitHub Actions cannot produce a package: the account's Actions budget is used up, so macOS jobs are refused, and artifact storage is full, so even a successful build cannot upload its package.

## Completed

- Unified arm64 macOS app (`mohbots.app`) and Swift launcher (`launcher.app`) with private bot matches and Connect, which launches the owner's separate `openmohaa` binary (D009).
- Persisted launcher controls for compass, fullscreen, nullbinds, telemetry, crosshair, and resolution.
- Launcher safety: bookmark credentials stay with their server address, text the engine would misread is rejected, a field still being edited is committed before launching, and `launcher.cfg` is readable only by its owner.
- Bot TDM respawns immediately through the unsaved `g_instant_team_respawn`, so no setting leaks into games started without the launcher.
- Bot reaction delay on every new target, holding still to fire inaccurate weapons, and aim error across the line of sight (D013).
- Client lean prediction follows the server's published `g_aalean` (D012).
- Compass-safe top messages, the fixed 80-degree 4:3-base view-model FOV, event-level nullbinds, and passive client telemetry.
- Server and client telemetry stop cleanly on write failures; the server log is capped below the engine's 2 GiB file-length limit.
- The Unit Tests workflow runs every focused Python test in `code/*/tests`.

## Remaining

- Restore GitHub Actions, rebuild `main`, and smoke-test that artifact at 1280x960 and one widescreen resolution (see the active plan for the checklist).
- Tune bot difficulty by feel after the aim-error change: all of `g_bot_aim_error` now becomes a miss, and the defaults were halved to compensate.
- Triage the open questions listed in the active plan.
- Later, collect and analyze an opt-in public-server movement recording.
- Review server bot commits after `8f614e61` individually before any client port.

## Blockers and unknowns

- Windows and Linux cannot provide arm64 Swift compilation or visual/runtime validation; the Builds workflow compiles the launcher, and runtime behavior needs the Mac.
- The GitHub Actions budget is used up, so macOS jobs do not start until it resets or the spending limit is raised, and the artifact storage quota is full, so no downloadable package exists for the current `main` until old artifacts are deleted or expire.
- `upload-artifact` documents that file permissions are not preserved; whether the downloaded apps and `install.command` stay executable is unverified.
- Public-server telemetry requires an owner-played session; raw captures stay outside Git.

## Assumptions

- Passing CI makes the `main` head the test candidate, not proof of visual correctness.
- Older launcher builds saved `sv_team_spawn_interval 0` into the game config. The current launcher no longer sets it, but an existing config keeps the saved value until it is removed or reset to 15.

## Verified evidence

- During the rewrite every commit was built on Linux (dedicated server, client, game and cgame modules, both renderers) with no new compiler warnings against the upstream base, and every focused Python test passed at each commit that has them.
- The launcher's Swift sources pass a syntax check at every commit.
- Before the Actions budget ran out, macOS CI built the arm64 client, packaged it, and compiled the Swift launcher for the server telemetry commit and for the aim-error commit, whose code matches the `main` head; only the artifact upload failed. The launcher commits between those two could not be compiled in CI.
- The Unit Tests workflow passed on the final code.
- The six follow-up commits each build on Linux with no new compiler warnings against the upstream base, pass every focused Python test, and pass the Swift syntax check; a stand-in CMake project on Linux exercised the simplified `bundle_macos.cmake`, including the icon rendering. Compiling the launcher and packaging still need macOS CI or a Mac.

## Relevant locations

- Launcher: `code/LauncherMac/`; compass layout: `code/client/cl_ui.cpp`; input: `code/client/cl_input.cpp`.
- View-model projection: GL1/GL2 `tr_backend.c`; client telemetry: `code/cgame/cg_client_telemetry.*`; server telemetry: `code/fgame/movement_telemetry.*`.
- Working plan: `.agent/plans/active/macos-client.md`.

## Next action

Restore GitHub Actions (raise the budget or wait for it to reset, and free artifact storage), re-run Builds for the `main` head, install that arm64 artifact on the Mac mini M4, and run the smoke-test checklist in the active plan at 1280x960 and one widescreen resolution; record build, resolution, and result before requesting a code change.
