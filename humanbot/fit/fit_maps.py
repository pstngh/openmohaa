"""Per-map human priors (aggregates only): route graph, occupancy, dwell, spawns, LOS.

Cells are 32x32 columns split into floor levels (z clusters inside each column). A
cell is kept only with at least MIN_ROWS duel rows from at least MIN_SESSIONS
sessions.

AREA_MAPS are practice maps of objective-map areas recorded outside the duel cohort
(dm/brownffa, the Bridge of obj/obj_team4, and dm/flag, part of V2): the whole objective
map is in them, and people play only the area. Their rows use the duel definitions
without the map list (both people alive, normal physics: the data repo's practice_areas.py),
and a cell or spawn needs MIN_SESSIONS player-sessions (two people, or one person in two
sessions): dm/flag is one recorded session, dm/brownffa mostly one. FULL_MAPS are whole deathmatch maps recorded in 1v1
outside the practice list (dm/mohdm6, Stalingrad, one of the three maps the owner wants bots for): their rows and sources
are counted the way AREA_MAPS' are. The transition kernel counts moves between kept cells; its dependence on
where the enemy is (for belief prediction) is one coefficient per distance band:

  P(next = k | cell, enemy direction) ~ count(cell -> k) * exp(beta[band] * cos(dir(k) - dir(enemy)))

LOS between cells is the fraction of duel ticks with centroid line of sight when the
two players stood in those cells (unordered pair, >= 2 ticks).

DROP_MAPS also get `drops`: the falls people took (the last tick on the ground before an air run and the first after,
more than DROP_MIN lower), one per take-off cell with its most common landing cell (fit_drops). On dm/mohdm6 people run off the
spawn buildings' upper floors into the street (falls of 400-650 u, past the engine paths' 400 u); the owner
(2026-10-25): that is a Stalingrad habit and must not touch other maps, so only DROP_MAPS get them.
"""
import json
import sys
from pathlib import Path

import numpy as np
import pandas as pd
from scipy import optimize

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

CELL = 32.0
LEVEL_GAP = 40.0
MIN_ROWS = 20
MIN_SESSIONS = 2
KERNEL_DIST_EDGES = [0.0, 384.0, 768.0]
MAPS = ["dm/crnodoors", "dm/main", "dm/vents", "dm/downladder"]
AREA_MAPS = ["dm/brownffa", "dm/flag"]
FULL_MAPS = ["dm/mohdm6"]
DROP_MAPS = ["dm/mohdm6"]
DROP_MIN = 96.0
DROP_NEAR = 96.0
DROP_CLIMB = 24.0


def column_levels(ix, iy, z):
    """Cluster z inside each (ix, iy) column; returns level id per row and level z table."""
    df = pd.DataFrame({"ix": ix, "iy": iy, "z": z})
    df["zr"] = np.round(df.z / 4) * 4
    level = np.zeros(len(df), int)
    levels = {}
    for (cx, cy), d in df.groupby(["ix", "iy"], sort=False):
        zs = np.sort(d.zr.unique())
        cuts = np.flatnonzero(np.diff(zs) > LEVEL_GAP)
        bounds = np.split(zs, cuts + 1)
        centers = [float(np.median(d.z[(d.zr >= b[0]) & (d.zr <= b[-1])])) for b in bounds]
        edges = [(b[0], b[-1]) for b in bounds]
        lid = np.zeros(len(d), int)
        for i, (lo, hi) in enumerate(edges):
            lid[(d.zr >= lo) & (d.zr <= hi).to_numpy()] = i
        level[d.index.to_numpy()] = lid
        for i, c in enumerate(centers):
            levels[(cx, cy, i)] = c
    return level, levels


def cell_keys(x, y, z):
    ix = np.floor(np.asarray(x) / CELL).astype(int)
    iy = np.floor(np.asarray(y) / CELL).astype(int)
    lv, levels = column_levels(ix, iy, np.asarray(z))
    return ix, iy, lv, levels


def nearest_level(levels, ix, iy, z):
    """Level id of the closest kept level in a column (for positions outside the fitting rows)."""
    best, bd = -1, 1e9
    for lid in range(8):
        c = levels.get((ix, iy, lid))
        if c is None:
            continue
        if abs(c - z) < bd:
            best, bd = lid, abs(c - z)
    return best


