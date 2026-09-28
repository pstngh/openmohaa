"""Telemetry capture layout shared by the loader, pack_capture.py and check_no_raw_data.py.

Logger output (code/fgame/movement_telemetry.cpp), relative to the game dir:
  telemetry/movement_frames.csv, movement_events.csv, movement_meta.txt      main triplet
  telemetry/segments/<epoch>_<ms>_movement_{frames,events}.csv, ..._meta.txt  bounded captures and
                                                                             rotated 1.5 GB segments
A bot-eval ZIP (humanbot/captures/DATE_pstN_bot-eval_schema13_<sha12>.zip) holds one or more
triplets with the same relative names ("movement_frames.csv", "segments/<epoch>_<ms>_movement_frames.csv").

The expected CSV headers are parsed from the C++ column contract
(code/fgame/movement_telemetry_schema.h and code/humanbot/hb_diag.h), so they follow the build.
Every CSV record is one physical line: the logger replaces CR/LF inside quoted fields by spaces.
"""
from __future__ import annotations

import configparser
import hashlib
import io
import re
import zipfile
from dataclasses import dataclass, field
from functools import lru_cache
from pathlib import Path

from hbeval import FORK_ROOT

SCHEMA = 13
MAX_ZIP_BYTES = 25 * 1024 * 1024
CAPTURE_RE = re.compile(r"^(?P<date>\d{4}-\d{2}-\d{2})_(?P<players>[^_]+)_bot-eval_schema(?P<schema>\d+)_(?P<sha>[0-9a-f]{12})$")
MEMBER_RE = re.compile(r"^(?P<prefix>(?:[^/]*/)*(?:\d+_\d+_)?)movement_(?P<kind>frames|events|meta)\.(?P<ext>csv|txt)$")
KIND_EXT = {"frames": "csv", "events": "csv", "meta": "txt"}
SCHEMA_H = Path("code/fgame/movement_telemetry_schema.h")
DIAG_H = Path("code/humanbot/hb_diag.h")
GOLDEN_CORE = Path("code/tests/humanbot/golden/movement_frames_core_header.txt")
GOLDEN_EVENTS = Path("code/tests/humanbot/golden/movement_events_header.txt")


# ---------------------------------------------------------------- column contract

@dataclass(frozen=True)
class Columns:
    schema: int
    core: tuple
    ext: tuple
    bot: tuple
    bot_defaults: tuple
    bot_types: tuple
    events: tuple

    @property
    def frames(self):
        return self.core + self.ext + self.bot

    def frame_header(self) -> str:
        return ",".join(self.frames)

    def event_header(self) -> str:
        return ",".join(self.events)


def _strip_comments(src: str) -> str:
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    return re.sub(r"//[^\n]*", " ", src)


def _string_array(src: str, name: str) -> tuple:
    m = re.search(re.escape(name) + r"\s*\[\s*\]\s*=\s*\{(.*?)\};", src, flags=re.S)
    if not m:
        raise ValueError(f"{name} not found in {SCHEMA_H}")
    return tuple(re.findall(r'"([^"\\]*)"', m.group(1)))


@lru_cache(maxsize=4)
def columns(root: Path = FORK_ROOT) -> Columns:
    """Column lists of the current build, parsed from the C++ headers."""
    root = Path(root)
    sch = _strip_comments((root / SCHEMA_H).read_text())
    m = re.search(r"MOVELOG_SCHEMA\s*=\s*(\d+)\s*;", sch)
    schema = int(m.group(1)) if m else -1
    diag = (root / DIAG_H).read_text()
    body = diag[diag.index("#define HB_DIAG_FIELDS"):]
    body = body[:body.index("namespace")]
    fields = re.findall(r'X\(\s*(\w+)\s*,\s*(\w+)\s*,\s*([^,]+?)\s*,\s*"([^"]+)"\s*\)', body)
    bot = tuple(f[3] for f in fields)
    defaults = tuple(_c_default(f[0], f[2]) for f in fields)
    return Columns(schema, _string_array(sch, "MOVELOG_FRAME_CORE_COLUMNS"), _string_array(sch, "MOVELOG_FRAME_EXT_COLUMNS"),
                   bot, defaults, tuple(f[0] for f in fields), _string_array(sch, "MOVELOG_EVENT_COLUMNS"))


def _c_default(ctype: str, literal: str):
    lit = literal.strip().rstrip("fF")
    return float(lit) if ctype == "float" else int(float(lit))


