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
// test_hb_belief.cpp: the belief filter and the no-truth-leak guarantee.
//
// No-truth-leak: two runs that differ only in where a hidden, silent enemy
// really is must produce bitwise identical decisions and diagnostics.

#include "hb_brain.h"
#include "hb_bundle.h"
#include "hb_perception_model.h"
#include "hb_test.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

static hb::RawInput BaseInput(const hb::MapPrior& m)
{
    hb::RawInput raw;
    hb::SelfState& s = raw.self;
    s.alive          = true;
    s.spectator      = false;
    s.origin         = m.cells[m.NumCells() / 2].center;
    s.eye            = s.origin + hb::Vec3(0, 0, 82);
    s.weaponClass    = hb::WEAPON_CLASS_SMG;
    s.weaponState    = 0;
    s.clipAmmo       = 32;
    s.clipSize       = 32;
    s.reserveAmmo    = 96;
    s.health         = 100;
    hb::RawEnemy e;
    e.id    = 1;
    e.alive = true;
    raw.enemies.push_back(e);
    return raw;
}

static bool SamePlan(const hb::TickPlan& a, const hb::TickPlan& b)
{
    bool same = a.owner == b.owner && a.chord == b.chord && a.attack == b.attack && a.lean == b.lean && a.crouch == b.crouch
             && a.jump == b.jump && a.walk == b.walk && a.use == b.use && a.yawDelta == b.yawDelta
             && a.pitchDelta == b.pitchDelta && a.viewStill == b.viewStill && a.command == b.command
             && a.navTarget.x == b.navTarget.x && a.navTarget.y == b.navTarget.y && a.navTarget.z == b.navTarget.z;
    for (int k = 0; k < 8; k++) {
        same = same && a.flickFrac[k] == b.flickFrac[k];
    }
    return same;
}

static bool SameDiag(const hb::Diag& a, const hb::Diag& b)
{
#define HB_SAME(type, member, def, column) \
    if (std::strcmp(column, "bot_eval_belief_err") && std::memcmp(&a.member, &b.member, sizeof(a.member))) { \
        std::printf("  diag differs: %s\n", column);                                                     \
        return false;                                                                                    \
    }
    HB_DIAG_FIELDS(HB_SAME)
#undef HB_SAME
    return true;
}

// A wall in the plane x = 0 for y <= 0, of any height: a corner at the origin.
class WallWorld : public hb::WorldQuery
{
public:
    bool Clear(const hb::Vec3& a, const hb::Vec3& b) const override
    {
        if ((a.x < 0.0f) == (b.x < 0.0f)) {
            return true;
        }
        const float y = a.y + (b.y - a.y) * (0.0f - a.x) / (b.x - a.x);
        return y > 0.0f;
    }
};

// The map's own cell visibility as geometry: corners appear wherever the believed paths come into view.
class TableWorld : public hb::WorldQuery
{
public:
    explicit TableWorld(const hb::MapPrior& m)
        : m_m(m)
    {}
    bool Clear(const hb::Vec3& a, const hb::Vec3& b) const override
    {
        const int ca = m_m.CellAt(a - hb::Vec3(0, 0, 60)), cb = m_m.CellAt(b - hb::Vec3(0, 0, 80));
        return ca < 0 || cb < 0 || m_m.Visibility(ca, cb) >= 0.5f;
    }

private:
    const hb::MapPrior& m_m;
};

