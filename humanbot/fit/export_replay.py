"""Export recorded human duel situations for the C++ replay harness (hb_replay).

Writes humanbot/cache/replay/<map>.hbr (git-ignored): every valid row of the
deathmatch sessions on the practice maps, in unbroken-segment order, as float32
columns. Segments that start with a respawn begin at the first live tick
(spawn_seg = 1): runs that start there are complete, not censored. The harness drives the bot modules through these situations and
computes the same statistics for the bot and for the recorded human.

File format (little endian):
  magic "HBR1", uint32 ncols, uint32 nrows, then ncols NUL-terminated names,
  then ncols x nrows float32 values (column-major).
"""
import struct
import sys
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hbdata as H  # noqa: E402

OUT = H.CACHE / "replay"

COLS = [
    # identity / sequence
    "seg_key", "spawn_seg", "t_ms", "eligible", "person_id",
    # context and inputs
    "ctx_i", "line_of_sight", "lage", "vis", "vage", "attack", "reloading", "clip_ammo", "clip_size", "weapon_ready",
    "action", "side", "fwd", "lean", "crouch_key", "jump_key", "run", "ducked", "on_ground",
    "en", "aim_yaw_error", "aim_pitch_error", "aim_total_error", "tgt_half_w_deg", "aim_height_fraction",
    "crosshair_on_opponent", "distance_xy", "distance_xyz", "relative_bearing", "opp_reloading",
    "view_yaw", "view_pitch", "yaw_d", "pitch_d",
    "origin_x", "origin_y", "origin_z", "eye_z", "velocity_x", "velocity_y", "velocity_z", "speed_xy",
    "opp_x", "opp_y", "opp_z", "opp_vx", "opp_vy", "opp_alive", "opp_attack", "opp_speed", "opp_walk",
    "opp_ducked", "self_approach_speed", "self_tangential_speed",
    "clear_front", "clear_back", "clear_left", "clear_right", "clear_front_left", "clear_front_right",
    "clear_back_left", "clear_back_right", "clear_move",
    "ev_dmg_taken", "ev_shot", "ev_kill", "ev_death", "opp_shot", "opp_reload_ev",
]


