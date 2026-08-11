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

**Decision:** A bot that detaches from a ladder while grounded should briefly continue in the ladder's existing exit direction before pathing or blocked recovery can regain control, using the same bounded commitment already used at the top. A pistol user inside melee range should mix deterministic, cadenced primary shots with edge-triggered bashes instead of bashing exclusively. Do not reinterpret normal ladder ascent as a teammate stop or apply strategic route-loop resets to target-chase/orbit movement.

**Rationale:** Annotated frames showed repeated grounded ladder detach/reattach caused by recovery taking control immediately, while the reported teammate pause was actually steady vertical ladder progress. Combat telemetry showed SMGs already firing aggressively and close pistol users choosing only melee despite loaded primary ammo. Keeping both corrections inside their existing owners addresses the measured behavior without adding another steering layer or weakening combat constraints.
