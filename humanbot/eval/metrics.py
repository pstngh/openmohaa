"""REPORT section 11 acceptance statistics with block-bootstrap confidence intervals.

Input: an analysis cache built by the data repo's unchanged scripts (features.parquet from
common.py; shots.parquet from combat.py; reloads_all.parquet from lives.py;
acquisitions.pkl from acquisition.py; engagements.pkl from engagements.py; plus
events.parquet) and its results dir (the scripts' JSON, used by validate() to check that
the definitions below reproduce the scripts' pooled numbers).

Every statistic mirrors the definition in the named script (behavior.py unless noted): the
per-tick sequences (next tick, hold ages, time since LOS changed, runs) are computed on
unbroken valid segments first and filtered by context afterwards; complete runs never touch
a segment edge. What is new here is only the grouping: every statistic is reduced to
per-block sufficient statistics, a block being one player in one session
(session_id, client_id, person). A subject group (all humans, the pooled bots, the owner) is
a set of blocks, and the confidence interval comes from resampling blocks (200 resamples,
2.5-97.5 percentiles). Pooled bots can be reweighted so the style-family mix of their duel
time matches target weights (humanbot/model/styles.json).

Kinds: ratio (sum of numerators / sum of denominators), quantile (weighted, pandas' linear
interpolation when weights are 1) and custom (occupancy concentration per map).

prepare(cache, results, moh_dir) -> MetricData; summarize(data, block_mask, ...) -> {metric: stats};
the "perception.*" statistics (perception.py: pre-aim, corners and reaction from the first visible body
part) are added when moh_dir has the practice maps, on the data repo's rebuilt body-part column when the cache
has an accepted one (people), else on the logged one (bot captures, schema 13);
dials(data, persons) -> realised style dials (humanbot/fit/fit_styles.py definitions);
validate(stats of every block, results dir) -> [(metric, ours, script's)].

CLI (debugging): metrics.py CACHE [RESULTS] [--persons NAME ...] [--moh-dir DIR] prints the statistics of the
chosen persons' blocks (default: every block) and the check against the scripts' JSON.
"""
from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path

import numpy as np
import pandas as pd

KEY = ["session_id", "client_id", "session_ms"]
CONTEXTS = ["hidden_nofire", "hidden_fire", "los_nofire", "los_fire", "reload"]
CTX_LABEL = {"hidden_nofire": "hidden, not firing", "hidden_fire": "hidden, firing", "los_nofire": "LOS, not firing",
             "los_fire": "LOS firefight", "reload": "reloading"}
CHORD_GROUPS = {"pure_strafe": [3, 5], "fwd_diag": [6, 8], "forward": [7], "neutral": [4], "back_any": [0, 1, 2]}
PRACTICE = ["dm/crnodoors", "dm/main", "dm/vents", "dm/downladder"]
SMG = ["MP40", "Thompson"]
N_BOOT = 200
MIN_UNITS = {"ratio": 30, "quantile": 20, "custom": 1200}

FEATURE_COLS = ["session_id", "client_id", "session_ms", "seg", "eligible", "person", "map", "line_of_sight", "attack",
                "reloading", "action", "side", "fwd", "lean", "speed_xy", "self_approach_speed", "distance_xy",
                "distance_xyz", "aim_total_error", "aim_yaw_error", "aim_height_fraction", "crosshair_on_opponent",
                "tgt_half_w_deg", "yaw_d", "clip_ammo", "jump_key", "crouch_key", "run", "origin_x", "origin_y", "weapon"]
LOOKUP_COLS = ["session_id", "client_id", "session_ms", "person", "is_bot", "practice", "normal_physics", "clip_ammo",
               "weapon"]


@dataclass
class Metric:
    name: str
    label: str
    section: str
    unit: str
    kind: str
    perf: bool = False       # skill or outcome statistic (flagged in reports; still a tell when off the human CI)


@dataclass
class MetricData:
    keys: list = field(default_factory=list)          # (session_id, client_id, person) per block
    index: dict = field(default_factory=dict)
    rows: list = field(default_factory=list)          # eligible duel rows per block
    metrics: dict = field(default_factory=dict)       # name -> Metric (insertion order = report order)
    ratio: dict = field(default_factory=dict)         # name -> (num, den, units) arrays per block
    quant: dict = field(default_factory=dict)         # name -> (q, sorted values, block per value)
    custom: dict = field(default_factory=dict)        # name -> factory(block mask) -> (fn(weights), units per block)
    notes: list = field(default_factory=list)
    frames: object = None                              # valid DM rows with sequence features (for dials())
    extra: dict = field(default_factory=dict)          # shots / acquisitions tables for dials()

    # ---- blocks
    @property
    def nb(self):
        return len(self.keys)

    def block_ids(self, sid, cid, person) -> np.ndarray:
        out = np.empty(len(sid), dtype=np.int64)
        for i, k in enumerate(zip(sid, (int(c) for c in cid), person)):
            j = self.index.get(k)
            if j is None:
                j = self.index[k] = len(self.keys)
                self.keys.append(k)
                self.rows.append(0)
            out[i] = j
        return out

    def block_ids_frame(self, df, sid="session_id", cid="client_id", person="person") -> np.ndarray:
        u = df[[sid, cid, person]].drop_duplicates()
        ids = self.block_ids(u[sid].to_numpy(), u[cid].to_numpy(), u[person].to_numpy())
        m = pd.Series(ids, index=pd.MultiIndex.from_frame(u))
        return m.reindex(pd.MultiIndex.from_frame(df[[sid, cid, person]])).to_numpy().astype(np.int64)

    def persons(self) -> np.ndarray:
        return np.array([k[2] for k in self.keys], dtype=object)

    def sessions(self) -> np.ndarray:
        return np.array([k[0] for k in self.keys], dtype=object)

    # ---- registration
    def _add(self, name, label, section, unit, kind, perf):
        self.metrics[name] = Metric(name, label, section, unit, kind, perf)

    def add_ratio(self, name, label, section, unit, blk, num, den=None, perf=False, units=None):
        blk = np.asarray(blk, dtype=np.int64)
        num = np.asarray(num, dtype=float)
        den = np.ones(len(blk)) if den is None else np.asarray(den, dtype=float)
        u = np.ones(len(blk)) if units is None else np.asarray(units, dtype=float)
        n = self.nb
        self._add(name, label, section, unit, "ratio", perf)
        self.ratio[name] = (np.bincount(blk, weights=num, minlength=n), np.bincount(blk, weights=den, minlength=n),
                            np.bincount(blk, weights=u, minlength=n))

    def add_rate_bins(self, base, label, section, unit, blk, bins, labels, num, perf=False, den=None):
        """One ratio metric per bin; `bins` is an array of bin index per row (-1 = none)."""
        bins = np.asarray(bins)
        for k, lab in enumerate(labels):
            m = bins == k
            self.add_ratio(f"{base}.{lab[0]}", f"{label} {lab[1]}", section, unit, blk[m],
                           np.asarray(num)[m], None if den is None else np.asarray(den)[m], perf)

    def add_quant(self, base, label, section, unit, blk, vals, qs=(("p50", 0.5),), perf=False):
        vals = np.asarray(vals, dtype=float)
        blk = np.asarray(blk, dtype=np.int64)
        ok = ~np.isnan(vals)
        vals, blk = vals[ok], blk[ok]
        o = np.argsort(vals, kind="stable")
        vals, blk = vals[o], blk[o]
        for suf, q in qs:
            name = f"{base}.{suf}" if suf else base
            self._add(name, f"{label} {suf}" if suf else label, section, unit, "quantile", perf)
            self.quant[name] = (q, vals, blk)

    def add_custom(self, name, label, section, unit, factory, perf=False):
        self._add(name, label, section, unit, "custom", perf)
        self.custom[name] = factory

    def finalize(self):
        n = self.nb
        for k, (a, b, u) in self.ratio.items():
            if len(a) < n:
                pad = n - len(a)
                self.ratio[k] = (np.r_[a, np.zeros(pad)], np.r_[b, np.zeros(pad)], np.r_[u, np.zeros(pad)])
        self.rows = list(self.rows) + [0] * (n - len(self.rows))


