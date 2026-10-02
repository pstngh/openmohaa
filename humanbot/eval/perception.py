"""Pre-aim, corner and reaction statistics timed from the first visible body part.

The definitions are those of openmohaa-movement's analysis/perception_all.py (REPORT sections 13 and 14). The
sighting machinery (sightings.py), the body-part geometry (parts.py) and the sight tracer (bsp.py) are the data
repo's own modules, imported by path; what is reimplemented here, with the same definitions, is only the
per-sighting tables and the corner trace of perception_all.py, which is a script. validate() checks the pooled
point estimates against perception_all.json.

Two visibility columns:
  * "rebuilt" (people): the column the data repo's unchanged vis_parts.py rebuilds from the practice-map geometry
    (<cache>/vis_parts.parquet, accepted in <results>/vis_parts.json), with its key-modelled leaned eye and lean;
  * "logged" (bots, schema 13): what the logger recorded, ext_vis_parts from the leaned eye origin + ext_eye_ofs,
    and ext_lean_angle. For a bot that is exactly its own perception. It is the column the people's rebuild was
    accepted against on 2026-09-28 (vis_parts_acceptance.md). The rebuild itself is not used on bot captures:
    there it fails the same acceptance test (its onsets come early; the bots' eyes are not the people's).

Every statistic is reduced to per-block values (a block is one player in one session) so that metrics.py's block
bootstrap and compare.py treat them like every other statistic. The appearance point and the corner come from
the future: they are analysis devices for scoring, never bot inputs.

  tables(cache, results, moh_dir) -> Tables     per-sighting and per-tick tables (needs the MOHAA folder for corners)
  register(D, T)                                 adds the "perception.*" metrics to a metrics.MetricData
  validate(allsum, results) -> [(metric, ours, perception_all.json's)]
"""
from __future__ import annotations

import json
import os
import warnings
from dataclasses import dataclass, field
from pathlib import Path
from types import SimpleNamespace

import numpy as np
import pandas as pd

from hbeval import EVAL_CACHE, import_analysis_module, movement_repo

KEY = ["session_id", "client_id", "session_ms"]
SMG = ["MP40", "Thompson"]
BOX = ["bbox_min_x", "bbox_min_y", "bbox_min_z", "bbox_max_x", "bbox_max_y", "bbox_max_z"]
COLS = KEY + ["schema", "map", "seg", "seg_t", "seg_len", "eligible", "person", "name", "capture_date", "opponent_id", "line_of_sight",
              "aim_total_error", "aim_yaw_error", "tgt_half_w_deg", "crosshair_on_opponent", "attack", "view_yaw",
              "view_pitch", "speed_xy", "eye_x", "eye_y", "eye_z", "origin_x", "origin_y", "origin_z", "distance_xyz",
              "ev_shot", "reloading", "clip_ammo"] + BOX
LOOK = 40                    # ticks of hidden history kept (2 s), as perception_all.py
CURVE_TICKS = [-10, -4, -2, 0, 2, 4]       # the error curve from the first part, ticks
CORNER_2S_TICKS = [-40, -20, -10, 0]       # the corner curve on sightings hidden >= 2 s, ticks
MOHAA_ENV = "MOHAA_DIR"


def mohaa_dir(explicit=None) -> Path | None:
    """A MOHAA folder whose main/ holds the pk3 files and 1v1-maps.pk3 (read only), or None."""
    for c in ([explicit] if explicit else []) + [os.environ.get(MOHAA_ENV)]:
        if c and (Path(c) / "main").is_dir():
            return Path(c).resolve()
    return None


def _modules(repo=None):
    # numba caches its compiled kernels next to the source unless told otherwise: keep the data repo untouched.
    # The modules load here under private names, so their kernels must not share a cache with the data repo's
    # scripts run as subprocesses (EVAL_CACHE / "numba"), which load bsp.py as plain "bsp".
    os.environ["NUMBA_CACHE_DIR"] = str(EVAL_CACHE / "numba_inproc")
    repo = repo or movement_repo()
    return SimpleNamespace(common=import_analysis_module("common", repo), sightings=import_analysis_module("sightings", repo),
                           parts=import_analysis_module("parts", repo), bsp=import_analysis_module("bsp", repo))


def eye_params(results: Path | None = None) -> dict:
    """The eye model of the data repo's vis_parts.py (results/vis_parts.json; default: its committed results):
    R and k place the roll of a leaning body's upper parts."""
    for c in ([Path(results) / "vis_parts.json"] if results else []) + (
            [movement_repo() / "analysis" / "results" / "vis_parts.json"] if movement_repo() else []):
        if c.exists():
            return json.loads(c.read_text())["eye_model"]["params"]
    raise FileNotFoundError("vis_parts.json (the eye model) not found")


