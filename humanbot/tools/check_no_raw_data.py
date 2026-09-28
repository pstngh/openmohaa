#!/usr/bin/env python3
"""Privacy guard of the public fork: no raw human data, no per-person tables, no human aliases.

Usage: check_no_raw_data.py [--movement-repo PATH] [--root DIR] [--upstream-base REV] [--all-files]

Checks every git-tracked file and every untracked file that is not git-ignored:
 1. raw data: no .csv .tsv .parquet .pkl .pickle .feather .arrow .npz .npy .h5 .hdf5 file and
    no other archive (.tar .tgz .gz .bz2 .xz .zst .7z .rar); raw CSV exists only inside
    capture ZIPs.
 2. capture ZIPs: only directly in humanbot/captures/, each <= 25 MiB, named
    DATE_<players>_bot-eval_schema13_<sha12>.zip where sha12 starts the SHA-256 of its first
    frames CSV, listed with its checksum in humanbot/captures/SHA256SUMS, holding logger
    triplets only, with the schema-13 frames and events headers, metadata at schema 13, and
    bot rows in every session.
 3. JSON under humanbot/: no per-person keys (person, alias, by_person, by_alias, by_player,
    by_name, by_raw_alias..., style_distance, nearest_style) and no name lists (names,
    aliases, players, people, persons, humans holding a list); no numeric array longer than
    100,000 values.
 4. aliases: no file and no capture ZIP member contains a human alias. The aliases are read
    at run time from the private data repo (manifest.json players, analysis/common.py PERSON
    keys and values) minus the owner's own aliases (pstN, which the owner uses on purpose
    in capture names, and the aliases PERSON maps to pstN) and the engine's default player
    name. Matching is case-insensitive on alphanumeric boundaries in text files (plus the
    alias stripped of leading/trailing punctuation). Files that the fork leaves identical to
    upstream OpenMoHAA (the parent of the first commit adding humanbot/, or --upstream-base /
    $HB_UPSTREAM_BASE) are not scanned, because upstream code uses a few aliases as ordinary
    words; --all-files scans everything. Aliases are never printed in full. Skipped with a
    warning when the data repo is absent (CI).
 5. code/humanbot/hb_embedded.cpp is in sync (humanbot/tools/embed_model.py --check).
Exits 1 on any violation, printing each.
"""
from __future__ import annotations

import argparse
import ast
import json
import os
import re
import subprocess
import sys
import zipfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "eval"))
import capture_io as cio  # noqa: E402

RAW_EXT = {".csv", ".tsv", ".parquet", ".pkl", ".pickle", ".feather", ".arrow", ".npz", ".npy", ".h5", ".hdf5"}
ARCHIVE_EXT = {".tar", ".tgz", ".gz", ".bz2", ".xz", ".zst", ".7z", ".rar"}
CAPTURES = "humanbot/captures"
FORBIDDEN_KEYS = {"person", "persons", "alias", "aliases", "style_distance", "nearest_style"}
FORBIDDEN_KEY_RE = re.compile(r"(^|_)by_(person|persons|alias|aliases|raw_alias|player|players|name|names)(_|$)", re.I)
NAME_LIST_KEYS = {"names", "aliases", "players", "people", "persons", "humans"}
MAX_NUMERIC = 100_000
OWNER = "pstN"
GENERIC_NAMES = {"unnamedsoldier"}      # the engine's default player name identifies nobody


class Report:
    def __init__(self):
        self.violations, self.warnings = [], []

    def bad(self, rule, where, msg):
        self.violations.append(f"[{rule}] {where}: {msg}")

    def warn(self, msg):
        self.warnings.append(msg)


# ------------------------------------------------------------------ file set

def git(root: Path, *args) -> str | None:
    try:
        p = subprocess.run(["git", "-C", str(root), *args], capture_output=True, timeout=120)
    except (OSError, subprocess.SubprocessError):
        return None
    return p.stdout.decode("utf-8", "surrogateescape") if p.returncode == 0 else None


def listed_files(root: Path, rep: Report) -> list:
    out = git(root, "ls-files", "-z", "--cached", "--others", "--exclude-standard")
    if out is None:
        rep.warn("not a git checkout: scanning every file below the root (ignore rules unknown)")
        return sorted(p.relative_to(root).as_posix() for p in root.rglob("*") if p.is_file() and ".git" not in p.parts)
    return sorted({f for f in out.split("\0") if f and (root / f).is_file()})


