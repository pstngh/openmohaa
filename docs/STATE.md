---
last_updated: 2026-08-10 America/New_York
branch: bots/server-ai
peer_branch: bots/macos-client
active_plan: .agent/plans/active/server-ai.md
audited_pre_continuity_tip: 676c85f1f078bf5cd86cee591f3f43ad8b33e2e9
working_tree_at_audit: clean
---

# Server AI handoff

Git is authoritative for the current commit. The hash above is the clean code tip audited before adding this tracked handoff; do not try to store the self-referential hash of the commit containing this file.

## Current objective

Continue evidence-driven server bot development without regressing working objective/FFA behavior. The immediate technical concern is remaining collision, doorway, and local back-and-forth movement after the latest Recast steering fixes. Keep the branch Linux x64 dedicated-server-only and use fresh telemetry to justify further movement changes.

## Complete

- Renamed the active branch from `agent/bot-demo-route-graphs` to `bots/server-ai` without changing its tip; local and remote tips matched at the retrofit audit.
- Built a configurable bot population/loadout/combat base and a server-side `g_bot_difficulty` scale where 100 removes artificial combat handicaps.
- Added schema 7 server telemetry, including bot decisions, objectives, route events, collision context, and delivered chat annotations.
- Added team contact sharing, objective planning, planting/defusing, grenade escape, reload defense/pistol switching, health pickup, and weapon-aware combat behavior.
- Added deterministic demo-derived graphs for `obj/obj_team2`, `obj/obj_team4`, `dm/mohdm6`, `dm/main`, `dm/crnodoors`, `dm/downladder`, and `dm/vents`.
- Added map-specific objective/practice behavior, including `obj_team2` defender opening roles.
- Established a server-only CI recipe that packages only Linux x86-64 `omohaaded` and `game.so`.
- Preserved cheats as an intentional private tuning-server policy.
- Configured the external tuning service to keep schema 7 movement telemetry enabled and clear exactly its active three-file triplet before every service start; a restart therefore starts a clean capture.
- Latest navigation series through `676c85f1` addresses multilevel progress, openable doors, door panels, blocked recovery, ladders, unreachable route hops, and shared Recast collision steering.
- A preserved schema 7 capture from deployed commit `1e30bea8` contained 183,128 bot frames across 15 `obj_team2` sessions. It exposed 27 strict 8-20 second small-area oscillations; most retained a stable strategic destination while repeatedly reversing, and several doorway episodes repeatedly contacted the same moved door-panel entity.
- Focused commits `20f8c3fa` and `13d2c465` make fully open door panels dynamic Recast obstacles, repath immediately on open-door contact, prefer a clearance-tested committed lateral exit over generic reverse recovery, and invalidate strategic routes when a local same-target loop is detected.
- The owner-annotated `13d2c465` capture rejected blind open-door repathing: it recorded 4,219 repath events, 61 repeated actor/door groups, 17 groups lasting over 20 seconds, and a four-minute maximum. Annotated bots remained nominally pathing while the final guard repeatedly reduced their command to zero.
- Commit `b094fd87` removes door-specific repathing and preserves the existing command for every unlocked door contact, matching a human holding movement while use logic and normal player physics open, push, and slide through the doorway. Throttled `bot_door_pushthrough` events now measure contact duration without changing movement.
- The complete stopped/flushed `b094fd87` capture contained 1,027 push events across 19 sessions and 681 same-bot/same-door episodes. Five lasted at least five seconds and the maximum was 16.6 seconds; its longest case spent 41.3% of frames nearly stationary, proving that forward pressure alone could still oscillate against a moving panel.
- Commit `1c1ed0be` keeps forward pressure but, after 350 ms of persistent contact, commits to the clearer panel-edge tangent for up to a 1.5-second recontact window. It never calls `FindPath` or creates a door-specific route target, and `bot_door_push_slide` records each physical edge commitment.

## Partial or under evaluation

- The recent navigation series is implemented but has not been accepted as eliminating the owner's observed wall/door/item contacts or mindless back-and-forth. A 443,879-frame human comparison covering 16 `lawl` sessions and 8 bots found that off-center bots had lateral input on 96.96% of frames versus 52.13% for `lawl`, moving lean was about 88.95% versus 39.81%, and bot lateral commands pointed toward the nearer wall 65.14% of the time.
- An always-on noncombat centering replacement failed visual acceptance because it removed roaming personality and lean without eliminating wall contact. The owner ordered its branch history deleted and the prior binaries restored. Deployed commit `1e30bea8` contains the narrower replacement: alternating active/neutral roaming phases plus a comparative forward-clearance veto that cancels only an unsafe optional strafe. It does not reverse direction, center the bot, or alter the final collision guard. Live acceptance remains pending.
- Passing commit `1c1ed0be` is deployed with committed forward-plus-edge door pushing and a fresh schema 7 capture. Its first two `obj_team2` sessions contained 82 push events, 54 edge commitments, and 59 same-bot/same-door episodes; none lasted five seconds and the 4.4-second maximum had only 0.9% nearly stationary frames while traveling 815 units. This is promising early evidence, not long-run or multi-map acceptance. Existing strafe/lean behavior and the separate wall-hugging concern are unchanged.
- Demo routes influence strategic destinations, but live Recast navigation and collision steering still determine movement between route points.
- Objective behavior works on tested rounds of `obj_team2` and has planted/defused on `obj_team4`, but map-specific regressions remain possible and must be tested separately.
- The CI branch-name filters now recognize `bots/server-ai`; the first post-retrofit server-only run passed.

