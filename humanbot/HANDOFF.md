# Handoff: human-imitation bots

For a Claude Code session picking this work up. The state is as of 2026-10-02.

## What this is

The bots of this fork (github.com/pstngh/openmohaa, public) run one data-fitted, stochastic brain
learned from five recorded people (397 player-minutes of 1v1 SMG duels). They move, aim, fire and
look like those players. Each bot draws a style family (presser, strafer, stopper), then its own
dials. Read [README.md](README.md) for how it works and [TESTING.md](TESTING.md) for the test
procedure.

- **Code:** all the work is on branch `claude/funny-cray-e3yyih`. `main` is untouched and no PR
  is open; open one only if the owner asks.
- **Human data:** github.com/pstngh/openmohaa-movement (private) holds the human recordings and
  their analysis. It is needed only to refit the model, calibrate, or compare bots with humans,
  checked out next to the fork (`../openmohaa-movement`, or `$HB_MOVEMENT_REPO`). Its own
  CLAUDE.md rules apply, and this work never commits to it.

## State

Built and pushed:
- the telemetry logger (`g_movelog`, schema 13);
- the brain library (`code/humanbot`) and the engine glue (`code/fgame/humanbot_*.cpp`);
- fitting and calibration (`humanbot/fit`) and evaluation (`humanbot/eval`);
- test harnesses: `hb_replay` (recorded traces), `hb_arena` (closed loop on the real Pmove),
  `pm_harness`;
- the owner kit: `humanbot/server/duel.cfg`, `soak.cfg`, `names.txt`, and `pack_capture.py`.

Verified:
- `ctest` passes 8/8, with both GCC RelWithDebInfo and clang Debug.
- CI is green: Builds on Linux, macOS and Windows, Unit Tests, and the Python checks.
- In the arena, two average-style bots are within 25% of the human value on 58% of 195
  statistics (median relative error 0.17), and on 49% of the 43 pre-aim statistics (median
  0.26); four seeds of 900 s, model `c29371288` of 2026-10-02 on a Linux workstation. On
  2026-09-28 it was 58% (0.20) in the build container and 53-55% on macOS.
- 16 bots take 90 us per bot per tick on that workstation (112 in the build container); the
  budget is 150.
- On the four practice maps, bot against bot (`humanbot/eval/reports/`), see "Corner pre-aim"
  below.

**The engine glue runs** (first live runs, 2026-09-28, on macOS and on the VPS):
- the model loads, bots join and fight, the navmesh is valid, the map prior reads "checksum ok"
  on dm/crnodoors and the visibility table is built and cached. Until `fabcc2d7` (2026-10-02)
  dm/main, dm/vents and dm/downladder rejected their recorded prior (its checksum was read as a
  float) and ran on a navmesh-derived map whose visibility never finished: the live runs of
  09-28 and the baseline report on those three maps had no recorded prior;
- no stuck bout over 2 s and no keyboard violation, with 2 and with 16 bots;
- think time on the VPS (2 bots): 58-67 us per bot per tick. The engine's traces (sight,
  clearance) cost more than the arena's world, so arena think times understate the engine's.
  On macOS a mostly idle server runs on the efficiency cores and reads 2-3x slower.

`humanbot_selftest 180` PASSes on the VPS since the wall reflex (wall pressure 0.5 per bot-minute
over all bots, 47-52 us per bot): see "Wall contact" below.

The VPS runs the test server as the systemd service `openmohaa-humanbot` (UDP 12403; binaries
in `~linuxuser/moh-humanbot`, home `~linuxuser/moh-humanbot-home`, source
`~linuxuser/openmohaa-humanbot-src`). The owner's normal server there (`openmohaa`, 12203) is
not part of this work: never touch it.

## The task now: the test server

Use the Linux VPS the human duels were recorded on; the owner plays from a Mac. Its telemetry
lives under `~/.local/share/openmohaa/main/`. Follow TESTING.md:

1. **Build on the server**, server only, no install:
   ```sh
   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_CLIENT=OFF
   cmake --build build
   (cd build && ctest --output-on-failure)
   ```
