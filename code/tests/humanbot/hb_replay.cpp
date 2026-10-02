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
// hb_replay.cpp: drive the bot modules through recorded human situations.
//
// Reads the git-ignored exports of humanbot/fit/export_replay.py and runs one
// module at a time open loop (the recorded situation does not react to the
// bot), computing each statistic the same way for the bot and for the human
// who was actually there. calibrate.py calls it with parameter overrides.
//
// Usage: hb_replay --mode movement|trigger|acquisition|tracking|hidden
//                  --data <dir> [--override <shared.json patch>] [--offsets k=v,...]
//                  [--seed N] [--stride N] [--out file.json]

#include "hb_belief.h"
#include "hb_bundle.h"
#include "hb_movement.h"
#include "hb_perception_model.h"
#include "hb_style.h"
#include "hb_trigger.h"
#include "hb_view.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace hb;

//
// Data
//
struct Replay {
    std::string                     map;
    int                             nrows = 0;
    std::map<std::string, std::vector<float>> cols;

    const float *C(const char *name) const
    {
        auto it = cols.find(name);
        if (it == cols.end()) {
            std::fprintf(stderr, "missing column %s\n", name);
            std::exit(2);
        }
        return it->second.data();
    }
};

static bool LoadHbr(const std::string& path, Replay& r)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        return false;
    }
    char magic[4];
    f.read(magic, 4);
    if (std::memcmp(magic, "HBR1", 4)) {
        return false;
    }
    uint32_t nc = 0, nr = 0;
    f.read(reinterpret_cast<char *>(&nc), 4);
    f.read(reinterpret_cast<char *>(&nr), 4);
    std::vector<std::string> names;
    for (uint32_t i = 0; i < nc; i++) {
        std::string s;
        char        ch;
        while (f.get(ch) && ch) {
            s += ch;
        }
        names.push_back(s);
    }
    r.nrows = static_cast<int>(nr);
    for (uint32_t i = 0; i < nc; i++) {
        std::vector<float>& v = r.cols[names[i]];
        v.resize(nr);
        f.read(reinterpret_cast<char *>(v.data()), static_cast<std::streamsize>(nr) * 4);
    }
    return static_cast<bool>(f);
}

struct Seg {
    int  a, b;   // [a, b)
    bool spawn;  // starts at the first live tick after a respawn: runs starting at a are complete
    int  index;  // position among all segments of the file (seeds per-segment random streams)
    // a run that starts at t has an observed start
    bool Known(int t) const { return t > a || spawn; }
};

// Unbroken segments. The export drops the empty usercmds at the start of every
// life (the client catching up with the respawn): the brain reproduces that
// dead time itself, so replays start at the first live tick.
// With a stride, only every stride-th segment: the bot runs and the statistics
// of both the bot and the human use the same segments.
static std::vector<Seg> Segments(const Replay& r, int stride)
{
    std::vector<Seg> out;
    const float     *k  = r.C("seg_key");
    const float     *sp = r.C("spawn_seg");
    int              a  = 0;
    int              n  = 0;
    for (int i = 1; i <= r.nrows; i++) {
        if (i == r.nrows || k[i] != k[i - 1]) {
            if (i - a >= 2) {
                if (n % stride == 0) {
                    out.push_back(Seg{a, i, sp[a] > 0.5f, n});
                }
                n++;
            }
            a = i;
        }
    }
    return out;
}

static bool Valid(float v)
{
    return v > -9000.0f;
}

//
// JSON output
//
struct Json {
    std::ostringstream s;
    bool               first = true;
    void Key(const std::string& k)
    {
        s << (first ? "" : ",") << "\"" << k << "\":";
        first = false;
    }
    void Num(const std::string& k, double v)
    {
        Key(k);
        if (std::isfinite(v)) {
            s << v;
        } else {
            s << "null";
        }
    }
    void Raw(const std::string& k, const std::string& v)
    {
        Key(k);
        s << v;
    }
    std::string Str() const { return "{" + s.str() + "}"; }
};

static std::string Arr(const std::vector<double>& v)
{
    std::ostringstream s;
    s << "[";
    for (size_t i = 0; i < v.size(); i++) {
        s << (i ? "," : "");
        if (std::isfinite(v[i])) {
            s << v[i];
        } else {
            s << "null";
        }
    }
    s << "]";
    return s.str();
}

static double Quantile(std::vector<double> v, double q)
{
    if (v.empty()) {
        return NAN;
    }
    std::sort(v.begin(), v.end());
    const double pos = q * (v.size() - 1);
    const size_t i   = static_cast<size_t>(pos);
    const double f   = pos - i;
    return i + 1 < v.size() ? v[i] * (1 - f) + v[i + 1] * f : v[i];
}

static std::string Quants(const std::vector<double>& v)
{
    Json j;
    j.Num("p10", Quantile(v, 0.1));
    j.Num("p25", Quantile(v, 0.25));
    j.Num("p50", Quantile(v, 0.5));
    j.Num("p75", Quantile(v, 0.75));
    j.Num("p90", Quantile(v, 0.9));
    j.Num("n", static_cast<double>(v.size()));
    return j.Str();
}

//
// Options
//
struct Options {
    std::string              mode;
    std::vector<std::string> data;
    std::string              overrideFile;
    std::string              offsets;
    std::string              out;
    uint64_t                 seed   = 1;
    int                      stride = 1;
    int                      maxEvents = 1000000;
};

static StyleOffsets ParseOffsets(const std::string& spec, const SharedModel& sh)
{
    StyleOffsets o;
    o.aimHeightFiring = sh.view.aimHeightFiring;
    std::stringstream ss(spec);
    std::string       kv;
    while (std::getline(ss, kv, ',')) {
        const size_t eq = kv.find('=');
        if (eq == std::string::npos) {
            continue;
        }
        const std::string k = kv.substr(0, eq);
        const float       v = static_cast<float>(std::atof(kv.c_str() + eq + 1));
        if (k == "diag") o.diagLogit = v;
        else if (k == "reverse") o.reverseLogit = v;
        else if (k == "hold") o.holdScale = v;
        else if (k == "lean") o.leanLogit = v;
        else if (k == "jump") o.jumpMult = v;
        else if (k == "crouch") o.crouchMult = v;
        else if (k == "walk") o.walkMult = v;
        else if (k == "release") o.releaseLogit = v;
        else if (k == "height") o.aimHeightFiring = v;
        else if (k == "noise") o.noiseScale = v;
        else if (k == "detect") o.detectMult = v;
    }
    return o;
}

//
// Movement: chord, lean and stance processes
//
struct MoveSeries {
    std::vector<int>  chord, lean;
    std::vector<char> crouch, jump, walk;
};

