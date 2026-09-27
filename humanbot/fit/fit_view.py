"""Fit the view model (shared, pooled).

Yaw controller (degrees per 50 ms tick), with the lags aim_model.py actually selected:
  rate[n] = rho*rate[n-1] + Kp*err[n-2] + Kself*wself[n-1] + Kopp*wopp[n-4] + b + eps[n]
fitted separately for firing and idle, on the smooth-tracking regime (|err| < FLICK_DEG,
mouse moving). Larger errors are closed by main-sequence flicks; still ticks come from a
two-state still gate. eps is AR(1) Student-t noise whose size is a lateral offset at the
target (world units), so the angle shrinks with distance.
Pitch: prate[n] = rho*prate[n-1] + Kp*perr[n-1] + Kt*tgt_pitch_rate[n-3] + b + eps.
"""
import sys
from pathlib import Path

import numpy as np
import pandas as pd
from scipy import optimize, stats

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

FLICK_DEG = 12.0
L = 6


def lagged(F):
    F = F.copy()
    F["w_self"] = np.degrees(-F.self_tangential_speed / F.distance_xy.clip(lower=20)) * 0.05
    F["w_opp"] = np.degrees(-F.opp_self_tangential_speed / F.distance_xy.clip(lower=20)) * 0.05
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    for k in range(L):
        F[f"e{k}"] = g.aim_yaw_error.shift(k)
        F[f"o{k}"] = g.w_opp.shift(k)
        F[f"l{k}"] = g.line_of_sight.shift(k)
        F[f"pe{k}"] = g.aim_pitch_error.shift(k)
        F[f"pw{k}"] = g.tgt_pitch_d.shift(k)
    F["y"] = g.yaw_d.shift(-1)
    F["py"] = g.pitch_d.shift(-1)
    F["ly"] = g.line_of_sight.shift(-1)
    F["nx_attack_v"] = g.attack.shift(-1)
    return F


def lsq(d, cols, target):
    X = np.c_[d[cols].to_numpy(), np.ones(len(d))]
    yv = d[target].to_numpy()
    c, *_ = np.linalg.lstsq(X, yv, rcond=None)
    res = yv - X @ c
    r2 = 1 - (res ** 2).sum() / ((yv - yv.mean()) ** 2).sum()
    return c, r2, res


def noise_model(res, dist, seg_keys):
    """Scale of the residual as a function of distance, t shape and lag-1 autocorrelation."""
    dist = np.asarray(dist)
    ab = np.abs(res)
    edges = np.array([80, 128, 192, 256, 320, 384, 512, 768, 1200])
    mids, med = [], []
    for lo, hi in zip(edges[:-1], edges[1:]):
        m = (dist >= lo) & (dist < hi)
        if m.sum() > 200:
            mids.append(np.median(dist[m]))
            med.append(np.median(ab[m]))
    mids, med = np.array(mids), np.array(med)

    # median |eps| in degrees = deg(atan(a / d)) + c : a lateral size a (units) plus a floor c
    def loss(p):
        a, c = p
        return ((np.degrees(np.arctan(a / mids)) + c - med) ** 2).sum()

    a, c = optimize.minimize(loss, [5.0, 0.3], method="Nelder-Mead").x
    scale = np.degrees(np.arctan(a / dist)) + c
    z = res / scale
    nu, _, s = stats.t.fit(z, floc=0)
    # lag-1 autocorrelation within segments
    s_ = pd.Series(res)
    same = pd.Series(list(zip(*seg_keys))).eq(pd.Series(list(zip(*seg_keys))).shift())
    ac = float(np.corrcoef(s_[same].to_numpy(), s_.shift()[same].to_numpy())[0, 1])
    return {"median_abs_units": round(float(a), 4), "median_abs_floor_deg": round(float(c), 4), "t_nu": round(float(nu), 4),
            "t_scale_per_median": round(float(s), 4), "ar1": round(ac, 4),
            "bins_dist": mids.round(1), "bins_median_abs_deg": med.round(4)}


