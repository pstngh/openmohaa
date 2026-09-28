#!/usr/bin/env python3
"""Pooled human reference for the bot evaluation: humanbot/eval/human_reference.json.

Usage: build_human_reference.py [--movement-repo PATH] [--analysis-cache DIR] [--reuse] [--n-boot 200]

Needs the private openmohaa-movement repository. The human analysis cache (frames/events/
features.parquet built by its load_captures.py + common.py) is taken from
--analysis-cache, $HB_ANALYSIS_CACHE or humanbot/cache/analysis, and rebuilt from the
captures when missing. A private work dir humanbot/eval/cache/human/ (git-ignored) links
those files, and the unchanged scripts (combat, lives, acquisition, aim_model, engagements,
behavior, styles) run there; --reuse keeps earlier outputs.

Written (pooled aggregates only, no alias, no per-person value):
  metrics      every metrics.py statistic over all human duel blocks (player x session):
               value, 95% block-bootstrap CI (lo, hi), bootstrap SE, units n, blocks
  style_cloud  the styles.py fingerprint over the human alias x capture rows: per-feature
               mean, std, min, max; row distances to the cloud centre (min / median / max);
               the within-person threshold (largest z-distance between two captures of one
               person) and the between-person distances (min / median)
  cohort       duel minutes, blocks, sessions, people (counts)
The script checks that its pooled statistics reproduce the scripts' JSON before writing.
"""
from __future__ import annotations

import argparse
import datetime as dt
import json
import os
import subprocess
import sys
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import metrics as M  # noqa: E402
from hbeval import (EVAL_CACHE, HUMAN_REFERENCE, human_analysis_cache, import_analysis_module, log, repo_arg,  # noqa: E402
                    repo_revision, run_analysis, write_json)

SCRIPTS = ["combat.py", "lives.py", "acquisition.py", "aim_model.py", "engagements.py", "behavior.py", "styles.py"]
LINKED = ["frames.parquet", "events.parquet", "features.parquet", "meta.json"]


def style_frame(styles_json: Path) -> pd.DataFrame:
    """styles.py fingerprint rows (alias x capture) with the features styles.py compares."""
    S = json.loads(Path(styles_json).read_text())
    df = pd.DataFrame(S["by_raw_alias_and_capture"]).T
    return df.drop(columns=["minutes", "mp40_share"]).astype(float)


def style_cloud(fpt: pd.DataFrame, person_of) -> dict:
    """Anonymous summary of the human style cloud (styles.py z-space: z-score over the human rows)."""
    mean, std = fpt.mean(), fpt.std()
    Z = ((fpt - mean) / std).to_numpy()
    D = np.sqrt(((Z[:, None, :] - Z[None, :, :]) ** 2).sum(-1))
    who = [person_of(i.rsplit("@", 1)[0]) for i in fpt.index]
    same = np.array([[a == b for b in who] for a in who])
    iu = np.triu_indices(len(who), 1)
    within = D[iu][same[iu]]
    between = D[iu][~same[iu]]
    centre = np.sqrt((Z ** 2).sum(1))
    return {"features": list(fpt.columns), "rows": int(len(fpt)), "people": int(len(set(who))),
            "mean": mean.to_dict(), "std": std.to_dict(), "min": fpt.min().to_dict(), "max": fpt.max().to_dict(),
            "centre_distance": {"min": float(centre.min()), "median": float(np.median(centre)), "max": float(centre.max())},
            "within_person_max_distance": float(within.max()) if len(within) else None,
            "within_person_pairs": int(len(within)),
            "between_person_distance": {"min": float(between.min()), "median": float(np.median(between))},
            "about": "z = (x - mean) / std per feature; distances are Euclidean in z-space over all features, as in "
                     "styles.py. Two captures of the same person are at most within_person_max_distance apart."}


