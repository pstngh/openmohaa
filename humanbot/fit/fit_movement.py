"""Fit the lean and stance model (shared, pooled over everyone).

The movement keys themselves are fitted by fit_keys.py. Lean is a 3-state
semi-Markov chain coupled to the strafe side; crouch, jump and walk are key
processes with a press hazard by context and a release hazard by hold age.
Crouch is a toggle in this engine (a press while standing ducks, 99.9%; the next
press, or a jump press, stands up, 98%), so its press hazard is fitted on
standing ticks and a stand-up hazard by crouched age ends each dip (people
crouch 3.5% of the time, in dips of about 350 ms).
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


LEAN_CTX_DEAD = len(H.CONTEXTS)   # sixth lean context: the opponent is dead (30% of the recorded rows)


def lean_contexts(F):
    """Context per row for the lean chain: the duel contexts on eligible rows, LEAN_CTX_DEAD while the
    opponent is dead (people lean far less then: 27% vs 59%), -1 elsewhere (not fitted)."""
    dead = ~F.eligible & ~F.opp_alive.fillna(False).astype(bool)
    return np.where(F.eligible, F.ctx_i, np.where(dead, LEAN_CTX_DEAD, -1))


def fit_lean(F, EL):
    """Lean as a 3-state semi-Markov chain coupled to the strafe side.

    next-state probabilities P(l' | l, age bin, ctx, strafe relation). For l == 0 the relation
    is the strafe side (none/left/right) and l' is drawn directly; for l != 0 it is
    none/agree/disagree and l' is stay / release / switch side. The relation uses the side key
    of the same tick as l' (people switch lean and strafe together: conditioning on the side
    of the tick before loses the coupling, 60% instead of 80% agreement in a teacher-forced
    replay), and every row is used, with the opponent-dead time as a context of its own (a
    lean carried out of it into the next fight is the chain's, not the eligible rows').
    """
    F = F.assign(lctx=lean_contexts(F))
    d = F[F.known_lean & F.nx_lean.notna() & F.nx_side.notna() & F.lctx.ge(0)].copy()
    l = d.lean.to_numpy().astype(int)
    nl = d.nx_lean.to_numpy().astype(int)
    side = d.nx_side.to_numpy().astype(int)
    side0 = d.side.to_numpy().astype(int)
    # side sign convention: side +1 = right key; lean +1 = right. Leaning, a strafe against the lean is the tick it
    # turns so (3: people decide there, the presser flipping the lean across with it 47% of the time, the strafers
    # letting it go 30-43%) or a tick it stays so (2: 15% and 14% a tick)
    rel = np.where(l == 0, side + 1, np.where(side == 0, 0, np.where(side == l, 1, np.where(side0 == -l, 2, 3))))
    ab = bin_index(d.age_lean, AGE_EDGES)
    ctx = d.lctx.to_numpy()
    nctx = len(H.CONTEXTS) + 1
    out = np.zeros((2, nctx, len(AGE_EDGES), 4, 3))   # [state none/leaning][ctx][age][rel][outcome]
    # outcome for none: 0 lean left, 1 stay none, 2 lean right; for leaning: 0 stay, 1 release, 2 switch side
    oc = np.where(l == 0, nl + 1, np.where(nl == l, 0, np.where(nl == 0, 1, 2)))
    st = (l != 0).astype(int)
    df = pd.DataFrame({"st": st, "ctx": ctx, "ab": ab, "rel": rel, "oc": oc})
    pooled = df.groupby(["st", "rel", "oc"]).size().unstack(fill_value=0)
    byc = df.groupby(["st", "ctx", "ab", "rel", "oc"]).size().unstack(fill_value=0)
    for s in (0, 1):
        for c in range(nctx):
            for b in range(len(AGE_EDGES)):
                for r in range(4):
                    prior = pooled.loc[(s, r)].to_numpy(float) if (s, r) in pooled.index else np.ones(3)
                    prior = (prior + 1) / (prior + 1).sum()
                    cnt = byc.loc[(s, c, b, r)].to_numpy(float) if (s, c, b, r) in byc.index else np.zeros(3)
                    out[s, c, b, r] = (cnt + 15 * prior) / (cnt.sum() + 15)
    # reaction to a context change (duel contexts): logit offset of leaving the current lean state
    base_leave = np.array([1.0 - out[st_, c_, b_, r_, 1 if st_ == 0 else 0] for st_, c_, b_, r_ in zip(st, ctx, ab, rel)])
    leave = np.where(st == 0, oc != 1, oc != 0).astype(float)
    cab = bin_index(d.age_ctx, CTX_AGE_EDGES)
    code = np.where((cab < len(CTX_AGE_EDGES) - 1) & (ctx < LEAN_CTX_DEAD), (ctx * 2 + st) * (len(CTX_AGE_EDGES) - 1) + cab, -1)
    b0, co = H.fit_logistic([code], leave, [len(H.CONTEXTS) * 2 * (len(CTX_AGE_EDGES) - 1)], l2=2.0,
                            offset=H.logit(base_leave))
    lean_change = co[0].reshape(len(H.CONTEXTS), 2, len(CTX_AGE_EDGES) - 1)
    wall = fit_lean_wall(d, l, nl, out[st, ctx, ab, rel])
    enemy = fit_lean_enemy(d, l, nl, out[st, ctx, ab, rel], wall)
    return {"age_edges": AGE_EDGES, "next": out.round(5), "ctx_age_edges": CTX_AGE_EDGES,
            "ctx_change_logit": lean_change.round(4), "ctx_change_bias": round(b0, 4), "wall": wall, "enemy": enemy,
            "doc": "next[state][ctx][age][rel][outcome]; ctx 0-4 the duel contexts, 5 the opponent dead; rel from the "
                   "side key of the next tick; state 0 none: rel = strafe side (0 left key, 1 none, 2 right key), "
                   "outcome 0 lean left / 1 none / 2 lean right (rel 3 unused); state 1 leaning: rel 0 no strafe / 1 agree / "
                   "2 disagree (held) / 3 turned against the lean this tick, outcome 0 stay / 1 release / 2 switch side"}


LEAN_WALL_RANGE = 96.0   # a wall closer than this at the side weighs 1 - clearance / range
LEAN_EDGE_OPEN = 96.0    # ... and is an edge to look past while the front diagonal on that side is this open


def lean_wall_features(D):
    """Right minus left of the side's wall closeness, and of the same while the front diagonal on that side is open."""
    wl = np.clip(1.0 - D.clear_left.to_numpy(float) / LEAN_WALL_RANGE, 0.0, 1.0)
    wr = np.clip(1.0 - D.clear_right.to_numpy(float) / LEAN_WALL_RANGE, 0.0, 1.0)
    el = wl * (D.clear_front_left.to_numpy(float) >= LEAN_EDGE_OPEN)
    er = wr * (D.clear_front_right.to_numpy(float) >= LEAN_EDGE_OPEN)
    return np.c_[wr - wl, er - el]


