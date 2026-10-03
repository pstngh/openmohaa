"""Closed-loop calibration of the pooled model (tuning.json) and of the style dial curves
(calibration.json).

Some parameters cannot be read off the recordings: the fitted controllers are attenuated
(people's intended aim point varies, the regressions see that as error), the look policy and
the sound precision have no recorded counterpart, and couplings between the modules only show
when the brain closes the loop. calibrate.py sets them by matching statistics of the humans:

  * open loop, hb_replay on the recorded traces (lean shares by context; the hidden view);
  * closed loop, hb_arena: two pooled bots on the real player movement, fixed seeds, the
    statistics of humanbot/eval/metrics.py under the keys of human_reference.json.

Each loop ties one parameter to one statistic with a monotone relation and takes damped
steps (additive, multiplicative or on the logit scale) until the bot matches the human value;
all loops step together from the same runs. Values in tuning.json that no loop owns are kept:
the hidden look policy (shares, dwell, re-aim), the hidden view's noise (see pooled_loops), the
sound precision and the pitch gain were set by hand from arena and replay runs (the error at a
sighting and the hidden view barely respond to them one at a time). The corner pre-aim's part
of that policy (preaim_*: how strongly a look decision favours a corner coming up, how readily a
look breaks off for one, the horizon, the offsets onto the cover side and below) was set by hand
from bot captures on the practice maps, scored with compare.py: corners are geometry, and the
arena's pillars are not the recorded maps.
The arena runs of the pooled loops and of every dial sweep but hold_angle's therefore give the
brains no geometry (hb_arena --no-corners: exposures without corners); on its pillars the corners
would turn the hidden view more than on the recorded maps, and the loops of the hidden view's
noise and stillness would make up for it. The uniform release shift stays 0 because it cannot hold fire on
target without also spraying far off it: the release is tilted instead, near and far from the
target and at taps, by three loops (the arena fires with the game's spread since 2026-09-28). The tap
tilt acts in sight and is matched on the holds begun in sight. Fire into cover is matched by the time
since a body part was last on screen: the overall hidden shift on the first second, a fade
(hidden_late_logit) on the later bins. The boost of the hidden press when an exposure is expected in the
crosshair (anticipation_logit 5, which the fade leaves out) was set by hand from bot captures on the
practice maps: in the arena a loop on the fire held before the first visible part ran it from 2 to its
bound 5 and moved that share 11% -> 13%, while on the recorded maps 5 brought the prefire after 2-5 s
hidden from 10% to 17% (people 26%). The couplings of assemble_model.py are set there by
hand; the wall reflex among them was set in the engine (the arena has too few walls to see it), and how
strongly the keys follow the route, the pull toward the enemy in a fight (engage_urgency), the pause after a
kill, the turn to a route behind the view, the route look's re-aim and the hidden re-aim hazard on practice-map
captures (in the arena the bots already cover as much ground as people: there the walls do not stop them). Stage "dials" sweeps each style dial's internal
offset in the arena (every bot pooled but for that offset), measures the realised dial
statistic (fit_styles.py definitions) and writes the monotone curve dial target -> offset. A
dial whose sweep spans less than a third of the human range is left inert, flat at its neutral
offset: hold_angle (2026-10-02; offsets -2..2 move the share parked on the appearance point
0.27-0.32, people 0.22-0.53. They do move the view onto the corner, but the parked share is
bounded by how often the belief picks the right corner). The burst dial counts the bursts begun in sight
and is relative to the pooled bot, like the skills (RELATIVE_DIALS).

Usage: calibrate.py [--stage pooled|dials|all] [--iters 8] [--seeds 1,2,3,4] [--seconds 900]
                    [--build DIR] [--replay-data DIR] [--dry-run]

Writes humanbot/model/tuning.json (a merge patch applied by assemble_model.py) and
humanbot/model/calibration.json, then reassembles and re-embeds the model. The per-iteration
log goes to humanbot/cache/calibration_log.json (git-ignored). Needs a build with hb_arena and
hb_replay (--build, default ../omh-build next to the fork) and the replay exports of
export_replay.py for the open-loop loops (skipped when missing).
"""
from __future__ import annotations

