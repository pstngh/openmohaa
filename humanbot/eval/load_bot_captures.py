#!/usr/bin/env python3
"""Load bot-eval captures into a cache that the data repo's unchanged analysis scripts accept.

Usage: load_bot_captures.py CACHE_DIR INPUT... [--date YYYY-MM-DD] [--movement-repo PATH]

INPUT is a bot-eval ZIP (DATE_pstN_bot-eval_schema13_<sha12>.zip; one or more logger
triplets inside, e.g. movement_frames.csv and segments/<epoch>_<ms>_movement_frames.csv),
a directory of such ZIPs, or a logger output directory (telemetry/ with the main triplet
and/or segments/). CACHE_DIR receives, in the format of openmohaa-movement's
analysis/load_captures.py:

  frames.parquet   every frame row, bot_* columns split off, + source, capture_date, role
                   ("bot-eval") and the sv_* / g_gametype columns from the metadata
  events.parquet   every event row
  meta.json        per-session metadata (+ source, capture_date, role)
  botcols.parquet  session_id, client_id, session_ms, the ORIGINAL is_bot and every bot_*
                   column, for every frame row (bots and humans)
  bot_roster.json  sidecar: one entry per masqueraded name (family, seed, drawn dials, ...)

Masquerade (so the analysis treats every bot as a person and the bot-vs-human duels
enter the duel mask): every row gets is_bot = 0; a bot's name becomes
"bot:<family>:<client_id>" and every human row (the owner playing the bots) becomes
"pstN@vsbot". opponent_name / opponent_bot and the events' actor/target names and
*_bot flags are rewritten the same way. A bot is identified by its style (family and
seed from its bot_style event, else from the bot_family / bot_style_seed columns), so
a bot that keeps its style across map changes keeps one name; <client_id> is the slot
where the style first appeared (".2", ".3" are appended if two styles would share a
name). Bots without a human-imitation brain are "bot:other:<client_id>", pooled-style
bots "bot:pooled:<client_id>".

read_meta / read_csv are imported from openmohaa-movement/analysis/load_captures.py
($HB_MOVEMENT_REPO, default ../openmohaa-movement); without that repository (CI) an
identical local copy of those two functions is used.
"""
from __future__ import annotations

import argparse
import configparser
import io
import json
import sys
import time
import zipfile
from dataclasses import dataclass, field
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import capture_io as cio  # noqa: E402
from hbeval import (BOT_PREFIX, FAMILY_NAMES, OWNER_NAME, ROLE, import_analysis_module, jsonable, log,  # noqa: E402
                    movement_repo, repo_arg)

KEY = ["session_id", "client_id", "session_ms"]
CORE_STR = {"session_id", "map", "name", "model", "weapon", "opponent_name"}
EVENT_STR = {"session_id", "event", "actor_name", "target_name", "weapon", "chat_message", "crosshair_hit_class",
             "sight_blocker_class"}
META_NUMERIC = ["sv_runspeed", "sv_dmspeedmult", "sv_gravity", "g_gametype", "sv_mapchecksum"]


# ------------------------------------------------------------------ readers

def _read_meta(text):
    c = configparser.ConfigParser(interpolation=None)
    c.read_string(text)
    return {s.removeprefix("session "): dict(c[s]) for s in c.sections()}


def _read_csv(raw, strings):
    import pyarrow.csv as pacsv
    opts = pacsv.ConvertOptions(column_types={c: "string" for c in strings}, strings_can_be_null=False)
    return pacsv.read_csv(io.BytesIO(raw), convert_options=opts).to_pandas()


def readers(repo: Path | None = None):
    """(read_meta, read_csv, origin): the data repo's functions, or the identical fallback."""
    repo = repo or movement_repo()
    if repo is not None:
        lc = import_analysis_module("load_captures", repo)
        return lc.read_meta, lc.read_csv, "openmohaa-movement/analysis/load_captures.py"
    return _read_meta, _read_csv, "local copy (openmohaa-movement not available)"


# ------------------------------------------------------------------ inputs

@dataclass
class Capture:
    label: str
    date: str | None
    triplets: list = field(default_factory=list)
    other: list = field(default_factory=list)
    keep: object = None      # open ZipFile


