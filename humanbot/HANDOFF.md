# Handoff: human-imitation bots

For a Claude Code session picking this work up. The state is as of 2026-10-05.

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
- `ctest` passes 8/8: since "Travel and the hidden view" checked with clang RelWithDebInfo on the Linux
  workstation (its GCC has no m4 for flex and bison); before it, with GCC RelWithDebInfo and clang Debug.
- CI is green: Builds on Linux, macOS and Windows, Unit Tests, and the Python checks.
- In the arena, two average-style bots are within 25% of the human value on 52% of 195
  statistics (median relative error 0.23, as before "Steering with the view" and "Back and forth with the enemy hidden"; 53% and 0.21 before "Late aim and low clips", 48% and 0.27 before "Where the
  bot looks when first seen", 50% and 0.25 before
  "The practice areas of the objective maps"; 48% before
  "Hits from an unseen enemy" and before "The lean follows the
  strafe", whose dials the average bot does not carry; 53% before "What the owner saw", where the turn to sounds behind costs
  most in the arena, 51% before "Corners and the lost enemy", 55% and 0.22 before "Travel and the hidden view", 58%
  and 0.18 before "Movement on the maps", 60% before the encounter changes); eight seeds of 900 s at 12 usercmds a
  frame, model `3040d79717e2` of 2026-10-05 on a Linux workstation. The arena is not the maps: in its open pillars
  the stronger route coupling makes the bots cover more ground than people (see "Encounters"), since
  the wall reflex slides along walls they no longer stop at pillars (see "Movement on the maps"), and
  a strafe that meets a pillar now runs on along it on the diagonal, so in fights the pooled bot
  approaches and takes the diagonal more than people (see "Travel and the hidden view"). Four seeds are not enough to compare two models: any four of the
  same eight runs range over 52-63% (see "Fire into cover, bursts and ladders"). With eight seeds
  the corner pre-aim's model (`c29371288`) and the out-of-ammunition one (`8d64ab93b`) both
  score 59%. The arena's pooled bots carry no style shift, so the out-of-ammunition fix below
  shows on the practice maps, not here.
- 16 bots take 81-90 us per bot per tick on that workstation (86 with the travel mode) (112 in the build container); the
  budget is 150. In the engine with 16 bots the brain takes 113-123 us per bot (median) and the 12 usercmds 43 us
  (see "What the owner saw").
- On the four practice maps, bot against bot (`humanbot/eval/reports/`), see "Corner pre-aim",
  "Out of ammunition", "Fire into cover, bursts and ladders", "Encounters", "Movement on the
  maps" and "Travel and the hidden view" below. Since "Movement on the maps" compare.py weights the bots' style families like the
  people's duel minutes (half presser), not like the draw (a fifth): earlier reports used the draw.
- On dm/brownffa and dm/flag, the practice areas of obj/obj_team4 and V2, bot against bot, measured like the data
  repository's report section 15: see "The practice areas of the objective maps".

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
   - A map without a recorded prior gets cells of 32 u, coarser on a big map (at most 3000 cells:
     `MAX_NAV_CELLS`, `humanbot_world.cpp`); the console says "is a big map: cells of N u".