static void MoveStats(const std::vector<const Replay *>& data, const std::vector<MoveSeries>& series, int stride, const char *who,
                      Json& out)
{
    // chord share by context, holds, reversals, hazard by age, lean, stance
    double cnt[CTX_COUNT][NUM_CHORDS] = {};
    double revN[CTX_COUNT] = {}, revY[CTX_COUNT] = {};
    double hazN[12] = {}, hazY[12] = {};
    double leanN[CTX_COUNT] = {}, leanY[CTX_COUNT] = {};
    double agreeN = 0, agreeY = 0;
    double crouchPress = 0, jumpPress = 0, minutes = 0, walkHid = 0, hidN = 0;
    std::vector<double> holdsAll, holdsFire, fwdHolds, fwdGaps;
    double              fwdN[CTX_COUNT] = {}, fwdY[CTX_COUNT][3] = {};
    double              fhN[3][12] = {}, fhY[3][12] = {};  // forward-key change hazard by from-state and age
    double              fxN[CTX_COUNT][3][12] = {}, fxY[CTX_COUNT][3][12] = {}, fxT[CTX_COUNT][3][3] = {};
    double              fcN[CTX_COUNT][6] = {}, fcY[CTX_COUNT][6] = {};  // forward share by ticks since the context began
    double              shN[CTX_COUNT][2][12] = {}, shY[CTX_COUNT][2][12] = {};  // side-key change hazard [ctx][strafing][age]
    double              ssN[CTX_COUNT][2][6] = {};  // side-key ticks [ctx][strafing][run age bin], censored runs included
    for (size_t f = 0; f < data.size(); f++) {
        const Replay&     r   = *data[f];
        const MoveSeries& s   = series[f];
        const float      *ctx = r.C("ctx_i");
        const float      *el  = r.C("eligible");
        for (const Seg& g : Segments(r, stride)) {
            int runStart = g.a, fwdStart = g.a, ctxStart = g.a, sideStart = g.a;
            for (int t = g.a; t < g.b; t++) {
                const int fwd = ChordFwd(s.chord[t]);
                if (t > g.a && ctx[t] != ctx[t - 1]) {
                    ctxStart = t;
                }
                if (t > g.a && ChordSide(s.chord[t]) != ChordSide(s.chord[t - 1])) {
                    sideStart = t;
                }
                if (el[t] > 0.5f) {
                    const int age = t - sideStart + 1;
                    const int ab  = age <= 5 ? 0 : (age <= 10 ? 1 : (age <= 20 ? 2 : (age <= 40 ? 3 : (age <= 80 ? 4 : 5))));
                    ssN[static_cast<int>(ctx[t])][ChordSide(s.chord[t]) != 0][ab]++;
                }
                if (el[t] > 0.5f && t + 1 < g.b && g.Known(sideStart)) {
                    const int age = t - sideStart + 1;
                    const int ab  = age <= 8 ? age - 1 : (age <= 12 ? 8 : (age <= 20 ? 9 : (age <= 40 ? 10 : 11)));
                    const int sd  = ChordSide(s.chord[t]);
                    const int cc  = static_cast<int>(ctx[t]);
                    shN[cc][sd != 0][ab]++;
                    shY[cc][sd != 0][ab] += ChordSide(s.chord[t + 1]) != sd;
                }
                if (t > g.a && fwd != ChordFwd(s.chord[t - 1])) {
                    const int prevFwd = ChordFwd(s.chord[t - 1]);
                    if (g.Known(fwdStart)) {
                        bool allEl = true;
                        for (int k = fwdStart; k < t; k++) {
                            allEl = allEl && el[k] > 0.5f;
                        }
                        if (allEl && prevFwd == 1) {
                            fwdHolds.push_back(50.0 * (t - fwdStart));
                        } else if (allEl && prevFwd == 0) {
                            fwdGaps.push_back(50.0 * (t - fwdStart));
                        }
                    }
                    fwdStart = t;
                }
                const int side = ChordSide(s.chord[t]);
                if (t > g.a && side != ChordSide(s.chord[t - 1])) {
                    // a side run [runStart, t) ended; complete only if it did not start at the segment edge
                    const int prevSide = ChordSide(s.chord[t - 1]);
                    if (prevSide != 0 && g.Known(runStart)) {
                        bool allEl = true;
                        int  nf    = 0;
                        for (int k = runStart; k < t; k++) {
                            allEl = allEl && el[k] > 0.5f;
                            nf += ctx[k] == CTX_LOS_FIRE;
                        }
                        if (allEl) {
                            holdsAll.push_back(50.0 * (t - runStart));
                            if (nf * 2 > t - runStart) {
                                holdsFire.push_back(50.0 * (t - runStart));
                            }
                        }
                    }
                    runStart = t;
                }
                if (el[t] < 0.5f) {
                    continue;
                }
                const int c = static_cast<int>(ctx[t]);
                cnt[c][s.chord[t]]++;
                fwdN[c]++;
                fwdY[c][fwd + 1]++;
                if (g.Known(ctxStart)) {
                    const int ca = t - ctxStart + 1;
                    const int cb = ca <= 2 ? 0 : (ca <= 5 ? 1 : (ca <= 10 ? 2 : (ca <= 20 ? 3 : (ca <= 40 ? 4 : 5))));
                    fcN[c][cb]++;
                    fcY[c][cb] += fwd == 1;
                }
                if (t + 1 < g.b && g.Known(fwdStart)) {
                    const int age = t - fwdStart + 1;
                    const int ab  = age <= 8 ? age - 1 : (age <= 12 ? 8 : (age <= 20 ? 9 : (age <= 40 ? 10 : 11)));
                    fhN[fwd + 1][ab]++;
                    fhY[fwd + 1][ab] += ChordFwd(s.chord[t + 1]) != fwd;
                    if (el[t] > 0.5f) {
                        const int cc = static_cast<int>(ctx[t]);
                        fxN[cc][fwd + 1][ab]++;
                        fxY[cc][fwd + 1][ab] += ChordFwd(s.chord[t + 1]) != fwd;
                        fxT[cc][fwd + 1][ChordFwd(s.chord[t + 1]) + 1]++;
                    }
                }
                minutes += 1.0 / 1200.0;
                leanN[c]++;
                leanY[c] += s.lean[t] != 0;
                if (s.lean[t] != 0 && side != 0) {
                    agreeN++;
                    agreeY += s.lean[t] == side;
                }
                if (c == CTX_HIDDEN_NOFIRE) {
                    hidN++;
                    walkHid += s.walk[t];
                }
                if (t > g.a) {
                    crouchPress += s.crouch[t] && !s.crouch[t - 1];
                    jumpPress += s.jump[t] && !s.jump[t - 1];
                }
                if (t + 1 < g.b && side != 0) {
                    const int nside = ChordSide(s.chord[t + 1]);
                    if (nside != side) {
                        revN[c]++;
                        revY[c] += nside == -side;
                    }
                    if (c == CTX_LOS_FIRE) {
                        // age of the current side (ticks), from the start of its run
                        int age = 1;
                        for (int k = t - 1; k >= g.a && ChordSide(s.chord[k]) == side; k--) {
                            age++;
                        }
                        const bool known = g.Known(t - age + 1);
                        if (known) {
                            const int ab = std::min(age, 12) - 1;
                            hazN[ab]++;
                            hazY[ab] += nside != side;
                        }
                    }
                }
            }
        }
    }
    Json j;
    std::ostringstream sh;
    sh << "{";
    for (int c = 0; c < CTX_COUNT; c++) {
        double tot = 0;
        for (int k = 0; k < NUM_CHORDS; k++) {
            tot += cnt[c][k];
        }
        std::vector<double> v;
        for (int k = 0; k < NUM_CHORDS; k++) {
            v.push_back(tot > 0 ? cnt[c][k] / tot : NAN);
        }
        sh << (c ? "," : "") << "\"" << c << "\":" << Arr(v);
    }
    sh << "}";
    j.Raw("chord_share_by_ctx", sh.str());
    std::vector<double> rev, lean, haz;
    double              revAllN = 0, revAllY = 0;
    for (int c = 0; c < CTX_COUNT; c++) {
        rev.push_back(revN[c] > 0 ? revY[c] / revN[c] : NAN);
        lean.push_back(leanN[c] > 0 ? leanY[c] / leanN[c] : NAN);
        revAllN += revN[c];
        revAllY += revY[c];
    }
    for (int a = 0; a < 12; a++) {
        haz.push_back(hazN[a] > 0 ? hazY[a] / hazN[a] : NAN);
    }
    j.Raw("reverse_share_by_ctx", Arr(rev));
    j.Num("reverse_share", revAllN > 0 ? revAllY / revAllN : NAN);
    j.Raw("lean_share_by_ctx", Arr(lean));
    j.Num("lean_strafe_agreement", agreeN > 0 ? agreeY / agreeN : NAN);
    j.Raw("side_switch_hazard_by_age_los_fire", Arr(haz));
    j.Raw("side_hold_ms", Quants(holdsAll));
    j.Raw("fwd_hold_ms", Quants(fwdHolds));
    j.Raw("fwd_none_ms", Quants(fwdGaps));
    std::vector<double> fwdShare;
    for (int c = 0; c < CTX_COUNT; c++) {
        fwdShare.push_back(fwdN[c] > 0 ? fwdY[c][2] / fwdN[c] : NAN);
    }
    j.Raw("fwd_share_by_ctx", Arr(fwdShare));
    std::ostringstream fcs;
    fcs << "{";
    for (int c = 0; c < CTX_COUNT; c++) {
        std::vector<double> v;
        for (int k = 0; k < 6; k++) {
            v.push_back(fcN[c][k] > 0 ? fcY[c][k] / fcN[c][k] : NAN);
        }
        fcs << (c ? "," : "") << "\"" << c << "\":" << Arr(v);
    }
    fcs << "}";
    j.Raw("fwd_share_by_ctx_age", fcs.str());
    std::ostringstream shs;
    shs << "{";
    for (int c = 0; c < CTX_COUNT; c++) {
        for (int k = 0; k < 2; k++) {
            std::vector<double> v;
            for (int a = 0; a < 12; a++) {
                v.push_back(shN[c][k][a] > 0 ? shY[c][k][a] / shN[c][k][a] : NAN);
            }
            shs << (c || k ? "," : "") << "\"" << c << (k ? "_strafe" : "_none") << "\":" << Arr(v);
        }
    }
    shs << "}";
    j.Raw("side_hazard_by_ctx_age", shs.str());
    std::ostringstream fxs;
    fxs << "{";
    for (int c = 0; c < CTX_COUNT; c++) {
        for (int k = 0; k < 3; k++) {
            std::vector<double> v;
            for (int a = 0; a < 12; a++) {
                v.push_back(fxN[c][k][a] > 0 ? fxY[c][k][a] / fxN[c][k][a] : NAN);
            }
            const double moved = fxT[c][k][0] + fxT[c][k][1] + fxT[c][k][2] - fxT[c][k][k];
            for (int to = 0; to < 3; to++) {
                v.push_back(moved > 0 && to != k ? fxT[c][k][to] / moved : NAN);
            }
            fxs << (c || k ? "," : "") << "\"" << c << "_from" << (k - 1) << "\":" << Arr(v);
        }
    }
    fxs << "}";
    j.Raw("fwd_hazard_by_ctx_age_then_dest", fxs.str());
    std::ostringstream sss;
    sss << "{";
    for (int c = 0; c < CTX_COUNT; c++) {
        double tot = 0;
        for (int k = 0; k < 2; k++) {
            for (int a = 0; a < 6; a++) {
                tot += ssN[c][k][a];
            }
        }
        for (int k = 0; k < 2; k++) {
            std::vector<double> v;
            for (int a = 0; a < 6; a++) {
                v.push_back(tot > 0 ? ssN[c][k][a] / tot : NAN);
            }
            sss << (c || k ? "," : "") << "\"" << c << (k ? "_strafe" : "_none") << "\":" << Arr(v);
        }
    }
    sss << "}";
    j.Raw("side_ticks_by_ctx_run_age", sss.str());
    for (int k = 0; k < 3; k++) {
        std::vector<double> hz;
        for (int a = 0; a < 12; a++) {
            hz.push_back(fhN[k][a] > 0 ? fhY[k][a] / fhN[k][a] : NAN);
        }
        j.Raw(k == 0 ? "fwd_hazard_from_back" : (k == 1 ? "fwd_hazard_from_none" : "fwd_hazard_from_fwd"), Arr(hz));
    }
    j.Raw("side_hold_ms_los_fire", Quants(holdsFire));
    double lf = 0, lfd = 0;
    for (int k = 0; k < NUM_CHORDS; k++) {
        lf += cnt[CTX_LOS_FIRE][k];
    }
    lfd = cnt[CTX_LOS_FIRE][6] + cnt[CTX_LOS_FIRE][8];
    j.Num("fwd_diag_fight", lf > 0 ? lfd / lf : NAN);
    j.Num("lean_fight", lean[CTX_LOS_FIRE]);
    j.Num("crouch_per_min", crouchPress / std::max(minutes, 1e-9));
    j.Num("jumps_per_min", jumpPress / std::max(minutes, 1e-9));
    j.Num("walk_hidden", hidN > 0 ? walkHid / hidN : NAN);
    j.Num("minutes", minutes);
    out.Raw(who, j.Str());
}

