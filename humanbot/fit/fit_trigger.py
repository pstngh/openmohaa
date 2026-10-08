"""Fit the trigger model (shared, pooled): per-tick press and release hazards.

All hazards are estimated on unbroken segments of the 1v1 duels (hbdata.widen_duels) with a
known part count, excluding reloads and empty clips, as additive logits:

  in sight   press   = b + E[err bin] + L[time since sight bin] + G[time since release bin] + D * damaged
             release = b + E[err bin] + L[time since sight bin] + A[hold age bin]
  hidden     press   = b + Y[|yaw err| bin] + L[time since sight loss bin] + G[gap bin] + D * damaged
             release = b + Y[|yaw err| bin] + L[time since sight loss, 50 ms steps] + A[hold age bin]

"In sight" is a body part of the opponent on screen (the data repo's rebuilt ext_vis_parts, or the logged one),
not the centroid ray: people's reaction runs from the first visible part (REPORT section 14),
they fire at visible parts, and the bot's own sight is its perceived parts. Its clock (`vage`)
starts when the parts come on screen. Error in sight is to the centroid in opponent body
half-widths; hidden, it is the yaw error to the (unseen) opponent, which the bot replaces
with the error to its belief.
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
# rounds left, a share of the clip (lower edges): with an eighth of it left people press 2.5 times less readily in
# sight than with half of it, and the bots, which did not, ran dry in sight four times as often per sighting
CLIP_EDGES = [0, 0.125, 0.25, 0.5]


def fit_side(d, press, los):
    y = d.nx_attack.to_numpy().astype(float)
    if not press:
        y = 1.0 - y
    err_codes = H.binidx(d.en, EN_EDGES) if los else H.binidx(d.aim_yaw_error.abs(), YAW_EDGES)
    lage_edges = LAGE_LOS_EDGES if los else LAGE_HID_EDGES
    lage = H.binidx(d.vage, lage_edges)
    age = H.binidx(d.age_attack, GAP_EDGES if press else HOLD_EDGES)
    dmg = (d.ev_dmg_taken.to_numpy() > 0).astype(int)
    codes = [err_codes, lage, age]
    sizes = [len(EN_EDGES) if los else len(YAW_EDGES), len(lage_edges), len(GAP_EDGES) if press else len(HOLD_EDGES)]
    if press:
        codes += [dmg, H.binidx(d.clip_ammo / d.clip_size.clip(lower=1), CLIP_EDGES)]
        sizes += [2, len(CLIP_EDGES)]
    b0, co = H.fit_logistic(codes, y, sizes, l2=0.5)
    out = {"bias": round(b0, 4), "err": co[0].round(4), "lage": co[1].round(4), "age": co[2].round(4), "n": int(len(d)),
           "rate": round(float(y.mean()), 5)}
    if press:
        # relative to the fullest bin, which goes into the bias
        out["damaged"] = round(float(co[3][1] - co[3][0]), 4)
        out["clip"] = (co[4] - co[4][-1]).round(4)
        out["bias"] = round(b0 + float(co[3][0]) + float(co[4][-1]), 4)
    return out


def main():
    F = H.load_dm()
    # the respawn click is still held or repeated in the first live ticks (fitted in fit_keys.py as the spawn click)
    EL = F[F.eligible & ~(F.spawn_seg & F.seg_k.lt(H.SPAWN_CLICK_TICKS))]
    # the sight's clock runs from the first part on screen: rows without a part count (hbdata.attach_vis_parts) are left out
    TE = EL[EL.vis_known & ~EL.reloading & EL.clip_ammo.gt(0) & EL.nx_attack.notna() & EL.vage.notna() & EL.known_attack]
    part = {"en_edges": EN_EDGES, "yaw_edges": YAW_EDGES, "lage_los_edges": LAGE_LOS_EDGES, "lage_hidden_edges": LAGE_HID_EDGES,
            "hold_edges": HOLD_EDGES, "gap_edges": GAP_EDGES, "clip_edges": CLIP_EDGES}
    part["press_los"] = fit_side(TE[~TE.attack & TE.vis.eq(1)], True, True)
    part["release_los"] = fit_side(TE[TE.attack & TE.vis.eq(1)], False, True)
    part["press_hidden"] = fit_side(TE[~TE.attack & TE.vis.eq(0)], True, False)
    part["release_hidden"] = fit_side(TE[TE.attack & TE.vis.eq(0)], False, False)
    part["sight"] = "a body part on screen (rebuilt or logged ext_vis_parts); the clock runs from when the parts came on screen"
    H.write_part("trigger", part)
    for k in ["press_los", "release_los", "press_hidden", "release_hidden"]:
        v = part[k]
        print(k, "n", v["n"], "rate", v["rate"], "bias", v["bias"], "dmg", v.get("damaged"), "clip", v.get("clip"))
        print("  err", v["err"], "\n  lage", v["lage"], "\n  age", v["age"])


if __name__ == "__main__":
    main()
