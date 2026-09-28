#!/usr/bin/env python3
"""Synthetic schema-13 bot-eval capture for the tests (no human data involved).

A tiny 2D simulation on a flat arena with a few pillars: two human-imitation bots
(presser, strafer) and one human ("pstN") fight a free-for-all deathmatch. They strafe
with a switch hazard, press forward or strafe, lean, crouch, jump, walk, track the
nearest enemy with a lagged noisy view, fire MP40/Thompson every 100 ms, hit, kill,
reload after kills and when empty, and respawn. Every frame and event column of the
current build is written (headers parsed from the C++ column contract), so the
unchanged analysis scripts accept the loaded cache.

Usage: make_fixture.py OUT_DIR [--seconds 150] [--seed 1] [--zip] [--date 2026-10-01]
  writes OUT_DIR/telemetry/movement_{frames,events}.csv + movement_meta.txt and, with
  --zip, OUT_DIR/<date>_pstN_bot-eval_schema13_<sha12>.zip holding that triplet.
"""
from __future__ import annotations

import argparse
import io
import json
import math
import sys
import zipfile
from dataclasses import dataclass
from pathlib import Path

import numpy as np

EVAL_DIR = Path(__file__).resolve().parents[1]
if str(EVAL_DIR) not in sys.path:
    sys.path.insert(0, str(EVAL_DIR))
import capture_io as cio  # noqa: E402
from hbeval import FAMILY_NAMES, OWNER_ALIAS, load_styles  # noqa: E402

TICK = 50
ARENA = 1600.0
PILLARS = [(300, 300, 520, 420), (1080, 280, 1300, 400), (700, 700, 900, 900), (260, 1150, 420, 1360),
           (1150, 1150, 1370, 1300), (720, 60, 860, 200), (740, 1400, 880, 1540)]
EYE = 82.0
HALF_W = 15.0
HEIGHT = 94.0
CLIP = {"MP40": 32, "Thompson": 30}
RELOAD_TICKS = {"MP40": 60, "Thompson": 47}
HITLOCS = [(0, 50.0, 0.09), (3, 25.0, 0.30), (4, 23.8, 0.12), (5, 22.5, 0.14), (6, 21.2, 0.08), (7, 20.0, 0.08),
           (8, 20.0, 0.05), (9, 15.0, 0.07), (10, 15.0, 0.07)]
FAMILY_HABITS = {  # fwd-diagonal share in fights, direct reversal, lean in fights, jumps/min, crouches/min, release
    "presser": dict(diag=0.55, reverse=0.8, lean=0.9, jumps=0.6, crouch=2.0, release=0.02),
    "strafer": dict(diag=0.10, reverse=0.5, lean=0.55, jumps=4.0, crouch=7.0, release=0.04),
    "stopper": dict(diag=0.10, reverse=0.15, lean=0.9, jumps=2.3, crouch=2.2, release=0.10),
    "human": dict(diag=0.35, reverse=0.6, lean=0.8, jumps=1.7, crouch=3.9, release=0.03),
}


@dataclass
class PlayerSpec:
    cid: int
    name: str
    is_bot: bool
    family: str = "human"          # presser / strafer / stopper for bots
    seed: int = 0
    weapon: str = "MP40"


@dataclass
class SessionSpec:
    map: str
    seconds: float
    players: list
    epoch: int
    ms: int = 123


def default_players():
    return [PlayerSpec(0, OWNER_ALIAS, False, weapon="MP40"),
            PlayerSpec(1, "Cpl. Brandt", True, "presser", 1234567, "MP40"),
            PlayerSpec(2, "Pvt. Keller", True, "strafer", 7654321, "Thompson")]


def wrap(a):
    return (a + 180.0) % 360.0 - 180.0


def seg_hits_box(p, q, box):
    """Does the 2D segment p-q cross the axis-aligned box (x0, y0, x1, y1)? (Liang-Barsky)"""
    x0, y0, x1, y1 = box
    dx, dy = q[0] - p[0], q[1] - p[1]
    t0, t1 = 0.0, 1.0
    for pk, qk in ((-dx, p[0] - x0), (dx, x1 - p[0]), (-dy, p[1] - y0), (dy, y1 - p[1])):
        if abs(pk) < 1e-9:
            if qk < 0:
                return False
        else:
            t = qk / pk
            if pk < 0:
                t0 = max(t0, t)
            else:
                t1 = min(t1, t)
            if t0 > t1:
                return False
    return True