static void RunMovement(const std::vector<const Replay *>& data, const ModelBundle& b, const StyleOffsets& style, const Options& o, Json& out)
{
    std::vector<MoveSeries> bot(data.size()), hum(data.size());
    Rng                     root(o.seed);
    for (size_t f = 0; f < data.size(); f++) {
        const Replay& r = *data[f];
        MoveSeries&   s = bot[f];
        MoveSeries&   h = hum[f];
        s.chord.assign(r.nrows, CHORD_NEUTRAL);
        s.lean.assign(r.nrows, 0);
        s.crouch.assign(r.nrows, 0);
        s.jump.assign(r.nrows, 0);
        s.walk.assign(r.nrows, 0);
        h = s;
        const float *ctx = r.C("ctx_i"), *lage = r.C("lage"), *los = r.C("line_of_sight");
        const float *dist = r.C("distance_xy"), *bear = r.C("relative_bearing"), *orel = r.C("opp_reloading");
        const float *oal = r.C("opp_alive"), *act = r.C("action"), *ln = r.C("lean");
        const float *cr = r.C("crouch_key"), *jp = r.C("jump_key"), *run = r.C("run");
        const float *cl[9] = {r.C("clear_back_left"), r.C("clear_back"), r.C("clear_back_right"), r.C("clear_left"), nullptr,
                              r.C("clear_right"), r.C("clear_front_left"), r.C("clear_front"), r.C("clear_front_right")};
        const float *ground = r.C("on_ground"), *ducked = r.C("ducked");
        for (int i = 0; i < r.nrows; i++) {
            h.chord[i]  = static_cast<int>(act[i]);
            h.lean[i]   = static_cast<int>(ln[i]);
            h.crouch[i] = cr[i] > 0.5f;
            h.jump[i]   = jp[i] > 0.5f;
            h.walk[i]   = run[i] < 0.5f;
        }
        for (const Seg& g : Segments(r, o.stride)) {
            const int segIdx = g.index + 1;
            Mover mv;
            mv.Init(&b.shared.movement, &b.shared.spawn);
            Rng rm = root.Derive(1000 + segIdx * 2), rs = root.Derive(1001 + segIdx * 2);
            // start from what the human was doing (the first live tick of a life, or mid-life)
            s.chord[g.a] = h.chord[g.a];
            if (g.spawn) {
                MoveOutput first;
                mv.Spawn(h.chord[g.a], first);
            } else {
                mv.SetKeys(ChordFwd(h.chord[g.a]), ChordSide(h.chord[g.a]));
            }
            for (int t = g.a; t + 1 < g.b; t++) {
                MoveInput in;
                in.ctx        = ClampI(static_cast<int>(ctx[t]), 0, CTX_COUNT - 1);
                in.losChanged = lage[t] >= 0.0f && lage[t] <= 50.0f;
                if (oal[t] > 0.5f && Valid(dist[t]) && Valid(bear[t])) {
                    in.enemyKnown   = true;
                    in.enemyBearing = bear[t];
                    in.enemyDist    = dist[t];
                }
                in.enemyReloading = orel[t] > 0.5f && los[t] > 0.5f;
                for (int c = 0; c < 9; c++) {
                    in.clearance[c] = (cl[c] && Valid(cl[c][t])) ? cl[c][t] : 128.0f;
                }
                in.onGround = ground[t] > 0.5f;
                in.ducked   = ducked[t] > 0.5f;
                in.enemyDead = !(oal[t] > 0.5f);
                MoveOutput mo;
                mv.Step(in, style, rm, rs, mo);
                s.chord[t + 1]  = mo.chord;
                s.lean[t + 1]   = mo.lean;
                s.crouch[t + 1] = mo.crouch;
                s.jump[t + 1]   = mo.jump;
                s.walk[t + 1]   = mo.walk;
            }
        }
    }
    MoveStats(data, bot, o.stride, "bot", out);
    MoveStats(data, hum, o.stride, "human", out);
}

