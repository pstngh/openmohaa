"""Fit the two-key movement model (shared, pooled over everyone).

The side key (left/none/right) and the forward key (back/none/forward) are two
coupled semi-Markov processes, like the two fingers that press them. Each key
has its own hold age; its hazard of changing depends on the context, its own
state and age, the other key's state, walls in its direction, a fresh LOS
change and a recent context change. What it changes to:
  side:    from a strafe, reverse or let go (P(reverse) by context, forward key, age);
           from none, left or right (P(right) by context and forward key);
  forward: a categorical over the other two states, tilted toward or away from
           the enemy by distance (approach), with the side key as context.
The per-context habits (calibrate.py, movement.habit) correct what a first-order
chain driven by recorded contexts cannot reach within short contexts (people take
~1 s to get onto forward after a reload starts: 35% forward at the first tick, 70%
after 17 ticks; a longer context-change window fits that but loses the stop on
first sight).
Everything is fitted on unbroken segments of the duel mask, from ticks whose
run start is observed.
"""
import sys
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

AGE_EDGES = [1, 2, 3, 4, 5, 7, 9, 13, 21, 41, 81, 161, 241]  # lower edges, ticks; long no-strafe runs keep slowing down
CLEAR_EDGES = [0.0, 8.0, 16.0, 32.0, 64.0, 127.9]
DIST_EDGES = [0.0, 96.0, 160.0, 288.0, 512.0, 768.0]
CTX_AGE_EDGES = [1, 2, 3, 5]          # ticks since the context changed; the last bin (5+) is the reference
NC = len(H.CONTEXTS)


def bidx(x, edges):
    return np.clip(np.searchsorted(np.asarray(edges, float), np.asarray(x, float), side="right") - 1, 0, len(edges) - 1)


def shrunk_table(k, n, keys, shape, prior_keys, strength=25.0):
    """Rates on a grid, each cell shrunk toward its parent cell (dropping the last key dimension first)."""
    df = pd.DataFrame({**{f"k{i}": v for i, v in enumerate(keys)}, "y": k, "n": n})
    full = df.groupby([f"k{i}" for i in range(len(keys))]).agg(y=("y", "sum"), n=("n", "sum"))
    parent = df.groupby([f"k{i}" for i in prior_keys]).agg(y=("y", "sum"), n=("n", "sum"))
    g0 = df.y.sum() / max(df.n.sum(), 1)
    out = np.zeros(shape)
    for idx in np.ndindex(*shape):
        pk = tuple(idx[i] for i in prior_keys)
        pk = pk[0] if len(pk) == 1 else pk
        py, pn = parent.loc[pk] if pk in parent.index else (0.0, 0.0)
        pr = (py + 2 * g0) / (pn + 2)
        fy, fn = full.loc[idx] if idx in full.index else (0.0, 0.0)
        out[idx] = (fy + strength * pr) / (fn + strength)
    return out


def offsets(base_logit, y, extra_codes, sizes, l2=2.0):
    b0, co = H.fit_logistic(extra_codes, y, sizes, l2=l2, offset=base_logit)
    return b0, co


def fit_side(EL):
    d = EL[EL.known_side & EL.nx_side.notna()].copy()
    s = d.side.to_numpy().astype(int) + 1
    f = d.fwd.to_numpy().astype(int) + 1
    ctx = d.ctx_i.to_numpy()
    ab = bidx(d.age_side, AGE_EDGES)
    nxs = d.nx_side.to_numpy().astype(int) + 1
    ch = (nxs != s).astype(float)
    # switch hazard [ctx][side][fwd][age], shrunk toward [ctx][side][age]
    P = shrunk_table(ch, np.ones_like(ch), [ctx, s, f, ab], (NC, 3, 3, len(AGE_EDGES)), [0, 1, 3])
    base = H.logit(P[ctx, s, f, ab])
    # walls in the strafe direction, fresh LOS change, recent context change
    clr = np.where(s == 0, -1, np.where(s == 2, d.clear_right, d.clear_left))
    wb = np.where(clr >= 0, bidx(clr, CLEAR_EDGES), -1)
    wb = np.where(wb == len(CLEAR_EDGES) - 1, -1, wb)       # open is the reference
    losc = (d.lage.fillna(1e9).to_numpy() <= 50).astype(int)
    cab = bidx(d.age_ctx, CTX_AGE_EDGES)
    cc = np.where(cab < len(CTX_AGE_EDGES) - 1, (ctx * 2 + (s != 1)) * (len(CTX_AGE_EDGES) - 1) + cab, -1)
    b0, co = offsets(base, ch, [wb, losc, cc], [len(CLEAR_EDGES) - 1, 2, NC * 2 * (len(CTX_AGE_EDGES) - 1)])
    P_logit = H.logit(P) + b0 + co[1][0]
    # what a strafe changes to: P(reverse) [ctx][fwd][age]; from none: P(right) [ctx][fwd]
    e = d[ch == 1]
    es = e.side.to_numpy().astype(int)
    ef = e.fwd.to_numpy().astype(int) + 1
    ectx = e.ctx_i.to_numpy()
    eab = bidx(e.age_side, AGE_EDGES)
    from_strafe = es != 0
    rev = (e.nx_side.to_numpy() == -es).astype(float)
    R = shrunk_table(rev[from_strafe], np.ones(from_strafe.sum()),
                     [ectx[from_strafe], ef[from_strafe], eab[from_strafe]], (NC, 3, len(AGE_EDGES)), [0, 2])
    rt = (e.nx_side.to_numpy() == 1).astype(float)
    L = shrunk_table(rt[~from_strafe], np.ones((~from_strafe).sum()), [ectx[~from_strafe], ef[~from_strafe]], (NC, 3), [0])
    return {"switch_logit": P_logit.round(4), "wall_logit": np.r_[co[0], 0.0].round(4),
            "los_change_logit": round(float(co[1][1] - co[1][0]), 4),
            "ctx_change_logit": co[2].reshape(NC, 2, len(CTX_AGE_EDGES) - 1).round(4),
            "reverse_p": R.round(5), "right_p": L.round(5)}


