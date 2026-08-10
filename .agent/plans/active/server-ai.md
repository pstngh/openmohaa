# Server AI continuity and tuning plan

## Purpose and expected outcome

Keep `bots/server-ai` independently recoverable while iterating toward human-like, objective-capable bots on a Linux x64 tuning server. The branch should produce a minimal dedicated-server artifact and use measured telemetry to improve navigation without regressing working maps.

## Repository orientation

- Bot behavior: `code/fgame/playerbot*.cpp`, `playerbot*.h`, and `g_bot.*`.
- Shared navigation: `code/fgame/navigation_*`.
- Telemetry: `code/fgame/movement_telemetry.*`.
- Generated route runtime data: `code/fgame/playerbot_route_data.cpp`.
- Route generator/tests: `code/tools/demoroutes/`.
- Cvars and user-facing semantics: `code/fgame/gamecvars.*` and `docs/markdown/03-configuration/03-configuration-bots.md`.
- Server-only CI: `.github/workflows/branches-build.yml`; unit-test exclusion: `.github/workflows/unit-testing.yml`.
- Deployment safety: `docs/SERVER_OPERATIONS.md`.

## Scope

- Server bot combat, navigation, coordination, objectives, routes, and telemetry.
- `obj/obj_team2` as the primary objective map, with independent regression coverage for `obj_team4`.
- `dm/mohdm6` FFA and `dm/main`, `dm/crnodoors`, `dm/downladder`, and `dm/vents` practice behavior.
- Linux x86-64 `omohaaded` and `game.so` only.

## Non-goals

- macOS launcher/client UI or packaging.
- Porting all experiments to the client branch automatically.
- Normal-mode behavior weights from realism matches.
- A second speculative steering framework layered over Recast without measured need.

## Completed milestones

- [x] Establish configurable bots, combat tuning, team intelligence, objective roles, plant/defuse, and telemetry.
- [x] Process the demo archive into deterministic objective/FFA/practice route graphs.
- [x] Restrict CI artifacts to the Linux x64 server and game module.
- [x] Add server difficulty with 100 as the artificial-combat-handicap maximum.
- [x] Add grenade/reload/health/weapon-aware behaviors and private tuning-server cheats.
- [x] Stabilize several objective, route, ladder, door, and blocked-recovery failure modes.
- [x] Rename the long-lived branch to `bots/server-ai` and add repository-first continuity.
- [x] Verify the renamed branch runs only server-only Linux x64 CI and produces the exact two-file artifact.

## Remaining milestones

- [x] Revalidate the external systemd deployment topology and current binary identity; deploy only if the code tip is not already live.
- [x] Start a clean schema 7 capture on the deployed current head.
- [x] Quantify the current `obj_team2` oscillation and doorway-contact signature by bot state from a preserved live schema 7 capture.
- [x] Compare the same telemetry measures with human samples, especially narrow corridors and non-combat lean/strafe behavior.
- [x] Make only the smallest evidence-supported steering/recovery correction, with a focused regression test.
- [ ] Recheck `obj_team2`, `obj_team4`, and at least one FFA/practice map before accepting it.
- [ ] Review mature shared bot commits for optional porting to `bots/macos-client`.

## Important discoveries

