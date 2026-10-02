"""Shared paths and helpers of the human-bot evaluation tools (humanbot/eval).

The human captures and the reference analysis live in the private
openmohaa-movement repository. Its analysis scripts are used unchanged: they
are run as subprocesses (like its run_all.sh) or imported by path, never
copied. Everything here also works without that repository (CI): callers get
None from movement_repo() and must skip what needs it.
"""
from __future__ import annotations

import importlib.util
import json
import math
import os
import subprocess
import sys
import time
from pathlib import Path

import numpy as np

EVAL_DIR = Path(__file__).resolve().parent
HB_ROOT = EVAL_DIR.parent                     # <fork>/humanbot
FORK_ROOT = HB_ROOT.parent
EVAL_CACHE = Path(os.environ.get("HB_EVAL_CACHE", EVAL_DIR / "cache"))   # git-ignored (humanbot/eval/.gitignore)
REPORTS_DIR = EVAL_DIR / "reports"
HUMAN_REFERENCE = EVAL_DIR / "human_reference.json"
STYLES_JSON = HB_ROOT / "model" / "styles.json"
CAPTURES_DIR = HB_ROOT / "captures"

OWNER_ALIAS = "pstN"                 # the repository owner, named on purpose in capture file names
OWNER_NAME = OWNER_ALIAS + "@vsbot"  # masqueraded name of the owner's rows in bot-eval captures
BOT_PREFIX = "bot:"
FAMILY_NAMES = ["presser", "strafer", "stopper"]   # bot_family 0, 1, 2 (code/humanbot/hb_diag.h)
ROLE = "bot-eval"

# run_all.sh order, without competitive.py, spatial.py, figures.py and targets.py
ANALYSIS_SCRIPTS = ["common.py", "combat.py", "lives.py", "acquisition.py", "aim_model.py", "engagements.py",
                    "behavior.py", "styles.py"]


def log(*args):
    print(*args, flush=True)


def movement_repo(explicit: str | os.PathLike | None = None) -> Path | None:
    """The private openmohaa-movement checkout, or None when it is not available.

    Order: explicit argument, $HB_MOVEMENT_REPO, then ../openmohaa-movement next to the fork.
    """
    cands = []
    if explicit:
        cands.append(Path(explicit))
    elif os.environ.get("HB_MOVEMENT_REPO"):
        cands.append(Path(os.environ["HB_MOVEMENT_REPO"]))
    else:
        cands.append(FORK_ROOT.parent / "openmohaa-movement")
    for c in cands:
        if (c / "analysis" / "common.py").is_file() and (c / "analysis" / "load_captures.py").is_file():
            return c.resolve()
    return None


def repo_arg(explicit: str | None) -> Path | None:
    """movement_repo() for a --movement-repo option: an explicit path must be a valid checkout."""
    repo = movement_repo(explicit)
    if explicit and repo is None:
        raise SystemExit(f"--movement-repo {explicit}: not an openmohaa-movement checkout (analysis/common.py missing)")
    return repo


def import_analysis_module(name: str, repo: Path | None = None):
    """Import <repo>/analysis/<name>.py by path (no bytecode is written into the data repo)."""
    repo = repo or movement_repo()
    if repo is None:
        raise FileNotFoundError("openmohaa-movement not found (set HB_MOVEMENT_REPO)")
    adir = str(repo / "analysis")
    key = "hb_movement_analysis_" + name
    if key in sys.modules:
        return sys.modules[key]
    old = sys.dont_write_bytecode
    sys.dont_write_bytecode = True
    added = adir not in sys.path
    if added:
        sys.path.insert(0, adir)   # the analysis modules import each other by plain name
    try:
        spec = importlib.util.spec_from_file_location(key, repo / "analysis" / (name + ".py"))
        mod = importlib.util.module_from_spec(spec)
        sys.modules[key] = mod
        spec.loader.exec_module(mod)
    finally:
        sys.dont_write_bytecode = old
        if added:
            sys.path.remove(adir)
    return mod


def repo_revision(repo: Path | None) -> str | None:
    if repo is None:
        return None
    try:
        out = subprocess.run(["git", "-C", str(repo), "rev-parse", "--short=12", "HEAD"], capture_output=True, text=True,
                             timeout=20)
        return out.stdout.strip() or None
    except (OSError, subprocess.SubprocessError):
        return None