// From west of the wall, a path running north on its east side comes out where the line from the eye
// passes the wall's end: the corner is the wall's edge, and the open side is north (left as seen from the eye).
static void TestCorner(const hb::ModelBundle& b)
{
    hb::MapPrior m;
    for (int k = 0; k < 6; k++) {
        hb::MapCell c;
        c.center = hb::Vec3(100.0f, -200.0f + 70.0f * k, 0.0f);
        m.cells.push_back(c);
    }
    WallWorld        w;
    hb::BeliefFilter f;
    f.Init(&b.shared.belief, &b.shared.perception, &m, hb::Rng(7));
    f.SetWorld(&w);
    const hb::Vec3 eye(-300.0f, -100.0f, 60.0f);
    // cells 1-3 behind the wall from this eye, 4 (the exposure) in the open, then 5
    const int path[hb::PATH_BACK + 3] = {1, 2, 3, 4, 5, -1};
    hb::Vec3  k;
    float     open = 0.0f;
    const bool ok  = f.FindCorner(eye, path, hb::PATH_BACK + 3, k, open);
    HB_REPORT("corner at (%.1f, %.1f, %.1f), open %+.0f", k.x, k.y, k.z, open);
    HB_CHECK(ok);
    HB_CHECK(std::fabs(k.x) < 2.0f && std::fabs(k.y) < 4.0f);
    HB_CHECK(open > 0.0f);
    // an exposure that is still hidden from the true eye: the path is followed on toward the bot
    const int late[hb::PATH_BACK + 3] = {0, 1, 2, 3, 4, 5};
    HB_CHECK(f.FindCorner(eye, late, hb::PATH_BACK + 3, k, open) && std::fabs(k.y) < 4.0f);
    // no crossing out of cover along the path: no corner
    const int hidden[hb::PATH_BACK + 3] = {0, 1, 2, 3, -1, -1};
    HB_CHECK(!f.FindCorner(eye, hidden, hb::PATH_BACK + 3, k, open));
}

// The enemy walks somewhere hidden; `variant` changes where. With a world the brain traces corners.
static int RunHidden(const hb::ModelBundle& b, const hb::MapPrior& m, int variant, std::vector<hb::TickPlan>& plans,
                     std::vector<hb::Diag>& diags, const hb::WorldQuery *world = nullptr)
{
    int cornerTicks = 0;
    hb::Brain     br;
    hb::Perceiver pc;
    br.Init(&b, &m, hb::SampleStyle(b.style, -1, 99), 1234, 4);
    br.SetWorld(world);
    pc.Init(&b.shared.perception, hb::Rng(99).Derive(hb::STREAM_PERCEPTION));
    hb::RawInput raw = BaseInput(m);
    for (int t = 0; t < 600; t++) {
        raw.self.timeMs = t * 50;
        hb::RawEnemy& e = raw.enemies[0];
        // the true hidden position differs between variants (and a careless glue even passes it)
        const hb::MapCell& c = m.cells[(variant * 97 + t / 7) % m.NumCells()];
        e.centroid = c.center + hb::Vec3(0, 0, 47);
        e.velocity = hb::Vec3(variant ? 150.0f : -80.0f, 0, 0);
        e.partMask = 0;
        e.inFov    = false;
        e.centroidLos = false;
        hb::Observation obs;
        pc.Process(raw, 96.4f, 64.4f, 1.0f, obs);
        hb::TickPlan plan;
        hb::Diag     d;
        br.Think(obs, plan, &d);
        plans.push_back(plan);
        diags.push_back(d);
        raw.self.viewYaw   = hb::Wrap180(raw.self.viewYaw + plan.yawDelta);
        raw.self.viewPitch = raw.self.viewPitch + plan.pitchDelta;
        const int f = br.Belief().Focus();
        if (f >= 0) {
            const hb::BeliefEstimate& e = br.Belief().Track(f);
            bool any = false;
            for (int i = 0; i < e.nExposure; i++) {
                any = any || e.cornerValid[i];
            }
            cornerTicks += any;
        }
    }
    return cornerTicks;
}

