#!/usr/bin/env python3
"""Compare bot-eval captures with the pooled human reference.

Usage: compare.py CAPTURE... [--name STEM] [--out-dir DIR] [--reuse] [--no-private] [--skip-analysis]
                  [--n-boot 200] [--date YYYY-MM-DD] [--movement-repo PATH] [--reference JSON] [--moh-dir DIR]

CAPTURE is a bot-eval ZIP, a directory of ZIPs or a logger telemetry directory. Writes
humanbot/eval/reports/<capture stem>/report.md and report.json (--out-dir to change;
several captures are reported together under a combined stem):

  (a) pooled bots vs pooled humans for every metrics.py statistic, with 95% block-bootstrap
      CIs; the bots are reweighted so their style-family mix of duel time matches
      humanbot/model/styles.json (presser / strafer / stopper weights)
  (b) per bot: dial recovery (realised dial - drawn dial of its bot_style event, as % of the
      human range max - min in styles.json) and the styles.py fingerprint checks (features
      outside the human min-max, z-distance to the human cloud centre, and with the private
      data repo the distance to the nearest human alias x capture row vs the within-person
      threshold; only the distance is printed, never which human is nearest)
  (c) tells: statistics whose bot and human CIs do not overlap, ranked by the standardized
      gap (bots - humans) / sqrt(SE_bots^2 + SE_humans^2)
The owner's rows (pstN@vsbot) are never pooled with the bots; they are reported
separately, with the owner's results against the bots.

With a MOHAA folder (--moh-dir or $MOHAA_DIR, read only) the "perception.*" statistics (pre-aim, the corner,
the reaction from the first visible part: perception.py) are scored too, on the body-part column the bots'
own perception logged (ext_vis_parts); the corners are traced through the practice maps' BSPs.

Pipeline: load_bot_captures.py -> humanbot/eval/cache/bot/<stem>/ (git-ignored) -> the
data repo's unchanged common.py, combat.py, lives.py, acquisition.py, aim_model.py,
engagements.py, behavior.py and styles.py -> metrics.py. The analysis scripts live in the
private openmohaa-movement repository; without it (CI, or --skip-analysis) only the capture
summary, the bot roster and the drawn dials are reported.
"""
from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import re
import shutil
import sys
from pathlib import Path

import numpy as np
import pandas as pd

sys.path.insert(0, str(Path(__file__).resolve().parent))
import load_bot_captures as LB  # noqa: E402
from hbeval import (BOT_PREFIX, EVAL_CACHE, HUMAN_REFERENCE, OWNER_NAME, REPORTS_DIR, load_styles, log,  # noqa: E402
                    movement_repo, repo_arg, repo_revision, rounded, run_analysis)

NOT_TELLS = {"fights.win_share"}          # outcome against the opponent, not a behaviour
DIAL_RANGE_EXTRA = {"mp40_share": (0.0, 1.0)}


# ------------------------------------------------------------------ helpers

def stem_of(inputs, name=None) -> str:
    if name:
        if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._+-]*", name) or ".." in name:
            raise SystemExit(f"--name must be a plain file name, not {name!r}")
        return name
    stems = []
    for i in inputs:
        p = Path(i)
        stems.append(p.stem if p.suffix.lower() == ".zip" else p.resolve().name)
    if len(stems) == 1:
        return stems[0]
    return "combined-" + hashlib.sha256("\n".join(sorted(stems)).encode()).hexdigest()[:10]


def family_of(person: str) -> str:
    return person.split(":")[1] if person.startswith(BOT_PREFIX) else "human"


def fmt(v, unit):
    if v is None or (isinstance(v, float) and not np.isfinite(v)):
        return "-"
    if unit in ("share", "per_tick"):
        return f"{100 * v:.1f}%"
    if unit == "ms":
        return f"{v:.0f} ms"
    if unit == "deg":
        return f"{v:.2f}°"
    if unit == "deg/s":
        return f"{v:.0f}°/s"
    if unit == "per_min":
        return f"{v:.2f}/min"
    if unit == "u":
        return f"{v:.0f} u"
    if unit == "u/s":
        return f"{v:.0f} u/s"
    if unit == "s":
        return f"{v:.1f} s"
    return f"{v:.3g}"


