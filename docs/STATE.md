---
last_updated: 2026-08-12 America/New_York
branch: bots/server-ai
peer_branch: bots/macos-client
current_product_commit: 953800996a840e7fd0b573f58b4ddd61337554e3
external_state_last_verified: 2026-08-12
---

# Server AI current state

Git and tests are canonical for implementation. External facts are timestamped and must be reverified before mutation.

## Current objective

Behaviorally accept or reject deployed `95380099`, an isolated fully-open-door-panel ownership experiment on top of the stable `b8dcc3b3` roaming build.

## Completed and validated

- The branch already contains configurable server bots, combat and objective behavior, schema 7 telemetry, deterministic demo-derived routes, navigation/recovery work, and Linux x64 server-only CI.
- `6a877fdc` corrected grounded ladder recovery direction. Its representative capture recorded 245 intended-side starts, 155 completions, 80 cancellations, and 7 timeouts; fast reattachment improved to 77.3% from roughly 63-64%. Exact-origin clustering still reached six, so ladder behavior is improved but not final.
- The same 4.308-session-hour capture established the door baseline: 6,210 actor/door episodes grouped across gaps up to 2.5 seconds, 153 at least five seconds, 50 at least ten seconds, and a 37.1-second maximum. Long cases travelled roughly 3,300-5,100 units but ended near their start while recovery, route oscillation, and recontact interleaved.
- A clean 3.90-server-minute schema 7 bot-only capture under deployed `3cd38ebf` recorded 497 world-guard episodes in 21.56 bot-alive minutes (roughly one per 2.6 seconds), 26.94% of moving ordinary frames below 12 units of forward-corner clearance, and 117 door episodes. During matched door-exit intervals only 49.3% of frames moved forward relative to view; 22.0% moved backward, 16.1% laterally, and 12.6% were nearly still. The owner rejected the visual result.
- The representative `5e82b49e` capture rejected both replacement experiments. World-guard cadence rose from 23.05 to 25.39 episodes per bot-minute, forward-corner clearance below 12 units remained 27.12% versus 26.94%, door completion fell from 60.5% to 42.1%, and cancellation rose from 32.6% to 52.8%.
- The same capture objectively confirmed lean flicker: ordinary movement produced roughly 91 lean transitions per minute, with 48.65% of lean episodes at most 250 ms and a 300 ms median. Direct left/right flips were rare; rapid active/neutral and geometry-interrupted on/off transitions were the dominant visual defect.
- `a447806e` keeps rotated schema 7 captures as timestamp-prefixed triplets directly beneath one `telemetry/segments` directory. This removes the per-segment child directory that failed on the VPS while preserving complete sessions and restart-time deletion. See D017.
- The exact passing `6a877fdc` artifact was restored and runtime-verified before source history changed, providing the deployment rollback baseline. Its hashes are `a9afbe5135f85151fa7488bd7fdca6f5d18ceee4b635f15aadf819a0d0788230` for `omohaaded` and `84dc45ae914d75031a9d9f3b165fe36397c7384c029093db155460253746e630` for `game.so`.
- Rejected gameplay commits `69a4a212` and `6a663e1d` are absent from the rewritten local ancestry. Flat telemetry and passive continuity were replayed independently.
- Product commit `b8dcc3b3` lengthens only noncombat active/neutral strafe phases from 400-900 ms to 1,200-2,700 ms. Combat timing, door logic, and the `6a877fdc` movement baseline remain unchanged.
- A fresh 20.293-server-minute schema 7 capture under that stable roaming build confirmed the separate door defect: 301 fully-open-panel phases produced only 73 clean completions (24.25%); 71 restarted the same door, 67 yielded to blocked recovery, 22 yielded to moving-door recontact, 5 timed out, and 42 lacked a terminal event. Fourteen actor/door episodes lasted at least five seconds and six lasted at least ten; the worst travelled 2,050.5 units for 34 units net while reversing 20 times.
- Product commit `95380099` changes only fully-open-panel collision ownership. A rotating panel prefers its physical free edge; same-door recontact retains one escape; generic recovery cannot interleave while it progresses; 12-unit directional checkpoints detect a 750 ms stall; completion remains 128 through-door units with a three-second maximum; stall/timeout grants recovery a two-second same-door retry window. Start and every observable terminal/cancellation are explicit. Moving-door, view, corner, lean, roaming, combat, ladder, and route ownership are unchanged. The 24 movement contracts, 3 telemetry tests, 7 route-data tests, continuity check, and `git diff --check` pass locally.
- GitHub server-only run `31645679441` passed for exact tip `95380099`; full/release/deploy jobs were skipped. Its artifact contained exactly ELF64 little-endian x86-64 `omohaaded` (`3c7267be7a6a62206e0b9ff70ea810da37457f88edddb73e317ba64e98059f09`) and `game.so` (`f63ca9d76ba256bd3b696449d67c1f175d04f839860f4b0f24b421c1f83b62b4`).
- That exact pair is deployed. Startup reports `9538009`, the intended `game.so` is loaded, `openmohaa.service` is active/enabled with zero restarts, UDP 12203/12300 listen, and a fresh growing schema 7 `obj_team2` triplet replaced the prior roughly 809 MB capture. The exact prior pair is preserved as `deploy-backups/pre-95380099`; the transient local artifact and remote `deploy-staging/95380099` were permanently deleted.

## Remaining work

1. Obtain owner visual feedback plus a representative `obj_team2` capture. Door start-to-terminal completion, cancellation reasons, repeated actor/door episodes, blocked/contact cadence, travel efficiency, and view-relative motion are the primary measures; lean, wall/guard, route-loop, objective, and combat metrics are regression controls.
2. If promising, check `obj_team4` and one FFA/practice map before acceptance. Keep residual ladder cycling and geometric wall placement separate.
3. Confirm the first live 1.5 GiB rotation creates one complete flat prefixed triplet without a service restart; this is operational validation, not a reason to retain raw telemetry.

## Blockers and unknowns

- `95380099` has build and runtime validation but no owner visual verdict or representative behavioral capture yet.
- The pre-change diagnostic capture contained no active `lawl` control or chat annotations. Historical matched telemetry remains the human reference.
- VPS state and telemetry may change after this timestamp. Fresh read-only inspection must override this handoff before mutation.
- Raw demos and telemetry are external and transient. Do not infer their presence or completeness from Git.

## Current guardrails

- Follow D012, D013, D018, D022, and D023. Do not restore the rejected D019/D021 coupled door/corner designs, always-on centering, blind door repathing, post-contact tangent carry, door-facing changes, or a forced ladder command gate.
- Hold map, bots, teams, cvars, and schema stable; never mix incomplete triplets, schemas, actors, or sessions.
- Telemetry is disposable and is never backed up. Delete every analysis pull after use.
- Rejected experiments require verified live rollback plus the exact clean-tree and force-with-lease history procedure in `docs/SERVER_OPERATIONS.md`.

## Cross-branch status

The peer was refreshed read-only at `b0f5debd` on 2026-08-12. No post-`8f614e61` server navigation/objective commit is approved for transfer. Use only explicit reviewed cherry-picks and update each branch's own state.

## Next action

Let the owner test deployed `95380099` on `obj_team2`, concentrating on full-door passages and repeated same-door contact. After the owner comments, reverify service identity read-only, pull every coherent schema 7 triplet transiently, compare start-to-terminal completion, cancellation reasons, blocked/contact cadence, five-/ten-second door episodes, travel efficiency, and view-relative motion against the recorded `6a877fdc`/`b8dcc3b3` baselines, delete the pull, and record one accept/reject verdict before any further movement edit.