static void TestNoTruthLeak(const hb::ModelBundle& b, const hb::MapPrior& m, const hb::WorldQuery *world = nullptr)
{
    std::vector<hb::TickPlan> p0, p1;
    std::vector<hb::Diag>     d0, d1;
    const int corners = RunHidden(b, m, 0, p0, d0, world);
    RunHidden(b, m, 1, p1, d1, world);
    if (world) {
        HB_REPORT("hidden run with the map's geometry: a corner on %d of %d ticks", corners, static_cast<int>(p0.size()));
        HB_CHECK(corners > 0);
    }
    int firstDiff = -1;
    for (size_t t = 0; t < p0.size(); t++) {
        if (!SamePlan(p0[t], p1[t]) || !SameDiag(d0[t], d1[t])) {
            firstDiff = static_cast<int>(t);
            break;
        }
    }
    if (firstDiff >= 0) {
        std::printf("  hidden truth changed the decisions at tick %d\n", firstDiff);
    }
    HB_CHECK(firstDiff < 0);
}

// A life starts like a human's: 2-4 ticks of empty usercmds (no keys, no run bit, no
// mouse, no fire), then the keys already held (none or forward), with the respawn
// click still held in about half of the lives and no strafe for the first ticks.
static void TestSpawn(const hb::ModelBundle& b, const hb::MapPrior& m)
{
    int       deadHist[8] = {};
    int       lives = 0, noneOrFwd = 0, click = 0, earlyStrafe = 0;
    const int n     = 600;
    for (int seed = 0; seed < n; seed++) {
        hb::Brain     br;
        hb::Perceiver pc;
        br.Init(&b, &m, hb::SampleStyle(b.style, -1, seed), 5000 + seed, 4);
        pc.Init(&b.shared.perception, hb::Rng(seed).Derive(hb::STREAM_PERCEPTION));
        hb::RawInput raw = BaseInput(m);
        raw.enemies[0].centroid = raw.self.origin + hb::Vec3(3000, 0, 0);
        int  dead      = 0;
        bool live      = false;
        int  liveTicks = 0;
        for (int t = 0; t < 12; t++) {
            raw.self.timeMs = t * 50;
            hb::Observation obs;
            pc.Process(raw, 96.4f, 64.4f, 1.0f, obs);
            hb::TickPlan plan;
            br.Think(obs, plan, nullptr);
            const bool empty = plan.walk && plan.chord == hb::CHORD_NEUTRAL && !plan.attack && plan.viewStill
                            && plan.yawDelta == 0.0f && plan.pitchDelta == 0.0f && plan.lean == 0;
            if (!live && empty) {
                dead++;
                continue;
            }
            if (!live) {
                live = true;
                lives++;
                const int f = hb::ChordFwd(plan.chord);
                noneOrFwd += hb::ChordSide(plan.chord) == 0 && f >= 0;
                click += plan.attack;
            }
            if (liveTicks++ < 3) {
                earlyStrafe += hb::ChordSide(plan.chord) != 0;
            }
        }
        deadHist[std::min(dead, 7)]++;
    }
    HB_REPORT("spawn: dead ticks 2/3/4 = %.2f/%.2f/%.2f, first keys none-or-forward %.2f, click held %.2f, strafe in 3 ticks %.3f",
              deadHist[2] / double(n), deadHist[3] / double(n), deadHist[4] / double(n), noneOrFwd / double(lives),
              click / double(lives), earlyStrafe / (3.0 * lives));
    HB_CHECK(lives == n);
    HB_CHECK(deadHist[0] == 0 && deadHist[1] == 0 && deadHist[2] + deadHist[3] + deadHist[4] >= n - 2);
    HB_CHECK(deadHist[3] > deadHist[2]);           // 23% / 39% / 38% in the recordings
    HB_CHECK(noneOrFwd / double(lives) > 0.95);
    HB_CHECK(click / double(lives) > 0.4 && click / double(lives) < 0.6);
    HB_CHECK(earlyStrafe / (3.0 * lives) < 0.05);
}