//
// Trigger
//
static void TriggerStats(const std::vector<const Replay *>& data, const std::vector<std::vector<char>>& att, int stride, const char *who,
                         Json& out)
{
    const double enEdges[] = {0, 0.5, 1, 1.5, 2, 3, 4, 6, 10, 20, 1000};
    double       enN[10] = {}, enY[10] = {};
    double       losN = 0, losY = 0, hidN = 0, hidY = 0;
    std::vector<double> holds, gaps, clean, bursts;
    double prefN = 0, prefY = 0;
    for (size_t f = 0; f < data.size(); f++) {
        const Replay& r  = *data[f];
        const auto&   a  = att[f];
        const float  *el = r.C("eligible"), *los = r.C("line_of_sight"), *rel = r.C("reloading"), *clip = r.C("clip_ammo");
        const float  *en = r.C("en");
        for (const Seg& g : Segments(r, stride)) {
            int runStart = g.a;
            for (int t = g.a; t < g.b; t++) {
                if (t > g.a && a[t] != a[t - 1]) {
                    if (g.Known(runStart)) {
                        (a[t - 1] ? holds : gaps).push_back(50.0 * (t - runStart));
                        if (a[t - 1]) {
                            bursts.push_back(std::ceil((t - runStart) / 2.0));
                        }
                    }
                    runStart = t;
                }
                if (el[t] < 0.5f || rel[t] > 0.5f || clip[t] <= 0.0f) {
                    continue;
                }
                if (los[t] > 0.5f) {
                    losN++;
                    losY += a[t];
                    if (Valid(en[t]) && en[t] >= 0.0f) {
                        int k = 0;
                        while (k + 1 < 10 && en[t] >= enEdges[k + 1]) {
                            k++;
                        }
                        enN[k]++;
                        enY[k] += a[t];
                    }
                } else {
                    hidN++;
                    hidY += a[t];
                }
                // sight gains: >= 10 hidden ticks before, >= 6 visible after
                if (t >= g.a + 10 && t + 6 <= g.b && los[t] > 0.5f && los[t - 1] < 0.5f) {
                    bool ok = true;
                    for (int k = t - 10; k < t; k++) {
                        ok = ok && los[k] < 0.5f;
                    }
                    for (int k = t; k < t + 6; k++) {
                        ok = ok && los[k] > 0.5f;
                    }
                    if (!ok) {
                        continue;
                    }
                    prefN++;
                    prefY += a[t - 1];
                    if (!a[t - 1] && Valid(en[t]) && en[t] > 2.0f) {
                        double first = NAN;
                        for (int k = t; k < std::min(g.b, t + 30) && los[k] > 0.5f; k++) {
                            if (a[k]) {
                                first = 50.0 * (k - t);
                                break;
                            }
                        }
                        if (std::isfinite(first)) {
                            clean.push_back(first);
                        }
                    }
                }
            }
        }
    }
    Json j;
    std::vector<double> pe;
    for (int k = 0; k < 10; k++) {
        pe.push_back(enN[k] > 0 ? enY[k] / enN[k] : NAN);
    }
    j.Raw("p_attack_by_err_halfwidths_los", Arr(pe));
    j.Num("p_attack_los", losN > 0 ? losY / losN : NAN);
    j.Num("p_attack_hidden", hidN > 0 ? hidY / hidN : NAN);
    j.Raw("attack_hold_ms", Quants(holds));
    j.Raw("attack_gap_ms", Quants(gaps));
    j.Raw("burst_shots_approx", Quants(bursts));
    j.Raw("clean_first_press_ms", Quants(clean));
    j.Num("prefire_share", prefN > 0 ? prefY / prefN : NAN);
    j.Num("tap_share", [&] {
        double n = 0, y = 0;
        for (double h : holds) {
            n++;
            y += h <= 100.0;
        }
        return n > 0 ? y / n : NAN;
    }());
    out.Raw(who, j.Str());
}