# ====================================================================== preparation

def _bins(x, edges, right=True):
    """pd.cut bin index (right-closed, (e0, e1], ...); -1 outside / NaN."""
    c = pd.cut(pd.Series(np.asarray(x, dtype=float)), edges, right=right)
    return c.cat.codes.to_numpy()


def _labels(edges, fmt=lambda v: f"{v:g}"):
    return [(f"{fmt(a)}-{fmt(b)}", f"({fmt(a)}, {fmt(b)}]") for a, b in zip(edges[:-1], edges[1:])]


def sequences(V: pd.DataFrame) -> pd.DataFrame:
    """behavior.py's per-segment sequence features (V sorted by session, client, time; valid rows)."""
    sid, cid, seg = V.session_id.to_numpy(), V.client_id.to_numpy(), V.seg.to_numpy()
    n = len(V)
    new = np.ones(n, dtype=bool)
    new[1:] = (sid[1:] != sid[:-1]) | (cid[1:] != cid[:-1]) | (seg[1:] != seg[:-1])
    last = np.ones(n, dtype=bool)
    last[:-1] = new[1:]
    V["_segstart"] = new
    V["_seglast"] = last
    V["_segid"] = np.cumsum(new) - 1
    V["ctx"] = np.select([V.reloading, V.line_of_sight.eq(1) & V.attack, V.line_of_sight.eq(1), V.attack],
                         ["reload", "los_fire", "los_nofire", "hidden_fire"], "hidden_nofire")
    idx = np.arange(n)
    for c in ["side", "attack", "action", "lean", "fwd", "line_of_sight"]:
        x = V[c].to_numpy()
        start = new.copy()
        start[1:] |= x[1:] != x[:-1]
        rid = np.cumsum(start) - 1
        first = idx[start][rid]
        age = (idx - first) * 50
        nx = np.empty(n, dtype=float)
        nx[:-1] = x[1:]
        nx[last] = np.nan
        if c == "line_of_sight":
            V["lage"] = age
        else:
            V["nx_" + c] = nx
            V["age_" + c] = age + 50
        V["_run_" + c] = rid
    V["en"] = V.aim_total_error / V.tgt_half_w_deg
    # jump/crouch/walk presses: behavior.py (a press on a segment's first tick counts)
    for c, k in (("jump_key", "jump_start"), ("crouch_key", "crouch_start")):
        x = V[c].to_numpy().astype(bool)
        prev = np.zeros(n, dtype=bool)
        prev[1:] = x[:-1]
        prev[new] = False
        V[k] = x & ~prev
        # fit_styles.py edges_per_min: the first tick of a segment never counts
        V[k + "_fs"] = x & ~prev & ~new
    walk = V.run.eq(0).to_numpy()
    prev = np.zeros(n, dtype=bool)
    prev[1:] = walk[:-1]
    prev[new] = False
    V["walk_start"] = walk & ~prev
    # time since the opponent was last visible within the segment
    t = V.session_ms.where(V.line_of_sight.eq(1))
    V["since_seen"] = V.session_ms - t.groupby(V["_segid"]).ffill()
    return V


def runs(V: pd.DataFrame, col: str) -> pd.DataFrame:
    """behavior.py runs(): complete runs of `col` (not touching a segment edge, every row eligible)."""
    rid = V["_run_" + col].to_numpy()
    n_runs = rid.max() + 1 if len(rid) else 0
    if n_runs == 0:
        return pd.DataFrame({"val": [], "n": [], "el": [], "ctx": [], "first": np.array([], dtype=np.int64), "ms": []})
    size = np.bincount(rid, minlength=n_runs)
    el = np.bincount(rid, weights=(~V.eligible.to_numpy()).astype(float), minlength=n_runs) == 0
    first = np.searchsorted(rid, np.arange(n_runs))
    seg = V["_segid"].to_numpy()
    seg_first = np.r_[True, seg[first][1:] != seg[first][:-1]]
    seg_last = np.r_[seg[first][1:] != seg[first][:-1], True]
    # modal context, ties to the context seen first (pandas value_counts order)
    code = pd.Categorical(V.ctx, categories=CONTEXTS).codes.astype(np.int64)
    k = len(CONTEXTS)
    cnt = np.bincount(rid * k + code, minlength=n_runs * k).reshape(n_runs, k)
    pos = np.full(n_runs * k, np.iinfo(np.int64).max)
    np.minimum.at(pos, rid * k + code, np.arange(len(rid)))
    pos = pos.reshape(n_runs, k)
    score = cnt.astype(np.int64) * (len(rid) + 1) - np.where(cnt > 0, pos, 0)
    ctx = np.array(CONTEXTS)[score.argmax(1)]
    t = pd.DataFrame({"val": V[col].to_numpy()[first], "n": size, "el": el, "ctx": ctx, "first": first})
    t = t[~seg_first & ~seg_last & t.el]
    t["ms"] = t.n * 50
    return t


def prepare(cache: Path, results: Path | None = None, verbose: bool = True, moh_dir=None) -> MetricData:
    cache = Path(cache)
    D = MetricData()
    say = print if verbose else (lambda *a, **k: None)
    F = pd.read_parquet(cache / "features.parquet", columns=FEATURE_COLS,
                        filters=[("valid", "==", True), ("dm_session", "==", True)])
    V = F.sort_values(KEY, kind="stable").reset_index(drop=True)
    del F
    for c in ["session_id", "person", "map", "weapon"]:
        V[c] = V[c].astype(object)
    V = sequences(V)
    V["blk"] = D.block_ids_frame(V)
    EL = V[V.eligible].copy()
    rows = np.bincount(EL.blk, minlength=D.nb)
    D.rows = list(rows)
    say(f"metrics: {len(V)} valid DM rows, {len(EL)} eligible, {D.nb} blocks")
    D.frames = V
    movement(D, V, EL)
    trigger(D, V, EL)
    view(D, V, EL)
    space(D, EL)
    tables(D, cache, V)
    if results is not None:
        import perception as PC
        column = PC.column_for(cache, Path(results))
        why = PC.available(cache, Path(results), moh_dir, column)
        if why is None:
            T = PC.tables(cache, Path(results), moh_dir, column=column, verbose=verbose)
            PC.register(D, T)
            D.extra["perception"] = T
        else:
            D.notes.append(f"perception statistics skipped: {why}")
    D.finalize()
    return D


# ---------------------------------------------------------------------- movement

