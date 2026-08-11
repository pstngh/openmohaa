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
- [x] Quantify the current `obj_team2` oscillation and doorway-contact signature by bot state from an analyzed live schema 7 capture.
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
- The external service was verified active on 2026-08-10 with the independently checked artifact for passing commit `5d3fd2b2`; its live binary pair matched that artifact, both intended UDP ports listened, and fresh schema 7 telemetry was growing. The exact `6e472321` pair remains at `deploy-backups/pre-5d3fd2b2` for rollback; its complete stopped telemetry capture was analyzed and later deleted under the owner's retention policy.
- A 443,879-frame schema 7 comparison covering 16 `lawl` sessions and 8 bots found that off-center bots had lateral input on 96.96% of frames versus 52.13% for `lawl`, moving lean was about 88.95% versus 39.81%, and bot lateral commands pointed toward the nearer wall 65.14% of the time.
- An always-on noncombat centering replacement failed visual acceptance: it removed roaming personality and lean without eliminating wall contact. The owner ordered its branch history deleted and the prior binaries restored; do not recreate an always-on centering overlay.
- Deployed commit `1e30bea8` alternates active and neutral noncombat style phases and vetoes an optional strafe only when a 112-unit forward sweep loses more clearance than the untouched navigation command. It never reverses the strafe, adds centering, or changes the final collision guard. Server-only run `31406192630` passed and clean schema 7 telemetry is recording.
- The analyzed `1e30bea8` capture contained 183,128 bot frames across 15 `obj_team2` sessions and 27 strict 8-20 second small-area oscillations. Most kept one strategic destination while returning near their start after 350-900+ units of travel; door episodes repeatedly hit moved entities such as 73, 78, 81, and 82.
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
- Focused comment review found that the apparent teammate stop was continuous ladder ascent, the ceiling-look sample was enemy aim, and SMGs already fired on most visible-target frames. Combat return windows were owned by target chase/orbit rather than a stable strategic route, so applying the route-loop reset there would be an ownership error. Earlier `6938284a` frames did prove repeated grounded ladder detach/reattach, and close pistol frames proved exclusive bashing despite available primary ammunition.
- Commit `66af0cff` gives grounded ladder detaches the existing 64-unit, one-second exit commitment and observational `bot_ladder_ground_exit` event, while close pistols fire at most once per 1.2 seconds and bash between ready shot opportunities. Twenty focused tests, all route tests, continuity, passing server-only run `31457072835`, exact artifact verification, safe deployment, both ports, and fresh schema 7 recording were verified. The exact prior binary pair remains available for rollback; the stopped `b89457ee` telemetry was analyzed and later deleted. Live owner and multi-map acceptance remain pending.
- The complete `66af0cff` autonomous capture covered 175 `obj_team2` sessions and proved the grounded exit experiment regressed: 88.5% of grounded exits reattached within 1.5 seconds versus 45.8% before it, with exact-origin cycles up to 16 exits in 30 seconds. The same capture exposed a separate 16.6-second fully-open-panel stall and stopped recording when frames crossed 2.15 GB; wall, lean, route, combat, and objective aggregates were otherwise broadly stable.
- Deployed commit `849be809` makes bottom exits use `-facingDir`, refuses attachment while exit commitment is active, gives open panels a clearance-probed committed tangent without repathing, and rotates complete standard telemetry triplets at 1.5 GiB. Twenty-two focused movement/telemetry tests, all route tests, continuity, passing server-only CI, exact artifact verification, safe deployment, ports, cleanup hook, and fresh schema 7 recording are verified; live behavioral acceptance remains pending.
- Server-only run `31505803523` produced exact ELF64 x86-64 hashes `04c746ec582235c6d8a311e8feef72a78fc6883ee952baea9bb7dba65809d649` (`omohaaded`) and `0a09f05a3e8e937a4fe7ef0a4241f48abecd225e0ad8f4854b77d33e4f1b0b54` (`game.so`). The deployed pair matches; `deploy-backups/pre-849be809` contains the exact `66af0cff` pair and old cleanup drop-in.
- A transient post-deployment pull covered 350,168 frames and 19,635 events across 33 autonomous `obj_team2` sessions. Ladder exact-origin cycles fell from 16 to 4, but 68.6% of grounded exits still reacquired within 1.5 seconds, concentrated at 1,050 ms after the existing owner released.
- Open-panel escape is rejected: 2.5-second grouping produced 14 five-second episodes, 4 ten-second episodes, and a 22.95-second maximum; 13 long episodes had already selected the new tangent and then entered strategic route oscillation. Entity 73 accounted for 11 long episodes and entity 87 retained a 9.35-second near-stationary stall.
- Ordinary six-second efficiency improved from the recorded 0.574 baseline to 0.685 and near-return windows fell from 15.87% to 1.75%, with no ordinary strong-command stall over 1.05 seconds. Wall placement did not improve: 29.04% of moving ordinary frames were below 12 units of lateral clearance and the strict one-sided rate was 23.63% versus the recorded 21.75%. The transient pull was deleted after analysis.
- Deployed commit `5940d700` keeps wall placement separate. Its complete 36-session capture shows a partial door improvement: episodes lasting at least five seconds fell from 14/346 to 7/368 and 127 of 130 bounded geometric phases completed. Retain that door phase for the next comparison, while treating three timeouts and the 19.95-second contact maximum as residual work.
- Reject the `5940d700` ladder command gate. All 39 post-owner reattachment recoveries timed out, median maximum projection reached only 90.4 of 128 units, recovery speed was typically zero, and movement suppression covered 96.6% of recovery frames. The unchanged strategic route reverses the forced displacement; first-reacquisition and exact-origin-cycle measures barely changed from `849be809`.
- Reject experiment `bd5e7f17`. A fresh 0.653-session-hour capture found 39 recovery starts, zero completions, 16 timeouts, and 23 silent external cancellations. All 39 destinations projected exactly -128 units onto the intended away vector because game yaw was applied directly to Recast X/Z axes despite the `(x,y,z)` to `(x,z,-y)` conversion. Fast reattachment and exact-origin clustering did not improve. The transient pull was deleted, the rejected commits were removed from canonical ancestry, and its VPS pair was replaced by `6a877fdc`.
- Deployed replacement `6a877fdc` preserves the door phase, derives `FindPathAway` angles from the converted Recast preferred vector, covers all four game-space cardinal directions, reports cancellation owner reasons, and exposes route-recovery activity in frame telemetry. Twenty-five bot movement/telemetry tests, all route and client-input suites, continuity, server-only run `31532011052`, exact artifact checks, deployment hashes, identity, service health, ports, cleanup, and fresh schema 7 recording pass; behavioral acceptance remains pending.