def column_for(cache: Path, results: Path) -> str:
    """"rebuilt" when the cache has an accepted rebuilt column, else "logged" (schema-13 captures)."""
    vp = Path(results) / "vis_parts.json"
    if (Path(cache) / "vis_parts.parquet").exists() and vp.exists():
        if (json.loads(vp.read_text()).get("decision") or {}).get("column_used_for_analysis") == "keys_lean_parts":
            return "rebuilt"
    return "logged"


def available(cache: Path, results: Path, moh, column: str = "rebuilt") -> str | None:
    """None when the tables can be built, else why not."""
    if movement_repo() is None:
        return "openmohaa-movement is not available"
    if mohaa_dir(moh) is None:
        return f"no MOHAA folder (pass --moh-dir or set ${MOHAA_ENV}): the corner trace needs the practice-map BSPs"
    if column == "logged":
        F = pd.read_parquet(Path(cache) / "features.parquet", columns=["eligible", "schema"])
        if not F.eligible.any() or F.loc[F.eligible, "schema"].min() < 13:
            return "the logged body-part column needs schema-13 captures (ext_vis_parts)"
        return None
    if not (Path(cache) / "vis_parts.parquet").exists() or not (Path(results) / "vis_parts.json").exists():
        return "vis_parts.parquet / vis_parts.json missing (run the data repo's vis_parts.py on this cache)"
    vp = json.loads((Path(results) / "vis_parts.json").read_text())
    if (vp.get("decision") or {}).get("column_used_for_analysis") != "keys_lean_parts":
        return "vis_parts.json: the rebuilt body-part column did not pass its acceptance test on this cache"
    return None


@dataclass
class Tables:
    F: pd.DataFrame = None                 # KEY, person per row (for the blocks)
    P: pd.DataFrame = None                 # part sightings (sightings.py Context.sightings + the hidden window)
    Cs: pd.DataFrame = None                # centroid sightings
    CT: pd.DataFrame = None                # control ticks: fully hidden, still hidden 500 ms later
    T: pd.DataFrame = None                 # eligible ticks with the visibility classes
    Sh: pd.DataFrame = None                # eligible SMG shot ticks
    curve2s: dict = field(default_factory=dict)   # tick -> yaw to the corner, sightings hidden >= 2 s with a corner
    eye: dict = field(default_factory=dict)
    column: str = "rebuilt"


