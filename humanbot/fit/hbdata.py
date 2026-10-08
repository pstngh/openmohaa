"""Shared data access for the human-bot fitting scripts.

The human captures live in the private openmohaa-movement repository and never
enter this one. These helpers locate that repository, reuse its analysis code
unchanged (by path, not by copy) and build the per-tick tables the fits need.

Sequence rule (from the data repo): every next-tick value, hold age and
time-since-LOS-change is computed on unbroken valid segments first, and only
then filtered by context. Runs touching a segment edge are censored, except
the first run after a respawn (the empty usercmds before it are dropped).

The rows the fits learn from (`eligible`, see widen_duels) are the 1v1 human duels of
every deathmatch map, not only the data repo's duel mask on the four practice maps.
"""
from __future__ import annotations

import json
import os
import subprocess
import sys
from pathlib import Path

import numpy as np
import pandas as pd

HERE = Path(__file__).resolve().parent
HB_ROOT = HERE.parent                      # <fork>/humanbot
FORK_ROOT = HB_ROOT.parent
CACHE = Path(os.environ.get("HB_CACHE", HB_ROOT / "cache"))
PARTS = CACHE / "parts"

CONTEXTS = ["hidden_nofire", "hidden_fire", "los_nofire", "los_fire", "reload"]
CHORDS = ["back-left", "back", "back-right", "left", "neutral", "right",
          "forward-left", "forward", "forward-right"]
TICK_MS = 50


def movement_repo() -> Path:
    """Path of the private openmohaa-movement checkout."""
    env = os.environ.get("HB_MOVEMENT_REPO")
    cands = [Path(env)] if env else []
    cands += [FORK_ROOT.parent / "openmohaa-movement", Path.home() / "openmohaa-movement"]
    for c in cands:
        if (c / "analysis" / "common.py").exists():
            return c.resolve()
    raise SystemExit("openmohaa-movement not found: set HB_MOVEMENT_REPO to its checkout")


def analysis_cache() -> Path:
    """Directory holding the data repo's run_all.sh caches (features.parquet ...)."""
    env = os.environ.get("HB_ANALYSIS_CACHE")
    cands = [Path(env)] if env else []
    cands += [CACHE / "analysis", movement_repo() / "cache"]
    for c in cands:
        if (c / "features.parquet").exists():
            return c.resolve()
    return cands[0]


def import_analysis():
    """Make the data repo's analysis modules importable (common, load_captures)."""
    p = str(movement_repo() / "analysis")
    if p not in sys.path:
        sys.path.insert(0, p)
    import common  # noqa: F401
    import load_captures  # noqa: F401
    return common, load_captures


def ensure_features(rebuild: bool = False) -> Path:
    """Build the data repo's caches with its own (unchanged) scripts if missing."""
    cache = analysis_cache()
    if rebuild or not (cache / "features.parquet").exists():
        repo = movement_repo()
        cache.mkdir(parents=True, exist_ok=True)
        subprocess.run([sys.executable, str(repo / "analysis" / "load_captures.py"), str(repo / "captures"), str(cache)], check=True)
        subprocess.run([sys.executable, str(repo / "analysis" / "common.py"), str(cache)], check=True)
    return cache


FRAME_COLS = [
    "session_id", "client_id", "session_ms", "seg", "seg_start", "valid", "dm_session", "eligible", "human_duel", "normal_physics",
    "person", "name",
    "capture_date", "map", "sv_mapchecksum", "line_of_sight", "attack", "reloading", "action", "side", "fwd", "lean", "lean_both",
    "crouch_key", "jump_key", "run", "ducked", "on_ground", "on_ladder", "speed_xy", "clear_move", "clear_left", "clear_right",
    "clear_front", "clear_back", "clear_front_left", "clear_front_right", "clear_back_left", "clear_back_right",
    "yaw_d", "pitch_d", "view_yaw", "view_pitch", "aim_yaw_error", "aim_pitch_error", "aim_total_error", "aim_height_fraction",
    "aim_closest_miss", "crosshair_on_opponent", "tgt_half_w_deg", "tgt_yaw_d", "tgt_pitch_d", "distance_xy", "distance_xyz",
    "height_delta", "relative_bearing", "self_approach_speed", "self_tangential_speed", "opp_self_tangential_speed",
    "opp_self_approach_speed", "opp_weapon_state", "opp_attack_primary", "opp_line_of_sight", "opp_speed_xy", "health",
    "opp_health", "clip_ammo", "clip_size", "weapon", "weapon_state", "origin_x", "origin_y", "origin_z", "velocity_x",
    "velocity_y", "velocity_z", "opponent_id", "opponent_origin_x", "opponent_origin_y", "opponent_origin_z",
    "opponent_velocity_x", "opponent_velocity_y", "has_opp", "opp_alive", "ev_shot", "ev_reload", "ev_dmg_taken",
    "ev_dmg_dealt", "ev_kill", "ev_death", "ev_spawn", "pm_flags", "eye_z", "ext_vis_parts",
]