def blocked(p, q):
    return any(seg_hits_box(p, q, b) for b in PILLARS)


def inside_pillar(x, y, pad=16.0):
    return any(b[0] - pad < x < b[2] + pad and b[1] - pad < y < b[3] + pad for b in PILLARS)


def clearance(x, y, ang):
    """Free distance (<=128) from (x, y) in direction `ang` (deg) to arena walls and pillars."""
    c, s = math.cos(math.radians(ang)), math.sin(math.radians(ang))
    for d in range(8, 129, 8):
        px, py = x + c * d, y + s * d
        if px < 16 or py < 16 or px > ARENA - 16 or py > ARENA - 16 or inside_pillar(px, py, 0.0):
            return float(d - 8)
    return 128.0


def style_json(spec: PlayerSpec, styles: dict, rng) -> str:
    fam = next(f for f in styles["families"] if f["name"] == spec.family)
    d = {"family": spec.family, "seed": spec.seed}
    for k in styles["dials"]:
        v = rng.normal(fam["centre"][k], fam["spread"][k])
        d[k] = float(f"{min(max(v, styles['min'][k]), styles['max'][k]):.4g}")
    for k in styles["skill_dials"]:
        d[k] = float(f"{rng.uniform(styles['min'][k], styles['max'][k]):.4g}")
    d["mp40_share"] = 0.95 if spec.weapon == "MP40" else 0.07
    return json.dumps(d, separators=(",", ":"))


class Player:
    def __init__(self, spec: PlayerSpec, rng):
        self.s = spec
        self.h = FAMILY_HABITS[spec.family if spec.is_bot else "human"]
        self.rng = rng
        self.alive = False
        self.respawn_at = 0
        self.health = 0.0
        self.x = self.y = 0.0
        self.vx = self.vy = self.vz = 0.0
        self.yaw = float(rng.uniform(-180, 180))
        self.pitch = 0.0
        self.side, self.side_age, self.gap = 1, 1, 0
        self.fwd, self.fwd_left = 0, 0
        self.attack, self.fire_tick = False, 0
        self.lean, self.lean_left = 0, 0
        self.crouch_key, self.ducked = 0, 0
        self.jump_key, self.air = 0, 0
        self.walk = 0
        self.clip = CLIP[spec.weapon]
        self.reload_left = 0
        self.reload_due = -1
        self.shots_in_burst = 0
        self.last_seen = None      # (tick, bearing)
        self.look = self.yaw


def spawn_point(rng, players):
    for _ in range(200):
        x, y = rng.uniform(80, ARENA - 80, size=2)
        if not inside_pillar(x, y, 24) and all(not p.alive or math.hypot(p.x - x, p.y - y) > 350 for p in players):
            return float(x), float(y)
    return 100.0, 100.0