4. **Movement.**
   - Usercmds go through `G_ClientThink` 12 times per frame (`Commit`, `g_humanbot_substeps`).
   - Look for jitter, bots standing still, or bots stuck on doors and ladders.
   - The stock code takes over for doors, and for 750 ms when a bot is stuck for 1.5 s or
     pushes into a wall for 1 s. Ladders are climbed by the adapter's own rule (`Ladder()` in
     `humanbot_adapter.cpp`). `g_humanbot_debug 1` prints every hand-off.
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
- **Two data sources, never pooled.** `pstngh/openmohaa-demos` (checked out next to the fork) may
  set the bot's choices: routes and map use, which fights to take, team and objective play,
  grenades, ladders, doors. The motor layer (key timing, aim, reaction, trigger) stays fitted on
  openmohaa-movement only. Never pool the two, and record each parameter's source in its fit. The
  demos are on the original AA maps at 1.1x physics with about 170 ms latency.
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
- **The wall reflex** (`hb_movement.cpp` `WallAhead`/`Reflex`, `wall_reflex_ms` 300 and
  `wall_reflex_logit` 4 in the model's couplings, set by hand in the engine): when the wall in the
  direction the bot goes (its held chord) is reached within 300 ms at the current speed, a key is let
  go of (the forward key only lets go, it never backs off), and no new key presses into a wall the
  bot touches. The key processes are fitted on people who steer along walls with the mouse; they
  barely react to walls, and the bots lacked the anticipation. The bot's clearance probes retry 2 u
  higher when they start in solid, and the reflex stands down when every probe is blocked (it froze
  the bots before that). Since "Movement on the maps" it slides: a blocked diagonal lets go of the
  key whose own direction is the more open, and forward into a wall adds a strafe toward the more
  open front diagonal (until then it also let go of a key whenever the forward or the sideways
  direction of a diagonal neared a wall). Since "Travel and the hidden view" a strafe into a wall
  adds forward when the front diagonal on its side is open, as people do, instead of stopping.
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
  tracking gain and full own-motion compensation (since "Movement on the maps" in proportion to how
  soon the enemy is expected out of it). The fitted still gate keeps working there.
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

## Out of ammunition (2026-10-02)

On the practice maps bots stood still facing a visible enemy for minutes (62% of the time they saw
the enemy without firing, people 30%). These were standoffs: both bots of a pair out of every round,
SMG and pistol, with nothing left to do. People never run dry (0.0% of their duel time: they die
first). The bots were dry 10% of their duel time and held a pistol 23% of it (people 1.8%). They
emptied the loadout firing at enemies they could not see (fire held with no body part visible 34%
of that time, people 17%), over lives three times as long as people's. Two calibrated shifts did it:
- **The burst-length dial** (`burst_median`, -1.47 logit at the pooled value) also lengthened the
  release of fire into cover. The arena's pooled bot carries no dial shift, so no loop saw it. It now
  acts in sight only (`hb_trigger.cpp`). Its arena sweep then spans 19% of the human range: the dial
  worked through fire into cover. `calibrate.py` leaves it inert, and every bot bursts like the
  pooled one. (It now counts the bursts begun in sight and works again: see "Fire into cover,
  bursts and ladders".)
- **`hidden_fire_logit`** was calibrated against fire held without the centroid ray. That counts
  people's fire at partly visible enemies, while the trigger's "hidden" means no body part visible.
  The loop now targets `perception.fire_held_no_part` (new in the human reference: 17.3%); the value
  went from 0.77 to 0.37.

People give no data on running dry, so the fallback is set by hand (`hb_weapon.cpp` `OutOfAmmo`,
`hb_brain.cpp`, `hb_nav.cpp`). With nothing in hand that fires and no round for the primary or the
pistol, the bot draws the pistol even when it is empty, closes in (urgency 1, no holding) and bashes:
the secondary attack, a tap at a time, within 100 u with the crosshair within 1.5 half-widths. The DM
pistol bash reaches 96 u, does 35 damage and needs no ammunition; people bash only with pistols. A
bot also goes back to the primary once it has rounds again, and draws a weapon after 1 s with nothing
in hand (never on a ladder). With a test patch that makes the bots spray until they run dry
(dm/vents), the dry bots moved 75% of the time and bashed 121 times: 11 hits, 3 kills.

Practice maps, bot vs bot, seeds 201-208 (`eval/reports/2026-10-02_outofammo_realmaps`; in brackets
the held-out seeds 301-308):

| | people | before (`b48d6868`) | now |
|---|---|---|---|
| fire held with no body part visible | 17.3% | 34% | 17.6% (17.5%) |
| ... more than 5 s after the parts were last on screen | 4.3% | 29% | 12% (12%) |
| duel time out of ammunition, a weapon in hand | 0% | 10.4% | 0.03% (0.05%) |
| holding a pistol | 1.8% | 23% | 1.1% (1.3%) |
| standoffs of 20 s or more | | 28 | 0 (0) |
| still while seeing the enemy without firing | 30% | 62% | 15% (14%) |
| accuracy | 19% | 6.4% | 8.6% (8.5%) |
| burst p50 / attack hold p50 / taps | 5 / 150 ms / 44% | 5 / 350 / 24% | 3 / 200 / 34% (3 / 200 / 36%) |
| other 219 statistics within 25% (median) | | 45% (0.30) | 52% (0.25) (52%, 0.24) |
| pre-aim statistics within 25% (median) | | 40% (0.29) | 52% (0.23) (50%, 0.25) |

## Fire into cover, bursts and ladders (2026-10-02)

Four things, scored on the practice maps (seeds 201-208, held-out 401-408) and in the arena.

**Ladders.** On dm/vents a bot could hang on the 464 u ladder for minutes. Two deadlocks did it:
- two bots on the ladder at once, one climbing up and one down: the ladder's state machine climbs
  only with room above or below, so each blocked the other until the capture ended;
- a bot that walked into the shaft from the top fell onto the head of one climbing. Every step off
  that head was a drop of more than 240 u, which the brain vetoes, and the climber could not climb
  through it: 17 minutes in one capture.

The stock code also climbed backwards (the back key, the view 52 deg down). People hold forward and
look up the ladder (pitch p50 -54 deg) or down it (+70) (obj/obj_team2: 81 climbs up, 7 down;
nobody took a ladder in the dm/vents duels). The adapter now climbs that way, toward the end nearer
the brain's goal (`Ladder()` in `humanbot_adapter.cpp`). A bot that has not moved 8 u for 1.5-3 s
turns back once and jumps off the second time (or after 30 s). Climbs take 5.2-6 s; none took over
10 s on 16 dm/vents servers (8 of them with 4 bots) nor in the captures below (before: 3 of 28 over
10 s, up to 48 s).

**Fire into cover long after sight.** The fitted hidden press reads people's aim error to the true
enemy. Long after sight people have lost track of it: 5 s on, their crosshair is a median 143 deg
from it. The bot reads its error to its own belief, which keeps tracking the enemy (21 deg; a
bot's fire into cover is also a sound the other bot turns to). So the bots kept firing. New
statistics: fire held with no body part visible, by the time since one was last on screen
(`perception.fire_held_no_part.<bin>` in perception.py, arena_metrics.h and the human reference).
- A calibrated fade of the hidden press (`hidden_late_logit` -0.87, ramping in linearly in log time
  from 0.5 s to 8 s since sight) is matched on the bins from 1 s on. The overall shift
  (`hidden_fire_logit`) is matched on the first second.
- The fade leaves out an enemy expected in the crosshair (the anticipation boost): people's late
  fire is mostly fire at an enemy about to come out. They prefire 26% of the sightings after 2-5 s
  hidden while holding fire 7.7% of that time. The boost (`anticipation_logit`) was set by hand to 5
  on the practice maps (2 before). In the arena a loop on it went to its bound 5 and moved the
  prefire from 11% to 13% only; on the maps 5 took the prefire after 2-5 s hidden from 10% to 17%.

**Burst length.** The median burst of 3 rounds (people 5) was not the trigger in sight: bursts begun
in sight were 5 rounds (people 6), bursts begun in cover 3 (people 3). The bots' bursts were mostly
fire into cover (26% of them begun in sight, people 61%), because the bots meet the enemy half as
often (18 sightings a minute against 33; see "Known gaps").
- The burst dial now measures what it shifts: the bursts begun in sight (`perception.burst_in_sight`;
  fit_styles.py, `metrics.dials`, the arena's `dial.burst_median`). People's values are 4-8.
- The dial is relative to the pooled bot, like the skills (`RELATIVE_DIALS` in calibrate.py). Such a
  burst ends where sight ends, and the arena's sightings are short, so no offset takes the pooled
  bot past 5.5 rounds there. Its sweep spans half the human range (19% before). On the practice maps
  the realised burst of the 16 bots now follows the drawn one: correlation 0.70, and 0.79 on the
  held-out seeds. Before, every bot burst about 5 rounds whatever it drew.
- The tap tilt is matched on the holds begun in sight with rounds in the clip
  (`perception.tap_share.in_sight`, people 15%). On all holds it made up for the bots' fewer holds
  into cover with too many taps in sight (19%), and the near-target release loop sat on its bound
  against it. They no longer pull against each other: `release_tap_logit` 3.0 -> 0.29,
  `release_near_logit` -2.94 -> -1.48.

**The arena check (58% -> 55% with 6c4fe13b) was seed noise.** Arena builds of `b48d6868` and
`6c4fe13b` reproduce the saved runs exactly. With seeds 5-8 added, both score 59% of 195 (any four
of the eight seeds: 56-61% and 52-63%); the median relative error rose from 0.17 to 0.19. Of the
19 statistics that crossed the 25% line, 14 moved by less than 3 standard errors of the seed spread.
The other five all come from the lower hidden shift: lost were fire held with the centroid hidden
(`trigger.hold_hidden` 20% -> 16%, people 24.5%), the late hidden press on the centroid ray and the
share of time hidden without fire; gained the stillness and the hidden press at 0.5-1 s. Bots fire at an enemy showing only parts as often as people (64% of that time
vs 61%) but see one a third as often (4% of duel time vs 12%): partial exposure, not the trigger.

Practice maps, bot vs bot (`eval/reports/2026-10-02_trigger_ladder_realmaps`). "Before" is
`6c4fe13b` on seeds 201-208 (held-out 301-308 in brackets), "now" this model on 201-208 (held-out
401-408):

| | people | before | now |
|---|---|---|---|
| fire held with no part visible, 0-0.5 s after a part was on screen | 43% | 47% (46%) | 48% (48%) |
| ... 0.5-1 s | 26% | 31% (29%) | 29% (29%) |
| ... 1-2 s | 20% | 27% (27%) | 21% (22%) |
| ... 2-5 s | 7.7% | 16% (15%) | 9.2% (9.1%) |
| ... more than 5 s | 4.3% | 12% (12%) | 5.7% (6.4%) |
| ... all | 17.3% | 17.6% (17.4%) | 11.3% (11.9%) |
| fire held before the first visible part | 23% | 20% (20%) | 16% (18%) |
| burst p50 / p90 | 5 / 12 | 3 / 10 (3 / 10) | 4 / 11 (3 / 10) |
| ... bursts begun in sight | 6 / 13 | 5 / 15 (5 / 16) | 5 / 14 (5 / 14) |
| taps of the holds begun in sight | 15% | 19% (21%) | 14% (17%) |
| burst dial, realised vs drawn: correlation over 16 bots | | 0.21 (0.18) | 0.70 (0.79) |
| accuracy | 19% | 8.6% (8.5%) | 9.9% (9.9%) |
| ladder climbs over 10 s / longest climb | | 3 / 12 s (0 / 9 s) | 0 / 5 s (0 / 6 s) |
| other 219 statistics within 25% (median) | | 52% (0.25) (52%, 0.24) | 48% (0.27) (50%, 0.25) |
| pre-aim statistics within 25% (median) | | 52% (0.23) (50%, 0.25) | 48% (0.26) (41%, 0.27) |

The overall fire into cover is now below people's, though each bin matches: the bots spend half of
their time with no part visible more than 5 s after sight (people 15%), where people hardly fire.
The same time mix moves the shares within 25% by 2-4 points: less time firing into cover, more time
hidden and still. The per-tick release with the centroid visible is now 1.5x people's, but that ray
also counts an enemy behind the bot; on the trigger's own sight (a body part on screen) the release
per tick is 2.2% against people's 2.5% (before 1.6%), and it follows people's by hold age.

## Encounters (2026-10-02)

On the practice maps the bots saw each other (a body part on screen) 15-16 times a minute of duel time
against people's 33, and had the enemy on screen 15% of that time against 34%. Diagnosed on the
captures of `9ece1afc` (seeds 201-208 and 401-408) against the people, with git-ignored scripts that
also drew a top-down picture per map of where people and bots spend time and meet (BSP brush sections
at a height, pooled occupancy, sighting onsets).

**What it was.**
- **Not distance: walls.** Hidden, the bots were *nearer* each other than people (median 430 u vs
  509), but with a clear line between their bodies 13.5% of duel time against 25.5%.
- **Both phases.** From both alive to the first part on screen: median 3.4-3.7 s vs 1.35 s. After
  that the bots were hidden 72% of the time vs 42%, and a hide was longer than 5 s in 9.4% of
  cases vs 0.8% (p90 4.7-5.0 s vs 1.75 s).
- **They did not travel.** With no enemy part on screen they got a median 64-70 u further in 2 s
  (people 158), stood still for more than 1 s 2.75 times a minute (people 0.67) and held the
  forward key 18% of that time (people 41%). In the open arena they cover people's distance: on the
  maps the walls stop them. The keys followed the route only weakly (`nav_*_logit` 1.0 / 1.5, set by
  hand): of their moving hunting ticks 41% went within 45 deg of the route. And their view was on
  the believed enemy through a wall (in that look mode the crosshair met a wall 75 u away; people's
  median 287 u), so the forward key ran into walls, the wall reflex and the fitted wall terms let go,
  and they strafed along corridors (velocity 89 deg off the view; people 41).
- **No chase.** At the end of a sighting people close on each other at 48 u/s, the bots at 7: the
  engage urgency was 0, and a hide of more than 3 s followed 14% of the bots' sightings (people 3%).
- **After a kill** they stood 1.5 s (`POST_KILL_MS`) and then walked (124-136 u/s); people run from
  0.5 s on (236 u/s, forward held 85% of the time).
- **A belief bug.** The perceiver left a dead enemy out of the observation, so the belief dropped its
  track as gone and seeded the respawned enemy from the occupancy prior, anywhere on the map. The
  spawn rule in `hb_belief` (`SeedFromSpawns`) never ran in a game. 0.5-1 s after the enemy
  respawned, the survivor's view was 50 deg off it (people 22).
- **Peeking follows.** The part-only time (4% vs 10.5%) was about people's share of the time on
  screen: few sightings, not a lack of peeking. Over time with no part on screen the occupancy
  pictures show bots lingering near spawns and in pockets (dm/vents' lower centre, the dm/downladder
  junction, the middle of dm/main) where people use the corridors and central rooms. People spend
  14% of their dm/main time in its lower room (it has a spawn point); the bots almost never.
- **The belief long after sight.** The window "2-4 s since seen" of `belief.*` (metrics.py counts
  within a life) is 75% time after the enemy respawned for people; as the bots kill more, it fills the
  same way, and that statistic follows the respawn behaviour more than the hidden view.

**What changed.**
- Evaluation: `perception.encounter.*` (perception.py `encounters()`, `arena_metrics.h`, the human
  reference; no existing value changed): sightings per minute, part on screen, the hidden spell
  between two sightings (p50/p75/p90), the time from both alive to the first part (p50/p75; a
  sequence that ends unseen counts as never), and the net distance covered in 2 s from a tick with no
  part on screen (p25/p50).
- `hb_perception_model`: a dead enemy stays listed, unseen. The belief draws it back at the spawn the
  game would pick (FFA rule, from where the bot is) at people's median respawn delay, 2.45 s
  (`RESPAWN_SEED_MS`; it was 1.6 s, the earliest respawn, and a bot that moves by then gets it wrong),
  and the view takes a new look decision then (`respawn_relook`).
- Route following (`nav_switch_logit` 3.0, `nav_choice_logit` 4.5): set on the maps. Encounters stop
  growing at that strength (in the sweep, 4.0 gave 26.9 sightings a minute against 26.6), and more makes the bots walk with
  the enemy out of view more often.
- `engage_urgency` 0.65, for the second after losing sight only: in the first second after the enemy
  left the screen the bots close in at 31-34 u/s (people 31; before 8). While the enemy is perceived
  there is no pull: with it in sight too, every bot pressed forward like the presser family (the
  arena sweep of the fight forward-diagonal dial spanned 0.44-0.67, people's styles 0.07-0.58), and
  it moved the fights' statistics away from the style dials.
- `post_kill_ms` 500 (was 1500): a bot moves on half a second after a kill.
- The view: the route look follows the route direction smoothed over about 0.3 s (the next path
  corner's direction swings round as a corner is passed) and re-aims once it turned 30 deg
  (`travel_follow_deg`; 15 made the route look turn at a median 87 deg/s); on the move a route more
  than 100 deg off the view is turned to (`route_turn_hazard` 0.2 per tick): with the keys following
  the route the bots walked backwards 10% of their hidden time (people 6%; with it 7-8%).
- `calibrate.py --stage pooled` (10 iterations) and `--stage dials` were rerun.

Tried and dropped (single captures, seeds 201-208): the wall reflex off (17 sightings a minute, but
walls touched 33% of the time vs 9%), no holding (20), looking down the route instead of at a
believed position behind a wall (the view turned away from the enemy, encounters unchanged), keys
that treat any direction within 45 deg of the route as aligned (more pure strafing on the maps), and
leaving the side key out of the post-sight pull (fewer re-peeks: 24.5 a minute).

Practice maps, bot vs bot (`eval/reports/2026-10-02_encounters_realmaps`): "before" is `9ece1afc`
re-scored with the new statistics, "now" this model; seeds 201-208, in brackets the held-out 401-408:

| | people | before | now |
|---|---|---|---|
| sightings (a part coming on screen) per minute of duel time | 33.3 | 15.1 (16.1) | 27.8 (28.4) |
| duel time with a part on screen | 34% | 15% (16%) | 33% (33%) |
| ... only parts (the centroid hidden) | 10.5% | 3.8% (4.0%) | 6.3% (6.1%) |
| hidden spell between sightings p50 / p90 | 400 / 1750 ms | 500 / 5000 (550 / 4800) | 350 / 1800 (350 / 1700) |
| both alive to the first part p50 / p75 | 1.35 / 1.9 s | 3.7 / 8.7 (3.4 / 7.7) | 1.9 / 3.45 (1.85 / 3.35) |
| net distance in 2 s with no part on screen p50 | 158 u | 64 (70) | 103 (101) |
| closing speed in the first second after losing sight | 31 u/s | 8 (6) | 34 (31) |
| speed 0.5-1.5 s after a kill p50 | 236 u/s | 124 (124) | 144 (144) |
| view to the respawned enemy, 0.5-1 s after its respawn | 22 deg | 50 (50) | 38 (36) |
| life length p50 | 7.1 s | 23.5 (23.1) | 12.4 (12.4) |
| accuracy | 19% | 9.9% (9.9%) | 15.3% (14.7%) |
| fire held with no part visible (all of that time) | 17.3% | 11.3% (11.9%) | 15.2% (15.7%) |
| encounter statistics within 25% (of 9) | | 11% (0%) | 67% (67%) |
| other 219 statistics within 25% (median) | | 48% (0.27) (50%, 0.25) | 50% (0.25) (49%, 0.27) |
| pre-aim statistics within 25% (of 44, median) | | 48% (0.26) (41%, 0.27) | 52% (0.23) (55%, 0.23) |

Statistics that crossed the 25% line (201-208). Gained: the firefight share of duel time (7% -> 18%,
people 17.6%) and the hidden share, accuracy, decisive fights ending in a kill, fire held with the
centroid hidden and with no part visible, prefire before the first part, stillness, walking, reloads
with the enemy dead, the speed before a sighting. Lost: the hidden view turns faster (yaw speed p50
10 -> 20 deg/s, people 8: it moves more and keeps more of its own motion out of the aim), the forward
key alone (hidden 8% -> 19%, people 12%: people travel on the forward diagonal, 30%, which the walls
deny the bots, 13%), decisive fights last longer (p50 1.4 -> 1.95 s, people 1.25), strafes in a
firefight are shorter (p25 150 -> 100 ms, people 200), and the view 4-8 s after sight is further from
the enemy (within 30 deg 62% -> 51%, people 76%). Think time with 16 bots in the arena: 78-79 us per
bot per tick (85-86 before); no bot was stuck for more than 1.7 s on the maps.

## Movement on the maps (2026-10-02)

In the open arena the bots moved like people; on the practice maps they did not. With the enemy hidden
they held the forward key alone 19% of the time (people 12%) or strafed where people run on the forward
diagonal (13% vs 30%), and covered 103 u in 2 s (158). In firefights at 160-512 u they approached
18-22% of the time (people 38%) and backed off 14% (5%). Their hidden view turned at a median 20 deg/s
(8). Diagnosed on the captures of `9d93d470` (seeds 201-208, 401-408) against the people, per tick and
at the same clearances, with git-ignored scripts (`humanbot/cache/move_wip/`: `mload.py` loads both
sides with `metrics.sequences()`; `mtab.py`, `msumm.py`, `dialrec.py` tabulate; `d*.py` are the
diagnosis tables; `famw.py` weights by family).

**What it was.**
- **Scoring.** compare.py reweighted the bots to the families' draw weights (0.2 / 0.4 / 0.4, each person
  alike) while the human reference pools minutes, half of them the one presser's (198 of 397). Per
  family the strafer and stopper bots approached like people of their family (13.6% and 15.5% vs 15.6%
  and 15.1%); weighted by family, people approach 25%, not 38%. styles.json now carries each family's
  `minutes_share` (fit_styles.py) and compare.py reweights the bots to it.
- **The fight-diagonal dial** (`diagLogit`) tilted the forward key's choice: from none while strafing a
  low-diagonal style (strafers and stoppers sat at the curve's -3 bound) gave "forward" e^-1.35 of its
  weight and "back" took the rest (back 57-62% of such presses, people of those families 22-34%). In the
  arena the pooled bot with that offset held the back key 10.8% of fight time (4.8% without) and backed
  off at 288-512 u 13-14% of it (people of those families 8%). The dial is now a rate (competing risks):
  it scales how readily a strafe gains the forward key (from none or from back), and back keeps its
  fitted rate. Recalibrated, its sweep spans 0.07-0.73 without saturating (before 0.07-0.58 at +-3).
- **The wall reflex** checked the component directions of a diagonal: a key was let go of when the
  forward direction alone, the sideways one alone or the diagonal reached a wall within 300 ms. Down a
  corridor on the diagonal (people's way: their velocity is a median 40 deg off the view) both
  components face a wall. Under the same condition people keep their keys on 70-82% of ticks and slide
  (forward -> diagonal 217 per 1000 ticks, -> none 32); the bots kept a diagonal or strafe 26-44% and
  stopped (forward -> none 160). With the enemy hidden the bots stood next to a wall (< 16 u, < 50 u/s)
  15% of the time (people 7%) and touched one 14% (9%). Bots and people let go of the forward key alike
  while the way ahead is open (56 vs 60 per 1000 ticks on the diagonal).
- **The hidden view.** The still gate ignored own motion: people standing (< 1 u/s) rest the mouse on
  75% of hidden ticks, moving on 19-31%; the bots 40% and 23-29%. The corner pre-aim held near corners
  (median 170 u) with the tracking gain and full own-motion compensation while running past them (the
  corner's direction sweeps at a median 22 deg/s): smooth turns of about 30 deg in 4 ticks, 35 a hidden
  minute. With route-look flicks (15 a minute) and re-aims the bots made 89-100 turns of 10 deg or more
  per hidden minute, people 46. The arena's hidden-noise loop sat at its bound with the arena view at
  12.5 deg/s.
- **Travel.** The bots' forward-key runs were short (the run around a hidden tick: p50 0.9 s vs 1.45 s),
  their paths less straight (net / path in 2 s 0.47 vs 0.56) and shorter (269 u vs 338). With the way
  ahead open they let go no more often than people; the excess is at walls (the reflex). In the spawn
  push (route urgency 1) they covered 163 u in 2 s, people 176 on the same definition.
- **Not the veto** (people press a key into a wall they touch 4.0 times per 1000 hidden ticks, the bots
  2.9) **nor the fitted wall terms** (fitted on people; with the way open the bots match).
- **Back-offs at close range** after the fixes are a tracking matter: at those ticks the bots' crosshair
  is a median 11.6 deg off the enemy (people 3.7) and 48% of them are plain strafes (people 21%); the back
  key in fights is held as often as people hold it (5.4% vs 5.2%).

**What changed.**
- `hb_movement` `Reflex`: only the held chord's direction counts; a blocked diagonal lets go of the key
  whose own direction has the more room (both when neither has), and forward into a wall adds a strafe
  toward the more open front diagonal (lets go when none is open).
- `hb_movement` `StepFwd`: the diagonal dial as a rate (see above).
- `fit_view.py`, `hb_view`: the still gate is fitted and applied separately for a standing bot (< 1 u/s,
  `view.still.enter_standing` / `stay_standing`).
- `hb_view`: a pre-aimed corner gets the tracking gain and own-motion compensation in proportion to its
  imminence, exp(-eta / pre-aim horizon), else the idle controller's (people's fitted `Kself` 0.41).
  `travel_follow_deg` 30 -> 60, `hidden_reaim_hazard` 0.15 -> 0.05 (hand-set, on the maps).
- compare.py: the bots' family mix is matched to the people's minutes (`minutes_share`).
- `calibrate.py --stage pooled` (10 iterations) and `--stage dials` were rerun. In the arena the hidden
  view now matches (yaw speed p50 7.4 vs 8.1 deg/s, mouse still 33% vs 32%).

Tried and dropped (seeds 201-208, single captures): a weaker reflex (logit 2: walls touched 14%, travel
112 u vs 117; logit 1 with the old reflex: 20%; logit 2 and a 150 ms horizon: 17%), hunt urgency 1.0
(+3-7 u of travel, about the run-to-run spread, and less standing), re-aims at 20 deg and a 3 s look
dwell (turns 100 ->
94-95 a minute), people's own-motion share on every corner with the idle gain (view 19.7 deg/s, but the
crosshair 1 s before a sighting 21 deg from the corner vs 14), and the dial scaled per context by how
much people's style shows there (fit on the families: 0.40 hidden, 0.83 hidden firing, 0.55 in sight not
firing, 0.20 reloading; the hidden diagonal rose a point, but the dial's sweep no longer reached the
lowest styles and the corner pre-aim lost 2 deg on both seed sets).

Practice maps, bot vs bot (`eval/reports/2026-10-02_movement_realmaps`): "before" is `9d93d470` re-scored
with the people's family mix, "now" this model; seeds 201-208, in brackets the held-out 401-408:

| | people | before | now |
|---|---|---|---|
| hidden, not firing: forward diagonal / forward alone | 29.9% / 11.9% | 16.0% / 17.1% (16.6% / 17.5%) | 23.4% / 12.9% (27.0% / 12.9%) |
| duel time touching a wall | 9.1% | 13.8% (12.2%) | 9.3% (8.3%) |
| hidden time standing next to a wall | 6.7% | 15.0% (14.1%) | 10.0% (9.6%) |
| net distance in 2 s with no part on screen p50 | 158 u | 104 (111) | 116 (128) |
| sightings per minute / part on screen | 33.3 / 34% | 29.7 / 35% (29.0 / 35%) | 31.1 / 38% (28.9 / 35%) |
| firefight 224-288 u: approach / back off | 38% / 5.3% | 22% / 12.7% (30% / 12.0%) | 24% / 10.4% (32% / 10.8%) |
| firefight 288-384 u: approach / back off | 39% / 5.9% | 23% / 8.8% (31% / 9.8%) | 26% / 7.9% (32% / 7.7%) |
| hidden yaw speed p50 / p99 | 8.1 / 544 | 20.2 / 851 (21.6 / 830) | 16.3 / 766 (17.0 / 725) |
| mouse still hidden / standing | 32% / 74% | 27% / 40% (27% / 41%) | 30% / 60% (30% / 61%) |
| view turns of 10 deg or more per hidden minute | 46 | 88 (92) | 71 (73) |
| diagonal dial: drawn vs realised correlation (16 bots) | | 0.77 (0.96) | 0.87 (0.98) |
| other 219 statistics within 25% (median) | | 51% (0.25) (57%, 0.22) | 58% (0.20) (58%, 0.20) |
| pre-aim statistics within 25% (of 44) | | 55% (0.23) (55%, 0.22) | 57% (0.23) (48%, 0.26) |

With the draw's weights (a fifth presser, as the numbers at the top of this section were measured) the diagonal
is 13% -> 19%, forward alone 19% -> 13%, back-offs at 160-512 u 13% -> 10.5% (people weighted alike 25%,
12%, 7%). On the held-out seeds 13 statistics crossed the 25% line each way, the losses small (|z| < 2.2):
reload-time keys (forward 21% -> 17%, people 26%), standing hidden (17.6% -> 15.2%, people 22.5%) and the
part lead p50 (one tick). Wall-pressure bouts of 500 ms or more fell from 0.34-0.38 to 0.15-0.16 per
bot-minute; no bot was stuck for 2 s. Think time with 16 bots in the arena: 82-85 us per bot per tick
(76-79 before).

## Travel and the hidden view (2026-10-03)

After "Movement on the maps" the bots, with the enemy hidden, covered 116-128 u in 2 s (people 158) and turned
their view at a median 16-17 deg/s (people 8) with 71-73 turns of 10 deg or more a hidden minute (people 46).
Diagnosed on the captures of `d014bcd2` (seeds 201-208, 401-408) against the people, per tick and with a
diagnostic build that logged why the view changed its target (git-ignored scripts in
`humanbot/cache/move_wip/s2/`: `sum.py` is the quick table, `t*.py` travel, `v*.py` view, `mkbin.sh` and
`diag_apply.py` the diagnostic build, `cmp.sh` compare.py on quick caches; the experiments' diff is
`s2/experimental.diff`).

**What it was.**
- **Not the walls' distance, the reaction to them.** Bots and people travel the same distance from walls
  (clearance distributions while moving match) and meet a wall in the direction they go equally often (19% of
  key-held hidden ticks). People then change keys at 25% a tick (in the open 15.5%); the bots at 64% (in the
  open 12.5%): 70 extra key changes a hidden minute, all at walls. Sliding along a wall costs little in Pmove
  (about 90% of the free speed at 10 deg, 77% at 20 deg, under half at 45 deg), and people let it slide.
- **Where the changes went.** A diagonal meeting a wall became a strafe in 44% of the bots' changes (people
  27%), and a strafe meeting a wall stopped in 49% (people 28%); people add the forward key to such a strafe in
  22% of their changes (the bots 3%). So the bots strafed (32% of hidden time vs 26%) where people run on the
  forward diagonal (30% vs 23%), and the key changes, not the view, made their paths crooked (with the view
  frozen their 2 s paths were 0.45 straight, people's 0.53).
- **The hidden view's turns, by cause.** Of the bots' 71 turns of 10 deg or more a hidden minute, holding a
  pre-aimed corner as the bot ran past it gave 17 (a third of the corner time was on corners within 120 u,
  the view turning at 33 deg/s there), the turn to a route behind the view 8 (117 deg at the median, from 11
  such decisions a minute), corners coming up 6, re-aims 5-7, the rest look decisions, look-arounds and the
  believed position moving. By size the excess was at 90 deg or more (18 a minute vs 5) and at 10-30 deg (30
  vs 21); turns of 30-90 deg matched.
- **The believed position flickered.** The belief's mode jumped more than 200 u 39 times a hidden minute,
  42% of those back within 2 s; it drives the goal, the chase and the belief look.
- **Not the route.** The route direction jumps (22 times a minute by more than 60 deg, mostly next to the
  navmesh corners at low speed), but its jumps add few key changes (0.007 of 0.21 a tick), and the route
  itself leads into a wall no more often than people meet one.

**What changed.**
- `hb_movement` `Reflex`: a strafe into a wall adds forward when the front diagonal on its side is open (it
  runs along the wall on the diagonal), else lets go as before. Unit test in `test_hb_modules`.
- `hb_view`: on the move, a corner whose direction the bot's own motion sweeps faster than
  `preaim_pass_dps` (90 deg/s) is passed: a watched one becomes a held direction, and such corners are not
  picked. `route_turn_hazard` 0.2 -> 0.05.
- `hb_belief`: the mode stays in its cell neighbourhood while that holds at least half the densest one's
  mass (`MODE_KEEP`); its jumps over 200 u fell to 24-27 a hidden minute.
- `calibrate.py`: the hidden view's noise is no longer looped. It was matched on the median hidden yaw speed
  of the arena without corners, which the look turns set more than the noise (+45% noise for +0.9 deg/s); with
  fewer route turns that view calmed and the loop raised the noise 0.125 -> 0.18, which on the maps raised
  the hidden yaw speed from 11-11.6 to 12-12.8 deg/s and moved the crosshair further from corners. It stays
  at 0.125. `calibrate.py --stage pooled` (10 iterations) and `--stage dials` were rerun.
- `test_hb_modules`: the low-diagonal style's back key may be 1.6x the pooled bot's (was 1.3x; 1.5x with the
  recalibrated forward habit, the bug it guards against was 2.25x).

Tried and dropped (seeds 201-208, single captures): a reflex that leaves glancing walls (under 30 deg) alone
with a mouse steer away from them at 30 deg/s, as people turn off such walls 4-5 deg per 300 ms (walls touched
12% vs 10%, no travel gain); a 200 ms reflex horizon (touched 12%, slower); a weaker reflex logit 3 (touched
12%); a diagonal at a wall going to the most open of forward, the strafe and the other diagonal (travel 139 u,
but strafe reversals 66% of strafe ends vs 46%) or to one of them by people's shares (travel 120 u); look
targets more than 100 deg off the route weighted down (no change); corners passed by distance (128 or 176 u)
or with a sweep of 45 or 60 deg/s (the view at people's speed, but the turn toward the enemy before a
sighting halved); following a passing corner with the idle controller's share of the own motion (the view as
lively as before).

**The cost.** The corner release takes the view's turn toward the enemy in the last 500 ms before a
sighting from 9-10 to 7-7.5 deg (people 13) and the share of sightings with the crosshair closer to the
corner than to the enemy from 58% to 53% (people 80%); without it, the hidden view turns as much as before
(16-19 deg/s: the bots move more now). The error at the first visible part is 11.5-12 deg (10.3-11.2). The
share of the 44 pre-aim statistics within 25% swings 9 points between two captures of the same model and
seeds (`c1_201` 57%, its re-run 48%); here 45% and 39%.

Practice maps, bot vs bot (`eval/reports/2026-10-03_travel_view_realmaps`): "before" is `d014bcd2`
(`move_wip/reports/c1_*`), "now" this model; seeds 201-208, in brackets the held-out 401-408:

| | people | before | now |
|---|---|---|---|
| net distance in 2 s with no part on screen p50 | 158 u | 116 (128) | 137 (147) |
| 2 s path p50 / straightness (net / path) | 338 u / 0.56 | 275 / 0.49 (291 / 0.49) | 297 / 0.51 (309 / 0.53) |
| hidden, not firing: forward diagonal / forward alone / strafe | 30% / 12% / 26% | 23% / 13% / 32% (27% / 13% / 32%) | 28% / 14% / 30% (30% / 14% / 29%) |
| ... strafer / stopper bots' diagonal (people of those families) | 24% / 18.5% | 18% / 16% (15% / 15%) | 23% / 21% (21% / 19%) |
| hidden standing / neutral | 22.5% / 26% | 16% / 25% (15% / 23%) | 15% / 22% (14% / 20%) |
| duel time touching a wall | 9.1% | 9.3% (8.3%) | 9.4% (7.9%) |
| sightings a minute / duel time with a part on screen | 33.3 / 34% | 31.1 / 38% (28.9 / 35%) | 32.1 / 40% (29.7 / 38%) |
| firefight at 224-288 u: approach / back off | 38% / 5.3% | 24% / 10.4% (32% / 10.8%) | 37% / 9.9% (46% / 9.4%) |
| firefight forward diagonal | 31% | 21% (29%) | 32% (40%) |
| hidden yaw speed p50 / p99 | 8.1 / 544 deg/s | 16.3 / 766 (17.0 / 725) | 11.5 / 677 (11.8 / 666) |
| hidden turns of 10 deg or more a minute (of 90 deg or more) | 46 (4.6) | 71 (18) (73 (17)) | 54 (11) (55 (10)) |
| mouse still hidden | 32% | 30% (30%) | 31% (31%) |
| crosshair from the corner 500 ms before the first part | 5.8 deg | 10.6 (10.7) | 11.0 (11.7) |
| view turn toward the enemy, last 500 ms before the first part | 13 deg | 10.1 (9.2) | 6.9 (7.6) |
| other 219 statistics within 25% (median) | | 58% (0.20) (58%, 0.20) | 58% (0.19) (54%, 0.22) |
| all 281 statistics within 25% | | 59% (57%) | 57% (53%) |

The bots still change keys at walls as often as before (64% a tick): what changed is what they change to.
No bot was stuck for 1.5 s on the maps (two or three times before), wall-pressure bouts of 500 ms or more fell
from 0.12-0.14 to 0.09-0.10 per bot-minute, and the style dials are recovered as before (fight diagonal:
drawn vs realised correlation 0.87 (0.95), reverse share 0.99). Think time with 16 bots in the arena: 81-84
us per bot per tick.

In the arena (eight seeds of 900 s) 51% of 195 statistics are within 25% (median relative error 0.24), 55%
before. Around its open pillars a strafe now runs on along a pillar: in fights the pooled bot holds the
diagonal 42% of the time (37% before, people 31%) and approaches at 96-160 u 54% (41%, people 36%); on the
recorded maps the same change brought fights to people's (above). The arena's hidden yaw speed p50 is 10.7
(10.9) with corners.

## Corners and the lost enemy (2026-10-03)

A diagnostic build logged every exposure and corner the belief offered, per tick (git-ignored scripts in
`humanbot/cache/move_wip/s3/`). 500 ms before a sighting the belief's heaviest offered corner was within 5 deg of
the corner the enemy came out of in 57% of the sightings, the view in 20%: the view, more than the belief, kept the
bots off the right corner. Where it was instead: following the lost enemy through the wall (16% of sightings), the
believed position through the wall (13%), holding (13%), a corner no longer offered (21%).

**What changed** (`hb_view`, `hb_brain`; three view tuning switches, all 1 in `assemble_model.py`):
- `lost_aim_last_seen`: for the 350 ms after losing sight the view aims at where the enemy was last seen, not at
  the believed position running on with its last velocity. People hold where it vanished (their view 4-5 deg from
  that spot over the next 400 ms while the enemy moved on to 10 deg); the bots followed it through the wall.
- `preaim_follow_geom`: a watched corner is followed by where it is, whichever exposure offers it (the exposure
  cells change as the cloud and the bot move).
- `belief_look_corner`: a belief look watches the corner nearest the believed position's direction, when the belief
  offers one, instead of the believed position through the wall. Unit test `TestViewCorners`.

Practice maps (seeds 201-208 and 401-408, pooled; `move_wip/reports/vc_*` against `v2_*`): the crosshair ends
closer to the corner than to the enemy at 58% of the sightings (53%, people 80%) and closer to where the enemy
appears 55% (48%, people 67%); the yaw from the corner, travel and the hidden view did not move. The costs: the view
within 30 deg of the enemy in the first 500 ms after losing it 84-87% (86-89%, people 92-93%), the first hit 400 ms
after the first part (350). Arena, eight seeds: 53% of 195 statistics within 25% (51%).

Tried and dropped: exposures from rolling the particles forward with the move kernel (64 rollouts: the right
corner offered as often but less often the heaviest; closer to the corner 52%), a 2000 ms pre-aim horizon (no
change). The belief's arrival times run long (the right corner's at -500 ms has a median 872 ms against the true
500). 68% of the bots' sightings (people 86%) need both players' movement.

## What the owner saw (2026-10-03)

The owner watched bot against bot on the VPS as a spectator (12:13-12:45 UTC) and saw five things. Each was checked
in that recording (`~/moh-humanbot-home/main/telemetry/segments/1791029609_244530_*`; the screenshots' times match the
ticks within 0.3 s by the followed bot's health) and against the recorded people (aggregates, people weighted by
family like compare.py). Scripts in the git-ignored `humanbot/cache/move_wip/s4/` (`obs5.py` measures all five per
capture; `lean_wall*.py`, `crouch.py`, `behind.py`, `still.py`, `reloads.py`, `strafe_lean.py`).

1. **Leaning into the wall at doorways.** bot3 leaned right with a wall 15-50 u to its right (shot0004). With one
   side walled and the other open, people's leans in sight go into a *flat* wall (the wall goes on ahead on that side)
   35% of the time and around an *edge* just ahead (the front diagonal open) 77%; the bots 46% and 58%, every family.
   The lean chain followed the strafe key only. Now `fit_movement.fit_lean_wall` fits a wall and an edge logit on the
   side a lean takes (chain as offset: -0.41, +0.51) and `Mover::StepLean` shifts the side (not whether there is a
   lean) by them; three times the wall term and an edge term of 3.74 were set on captures (`assemble_model.py`).
   Edges are passed quickly (median 100 ms): in edge moments of a quarter second or more the bots now lean around
   the edge like people (79% vs 76%); the rest of the gap is that people lean toward the opening before they get
   there (66% of edge moments start leaned that way, the bots' 41%).
2. **Strafing without leaning.** It is the strafer style: both recorded strafer-family players lean on 48% and 64%
   of their strafing while firing (the presser, the owner, 94%; the stoppers 84-93%). The bots match by family
   (strafers 63-72%). No change.
3. **Crouching for no reason.** People crouch three times as readily in the half second after the enemy fired
   (hidden 1.3 -> 4.2 a minute, firing back in sight 1.7 -> 6.0) and hardly otherwise; the bots crouched at one rate.
   The crouch's press hazard is now fitted with and without enemy gunfire heard in the last 500 ms
   (`press_hazard_fire`; the brain marks a heard `SOUND_GUNFIRE`).
4. **Not turning to footsteps behind.** With an unseen enemy running or firing behind them within 1000 u, people face
   it within a second 62% of the time and are hit first 7%; the bots 39% and 19%. The view now turns to an enemy heard
   more than 90 deg off it (footsteps, gunfire) 150 ms after the sound (`sound_turn_*`, VIEW_SOUND), onto the corner
   nearest the sound when the belief offers one within 45 deg, holds that way 3 s against route turns, look-arounds
   and corners (a new sound behind re-aims), and the hunt goal is picked again. People who turn to a noise behind see
   the enemy within 2 s 90% of the time and turn away again 2%; held only 0.9 s the bots turned away again 29% of the
   time and the hidden view turned at 16.8 deg/s at the median.
5. **Standing still mid-fight.** Overall the bots stand still in fights less than people (still bouts over 1 s 0.3
   a fight-minute vs 0.6). Their long ones are a different kind: a third of them come during a reload in the
   enemy's view (people's 5%), as at 12:31:50 when bot3 emptied its clip at bot2 and stood 2 s reloading while hit
   from 79 to 14. The bots begin reloads with the enemy on screen five times as often (1.4-1.9 a minute alive vs
   0.3), nearly always dry, because their fights in view last longer (sightings over 2 s 7.5% vs 2.1%). Changes,
   all fitted: the trigger's press hazard by rounds left (`trigger.press_*.clip`: with an eighth of the clip people
   press 2.5 times less readily in sight); the early reload with the enemy out of sight by rounds left and time since
   sight (`weapon.tactical`, was 0.004 a tick below half a clip); and the stop between strafes: after letting go of a
   strafe people press the other side 95-98% of the time a tick later (`keys.side.opposite_p` by the ticks since the
   strafe ended; the bots chose a side at random), and how readily a strafe follows at once is a habit, the new style
   dial `counter_strafe` (people 0.15-0.66; the stoppers turn this way more than they reverse directly).

Practice maps, bot vs bot (`move_wip/reports/fc_*` against `vc_*`, the model of "Corners and the lost enemy"); seeds
201-208, in brackets 401-408:

| | people | before | now |
|---|---|---|---|
| leans in sight at a flat wall: into it | 35% | 46% (43%) | 35% (34%) |
| ... at an edge just ahead: around it | 77% | 58% (60%) | 63% (63%) |
| crouches a minute, hidden: quiet / after enemy fire | 2.3 / 5.4 | 3.6 / 4.6 (3.6 / 4.3) | 2.9 / 5.7 (2.6 / 5.6) |
| enemy heard behind: faced within 1 s / hit first | 62% / 7% | 39% / 19% (39% / 20%) | 53% / 12% (49% / 15%) |
| after a strafe let go: the next goes the other way (stoppers) | 86% | 57% | 76% (73%) |
| reloads begun with the enemy on screen, a minute alive | 0.3 | 1.9 (1.4) | 1.6 (1.3) |
| hidden yaw speed p50 / p99 | 8.1 / 544 deg/s | 10.7 / 692 (12.1 / 683) | 11.4 / 860 (12.0 / 815) |
| hidden: back key held | 6.1% | 6.0% (5.7%) | 8.8% (7.6%) |
| net distance in 2 s with no part on screen p50 | 158 u | 135 (152) | 122 (135) |
| crosshair closer to the corner than to the enemy | 80% | 59% (57%) | 57% (58%) |
| attack gap p50 | 400 ms | 300 (300) | 350 (350) |
| all 281 statistics within 25% | | 56% (52%) | 55% (51%) |

The costs are the sound turn's: the turns to a noise are fast flicks (hidden yaw p99 up), and while the view holds
the noise the route is behind it, so the bots walk backwards more and cover less ground hidden. Aimed at the noise
itself instead of its corner the crosshair was further from the corner 1 s before a sighting (24 deg vs 15 before,
now 19-23). In the arena (eight seeds) 48% of 195 statistics are within 25% (53% before): its small open floor keeps
the enemy audible, so the sound turns make the hidden view turn at 15.8 deg/s at the median (9.4 before).

Tried and dropped: pulling a bot reloading in the enemy's view toward cover (`reload_sight_urgency` 0.65 and 1.0:
the enemy stayed on screen 84-86% of the following 2 s, 86% before, people 63%); a quiet time of 3-6 s after a
sound turn (hidden view 15-15.6 deg/s); holding the sound's direction without letting a new sound re-aim (the bots
then faced an enemy behind within a second 33-35% of the time); the counter-strafe dial as the press chance itself
rather than a shift of the fitted one (stoppers 60-63% one tick after letting go, but strafing up while hidden).