2. **Start it.** Copy `humanbot/server/*.cfg` into `~/.local/share/openmohaa/main/` (and
   `humanbot/server/names.txt` to `main/humanbot/names.txt` for the disguise test), then run
   ```sh
   build/RelWithDebInfo/omohaaded +set fs_basepath /path/to/mohaa +set com_target_game 0 +exec duel.cfg
   ```
   `fs_basepath` must hold `main/` with the pak files and the practice maps. The server uses UDP
   port 12203. `sv_maxbots` has to be above 0 before a map loads: it reserves the slots and makes
   the navmesh get built. `duel.cfg` sets it.
3. **Smoke test** in the server console:
   - At startup: `humanbot: model <sha12> (embedded)`.
   - `humanbot_selftest 60`: navmesh valid, map prior "checksum ok" on dm/crnodoors, dm/main,
     dm/vents and dm/downladder, visibility ready, then `PASS`.
   - `humanbot_list`: each bot's style and think time.
4. **Duels:** 20-30 min per practice map. Then copy the telemetry to the Mac,
   `pack_capture.py` it into `humanbot/captures/`, push, and run `compare.py`.
5. **Soak:** `soak.cfg`, 16 bots for a few hours.

### Likely first-run problems, in pipeline order

1. **Model does not load.** The line above `no usable model` says why. `g_humanbot_model_dir`
   names a folder in the game filesystem whose `shared.json`, `styles.json` and
   `calibration.json` are merge patches over the embedded model. Leave it empty.
2. **Bot not created or not joining.**
   - A human bot is attached in `BotController::AttachHumanBot` (`playerbot.cpp`) when the model
     loaded.
   - It picks the SMG (`primarydmweapon smg`), then joins 0.5-2 s after connecting: axis or
     allies by its MP40 share in free-for-all, auto-join in team modes (`Join` in
     `humanbot_adapter.cpp`).
3. **Map context.**
   - `checksum MISMATCH` on a practice map means the map file differs from the recorded one; the
     bots then use a prior derived from the navmesh.
   - The visibility table builds 2 ms per frame and is cached in
     `~/.local/share/openmohaa/main/humanbot/vis/`.
4. **Movement.**
   - Usercmds go through `G_ClientThink` 4 times per frame (`Commit`, `g_humanbot_substeps`).
   - Look for jitter, bots standing still, or bots stuck on doors and ladders.
   - The stock code takes over for ladders and doors, and for 750 ms when a bot is stuck for
     1.5 s or pushes into a wall for 1 s. `g_humanbot_debug 1` prints every hand-off.
5. **Perception.**
   - Sight uses the skeleton tags "Bip01 Head/Spine2/Spine1/Pelvis/L Foot/R Foot", with a bbox
     fallback (`humanbot_perception.cpp`).
   - A bot that never reacts to a visible enemy points here.
6. **Firing and reload:** the buttons of each usercmd, `SendCommand("reload")` and the weapon
   switch (`useWeapon`), all in `Commit`.

Debugging:
- The telemetry has one `bot_*` column per diagnostic in `code/humanbot/hb_diag.h`: context,
  focus, detection, belief, view mode, flick, trigger hazards, chord, owner, think time.
- For a brain problem, reproduce it offline in `hb_arena`, which runs the same brain. Keep
  engine-glue fixes in `code/fgame/humanbot_*.cpp`.

## Working rules

- **Privacy: the fork is public.** Only aggregates belong in it:
  - the pooled model;
  - the anonymous style distribution (no aliases);
  - map priors;
  - the pooled human reference;
  - the owner's own captures against bots (alias `pstN`, which may be named).

  Never commit raw human captures, per-frame or per-person tables, or any other player's alias.
  `humanbot/tools/check_no_raw_data.py` enforces this in CI.
- **Human-fair perception.** Never give a bot an enemy's true hidden position (not even for
  evaluation feedback).
- **Before every push** (the Python checks need `pip install -r humanbot/eval/requirements.txt`):
  ```sh
  (cd build && ctest --output-on-failure)
  python3 humanbot/tools/embed_model.py --check
  python3 humanbot/tools/check_no_raw_data.py
  python3 -m unittest discover -s humanbot/eval/tests
  ```
