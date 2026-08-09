---
last_updated: 2026-08-09 America/New_York
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
- Latest navigation series through `676c85f1` addresses multilevel progress, openable doors, door panels, blocked recovery, ladders, unreachable route hops, and shared Recast collision steering.

## Partial or under evaluation

- The recent navigation series is implemented but has not been accepted as eliminating the owner's observed wall/door/item contacts or mindless back-and-forth. Do not stack another speculative fix without a fresh post-build telemetry comparison.
- Demo routes influence strategic destinations, but live Recast navigation and collision steering still determine movement between route points.
- Objective behavior works on tested rounds of `obj_team2` and has planted/defused on `obj_team4`, but map-specific regressions remain possible and must be tested separately.
- The continuity retrofit updates CI branch-name filters from the former branch name. The first CI run containing that update remains to be verified.

## Known blockers and uncertainty

- The Windows checkout cannot locally prove the Linux x64 server package; GitHub Actions is the authoritative clean build.
- The current VPS hostname, credentials, service unit, game root, and deployed binary checksum are intentionally not stored in Git. Revalidate them from an approved external source before deployment.
- Repository history proves implementation and CI results, not whether the latest binary is currently deployed or whether existing telemetry was captured from it.
- Raw demo databases and telemetry captures live outside the repository; regenerate compact route data only from documented, approved inputs.

## Working tree at handoff

The pre-retrofit tree was clean. The intended handoff condition after committing and pushing these continuity files is also clean. If `git status -sb` shows anything else, treat it as authoritative and inspect every path before continuing.

## Validation recorded

- GitHub `Builds` succeeded for audited tip `676c85f1` under the former branch name on 2026-08-08.
- Branch tips were refreshed and verified equal locally/remotely after the GitHub rename.
- On 2026-08-09, all 7 route-generator tests and the continuity check passed on the retrofit working tree.
- The rename itself triggered workflows before the server-only name filters were updated; those transitional runs do not validate the final branch configuration.

## Cross-branch status

- Peer branch last inspected at `0a5da85c` on 2026-08-09.
- The peer contains client-only commits for 4:3 resolutions, passive client telemetry, and compass-aware messages after shared commit `8f614e61`.
- No server navigation/objective commits after `8f614e61` have been approved for transfer to the macOS branch. Review individual hashes when that product needs them.

## Next action

Push the continuity/CI-name commit, then verify that the `Builds` workflow for `bots/server-ai` runs the single `Build Linux x64 dedicated server` job and packages only `omohaaded` and `game.so`.