### Usercmds and the respawn (2026-10-03, from the openmohaa-movement session)

- People send 12.5 (250 fps) to 25 (500 fps) usercmds per 50 ms frame. `g_humanbot_substeps` is now 12 (the
  server cfgs set 12), `MAX_SUBSTEPS` 32 (`hb_types.h`). In the engine with 16 bots (local workstation) the frame's
  usercmds through `G_ClientThink` take 43 us per bot (18 at 4); the brain's think is unchanged (median 113-123 us
  per bot with 16 bots; the first bot of the frame reads 250-290 us at either count, so `humanbot_selftest`'s
  150 us check flags it with 16 bots). The arena's Pmove costs 7.5 us per bot per tick at 12. Movement on the maps
  with the final model at 12 vs 4 usercmds: no visible difference in the five checks above; the arena 53% vs 51%.
- After a respawn the bot sends no usercmd for as many ticks as a client at its ping (the disguise's ping model,
  stepped always) needs to see the respawn: 1 at 48 ms, 3 at 98 ms, linear between, after the spawn tick
  (`Brain::SetPingMs`, `TickPlan::send`). Without a ping (arena, replay) the fitted dead ticks are drawn as before.
- Not done: a human tick covers 47-57 ms of client time (B2 of the note); the last usercmd still lands on the frame.