def fmt_ci(s, unit):
    if not s:
        return "-"
    return f"{fmt(s.get('value'), unit)} [{fmt(s.get('lo'), unit)}, {fmt(s.get('hi'), unit)}]"


def overlap(a, b) -> bool | None:
    vals = [a.get("lo"), a.get("hi"), b.get("lo"), b.get("hi")]
    if any(v is None or not np.isfinite(v) for v in vals):
        return None
    return bool(a["lo"] <= b["hi"] and b["lo"] <= a["hi"])


def zgap(a, b):
    sa, sb = a.get("se"), b.get("se")
    if any(v is None or not np.isfinite(v) for v in (a.get("value"), b.get("value"), sa, sb)):
        return None
    den = np.sqrt(sa ** 2 + sb ** 2)
    return float((a["value"] - b["value"]) / den) if den > 0 else None


# ------------------------------------------------------------------ comparison pieces

def pooled_table(D, bots, owner, human_ref) -> tuple:
    rows, tells = {}, []
    H = human_ref.get("metrics", {})
    for name, m in D.metrics.items():
        b, o, h = bots.get(name, {}), owner.get(name, {}), H.get(name)
        if h is not None:
            h = {k: h.get(k) for k in ("value", "lo", "hi", "se", "n", "n_blocks", "reliable")}
        r = {"label": m.label, "section": m.section, "unit": m.unit, "skill": m.perf, "bots": b, "owner": o, "human": h}
        if h is not None and b:
            ov = overlap(b, h)
            z = zgap(b, h)
            r["z"] = z
            tell = (ov is False and b.get("reliable") and h.get("reliable") and name not in NOT_TELLS)
            r["tell"] = bool(tell)
            if tell:
                rel = (b["value"] - h["value"]) / abs(h["value"]) if h["value"] else None
                tells.append({"metric": name, "label": m.label, "unit": m.unit, "bots": b, "human": h, "z": z, "rel_gap": rel})
            if o:
                r["owner_differs_from_humans"] = bool(overlap(o, h) is False and o.get("reliable") and h.get("reliable"))
        rows[name] = r
    tells.sort(key=lambda t: -abs(t["z"]) if t["z"] is not None else 0)
    return rows, tells


def dial_recovery(D, roster, styles) -> dict:
    import metrics as M
    bots = [r for r in roster if r["is_bot"]]
    real = M.dials(D, [r["name"] for r in bots])
    out = {}
    for r in bots:
        drawn = r.get("drawn_dials") or {}
        got = real.get(r["name"], {})
        dd = {}
        for k in M.DIALS:
            lo, hi = DIAL_RANGE_EXTRA.get(k, (styles["min"].get(k), styles["max"].get(k)))
            rng = (hi - lo) if lo is not None and hi is not None else None
            dv, rv = drawn.get(k), got.get(k)
            err = None
            if dv is not None and rv is not None and np.isfinite(rv) and rng:
                err = 100.0 * (rv - dv) / rng
            dd[k] = {"drawn": dv, "realised": rv, "error_pct_of_human_range": err,
                     "human_range": [lo, hi]}
        out[r["name"]] = {"family": r["family"], "seed": r["seed"], "duel_minutes": got.get("minutes", 0.0), "dials": dd}
    return out


def human_rows(repo):
    """Private: the human alias x capture fingerprint rows (styles.py output), without their labels."""
    cands = [EVAL_CACHE / "human" / "results" / "styles.json"]
    if repo is not None:
        cands.append(repo / "analysis" / "results" / "styles.json")
    for c in cands:
        if c.exists():
            S = json.loads(c.read_text())
            df = pd.DataFrame(S["by_raw_alias_and_capture"]).T
            return df.drop(columns=["minutes", "mp40_share"]).astype(float).reset_index(drop=True)
    return None