import argparse
import concurrent.futures as cf
import copy
import json
import math
import os
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

MODEL = H.HB_ROOT / "model"
REFERENCE = H.HB_ROOT / "eval" / "human_reference.json"
CONTEXTS = ["hidden_nofire", "hidden_fire", "los_nofire", "los_fire", "reload"]


def logit(p):
    p = min(max(p, 1e-4), 1 - 1e-4)
    return math.log(p / (1 - p))


def get_path(d, path):
    for k in path:
        d = d[k]
    return d


def set_path(d, path, v):
    for k in path[:-1]:
        if isinstance(k, int):
            d = d[k]
        else:
            d = d.setdefault(k, {})
    d[path[-1]] = v


@dataclass
class Loop:
    """One parameter tied to one statistic (or the mean of several: the step uses their mean ratio)."""
    name: str
    path: list                 # into shared.json; an int last element indexes an array
    kind: str                  # "add", "mult" or "logit" (how the parameter steps)
    source: str                # "arena" or "replay:<mode>"
    stats: list                # statistic keys (arena metrics or replay "bot"/"human" keys)
    lo: float
    hi: float
    init: float
    sign: int = 1              # +1: raising the parameter raises the statistic, -1: lowers it
    gain: float = 0.8
    stat_kind: str = "share"   # "share" (logit error), "ratio" (log error) or "diff" (difference)
    value: float = field(default=None)

    def step(self, bot, human, it=0):
        if self.stat_kind == "share":
            err = sum(logit(h) - logit(b) for b, h in zip(bot, human)) / len(bot)
        elif self.stat_kind == "ratio":
            err = sum(math.log(max(h, 1e-6) / max(b, 1e-6)) for b, h in zip(bot, human)) / len(bot)
        else:
            err = sum(h - b for b, h in zip(bot, human)) / len(bot)
        # damped, and more so as the loops settle (coupled loops otherwise keep overshooting)
        err *= self.sign * self.gain / (1.0 + 0.35 * it)
        v = self.value
        if self.kind == "mult":
            v *= math.exp(err)
        elif self.kind == "logit":
            v += err
        else:
            v += err
        self.value = min(max(v, self.lo), self.hi)
        return err