# The owner (2026-10-08): learn from the 1v1 duels of every deathmatch map, not only the four practice maps
# (dm/brownffa and dm/flag above all, the maps they play). A game counts when the player held an SMG for at least this
# share of its duel time: dm/codex_dust2_v2 (07-26) was played with rifles 55-67% of it; every other game 0.96-1.
WIDE_SMG_MIN = 0.5
SMG = ["MP40", "Thompson"]


def load_dm(cols=None) -> pd.DataFrame:
    """Valid human rows of deathmatch sessions, sorted, with sequence features.

    Adds ctx, next-tick values (nx_*), ages (age_*, ticks, 1 on the first tick of
    a run), known_* (the run's start is observed: it did not start at a segment
    edge, or the segment starts with a respawn) and `lage` (ms since
    line_of_sight last changed, or since the respawn; NaN when unobserved). `vis`
    is 1 while a body part of the opponent is on screen (attach_vis_parts; 0 outside
    `eligible`) and `vage` the ms since it changed, the way `lage` follows line_of_sight. The empty usercmds
    right after each respawn are dropped (drop_spawn_dead_time).
    """
    cache = ensure_features()
    src = cache / "features.parquet"
    seq = CACHE / "dm_seq_v9.parquet"
    if cols is None and seq.exists() and seq.stat().st_mtime > src.stat().st_mtime:
        return pd.read_parquet(seq)
    F = pd.read_parquet(src, columns=cols or FRAME_COLS, filters=[("valid", "==", True), ("dm_session", "==", True)])
    F = F.sort_values(["session_id", "client_id", "session_ms"], kind="stable").reset_index(drop=True)
    widen_duels(F)
    F = attach_vis_parts(F, cache)
    F = drop_spawn_dead_time(F)
    add_sequences(F)
    if cols is None:
        CACHE.mkdir(parents=True, exist_ok=True)
        F.to_parquet(seq, index=False)
    return F


def widen_duels(F: pd.DataFrame) -> None:
    """`eligible` becomes the 1v1 human duels of every deathmatch map with normal physics (the data repo's duel mask,
    kept as `eligible_practice`, plus the other dm maps' duels in games the player held an SMG in, WIDE_SMG_MIN)."""
    F["eligible_practice"] = F.eligible.astype(bool)
    duel = F.human_duel.astype(bool) & F.normal_physics.astype(bool) & F["map"].astype(str).str.startswith("dm/")
    smg = F.weapon.isin(SMG).where(duel)
    share = smg.groupby([F.session_id, F.client_id], sort=False).transform("mean")
    F["eligible"] = F.eligible_practice | (duel & share.ge(WIDE_SMG_MIN))


def load_team() -> pd.DataFrame:
    """Valid human rows of the team matches (objective mode, obj/obj_team2 and obj/obj_team4, normal physics), sorted,
    with the sequence features of load_dm. Context, never pooled with the duels (the owner, 2026-10-08: "use the team
    matches too but just as extra context as gameplay is much different"): `eligible` is False on every row, and
    `team_ctx` marks the rows a fit may take context from (an SMG held, the nearest enemy alive and known; with two
    enemies alive the opponent columns describe the nearest)."""
    cache = ensure_features()
    src = cache / "features.parquet"
    seq = CACHE / "team_seq_v1.parquet"
    if seq.exists() and seq.stat().st_mtime > src.stat().st_mtime:
        return pd.read_parquet(seq)
    cols = [c for c in FRAME_COLS if c != "ext_vis_parts"] + ["g_gametype"]
    F = pd.read_parquet(src, columns=cols, filters=[("valid", "==", True), ("g_gametype", "==", 4)])
    F = F[F.normal_physics.astype(bool)]
    F = F.sort_values(["session_id", "client_id", "session_ms"], kind="stable").reset_index(drop=True)
    F["eligible"] = False
    F["vis"] = np.zeros(len(F), "int8")
    F["vis_known"] = False
    F = drop_spawn_dead_time(F)
    add_sequences(F)
    F["team_ctx"] = F.weapon.isin(SMG) & F.opp_alive.eq(1) & F.has_opp.astype(bool)
    CACHE.mkdir(parents=True, exist_ok=True)
    F.to_parquet(seq, index=False)
    return F


SPAWN_STALE_TICKS = 5   # the logged parts are traced from the previous life's eye for ~250 ms after a respawn


