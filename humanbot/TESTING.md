# Testing the human-imitation bots

This is the owner's procedure for the first live tests. It covers building a Linux dedicated
server, playing recorded 1v1 duels against the bots, running a 16-bot soak, and turning the
recordings into a comparison with the human duels. Everything the bots do has only been run
in the test harnesses so far (`code/tests/humanbot`, `code/tests/pmove`); the engine glue
(`code/fgame/humanbot_*.cpp`) runs for the first time on your server. Steps 2 and 3 are
there to catch that early.

## Where to run it

On the Linux server (VPS) where you recorded the human duels, then play from your Mac as
usual. This keeps the practice maps, settings and machine the same as in the human recordings
the bots are compared with. It is also the platform CI builds and tests. Everything the bot
does happens on the server, so your ping changes only how the game feels to you, not the bot.
The 16-bot soak test can then run for hours without tying up your Mac. The recordings, the fork
and Python stay on your Mac: copy the telemetry over for steps 4 and 5.

A Mac-only setup also works if you no longer have the server. The fork builds on macOS (CI
does it), but you run the dedicated server and your client side by side on one machine, and
these steps are written for Linux.

## 1. Build (on the server)

```sh
sudo apt-get install -y git cmake ninja-build clang lld flex bison libcurl4-openssl-dev
git clone -b claude/funny-cray-e3yyih https://github.com/pstngh/openmohaa.git && cd openmohaa
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_CLIENT=OFF
cmake --build build
(cd build && ctest --output-on-failure)     # 8 tests, under a minute
```

Nothing needs installing. `build/RelWithDebInfo/omohaaded` loads the `game.so` next to it and
reads the game data from `fs_basepath`, so your current server install stays as it is, and you
can switch back by starting the old binary.

`ctest` runs the brain's statistical unit tests, the Pmove sub-step test and the telemetry
header test. It also runs two arena tests on the real player movement code:
- `test_hb_arena`: two bots play two minutes and must not get stuck, fight walls, break the
  keyboard contract, or think longer than 150 us per tick;
- `test_hb_arena_load`: 16 bots must stay within that budget.

## 2. Smoke test (5 minutes)

