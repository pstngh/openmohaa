# Handoff: human-imitation bots

For a Claude Code session picking this work up. The state is as of 2026-09-28.

## What this is

The bots of this fork (github.com/pstngh/openmohaa, public) run one data-fitted, stochastic brain
learned from six recorded players (346 player-minutes of 1v1 SMG duels). They move, aim, fire and
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
  statistics (median relative error 0.20; four seeds of 900 s, in the build container). On
  macOS the same model scores 53%; with the wall terms of 2026-09-28, 55% (median 0.22).
- 16 bots take 112 us per bot per tick; the budget is 150.

**The engine glue runs** (first live runs, 2026-09-28, on macOS and on the VPS):
- the model loads, bots join and fight, the navmesh is valid, the map prior reads "checksum ok"
  on dm/crnodoors and the visibility table is built and cached;
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

## Known gaps (arena, two average-style bots)

- **Aim at a sighting:** 13 deg off vs people's 5, so the first shot comes at 250 ms vs 150. People
  peek into their own crosshair; the bots hear and track hidden enemies as well as people do but
  never peek. The likely next feature is corner clearing: slow before an exposure and pre-aim it.
- **Close range:** firing at under 128 u, aim error is 21 deg vs 14. Turn speed in fights is
  p99 620 deg/s vs 300.
- **Trigger:** bots keep firing with the crosshair more than 10 body half-widths off 11% of the time;
  people do it 4%.
- **Retreats:** bots back off in fights at 100-300 u twice as often as people.
- **Seeing the enemy without firing:** people stand still 31% of that time, bots 13%.
- **The arena is not a recorded map.** Statistics tied to map geometry (fight distances, context
  shares) are judged on real captures only.

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
  `calibrate.py` docstring).
- The reaction skill shifts the trigger's press hazard instead of the detection rate. Both skills
  are relative to the average bot.
- AFK behavior is not modelled.
- With more than 2 bots, the reload statistics are skewed (88% of human reloads happen while the
  opponent is dead).
- The upstream Unit Tests workflow builds without the client; it could not find SDL2 on this fork.
