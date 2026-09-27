"""Fit the movement and stance model (shared, pooled over everyone).

Chords are the 9 digital (forward, right) combinations, relative to the view:
index = (fwd + 1) * 3 + (side + 1). A chord is held as a semi-Markov state:

  P(switch at the next tick) = sigmoid(A[ctx][chord][age bin] + wall[clearance bin]
                                       + los_change * [LOS changed this tick or the last])
  next chord ~ softmax over the other 8 chords of
      T[ctx][from][to] + radial[ctx][distance bin] * r(to) + radial_reload * reloading * r(to)

where r(to) = cos(angle of `to` in the view frame - bearing to the enemy); 0 for neutral.
Lean, crouch, jump and walk are separate hazard processes. Everything is computed
on the duel mask, on unbroken segments, from ticks whose run start is observed.
"""
import sys
from pathlib import Path

import numpy as np
import pandas as pd
from scipy import optimize

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

AGE_EDGES = [1, 2, 3, 4, 5, 7, 9, 13, 21, 41]                  # lower edges, ticks
CLEAR_EDGES = [0.0, 8.0, 16.0, 32.0, 64.0, 127.9]               # lower edges, units (128 = open)
DIST_EDGES = [0.0, 96.0, 160.0, 288.0, 512.0, 768.0]            # lower edges, units
CHORD_FWD = np.array([-1, -1, -1, 0, 0, 0, 1, 1, 1])
CHORD_SIDE = np.array([-1, 0, 1, -1, 0, 1, -1, 0, 1])          # +1 = right
# direction of each chord in the view frame, degrees, positive = left (like relative_bearing)
CHORD_ANGLE = np.degrees(np.arctan2(-CHORD_SIDE, CHORD_FWD))
CHORD_MOVING = ~((CHORD_FWD == 0) & (CHORD_SIDE == 0))
CLASS = {0: "back_diag", 1: "back", 2: "back_diag", 3: "strafe", 4: "neutral", 5: "strafe",
         6: "fwd_diag", 7: "fwd", 8: "fwd_diag"}


def bin_index(x, edges):
    return np.clip(np.searchsorted(np.asarray(edges), np.asarray(x, float), side="right") - 1, 0, len(edges) - 1)


def fit_switch(EL):
    """Base switch logits A[ctx][chord][age] with hierarchical shrinkage, then wall and LOS-change terms."""
    d = EL[EL.known_action & EL.nx_action.notna()].copy()
    d["sw"] = d.nx_action.ne(d.action).astype(float)
    d["ab"] = bin_index(d.age_action, AGE_EDGES)
    d["cls"] = d.action.map(CLASS)
    nC, nA = len(H.CONTEXTS), len(AGE_EDGES)
    A = np.zeros((nC, 9, nA))
    counts = np.zeros((nC, 9, nA), int)
    g_all = d.groupby(["cls", "ab"]).sw.agg(["sum", "size"])
    g_ctx = d.groupby(["ctx_i", "cls", "ab"]).sw.agg(["sum", "size"])
    g_cell = d.groupby(["ctx_i", "action", "ab"]).sw.agg(["sum", "size"])
    for ci in range(nC):
        for a in range(9):
            for b in range(nA):
                cls = CLASS[a]
                k0, n0 = g_all.loc[(cls, b)] if (cls, b) in g_all.index else (0.0, 0.0)
                k1, n1 = g_ctx.loc[(ci, cls, b)] if (ci, cls, b) in g_ctx.index else (0.0, 0.0)
                k2, n2 = g_cell.loc[(ci, a, b)] if (ci, a, b) in g_cell.index else (0.0, 0.0)
                p1 = H.shrink_rate(k1, n1, k0, n0, strength=30)
                p2 = (k2 + 20 * p1) / (n2 + 20)
                A[ci, a, b] = H.logit(p2)
                counts[ci, a, b] = n2
    # additive wall and LOS-change terms (logistic regression with the base as offset)
    base = A[d.ctx_i.to_numpy(), d.action.to_numpy().astype(int), d.ab.to_numpy()]
    clr = d.clear_move.to_numpy()
    moving = CHORD_MOVING[d.action.to_numpy().astype(int)]
    cb = np.where(moving & (clr >= 0), bin_index(clr, CLEAR_EDGES), len(CLEAR_EDGES))   # last column = not applicable
    losc = (d.lage.fillna(1e9).to_numpy() <= 50).astype(float)
    y = d.sw.to_numpy()
    nW = len(CLEAR_EDGES)

    def nll(w):
        wall = np.r_[w[:nW - 1], 0.0, 0.0]          # open (128) and neutral are the reference
        z = base + wall[cb] + w[nW - 1] * losc
        p = H.sigmoid(z)
        eps = 1e-9
        ll = y * np.log(p + eps) + (1 - y) * np.log(1 - p + eps)
        r = y - p
        gw = np.array([-(r[cb == j]).sum() for j in range(nW - 1)] + [-(r * losc).sum()])
        return -ll.sum(), gw

    res = optimize.minimize(nll, np.zeros(nW), jac=True, method="L-BFGS-B")
    wall = np.r_[res.x[:nW - 1], 0.0]
    return {"age_edges": AGE_EDGES, "logit": A.round(4), "n": counts, "clear_edges": CLEAR_EDGES,
            "wall_logit": wall.round(4), "los_change_logit": round(float(res.x[nW - 1]), 4)}


