#!/usr/bin/env python3
"""Pack movement-telemetry logger output into bot-eval capture ZIPs.

Usage: pack_capture.py TELEMETRY_DIR [--out humanbot/captures] [--date YYYY-MM-DD]
                       [--skip-refused] [--force] [--max-bytes N]

TELEMETRY_DIR is the logger's telemetry/ directory (or the game dir holding it): the main
triplet movement_frames.csv, movement_events.csv, movement_meta.txt and/or the segment
triplets segments/<epoch>_<ms>_movement_{frames,events}.csv + ..._meta.txt.

Every triplet is checked first. A triplet is refused when its frames header is not the
schema-13 header of this build (code/fgame/movement_telemetry_schema.h + hb_diag.h), its
events header or metadata schema is not 13, or it has no bot rows; nothing is written
while a triplet is refused unless --skip-refused is given. Sessions without bot rows are
dropped (a bot-eval capture must hold bot rows in every session).

The sessions are packed in capture order into ZIPs of at most 25 MiB, each named
DATE_pstN_bot-eval_schema13_<sha12>.zip, where sha12 is the first 12 hex digits of the
SHA-256 of the first frames CSV in that ZIP. A ZIP holds one or more triplets under their
original relative names, restricted to its sessions (headers kept). A session never spans
two ZIPs unless it alone exceeds the limit; its frames and events are then split by time,
each part with the header repeated and the session's metadata section. The ZIPs are
deterministic (fixed timestamps), so packing the same output again reproduces them.
<out>/SHA256SUMS gets one "<sha256>  <zip name>" line per ZIP (check with
`cd humanbot/captures && sha256sum -c SHA256SUMS`). DATE defaults to the local date of the
earliest packed session (created_epoch).
"""
from __future__ import annotations

import argparse
import math
import re
import sys
import time
import zipfile
import zlib
from dataclasses import dataclass, field
from pathlib import Path

import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "eval"))
import capture_io as cio  # noqa: E402
from hbeval import CAPTURES_DIR, OWNER_ALIAS  # noqa: E402

ZIP_MARGIN = 64 * 1024          # local headers, central directory and estimation slack


@dataclass
class Unit:
    """One session (or a time slice of one) of one triplet."""
    prefix: str
    session: str
    frames: list                 # bytes lines without terminator
    events: list
    meta: str
    epoch: int
    part: int = 0
    est: int = 0
    f_ms: object = None          # session_ms of each frames line (numpy)
    e_ms: object = None          # session_ms of each events line


@dataclass
class TripletPlan:
    prefix: str
    frame_header: bytes
    event_header: bytes
    meta_pre: str
    units: list = field(default_factory=list)
    dropped: list = field(default_factory=list)
    problems: list = field(default_factory=list)


def deflated_size(lines) -> int:
    c = zlib.compressobj(6, zlib.DEFLATED, -15)
    n = 0
    for ln in lines:
        n += len(c.compress(ln))
        n += len(c.compress(b"\n"))
    return n + len(c.flush())