static void TestTracking(const hb::ModelBundle& b, const hb::MapPrior& m)
{
    hb::BeliefFilter bf;
    bf.Init(&b.shared.belief, &b.shared.perception, &m, hb::Rng(4));
    hb::Observation obs;
    obs.self.alive  = true;
    obs.self.spectator = false;
    obs.self.origin = m.cells[0].center;
    obs.self.eye    = obs.self.origin + hb::Vec3(0, 0, 82);
    hb::EnemyObs eo;
    eo.id = 1;
    obs.enemies.push_back(eo);
    const hb::Vec3 seenAt = m.cells[m.NumCells() / 3].center;
    // seen for a few ticks, then gone
    for (int t = 0; t < 5; t++) {
        obs.self.timeMs          = t * 50;
        obs.enemies[0].detected  = true;
        obs.enemies[0].pos       = seenAt + hb::Vec3(0, 0, 47);
        obs.enemies[0].vel       = hb::Vec3(200, 0, 0);
        bf.Update(obs, 96.4f, 64.4f);
    }
    const hb::BeliefEstimate& e = bf.Track(0);
    HB_CHECK(e.detected && e.seenThisLife);
    HB_CHECK((e.mode - seenAt).lengthXY() < 1.0f);
    obs.enemies[0].detected = false;
    float spread200 = 0.0f;
    for (int t = 5; t < 85; t++) {
        obs.self.timeMs = t * 50;
        bf.Update(obs, 96.4f, 64.4f);
        if (t == 9) {
            spread200 = bf.Track(0).spread;
            // dead reckoning: the mode moved along the last seen velocity
            HB_CHECK(bf.Track(0).mode.x > seenAt.x + 20.0f);
        }
    }
    HB_REPORT("belief spread 200 ms after loss %.0f u, 4 s after %.0f u", spread200, bf.Track(0).spread);
    HB_CHECK(bf.Track(0).spread > spread200);
    HB_CHECK(bf.Track(0).msSinceSeen >= 4000);

    // a gunshot pulls the belief toward its bearing
    auto bearingErr = [&](const hb::Vec3& src) {
        const hb::BeliefEstimate& est = bf.Track(0);
        return std::fabs(hb::Wrap180(hb::YawOf(est.mode - obs.self.origin) - hb::YawOf(src - obs.self.origin)));
    };
    const hb::Vec3 src = m.cells[(m.NumCells() * 3) / 4].center;
    const float before = bearingErr(src);
    hb::SoundObs so;
    so.type      = hb::SOUND_GUNFIRE;
    so.yaw       = hb::YawOf(src - obs.self.origin);
    so.yawSigma  = 10.0f;
    so.dist      = (src - obs.self.origin).length();
    so.distLogSd = 0.35f;
    obs.sounds.push_back(so);
    obs.self.timeMs += 50;
    bf.Update(obs, 96.4f, 64.4f);
    obs.sounds.clear();
    HB_REPORT("bearing error to a gunshot: %.1f deg before, %.1f after", before, bearingErr(src));
    HB_CHECK(bearingErr(src) < 25.0f);

    // a kill re-seeds the enemy on the spawn points after the respawn delay
    obs.gotKillOf = 1;
    obs.self.timeMs += 50;
    bf.Update(obs, 96.4f, 64.4f);
    obs.gotKillOf = -1;
    HB_CHECK(bf.Track(0).dead);
    for (int t = 0; t < 40; t++) {
        obs.self.timeMs += 50;
        bf.Update(obs, 96.4f, 64.4f);
    }
    HB_CHECK(!bf.Track(0).dead);
    float nearest = 1e9f;
    for (const hb::MapSpawn& sp : m.spawns) {
        nearest = std::min(nearest, (sp.pos - bf.Track(0).mode).lengthXY());
    }
    HB_REPORT("belief mode after respawn is %.0f u from a spawn point", nearest);
    HB_CHECK(nearest < 300.0f);
}