def upstream_base(root: Path, explicit: str | None) -> str | None:
    for rev in (explicit, os.environ.get("HB_UPSTREAM_BASE")):
        if rev:
            return (git(root, "rev-parse", "--verify", "-q", rev + "^{commit}") or "").strip() or None
    for ref in ("upstream/main", "upstream/master"):
        if git(root, "rev-parse", "--verify", "-q", ref):
            mb = git(root, "merge-base", "HEAD", ref)
            if mb:
                return mb.strip()
    first = (git(root, "log", "--format=%H", "--reverse", "--", "humanbot", "code/humanbot") or "").split()
    if first:
        par = git(root, "rev-parse", "--verify", "-q", first[0] + "^")
        return par.strip() if par else None
    return None


def fork_changed(root: Path, base: str | None, files: list) -> set | None:
    """Files that differ from upstream (changed since `base` or untracked); None = unknown (scan all)."""
    if base is None:
        return None
    diff = git(root, "diff", "--name-only", "-z", base, "--")
    others = git(root, "ls-files", "-z", "--others", "--exclude-standard")
    if diff is None or others is None:
        return None
    return {f for f in diff.split("\0") + others.split("\0") if f}


# ------------------------------------------------------------------ aliases

def load_aliases(repo: Path) -> tuple:
    """(aliases to search, owner aliases excluded)."""
    names = set()
    man = json.loads((repo / "manifest.json").read_text())
    for c in man.get("captures", []):
        names.update(str(p) for p in c.get("players", []))
    person = {}
    tree = ast.parse((repo / "analysis" / "common.py").read_text())
    for node in tree.body:
        if isinstance(node, ast.Assign) and any(getattr(t, "id", None) == "PERSON" for t in node.targets):
            person = ast.literal_eval(node.value)
    names |= set(person) | set(person.values())
    owner = {OWNER} | {k for k, v in person.items() if v == OWNER}
    keep = set()
    for n in names:
        if n in owner or n.lower() in GENERIC_NAMES or not n.strip():
            continue
        keep.add(n)
        core = re.sub(r"^[^0-9A-Za-z]+|[^0-9A-Za-z]+$", "", n)
        if core != n and len(core) >= 3 and core not in owner:
            keep.add(core)
    return sorted(keep, key=lambda s: (-len(s), s)), sorted(owner)


class AliasScanner:
    def __init__(self, aliases):
        self.aliases = aliases
        self.lower = [a.lower().encode("utf-8") for a in aliases]
        self.maxlen = max((len(a) for a in self.lower), default=1)

    @staticmethod
    def _alnum(c: int) -> bool:
        return 48 <= c <= 57 or 65 <= c <= 90 or 97 <= c <= 122

    def scan(self, data: bytes, chunk: int = 1 << 24):
        """Yield (alias index, byte offset) of every bounded occurrence."""
        n = len(data)
        start = 0
        while start < n:
            end = min(n, start + chunk)
            lo = max(0, start - self.maxlen)
            hi = min(n, end + self.maxlen)
            seg = data[lo:hi].lower()
            for i, a in enumerate(self.lower):
                j = seg.find(a)
                while j >= 0:
                    off = lo + j
                    if start <= off < end:
                        before = data[off - 1] if off > 0 else 32
                        after = data[off + len(a)] if off + len(a) < n else 32
                        if not self._alnum(before) and not self._alnum(after):
                            yield i, off
                    j = seg.find(a, j + 1)
            start = end

    def mask(self, i):
        a = self.aliases[i]
        return f"alias #{i + 1} ({a[0]}{'*' * (len(a) - 1)}, {len(a)} chars)"


def line_of(data: bytes, off: int) -> int:
    return data.count(b"\n", 0, off) + 1


def scan_text(rep: Report, sc: AliasScanner, where: str, data: bytes, limit: int = 5):
    hits = 0
    for i, off in sc.scan(data):
        hits += 1
        if hits <= limit:
            rep.bad("alias", f"{where}:{line_of(data, off)}", f"contains {sc.mask(i)}")
    if hits > limit:
        rep.bad("alias", where, f"{hits - limit} more alias occurrences")


# ------------------------------------------------------------------ JSON

def check_json(rep: Report, rel: str, data: bytes):
    try:
        doc = json.loads(data.decode("utf-8"))
    except (UnicodeDecodeError, ValueError) as ex:
        rep.bad("json", rel, f"does not parse ({ex.__class__.__name__}); cannot verify it")
        return

    def numeric_count(x):
        if isinstance(x, bool):
            return None
        if isinstance(x, (int, float)):
            return 1
        if isinstance(x, list):
            tot = 0
            for v in x:
                c = numeric_count(v)
                if c is None:
                    return None
                tot += c
            return tot
        return None

    def walk(x, path):
        if isinstance(x, dict):
            for k, v in x.items():
                ks = str(k)
                p = f"{path}.{ks}" if path else ks
                if ks.lower() in FORBIDDEN_KEYS or FORBIDDEN_KEY_RE.search(ks):
                    rep.bad("json", rel, f"per-person key '{p}'")
                if ks.lower() in NAME_LIST_KEYS and isinstance(v, list) and any(isinstance(e, str) for e in v):
                    rep.bad("json", rel, f"name list '{p}'")
                walk(v, p)
        elif isinstance(x, list):
            c = numeric_count(x)
            if c is not None and c > MAX_NUMERIC:
                rep.bad("json", rel, f"numeric array '{path}' with {c} values (> {MAX_NUMERIC})")
                return
            for i, v in enumerate(x):
                if isinstance(v, (dict, list)):
                    walk(v, f"{path}[{i}]")
    walk(doc, "")