def simulate(spec: SessionSpec, styles: dict, seed: int):
    """Frame rows (dict of column -> list), event rows (list of dicts) and the meta section of one session."""
    rng = np.random.default_rng(seed)
    cols = cio.columns()
    sid = f"{spec.map.replace('/', '_')}_{spec.epoch}_{spec.ms}"
    ps = [Player(p, np.random.default_rng(seed * 100 + p.cid)) for p in spec.players]
    n_ticks = int(spec.seconds * 1000 / TICK)
    server0, frame0 = 7_000_000 + seed * 1000, 140_000
    F = {c: [] for c in cols.frames}
    E = []

    def event(t, kind, actor=None, target=None, **kw):
        row = {"session_ms": t * TICK, "server_ms": server0 + t * TICK, "frame": frame0 + t, "event": kind,
               "actor_id": actor.s.cid if actor else -1, "actor_name": actor.s.name if actor else "",
               "actor_bot": int(actor.s.is_bot) if actor else 0, "target_id": target.s.cid if target else -1,
               "target_name": target.s.name if target else "", "target_bot": int(target.s.is_bot) if target else 0}
        row.update(kw)
        E.append(row)

    event(0, "session_start")
    for p in ps:
        event(0, "client_begin", p, health_before=0.0, health_after=0.0)
        if p.s.is_bot:
            event(0, "bot_style", p, chat_message=style_json(p.s, styles, rng))
    for t in range(n_ticks):
        # respawns
        for p in ps:
            if not p.alive and t >= p.respawn_at:
                p.alive, p.health = True, 100.0
                p.x, p.y = spawn_point(rng, ps)
                p.vx = p.vy = p.vz = 0.0
                p.clip, p.reload_left, p.reload_due, p.attack = CLIP[p.s.weapon], 0, -1, False
                p.last_seen = None
                event(t, "spawn", p, health_before=100.0, health_after=100.0, position_x=p.x, position_y=p.y)
        live = [p for p in ps if p.alive]
        opp = {}
        for p in live:
            others = [q for q in live if q is not p]
            opp[p.s.cid] = min(others, key=lambda q: (q.x - p.x) ** 2 + (q.y - p.y) ** 2) if others else None
        geo = {}
        for p in live:
            q = opp[p.s.cid]
            if q is None:
                geo[p.s.cid] = None
                continue
            dx, dy = q.x - p.x, q.y - p.y
            dist = max(math.hypot(dx, dy), 1.0)
            los = not blocked((p.x, p.y), (q.x, q.y))
            bearing = math.degrees(math.atan2(dy, dx))
            geo[p.s.cid] = (q, dx, dy, dist, los, bearing)
        # decisions and motion
        for p in live:
            g = geo[p.s.cid]
            los = bool(g and g[4])
            if g and los:
                p.last_seen = (t, g[5])
            fight = los and p.attack
            # strafe: renewal hazard, reverse or stop
            if p.side != 0:
                p.side_age += 1
                haz = [0.0, 0.01, 0.06, 0.12, 0.17][min(p.side_age, 4)]
                if not fight:
                    haz *= 0.7
                if rng.random() < haz:
                    if rng.random() < p.h["reverse"]:
                        p.side, p.side_age = -p.side, 1
                    else:
                        p.side, p.gap = 0, int(rng.integers(1, 4) if fight else rng.integers(2, 30))
            else:
                p.gap -= 1
                if p.gap <= 0:
                    p.side, p.side_age = int(rng.choice([-1, 1])), 1
            p.fwd_left -= 1
            if p.fwd_left <= 0:
                p.fwd_left = int(rng.integers(4, 16))
                pf = p.h["diag"] if fight else 0.35
                u = rng.random()
                p.fwd = 1 if u < pf else (-1 if u > 0.95 else 0)
            p.lean_left -= 1
            if p.lean_left <= 0:
                p.lean_left = int(rng.integers(3, 9))
                pl = p.h["lean"] if fight else 0.45
                p.lean = (p.side or int(rng.choice([-1, 1]))) if rng.random() < pl else 0
            p.crouch_key = -127 if rng.random() < p.h["crouch"] / 1200 else 0
            if p.crouch_key:
                p.ducked = 6
            elif p.ducked:
                p.ducked -= 1
            p.jump_key = 127 if p.air == 0 and rng.random() < p.h["jumps"] / 1200 else 0
            if p.jump_key:
                p.air, p.vz = 10, 255.0
            if p.walk:
                p.walk -= 1
            elif not los and not p.attack and rng.random() < 0.004:
                p.walk = int(rng.integers(15, 30))
            # velocity from keys relative to the view
            th = math.radians(p.yaw)
            fx, fy, rx, ry = math.cos(th), math.sin(th), math.sin(th), -math.cos(th)
            mx, my = p.fwd * fx + p.side * rx, p.fwd * fy + p.side * ry
            n = math.hypot(mx, my)
            speed = 240.0 * (0.6 if p.walk else 1.0) * (0.45 if p.ducked else 1.0)
            tx, ty = (mx / n * speed, my / n * speed) if n > 0 else (0.0, 0.0)
            p.vx, p.vy = 0.45 * p.vx + 0.55 * tx, 0.45 * p.vy + 0.55 * ty
            if abs(p.vx) < 3 and abs(p.vy) < 3 and n == 0:
                p.vx = p.vy = 0.0
            nx, ny = p.x + p.vx * TICK / 1000, p.y + p.vy * TICK / 1000
            if nx < 32 or ny < 32 or nx > ARENA - 32 or ny > ARENA - 32 or inside_pillar(nx, ny, 16):
                p.vx = p.vy = 0.0
            else:
                p.x, p.y = nx, ny
            if p.air:
                p.air -= 1
                p.vz = p.vz - 800 * TICK / 1000 if p.air else 0.0
            # view: lagged tracking with Laplace noise; belief-driven look while hidden
            if g:
                q, dx, dy, dist, _, bearing = g
                if los:
                    err = wrap(bearing - p.yaw)
                    gain = 0.45 if p.attack else 0.25
                    p.yaw = wrap(p.yaw + gain * err + rng.laplace(0, 1.2 + 150 / dist))
                elif p.last_seen and t - p.last_seen[0] < 60:
                    p.yaw = wrap(p.yaw + 0.2 * wrap(p.last_seen[1] - p.yaw) + rng.laplace(0, 1.5))
                else:
                    if rng.random() < 0.012:
                        p.look = wrap(bearing + rng.normal(0, 40))
                    p.yaw = wrap(p.yaw + 0.25 * wrap(p.look - p.yaw) + (rng.laplace(0, 1.0) if rng.random() < 0.7 else 0.0))
                want_h = 0.45 if p.attack else 0.66
                want_pitch = math.degrees(math.atan2(EYE - want_h * HEIGHT, dist))
                p.pitch = 0.7 * p.pitch + 0.3 * want_pitch + rng.normal(0, 0.4)
            else:
                p.yaw = wrap(p.yaw + rng.laplace(0, 2.0))
            # trigger
            if p.reload_left == 0:
                if g:
                    q, dx, dy, dist, _, bearing = g
                    err = abs(wrap(bearing - p.yaw))
                    en = err / math.degrees(math.atan2(HALF_W, dist))
                else:
                    en = 99.0
                if not p.attack:
                    pp = (0.3 if en < 4 else 0.03) if los else (0.02 if p.last_seen and t - p.last_seen[0] < 20 else 0.002)
                    if rng.random() < pp and p.clip > 0:
                        p.attack, p.fire_tick = True, t
                else:
                    pr = (p.h["release"] if en < 4 else 0.3 if en > 10 else 0.08) if los else 0.15
                    if rng.random() < pr:
                        p.attack = False
            else:
                p.attack = False
        # weapons, shots, hits (after everyone moved)
        for p in live:
            if not p.alive:
                continue
            if p.reload_left > 0:
                p.reload_left -= 1
                if p.reload_left == 0:
                    p.clip = CLIP[p.s.weapon]
                continue
            want_reload = p.clip == 0 or (p.reload_due >= 0 and t >= p.reload_due and p.clip < CLIP[p.s.weapon])
            if want_reload and not p.attack:
                p.reload_left, p.reload_due, p.shots_in_burst = RELOAD_TICKS[p.s.weapon], -1, 0
                event(t, "reload", p, weapon=p.s.weapon, fire_mode=0, health_before=p.health, health_after=p.health)
                continue
            if p.attack and p.clip > 0 and (t - p.fire_tick) % 2 == 0:
                p.clip -= 1
                p.shots_in_burst += 1
                g = geo[p.s.cid]
                aim = {}
                if g:
                    q, dx, dy, dist, los, bearing = g
                    yaw_err = wrap(bearing - p.yaw)
                    aim = dict(aim_target_id=q.s.cid, aim_yaw_error=yaw_err, aim_total_error=abs(yaw_err),
                               line_of_sight=int(los), crosshair_on_target=int(los and abs(yaw_err) < math.degrees(math.atan2(HALF_W, dist))))
                event(t, "shot", p, weapon=p.s.weapon, fire_mode=0, health_before=p.health, health_after=p.health,
                      means_of_death=18, position_x=p.x, position_y=p.y, position_z=EYE, view_yaw=p.yaw, view_pitch=p.pitch, **aim)
                if g and g[4] and g[0].alive:
                    q = g[0]
                    on = aim["crosshair_on_target"] == 1
                    if rng.random() < (0.45 if on else 0.05):
                        loc, dmg, _ = HITLOCS[rng.choice(len(HITLOCS), p=[w for _, _, w in HITLOCS])]
                        before = q.health
                        q.health = max(0.0, q.health - dmg)
                        event(t, "damage", p, q, weapon=p.s.weapon, damage=dmg, health_before=before, health_after=q.health,
                              means_of_death=18, hit_location=loc, position_x=q.x, position_y=q.y, position_z=47.0, **aim)
                        if q.health <= 0:
                            q.alive, q.attack = False, False
                            q.respawn_at = t + int(rng.integers(30, 70))
                            event(t, "death", p, q, weapon=p.s.weapon, means_of_death=18, hit_location=loc,
                                  position_x=q.x, position_y=q.y)
                            if rng.random() < (0.97 if p.clip <= 20 else 0.7):
                                p.reload_due = t + int(rng.integers(11, 15))
            if not p.attack:
                p.shots_in_burst = 0
        # frame rows (every connected player, alive or not)
        for p in ps:
            g = geo.get(p.s.cid) if p.alive else None
            row = frame_row(p, g, t, sid, spec.map, server0, frame0, cols)
            for c in cols.frames:
                F[c].append(row.get(c, 0))
    event(n_ticks - 1, "session_end")
    meta = meta_section(sid, spec)
    return sid, F, E, meta


