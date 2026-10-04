"""Style dials and the anonymous style distribution (humanbot/model/styles.json).

Each dial is a human-readable target measured the same way for every alias x capture
row of the duel cohort (the within-person spread comes from people with several
captures). People are clustered into style families; the published distribution
keeps only family weights, centres and spreads, the human min-max of every dial and
the skill ranges. No alias, person or per-row value is written out.
"""
import json
import sys
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

STYLE_DIALS = ["fwd_diag_fight", "reverse_share", "side_hold_ms", "lean_fight", "jumps_per_min", "crouch_per_min",
               "walk_hidden", "burst_median", "aim_height_firing", "hold_angle", "counter_strafe"]
SKILL_DIALS = ["aim_error_fight_deg", "reaction_ms"]
FAMILY_NAMES = ["presser", "strafer", "stopper"]
# dials that define the families (REPORT section 9): diagonal press, reverse-vs-stop, lean habit
CLUSTER_DIALS = ["fwd_diag_fight", "reverse_share", "lean_fight"]


def complete_side_holds(d):
    g = d.groupby(["session_id", "client_id", "seg"], sort=False)
    rid = (d.side != g.side.shift()).cumsum()
    r = d.assign(_r=rid).groupby("_r").agg(v=("side", "first"), n=("side", "size"), el=("eligible", "min"))
    edges = d.assign(_r=rid).groupby(["session_id", "client_id", "seg"])._r.agg(["min", "max"])
    r = r[~r.index.isin(set(edges["min"]) | set(edges["max"])) & r.el & r.v.ne(0)]
    return r.n * 50


def counter_strafe(d):
    """Of the strafes let go to no strafe key, the share followed by a strafe (either side) the very next tick; pauses
    cut by a segment's end are left out. People 0.25-0.73: the stoppers turn this way more than they reverse."""
    n = k = 0
    for _, g in d.groupby(["session_id", "client_id", "seg"], sort=False):
        s = g.side.to_numpy()
        nz = np.flatnonzero(s != 0)
        ends = np.flatnonzero((s[:-1] != 0) & (s[1:] == 0))
        nxt = np.searchsorted(nz, ends + 2)
        ok = nxt < len(nz)
        n += int(ok.sum())
        k += int((nz[nxt[ok]] == ends[ok] + 2).sum())
    return k / n if n else np.nan


def edges_per_min(d, col):
    g = d.groupby(["session_id", "client_id", "seg"], sort=False)
    st = d[col] & g[col].shift().eq(False)
    el = d.eligible
    return st[el].sum() / max(el.sum() / 1200, 1e-9)


def bursts(events, F):
    """Median burst length (consecutive 100 ms SMG shots, eligible shots only) per alias x capture, over the bursts
    whose first round left with a body part of the enemy on screen (perception.burst_in_sight). The dial shifts the
    release in sight only; fire into cover is let go of as people let go of it, whatever the style."""
    S = events[events.event.eq("shot") & events.actor_bot.eq(0) & events.weapon.isin(["MP40", "Thompson"])]
    S = S.rename(columns={"actor_id": "client_id"})
    fk = F.set_index(["session_id", "client_id", "session_ms"])[["eligible", "name", "capture_date", "vis"]]
    m = fk.reindex(pd.MultiIndex.from_frame(S[["session_id", "client_id", "session_ms"]]))
    S = S.assign(el=m.eligible.to_numpy(), name=m.name.to_numpy(), cap=m.capture_date.to_numpy(), vis=m.vis.to_numpy())
    S = S[S.el.eq(True)].sort_values(["session_id", "client_id", "session_ms"])
    gap = S.groupby(["session_id", "client_id"]).session_ms.diff()
    S["bid"] = (~gap.eq(100)).cumsum()
    b = S.groupby("bid").agg(n=("session_ms", "size"), name=("name", "first"), cap=("cap", "first"), vis=("vis", "first"))
    return b[b.vis.eq(1)].groupby(["name", "cap"]).n.median()


