"""Fit the lean and stance model (shared, pooled over everyone).

The movement keys themselves are fitted by fit_keys.py. Lean is a 3-state
semi-Markov chain coupled to the strafe side; crouch, jump and walk are key
processes with a press hazard by context and a release hazard by hold age.
Everything is computed on the duel mask, on unbroken segments, from ticks whose
run start is observed (every life starts with 2-4 ticks of empty usercmds while
the client catches up with the respawn, so segment-start runs are censored).
"""
import sys
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

AGE_EDGES = [1, 2, 3, 4, 5, 7, 9, 13, 21, 41]                  # lower edges, ticks
CTX_AGE_EDGES = [1, 2, 3, 5]          # ticks since the context changed; the last bin (5+) is the reference


def bin_index(x, edges):
    return np.clip(np.searchsorted(np.asarray(edges), np.asarray(x, float), side="right") - 1, 0, len(edges) - 1)


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
    # reaction to a context change: logit offset of leaving the current lean state
    base_leave = np.array([1.0 - out[st_, c_, b_, r_, 1 if st_ == 0 else 0] for st_, c_, b_, r_ in zip(st, ctx, ab, rel)])
    leave = np.where(st == 0, oc != 1, oc != 0).astype(float)
    cab = bin_index(d.age_ctx, CTX_AGE_EDGES)
    code = np.where(cab < len(CTX_AGE_EDGES) - 1, (ctx * 2 + st) * (len(CTX_AGE_EDGES) - 1) + cab, -1)
    b0, co = H.fit_logistic([code], leave, [len(H.CONTEXTS) * 2 * (len(CTX_AGE_EDGES) - 1)], l2=2.0,
                            offset=H.logit(base_leave))
    lean_change = co[0].reshape(len(H.CONTEXTS), 2, len(CTX_AGE_EDGES) - 1)
    return {"age_edges": AGE_EDGES, "next": out.round(5), "ctx_age_edges": CTX_AGE_EDGES,
            "ctx_change_logit": lean_change.round(4), "ctx_change_bias": round(b0, 4),
            "doc": "next[state][ctx][age][rel][outcome]; state 0 none: rel = strafe side (0 left key, 1 none, 2 right key), "
                   "outcome 0 lean left / 1 none / 2 lean right; state 1 leaning: rel 0 no strafe / 1 agree / 2 disagree, "
                   "outcome 0 stay / 1 release / 2 switch side"}


def hold_pmf(runs_ms, max_ticks=40):
    """Discrete pmf of complete hold lengths in ticks (index 0 = 1 tick)."""
    t = np.clip(np.round(np.asarray(runs_ms) / 50).astype(int), 1, max_ticks)
    pmf = np.bincount(t - 1, minlength=max_ticks).astype(float)
    return (pmf / max(pmf.sum(), 1)).round(5)


def complete_runs(F, col):
    """Complete runs of a column over eligible rows: not touching the segment end, nor its start
    unless the segment starts with a respawn."""
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    rid = (F[col] != g[col].shift()).cumsum()
    t = F.assign(_r=rid).groupby("_r").agg(val=(col, "first"), n=(col, "size"), el=("eligible", "min"),
                                           ctx=("ctx", lambda x: x.value_counts().index[0]))
    edges = F.assign(_r=rid).groupby(["session_id", "client_id", "seg"]).agg(first=("_r", "min"), last=("_r", "max"),
                                                                             spawn=("spawn_seg", "first"))
    censored = set(edges["last"]) | set(edges["first"][~edges.spawn])
    t = t[~t.index.isin(censored) & t.el]
    t["ms"] = t.n * 50
    return t


def fit_stance(F, EL):
    """Press hazards (per tick, by context) and hold-length pmfs for crouch, jump and walk keys."""
    out = {}
    F = F.assign(walk_key=F.run.eq(0))
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    first = g.cumcount().eq(0)
    for key, col in [("crouch", "crouch_key"), ("jump", "jump_key"), ("walk", "walk_key")]:
        # before the first live tick of a respawn every key was up (empty usercmds)
        prev = g[col].shift().where(~(first & F.spawn_seg), False)
        start = F[col] & prev.eq(False)
        idle = prev.eq(False)
        E = F.assign(_s=start, _idle=idle)
        E = E[E.eligible & E._idle]
        haz = E.groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
        runs = complete_runs(F, col)
        runs = runs[runs.val.eq(True)]
        cap = 240 if key == "walk" else 20
        # release hazard by hold age (survival estimate: long walks are often censored)
        held = F[col].astype(bool)
        rid = (F[col] != g[col].shift()).cumsum()
        age = F.groupby([F.session_id, F.client_id, F.seg, rid], sort=False).cumcount() + 1
        first_tick = g.cumcount().eq(0)
        rfirst = rid.where(first_tick).groupby([F.session_id, F.client_id, F.seg], sort=False).transform("max")
        known = rid.ne(rfirst) | F.spawn_seg
        nxt = held.astype(float).groupby([F.session_id, F.client_id, F.seg], sort=False).shift(-1)
        R = pd.DataFrame({"age": age, "rel": 1.0 - nxt})[held & nxt.notna() & known & F.eligible]
        rel_edges = [1, 2, 3, 4, 6, 9, 13, 21, 41, 81, 161]
        rb = bin_index(R.age, rel_edges)
        k = R.groupby(rb).rel.agg(["sum", "size"]).reindex(range(len(rel_edges))).fillna(0)
        prior = k["sum"].sum() / max(k["size"].sum(), 1)
        rel = ((k["sum"] + 5 * prior) / (k["size"] + 5)).to_numpy()
        out[key] = {"press_hazard": haz.round(6), "hold_pmf": hold_pmf(runs.ms, cap), "hold_ms_median": float(runs.ms.median()),
                    "release_age_edges": rel_edges, "release_hazard": rel.round(5)}
    return out


def main():
    F = H.load_dm()
    EL = F[F.eligible]
    # People press keys toward a wall they are touching about as often as in the open (forward 3.2% vs
    # 3.6% per tick) and slide along it, so walls veto nothing; only ledges deep enough to hurt do.
    part = {"lean": fit_lean(EL), "stance": fit_stance(F, EL), "veto_clearance": 0.0}
    p = H.write_part("movement", part)
    print("wrote", p)
    for k, v in part["stance"].items():
        print(k, "press/min by ctx", (np.asarray(v["press_hazard"]) * 1200).round(2), "hold median", v["hold_ms_median"])


if __name__ == "__main__":
    main()