def pooled_loops(shared):
    L = []
    view = shared["view"]
    tun = view.get("tuning", {})
    still0 = tun.get("still_logit", [0.0] * 5)
    for i, c in enumerate(CONTEXTS):
        L.append(Loop(f"still_logit.{c}", ["view", "tuning", "still_logit", i], "logit", "arena", [f"view.mouse_still.{c}"],
                      -3.0, 3.0, still0[i]))
    # The hand's noise without a visible enemy (hidden_noise_scale) is no longer looped: it was matched on the median
    # hidden yaw speed of the arena without corners, which the look turns set more than the noise (+45% noise for
    # +0.9 deg/s). Once the bots stopped turning back to their route (2026-10-03), that view calmed by 0.5 deg/s and the
    # loop raised the noise from 0.125 to 0.18; on the recorded maps, where corners turn the view, it raised the hidden
    # yaw speed from 10.9-11.6 to 12-12.8 deg/s (people 8.1). It stays at its last looped value in tuning.json.
    L += [
        Loop("track_gain_scale", ["view", "tuning", "track_gain_scale"], "mult", "arena",
             ["aim.error_firing.0-128", "aim.error_firing.128-192", "aim.error_firing.192-256"], 0.5, 2.5,
             tun.get("track_gain_scale", 1.0), sign=-1, gain=0.6, stat_kind="ratio"),
        Loop("noise_scale", ["view", "tuning", "noise_scale"], "mult", "arena",
             ["aim.error_firing.512-768", "aim.error_firing.768-1200"], 0.2, 2.0, tun.get("noise_scale", 1.0),
             gain=0.6, stat_kind="ratio"),
        Loop("aim_height.firing", ["view", "aim_height", "firing"], "add", "arena", ["aim.height.firing"], 0.25, 0.6,
             view["aim_height"]["firing"], stat_kind="diff"),
        Loop("aim_height.idle", ["view", "aim_height", "idle"], "add", "arena", ["aim.height.idle"], 0.45, 0.8,
             view["aim_height"]["idle"], stat_kind="diff"),
        # trigger: the press with the enemy in sight, and fire without sight. "Without sight" is the trigger's
        # own: no body part visible (people's fire at a partly visible enemy is fire in sight; on the centroid
        # ray it counted as hidden, and the loop made the bots fire at enemies they could not see instead).
        # Fire into cover is matched by the time since a part was last on screen: in the first second by the
        # overall shift, later by the fade (people lose track of the enemy; the bots spend more of their time
        # long after sight, so matching the overall share would make them fire into cover too early)
        Loop("press_los_logit", ["trigger", "tuning", "press_los_logit"], "add", "arena",
             ["trigger.press_los_near.0", "trigger.press_los_near.50-100", "trigger.press_los_near.150-250"], -3.0, 3.0,
             shared["trigger"].get("tuning", {}).get("press_los_logit", 0.0)),
        Loop("hidden_fire_logit", ["trigger", "tuning", "hidden_fire_logit"], "add", "arena",
             ["perception.fire_held_no_part.0-500ms", "perception.fire_held_no_part.500-1000ms"], -3.0, 3.0,
             shared["trigger"].get("tuning", {}).get("hidden_fire_logit", 0.0)),
        Loop("hidden_late_logit", ["trigger", "tuning", "hidden_late_logit"], "add", "arena",
             ["perception.fire_held_no_part.1000-2000ms", "perception.fire_held_no_part.2000-5000ms",
              "perception.fire_held_no_part.gt5000ms"], -4.0, 1.0,
             shared["trigger"].get("tuning", {}).get("hidden_late_logit", 0.0)),
        # the release with the enemy in sight, tilted near and far from the target, and the taps (the arena
        # has the game's spread: a spray far off the target also widens the rounds that follow). The tap tilt
        # acts in sight only, so it is matched on the holds begun in sight: on all holds it made up for the
        # bots' fewer holds begun in cover (people tap 59% of those) with too many taps in sight
        Loop("release_near_logit", ["trigger", "tuning", "release_near_logit"], "add", "arena",
             ["trigger.release_los.0-1", "trigger.release_los.1-2", "trigger.release_los.2-3", "trigger.release_los.3-4",
              "trigger.release_los.4-6"], -3.0, 3.0, shared["trigger"].get("tuning", {}).get("release_near_logit", 0.0)),
        Loop("release_far_logit", ["trigger", "tuning", "release_far_logit"], "add", "arena",
             ["trigger.release_los.6-10", "trigger.release_los.10-1000"], -3.0, 3.0,
             shared["trigger"].get("tuning", {}).get("release_far_logit", 0.0)),
        Loop("release_tap_logit", ["trigger", "tuning", "release_tap_logit"], "add", "arena", ["perception.tap_share.in_sight"],
             -3.0, 3.0, shared["trigger"].get("tuning", {}).get("release_tap_logit", 0.0)),
    ]
    mv = shared["movement"]
    lean0 = mv["lean"].get("ctx_logit", [0.0] * 5)
    habit = mv.get("habit", {})
    side0 = habit.get("side_ctx_logit", [0.0] * 5)
    fwd0 = habit.get("fwd_ctx_logit", [0.0] * 5)
    for i, c in enumerate(CONTEXTS):
        L.append(Loop(f"lean.ctx_logit.{c}", ["movement", "lean", "ctx_logit", i], "logit", "replay:movement",
                      [f"lean_share_by_ctx[{i}]"], -3.0, 3.0, lean0[i]))
        L.append(Loop(f"habit.side.{c}", ["movement", "habit", "side_ctx_logit", i], "logit", "replay:movement",
                      [f"side_share[{i}]"], -3.0, 3.0, side0[i], gain=0.6))
        L.append(Loop(f"habit.fwd.{c}", ["movement", "habit", "fwd_ctx_logit", i], "logit", "replay:movement",
                      [f"fwd_share_by_ctx[{i}]"], -3.0, 3.0, fwd0[i], gain=0.6))
    L.append(Loop("habit.reverse", ["movement", "habit", "reverse_logit"], "logit", "replay:movement", ["reverse_share"],
                  -3.0, 3.0, habit.get("reverse_logit", 0.0)))
    L.append(Loop("habit.walk", ["movement", "habit", "walk_mult"], "mult", "replay:movement", ["walk_hidden"],
                  0.2, 10.0, habit.get("walk_mult", 1.0)))
    return L