def movement(D: MetricData, V, EL):
    blk = EL.blk.to_numpy()
    ctx = EL.ctx.to_numpy()
    act = EL.action.to_numpy()
    for c in CONTEXTS:
        D.add_ratio(f"movement.context_share.{c}", f"Share of duel time: {CTX_LABEL[c]}", "movement", "share",
                    blk, ctx == c)
    for c in CONTEXTS:
        m = ctx == c
        for g, codes in CHORD_GROUPS.items():
            D.add_ratio(f"movement.chord.{c}.{g}", f"Key chord {g.replace('_', ' ')} ({CTX_LABEL[c]})", "movement", "share",
                        blk[m], np.isin(act[m], codes))
    T = EL[(EL.side != 0) & EL.nx_side.notna()]
    end = T[T.nx_side != T.side]
    for c in CONTEXTS:
        d = end[end.ctx.eq(c)]
        D.add_ratio(f"movement.strafe_reverse.{c}", f"Strafe end is a direct reverse ({CTX_LABEL[c]})", "movement", "share",
                    d.blk, d.nx_side == -d.side)
    H = T[T.ctx.eq("los_fire")]
    age = H.age_side.clip(upper=1500).to_numpy()
    groups = [("50", [50]), ("100", [100]), ("150", [150]), ("200", [200]), ("250", [250]),
              ("300-450", [300, 350, 400, 450]), ("500-1000", list(range(500, 1001, 50))),
              ("1050-1500", list(range(1050, 1501, 50)))]
    for lab, ages in groups:
        m = np.isin(age, ages)
        D.add_ratio(f"movement.switch_hazard.{lab}ms", f"Strafe switch hazard per tick at hold age {lab} ms (LOS firefight)",
                    "movement", "per_tick", H.blk.to_numpy()[m], (H.nx_side != H.side).to_numpy()[m])
    S = runs(V, "side")
    S = S[S.val.ne(0)]
    sb = V.blk.to_numpy()[S["first"].to_numpy()]
    lf = S.ctx.eq("los_fire").to_numpy()
    D.add_quant("movement.side_hold.los_fire", "Side-hold duration (LOS firefight)", "movement", "ms", sb[lf], S.ms[lf],
                (("p25", .25), ("p50", .5), ("p75", .75), ("p90", .9)))
    D.add_quant("movement.side_hold.all", "Side-hold duration (all contexts)", "movement", "ms", sb, S.ms,
                (("p50", .5), ("p90", .9)))
    N = runs(V, "side")
    N = N[N.val.eq(0) & N.ctx.eq("los_fire")]
    D.add_quant("movement.neutral_gap.los_fire", "Neutral gap between strafes (LOS firefight)", "movement", "ms",
                V.blk.to_numpy()[N["first"].to_numpy()], N.ms, (("p50", .5), ("p90", .9)))
    LF = EL[EL.ctx.eq("los_fire")]
    db = _bins(LF.distance_xy, [0, 96, 160, 224, 288, 384, 512, 768, 1200])
    labs = _labels([0, 96, 160, 224, 288, 384, 512, 768, 1200])
    D.add_rate_bins("movement.approach.los_fire", "Approaching >40 u/s in LOS firefight at distance", "movement", "share",
                    LF.blk.to_numpy(), db, labs, (LF.self_approach_speed > 40).to_numpy())
    D.add_rate_bins("movement.retreat.los_fire", "Retreating >40 u/s in LOS firefight at distance", "movement", "share",
                    LF.blk.to_numpy(), db, labs, (LF.self_approach_speed < -40).to_numpy())
    still = (EL.speed_xy < 5).to_numpy()
    D.add_ratio("movement.still.all", "Standing still (<5 u/s), all duel time", "movement", "share", blk, still)
    for c in CONTEXTS:
        m = ctx == c
        D.add_ratio(f"movement.still.{c}", f"Standing still (<5 u/s) ({CTX_LABEL[c]})", "movement", "share", blk[m], still[m])
    lean = (EL.lean != 0).to_numpy()
    D.add_ratio("movement.lean.all", "Lean held, all duel time", "movement", "share", blk, lean)
    D.add_ratio("movement.lean.los_fire", "Lean held in LOS firefights", "movement", "share", blk[ctx == "los_fire"],
                lean[ctx == "los_fire"])
    for col, lab in (("jump_start", "Jump presses per minute"), ("crouch_start", "Crouch presses per minute"),
                     ("walk_start", "Walk presses per minute")):
        D.add_ratio(f"movement.{col.replace('_start', '')}_per_min", lab, "movement", "per_min", blk, EL[col].to_numpy(),
                    np.full(len(EL), 1 / 1200))
    D.add_ratio("movement.walk.hidden_nofire", "Walking while hidden and not firing", "movement", "share",
                blk[ctx == "hidden_nofire"], (EL.run == 0).to_numpy()[ctx == "hidden_nofire"])


# ---------------------------------------------------------------------- trigger

def trigger(D: MetricData, V, EL):
    TE = EL[~EL.reloading & EL.clip_ammo.gt(0) & EL.nx_attack.notna()]
    ab = [-1, 0, 100, 250, 500, 1000, 1e6]
    alabs = [("0", "0 ms"), ("50-100", "50-100 ms"), ("150-250", "150-250 ms"), ("300-500", "300-500 ms"),
             ("550-1000", "550-1000 ms"), ("gt1000", "over 1 s")]
    P = TE[~TE.attack & TE.line_of_sight.eq(1) & TE.en.gt(0) & TE.en.le(3)]
    D.add_rate_bins("trigger.press_los_near", "Press hazard with LOS and aim within 3 half-widths, LOS gained", "trigger",
                    "per_tick", P.blk.to_numpy(), _bins(P.lage, ab), [(a, b + " ago") for a, b in alabs], P.nx_attack.to_numpy())
    Ph = TE[~TE.attack & TE.line_of_sight.eq(0)]
    D.add_rate_bins("trigger.press_hidden", "Press hazard while hidden, LOS lost", "trigger", "per_tick",
                    Ph.blk.to_numpy(), _bins(Ph.lage, ab), [(a, b + " ago") for a, b in alabs], Ph.nx_attack.to_numpy())
    Rl = TE[TE.attack & TE.line_of_sight.eq(1)]
    eb = [0, 1, 2, 3, 4, 6, 10, 1000]
    D.add_rate_bins("trigger.release_los", "Release hazard, LOS, aim error in half-widths", "trigger", "per_tick",
                    Rl.blk.to_numpy(), _bins(Rl.en, eb), _labels(eb), 1 - Rl.nx_attack.to_numpy())
    Rh = TE[TE.attack & TE.line_of_sight.eq(0)]
    lg = Rh.lage.clip(upper=1500).to_numpy()
    rb = np.select([lg <= 100, lg <= 250, lg <= 450, lg <= 950], [0, 1, 2, 3], 4)
    D.add_rate_bins("trigger.release_hidden", "Release hazard while hidden, LOS lost", "trigger", "per_tick",
                    Rh.blk.to_numpy(), rb, [("0-100", "0-100 ms ago"), ("150-250", "150-250 ms ago"),
                                            ("300-450", "300-450 ms ago"), ("500-950", "500-950 ms ago"),
                                            ("1000-1500", "1 s or more ago")], 1 - Rh.nx_attack.to_numpy())
    LSE = EL[EL.line_of_sight.eq(1) & ~EL.reloading & EL.clip_ammo.gt(0)]
    hb = [0, 0.5, 1, 1.5, 2, 3, 4, 6, 10, 20, 1000]
    D.add_rate_bins("trigger.hold_los", "Fire held, LOS, aim error in half-widths", "trigger", "share", LSE.blk.to_numpy(),
                    _bins(LSE.en, hb), _labels(hb), LSE.attack.to_numpy())
    Hh = EL[EL.line_of_sight.eq(0) & ~EL.reloading & EL.clip_ammo.gt(0)]
    D.add_ratio("trigger.hold_hidden", "Fire held without LOS", "trigger", "share", Hh.blk, Hh.attack)
    AT = runs(V, "attack")
    b = V.blk.to_numpy()[AT["first"].to_numpy()]
    held = AT.val.eq(True).to_numpy()
    D.add_quant("trigger.attack_hold", "Attack hold duration", "trigger", "ms", b[held], AT.ms[held], (("p50", .5), ("p90", .9)))
    D.add_ratio("trigger.tap_share", "Attack holds that are taps (<=100 ms)", "trigger", "share", b[held],
                AT.ms.to_numpy()[held] <= 100)
    D.add_quant("trigger.attack_gap", "Gap between attack holds", "trigger", "ms", b[~held], AT.ms[~held], (("p50", .5),))