Copy `humanbot/server/duel.cfg` and `humanbot/server/soak.cfg` into the server's data folder
(`~/.local/share/openmohaa/main/`, or the game's `main/`). Then start the server with the
duel configuration, pointing `fs_basepath` at the MOHAA folder that holds `main/` with the
game's pak files and the practice maps:

```sh
build/RelWithDebInfo/omohaaded +set fs_basepath /path/to/mohaa +set com_target_game 0 +exec duel.cfg
```

The server listens on the usual port (12203 UDP); connect from your Mac as you did for the
recorded duels.

In the console, check:

1. `humanbot: model <12 hex digits> (embedded)` shows during startup. `no usable model; the
   stock bots are used` means the model failed to load; the reason is printed on the line
   above.
2. `humanbot_selftest 60` prints the map status:
   - navigation mesh `valid`;
   - map prior `recorded human prior, checksum ok` on the four practice maps and on dm/brownffa
     and dm/flag (`derived from the navmesh` on any other map);
   - visibility `ready`. It is built in the background the first time a map loads and cached
     under `~/.local/share/openmohaa/main/humanbot/vis/`.

   The test then runs a 60 s bot game. It ends with one line per bot (stuck bouts, wall
   pressure, keyboard violations, think time), the wall pressure over all bots and
   `humanbot_selftest: PASS`.
3. `humanbot_list` shows each bot's style family, seed and drawn dials.

Please send the console log if anything reads FAIL, MISSING or MISMATCH.

## 3. Duels against the bot (the main test)

`duel.cfg` records everything. It pins the settings that must match the human recordings:
`sv_fps 20`, free-for-all, normal physics (`sv_runspeed 250`, `sv_dmspeedmult 1`,
`sv_gravity 800`) and one bot. Telemetry goes to
`~/.local/share/openmohaa/main/telemetry/segments/` on the server. It records only while a
human is connected (`g_movelog_need_human`), so every visit is a capture of its own, and a
new segment starts every hour or 512 MB (`g_movelog_rollover`). Then:

1. Join the server and play the bot 1v1 on the practice maps (`map dm/crnodoors`,
   `map dm/main`, `map dm/vents`, `map dm/downladder`; also `map dm/brownffa` and `map dm/flag`,
   practice areas of the objective maps that have their own map prior since 2026-10-04). Play with an SMG, as in the
   recordings. The bot joins axis or allies by its drawn weapon habit (MP40 or Thompson). About
   20-30 minutes per map is plenty.
2. Vary the opponent between sessions:
   - `g_humanbot_seed 0` (the default) draws a new style every run. Set a number (for example
     `g_humanbot_seed 7`) to meet the same styles again.
   - `g_humanbot_families "1 0 0"` gives only pressers; `"0 1 0"` strafers, `"0 0 1"` stoppers.
     The default is the recorded mix, about 20/40/40.
   - `addbotstyle strafer` adds one bot of a family (`kick` or `removebot` to drop one).
   - `g_humanbot_disguise 1` gives the bots names and a ping, and lists them as players. Use it
     for a blind test with a friend. The names come from `main/humanbot/names.txt` (copy
     `humanbot/server/names.txt` there), or a built-in list.
3. Watch for what feels wrong and note the time (the `session_ms` in the telemetry makes it
   easy to find again). Things the tests cannot judge: the bot's line through doors and
   ladders (it climbs with the forward key and the view up or down the ladder, as people do),
   how it gets unstuck (the stock bot code takes over for a moment), and whether it behaves
   sanely on other maps and game types.

## 4. Pack and push the captures (on your Mac)

Copy the duel telemetry from the server, then pack it in your checkout of the fork:

```sh
scp -r you@server:.local/share/openmohaa/main/telemetry ./duel-telemetry
python3 -m pip install -r humanbot/eval/requirements.txt
python3 humanbot/tools/pack_capture.py ./duel-telemetry --out humanbot/captures
(cd humanbot/captures && shasum -a 256 -c SHA256SUMS)
git add humanbot/captures && git commit -m "Add bot-eval captures" && git push
```

Move or delete the server's `telemetry/` folder once it is packed, so the next session starts
clean (and the soak test's bot-only captures never mix in).

`pack_capture.py` checks every file (schema 13, the build's columns, bot rows present) and
writes ZIPs named `DATE_pstN_bot-eval_schema13_<sha12>.zip` of at most 25 MiB each. Only your
own games against the bots belong here: never human-vs-human recordings, which stay in the
private data repository. `check_no_raw_data.py` (run in CI) refuses anything else.

## 5. Compare with the human duels

```sh
python3 humanbot/eval/compare.py humanbot/captures/<zip>... --moh-dir /path/to/mohaa   # --movement-repo PATH if not ../openmohaa-movement
```

`--moh-dir` (or `$MOHAA_DIR`) is the MOHAA folder with the practice maps; it is only read. With it the
report also scores the pre-aim statistics (`perception.*`: the aim error at the first visible body part,
the reaction timed from it, and where the crosshair waits while the enemy is hidden, against the edge of
cover it comes out from) and the encounters (`perception.encounter.*`: how often a body part of the
enemy comes on screen, the hides between sightings, the time from both alive to the first sight, the
ground covered while the enemy is hidden), on the body parts the bots' own perception logged.

With the private `openmohaa-movement` checkout next to the fork, this runs its unchanged
analysis scripts on the bot captures. It writes `humanbot/eval/reports/<stem>/report.md` with:

- (a) the pooled bots against the pooled humans for every statistic, with 95% CIs;
- (b) each bot's dial recovery (its realised style against the style it drew);
- (c) the tells: the statistics whose bot and human CIs do not overlap, largest first.

Your own rows are reported separately (`pstN@vsbot`), never pooled with the bots. The pooled bots
are reweighted so their style families have the people's share of the recorded minutes (half of
them are the presser's), as the human numbers pool people over their minutes.

## 6. Soak test (a few hours, unattended, on the server)

```sh
build/RelWithDebInfo/omohaaded +set fs_basepath /path/to/mohaa +set com_target_game 0 +exec soak.cfg
```

`soak.cfg` runs 16 bots and no humans on the stock DM maps, with a map change every
15 minutes. After a few hours, check that the server is still up. Then check
`humanbot_list` (think time per bot, including the maximum) and the console for errors.
These captures contain bots only: do not pack them with the duel captures.

## Known gaps (from the closed-loop arena, two pooled bots)

The model matches the human duels within 25% on 51% of 195 statistics in the arena (on the practice
maps, bot against bot, 53-57% of all statistics). These are furthest off, and what to look for in the
live test:

- **Aim at the moment of a sighting.** People mostly see the enemy where they already aim: they
  wait on the edge of cover it comes out from. The bots now pre-aim that corner too, but pick
  the one the enemy comes out of less often, and since 2026-10-03 they let a corner they run
  past go by instead of following it round. Bot against bot on the practice maps they are 11.5-12
  degrees off at the first visible body part (people 5.2). The first shot therefore comes later
  (200-250 ms after a clean sighting vs 150), and fewer early shots hit.
- **Close-range tracking.** Firing at under 128 u, the bots' aim error is 19 degrees (people
  14). They also turn faster in fights (p99 620 deg/s vs 305).
- **Trigger far off target.** Bots keep firing with the crosshair more than 10 body
  half-widths off 9% of the time (people 5%).
- **Encounters.** Since 2026-10-02 the bots go after the enemy: their keys follow their route,
  they chase for a second after losing sight, move on right after a kill, and expect a killed enemy
  back at the spawn the game picks. Bot against bot on the practice maps they now see each other
  29-31 times a minute (people 33) and have the enemy on screen a third of the time, as
  people do. They also run corridors on the forward diagonal as people do and slide along walls
  instead of stopping at them (touching a wall as often as people); since 2026-10-03 a strafe that
  meets a wall turns into the forward diagonal along it, as people's does. Still short: they cover
  less ground while the enemy is hidden (about nine tenths of people's), and their view turns about
  1.4 times as fast as people's while the enemy is hidden. Tell us if they look like they patrol,
  stop at walls, or turn their view too much.
- **Backing off in fights at close range.** Bots back off at 150-300 u about twice as often as
  people: their crosshair trails the enemy, so a sidestep also moves them away from it.
- **Stillness while seeing the enemy without firing.** People stand still 30% of that time,
  bots 8-11%. (Bots that stood frozen in front of an enemy for minutes had run out of
  ammunition; since 2026-10-02 they fire into cover like people and rarely run dry. One that
  does closes in and hits with the pistol butt.)
- **Ladders.** Since 2026-10-02 a bot that stalls on a ladder (another bot in the way) turns
  back, and jumps off if it stalls again. Tell us if a bot still hangs on one.
- **The arena is not a recorded map.** Its context mix differs (more hidden time, fewer
  fights), so statistics tied to the map (fight distances, time in fights) can only be judged
  on your captures.