def frame_row(p: Player, g, t, sid, mapname, server0, frame0, cols) -> dict:
    s = p.s
    r = {"schema": cio.SCHEMA, "session_id": sid, "session_ms": t * TICK, "server_ms": server0 + t * TICK,
         "frame": frame0 + t, "frame_ms": TICK, "map": mapname, "client_id": s.cid, "name": s.name,
         "model": "allied_airborne" if s.cid % 2 == 0 else "german_wehrmacht_soldier", "is_bot": int(s.is_bot), "team": 2,
         "alive": int(p.alive), "spectator": 0, "ping": 0, "health": p.health if p.alive else 0.0, "max_health": 100.0,
         "weapon": s.weapon if p.alive else "", "opponent_name": "", "opponent_id": -1}
    for c, d in zip(cols.bot, cols.bot_defaults):
        r[c] = d
    if s.is_bot:          # the brain keeps its identity while the bot is dead
        r.update(bot_family=FAMILY_NAMES.index(s.family), bot_style_seed=s.seed, bot_kbd_ok=1)
    if not p.alive:
        r.update(weapon_state=-1, clip_ammo=-1, clip_size=-1, reserve_ammo=-1)
        return r
    speed = math.hypot(p.vx, p.vy)
    up = p.crouch_key or p.jump_key
    lean_l, lean_r = int(p.lean < 0), int(p.lean > 0)
    r.update(origin_x=p.x, origin_y=p.y, origin_z=0.0, eye_x=p.x, eye_y=p.y, eye_z=EYE if not p.ducked else 48.0,
             velocity_x=p.vx, velocity_y=p.vy, velocity_z=p.vz, speed_xy=speed, speed_xyz=math.hypot(speed, p.vz),
             view_pitch=p.pitch, view_yaw=p.yaw, view_roll=0.0, cmd_server_ms=server0 + t * TICK - 8, cmd_msec=8,
             cmd_angle_pitch=int(p.pitch * 65536 / 360) & 0xFFFF, cmd_angle_yaw=int(p.yaw * 65536 / 360) & 0xFFFF,
             cmd_forward=127 * p.fwd, cmd_right=127 * p.side, cmd_up=up, attack_primary=int(p.attack), run=int(not p.walk),
             lean_left=lean_l, lean_right=lean_r,
             buttons=int(p.attack) | (0 if p.walk else 2) | (lean_l << 4) | (lean_r << 5) | (1 << 14) | (1 << 15),
             pm_flags=(3 if p.ducked else 0) | (1024 if p.jump_key else 0), on_ground=int(p.air == 0),
             bbox_min_x=-15.0, bbox_min_y=-15.0, bbox_min_z=0.0, bbox_max_x=15.0, bbox_max_y=15.0,
             bbox_max_z=54.0 if p.ducked else 94.0, weapon_state=5 if p.reload_left else (1 if p.attack else 0),
             clip_ammo=p.clip, clip_size=CLIP[s.weapon], reserve_ammo=128,
             fire_spread_mult=0.3 + 0.24 * p.shots_in_burst if p.attack else 0.3,
             crosshair_entity=1022, crosshair_distance=512.0, ext_ping=0 if s.is_bot else 35, ext_usercmds=1 if s.is_bot else 6,
             ext_lean_angle=40.0 * p.lean, ext_eye_ofs_x=0.0, ext_eye_ofs_y=0.0, ext_eye_ofs_z=EYE, sight_fraction=1.0)
    th = p.yaw
    for c, a in (("clear_front", 0), ("clear_back", 180), ("clear_left", 90), ("clear_right", -90), ("clear_front_left", 45),
                 ("clear_front_right", -45), ("clear_back_left", 135), ("clear_back_right", -135)):
        r[c] = clearance(p.x, p.y, th + a)
    move_dir = th - 90 * p.side + (0 if p.fwd >= 0 else 180) if (p.fwd or p.side) else th
    r["clear_move"] = clearance(p.x, p.y, move_dir)
    r["clear_reverse"] = clearance(p.x, p.y, move_dir + 180)
    for c in ("clear_move_entity", "clear_reverse_entity", "clear_toward_opponent_entity", "clear_away_opponent_entity"):
        r[c] = 1022
    if g:
        q, dx, dy, dist, los, bearing = g
        yaw_err = wrap(bearing - p.yaw)
        tgt_pitch = math.degrees(math.atan2(EYE - 47.0, dist))
        pitch_err = tgt_pitch - p.pitch
        total = math.degrees(math.acos(max(-1.0, min(1.0, math.cos(math.radians(yaw_err)) * math.cos(math.radians(pitch_err))))))
        ux, uy = dx / dist, dy / dist
        half_w = math.degrees(math.atan2(HALF_W, dist))
        on = los and abs(yaw_err) < half_w and abs(pitch_err) < math.degrees(math.atan2(47.0, dist))
        r.update(opponent_id=q.s.cid, opponent_name=q.s.name, opponent_bot=int(q.s.is_bot), opponent_origin_x=q.x,
                 opponent_origin_y=q.y, opponent_origin_z=0.0, opponent_eye_x=q.x, opponent_eye_y=q.y, opponent_eye_z=EYE,
                 opponent_velocity_x=q.vx, opponent_velocity_y=q.vy, opponent_velocity_z=q.vz, distance_xy=dist,
                 distance_xyz=dist, body_gap_xy=max(0.0, dist - 30.0), body_contact=int(dist <= 30.0), height_delta=0.0,
                 relative_bearing=yaw_err, self_approach_speed=p.vx * ux + p.vy * uy,
                 self_tangential_speed=-p.vx * uy + p.vy * ux, opponent_approach_speed=-(q.vx * ux + q.vy * uy),
                 closing_speed=(p.vx - q.vx) * ux + (p.vy - q.vy) * uy, aim_pitch_error=pitch_err, aim_yaw_error=yaw_err,
                 aim_total_error=total, aim_dot=math.cos(math.radians(total)), aim_closest_miss=dist * math.sin(math.radians(total)),
                 aim_height_fraction=(EYE - dist * math.tan(math.radians(p.pitch))) / HEIGHT, line_of_sight=int(los),
                 crosshair_entity=q.s.cid if on else 1022, crosshair_distance=dist if on else 400.0, crosshair_on_opponent=int(on),
                 sight_blocker_entity=1022 if not los else 1023, sight_fraction=1.0 if los else 0.5,
                 sight_distance=dist if los else dist * 0.5, clear_toward_opponent=clearance(p.x, p.y, bearing),
                 clear_away_opponent=clearance(p.x, p.y, bearing + 180), ext_vis_parts=6 if los else 0,
                 ext_in_fov=int(abs(yaw_err) < 60))
    if s.is_bot:
        r.update(bot_family=FAMILY_NAMES.index(s.family), bot_style_seed=s.seed, bot_owner=1, bot_ctx=1,
                 bot_focus_id=g[0].s.cid if g else -1, bot_detected=int(bool(g and g[4])), bot_los=int(bool(g and g[4])),
                 bot_chord=(p.fwd + 1) * 3 + (p.side + 1), bot_chord_age_ms=TICK * p.side_age, bot_lean=p.lean,
                 bot_crouch=int(bool(p.ducked)), bot_jump=int(bool(p.jump_key)), bot_walk=int(bool(p.walk)),
                 bot_want_fire=int(p.attack), bot_kbd_ok=1, bot_think_us=120 + (t * 37 + s.cid * 11) % 200, bot_substeps=1)
    return r