## Known blockers and uncertainty

- The Windows checkout cannot locally prove the Linux x64 server package; GitHub Actions is the authoritative clean build.
- The current VPS hostname, credentials, service unit, game root, and deployed binary checksum are intentionally not stored in Git. Revalidate them from an approved external source before deployment.
- The external service was verified active after deploying the independently checked artifact for passing commit `1c1ed0be` on 2026-08-10. Its live binary pair matched that artifact, both intended UDP ports listened, and a fresh schema 7 triplet was recording; revalidate before later deployment because external state can change.
- Raw demo databases and telemetry captures live outside the repository; regenerate compact route data only from documented, approved inputs.

## Working tree at handoff

The pre-retrofit tree was clean. The intended handoff condition after committing and pushing these continuity files is also clean. If `git status -sb` shows anything else, treat it as authoritative and inspect every path before continuing.

## Validation recorded

- GitHub `Builds` succeeded for audited tip `676c85f1` under the former branch name on 2026-08-08.
- Branch tips were refreshed and verified equal locally/remotely after the GitHub rename.
- On 2026-08-09, all 7 route-generator tests and the continuity check passed on the retrofit working tree.
- On 2026-08-10, the focused roaming-phase/wall-veto experiment passed 6 source-contract tests, all 7 route-generator tests, `git diff --check`, and the continuity check. The source contract also prevents const inputs to the legacy mutating `Q_clamp` macro after CI exposed both refactored call sites.
- GitHub run `31406192630` passed for commit `1e30bea8`: only `Build Linux x64 dedicated server` ran, and artifact `openmohaa-bot-server-linux-x64` passed exact-file and x86-64 checks. The independently verified artifact was deployed successfully; the prior binary pair remains available outside Git as the exact rollback copy.
- On 2026-08-10, 11 focused movement/navigation source-contract tests, all 7 route-generator tests, `git diff --check`, and continuity passed for the door/oscillation fix. Run `31410902615` exposed one missing telemetry declaration; commit `13d2c465` fixed it mechanically, and server-only run `31411165741` then passed exact-file and x86-64 artifact checks. The independently verified artifact was deployed as one pair, with the exact prior pair retained outside Git for rollback.
- GitHub run `31316489080` passed for continuity commit `380331a0`: only `Build Linux x64 dedicated server` ran, and artifact `openmohaa-bot-server-linux-x64` passed the exact-file and x86-64 checks.
- Commit `b094fd87` passed the same 11 focused tests, all 7 route-generator tests, `git diff --check`, continuity, and server-only run `31415534984` with exact-file and x86-64 artifact checks. The first deployment transaction aborted on checksum-command quoting and verified exact automatic rollback; the corrected transaction installed the independently checked pair, restarted cleanly, and retained the prior pair outside Git.
- Commit `1c1ed0be` passed all 11 focused movement/navigation tests, all 7 route-generator tests, `git diff --check`, continuity, and server-only run `31417596004`. The exact x86-64 artifact hashes are `a93fe0b429c130dbec7a1b7888022977b5345ea1199ebdd9af6a8ca1f2794840` for `omohaaded` and `47b34b010c3798b7cd3df467ad58e87252b74761ea794db6504f47729e730c9d` for `game.so`; the deployed pair matches, the complete prior capture is preserved outside Git, and the exact `b094fd87` rollback pair remains on the VPS.

## Cross-branch status

- Peer branch last inspected at `0a5da85c` on 2026-08-09.
- The peer contains client-only commits for 4:3 resolutions, passive client telemetry, and compass-aware messages after shared commit `8f614e61`.
- No server navigation/objective commits after `8f614e61` have been approved for transfer to the macOS branch. Review individual hashes when that product needs them.

## Next action

Exercise deployed commit `1c1ed0be` longer on `obj_team2`, then on `obj_team4` and an FFA/practice map, deliberately pushing through doors opened by the player and by bots. Pull the fresh schema 7 triplet and compare `bot_door_pushthrough`/`bot_door_push_slide` episode duration, stationary share, panel clearance, route oscillation, recovery, and stalls against the complete `b094fd87` capture. Treat wall proximity, speed, lean, and roaming style as a separate secondary acceptance check.
