# Durable decisions

This log records owner-approved choices that a future session might otherwise undo. Git and current code remain authoritative about implementation details. Amend an entry when the owner changes the decision; do not silently contradict it elsewhere.

## D001 - Repository-first continuity

**Decision:** Git, tracked documentation, tests, and generated-data provenance are canonical. Chats are disposable execution contexts.

**Rationale:** Work spans many sessions and must be resumable by another AI without private conversation history.

## D002 - Two product branches

**Decision:** Keep `bots/server-ai` and `bots/macos-client` independent. Use reviewed commit-level ports, not whole-branch merges.

**Rationale:** The server has experimental telemetry, routes, cheats, and server-only CI that must not leak into the normal macOS online client.

## D003 - Server-only artifact

**Decision:** `bots/server-ai` builds Linux x86-64 dedicated-server binaries only: `omohaaded` and `game.so`.

**Rationale:** The tuning VPS does not need client, renderer, audio, codec, or macOS artifacts. Commit `b6921faa` established this boundary.

## D004 - Evidence-driven normal-mode bots

**Decision:** Tune normal bots primarily from normal-mode human telemetry and demos, especially SMG play. Realism demos may add route geometry but not movement/combat probabilities. Do not add artificial burst fire without evidence.

**Rationale:** Realism weapon balance and camping/movement style do not represent the intended bot matches; apparent firing gaps in early samples were likely peeking rather than burst logic.

## D005 - Movement and navigation intent

**Decision:** Bots should know navigable space, pre-turn along their path, and avoid walls, items, and doors without visibly stopping to stare or spin. Narrow spaces may suppress incompatible strafe components, but movement overrides must return to a meaningful path or target.

**Rationale:** Human players anticipate walls. Remaining collision, doorway oscillation, and local back-and-forth are defects to measure, not desired personality.

## D006 - Weapons and combat spacing

**Decision:** SMG is the fallback/default loadout. Rifle, sniper, shotgun, StG, and BAR shares are independently configurable. Pistol users may close to the weapon's actual bash range. A bot that switches to pistol during a reload should return to its main weapon afterward.

**Rationale:** Intended matches are predominantly SMG; pistol melee range must come from weapon data rather than guesswork.

## D007 - Objective behavior

**Decision:** Attackers must advance while clearing multiple useful routes, plant, and defend planted bombs. Defenders play normal map control before a plant, then intentionally approach the earliest planted bomb to defuse/defend it. They should not permanently camp bomb sites before a plant.

**Rationale:** This matches coordinated human objective play and avoids spawn camping/idling observed in early bot builds.

## D008 - Tuning server policy

**Decision:** Cheats remain enabled on the private bot-tuning server while tuning is active and should remain easy to excise later. The normal client branch must not inherit that server policy. Leave CPU scheduling at the OS default unless measurements justify pinning.

**Rationale:** The server is for controlled LAN/private bot work, while the macOS client is also used on ordinary online servers.

## D009 - macOS product boundary

**Decision:** The macOS product targets arm64 and combines the game client with the launcher. Launcher-only bot-match settings must remain scoped to launcher-started private sessions. Client telemetry is opt-in, passive, and usable on unmodified public servers.

**Rationale:** The same client is used for both local bot matches and normal online multiplayer.

## D010 - Telemetry privacy and provenance

**Decision:** Do not commit raw captures. Server telemetry may intentionally capture delivered in-game chat so the owner can annotate tests; client telemetry omits names, chat, and network addresses. Generated route graphs must be reproducible from documented inputs and a deterministic generator.

**Rationale:** The analysis needs behavioral context without turning the repository into an archive of private match data.

## D011 - Upstream contribution restriction

**Decision:** Do not submit AI-generated code or documentation from these branches to upstream OpenMoHAA.

**Rationale:** Upstream `CONTRIBUTING.md` explicitly rejects AI-generated contributions.

## D012 - Recast corridor comfort without global erosion

**Decision:** Keep Recast's global agent radius equal to the physical player half-width. For ordinary bot route travel only, wall comfort may adjust the first Detour corner when the proposed point remains within the existing polygon corridor and proves adequate clearance. Tight corridors, partial queries, short goals, off-mesh links, and explicit combat/movement owners retain the original corner.

**Rationale:** Telemetry showed repeated full-speed lanes at near-zero hull clearance even when optional strafe and collision avoidance were inactive, because a physically valid shortest path may run on the eroded navmesh boundary. Increasing the global radius could remove valid doors and passages, while the rejected always-on input-centering layer damaged visible roaming personality without fixing contact.

## D013 - Human-style traversal commitment