def reactions(F):
    """Clean first trigger press after a sight gain (acquisition.py definition), per alias x capture."""
    rows = []
    PRE = 10
    cols = ["line_of_sight", "eligible", "attack", "aim_total_error", "tgt_half_w_deg"]
    for (sid, cid, seg), b in F[F.seg.ge(0)].groupby(["session_id", "client_id", "seg"], sort=False):
        if len(b) < PRE + 6:
            continue
        los = b.line_of_sight.to_numpy()
        el = b.eligible.to_numpy()
        att = b.attack.to_numpy()
        err = b.aim_total_error.to_numpy()
        hw = b.tgt_half_w_deg.to_numpy()
        for i in np.flatnonzero((los[1:] == 1) & (los[:-1] == 0)) + 1:
            if i < PRE or not el[i] or los[i - PRE:i].any() or i + 6 > len(b) or not (los[i:i + 6] == 1).all():
                continue
            if att[i - 1] or not err[i] > 2 * hw[i]:
                continue
            vis = los[i:i + 30] == 1
            bout = np.cumprod(vis).astype(bool)
            press = np.flatnonzero(att[i:i + 30][:len(bout)] & bout)
            rows.append({"name": b.name.iloc[0], "cap": b.capture_date.iloc[0], "t": press[0] * 50 if len(press) else np.nan})
    R = pd.DataFrame(rows)
    return R.groupby(["name", "cap"]).t.median()


def hold_angles():
    """Hold or clear an angle (REPORT section 14): the share of part sightings with the crosshair parked within
    5 deg of where the enemy will appear, 500 ms before the first part, per alias x capture. Definitions of the
    data repo's perception_all.py on its rebuilt body-part column (humanbot/eval/perception.py)."""
    sys.path.insert(0, str(H.HB_ROOT / "eval"))
    import perception as PC
    T = PC.tables(H.analysis_cache(), H.movement_repo() / "analysis" / "results", None, verbose=False, corners=False)
    P = T.P.assign(name=T.F.name.to_numpy()[T.P.row], cap=T.F.capture_date.to_numpy()[T.P.row])
    return P.groupby(["name", "cap"]).held.mean()


def dial_table(F, E):
    EL = F[F.eligible]
    rows = {}
    bmed = bursts(E, F)
    react = reactions(F)
    holds = hold_angles()
    for (name, cap), d in EL.groupby(["name", "capture_date"]):
        full = F[F.name.eq(name) & F.capture_date.eq(cap)]
        lf = d[d.ctx.eq("los_fire")]
        t = d[(d.side != 0) & d.nx_side.notna() & (d.nx_side != d.side)]
        hid = d[d.ctx.eq("hidden_nofire")]
        smg = d.weapon.isin(["MP40", "Thompson"])
        rows[(name, cap)] = {
            "person": H.import_analysis()[0].PERSON.get(name, name),
            "minutes": len(d) / 1200,
            "fwd_diag_fight": lf.action.isin([6, 8]).mean(),
            "reverse_share": (t.nx_side == -t.side).mean(),
            "side_hold_ms": complete_side_holds(full).median(),
            "lean_fight": (lf.lean != 0).mean(),
            "jumps_per_min": edges_per_min(full.assign(k=full.jump_key), "k"),
            "crouch_per_min": edges_per_min(full.assign(k=full.crouch_key), "k"),
            "walk_hidden": hid.run.eq(0).mean(),
            "burst_median": float(bmed.get((name, cap), np.nan)),
            "aim_height_firing": lf.aim_height_fraction.median(),
            "hold_angle": float(holds.get((name, cap), np.nan)),
            "counter_strafe": counter_strafe(full),
            "mp40_share": d.weapon.eq("MP40").sum() / max(1, smg.sum()),
            "aim_error_fight_deg": lf.aim_total_error.median(),
            "reaction_ms": float(react.get((name, cap), np.nan)),
        }
    return pd.DataFrame(rows).T


def cluster_people(P):
    """k-means (k=3, deterministic farthest-point init) on z-scored person means."""
    X = P[CLUSTER_DIALS].astype(float)
    Z = ((X - X.mean()) / X.std()).to_numpy()
    n = len(Z)
    # init: the two farthest people, then the farthest from both
    D = ((Z[:, None, :] - Z[None, :, :]) ** 2).sum(-1)
    i, j = np.unravel_index(D.argmax(), D.shape)
    k = int(np.argmax(np.minimum(D[i], D[j])))
    C = Z[[i, j, k]].copy()
    lab = np.zeros(n, int)
    for _ in range(50):
        lab = ((Z[:, None, :] - C[None, :, :]) ** 2).sum(-1).argmin(1)
        C2 = np.array([Z[lab == c].mean(0) if (lab == c).any() else C[c] for c in range(3)])
        if np.allclose(C2, C):
            break
        C = C2
    return pd.Series(lab, index=P.index)