def attach_vis_parts(F: pd.DataFrame, cache: Path) -> pd.DataFrame:
    """`vis`: a body part of the opponent on screen. On the practice maps the rebuilt column of the data repo's
    vis_parts.py (as before the duels were widened); elsewhere the logged ext_vis_parts (schema 13: the 2026-09-28 and
    10-04 captures), with the centroid ray for the first SPAWN_STALE_TICKS of a life. Rows with neither (the duels of
    dm/brownffa before schema 13) get the centroid ray and `vis_known` False: the fits that time the sight from the
    first part (the trigger, the early reload, the bursts begun in sight) leave them out. 0 outside `eligible`."""
    p = cache / "vis_parts.parquet"
    if not p.exists():
        raise SystemExit(f"{p} missing: run the data repo's analysis/vis_parts.py (it needs the MOHAA folder)")
    V = pd.read_parquet(p, columns=["session_id", "client_id", "session_ms", "vis_parts_rebuilt"])
    F = F.merge(V, on=["session_id", "client_id", "session_ms"], how="left", validate="one_to_one")
    rebuilt = F.pop("vis_parts_rebuilt")
    logged = F.pop("ext_vis_parts") if "ext_vis_parts" in F else pd.Series(np.nan, index=F.index)
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    stale = g.ev_spawn.transform("first").fillna(0).gt(0) & g.cumcount().lt(SPAWN_STALE_TICKS)
    los = F.line_of_sight.fillna(0).gt(0)
    from_log = np.where(stale, los, logged.fillna(0).gt(0))
    vis = np.where(rebuilt.notna(), rebuilt.fillna(0).gt(0), np.where(logged.notna(), from_log, los))
    F["vis_known"] = F.eligible & (rebuilt.notna() | logged.notna())
    F["vis"] = (vis & F.eligible).astype("int8")
    return F


SPAWN_DEAD_MAX_TICKS = 6
SPAWN_CLICK_TICKS = 10      # the respawn click can still be held or repeated this long after the first live tick


def drop_spawn_dead_time(F: pd.DataFrame) -> pd.DataFrame:
    """Drop the empty usercmds at the start of every life.

    For 2-4 ticks after a respawn the client still sends empty usercmds (no keys,
    no run bit, no mouse, no attack) while it catches up with the server. They are
    not decisions, so each spawn segment starts at its first live tick instead, and
    `spawn_seg` marks segments whose first live tick is a real start: runs that
    begin there are complete, not censored. Only leading rows are dropped, so the
    remaining rows of every segment stay unbroken.
    """
    keys = [F.session_id, F.client_id, F.seg]
    g = F.groupby(keys, sort=False)
    spawn = g.ev_spawn.transform("first").fillna(0).astype(bool)
    empty = F.run.eq(0) & F.action.eq(4) & ~F.attack.astype(bool) & F.yaw_d.fillna(0).eq(0)
    lead = empty.astype(int).groupby(keys, sort=False).cummin().astype(bool)
    dead = spawn & lead & g.cumcount().lt(SPAWN_DEAD_MAX_TICKS)
    ndead = dead.astype(int).groupby(keys, sort=False).transform("sum")
    # where the life started (the player settles onto the floor during the dead time)
    at = {f"spawn_{c}": g[src].transform("first") for c, src in [("x", "origin_x"), ("y", "origin_y"), ("z", "origin_z"),
                                                                   ("yaw", "view_yaw")]}
    F = F.assign(spawn_seg=spawn, dead_ticks=ndead.where(spawn, -1), **at).loc[~dead].reset_index(drop=True)
    return F


def add_sequences(F: pd.DataFrame) -> None:
    keys = ["session_id", "client_id", "seg"]
    g = F.groupby(keys, sort=False)
    F["seg_k"] = g.cumcount()      # ticks since the segment's first (live) tick
    F["ctx"] = np.select([F.reloading, F.line_of_sight.eq(1) & F.attack, F.line_of_sight.eq(1), F.attack],
                         ["reload", "los_fire", "los_nofire", "hidden_fire"], "hidden_nofire")
    F["ctx_i"] = F.ctx.map({c: i for i, c in enumerate(CONTEXTS)}).astype("int8")
    first_tick = g.cumcount().eq(0)
    for c in ["action", "side", "fwd", "lean", "attack", "crouch_key", "jump_key", "run", "line_of_sight", "vis"]:
        if c not in F:
            continue
        F["nx_" + c] = g[c].shift(-1)
        run = (F[c] != g[c].shift()).cumsum()
        F["age_" + c] = F.groupby([F.session_id, F.client_id, F.seg, run], sort=False).cumcount() + 1
        # a run that starts on the segment's first tick has an unknown (censored) start,
        # unless the segment starts with a respawn (see drop_spawn_dead_time)
        run_first = run.where(first_tick).groupby([F.session_id, F.client_id, F.seg], sort=False).transform("max")
        F["known_" + c] = run.ne(run_first) | F.spawn_seg
    # ticks since the context changed (1 on the first tick of a context run)
    crun = (F["ctx_i"] != g["ctx_i"].shift()).cumsum()
    F["age_ctx"] = F.groupby([F.session_id, F.client_id, F.seg, crun], sort=False).cumcount() + 1
    crun_first = crun.where(first_tick).groupby([F.session_id, F.client_id, F.seg], sort=False).transform("max")
    F.loc[crun.eq(crun_first), "age_ctx"] = 10000   # context started before the segment: treat as old
    F["lage"] = (F["age_line_of_sight"] - 1) * TICK_MS
    F.loc[~F["known_line_of_sight"], "lage"] = np.nan
    if "vis" in F:
        F["vage"] = (F["age_vis"] - 1) * TICK_MS
        F.loc[~F["known_vis"], "vage"] = np.nan
    F["en"] = F.aim_total_error / F.tgt_half_w_deg