def fit_lean_wall(d, l, nl, P):
    """The side a lean takes by the walls beside (people lean away from a flat wall at their side and around an
    edge just ahead on one side): a logistic of ending the tick leaned right, over the rows that lean on or stay
    leaned, with the chain's own odds of that side as offset (P: the chain's probabilities of each row)."""
    from scipy.optimize import minimize
    sel = (nl != 0) & np.isfinite(d.clear_left.to_numpy(float)) & np.isfinite(d.clear_right.to_numpy(float))
    # off -> on: P = [left, none, right]; leaning: P = [stay, release, switch]
    pr = np.where(l == 0, P[:, 2], np.where(l == 1, P[:, 0], P[:, 2])) / np.maximum(P[:, 0] + P[:, 2], 1e-9)
    off = H.logit(np.clip(pr, 1e-6, 1 - 1e-6))[sel]
    X = lean_wall_features(d)[sel]
    y = (nl[sel] == 1).astype(float)

    def nll(w):
        z = off + X @ w
        return np.sum(np.logaddexp(0.0, z) - y * z) + 0.5 * np.sum(w * w)

    w = minimize(nll, np.zeros(2), method="BFGS").x
    return {"range": LEAN_WALL_RANGE, "edge_open": LEAN_EDGE_OPEN, "wall_logit": round(float(w[0]), 4),
            "edge_logit": round(float(w[1]), 4)}


LEAN_ENEMY_DEG = (3.0, 150.0)   # the enemy's side counts while he is this far off the view
LEAN_ENEMY_GROUPS = [0, 0, 1, 1, 2]   # per context: hidden (no fire, firing), in sight (no fire, firing), reloading


def lean_enemy_side(D):
    """+1 with the enemy to the right of the view, -1 to the left, 0 within LEAN_ENEMY_DEG[0] of it or behind (or none)."""
    b = (np.degrees(np.arctan2(D.opponent_origin_y.to_numpy(float) - D.origin_y.to_numpy(float),
                               D.opponent_origin_x.to_numpy(float) - D.origin_x.to_numpy(float)))
         - D.view_yaw.to_numpy(float) + 180.0) % 360.0 - 180.0
    a = np.abs(b)
    ok = np.isfinite(b) & (a >= LEAN_ENEMY_DEG[0]) & (a <= LEAN_ENEMY_DEG[1])
    return np.where(ok, np.where(b > 0, -1.0, 1.0), 0.0)