def tables(cache: Path, results: Path, moh, column: str = "rebuilt", verbose: bool = True, corners: bool = True) -> Tables:
    """perception_all.py's events() on one visibility column: "rebuilt" (vis_parts.py), or "logged" (ext_vis_parts,
    with the logged leaned eye and lean angle; schema 13). corners=False skips the corner trace (no MOHAA folder
    needed; the corner columns are then NaN)."""
    say = print if verbose else (lambda *a, **k: None)
    cache, results = Path(cache), Path(results)
    mods = _modules()
    wrap = mods.common.wrap
    Seqmod = mods.sightings
    PRE, HOLD, POST = Seqmod.PRE, Seqmod.HOLD, Seqmod.POST
    view_basis, in_frustum, part_points = mods.parts.view_basis, mods.parts.in_frustum, mods.parts.part_points
    EYE = eye_params(results if column == "rebuilt" else None)
    logged = ["ext_vis_parts", "ext_lean_angle", "ext_eye_ofs_x", "ext_eye_ofs_y", "ext_eye_ofs_z"]
    F = pd.read_parquet(cache / "features.parquet", columns=COLS + (logged if column == "logged" else []))
    F = F.sort_values(KEY, kind="stable").reset_index(drop=True)
    if column == "logged":
        # the logged leaned eye and lean; the roll pivot sits R under the stance's eye (the unleaned view height)
        F = F.assign(leye_x=F.origin_x + F.ext_eye_ofs_x, leye_y=F.origin_y + F.ext_eye_ofs_y,
                     leye_z=F.origin_z + F.ext_eye_ofs_z, lean_sim=F.ext_lean_angle, eye_h=F.eye_z - F.origin_z)
    else:
        F = F.merge(pd.read_parquet(cache / "vis_parts.parquet"), on=KEY, how="left", validate="one_to_one")
    F["block"] = F.session_id.astype(str) + "|" + F.client_id.astype(str)
    N = len(F)
    el = F.eligible.to_numpy()
    if column == "logged":
        vparts = np.where(el & F.schema.ge(13).to_numpy(), F.ext_vis_parts.fillna(0).to_numpy(), 0).astype(int)
    else:
        vparts = np.where(el, F.vis_parts_rebuilt.fillna(0).to_numpy(), 0).astype(int)

    # ---- per-tick joins
    E = pd.read_parquet(cache / "events.parquet")
    Dm = E[E.event.eq("damage") & E.actor_bot.eq(0) & E.target_bot.eq(0) & E.actor_id.ne(E.target_id) & E.means_of_death.eq(18)]
    hit = Dm.groupby(["session_id", "actor_id", "session_ms"]).size()
    hit.index.names = KEY
    mi = pd.MultiIndex.from_frame(F[KEY])
    F["hit"] = hit.reindex(mi).fillna(0).gt(0).to_numpy()
    S = E[E.event.eq("shot") & E.actor_bot.eq(0) & E.weapon.isin(SMG)].groupby(["session_id", "actor_id", "session_ms"]).line_of_sight.max()
    S.index.names = KEY
    F["smg_shot_los"] = S.reindex(mi).to_numpy()
    F["ev_shot"] = F.ev_shot.fillna(0)
    del E, Dm
    oi = pd.Series(np.arange(N), index=mi).reindex(pd.MultiIndex.from_arrays([F.session_id, F.opponent_id, F.session_ms])).to_numpy()
    oi = np.where(np.isnan(oi), 0, oi).astype(np.int64)
    col = lambda c: F[c].to_numpy()
    opp_org = np.stack([col("origin_x"), col("origin_y"), col("origin_z")], 1)[oi]
    opp_min = np.stack([col("bbox_min_x"), col("bbox_min_y"), col("bbox_min_z")], 1)[oi]
    opp_max = np.stack([col("bbox_max_x"), col("bbox_max_y"), col("bbox_max_z")], 1)[oi]
    cen = opp_org + (opp_min + opp_max) / 2
    leye = np.stack([col("leye_x"), col("leye_y"), col("leye_z")], 1).astype(float)
    fwd, left, up = view_basis(col("view_pitch"), col("view_yaw"))
    ex, ey, vy = col("eye_x"), col("eye_y"), col("view_yaw")
    d_le = cen - leye
    with np.errstate(invalid="ignore", divide="ignore"):
        tot_le = np.degrees(np.arccos(np.clip((fwd * d_le).sum(1) / np.linalg.norm(d_le, axis=1), -1, 1)))
    yaw_le = wrap(np.degrees(np.arctan2(d_le[:, 1], d_le[:, 0])) - vy)
    C = Seqmod.Context(F, F.hit.to_numpy(), tot_le, yaw_le)
    Q = C.Q
    los, att, yerr = C.los, C.att, C.yerr
    lean_abs = np.abs(col("lean_sim"))
    ks = list(range(-LOOK, 1))
    MAPS = {m: mods.bsp.Map(mods.bsp.PakFS(mohaa_dir(moh)), m, ck) for m, ck in mods.bsp.PRACTICE_CHECKSUMS.items()} if corners else {}
    mapv = F["map"].to_numpy()
    opp_lean, opp_pivot, opp_yaw = col("lean_sim")[oi], col("eye_h")[oi] - EYE["R"], vy[oi]
    pitch_v = col("view_pitch")

    def parts_at(rows):
        return part_points(opp_org[rows], opp_min[rows, 2], opp_max[rows, 2], opp_yaw[rows], opp_lean[rows], opp_pivot[rows], EYE["k"])

    def per_map(fn, e, q, mrow, fill):
        res = np.full(len(e), fill)
        for m, Mp in MAPS.items():
            r = mrow == m
            if r.any():
                res[r] = getattr(Mp, fn)(e[r], q[r])
        return res

    def angles_to(points, rows):
        d = points - leye[rows]
        with np.errstate(invalid="ignore", divide="ignore"):
            tot = np.degrees(np.arccos(np.clip((fwd[rows] * d).sum(1) / np.linalg.norm(d, axis=1), -1, 1)))
        yaw = wrap(np.degrees(np.arctan2(d[:, 1], d[:, 0])) - vy[rows])
        pit = np.degrees(np.arctan2(-d[:, 2], np.hypot(d[:, 0], d[:, 1]))) - pitch_v[rows]
        return tot, yaw, pit

    def corners(pi, valid):
        """perception_all.py corners(): the edge of cover each enemy comes out from, traced from the viewer's
        leaned eye at the first part back along the enemy's last 500 ms, and the crosshair's angles to it."""
        n = len(pi)
        E0, mp = leye[pi], mapv[pi]
        Qb = np.stack([parts_at(pi - b) for b in range(PRE + 1)], 1)
        vis0 = np.stack([in_frustum(E0, Qb[:, 0, j], fwd[pi], left[pi], up[pi]) & per_map("sight", E0, Qb[:, 0, j], mp, False)
                         for j in range(5)], 1)
        occ = np.zeros((n, PRE + 1, 5), bool)
        for b in range(1, PRE + 1):
            for j in range(5):
                occ[:, b, j] = ~per_map("sight", E0, Qb[:, b, j], mp, False)
        bstar = np.where(occ[:, 1:].any(1), occ[:, 1:].argmax(1) + 1, 99)
        bstar = np.where(vis0, bstar, 99)
        cross_t = np.full((n, 5), np.nan)
        hid_pt = np.full((n, 5, 3), np.nan)
        for j in range(5):
            r = np.flatnonzero(bstar[:, j] <= PRE)
            a_, b_ = Qb[r, bstar[r, j], j], Qb[r, bstar[r, j] - 1, j]
            lo, hi = np.zeros(len(r)), np.ones(len(r))
            for _ in range(14):
                mid = (lo + hi) / 2
                ok = per_map("sight", E0[r], a_ + mid[:, None] * (b_ - a_), mp[r], False)
                hi, lo = np.where(ok, mid, hi), np.where(ok, lo, mid)
            cross_t[r, j] = -bstar[r, j] + (lo + hi) / 2
            hid_pt[r, j] = a_ + lo[:, None] * (b_ - a_)
        found = np.isfinite(cross_t).any(1) & bool(MAPS)
        fp = np.where(found, np.nanargmin(np.where(np.isfinite(cross_t), cross_t, np.inf), 1), 0)
        H = hid_pt[np.arange(n), fp]
        K = np.full((n, 3), np.nan)
        if found.any():
            K[found] = E0[found] + per_map("first_block", E0[found], H[found], mp[found], 1.0)[:, None] * (H[found] - E0[found])
        curve = {}
        A = cen[pi]
        for k in sorted(set([-10, -5, 0] + CORNER_2S_TICKS)):
            rows = np.clip(pi + k, 0, N - 1)
            ok = valid[:, ks.index(k)] & found
            tK, yK, pK = angles_to(K, rows)
            _, yA, _ = angles_to(A, rows)
            _, yE, _ = angles_to(cen[rows], rows)
            side = np.sign(wrap(yA - yK))
            curve[k] = {"tot_K": np.where(ok, tK, np.nan), "yaw_K": np.where(ok, np.abs(yK), np.nan),
                        "pitch_K": np.where(ok, pK, np.nan), "lead": np.where(ok, -yK * side, np.nan),
                        "yaw_A": np.where(ok, np.abs(yA), np.nan), "yaw_E": np.where(ok, np.abs(yE), np.nan)}
        t = pd.DataFrame({"corner_found": found, "corner_dist": np.linalg.norm(K - E0, axis=1)})
        for k, lbl in [(-10, "m500"), (-5, "m250"), (0, "0")]:
            for nme, v in curve[k].items():
                t[f"{nme}_{lbl}"] = v
        fin = lambda c: np.isfinite(t[c].to_numpy())
        t["near5_K_m500"] = np.where(fin("tot_K_m500"), t.tot_K_m500 <= 5, np.nan)
        t["on_edge_m500"] = np.where(fin("lead_m500"), np.abs(t.lead_m500) <= 2, np.nan)
        t["open_side_m500"] = np.where(fin("lead_m500"), t.lead_m500 > 2, np.nan)
        t["cover_side_m500"] = np.where(fin("lead_m500"), t.lead_m500 < -2, np.nan)
        t["closer_K_than_E_m500"] = np.where(fin("yaw_K_m500"), t.yaw_K_m500 < t.yaw_E_m500, np.nan)
        return t, curve

    # ---- the tables of one visibility column (perception_all.py events())
    vp = (vparts > 0) & C.el
    hid = C.el & ~los & ~vp
    hid_before = Q.before(hid)
    vp_from, los_from = Q.run_from(vp), Q.run_from(los)
    P = C.sightings((hid_before >= PRE) & vp & (vp_from >= HOLD), vp_from, hid_before, lean_abs)
    Cs = C.sightings((Q.before(C.el & ~los) >= PRE) & los & (los_from >= HOLD), los_from, Q.before(C.el & ~los), lean_abs)
    pi = P.row.to_numpy()
    ci = Cs.row.to_numpy()
    vb = Q.before(vp)[ci]
    lag = Q.first_in_bout(vp, ci, np.minimum(los_from[ci], POST))
    Cs["part_lead_ms"] = np.where(vb > 0, vb * 50.0, np.where(vp[ci], 0.0, -lag))
    Cs["part_first"] = vb > 0
    valid = np.asarray(ks)[None, :] >= -hid_before[pi][:, None]
    appear = Seqmod.appearance_yaw(Q, pi, ks, ex, ey, vy, cen[pi, 0], cen[pi, 1])
    W = {"yaw_appear": np.where(valid, appear, np.nan), "speed": np.where(valid, Q.win(col("speed_xy"), pi, ks), np.nan)}
    e5, e0 = Q.win(yerr, pi, [-10])[:, 0], yerr[pi]
    s = np.sign(e5)
    v5 = Q.win(vy, pi, [-10])[:, 0]
    dv = wrap(vy[pi] - v5)
    with warnings.catch_warnings():
        warnings.simplefilter("ignore", RuntimeWarning)
        P["yaw_err_m500"] = np.abs(e5)
        P["appear_m500"] = W["yaw_appear"][:, ks.index(-10)]
        P["view_turn_toward"] = s * dv
        P["view_turn_abs"] = np.abs(dv)
        P["speed_m400_m200"] = np.nanmedian(W["speed"][:, ks.index(-8):ks.index(-4)], axis=1)
        P["speed_m2000_m1000"] = np.nanmedian(W["speed"][:, ks.index(-40):ks.index(-20)], axis=1)
    P["held"] = P.appear_m500.le(5)
    P["closer_to_appearance_point"] = P.appear_m500 < P.yaw_err_m500
    P["long"] = hid_before[pi] >= LOOK
    # control: fully hidden ticks that stay hidden for the next 500 ms
    ctl = np.flatnonzero(hid & (Q.run_from(hid) >= 11))
    fut = ctl + 10
    ctl_true = np.abs(yerr[ctl])
    ctl_future = np.abs(wrap(np.degrees(np.arctan2(cen[fut, 1] - ey[ctl], cen[fut, 0] - ex[ctl])) - vy[ctl]))
    CT = pd.DataFrame({"row": ctl, "closer": ctl_future < ctl_true})
    K, curve = corners(pi, valid)
    P = pd.concat([P, K.set_index(P.index)], axis=1)
    curve2s = {k: curve[k]["yaw_K"][(P.corner_found & P.long).to_numpy()] for k in CORNER_2S_TICKS}
    rows2s = pi[(P.corner_found & P.long).to_numpy()]
    # visibility classes of the eligible ticks and the SMG shots
    Tk = pd.DataFrame({"row": np.flatnonzero(el)})
    c_vis = F.line_of_sight.eq(1).to_numpy()[el]
    p_vis = vparts[el] > 0
    Tk["part_only"] = p_vis & ~c_vis
    # fire held at an enemy no part of which is visible, while the weapon could fire (the trigger's hidden side)
    Tk["no_part_loaded"] = ~p_vis & ~F.reloading.to_numpy(bool)[el] & (F.clip_ammo.to_numpy()[el] > 0)
    Tk["attack"] = F.attack.to_numpy(bool)[el]
    shot = F.smg_shot_los.notna().to_numpy()[el]
    Sh = pd.DataFrame({"row": Tk.row.to_numpy()[shot], "hit": F.hit.to_numpy()[el][shot],
                       "cls": np.select([c_vis[shot], p_vis[shot]], ["centre_visible", "part_only"], "no_part")})
    say(f"perception: {len(P)} part sightings, {len(Cs)} centroid sightings, {int(P.corner_found.sum())} corners "
        f"({column} column)")
    T = Tables(F=F[KEY + ["person", "name", "capture_date"]].copy(), P=P, Cs=Cs, CT=CT, T=Tk, Sh=Sh,
               curve2s={"ticks": curve2s, "rows": rows2s},
               eye=EYE, column=column)
    return T


