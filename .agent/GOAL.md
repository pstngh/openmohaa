# Server AI north star

## Mission

Make OpenMoHAA bots on `bots/server-ai` move, roam, fight, coordinate, plant, and defuse with the committed, anticipatory behavior of capable human MOHAA players. Improve the smallest responsible behavior owner from measured human and bot evidence while preserving compatibility and useful bot personality.

The primary use is one or two humans playing objective and FFA matches against roughly six to eight bots on a Linux tuning server.

## Success

- Bots maintain purposeful progress instead of hugging walls, bouncing, hesitating, looping locally, or deadlocking on doors and ladders.
- Strafing, leaning, route choice, crosshair placement, combat control, and objective intent remain expressive rather than mechanically centered.
- Attackers advance and plant; defenders play map control, then intentionally defend or defuse planted objectives.
- Behavioral claims have focused tests, comparable schema 7 telemetry, owner observation, and relevant multi-map checks.
- The branch produces only the verified Linux x86-64 dedicated-server pair: `omohaaded` and `game.so`.

## Constraints and non-goals

- Preserve original assets, scripts, networking, multiplayer clients, mods, and normal behavior unless a narrowly approved change requires otherwise.
- Normal-mode human evidence drives normal tuning. Realism data may contribute physical route geometry only.
- Do not embed machine learning, replay fixed demo trajectories, or introduce artificial burst fire without new evidence.
- Prefer correcting an existing owner over adding another steering system.
- Raw telemetry and demos remain transient and outside Git; credentials and deployment secrets are never persisted.
- Do not solve unrelated engine issues, develop macOS client/launcher features here, or port experiments automatically.
- Rejected server experiments do not accumulate in canonical history; remove only the proven range with an exact lease-safe rewrite.
- Do not send AI-generated work upstream.

## Branch boundary

This goal and live state belong only to `bots/server-ai`. Shared fixes move to `bots/macos-client` only through explicit reviewed cherry-picks. D020 records why this branch is sufficient and when a separate fork would become appropriate.