- **Model changes** go through the scripts, never hand edits of `shared.json`:
  - edit `humanbot/fit/*.py`;
  - run `humanbot/fit/run_fits.sh` (it needs the data repo);
  - run `humanbot/fit/calibrate.py --stage all --build build`, which writes `tuning.json` and
    `calibration.json`, reassembles and re-embeds. The test binaries (`hb_arena`, `hb_replay`)
    are in `build/`; the server's (`omohaaded`, `game.so`) in `build/RelWithDebInfo/`.
  - `hb_replay` needs the git-ignored replay exports: `humanbot/fit/export_replay.py` recreates
    them.
- **Arena check:**
  ```sh
  build/hb_arena --seconds 900 --pooled --seed 1 --reference humanbot/eval/human_reference.json
  ```
  It prints bot vs human for every shared statistic. `--bots 16 --load` checks the think budget.

## Wall contact (engine)

The first live runs showed the biggest tell so far: the bots ran into walls. Measured from the
logger's own columns, the same way for people and bots (2 bots per map, same seed):

| per map: bots before -> now (people) | crnodoors | main | vents | downladder |
|---|---|---|---|---|
| time within 16 u of a wall | 54% -> 25% (26%) | 63% -> 37% (28%) | 61% -> 36% (33%) | 56% -> 37% (22%) |
| touching a wall | 28% -> 6% (5%) | 29% -> 10% (7%) | 36% -> 8% (9%) | 33% -> 10% (4%) |
| wall contacts per minute | 44 -> 5.3 (4.6) | 44 -> 5.3 (5.5) | 39 -> 5.6 (7.7) | 38 -> 8.8 (3.4) |
| of those held 500 ms or more, per minute | 3.9 -> 0.2 (0.1) | 2.8 -> 0.2 (0.3) | 1.4 -> 0.05 (0.4) | 2.6 -> 0.6 (0.04) |