# ---------------------------------------------------------------------- metrics

def _label_t(t):
    return f"{t * 50:+d} ms"


def register(D, T: Tables, prefix: str = "perception"):
    """Adds the perception statistics to a metrics.MetricData; every row of these tables is a duel row, so its
    block already exists."""
    nb0 = D.nb
    F = T.F

    def blk(rows):
        rows = np.asarray(rows, dtype=np.int64)
        if not len(rows):
            return np.zeros(0, np.int64)
        return D.block_ids_frame(F.iloc[rows][KEY + ["person"]].reset_index(drop=True))

    sec, pre = "perception", prefix
    P, Cs = T.P, T.Cs
    bp = blk(P.row)
    # visibility and shots
    bt = blk(T.T.row)
    D.add_ratio(f"{pre}.part_only_share", "Share of duel time with a body part on screen and the centroid hidden", sec, "share",
                bt, T.T.part_only.to_numpy())
    m = T.T.no_part_loaded.to_numpy()
    D.add_ratio(f"{pre}.fire_held_no_part", "Fire held with no body part of the enemy visible (loaded, not reloading)", sec,
                "share", bt[m], T.T.attack.to_numpy()[m])
    bs = blk(T.Sh.row)
    for c, lab in (("part_only", "a part on screen, centroid hidden"), ("centre_visible", "centroid visible")):
        m = T.Sh.cls.eq(c).to_numpy()
        D.add_ratio(f"{pre}.smg_hit.{c}", f"SMG shots that hit, {lab}", sec, "share", bs[m], T.Sh.hit.to_numpy()[m], perf=True)
    bc = blk(Cs.row)
    D.add_ratio(f"{pre}.centroid_sighting_part_first", "Centroid sightings with a body part on screen first", sec, "share",
                bc, Cs.part_first.to_numpy())
    m = Cs.part_first.to_numpy()
    D.add_quant(f"{pre}.part_lead", "Lead of the first part over the centroid", sec, "ms", bc[m],
                Cs.part_lead_ms.to_numpy()[m], (("p50", .5),))
    # aim error around the first part
    for t in CURVE_TICKS:
        D.add_quant(f"{pre}.error_from_part.{t * 50}ms", f"Median aim error at {_label_t(t)} from the first visible part",
                    sec, "deg", bp, P[f"e{t * 50}"].to_numpy(), (("", .5),), perf=t > 0)
    # reaction from the first part
    D.add_ratio(f"{pre}.reaction.attack_before", "Fire already held on the tick before the first part (prefire)", sec,
                "share", bp, P.attack_before.to_numpy())
    cl = P.clean.to_numpy()
    tp = P.t_press.to_numpy()
    D.add_quant(f"{pre}.reaction.clean_first_press", "Reaction: first press after a clean sighting, from the first part",
                sec, "ms", bp[cl], tp[cl], (("p50", .5),))
    ok = cl & np.isfinite(tp)
    D.add_ratio(f"{pre}.reaction.clean_first_press_mean", "Reaction: mean first press after a clean sighting, from the first part",
                sec, "ms", bp[ok], tp[ok])
    D.add_quant(f"{pre}.reaction.clean_t_close", "Crosshair within half-width+1.5 deg after a clean sighting, from the first part",
                sec, "ms", bp[cl], P.t_close.to_numpy()[cl], (("p50", .5),), perf=True)
    D.add_quant(f"{pre}.reaction.first_hit", "First hit after the first part", sec, "ms", bp, P.t_first_hit.to_numpy(),
                (("p50", .5),), perf=True)
    D.add_ratio(f"{pre}.reaction.no_hit", "Part sightings whose visible bout ends without a hit", sec, "share", bp,
                P.t_first_hit.isna().to_numpy(), perf=True)
    # while hidden, 500 ms before the first part
    D.add_quant(f"{pre}.preaim.yaw_to_enemy.m500", "Yaw error to the hidden enemy 500 ms before the first part", sec, "deg",
                bp, P.yaw_err_m500.to_numpy(), (("", .5),))
    D.add_quant(f"{pre}.preaim.yaw_to_appearance.m500", "Yaw error to where the enemy will appear, 500 ms before the first part",
                sec, "deg", bp, P.appear_m500.to_numpy(), (("", .5),))
    D.add_ratio(f"{pre}.preaim.parked.m500", "Crosshair parked within 5 deg of where the enemy will appear, 500 ms before",
                sec, "share", bp, P.held.to_numpy())
    D.add_ratio(f"{pre}.preaim.closer_to_appearance.m500", "Crosshair closer to where the enemy will appear than to the enemy, -500 ms",
                sec, "share", bp, P.closer_to_appearance_point.to_numpy())
    D.add_quant(f"{pre}.preaim.view_turn_toward", "View turn toward the enemy over the last 500 ms before the first part",
                sec, "deg", bp, P.view_turn_toward.to_numpy(), (("", .5),))
    D.add_quant(f"{pre}.preaim.view_turn_abs", "View turn over the last 500 ms before the first part", sec, "deg", bp,
                P.view_turn_abs.to_numpy(), (("", .5),))
    D.add_quant(f"{pre}.preaim.speed_m400_m200", "Speed 400-200 ms before the first part", sec, "u/s", bp,
                P.speed_m400_m200.to_numpy(), (("", .5),))
    D.add_quant(f"{pre}.preaim.speed_m2000_m1000", "Speed 2-1 s before the first part", sec, "u/s", bp,
                P.speed_m2000_m1000.to_numpy(), (("", .5),))
    D.add_ratio(f"{pre}.preaim.control_closer_to_future", "Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms",
                sec, "share", blk(T.CT.row), T.CT.closer.to_numpy())
    # the corner the enemy comes out from (sightings with a corner found)
    Fk = P.corner_found.to_numpy()
    bk = bp[Fk]
    K = P[Fk]
    for key, lab in (("yaw_K_m500", "Yaw error to the corner"), ("tot_K_m500", "Angle to the corner"),
                     ("pitch_K_m500", "Pitch to the corner (- = corner above the crosshair)"),
                     ("lead_m500", "Crosshair past the corner's edge (+ open side, - cover)"),
                     ("yaw_A_m500", "Yaw error to the appearance point (sightings with a corner)"),
                     ("yaw_E_m500", "Yaw error to the hidden enemy (sightings with a corner)")):
        nm = {"yaw_K_m500": "yaw", "tot_K_m500": "angle", "pitch_K_m500": "pitch", "lead_m500": "lead",
              "yaw_A_m500": "yaw_to_appearance", "yaw_E_m500": "yaw_to_enemy"}[key]
        D.add_quant(f"{pre}.corner.{nm}.m500", f"{lab}, 500 ms before the first part", sec, "deg", bk, K[key].to_numpy(),
                    (("", .5),))
    for key, nm, lab in (("near5_K_m500", "within5", "Crosshair within 5 deg of the corner"),
                         ("closer_K_than_E_m500", "closer_than_enemy", "Crosshair closer to the corner than to the enemy"),
                         ("cover_side_m500", "cover_side", "Crosshair on the cover side of the corner (> 2 deg)"),
                         ("on_edge_m500", "on_edge", "Crosshair within 2 deg of the corner's edge"),
                         ("open_side_m500", "open_side", "Crosshair out past the corner on the open side (> 2 deg)")):
        v = K[key].to_numpy(dtype=float)
        ok = np.isfinite(v)
        D.add_ratio(f"{pre}.corner.{nm}.m500", f"{lab}, 500 ms before the first part", sec, "share", bk[ok], v[ok])
    D.add_quant(f"{pre}.corner.angle.0ms", "Angle to the corner at the first part", sec, "deg", bk, K.tot_K_0.to_numpy(), (("", .5),))
    D.add_quant(f"{pre}.corner.distance", "Distance from the eye to the corner", sec, "u", bk, K.corner_dist.to_numpy(),
                (("p50", .5),))
    b2 = blk(T.curve2s["rows"])
    for k in CORNER_2S_TICKS:
        D.add_quant(f"{pre}.corner_2s.yaw.{k * 50}ms", f"Yaw error to the corner at {_label_t(k)} (sightings hidden >= 2 s)",
                    sec, "deg", b2, T.curve2s["ticks"][k], (("", .5),))
    if D.nb != nb0:
        raise RuntimeError(f"perception tables brought {D.nb - nb0} blocks that metrics.prepare did not see")