## Implementation discipline

1. Confirm a problem from telemetry and map context.
2. Identify the current command owner and override priority before editing movement.
3. Prefer removing conflicting ownership or correcting a path primitive over adding another movement layer.
4. Add a focused deterministic/source-contract test when runtime simulation is unavailable.
5. Build server-only, deploy safely, reset telemetry for a clean comparison, and evaluate the same scenario.
6. Replace or revert failed experiments instead of accumulating dead tuning code.

## Validation and acceptance

- `python code/tools/demoroutes/test_build_bot_route_data.py` passes.
- `python -m unittest discover -s code/tools/bots -p 'test_*.py' -v` passes.
- `python .agent/check_continuity.py` passes.
- GitHub's server-only job configures with client/renderers/codecs disabled and builds targets `omohaaded game`.
- Artifact contains exactly x86-64 `omohaaded` and `game.so`.
- A movement change needs fresh telemetry showing fewer target contacts/reversals without new stalls, route abandonment, objective regressions, or reduced combat control.

## Recovery and rollback

- The owner requires rejected server-AI experiments to be removed from canonical branch history. Verify a clean tree, exact rejected range, safe parent, and unchanged remote lease before an exact force-with-lease rewrite; never erase unrelated work. Retain the last known-good binary pair until its replacement is deployed and verified.
- Retain or obtain the previously known-good CI artifact before deploying a movement experiment.
- If generated routes regress, revert both `playerbot_route_data.cpp` and its summary to the last validated generator result; never hand-edit generated graph arrays.
- Telemetry is disposable diagnostic data. Pull transient copies outside Git only for immediate analysis; do not retain telemetry backups after analysis or before an owner-requested clear or restart.
- A service restart for segmented telemetry must clear both the primary triplet and the exact `telemetry/segments` subtree without creating an archival copy.

## Next action

Let `6a877fdc` accumulate a representative clean capture, pull it transiently, and compare recovery-direction and lifecycle outcomes, reattachment timing, exact-origin cycles, ordinary controls, objectives, and the retained door phase against `5940d700` and the rejected experiment. Delete the pull after analysis; do not combine this acceptance pass with new wall-clearance tuning.