def fit_fwd(EL):
    d = EL[EL.known_fwd & EL.nx_fwd.notna()].copy()
    f = d.fwd.to_numpy().astype(int) + 1
    s = d.side.to_numpy().astype(int) + 1
    ctx = d.ctx_i.to_numpy()
    ab = bidx(d.age_fwd, AGE_EDGES)
    nxf = d.nx_fwd.to_numpy().astype(int) + 1
    ch = (nxf != f).astype(float)
    P = shrunk_table(ch, np.ones_like(ch), [ctx, f, s, ab], (NC, 3, 3, len(AGE_EDGES)), [0, 1, 3])
    base = H.logit(P[ctx, f, s, ab])
    clr = np.where(f == 1, -1, np.where(f == 2, d.clear_front, d.clear_back))
    wb = np.where(clr >= 0, bidx(clr, CLEAR_EDGES), -1)
    wb = np.where(wb == len(CLEAR_EDGES) - 1, -1, wb)
    losc = (d.lage.fillna(1e9).to_numpy() <= 50).astype(int)
    cab = bidx(d.age_ctx, CTX_AGE_EDGES)
    cc = np.where(cab < len(CTX_AGE_EDGES) - 1, (ctx * 3 + f) * (len(CTX_AGE_EDGES) - 1) + cab, -1)
    b0, co = offsets(base, ch, [wb, losc, cc], [len(CLEAR_EDGES) - 1, 2, NC * 3 * (len(CTX_AGE_EDGES) - 1)])
    P_logit = H.logit(P) + b0 + co[1][0]
    # destination: logits T[ctx][from][side][to] + approach[ctx][dist] * (to - from) * cos(bearing)
    e = d[ch == 1]
    ef = e.fwd.to_numpy().astype(int) + 1
    es = e.side.to_numpy().astype(int) + 1
    ectx = e.ctx_i.to_numpy()
    to = e.nx_fwd.to_numpy().astype(int) + 1
    has = e.has_opp.to_numpy() & e.distance_xy.notna().to_numpy()
    cosb = np.where(has, np.cos(np.radians(e.relative_bearing.fillna(0).to_numpy())), 0.0)
    db = bidx(e.distance_xy.fillna(1e9), DIST_EDGES)
    rel = (e.opp_weapon_state.fillna(-1).to_numpy() == 5) & (e.line_of_sight.to_numpy() == 1)
    nD = len(DIST_EDGES)
    from scipy import optimize
    nT = NC * 27
    alt = np.arange(3)[None, :]
    mask = alt != ef[:, None]
    dirv = (alt - ef[:, None]) * cosb[:, None]          # change in approach for each candidate
    rows = np.arange(len(e))

    def unpack(w):
        return w[:nT].reshape(NC, 3, 3, 3), w[nT:nT + NC * nD].reshape(NC, nD), w[-1]

    def nll(w):
        T, Ap, Ar = unpack(w)
        k = Ap[ectx, db] + Ar * rel
        Z = T[ectx, ef, es, :] + k[:, None] * dirv
        Z = np.where(mask, Z, -np.inf)
        Zm = Z.max(1, keepdims=True)
        E_ = np.exp(Z - Zm)
        S = E_.sum(1, keepdims=True)
        Pm = E_ / S
        ll = (Z[rows, to] - Zm[:, 0] - np.log(S[:, 0])).sum()
        Y = np.zeros_like(Pm)
        Y[rows, to] = 1.0
        G = Pm - Y
        gT = np.zeros((NC, 3, 3, 3))
        np.add.at(gT, (ectx, ef, es), G)
        gk = (G * dirv).sum(1)
        gA = np.zeros((NC, nD))
        np.add.at(gA, (ectx, db), gk)
        lam = 0.05
        return -ll + lam * (T ** 2).sum(), np.r_[(gT + 2 * lam * T).ravel(), gA.ravel(), (gk * rel).sum()]

    res = optimize.minimize(nll, np.zeros(nT + NC * nD + 1), jac=True, method="L-BFGS-B", options={"maxiter": 3000})
    T, Ap, Ar = unpack(res.x)
    for c in range(NC):
        for fr in range(3):
            T[c, fr, :, fr] = -30.0
    return {"switch_logit": P_logit.round(4), "wall_logit": np.r_[co[0], 0.0].round(4),
            "los_change_logit": round(float(co[1][1] - co[1][0]), 4),
            "ctx_change_logit": co[2].reshape(NC, 3, len(CTX_AGE_EDGES) - 1).round(4),
            "next_logit": T.round(4), "approach": Ap.round(4), "approach_enemy_reload": round(float(Ar), 4)}