static void RunTrigger(const std::vector<const Replay *>& data, const ModelBundle& b, const StyleOffsets& style, const Options& o, Json& out)
{
    std::vector<std::vector<char>> bot(data.size()), hum(data.size());
    Rng                            root(o.seed);
    for (size_t f = 0; f < data.size(); f++) {
        const Replay& r = *data[f];
        bot[f].assign(r.nrows, 0);
        hum[f].assign(r.nrows, 0);
        // the trigger's sight is a body part on screen, its clock from when the parts came on screen (fit_trigger.py)
        const float *att = r.C("attack"), *los = r.C("vis"), *lage = r.C("vage"), *en = r.C("en");
        const float *yaw = r.C("aim_yaw_error"), *rel = r.C("reloading"), *clip = r.C("clip_ammo"), *ready = r.C("weapon_ready");
        const float *dmg = r.C("ev_dmg_taken"), *oal = r.C("opp_alive");
        for (int i = 0; i < r.nrows; i++) {
            hum[f][i] = att[i] > 0.5f;
        }
        for (const Seg& g : Segments(r, o.stride)) {
            const int segIdx = g.index + 1;
            Trigger tr;
            tr.Init(&b.shared.trigger);
            Rng rng = root.Derive(5000 + segIdx);
            for (int t = g.a; t + 1 < g.b; t++) {
                TriggerInput in;
                in.canFire       = rel[t] < 0.5f && clip[t] > 0.0f && ready[t] > 0.5f;
                in.los           = los[t] > 0.5f;
                in.lageMs        = lage[t] >= 0.0f ? static_cast<int>(lage[t]) : 5000;
                in.errHalfWidths = Valid(en[t]) && en[t] >= 0.0f ? en[t] : 100.0f;
                in.hiddenYawErr  = oal[t] > 0.5f && Valid(yaw[t]) ? std::fabs(yaw[t]) : 180.0f;
                in.damaged       = dmg[t] > 0.0f;
                in.releaseLogit  = style.releaseLogit;
                bot[f][t + 1]    = tr.Step(in, rng);
            }
        }
    }
    TriggerStats(data, bot, o.stride, "bot", out);
    TriggerStats(data, hum, o.stride, "human", out);
}