def meta_section(sid: str, spec: SessionSpec) -> str:
    lines = [f"[session {sid}]", f"schema={cio.SCHEMA}", f"session_id={sid}", f"created_epoch={spec.epoch}",
             f"map={spec.map}", "game_dir=main", "target_game=0", "protocol=17", "sample_hz=20",
             "telemetry_profile=human_and_bot_brain", "clearance_probe_units=128.000", "sv_mapChecksum=123456789",
             "g_gametype=1", "sv_fps=20", "sv_runspeed=250", "sv_dmspeedmult=1", "sv_gravity=800", "g_playerdmhealth=100",
             "sv_maxbots=2", "sv_numbots=2", "sv_minPlayers=0", "dmflags=0", "com_target_game=0", "g_movelog_max_mb=0",
             "g_movelog_max_seconds=0", "g_humanbot=1", "hb_model_sha256=" + "0" * 64]
    return "\n".join(lines) + "\n\n"


FRAME_STR = {"session_id", "map", "name", "model", "weapon", "opponent_name"}
# integer core/ext columns of the logger (everything else numeric is written with %.3f)
FRAME_INT = {"schema", "session_ms", "server_ms", "frame", "frame_ms", "client_id", "is_bot", "team", "alive", "spectator",
             "ping", "cmd_server_ms", "cmd_msec", "cmd_angle_pitch", "cmd_angle_yaw", "cmd_angle_roll", "cmd_forward",
             "cmd_right", "cmd_up", "buttons", "attack_primary", "attack_secondary", "run", "use", "lean_left", "lean_right",
             "pm_flags", "move_result", "on_ground", "on_ladder", "zoomed", "weapon_state", "clip_ammo", "clip_size",
             "reserve_ammo", "opponent_id", "opponent_bot", "body_contact", "line_of_sight", "crosshair_entity",
             "crosshair_on_opponent", "sight_blocker_entity", "clear_move_entity", "clear_move_startsolid",
             "clear_reverse_entity", "clear_reverse_startsolid", "clear_toward_opponent_entity",
             "clear_toward_opponent_startsolid", "clear_away_opponent_entity", "clear_away_opponent_startsolid", "ext_ping",
             "ext_usercmds", "ext_vis_parts", "ext_in_fov"}