- At retrofit, the branches diverged after `8f614e61`; server history contains about forty additional bot/server commits and client history contains three client-only commits.
- The server branch still contains inherited launcher/client source in its tree, but commit `b6921faa` intentionally makes its active build server-only.
- The CI filters were keyed to the former branch names; a rename without updating both workflow files would accidentally run the full build and unit-test workflow.
- Strategic demo routes do not replace live navigation. Recast/path steering remains responsible for wall and doorway behavior between graph nodes.
- A successful plant/defuse event does not prove all bots advance correctly; telemetry must be analyzed per bot and per round.
- The external service was verified active on 2026-08-10 with the independently checked artifact for passing commit `b094fd87`; its live binary pair matched that artifact and fresh schema 7 telemetry was growing.
- A 443,879-frame schema 7 comparison covering 16 `lawl` sessions and 8 bots found that off-center bots had lateral input on 96.96% of frames versus 52.13% for `lawl`, moving lean was about 88.95% versus 39.81%, and bot lateral commands pointed toward the nearer wall 65.14% of the time.
- An always-on noncombat centering replacement failed visual acceptance: it removed roaming personality and lean without eliminating wall contact. The owner ordered its branch history deleted and the prior binaries restored; do not recreate an always-on centering overlay.
- Deployed commit `1e30bea8` alternates active and neutral noncombat style phases and vetoes an optional strafe only when a 112-unit forward sweep loses more clearance than the untouched navigation command. It never reverses the strafe, adds centering, or changes the final collision guard. Server-only run `31406192630` passed and clean schema 7 telemetry is recording.
- The preserved `1e30bea8` capture contained 183,128 bot frames across 15 `obj_team2` sessions and 27 strict 8-20 second small-area oscillations. Most kept one strategic destination while returning near their start after 350-900+ units of travel; door episodes repeatedly hit moved entities such as 73, 78, 81, and 82.
- Two command-owner defects explained the pattern: objective progress counted repeated travel despite returning locally, while generic blocked recovery mixed the remaining path delta with a reverse offset and then reissued the same path. Fully open door panels were also excluded from runtime Recast obstacles even after moving into their open positions.
- Commits `20f8c3fa` and `13d2c465` added local same-target loop invalidation, deterministic lateral recovery, open-door repathing, and runtime Recast door-panel obstacles. The owner-annotated live capture rejected the repath portion: 4,219 repaths included 17 actor/door groups over 20 seconds and a four-minute maximum, while affected bots remained nominally pathing with commands cancelled by the final guard.
- Commit `b094fd87` removes door repathing and preserves the existing command for every unlocked door contact, matching a human holding movement while use logic and player physics push/slide through. It records throttled `bot_door_pushthrough` contacts; the first autonomous 53-second sample had no same actor/door episode longer than about four seconds, but owner and longer multi-map acceptance remain pending.

## Implementation discipline

1. Confirm a problem from telemetry and map context.
2. Identify the current command owner and override priority before editing movement.
3. Prefer removing conflicting ownership or correcting a path primitive over adding another movement layer.
4. Add a focused deterministic/source-contract test when runtime simulation is unavailable.
5. Build server-only, deploy safely, reset telemetry for a clean comparison, and evaluate the same scenario.
6. Replace or revert failed experiments instead of accumulating dead tuning code.

## Validation and acceptance

- `python code/tools/demoroutes/test_build_bot_route_data.py` passes.
- `python .agent/check_continuity.py` passes.
- GitHub's server-only job configures with client/renderers/codecs disabled and builds targets `omohaaded game`.
- Artifact contains exactly x86-64 `omohaaded` and `game.so`.
- A movement change needs fresh telemetry showing fewer target contacts/reversals without new stalls, route abandonment, objective regressions, or reduced combat control.

## Recovery and rollback

- Use `git revert <focused-commit>` for an already-pushed regression; never reset shared branch history.
- Retain or obtain the previously known-good CI artifact before deploying a movement experiment.
- If generated routes regress, revert both `playerbot_route_data.cpp` and its summary to the last validated generator result; never hand-edit generated graph arrays.
- Do not delete telemetry until a copy is preserved outside the live game directory when it is needed as evidence.

## Next action

Exercise deployed commit `b094fd87` on `obj_team2` plus an FFA/practice map, deliberately pushing through doors opened by the player and by bots. Pull the fresh schema 7 triplet and compare repeated `bot_door_pushthrough` episode duration, stationary share, progress through panel bounds, route oscillation, recovery, and stalls against the rejected `13d2c465` capture. Evaluate wall proximity, speed, lean, and roaming style separately before any further steering change.