class Runner:
    def __init__(self, build: Path, replay_data: Path | None, seeds, seconds, workdir: Path):
        self.arena = build / "hb_arena"
        self.replay = build / "hb_replay"
        self.replay_data = replay_data
        self.seeds = seeds
        self.seconds = seconds
        self.work = workdir
        self.ref = {k: v["value"] for k, v in json.loads(REFERENCE.read_text())["metrics"].items()}

    def arena_metrics(self, patch: dict, extra=(), corners=False):
        f = self.work / "patch.json"
        f.write_text(json.dumps(patch))

        def one(seed):
            cmd = [str(self.arena), "--seconds", str(self.seconds), "--bots", "2", "--pooled", "--seed", str(seed),
                   "--quiet", "--override", str(f), *extra, *(() if corners else ("--no-corners",))]
            out = subprocess.run(cmd, capture_output=True, text=True, check=False).stdout
            return json.loads(out.splitlines()[0])["metrics"]

        with cf.ThreadPoolExecutor(max(1, min(len(self.seeds), os.cpu_count() or 1))) as ex:
            runs = list(ex.map(one, self.seeds))
        keys = set().union(*[r.keys() for r in runs])
        out = {}
        for k in keys:
            v = [r[k] for r in runs if k in r and r[k] != -1 and math.isfinite(r[k])]
            out[k] = sum(v) / len(v) if v else float("nan")
        return out

    def replay_stats(self, mode: str, patch: dict, stride=2):
        if not self.replay_data:
            return None, None
        f = self.work / "patch.json"
        f.write_text(json.dumps(patch))
        out = self.work / f"replay_{mode}.json"
        subprocess.run([str(self.replay), "--mode", mode, "--data", str(self.replay_data), "--override", str(f),
                        "--stride", str(stride), "--out", str(out)], capture_output=True, text=True, check=True)
        j = json.loads(out.read_text())
        return j["bot"], j["human"]


def stat_of(d, key):
    if key.startswith("side_share["):
        # share of ticks with the side key pressed, from the chord shares (side = chord % 3 - 1)
        c = int(key[len("side_share["):-1])
        ch = d["chord_share_by_ctx"][str(c)]
        return sum(v for i, v in enumerate(ch) if i % 3 != 1)
    if "[" in key:
        k, i = key[:-1].split("[")
        return d[k][int(i)]
    return d[key]


def calibrate_pooled(runner: Runner, iters: int, log: list, only=()):
    shared = json.loads((MODEL / "shared.json").read_text())
    tuning = json.loads((MODEL / "tuning.json").read_text()) if (MODEL / "tuning.json").exists() else {}
    loops = pooled_loops(shared)
    if only:
        loops = [lp for lp in loops if any(lp.name.startswith(o) for o in only)]
    for lp in loops:
        lp.value = lp.init
    # arrays are written whole (a merge patch replaces arrays)
    habit = shared["movement"].get("habit", {})
    arrays = {("view", "tuning", "still_logit"): list(shared["view"].get("tuning", {}).get("still_logit", [0.0] * 5)),
              ("movement", "lean", "ctx_logit"): list(shared["movement"]["lean"].get("ctx_logit", [0.0] * 5)),
              ("movement", "habit", "side_ctx_logit"): list(habit.get("side_ctx_logit", [0.0] * 5)),
              ("movement", "habit", "fwd_ctx_logit"): list(habit.get("fwd_ctx_logit", [0.0] * 5))}

    def patch_now():
        p = copy.deepcopy(tuning)
        for lp in loops:
            if isinstance(lp.path[-1], int):
                arrays[tuple(lp.path[:-1])][lp.path[-1]] = round(lp.value, 4)
            else:
                set_path(p, lp.path, round(lp.value, 4))
        for path, arr in arrays.items():
            set_path(p, list(path), list(arr))
        return p

    need_arena = any(lp.source == "arena" for lp in loops)
    for it in range(iters):
        p = patch_now()
        A = runner.arena_metrics(p) if need_arena else {}
        rep = {}
        for mode in sorted({lp.source.split(":")[1] for lp in loops if lp.source.startswith("replay:")}):
            rep[mode] = runner.replay_stats(mode, p)
        entry = {"iter": it, "loops": {}}
        for lp in loops:
            if lp.source == "arena":
                bot = [A.get(k, float("nan")) for k in lp.stats]
                hum = [runner.ref[k] for k in lp.stats]
            else:
                b, h = rep[lp.source.split(":")[1]]
                if b is None:
                    continue
                bot = [stat_of(b, k) for k in lp.stats]
                hum = [stat_of(h, k) for k in lp.stats]
            if any(not math.isfinite(x) for x in bot):
                continue
            before = lp.value
            lp.step(bot, hum, it)
            entry["loops"][lp.name] = {"value": round(before, 4), "next": round(lp.value, 4),
                                       "bot": [round(x, 4) for x in bot], "human": [round(x, 4) for x in hum]}
        log.append(entry)
        worst = sorted(entry["loops"].items(),
                       key=lambda kv: -max(abs(b - h) / max(abs(h), 1e-3) for b, h in zip(kv[1]["bot"], kv[1]["human"])))
        print(f"iter {it}: " + "; ".join(f"{k} {v['value']}->{v['next']} bot {v['bot']} human {v['human']}"
                                          for k, v in worst[:6]), flush=True)
    return patch_now()


