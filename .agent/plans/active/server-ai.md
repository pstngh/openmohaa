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

- [ ] Revalidate the external systemd deployment topology and current binary identity; deploy only if the code tip is not already live.
- [ ] Start a clean schema 7 capture on the deployed current head.
- [ ] Quantify wall/door/item contacts, oscillation, and local route reversals by map and bot state.
- [ ] Compare the same telemetry measures with human samples, especially narrow corridors and non-combat lean/strafe behavior.
- [ ] Make only the smallest evidence-supported steering/recovery correction, with a focused regression test.
- [ ] Recheck `obj_team2`, `obj_team4`, and at least one FFA/practice map before accepting it.
- [ ] Review mature shared bot commits for optional porting to `bots/macos-client`.

## Important discoveries

- At retrofit, the branches diverged after `8f614e61`; server history contains about forty additional bot/server commits and client history contains three client-only commits.
- The server branch still contains inherited launcher/client source in its tree, but commit `b6921faa` intentionally makes its active build server-only.
- The CI filters were keyed to the former branch names; a rename without updating both workflow files would accidentally run the full build and unit-test workflow.
- Strategic demo routes do not replace live navigation. Recast/path steering remains responsible for wall and doorway behavior between graph nodes.
- A successful plant/defuse event does not prove all bots advance correctly; telemetry must be analyzed per bot and per round.
- Repository evidence does not identify which commit is deployed on the external VPS.

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

Download the passing `380331a0` artifact and compare its two binary checksums read-only with the VPS deployment before collecting or changing telemetry.