def fit_drops(D, key_of, levels, cx, cy, cz):
    """People's falls on the map's rows (sorted; every living tick, not only the duel ticks: a fall is a way about the
    map, and a fall while the enemy was dead broke into pieces), as route edges. key_of maps (ix, iy, level) to the kept
    cells, levels their floor heights, and cx, cy, cz are the kept cells' centres.

    A fall's take-off cell is the cell it left from, or, when people left from a spot too brief to be a cell (the top of
    dm/mohdm6's ledge railing, which they jump onto), the cell within DROP_NEAR they climbed from (up to 72 u below);
    its landing cell likewise (within 48 u of height). A fall counts when the falls within DROP_NEAR of its take-off (on
    its level) number at least 3 from MIN_SESSIONS sources. One drop per take-off cell, with its most common landing cell,
    the median take-off and landing points, and 1 when the take-off is more than DROP_CLIMB above the cell's floor (the
    bot jumps onto it)."""
    key = D.groupby(["session_id", "client_id", "seg"], sort=False).ngroup().to_numpy()
    ms, grd = D.session_ms.to_numpy(), D.on_ground.astype(bool).to_numpy()
    z, src = D.origin_z.to_numpy(), D.src.to_numpy()
    xyz = D[["origin_x", "origin_y", "origin_z"]].to_numpy()

    def cell_of(p):
        ix, iy = int(np.floor(p[0] / CELL)), int(np.floor(p[1] / CELL))
        lid = nearest_level(levels, ix, iy, p[2])
        return key_of.get((ix, iy, lid), -1) if lid >= 0 and abs(levels[(ix, iy, lid)] - p[2]) <= LEVEL_GAP else -1

    def near_cell(p, below, above):
        d = np.hypot(cx - p[0], cy - p[1])
        ok = (d <= DROP_NEAR) & (cz >= p[2] - below) & (cz <= p[2] + above)
        return int(np.flatnonzero(ok)[np.argmin(d[ok])]) if ok.any() else -1

    rows, i, n = [], 0, len(D)
    while i < n - 1:
        if grd[i] and not grd[i + 1] and key[i + 1] == key[i] and ms[i + 1] - ms[i] == 50:
            j = i + 1
            while j < n and key[j] == key[i] and ms[j] - ms[j - 1] == 50 and not grd[j]:
                j += 1
            if j < n and key[j] == key[i] and ms[j] - ms[j - 1] == 50 and grd[j] and z[j] - z[i] < -DROP_MIN:
                a, b = cell_of(xyz[i]), cell_of(xyz[j])
                a = a if a >= 0 else near_cell(xyz[i], 72.0, 24.0)
                b = b if b >= 0 else near_cell(xyz[j], 48.0, 48.0)
                if a >= 0 and b >= 0 and a != b:
                    rows.append((a, b, src[i], *xyz[i], *xyz[j]))
            i = j
        else:
            i += 1
    out = []
    if not rows:
        return out
    R = pd.DataFrame(rows, columns=["a", "b", "src", "x0", "y0", "z0", "x1", "y1", "z1"])
    P = R[["x0", "y0", "z0"]].to_numpy()
    nb = (np.hypot(P[:, None, 0] - P[None, :, 0], P[:, None, 1] - P[None, :, 1]) <= DROP_NEAR) & (np.abs(P[:, None, 2] - P[None, :, 2]) < 48.0)
    srcs = R.src.to_numpy()
    R["keep"] = [nb[k].sum() >= 3 and len(set(srcs[nb[k]])) >= MIN_SESSIONS for k in range(len(R))]
    for a, d in R[R.keep].groupby("a"):
        b = int(d.b.mode().iloc[0])
        take = d[["x0", "y0", "z0"]].median()
        out.append([int(a), b, len(d)] + [round(float(v), 1) for v in take] + [round(float(v), 1) for v in d[d.b.eq(b)][["x1", "y1", "z1"]].median()]
                   + [int(take.z0 - cz[a] > DROP_CLIMB)])
    return out