def check_frame_header(header: str, root: Path = FORK_ROOT) -> list:
    """Problems that make `header` (first line of movement_frames.csv) not a schema-13 frame header."""
    cols = header.strip("\r\n").split(",")
    c = columns(root)
    if c.schema == SCHEMA:
        exp = list(c.frames)
        if cols == exp:
            return []
        missing = [x for x in exp if x not in cols]
        extra = [x for x in cols if x not in exp]
        msg = f"frames header differs from the schema-{SCHEMA} header of this build ({len(cols)} vs {len(exp)} columns"
        if missing:
            msg += "; missing " + ", ".join(missing[:5]) + ("..." if len(missing) > 5 else "")
        if extra:
            msg += "; unexpected " + ", ".join(extra[:5]) + ("..." if len(extra) > 5 else "")
        if not missing and not extra:
            msg += "; same columns in another order"
        return [msg + ")"]
    # the build moved to another schema: check the invariant structure only
    golden = (Path(root) / GOLDEN_CORE).read_text().strip().split(",")
    probs = []
    if cols[:len(golden)] != golden:
        probs.append("frames header does not start with the 133 core columns")
    rest = cols[len(golden):]
    n_ext = sum(1 for x in rest if x.startswith("ext_"))
    if not all(x.startswith("ext_") for x in rest[:n_ext]) or not all(x.startswith("bot_") for x in rest[n_ext:]):
        probs.append("frames header: expected ext_* then bot_* columns after the core columns")
    if "bot_family" not in rest:
        probs.append("frames header has no bot_family column")
    return probs


def check_event_header(header: str, root: Path = FORK_ROOT) -> list:
    cols = header.strip("\r\n").split(",")
    exp = list(columns(root).events)
    return [] if cols == exp else [f"events header is not the {len(exp)}-column event header"]


# ---------------------------------------------------------------- triplets

@dataclass
class Triplet:
    """One logger output triplet; `read(kind)` returns the bytes of frames/events/meta."""
    prefix: str
    names: dict = field(default_factory=dict)     # kind -> member name or path
    opener: object = None                         # callable(name) -> bytes

    def read(self, kind: str) -> bytes:
        return self.opener(self.names[kind])

    @property
    def complete(self) -> bool:
        return all(k in self.names for k in KIND_EXT)

    @property
    def label(self) -> str:
        return self.prefix or "(main)"


def _triplet_order(prefix: str):
    m = re.search(r"(\d+)_(\d+)_$", prefix)
    seg = prefix.count("segments/") > 0 or m is not None
    return (seg, int(m.group(1)) if m else 0, int(m.group(2)) if m else 0, prefix)


def group_triplets(names, opener) -> tuple:
    """(complete triplets in capture order, names that are not telemetry members, incomplete prefixes)."""
    groups, other = {}, []
    for n in names:
        m = MEMBER_RE.match(n)
        if not m or m.group("ext") != KIND_EXT[m.group("kind")]:
            other.append(n)
            continue
        t = groups.setdefault(m.group("prefix"), Triplet(m.group("prefix"), opener=opener))
        t.names[m.group("kind")] = n
    trips = [groups[p] for p in sorted(groups, key=_triplet_order)]
    return [t for t in trips if t.complete], other, [t.label for t in trips if not t.complete]


def zip_triplets(zf: zipfile.ZipFile):
    names = [i.filename for i in zf.infolist() if not i.is_dir()]
    return group_triplets(names, zf.read)


def telemetry_dir(path: Path) -> Path:
    """Accept the game dir (containing telemetry/) or the telemetry dir itself."""
    path = Path(path)
    if not (path / "movement_frames.csv").exists() and not (path / "segments").is_dir() and (path / "telemetry").is_dir():
        return path / "telemetry"
    return path


def dir_triplets(path: Path):
    """Triplets of a logger output directory (top level and segments/)."""
    root = telemetry_dir(path)
    names = []
    for p in sorted(root.rglob("movement_*")) + sorted(root.rglob("*_movement_*")):
        if p.is_file():
            rel = p.relative_to(root).as_posix()
            if rel not in names:
                names.append(rel)
    return group_triplets(names, lambda n: (root / n).read_bytes())


# ---------------------------------------------------------------- content helpers

def sha256_hex(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_meta_text(text: str) -> dict:
    """{session_id: {key: value}}; same parsing as the data repo's load_captures.read_meta."""
    c = configparser.ConfigParser(interpolation=None)
    c.read_string(text)
    return {s.removeprefix("session "): dict(c[s]) for s in c.sections()}


def meta_sections(text: str) -> tuple:
    """(preamble text before the first section, {session_id: raw section text}) keeping the bytes as written."""
    parts = re.split(r"(?m)^(?=\[session )", text)
    pre, secs = parts[0], {}
    for p in parts[1:]:
        sid = p[len("[session "):p.index("]")]
        secs[sid] = p if p.endswith("\n") else p + "\n"
    return pre, secs


def first_line(data: bytes) -> str:
    end = data.find(b"\n")
    return (data if end < 0 else data[:end]).decode("utf-8", errors="replace")


def split_lines(data: bytes) -> list:
    """Records of a logger CSV as bytes lines without terminators (header first)."""
    lines = data.split(b"\n")
    if lines and lines[-1] == b"":
        lines.pop()
    return [ln[:-1] if ln.endswith(b"\r") else ln for ln in lines]


def csv_columns(data: bytes, include: list, strings=()) -> "pyarrow.Table":
    """Read only `include` columns of a CSV with pyarrow (fast; used for session / is_bot scans)."""
    import pyarrow.csv as pacsv
    opts = pacsv.ConvertOptions(include_columns=list(include),
                                column_types={c: "string" for c in strings if c in include},
                                strings_can_be_null=False)
    return pacsv.read_csv(io.BytesIO(data), convert_options=opts)