# style dial -> (hb_arena --offset name, realised statistic, offset grid); fit_styles.py definitions
DIAL_SWEEPS = {
    "fwd_diag_fight": ("diag_logit", "dial.fwd_diag_fight", [-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0]),
    "reverse_share": ("reverse_logit", "dial.reverse_share", [-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0]),
    "side_hold_ms": ("hold_scale", "dial.side_hold_ms", [0.5, 0.7, 0.85, 1.0, 1.2, 1.5, 2.0]),
    "lean_fight": ("lean_logit", "dial.lean_fight", [-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0]),
    "jumps_per_min": ("jump_mult", "dial.jumps_per_min", [0.1, 0.3, 0.6, 1.0, 2.0, 4.0, 8.0]),
    "crouch_per_min": ("crouch_mult", "dial.crouch_per_min", [0.1, 0.3, 0.6, 1.0, 2.0, 4.0, 8.0]),
    "walk_hidden": ("walk_mult", "dial.walk_hidden", [0.0, 0.3, 0.6, 1.0, 2.0, 4.0, 8.0]),
    "burst_median": ("release_logit", "dial.burst_median", [-3.0, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5]),
    "aim_height_firing": ("aim_height_firing", "dial.aim_height_firing", [0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6]),
    "hold_angle": ("hold_logit", "dial.hold_angle", [-2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0]),
}
SKILL_NEUTRAL = {"aim_error_fight_deg": 1.0, "reaction_ms": 0.0}   # the pooled bot's offsets
# Style dials that are relative to the pooled bot, like the skills (their neutral offsets). The burst length counts
# the bursts begun in sight, and such a burst is cut where sight ends: the arena's sightings are shorter than on the
# recorded maps, so no release offset gets its pooled bot past ~5.3 rounds (people 4-8). A bot drawn x% above the
# average person bursts x% longer than the pooled bot.
RELATIVE_DIALS = {"burst_median": 0.0}
SKILL_SWEEPS = {
    "aim_error_fight_deg": ("noise_scale", "skill.aim_error_fight_deg", [0.4, 0.6, 0.8, 1.0, 1.3, 1.7, 2.2]),
    "reaction_ms": ("reaction_logit", "skill.reaction_ms", [-1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 3.0]),
}


def isotonic(y, increasing=True):
    """Pool-adjacent-violators fit of a monotone sequence (equal weights)."""
    y = list(y) if increasing else [-v for v in y]
    blocks = [[v, 1] for v in y]
    i = 0
    while i < len(blocks) - 1:
        if blocks[i][0] > blocks[i + 1][0]:
            v = (blocks[i][0] * blocks[i][1] + blocks[i + 1][0] * blocks[i + 1][1]) / (blocks[i][1] + blocks[i + 1][1])
            blocks[i] = [v, blocks[i][1] + blocks[i + 1][1]]
            del blocks[i + 1]
            i = max(i - 1, 0)
        else:
            i += 1
    out = []
    for v, n in blocks:
        out += [v] * n
    return out if increasing else [-v for v in out]