//
// View: sight acquisition and sustained tracking
//
static void RunView(const std::vector<const Replay *>& data, const ModelBundle& b, const StyleOffsets& style, const Options& o, bool tracking,
                    Json& out)
{
    const int              W = 30;
    std::vector<std::vector<double>> botErr(W), humErr(W);
    std::vector<double>              botClose, humClose, botPress, humPress;
    // tracking stats by distance bin
    const double dEdges[] = {0, 128, 192, 256, 384, 512, 768, 1200};
    std::vector<double> bErrD[7], hErrD[7], bOnD[7], hOnD[7], bHD[7], hHD[7];
    Rng root(o.seed);
    int events = 0;
    for (size_t f = 0; f < data.size(); f++) {
        const Replay& r   = *data[f];
        const float  *el  = r.C("eligible"), *los = r.C("line_of_sight"), *vy = r.C("view_yaw"), *vp = r.C("view_pitch");
        const float  *ey  = r.C("aim_yaw_error"), *ep = r.C("aim_pitch_error"), *et = r.C("aim_total_error");
        const float  *dxyz = r.C("distance_xyz"), *hw = r.C("tgt_half_w_deg"), *att = r.C("attack");
        const float  *vx = r.C("velocity_x"), *vyv = r.C("velocity_y"), *ox = r.C("opp_vx"), *oy = r.C("opp_vy");
        const float  *en = r.C("en"), *rel = r.C("reloading"), *clip = r.C("clip_ammo"), *hf = r.C("aim_height_fraction");
        const float  *onb = r.C("crosshair_on_opponent"), *ez = r.C("eye_z"), *oz = r.C("origin_z"), *opz = r.C("opp_z");
        for (const Seg& g : Segments(r, o.stride)) {
            for (int i = g.a + 10; i + 6 <= g.b && events < o.maxEvents; i++) {
                if (!(los[i] > 0.5f && el[i] > 0.5f)) {
                    continue;
                }
                int start;
                if (!tracking) {
                    // sight gain after >= 500 ms occluded, >= 300 ms visible
                    if (los[i - 1] > 0.5f) {
                        continue;
                    }
                    bool ok = true;
                    for (int k = i - 10; k < i; k++) {
                        ok = ok && los[k] < 0.5f;
                    }
                    for (int k = i; k < i + 6; k++) {
                        ok = ok && los[k] > 0.5f;
                    }
                    if (!ok) {
                        continue;
                    }
                    start = i;
                } else {
                    // the 7th tick of a LOS run of >= 12 ticks
                    if (i < g.a + 6) {
                        continue;
                    }
                    bool ok = true;
                    for (int k = i - 6; k <= i; k++) {
                        ok = ok && los[k] > 0.5f;
                    }
                    if (!ok || los[i - 7] > 0.5f || i + 6 > g.b) {
                        continue;
                    }
                    start = i;
                }
                events++;
                ViewControl vc;
                vc.Init(&b.shared.view);
                Trigger tr;
                tr.Init(&b.shared.trigger);
                Rng       rng  = root.Derive(9000 + events);
                Rng       rngT = root.Derive(9001 + events);
                SelfState self;
                self.alive     = true;
                self.eye       = Vec3(0, 0, 0);
                self.origin    = Vec3(0, 0, -82);
                self.viewYaw   = vy[start];
                self.viewPitch = vp[start];
                vc.Reset(self);
                bool detected  = false;
                int  detTick   = -1;
                bool closeDone = false, pressDone = false;
                bool botPrevAttack = att[start - 1] > 0.5f;
                const bool clean   = !tracking && att[start - 1] < 0.5f && Valid(en[start]) && en[start] > 2.0f;
                bool humClosed = false, humPressed = false;
                const int end = tracking ? std::min(g.b, start + 60) : std::min(g.b, start + W);
                for (int k = start; k < end && los[k] > 0.5f; k++) {
                    const int   rel_ = k - start;
                    const float tyaw = vy[k] + ey[k];
                    const float tpit = vp[k] + ep[k];
                    const float d    = Valid(dxyz[k]) ? std::max(24.0f, dxyz[k]) : 400.0f;
                    const Vec3  cen  = AnglesForward(tpit, tyaw) * d;
                    const float eyeH = Valid(ez[k]) && Valid(oz[k]) ? ez[k] - oz[k] : 82.0f;
                    // feet of the target: centroid minus half its height (94 standing)
                    const Vec3 feet = cen - Vec3(0, 0, 47.0f);
                    self.velocity   = Vec3(vx[k], vyv[k], 0);
                    self.timeMs     = rel_ * 50;
                    (void)eyeH;
                    (void)opz;
                    // bot error before this tick's move
                    const Vec3  dv   = cen - self.eye;
                    const Vec3  fw   = AnglesForward(self.viewPitch, self.viewYaw);
                    const float berr = std::acos(Clamp(fw.dot(dv) / dv.length(), -1.0f, 1.0f)) * RAD2DEG;
                    const float half = std::atan(15.0f / d) * RAD2DEG;
                    if (!tracking) {
                        botErr[rel_].push_back(berr);
                        humErr[rel_].push_back(et[k]);
                        if (!closeDone && berr <= half + 1.5f) {
                            botClose.push_back(rel_ * 50.0);
                            closeDone = true;
                        }
                        if (!humClosed && et[k] <= hw[k] + 1.5f) {
                            humClose.push_back(rel_ * 50.0);
                            humClosed = true;
                        }
                    }
                    // detection
                    const float byaw = std::fabs(Wrap180(YawOf(dv) - self.viewYaw));
                    const float bpit = std::fabs(PitchOf(dv) - self.viewPitch);
                    const bool  inFov = byaw <= 48.2f && bpit <= 32.2f;
                    if (tracking) {
                        detected = true;
                    } else if (!detected) {
                        const float pd = Perceiver::DetectProb(b.shared.perception, berr, d, NUM_PARTS, style.detectMult, inFov);
                        if (rng.Uniform() < pd) {
                            detected = true;
                            detTick  = rel_;
                        }
                    }
                    // trigger (bot's own, on its own error) for acquisition; the human's while tracking
                    bool firing;
                    if (tracking) {
                        firing = att[k - 1] > 0.5f;
                    } else {
                        TriggerInput ti;
                        ti.canFire       = rel[k] < 0.5f && clip[k] > 0.0f;
                        ti.los           = inFov;
                        ti.lageMs        = rel_ * 50;
                        ti.errHalfWidths = berr / std::max(0.05f, half);
                        ti.releaseLogit  = style.releaseLogit;
                        const bool a     = tr.Step(ti, rngT);
                        firing           = botPrevAttack;
                        botPrevAttack    = a;
                        if (clean && !pressDone && a) {
                            botPress.push_back(rel_ * 50.0);
                            pressDone = true;
                        }
                        if (clean && !humPressed && att[k] > 0.5f) {
                            humPress.push_back(rel_ * 50.0);
                            humPressed = true;
                        }
                    }
                    if (tracking && firing) {
                        int db = 0;
                        while (db + 1 < 7 && d >= dEdges[db + 1]) {
                            db++;
                        }
                        // the bot's crosshair on the target's body box (15 half width, 94 tall)
                        const float yawOff = std::fabs(Wrap180(YawOf(dv) - self.viewYaw));
                        const float dxy    = dv.lengthXY();
                        const float rayZ   = -std::tan(self.viewPitch * DEG2RAD) * dxy;
                        const float hfrac  = (rayZ - feet.z) / 94.0f;
                        const bool  on     = yawOff <= half && hfrac >= 0.0f && hfrac <= 1.0f;
                        bErrD[db].push_back(berr);
                        hErrD[db].push_back(et[k]);
                        bOnD[db].push_back(on);
                        hOnD[db].push_back(onb[k] > 0.5f);
                        bHD[db].push_back(hfrac);
                        if (Valid(hf[k])) {
                            hHD[db].push_back(hf[k]);
                        }
                    }
                    ViewInput vi;
                    vi.ctx         = firing ? CTX_LOS_FIRE : CTX_LOS_NOFIRE;
                    vi.firing      = firing;
                    vi.track       = detected;
                    vi.detected    = detected;
                    vi.acquisition = detected && (tracking ? false : rel_ - detTick < 10);
                    vi.enemyFeet   = feet;
                    vi.enemyVel    = Vec3(ox[k], oy[k], 0);
                    vi.aimHeightFiring = style.aimHeightFiring;
                    vi.noiseScale  = style.noiseScale;
                    ViewOutput vo;
                    if (detected) {
                        vc.Step(self, vi, rng, vo);
                        self.viewYaw   = Wrap180(self.viewYaw + vo.yawDelta);
                        self.viewPitch = Clamp(self.viewPitch + vo.pitchDelta, -85.0f, 85.0f);
                    }
                }
            }
        }
    }
    Json jb, jh;
    if (!tracking) {
        std::vector<double> mb, mh;
        for (int k = 0; k < W; k++) {
            mb.push_back(Quantile(botErr[k], 0.5));
            mh.push_back(Quantile(humErr[k], 0.5));
        }
        jb.Raw("median_error_curve_deg", Arr(mb));
        jh.Raw("median_error_curve_deg", Arr(mh));
        jb.Raw("t_close_ms", Quants(botClose));
        jh.Raw("t_close_ms", Quants(humClose));
        jb.Raw("clean_first_press_ms", Quants(botPress));
        jh.Raw("clean_first_press_ms", Quants(humPress));
    } else {
        std::vector<double> e1, e2, o1, o2, h1, h2;
        for (int k = 0; k < 7; k++) {
            e1.push_back(Quantile(bErrD[k], 0.5));
            e2.push_back(Quantile(hErrD[k], 0.5));
            double s1 = 0, s2 = 0;
            for (double v : bOnD[k]) {
                s1 += v;
            }
            for (double v : hOnD[k]) {
                s2 += v;
            }
            o1.push_back(bOnD[k].empty() ? NAN : s1 / bOnD[k].size());
            o2.push_back(hOnD[k].empty() ? NAN : s2 / hOnD[k].size());
            h1.push_back(Quantile(bHD[k], 0.5));
            h2.push_back(Quantile(hHD[k], 0.5));
        }
        jb.Raw("firing_err_med_by_distance", Arr(e1));
        jh.Raw("firing_err_med_by_distance", Arr(e2));
        jb.Raw("firing_on_body_by_distance", Arr(o1));
        jh.Raw("firing_on_body_by_distance", Arr(o2));
        jb.Raw("firing_height_frac_by_distance", Arr(h1));
        jh.Raw("firing_height_frac_by_distance", Arr(h2));
        std::vector<double> allb, allh;
        for (int k = 0; k < 7; k++) {
            allb.insert(allb.end(), bErrD[k].begin(), bErrD[k].end());
            allh.insert(allh.end(), hErrD[k].begin(), hErrD[k].end());
        }
        jb.Num("firing_err_med", Quantile(allb, 0.5));
        jh.Num("firing_err_med", Quantile(allh, 0.5));
    }
    jb.Num("events", events);
    out.Raw("bot", jb.Str());
    out.Raw("human", jh.Str());
}