def fingerprints(results: Path, roster, cloud, hrows) -> dict:
    S = json.loads((results / "styles.json").read_text())
    rows = S.get("by_raw_alias_and_capture", {})
    feats = cloud["features"]
    mean = pd.Series(cloud["mean"])[feats]
    std = pd.Series(cloud["std"])[feats]
    lo, hi = pd.Series(cloud["min"])[feats], pd.Series(cloud["max"])[feats]
    Zh = ((hrows[feats] - mean) / std).to_numpy() if hrows is not None else None
    out = {}
    names = {r["name"] for r in roster}
    for key, fp in rows.items():
        name, date = key.rsplit("@", 1)
        if name not in names:
            continue
        x = pd.Series(fp)[feats].astype(float)
        z = ((x - mean) / std).to_numpy()
        ok = np.isfinite(z)
        outside = {f: [float(x[f]), float(lo[f]), float(hi[f])] for f in feats
                   if np.isfinite(x[f]) and (x[f] < lo[f] or x[f] > hi[f])}
        cd = float(np.sqrt((z[ok] ** 2).sum()))
        r = {"capture_date": date, "minutes": fp.get("minutes"), "features_used": int(ok.sum()), "features": len(feats),
             "outside_human_range": outside, "centre_distance": cd,
             "human_centre_distance_max": cloud["centre_distance"]["max"],
             "inside_human_spread": cd <= cloud["centre_distance"]["max"]}
        if Zh is not None:
            d = np.sqrt(np.nansum((Zh[:, ok] - z[ok][None, :]) ** 2, axis=1))
            r["nearest_human_distance"] = float(d.min())
            r["within_person_threshold"] = cloud["within_person_max_distance"]
            r["as_close_as_a_human_to_themself"] = bool(d.min() <= cloud["within_person_max_distance"])
        out.setdefault(name, []).append(r)
    return out


def opponent_mix(cache: Path) -> dict:
    """Share of the bots' duel time against another bot vs against the owner (logger opponent = nearest enemy)."""
    F = pd.read_parquet(cache / "features.parquet", columns=["session_id", "client_id", "opponent_id", "person", "eligible"])
    who = F.drop_duplicates(["session_id", "client_id"]).set_index(["session_id", "client_id"]).person
    E = F[F.eligible & F.person.str.startswith(BOT_PREFIX)]
    opp = who.reindex(pd.MultiIndex.from_arrays([E.session_id, E.opponent_id])).to_numpy()
    vs_bot = pd.Series(opp).fillna("").str.startswith(BOT_PREFIX)
    return {"bot_duel_rows": int(len(E)), "share_vs_bots": float(vs_bot.mean()) if len(E) else None,
            "share_vs_owner": float((pd.Series(opp) == OWNER_NAME).mean()) if len(E) else None}


def owner_results(cache: Path, summaries) -> dict:
    E = pd.read_parquet(cache / "events.parquet", columns=["session_id", "event", "actor_id", "actor_name", "target_id",
                                                           "target_name"])
    K = E[E.event.eq("death")]
    kills = K[K.actor_name.eq(OWNER_NAME) & K.target_name.str.startswith(BOT_PREFIX)]
    deaths = K[K.target_name.eq(OWNER_NAME) & K.actor_name.str.startswith(BOT_PREFIX)]
    suic = K[K.target_name.eq(OWNER_NAME) & (K.actor_id.eq(K.target_id) | K.actor_id.lt(0))]
    by_fam = {}
    for f in sorted({family_of(n) for n in pd.concat([kills.target_name, deaths.actor_name])}):
        k = int(kills.target_name.map(family_of).eq(f).sum())
        d = int(deaths.actor_name.map(family_of).eq(f).sum())
        by_fam[f] = {"kills": k, "deaths": d}
    s = summaries.get("owner", {})
    return {"kills_of_bots": int(len(kills)), "deaths_to_bots": int(len(deaths)), "suicides": int(len(suic)),
            "kd": (len(kills) / len(deaths)) if len(deaths) else None, "by_bot_family": by_fam,
            "accuracy": s.get("combat.accuracy"), "decisive_engagements_won": s.get("fights.win_share"),
            "first_hitter_wins": s.get("fights.first_hitter_wins")}


# ------------------------------------------------------------------ report