def fit_map(F, mp):
    area = mp in AREA_MAPS or mp in FULL_MAPS
    D = F[F["map"].eq(mp) & (F.human_duel & F.normal_physics if area else F.eligible)].copy()
    # what a cell's sources count: sessions, or on an area map player-sessions
    D["src"] = D.session_id + ":" + D.client_id.astype(str) if area else D.session_id
    ix, iy, lv, levels = cell_keys(D.origin_x.to_numpy(), D.origin_y.to_numpy(), D.origin_z.to_numpy())
    D["ix"], D["iy"], D["lv"] = ix, iy, lv
    agg = D.groupby(["ix", "iy", "lv"]).agg(n=("session_id", "size"), ns=("src", "nunique"))
    kept = agg[(agg.n >= MIN_ROWS) & (agg.ns >= MIN_SESSIONS)].reset_index()
    kept["cid"] = np.arange(len(kept))
    key = {(r.ix, r.iy, r.lv): r.cid for r in kept.itertuples()}
    D["cid"] = [key.get(k, -1) for k in zip(D.ix, D.iy, D.lv)]
    ncell = len(kept)
    cz = np.array([levels[(r.ix, r.iy, r.lv)] for r in kept.itertuples()])
    occ = np.zeros((ncell, len(H.CONTEXTS)), int)
    m = D.cid.ge(0)
    np.add.at(occ, (D.cid[m].to_numpy(), D.ctx_i[m].to_numpy()), 1)
    still = np.zeros(ncell)
    np.add.at(still, D.cid[m].to_numpy(), (D.speed_xy[m] < 5).to_numpy().astype(float))
    still = still / np.maximum(occ.sum(1), 1)
    # transitions within unbroken segments (next tick in the same segment)
    g = D.groupby(["session_id", "client_id", "seg"], sort=False)
    D["nx_cid"] = g.cid.shift(-1)
    D["nx_ms"] = g.session_ms.shift(-1)
    T = D[D.cid.ge(0) & D.nx_cid.ge(0) & (D.nx_ms - D.session_ms).eq(50)].copy()
    T["nx_cid"] = T.nx_cid.astype(int)
    stay = T[T.cid.eq(T.nx_cid)]
    mv = T[T.cid.ne(T.nx_cid)]
    edges = mv.groupby(["cid", "nx_cid"]).size().reset_index(name="count")
    # leave probability while the enemy is hidden (belief prediction speed)
    hid = T[T.line_of_sight.eq(0)]
    out_n = np.bincount(hid.cid[hid.cid.ne(hid.nx_cid)], minlength=ncell)
    all_n = np.bincount(hid.cid, minlength=ncell)
    p0 = out_n.sum() / max(all_n.sum(), 1)
    leave = (out_n + 10 * p0) / (all_n + 10)
    # enemy-direction coefficient of the kernel (hidden moves)
    cx = (kept.ix.to_numpy() + 0.5) * CELL
    cy = (kept.iy.to_numpy() + 0.5) * CELL
    nb = {}
    for r in edges.itertuples():
        nb.setdefault(r.cid, []).append((r.nx_cid, r.count))
    hm = mv[mv.line_of_sight.eq(0) & mv.opponent_origin_x.notna()]
    grp, y, base, cosv, band = [], [], [], [], []
    gid = 0
    for r in hm.itertuples():
        alts = nb.get(r.cid)
        if not alts or len(alts) < 2:
            continue
        ex, ey = r.opponent_origin_x - cx[r.cid], r.opponent_origin_y - cy[r.cid]
        de = np.hypot(ex, ey)
        if de < 1:
            continue
        b = int(np.searchsorted(KERNEL_DIST_EDGES, de, side="right") - 1)
        for to, cnt in alts:
            chosen = 1 if to == r.nx_cid else 0
            dx, dy = cx[to] - cx[r.cid], cy[to] - cy[r.cid]
            dn = np.hypot(dx, dy)
            grp.append(gid)
            y.append(chosen)
            base.append(np.log(cnt - chosen + 0.5))      # leave this move out of its own base count
            cosv.append(0.0 if dn < 1 else (dx * ex + dy * ey) / (dn * de))
            band.append(b)
        gid += 1
    grp, y, base, cosv, band = map(np.asarray, (grp, y, base, cosv, band))
    y = y.astype(float)
    nb_ = len(KERNEL_DIST_EDGES)

    def nll(beta):
        z = base + beta[band] * cosv
        zmax = pd.Series(z).groupby(grp).transform("max").to_numpy()
        e = np.exp(z - zmax)
        s = pd.Series(e).groupby(grp).transform("sum").to_numpy()
        return -(y * (z - zmax - np.log(s))).sum()

    beta = optimize.minimize(nll, np.zeros(nb_), method="Nelder-Mead", options={"xatol": 1e-3, "fatol": 1e-2}).x
    # spawns: where each life started
    sp = F[F["map"].eq(mp) & F.spawn_seg & F.seg_k.eq(0)]
    sp = sp.assign(px=sp.spawn_x.round(0), py=sp.spawn_y.round(0), pz=sp.spawn_z.round(0),
                   src=sp.session_id + ":" + sp.client_id.astype(str) if area else sp.session_id)
    spawns = sp.groupby(["px", "py", "pz"]).agg(n=("session_id", "size"), ns=("src", "nunique"),
                                                 yaw=("spawn_yaw", "median")).reset_index()
    spawns = spawns[(spawns.n >= 3) & (spawns.ns >= MIN_SESSIONS)]
    # empirical LOS between cells (unordered pairs), from both players' cells in duel ticks
    oix = np.floor(D.opponent_origin_x / CELL).astype("Int64")
    oiy = np.floor(D.opponent_origin_y / CELL).astype("Int64")
    ocid = []
    for a, b, zz in zip(oix, oiy, D.opponent_origin_z):
        if pd.isna(a):
            ocid.append(-1)
            continue
        lid = nearest_level(levels, int(a), int(b), float(zz))
        ocid.append(key.get((int(a), int(b), lid), -1))
    D["ocid"] = ocid
    P = D[D.cid.ge(0) & D.ocid.ge(0)]
    lo = np.minimum(P.cid, P.ocid)
    hi = np.maximum(P.cid, P.ocid)
    los = pd.DataFrame({"a": lo, "b": hi, "v": P.line_of_sight.eq(1).astype(int)}).groupby(["a", "b"]).v.agg(["size", "sum"])
    los = los[los["size"] >= 2].reset_index()
    checksum = D.sv_mapchecksum.dropna()
    R = {
        "map": mp, "checksum": int(checksum.mode().iloc[0]) if len(checksum) else 0, "cell_size": CELL,
        "cells": {"ix": kept.ix.to_numpy(), "iy": kept.iy.to_numpy(), "z": cz.round(1),
                  "occ": occ.ravel(), "leave": leave.round(4), "still": still.round(4)},
        "edges": {"from": edges.cid.to_numpy(), "to": edges.nx_cid.to_numpy(), "count": edges["count"].to_numpy()},
        "kernel": {"dist_edges": KERNEL_DIST_EDGES, "beta": beta.round(4)},
        "spawns": [[float(r.px), float(r.py), float(r.pz), round(float(r.yaw), 1), int(r.n)] for r in spawns.itertuples()],
        "los": {"a": los.a.to_numpy(), "b": los.b.to_numpy(), "n": los["size"].to_numpy(), "vis": los["sum"].to_numpy()},
        "hidden_move_speed": round(float(D[D.line_of_sight.eq(0) & D.speed_xy.gt(20)].speed_xy.median()), 1),
        "source": {"sessions": int(D.session_id.nunique()), "rows": int(len(D))},
    }
    if mp in FULL_MAPS:
        R["whole_map"] = True   # the bots hold no angle off people's ground there (hb_nav.cpp)
    if mp in DROP_MAPS:
        A = F[F["map"].eq(mp) & F.normal_physics].copy()
        A["src"] = A.session_id + ":" + A.client_id.astype(str)
        R["drops"] = fit_drops(A, key, levels, (kept.ix.to_numpy() + 0.5) * CELL, (kept.iy.to_numpy() + 0.5) * CELL, cz)
    return R


def main():
    F = H.load_dm()
    F["practice"] = F["map"].isin(MAPS)
    out_dir = H.HB_ROOT / "maps"
    out_dir.mkdir(parents=True, exist_ok=True)
    for mp in sys.argv[1:] or MAPS + AREA_MAPS + FULL_MAPS:
        R = fit_map(F, mp)
        p = out_dir / (mp.replace("/", "_") + ".json")
        p.write_text(json.dumps(H.jsonable(R), separators=(",", ":")))
        print(mp, "cells", len(R["cells"]["ix"]), "edges", len(R["edges"]["from"]), "los pairs", len(R["los"]["a"]),
              "spawns", len(R["spawns"]), "drops", len(R.get("drops", [])), "beta", R["kernel"]["beta"], "speed", R["hidden_move_speed"],
              "checksum", R["checksum"], "bytes", p.stat().st_size)


if __name__ == "__main__":
    main()