SPAWN_AGE_EDGES = [1, 2, 3, 5, 9, 21, 41, 81]    # ticks since the first live tick; the last edge ends the spawn run


def fit_spawn(F):
    """How a life starts: the empty usercmds, the keys at the first live tick, the first run of each key and the click.

    After a respawn the client sends 2-4 ticks of empty usercmds, then the keys it already holds (forward in 44%
    of lives, almost never a strafe key). The first run of each key is unlike later ones: nothing changes for about
    4 ticks, then the player starts to strafe or go. The respawn click is still held (or clicked again) in half of
    the lives for a few ticks; it never fires.
    """
    S = F[F.spawn_seg & F.eligible]
    first = S[S.seg_k.eq(0)]
    dead = np.bincount(first.dead_ticks.clip(0, H.SPAWN_DEAD_MAX_TICKS).astype(int), minlength=H.SPAWN_DEAD_MAX_TICKS + 1)
    chord = np.bincount(first.action.astype(int), minlength=9).astype(float)
    out = {"dead_ticks_pmf": (dead / dead.sum()).round(5), "chord_p": ((chord + 0.5) / (chord + 0.5).sum()).round(5),
           "age_edges": SPAWN_AGE_EDGES}
    nA = len(SPAWN_AGE_EDGES)
    for key, rows, prior_tab in [("side", 2, None), ("fwd", 3, None)]:
        age = S["age_" + key]
        spawn_run = S.seg_k.lt(age)               # the key has not changed since the first live tick
        d = S[spawn_run & S["nx_" + key].notna()]
        st = d[key].astype(int)
        row = st.ne(0).astype(int) if key == "side" else st + 1
        ab = bidx(d["age_" + key], SPAWN_AGE_EDGES)
        ch = d["nx_" + key].ne(d[key]).astype(float)
        k = np.zeros((rows, nA))
        n = np.zeros((rows, nA))
        np.add.at(k, (row.to_numpy(), ab), ch.to_numpy())
        np.add.at(n, (row.to_numpy(), ab), 1.0)
        pooled = (k.sum(1, keepdims=True) + 1) / (n.sum(1, keepdims=True) + 2)
        out[key + "_switch_p"] = ((k + 5 * pooled) / (n + 5)).round(5)
    # the click: attack at live tick t as a two-state chain
    att = S[S.seg_k.lt(H.SPAWN_CLICK_TICKS)].pivot_table(index=["session_id", "client_id", "seg"], columns="seg_k",
                                                         values="attack", aggfunc="first")
    att = att.dropna().astype(bool).to_numpy()
    stay, press = [], []
    for t in range(1, att.shape[1]):
        prev, cur = att[:, t - 1], att[:, t]
        stay.append((cur[prev].sum() + 0.5) / (prev.sum() + 1))
        press.append((cur[~prev].sum() + 0.5) / ((~prev).sum() + 1))
    out["click_first_p"] = round(float(att[:, 0].mean()), 5)
    out["click_stay_p"] = np.round(stay, 5)
    out["click_press_p"] = np.round(press, 5)
    return out


def main():
    F = H.load_dm()
    EL = F[F.eligible]
    part = {"age_edges": AGE_EDGES, "clear_edges": CLEAR_EDGES, "dist_edges": DIST_EDGES, "ctx_age_edges": CTX_AGE_EDGES,
            "side": fit_side(EL), "fwd": fit_fwd(EL)}
    H.write_part("keys", part)
    sp = fit_spawn(F)
    H.write_part("spawn", sp)
    print("spawn: dead ticks", sp["dead_ticks_pmf"], "chord", sp["chord_p"])
    print("  side switch [none, strafe] by age", sp["side_switch_p"])
    print("  fwd switch [back, none, fwd] by age", sp["fwd_switch_p"])
    print("  click first", sp["click_first_p"], "stay", sp["click_stay_p"], "press", sp["click_press_p"])
    sig = lambda z: 1 / (1 + np.exp(-np.asarray(z)))
    print("side switch p, LOS fire, strafe right, fwd none, by age:", sig(np.array(part["side"]["switch_logit"])[3, 2, 1, :6]).round(3))
    print("P(reverse) LOS fire fwd none by age:", np.array(part["side"]["reverse_p"])[3, 1, :6].round(3))
    print("fwd approach coef [ctx][dist]:\n", np.array(part["fwd"]["approach"]).round(2))


if __name__ == "__main__":
    main()