def work_dir(src: Path, work: Path):
    work.mkdir(parents=True, exist_ok=True)
    for f in LINKED:
        dst = work / f
        if dst.is_symlink() or dst.exists():
            if dst.is_symlink() and dst.resolve() == (src / f).resolve():
                continue
            dst.unlink()
        os.symlink((src / f).resolve(), dst)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--movement-repo")
    ap.add_argument("--analysis-cache", help="data-repo cache with features.parquet (default $HB_ANALYSIS_CACHE or "
                                             "humanbot/cache/analysis)")
    ap.add_argument("--work", default=str(EVAL_CACHE / "human"))
    ap.add_argument("--reuse", action="store_true", help="keep existing script outputs in the work dir")
    ap.add_argument("--n-boot", type=int, default=M.N_BOOT)
    ap.add_argument("--out", default=str(HUMAN_REFERENCE))
    a = ap.parse_args(argv)
    repo = repo_arg(a.movement_repo)
    if repo is None:
        raise SystemExit("openmohaa-movement not found: set HB_MOVEMENT_REPO (the human reference needs the private data)")
    work = Path(a.work)
    results = work / "results"
    src = Path(a.analysis_cache) if a.analysis_cache else human_analysis_cache()
    if src is None or not (src / "features.parquet").exists():
        log("no human analysis cache: building it from the captures (load_captures.py + common.py)")
        src = work / "base"
        src.mkdir(parents=True, exist_ok=True)
        subprocess.run([sys.executable, str(repo / "analysis" / "load_captures.py"), str(repo / "captures"), str(src)],
                       check=True, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1"))
        run_analysis(src, results, ["common.py"], repo)
    work_dir(src, work)
    todo = [s for s in SCRIPTS if not (a.reuse and (results / (s.replace(".py", ".json"))).exists())]
    if todo:
        log(f"running {', '.join(todo)} on {work}")
        run_analysis(work, results, todo, repo, quiet=False)
    D = M.prepare(work, results)
    allm = M.summarize(D, np.ones(D.nb, bool), n_boot=a.n_boot)
    pairs = M.validate(allm, results)
    bad = M.mismatches(pairs)
    log(f"validation against the scripts' JSON: {len(pairs)} statistics checked, {len(bad)} differ")
    for b in bad:
        log("  differs:", b)
    common = import_analysis_module("common", repo)
    person = common.PERSON
    cloud = style_cloud(style_frame(results / "styles.json"), lambda n: person.get(n, n))
    per = D.persons()
    EL_rows = np.asarray(D.rows)
    out = {
        "version": 1,
        "about": "Pooled human 1v1 duel reference (openmohaa-movement REPORT section 11 acceptance statistics) for "
                 "humanbot/eval/compare.py. Aggregates over all humans only: no alias, no per-person value.",
        "generated": dt.date.today().isoformat(),
        "generator": "humanbot/eval/build_human_reference.py",
        "analysis_revision": repo_revision(repo),
        "bootstrap": {"n_boot": a.n_boot, "seed": 0, "block": "player x session", "ci": [2.5, 97.5]},
        "cohort": {"duel_minutes": float(EL_rows.sum() / 1200), "blocks": int((EL_rows > 0).sum()),
                   "sessions": int(len(set(D.sessions()[EL_rows > 0]))), "people": int(len(set(per[EL_rows > 0])))},
        "validation": {"checked": len(pairs), "differ": len(bad)},
        "metrics": {},
        "style_cloud": cloud,
    }
    for name, m in D.metrics.items():
        s = allm[name]
        out["metrics"][name] = {"label": m.label, "section": m.section, "unit": m.unit, "kind": m.kind,
                                "performance": m.perf, **{k: s[k] for k in ("value", "lo", "hi", "se", "n", "n_blocks")},
                                "reliable": s["reliable"]}
    write_json(Path(a.out), out)
    log(f"wrote {a.out}: {len(out['metrics'])} metrics, style cloud of {cloud['rows']} rows "
        f"(within-person threshold {cloud['within_person_max_distance']:.2f})")
    if bad:
        log("WARNING: some statistics differ from the scripts' output; check metrics.py definitions")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
