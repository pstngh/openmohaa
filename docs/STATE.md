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

## Partial or under evaluation

- The recent navigation series is implemented but has not been accepted as eliminating the owner's observed wall/door/item contacts or mindless back-and-forth. A 443,879-frame human comparison covering 16 `lawl` sessions and 8 bots found that off-center bots had lateral input on 96.96% of frames versus 52.13% for `lawl`, moving lean was about 88.95% versus 39.81%, and bot lateral commands pointed toward the nearer wall 65.14% of the time.
- An always-on noncombat centering replacement failed visual acceptance because it removed roaming personality and lean without eliminating wall contact. The owner ordered its branch history deleted and the prior binaries restored. Deployed commit `1e30bea8` contains the narrower replacement: alternating active/neutral roaming phases plus a comparative forward-clearance veto that cancels only an unsafe optional strafe. It does not reverse direction, center the bot, or alter the final collision guard. Live acceptance remains pending.
- Passing commit `13d2c465` is now deployed with the focused door/oscillation recovery changes. Fresh schema 7 recording began at deployment, but live acceptance is pending; this change deliberately leaves the uncertain strafe/lean experiment unchanged and does not claim to solve the remaining wall-hugging concern.
- Demo routes influence strategic destinations, but live Recast navigation and collision steering still determine movement between route points.
- Objective behavior works on tested rounds of `obj_team2` and has planted/defused on `obj_team4`, but map-specific regressions remain possible and must be tested separately.
- The CI branch-name filters now recognize `bots/server-ai`; the first post-retrofit server-only run passed.

## Known blockers and uncertainty

- The Windows checkout cannot locally prove the Linux x64 server package; GitHub Actions is the authoritative clean build.
- The current VPS hostname, credentials, service unit, game root, and deployed binary checksum are intentionally not stored in Git. Revalidate them from an approved external source before deployment.
- The external service was verified active after deploying the independently checked artifact for passing commit `13d2c465` on 2026-08-10. Its live binary pair matched that artifact and a fresh schema 7 triplet was recording; revalidate before later deployment because external state can change.
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

## Cross-branch status

- Peer branch last inspected at `0a5da85c` on 2026-08-09.
- The peer contains client-only commits for 4:3 resolutions, passive client telemetry, and compass-aware messages after shared commit `8f614e61`.
- No server navigation/objective commits after `8f614e61` have been approved for transfer to the macOS branch. Review individual hashes when that product needs them.

## Next action

Exercise deployed commit `13d2c465` on `obj_team2` plus an FFA/practice map, deliberately crossing doors opened by the player and by bots. Then pull the fresh schema 7 triplet and compare strict small-area loop rate/duration, route-oscillation events, lateral-versus-reverse recovery, repeated same-door contacts, open-door repaths, collision-guard suppression, and stalls. Treat wall proximity, speed, lean, and roaming style as a separate secondary acceptance check.