## The lean follows the strafe (2026-10-04)

The owner, spectating the rebuilt server: "the bots hold a lean direction for too long (and lean into a wall for a
split second as a result)". In the live session of 2026-10-04 (`move_wip/s4/live2/`, bot1 strafer, bot2 presser) and
the practice-map captures, the presser bots held a lean side a median 400-500 ms, over 2 s 8-14% of leans (presser
people 300 ms, 1.3%) and leaned against their own strafe 30-31% of lean time (people 15.5%); the strafer bots flipped
the lean across 2.7-5.2 times a second while their strafe was against it (strafer people 0.12: they let go of it).
Cause: the style's lean habit (`lean_fight` and the calibrated per-context logit) raised *stay* against both letting
go and switching, and the chain did not tell the tick the strafe turns against the lean apart from the ticks it stays
so, while people decide at the turn.

Changes:
- The lean chain has a fourth strafe relation (`fit_movement.fit_lean`, `MovementModel::leanRels`; a 3-relation
  model still loads): the tick the strafe turns against the lean (pooled, in sight firing: flip across 38%, let go
  12%) apart from the ticks it stays against (15% and 7.5% a tick).
- Two style dials, measured per person over the whole recording (`fit_styles.lean_against`): `lean_switch` (leaning,
  with the next tick's strafe against the lean, the lean switches across: presser 0.27, stoppers 0.03, strafers
  0.007) and `lean_drop` (it is let go: presser and stoppers 0.06, strafers 0.20). With the strafe against the lean,
  `Mover::StepLean` takes the chain's own odds there by context, shifted by the two dials. The lean habit only
  governs letting go otherwise: its calibrated per-context part (+2.2 hidden firing) had cancelled the strafers'
  let-go in fights.
- `calibrate.py`: sweeps for both (`hb_arena --offset lean_switch_logit=`, `lean_drop_logit=`); the `lean_switch`
  curve's targets are spaced on the logit scale (`LOGIT_SPACED`; people span 0.002-0.32, and evenly spaced the
  stoppers' 0.03 fell between 0.002 and 0.05). Recalibrated: the pooled lean loops (`--stage pooled --only lean`) and
  the dials `lean_fight`, `lean_switch` and `lean_drop`. The other dials' curves are kept from 2026-10-03: a full
  re-sweep only redrew arena noise (the crouch sweep read 8.3 a minute at the multiplier that read 9.3 before), and it
  raised the bots' quiet hidden crouches on the maps by a fifth (2.6-2.9 to 3.1-3.2 a minute, people 2.3).
- The fitted lean wall logit is now -0.32 (was -0.41: the chain explains more of the side); three times it is -0.95.

Practice maps, bot vs bot (`move_wip/reports/ld_*` against `fc_*`, the model of "What the owner saw"); seeds
201-208, in brackets 401-408; "all leans" weighted by family like compare.py:

| | people | before | now |
|---|---|---|---|
| presser bots' leans held: p90 / over 2 s | 950 ms / 1.3% | 1900 ms / 9.2% (1750 / 7.5%) | 900 ms / 1.9% (990 / 2.0%) |
| ... leaning against their own strafe, share of lean time | 15.5% | 31% (30%) | 14% (15.5%) |
| the strafe turns against the lean: a presser flips it across | 46% | 6.5% (8%) | 52% (49%) |
| ... a strafer lets go of it / flips it | 36% / 0.2% | 12% / 27% (10% / 25%) | 21% / 4.5% (25% / 5%) |
| ... a stopper keeps it | 85% | 85% (83%) | 86.5% (85%) |
| all leans held: p90 / over 2 s | 1140 ms / 2.9% | 1525 ms / 6.2% (1480 / 5.6%) | 1170 ms / 3.6% (1200 / 3.5%) |
| leans in sight at a flat wall: into it / at an edge just ahead: around it | 35% / 77% | 35% / 63% (34% / 63%) | 31% / 66% (31% / 64%) |
| crouches a minute, hidden: quiet / after enemy fire | 2.3 / 5.4 | 2.9 / 5.7 (2.6 / 5.6) | 2.9 / 6.1 (2.7 / 5.7) |
| enemy heard behind: faced within 1 s / hit first | 62% / 7% | 53% / 12% (49% / 15%) | 52.5% / 13% (50% / 16%) |
| reloads begun with the enemy on screen, a minute alive | 0.3 | 1.6 (1.3) | 1.7 (1.3) |
| still bouts over 1 s, a fight-minute | 0.64 | 0.39 (0.33) | 0.36 (0.25) |
| all 281 statistics within 25% / tells | | 55% / 148 (51% / 136) | 56% / 137 (52% / 133) |

No statistic crossed 25% for the worse in both seed sets. In the arena (eight seeds) 48% of 195 are within 25%, as
before.

Not changed: leans with the head at a wall (within 12 u on the lean side) are 6.3-7.6% of lean time (7.1-7.4% before,
people 6.3%), and of the same kind: people's are mostly strafing into the wall with the lean (63%), the bots' mostly
with no strafe key held (48-51%, people 23%): the bot stopped at the wall, often the wall reflex letting go of the
strafe, and the lean stayed on. That is the next candidate for the owner's "leans into a wall for a split second".
The strafer bots let go at the turn 21-25% vs 36% (in sight firing 23% vs 53%): one style shift on a chain shared
by everyone, half of it the presser, cannot give the strafers their let-go in fights alone; they lean against their
strafe 24-26.5% of lean time vs 15%. The stopper bots keep the lean at the turn as their people do, but their leans
last longer (p90 1.9-1.95 s vs 1.6, over 2 s 9% vs 7%; 1.55 s before, when their leans ended by flipping).
Scripts in the git-ignored `move_wip/s5/`: `turn.py` (what the lean does at the turn), `dialfam.py` (the two dials
realised per family), `dropctx.py` (by context), `score.py` (a report's share within 25%); `move_wip/s4/lean_hold.py`
and `obs5.py` as before.

## Hits from an unseen enemy, and big maps (2026-10-04)

The owner, playing the bots on the server of `55a6d0d6`: right after spawning, bots often did not look at them, and
one walked past a doorway looking at the wall without turning even when shot. In that session (13:05:15 UTC, telemetry
copied to `move_wip/s6/live3/`) the bot walked toward the owner with its route look held 60 deg to the side; hit from
61 deg off (100 to 9 health) its view did not move for 1.3 s, until it turned to the gunfire 1.35 s later. A hit
turned the view only when it was felt more than 60 deg off; the felt direction is +-20 deg off (`damage_sigma_deg`)
and the bot sees 48 deg to each side, so a shooter just outside the view often went unanswered (in that session 4 of
7 unseen hits from 60-90 deg turned the view). Now any hit from an enemy not in sight turns the view toward where it
is felt (`ViewControl::Step`, after the same 100 ms).

Also from that session: the owner's rcon `map dm/brownffa` killed the test server (`std::bad_alloc` in
`MapPrior::InitRuntimeVisibility`). Without a recorded prior that map had 229,322 cells of 32 u, a 13 GB visibility
table. A navmesh-derived prior now takes the smallest cell size (32, 48, 64 ... 768 u) that keeps it under 3000 cells:
dm/brownffa gets 2,594 cells of 384 u, set up in 25 ms, its visibility table traced in 5 s (2 minutes at 2 ms a
frame) and cached. The map load still stalls 2.5 s while the engine builds its navigation mesh.

Practice maps, bot vs bot (`move_wip/reports/hf_*` against `ld_*`); seeds 201-208, in brackets 401-408. Hit with no
part of the shooter on screen (`move_wip/s6/hit_unseen.py`):

| shooter off the view | people | before | now |
|---|---|---|---|
| 48-60 deg: facing it within 0.5 s / 1 s | 91% / 100% (11 hits) | 76% / 88% (85% / 94%) | 88% / 100% (89% / 100%) |
| 60-75 deg: the same | 87% / 93% (15) | 79% / 87% (86% / 97%) | 86% / 98% (89% / 97%) |
| 75-180 deg: within 1 s | 75-82% | 91-99% | 91-100% |

The five checks of "What the owner saw" and the lean (`obs5.py`) stay where they were; 281 statistics within 25%
55% (56%), 56% / 52% before; no statistic crossed 25% for the worse in both seed sets. In the arena (eight seeds) 50%
of 195 (48%).

## The practice areas of the objective maps: dm/brownffa and dm/flag (2026-10-04)

The owner's games of 2026-10-04 with a friend (two people, no bots) are a training capture in openmohaa-movement
(`828edef`), whose report section 15 compares the practice maps with the same ground in the team matches: dm/brownffa
is the Bridge of obj/obj_team4, dm/flag part of V2 (obj/obj_team2). Bot against bot on those two maps (8 servers, 4 per
map, 240 s at `timescale 10`; seeds 201-208 and 401-408) was measured with section 15's own code (`cohorts.py`),
people's side reproducing the report's numbers (git-ignored scripts in `humanbot/cache/move_wip/s8/`: `quickbf.sh`
the captures, `s15.py` section 15 for people and bots, `final_bf.py` the table below, `where.py` and `fig_report.py`
the pictures, `spawnrun.py` the first seconds of a life, `viewoff.py`, `stillbouts.py`, `motion_dir.py`, `respawn.py`,
`behind2.py` the diagnoses, `crossed.py` and `arena_score.py` the checks).