# ---------------------------------------------------------------------- aim / view / belief

def view(D: MetricData, V, EL):
    L = EL[EL.line_of_sight.eq(1)]
    edges = [0, 128, 192, 256, 384, 512, 768, 1200]
    labs = _labels(edges)
    Lf = L[L.attack]
    db = _bins(Lf.distance_xyz, edges)
    for k, (lab, lab2) in enumerate(labs):
        m = db == k
        D.add_quant(f"aim.error_firing.{lab}", f"Median aim error firing with LOS at {lab2} u", "aim", "deg",
                    Lf.blk.to_numpy()[m], Lf.aim_total_error.to_numpy()[m], (("", .5),), perf=True)
        D.add_ratio(f"aim.on_body_firing.{lab}", f"Crosshair on body firing with LOS at {lab2} u", "aim", "share",
                    Lf.blk.to_numpy()[m], Lf.crosshair_on_opponent.to_numpy()[m], perf=True)
    D.add_quant("aim.height.firing", "Aim height (fraction of body) firing with LOS", "aim", "fraction", Lf.blk,
                Lf.aim_height_fraction, (("", .5),))
    Li = L[~L.attack]
    D.add_quant("aim.height.idle", "Aim height (fraction of body) with LOS, not firing", "aim", "fraction", Li.blk,
                Li.aim_height_fraction, (("", .5),))
    ayaw = (EL.yaw_d.abs() * 20).to_numpy()
    zero = (EL.yaw_d.abs() < 0.01).to_numpy()
    ctx = EL.ctx.to_numpy()
    blk = EL.blk.to_numpy()
    for c in CONTEXTS:
        m = ctx == c
        D.add_quant(f"view.yaw_speed.{c}", f"Yaw speed ({CTX_LABEL[c]})", "view", "deg/s", blk[m], ayaw[m],
                    (("p50", .5), ("p99", .99)))
        D.add_ratio(f"view.mouse_still.{c}", f"Mouse still between ticks ({CTX_LABEL[c]})", "view", "share", blk[m], zero[m])
    # behavior.py turn main sequence: runs of same-sign yaw steps >= 3 deg (all valid rows)
    Vt = V[V.yaw_d.notna()]
    yd = Vt.yaw_d.to_numpy()
    sg = np.where(np.abs(yd) >= 3, np.sign(yd), 0)
    segid = Vt["_segid"].to_numpy()
    new = np.ones(len(Vt), dtype=bool)
    new[1:] = (segid[1:] != segid[:-1]) | (sg[1:] != sg[:-1])
    uid = np.cumsum(new) - 1
    keep = sg != 0
    nu = uid.max() + 1 if len(uid) else 0
    amp = np.bincount(uid[keep], weights=np.abs(yd[keep]), minlength=nu)
    peak = np.zeros(nu)
    np.maximum.at(peak, uid[keep], np.abs(yd[keep]) * 20)
    first = np.searchsorted(uid, np.arange(nu))
    ub = Vt.blk.to_numpy()[first]
    isturn = np.bincount(uid[keep], minlength=nu) > 0
    for lo, hi in ((90, 135), (135, 180), (180, 360)):
        m = isturn & (amp > lo) & (amp <= hi)
        D.add_quant(f"view.turn_peak.{lo}-{hi}", f"Peak yaw speed of {lo}-{hi} deg turns", "view", "deg/s", ub[m], peak[m],
                    (("p50", .5),))
    Hd = V[V.eligible & V.line_of_sight.eq(0)]
    sb = _bins(Hd.since_seen.fillna(1e9), [-1, 250, 500, 1000, 2000, 4000, 8000, 1e8, 2e9])
    slabs = [("0-250", "<=250 ms"), ("250-500", "250-500 ms"), ("500-1000", "0.5-1 s"), ("1000-2000", "1-2 s"),
             ("2000-4000", "2-4 s"), ("4000-8000", "4-8 s"), ("gt8000", ">8 s"), ("never", "never seen this life")]
    ye = Hd.aim_yaw_error.abs().to_numpy()
    for k, (lab, lab2) in enumerate(slabs):
        m = sb == k
        D.add_quant(f"belief.hidden_yaw_error.{lab}", f"Yaw error to the hidden opponent, last seen {lab2}", "belief", "deg",
                    Hd.blk.to_numpy()[m], ye[m], (("", .5),))
        D.add_ratio(f"belief.within30.{lab}", f"View within 30 deg of the hidden opponent, last seen {lab2}", "belief",
                    "share", Hd.blk.to_numpy()[m], ye[m] < 30)


# ---------------------------------------------------------------------- space

def space(D: MetricData, EL):
    for mp in PRACTICE:
        d = EL[EL["map"].eq(mp)]
        if not len(d):
            continue
        x, y, b = d.origin_x.to_numpy(), d.origin_y.to_numpy(), d.blk.to_numpy()

        def factory(mask, x=x, y=y, b=b):
            sel = mask[b]
            if sel.sum() < 2:
                return None, np.zeros(len(mask))
            xs, ys = x[sel], y[sel]
            x0, x1 = np.percentile(xs, [0.2, 99.8])
            y0, y1 = np.percentile(ys, [0.2, 99.8])
            pad, cell = 64, 32.0
            bx = np.arange(x0 - pad, x1 + pad + cell, cell)
            by = np.arange(y0 - pad, y1 + pad + cell, cell)
            ix = np.searchsorted(bx, xs, side="right") - 1
            iy = np.searchsorted(by, ys, side="right") - 1
            ix[xs == bx[-1]] = len(bx) - 2
            iy[ys == by[-1]] = len(by) - 2
            ok = (ix >= 0) & (ix < len(bx) - 1) & (iy >= 0) & (iy < len(by) - 1)
            cell_id = np.where(ok, ix * (len(by) - 1) + iy, -1)
            bb = b[sel]
            ncell = (len(bx) - 1) * (len(by) - 1)

            def fn(w):
                ww = w[bb] * (cell_id >= 0)
                h = np.bincount(np.maximum(cell_id, 0), weights=ww, minlength=ncell)
                occ = np.sort(h[h > 0])[::-1]
                if not len(occ):
                    return np.nan
                return occ[:max(1, int(0.1 * len(occ)))].sum() / occ.sum()
            return fn, np.bincount(bb, minlength=len(mask)).astype(float)
        D.add_custom(f"space.top10_share.{mp.split('/')[1]}", f"Share of duel time in the busiest 10% of 32u cells ({mp})",
                     "space", "share", factory)