# ------------------------------------------------------------------ capture ZIPs

def read_sums(root: Path) -> dict:
    p = root / CAPTURES / "SHA256SUMS"
    out = {}
    if p.exists():
        for ln in p.read_text().splitlines():
            parts = ln.split(None, 1)
            if len(parts) == 2:
                out[Path(parts[1].lstrip("*").strip()).name] = parts[0].lower()
    return out


def check_capture_zip(rep: Report, root: Path, rel: str, sums: dict, sc: AliasScanner | None):
    path = root / rel
    size = path.stat().st_size
    if size > cio.MAX_ZIP_BYTES:
        rep.bad("zip", rel, f"{size} bytes > {cio.MAX_ZIP_BYTES} (25 MiB)")
    m = cio.CAPTURE_RE.match(path.stem) if path.suffix == ".zip" else None
    if not m or int(m.group("schema")) != cio.SCHEMA:
        rep.bad("zip", rel, f"name is not DATE_<players>_bot-eval_schema{cio.SCHEMA}_<sha12>.zip")
    data = path.read_bytes()
    want = sums.get(path.name)
    if want is None:
        rep.bad("zip", rel, f"not listed in {CAPTURES}/SHA256SUMS")
    elif want != cio.sha256_hex(data):
        rep.bad("zip", rel, f"SHA-256 differs from {CAPTURES}/SHA256SUMS")
    try:
        zf = zipfile.ZipFile(path)
    except zipfile.BadZipFile:
        rep.bad("zip", rel, "not a readable ZIP")
        return
    with zf:
        cache = {}

        def member(name):          # decompress each member once
            if name not in cache:
                cache[name] = zf.read(name)
            return cache[name]
        trips, other, incomplete = cio.group_triplets([i.filename for i in zf.infolist() if not i.is_dir()], member)
        for o in other:
            rep.bad("zip", rel, f"unexpected member {o} (only logger triplets belong in a capture)")
        for p in incomplete:
            rep.bad("zip", rel, f"incomplete triplet {p}")
        if not trips:
            rep.bad("zip", rel, "no movement_frames/events/meta triplet")
            return
        frames_members = [n for n in zf.namelist() if cio.MEMBER_RE.match(n) and "movement_frames" in n]
        if m and frames_members and cio.sha256_hex(member(frames_members[0]))[:12] != m.group("sha"):
            rep.bad("zip", rel, "sha12 in the name is not the SHA-256 of its first frames CSV")
        for t in trips:
            where = f"{rel}!{t.label}"
            fb = t.read("frames")
            for pr in cio.check_frame_header(cio.first_line(fb), root):
                rep.bad("zip", where, pr)
            eb = t.read("events")
            for pr in cio.check_event_header(cio.first_line(eb), root):
                rep.bad("zip", where, pr)
            try:
                md = cio.read_meta_text(t.read("meta").decode("utf-8", "replace"))
                badm = [s for s, v in md.items() if v.get("schema") != str(cio.SCHEMA)]
                if badm:
                    rep.bad("zip", where, f"metadata sessions not at schema {cio.SCHEMA}: {', '.join(badm[:3])}")
            except Exception as ex:  # noqa: BLE001
                rep.bad("zip", where, f"metadata does not parse ({ex.__class__.__name__})")
            try:
                tab = cio.csv_columns(fb, ["schema", "session_id", "is_bot"], strings=["session_id"])
                sid = tab.column("session_id").to_pylist()
                bot = tab.column("is_bot").to_pylist()
                sch = set(tab.column("schema").to_pylist())
                if sch - {cio.SCHEMA}:
                    rep.bad("zip", where, f"frame rows with schema {sorted(sch - {cio.SCHEMA})[:3]}")
                has = {}
                for s, b in zip(sid, bot):
                    has[s] = has.get(s, False) or b == 1
                nobot = [s for s, v in has.items() if not v]
                if nobot:
                    rep.bad("zip", where, f"{len(nobot)} session(s) without bot rows: {', '.join(nobot[:3])}")
            except Exception as ex:  # noqa: BLE001
                rep.bad("zip", where, f"frames CSV does not parse ({ex.__class__.__name__}: {str(ex)[:80]})")
            if sc is not None:
                for kind in ("frames", "events", "meta"):
                    scan_text(rep, sc, f"{rel}!{t.names[kind]}", t.read(kind))
            for kind in ("frames", "events", "meta"):
                cache.pop(t.names[kind], None)