def open_inputs(inputs, date: str | None = None) -> list:
    caps, seen = [], set()

    def add_zip(z: Path):
        z = z.resolve()
        if z in seen:
            return
        seen.add(z)
        m = cio.CAPTURE_RE.match(z.stem)
        if not m:
            log(f"warning: {z.name} does not follow DATE_pstN_bot-eval_schema13_<sha12>.zip")
        zf = zipfile.ZipFile(z)
        trips, other, incomplete = cio.zip_triplets(zf)
        if incomplete:
            log(f"warning: {z.name}: incomplete triplets ignored: {incomplete}")
        caps.append(Capture(z.stem, date or (m.group("date") if m else None), trips, other, zf))

    for inp in inputs:
        p = Path(inp)
        if p.is_file() and p.suffix.lower() == ".zip":
            add_zip(p)
        elif p.is_dir() and sorted(p.glob("*.zip")):
            for z in sorted(p.glob("*.zip")):
                add_zip(z)
        elif p.is_dir():
            root = cio.telemetry_dir(p).resolve()
            if root in seen:
                continue
            seen.add(root)
            trips, other, incomplete = cio.dir_triplets(root)
            if not trips:
                raise SystemExit(f"{p}: no ZIPs and no movement_frames/events/meta triplet")
            if incomplete:
                log(f"warning: {p}: incomplete triplets ignored: {incomplete}")
            caps.append(Capture(p.resolve().name, date, trips))
        else:
            raise SystemExit(f"{p}: not a capture ZIP or directory")
    return caps


def _date_from_meta(md: dict) -> str:
    ep = [int(v["created_epoch"]) for v in md.values() if str(v.get("created_epoch", "")).isdigit()]
    return time.strftime("%Y-%m-%d", time.localtime(min(ep) if ep else time.time()))


# ------------------------------------------------------------------ masquerade

def parse_style(text: str) -> dict | None:
    try:
        d = json.loads(text)
        return d if isinstance(d, dict) else None
    except (TypeError, ValueError):
        return None


def build_roster(f: pd.DataFrame, e: pd.DataFrame, meta: dict) -> pd.DataFrame:
    """One row per (session_id, client_id) seen in frames or events: original bot flag, family, seed, masked name."""
    ids = []
    fb = f.groupby(["session_id", "client_id"], sort=False).is_bot.max().rename("bot")
    ids.append(fb.reset_index())
    for idc, bc in (("actor_id", "actor_bot"), ("target_id", "target_bot")):
        d = e[e[idc].ge(0)][["session_id", idc, bc]].rename(columns={idc: "client_id", bc: "bot"})
        ids.append(d.groupby(["session_id", "client_id"]).bot.max().reset_index())
    R = pd.concat(ids).groupby(["session_id", "client_id"], sort=False).bot.max().reset_index()
    # family / seed from the frame columns (rows of a human-imitation brain have bot_family >= 0)
    if "bot_family" in f:
        fr = f[f.is_bot.eq(1) & f.bot_family.ge(0)]
        code = fr.groupby(["session_id", "client_id"]).bot_family.agg(lambda s: s.value_counts().index[0])
        seed = fr.groupby(["session_id", "client_id"]).bot_style_seed.agg(lambda s: s.value_counts().index[0]) \
            if "bot_style_seed" in f else code * 0
        R = R.join(code.rename("code"), on=["session_id", "client_id"]).join(seed.rename("fseed"), on=["session_id", "client_id"])
    else:
        R["code"], R["fseed"] = np.nan, np.nan
    # bot_style events (the drawn dials); the last one of a (session, client) wins
    st = e[e.event.eq("bot_style")]
    styles = {}
    for sid, aid, msg in zip(st.session_id, st.actor_id, st.chat_message):
        d = parse_style(msg)
        if d is not None:
            styles[(sid, int(aid))] = d
    R["style"] = [styles.get((s, int(c))) for s, c in zip(R.session_id, R.client_id)]
    fam, seed = [], []
    for bot, code, fseed, sty in zip(R.bot, R.code, R.fseed, R["style"]):
        if bot != 1:
            fam.append("human")
            seed.append(None)
        elif sty is not None and sty.get("family"):
            fam.append(str(sty["family"]))
            seed.append(int(sty.get("seed", 0)))
        elif pd.notna(code) and 0 <= int(code) < len(FAMILY_NAMES):
            fam.append(FAMILY_NAMES[int(code)])
            seed.append(int(fseed) if pd.notna(fseed) else 0)
        else:
            fam.append("other")
            seed.append(None)
    R["family"], R["seed"] = fam, seed
    # session order: creation time, then id
    order = {s: (int(v.get("created_epoch", 0) or 0), s) for s, v in meta.items()}
    R["_o"] = [order.get(s, (0, s)) for s in R.session_id]
    R = R.sort_values(["_o", "client_id"], kind="stable").drop(columns="_o").reset_index(drop=True)
    # style identity -> masked name
    keys = []
    for s, c, fm, sd in zip(R.session_id, R.client_id, R.family, R.seed):
        keys.append(("human",) if fm == "human" else (fm, sd) if fm in FAMILY_NAMES else (fm, "slot", int(c)))
    R["key"] = keys
    # two slots of one session with the same style are different bots
    dup = R[R.family.ne("human")].duplicated(["session_id", "key"], keep=False)
    for i in dup[dup].index:
        R.at[i, "key"] = R.at[i, "key"] + ("slot", int(R.at[i, "client_id"]))
    names, used = {}, set()
    out = []
    for k, fm, c in zip(R.key, R.family, R.client_id):
        if fm == "human":
            out.append(OWNER_NAME)
            continue
        if k not in names:
            base = f"{BOT_PREFIX}{fm}:{int(c)}"
            n, j = base, 2
            while n in used:
                n, j = f"{base}.{j}", j + 1
            used.add(n)
            names[k] = n
        out.append(names[k])
    R["name"] = out
    return R