# perception metric -> (path in perception_all.json to a {"pooled": {"est": ...}} dict or a pooled number, scale, digits)
REF = {
    "perception.part_only_share": (["visibility_pct_of_duel_time", "part_only"], .01, 2),
    "perception.smg_hit.part_only": (["smg_shots", "hit_pct", "part_only"], .01, 2),
    "perception.smg_hit.centre_visible": (["smg_shots", "hit_pct", "centre_visible"], .01, 2),
    "perception.centroid_sighting_part_first": (["centre_sighting_part_first_pct"], .01, 1),
    "perception.part_lead.p50": (["centre_sighting_part_lead_ms", "part_first_median"], 1, 0),
    "perception.reaction.attack_before": (["reaction_from_part", "attack_before_pct"], .01, 1),
    "perception.reaction.clean_first_press.p50": (["reaction_from_part", "clean_first_press_ms"], 1, 0),
    "perception.reaction.clean_first_press_mean": (["reaction_from_part", "clean_first_press_mean_ms"], 1, 0),
    "perception.reaction.clean_t_close.p50": (["reaction_from_part", "clean_close_ms"], 1, 0),
    "perception.reaction.first_hit.p50": (["reaction_from_part", "first_hit_ms"], 1, 0),
    "perception.reaction.no_hit": (["reaction_from_part", "no_hit_in_bout_pct"], .01, 1),
    "perception.preaim.yaw_to_enemy.m500": (["last_500ms_before_first_part", "yaw_err_true_m500"], 1, 2),
    "perception.preaim.yaw_to_appearance.m500": (["last_500ms_before_first_part", "yaw_err_to_appearance_point_m500"], 1, 2),
    "perception.preaim.parked.m500": (["last_500ms_before_first_part", "crosshair_within_5deg_of_appearance_point_at_m500_pct"], .01, 1),
    "perception.preaim.closer_to_appearance.m500": (["last_500ms_before_first_part",
                                                     "crosshair_closer_to_appearance_point_than_to_enemy_at_m500_pct"], .01, 1),
    "perception.preaim.view_turn_toward": (["last_500ms_before_first_part", "view_turn_toward_enemy_deg"], 1, 2),
    "perception.preaim.view_turn_abs": (["last_500ms_before_first_part", "view_turn_abs_deg"], 1, 2),
    "perception.preaim.speed_m400_m200": (["last_500ms_before_first_part", "speed_m400_m200_ups"], 1, 0),
    "perception.preaim.speed_m2000_m1000": (["last_500ms_before_first_part", "speed_m2000_m1000_ups"], 1, 0),
    "perception.preaim.control_closer_to_future": (["control_hidden_no_sighting_next_500ms",
                                                    "crosshair_closer_to_enemy_500ms_later_than_to_enemy_now_pct"], .01, 2),
    "perception.corner.yaw.m500": (["corner_check", "m500", "yaw_to_corner_deg"], 1, 2),
    "perception.corner.angle.m500": (["corner_check", "m500", "angle_to_corner_deg"], 1, 2),
    "perception.corner.pitch.m500": (["corner_check", "m500", "pitch_to_corner_deg"], 1, 2),
    "perception.corner.lead.m500": (["corner_check", "m500", "lead_deg"], 1, 2),
    "perception.corner.yaw_to_appearance.m500": (["corner_check", "m500", "yaw_to_appearance_point_deg"], 1, 2),
    "perception.corner.yaw_to_enemy.m500": (["corner_check", "m500", "yaw_to_enemy_deg"], 1, 2),
    "perception.corner.within5.m500": (["corner_check", "m500", "within_5deg_of_corner_pct"], .01, 1),
    "perception.corner.closer_than_enemy.m500": (["corner_check", "m500", "closer_to_corner_than_to_enemy_pct"], .01, 1),
    "perception.corner.cover_side.m500": (["corner_check", "m500", "cover_side_pct"], .01, 1),
    "perception.corner.on_edge.m500": (["corner_check", "m500", "on_edge_within_2deg_pct"], .01, 1),
    "perception.corner.open_side.m500": (["corner_check", "m500", "open_side_pct"], .01, 1),
    "perception.corner.angle.0ms": (["corner_check", "onset", "angle_to_corner_deg"], 1, 2),
    "perception.corner.distance.p50": (["corner_check", "corner_distance_u"], 1, 0),
}
for _t in CURVE_TICKS:
    REF[f"perception.error_from_part.{_t * 50}ms"] = (["error_curve_from_part_deg", str(_t * 50)], 1, 2)
