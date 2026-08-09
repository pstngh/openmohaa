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