def write_part(name: str, obj) -> Path:
    PARTS.mkdir(parents=True, exist_ok=True)
    p = PARTS / f"{name}.json"
    p.write_text(json.dumps(jsonable(obj), indent=1, sort_keys=False))
    return p


def read_part(name: str):
    return json.loads((PARTS / f"{name}.json").read_text())


def jsonable(x):
    if isinstance(x, dict):
        return {str(k): jsonable(v) for k, v in x.items()}
    if isinstance(x, (list, tuple)):
        return [jsonable(v) for v in x]
    if isinstance(x, np.ndarray):
        return [jsonable(v) for v in x.tolist()]
    if isinstance(x, (np.integer,)):
        return int(x)
    if isinstance(x, (np.floating, float)):
        v = float(x)
        if not np.isfinite(v):
            return None
        return float(f"{v:.6g}")
    if isinstance(x, (np.bool_,)):
        return bool(x)
    return x


def logit(p, eps=1e-6):
    p = np.clip(np.asarray(p, dtype=float), eps, 1 - eps)
    return np.log(p / (1 - p))


def sigmoid(z):
    return 1.0 / (1.0 + np.exp(-np.asarray(z, dtype=float)))


def shrink_rate(k, n, k0, n0, strength=20.0):
    """Empirical-Bayes rate: k/n shrunk toward k0/n0 with `strength` pseudo-counts."""
    k = np.asarray(k, float)
    n = np.asarray(n, float)
    prior = (k0 + 0.5) / (n0 + 1.0)
    return (k + strength * prior) / (n + strength)


def fit_logistic(codes, y, sizes, l2=1.0, offset=None, weights=None, prior=None):
    """Additive logistic regression over categorical factors.

    codes: list of int arrays (one per factor, values in [0, size)); a -1 code means
    'factor absent' (no contribution). prior: per factor None or the coefficients' prior
    means (the L2 penalty pulls toward them instead of 0). Returns (intercept, [coef arrays]).
    """
    y = np.asarray(y, float)
    n = len(y)
    off = np.zeros(n) if offset is None else np.asarray(offset, float)
    w = np.ones(n) if weights is None else np.asarray(weights, float)
    starts = np.cumsum([0] + list(sizes))[:-1]
    nparam = 1 + int(sum(sizes))
    cols = [np.where(c >= 0, c + s + 1, -1) for c, s in zip(codes, starts)]
    mu = np.zeros(nparam - 1)
    for s, k, p in zip(starts, sizes, prior or [None] * len(sizes)):
        if p is not None:
            mu[s:s + k] = np.asarray(p, float).ravel()

    def unpack(theta):
        return theta[0], [theta[1 + s:1 + s + k] for s, k in zip(starts, sizes)]

    def f(theta):
        z = off + theta[0]
        for c in cols:
            z = z + np.where(c >= 0, theta[np.maximum(c, 0)], 0.0)
        p = sigmoid(z)
        eps = 1e-12
        ll = (w * (y * np.log(p + eps) + (1 - y) * np.log(1 - p + eps))).sum()
        r = w * (p - y)
        g = np.zeros(nparam)
        g[0] = r.sum()
        for c in cols:
            m = c >= 0
            np.add.at(g, c[m], r[m])
        g[1:] += 2 * l2 * (theta[1:] - mu)
        return -ll + l2 * ((theta[1:] - mu) ** 2).sum(), g

    from scipy import optimize
    res = optimize.minimize(f, np.r_[0.0, mu], jac=True, method="L-BFGS-B", options={"maxiter": 3000})
    b0, coefs = unpack(res.x)
    return float(b0), [np.asarray(c) for c in coefs]


def binidx(x, edges):
    """Index of the bin whose lower edge is <= x (edges ascending); NaN -> -1."""
    x = np.asarray(x, float)
    i = np.clip(np.searchsorted(np.asarray(edges, float), x, side="right") - 1, 0, len(edges) - 1)
    return np.where(np.isnan(x), -1, i)
