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
- The external service was verified active on 2026-08-10 with the independently checked artifact for passing commit `5d3fd2b2`; its live binary pair matched that artifact, both intended UDP ports listened, and fresh schema 7 telemetry was growing. The exact `6e472321` pair remains at `deploy-backups/pre-5d3fd2b2` for rollback, and its complete stopped capture is preserved outside the live telemetry directory.
- A 443,879-frame schema 7 comparison covering 16 `lawl` sessions and 8 bots found that off-center bots had lateral input on 96.96% of frames versus 52.13% for `lawl`, moving lean was about 88.95% versus 39.81%, and bot lateral commands pointed toward the nearer wall 65.14% of the time.
- An always-on noncombat centering replacement failed visual acceptance: it removed roaming personality and lean without eliminating wall contact. The owner ordered its branch history deleted and the prior binaries restored; do not recreate an always-on centering overlay.
- Deployed commit `1e30bea8` alternates active and neutral noncombat style phases and vetoes an optional strafe only when a 112-unit forward sweep loses more clearance than the untouched navigation command. It never reverses the strafe, adds centering, or changes the final collision guard. Server-only run `31406192630` passed and clean schema 7 telemetry is recording.
- The preserved `1e30bea8` capture contained 183,128 bot frames across 15 `obj_team2` sessions and 27 strict 8-20 second small-area oscillations. Most kept one strategic destination while returning near their start after 350-900+ units of travel; door episodes repeatedly hit moved entities such as 73, 78, 81, and 82.
- Two command-owner defects explained the pattern: objective progress counted repeated travel despite returning locally, while generic blocked recovery mixed the remaining path delta with a reverse offset and then reissued the same path. Fully open door panels were also excluded from runtime Recast obstacles even after moving into their open positions.
- Commits `20f8c3fa` and `13d2c465` added local same-target loop invalidation, deterministic lateral recovery, open-door repathing, and runtime Recast door-panel obstacles. The owner-annotated live capture rejected the repath portion: 4,219 repaths included 17 actor/door groups over 20 seconds and a four-minute maximum, while affected bots remained nominally pathing with commands cancelled by the final guard.
- Commit `b094fd87` removes door repathing and preserves the existing command for every unlocked door contact, matching a human holding movement while use logic and player physics push/slide through. It records throttled `bot_door_pushthrough` contacts; the first autonomous 53-second sample had no same actor/door episode longer than about four seconds, but owner and longer multi-map acceptance remain pending.
- The complete stopped/flushed `b094fd87` capture had five same-bot/same-door episodes of at least five seconds and a 16.6-second maximum with 41.3% nearly stationary frames. `1c1ed0be` added a clearance-probed edge tangent and reduced its complete 15-session capture to two five-second episodes, none at ten seconds, and a moving 7.55-second maximum. Post-contact tangent carry in `f64bad90` produced a broad 81.95-second frame/corner episode; direction preservation and world cancellation in `7d1bdcc8` still bounced laterally; immediate route resumption in `593e1bf9` restored a 12.2-second strict and 23.05-second broad maximum.
- The complete `9f74d560` capture had 912 contacts across 16 `obj_team2` sessions, ten strict five-second episodes, no strict ten-second episode, an 8.0-second strict maximum, and a 14.35-second broad maximum. Frame analysis found high-speed oscillation against panels because fully open unlocked doors still entered door-push handling.
- Commit `6ada8557` keeps the original-approach and contact-only tangent behavior but returns fully open panels to Recast and normal collision handling. Its first six sessions had 188 contacts and 129 strict episodes, none reaching five seconds, with a 1.1-second strict maximum; broad grouping also had no five-second episode and a 4.35-second maximum. A sampled door-132 bot moved hundreds of units away after opening instead of remaining at the panel; owner, longer, and multi-map acceptance remain pending.
- The complete 15-session owner-comment capture from `6ada8557` kept the door-push long tail eliminated, but confirmed three separate residuals: ordinary route commands could rotate sharply while existing velocity lagged, a nearby bot ran with 5-11 units of clearance on one corridor side despite much more room on the other, and ordinary path lateral/corner movement had zero lean while the optional strafe overlay leaned about 92% of its frames. Fully open panel contacts were not identifiable because they deliberately bypassed door-push logging.
- Commit `6938284a` limits only abrupt turns above 45 degrees during fast ordinary path travel to 720 degrees/second, and yields whenever its rounded direction has worse 64-unit clearance. Direct movement, combat, active collision avoidance, blocked recovery, ladders, jumps, and door commits bypass it. `bot_route_turn_limited` records activation, and observational `bot_open_door_panel_contact` events identify displaced open-panel collisions without repathing or changing input. Live visual and multi-map acceptance remain pending; lean and corridor placement stay separate.
- The owner-comment `6938284a` sample separated two problems. Piikon repeatedly reattached to a ladder after blocked recovery, which remains a dedicated ladder fix. After leaving the ladder, the same bot ran at full speed with 0-7.6 units of one-sided clearance while optional strafe, collision avoidance, and the route-turn limiter were inactive; comparable straight base-navigation frames were below 8 units 26.7% of the time versus 26.3% for `6ada8557`. Repeated shared wall-side lanes across bots identify Recast corridor placement, not random movement style, as the owner of that contact.
- Commit `6e472321` corrects that path primitive without globally increasing `agentRadius` or adding a final-input centering layer. Ordinary bot travel may replace Detour's first corner with a 96-unit same-corridor look-ahead inset only below 8 units of boundary clearance and only when the candidate retains at least 14 units; it targets 16 units and bypasses tight/partial routes, short goals, off-mesh links, combat, direct movement, ladders, jumps, recovery, collision avoidance, and active door commits. Fifteen focused tests, all route tests, continuity, passing server-only run `31431670669`, exact artifact verification, deployment hashes, ports, and a fresh schema 7 restart were verified; live acceptance remains pending.
- The complete post-restart `6e472321` snapshot covered 13 owner sessions and separated visible personality from path commitment. `lawl` used strong lateral input on 63.47% and lean on 65.98% of moving noncombat frames, but occupied a strict one-sided wall lane only 4.92% of the time. Bots used less strong lateral input and lean yet occupied that lane 18.98% overall and 21.75% under ordinary path ownership. Median six-second progress efficiency was 0.703 for `lawl` versus 0.574 for bots, while near-return windows were 4.60% versus 15.87%; the correct target is therefore committed geometry and progress, not removal of strafe or lean.
- The 18 owner annotations confirmed multiple owners: door push/slide sequences continued into open-panel contacts and recoveries; an exact objective loop had 0.01 six-second efficiency; a later small-area complaint occurred during combat; Grim approached a teammate within 58 units before the door chain and then remained effectively stopped for 1.5 seconds; one annotated lean window contained short on/off pulses and a fast side flip. Rotating-door free-edge preference, wider accepted Recast comfort, earlier collision probing, immediate loop reseeding, and lean hysteresis are one focused experiment, but combat looping and ladders remain separately evaluated.
- Commit `5d3fd2b2` implements that focused experiment and passed 15 movement/navigation source-contract tests, all 7 route-generator tests, continuity, and server-only run `31451606935`. The exact independently verified ELF64 x86-64 artifact was deployed as a pair, with hashes recorded in `docs/STATE.md`; the service is active with zero restarts and clean schema 7 telemetry is growing.
- The complete stopped `5d3fd2b2` baseline confirmed that crosshair placement is a distinct human traversal signal. On moving `dm/mohdm6` frames, `lawl` had 40.364 degrees median view-to-current-velocity error versus 7.362 for bots and led 250 ms future velocity by at least 5 degrees on 59.78% of frames versus 23.88%; bots were effectively facing the current command.
- Commit `b89457ee` changes only ordinary noncombat view placement: it previews 96 units along the live route, uses Detour straight corners for Recast and ordered nodes for the legacy backend, and preserves all explicit movement/aim owners plus the old fallback. Eighteen focused tests, all route tests, continuity, passing server-only run `31455184141`, exact artifact verification, safe deployment, ports, and fresh schema 7 recording were verified; live owner and multi-map acceptance remain pending.

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

Owner-test deployed commit `b89457ee` on `obj_team2`, focusing on visible crosshair lead through corners without loss of wall/door/loop/lean behavior. Preserve and pull the resulting clean capture, compare view-to-current/future-velocity alignment and traversal measures with the complete `5d3fd2b2` baseline, then recheck `obj_team4` and `dm/mohdm6` before acceptance.