def plan_triplet(t: cio.Triplet) -> TripletPlan:
    fb, eb = t.read("frames"), t.read("events")
    meta_text = t.read("meta").decode("utf-8", errors="replace")
    flines, elines = cio.split_lines(fb), cio.split_lines(eb)
    P = TripletPlan(t.prefix, flines[0] if flines else b"", elines[0] if elines else b"", "")
    cols = cio.columns()
    if cols.schema != cio.SCHEMA:
        P.problems.append(f"this build writes schema {cols.schema}; pack_capture.py packs schema {cio.SCHEMA} only")
        return P
    P.problems += cio.check_frame_header(P.frame_header.decode("utf-8", "replace"))
    P.problems += cio.check_event_header(P.event_header.decode("utf-8", "replace"))
    try:
        md = cio.read_meta_text(meta_text)
    except Exception as ex:  # noqa: BLE001 - any parse failure refuses the triplet
        P.problems.append(f"metadata does not parse ({ex})")
        return P
    bad = sorted(s for s, v in md.items() if v.get("schema") != str(cio.SCHEMA))
    if bad:
        P.problems.append(f"metadata sessions not at schema {cio.SCHEMA}: {', '.join(bad[:3])}")
    if P.problems:
        return P
    ft = cio.csv_columns(fb, ["schema", "session_id", "session_ms", "is_bot"], strings=["session_id"])
    et = cio.csv_columns(eb, ["session_id", "session_ms"], strings=["session_id"])
    if ft.num_rows != len(flines) - 1 or et.num_rows != len(elines) - 1:
        P.problems.append("CSV records span several lines; not a logger file")
        return P
    schema = ft.column("schema").to_numpy()
    if len(schema) and (schema != cio.SCHEMA).any():
        P.problems.append(f"frame rows with schema {sorted(set(schema.tolist()) - {cio.SCHEMA})[:3]}")
        return P
    fsid = np.asarray(ft.column("session_id").to_pylist(), dtype=object)
    fms = ft.column("session_ms").to_numpy()
    bot = ft.column("is_bot").to_numpy() == 1
    esid = np.asarray(et.column("session_id").to_pylist(), dtype=object)
    ems = et.column("session_ms").to_numpy()
    if not bot.any():
        P.problems.append("no bot rows")
        return P
    pre, secs = cio.meta_sections(meta_text)
    P.meta_pre = pre
    order = list(dict.fromkeys(fsid.tolist()))
    for s in order:
        fi = np.flatnonzero(fsid == s)
        if not bot[fi].any():
            P.dropped.append(s)
            continue
        ei = np.flatnonzero(esid == s)
        if s not in secs:
            print(f"warning: {t.label}: session {s} has no metadata section (its rows will not be eligible)")
        u = Unit(t.prefix, s, [flines[i + 1] for i in fi], [elines[i + 1] for i in ei], secs.get(s, ""),
                 int(md.get(s, {}).get("created_epoch", 0) or 0))
        u.f_ms, u.e_ms = fms[fi], ems[ei]
        P.units.append(u)
    return P


def split_by_time(u: Unit, budget: int) -> list:
    """Split a session's frames and events into time slices whose compressed size fits `budget`."""
    k = max(2, math.ceil(u.est / (0.85 * budget)))
    while True:
        ticks = np.unique(u.f_ms)
        cuts = [ticks[int(len(ticks) * j / k)] for j in range(1, k)]
        edges = [-np.inf] + cuts + [np.inf]
        parts = []
        for j in range(k):
            fm = (u.f_ms >= edges[j]) & (u.f_ms < edges[j + 1])
            em = (u.e_ms >= edges[j]) & (u.e_ms < edges[j + 1])
            p = Unit(u.prefix, u.session, [ln for ln, m in zip(u.frames, fm) if m], [ln for ln, m in zip(u.events, em) if m],
                     u.meta, u.epoch, part=j + 1)
            p.f_ms, p.e_ms = u.f_ms[fm], u.e_ms[em]
            p.est = deflated_size(p.frames) + deflated_size(p.events) + len(p.meta)
            parts.append(p)
        if max(p.est for p in parts) <= budget or k >= len(ticks):
            return parts
        k *= 2


def zip_bytes(group: list, plans: dict, date: str) -> tuple:
    """(zip file bytes, sha12 of the first frames member) for a list of units."""
    import io
    y, m, d = (int(x) for x in date.split("-"))
    buf = io.BytesIO()
    first_frames = None
    with zipfile.ZipFile(buf, "w", zipfile.ZIP_DEFLATED) as zf:
        prefixes = list(dict.fromkeys(u.prefix for u in group))
        for pf in prefixes:
            P = plans[pf]
            us = [u for u in group if u.prefix == pf]
            frames = P.frame_header + b"\n" + b"".join(ln + b"\n" for u in us for ln in u.frames)
            events = P.event_header + b"\n" + b"".join(ln + b"\n" for u in us for ln in u.events)
            meta = P.meta_pre + "".join(dict.fromkeys(u.meta for u in us))
            if first_frames is None:
                first_frames = frames
            for kind, data in (("frames", frames), ("events", events), ("meta", meta.encode("utf-8"))):
                zi = zipfile.ZipInfo(f"{pf}movement_{kind}.{cio.KIND_EXT[kind]}", date_time=(y, m, d, 0, 0, 0))
                zi.compress_type = zipfile.ZIP_DEFLATED
                zi.external_attr = 0o644 << 16
                zi.create_system = 3
                zf.writestr(zi, data)
    return buf.getvalue(), cio.sha256_hex(first_frames)[:12]


def pack_groups(units: list, budget: int) -> list:
    groups, cur, size = [], [], 0
    for u in units:
        if cur and size + u.est > budget:
            groups.append(cur)
            cur, size = [], 0
        cur.append(u)
        size += u.est
    if cur:
        groups.append(cur)
    return groups