def main():
    F = H.load_dm()
    common, _ = H.import_analysis()
    F = F[F.seg.ge(0) & F["map"].isin(["dm/crnodoors", "dm/main", "dm/vents", "dm/downladder"])].copy()
    # opponent columns not in the default sequence table
    extra = pd.read_parquet(H.analysis_cache() / "features.parquet",
                            columns=["session_id", "client_id", "session_ms", "opp_cmd_forward", "opp_pm_flags", "opp_self_tangential_speed"],
                            filters=[("valid", "==", True), ("dm_session", "==", True)])
    F = F.merge(extra.drop(columns=["opp_self_tangential_speed"]), on=["session_id", "client_id", "session_ms"], how="left")
    # opponent's run key and events at the same tick (sounds for the belief replay)
    fr = pd.read_parquet(H.analysis_cache() / "features.parquet", columns=["session_id", "client_id", "session_ms", "run"],
                         filters=[("valid", "==", True)])
    fr = fr.rename(columns={"client_id": "opponent_id", "run": "opp_run"})
    F = F.merge(fr, on=["session_id", "opponent_id", "session_ms"], how="left")
    ev = pd.read_parquet(H.analysis_cache() / "events.parquet", columns=["session_id", "session_ms", "event", "actor_id"])
    shots = ev[ev.event.eq("shot")].groupby(["session_id", "actor_id", "session_ms"]).size().rename("opp_shot").reset_index()
    rel = ev[ev.event.eq("reload")].groupby(["session_id", "actor_id", "session_ms"]).size().rename("opp_reload_ev").reset_index()
    for d in (shots, rel):
        d.rename(columns={"actor_id": "opponent_id"}, inplace=True)
        F = F.merge(d, on=["session_id", "opponent_id", "session_ms"], how="left")
    F = F.sort_values(["session_id", "client_id", "session_ms"], kind="stable").reset_index(drop=True)
    seg_key = F.groupby(["session_id", "client_id", "seg"], sort=False).ngroup()
    persons = {p: i for i, p in enumerate(sorted(F.person.unique()))}
    out = pd.DataFrame({
        "seg_key": seg_key, "spawn_seg": F.spawn_seg.astype(float), "t_ms": F.session_ms, "eligible": F.eligible.astype(float), "person_id": F.person.map(persons),
        "ctx_i": F.ctx_i, "line_of_sight": F.line_of_sight, "lage": F.lage.fillna(-1), "vis": F.vis.astype(float),
        "vage": F.vage.fillna(-1), "attack": F.attack.astype(float),
        "reloading": F.reloading.astype(float), "clip_ammo": F.clip_ammo, "clip_size": F.clip_size,
        "weapon_ready": F.weapon_state.isin([0, 1]).astype(float),
        "action": F.action, "side": F.side, "fwd": F.fwd, "lean": F.lean, "crouch_key": F.crouch_key.astype(float),
        "jump_key": F.jump_key.astype(float), "run": F.run, "ducked": F.ducked.astype(float), "on_ground": F.on_ground,
        "en": F.en.fillna(-1), "aim_yaw_error": F.aim_yaw_error, "aim_pitch_error": F.aim_pitch_error,
        "aim_total_error": F.aim_total_error, "tgt_half_w_deg": F.tgt_half_w_deg, "aim_height_fraction": F.aim_height_fraction,
        "crosshair_on_opponent": F.crosshair_on_opponent, "distance_xy": F.distance_xy, "distance_xyz": F.distance_xyz,
        "relative_bearing": F.relative_bearing, "opp_reloading": F.opp_weapon_state.eq(5).astype(float),
        "view_yaw": F.view_yaw, "view_pitch": F.view_pitch, "yaw_d": F.yaw_d.fillna(0), "pitch_d": F.pitch_d.fillna(0),
        "origin_x": F.origin_x, "origin_y": F.origin_y, "origin_z": F.origin_z, "eye_z": F.eye_z,
        "velocity_x": F.velocity_x, "velocity_y": F.velocity_y, "velocity_z": F.velocity_z, "speed_xy": F.speed_xy,
        "opp_x": F.opponent_origin_x, "opp_y": F.opponent_origin_y, "opp_z": F.opponent_origin_z,
        "opp_vx": F.opponent_velocity_x, "opp_vy": F.opponent_velocity_y, "opp_alive": F.opp_alive.fillna(0),
        "opp_attack": F.opp_attack_primary.fillna(0), "opp_speed": F.opp_speed_xy.fillna(0),
        "opp_walk": F.opp_run.eq(0).astype(float), "opp_ducked": (F.opp_pm_flags.fillna(0).astype(int) & 1).astype(float),
        "self_approach_speed": F.self_approach_speed, "self_tangential_speed": F.self_tangential_speed,
        "clear_front": F.clear_front, "clear_back": F.clear_back, "clear_left": F.clear_left, "clear_right": F.clear_right,
        "clear_front_left": F.clear_front_left, "clear_front_right": F.clear_front_right,
        "clear_back_left": F.clear_back_left, "clear_back_right": F.clear_back_right, "clear_move": F.clear_move,
        "ev_dmg_taken": F.ev_dmg_taken, "ev_shot": F.ev_shot, "ev_kill": F.ev_kill, "ev_death": F.ev_death,
        "opp_shot": F.opp_shot.fillna(0), "opp_reload_ev": F.opp_reload_ev.fillna(0),
    })
    assert list(out.columns) == COLS
    OUT.mkdir(parents=True, exist_ok=True)
    for mp, d in out.groupby(F["map"]):
        path = OUT / (mp.replace("/", "_") + ".hbr")
        arr = d.to_numpy(dtype=np.float32)
        arr = np.nan_to_num(arr, nan=-9999.0)
        with open(path, "wb") as fh:
            fh.write(b"HBR1")
            fh.write(struct.pack("<II", arr.shape[1], arr.shape[0]))
            for c in COLS:
                fh.write(c.encode() + b"\0")
            fh.write(np.ascontiguousarray(arr.T).astype("<f4").tobytes())
        print(mp, arr.shape, path.stat().st_size // 1024, "KiB")


if __name__ == "__main__":
    main()