def write_markdown(path: Path, R: dict):
    L = []
    cap = R["capture"]
    L.append(f"# Bot evaluation: {R['stem']}")
    L.append("")
    L.append(f"Generated {R['generated']} by `humanbot/eval/compare.py`. Captures: {', '.join('`' + c + '`' for c in cap['inputs'])}. "
             f"{len(cap['sessions'])} sessions on {', '.join(cap['maps'])}; capture dates {', '.join(cap['capture_dates'])}.")
    href = R.get("human_reference") or {}
    if href:
        co = href.get("cohort", {})
        L.append(f"Human reference: `human_reference.json` ({href.get('generated')}), {co.get('duel_minutes', 0):.0f} duel minutes, "
                 f"{co.get('blocks')} player-sessions, {co.get('people')} people.")
    L.append("")
    L.append("## Bots in this capture")
    L.append("")
    L.append("| Bot | Family | Seed | Sessions | Duel minutes |")
    L.append("|---|---|---:|---:|---:|")
    for b in R["bots"]:
        L.append(f"| `{b['name']}` | {b['family']} | {b['seed'] if b['seed'] is not None else '-'} | {len(b['sessions'])} | "
                 f"{b.get('duel_minutes', 0):.1f} |")
    ow = R.get("owner_minutes")
    if ow is not None:
        L.append("")
        L.append(f"The owner (`{OWNER_NAME}`) has {ow:.1f} duel minutes against the bots; those rows are never pooled with the bots.")
    op = R.get("opponents")
    if op and op.get("share_vs_owner") is not None:
        L.append("")
        L.append(f"Bot duel time against the owner {100 * op['share_vs_owner']:.0f}%, against other bots "
                 f"{100 * op['share_vs_bots']:.0f}%.")
    fm = R.get("family_mix")
    if fm:
        L.append("")
        L.append("Family mix of bot duel time (observed → reweighted to the styles.json targets): " + ", ".join(
            f"{k} {100 * v:.0f}% → {100 * fm['targets_used'].get(k, v):.0f}%" for k, v in fm["observed"].items()) + ".")
    if R.get("analysis") is None:
        L.append("")
        L.append("## Analysis skipped")
        L.append("")
        L.append(R.get("analysis_skipped", "The statistics need the private openmohaa-movement analysis scripts."))
        _drawn_table(L, R)
        _notes(L, R)
        path.write_text("\n".join(L) + "\n")
        return
    A = R["analysis"]
    L.append("")
    L.append(f"## Tells ({len(A['tells'])})")
    L.append("")
    L.append("Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap "
             "z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.")
    L.append("")
    if A["tells"]:
        L.append("| # | Statistic | Bots | Humans | z |")
        L.append("|---:|---|---|---|---:|")
        for i, t in enumerate(A["tells"], 1):
            L.append(f"| {i} | {t['label']} | {fmt_ci(t['bots'], t['unit'])} | {fmt_ci(t['human'], t['unit'])} | "
                     f"{t['z']:+.1f} |" if t["z"] is not None else f"| {i} | {t['label']} | {fmt_ci(t['bots'], t['unit'])} | "
                     f"{fmt_ci(t['human'], t['unit'])} | - |")
    else:
        L.append("None.")
    L.append("")
    L.append("## Per bot")
    for name, pb in A["per_bot"].items():
        L.append("")
        L.append(f"### `{name}` ({pb['family']}, seed {pb['seed']}, {pb['duel_minutes']:.1f} duel minutes)")
        L.append("")
        L.append("Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn "
                 "in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.")
        L.append("")
        L.append("| Dial | Drawn | Realised | Error (% of human range) |")
        L.append("|---|---:|---:|---:|")
        for k, d in pb["dials"].items():
            e = d["error_pct_of_human_range"]
            mark = " ⚠" if e is not None and abs(e) >= 25 else ""
            L.append(f"| {k} | {_num(d['drawn'])} | {_num(d['realised'])} | {('%+.0f%%' % e) if e is not None else '-'}{mark} |")
        for fp in pb.get("fingerprint", []):
            L.append("")
            txt = (f"Style fingerprint ({fp['capture_date']}, {fp['minutes']:.1f} min, styles.py features {fp['features_used']}/"
                   f"{fp['features']}): z-distance to the human cloud centre {fp['centre_distance']:.1f} (human captures: up to "
                   f"{fp['human_centre_distance_max']:.1f}; {'inside' if fp['inside_human_spread'] else 'OUTSIDE'} the human spread).")
            if "nearest_human_distance" in fp:
                txt += (f" Nearest human capture at z-distance {fp['nearest_human_distance']:.1f} vs the within-person threshold "
                        f"{fp['within_person_threshold']:.1f} ({'as close as' if fp['as_close_as_a_human_to_themself'] else 'farther than'}"
                        f" two captures of one human).")
            L.append(txt)
            if fp["outside_human_range"]:
                L.append("")
                L.append("Features outside the human min–max: " + "; ".join(
                    f"{k} {v[0]:.3g} (humans {v[1]:.3g}–{v[2]:.3g})" for k, v in fp["outside_human_range"].items()) + ".")
            else:
                L.append("")
                L.append("Every fingerprint feature is inside the human min–max.")
    L.append("")
    L.append(f"## The owner against the bots (`{OWNER_NAME}`)")
    L.append("")
    o = A["owner_vs_bots"]
    L.append(f"Kills of bots {o['kills_of_bots']}, deaths to bots {o['deaths_to_bots']}, suicides {o['suicides']}"
             + (f", K/D {o['kd']:.2f}" if o["kd"] is not None else "") + ". "
             + (f"Shot accuracy {fmt_ci(o['accuracy'], 'share')}. " if o.get("accuracy") else "")
             + (f"Decisive engagements won {fmt_ci(o['decisive_engagements_won'], 'share')}." if o.get("decisive_engagements_won") else ""))
    if o["by_bot_family"]:
        L.append("")
        L.append("By bot family: " + ", ".join(f"{k} {v['kills']}–{v['deaths']}" for k, v in o["by_bot_family"].items()) + " (kills–deaths).")
    diff = [r for r in A["pooled"].values() if r.get("owner_differs_from_humans")]
    L.append("")
    L.append(f"{len(diff)} statistics of the owner against bots fall outside the human CI (owner column below, marked •).")
    L.append("")
    L.append("## All statistics")
    L.append("")
    L.append("Pooled bots are reweighted to the human family mix; humans and the owner are pooled over their duel time. "
             "▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.")
    sec = None
    for name, r in A["pooled"].items():
        if r["section"] != sec:
            sec = r["section"]
            L.append("")
            L.append(f"### {sec.capitalize()}")
            L.append("")
            L.append("| Statistic | Bots | Humans | Owner | n bots |")
            L.append("|---|---|---|---|---:|")
        b, h, o = r["bots"], r["human"] or {}, r["owner"]
        mark = " ▲" if r.get("tell") else ""
        omark = " •" if r.get("owner_differs_from_humans") else ""
        L.append(f"| {r['label']}{mark} | {fmt_ci(b, r['unit'])} | {fmt_ci(h, r['unit'])} | {fmt_ci(o, r['unit'])}{omark} | "
                 f"{b.get('n', 0):.0f} |")
    L.append("")
    v = A["validation"]
    L.append(f"Consistency: {v['checked']} pooled statistics (bots and owner together) were recomputed from the analysis "
             f"scripts' own JSON; {v['differ']} differ." + ("" if not v["differ"] else " See report.json."))
    _notes(L, R)
    path.write_text("\n".join(L) + "\n")