# ------------------------------------------------------------------ main

def check(root: Path, repo: Path | None, base_rev: str | None = None, all_files: bool = False) -> Report:
    rep = Report()
    files = listed_files(root, rep)
    sums = read_sums(root)
    sc = None
    if repo is None:
        rep.warn("openmohaa-movement not found: the human-alias check is skipped (set HB_MOVEMENT_REPO)")
    else:
        aliases, owner = load_aliases(repo)
        sc = AliasScanner(aliases)
    scope = None
    if sc is not None and not all_files:
        base = upstream_base(root, base_rev)
        scope = fork_changed(root, base, files)
        if scope is None:
            rep.warn("upstream base unknown: scanning every file for aliases")
    for rel in files:
        path = root / rel
        low = rel.lower()
        ext = path.suffix.lower()
        if ext in RAW_EXT:
            rep.bad("raw", rel, f"{ext} data file (raw captures belong in the private data repo; bot captures only as "
                                f"ZIPs in {CAPTURES}/)")
            continue
        if ext in ARCHIVE_EXT:
            rep.bad("raw", rel, f"{ext} archive (only bot-eval capture ZIPs in {CAPTURES}/ are allowed)")
            continue
        if ext == ".zip":
            if Path(rel).parent.as_posix() != CAPTURES:
                rep.bad("zip", rel, f"ZIP outside {CAPTURES}/")
            else:
                check_capture_zip(rep, root, rel, sums, sc)
            continue
        data = None
        if ext == ".json" and low.startswith("humanbot/"):
            data = path.read_bytes()
            check_json(rep, rel, data)
        if sc is not None and (scope is None or rel in scope):
            if data is None:
                if path.stat().st_size > 64 * 1024 * 1024:
                    rep.warn(f"{rel}: over 64 MB, alias scan skipped")
                    continue
                data = path.read_bytes()
            if b"\0" in data[:8192]:
                continue          # binary file
            scan_text(rep, sc, rel, data)
    for z in sorted(set(sums) - {Path(f).name for f in files if f.startswith(CAPTURES + "/")}):
        rep.warn(f"{CAPTURES}/SHA256SUMS lists {z}, which is not in the checkout")
    emb = root / "humanbot" / "tools" / "embed_model.py"
    if not emb.exists():
        rep.bad("embed", "humanbot/tools/embed_model.py", "missing; cannot check code/humanbot/hb_embedded.cpp")
    else:
        p = subprocess.run([sys.executable, str(emb), "--check"], cwd=str(root), capture_output=True, text=True)
        if p.returncode != 0:
            rep.bad("embed", "code/humanbot/hb_embedded.cpp", (p.stderr or p.stdout).strip().splitlines()[-1]
                    if (p.stderr or p.stdout).strip() else f"embed_model.py --check exited {p.returncode}")
    rep.files = len(files)
    rep.alias_scope = "off" if sc is None else ("all files" if scope is None else f"{len(scope)} fork-changed files")
    return rep


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--movement-repo", help="openmohaa-movement checkout (default $HB_MOVEMENT_REPO or ../openmohaa-movement)")
    ap.add_argument("--root", default=str(HERE.parents[1]), help="fork checkout to check (default: this one)")
    ap.add_argument("--upstream-base", help="upstream revision whose unchanged files skip the alias scan")
    ap.add_argument("--all-files", action="store_true", help="scan every file for aliases, upstream ones too")
    a = ap.parse_args(argv)
    root = Path(a.root).resolve()
    if a.movement_repo:
        repo = Path(a.movement_repo)
        repo = repo.resolve() if (repo / "manifest.json").exists() and (repo / "analysis" / "common.py").exists() else None
    else:
        env = os.environ.get("HB_MOVEMENT_REPO")
        cand = Path(env) if env else root.parent / "openmohaa-movement"
        repo = cand.resolve() if (cand / "manifest.json").exists() and (cand / "analysis" / "common.py").exists() else None
    rep = check(root, repo, a.upstream_base, a.all_files)
    for w in rep.warnings:
        print("WARNING:", w)
    for v in rep.violations:
        print("VIOLATION", v)
    print(f"check_no_raw_data: {rep.files} files, alias scan: {rep.alias_scope}; "
          f"{len(rep.violations)} violation(s), {len(rep.warnings)} warning(s)")
    return 1 if rep.violations else 0


if __name__ == "__main__":
    sys.exit(main())
