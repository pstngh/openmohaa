"""Reload, weapon-switch and respawn policy (shared, pooled).

Runs the data repo's lives.py unchanged (as a subprocess, into the git-ignored
cache) and turns its pooled tables into sampling tables.
"""
import json
import subprocess
import sys
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402


def run_analysis(script):
    res = H.CACHE / "results"
    res.mkdir(parents=True, exist_ok=True)
    out = res / (Path(script).stem + ".json")
    if not out.exists():
        cache = H.ensure_features()
        subprocess.run([sys.executable, str(H.movement_repo() / "analysis" / script), str(cache), str(res)], check=True,
                       stdout=subprocess.DEVNULL)
    return json.loads(out.read_text())


def quantile_table(q):
    probs = [0.0]
    vals = [None]
    for k in ["p5", "p10", "p25", "p50", "p75", "p90", "p95"]:
        probs.append(int(k[1:]) / 100)
        vals.append(q[k])
    vals[0] = max(0.0, q["p5"] - (q["p10"] - q["p5"]))
    probs.append(1.0)
    vals.append(q["p95"] + (q["p95"] - q["p90"]))
    return probs, vals


TAC_CLIP_EDGES = [0, 0.125, 0.25, 0.5, 0.75]   # rounds left as a share of the clip (lower edges)
TAC_SEEN_EDGES = [0, 1000]                     # ms since a part of the enemy was last on screen (lower edges)


def fit_tactical():
    """Reloads begun with the enemy alive and out of sight, rounds still in the clip: hazard per tick by rounds left
    and time since sight. People reload early (0.15-0.2 a second with an eighth of the clip left, 0.01 with three
    quarters), so they seldom meet the enemy with a near-empty clip; the bots' hand-set hazard (0.004 a tick below
    half a clip) left them running dry in sight five times as often."""
    F = H.load_dm()
    key = [F.session_id, F.client_id, F.seg]
    nxr = F.reloading.astype(float).groupby(key, sort=False).shift(-1)
    gap = F.session_ms.groupby(key, sort=False).shift(-1) - F.session_ms
    E = F[F.eligible & F.vis.eq(0) & ~F.reloading & F.clip_ammo.gt(0) & F.clip_ammo.lt(F.clip_size)
          & F.weapon.isin(["MP40", "Thompson"]) & nxr.notna() & gap.eq(50)]
    y = nxr[E.index].to_numpy()
    cb = H.binidx(E.clip_ammo / E.clip_size, TAC_CLIP_EDGES)
    sb = H.binidx(E.vage.fillna(1e9), TAC_SEEN_EDGES)
    T = np.zeros((len(TAC_CLIP_EDGES), len(TAC_SEEN_EDGES)))
    for c in range(len(TAC_CLIP_EDGES)):
        mc = cb == c
        prior = (y[mc].sum() + 1e-3) / (mc.sum() + 1.0)
        for s in range(len(TAC_SEEN_EDGES)):
            m = mc & (sb == s)
            T[c, s] = (y[m].sum() + 200 * prior) / (m.sum() + 200)
    return {"clip_edges": TAC_CLIP_EDGES, "seen_edges": TAC_SEEN_EDGES, "hazard": T.round(6)}


def main():
    L = run_analysis("lives.py")
    pk = L["post_kill_reload_by_rounds_left"]
    edges, ps = [], []
    for k, v in pk.items():
        lo = int(k.split("-")[0])
        edges.append(lo)
        ps.append(v["p_reload_within_3s"])
    order = np.argsort(edges)
    dprobs, dms = quantile_table(L["post_kill_reload_delay_ms"])
    rprobs, rms = quantile_table(L["respawn_delay_ms"])
    part = {"post_kill_round_edges": [edges[i] for i in order], "post_kill_reload_p": [ps[i] for i in order],
            "post_kill_delay": {"probs": dprobs, "ms": dms},
            "respawn": {"probs": rprobs, "ms": rms},
            "reload_context_pct": L["reload_context_pct"],
            "weapon_switch_rate_per_min": L["weapon_switch_rate_per_min"],
            "tactical": fit_tactical()}
    H.write_part("weapon", part)
    print(json.dumps(H.jsonable(part), indent=0)[:1500])


if __name__ == "__main__":
    main()
