"""Fit the trigger model (shared, pooled): per-tick press and release hazards.

All hazards are estimated on unbroken segments of the duel mask, excluding reloads
and empty clips, as additive logits:

  with LOS   press   = b + E[err bin] + L[time since LOS gain bin] + G[time since release bin] + D * damaged
             release = b + E[err bin] + L[time since LOS gain bin] + A[hold age bin]
  hidden     press   = b + Y[|yaw err| bin] + L[time since LOS loss bin] + G[gap bin] + D * damaged
             release = b + Y[|yaw err| bin] + L[time since LOS loss, 50 ms steps] + A[hold age bin]

Error with LOS is in opponent body half-widths; hidden, it is the yaw error to the
(unseen) opponent, which the bot replaces with the error to its belief.
"""
import sys
from pathlib import Path
import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

EN_EDGES = [0, 0.5, 1, 1.5, 2, 3, 4, 6, 10, 20]
YAW_EDGES = [0, 5, 10, 20, 45, 90]
LAGE_LOS_EDGES = [0, 50, 100, 150, 200, 250, 350, 500, 750, 1000, 1500, 2500]
LAGE_HID_EDGES = [0, 50, 100, 150, 200, 250, 300, 350, 400, 500, 600, 750, 1000, 1500, 2500, 5000]
HOLD_EDGES = [1, 2, 3, 4, 6, 9, 13, 21]          # ticks (1 = first tick held)
GAP_EDGES = [1, 2, 3, 4, 6, 9, 13, 21, 41, 81]    # ticks since release


def fit_side(d, press, los):
    y = d.nx_attack.to_numpy().astype(float)
    if not press:
        y = 1.0 - y
    err_codes = H.binidx(d.en, EN_EDGES) if los else H.binidx(d.aim_yaw_error.abs(), YAW_EDGES)
    lage_edges = LAGE_LOS_EDGES if los else LAGE_HID_EDGES
    lage = H.binidx(d.lage, lage_edges)
    age = H.binidx(d.age_attack, GAP_EDGES if press else HOLD_EDGES)
    dmg = (d.ev_dmg_taken.to_numpy() > 0).astype(int)
    codes = [err_codes, lage, age]
    sizes = [len(EN_EDGES) if los else len(YAW_EDGES), len(lage_edges), len(GAP_EDGES) if press else len(HOLD_EDGES)]
    if press:
        codes.append(dmg)
        sizes.append(2)
    b0, co = H.fit_logistic(codes, y, sizes, l2=0.5)
    out = {"bias": round(b0, 4), "err": co[0].round(4), "lage": co[1].round(4), "age": co[2].round(4), "n": int(len(d)),
           "rate": round(float(y.mean()), 5)}
    if press:
        out["damaged"] = round(float(co[3][1] - co[3][0]), 4)
        out["bias"] = round(b0 + float(co[3][0]), 4)
    return out


def main():
    F = H.load_dm()
    EL = F[F.eligible]
    TE = EL[~EL.reloading & EL.clip_ammo.gt(0) & EL.nx_attack.notna() & EL.lage.notna() & EL.known_attack]
    part = {"en_edges": EN_EDGES, "yaw_edges": YAW_EDGES, "lage_los_edges": LAGE_LOS_EDGES, "lage_hidden_edges": LAGE_HID_EDGES,
            "hold_edges": HOLD_EDGES, "gap_edges": GAP_EDGES}
    part["press_los"] = fit_side(TE[~TE.attack & TE.line_of_sight.eq(1)], True, True)
    part["release_los"] = fit_side(TE[TE.attack & TE.line_of_sight.eq(1)], False, True)
    part["press_hidden"] = fit_side(TE[~TE.attack & TE.line_of_sight.eq(0)], True, False)
    part["release_hidden"] = fit_side(TE[TE.attack & TE.line_of_sight.eq(0)], False, False)
    H.write_part("trigger", part)
    for k in ["press_los", "release_los", "press_hidden", "release_hidden"]:
        v = part[k]
        print(k, "n", v["n"], "rate", v["rate"], "bias", v["bias"], "dmg", v.get("damaged"))
        print("  err", v["err"], "\n  lage", v["lage"], "\n  age", v["age"])


if __name__ == "__main__":
    main()
