# Testing the human-imitation bots

This is the owner's procedure for the first live tests. It covers building a Linux dedicated
server, playing recorded 1v1 duels against the bots, running a 16-bot soak, and turning the
recordings into a comparison with the human duels. Everything the bots do has only been run
in the test harnesses so far (`code/tests/humanbot`, `code/tests/pmove`); the engine glue
(`code/fgame/humanbot_*.cpp`) runs for the first time on your server. Steps 2 and 3 are
there to catch that early.

## 1. Build

On Linux (the tests below are native, not for MSVC or cross builds):

```sh
sudo apt-get install -y cmake ninja-build clang lld flex bison libsdl2-dev libopenal-dev libcurl4-openssl-dev
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_CLIENT=OFF
cmake --build build
(cd build && ctest --output-on-failure)     # 7 tests, under a minute
cmake --install build --prefix /path/to/mohaa
```

`ctest` runs the brain's statistical unit tests, the Pmove sub-step test, the telemetry header
test and `test_hb_arena`: two bots play two minutes on the real player movement code and must
not get stuck, fight walls, break the keyboard contract, or think longer than 150 us per tick.

## 2. Smoke test (5 minutes)

Copy `humanbot/server/duel.cfg` and `humanbot/server/soak.cfg` into the game's `main/`
folder, then start the server with the duel configuration:

```sh
./omohaaded +set com_target_game 0 +exec duel.cfg
```

In the console, check:

1. `humanbot: model <12 hex digits> (embedded)` shows during startup. `no usable model; the
   stock bots are used` means the model failed to load; the reason is printed on the line
   above.
2. `humanbot_selftest 60` prints the map status:
   - navigation mesh `valid`;
   - map prior `recorded human prior, checksum ok` on the four practice maps (`derived from
     the navmesh` on any other map);
   - visibility `ready`. It is built in the background the first time a map loads and cached
     under `main/humanbot/vis/`.

   The test then runs a 60 s bot game. It ends with one line per bot (stuck bouts, wall
   pressure, keyboard violations, think time) and `humanbot_selftest: PASS`.
3. `humanbot_list` shows each bot's style family, seed and drawn dials.

Please send the console log if anything reads FAIL, MISSING or MISMATCH.

## 3. Duels against the bot (the main test)

`duel.cfg` records everything. It pins the settings that must match the human recordings:
`sv_fps 20`, free-for-all, normal physics (`sv_runspeed 250`, `sv_dmspeedmult 1`,
`sv_gravity 800`) and one bot. Telemetry goes to `main/telemetry/segments/`, one capture per
hour or 512 MB. Then:

1. Join the server and play the bot 1v1 on the practice maps (`map dm/crnodoors`,
   `map dm/main`, `map dm/vents`, `map dm/downladder`). Play with an SMG, as in the
   recordings. The bot joins axis or allies by its drawn weapon habit (MP40 or Thompson). About
   20-30 minutes per map is plenty.
2. Vary the opponent between sessions:
   - `g_humanbot_seed 0` (the default) draws a new style every run. Set a number (for example
     `g_humanbot_seed 7`) to meet the same styles again.
   - `g_humanbot_families "1 0 0"` gives only pressers; `"0 1 0"` strafers, `"0 0 1"` stoppers.
     The default is the recorded mix, about 50/33/17.
   - `addbotstyle strafer` adds one bot of a family (`kick` or `removebot` to drop one).
   - `g_humanbot_disguise 1` gives the bots names from `humanbot/names.txt` and a ping, and
     lists them as players. Use it for a blind test with a friend.
3. Watch for what feels wrong and note the time (the `session_ms` in the telemetry makes it
   easy to find again). Things the tests cannot judge: the bot's line through doors and
   ladders, how it gets unstuck (the stock bot code takes over for a moment), and whether it
   behaves sanely on other maps and game types.

## 4. Pack and push the captures

```sh
python3 -m pip install -r humanbot/eval/requirements.txt
python3 humanbot/tools/pack_capture.py /path/to/mohaa/main/telemetry --out humanbot/captures
(cd humanbot/captures && sha256sum -c SHA256SUMS)
git add humanbot/captures && git commit -m "Add bot-eval captures" && git push
```

`pack_capture.py` checks every file (schema 13, the build's columns, bot rows present) and
writes ZIPs named `DATE_pstN_bot-eval_schema13_<sha12>.zip` of at most 25 MiB each. Only your
own games against the bots belong here: never human-vs-human recordings, which stay in the
private data repository. `check_no_raw_data.py` (run in CI) refuses anything else.

## 5. Compare with the human duels

```sh
python3 humanbot/eval/compare.py humanbot/captures/<zip>...     # --movement-repo PATH if not ../openmohaa-movement
```

With the private `openmohaa-movement` checkout next to the fork, this runs its unchanged
analysis scripts on the bot captures. It writes `humanbot/eval/reports/<stem>/report.md` with:

- (a) the pooled bots against the pooled humans for every statistic, with 95% CIs;
- (b) each bot's dial recovery (its realised style against the style it drew);
- (c) the tells: the statistics whose bot and human CIs do not overlap, largest first.

Your own rows are reported separately (`pstN@vsbot`), never pooled with the bots.

## 6. Soak test (a few hours, unattended)

```sh
./omohaaded +set com_target_game 0 +exec soak.cfg
```

`soak.cfg` runs 16 bots and no humans on the stock DM maps, with a map change every
15 minutes. After a few hours, check that the server is still up. Then check
`humanbot_list` (think time per bot, including the maximum) and the console for errors.
These captures contain bots only: do not pack them with the duel captures.

## Known gaps (from the closed-loop arena, two pooled bots)

The model matches the human duels within 25% on 55% of 195 statistics. These are furthest off,
and what to look for in the live test:

- **Aim at the moment of a sighting.** The bots are 13.5 degrees off when the enemy appears;
  people are 5.2 degrees off. People mostly see the enemy where they already aim, because they
  peek into their own crosshair. The bots hear and track hidden enemies about as well as people,
  but do not yet peek. The first shot therefore comes later (250 ms after a clean sighting vs
  150), and fewer early shots hit.
- **Close-range tracking.** Firing at under 128 u, the bots' aim error is 22 degrees (people
  14). They also turn faster in fights (p99 650 deg/s vs 300).
- **Trigger far off target.** Bots keep firing with the crosshair more than 10 body
  half-widths off 11% of the time (people 4%).
- **Retreats at mid range.** Bots back off in fights at 100-300 u about twice as often as
  people.
- **Stillness while seeing the enemy without firing.** People stand still 31% of that time,
  bots 13%.
- **The arena is not a recorded map.** Its context mix differs (more hidden time, fewer
  fights), so statistics tied to the map (fight distances, time in fights) can only be judged
  on your captures.