# the offset of a bot that does not shift the pooled model (logits 0, multipliers 1)
NEUTRAL_OFFSET = {"diag_logit": 0.0, "reverse_logit": 0.0, "hold_scale": 1.0, "lean_logit": 0.0, "jump_mult": 1.0,
                  "crouch_mult": 1.0, "walk_mult": 1.0, "release_logit": 0.0, "hold_logit": 0.0, "noise_scale": 1.0,
                  "reaction_logit": 0.0}
MIN_REACH = 1 / 3   # a sweep must span this share of the human range to be inverted


def dial_curve(offsets, stats, lo, hi, n=7, scale=None, neutral=None):
    """Monotone curve dial target -> offset from a sweep: isotonic fit of the statistic over the offset
    grid, inverted on n targets spanning the human range [lo, hi] (clamped to what the sweep reached).
    With scale=(neutral offset, human pooled value) the target is relative: a dial x asks for the
    statistic the bot shows at the neutral offset times x / pooled. A sweep that spans less than
    MIN_REACH of the targets' range cannot tell the offsets apart: inverting its noise would send the
    bots to the grid's ends, so with a neutral offset the curve is flat there (the dial is inert: every
    bot behaves like the pooled bot on it)."""
    pts = [(o, s) for o, s in zip(offsets, stats) if math.isfinite(s)]
    if len(pts) < 3:
        return None
    o = [p[0] for p in pts]
    s = [p[1] for p in pts]
    n_up = sum(1 for a, b in zip(s, s[1:]) if b > a) - sum(1 for a, b in zip(s, s[1:]) if b < a)
    inc = n_up >= 0
    fit = isotonic(s, inc)
    # strictly monotone for the inversion: nudge ties
    for k in range(1, len(fit)):
        if inc and fit[k] <= fit[k - 1]:
            fit[k] = fit[k - 1] + 1e-6
        if not inc and fit[k] >= fit[k - 1]:
            fit[k] = fit[k - 1] - 1e-6
    xs = [lo + (hi - lo) * k / (n - 1) for k in range(n)]
    target = lambda x: x  # noqa: E731
    if scale is not None:
        neutral, pooled = scale
        k0 = min(range(len(o)), key=lambda k: abs(o[k] - neutral))
        target = lambda x: fit[k0] * x / pooled  # noqa: E731

    def invert(x):
        f, oo = (fit, o) if inc else (fit[::-1], o[::-1])
        if x <= f[0]:
            return oo[0]
        if x >= f[-1]:
            return oo[-1]
        j = next(k for k in range(1, len(f)) if f[k] >= x)
        t = (x - f[j - 1]) / (f[j] - f[j - 1])
        return oo[j - 1] + t * (oo[j] - oo[j - 1])

    reach = (max(fit) - min(fit)) / max(abs(target(hi) - target(lo)), 1e-12)
    sweep = {"offset": o, "realised": [round(v, 4) for v in s], "reach": round(reach, 3)}
    if reach < MIN_REACH and neutral is not None:
        return {"x": [round(lo, 4), round(hi, 4)], "y": [neutral, neutral], "sweep": sweep, "flat": True}
    return {"x": [round(v, 4) for v in xs], "y": [round(invert(target(x)), 4) for x in xs], "sweep": sweep}