EVENT_STR = {"session_id", "event", "chat_message", "actor_name", "target_name", "weapon", "crosshair_hit_class",
             "sight_blocker_class"}
EVENT_INT = {"schema", "session_ms", "server_ms", "frame", "chat_mode", "actor_id", "actor_bot", "target_id", "target_bot",
             "fire_mode", "means_of_death", "hit_location", "aim_target_id", "line_of_sight", "crosshair_entity",
             "crosshair_on_target", "sight_blocker_entity"}


def csv_bytes(header: str, names, data: dict, str_cols, int_cols) -> bytes:
    """Rows formatted like the logger: ints bare, floats %.3f, strings always quoted (quotes doubled)."""
    parts = []
    for c in names:
        v = data[c]
        if c in str_cols:
            parts.append(['"' + str(x).replace('"', '""') + '"' for x in v])
        elif c in int_cols:
            parts.append(np.asarray(v, dtype=np.int64).astype(str).tolist())
        else:
            parts.append(np.char.mod("%.3f", np.asarray(v, dtype=float)).tolist())
    out = io.StringIO()
    out.write(header + "\n")
    for row in zip(*parts):
        out.write(",".join(row))
        out.write("\n")
    return out.getvalue().encode()


def frames_csv(F: dict, cols) -> bytes:
    ints = FRAME_INT | {c for c, t in zip(cols.bot, cols.bot_types) if t != "float"}
    return csv_bytes(cols.frame_header(), cols.frames, F, FRAME_STR, ints)


