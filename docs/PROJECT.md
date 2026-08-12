# Project memory

## Mission

Develop a recoverable, evidence-driven OpenMoHAA fork with bots that move, fight, coordinate, plant, and defuse like human MOHAA players, plus a polished arm64 macOS client/launcher for local bot matches and normal online multiplayer connections. Git and tracked continuity files must allow a new AI session to resume without prior chat access.

## Intended users

- The repository owner, primarily for one- or two-human matches against six to eight bots.
- Friends connecting to private or internet-hosted bot matches.
- The repository owner's macOS client for local bot sessions, public-server play, and human movement recording.

## Current scope

- Tune bot combat, navigation, coordination, weapon choice, objective play, and movement from server telemetry and processed human demos.
- Maintain a Linux x64 dedicated-server-only build for bot testing and telemetry.
- Maintain an arm64 macOS client and launcher with bot controls, crosshair/nullbind options, compass-aware layout, and passive client telemetry.
- Preserve explicit branch boundaries while allowing selected shared bot fixes to be ported by commit.
- Maintain deterministic demo-derived route graphs for supported objective, FFA, and practice maps.

## Non-goals

- Training a machine-learning model inside the game runtime.
- Using realism-mode demos to tune normal weapon/combat behavior. Realism data may contribute physical route geometry only.
- Replaying fixed demo trajectories verbatim; routes should branch, merge, rejoin, and yield to live gameplay.
- Solving every OpenMoHAA upstream issue or redesigning unrelated engine systems.
- Automatically merging the server and client branches.
- Submitting this AI-assisted fork upstream in conflict with upstream contribution policy.

## Requirements and constraints

- Favor the smallest clear and robust implementation that explains observed behavior.
- Human telemetry and demos are evidence; do not infer behavior from anecdotes when measurable data exists.
- Bots should avoid wall/door/object collisions through navigation and steering, not through conspicuous stop-and-spin behavior.
- Normal movement should continue toward a meaningful route, objective, patrol/cover destination, sound, item, or enemy. Temporary overrides must return control cleanly.
- SMGs are the default bot weapon. Rifle, sniper, shotgun, StG, and BAR use explicit percentages; gameplay tuning currently prioritizes normal-mode SMG behavior.
- Do not introduce artificial burst fire without new human evidence.
- Objective attackers must advance, spread across useful routes, and plant. Defenders play normally before a plant, then prioritize the earliest planted bomb for defuse/defense.
- Client-only conveniences must not silently change normal public-server behavior.
- Raw private telemetry, demo archives, game assets, and infrastructure secrets remain outside Git.

## Primary deliverables

1. `bots/server-ai`: Linux x64 server-only bot AI, telemetry, route graphs, tests, and safe deployment runbook.
2. `bots/macos-client`: arm64 macOS client/launcher, local bot controls, client UI/input features, and passive human telemetry.
3. Repository-first continuity: a stable branch goal, durable decisions, and one live state snapshot.

## Acceptance criteria

- A fresh session can identify its branch, objective, completed work, known uncertainty, validation state, and one next action from tracked files alone.
- Server CI builds and packages only `omohaaded` and `game.so` on the server branch.
- Client CI produces the arm64 macOS application and launcher and passes its focused Python tests.
- Bot changes are evaluated with reproducible telemetry or focused tests and do not regress already-working objective maps without evidence.
- Cross-branch transfers identify the exact source commit and are retested on the receiving branch.

## Unresolved requirements

- Quantify and reduce the remaining wall, doorway, item, and back-and-forth navigation artifacts without making movement robotic.
- Decide which mature server bot changes should be ported to the macOS branch and when.
- Expand map-specific roles/routes only after sufficient demo evidence and map validation.
- Validate the newest compass layout/toggle and client telemetry in a real macOS public-server session.
