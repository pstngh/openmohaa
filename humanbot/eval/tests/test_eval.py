"""Tests of the bot-evaluation tools; no human data needed.

Run: python -m unittest discover -s humanbot/eval/tests -v

A synthetic schema-13 capture (make_fixture.py: two bots and the owner, two sessions) is
built in a temp dir. Without the private openmohaa-movement repository (CI) the tests
cover the loader (its local read_meta/read_csv copy), the statistics machinery of
metrics.py (weighted quantiles, family reweighting, block bootstrap), compare.py without
the analysis (capture summary and drawn dials), pack_capture.py and check_no_raw_data.py.
The statistics themselves need features.parquet from the data repo's unchanged
common.py, so the full compare run (and the check that the unchanged analysis scripts
accept the masqueraded cache) only runs where that repository is present.
"""
from __future__ import annotations

import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
import zipfile
from pathlib import Path

import numpy as np
import pandas as pd

TESTS = Path(__file__).resolve().parent
EVAL = TESTS.parent
FORK = EVAL.parents[1]
for p in (str(EVAL), str(TESTS), str(FORK / "humanbot" / "tools")):
    if p not in sys.path:
        sys.path.insert(0, p)

import capture_io as cio  # noqa: E402
import check_no_raw_data as CK  # noqa: E402
import compare  # noqa: E402
import hbeval  # noqa: E402
import load_bot_captures as LB  # noqa: E402
import make_fixture as MF  # noqa: E402
import metrics as M  # noqa: E402
import pack_capture as PK  # noqa: E402

REPO = hbeval.movement_repo()
SECONDS = 90


class Env:
    """Temporary HB_MOVEMENT_REPO override (a missing path simulates CI)."""

    def __init__(self, value):
        self.value, self.old = value, None

    def __enter__(self):
        self.old = os.environ.get("HB_MOVEMENT_REPO")
        os.environ["HB_MOVEMENT_REPO"] = self.value
        return self

    def __exit__(self, *exc):
        if self.old is None:
            os.environ.pop("HB_MOVEMENT_REPO", None)
        else:
            os.environ["HB_MOVEMENT_REPO"] = self.old


def setUpModule():
    global TMP, ZIP
    TMP = Path(tempfile.mkdtemp(prefix="hb_eval_test_"))
    ZIP = MF.make_zip(TMP / "fixture", seconds=SECONDS, seed=2, date="2026-10-01")


def tearDownModule():
    shutil.rmtree(TMP, ignore_errors=True)


class TestSchema(unittest.TestCase):
    def test_columns_follow_the_cpp_contract(self):
        c = cio.columns()
        self.assertEqual(c.schema, 13)
        self.assertEqual(len(c.core), 133)
        self.assertEqual(len(c.events), 49)
        self.assertTrue(all(x.startswith("ext_") for x in c.ext))
        self.assertTrue(all(x.startswith("bot_") for x in c.bot))
        self.assertIn("bot_family", c.bot)
        golden = (FORK / cio.GOLDEN_CORE).read_text().strip().split(",")
        self.assertEqual(list(c.core), golden)
        self.assertEqual(cio.check_frame_header(c.frame_header()), [])
        self.assertTrue(cio.check_frame_header(",".join(c.core)))

    def test_triplet_grouping(self):
        names = ["movement_frames.csv", "movement_events.csv", "movement_meta.txt",
                 "segments/17_5_movement_frames.csv", "segments/17_5_movement_events.csv", "segments/17_5_movement_meta.txt",
                 "segments/9_1_movement_frames.csv", "notes.txt"]
        trips, other, incomplete = cio.group_triplets(names, None)
        self.assertEqual([t.prefix for t in trips], ["", "segments/17_5_"])
        self.assertEqual(other, ["notes.txt"])
        self.assertEqual(incomplete, ["segments/9_1_"])