def calibrate_dials(runner: Runner, log: list, only=()):
    styles = json.loads((MODEL / "styles.json").read_text())
    tuning = json.loads((MODEL / "tuning.json").read_text()) if (MODEL / "tuning.json").exists() else {}
    cal = json.loads((MODEL / "calibration.json").read_text())
    for group, sweeps in (("dial", DIAL_SWEEPS), ("skill", SKILL_SWEEPS)):
        for name, (offset, stat, grid) in sweeps.items():
            if only and name not in only:
                continue
            realised = []
            for v in grid:
                # holding or clearing an angle is about corners: its sweep is the one run with the arena's geometry
                m = runner.arena_metrics(tuning, extra=("--offset", f"{offset}={v}"), corners=name == "hold_angle")
                realised.append(m.get(stat, float("nan")))
            # skills are relative to the average: the arena's fights are longer and its sightings less
            # pre-aimed than on the recorded maps, so absolute aim error and reaction targets would push
            # every bot to an extreme; a bot drawn x% faster than the average human reacts x% faster
            # than the pooled bot
            scale = ((SKILL_NEUTRAL[name], styles["pooled"][name]) if group == "skill"
                     else (RELATIVE_DIALS[name], styles["pooled"][name]) if name in RELATIVE_DIALS else None)
            c = dial_curve(grid, realised, styles["min"][name], styles["max"][name], scale=scale,
                           neutral=NEUTRAL_OFFSET.get(offset))
            log.append({"dial": name, "offset": offset, "grid": grid, "realised": realised, "curve": c})
            print(f"{group} {name}: {offset} {grid} -> {[round(r, 3) for r in realised]}", flush=True)
            if c and c["sweep"]["reach"] < MIN_REACH:
                print(f"  {name}: the sweep spans {c['sweep']['reach']:.0%} of the human range: "
                      + (f"inert (flat at the neutral {offset} {c['y'][0]})" if c.get("flat") else "inverted anyway (no neutral offset)"),
                      flush=True)
            if c:
                cal[group][name] = {"x": c["x"], "y": c["y"]}
    cal["about"] = ("Monotone curves from a style dial target (the human-measured statistic, fit_styles.py "
                    "definitions) to the internal offset that realises it in closed loop. Written by calibrate.py.")
    return cal


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--stage", default="pooled", choices=["pooled", "dials", "all"])
    ap.add_argument("--iters", type=int, default=8)
    ap.add_argument("--seeds", default="1,2,3,4")
    ap.add_argument("--seconds", type=int, default=900)
    ap.add_argument("--build", default=str(H.HB_ROOT.parent.parent / "omh-build"))
    ap.add_argument("--replay-data", default=str(H.CACHE / "replay"))
    ap.add_argument("--dry-run", action="store_true", help="do not write tuning.json / calibration.json")
    ap.add_argument("--only", default="", help="comma-separated loop name prefixes to run (default: all)")
    a = ap.parse_args(argv)
    build = Path(a.build)
    for exe in ("hb_arena", "hb_replay"):
        if not (build / exe).exists():
            sys.exit(f"calibrate: {build / exe} missing (build the fork with tests enabled)")
    rd = Path(a.replay_data)
    replay_data = rd if (rd / "dm_main.hbr").exists() else None
    if replay_data is None:
        print("calibrate: no replay exports, the open-loop loops are skipped")
    log = []
    with tempfile.TemporaryDirectory() as tmp:
        runner = Runner(build, replay_data, [int(s) for s in a.seeds.split(",")], a.seconds, Path(tmp))
        if a.stage in ("pooled", "all"):
            patch = calibrate_pooled(runner, a.iters, log, [x for x in a.only.split(",") if x])
            if not a.dry_run:
                (MODEL / "tuning.json").write_text(json.dumps(patch, indent=1, sort_keys=True) + "\n")
                print("wrote", MODEL / "tuning.json")
                subprocess.run([sys.executable, str(Path(__file__).with_name("assemble_model.py"))], check=True)
                subprocess.run([sys.executable, str(H.HB_ROOT / "tools" / "embed_model.py")], check=True)
        if a.stage in ("dials", "all"):
            cal = calibrate_dials(runner, log, [x for x in a.only.split(",") if x])
            if not a.dry_run:
                (MODEL / "calibration.json").write_text(json.dumps(cal, indent=1) + "\n")
                print("wrote", MODEL / "calibration.json")
                subprocess.run([sys.executable, str(H.HB_ROOT / "tools" / "embed_model.py")], check=True)
    H.CACHE.mkdir(parents=True, exist_ok=True)
    (H.CACHE / "calibration_log.json").write_text(json.dumps(log, indent=1))


if __name__ == "__main__":
    main()