for _t in CORNER_2S_TICKS:
    REF[f"perception.corner_2s.yaw.{_t * 50}ms"] = (["corner_check", "curve_hidden_2s", str(_t * 50), "pooled", "yaw_to_corner"], 1, 2)


def validate(allsum: dict, results: Path) -> list:
    """[(metric, ours, perception_all.json's)] for every pooled estimate the script wrote; the script rounds, so a
    value reproduces when it is within half a unit of the last digit kept."""
    p = Path(results) / "perception_all.json"
    if not p.exists():
        return []
    R = json.loads(p.read_text())
    pairs = []
    for name, (path, scale, digits) in REF.items():
        d = R
        for k in path:
            d = d.get(k) if isinstance(d, dict) else None
        if isinstance(d, dict):
            d = (d.get("pooled") or {}).get("est") if isinstance(d.get("pooled"), dict) else d.get("pooled")
        if name in allsum and d is not None:
            pairs.append((name, allsum[name]["value"], float(d) * scale, 0.5 * 10 ** -digits * scale * 1.0001))
    return pairs


def mismatches(pairs) -> list:
    bad = []
    for name, ours, ref, tol in pairs:
        if ours is None or not np.isfinite(ours) or abs(ours - ref) > tol:
            bad.append((name, ours, ref))
    return bad