**Decision:** A bot opening a rotating door should immediately favor the panel's free edge away from the hinge, falling back to the other side only when the opening edge is materially blocked. Ordinary wall comfort belongs in the existing Recast corridor primitive, proven local route loops should reseed without an idle cooldown, and short lean gaps or side changes should use hysteresis. Do not restore an always-on final-input centering layer or carry a door tangent after panel contact ends.

**Rationale:** Owner-annotated telemetry showed repeated panel contacts, hinge-side corrections, wall-side ordinary paths, route loops, and lean flicker. The owner's human movement instead combines strong strafe and lean with committed forward progress, so the correction must preserve personality while stabilizing the geometric target and traversal state.

## D014 - Crosshair-led ordinary traversal

**Decision:** During ordinary noncombat route travel, place the bot's view a short deterministic distance ahead along the already-selected path instead of locking it to the current movement vector. This is observational view placement only: it must not issue movement input, choose a route, repath, or displace combat, direct movement, door, collision-avoidance, recovery, ladder, or jump ownership. Use route height for natural ramp/stair pitch and retain current-direction aim as the fallback.

**Rationale:** Matched telemetry showed `lawl` routinely looking away from current velocity and toward future travel while bots aligned their view tightly with the current command. Human crosshair placement is therefore part of anticipatory traversal: the view leads a corner and movement catches up. Keeping the preview inside the existing path avoids another steering layer and preserves the movement rules already improved from owner feedback.

## D015 - Focus grounded ladder exits and close-pistol pressure

**Decision:** Ladder exit direction is attachment-position-specific: a completed top exit follows `+facingDir`, while a grounded bottom detach follows `-facingDir` because `FuncLadder::PositionOnLadder` places the user behind the panel. The bounded exit commitment must reject immediate reacquisition of the same ladder volume before pathing or recovery regains control. A pistol user inside melee range should still mix deterministic, cadenced primary shots with edge-triggered bashes instead of bashing exclusively. Do not reinterpret normal ladder ascent as a teammate stop or apply strategic route-loop resets to target-chase/orbit movement.

**Rationale:** Annotated frames originally showed repeated grounded ladder detach/reattach, while the reported teammate pause was steady vertical progress. The first grounded implementation used the top direction and allowed attachment to cancel its commitment; its complete capture reattached within 1.5 seconds on 88.5% of grounded exits and repeated exact origins up to 16 times in 30 seconds. Ladder geometry and the measured regression require opposite bottom direction plus retained ownership. Combat telemetry separately showed close pistol users choosing only melee despite loaded primary ammunition.

## D016 - Fully open door panels need a physical escape owner

**Decision:** Keep a fully open door panel as ordinary displaced collision geometry, but when the final movement guard traces that panel, commit to the clearer tangent toward one physical panel end until contact clears. Retain the through-door approach only for the existing short exit phase. Do not repath, reopen the door, or override movement when no valid panel tangent exists.

**Rationale:** The complete `66af0cff` capture contained a 16.6-second near-stationary stall against a fully open panel while route planning and the final guard repeated. Pure collision-plane projection can reduce a square-on route command to zero. A stable, locally probed tangent removes that physical deadlock without restoring the rejected door-specific route owner.

## D017 - Segment always-on server telemetry before 2 GiB

**Decision:** Keep always-on telemetry as complete schema-consistent triplets and rotate before the engine's signed 2 GiB file boundary. Each 1.5 GiB frame segment closes with `session_end`; recording continues as timestamp-prefixed frame/event/metadata files directly beneath one `telemetry/segments` directory with headers and a new session. Do not create another directory per segment. Service-start cleanup must remove the exact primary triplet and segment subtree without an archival copy, then recreate only the empty owned segment directory. Telemetry pulls are transient analysis inputs and are deleted after analysis.

**Rationale:** The `66af0cff` logger stopped at 2,153,632,622 frame bytes because the next file-length check overflowed its internal signed representation and misclassified the triplet. A later live rotation emitted repeated zero-byte writes and terminated the server when its per-segment child directory could not be created despite available disk, inodes, and unrestricted file size. Flat prefixed triplets require only one pre-created writable directory, preserve grouping and analysis boundaries, and honor the owner's restart-reset/no-backup policy.

## D018 - Keep geometric door completion; correct ladder route and Recast direction ownership

**Decision:** Keep the fully-open-panel phase from `5940d700`: after contact clears, carry only the captured through-door approach until 128 units of progress or three seconds, with the panel-end tangent contact-only and world collision retaining cancellation. After a grounded ladder's existing 64-unit exit owner releases, observe for a real reattachment; if it occurs, detach and rebuild a reachable ordinary pather corridor with `FindPathAway` toward the established away side for 128 units or at most three seconds. `FindPathAway` must derive its search angles after converting the preferred direction to Recast space; game yaw must never be applied directly to Recast X/Z axes. Do not force a command vector or mark normal route movement suppressed. Explicit higher-level movement may supersede the temporary route and must emit a cancellation event with an owner reason. Do not add door repathing, post-contact tangent carry, global centering, or ordinary wall-placement changes.