def events_csv(E: list, sid_of: list, cols) -> bytes:
    defaults = {"fire_mode": -1, "means_of_death": -1, "hit_location": -1, "aim_target_id": -1, "crosshair_entity": 1023,
                "sight_blocker_entity": 1023, "sight_fraction": 1.0, "sight_distance": -1.0, "crosshair_distance": -1.0}
    data = {c: [] for c in cols.events}
    for e, sid in zip(E, sid_of):
        e = dict(e, schema=cio.SCHEMA, session_id=sid)
        for c in cols.events:
            data[c].append(e.get(c, defaults.get(c, "" if c in EVENT_STR else 0)))
    return csv_bytes(cols.event_header(), cols.events, data, EVENT_STR, EVENT_INT)


def write_triplet(root: Path, prefix: str, sessions: list, styles: dict, seed: int) -> dict:
    """Simulate `sessions` (SessionSpec list) into <root>/<prefix>movement_{frames,events,meta}."""
    cols = cio.columns()
    F_all, E_all, sid_of, meta = {c: [] for c in cols.frames}, [], [], "# OpenMoHAA movement and aim telemetry sessions\n\n"
    for i, s in enumerate(sessions):
        sid, F, E, m = simulate(s, styles, seed * 10 + i)
        for c in cols.frames:
            F_all[c].extend(F[c])
        E_all.extend(E)
        sid_of.extend([sid] * len(E))
        meta += m
    root.mkdir(parents=True, exist_ok=True)
    paths = {k: root / f"{prefix}movement_{k}.{cio.KIND_EXT[k]}" for k in cio.KIND_EXT}
    paths["frames"].parent.mkdir(parents=True, exist_ok=True)
    paths["frames"].write_bytes(frames_csv(F_all, cols))
    paths["events"].write_bytes(events_csv(E_all, sid_of, cols))
    paths["meta"].write_text(meta)
    return paths