def _num(v):
    if v is None or (isinstance(v, float) and not np.isfinite(v)):
        return "-"
    return f"{v:.4g}"


def _drawn_table(L, R):
    bots = [b for b in R["bots"] if b.get("drawn_dials")]
    if not bots:
        return
    keys = [k for k in bots[0]["drawn_dials"] if k not in ("family", "seed")]
    L.append("")
    L.append("Drawn dials (bot_style events):")
    L.append("")
    L.append("| Bot | " + " | ".join(keys) + " |")
    L.append("|---|" + "---:|" * len(keys))
    for b in bots:
        L.append(f"| `{b['name']}` | " + " | ".join(_num(b["drawn_dials"].get(k)) for k in keys) + " |")


def _notes(L, R):
    if R.get("notes"):
        L.append("")
        L.append("## Notes")
        L.append("")
        for n in R["notes"]:
            L.append(f"- {n}")


# ------------------------------------------------------------------ main

def run(inputs, name=None, out_dir=None, reference=HUMAN_REFERENCE, private=True, reuse=False, n_boot=200,
        skip_analysis=False, repo=None, date=None, cache_root=None, moh_dir=None) -> Path:
    stem = stem_of(inputs, name)
    cache = Path(cache_root or EVAL_CACHE / "bot") / stem
    results = cache / "results"
    out = Path(out_dir) if out_dir else REPORTS_DIR / stem
    repo = repo if repo is not None else movement_repo()
    if reuse and (cache / "bot_roster.json").exists():
        summary = json.loads((cache / "bot_roster.json").read_text())
    else:
        if cache.exists():
            shutil.rmtree(cache)          # no stale script outputs from an earlier run
        summary = LB.load(inputs, cache, date, repo)
    roster = summary["roster"]
    styles = load_styles()
    targets = {f["name"]: float(f["weight"]) for f in styles["families"]}
    R = {"version": 1, "stem": stem, "generated": dt.date.today().isoformat(),
         "capture": {k: summary[k] for k in ("captures", "sessions", "maps", "capture_dates", "frames", "events")},
         "bots": [], "notes": list(summary.get("notes", []))}
    R["capture"]["inputs"] = R["capture"].pop("captures")
    href = json.loads(Path(reference).read_text()) if Path(reference).exists() else None
    if href:
        R["human_reference"] = {k: href.get(k) for k in ("generated", "analysis_revision", "cohort", "bootstrap")}
    bots_meta = [dict(r) for r in roster if r["is_bot"]]
    R["bots"] = bots_meta
    if skip_analysis or repo is None or href is None:
        R["analysis"] = None
        why = ("--skip-analysis given" if skip_analysis else
               "openmohaa-movement (the unchanged analysis scripts) is not available" if repo is None else
               f"{reference} is missing (run build_human_reference.py)")
        R["analysis_skipped"] = f"The statistics were not computed: {why}. Only the capture summary and the drawn dials are shown."
    else:
        import metrics as M
        failed = {}
        if not (reuse and (results / "styles.json").exists()):
            log(f"running the analysis scripts on {cache}")
            timing = run_analysis(cache, results, repo=repo, quiet=False, keep_going=True)
            failed = {k: v for k, v in timing.items() if isinstance(v, str)}
            for k, v in failed.items():
                R["notes"].append(f"analysis/{k} {v} (log in {results / 'logs'}); its statistics are missing")
        D = M.prepare(cache, results, moh_dir=moh_dir)
        if "perception" in D.extra:
            R["notes"].append("perception statistics on the bots' logged body-part column (ext_vis_parts: their own "
                              "perception); the people's are on the data repo's rebuilt column, accepted against the "
                              "logged one on 2026-09-28")
        per = D.persons()
        fam = np.array([family_of(p) for p in per], dtype=object)
        is_bot = np.array([p.startswith(BOT_PREFIX) for p in per])
        is_owner = per == OWNER_NAME
        rows = np.asarray(D.rows, float)
        observed = {f: float(rows[is_bot & (fam == f)].sum() / max(rows[is_bot].sum(), 1)) for f in sorted(set(fam[is_bot]))}
        present = {f: w for f, w in targets.items() if observed.get(f, 0) > 0}
        tot = sum(present.values())
        fam_total = sum(v for f, v in observed.items() if f in targets)
        used = {f: (present[f] / tot * fam_total if f in present else v) for f, v in observed.items()} if tot else observed
        R["family_mix"] = {"targets": targets, "observed": observed, "targets_used": used}
        R["opponents"] = opponent_mix(cache)
        if R["opponents"]["share_vs_bots"]:
            R["notes"].append(f"{100 * R['opponents']['share_vs_bots']:.0f}% of the bots' duel time is against another bot "
                              "(the logged opponent is the nearest living enemy); the human reference is humans against "
                              "humans only")
        if set(observed) - set(targets):
            R["notes"].append("bots outside the three style families (" + ", ".join(sorted(set(observed) - set(targets)))
                              + ") keep their own share of the pooled bot time")
        summaries = {"bots": M.summarize(D, is_bot, fam, targets, n_boot=n_boot),
                     "owner": M.summarize(D, is_owner, n_boot=n_boot),
                     "all": M.summarize(D, np.ones(D.nb, bool), n_boot=2)}
        pairs = M.validate(summaries["all"], results)
        bad = M.mismatches(pairs)
        pooled, tells = pooled_table(D, summaries["bots"], summaries["owner"], href)
        per_bot = dial_recovery(D, roster, styles)
        mins = {p: rows[per == p].sum() / 1200 for p in set(per)}
        for b in bots_meta:
            b["duel_minutes"] = float(mins.get(b["name"], 0.0))
        R["owner_minutes"] = float(mins.get(OWNER_NAME, 0.0))
        cloud = href.get("style_cloud")
        if cloud and (results / "styles.json").exists() and "styles.py" not in failed:
            hrows = human_rows(repo) if private else None
            if private and hrows is None:
                R["notes"].append("private nearest-human check skipped: no human styles.json available")
            fps = fingerprints(results, roster, cloud, hrows)
            for n, fp in fps.items():
                if n in per_bot:
                    per_bot[n]["fingerprint"] = fp
        R["analysis"] = {"analysis_revision": repo_revision(repo), "n_boot": n_boot, "pooled": pooled, "tells": tells,
                         "per_bot": per_bot, "owner_vs_bots": owner_results(cache, summaries),
                         "validation": {"checked": len(pairs), "differ": len(bad),
                                        "details": [{"metric": a, "ours": b, "script": c} for a, b, c in bad]}}
        R["notes"] += D.notes
        R["notes"].append("engagements.py counts contact episodes only while exactly two players are alive; with more than "
                          "two players in a session the fight statistics cover the phases where one player is dead")
        if len(bots_meta) and min(b["duel_minutes"] for b in bots_meta) < 20:
            R["notes"].append("some bots have under 20 duel minutes; human alias x capture fingerprints have 25-60, so "
                              "fingerprints and dials of short captures are noisy")
    out.mkdir(parents=True, exist_ok=True)
    (out / "report.json").write_text(json.dumps(rounded(R, 5), indent=1) + "\n")
    write_markdown(out / "report.md", json.loads((out / "report.json").read_text()))
    log(f"report: {out / 'report.md'}")
    if R.get("analysis"):
        log(f"{len(R['analysis']['tells'])} tells; validation {R['analysis']['validation']['differ']} of "
            f"{R['analysis']['validation']['checked']} differ")
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("captures", nargs="+")
    ap.add_argument("--name", help="report stem (default: the capture stem)")
    ap.add_argument("--out-dir", help="report directory (default humanbot/eval/reports/<stem>)")
    ap.add_argument("--reference", default=str(HUMAN_REFERENCE))
    ap.add_argument("--no-private", action="store_true", help="skip the nearest-human check even if the data repo is here")
    ap.add_argument("--skip-analysis", action="store_true", help="capture summary and drawn dials only")
    ap.add_argument("--reuse", action="store_true", help="reuse the cache and script outputs of an earlier run")
    ap.add_argument("--n-boot", type=int, default=200)
    ap.add_argument("--date", help="capture date for logger directories")
    ap.add_argument("--movement-repo")
    ap.add_argument("--moh-dir", help="MOHAA folder with the practice maps (perception statistics); default $MOHAA_DIR")
    a = ap.parse_args(argv)
    run(a.captures, a.name, a.out_dir, a.reference, not a.no_private, a.reuse, a.n_boot, a.skip_analysis,
        repo_arg(a.movement_repo), a.date, moh_dir=a.moh_dir)


if __name__ == "__main__":
    main()