def fit_next(EL):
    """Conditional logit for the chord chosen at a switch."""
    d = EL[EL.known_action & EL.nx_action.notna() & EL.nx_action.ne(EL.action)].copy()
    frm = d.action.to_numpy().astype(int)
    to = d.nx_action.to_numpy().astype(int)
    ctx = d.ctx_i.to_numpy()
    has = d.has_opp.to_numpy() & d.distance_xy.notna().to_numpy()
    bearing = np.where(has, d.relative_bearing.fillna(0).to_numpy(), 0.0)
    dist = d.distance_xy.fillna(1e9).to_numpy()
    db = bin_index(dist, DIST_EDGES)
    rel = (d.opp_weapon_state.fillna(-1).to_numpy() == 5) & (d.line_of_sight.to_numpy() == 1)
    nC, nD = len(H.CONTEXTS), len(DIST_EDGES)
    # r[i, c] for every candidate chord
    R = np.cos(np.radians(CHORD_ANGLE[None, :] - bearing[:, None])) * CHORD_MOVING[None, :]
    R = R * has[:, None]
    mask = np.ones((len(d), 9), bool)
    mask[np.arange(len(d)), frm] = False
    # parameters: T[ctx][from][to] (9x9 per ctx, diag unused) + radial[ctx][db] + radial_reload
    nT = nC * 81
    nR = nC * nD

    def unpack(w):
        return w[:nT].reshape(nC, 9, 9), w[nT:nT + nR].reshape(nC, nD), w[nT + nR]

    lam = 0.05  # small ridge on the transition logits (keeps rare cells finite)

    def nll(w):
        T, Rd, Rr = unpack(w)
        coef = Rd[ctx, db] + Rr * rel
        Z = T[ctx, frm, :] + coef[:, None] * R
        Z = np.where(mask, Z, -np.inf)
        Zm = Z.max(1, keepdims=True)
        E = np.exp(Z - Zm)
        S = E.sum(1, keepdims=True)
        P = E / S
        ll = (Z[np.arange(len(d)), to] - Zm[:, 0] - np.log(S[:, 0])).sum()
        Y = np.zeros_like(P)
        Y[np.arange(len(d)), to] = 1.0
        G = (P - Y)                                  # dNLL/dZ
        gT = np.zeros((nC, 9, 9))
        np.add.at(gT, (ctx, frm), G)
        gcoef = (G * R).sum(1)
        gRd = np.zeros((nC, nD))
        np.add.at(gRd, (ctx, db), gcoef)
        gRr = (gcoef * rel).sum()
        f = -ll + lam * (T ** 2).sum()
        g = np.r_[(gT + 2 * lam * T).ravel(), gRd.ravel(), gRr]
        return f, g

    w0 = np.zeros(nT + nR + 1)
    res = optimize.minimize(nll, w0, jac=True, method="L-BFGS-B", options={"maxiter": 2000})
    T, Rd, Rr = unpack(res.x)
    for c in range(nC):
        np.fill_diagonal(T[c], -30.0)
    return {"logit": T.round(4), "dist_edges": DIST_EDGES, "radial": Rd.round(4), "radial_enemy_reload": round(float(Rr), 4),
            "n_events": int(len(d)), "converged": bool(res.success)}