What did it, in order of effect:
- **The wall reflex** (`hb_movement.cpp` `WallAhead`, `wall_reflex_ms` 300 and `wall_reflex_logit`
  4 in the model's couplings, set by hand in the engine): a held key whose wall is reached within
  300 ms at the current speed is let go of (the forward key only lets go, it never backs off), and
  no new key presses into a wall the bot touches. The key processes are fitted on people who steer
  along walls with the mouse; they barely react to walls, and the bots lacked the anticipation.
  The bot's clearance probes retry 2 u higher when they start in solid, and the reflex stands down
  when every probe is blocked (it froze the bots before that).
- **Walls in the fitted key choice** (`fit_keys.py`: `choice_wall_logit`, `diag_wall_logit`).
- **Wall steering** (`humanbot_adapter.cpp`, `g_humanbot_wall_steer`): the navmesh is built for a
  1 u agent, so its corners lie inside the player's box; walls push the route sideways.
- **The route look** (`hb_view.cpp`): on the move the view also looks down the route (and follows
  it as it turns), not only with a diffuse belief.

Still off: the bots move 60-70% of the time (people 77-84%; the reflex costs 4-7 points: after
letting go at a wall a bot can idle before the next key, where a person turns along the wall), and
on the move they hold the forward key less (35-45%, people 67-75%) because their view is on the
believed enemy more than on the route. In the arena the reflex costs about a point of the
statistics within 25% (retreats at 96-224 u, already a gap, grow).

## Corner pre-aim (2026-10-02)

People mostly see the enemy where they already aim: 500 ms before the first visible body part
their crosshair is a median 5.8 deg from the edge of cover the enemy comes out from, on its cover
side (the data repo's REPORT section 14). The bots now do the same:
- `hb_belief` gives each exposure (where a believed path first enters the bot's view) its corner:
  from the bot's eye it traces the believed path at head height to where it crosses out of cover,
  then the line to its last hidden point to where it meets the map. The traces ask the map only
  (`WorldQuery`, `CONTENTS_BODY` left out), so no answer depends on a hidden enemy. Corners are
  cached per exposure cell, at most 2 new ones per tick, none nearer than 96 u.
- `hb_view` weighs look decisions toward a corner by its imminence (belief mass x
  exp(-eta / 1 s); `preaim_weight` 60) and breaks other looks off for one (`preaim_hazard` 0.3).
  The view waits 1.8 deg onto the cover side and 2.7 deg below (people's medians), turns onto it
  in one flick from 3 deg off, follows the corner as it is re-traced, and holds it with the
  tracking gain and full own-motion compensation. The fitted still gate keeps working there.
- The trigger's sight is any perceived body part, timed from when the parts came on screen
  (`fit_trigger.py` on the rebuilt `ext_vis_parts`), and with only parts showing the view aims at
  a visible part.

The `preaim_*` policy was set by hand on real-map captures, because the arena's pillars are not
the recorded maps. Each capture was 8 dedicated servers, 2 per practice map (seeds 101-108),
2 bots each, at `timescale 10` for 240 s (about 40 game-minutes), scored with `compare.py
--moh-dir`. The weight saturates near 20; a hazard of 1.0 is no better than 0.3. A rule that
stood the still gate down while the view was over 3 deg off its corner made the hidden view three
times as lively as people's (yaw speed p50 24 vs 8 deg/s, mouse still 21% vs 32%) for no pre-aim
gain, and was removed.

Final check on held-out seeds 201-208 (`eval/reports/2026-10-02_preaim_realmaps`):

| practice maps, bot vs bot | people | baseline (`2ba0b347`) | prior fix only | now |
|---|---|---|---|---|
| aim error at the first visible part | 5.2 deg | 11.6 | 10.7 | 10.2 |
| crosshair from the corner, 500 ms before | 5.8 deg | 16.2 | 13.4 | 8.2 |
| out past the corner on the open side (+), 500 ms before | -1.8 deg | +7.6 | +7.0 | +2.0 |
| closer to the corner than to the hidden enemy | 80% | 35% | 33% | 57% |
| from the corner 1 s before (hidden 2 s) | 5.8 deg | 18.0 | 16.4 | 9.0 |
| view turn toward the appearance in the last 500 ms | 13.0 deg | 2.7 | 2.2 | 7.1 |
| first press after a clean sighting, mean | 232 ms | 301 | 274 | 253 |
| hidden yaw speed p50 / mouse still while hidden | 8.1 / 32% | 4.3 / 39% | 7.2 / 33% | 11.2 / 33% |
| other 219 statistics within 25% (median rel. error) | | 39% (0.33) | 44% (0.31) | 45% (0.30) |
| the 43 pre-aim statistics within 25% (median) | | 23% (0.43) | 30% (0.48) | 40% (0.29) |

To make such a capture: per server, a cfg with `sv_fps 20`, `g_gametype 1`, `fraglimit 0`,
`timelimit 0`, `sv_runspeed 250`, `sv_dmspeedmult 1`, `sv_gravity 800`, `sv_maxbots 2`,
`g_humanbot_seed N`, `g_movelog 1`, `g_movelog_need_human 0`, `sv_cheats 1`, `timescale 10` and
the map. Start `omohaaded` with its own `fs_homepath` and `net_port`, stop it after 240 s, then run
`compare.py <home>/main/telemetry ... --moh-dir <mohaa>`. A merge patch in
`g_humanbot_model_dir` tries a parameter without a rebuild. Run one `compare.py` at a time:
`behavior.py` got OOM-killed with two at once.

## Known gaps (two average-style bots; "real maps" = the report above)

- **Aim at a sighting:** 10.2 deg off at the first visible part vs people's 5.2, and the first press
  comes at 250 ms vs 150. The bots pre-aim corners now but pick the one the enemy comes out of
  less often: closer to the corner than to the enemy 57% vs 80%. An ad-hoc check on the tuning
  captures found them within 4 deg of the true corner at the onset 33% of the time vs 53%, and
  their error given that distance matching people's. The next lever is which exposures the belief
  offers, not the view. Downstream of it: 32% of sightings start with fire already held (people 23%),
  the first hit lands 300 ms after the first part (250), and 49% of part sightings end without a
  hit (28%).
- **Partial exposure:** the enemy shows only parts (centroid hidden) 5.1% of duel time vs 10.5%,
  and bots hit 12% of their rounds there vs 22%. Bots rarely peek with part of the body.
- **Hold or clear an angle (`hold_angle` dial):** wired in (it scales the pre-aim horizon and the
  hold hazard near exposures) but inert. Its sweep moves the share parked on the appearance point
  by under a sixth of the human range (0.22-0.53); the bots' parked share is set by the corner
  choice above. `calibrate.py` leaves any such dial at its neutral offset.
- **Close range:** firing at under 128 u, aim error is 19 deg vs 14 (real maps 20). Turn speed in
  fights is p99 620 deg/s vs 305 (real maps 710).
- **Trigger and spread (2026-09-28):** against the owner the bot hit 15% of its rounds (people 20%)
  with its crosshair on the body more often than people: MOHAA widens the SMG spread 0.3 a round
  (reset after 250 ms without one), and the bot sprayed. The arena now fires with the game's spread,
  and three calibrated loops tilt the release (`release_near_logit`, `release_far_logit`,
  `release_tap_logit`). In the engine (bot vs bot, 4 maps) the spread at firing fell from 1.26 to
  1.02 (people 1.23) and the kills a minute rose 16%. Still off: the near and tap loops pull
  against each other (near-target release 1.5x people's, taps 34% vs 45%).
- **Retreats:** bots back off in fights at 100-300 u twice as often as people (real maps 15-23%
  of fight ticks at 96-288 u vs 5-10%).
- **Seeing the enemy without firing:** people stand still 30% of that time. In the arena bots stand
  still 11%; on the real maps it swings between captures from 15% to 76% (62% in the report above),
  and that context takes 3-15% of duel time vs 7%. Some bots stand facing a visible enemy without
  firing for long stretches. Not investigated.
- **The arena is not a recorded map.** Statistics tied to map geometry (fight distances, context
  shares, the corners: crosshair from the corner 11.9 deg in the arena vs 8.2 on the maps) are
  judged on real captures only.

## Deviations from the original plan

- The plan's branch `claude/intelligent-euler-5nq2po` never existed; the work is on
  `claude/funny-cray-e3yyih`.
- Visibility cells are 32 u, not 64.
- Movement is two coupled keys (strafe and forward) instead of one chord model.
- Walls veto no fitted key; drops deeper than 240 u do. Walls shift fitted odds instead (of
  letting go, in the key's direction and along a diagonal, and of what a key changes to), and the
  hand-set wall reflex lets go of keys before the bot hits a wall and vetoes pressing into one it
  touches.
- Engage urgency is 0: bots do not push forward to engage.
- The hidden look policy, the sound precision and the pitch gain were set by hand (see the
  `calibrate.py` docstring). So was the corner pre-aim's part of it (`preaim_*`), on real-map
  captures. The pooled loops and every dial sweep except `hold_angle`'s run without corners
  (`hb_arena --no-corners`).
- The reaction skill shifts the trigger's press hazard instead of the detection rate. Both skills
  are relative to the average bot.
- AFK behavior is not modelled.
- With more than 2 bots, the reload statistics are skewed (88% of human reloads happen while the
  opponent is dead).
- The upstream Unit Tests workflow builds without the client; it could not find SDL2 on this fork.
- **`g_humanbot_skill` (the owner's difficulty, read live; 0 = as fitted).** Per unit: aim noise
  x e^-0.5, press logit +0.7, detection rate x e^0.5, and a fire gate: at each sub-step a round is
  skipped with the crosshair off the body, or started with it on, with chance 0.5 per unit (capped
  at 1). People fire on target (30% of their rounds against 20% of their frames); the bots do not.
  Bot against bot in the arena it barely shows (the boosted bot wins 54% of kills at 2, 52% at 0):
  the owner judges it in play. The owner beat 3 bots 23-4 at 0.
- **The owner's rule: no two headshots in a row.** After one of its rounds hits a head, a bot aims at
  the chest for 300 ms and holds its fire for the first 100 ms (`hb_brain.cpp`, `hb_view.cpp`).
  People do land two in a row (3.2% of their kills); the owner does not want the bots to.
