/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMoHAA source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMoHAA source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// test_hb_core.cpp: RNG streams, model bundle, map priors and style sampling.

#include "hb_bundle.h"
#include "hb_map.h"
#include "hb_rng.h"
#include "hb_style.h"
#include "hb_test.h"

#include <cmath>
#include <map>
#include <vector>

static void TestRng()
{
    // xoshiro256** seeded by splitmix64: identical on every platform
    hb::Rng        r(42);
    const uint64_t expect[4] = {0x15780b2e0c2ec716ULL, 0x6104d9866d113a7eULL, 0xae17533239e499a1ULL, 0xecb8ad4703b360a1ULL};
    for (uint64_t e : expect) {
        HB_CHECK(r.Next() == e);
    }
    HB_CHECK(hb::Rng(42).Derive(hb::STREAM_VIEW).Next() == 0xc89fffe8b8ad4928ULL);
    HB_CHECK(hb::Rng(7).Uniform() == 0.7005764821796896);
    // sub-streams are independent of each other's use
    hb::Rng a = hb::Rng(9).Derive(1);
    hb::Rng b = hb::Rng(9).Derive(2);
    hb::Rng a2 = hb::Rng(9).Derive(1);
    for (int i = 0; i < 100; i++) {
        b.Next();
    }
    HB_CHECK(a.Next() == a2.Next());

    // distribution moments
    hb::Rng g(123);
    const int n = 200000;
    double    s = 0, s2 = 0, st = 0, sg = 0, sb = 0, sl = 0;
    int       cat[3] = {0, 0, 0};
    const double w[3] = {0.2, 0.5, 0.3};
    for (int i = 0; i < n; i++) {
        const double x = g.Normal();
        s += x;
        s2 += x * x;
        const double t = g.StudentTUnitVar(6.0);
        st += t * t;
        sg += g.Gamma(2.5);
        sb += g.Beta(2.0, 5.0);
        sl += std::log(g.LogNormal(300.0, 0.5));
        cat[g.Categorical(w, 3)]++;
    }
    HB_CHECK_NEAR(s / n, 0.0, 0.01);
    HB_CHECK_NEAR(s2 / n, 1.0, 0.02);
    HB_CHECK_NEAR(st / n, 1.0, 0.06);
    HB_CHECK_NEAR(sg / n, 2.5, 0.03);
    HB_CHECK_NEAR(sb / n, 2.0 / 7.0, 0.005);
    HB_CHECK_NEAR(sl / n, std::log(300.0), 0.01);
    HB_CHECK_NEAR(cat[0] / double(n), 0.2, 0.005);
    HB_CHECK_NEAR(cat[1] / double(n), 0.5, 0.005);
    const std::vector<double> qp = {0.0, 0.5, 1.0};
    const std::vector<double> qv = {100.0, 200.0, 400.0};
    int below = 0;
    for (int i = 0; i < 20000; i++) {
        below += g.FromQuantiles(qp, qv) < 200.0;
    }
    HB_CHECK_NEAR(below / 20000.0, 0.5, 0.02);
}