def run_analysis(cache: Path, results: Path, scripts=None, repo: Path | None = None, quiet: bool = True,
                 keep_going: bool = False) -> dict:
    """Run the data repo's unchanged analysis scripts on `cache`, writing JSON into `results`.

    Each script runs as `python <repo>/analysis/<script> <cache> [<results>]` exactly like
    run_all.sh; stdout/stderr go to <results>/logs/<script>.log. Raises on the first failure,
    or with keep_going records it (timing value "failed: <last log line>") and goes on;
    common.py must always succeed. Returns {script: seconds or failure}.
    """
    repo = repo or movement_repo()
    if repo is None:
        raise FileNotFoundError("the analysis scripts need openmohaa-movement (set HB_MOVEMENT_REPO)")
    results.mkdir(parents=True, exist_ok=True)
    logs = results / "logs"
    logs.mkdir(exist_ok=True)
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE="1", MPLBACKEND="Agg", NUMBA_CACHE_DIR=str(EVAL_CACHE / "numba"))
    timing = {}
    for s in scripts or ANALYSIS_SCRIPTS:
        args = [sys.executable, str(repo / "analysis" / s), str(cache)]
        if s != "common.py":
            args.append(str(results))
        t = time.time()
        with open(logs / (s.replace(".py", ".log")), "w") as fh:
            p = subprocess.run(args, stdout=fh, stderr=subprocess.STDOUT, env=env, cwd=str(repo))
        timing[s] = round(time.time() - t, 1)
        if p.returncode != 0:
            tail = (logs / s.replace(".py", ".log")).read_text(errors="replace").splitlines()[-25:]
            if keep_going and s != "common.py":
                timing[s] = "failed: " + (tail[-1] if tail else f"exit {p.returncode}")
                log(f"  {s:16s} FAILED ({timing[s]})")
                continue
            raise RuntimeError(f"analysis/{s} failed (exit {p.returncode}):\n" + "\n".join(tail))
        if not quiet:
            log(f"  {s:16s} ok ({timing[s]} s)")
    return timing


def human_analysis_cache() -> Path | None:
    """Existing data-repo cache for the human captures ($HB_ANALYSIS_CACHE or humanbot/cache/analysis)."""
    cands = [Path(os.environ["HB_ANALYSIS_CACHE"])] if os.environ.get("HB_ANALYSIS_CACHE") else []
    cands.append(HB_ROOT / "cache" / "analysis")
    for c in cands:
        if (c / "features.parquet").is_file() and (c / "events.parquet").is_file():
            return c.resolve()
    return None


def load_styles(path: Path = STYLES_JSON) -> dict:
    return json.loads(Path(path).read_text())


def family_weights(styles: dict | None = None) -> dict:
    styles = styles or load_styles()
    return {f["name"]: float(f["weight"]) for f in styles["families"]}


def jsonable(x):
    """JSON-safe copy: numpy scalars to Python, NaN/inf to None, tuple keys joined with '|'."""
    if isinstance(x, dict):
        return {("|".join(map(str, k)) if isinstance(k, tuple) else str(k)): jsonable(v) for k, v in x.items()}
    if isinstance(x, (list, tuple)):
        return [jsonable(v) for v in x]
    if isinstance(x, np.ndarray):
        return [jsonable(v) for v in x.tolist()]
    if isinstance(x, (np.integer,)):
        return int(x)
    if isinstance(x, (np.bool_,)):
        return bool(x)
    if isinstance(x, (np.floating, float)):
        v = float(x)
        return None if not math.isfinite(v) else v
    return x


def rounded(x, digits: int = 6):
    """jsonable() with floats rounded to `digits` significant digits."""
    x = jsonable(x)
    if isinstance(x, dict):
        return {k: rounded(v, digits) for k, v in x.items()}
    if isinstance(x, list):
        return [rounded(v, digits) for v in x]
    if isinstance(x, float):
        return float(f"{x:.{digits}g}")
    return x


def write_json(path: Path, obj, digits: int = 6):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(rounded(obj, digits), indent=1, sort_keys=False) + "\n")
