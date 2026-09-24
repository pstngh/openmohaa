# macOS client and launcher plan

## Purpose and expected outcome

Keep this repository independently recoverable and deliver a polished arm64 OpenMoHAA client/launcher for private bot matches, normal online multiplayer, and passive human movement recording.

## Repository orientation

- Launcher Swift code: `code/LauncherMac/Sources/LauncherMac/`.
- Launcher package/resources: `code/LauncherMac/Package.swift`, `code/LauncherMac/Resources/`, and `code/LauncherMac/install.command`.
- Client telemetry: `code/cgame/cg_client_telemetry.*`.
- Client HUD/compass layout: `code/client/cl_ui.cpp`.
- Input/nullbind behavior: `code/client/cl_input.cpp`.
- Focused tests: `code/client/tests/` and `code/cgame/tests/`.
- macOS build/package recipe: `.github/workflows/shared-build-macos.yml`.
- User-facing bot/client telemetry docs: `docs/markdown/03-configuration/03-configuration-bots.md`.

## Scope

- arm64 macOS game client and launcher.
- Private launcher-started bot matches and ordinary online connections.
- Crosshair, nullbind, compass, fullscreen, fixed view-model FOV, resolution, and client-telemetry features.
- Selectively ported shared bot fixes after server validation.

## Non-goals

- Server-only cheats, newer server telemetry schemas, VPS operations, generated route graphs, or server-only CI.
- Wholesale synchronization with the server repository.
- Changing normal public-server semantics to match private bot-session options, beyond the build-wide rules in D012.

## Completed milestones

- [x] Build and package a unified arm64 macOS application and launcher.
- [x] Add bot-match controls, loadouts, crosshair, nullbind toggle, and relevant bot cvars.
- [x] Add 1280x960 and 1344x1008 resolution choices.
- [x] Add passive `cl_movelog` client telemetry for unmodified public servers.
- [x] Add automatic compass-safe top-message layout and a persisted launcher compass toggle.
- [x] Add a persisted fullscreen/windowed launcher switch shared by normal and bot launches.
- [x] Lock the first-person weapon/arms projection to the original aspect-corrected 80-degree 4:3 base FOV without a setting or cvar.
- [x] Audit `a2f3401..dev` (2026-09-23) and settle every finding with the owner (D012-D014).
- [x] Rewrite the history after `a2f3401` into a clean series with the audit fixes folded in, replace `main`, and keep the old history at the tag `dev` (2026-09-24).

## Remaining milestones

- [ ] Restore GitHub Actions (budget and artifact storage), rebuild `main`, and install that arm64 artifact on the Mac mini M4.
- [ ] Smoke-test at 1280x960 and one widescreen resolution:
  - Connect with bookmark switching; a changed address must clear or swap the credentials.
  - A nickname or password containing `+`, `"`, `;`, or `//` shows the alert instead of launching.
  - A bot count or health typed without pressing Return is used by Play.
  - Verify reports missing retail paks without turning green.
  - Fullscreen/windowed, compass alignment at the default `ui_compass_scale`, and the crosshair overlay on an MG nest.
  - Weapon framing while world FOV changes, including whether first-person sprite effects such as muzzle flashes line up with the barrel.
  - Bot aim and difficulty feel after the aim-error change.
- [ ] Check whether the downloaded artifact keeps the executable bits of the apps and `install.command`; if not, archive the package with `ditto` or `tar` before upload.
- [ ] Confirm `obj/obj_team2` and `obj/obj_team4`, which upstream hid from the FFA/TDM map lists, work in both modes.
- [ ] Record a natural public-server session with `Record my movement` enabled and flush it cleanly.
- [ ] Analyze human non-combat/combat lean, strafe, corridor, collision, aim-target, and wall-clearance behavior from the new capture.
- [ ] Compare mature server bot commits after `8f614e61` and port only explicitly wanted, client-safe changes.

## Important discoveries

- A launcher-started `mohbots.app` process keeps the bot-match settings it was started with, including cheats, LAN-only hosting, the observer lock, and the dmflags, until it quits. Host other games from a fresh process.
- The compass layout uses the rendered widget bounds, falls back conservatively, and refreshes on UI realignment or cvar changes rather than every frame.
- The view model is recognized by its first-person render flag. Sprite effects still use the world projection, because the sprite pass batches sprites by shader and cannot switch projections mid-batch without a restructure.
- Client telemetry is predicted/local rather than authoritative server state and can only reason about entities the server sends to the client.
- `fs_homepath` must be the same absolute path the engine derives for `fs_basepath`; `.` registers the game folder twice and indexes every pak file twice.
- The server repository has materially newer bot code but also server-only policies and schemas; filename similarity is not evidence that a port is safe.

## Implementation discipline

1. Preserve normal online behavior unless the owner explicitly requests a client-wide change.
2. Keep launcher-only bot policy in launcher-generated arguments, and never pass it through an archived cvar.
3. Add focused test coverage for input, telemetry, and layout behavior.
4. Use GitHub macOS CI for compilation and the actual M4 for runtime/visual acceptance.
5. Port server bot changes by reviewed hash and retest here.

## Validation and acceptance

- Every `code/*/tests/test_*.py` passes.
- `python .agent/check_continuity.py` passes.
- GitHub macOS arm64 Builds and Unit Tests workflows pass.
- The runtime smoke test shows readable top messages with both compass states at representative aspect ratios.
- Enabling client telemetry creates frame/input/meta files, disabling it flushes them, and normal multiplayer input is unchanged.

## Recovery and rollback

- Revert focused commits on `main` instead of restoring an older snapshot; the pre-rewrite history stays at the tag `dev` for reference.
- Preserve the last passing macOS artifact before testing a layout/input change.
- If a port from the server repository fails, revert the port commit here; do not rewrite the source.

## Next action

Restore GitHub Actions (raise the budget or wait for it to reset, and free artifact storage), re-run Builds for the `main` head, install that arm64 artifact on the Mac mini M4, and run the smoke-test checklist above at 1280x960 and one widescreen resolution; record build, resolution, and result before requesting a code change.