static void TestBundle(hb::ModelBundle& bundle)
{
    std::string err;
    HB_CHECK(hb::LoadBundle({}, bundle, err));
    HB_CHECK(err.empty());
    HB_CHECK(bundle.source == "embedded");
    // the embedded hash matches the texts (embed_model.py is in sync)
    std::string all;
    for (const std::string& name : hb::EmbeddedNames()) {
        all += name + "\n" + hb::EmbeddedText(name) + "\n";
    }
    HB_CHECK(hb::Sha256Hex(all) == hb::EmbeddedSha256());
    HB_CHECK(hb::Sha256Hex("abc") == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

    const hb::SharedModel& s = bundle.shared;
    const size_t nAge = s.movement.ageEdges.size();
    HB_CHECK(s.movement.side.switchLogit.v.size() == size_t(hb::CTX_COUNT * 9) * nAge);
    HB_CHECK(s.movement.fwd.switchLogit.v.size() == size_t(hb::CTX_COUNT * 9) * nAge);
    HB_CHECK(s.movement.fwdNext.v.size() == size_t(hb::CTX_COUNT * 27));
    HB_CHECK(s.movement.side.wallLogit.size() == s.movement.clearEdges.size());
    // walls raise the chance of letting go of a key; the open bin is the reference
    HB_CHECK(s.movement.side.wallLogit.front() > 0.0f && s.movement.side.wallLogit.back() == 0.0f);
    HB_CHECK(s.view.stillStay.size() == hb::CTX_COUNT);
    HB_CHECK(s.view.firing.Kp > s.view.idle.Kp);  // firing tracks much harder than idle
    // lean next-state rows are distributions
    const hb::Table& ln = s.movement.leanNext;
    for (size_t i = 0; i < ln.v.size(); i += 3) {
        HB_CHECK_NEAR(ln.v[i] + ln.v[i + 1] + ln.v[i + 2], 1.0, 1e-3);
    }

    // a valid override is merge-patched
    std::map<std::string, std::string> ov;
    ov["shared.json"] = "{\"view\":{\"tuning\":{\"noise_scale\":1.7}}}";
    hb::ModelBundle b2;
    HB_CHECK(hb::LoadBundle(ov, b2, err));
    HB_CHECK_NEAR(b2.shared.view.noiseScale, 1.7, 1e-6);
    HB_CHECK(b2.sha256 != bundle.sha256);
    // a broken override falls back to the embedded model and says why
    ov["shared.json"] = "{\"movement\":{\"keys\":{\"side\":{\"switch_logit\":[1,2,3]}}}}";
    hb::ModelBundle b3;
    HB_CHECK(!hb::LoadBundle(ov, b3, err));
    HB_CHECK(!err.empty());
    HB_CHECK_NEAR(b3.shared.view.noiseScale, bundle.shared.view.noiseScale, 1e-6);
    ov["shared.json"] = "{not json";
    HB_CHECK(!hb::LoadBundle(ov, b3, err));
}

static void TestMaps()
{
    // the practice maps' BSP header checksums (sv_mapChecksum), which the engine glue compares exactly: the four duel
    // maps and two practice areas of the objective maps (dm/brownffa, the Bridge of obj_team4; dm/flag, part of V2)
    const std::map<std::string, int32_t> sums = {{"maps/dm_crnodoors.json", -17593952}, {"maps/dm_main.json", 832297909},
                                                 {"maps/dm_vents.json", 1765529479}, {"maps/dm_downladder.json", -1474374008},
                                                 {"maps/dm_brownffa.json", -1751530710}, {"maps/dm_flag.json", 166358282}};
    int nmaps = 0;
    for (const std::string& name : hb::EmbeddedNames()) {
        if (name.compare(0, 5, "maps/") != 0) {
            continue;
        }
        nmaps++;
        hb::MapPrior m;
        std::string  err;
        HB_CHECK(hb::LoadMapPrior(hb::EmbeddedText(name), "", m, err));
        HB_CHECK(m.NumCells() > 100);
        HB_CHECK(m.checksum != 0);
        HB_CHECK(sums.count(name) && sums.at(name) == m.checksum);
        // every cell finds itself
        int self = 0;
        for (int c = 0; c < m.NumCells(); c++) {
            self += m.CellAt(m.cells[c].center) == c;
        }
        HB_CHECK(self > m.NumCells() * 0.98);
        // the route graph is mostly connected
        // from the busiest cell (a few recorded cells are dead ends)
        int hub = 0;
        for (int c = 1; c < m.NumCells(); c++) {
            if (m.cells[c].occTotal > m.cells[hub].occTotal) {
                hub = c;
            }
        }
        std::vector<int>   next;
        std::vector<float> dist;
        m.PathTreeTo(hub, next, dist);
        int reach = 0;
        for (int c = 0; c < m.NumCells(); c++) {
            reach += dist[c] < 1e29f;
        }
        HB_REPORT("%s: %d cells, %d reach the busiest cell", name.c_str(), m.NumCells(), reach);
        HB_CHECK(reach > m.NumCells() * 0.8);
        // following next pointers strictly shortens the path
        for (int c = 0; c < m.NumCells(); c++) {
            if (next[c] >= 0) {
                HB_CHECK(dist[next[c]] < dist[c]);
            }
        }
        // empirical visibility is symmetric and a cell sees itself
        HB_CHECK(m.Visibility(3, 7) == m.Visibility(7, 3));
        HB_CHECK(m.Visibility(5, 5) == 1.0f);
        // runtime table overrides the empirical one
        m.InitRuntimeVisibility();
        m.SetRuntimeVisibility(3, 7, true);
        m.FinishRuntimeVisibility();
        HB_CHECK(m.Visibility(7, 3) == 1.0f);
        std::vector<uint8_t> bits = m.RuntimeBits();
        HB_CHECK(m.LoadRuntimeBits(bits));
        bits.pop_back();
        HB_CHECK(!m.LoadRuntimeBits(bits));
    }
    HB_CHECK(nmaps == 6);
}

static void TestStyle(const hb::ModelBundle& b)
{
    int fam[hb::FAMILY_COUNT] = {0, 0, 0};
    const int n = 6000;
    for (int i = 0; i < n; i++) {
        const hb::StyleDials d = hb::SampleStyle(b.style, -1, static_cast<uint32_t>(i * 7919 + 1));
        fam[d.family]++;
        for (int k = 0; k < hb::DIAL_COUNT; k++) {
            HB_CHECK(d.dial[k] >= b.style.dialMin[k] - 1e-4f && d.dial[k] <= b.style.dialMax[k] + 1e-4f);
        }
        for (int k = 0; k < hb::SKILL_COUNT; k++) {
            HB_CHECK(d.skill[k] >= b.style.skillMin[k] - 1e-4f && d.skill[k] <= b.style.skillMax[k] + 1e-4f);
        }
        // the owner's rule (2026-10-08), as the model ships: every bot reacts and aims like the best recorded profile
        HB_CHECK(d.skill[hb::SKILL_REACTION] == b.style.skillMin[hb::SKILL_REACTION]);
        HB_CHECK(d.skill[hb::SKILL_AIM_ERROR] == b.style.skillMin[hb::SKILL_AIM_ERROR]);
        HB_CHECK(d.mp40Share >= 0.0f && d.mp40Share <= 1.0f);
    }
    for (int f = 0; f < hb::FAMILY_COUNT; f++) {
        HB_CHECK_NEAR(fam[f] / double(n), b.style.families[f].weight, 0.025);
    }
    // determinism and the userinfo key round trip
    const hb::StyleDials a = hb::SampleStyle(b.style, hb::FAMILY_STOPPER, 77);
    const hb::StyleDials c = hb::SampleStyle(b.style, hb::FAMILY_STOPPER, 77);
    for (int k = 0; k < hb::DIAL_COUNT; k++) {
        HB_CHECK(a.dial[k] == c.dial[k]);
    }
    int      fam2  = -1;
    uint32_t seed2 = 0;
    HB_CHECK(hb::ParseStyleKey(hb::StyleKey(a).c_str(), fam2, seed2));
    HB_CHECK(fam2 == hb::FAMILY_STOPPER && seed2 == 77u);
    HB_CHECK(!hb::ParseStyleKey("nobody:5", fam2, seed2));
    HB_CHECK(hb::FamilyFromName("random") < 0);
    // families keep their habits: pressers press forward-diagonally more than strafers
    double diagP = 0, diagS = 0, revT = 0, revP = 0;
    for (int i = 0; i < 500; i++) {
        diagP += hb::SampleStyle(b.style, hb::FAMILY_PRESSER, i).dial[hb::DIAL_FWD_DIAG];
        diagS += hb::SampleStyle(b.style, hb::FAMILY_STRAFER, i).dial[hb::DIAL_FWD_DIAG];
        revT += hb::SampleStyle(b.style, hb::FAMILY_STOPPER, i).dial[hb::DIAL_REVERSE];
        revP += hb::SampleStyle(b.style, hb::FAMILY_PRESSER, i).dial[hb::DIAL_REVERSE];
    }
    HB_CHECK(diagP > 2.0 * diagS);
    HB_CHECK(revT < revP);
    // offsets are monotone in the dials
    hb::StyleDials lo = a, hi = a;
    lo.family = hi.family = hb::FAMILY_PRESSER;
    lo.dial[hb::DIAL_FWD_DIAG] = b.style.dialMin[hb::DIAL_FWD_DIAG];
    hi.dial[hb::DIAL_FWD_DIAG] = b.style.dialMax[hb::DIAL_FWD_DIAG];
    HB_CHECK(hb::ComputeOffsets(lo, b.calib, b.shared).diagLogit < hb::ComputeOffsets(hi, b.calib, b.shared).diagLogit);
    const std::string js = hb::DialsJson(a);
    HB_CHECK(js.find("\"family\":\"stopper\"") != std::string::npos);
    HB_CHECK(js.find("mp40_share") != std::string::npos);
}

int main()
{
    hb::ModelBundle bundle;
    TestRng();
    TestBundle(bundle);
    TestMaps();
    TestStyle(bundle);
    return hbtest::Finish("test_hb_core");
}