//
// Hidden: belief + look policy along the human's own path
//
static void RunHidden(const std::vector<const Replay *>& data, const ModelBundle& b, const StyleOffsets& style, const Options& o, Json& out)
{
    const double sEdges[] = {-1, 250, 500, 1000, 2000, 4000, 8000, 1e8};
    std::vector<double> bE[8], hE[8], belE[8], expE[8];
    double hidTicks = 0, botTurns = 0, humTurns = 0, botStill = 0, humStill = 0;
    double modeN[9] = {};  // hidden look modes of the bot
    Rng    root(o.seed);
    for (size_t f = 0; f < data.size(); f++) {
        const Replay& r = *data[f];
        MapPrior      m;
        std::string   err;
        if (!LoadMapPrior(EmbeddedText("maps/" + r.map + ".json"), "", m, err)) {
            std::fprintf(stderr, "no prior for %s: %s\n", r.map.c_str(), err.c_str());
            continue;
        }
        const float *el = r.C("eligible"), *los = r.C("line_of_sight"), *vy = r.C("view_yaw"), *vp = r.C("view_pitch");
        const float *ey = r.C("aim_yaw_error"), *ox = r.C("origin_x"), *oy = r.C("origin_y"), *oz = r.C("origin_z");
        const float *ez = r.C("eye_z"), *vx = r.C("velocity_x"), *vyv = r.C("velocity_y"), *px = r.C("opp_x"), *py = r.C("opp_y");
        const float *pz = r.C("opp_z"), *pvx = r.C("opp_vx"), *pvy = r.C("opp_vy"), *oal = r.C("opp_alive");
        const float *osh = r.C("opp_shot"), *orl = r.C("opp_reload_ev"), *osp = r.C("opp_speed"), *owk = r.C("opp_walk");
        const float *odk = r.C("opp_ducked"), *dmg = r.C("ev_dmg_taken"), *kill = r.C("ev_kill"), *att = r.C("attack");
        const float *yd = r.C("yaw_d");
        for (const Seg& g : Segments(r, o.stride)) {
            const int segIdx = g.index + 1;
            if (g.b - g.a < 20) {
                continue;
            }
            Perceiver    pc;
            BeliefFilter bf;
            ViewControl  vc;
            pc.Init(&b.shared.perception, root.Derive(20000 + segIdx));
            bf.Init(&b.shared.belief, &b.shared.perception, &m, root.Derive(30000 + segIdx));
            vc.Init(&b.shared.view);
            Rng       rv = root.Derive(40000 + segIdx);
            SelfState self;
            self.alive     = true;
            self.spectator = false;
            self.viewYaw   = vy[g.a];
            self.viewPitch = vp[g.a];
            vc.Reset(self);
            int   lastStep = -100, lastSeen = -1000000;
            std::vector<float> botYaw;
            for (int t = g.a; t < g.b; t++) {
                RawInput raw;
                self.timeMs   = (t - g.a) * 50;
                self.origin   = Vec3(ox[t], oy[t], oz[t]);
                self.eye      = Vec3(ox[t], oy[t], Valid(ez[t]) ? ez[t] : oz[t] + 82.0f);
                self.velocity = Vec3(vx[t], vyv[t], 0);
                raw.self      = self;
                const bool alive = oal[t] > 0.5f && Valid(px[t]);
                RawEnemy   e;
                e.id    = 1;
                e.alive = alive;
                const Vec3 cen = alive ? Vec3(px[t], py[t], pz[t] + 47.0f) : Vec3();
                if (alive && los[t] > 0.5f) {
                    const Vec3 dv   = cen - self.eye;
                    const bool inFov = std::fabs(Wrap180(YawOf(dv) - self.viewYaw)) <= 48.2f
                                    && std::fabs(PitchOf(dv) - self.viewPitch) <= 32.2f;
                    if (inFov) {
                        e.inFov       = true;
                        e.partMask    = (1 << NUM_PARTS) - 1;
                        e.centroid    = cen;
                        e.velocity    = Vec3(pvx[t], pvy[t], 0);
                        e.centroidLos = true;
                    }
                }
                raw.enemies.push_back(e);
                if (alive) {
                    if (osh[t] > 0.0f) {
                        raw.sounds.push_back(RawSound{SOUND_GUNFIRE, 1, cen});
                    }
                    if (orl[t] > 0.0f) {
                        raw.sounds.push_back(RawSound{SOUND_RELOAD, 1, cen});
                    }
                    if (osp[t] > 150.0f && owk[t] < 0.5f && odk[t] < 0.5f && t - lastStep >= 6) {
                        raw.sounds.push_back(RawSound{SOUND_FOOTSTEP, 1, cen - Vec3(0, 0, 47.0f)});
                        lastStep = t;
                    }
                    if (dmg[t] > 0.0f) {
                        RawDamage d;
                        d.attackerId  = 1;
                        d.attackerPos = cen;
                        d.damage      = dmg[t];
                        raw.damage.push_back(d);
                    }
                }
                if (kill[t] > 0.0f) {
                    raw.gotKillOf = 1;
                }
                Observation obs;
                pc.Process(raw, 96.4f, 64.4f, style.detectMult, obs);
                bf.Update(obs, 96.4f, 64.4f);
                if (bf.NumTracks() == 0) {
                    // the opponent has not been listed yet (dead at the start of this life)
                    botYaw.push_back(self.viewYaw);
                    continue;
                }
                const BeliefEstimate& be = bf.Track(0);
                ViewInput             vi;
                vi.ctx      = att[t] > 0.5f ? CTX_HIDDEN_FIRE : CTX_HIDDEN_NOFIRE;
                vi.firing   = att[t] > 0.5f;
                vi.detected = be.detected;
                vi.track    = be.detected || (!be.dead && be.seenThisLife && be.msSinceSeen < 350);
                if (be.detected) {
                    vi.enemyFeet = be.lastSeenPos;
                    vi.enemyVel  = be.lastSeenVel;
                } else if (vi.track) {
                    vi.enemyFeet = be.mode;
                    vi.enemyVel  = be.lastSeenVel;
                }
                vi.belief          = &be;
                vi.moving          = self.velocity.lengthXY() > 50.0f;
                vi.navValid        = vi.moving;
                vi.navYaw          = YawOf(self.velocity);
                vi.sounds          = &obs.sounds;
                vi.damage          = &obs.damage;
                vi.aimHeightFiring = style.aimHeightFiring;
                vi.noiseScale      = style.noiseScale;
                ViewOutput vo;
                vc.Step(self, vi, rv, vo);
                if (los[t] > 0.5f) {
                    lastSeen = t;
                }
                // stats on hidden duel ticks, before this tick's move
                if (el[t] > 0.5f && los[t] < 0.5f && alive && Valid(ey[t])) {
                    const float be_ = std::fabs(Wrap180(YawOf(cen - self.eye) - self.viewYaw));
                    const double since = lastSeen < g.a ? 1e9 : 50.0 * (t - lastSeen);
                    int          k     = 0;
                    while (k + 1 < 7 && since > sEdges[k + 1]) {
                        k++;
                    }
                    if (since >= 1e8) {
                        k = 7;
                    }
                    bE[k].push_back(be_);
                    // where the belief puts the enemy, and the most likely exposure, seen from here
                    if (be.valid && !be.dead) {
                        belE[k].push_back(std::fabs(Wrap180(YawOf(be.mode - self.origin) - YawOf(cen - self.eye))));
                        if (be.nExposure > 0) {
                            expE[k].push_back(std::fabs(Wrap180(YawOf(be.exposure[0] - self.eye) - YawOf(cen - self.eye))));
                        }
                    }
                    if (att[t] < 0.5f && vo.mode >= 0 && vo.mode < 9) {
                        modeN[vo.mode]++;
                    }
                    hE[k].push_back(std::fabs(ey[t]));
                    if (att[t] < 0.5f) {
                        hidTicks++;
                        botStill += vo.still;
                        humStill += std::fabs(yd[t]) < 0.01f;
                    }
                }
                botYaw.push_back(self.viewYaw);
                self.viewYaw   = Wrap180(self.viewYaw + vo.yawDelta);
                self.viewPitch = Clamp(self.viewPitch + vo.pitchDelta, -85.0f, 85.0f);
            }
            // large turns (>= 45 deg within 300 ms), counted on hidden non-firing ticks
            auto turns = [&](auto yawAt) {
                double n    = 0;
                bool   prev = false;
                for (int t = g.a + 6; t < g.b; t++) {
                    float sum = 0.0f;
                    for (int k = t - 5; k <= t; k++) {
                        sum += Wrap180(yawAt(k) - yawAt(k - 1));
                    }
                    const bool big = std::fabs(sum) >= 45.0f && los[t] < 0.5f && att[t] < 0.5f && el[t] > 0.5f;
                    n += big && !prev;
                    prev = big;
                }
                return n;
            };
            botTurns += turns([&](int k) { return botYaw[k - g.a]; });
            humTurns += turns([&](int k) { return vy[k]; });
        }
    }
    Json jb, jh;
    std::vector<double> mb, mh, nb;
    for (int k = 0; k < 8; k++) {
        mb.push_back(Quantile(bE[k], 0.5));
        mh.push_back(Quantile(hE[k], 0.5));
        nb.push_back(static_cast<double>(bE[k].size()));
    }
    jb.Raw("hidden_yaw_err_med_by_since_seen", Arr(mb));
    std::vector<double> mbel, mexp, modes;
    double              modeTot = 0;
    for (int k = 0; k < 8; k++) {
        mbel.push_back(Quantile(belE[k], 0.5));
        mexp.push_back(Quantile(expE[k], 0.5));
    }
    for (double v : modeN) {
        modeTot += v;
    }
    for (int k = 0; k < 9; k++) {
        modes.push_back(modeTot > 0 ? modeN[k] / modeTot : NAN);
    }
    jb.Raw("belief_mode_yaw_err_med_by_since_seen", Arr(mbel));
    jb.Raw("top_exposure_yaw_err_med_by_since_seen", Arr(mexp));
    jb.Raw("view_mode_share_hidden", Arr(modes));
    jh.Raw("hidden_yaw_err_med_by_since_seen", Arr(mh));
    jh.Raw("n_by_since_seen", Arr(nb));
    const double minutes = hidTicks / 1200.0;
    jb.Num("large_turns_per_min", botTurns / std::max(minutes, 1e-9));
    jh.Num("large_turns_per_min", humTurns / std::max(minutes, 1e-9));
    jb.Num("still_share_hidden", hidTicks > 0 ? botStill / hidTicks : NAN);
    jh.Num("still_share_hidden", hidTicks > 0 ? humStill / hidTicks : NAN);
    out.Raw("bot", jb.Str());
    out.Raw("human", jh.Str());
}