**Rationale:** The complete `5940d700` capture supports retaining the door half: five-second episodes fell from 14/346 to 7/368 and 127 of 130 geometric phases completed. Its ladder command gate failed, so route ownership replaced it. The first route implementation also failed: 39 starts produced zero completions, and all 39 destinations projected exactly -128 units onto the intended away vector. The defect was deterministic coordinate misuse: game `(x,y,z)` converts to Recast `(x,z,-y)`, so direct game yaw reversed every observed Y-axis escape. Twenty-three starts also disappeared without a terminal event because explicit movement owners cleared the route. Correct the shared path primitive, expose every cancellation and active frame, and keep wall placement plus door residuals separate.

## D019 - Reject the unified physical free-edge door traversal

**Decision:** Reject and remove the D019 unified opening/fully-open traversal. Restore the `6a877fdc` door baseline while treating remaining door contact as an unresolved separate owner. Do not restore this experiment without new evidence and an isolated design.

**Rationale:** The representative replacement capture improved view-relative forward motion but made actual traversal worse: completion fell from 60.5% to 42.1%, cancellation rose from 32.6% to 52.8%, and blocked-door cadence remained roughly 24 events per server-minute. Visual feedback also rejected it.

## D020 - Branch-local passive continuity

**Decision:** Keep server-AI continuity only on `bots/server-ai` as passive Markdown. The branch is sufficient isolation; create a separate fork only if the workstream later needs its own permissions, secrets, issues, releases, ownership, or similar repository administration. Cross-workstream changes still move only through explicit reviewed cherry-picks.

**Rationale:** The user requires branch-specific continuity. A long-lived branch already isolates code history, live state, CI selection, and deployment responsibility, while another fork would add synchronization and administration without improving product isolation. Passive files remain reviewable and removable with zero effect on project operations.

## D021 - Reject coupled door-facing and anticipatory-corner replacement

**Decision:** Reject and remove both D021 product changes. Keep view ownership and the existing D012 first-corner comfort behavior at the `6a877fdc` baseline until either residual is isolated with new evidence.

**Rationale:** The replacement did not improve the primary contact criterion. World-guard cadence worsened by about 10%, from 23.05 to 25.39 episodes per bot-minute, and forward-corner clearance below 12 units remained effectively unchanged at 27.12% versus 26.94%. The door half also regressed as recorded in D019.

## D022 - Stable roaming strafe and lean intent

**Decision:** Preserve alternating noncombat active and neutral roaming personality, but hold each phase for three times the configured combat strafe interval. A wall-clearance veto or recovery/collision interruption ends the current active phase and requires a full neutral phase before another lean/strafe intent. Keep combat strafe timing, geometry veto direction, final collision ownership, lean-release grace, and side-switch hysteresis unchanged.

**Rationale:** The rejected capture recorded roughly 91 ordinary lean transitions per minute, a 300 ms median lean episode, and 48.65% of episodes at or below 250 ms. Direct left/right flips were rare, so the visible twitch was active/neutral and geometry-interrupted on/off flicker rather than insufficient side-switch hysteresis. Longer stable phases lower transition cadence without removing lean, centering the bot, or adding another steering owner.

## D023 - Evaluate one fully open door owner in isolation

**Decision:** Evaluate `95380099` as a door-only experiment. Once a fully open panel enters the final guard, one locally probed escape owns movement while it makes directional progress. Rotating panels prefer the physical free edge away from the hinge; same-door recontact retains that edge; generic blocked recovery cannot interleave. A 750 ms directional stall or the existing three-second maximum ends the owner and grants recovery a two-second same-door retry window. Keep completion at 128 through-door units and emit an explicit start plus every observable terminal/cancellation. Do not change view, corner comfort, leaning, roaming, combat, ladders, moving-door behavior, or routes. If owner observation and comparable telemetry reject it, restore the exact prior deployed pair and remove the experiment lease-safely before starting another design.

**Rationale:** The post-`b8dcc3b3` diagnostic capture produced only 73 clean completions from 301 fully-open-panel phases. Same-door restart (71), generic blocked recovery (67), and moving-door recontact (22) directly interrupted most attempts. Fourteen actor/door episodes lasted at least five seconds; the worst travelled 2,050.5 units for only 34 units net with 20 reversals. This identifies competing state ownership and hinge-side selection without supporting another coupled door-facing, corner, repath, or post-contact-tangent design; D019 and D021 remain rejected.
