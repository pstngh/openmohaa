"""Pooled human velocity response to the movement keys (targets for the pmove harness).

Measured on the duel mask (`eligible`), with every next-tick value taken inside an
unbroken segment (same session, client and `seg`, consecutive 50 ms rows) before any
context filter. "Steady" context = on the ground, not ducked, BUTTON_RUN held, an SMG
(MP40 or Thompson: 240 u/s run speed) in hand.

  (a) strafe onset: |vel_right| at ticks 1..6 after `side` goes 0 -> +-1, with fwd == 0
      for the 2 ticks before, the player nearly still (speed_xy < 30 on the last of
      them) and the strafe key (alone) held through tick 6;
  (b) strafe reversal: ticks until vel_right takes the sign of the new key after `side`
      flips +-1 -> -+1 between two rows (fwd == 0, the old key held >= 3 ticks), both
      for every flip and for flips from a full-speed strafe (|vel_right| >= 180);
  (c) stop: ticks until speed_xy < 5 after every movement key is released from a
      full-speed strafe (|vel_right| >= 180, key held >= 3 ticks). Humans nearly always
      press a key again first, so this is censored: the Kaplan-Meier median is the
      target (its early events are mostly wall contacts; the median speed of the stops
      still released shows the pure friction decay); the median of the few completed
      stops is biased low;
  (d) forward run speed: median speed_xy with fwd == 1, side == 0 (raw, and "held" once
      that chord has been held >= 8 ticks).

Also: held speeds for walking (no BUTTON_RUN) and crouch-running (crouch key released or
held), and the jump: vertical speed on the take-off row (the think adds the impulse),
on the first airborne row, the first-row rise and the apex above the take-off height.

Tick convention: tick 1 is the first 50 ms row whose last usercmd carries the new key
state (the key changed somewhere inside that server frame); tick k is k - 1 rows later.

Prints the result and writes it to humanbot/cache/velocity_response.json (git-ignored).
The pooled medians are hard-coded in code/tests/pmove/test_pmove_substeps.cpp.

usage: fit_velocity_response.py [--features PATH] [--out PATH]
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

SMG = ["MP40", "Thompson"]
FULL_STRAFE = 180.0      # u/s; the held strafe speed with an SMG is 202.2
STILL = 30.0             # u/s
STOPPED = 5.0            # u/s
HELD_TICKS = 8           # "held" steady-state speeds
ONSET_TICKS = 6
REV_TICKS = 6
STOP_TICKS = 12
JUMP_SPEED = float(np.sqrt(2 * 800 * 56))   # "jump 56" at sv_gravity 800: 299.33 u/s
KEY = ["session_id", "client_id", "seg"]
COLS = ["session_id", "client_id", "session_ms", "seg", "eligible", "fwd", "side", "cmd_up", "vel_right",
        "speed_xy", "on_ground", "ducked", "run", "weapon", "velocity_z", "origin_z"]


def quantiles(x, qs=(0.1, 0.25, 0.5, 0.75, 0.9)):
    x = np.asarray(x, float)
    x = x[np.isfinite(x)]
    if not len(x):
        return {"n": 0}
    out = {"n": int(len(x))}
    for q in qs:
        out[f"q{int(round(q * 100)):02d}"] = float(np.quantile(x, q))
    return out


def load(path: Path) -> pd.DataFrame:
    F = pd.read_parquet(path, columns=COLS)
    F = F[F.seg >= 0]
    F = F.sort_values(["session_id", "client_id", "session_ms"], kind="stable").reset_index(drop=True)
    F["steady"] = (F.eligible & F.on_ground.eq(1) & ~F.ducked & F.run.eq(1) & F.weapon.isin(SMG))
    return F


class Shifter:
    """Column values k rows ahead inside the same unbroken segment (NaN outside it)."""

    def __init__(self, F: pd.DataFrame):
        self.F = F
        self.g = F.groupby(KEY, sort=False)
        self.cache = {}

    def __call__(self, col: str, k: int) -> np.ndarray:
        key = (col, k)
        if key not in self.cache:
            s = self.F[col] if k == 0 else self.g[col].shift(-k)
            self.cache[key] = s.to_numpy(dtype=float)
        return self.cache[key]


def held_age(F: pd.DataFrame, cond: pd.Series) -> np.ndarray:
    """Ticks `cond` has been true in a row within the segment (0 where false)."""
    c = cond.to_numpy(dtype=bool)
    first = F.groupby(KEY, sort=False).cumcount().to_numpy() == 0
    prev = np.r_[False, c[:-1]]
    streak = np.cumsum(c & (first | ~prev))
    age = pd.Series(c.astype(int)).groupby(streak).cumsum().to_numpy()
    return np.where(c, age, 0)


def strafe_onset(S: Shifter) -> dict:
    side, fwd, ok = (lambda k: S("side", k)), (lambda k: S("fwd", k)), (lambda k: S("steady", k))
    m = (side(-2) == 0) & (side(-1) == 0) & (fwd(-2) == 0) & (fwd(-1) == 0) & (S("speed_xy", -1) < STILL)
    m &= np.abs(side(0)) == 1
    for k in range(-2, ONSET_TICKS):
        m &= ok(k) == 1
    for k in range(ONSET_TICKS):
        m &= (side(k) == side(0)) & (fwd(k) == 0)
    lat = [np.abs(S("vel_right", k)[m]) for k in range(ONSET_TICKS)]
    return {
        "n": int(m.sum()),
        "lat_speed_median": [float(np.median(v)) for v in lat],
        "lat_speed_q25": [float(np.quantile(v, 0.25)) for v in lat],
        "lat_speed_q75": [float(np.quantile(v, 0.75)) for v in lat],
        "start_speed_median": float(np.median(S("speed_xy", -1)[m])),
    }


def strafe_reversal(S: Shifter, min_speed: float | None) -> dict:
    side, fwd, ok = (lambda k: S("side", k)), (lambda k: S("fwd", k)), (lambda k: S("steady", k))
    s0 = side(-1)
    m = (np.abs(s0) == 1) & (side(0) == -s0) & (fwd(0) == 0) & (ok(0) == 1)
    for k in range(-3, 0):
        m &= (side(k) == s0) & (fwd(k) == 0) & (ok(k) == 1)
    if min_speed is not None:
        m &= s0 * S("vel_right", -1) >= min_speed
    new = -s0
    ticks = np.full(len(m), np.nan)
    held = m.copy()      # the new key still held (alone, steady context) through tick k
    alive = m.copy()     # ... and vel_right has not changed sign yet
    profile = []
    for k in range(REV_TICKS + 2):
        still = (side(k) == new) & (fwd(k) == 0) & (ok(k) == 1)
        held &= still
        alive &= still
        lat_new = new * S("vel_right", k)
        if k < REV_TICKS:
            profile.append(float(np.median(lat_new[held])) if held.any() else None)
        hit = alive & (lat_new > 0)
        ticks[hit] = k + 1
        alive &= ~hit
    t = ticks[m]
    done = t[np.isfinite(t)]
    dist = {str(int(v)): int(c) for v, c in zip(*np.unique(done, return_counts=True))}
    return {
        "min_prev_lat_speed": min_speed,
        "n": int(m.sum()),
        "n_resolved": int(len(done)),
        "ticks_to_sign_change_median": float(np.median(done)) if len(done) else None,
        "ticks_to_sign_change_dist": dist,
        "prev_lat_speed_median": float(np.median((s0 * S("vel_right", -1))[m])),
        "lat_speed_new_dir_median": profile,
    }


def stop(S: Shifter) -> dict:
    side, fwd, ok = (lambda k: S("side", k)), (lambda k: S("fwd", k)), (lambda k: S("steady", k))
    s0 = side(-1)
    m = (np.abs(s0) == 1) & (fwd(-1) == 0) & (side(0) == 0) & (fwd(0) == 0)
    for k in range(-3, 0):
        m &= (side(k) == s0) & (fwd(k) == 0) & (ok(k) == 1)
    m &= np.abs(S("vel_right", -1)) >= FULL_STRAFE
    ticks = np.full(len(m), np.nan)
    alive = m.copy()
    at_risk, events, profile = [], [], []
    for k in range(STOP_TICKS):
        # still released, on the ground and on the duel mask
        alive &= (side(k) == 0) & (fwd(k) == 0) & (S("eligible", k) == 1) & (S("on_ground", k) == 1)
        sp = S("speed_xy", k)
        profile.append(float(np.median(sp[alive])) if alive.any() else None)
        at_risk.append(int(alive.sum()))
        hit = alive & (sp < STOPPED)
        events.append(int(hit.sum()))
        ticks[hit] = k + 1
        alive &= ~hit
    surv, km_median, curve = 1.0, None, []
    for k, (r, e) in enumerate(zip(at_risk, events)):
        if r:
            surv *= 1.0 - e / r
        curve.append(surv)
        if km_median is None and surv <= 0.5:
            km_median = k + 1
    done = ticks[m]
    done = done[np.isfinite(done)]
    return {
        "n": int(m.sum()),
        "n_resolved": int(len(done)),
        "ticks_to_stop_km_median": km_median,
        "ticks_to_stop_resolved_median": float(np.median(done)) if len(done) else None,
        "km_survival": curve,
        "at_risk": at_risk,
        "speed_median_while_released": profile,
        "prev_speed_median": float(np.median(S("speed_xy", -1)[m])),
    }


def held_speeds(F: pd.DataFrame) -> dict:
    base = F.eligible & F.on_ground.eq(1) & F.weapon.isin(SMG)
    fwd_only = F.fwd.eq(1) & F.side.eq(0)
    chords = {
        "run_forward": base & ~F.ducked & F.run.eq(1) & fwd_only,
        "run_strafe": base & ~F.ducked & F.run.eq(1) & F.fwd.eq(0) & F.side.ne(0) & F.cmd_up.eq(0),
        "run_back": base & ~F.ducked & F.run.eq(1) & F.fwd.eq(-1) & F.side.eq(0) & F.cmd_up.eq(0),
        "walk_forward": base & ~F.ducked & F.run.eq(0) & fwd_only & F.cmd_up.eq(0),
        "crouch_forward_key_released": base & F.ducked & F.run.eq(1) & fwd_only & F.cmd_up.eq(0),
        "crouch_forward_key_held": base & F.ducked & F.run.eq(1) & fwd_only & F.cmd_up.lt(0),
    }
    out = {}
    for name, cond in chords.items():
        age = held_age(F, cond)
        out[name] = {"all": quantiles(F.speed_xy[cond]), "held": quantiles(F.speed_xy[age >= HELD_TICKS]),
                     "mode": float(F.speed_xy[cond].round(2).mode().iloc[0]) if cond.any() else None}
    return out


def jump(S: Shifter, F: pd.DataFrame) -> dict:
    vz0 = S("velocity_z", 0)
    takeoff = (S("eligible", 0) == 1) & (S("on_ground", 0) == 1) & (np.abs(vz0 - JUMP_SPEED) < 0.05)
    first = takeoff & (S("on_ground", 1) == 0)
    z0 = S("origin_z", 0)
    apex = np.full(len(F), np.nan)
    land = np.full(len(F), np.nan)
    alive = first.copy()
    zmax = np.where(first, S("origin_z", 1), np.nan)
    for k in range(2, 60):
        og = S("on_ground", k)
        z = S("origin_z", k)
        landed = alive & (og == 1)
        apex[landed] = zmax[landed] - z0[landed]
        land[landed] = z[landed] - z0[landed]
        alive &= og == 0
        zmax = np.where(alive, np.fmax(zmax, z), zmax)
        if not alive.any():
            break
    flat = np.isfinite(land) & (np.abs(land) < 1)
    return {
        "n_takeoffs": int(takeoff.sum()),
        "takeoff_vz_median": float(np.median(vz0[takeoff])),
        "first_air_vz": quantiles(S("velocity_z", 1)[first]),
        "first_air_rise": quantiles((S("origin_z", 1) - z0)[first]),
        "apex_rise": quantiles(apex[np.isfinite(apex)]),
        "apex_rise_flat_landing": quantiles(apex[flat]),
        "share_apex_above_52": float(np.mean(apex[np.isfinite(apex)] > 52)) if np.isfinite(apex).any() else None,
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    ap.add_argument("--features", type=Path, default=None, help="features.parquet (default: the hbdata cache)")
    ap.add_argument("--out", type=Path, default=H.CACHE / "velocity_response.json")
    args = ap.parse_args()
    features = args.features or (H.analysis_cache() / "features.parquet")

    F = load(features)
    S = Shifter(F)
    res = {
        "source": {"features": str(features), "rows": int(len(F)), "eligible_rows": int(F.eligible.sum())},
        "context": "eligible, unbroken segments; steady = on ground, not ducked, BUTTON_RUN, MP40/Thompson",
        "tick_convention": "tick 1 = first 50 ms row whose last usercmd has the new key state",
        "strafe_onset": strafe_onset(S),
        "strafe_reversal_full_speed": strafe_reversal(S, FULL_STRAFE),
        "strafe_reversal_all": strafe_reversal(S, None),
        "stop_from_full_strafe": stop(S),
        "held_speeds": held_speeds(F),
        "jump": jump(S, F),
    }
    res["run_speed_forward_median"] = res["held_speeds"]["run_forward"]["all"].get("q50")
    res["run_speed_forward_held_median"] = res["held_speeds"]["run_forward"]["held"].get("q50")

    text = json.dumps(H.jsonable(res), indent=1)
    print(text)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(text + "\n")
    print(f"wrote {args.out}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