def main():
    F = H.load_dm()
    common, _ = H.import_analysis()
    E = pd.read_parquet(H.analysis_cache() / "events.parquet",
                        columns=["session_id", "session_ms", "event", "actor_id", "actor_bot", "weapon"])
    T = dial_table(F, E)
    num = T.drop(columns=["person"]).astype(float)
    P = num.groupby(T.person).apply(lambda d: d.drop(columns=["minutes"]).mean())
    lab = cluster_people(P)
    # name families by their habits: presser = most forward-diagonal, stopper = lowest reversal
    fam = {}
    order = P.groupby(lab).fwd_diag_fight.mean().sort_values(ascending=False).index.tolist()
    fam[order[0]] = "presser"
    rest = order[1:]
    rv = P.groupby(lab).reverse_share.mean()
    stop = min(rest, key=lambda c: rv[c])
    fam[stop] = "stopper"
    fam[[c for c in rest if c != stop][0]] = "strafer"
    person_family = lab.map(fam)
    T["family"] = T.person.map(person_family)
    out = {"version": 1, "dials": STYLE_DIALS, "skill_dials": SKILL_DIALS, "families": []}
    n_people = len(P)
    for f in FAMILY_NAMES:
        people = person_family[person_family.eq(f)].index
        rows = num[T.family.eq(f)]
        centre = P.loc[people, STYLE_DIALS].mean()
        between = P.loc[people, STYLE_DIALS].std(ddof=1) if len(people) > 1 else pd.Series(0.0, index=STYLE_DIALS)
        # within-person spread across captures, pooled over everyone with >= 2 captures
        within = []
        for p, d in num[STYLE_DIALS].groupby(T.person):
            if len(d) >= 2:
                within.append(d.var(ddof=1))
        within = np.sqrt(pd.concat(within, axis=1).mean(axis=1))
        spread = np.sqrt(between.fillna(0) ** 2 + within ** 2)
        # weight: the chance a bot draws the family (each person alike); minutes_share: the family's share of the
        # recorded duel minutes, the mix of the pooled human reference (compare.py reweights the bots to it)
        out["families"].append({"name": f, "weight": round(len(people) / n_people, 4), "people": int(len(people)),
                                "minutes_share": round(float(num.minutes[T.family.eq(f)].sum() / num.minutes.sum()), 4),
                                "centre": {k: round(float(centre[k]), 4) for k in STYLE_DIALS},
                                "spread": {k: round(float(spread[k]), 4) for k in STYLE_DIALS}})
    out["min"] = {k: round(float(num[k].min()), 4) for k in STYLE_DIALS + SKILL_DIALS}
    out["max"] = {k: round(float(num[k].max()), 4) for k in STYLE_DIALS + SKILL_DIALS}
    # weapon preference: bimodal (people switch between sessions), from the alias x capture rows
    mp = np.sort(num.mp40_share.to_numpy())
    comps = [("thompson", mp[mp < 0.3]), ("split", mp[(mp >= 0.3) & (mp < 0.75)]), ("mp40", mp[mp >= 0.75])]
    out["weapon_mix"] = [{"name": nm, "weight": round(len(v) / len(mp), 4), "mean": round(float(v.mean()), 4) if len(v) else 0.5,
                          "sd": round(float(max(v.std(), 0.05)) if len(v) > 1 else 0.08, 4)} for nm, v in comps]
    out["pooled"] = {k: round(float(np.average(num[k], weights=num.minutes)), 4) for k in STYLE_DIALS + SKILL_DIALS + ["mp40_share"]}
    (H.HB_ROOT / "model").mkdir(parents=True, exist_ok=True)
    (H.HB_ROOT / "model" / "styles.json").write_text(json.dumps(H.jsonable(out), indent=1))
    # private diagnostics stay in the git-ignored cache
    T.to_csv(H.CACHE / "style_dials_by_alias_capture.csv")
    (H.CACHE / "style_families_private.json").write_text(json.dumps(person_family.to_dict(), indent=1))
    pd.set_option("display.width", 250)
    print(T.round(3).to_string())
    print("families:", person_family.to_dict())
    print(json.dumps(H.jsonable(out["families"]), indent=0)[:3000])


if __name__ == "__main__":
    main()