int main(int argc, char **argv)
{
    Options o;
    for (int i = 1; i < argc; i++) {
        const std::string a = argv[i];
        auto next = [&]() { return i + 1 < argc ? std::string(argv[++i]) : std::string(); };
        if (a == "--mode") o.mode = next();
        else if (a == "--data") o.data.push_back(next());
        else if (a == "--override") o.overrideFile = next();
        else if (a == "--offsets") o.offsets = next();
        else if (a == "--seed") o.seed = std::strtoull(next().c_str(), nullptr, 10);
        else if (a == "--stride") o.stride = std::max(1, std::atoi(next().c_str()));
        else if (a == "--max-events") o.maxEvents = std::atoi(next().c_str());
        else if (a == "--out") o.out = next();
    }
    std::map<std::string, std::string> ov;
    if (!o.overrideFile.empty()) {
        std::ifstream     f(o.overrideFile);
        std::stringstream ss;
        ss << f.rdbuf();
        ov["shared.json"] = ss.str();
    }
    ModelBundle b;
    std::string err;
    if (!LoadBundle(ov, b, err)) {
        std::fprintf(stderr, "model: %s\n", err.c_str());
        return 2;
    }
    const StyleOffsets style = ParseOffsets(o.offsets, b.shared);
    std::vector<Replay> reps;
    for (const std::string& d : o.data) {
        for (const char *mp : {"dm_crnodoors", "dm_main", "dm_vents", "dm_downladder"}) {
            Replay r;
            if (LoadHbr(d + "/" + mp + ".hbr", r)) {
                r.map = mp;
                reps.push_back(std::move(r));
            }
        }
    }
    if (reps.empty()) {
        std::fprintf(stderr, "no replay data (run humanbot/fit/export_replay.py)\n");
        return 2;
    }
    std::vector<const Replay *> data;
    for (const Replay& r : reps) {
        data.push_back(&r);
    }
    Json out;
    out.s << "\"mode\":\"" << o.mode << "\"";
    out.first = false;
    if (o.mode == "movement") {
        RunMovement(data, b, style, o, out);
    } else if (o.mode == "trigger") {
        RunTrigger(data, b, style, o, out);
    } else if (o.mode == "acquisition") {
        RunView(data, b, style, o, false, out);
    } else if (o.mode == "tracking") {
        RunView(data, b, style, o, true, out);
    } else if (o.mode == "hidden") {
        RunHidden(data, b, style, o, out);
    } else {
        std::fprintf(stderr, "unknown --mode %s\n", o.mode.c_str());
        return 2;
    }
    const std::string js = out.Str();
    if (o.out.empty()) {
        std::printf("%s\n", js.c_str());
    } else {
        std::ofstream(o.out) << js << "\n";
    }
    return 0;
}