def _lookup(R: pd.DataFrame, sid, cid, fallback):
    idx = pd.MultiIndex.from_arrays([R.session_id, R.client_id])
    s = pd.Series(R.name.to_numpy(), index=idx)
    v = s.reindex(pd.MultiIndex.from_arrays([sid, cid])).to_numpy()
    return np.where(pd.isna(v), fallback, v)


def masquerade(f: pd.DataFrame, e: pd.DataFrame, R: pd.DataFrame):
    dt = f["name"].dtype
    f["name"] = pd.array(_lookup(R, f.session_id, f.client_id, f["name"].to_numpy()), dtype=dt)
    has = f.opponent_id.ge(0).to_numpy()
    f["opponent_name"] = pd.array(np.where(has, _lookup(R, f.session_id, f.opponent_id, f.opponent_name.to_numpy()),
                                           f.opponent_name.to_numpy()), dtype=f["opponent_name"].dtype)
    f["is_bot"] = np.zeros(len(f), dtype=f.is_bot.dtype)
    f["opponent_bot"] = np.zeros(len(f), dtype=f.opponent_bot.dtype)
    for idc, nc, bc in (("actor_id", "actor_name", "actor_bot"), ("target_id", "target_name", "target_bot")):
        has = e[idc].ge(0).to_numpy()
        e[nc] = pd.array(np.where(has, _lookup(R, e.session_id, e[idc], e[nc].to_numpy()), e[nc].to_numpy()),
                         dtype=e[nc].dtype)
        e[bc] = np.zeros(len(e), dtype=e[bc].dtype)


def roster_summary(R: pd.DataFrame, f: pd.DataFrame) -> list:
    rows = f.groupby("name").size()
    out = []
    for name, d in R.groupby("name", sort=False):
        sty = next((s for s in d["style"] if s is not None), None)
        sd = d.seed.iloc[0]
        out.append({"name": name, "family": d.family.iloc[0], "seed": int(sd) if sd is not None and pd.notna(sd) else None,
                    "is_bot": name != OWNER_NAME, "sessions": sorted(d.session_id.unique().tolist()),
                    "client_ids": sorted({int(c) for c in d.client_id}), "frame_rows": int(rows.get(name, 0)),
                    "drawn_dials": sty})
    return out


# ------------------------------------------------------------------ main

