---
last_updated: 2026-08-12 America/New_York
branch: bots/server-ai
peer_branch: bots/macos-client
current_product_commit: 6a663e1dc37d9e2559935ef2b7dc2c5646ad8662
external_state_last_verified: 2026-08-12
---

# Server AI current state

Git and tests are canonical for implementation. External facts are timestamped and must be reverified before mutation.

## Current objective

Build, deploy, and behaviorally evaluate the independently reviewable `69a4a212` door-facing correction and `6a663e1d` anticipatory Recast corner clearance while retaining the validated `6a877fdc` ladder-route correction.

## Completed and validated

- The branch already contains configurable server bots, combat and objective behavior, schema 7 telemetry, deterministic demo-derived routes, navigation/recovery work, and Linux x64 server-only CI.
- `6a877fdc` corrected grounded ladder recovery direction. Its representative capture recorded 245 intended-side starts, 155 completions, 80 cancellations, and 7 timeouts; fast reattachment improved to 77.3% from roughly 63-64%. Exact-origin clustering still reached six, so ladder behavior is improved but not final.
- The same 4.308-session-hour capture established the door baseline: 6,210 actor/door episodes grouped across gaps up to 2.5 seconds, 153 at least five seconds, 50 at least ten seconds, and a 37.1-second maximum. Long cases travelled roughly 3,300-5,100 units but ended near their start while recovery, route oscillation, and recontact interleaved.
- A clean 3.90-server-minute schema 7 bot-only capture under deployed `3cd38ebf` recorded 497 world-guard episodes in 21.56 bot-alive minutes (roughly one per 2.6 seconds), 26.94% of moving ordinary frames below 12 units of forward-corner clearance, and 117 door episodes. During matched door-exit intervals only 49.3% of frames moved forward relative to view; 22.0% moved backward, 16.1% laterally, and 12.6% were nearly still. The owner rejected the visual result.
- `69a4a212` replaces the rejected door commit while retaining one bounded physical free-edge traversal. Its noncombat traversal view now follows the captured through-door approach until geometric completion or cancellation, so ordinary route look-ahead cannot make the bot back through the panel. See D019 and D021.
- `6a663e1d` samples both current and upcoming first-corner wall clearance before engaging the existing comfort inset. Both projections must stay in the selected Detour corridor and the existing width, short-goal, off-mesh, combat, direct-move, door, ladder, jump, and recovery gates remain intact. See D012 and D021.
- `a447806e` keeps rotated schema 7 captures as timestamp-prefixed triplets directly beneath one `telemetry/segments` directory. This removes the per-segment child directory that failed on the VPS while preserving complete sessions and restart-time deletion. See D017.
- The rewritten candidate passed all 28 bot/telemetry source contracts, 7 route-data tests, and `git diff --check` locally. Linux compilation and artifact verification remain pending CI.
- At the last external check on 2026-08-12, deployed `3cd38ebf` still matched its verified binary hashes, the service was active as PID 39991 with one automatic restart, intended UDP ports listened, and fresh primary schema 7 telemetry grew. The restart followed repeated zero-byte telemetry writes and failure to create a nested segment directory; disk, inode, unit sandbox, and file-size limits were not exhausted. The exact `6a877fdc` binary pair remained available for rollback.

## Remaining work

1. Push the exact rewritten branch with force-with-lease and require a passing server-only Linux x64 build.
2. Verify the artifact contains only x86-64 `omohaaded` and `game.so`, then reverify the VPS and deploy that exact pair with the prior pair preserved.
3. Amend the service-start reset to recreate an empty owned `telemetry/segments` directory after deleting it, restart through the deployment, and verify clean schema 7 growth.
4. Obtain owner visual feedback plus a representative capture on `obj_team2`; compare corner/world-guard cadence and door lifecycle/view-relative movement against the rejected snapshot.
5. If promising, check `obj_team4` and one FFA/practice map before acceptance. Keep residual ladder cycling separate.

## Blockers and unknowns

- The rewritten commits have not yet passed GitHub's Linux server-only job or been deployed. Windows cannot prove the artifact locally.
- The analyzed snapshot was short and bot-only; it decisively exposed frequent contact but cannot establish multi-map acceptance or compare new behavior with `lawl`.
- VPS state and telemetry may change after this timestamp. Fresh read-only inspection must override this handoff before mutation.
- Raw demos and telemetry are external and transient. Do not infer their presence or completeness from Git.

## Current guardrails

- Follow D012, D013, D018, D019, and D021. Do not restore always-on centering, blind door repathing, hinge selection, post-contact tangent carry, or a forced ladder command gate.
- Hold map, bots, teams, cvars, and schema stable; never mix incomplete triplets, schemas, actors, or sessions.
- Telemetry is disposable and is never backed up. Delete every analysis pull after use.
- Rejected experiments require verified live rollback plus the exact clean-tree and force-with-lease history procedure in `docs/SERVER_OPERATIONS.md`.

## Cross-branch status

The peer was last recorded read-only at `b0f5debd` on 2026-08-10 and is stale until fetched again. No post-`8f614e61` server navigation/objective commit is approved for transfer. Use only explicit reviewed cherry-picks and update each branch's own state.

## Next action

Push the clean rewritten tip with an exact force-with-lease against verified origin `89791cd8`, wait for a passing server-only Linux x64 build, verify its two-file artifact, and only then reverify and deploy it to `openmohaa-bot-vps` with the service-start segment-directory recreation included.