def make_telemetry(out: Path, seconds: float = 150, seed: int = 1, maps=("dm/crnodoors", "dm/vents"),
                   extra_segment: bool = False, human_only_session: bool = False) -> Path:
    """<out>/telemetry with a main triplet (one session per map); optionally a segments/ triplet and a
    session without bots (pack_capture must drop it)."""
    styles = load_styles()
    tel = Path(out) / "telemetry"
    epoch = 1790500000 + seed * 10000
    sessions = [SessionSpec(m, seconds, default_players(), epoch + 600 * i, 100 + i) for i, m in enumerate(maps)]
    if human_only_session:
        sessions.append(SessionSpec("dm/main", min(seconds, 20), [PlayerSpec(0, OWNER_ALIAS, False)], epoch + 5000, 999))
    write_triplet(tel, "", sessions, styles, seed)
    if extra_segment:
        seg = [SessionSpec("dm/downladder", seconds, default_players(), epoch + 9000, 555)]
        write_triplet(tel, f"segments/{epoch + 9000}_555_", seg, styles, seed + 7)
    return tel


def make_zip(out: Path, seconds: float = 150, seed: int = 1, date: str = "2026-10-01") -> Path:
    """A capture ZIP in the humanbot/captures naming, holding one main triplet."""
    tel = make_telemetry(Path(out) / f"src{seed}", seconds, seed)
    frames = (tel / "movement_frames.csv").read_bytes()
    z = Path(out) / f"{date}_{OWNER_ALIAS}_bot-eval_schema{cio.SCHEMA}_{cio.sha256_hex(frames)[:12]}.zip"
    with zipfile.ZipFile(z, "w", zipfile.ZIP_DEFLATED) as zf:
        for k in ("frames", "events", "meta"):
            name = f"movement_{k}.{cio.KIND_EXT[k]}"
            zf.writestr(name, (tel / name).read_bytes())
    return z


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("--seconds", type=float, default=150)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--zip", action="store_true")
    ap.add_argument("--date", default="2026-10-01")
    a = ap.parse_args(argv)
    if a.zip:
        print(make_zip(Path(a.out), a.seconds, a.seed, a.date))
    else:
        print(make_telemetry(Path(a.out), a.seconds, a.seed))


if __name__ == "__main__":
    main()