def update_sums(out: Path, entries: dict):
    p = out / "SHA256SUMS"
    lines = {}
    if p.exists():
        for ln in p.read_text().splitlines():
            parts = ln.split(None, 1)
            if len(parts) == 2:
                lines[parts[1].lstrip("*").strip()] = parts[0]
    lines.update(entries)
    p.write_text("".join(f"{h}  {n}\n" for n, h in sorted(lines.items())))


def pack(src: Path, out: Path = CAPTURES_DIR, date: str | None = None, skip_refused: bool = False, force: bool = False,
         max_bytes: int = cio.MAX_ZIP_BYTES) -> int:
    trips, other, incomplete = cio.dir_triplets(src)
    if not trips and not incomplete:
        print(f"{src}: no movement_frames/events/meta triplet found", file=sys.stderr)
        return 1
    refused = [(p, "incomplete triplet (frames, events and meta are all required)") for p in incomplete]
    plans = {}
    for t in trips:
        P = plan_triplet(t)
        if P.problems:
            refused += [(t.label, pr) for pr in P.problems]
            continue
        plans[t.prefix] = P
        for s in P.dropped:
            print(f"{t.label}: dropping session {s}: no bot rows")
    for label, why in refused:
        print(f"REFUSED {label}: {why}", file=sys.stderr)
    if refused and not skip_refused:
        print("nothing written (use --skip-refused to pack the other triplets)", file=sys.stderr)
        return 1
    units = [u for P in plans.values() for u in P.units]
    if not units:
        print("nothing to pack", file=sys.stderr)
        return 1
    if date is None:
        ep = [u.epoch for u in units if u.epoch > 0]
        date = time.strftime("%Y-%m-%d", time.localtime(min(ep) if ep else time.time()))
    if not re.fullmatch(r"\d{4}-\d{2}-\d{2}", date):
        print(f"--date must be YYYY-MM-DD, not {date!r}", file=sys.stderr)
        return 1
    budget = max_bytes - ZIP_MARGIN
    written = {}
    for _attempt in range(6):
        sized = []
        for u in units:
            u.est = u.est or deflated_size(u.frames) + deflated_size(u.events) + len(u.meta)
            sized += split_by_time(u, budget) if u.est > budget else [u]
        blobs = [zip_bytes(g, plans, date) + (g,) for g in pack_groups(sized, budget)]
        if all(len(b) <= max_bytes for b, _, _ in blobs):
            break
        budget = int(budget * 0.85)      # the size estimate was too optimistic: pack tighter
    else:
        print("could not fit the sessions under the size limit", file=sys.stderr)
        return 1
    named = [(f"{date}_{OWNER_ALIAS}_bot-eval_schema{cio.SCHEMA}_{sha12}.zip", data, g) for data, sha12, g in blobs]
    clash = [n for n, data, _ in named if (out / n).exists() and (out / n).read_bytes() != data]
    if clash and not force:
        for n in clash:
            print(f"{out / n} exists with other content (use --force to replace it)", file=sys.stderr)
        print("nothing written", file=sys.stderr)
        return 1
    out.mkdir(parents=True, exist_ok=True)
    for name, data, g in named:
        dst = out / name
        tmp = dst.with_suffix(".zip.tmp")
        tmp.write_bytes(data)
        tmp.replace(dst)
        written[name] = cio.sha256_hex(data)
        sess = list(dict.fromkeys(f"{u.session}" + (f" (part {u.part})" if u.part else "") for u in g))
        print(f"{name}: {len(data) / 1e6:.2f} MB, {len(sess)} session(s): {', '.join(sess)}")
    update_sums(out, written)
    print(f"{len(written)} ZIP(s) in {out}; SHA256SUMS updated")
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("telemetry")
    ap.add_argument("--out", default=str(CAPTURES_DIR))
    ap.add_argument("--date", help="capture date YYYY-MM-DD (default: local date of the earliest session)")
    ap.add_argument("--skip-refused", action="store_true", help="pack the valid triplets even if others are refused")
    ap.add_argument("--force", action="store_true", help="replace an existing ZIP of the same name with other content")
    ap.add_argument("--max-bytes", type=int, default=cio.MAX_ZIP_BYTES, help=argparse.SUPPRESS)
    a = ap.parse_args(argv)
    return pack(Path(a.telemetry), Path(a.out), a.date, a.skip_refused, a.force, a.max_bytes)


if __name__ == "__main__":
    sys.exit(main())