def fit_lean(EL):
    """Lean as a 3-state semi-Markov chain coupled to the strafe side.

    next-state probabilities P(l' | l, age bin, ctx, strafe relation). For l == 0 the relation
    is the strafe side (none/left/right) and l' is drawn directly; for l != 0 it is
    none/agree/disagree and l' is stay / release / switch side.
    """
    d = EL[EL.known_lean & EL.nx_lean.notna()].copy()
    l = d.lean.to_numpy().astype(int)
    nl = d.nx_lean.to_numpy().astype(int)
    side = d.side.to_numpy().astype(int)
    # side sign convention: side +1 = right key; lean +1 = right
    rel = np.where(l == 0, side + 1, np.where(side == 0, 0, np.where(side == l, 1, 2)))
    ab = bin_index(d.age_lean, AGE_EDGES)
    ctx = d.ctx_i.to_numpy()
    out = np.zeros((2, len(H.CONTEXTS), len(AGE_EDGES), 3, 3))   # [state none/leaning][ctx][age][rel][outcome]
    # outcome for none: 0 lean left, 1 stay none, 2 lean right; for leaning: 0 stay, 1 release, 2 switch side
    oc = np.where(l == 0, nl + 1, np.where(nl == l, 0, np.where(nl == 0, 1, 2)))
    st = (l != 0).astype(int)
    df = pd.DataFrame({"st": st, "ctx": ctx, "ab": ab, "rel": rel, "oc": oc})
    pooled = df.groupby(["st", "rel", "oc"]).size().unstack(fill_value=0)
    byc = df.groupby(["st", "ctx", "ab", "rel", "oc"]).size().unstack(fill_value=0)
    for s in (0, 1):
        for c in range(len(H.CONTEXTS)):
            for b in range(len(AGE_EDGES)):
                for r in range(3):
                    prior = pooled.loc[(s, r)].to_numpy(float) if (s, r) in pooled.index else np.ones(3)
                    prior = (prior + 1) / (prior + 1).sum()
                    cnt = byc.loc[(s, c, b, r)].to_numpy(float) if (s, c, b, r) in byc.index else np.zeros(3)
                    out[s, c, b, r] = (cnt + 15 * prior) / (cnt.sum() + 15)
    return {"age_edges": AGE_EDGES, "next": out.round(5),
            "doc": "next[state][ctx][age][rel][outcome]; state 0 none: rel = strafe side (0 left key, 1 none, 2 right key), "
                   "outcome 0 lean left / 1 none / 2 lean right; state 1 leaning: rel 0 no strafe / 1 agree / 2 disagree, "
                   "outcome 0 stay / 1 release / 2 switch side"}


def hold_pmf(runs_ms, max_ticks=40):
    """Discrete pmf of complete hold lengths in ticks (index 0 = 1 tick)."""
    t = np.clip(np.round(np.asarray(runs_ms) / 50).astype(int), 1, max_ticks)
    pmf = np.bincount(t - 1, minlength=max_ticks).astype(float)
    return (pmf / max(pmf.sum(), 1)).round(5)


def complete_runs(F, col):
    """Complete runs (not touching a segment edge) of a column over eligible rows."""
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    rid = (F[col] != g[col].shift()).cumsum()
    t = F.assign(_r=rid).groupby("_r").agg(val=(col, "first"), n=(col, "size"), el=("eligible", "min"),
                                           ctx=("ctx", lambda x: x.value_counts().index[0]))
    edges = F.assign(_r=rid).groupby(["session_id", "client_id", "seg"])._r.agg(["min", "max"])
    t = t[~t.index.isin(set(edges["min"]) | set(edges["max"])) & t.el]
    t["ms"] = t.n * 50
    return t


def fit_stance(F, EL):
    """Press hazards (per tick, by context) and hold-length pmfs for crouch, jump and walk keys."""
    out = {}
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    F = F.assign(walk_key=F.run.eq(0))
    for key, col in [("crouch", "crouch_key"), ("jump", "jump_key"), ("walk", "walk_key")]:
        prev = g[col].shift() if col != "walk_key" else F.groupby(["session_id", "client_id", "seg"], sort=False).walk_key.shift()
        start = F[col] & prev.eq(False)
        idle = prev.eq(False)
        E = F.assign(_s=start, _idle=idle)
        E = E[E.eligible & E._idle]
        haz = E.groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
        runs = complete_runs(F, col)
        runs = runs[runs.val.eq(True)]
        cap = 240 if key == "walk" else 20
        out[key] = {"press_hazard": haz.round(6), "hold_pmf": hold_pmf(runs.ms, cap), "hold_ms_median": float(runs.ms.median())}
    return out


def main():
    F = H.load_dm()
    EL = F[F.eligible]
    part = {"chords": H.CHORDS, "chord_angle_deg": CHORD_ANGLE.round(3),
            "switch": fit_switch(EL), "next": fit_next(EL), "lean": fit_lean(EL), "stance": fit_stance(F, EL),
            "veto_clearance": 16.0}
    p = H.write_part("movement", part)
    print("wrote", p)
    sw = part["switch"]
    print("wall logit", sw["wall_logit"], "los change", sw["los_change_logit"])
    print("radial", np.asarray(part["next"]["radial"]), "reload", part["next"]["radial_enemy_reload"])
    for k, v in part["stance"].items():
        print(k, "press/min by ctx", (np.asarray(v["press_hazard"]) * 1200).round(2), "hold median", v["hold_ms_median"])


if __name__ == "__main__":
    main()