def fit_lean_enemy(d, l, nl, P, wall):
    """The side a lean takes by the enemy's side (people lean to see past what hides him: with him hidden 10-90 deg
    off the view they lean away from his side 76% of the time, 66-85% by map and 67-80% by person, and 72% with no
    strafe key held; in sight toward him, 63%): a logistic of ending the tick leaned right with the chain and the
    fitted wall terms as offset, one weight per context group (LEAN_ENEMY_GROUPS). The opponent-dead rows are left out."""
    from scipy.optimize import minimize
    ctx = d.lctx.to_numpy()
    sel = (nl != 0) & np.isfinite(d.clear_left.to_numpy(float)) & np.isfinite(d.clear_right.to_numpy(float)) & (ctx != LEAN_CTX_DEAD)
    pr = np.where(l == 0, P[:, 2], np.where(l == 1, P[:, 0], P[:, 2])) / np.maximum(P[:, 0] + P[:, 2], 1e-9)
    off = H.logit(np.clip(pr, 1e-6, 1 - 1e-6)) + lean_wall_features(d) @ np.array([wall["wall_logit"], wall["edge_logit"]])
    e = lean_enemy_side(d)
    grp = np.asarray(LEAN_ENEMY_GROUPS)[np.clip(ctx, 0, len(LEAN_ENEMY_GROUPS) - 1)]
    ng = max(LEAN_ENEMY_GROUPS) + 1
    X = np.zeros((len(d), ng))
    X[np.arange(len(d)), grp] = e
    X, off, y = X[sel], off[sel], (nl[sel] == 1).astype(float)

    def nll(w):
        z = off + X @ w
        return np.sum(np.logaddexp(0.0, z) - y * z) + 0.5 * np.sum(w * w)

    w = minimize(nll, np.zeros(ng), method="BFGS").x
    return {"min_deg": LEAN_ENEMY_DEG[0], "max_deg": LEAN_ENEMY_DEG[1],
            "logit": [round(float(w[g]), 4) for g in LEAN_ENEMY_GROUPS],
            "doc": "per context: the logit of leaning toward the enemy's side (+) or away from it (-), the enemy known "
                   "(seen, or believed while hidden) min_deg-max_deg off the view; fitted with the chain and the wall "
                   "terms as offset"}


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


UP_AGE_EDGES = [1, 2, 3, 4, 5, 7, 9, 13, 21, 41]   # ticks crouched (lower edges)
QUIET_TICKS = 20         # quiet: no part of the enemy on screen, none of his shots heard and no hit taken for 1 s
TERRAIN_UP = 12.0        # a jump that lands this much higher went onto something
FIRE_HEARD_TICKS = 10    # the crouch's "enemy just fired": its attack held in this many ticks up to now


def fit_stand_up(F, g, first):
    """Hazard of the press that ends a crouch (crouch or jump key) by ticks crouched on the ground."""
    duck = F.ducked.astype(bool) & F.on_ground.astype(bool)
    seg = [F.session_id, F.client_id, F.seg]
    rid = (duck != duck.groupby(seg, sort=False).shift()).cumsum()
    age = duck.groupby(rid).cumcount() + 1
    press = F.crouch_key & g.crouch_key.shift().where(~(first & F.spawn_seg), False).eq(False)
    press |= F.jump_key & g.jump_key.shift().where(~(first & F.spawn_seg), False).eq(False)
    first_run = rid.groupby(seg, sort=False).transform("min")
    known = rid.ne(first_run) | F.spawn_seg
    D = pd.DataFrame({"age": age, "up": press})[duck & F.eligible & known]
    b = bin_index(D.age, UP_AGE_EDGES)
    k = D.groupby(b).up.agg(["sum", "size"]).reindex(range(len(UP_AGE_EDGES))).fillna(0)
    prior = k["sum"].sum() / max(k["size"].sum(), 1)
    return ((k["sum"] + 5 * prior) / (k["size"] + 5)).to_numpy()


def quiet_rows(F, g):
    """Quiet: no part of the enemy on screen, none of his shots and no hit taken in the last QUIET_TICKS (and now)."""
    def recent(col):
        return g[col].transform(lambda x: x.fillna(0).astype(float).rolling(QUIET_TICKS + 1, min_periods=1).max()).gt(0)
    return ~(recent("vis") | recent("opp_attack_primary") | recent("ev_dmg_taken"))


def terrain_jumps(F, start):
    """Jump presses that land TERRAIN_UP or more higher (within 1.5 s, the first tick on the ground 150 ms or more
    after the press): onto a box, a step, a ledge. The bot's navigation takes it up such things without a jump."""
    out = np.zeros(len(F), bool)
    z = F.origin_z.to_numpy(float)
    og = F.on_ground.to_numpy().astype(bool)
    run = (F.session_id.astype(str) + "|" + F.client_id.astype(str) + "|" + F.seg.astype(str)).to_numpy()
    n = len(F)
    for i in np.flatnonzero(np.asarray(start)):
        j = i + 3
        while j < min(i + 30, n) and run[j] == run[i] and not og[j]:
            j += 1
        if j < n and run[j] == run[i] and og[j] and z[j] - z[i] >= TERRAIN_UP:
            out[i] = True
    return out