def fit_controllers(F):
    D = lagged(F)
    base = (D.eligible & D.y.notna() & D[[f"l{k}" for k in range(L)]].eq(1).all(axis=1) & D.ly.eq(1)
            & D.distance_xy.gt(80)
            & D[["yaw_d", "w_self"] + [f"e{k}" for k in range(L)] + [f"o{k}" for k in range(L)]].notna().all(axis=1))
    D = D[base].copy()
    out = {"flick_deg": FLICK_DEG}
    cols = ["yaw_d", "e1", "w_self", "o3"]
    for mode, attack in [("firing", True), ("idle", False)]:
        d = D[D.attack.eq(attack) & D.e1.abs().lt(FLICK_DEG) & D.y.abs().ge(0.01)]
        c, r2, res = lsq(d, cols, "y")
        nm = noise_model(res, d.distance_xyz.to_numpy(), [d.session_id.to_numpy(), d.client_id.to_numpy(), d.seg.to_numpy()])
        out[mode] = {"rho": round(c[0], 5), "Kp": round(c[1], 5), "Kself": round(c[2], 5), "Kopp": round(c[3], 5),
                     "bias": round(c[4], 5), "R2": round(r2, 4), "n": int(len(d)), "noise": nm}
    # pitch
    P = D[D.py.notna() & D.pitch_d.notna() & D[["pe0", "pw2"]].notna().all(axis=1) & D.pe0.abs().lt(30) & D.py.abs().ge(0.01)]
    for mode, attack in [("firing", True), ("idle", False)]:
        d = P[P.attack.eq(attack)]
        c, r2, res = lsq(d, ["pitch_d", "pe0", "pw2"], "py")
        nm = noise_model(res, d.distance_xyz.to_numpy(), [d.session_id.to_numpy(), d.client_id.to_numpy(), d.seg.to_numpy()])
        out["pitch_" + mode] = {"rho": round(c[0], 5), "Kp": round(c[1], 5), "Kt": round(c[2], 5), "bias": round(c[3], 5),
                                "R2": round(r2, 4), "n": int(len(d)), "noise": nm}
    return out


def fit_still(F):
    g = F.groupby(["session_id", "client_id", "seg"], sort=False)
    ys = F.yaw_d.abs() < 0.01
    nx = ys.astype(float).groupby([F.session_id, F.client_id, F.seg], sort=False).shift(-1)
    E = F.assign(ys=ys, nx=nx)
    E = E[E.eligible & E.yaw_d.notna() & E.nx.notna()]
    t = E.groupby(["ctx_i", "ys"]).nx.mean().unstack()
    enter = t[False].reindex(range(len(H.CONTEXTS))).to_numpy(float)
    stay = t[True].reindex(range(len(H.CONTEXTS))).to_numpy(float)
    share = E.groupby("ctx_i").ys.mean().reindex(range(len(H.CONTEXTS))).to_numpy(float)
    return {"enter": enter.round(5), "stay": stay.round(5), "share": share.round(5)}


def fit_flicks(F):
    """Turn segments (runs of same-sign yaw steps >= 3 deg/tick), like behavior.py."""
    V = F[F.yaw_d.notna()].copy()
    V["sg"] = np.sign(V.yaw_d).where(V.yaw_d.abs() >= 3, 0)
    V["u"] = (V.sg != V.groupby(["session_id", "client_id", "seg"]).sg.shift()).cumsum()
    U = V[V.sg != 0].groupby("u").agg(n=("yaw_d", "size"), amp=("yaw_d", lambda s: s.abs().sum()),
                                      peak=("yaw_d", lambda s: s.abs().max()), los=("line_of_sight", "first"),
                                      e0=("aim_yaw_error", "first"), e1=("aim_yaw_error", "last"), sg=("sg", "first"),
                                      el=("eligible", "first"))
    U = U[U.el]
    amp_edges = [3, 10, 20, 30, 45, 60, 90, 135, 180, 360]
    rows = []
    for lo, hi in zip(amp_edges[:-1], amp_edges[1:]):
        u = U[(U.amp >= lo) & (U.amp < hi)]
        if len(u) < 30:
            continue
        rows.append({"amp_lo": lo, "amp_hi": hi, "amp_med": float(u.amp.median()), "n": int(len(u)),
                     "ticks_med": float(u.n.median()), "ticks_p90": float(u.n.quantile(0.9)),
                     "ticks_sigma": float(np.log(max(u.n.quantile(0.9), 1) / max(u.n.median(), 1)) / 1.2816 if u.n.quantile(0.9) > u.n.median() else 0.25),
                     "peak_frac_med": float((u.peak / u.amp).median())})
    C = U[U.los.eq(1) & (np.sign(U.e0) == U.sg) & U.e0.abs().ge(10)]
    g = (C.amp / C.e0.abs()).clip(0.05, 3)
    lg = np.log(g)
    gain = {"median": round(float(np.exp(lg.median())), 4), "sigma": round(float(lg.std()), 4),
            "overshoot_share": round(float((np.sign(C.e1) != np.sign(C.e0)).mean()), 4), "n": int(len(C))}
    return {"main_sequence": rows, "corrective_gain": gain}


def main():
    F = H.load_dm()
    part = fit_controllers(F)
    part["still"] = fit_still(F)
    part.update(fit_flicks(F))
    H.write_part("view", part)
    for k in ["firing", "idle", "pitch_firing", "pitch_idle"]:
        v = part[k]
        print(k, {kk: vv for kk, vv in v.items() if kk != "noise"})
        print("   noise", {kk: vv for kk, vv in v["noise"].items() if not kk.startswith("bins")})
    print("still", part["still"])
    for r in part["main_sequence"]:
        print(r)
    print("gain", part["corrective_gain"])


if __name__ == "__main__":
    main()