# ---------------------------------------------------------------------- per-row tables of the scripts

def tables(D: MetricData, cache: Path, V):
    cache = Path(cache)
    # ---- acquisition.py
    p = cache / "acquisitions.pkl"
    if p.exists():
        A = pd.read_pickle(p)
        if len(A):
            A = A.reset_index(drop=True)
            A["blk"] = D.block_ids_frame(A.assign(session_id=A.session_id.astype(object), person=A.person.astype(object)))
            D.extra["acquisitions"] = A
            ce = pd.DataFrame(A.curve_err.tolist())
            ca = pd.DataFrame(A.curve_attack.tolist())
            b = A.blk.to_numpy()
            for t in (-500, -200, 0, 100, 200, 300, 500, 1000):
                if t in ce:
                    D.add_quant(f"acquisition.error_curve.{t}ms", f"Median aim error at {t:+d} ms from sight gain",
                                "acquisition", "deg", b, ce[t], (("", .5),), perf=t > 0)
            for t in (-500, -200, 0, 100, 200, 400, 700, 1000, 1450):
                if t in ca:
                    v = ca[t].to_numpy(dtype=float)
                    ok = ~np.isnan(v)
                    D.add_ratio(f"acquisition.attack_curve.{t}ms", f"Fire held at {t:+d} ms from sight gain", "acquisition",
                                "share", b[ok], v[ok])
            clean = (~A.attack_before.astype(bool) & (A.err0_total > 2 * A.half_w)).to_numpy()
            D.add_quant("acquisition.clean_first_press", "Reaction: first trigger press after a clean sighting",
                        "acquisition", "ms", b[clean], A.t_first_attack_press.to_numpy(dtype=float)[clean],
                        (("p25", .25), ("p50", .5), ("p75", .75), ("p90", .9)))
            D.add_quant("acquisition.clean_t_close", "Crosshair within half-width+1.5 deg after a clean sighting",
                        "acquisition", "ms", b[clean], A.t_close.to_numpy(dtype=float)[clean], (("p50", .5),), perf=True)
            D.add_ratio("acquisition.on_target_at_0", "Crosshair already on the body at the first visible tick",
                        "acquisition", "share", b, A.on_target_at_0.astype(float), perf=True)
            D.add_ratio("acquisition.attacking_before", "Fire already held on the tick before sight (prefire)",
                        "acquisition", "share", b, A.attack_before.astype(float))
    else:
        D.notes.append("acquisitions.pkl missing: acquisition metrics skipped (run acquisition.py)")
    # ---- combat.py shots
    p = cache / "shots.parquet"
    if p.exists():
        S = pd.read_parquet(p, columns=["session_id", "client_id", "session_ms", "f_person", "burst_id", "burst_len", "hit",
                                        "weapon"])
        S = S.rename(columns={"f_person": "person"})
        S["session_id"] = S.session_id.astype(object)
        S["person"] = S.person.astype(object)
        if len(S):
            S["blk"] = D.block_ids_frame(S)
            D.extra["shots"] = S
            B = S.drop_duplicates("burst_id")
            D.add_quant("trigger.burst_length", "Shots per burst (consecutive 100 ms shots)", "trigger", "shots", B.blk,
                        B.burst_len, (("p50", .5), ("p90", .9)))
            D.add_ratio("combat.accuracy", "Shot accuracy (eligible SMG shots that hit)", "combat", "share", S.blk,
                        S.hit.astype(float), perf=True)
    else:
        D.notes.append("shots.parquet missing: burst and accuracy metrics skipped (run combat.py)")
    # ---- lives.py: lives, respawn delay, post-kill reload policy
    E = pd.read_parquet(cache / "events.parquet", columns=["session_id", "session_ms", "event", "actor_id", "actor_name",
                                                           "actor_bot", "target_id", "target_bot"])
    E["session_id"] = E.session_id.astype(object)
    Fl = pd.read_parquet(cache / "features.parquet", columns=LOOKUP_COLS)
    Fl["session_id"] = Fl.session_id.astype(object)
    Fl["person"] = Fl.person.astype(object)
    dm = Fl[Fl.practice & Fl.normal_physics & Fl.is_bot.eq(0)]
    hs = set(dm.session_id) - set(Fl.loc[Fl.is_bot.eq(1), "session_id"])
    # person of a (session, client): as in the frame rows (common.py's alias mapping already applied)
    who = Fl.drop_duplicates(["session_id", "client_id"]).set_index(["session_id", "client_id"]).person
    lives(D, E, hs, who)
    postkill(D, E, Fl, hs)
    p = cache / "reloads_all.parquet"
    if p.exists():
        RL = pd.read_parquet(p, columns=["session_id", "client_id", "person", "context", "is_empty"])
        RL["session_id"] = RL.session_id.astype(object)
        RL["person"] = RL.person.astype(object)
        RL = RL[RL.person.notna()]
        if len(RL):
            b = D.block_ids_frame(RL)
            D.add_ratio("reload.opponent_dead_share", "Reloads started with the opponent dead", "reload", "share", b,
                        RL.context.eq("opponent_dead").to_numpy())
            alive = RL.context.ne("opponent_dead").to_numpy()
            D.add_ratio("reload.empty_share_opponent_alive", "Reloads with the opponent alive that are forced (empty clip)",
                        "reload", "share", b[alive], RL.is_empty.to_numpy()[alive])
    # ---- engagements.py
    p = cache / "engagements.pkl"
    if p.exists():
        X = pd.read_pickle(p)
        if len(X):
            engagements(D, X)
    else:
        D.notes.append("engagements.pkl missing: fight metrics skipped (run engagements.py)")


def lives(D: MetricData, E, hs, who):
    """lives.py lives table: a life runs from a spawn to the first death before the next spawn."""
    ev = E[E.session_id.isin(hs)]
    sp = ev[ev.event.eq("spawn")][["session_id", "actor_id", "session_ms"]]
    if not len(sp):
        return
    de = ev[ev.event.eq("death")][["session_id", "target_id", "session_ms"]].rename(columns={"target_id": "actor_id"})
    sp = sp.sort_values(["session_id", "actor_id", "session_ms"], kind="stable")
    sp["t_next"] = sp.groupby(["session_id", "actor_id"]).session_ms.shift(-1)
    a = sp.rename(columns={"session_ms": "t_spawn"}).sort_values("t_spawn")
    b = de.rename(columns={"session_ms": "t_death"}).sort_values("t_death")
    L = pd.merge_asof(a, b, left_on="t_spawn", right_on="t_death", by=["session_id", "actor_id"], direction="forward",
                      allow_exact_matches=False)
    L.loc[L.t_death >= L.t_next.fillna(np.inf), "t_death"] = np.nan
    L["person"] = who.reindex(pd.MultiIndex.from_arrays([L.session_id, L.actor_id])).to_numpy()
    L = L[pd.notna(L.person)]
    L["life_ms"] = L.t_death - L.t_spawn
    L["respawn_ms"] = L.t_next - L.t_death
    L = L.rename(columns={"actor_id": "client_id"})
    L["person"] = L.person.astype(object)
    blk = D.block_ids_frame(L)
    D.add_quant("lives.respawn_delay", "Respawn delay after death", "lives", "ms", blk, L.respawn_ms, (("p50", .5),))
    D.add_quant("lives.life_length", "Life length (lives ending in death)", "lives", "s", blk, L.life_ms / 1000,
                (("p50", .5),), perf=True)