def fit_stance(F, EL):
    """Press hazards (per tick, by context) and hold-length pmfs for crouch, jump and walk keys.

    Crouches and jumps are pressed mostly in fights (people: 91% of crouches and 67% of jumps come with a part of the
    enemy on screen within a second either side, or his shot or a hit in the last second); out of them, quiet
    (quiet_rows), people crouch 1.1 times a minute where hidden time as a whole has 2.4, and their quiet jumps are
    mostly up onto something. Both keys get a press hazard of their own for quiet time (press_hazard_quiet), and jumps
    that land on something higher are not counted as presses (the bots take such places without them). With the enemy
    dead (after a kill, all human DM sessions as for the lean chain) the crouch gets one more (press_hazard_dead):
    reloading then, people crouch 0.2-0.9 times a minute, against 2.8 while reloading in a fight. Their jumps then
    (3.7 a minute, not onto anything) are left out: the owner asked for no jumps without a reason (2026-10-07)."""
    out = {}
    F = F.assign(walk_key=F.run.eq(0))
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    first = g.cumcount().eq(0)
    standing = ~F.ducked.astype(bool) & F.on_ground.astype(bool)
    quiet = quiet_rows(F, g)
    dead = ~F.eligible & ~F.opp_alive.fillna(False).astype(bool)
    for key, col in [("crouch", "crouch_key"), ("jump", "jump_key"), ("walk", "walk_key")]:
        # before the first live tick of a respawn every key was up (empty usercmds)
        prev = g[col].shift().where(~(first & F.spawn_seg), False)
        start = F[col] & prev.eq(False)
        idle = prev.eq(False)
        if key == "crouch":
            idle &= standing      # a press while crouched stands up: that is the stand-up hazard
        if key == "jump":
            start &= ~terrain_jumps(F, start)
        E = F.assign(_s=start, _idle=idle, _q=quiet)
        E = E[E.eligible & E._idle]
        haz = E.groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
        hquiet = hdead = None
        if key in ("crouch", "jump"):
            hquiet = E[E._q].groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
            haz = E[~E._q].groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
            Ed = F.assign(_s=start, _idle=idle)[dead & idle]
            hdead = Ed.groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
        if key == "crouch":
            # people crouch three times as readily in the half second after the enemy fired (hidden 1.3 -> 4.2 a
            # minute, firing back in sight 1.7 -> 6.0); without it they hardly crouch. The bot hears every shot.
            fired = g.opp_attack_primary.transform(
                lambda s: s.fillna(0).astype(float).rolling(FIRE_HEARD_TICKS, min_periods=1).max()).gt(0)
            E = E.assign(_f=fired[E.index])
            hq = E[~E._f & ~E._q].groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
            hf = E[E._f].groupby("ctx_i")._s.mean().reindex(range(len(H.CONTEXTS))).fillna(0).to_numpy()
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
        if hquiet is not None:
            out[key]["press_hazard_quiet"] = hquiet.round(6)
            if key == "crouch":
                out[key]["press_hazard_dead"] = hdead.round(6)
        if key == "crouch":
            out[key]["press_hazard"] = hq.round(6)
            out[key]["press_hazard_fire"] = hf.round(6)
            out[key]["up_age_edges"] = UP_AGE_EDGES
            out[key]["up_hazard"] = fit_stand_up(F, g, first).round(5)
    return out


def main():
    F = H.load_dm()
    EL = F[F.eligible]
    # People press keys toward a wall they are touching about as often as in the open (forward 3.2% vs
    # 3.6% per tick) and slide along it, so walls veto nothing; only ledges deep enough to hurt do.
    part = {"lean": fit_lean(F, EL), "stance": fit_stance(F, EL), "veto_clearance": 0.0}
    p = H.write_part("movement", part)
    print("wrote", p)
    for k, v in part["stance"].items():
        print(k, "press/min by ctx", (np.asarray(v["press_hazard"]) * 1200).round(2), "hold median", v["hold_ms_median"])
        if "press_hazard_quiet" in v:
            print(k, "  quiet", (np.asarray(v["press_hazard_quiet"]) * 1200).round(2))
            if "press_hazard_dead" in v:
                print(k, "  enemy dead", (np.asarray(v["press_hazard_dead"]) * 1200).round(2))
        if "press_hazard_fire" in v:
            print(k, "  after enemy fire", (np.asarray(v["press_hazard_fire"]) * 1200).round(2))
    print("lean wall", part["lean"]["wall"])
    print("lean enemy", part["lean"]["enemy"]["logit"])


if __name__ == "__main__":
    main()