// Out of ammunition (people never are: they die first) the bot closes in and bashes with the pistol, a tap at a time,
// once the enemy is in reach with the crosshair on it. With rounds left it shoots and never bashes.
static void TestOutOfAmmo(const hb::ModelBundle& b, const hb::MapPrior& m)
{
    struct Result {
        int   bashes = 0, doubles = 0, fires = 0;
        float urgency = 0.0f;
    };
    auto run = [&](float dist, bool dry) {
        hb::Brain br;
        br.Init(&b, &m, hb::SampleStyle(b.style, -1, 7), 77, 4);
        hb::Observation obs;
        hb::SelfState&  s = obs.self;
        s.alive       = true;
        s.spectator   = false;
        s.origin      = m.cells[m.NumCells() / 2].center;
        s.eye         = s.origin + hb::Vec3(0, 0, 82);
        s.health      = 100;
        s.weaponState = 0;
        s.hasPistol   = true;
        s.weaponClass = hb::WEAPON_CLASS_PISTOL;
        s.clipSize    = 7;
        s.clipAmmo    = dry ? 0 : 7;
        s.reserveAmmo = dry ? 0 : 50;
        s.pistolAmmo  = s.clipAmmo + s.reserveAmmo;
        hb::EnemyObs e;
        e.id          = 1;
        e.detected    = true;
        e.visParts    = hb::NUM_PARTS;
        e.partMask    = (1 << hb::NUM_PARTS) - 1;
        e.centroidLos = true;
        e.pos         = s.eye + hb::Vec3(dist, 0, -40);
        for (int k = 0; k < hb::NUM_PARTS; k++) {
            e.partPos[k] = e.pos;
        }
        obs.enemies.push_back(e);
        s.viewYaw   = hb::YawOf(e.pos - s.eye);
        s.viewPitch = hb::PitchOf(e.pos - s.eye);
        Result        r;
        bool          prev = false;
        hb::TickPlan  plan;
        hb::Diag      d;
        for (int t = 0; t < 200; t++) {
            s.timeMs                  = 1000 + t * 50;
            obs.enemies[0].visibleMs  = t * 50;
            br.Think(obs, plan, &d);
            r.bashes  += plan.bash;
            r.doubles += plan.bash && prev;
            prev       = plan.bash;
            r.fires   += t >= 20 && plan.attack;   // the first live ticks may repeat the respawn click
            r.urgency  = d.nav_urgency;
        }
        return r;
    };
    const Result nearDry = run(70.0f, true), farDry = run(400.0f, true), nearLoaded = run(70.0f, false);
    HB_REPORT("out of ammunition: %d bash taps in 10 s at 70 u (%d on consecutive ticks), %d at 400 u (nav urgency %.1f); "
              "loaded: %d bashes, %d ticks firing", nearDry.bashes, nearDry.doubles, farDry.bashes, farDry.urgency,
              nearLoaded.bashes, nearLoaded.fires);
    HB_CHECK(nearDry.bashes > 20 && nearDry.doubles == 0 && nearDry.fires == 0);
    HB_CHECK(farDry.bashes == 0 && farDry.urgency > 0.99f);
    HB_CHECK(nearLoaded.bashes == 0 && nearLoaded.fires > 0);
}

int main()
{
    hb::ModelBundle b;
    std::string     err;
    if (!hb::LoadBundle({}, b, err)) {
        std::printf("bundle failed: %s\n", err.c_str());
        return 1;
    }
    hb::MapPrior m;
    if (!hb::LoadMapPrior(hb::EmbeddedText("maps/dm_main.json"), "", m, err)) {
        std::printf("map failed: %s\n", err.c_str());
        return 1;
    }
    TestNoTruthLeak(b, m);
    // the corner traces ask the map only: still nothing of the hidden truth gets in
    TableWorld w(m);
    TestNoTruthLeak(b, m, &w);
    TestCorner(b);
    TestSpawn(b, m);
    TestTracking(b, m);
    TestOutOfAmmo(b, m);
    return hbtest::Finish("test_hb_belief");
}
