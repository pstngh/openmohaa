# Durable decisions

This log records owner-approved choices that a future session might otherwise undo. Git and current code remain authoritative about implementation details. Amend an entry when the owner changes the decision; do not silently contradict it elsewhere.

## D001 - Repository-first continuity

**Decision:** Git, tracked documentation, tests, and generated-data provenance are canonical. Chats are disposable execution contexts.

**Rationale:** Work spans many sessions and must be resumable by another AI without private conversation history.

## D002 - Two product lines

**Decision:** Keep the server line (`bots/server-ai` in `pstngh/openmohaa`) and the macOS client (`main` in `pstngh/openmohaa-macos-client`) independent. Use reviewed commit-level ports, not whole-branch merges.

**Rationale:** The server has experimental telemetry, routes, cheats, and server-only CI that must not leak into the normal macOS online client.

## D003 - Server-only artifact

**Decision:** The server line builds Linux x86-64 dedicated-server binaries only: `omohaaded` and `game.so`.

**Rationale:** The tuning VPS does not need client, renderer, audio, codec, or macOS artifacts. Commit `b6921faa` in the server repository established this boundary.

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

**Decision:** Cheats remain enabled on the private bot-tuning server while tuning is active and should remain easy to excise later. The macOS client must not inherit that server policy. Leave CPU scheduling at the OS default unless measurements justify pinning.

**Rationale:** The server is for controlled LAN/private bot work, while the macOS client is also used on ordinary online servers.

## D009 - macOS product boundary

**Decision:** The macOS product targets arm64 and combines the game client with the launcher. Launcher-only bot-match settings must remain scoped to launcher-started private sessions. Shared display choices, including the persisted fullscreen/windowed switch, apply to both normal and bot launches and default to existing fullscreen behavior. The first-person weapon/arms projection is fixed to the original aspect-corrected 80-degree 4:3 base FOV (approximately 96.42 degrees at 16:9), independent of world FOV, with no user setting. Client telemetry is opt-in, passive, and usable on unmodified public servers. Connect intentionally launches a separate owner-supplied `openmohaa` binary placed next to `launcher.app`, while bot matches launch the bundled `mohbots.app`; CI packages only `mohbots.app` and `launcher.app`, so Connect must not be redirected to the bundle.

**Rationale:** The same launcher and display preferences are used for both local bot matches and normal online multiplayer; locking the viewmodel preserves original weapon framing while allowing the world camera FOV to vary.

## D010 - Telemetry privacy and provenance

**Decision:** Do not commit raw captures. Server telemetry may intentionally capture delivered in-game chat so the owner can annotate tests; client telemetry omits names, chat, and network addresses. Generated route graphs must be reproducible from documented inputs and a deterministic generator.

**Rationale:** The analysis needs behavioral context without turning the repository into an archive of private match data.

## D011 - Upstream contribution restriction

**Decision:** Do not submit AI-generated code or documentation from this fork to upstream OpenMoHAA.

**Rationale:** Upstream `CONTRIBUTING.md` explicitly rejects AI-generated contributions.

## D012 - Build-wide gameplay rules

**Decision:** These rules apply to every game hosted with this build, not only to launcher bot matches. Leaning is allowed in every game mode, including single-player, and while moving without the Spearhead/Breakthrough lean dmflag; the server publishes `g_aalean` so clients predict its lean rules. The infinite ammo dmflag gives a bottomless clip. FFA and TDM ignore map-script spawn and respawn restrictions (`disablespawn`, `level.dmrespawning 0`, `stopteamrespawn`). Spectators stay on a followed player who dies. The stock `god` cheat still shows damage feedback. `g_healrate` defaults to 0 in every game.

**Rationale:** The owner reviewed these after the 2026-09-23 audit and kept them global. Fix defects inside them, but do not scope them to launcher sessions without a new decision.

## D013 - Bot reaction, firing, and aim

**Decision:** Every newly acquired target, including a switch from one target to another, waits `g_bot_attack_react_min_delay` before the bot may fire. A bot that stops to fire a weapon that is inaccurate on the move holds still for that frame instead of strafing. Aim error lies across the line of sight, so `g_bot_aim_error` is the actual miss distance at the target; the defaults were halved when this changed. Bots keep 360-degree vision without a fog-distance cap, and they strafe and lean even without an enemy.

**Rationale:** Chosen by the owner after the 2026-09-23 audit. The first three make the bots follow their tuning cvars; the owner left vision and strafing unchanged.

## D014 - Client defaults

**Decision:** The sun's lens flare follows `r_drawSun`, which defaults to 0, so it is off unless enabled. `cl_nullbind` defaults to 1 in the engine and in the launcher, and the launcher also applies it to Connect sessions.

**Rationale:** The owner kept these defaults after the 2026-09-23 audit.