**The style fit first.** `fit_styles.py` now applies the data repository's rule: a person needs 10 duel minutes
(`common.MIN_PERSON_DUEL_MIN`) and a capture row 5 (`MIN_ROW_DUEL_MIN`) to stand for a style. The friend's 1.5 duel
minutes and the owner's 1.4-minute dm/downladder game of 10-04 count only in the minute-weighted pooled values. The
families, centres, spreads and ranges are as before (the same five people); a refit with 10-04 moves the four duel
maps by seed noise (281 statistics within 25%: 54% and 55% against 55% and 56%, none crossed the line for the worse in
both seed sets, the owner's five checks unchanged; `move_wip/reports/r1_*` against `hf_*`).

**What the bots did differently.**
1. **They left the practice ground.** The practice maps hold the whole objective map; people play only the area. With
   no recorded prior, the bots had a navmesh map of the whole objective map (cells of 384 u on dm/brownffa, 256 u on
   dm/flag) and hunted all over it: 87% and 98% of their time on ground people never used, two bots in sight of each
   other 0.1-0.2 times a minute (people 13.5-15.7), 1.5-3 minutes from a spawn to the first sight (people 2.1-2.3 s).
2. **They did not run to the fight.** After a spawn people turn down the walkway and run with forward held (65-84% of
   the 1-2.5 s after it), meeting the enemy about 2 s later. The bots shuffled about the spawn pocket. With the enemy out
   of sight people on these maps hold forward 76-78% of the time they move, the bots 36-45%, strafing sideways most of
   the rest (their motion 75-89 deg off their view, people's 40). People's data shows the rule behind it, on every
   practice map: the farther away the enemy, the longer people keep forward held and the less readily they start a
   strafe (with forward held they let go of it at 9% a tick within 300 u of the enemy, 5% at 300-500, under 3% beyond
   700 u; the duel maps and dm/brownffa and dm/flag give the same rates). The key model had no distance at all.
3. **They froze for seconds.** With the enemy hidden they stood still over 2 s 1.8 times a minute on dm/brownffa
   (people 0.3) and 0.7 on dm/flag (0.3). 60% of that time a bot stood pressed against walls on three sides (spawn
   pockets, stair corners) with its route running into a wall it touched: it would not press into it, the route pull
   leant away from every open direction, and the fitted keys start ever more slowly the longer one stands.
4. **Fights:** within every style the bots back off about three times as often as people (9-13% of firing time with
   the enemy in sight, people 2-6%), as on the duel maps (see "Known gaps"). How hard they push is the style mix (half
   of people's time here is the owner, a presser). Not changed.
5. **Aim at a sighting:** 15-16 deg off at the first visible part against people's 4-5 (12 on the duel maps). Not
   changed (see "Known gaps").

**What changed.**
- `fit_maps.py`: recorded priors for dm/brownffa and dm/flag (`AREA_MAPS`) from people's rows with the duel
  definitions without the map list (both alive, normal physics, as section 15). dm/flag is one recorded session and
  dm/brownffa mostly one, so on those maps a cell or spawn needs two player-sessions (two people, or one person in two
  sessions) instead of two sessions: 369 and 242 cells of 32 u. The friend's movement is in them as per-cell counts,
  like everyone's in the other maps; the owner agreed. `hbdata.py` loads `human_duel` and `normal_physics`
  (`dm_seq_v8`).
- `fit_keys.py`: with the enemy hidden, an offset of each key's change rate by distance (`hid_dist_logit`,
  `HID_DIST_EDGES` 0/300/500/700/1000 u; side key by strafing and the forward key, forward key by its state), fitted on
  the duel maps. `hb_movement` (`HidDist`) applies it with the distance to the believed position.
- `hb_movement`: with the enemy hidden the route pull grows with that distance, x1 within 500 u to x3 from 900 u
  (`nav_far_*` in the couplings, set by hand on captures of all six maps). People's rate by distance is the same on
  every practice map, but the far pull stands in for something else on dm/brownffa and dm/flag (the sideways gait
  below), and the duel maps' hidden fights sit at 250-450 u: with the ramp at 300 to 700 u the duel bots stood with the
  enemy hidden 15% of that time (22% before, people 22.5%), fought in sight more (22% of duel time, people 18%) and
  five statistics crossed 25% for the worse in both seed sets. From a spawn to the first sight on dm/brownffa /
  dm/flag (people 2.3 / 2.1 s): without the far pull 7.9-9.2 / 7.5-8.9 s, ramp 300-700 u 4.4-4.7 / 5.6-6.0 s,
  500-900 u 6.0 / 5.6-7.1 s, 600-1000 u 6.7-7.4 / 6.4-7.8 s; on the duel maps 500-900 and 600-1000 were the same
  within seed noise (`move_wip/s8/` `bn*_`, `n*_` and `patch_n*.json`). The far pull gives way while the bot presses
  into a wall (`MoveInput::wallPressMs`): three times the pull outweighed letting go of a key held into it, and in the
  arena the bots pushed against pillars (wall-pressure bouts 0.03-0.23 a bot-minute and one stuck 2.15 s in eight
  seeds; none before or with it off). On the maps the pushes of a second or more the engine hands to the stock code
  doubled on dm/crnodoors and dm/flag with the first ramp (to 0.09-0.10 a bot-minute); now 0-0.06 as before.
- `hb_movement`: out of a nook. A bot travelling somewhere (hunting or out of the spawn, route urgency 0.5 or more,
  `MoveInput::travelling`), that has held no key for a second and whose best direction for its route runs into a wall
  it touches takes the open chord nearest the route (`UNSTICK_*`, set by hand). Not after an enemy just lost: with the
  rule there too, the duel bots stood still for a second or more right after losing sight a third as often (0.04 a
  fight-minute against 0.11; people hold the corner the enemy went out of sight at); now 0.08-0.11.
- `calibrate.py`: the strafe and forward habits with the enemy hidden re-looped (`--only habit.side.hidden,
  habit.fwd.hidden`; the distance term acts only there: a full habit run drifted the in-sight loops on noise and the
  bots stood still in fights half as often as before, 0.18 bouts over 1 s a fight-minute against 0.28-0.37, people 0.64).
- Unit tests: the six priors and their checksums (`test_hb_core`), the distance term, the way out of a nook and the
  far pull giving way at a wall (`test_hb_modules`); the open-loop pure-strafe band widened to 0.83 (0.80, under 0.8 by a hair before).

Tried and dropped: the fight-diagonal style dial at the strength people's styles show in each context. The dial is
measured in fights; across the five people its difference shows at 0.52 of that with the enemy hidden, 0.87 firing
into cover, 0.77 in sight not firing and 0.22 reloading (logit slopes of the share of strafing time with forward held).
Scaled so (and its curve re-swept down to -5), the strafer and stopper bots held forward out of sight 2-3 points more
on dm/brownffa and dm/flag, but on the duel maps the bots, the pressers most, stood still in fights half as often
(still bouts over 1 s 0.15 a fight-minute against 0.32; with the scale off 0.28; people 0.64). A route look weighted up with the believed distance (`travel_far_share` 2: no change, the corner
pre-aim and the turn to noises take most of the hidden view); without the turn to noises, with fewer holds
(`hold_hazard` 0.005) or with a doubled route pull at every distance (single half-size captures: 7.0-8.8, 5.2-6.2 and
4.0-5.1 s from a spawn to the first sight; forward held 38-44% in every variant).

dm/brownffa and dm/flag, bot vs bot (`move_wip/s8/final_bf.py`; seeds 201-208, in brackets 401-408; "before" is
`cab9e163`, one seed set; "pull off" the final tree with `nav_far_mult` 1):

| | people | before | pull off | now |
|---|---|---|---|---|
| dm/brownffa: time on ground people never used | 0 | 87% | 7% (9%) | 5% (5%) |
| ... two bots in sight of each other, a minute | 13.5 | 0.1 | 4.0 (4.0) | 5.9 (5.8) |
| ... from a spawn to the first sight (median) | 2.3 s | 89 s | 9.2 s (7.9) | 6.0 s (6.0) |
| ... standing over 2 s with the enemy hidden, a minute | 0.30 | 2.35 | 1.06 (0.52) | 0.80 (0.37) |
| dm/flag: time on ground people never used | 0 | 98.5% | 14% (10%) | 10% (8%) |
| ... two bots in sight of each other, a minute | 15.7 | 0.2 | 4.5 (4.3) | 5.5 (5.1) |
| ... from a spawn to the first sight (median) | 2.1 s | 178 s | 7.5 s (8.9) | 5.6 s (7.1) |
| ... standing over 2 s with the enemy hidden, a minute | 0.34 | 1.83 | 0.56 (0.79) | 0.26 (0.50) |

With the ramp at 300 to 700 u: 7.8-8.2 and 5.5-6.0 sightings a minute, 0.16-0.37 and 0.21-0.42 stands over 2 s.


The four duel maps (`move_wip/reports/fin_*` against `hf_*`, the model of "Hits from an unseen enemy"): 281
statistics within 25% 56% (52%), 55% (56%) before; one crossed for the worse in both seed sets (the view's turn toward
a corner's appearance within 500 ms, 13-15 deg against 12-12.5, people 10) and one for the better. The owner's checks
(`obs5.py`) stay where they were: leans at walls, crouches, the turn to a noise behind (faced within 1 s 51% (52%),
52.5% (49%) before), with two small drifts also seen with the far pull off: reloads begun with the enemy on screen 1.55
(1.33) a minute alive against 1.47 (1.23), people 0.3, and still bouts over 1 s in fights 0.29 (0.27) a fight-minute
against 0.37 (0.28), people 0.64. Standing with the enemy hidden 18% (18%) of that time (22%, people 22.5%), in sight
firing 21% of duel time (19%, people 18%). In the arena (eight seeds) 48% of 195 within 25% (50%), no stuck bout.


**Still off on these maps.** The sideways gait: with the enemy hidden the bots hold forward 33-42% of the time they
move (people 72.5-74.5%) and move 73-89 deg off their view (people 40). The far pull was meant to make up for it and
could not without spoiling the duel maps; the next step is the gait itself. Their route lies behind the view 30% of that
time (in a turn to a noise the route is behind 36% of it), the hunt goal hops (they reach the believed position, the
belief moves on and the goal with it: goal jumps of 200 u or more 27-33 times a minute, on the duel maps too), and a
quarter of their hidden movement is in a hold with no route at all, where the fitted keys wander. People look toward
the enemy (18 deg off it at the median, the bots 34) and run on the forward diagonal toward it. On dm/flag a tenth of
the bots' time is on a loop people never used: the navmesh route between the two spawns is shorter through it, and the
route is the navmesh's, not the prior's. Crouching: 4-6 presses a minute against 3.6 (dm/brownffa) and 2.0 (dm/flag).

## Where the bot looks when first seen (2026-10-05)

The owner, playing the server of `62c99112` for a minute on dm/crnodoors: "when I first see the bot (after he spawns)
he's almost looking elsewhere than towards me, and takes a while to look at me". In that session (8 bot lives,
`move_wip/s9/live4/`, `firstsee.py`, `spawntl.py`) the bot looked 87-167 deg away from the owner in 3 lives when the
owner first had it on screen, though the position it believed the owner at was within 10 deg of the owner each time. Over the
practice-map captures (`spawnview.py`: the first moment the opponent has a player on screen within 10 s of that
player's spawn; on screen = some part visible and within 48 deg of the opponent's view, since the logged `ext_in_fov`
differs between the recordings' schemas) people look 5 deg off the enemy at the median, over 45 deg in 4% of lives and
never over 90; the bots 18 deg, 25% and 10%, and a fifth of them took over 500 ms to get the view on (people 5%). In
nine in ten of those lives the bot's belief was right within 30 deg and its view more than 30 deg off its own belief
(`spawnmode.py`): turned to a sound (40-45% of the lives over 45 deg off, 66-71% of those over 90), holding a direction
(28-30%, mostly one kept after running past a pre-aimed corner) or watching a corner (15-18%).

Causes and changes:
- **Front/back confusion of footsteps** (`front_back_confusion`, a plan value of 0.25: a quarter of footsteps heard on
  the mirrored side). Since the turn to a noise behind ("What the owner saw") the bots acted on it: one hearing an enemy
  in front would spin round. People do not (`wrongway.py`): with an unseen enemy running in front of them within 1000 u
  they turn around within a second 2.4% of the time, no more than with a quiet one (3.1%), and they face one running
  behind them 69% (quiet 33%). The bots turned around 11% (quiet 8-9%). Now 0: they turn around 2.7% (3.1%).
- **Footsteps just outside the view** (48-90 deg) were not turned to (`sound_turn_deg` 90). People get their view onto
  an unseen enemy running there within a second 74-80% of the time (quiet 51-53%), the bots 70-71% (61-64%)
  (`hearband.py`). Now 48: 80-85%.
- **A held direction gives way** once the enemy is believed out of view of it (`hold_belief_deg` 48,
  `ViewControl::Step`): a new look, which with a focused belief watches the believed position or its corner.

Practice maps, bot vs bot (`move_wip/reports/hr3_*` against `fin_*`, the model of "The practice areas of the objective
maps"); seeds 201-208, in brackets 401-408; `hr1_*` the hearing alone, `hr2_*` with the turn outside the view:

| | people | before | hearing | + outside the view | + hold gives way |
|---|---|---|---|---|---|
| first seen after a spawn: view off the enemy, median | 5 deg | 18 (18) | 15 (16) | 13 (14) | 12 (13) |
| ... over 45 deg / over 90 deg | 4% / 0% | 25% / 10% (25% / 10%) | 18% / 4% (19% / 4%) | 12% / 2% (13% / 3%) | 11% / 2% (11% / 3%) |
| ... view on the enemy after over 500 ms | 5% | 21% (22%) | 15% (16%) | 12% (13%) | 10% (10%) |
| enemy running in front, unseen: turns around within 1 s | 2.4% | 11.3% | 2.7% | 2.8% | 2.7% |
| enemy heard behind: faced within 1 s / hit first (`obs5.py`) | 62% / 7% | 51% / 14% (52% / 14%) | | 58% / 9% (55% / 11%) | 60% / 10% (57% / 13%) |
| all 281 statistics within 25% | | 56% (52%) | 58% (55%) | 60% (57%) | 58% (58%) |

Against `fin_*`, 12 statistics crossed 25% for the better in both seed sets (the belief's accuracy with the enemy hidden
now at people's: within 30 deg 4-8 s after losing sight 66% against 53%, people 76%; the hidden view's p99 yaw speed
650-660 deg/s against 840, people 545; the crosshair 500 ms before a sighting 15.5-17 deg off against 19-21, people 14;
at the first part 13-14.5 against 16-19, people 5) and 4 for the worse (forward alone with the enemy hidden 15-16% of
that time, 13.5-15% before, people 12%; rounds released per tick 3-4 ticks into a burst; shots in the 500 ms before a
sighting 23% against 21-22%, people 18%; the turn of the view in the 500 ms before a sighting 10.4-10.8 deg against
11-12.5, people 14.5). The bots find each other sooner (first sight p75 2.2 s against 2.45-2.55, people 1.9) and are in
sight firing more (24% of duel time against 21.5-22.5%, people 17.5%), so they begin more reloads with the enemy on
screen (1.5-1.8 a minute alive against 1.3-1.55, people 0.3). Quiet crouches with the enemy hidden 2.9-3.0 a minute
(2.7-2.8, people 2.3). With the enemy quiet and just outside the view the bots now look toward it more than people
(48-70 deg: 74% within a second against 53%): their belief is better than before and the hold gives way to it. On
dm/brownffa and dm/flag (seeds 201-208): first seen after a spawn over 45 deg off 11% (24%), over 90 4% (14%); two bots in
sight of each other 7.3 and 5.6 times a minute (5.9, 5.5); from a spawn to the first sight 5.1 and 5.3 s (6.0, 5.6); aim
at the first sight 11 deg off (15-16). In the arena (eight seeds) 53% of 195 within 25% (48%), no stuck bout.

Still off: the remaining lives seen over 45 deg off (11% against 4%) are spread over a pre-aimed corner away from the
believed position (27-31% of them), a turn to a sound under way (23-26%), a route look (15-17%) and look-arounds (7-8%).

## Late aim and low clips (2026-10-05)

The owner, on the server of `85b943f3`: "last kill i made, what happened to the bot? it didnt shoot me at all"
(`move_wip/s9/live4/1791163568_607971_*`). The bot (a strafer with the slowest reaction draw, 189 ms) had fired 16 rounds
into cover at the owner's footsteps, was looking 30-40 deg off when the owner came round, got its crosshair onto the
owner only about 2 s later, and then tracked the owner for 7 s at 1-2% a tick of pressing (16 rounds left, not reloading) until it
died. The press hazard in sight falls with the time since the parts came on screen (-1.56 from 2.5 s); with the slow
reaction draw (-1.17) and a long gap since the last burst (-0.32) the crosshair on the body gave 1.5% a tick. People
with the crosshair on the body press 21% a tick 1-2.5 s into a sighting and 11-14% after 2.5 s; the additive model fits
those cells (8.5% from 2.5 s). What it lacks is a late aim: people's aim is within 2 body half-widths by 0.3 s in nine
sightings in ten, and when it gets there 0.3-1.5 s in they press at 20-38% a tick in the next 250 ms
(`move_wip/s9/lateaim.py`), as early as any. Earlier in the same session the bot stopped firing for another reason: a
brief loss of sight with 6 of 30 rounds left, a reload begun 150 ms later (at people's rate for that moment), and the
owner back in view for the 2 s of it.

Changes:
- **The press clock restarts when the aim arrives late** (`aim_arrive_hw` 2 in the trigger tuning, `Brain::Think`): the
  first time in a sighting the crosshair comes within 2 body half-widths of the enemy, if it was not there at the
  sighting's first tick, the clock is set back to 200 ms (the fitted peak; `AIM_ARRIVE_CLOCK_MS`). In the test above
  (`test_hb_belief` `TestLateAim`) the press goes from 2.2% to 17% a tick. Bot against bot a late aim is rare, as for
  people: arrivals 0.8-1.5 s into a sighting press 32% a tick in the next 250 ms (19% before, people 20%).
- **Little left in the clip in a fight: make for cover** (`low_clip_cover` 0.25 in the nav model, `Navigator::Step`):
  with under a quarter of the clip and the enemy in sight or lost under a second ago, the intent is the reload cover at
  the reload urgency (0.4; 0.8 did no better). People in sight with under a quarter of the clip are out of the enemy's
  sight 1-1.5 s later 54-59% of the time (30-35% with more) and move away from it 2.5 times as often; the bots 20-38%.
  The bots go into fights with clips as full as people's and fire as many rounds a minute; they ran dry in view.

Practice maps, bot vs bot (`move_wip/reports/tc2_*` against `hr3_*`, the model of "Where the bot looks when first seen";
seeds 201-208, in brackets 401-408; `tc1_201` the clock alone, `tc3_201` the cover at urgency 0.8):

| | people | before | now |
|---|---|---|---|
| reloads begun with the enemy on screen, a minute alive | 0.31 | 1.61 (1.42) | 1.27 (1.25) |
| ... the enemy on screen in the next 2 s | 63% | 85% | 78% |
| in sight with under an eighth of the clip: out of sight 1-1.5 s later | 59% | 20% (21%) | 31% (27%) |
| ... an eighth to a quarter | 54% | 38% (38%) | 46% (43%) |
| reloads begun with the enemy lost under 2 s, a minute alive | 0.39 | 0.44 | 0.54 (0.56) |
| all 281 statistics within 25% | | 58% (58%) | 59% (56%) |

No statistic crossed 25% for the worse in both seed sets (one for the better: the mouse at rest while reloading). The
first look at a sighting after a spawn and the owner's other checks stay where they were. In the arena (eight seeds)
52% of 195 within 25% (53%), no stuck bout.

## Back and forth with the enemy hidden (2026-10-05)

The "sideways walk" of the practice areas ("Known gaps") is mostly running back and forth. Of the 3 s stretches with
the enemy hidden in which a player ran 300 u or more, people end within 100 u of where they started 9% of the time on
dm/brownffa and dm/flag and 18% on the duel maps; the bots of `bd746743` 41% and 34%, and their 2 s paths were half as
straight (net over path 0.29-0.33 against 0.58-0.61). In the owner's games of 2026-10-04/05 (`move_wip/s9/live4/`,
`live5/`) it is 18% of the bots' hidden stretches. On dm/brownffa and dm/flag 17% of the bots' alive time was at the
exit of a spawn area (people 4-5%), strafing along the wall beside the opening with the opening straight ahead and
never pressing forward through it, or running past a turn of the path and back. Diagnosed on captures at `bd746743`
(`move_wip/reports`/`eval/cache/bot` `bt0_*`, `tc2_*`) and one diagnostic server that printed the path's corners each
tick (git-ignored scripts in `humanbot/cache/move_wip/s10/`: `dith2.py` the back and forth, `qs.py` the gait table,
`rstab.py` how steady the route is, `trace.py` tick traces, `dspots.py` where, `trans.py` key transitions, `walls.py`;
the experiments' code is `s10/experimental_all.diff`).

**What it was.**
- **The keys followed the route loosely.** A key changed for the route only when that improved how its chord goes the
  route's way by more than 0.3 (the cosine of the angle): forward alone with the route 45 deg off, or a strafe with the
  route straight ahead, was "good enough" and kept. With a stable route in the open the held chord was within 22 deg of
  it on 43% of ticks; at a turn of the path the bots ran past it, the route pointed back, and they came back.
- **The route swung.** The walls' push on the route (`g_humanbot_wall_steer`, "Wall contact") at full strength turned
  the route more than 30 deg on 22% of ticks, and in a corridor the walls on either side took turns (15 times a minute
  across); the route was within 15 deg of where it pointed half a second earlier on only a fifth of the ticks the bots
  ran hidden. Corners passed close by swung it too.
- **Not the hunt goal** (3% of the route's big swings came with a new goal), **nor the view**: kept within 60 deg of
  the route, or turned to it at path turns, the view lagged its target (31 deg at the median while looking down the
  route) and the gait did not change.

**What changed.**
- `nav_deadband` 0.1 (coupling, `assemble_model.py`; was the constant 0.3 in `hb_movement.cpp`). Unit test
  `TestRouteTurns`: with a route that takes a new bearing every second, the keys go its way at a cosine of 0.89 (0.86).
- `g_humanbot_wall_steer` 0.5 (the default and `duel.cfg`; was 1): the bots touch walls as often as before and as people.

Bot against bot (`move_wip/reports/ov_*` against `tc2_*` on the duel maps, `bov_*` against `bt0_*` on dm/brownffa
and dm/flag; seeds 201-208, in brackets 401-408):

| | people | before | now |
|---|---|---|---|
| dm/brownffa, dm/flag: ran 300 u in 3 s, back within 100 u | 9% | 41% (41%) | 34% (37%) |
| ... straightness of the path (net / path) | 0.61 | 0.29 (0.30) | 0.35 (0.33) |
| ... forward held while moving hidden | 77% | 43% (39%) | 47% (44%) |
| ... net distance in 2 s hidden p50 | 267 u | 101 (97) | 117 (113) |
| ... touching a wall / standing at one | 9.1% / 6.3% | 9.8% / 11.4% (9.9% / 11.1%) | 10.1% / 10.4% (9.7% / 9.7%) |
| dm/brownffa: two bots in sight of each other, a minute / from a spawn to the first sight | 13.5 / 2.3 s | 7.3 / 5.0 s (7.4 / 4.9 s) | 10.1 / 3.6 s (10.3 / 3.4 s) |
| dm/flag: the same | 15.7 / 2.1 s | 5.7 / 5.3 s (5.5 / 5.6 s) | 5.4 / 5.8 s (5.4 / 6.0 s) |
| duel maps: ran 300 u in 3 s, back within 100 u | 18% | 34% (34%) | 31% (35%) |
| ... forward held while moving hidden / net distance in 2 s hidden | 63% / 212 u | 54% / 157 (51% / 147) | 57% / 182 (55% / 176) |
| ... touching a wall | 7.3% | 5.9% (6.3%) | 5.7% (5.5%) |
| ... all 281 statistics within 25% | | 59% (56%) | 58% (56%) |

Two statistics crossed 25% for the worse in both seed sets: strafes are held a median 200 ms (250 before, people 300)
and the enemy is on screen 44-45% of duel time (40-41%, people 34%); two for the better (back-offs at 288-384 u in
fights, the hidden release 1-1.5 s into a hold). The owner's checks (`obs5.py`) stay where they were except that the bots,
in view of each other more, begin a few more reloads with the enemy on screen (1.71 (1.39) a minute alive against 1.51
(1.24), people 0.31). In the arena (eight seeds) 52% of 195 within 25% (52%), no stuck bout; 83 us per bot per tick
with 16 bots.

Tried and dropped (dm/brownffa and dm/flag, seeds 201-208, and the duel maps where noted): `nav_deadband` 0 (as 0.1)
and 0.2 (half the gain); the coupling 1.5x or 2x (`nav_switch_logit`/`nav_choice_logit`; travel 134-138 u, but the bots
stood hidden 10% of the time, people 15%, and on the duel maps 56% within 25%); the walls' push off (back and forth 28%,
but walls touched 12.7%) or smoothed over ticks (no change); aiming 64-128 u along the straightened path or 24 u wide of
its corners (small, more wall contact; `IPather::GetCorners` was added for it); the navigation mesh built for a 16 u
agent (worse); the keys following a smoothed route (worse); judging each key with the other at the best chord's state
(back and forth 35%, standing 28%); a hold that stops the keys (standing 21-38%); the view kept within 45-60 deg of the
route, turned to it at path turns or more readily behind (no change); shorter holds of a sound's direction (no change).

**Still off.** On dm/flag the bots still loop through the junction west of the spawn and the side route people never
use; the back and forth is still three to four times people's; with the enemy hidden the bots hold forward less than
people and run on more crooked paths, and people's straight paths come from steering with the mouse, which the bots'
keys-follow-the-route design does not do.

## Steering with the view (2026-10-05)

The owner asked for a travel mode: with the enemy out of sight, the view leading along the path and the keys mostly on
forward or a forward diagonal, keeping the corner pre-aim and the turn to sounds behind for when the enemy is expected
soon. (The owner had not played the test server since 05:45 UTC that morning: one stranger connected at 05:57 and timed
out before joining; no telemetry was written.) Diagnosed on the captures of `5a31a932` (`bov_*`, `ov_*`) and a
diagnostic server that printed the path's corners each tick (git-ignored scripts in `humanbot/cache/move_wip/s11/`:
`lead.py` how the view leads the path, `modes.py` the view modes and keys, `rev*.py` and `trace2.py` the back and
forth, `junc.py` dm/flag's junction, `flicks.py` the hidden view's quick turns, `chk.sh` the table below).

**What it was.**
- **People look where they will be.** Running with the enemy hidden on dm/brownffa and dm/flag their view is a median
  20 deg off the way to where they are a second later (72% of the time within 30 deg), and they hold forward (28%) or a
  forward diagonal (49%), a plain strafe 15%. The bots looked at corners, the believed position and sounds (24 deg; 58%)
  and strafed 35-38% of the time they ran.
- **The view alone did not change the keys.** With the view leading the path, the way was within 45 deg of it three
  quarters of the time, and the fitted key habits still held a plain strafe 31% of it (half of those begun from a
  diagonal at a wall, the rest from short back-diagonal steps).
- **Doors.** On dm/flag half of the back and forth was at the junction west of the spawn: a rotating door (hinge at
  (1878, 2880)) closes a corridor's mouth, and another at (1198, 2320). The practice maps' doors open to the use key
  only, aimed at them (`Player::getUseableEntities`: 64 u along the view); the bots pressed use only with a closed door
  straight ahead in their view, and with the view on the corridor beyond or on a corner they slid along the closed leaf
  for seconds (people open it and walk through).
- **Narrow mouths.** On dm/brownffa 60% of it is at two corridor mouths about 70 u wide ((-1385, 1566) and (-1109, 1060)):
  approaching on a diagonal the bots hit the wall beside the opening, slide past and come back (not fixed).
- **Near the enemy.** Within 700 u of a hidden enemy the bots end 39-48% of their 3 s runs where they began, people
  3-10%: people keep going forward there (64% of the time at 400-1000 u, the bots 45-50%), the bots strafe and turn about.

**What changed.**
- **The view's travel mode** (`hb_view`, `travel_*` in the view tuning, `assemble_model.py`): hunting or out of the
  spawn, with no enemy expected soon (the imminence below 0.2; back in only below 0.1, else the view flipped between a
  corner and the way 16 times a minute), the view looks toward the point 200 u ahead on the path. That direction follows
  the point with a quarter of the gap a tick, a turn of more than 45 deg is one sweep (0.15 a tick), the mouse does not
  rest with the way more than 20 deg off, and the fitted controller turns the view. Look-arounds and the turns to a hit
  or a sound run their course in it; a sound behind still turns the view.
- **The keys follow the way more firmly while the view leads** (`travel_pull` 3 in the couplings), not while pressing
  into a wall (in the arena a bot pushed against a pillar for 2.1 s with it).
- **Doors:** the engine traces 96 u from the eye along the path's direction for a closed door (`DoorAhead`,
  `humanbot_adapter.cpp`); whenever the bot has a way to go the view turns to it (one sweep), and the existing use logic
  opens it.
- The engine passes the straightened path's next corners (`IPather::GetCorners`, `SelfState::navCorner`), and
  `Navigator::PointAhead` walks them (in the arena, the route cells). Unit tests `TestTravelLead`, `TestFarPullWall`.

Bot against bot (`move_wip/reports`/`eval/cache/bot` `bty_*` against `bov_*` on dm/brownffa and dm/flag, `ty_*` against
`ov_*` on the duel maps; seeds 201-208, in brackets 401-408):

| | people | before | now |
|---|---|---|---|
| dm/brownffa, dm/flag: ran 300 u in 3 s, back within 100 u | 9% | 34% (37%) | 25.5% (24.7%) |
| ... straightness of the path (net / path) | 0.61 | 0.35 (0.33) | 0.43 (0.44) |
| ... moving hidden: forward held / plain strafe | 77% / 15% | 47% / 35% (44% / 38%) | 58% / 29% (56% / 32%) |
| ... moving hidden: view within 45 deg of the motion | 75% | 45% (42%) | 55% (52.5%) |
| ... view off the way to where the bot is 1 s later p50 | 20 deg | 24 (24) | 18.7 (17.7) |
| ... net distance in 2 s hidden p50 | 267 u | 117 (113) | 170 (162) |
| dm/flag: time at the junction door's mouth / on the loop people never use | 2.2% / 0 | 5.8% / 14% (6.1% / 11.5%) | 3.3% / 7.3% (3.4% / 8.4%) |
| dm/flag: sightings a minute / from a spawn to the first sight | 15.7 / 2.1 s | 5.4 / 5.8 s (5.4 / 6.0 s) | 8.9 / 3.8 s (8.8 / 3.9 s) |
| dm/brownffa: the same | 13.5 / 2.3 s | 10.1 / 3.6 s (10.3 / 3.4 s) | 12.2 / 3.0 s (12.2 / 3.0 s) |
| ... hidden yaw speed p50 / turns of 10 deg or more a hidden minute | 10.2 deg/s / 39 | 10.7 / 54 (10.2) | 15.7 / 61 (15.8) |
| duel maps: ran 300 u in 3 s, back within 100 u | 18% | 31% (35%) | 27% (28%) |
| ... forward held moving hidden / net distance in 2 s hidden | 63% / 212 u | 57% / 182 (55% / 176) | 62% / 205 (58% / 190) |
| ... hidden yaw speed p50 / turns of 10 deg or more a hidden minute | 9.0 deg/s / 39 | 14.9 / 54 (15.0 / 54) | 18.6 / 58 (19.1 / 61) |
| ... all 281 statistics within 25% | | 58% (56%) | 56% (54%) |

On the duel maps no statistic crossed 25% for the worse in both seed sets, and one for the better: the view's turn
toward the enemy in the last 500 ms before a sighting, 10.3-12 deg (7.9-8.9, people 13). The owner's checks (`obs5.py`)
stay where they were (an enemy heard behind faced within a second 61% (56%), people 62%). Walls are touched as before
(5.5-5.9% of duel time, people 7.3%; 8.6-8.8% on dm/brownffa and dm/flag, people 9.1%), no bot was stuck for 1.5 s. In
the arena (eight seeds) 52% of 195 within 25% (52%), median 0.23, no stuck bout; 86 us per bot with 16 bots. Two runs
of the same model and seeds differ by up to 3 points in the back and forth.

Tried and dropped (dm/brownffa and dm/flag, seeds 201-208): the view on the point ahead without the smoothing, its
error gain x2.5 and sweeps from 30 deg at 0.3 a tick (a quick turn begun 65 times a travel minute, now 50; x1.5 and x2.5
overshoot a 40 deg turn by 20% and 45%); the travel mode up to an imminence of 0.1 (no gain); no hysteresis; the pull as
before (the back and forth 3-4 points higher) or x2; the keys heading for the farthest point along the path, of 192,
128, 80 and 48 u, that the body reaches in a straight line (a box trace above step height, inner corners rounded 16 u)
and the view led to it (no gain); sounds behind not turned to on the way unless the enemy is expected soon (no gain, the
view turned more).

**Still off.** The back and forth is 25% and 27-28% (people 9% and 18%), most of it near a hidden enemy and at
dm/brownffa's two corridor mouths; the bots still hold a plain strafe twice as often as people while they run hidden,
and their hidden view turns more than before (median 16-19 deg/s, people 9-10; 58-61 turns of 10 deg or more a hidden
minute, 54 before, people 39).

## Known gaps (two average-style bots; "real maps" = the reports above)

- **Aim at a sighting:** 11.5-12.1 deg off at the first visible part vs people's 5.2 (10.2 before
  "Encounters": the bots now meet more often on the move; 10.3-11.2 before "Travel and the hidden
  view": they run more, and pass corners they run by), and the first press after a clean sighting
  comes at 200-250 ms vs 150. The bots pre-aim corners but pick the one the enemy comes out of less
  often: closer to the corner than to the enemy 58% vs 80% (53% before "Corners and the lost enemy"), and
  in the last 500 ms before the first part their view turns 7-7.5 deg toward the enemy vs 13. An ad-hoc check
  on the tuning captures found them within 4 deg of the true corner at the onset 33% of the time vs 53%, and
  their error given that distance matching people's. The belief offers the right corner as its heaviest in 57%
  of sightings; within 5 deg the right corner is not offered at all in 40% (see "Corners and the lost enemy").
  The owner judged this gap minor next to what shows in play. Downstream of it: the first hit lands 400 ms after the first part (250),
  and 35% of part sightings end without a hit (28%; 45% before "Encounters"). Prefire is short after
  long hides: the bots hold fire before the first part at 20% of the part sightings vs 23%; their
  anticipation of where and when the enemy comes out is the limit.
- **Partial exposure:** the enemy shows only parts (centroid hidden) 6.1-6.3% of duel time vs 10.5%
  (4% before "Encounters"), and bots hit 12% of their rounds there vs 22%. It is about people's
  share of the time on screen: the bots see the enemy less often, not peek less. They fire at such
  views as often as people, so the centroid-based hidden fire (`trigger.hold_hidden` 19% vs 24.5%,
  13% before) is low for want of these views, not the trigger.
- **Encounters** (see "Encounters", "Movement on the maps" and "Travel and the hidden view"; people's
  family mix): 30-32 sightings a minute vs 33, the enemy on screen 37.5-40% of duel time vs 34%. Still
  short: the ground covered with the enemy out of sight (137-147 u in 2 s vs 158; 116-128 before "Travel
  and the hidden view": near walls the reflex still changes keys at 64% a tick vs people's 25%, now mostly
  onto the forward diagonal along the wall; the forward key's runs are 1.1 s vs 1.4; the bots park
  next to walls, no key held, 5% of their hidden time vs 3%), the
  time from both alive to the first part (1.4-1.55 s vs 1.35), the run after a kill (144 u/s vs 236),
  the view 1-4 s after the enemy respawned (39-49 deg off it vs 17; the belief is within 7 deg, the
  view is on corners), and dm/main's lower room, where people spend 14% of their time and the bots
  almost none (the server's spawn rule rarely puts them there and their routes do not lead there).
  Lives are 10-11 s vs 7. The hidden diagonal is 28-30% vs 30% (per family the strafer and stopper bots
  21-23% and 19-21% vs 24% and 18.5%; 15-18% before "Travel and the hidden view"); forward alone 14% vs
  12%, standing 14-15% vs 22.5%.
- **The practice areas (dm/brownffa, dm/flag)** (see "The practice areas of the objective maps", "Back and forth
  with the enemy hidden" and "Steering with the view"): the bots meet 12.2 times a minute on dm/brownffa and 8.9 on
  dm/flag vs 13.5-15.7 and take 3 and 3.8-3.9 s from a spawn to the first sight vs 2.1-2.3. With the enemy hidden they
  run back and forth (25% of the 3 s stretches with 300 u run end within 100 u of the start, people 9%), most of it near
  the enemy and at two corridor mouths on dm/brownffa, and hold forward 56-58% of the time they move (77%); on dm/flag
  8-10% of their time is on a loop people never use (the navmesh route between the spawns). They stand over 2 s with the
  enemy hidden 0.26-0.8 times a minute vs 0.3 (before "Steering with the view"). Fights there:
  back off 10.5-13.5% of firing time vs 3-4%, aim 14-16 deg off at the first sight vs 4-5, crouch 4-6 a minute vs 2-3.6.
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
  1.02 (people 1.23) and the kills a minute rose 16%. Since the tap loop is matched on the holds
  begun in sight (see "Fire into cover, bursts and ladders") the near and tap loops no longer pull
  against each other.
- **Retreats:** bots back off in fights at 160-288 u 9-15% of the time vs 5-7% (people's family mix;
  14-23% before "Movement on the maps") and approach 36-46% vs 35-38% (24-33% before "Travel and the
  hidden view"; on the held-out seeds above people's, with the fight diagonal 40% vs 31%). The back key
  is held as often as people hold it: at those ticks the bots' crosshair is 12 deg off the enemy (people
  4), so a plain strafe drifts away from it (the close-range tracking below). The low-diagonal styles'
  dial used to push them backwards.
- **Seeing the enemy without firing:** people stand still 30% of that time, bots 8-9% on the real
  maps (14% before "Encounters") and 11% in the arena. The long freezes were bots out of ammunition (see above).
- **Reloading in the enemy's view:** the bots begin 1.25-1.3 reloads a minute alive with the enemy on screen vs
  people's 0.3 (1.3-1.6 before "Late aim and low clips"), nearly all with the clip run dry, and keep the enemy on screen
  78% of the next 2 s vs 63%: their fights in view last longer (the aim at a sighting above), and with little left in
  the clip they leave the enemy's sight about half as readily as people (27-46% vs 54-59%). This is what the owner saw
  as standing still mid-fight.
- **Leaning toward an opening:** people already lean toward the edge they reach (66% of edge moments begin leaned
  that way, the bots' 41%); the wall term acts once the bot is there.
- **Leaning all the time (presser and stopper styles)**: found in the owner's games of 2026-10-04/05, not changed: the
  owner judged the leaning fine for now (2026-10-05) and asked to leave it. The
  presser-style bots lean 84% of their hidden time, 91% while reloading and 92% in sight not firing; the presser 54%,
  47% and 52%. The stopper-style bots 81%, 79% and 88%; their people 68%, 43% and 44%. Firing, both match (93-97% vs
  88-93%); the strafer style matches everywhere. In the owner's games those bots leaned 75-92% of the time.
- **Leaning at walls** (see "The lean follows the strafe"): half the bots' leans with the head at a wall come with no
  strafe key held (people 23%): the bot stopped at the wall and kept the lean. The strafer bots let go of a lean at
  the strafe's turn 21-25% vs 36%, and the stopper bots' leans last longer (p90 1.9 s vs 1.6).
- **Turning to a noise behind** (see "What the owner saw" and "Where the bot looks when first seen"): 57-60% within a
  second vs 62%, hit first 10-13% vs 7% (49-53% and 12-15% before the bots heard which side footsteps come from).
  While the view holds the noise the route is behind it: the back key 7.6-8.8% of hidden time vs 6%, 2 s hidden
  travel 122-135 u vs 158.
- **Stopping between strafes (stoppers):** after letting go of a strafe the stopper bots press again a tick later
  45-50% of the time (the dial's target 52% over all recorded maps; the stopper people on the practice maps 68%) and
  pause a quarter second or more 27-31% vs 13%.
- **Fire into cover:** it fades with the time since sight like people's (more than 5 s after the
  parts were last on screen 6.4-6.8% of that time vs 4.3%); over all such time it is 15-16% vs 17%
  (11-12% before "Encounters").
- **Burst length:** bursts begun in sight are 5 rounds vs 6 (p90 14 vs 13); all bursts 4 vs 5. The
  burst dial works (per bot, realised against drawn, correlation 0.84 on seeds 201-208) but is
  relative: no offset makes the arena's pooled bot burst past 5.5-5.75 rounds.
- **The hidden view turns too much:** since "Steering with the view" (the view leads along the way) p50 16-19 deg/s vs
  9-10 and 58-61 turns of 10 deg or more a hidden minute vs 39 (54 before). Before it: p50 13-14 deg/s vs 8, p99 645-660 vs 545 (p99
  815-860 from the turn to sounds behind, see "What the owner saw", until the bots heard which side footsteps come from); before it p50 11.5-11.8 deg/s (16-17
  before "Travel and the hidden view", 20 before "Movement on the maps"; in the arena with corners 10.7),
  p99 666-677 vs 545, 54-55 turns of 10 deg or more per hidden minute vs 46, of them 10-11 of 90 deg or
  more vs 4.6 (the remaining big ones: the turn to a route behind the view, corners coming up, look
  decisions and look-arounds; 30-90 deg turns are now fewer than people's, 17 vs 20). Passing corners
  more readily brings the view to people's speed but costs the turn toward the enemy before a sighting
  (see "Travel and the hidden view"). The bots rest the mouse 61-62% of their standing hidden time vs 74%.
- **Ladders** are climbed as people climb them, with a watchdog (see above). People never took the
  dm/vents ladder in the recorded duels; the bots take it when the navmesh route goes up it.
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
  hand-set wall reflex lets go of a key (or adds a strafe to forward, or forward to a strafe, to run
  along the wall) before the bot hits a wall in the direction it goes, and vetoes pressing into one it
  touches.
- The fight-diagonal dial is a rate: it scales how readily a strafe gains the forward key and keeps
  it, not the choice between forward and back.
- The still gate of the view has separate fitted odds for a standing bot. A pre-aimed corner is held
  like a target (tracking gain, full own-motion compensation) in proportion to how soon the enemy is
  expected out of it, unless the bot's own motion sweeps it past faster than 90 deg/s
  (`preaim_pass_dps`): then the view keeps its direction.
- Right after losing sight the view holds where the enemy was last seen; a belief look watches the corner
  nearest the believed position's direction when the belief offers one (see "Corners and the lost enemy").
- The belief's mode (the position the goal, the chase and the belief look use) keeps its cell
  neighbourhood while that holds at least half the densest one's mass.
- compare.py weights the pooled bots' style families like the people's duel minutes
  (`minutes_share` in styles.json), not like the draw weights.
- Engage urgency 0.65 acts only for the second after losing sight: in sight the fitted keys and the style
  dials move the bot. The route coupling, the engage urgency, the pause after a kill (0.5 s), the turn
  to a route behind the view (0.05 a tick), the route look's re-aim (60 deg), the hidden re-aim hazard
  (0.05) and the corner pass rate (90 deg/s) were set by hand on practice-map captures (see
  "Encounters", "Movement on the maps" and "Travel and the hidden view").
- Footsteps are heard on the side they come from (`front_back_confusion` 0; the plan's 0.25 made the bots spin round
  to enemies in front, which people do not), turned to from 48 deg off the view (`sound_turn_deg`), and a held view
  direction gives way once the enemy is believed out of view of it (`hold_belief_deg`); all three set on practice-map
  captures (see "Where the bot looks when first seen").
- The hidden look policy, the hidden view's noise (0.125, last looped 2026-10-02), the sound precision
  and the pitch gain were set by hand (see the `calibrate.py` docstring). So was the corner pre-aim's part of it (`preaim_*`), on real-map
  captures. The pooled loops and every dial sweep except `hold_angle`'s run without corners
  (`hb_arena --no-corners`).
- The lean's wall terms (three times the fitted wall logit, edge 3.74) and the turn to a sound behind (`sound_turn_*`:
  every such sound, after 150 ms, held 3 s, onto a corner within 45 deg) were set by hand on practice-map captures
  (see "What the owner saw"). The early reload and the trigger's rounds-left term replace hand-set values with fits.
- The style dial `counter_strafe` (a strafe the tick after one is let go) shifts the fitted hazard at that tick; its
  curve is swept like the others.
- A hit from an enemy not in sight turns the view toward where it is felt, from any angle (before 2026-10-04 only
  hits felt more than 60 deg off: a hand-set gate). Maps without a recorded prior get coarser cells when they would
  have more than 3000 (hand-set: the visibility table's size and build time).
- A key changes for the route when that makes its chord go the route's way better by more than 0.1 (the cosine;
  `nav_deadband`, hand-set on practice-map captures, 0.3 before 2026-10-05), and the walls push the route at half
  strength (`g_humanbot_wall_steer` 0.5, 1 before): see "Back and forth with the enemy hidden".
- With the enemy hidden the route pull grows with its believed distance (x1 within 500 u, x3 from 900 u: `nav_far_*`,
  hand-set on all six maps, see "The practice areas of the objective maps") and gives way while the bot presses into a
  wall. A bot travelling with its route into a wall it touches, that has held no key for a second, takes the open
  chord nearest the route (`UNSTICK_*`, hand-set). The keys' change rates with the enemy hidden have a fitted distance
  term (`hid_dist_logit`).
- A person needs 10 duel minutes and a capture row 5 to stand for a style (the data repository's rule); shorter games
  count only in the pooled values. The priors of dm/brownffa and dm/flag need two player-sessions per cell or spawn
  (two people, or one person in two sessions) instead of two sessions: each map is one or two recorded sessions.
- In sight the press hazard's clock restarts at 200 ms when the crosshair first reaches the enemy late in a sighting
  (`aim_arrive_hw`, hand-set threshold of 2 body half-widths); the fit's clock runs from the parts on screen. In a
  fight with under a quarter of the clip left the bot makes for cover (`low_clip_cover`, hand-set at people's step in
  leaving the enemy's sight). See "Late aim and low clips".
- Travelling with no enemy expected soon the view leads along the way (`travel_*` in the view tuning) and the keys follow
  the way three times as firmly (`travel_pull`); a closed door across the way is looked at so the use key opens it. All
  hand-set on captures of dm/brownffa and dm/flag (see "Steering with the view").
- The style dials `lean_switch` and `lean_drop` shift the lean chain's switch and let-go while the strafe is against
  the lean (the habit does not act there). Since 2026-10-04 only the dials a change touches are re-swept; the others
  keep their curves (a re-sweep redraws the arena's noise).
- One style dial is inert: `hold_angle` (its arena sweep spans under a third of the human range).
  The burst dial counts the bursts begun in sight and is relative to the pooled bot, like the
  skills. The out-of-ammunition fallback (pistol bash), the ladder climb and its watchdog, and the
  anticipation boost of the hidden press (`anticipation_logit` 5) are set by hand.
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
