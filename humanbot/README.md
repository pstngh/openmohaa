# Human-imitation bots

The deathmatch bots of this fork do not use the stock bot's decision code. Every bot runs one
data-fitted, stochastic brain learned from six recorded players (346 player-minutes of 1v1 SMG
duels). It moves, aims, fires and looks the way those players did. Each bot draws a style
family (presser, strafer or stopper), then its own dials and skill within that family. There is
no neural net: the brain is small semi-Markov processes, controllers and hazards fitted on the
recordings, plus a particle filter for where the enemy might be. The stock code still handles
ladders, doors and getting unstuck, for a moment each time.

The owner's test procedure is in [TESTING.md](TESTING.md).

## How a bot thinks

One 50 ms server frame, for every bot (`code/fgame/humanbot_adapter.cpp`):

1. **Perception.** The adapter hands the brain only what a player could know (`humanbot_perception.cpp`):
   - which body parts are inside a 16:9 field of view (fov 80 at 4:3, Hor+) and not occluded;
   - the sounds of the last frame (footsteps, gunfire, reloads), with human-like direction and
     distance noise;
   - the direction damage came from;
   - the map: routes, visibility, where people tend to be.

   The brain never sees an enemy's hidden position.
2. **Brain** (`code/humanbot`, engine-independent):
   - `hb_perception_model`: detection by eccentricity, distance and visible parts.
   - `hb_belief`: a particle filter over each enemy's position, driven by sights, sounds and
     looking where the enemy is not.
   - `hb_trigger`: press and release hazards by aim error, time since sight and hold age.
   - `hb_view`: tracking controllers, main-sequence flicks, a still gate and the look policy
     while the enemy is hidden.
   - `hb_movement`: the side key and the forward key as two coupled semi-Markov processes;
     lean, and crouch/jump/walk.
   - `hb_nav`: where to go (hunt, hold, cover); `hb_weapon`: reloads and weapon switches.
   - `hb_style`: the dials of this bot.
3. **Usercmds.** `hb_substep` turns the tick's decision into 4 usercmds (`g_humanbot_substeps`).
   Keys are digital, key changes land on one sub-step, the view moves along the flick's
   minimum-jerk profile, and eye info goes out like a client's. The server runs them through the
   normal `Player::ClientThink`: the bot moves with exactly the player physics.

All bots decide on the same snapshot of the world (`PrepareThink`) before any of them moves
(`CommitThink`), like clients whose packets arrive together.

## Where things are

| Path | What |
|---|---|
| `code/humanbot/` | The brain library. Engine-independent C++, used by the game module and the tests. `hb_embedded.cpp` is the model, generated. |
| `code/fgame/humanbot_*.cpp` | Engine glue: adapter, perception traces, map world (priors, navmesh, visibility cache), eye, presentation, console commands. |
| `code/fgame/movement_telemetry*` | The telemetry logger (`g_movelog`, schema 13), including one `bot_*` column per brain diagnostic (`hb_diag.h`). |
| `code/tests/humanbot/` | Unit tests; `hb_replay`, the brain on recorded human traces; `hb_arena`, bots in closed loop on the real Pmove, with `arena_metrics.h` computing the evaluation's statistics. |
| `code/tests/pmove/` | `pm_harness`: the real `Pmove()` in a box world, as the server runs it. |
| `humanbot/fit/` | Fitting scripts. They need the private recordings; `run_fits.sh` runs them all. `calibrate.py` does the closed-loop calibration. |
| `humanbot/model/` | `shared.json`, the pooled model; `styles.json`, the anonymous style distribution; `calibration.json`, the dial curves; `tuning.json`, the calibrated pooled parameters. |
| `humanbot/maps/` | Map priors of the four practice maps (aggregate occupancy, routes, spawns). Other maps get a prior derived from the navmesh. |
| `humanbot/eval/` | Evaluation: `compare.py`, `metrics.py`, `load_bot_captures.py`, `human_reference.json` (pooled human statistics with CIs). |
| `humanbot/tools/` | `pack_capture.py`, bot-eval ZIPs; `check_no_raw_data.py`, the privacy guard; `embed_model.py`, which writes `hb_embedded.cpp`. |
| `humanbot/server/` | `duel.cfg`, `soak.cfg`, `names.txt`. |
| `humanbot/captures/` | Bot-eval captures (the owner's games against the bots). |

## Cvars and commands

| Cvar | Default | |
|---|---|---|
| `g_humanbot_substeps` | 4 | usercmds per 50 ms frame (1-8) |
| `g_humanbot_fov` / `g_humanbot_aspect` | 80 / 1.778 | the bot's field of view (Hor+) |
| `g_humanbot_seed` | 0 | 0: new styles every run; any other value repeats them |
| `g_humanbot_families` | "" | family weights "presser strafer stopper"; "" is the recorded mix |
| `g_humanbot_disguise` | 0 | 1: names from `main/humanbot/names.txt` (else a built-in list), a realistic ping, listed as players (off: labelled bots) |
| `g_humanbot_model_dir` | "" | game-relative directory with `shared.json` / `styles.json` / `calibration.json` merge patches and `maps/<map>.json` |
| `g_humanbot_debug` | 0 | 1: print every hand-off between the brain and the stock code (ladder, door, stuck recovery) |

Commands:
- `addbotstyle <presser|strafer|stopper|random> [name]`
- `humanbot_list`: each bot's style and think time.
- `humanbot_reload`: reloads the model after a `g_humanbot_model_dir` edit.
- `humanbot_selftest [seconds]`: map status, then a timed bot game with pass/fail checks.

Bots never chat.

## Rebuilding the model

```sh
humanbot/fit/run_fits.sh          # needs ../openmohaa-movement (git lfs pull) and its analysis cache
humanbot/fit/calibrate.py --stage all
```

The fits read the recordings through the data repository's unchanged analysis code:

- `fit_keys.py`: the two movement keys and how a life starts;
- `fit_movement.py`: lean and stance;
- `fit_trigger.py`, `fit_view.py`, `fit_weapon.py`;
- `fit_maps.py`: priors;
- `fit_styles.py`: families and dials.

`assemble_model.py` merges the parts with `tuning.json` into `shared.json`. `embed_model.py`
then writes the embedded copy, which CI checks is in sync.

Some parameters have no direct counterpart in the recordings: the fitted controllers are
attenuated, the look policy and sound precision were never recorded, and the modules interact.
`calibrate.py` tunes them against human statistics:

- open loop, on the recorded traces (`hb_replay`): lean, strafe and forward habits per context;
- closed loop, in the arena (`hb_arena`, two pooled bots, fixed seeds): stillness, turn speeds,
  tracking gain, aim noise, aim heights, press hazard and hidden fire.

It then sweeps each style dial's internal offset in the arena and writes the monotone curves
from dial target to offset. The two skills (aim error, reaction) are relative to the pooled bot.

## Privacy

The fork holds only aggregates:

- the pooled model;
- an anonymous style distribution (family centres and spreads, no aliases);
- map priors;
- the pooled human reference;
- captures of the owner playing the bots.

Raw human captures, per-frame derivatives and per-person tables stay in the private data
repository. `check_no_raw_data.py` enforces this in CI.