def postkill(D: MetricData, E, Fl, hs):
    """lives.py post-kill reload policy: P(reload within 3 s of a kill | rounds left at the kill)."""
    K = E[E.event.eq("death") & E.actor_bot.eq(0) & E.target_bot.eq(0) & (E.actor_id != E.target_id) & E.actor_id.ge(0)
          & E.session_id.isin(hs)]
    if not len(K):
        return
    fk = Fl.set_index(KEY)
    cur = fk.reindex(pd.MultiIndex.from_arrays([K.session_id, K.actor_id, K.session_ms]))
    K = K.assign(rounds=cur.clip_ammo.to_numpy(), w=cur.weapon.to_numpy(), prac=cur.practice.to_numpy(),
                 person=cur.person.to_numpy())
    K = K[K.w.isin(SMG) & K.prac.eq(True)].copy()
    if not len(K):
        return
    R = E[E.event.eq("reload") & E.actor_bot.eq(0)]
    rl = R.groupby(["session_id", "actor_id"]).session_ms.apply(np.sort).to_dict()
    after = []
    for s, c, t in zip(K.session_id, K.actor_id, K.session_ms):
        a = rl.get((s, c))
        i = np.searchsorted(a, t, side="left") if a is not None else 0
        after.append(a[i] - t if a is not None and i < len(a) else np.nan)
    K["after"] = after
    K["rl3"] = K.after.le(3000)
    K = K.rename(columns={"actor_id": "client_id"})
    blk = D.block_ids_frame(K)
    edges = [-1, 0, 3, 6, 10, 15, 20, 25, 29, 32]
    labs = [("0", "0"), ("1-3", "1-3"), ("4-6", "4-6"), ("7-10", "7-10"), ("11-15", "11-15"), ("16-20", "16-20"),
            ("21-25", "21-25"), ("26-29", "26-29"), ("30-32", "30-32")]
    D.add_rate_bins("reload.post_kill_within_3s", "Reload within 3 s of a kill, rounds left", "reload", "share", blk,
                    _bins(K.rounds, edges), labs, K.rl3.to_numpy())
    m = K.rl3.to_numpy()
    D.add_quant("reload.post_kill_delay", "Delay from kill to reload (reloads within 3 s)", "reload", "ms", blk[m],
                K.after.to_numpy()[m], (("p50", .5),))


def engagements(D: MetricData, X):
    """engagements.py outcomes, attributed to each participant's block."""
    rows = []
    for side in ("a", "b"):
        rows.append(pd.DataFrame({"session_id": X.session_id.astype(object), "client_id": X[side], "person": X["p" + side].astype(object),
                                  "kills": X.kills, "killer": X.killer, "first_hit": X.first_hit, "first_shot": X.first_shot,
                                  "dur_ms": X.dur_ms, "dist0": X.dist0, "me": X[side]}))
    P = pd.concat(rows, ignore_index=True)
    b = D.block_ids_frame(P)
    kill = P.kills.gt(0).to_numpy()
    D.add_ratio("fights.kill_share", "Engagements that end in a kill", "fights", "share", b, kill, perf=True)
    fh = kill & P.first_hit.ge(0).to_numpy()
    D.add_ratio("fights.first_hitter_wins", "First hitter wins (decisive engagements)", "fights", "share", b[fh],
                (P.first_hit == P.killer).to_numpy()[fh], perf=True)
    D.add_ratio("fights.first_shooter_wins", "First shooter wins (decisive engagements)", "fights", "share", b[kill],
                (P.first_shot == P.killer).to_numpy()[kill], perf=True)
    D.add_quant("fights.duration_decisive", "Duration of decisive engagements", "fights", "ms", b[kill],
                P.dur_ms.to_numpy()[kill], (("p50", .5),), perf=True)
    D.add_quant("fights.start_distance", "Engagement start distance", "fights", "u", b, P.dist0, (("p50", .5),))
    D.add_ratio("fights.win_share", "Decisive engagements won (subject's own)", "fights", "share", b[kill],
                (P.killer == P.me).to_numpy()[kill], perf=True)


# ====================================================================== bootstrap

def wquantile(vals, w, q):
    """Quantile of sorted `vals` with weights `w` (>=0): pandas' linear interpolation on repeated values
    when the weights are counts; weights are rescaled to sum to the number of weighted values."""
    pos = w > 0
    npos = int(pos.sum())
    if npos == 0:
        return np.nan
    cw = np.cumsum(w)
    tot = cw[-1]
    scale = npos / tot if not np.allclose(w[pos], np.round(w[pos])) else 1.0
    cw = cw * scale
    N = cw[-1]
    h = q * (N - 1)
    k = np.floor(h)
    j0 = min(np.searchsorted(cw, k, side="right"), len(vals) - 1)
    j1 = min(np.searchsorted(cw, min(k + 1, N - 1), side="right"), len(vals) - 1)
    return vals[j0] + (h - k) * (vals[j1] - vals[j0])


def block_weights(counts, fam, rows, targets):
    """Per-block weights for resample counts (R x B): family groups reweighted to `targets` shares of duel time.

    fam: group label per block; targets: {label: weight} for the families to rebalance (others keep their
    own time share). Families absent from the resample drop out and the targets renormalise over those present.
    """
    if not targets:
        return counts.astype(float)
    labels = sorted(set(fam))
    G = np.array([[f == g for g in labels] for f in fam], dtype=float)     # B x G
    time = counts @ (G * np.asarray(rows, float)[:, None])                  # R x G
    tot = time.sum(1, keepdims=True)
    share = np.divide(time, tot, out=np.zeros_like(time), where=tot > 0)
    tw = np.array([targets.get(g, np.nan) for g in labels])
    fammask = ~np.isnan(tw)
    present = (share > 0) & fammask[None, :]
    fam_share = (share * present).sum(1, keepdims=True)
    tws = np.where(present, np.nan_to_num(tw)[None, :], 0.0)
    tsum = tws.sum(1, keepdims=True)
    target = np.where(fammask[None, :], np.divide(tws * fam_share, tsum, out=np.zeros_like(tws), where=tsum > 0), share)
    gscale = np.divide(target, share, out=np.zeros_like(share), where=share > 0)   # R x G
    return counts * (gscale @ G.T)