def load(inputs, cache: Path, date: str | None = None, repo: Path | None = None) -> dict:
    """Read the captures, masquerade and write the cache. Returns a summary (bot roster included)."""
    read_meta, read_csv, origin = readers(repo)
    cache = Path(cache)
    cache.mkdir(parents=True, exist_ok=True)
    frames, events, meta, notes = [], [], {}, []
    caps = open_inputs(inputs, date)
    for cap in caps:
        if cap.other:
            notes.append(f"{cap.label}: ignored non-telemetry members {cap.other[:5]}")
        for t in cap.triplets:
            fb = t.read("frames")
            md = read_meta(t.read("meta").decode("utf-8", errors="replace"))
            probs = cio.check_frame_header(cio.first_line(fb))
            if probs:
                log(f"warning: {cap.label} {t.label}: {probs[0]}")
            f = read_csv(fb, CORE_STR)
            e = read_csv(t.read("events"), EVENT_STR)
            source = cio.sha256_hex(fb)[:12]
            cdate = cap.date or _date_from_meta(md)
            for s, d in md.items():
                d.update(source=source, capture_date=cdate, role=ROLE)
                if s in meta:   # a session split by time over several ZIPs: keep one entry
                    continue
                meta[s] = d
            for d in (f, e):
                d["source"] = source
                d["capture_date"] = cdate
                d["role"] = ROLE
            frames.append(f)
            events.append(e)
            log(f"{cap.label} {t.label}: {len(f)} frames, {len(e)} events")
        if cap.keep is not None:
            cap.keep.close()
    if not frames:
        raise SystemExit("no telemetry triplets found")
    f = pd.concat(frames, ignore_index=True)
    e = pd.concat(events, ignore_index=True)
    dup = f.duplicated(KEY)
    if dup.any():
        notes.append(f"dropped {int(dup.sum())} duplicate frame rows (the same capture loaded twice?)")
        f = f[~dup].reset_index(drop=True)
        e = e.drop_duplicates(ignore_index=True)
    R = build_roster(f, e, meta)
    botc = [c for c in f.columns if c.startswith("bot_")]
    b = f[["session_id", "client_id", "session_ms", "is_bot"] + botc + ["source"]].copy()
    masquerade(f, e, R)
    f = f.drop(columns=botc)
    for col in META_NUMERIC:
        f[col] = pd.to_numeric(f.session_id.map({s: v.get(col) for s, v in meta.items()}), errors="coerce")
    f = f.sort_values(KEY, kind="stable").reset_index(drop=True)
    e = e.sort_values(["session_id", "session_ms"], kind="stable").reset_index(drop=True)
    b = b.sort_values(["session_id", "client_id", "session_ms"], kind="stable").reset_index(drop=True)
    f.to_parquet(cache / "frames.parquet", index=False)
    e.to_parquet(cache / "events.parquet", index=False)
    b.to_parquet(cache / "botcols.parquet", index=False)
    (cache / "meta.json").write_text(json.dumps(meta, indent=1))
    roster = roster_summary(R, f)
    summary = {"captures": [c.label for c in caps], "readers": origin, "frames": int(len(f)), "events": int(len(e)),
               "sessions": sorted(meta), "maps": sorted({v.get("map", "") for v in meta.values()}),
               "capture_dates": sorted({v["capture_date"] for v in meta.values()}), "roster": roster, "notes": notes}
    (cache / "bot_roster.json").write_text(json.dumps(jsonable(summary), indent=1))
    for n in notes:
        log("note:", n)
    log(f"cache {cache}: frames {f.shape}, events {e.shape}, {len(meta)} sessions, "
        f"{sum(r['is_bot'] for r in roster)} bots, {sum(not r['is_bot'] for r in roster)} human name(s)")
    return summary


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cache")
    ap.add_argument("inputs", nargs="+")
    ap.add_argument("--date", help="capture date for logger directories (default: from created_epoch)")
    ap.add_argument("--movement-repo", help="openmohaa-movement checkout (default $HB_MOVEMENT_REPO or ../openmohaa-movement)")
    a = ap.parse_args(argv)
    load(a.inputs, Path(a.cache), a.date, repo_arg(a.movement_repo))


if __name__ == "__main__":
    main()