class TestLoader(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cache = TMP / "cache_loader"
        with Env(str(TMP / "no-such-repo")):            # CI conditions: local read_meta / read_csv
            cls.summary = LB.load([ZIP], cls.cache)
        cls.F = pd.read_parquet(cls.cache / "frames.parquet")
        cls.E = pd.read_parquet(cls.cache / "events.parquet")
        cls.B = pd.read_parquet(cls.cache / "botcols.parquet")
        cls.meta = json.loads((cls.cache / "meta.json").read_text())

    def test_files_and_format(self):
        self.assertTrue(self.summary["readers"].startswith("local copy"))
        cols = cio.columns()
        expect = [c for c in cols.frames if not c.startswith("bot_")] + ["source", "capture_date", "role"] + LB.META_NUMERIC
        self.assertEqual(list(self.F.columns), expect)
        self.assertEqual(list(self.E.columns), list(cols.events) + ["source", "capture_date", "role"])
        self.assertFalse(self.F.duplicated(LB.KEY).any())
        self.assertTrue((self.F.session_ms % 50 == 0).all())
        self.assertEqual(len(self.meta), 2)
        for v in self.meta.values():
            self.assertEqual(v["role"], "bot-eval")
            self.assertEqual(v["capture_date"], "2026-10-01")
            self.assertEqual(len(v["source"]), 12)
        self.assertTrue(self.F.g_gametype.eq(1).all() and self.F.sv_runspeed.eq(250).all())

    def test_masquerade(self):
        self.assertEqual(set(self.F.name), {"pstN@vsbot", "bot:presser:1", "bot:strafer:2"})
        self.assertTrue(self.F.is_bot.eq(0).all() and self.F.opponent_bot.eq(0).all())
        self.assertTrue(set(self.F.opponent_name) <= {"", "pstN@vsbot", "bot:presser:1", "bot:strafer:2"})
        names = set(self.E.actor_name) | set(self.E.target_name)
        self.assertTrue(names <= {"", "pstN@vsbot", "bot:presser:1", "bot:strafer:2"})
        self.assertTrue(self.E.actor_bot.eq(0).all() and self.E.target_bot.eq(0).all())
        # the original flags and brain columns are kept
        orig = self.B.groupby("client_id").is_bot.max().to_dict()
        self.assertEqual(orig, {0: 0, 1: 1, 2: 1})
        self.assertEqual(len(self.B), len(self.F))
        self.assertEqual(set(self.B[self.B.is_bot.eq(1)].bot_family), {0, 1})
        roster = {r["name"]: r for r in self.summary["roster"]}
        self.assertEqual(roster["bot:presser:1"]["seed"], 1234567)
        self.assertEqual(roster["bot:strafer:2"]["drawn_dials"]["family"], "strafer")
        self.assertIsNone(roster["pstN@vsbot"]["drawn_dials"])

    def test_same_style_keeps_one_name_across_slots(self):
        f = pd.DataFrame({"session_id": ["s1", "s1", "s2", "s2"], "client_id": [3, 4, 5, 4], "is_bot": [1, 0, 1, 0],
                          "bot_family": [0, -1, 0, -1], "bot_style_seed": [7, 0, 7, 0]})
        e = pd.DataFrame({"session_id": [], "event": [], "actor_id": [], "actor_bot": [], "target_id": [], "target_bot": [],
                          "chat_message": []})
        R = LB.build_roster(f, e.astype({"actor_id": int, "target_id": int, "actor_bot": int, "target_bot": int}),
                            {"s1": {"created_epoch": "1"}, "s2": {"created_epoch": "2"}})
        d = dict(zip(zip(R.session_id, R.client_id), R.name))
        self.assertEqual(d[("s1", 3)], "bot:presser:3")
        self.assertEqual(d[("s2", 5)], "bot:presser:3")
        self.assertEqual(d[("s1", 4)], "pstN@vsbot")

    @unittest.skipIf(REPO is None, "openmohaa-movement not available")
    def test_local_readers_match_the_data_repo(self):
        rm, rc, origin = LB.readers(REPO)
        self.assertIn("openmohaa-movement", origin)
        with zipfile.ZipFile(ZIP) as zf:
            fb, mb = zf.read("movement_frames.csv"), zf.read("movement_meta.txt").decode()
        self.assertEqual(rm(mb), LB._read_meta(mb))
        pd.testing.assert_frame_equal(rc(fb, LB.CORE_STR), LB._read_csv(fb, LB.CORE_STR))


class TestMetricsCore(unittest.TestCase):
    def test_weighted_quantile_matches_pandas(self):
        rng = np.random.default_rng(0)
        v = np.sort(rng.normal(size=301))
        for q in (0.1, 0.25, 0.5, 0.9, 0.99):
            self.assertAlmostEqual(M.wquantile(v, np.ones(len(v)), q), pd.Series(v).quantile(q), places=10)
        w = rng.integers(0, 4, len(v)).astype(float)
        rep = np.repeat(v, w.astype(int))
        for q in (0.25, 0.5, 0.75):
            self.assertAlmostEqual(M.wquantile(v, w, q), pd.Series(rep).quantile(q), places=10)

    def test_family_weights_hit_the_targets(self):
        fam = np.array(["presser", "presser", "strafer", "pooled"], dtype=object)
        rows = np.array([100.0, 100.0, 100.0, 700.0])
        W = M.block_weights(np.ones((1, 4)), fam, rows, {"presser": 0.5, "strafer": 0.5, "stopper": 0.2})
        t = W[0] * rows
        share = {f: t[fam == f].sum() / t.sum() for f in set(fam)}
        self.assertAlmostEqual(share["pooled"], 0.7)          # non-family bots keep their share
        self.assertAlmostEqual(share["presser"], 0.15)        # stopper absent: 0.5 / (0.5 + 0.5) of 0.3
        self.assertAlmostEqual(share["strafer"], 0.15)

    def test_block_bootstrap(self):
        D = M.MetricData()
        rng = np.random.default_rng(1)
        nb = 12
        blk = D.block_ids([f"s{i}" for i in range(nb)], list(range(nb)), ["p"] * nb)
        D.rows = [100] * nb
        rows_blk = np.repeat(blk, 100)
        hit = rng.random(len(rows_blk)) < 0.3
        D.add_ratio("x.share", "share", "x", "share", rows_blk, hit)
        D.add_quant("x.val", "val", "x", "ms", rows_blk, rng.exponential(300, len(rows_blk)), (("p50", .5), ("p90", .9)))
        D.finalize()
        S = M.summarize(D, np.ones(nb, bool), n_boot=100)
        self.assertAlmostEqual(S["x.share"]["value"], hit.mean())
        for k in ("x.share", "x.val.p50", "x.val.p90"):
            self.assertLessEqual(S[k]["lo"], S[k]["value"])
            self.assertGreaterEqual(S[k]["hi"], S[k]["value"])
            self.assertEqual(S[k]["n_blocks"], nb)
            self.assertTrue(S[k]["reliable"])
        half = np.arange(nb) < 6
        S2 = M.summarize(D, half, n_boot=50)
        self.assertEqual(S2["x.share"]["n"], 600)
        self.assertAlmostEqual(S2["x.share"]["value"], hit[:600].mean())

    def test_tells_rank_non_overlapping_cis(self):
        m = M.MetricData()
        m._add("a", "A", "s", "share", "ratio", False)
        m._add("b", "B", "s", "share", "ratio", False)
        bots = {"a": {"value": .5, "lo": .45, "hi": .55, "se": .02, "reliable": True},
                "b": {"value": .2, "lo": .1, "hi": .3, "se": .05, "reliable": True}}
        human = {"metrics": {"a": {"value": .3, "lo": .28, "hi": .32, "se": .01, "reliable": True},
                             "b": {"value": .25, "lo": .2, "hi": .3, "se": .02, "reliable": True}}}
        rows, tells = compare.pooled_table(m, bots, {}, human)
        self.assertEqual([t["metric"] for t in tells], ["a"])
        self.assertGreater(tells[0]["z"], 8)
        self.assertFalse(rows["b"]["tell"])


class TestCompare(unittest.TestCase):
    def test_smoke_without_analysis(self):
        out = compare.run([ZIP], out_dir=TMP / "report_smoke", skip_analysis=True, cache_root=TMP / "cmp_cache")
        R = json.loads((out / "report.json").read_text())
        self.assertIsNone(R["analysis"])
        self.assertEqual({b["name"] for b in R["bots"]}, {"bot:presser:1", "bot:strafer:2"})
        self.assertTrue(all(b["drawn_dials"] for b in R["bots"]))
        md = (out / "report.md").read_text()
        self.assertIn("Analysis skipped", md)
        self.assertIn("`bot:strafer:2`", md)
        rep = CK.Report()
        CK.check_json(rep, "report.json", (out / "report.json").read_bytes())
        self.assertEqual(rep.violations, [])

    @unittest.skipIf(not hbeval.HUMAN_REFERENCE.exists(), "human_reference.json not built yet")
    def test_against_the_committed_reference(self):
        ref = json.loads(hbeval.HUMAN_REFERENCE.read_text())
        rep = CK.Report()
        CK.check_json(rep, "human_reference.json", hbeval.HUMAN_REFERENCE.read_bytes())
        self.assertEqual(rep.violations, [])
        cloud = ref["style_cloud"]
        for k in ("mean", "std", "min", "max"):
            self.assertEqual(set(cloud[k]), set(cloud["features"]))
        self.assertGreater(cloud["within_person_max_distance"], 0)
        # tells: the reference against itself has none; one shifted statistic is the only tell
        D = M.MetricData()
        for name, m in ref["metrics"].items():
            D._add(name, m["label"], m["section"], m["unit"], m["kind"], m["performance"])
        bots = {k: {f: m[f] for f in ("value", "lo", "hi", "se", "n", "n_blocks", "reliable")} for k, m in ref["metrics"].items()}
        self.assertEqual(compare.pooled_table(D, bots, {}, ref)[1], [])
        key = "movement.chord.los_fire.pure_strafe"
        bots[key] = dict(bots[key], value=bots[key]["value"] - .3, lo=bots[key]["lo"] - .3, hi=bots[key]["hi"] - .3)
        self.assertEqual([t["metric"] for t in compare.pooled_table(D, bots, {}, ref)[1]], [key])
        # fingerprints against the anonymous cloud (no private rows)
        res = TMP / "fp_results"
        res.mkdir(exist_ok=True)
        centre = dict(cloud["mean"], minutes=30.0, mp40_share=0.5)
        far = dict(centre, lf_err_med=cloud["max"]["lf_err_med"] * 3)
        (res / "styles.json").write_text(json.dumps({"by_raw_alias_and_capture": {
            "bot:presser:1@2026-10-01": centre, "bot:strafer:2@2026-10-01": far, "pstN@vsbot@2026-10-01": centre}}))
        fps = compare.fingerprints(res, [{"name": "bot:presser:1"}, {"name": "bot:strafer:2"}], cloud, None)
        self.assertEqual(set(fps), {"bot:presser:1", "bot:strafer:2"})
        self.assertAlmostEqual(fps["bot:presser:1"][0]["centre_distance"], 0.0)
        self.assertTrue(fps["bot:presser:1"][0]["inside_human_spread"])
        self.assertEqual(list(fps["bot:strafer:2"][0]["outside_human_range"]), ["lf_err_med"])
        self.assertNotIn("nearest_human_distance", fps["bot:strafer:2"][0])

    @unittest.skipIf(REPO is None or not hbeval.HUMAN_REFERENCE.exists(), "needs openmohaa-movement and human_reference.json")
    def test_full_compare_runs_the_unchanged_scripts(self):
        out = compare.run([ZIP], out_dir=TMP / "report_full", cache_root=TMP / "cmp_cache_full", n_boot=50)
        R = json.loads((out / "report.json").read_text())
        A = R["analysis"]
        self.assertEqual(A["validation"]["differ"], 0)
        self.assertGreater(A["validation"]["checked"], 50)
        self.assertTrue(A["tells"])
        self.assertEqual(set(A["per_bot"]), {"bot:presser:1", "bot:strafer:2"})
        pb = A["per_bot"]["bot:presser:1"]
        self.assertIn("error_pct_of_human_range", pb["dials"]["reverse_share"])
        self.assertTrue(pb["fingerprint"])
        fp = pb["fingerprint"][0]
        self.assertIn("centre_distance", fp)
        # only distances: nothing says which human capture is nearest
        self.assertLessEqual(set(fp), {"capture_date", "minutes", "features_used", "features", "outside_human_range",
                                       "centre_distance", "human_centre_distance_max", "inside_human_spread",
                                       "nearest_human_distance", "within_person_threshold",
                                       "as_close_as_a_human_to_themself"})
        for k in ("bots", "human", "owner"):
            self.assertIn(k, A["pooled"]["movement.switch_hazard.150ms"])
        for s in ("combat.json", "lives.json", "acquisition.json", "aim_model.json", "engagements.json", "behavior.json",
                  "styles.json"):
            self.assertTrue((TMP / "cmp_cache_full" / ZIP.stem / "results" / s).exists())
        rep = CK.Report()
        CK.check_json(rep, "report.json", (out / "report.json").read_bytes())
        self.assertEqual(rep.violations, [])


class TestPack(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.src = TMP / "pack_src"
        MF.make_telemetry(cls.src, seconds=40, seed=5, extra_segment=True, human_only_session=True)

    def run_pack(self, out, *extra):
        p = subprocess.run([sys.executable, str(FORK / "humanbot/tools/pack_capture.py"), str(self.src), "--out", str(out),
                            "--date", "2026-10-03", *extra], capture_output=True, text=True)
        return p.returncode, p.stdout + p.stderr

    def check_zips(self, out, limit):
        sums = dict(reversed(ln.split("  ")) for ln in (out / "SHA256SUMS").read_text().splitlines())
        zips = sorted(out.glob("*.zip"))
        self.assertEqual(set(sums), {z.name for z in zips})
        for z in zips:
            self.assertLessEqual(z.stat().st_size, limit)
            m = cio.CAPTURE_RE.match(z.stem)
            self.assertIsNotNone(m)
            self.assertEqual(m.group("date"), "2026-10-03")
            self.assertEqual(cio.sha256_hex(z.read_bytes()), sums[z.name])
            with zipfile.ZipFile(z) as zf:
                first = next(n for n in zf.namelist() if "movement_frames" in n)
                self.assertEqual(cio.sha256_hex(zf.read(first))[:12], m.group("sha"))
                trips, other, inc = cio.zip_triplets(zf)
                self.assertFalse(other or inc)
                for t in trips:
                    tab = cio.csv_columns(t.read("frames"), ["session_id", "is_bot"], strings=["session_id"]).to_pandas()
                    self.assertTrue(tab.groupby("session_id").is_bot.max().eq(1).all())
                    self.assertEqual(cio.check_frame_header(cio.first_line(t.read("frames"))), [])
        return zips

    def test_split_by_session_and_roundtrip(self):
        out = TMP / "pack_sessions"
        code, log = self.run_pack(out, "--max-bytes", "400000")
        self.assertEqual(code, 0, log)
        self.assertIn("no bot rows", log)
        zips = self.check_zips(out, 400000)
        self.assertEqual(len(zips), 3)          # one session per ZIP
        with Env(str(TMP / "no-such-repo")):
            LB.load([out], TMP / "pack_c1")
            LB.load([self.src], TMP / "pack_c2", date="2026-10-03")
        a = pd.read_parquet(TMP / "pack_c1" / "frames.parquet").drop(columns=["source"])
        b = pd.read_parquet(TMP / "pack_c2" / "frames.parquet").drop(columns=["source"])
        b = b[b.session_id.isin(set(a.session_id))].reset_index(drop=True)          # minus the session without bots
        pd.testing.assert_frame_equal(a.reset_index(drop=True), b)
        code2, _ = self.run_pack(out, "--max-bytes", "400000")          # deterministic: identical re-pack
        self.assertEqual(code2, 0)

    def test_split_by_time_when_a_session_is_too_big(self):
        out = TMP / "pack_time"
        code, log = self.run_pack(out, "--max-bytes", "120000")
        self.assertEqual(code, 0, log)
        self.assertIn("(part 2)", log)
        self.check_zips(out, 120000)
        with Env(str(TMP / "no-such-repo")):
            LB.load([out], TMP / "pack_c3")
        f = pd.read_parquet(TMP / "pack_c3" / "frames.parquet")
        self.assertFalse(f.duplicated(LB.KEY).any())
        self.assertEqual(f.session_id.nunique(), 3)

    def test_one_zip_with_two_triplets(self):
        out = TMP / "pack_one"
        code, log = self.run_pack(out)
        self.assertEqual(code, 0, log)
        (z,) = self.check_zips(out, cio.MAX_ZIP_BYTES)
        with zipfile.ZipFile(z) as zf:
            self.assertEqual(len(cio.zip_triplets(zf)[0]), 2)

    def test_refusals(self):
        bad = TMP / "pack_bad" / "telemetry"
        shutil.copytree(self.src / "telemetry", bad)
        fr = bad / "movement_frames.csv"
        lines = fr.read_bytes().split(b"\n")
        lines[0] = ",".join(cio.columns().core).encode()             # a schema-12 style header
        fr.write_bytes(b"\n".join(lines))
        p = subprocess.run([sys.executable, str(FORK / "humanbot/tools/pack_capture.py"), str(bad), "--out",
                            str(TMP / "pack_bad_out")], capture_output=True, text=True)
        self.assertEqual(p.returncode, 1)
        self.assertIn("REFUSED", p.stderr)
        self.assertIn("schema-13 header", p.stderr)
        self.assertFalse((TMP / "pack_bad_out").exists())
        nobot = TMP / "pack_nobot"
        MF.write_triplet(nobot, "", [MF.SessionSpec("dm/main", 10, [MF.PlayerSpec(0, "pstN", False)], 1790000000)],
                         hbeval.load_styles(), 3)
        rc = PK.pack(nobot, TMP / "pack_nobot_out", "2026-10-03")
        self.assertEqual(rc, 1)


class TestPrivacyGuard(unittest.TestCase):
    FILES = ["humanbot/tools/check_no_raw_data.py", "humanbot/tools/embed_model.py", "humanbot/eval/capture_io.py",
             "humanbot/eval/hbeval.py", "code/humanbot/hb_embedded.cpp", "code/humanbot/hb_diag.h",
             "code/fgame/movement_telemetry_schema.h", str(cio.GOLDEN_CORE), str(cio.GOLDEN_EVENTS)]

    def setUp(self):
        self.root = Path(tempfile.mkdtemp(prefix="hb_guard_", dir=TMP))
        for f in self.FILES:
            (self.root / f).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(FORK / f, self.root / f)
        for d in ("humanbot/model", "humanbot/maps"):
            shutil.copytree(FORK / d, self.root / d)
        # in sync by construction: this test is about the privacy rules, not the model embedding
        subprocess.run([sys.executable, str(self.root / "humanbot/tools/embed_model.py")], check=True, capture_output=True)
        subprocess.run(["git", "init", "-q", str(self.root)], check=True)
        (self.root / ".gitignore").write_text("humanbot/eval/cache/\n__pycache__/\n")
        self.repo = self.root.parent / (self.root.name + "_movement")
        (self.repo / "analysis").mkdir(parents=True)
        (self.repo / "manifest.json").write_text(json.dumps({"captures": [{"players": ["zorglub", "pstN"]},
                                                                          {"players": ["^kwix^"]}]}))
        (self.repo / "analysis" / "common.py").write_text('PERSON = {"zorgy": "zorglub", "q": "pstN"}\n')

    def guard(self, *extra):
        p = subprocess.run([sys.executable, str(self.root / "humanbot/tools/check_no_raw_data.py"), "--root", str(self.root),
                            "--movement-repo", str(self.repo), *extra], capture_output=True, text=True)
        return p.returncode, p.stdout + p.stderr

    def test_clean_tree_passes(self):
        code, out = self.guard()
        self.assertEqual(code, 0, out)

    def test_planted_csv_fails(self):
        (self.root / "humanbot" / "eval").mkdir(parents=True, exist_ok=True)
        (self.root / "humanbot/eval/leak.csv").write_text("a,b\n1,2\n")
        (self.root / "humanbot/eval/cache").mkdir()
        (self.root / "humanbot/eval/cache/ok.parquet").write_bytes(b"ignored")        # git-ignored: allowed
        code, out = self.guard()
        self.assertEqual(code, 1, out)
        self.assertIn("humanbot/eval/leak.csv", out)
        self.assertNotIn("ok.parquet", out)

    def test_aliases_json_and_zips(self):
        (self.root / "docs").mkdir()
        (self.root / "docs/notes.md").write_text("pstN played q and Zorglub.\nkwix won; zorgy too\nzorglubs is a word\n")
        (self.root / "humanbot/bad.json").write_text(json.dumps({"by_person": {"x": 1}, "names": ["a"],
                                                                 "big": list(range(100001)), "people": 3}))
        (self.root / "stray.zip").write_bytes(b"PK")
        code, out = self.guard()
        self.assertEqual(code, 1, out)
        self.assertIn("docs/notes.md:1", out)             # zorglub
        self.assertIn("docs/notes.md:2", out)             # kwix (^kwix^ stripped) and zorgy
        self.assertNotIn("docs/notes.md:3", out)          # no alias on a word boundary
        self.assertNotIn("zorglub", out.lower())          # aliases are never printed in full
        self.assertIn("per-person key 'by_person'", out)
        self.assertIn("name list 'names'", out)
        self.assertIn("numeric array 'big'", out)
        self.assertNotIn("'people'", out)
        self.assertIn("ZIP outside humanbot/captures/", out)

    def test_capture_zip_rules(self):
        cap = self.root / "humanbot/captures"
        cap.mkdir(parents=True)
        shutil.copy2(ZIP, cap / ZIP.name)
        code, out = self.guard()
        self.assertEqual(code, 1, out)
        self.assertIn("not listed in humanbot/captures/SHA256SUMS", out)
        (cap / "SHA256SUMS").write_text(f"{cio.sha256_hex((cap / ZIP.name).read_bytes())}  {ZIP.name}\n")
        code, out = self.guard()
        self.assertEqual(code, 0, out)
        # a capture of humans only is refused
        nobot = TMP / "guard_nobot"
        paths = MF.write_triplet(nobot, "", [MF.SessionSpec("dm/main", 5, [MF.PlayerSpec(0, "pstN", False)], 1790000000)],
                                 hbeval.load_styles(), 4)
        name = f"2026-10-04_pstN_bot-eval_schema13_{cio.sha256_hex(paths['frames'].read_bytes())[:12]}.zip"
        with zipfile.ZipFile(cap / name, "w") as zf:
            for k, p in paths.items():
                zf.write(p, p.name)
        with open(cap / "SHA256SUMS", "a") as fh:
            fh.write(f"{cio.sha256_hex((cap / name).read_bytes())}  {name}\n")
        code, out = self.guard()
        self.assertEqual(code, 1, out)
        self.assertIn("without bot rows", out)

    def test_alias_check_skipped_without_the_data_repo(self):
        (self.root / "notes.txt").write_text("zorglub\n")
        p = subprocess.run([sys.executable, str(self.root / "humanbot/tools/check_no_raw_data.py"), "--root", str(self.root),
                            "--movement-repo", str(self.root / "missing")], capture_output=True, text=True)
        self.assertEqual(p.returncode, 0, p.stdout)
        self.assertIn("alias check is skipped", p.stdout)


if __name__ == "__main__":
    unittest.main()