def summarize(D: MetricData, mask, fam=None, targets=None, n_boot: int = N_BOOT, seed: int = 0) -> dict:
    """Point estimate, bootstrap CI (2.5/97.5), SE, units and blocks for every metric over the blocks in `mask`.

    fam/targets: optional family label per block and target family weights (see block_weights)."""
    mask = np.asarray(mask, dtype=bool)
    sel = np.flatnonzero(mask)
    nb = len(sel)
    out = {}
    if nb == 0:
        return out
    rng = np.random.default_rng(seed)
    C = np.stack([np.bincount(rng.integers(0, nb, nb), minlength=nb) for _ in range(n_boot)]).astype(float)
    rows = np.asarray(D.rows, float)[sel]
    famsel = None if fam is None else np.asarray(fam, dtype=object)[sel]
    ones = np.ones((1, nb))
    W0 = block_weights(ones, famsel, rows, targets) if targets else ones
    W = block_weights(C, famsel, rows, targets) if targets else C
    full0 = np.zeros((1, D.nb))
    full0[0, sel] = W0[0]

    def pack(m, v0, boot, units):
        boot = boot[~np.isnan(boot)]
        u = units[sel]
        res = {"value": v0, "lo": float(np.percentile(boot, 2.5)) if len(boot) > 1 else np.nan,
               "hi": float(np.percentile(boot, 97.5)) if len(boot) > 1 else np.nan,
               "se": float(np.std(boot, ddof=1)) if len(boot) > 1 else np.nan, "n": float(u.sum()),
               "n_blocks": int((u > 0).sum())}
        res["reliable"] = bool(res["n"] >= MIN_UNITS[m.kind] and res["n_blocks"] >= 2 and np.isfinite(v0))
        out[m.name] = res

    for name, m in D.metrics.items():
        if m.kind == "ratio":
            num, den, units = D.ratio[name]
            a, b = num[sel], den[sel]
            d0 = W0 @ b
            v0 = float((W0 @ a)[0] / d0[0]) if d0[0] > 0 else np.nan
            db = W @ b
            vb = np.divide(W @ a, db, out=np.full(n_boot, np.nan), where=db > 0)
            pack(m, v0, vb, units)
        elif m.kind == "quantile":
            q, vals, blk = D.quant[name]
            loc = np.full(D.nb, -1)
            loc[sel] = np.arange(nb)
            lb = loc[blk]
            keep = lb >= 0
            v, lb = vals[keep], lb[keep]
            units = np.bincount(blk, minlength=D.nb).astype(float)
            if not len(v):
                pack(m, np.nan, np.array([]), units)
                continue
            v0 = wquantile(v, W0[0][lb], q)
            vb = np.array([wquantile(v, W[r][lb], q) for r in range(n_boot)])
            pack(m, float(v0), vb, units)
        else:
            fn, units = D.custom[name](mask)
            if fn is None:
                pack(m, np.nan, np.array([]), units)
                continue
            full = np.zeros((n_boot, D.nb))
            full[:, sel] = W
            v0 = fn(full0[0])
            vb = np.array([fn(full[r]) for r in range(n_boot)])
            pack(m, float(v0), vb, units)
    return out


# ====================================================================== style dials (fit_styles.py definitions)

DIALS = ["fwd_diag_fight", "reverse_share", "side_hold_ms", "lean_fight", "jumps_per_min", "crouch_per_min", "walk_hidden",
         "burst_median", "aim_height_firing", "hold_angle", "counter_strafe", "aim_error_fight_deg", "reaction_ms", "mp40_share"]


def counter_strafe(full):
    """fit_styles.py counter_strafe: of the strafes let go, the share followed by a strafe the very next tick."""
    n = k = 0
    for _, g in full.groupby(["session_id", "client_id", "seg"], sort=False):
        s = g.side.to_numpy()
        nz = np.flatnonzero(s != 0)
        ends = np.flatnonzero((s[:-1] != 0) & (s[1:] == 0))
        nxt = np.searchsorted(nz, ends + 2)
        ok = nxt < len(nz)
        n += int(ok.sum())
        k += int((nz[nxt[ok]] == ends[ok] + 2).sum())
    return k / n if n else np.nan


def dials(D: MetricData, persons) -> dict:
    """Realised style dials per person, measured as humanbot/fit/fit_styles.py dial_table() measures a human
    (pooled over all of the person's sessions and capture dates)."""
    V = D.frames
    S = runs(V, "side")
    S = S[S.val.ne(0)]
    S["person"] = V.person.to_numpy()[S["first"].to_numpy()]
    acq = D.extra.get("acquisitions")
    out = {}
    for p in persons:
        full = V[V.person.eq(p)]
        d = full[full.eligible]
        if not len(d):
            continue
        lf = d[d.ctx.eq("los_fire")]
        t = d[(d.side != 0) & d.nx_side.notna() & (d.nx_side != d.side)]
        hid = d[d.ctx.eq("hidden_nofire")]
        smg = d.weapon.isin(SMG)
        mins = len(d) / 1200
        r = {"minutes": mins,
             "fwd_diag_fight": lf.action.isin([6, 8]).mean() if len(lf) else np.nan,
             "reverse_share": (t.nx_side == -t.side).mean() if len(t) else np.nan,
             "side_hold_ms": S[S.person.eq(p)].ms.median(),
             "lean_fight": (lf.lean != 0).mean() if len(lf) else np.nan,
             "jumps_per_min": d.jump_start_fs.sum() / max(mins, 1e-9),
             "crouch_per_min": d.crouch_start_fs.sum() / max(mins, 1e-9),
             "walk_hidden": hid.run.eq(0).mean() if len(hid) else np.nan,
             "counter_strafe": counter_strafe(full),
             "burst_median": np.nan, "aim_height_firing": lf.aim_height_fraction.median(),
             "aim_error_fight_deg": lf.aim_total_error.median(),
             "reaction_ms": np.nan,
             "mp40_share": d.weapon.eq("MP40").sum() / max(1, smg.sum())}
        if acq is not None:
            a = acq[acq.person.eq(p)]
            clean = a[~a.attack_before.astype(bool) & (a.err0_total > 2 * a.half_w)]
            r["reaction_ms"] = clean.t_first_attack_press.median() if len(clean) else np.nan
        r["hold_angle"] = np.nan
        if "perception" in D.extra:
            T = D.extra["perception"]
            ps = T.P[T.P.person.eq(p)]
            r["hold_angle"] = float(ps.held.mean()) if len(ps) else np.nan
            # the bursts begun with a body part visible (perception.burst_in_sight)
            sh = T.Sh[T.F.person.to_numpy()[T.Sh.row.to_numpy()] == p]
            g = sh.groupby("burst")
            n = g.size()[g.cls.first().ne("no_part")]
            r["burst_median"] = float(n.median()) if len(n) else np.nan
        out[p] = r
    return out


# ====================================================================== validation against the scripts

def _get(d, *path):
    for k in path:
        if not isinstance(d, dict) or k not in d:
            return None
        d = d[k]
    return d


def validate(allsum: dict, results: Path, tol: float = 1e-3) -> list:
    """Compare pooled (every block) point estimates with the scripts' JSON. Returns [(metric, ours, script)]
    for every checked metric; a mismatch beyond `tol` (relative) means a definition drifted."""
    results = Path(results)
    J = {}
    for f in ("behavior", "combat", "lives", "acquisition", "engagements"):
        p = results / f"{f}.json"
        if p.exists():
            J[f] = json.loads(p.read_text())
    pairs = []

    def chk(metric, ref, scale=1.0):
        if metric in allsum and ref is not None:
            pairs.append((metric, allsum[metric]["value"], float(ref) * scale))

    B = J.get("behavior", {})
    for c in CONTEXTS:
        chk(f"movement.context_share.{c}", _get(B, "context_share", c))
        ap = _get(B, "action_pct_by_context", c) or {}
        if ap:
            chk(f"movement.chord.{c}.pure_strafe", ap.get("left", 0) + ap.get("right", 0), 0.01)
            chk(f"movement.chord.{c}.neutral", ap.get("neutral", 0), 0.01)
            chk(f"movement.chord.{c}.back_any", ap.get("back", 0) + ap.get("back-left", 0) + ap.get("back-right", 0), 0.01)
        chk(f"movement.strafe_reverse.{c}", _get(B, "strafe_end_direct_reverse_share_by_context", c))
        chk(f"movement.still.{c}", _get(B, "usage_by_context", c, "still_lt5"))
        chk(f"view.yaw_speed.{c}.p99", _get(B, "yaw_speed_deg_s_by_context", c, "p99"))
        chk(f"view.yaw_speed.{c}.p50", _get(B, "yaw_speed_deg_s_by_context", c, "p50"))
        chk(f"view.mouse_still.{c}", _get(B, "yaw_speed_deg_s_by_context", c, "zero_share"))
    chk("movement.lean.los_fire", _get(B, "usage_by_context", "los_fire", "lean_any"))
    for a in ("50", "100", "150", "200", "250"):
        chk(f"movement.switch_hazard.{a}ms", _get(B, "strafe_switch_hazard_by_age_los_fire", a, "p_end"))
    for q in ("p25", "p50", "p75", "p90"):
        chk(f"movement.side_hold.los_fire.{q}", _get(B, "side_hold_ms_by_context", "los_fire", q))
    for lab, key in [("96-160", "(96, 160]"), ("288-384", "(288, 384]")]:
        chk(f"movement.approach.los_fire.{lab}", _get(B, "radial_los_fire_by_distance", key, "p_approach_gt40"))
        chk(f"movement.retreat.los_fire.{lab}", _get(B, "radial_los_fire_by_distance", key, "p_retreat_lt_m40"))
    for lab, key in [("0-0.5", "(0.0, 0.5]"), ("4-6", "(4.0, 6.0]"), ("6-10", "(6.0, 10.0]")]:
        chk(f"trigger.hold_los.{lab}", _get(B, "p_attack_held_by_aim_error_halfwidths_los", key))
    chk("trigger.hold_hidden", _get(B, "p_attack_held_hidden"))
    chk("trigger.attack_hold.p50", _get(B, "attack_hold_ms", "p50"))
    chk("trigger.attack_hold.p90", _get(B, "attack_hold_ms", "p90"))
    chk("trigger.attack_gap.p50", _get(B, "attack_gap_ms", "p50"))
    for lab, key in [("0-128", "(0, 128]"), ("256-384", "(256, 384]"), ("512-768", "(512, 768]")]:
        chk(f"aim.error_firing.{lab}", _get(B, "aim_los_firing_by_distance", key, "total_err_med"))
        chk(f"aim.on_body_firing.{lab}", _get(B, "aim_los_firing_by_distance", key, "on_target"))
    for lo, hi in ((90, 135), (135, 180), (180, 360)):
        chk(f"view.turn_peak.{lo}-{hi}.p50", _get(B, "turn_main_sequence", f"({lo}, {hi}]", "peak_deg_s_med"))
    for lab, key in [("0-250", "(-1.0, 250.0]"), ("1000-2000", "(1000.0, 2000.0]"), ("never", "(100000000.0, 2000000000.0]")]:
        chk(f"belief.hidden_yaw_error.{lab}", _get(B, "hidden_crosshair_yaw_error_by_time_since_seen", key, "median"))
        chk(f"belief.within30.{lab}", _get(B, "hidden_crosshair_yaw_error_by_time_since_seen", key, "within30"))
    A = J.get("acquisition", {})
    for t in ("-500", "-200", "0", "200", "1000"):
        chk(f"acquisition.error_curve.{t}ms", _get(A, "median_error_curve_deg", t))
    for t in ("-500", "0", "400", "1450"):
        chk(f"acquisition.attack_curve.{t}ms", _get(A, "attack_prob_curve", t))
    for q in ("p25", "p50", "p75", "p90"):
        chk(f"acquisition.clean_first_press.{q}", _get(A, "clean_t_first_attack_press_ms", q))
    chk("acquisition.clean_t_close.p50", _get(A, "clean_t_close_ms", "p50"))
    chk("acquisition.on_target_at_0", _get(A, "on_target_at_first_visible_tick_pct"), 0.01)
    chk("acquisition.attacking_before", _get(A, "attacking_before_visible_pct"), 0.01)
    C = J.get("combat", {})
    chk("trigger.burst_length.p50", _get(C, "burst_length_shots", "p50"))
    chk("trigger.burst_length.p90", _get(C, "burst_length_shots", "p90"))
    L = J.get("lives", {})
    chk("lives.respawn_delay.p50", _get(L, "respawn_delay_ms", "p50"))
    chk("lives.life_length.p50", _get(L, "life_s_ending_in_death", "p50"))
    for lab in ("1-3", "11-15", "21-25", "26-29"):
        chk(f"reload.post_kill_within_3s.{lab}", _get(L, "post_kill_reload_by_rounds_left", lab, "p_reload_within_3s"))
    chk("reload.post_kill_delay.p50", _get(L, "post_kill_reload_delay_ms", "p50"))
    chk("reload.opponent_dead_share", _get(L, "reload_context_pct", "opponent_dead"), 0.01)
    G = J.get("engagements", {})
    chk("fights.kill_share", _get(G, "outcome_pct", "kill"), 0.01)
    chk("fights.first_hitter_wins", _get(G, "first_hitter_wins_pct"), 0.01)
    chk("fights.first_shooter_wins", _get(G, "first_shooter_wins_pct"), 0.01)
    chk("fights.duration_decisive.p50", _get(G, "duration_ms_kill", "p50"))
    chk("fights.start_distance.p50", _get(G, "start_distance", "p50"))
    return pairs


def mismatches(pairs, tol=2e-3):
    """Pairs whose values differ beyond rounding (scripts round some outputs to 3-4 digits or 0.1 %)."""
    bad = []
    for name, ours, ref in pairs:
        if ours is None or not np.isfinite(ours):
            if ref is not None and np.isfinite(ref):
                bad.append((name, ours, ref))
            continue
        if abs(ours - ref) > max(tol * max(abs(ref), 1e-9), 5e-4 if abs(ref) <= 1 else 1e-6):
            bad.append((name, ours, ref))
    return bad


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description="Print the acceptance statistics of an analysis cache.")
    ap.add_argument("cache")
    ap.add_argument("results", nargs="?")
    ap.add_argument("--persons", nargs="*", help="only these persons' blocks (default: all)")
    ap.add_argument("--n-boot", type=int, default=N_BOOT)
    ap.add_argument("--moh-dir", help="MOHAA folder with the practice maps (perception statistics); default $MOHAA_DIR")
    a = ap.parse_args(argv)
    D = prepare(Path(a.cache), a.results, moh_dir=a.moh_dir)
    mask = np.isin(D.persons(), a.persons) if a.persons else np.ones(D.nb, bool)
    S = summarize(D, mask, n_boot=a.n_boot)
    for name, m in D.metrics.items():
        r = S.get(name, {})
        print(f"{name:55s} {r.get('value', np.nan):12.5g}  [{r.get('lo', np.nan):.5g}, {r.get('hi', np.nan):.5g}]  "
              f"n={r.get('n', 0):.0f} blocks={r.get('n_blocks', 0)}")
    if a.results and not a.persons:
        pairs = validate(S, Path(a.results))
        bad = mismatches(pairs)
        print(f"checked against the scripts' JSON: {len(pairs)}, differ: {len(bad)}")
        for b in bad:
            print("  differs:", b)


if __name__ == "__main__":
    main()
