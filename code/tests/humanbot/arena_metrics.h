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
// arena_metrics.h: the statistics of humanbot/eval/metrics.py computed on arena
// frames, under the keys of human_reference.json.
//
// One Life is one segment in the metrics' sense: the rows of one bot from a
// spawn to its death. As in metrics.py, sequence features (next tick, hold
// ages, time since LOS changed) are computed on the whole life first and
// filtered by context afterwards; a complete run touches neither end of its
// life and has every row eligible (a living opponent). The acquisition
// statistics follow the data repo's acquisition.py and the style dials
// ("dial.*", "skill.*") humanbot/fit/fit_styles.py. "diag.*" keys are brain
// diagnostics with no human counterpart. The "perception.*" statistics are
// humanbot/eval/perception.py's (the data repo's perception_all.py): sightings
// timed from the first visible body part, the view while the enemy is hidden,
// and the corner it comes out from, traced through the arena's own geometry.

#pragma once

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace arena
{

const int NCTX = 5;
static const char *const CTX_NAMES[NCTX] = {"hidden_nofire", "hidden_fire", "los_nofire", "los_fire", "reload"};
enum { CTX_HIDDEN_NOFIRE, CTX_HIDDEN_FIRE, CTX_LOS_NOFIRE, CTX_LOS_FIRE, CTX_RELOAD };

struct P3 {
    float x = NAN, y = NAN, z = NAN;
};

struct Frame {
    int   t         = 0;      // ms
    bool  eligible  = false;  // alive with a living opponent (the duel mask)
    int   ctx       = 0;      // logger context; LOS is false without an opponent
    bool  los       = false;  // centroid ray from the unleaned eye to the nearest living enemy
    bool  attack    = false;  // attack key of the last usercmd
    bool  reloading = false;
    int   clip      = 0;
    int   chord     = 4;      // (fwd + 1) * 3 + (side + 1)
    int   side = 0, fwd = 0, lean = 0;
    bool  jump = false, crouch = false, run = true;
    float speed    = 0.0f;    // horizontal speed, u/s
    float approach = 0.0f;    // own horizontal velocity toward the opponent, u/s
    float yawD     = NAN;     // view yaw change since the previous row, deg (NaN on a life's first row)
    // opponent columns (eligible rows)
    float aimErr = NAN, aimYawErr = NAN, halfW = NAN, height = NAN, distXY = NAN, distXYZ = NAN;
    bool  onBody = false;
    // brain diagnostics
    int  viewMode  = -1;
    bool flick     = false, still = false, detected = false;
    int  navIntent = -1;
    float aimH = NAN, errPitch = NAN, errYaw = NAN;   // wanted aim height, view error to the controller's target
    float beliefErr = NAN;                            // belief mode to the true position (u), hidden rows
    // body-part perception (perception.py)
    int   visParts  = 0;      // the opponent's parts inside the frustum and unoccluded from the leaned eye
    bool  shot      = false;  // a round of ours left this tick
    bool  hit       = false;  // and hit the opponent
    float viewYaw = NAN, viewPitch = NAN;
    P3    eye, leye, cen;     // unleaned (logger) eye, leaned eye, the opponent's centroid
    P3    oppParts[5];        // the opponent's head, chest, belly, pelvis and feet
    float targetYaw = NAN, targetPitch = NAN;   // the view controller's target (diagnostics)
};

struct Shot {
    int  t        = 0;
    bool eligible = false;
    bool hit      = false;
};

struct Life {
    std::vector<Frame> rows;
    std::vector<Shot>  shots;
};

using Metrics = std::map<std::string, double>;

// Linear interpolation between order statistics (pandas' and numpy's default).
inline double Quantile(std::vector<double> v, double q)
{
    if (v.empty()) {
        return NAN;
    }
    std::sort(v.begin(), v.end());
    const double pos = q * static_cast<double>(v.size() - 1);
    const size_t i   = static_cast<size_t>(std::floor(pos));
    const double f   = pos - static_cast<double>(i);
    return i + 1 < v.size() ? v[i] * (1.0 - f) + v[i + 1] * f : v[i];
}

// pd.cut bin index: right-closed (e0, e1], ...; -1 outside or NaN
inline int Bin(double x, const std::vector<double>& e)
{
    if (!(x > e.front()) || x > e.back()) {
        return -1;
    }
    for (size_t k = 1; k < e.size(); k++) {
        if (x <= e[k]) {
            return static_cast<int>(k) - 1;
        }
    }
    return -1;
}

inline std::string G(double v)
{
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%g", v);
    return buf;
}

inline std::vector<std::string> EdgeLabels(const std::vector<double>& e)
{
    std::vector<std::string> out;
    for (size_t k = 0; k + 1 < e.size(); k++) {
        out.push_back(G(e[k]) + "-" + G(e[k + 1]));
    }
    return out;
}

struct Acc {
    std::map<std::string, std::pair<double, double>> ratio;
    std::map<std::string, std::vector<double>>       vals;

    void R(const std::string& k, double num, double den = 1.0)
    {
        std::pair<double, double>& r = ratio[k];
        r.first += num;
        r.second += den;
    }
    void V(const std::string& k, double v)
    {
        if (std::isfinite(v)) {
            vals[k].push_back(v);
        }
    }
};

struct Run {
    int  first    = 0;
    int  n        = 0;
    int  val      = 0;
    int  ctx      = 0;     // modal context, ties to the context seen first
    bool complete = true;
};

// Runs of one integer column over a life.
template<class Get>
std::vector<Run> Runs(const std::vector<Frame>& r, Get get)
{
    std::vector<Run> out;
    const int        n = static_cast<int>(r.size());
    int              i = 0;
    while (i < n) {
        const int v = get(r[i]);
        int       cnt[NCTX] = {};
        int       pos[NCTX];
        std::fill(pos, pos + NCTX, INT_MAX);
        bool el = true;
        int  j  = i;
        while (j < n && get(r[j]) == v) {
            el = el && r[j].eligible;
            cnt[r[j].ctx]++;
            pos[r[j].ctx] = std::min(pos[r[j].ctx], j);
            j++;
        }
        int best = 0;
        for (int c = 1; c < NCTX; c++) {
            if (cnt[c] > cnt[best] || (cnt[c] > 0 && cnt[c] == cnt[best] && pos[c] < pos[best])) {
                best = c;
            }
        }
        Run run;
        run.first    = i;
        run.n        = j - i;
        run.val      = v;
        run.ctx      = best;
        run.complete = el;
        out.push_back(run);
        i = j;
    }
    if (!out.empty()) {
        out.front().complete = false;
        out.back().complete  = false;
    }
    return out;
}

const double PERC_PI = 3.14159265358979323846;

// Sight through the arena's geometry, bodies ignored (perception.py traces the map only).
using SightFn = std::function<bool(const P3& from, const P3& to)>;

inline double Wrap180d(double a)
{
    a = std::fmod(a + 180.0, 360.0);
    if (a < 0.0) {
        a += 360.0;
    }
    return a - 180.0;
}

inline double NanMedian(std::vector<double> v)
{
    v.erase(std::remove_if(v.begin(), v.end(), [](double x) { return !std::isfinite(x); }), v.end());
    return Quantile(v, 0.5);
}

struct Basis {
    double f[3], l[3], u[3];
};

inline Basis ViewBasis(double pitchDeg, double yawDeg)
{
    const double p = pitchDeg * PERC_PI / 180.0, y = yawDeg * PERC_PI / 180.0;
    Basis        b = {{std::cos(p) * std::cos(y), std::cos(p) * std::sin(y), -std::sin(p)},
                      {-std::sin(y), std::cos(y), 0.0},
                      {std::sin(p) * std::cos(y), std::sin(p) * std::sin(y), std::cos(p)}};
    return b;
}

inline bool InFrustum(const P3& eye, const P3& q, const Basis& b, double tanH, double tanV)
{
    const double d[3] = {q.x - eye.x, q.y - eye.y, q.z - eye.z};
    const double x    = d[0] * b.f[0] + d[1] * b.f[1] + d[2] * b.f[2];
    return x > 1.0 && std::fabs(d[0] * b.l[0] + d[1] * b.l[1] + d[2] * b.l[2]) <= x * tanH
        && std::fabs(d[0] * b.u[0] + d[1] * b.u[1] + d[2] * b.u[2]) <= x * tanV;
}

inline P3 Lerp(const P3& a, const P3& b, double t)
{
    P3 o;
    o.x = static_cast<float>(a.x + t * (b.x - a.x));
    o.y = static_cast<float>(a.y + t * (b.y - a.y));
    o.z = static_cast<float>(a.z + t * (b.z - a.z));
    return o;
}

// total angle, yaw and pitch (deg) from the view of row f (leaned eye) to p; pitch + = p below the crosshair
inline void AnglesTo(const Frame& f, const P3& p, double& tot, double& yaw, double& pit)
{
    const Basis  b    = ViewBasis(f.viewPitch, f.viewYaw);
    const double d[3] = {p.x - f.leye.x, p.y - f.leye.y, p.z - f.leye.z};
    const double len  = std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    tot = len > 0 ? std::acos(std::max(-1.0, std::min(1.0, (d[0] * b.f[0] + d[1] * b.f[1] + d[2] * b.f[2]) / len))) * 180.0 / PERC_PI
                  : NAN;
    yaw = Wrap180d(std::atan2(d[1], d[0]) * 180.0 / PERC_PI - f.viewYaw);
    pit = std::atan2(-d[2], std::hypot(d[0], d[1])) * 180.0 / PERC_PI - f.viewPitch;
}

// humanbot/eval/perception.py on arena lives (one life = one segment). Values go into A under the same keys.
inline void Perception(const std::vector<Life>& lives, Acc& A, const SightFn& sight, double tanH, double tanV)
{
    const int PRE = 10, HOLD = 6, POST = 30, LOOK = 40;
    for (const Life& L : lives) {
        const std::vector<Frame>& r = L.rows;
        const int                 n = static_cast<int>(r.size());
        if (n < PRE + HOLD) {
            continue;
        }
        std::vector<char> el(n), los(n), vp(n), hid(n), hidC(n), att(n), close(n), on(n), hitv(n), shotv(n);
        for (int i = 0; i < n; i++) {
            const Frame& f = r[i];
            el[i]    = f.eligible;
            los[i]   = f.eligible && f.los;
            vp[i]    = f.eligible && f.visParts > 0;
            hid[i]   = el[i] && !los[i] && !vp[i];
            hidC[i]  = el[i] && !los[i];
            att[i]   = f.attack;
            close[i] = f.eligible && f.aimErr <= f.halfW + 1.5f;
            on[i]    = f.eligible && f.onBody;
            hitv[i]  = f.eligible && f.hit;
            shotv[i] = f.eligible && f.shot;
        }
        auto runEnd = [n](const std::vector<char>& c) {
            std::vector<int> o(n, 0);
            for (int i = 0; i < n; i++) {
                o[i] = c[i] ? (i > 0 ? o[i - 1] : 0) + 1 : 0;
            }
            return o;
        };
        auto runFrom = [n](const std::vector<char>& c) {
            std::vector<int> o(n, 0);
            for (int i = n - 1; i >= 0; i--) {
                o[i] = c[i] ? (i + 1 < n ? o[i + 1] : 0) + 1 : 0;
            }
            return o;
        };
        auto before = [&](const std::vector<char>& c) {
            std::vector<int> e = runEnd(c), o(n, 0);
            for (int i = 1; i < n; i++) {
                o[i] = e[i - 1];
            }
            return o;
        };
        auto err  = [&](int i) { return i >= 0 && i < n && el[i] ? static_cast<double>(r[i].aimErr) : NAN; };
        auto yerr = [&](int i) { return i >= 0 && i < n && el[i] ? static_cast<double>(r[i].aimYawErr) : NAN; };
        auto firstIn = [&](const std::vector<char>& c, int i, int bout) {
            for (int k = 0; k < std::min(POST, bout); k++) {
                if (i + k < n && c[i + k]) {
                    return k * 50.0;
                }
            }
            return static_cast<double>(NAN);
        };
        const std::vector<int> hidBefore = before(hid), vpFrom = runFrom(vp), losFrom = runFrom(los);
        const std::vector<int> hidCBefore = before(hidC), vpBefore = before(vp), hidFrom = runFrom(hid);
        std::vector<char>      novp(n);
        for (int i = 0; i < n; i++) {
            novp[i] = !vp[i];
        }
        const std::vector<int> novpEnd = runEnd(novp);
        // bursts: rounds exactly 100 ms apart, each counted from its first round
        int burstLen = 0, lastShot = -100;
        bool burstSight = false;
        auto endBurst = [&]() {
            if (burstLen > 0 && burstSight) {
                A.V("perception.burst_in_sight", burstLen);
            }
            burstLen = 0;
        };
        // visibility of the duel time and the shots
        for (int i = 0; i < n; i++) {
            if (!el[i]) {
                continue;
            }
            A.R("diag.perception.sightings_per_min", 0.0, 1.0 / 1200.0);
            if (r[i].viewMode == 2 && std::isfinite(r[i].targetYaw)) {
                A.V("diag.preaim.view_to_target", std::fabs(Wrap180d(r[i].targetYaw - r[i].viewYaw)));
                A.R("diag.preaim.flick", r[i].flick);
                A.R("diag.preaim.still", r[i].still);
                if (i > 0 && r[i - 1].viewMode == 2 && std::isfinite(r[i - 1].targetYaw)) {
                    A.R("diag.preaim.target_jump5", std::fabs(Wrap180d(r[i].targetYaw - r[i - 1].targetYaw)) > 5.0);
                }
                A.R("diag.preaim.entry", i > 0 && r[i - 1].viewMode != 2);
            }
            const bool cvis = r[i].los, pvis = r[i].visParts > 0;
            A.R("perception.part_only_share", pvis && !cvis);
            if (!pvis && !r[i].reloading && r[i].clip > 0) {
                A.R("perception.fire_held_no_part", r[i].attack);
                // since a part was last on screen in this life (perception.py NO_PART_SINCE)
                if (novpEnd[i] <= i) {
                    const double since = (novpEnd[i] - 1) * 50.0;
                    const char  *lab   = since < 500.0    ? "0-500ms"
                                       : since < 1000.0   ? "500-1000ms"
                                       : since < 2000.0   ? "1000-2000ms"
                                       : since < 5000.0   ? "2000-5000ms"
                                                          : "gt5000ms";
                    A.R(std::string("perception.fire_held_no_part.") + lab, r[i].attack);
                }
            }
            if (shotv[i]) {
                if (burstLen > 0 && lastShot == i - 2) {
                    burstLen++;
                } else {
                    endBurst();
                    burstLen   = 1;
                    burstSight = pvis;
                }
                lastShot = i;
            }
            if (r[i].shot) {
                if (cvis) {
                    A.R("perception.smg_hit.centre_visible", r[i].hit);
                } else if (pvis) {
                    A.R("perception.smg_hit.part_only", r[i].hit);
                }
            }
            // control: fully hidden ticks still hidden 500 ms later
            if (hid[i] && hidFrom[i] >= 11 && i + 10 < n) {
                const Frame& g  = r[i];
                const P3&    c  = r[i + 10].cen;
                const double fy = std::fabs(Wrap180d(std::atan2(c.y - g.eye.y, c.x - g.eye.x) * 180.0 / PERC_PI - g.viewYaw));
                A.R("perception.preaim.control_closer_to_future", fy < std::fabs(yerr(i)));
            }
        }
        endBurst();
        // encounters (perception.py encounters()): every run of a part on screen is a sighting; a hidden spell is a run
        // with none on screen between two of them; the first sight is timed from the start of a duel sequence (both
        // alive), never (1e6 ms) when it ended first; the travel is the net distance in 2 s from a tick with none on
        // screen (every 10th tick of the life; the unleaned eye is above the origin)
        {
            std::vector<char> hidP(n);
            for (int i = 0; i < n; i++) {
                hidP[i] = el[i] && !vp[i];
            }
            const std::vector<int> elFrom = runFrom(el), hidPFrom = runFrom(hidP);
            for (int i = 0; i < n; i++) {
                if (!el[i]) {
                    continue;
                }
                A.R("perception.encounter.part_on_screen", vp[i]);
                A.R("perception.encounter.sightings_per_min", vp[i] && !(i > 0 && vp[i - 1]), 1.0 / 1200.0);
                if (hidP[i] && i > 0 && vp[i - 1] && i + hidPFrom[i] < n && vp[i + hidPFrom[i]]) {
                    A.V("perception.encounter.hidden_spell", hidPFrom[i] * 50.0);
                }
                if (i == 0 || !el[i - 1]) {
                    double first = 1e6;
                    for (int k = 0; k < elFrom[i]; k++) {
                        if (vp[i + k]) {
                            first = k * 50.0;
                            break;
                        }
                    }
                    A.V("perception.encounter.first_sight", first);
                }
                if (hidP[i] && elFrom[i] > 40 && i % 10 == 0) {
                    A.V("perception.encounter.travel_2s_hidden",
                        std::hypot(r[i + 40].eye.x - r[i].eye.x, r[i + 40].eye.y - r[i].eye.y));
                }
            }
        }
        // centroid sightings: was a part on screen first?
        for (int i = 1; i < n; i++) {
            if (hidCBefore[i] >= PRE && los[i] && losFrom[i] >= HOLD) {
                const int vb = vpBefore[i];
                A.R("perception.centroid_sighting_part_first", vb > 0);
                if (vb > 0) {
                    A.V("perception.part_lead", vb * 50.0);
                }
            }
        }
        // part sightings
        for (int i = PRE; i < n; i++) {
            if (!(hidBefore[i] >= PRE && vp[i] && vpFrom[i] >= HOLD)) {
                continue;
            }
            const int  hb   = hidBefore[i];
            const int  bout = std::min(vpFrom[i], POST);
            A.R("diag.perception.sightings_per_min", 1.0, 0.0);
            {
                // the view's mode on the tick before the first part, and the error at the part by mode
                const std::string md = std::to_string(r[i - 1].viewMode);
                A.V("diag.perception.err0.mode." + md, err(i));
                A.V("diag.perception.yaw0.mode." + md, std::fabs(yerr(i)));
                {
                    // vertical: the view's pitch against the line to the centroid (+ = centroid below the crosshair)
                    const Frame& f0 = r[i];
                    const double dz = f0.cen.z - f0.eye.z, dh = std::hypot(f0.cen.x - f0.eye.x, f0.cen.y - f0.eye.y);
                    A.V("diag.perception.pitch0.mode." + md, -std::atan2(dz, dh) * 180.0 / PERC_PI - f0.viewPitch);
                }
                const Frame& g = r[i - 1];
                if (std::isfinite(g.targetYaw)) {
                    // where the view was headed vs where it was, and vs where the enemy showed
                    const P3& c  = r[i].cen;
                    const double cy = std::atan2(c.y - g.eye.y, c.x - g.eye.x) * 180.0 / PERC_PI;
                    A.V("diag.perception.view_to_target.mode." + md, std::fabs(Wrap180d(g.targetYaw - g.viewYaw)));
                    A.V("diag.perception.target_to_enemy.mode." + md, std::fabs(Wrap180d(g.targetYaw - cy)));
                }
                for (int mm : {0, 1, 2, 3, 4, 5, 6, 7, 8}) {
                    A.R("diag.perception.mode_share." + std::to_string(mm), r[i - 1].viewMode == mm);
                }
            }
            const bool pre  = att[i - 1];
            const double e0 = err(i);
            for (int t : {-500, -200, -100, 0, 100, 200}) {
                A.V("perception.error_from_part." + std::to_string(t) + "ms", err(i + t / 50));
            }
            A.R("perception.reaction.attack_before", pre);
            const bool clean = !pre && e0 > 2.0 * r[i].halfW;
            if (clean) {
                const double tp = firstIn(att, i, bout);
                A.V("perception.reaction.clean_first_press", tp);
                if (std::isfinite(tp)) {
                    A.R("perception.reaction.clean_first_press_mean", tp);
                }
                A.V("perception.reaction.clean_t_close", firstIn(close, i, bout));
            }
            const double th = firstIn(hitv, i, bout);
            A.V("perception.reaction.first_hit", th);
            A.R("perception.reaction.no_hit", !std::isfinite(th));
            // the hidden window: 500 ms before the first part (always inside the hidden run)
            const P3& ap     = r[i].cen;
            auto      appear = [&](int k) {
                const Frame& g = r[i + k];
                return std::fabs(Wrap180d(std::atan2(ap.y - g.eye.y, ap.x - g.eye.x) * 180.0 / PERC_PI - g.viewYaw));
            };
            const double e5 = yerr(i - 10), a5 = appear(-10);
            const double dv = Wrap180d(r[i].viewYaw - r[i - 10].viewYaw);
            const double sg = e5 > 0 ? 1.0 : (e5 < 0 ? -1.0 : 0.0);
            A.V("perception.preaim.yaw_to_enemy.m500", std::fabs(e5));
            A.V("perception.preaim.yaw_to_appearance.m500", a5);
            A.R("perception.preaim.parked.m500", a5 <= 5.0);
            A.R("perception.preaim.closer_to_appearance.m500", a5 < std::fabs(e5));
            A.V("perception.preaim.view_turn_toward", sg * dv);
            A.V("perception.preaim.view_turn_abs", std::fabs(dv));
            std::vector<double> sp;
            for (int k = -8; k <= -5; k++) {
                sp.push_back(r[i + k].speed);
            }
            A.V("perception.preaim.speed_m400_m200", NanMedian(sp));
            sp.clear();
            for (int k = -40; k <= -21; k++) {
                if (k >= -hb && i + k >= 0) {
                    sp.push_back(r[i + k].speed);
                }
            }
            A.V("perception.preaim.speed_m2000_m1000", NanMedian(sp));
            if (!sight) {
                continue;
            }
            // the corner: from the leaned eye at the first part, each part visible then traced back along the
            // enemy's last 500 ms to its last occluded tick; the crossing is bisected, the first part out marks
            // the exit, and the corner is where the ray to its occluded side first hits the geometry
            const Frame& f0 = r[i];
            const Basis  b0 = ViewBasis(f0.viewPitch, f0.viewYaw);
            double       bestCross = 1e9;
            P3           H;
            for (int j = 0; j < 5; j++) {
                const P3& q0 = f0.oppParts[j];
                if (!InFrustum(f0.leye, q0, b0, tanH, tanV) || !sight(f0.leye, q0)) {
                    continue;
                }
                int bstar = -1;
                for (int bk = 1; bk <= PRE; bk++) {
                    if (!sight(f0.leye, r[i - bk].oppParts[j])) {
                        bstar = bk;
                        break;
                    }
                }
                if (bstar < 0) {
                    continue;
                }
                const P3& a = r[i - bstar].oppParts[j];
                const P3& c = r[i - bstar + 1].oppParts[j];
                double    lo = 0.0, hi = 1.0;
                for (int it = 0; it < 14; it++) {
                    const double mid = 0.5 * (lo + hi);
                    if (sight(f0.leye, Lerp(a, c, mid))) {
                        hi = mid;
                    } else {
                        lo = mid;
                    }
                }
                const double cross = -bstar + 0.5 * (lo + hi);
                if (cross < bestCross) {
                    bestCross = cross;
                    H         = Lerp(a, c, lo);
                }
            }
            if (bestCross > 1e8) {
                A.R("diag.perception.corner_found", 0.0);
                continue;
            }
            double frac = 1.0;
            if (!sight(f0.leye, H)) {
                double lo = 0.0, hi = 1.0;
                for (int it = 0; it < 24; it++) {
                    const double mid = 0.5 * (lo + hi);
                    if (sight(f0.leye, Lerp(f0.leye, H, mid))) {
                        lo = mid;
                    } else {
                        hi = mid;
                    }
                }
                frac = hi;
            }
            const P3 K = Lerp(f0.leye, H, frac);
            A.R("diag.perception.corner_found", 1.0);
            A.V("perception.corner.distance", std::sqrt((K.x - f0.leye.x) * (K.x - f0.leye.x) + (K.y - f0.leye.y) * (K.y - f0.leye.y)
                                                        + (K.z - f0.leye.z) * (K.z - f0.leye.z)));
            for (int k : {-40, -20, -10, 0}) {
                if (k < -hb) {
                    continue;
                }
                const Frame& g = r[i + k];
                double       tK, yK, pK, tA, yA, pA, tE, yE, pE;
                AnglesTo(g, K, tK, yK, pK);
                AnglesTo(g, ap, tA, yA, pA);
                AnglesTo(g, g.cen, tE, yE, pE);
                if (hb >= LOOK) {
                    A.V("perception.corner_2s.yaw." + std::to_string(k * 50) + "ms", std::fabs(yK));
                }
                if (k == 0) {
                    A.V("perception.corner.angle.0ms", tK);
                }
                if (k != -10) {
                    continue;
                }
                const double side = Wrap180d(yA - yK) > 0 ? 1.0 : (Wrap180d(yA - yK) < 0 ? -1.0 : 0.0);
                const double lead = -yK * side;
                A.V("diag.perception.lead_m500.mode." + std::to_string(g.viewMode), lead);
                A.V("diag.perception.cornyaw_m500.mode." + std::to_string(g.viewMode), std::fabs(yK));
                A.R("diag.perception.m500_mode_share." + std::to_string(g.viewMode), 1.0);
                A.V("perception.corner.yaw.m500", std::fabs(yK));
                A.V("perception.corner.angle.m500", tK);
                A.V("perception.corner.pitch.m500", pK);
                A.V("perception.corner.lead.m500", lead);
                A.V("perception.corner.yaw_to_appearance.m500", std::fabs(yA));
                A.V("perception.corner.yaw_to_enemy.m500", std::fabs(yE));
                if (std::isfinite(tK)) {
                    A.R("perception.corner.within5.m500", tK <= 5.0);
                    A.R("perception.corner.closer_than_enemy.m500", std::fabs(yK) < std::fabs(yE));
                    A.R("perception.corner.cover_side.m500", lead < -2.0);
                    A.R("perception.corner.on_edge.m500", std::fabs(lead) <= 2.0);
                    A.R("perception.corner.open_side.m500", lead > 2.0);
                }
            }
        }
    }
}

inline Metrics Compute(const std::vector<Life>& lives, const SightFn& sight = nullptr, double tanH = 0.0, double tanV = 0.0)
{
    static const char *const GROUP_NAMES[5] = {"pure_strafe", "fwd_diag", "forward", "neutral", "back_any"};
    auto                     group = [](int chord) {
        switch (chord) {
        case 3:
        case 5:
            return 0;
        case 6:
        case 8:
            return 1;
        case 7:
            return 2;
        case 4:
            return 3;
        default:
            return 4;
        }
    };
    const std::vector<double> DIST_APPROACH = {0, 96, 160, 224, 288, 384, 512, 768, 1200};
    const std::vector<double> DIST_AIM      = {0, 128, 192, 256, 384, 512, 768, 1200};
    const std::vector<double> LAGE          = {-1, 0, 100, 250, 500, 1000, 1e6};
    const char *const         LAGE_LABELS[] = {"0", "50-100", "150-250", "300-500", "550-1000", "gt1000"};
    const std::vector<double> EN_RELEASE    = {0, 1, 2, 3, 4, 6, 10, 1000};
    const std::vector<double> EN_HOLD       = {0, 0.5, 1, 1.5, 2, 3, 4, 6, 10, 20, 1000};
    const std::vector<double> SINCE         = {-1, 250, 500, 1000, 2000, 4000, 8000, 1e8, 2e9};
    const char *const         SINCE_LABELS[] = {"0-250", "250-500", "500-1000", "1000-2000", "2000-4000", "4000-8000", "gt8000",
                                                "never"};
    const std::vector<std::string> approachLabels = EdgeLabels(DIST_APPROACH);
    const std::vector<std::string> aimLabels      = EdgeLabels(DIST_AIM);
    const std::vector<std::string> releaseLabels  = EdgeLabels(EN_RELEASE);
    const std::vector<std::string> holdLabels     = EdgeLabels(EN_HOLD);

    Acc       A;
    long long elig = 0, ctxN[NCTX] = {}, chordN[NCTX][5] = {};
    long long stillN[NCTX] = {}, leanN[NCTX] = {};
    long long jumpS = 0, crouchS = 0, walkS = 0, jumpFs = 0, crouchFs = 0;
    long long flickStarts = 0, largeTurns = 0, hiddenNoFire = 0;
    std::vector<double> yaw[NCTX], yawFree[NCTX];
    long long           mouseStill[NCTX] = {}, yawN[NCTX] = {};
    long long           flickN[NCTX] = {}, planStill[NCTX] = {}, detN[NCTX] = {};
    std::map<int, long long> modeN[NCTX], intentN[NCTX];
    std::vector<double>      aimH[NCTX], errPitch[NCTX], errYaw[NCTX];
    std::map<int, std::vector<double>> acqErrByMode;

    for (const Life& L : lives) {
        const std::vector<Frame>& r = L.rows;
        const int                 n = static_cast<int>(r.size());
        if (!n) {
            continue;
        }
        // sequence features on the whole life
        std::vector<int>    ageSide(n), lage(n);
        std::vector<double> since(n, NAN);
        int                 lastSeen = INT_MIN;
        for (int i = 0; i < n; i++) {
            ageSide[i] = i > 0 && r[i].side == r[i - 1].side ? ageSide[i - 1] + 50 : 50;
            lage[i]    = i > 0 && r[i].los == r[i - 1].los ? lage[i - 1] + 50 : 0;
            if (r[i].los) {
                lastSeen = r[i].t;
            }
            since[i] = lastSeen == INT_MIN ? NAN : static_cast<double>(r[i].t - lastSeen);
        }

        for (int i = 0; i < n; i++) {
            const Frame& f       = r[i];
            const bool   hasNext = i + 1 < n;
            const Frame *nx      = hasNext ? &r[i + 1] : nullptr;
            const bool   prevJump = i > 0 && r[i - 1].jump, prevCrouch = i > 0 && r[i - 1].crouch;
            const bool   prevWalk = i > 0 && !r[i - 1].run;
            const bool   prevFlick = i > 0 && r[i - 1].flick;
            if (!f.eligible) {
                continue;
            }
            const int         c  = f.ctx;
            const std::string cn = CTX_NAMES[c];
            elig++;
            ctxN[c]++;
            chordN[c][group(f.chord)]++;
            stillN[c] += f.speed < 5.0f;
            leanN[c] += f.lean != 0;
            flickN[c] += f.flick;
            planStill[c] += f.still;
            detN[c] += f.detected;
            modeN[c][f.viewMode]++;
            if (std::isfinite(f.aimH) && std::isfinite(f.height) && f.los) {
                A.V("diag." + cn + ".hdiff", f.height - f.aimH);
                if (!f.flick && f.detected) {
                    A.V("diag." + cn + ".hdiff_settled", f.height - f.aimH);
                }
            }
            if (std::isfinite(f.aimH)) {
                aimH[c].push_back(f.aimH);
                errPitch[c].push_back(f.errPitch);
                errYaw[c].push_back(std::fabs(f.errYaw));
            }
            intentN[c][f.navIntent]++;
            flickStarts += f.flick && !prevFlick;
            hiddenNoFire += !f.los && !f.attack;
            jumpS += f.jump && !prevJump;
            crouchS += f.crouch && !prevCrouch;
            walkS += !f.run && !prevWalk;
            jumpFs += f.jump && !prevJump && i > 0;
            crouchFs += f.crouch && !prevCrouch && i > 0;

            // a strafe let go, then a strafe the very next tick (the counter-strafe habit); pauses cut by the life's
            // end are left out
            if (f.side != 0 && hasNext && nx->side == 0) {
                int j = i + 2;
                while (j < n && r[j].side == 0) {
                    j++;
                }
                if (j < n) {
                    A.R("dial.counter_strafe", j == i + 2);
                }
            }
            // how strafes end (context of the tick before the change) and the switch hazard
            if (f.side != 0 && hasNext) {
                if (nx->side != f.side) {
                    A.R("movement.strafe_reverse." + cn, nx->side == -f.side);
                    A.R("dial.reverse_share", nx->side == -f.side);
                }
                if (c == CTX_LOS_FIRE) {
                    const int age = std::min(ageSide[i], 1500);
                    std::string lab;
                    if (age <= 250) {
                        lab = G(age);
                    } else if (age <= 450) {
                        lab = "300-450";
                    } else if (age <= 1000) {
                        lab = "500-1000";
                    } else {
                        lab = "1050-1500";
                    }
                    A.R("movement.switch_hazard." + lab + "ms", nx->side != f.side);
                }
            }
            if (c == CTX_LOS_FIRE) {
                const int b = Bin(f.distXY, DIST_APPROACH);
                if (b >= 0) {
                    A.R("movement.approach.los_fire." + approachLabels[b], f.approach > 40.0f);
                    A.R("movement.retreat.los_fire." + approachLabels[b], f.approach < -40.0f);
                }
                A.V("dial.aim_height_firing", f.height);
                A.V("skill.aim_error_fight_deg", f.aimErr);
            }
            if (c == CTX_HIDDEN_NOFIRE) {
                A.R("movement.walk.hidden_nofire", !f.run);
            }

            // trigger
            const double en = f.aimErr / f.halfW;
            if (!f.reloading && f.clip > 0 && hasNext) {
                if (!f.attack && f.los && en > 0.0 && en <= 3.0) {
                    const int b = Bin(lage[i], LAGE);
                    if (b >= 0) {
                        A.R(std::string("trigger.press_los_near.") + LAGE_LABELS[b], nx->attack);
                    }
                } else if (!f.attack && !f.los) {
                    const int b = Bin(lage[i], LAGE);
                    if (b >= 0) {
                        A.R(std::string("trigger.press_hidden.") + LAGE_LABELS[b], nx->attack);
                    }
                } else if (f.attack && f.los) {
                    const int b = Bin(en, EN_RELEASE);
                    if (b >= 0) {
                        A.R("trigger.release_los." + releaseLabels[b], !nx->attack);
                    }
                } else if (f.attack && !f.los) {
                    const int   lg  = std::min(lage[i], 1500);
                    const char *lab = lg <= 100 ? "0-100" : lg <= 250 ? "150-250" : lg <= 450 ? "300-450" : lg <= 950 ? "500-950"
                                                                                                                   : "1000-1500";
                    A.R(std::string("trigger.release_hidden.") + lab, !nx->attack);

                }
            }
            if (!f.reloading && f.clip > 0) {
                if (f.los) {
                    const int b = Bin(en, EN_HOLD);
                    if (b >= 0) {
                        A.R("trigger.hold_los." + holdLabels[b], f.attack);
                    }
                } else {
                    A.R("trigger.hold_hidden", f.attack);
                }
            }

            // aim
            if (f.los && f.attack) {
                const int b = Bin(f.distXYZ, DIST_AIM);
                if (b >= 0) {
                    A.V("aim.error_firing." + aimLabels[b], f.aimErr);
                    A.R("aim.on_body_firing." + aimLabels[b], f.onBody);
                }
                A.V("aim.height.firing", f.height);
            } else if (f.los) {
                A.V("aim.height.idle", f.height);
            }

            // view
            if (std::isfinite(f.yawD)) {
                const double s = std::fabs(f.yawD) * 20.0;
                yaw[c].push_back(s);
                yawN[c]++;
                mouseStill[c] += std::fabs(f.yawD) < 0.01f;
                if (!f.flick && !f.still) {
                    yawFree[c].push_back(s);
                }
            }
            if (!f.los) {
                const double s = std::isfinite(since[i]) ? since[i] : 1e9;
                const int    b = Bin(s, SINCE);
                if (b >= 0 && std::isfinite(f.aimYawErr)) {
                    A.V(std::string("belief.hidden_yaw_error.") + SINCE_LABELS[b], std::fabs(f.aimYawErr));
                    A.R(std::string("belief.within30.") + SINCE_LABELS[b], std::fabs(f.aimYawErr) < 30.0f);
                }
            }
        }

        // complete side holds, neutral gaps and attack holds
        for (const Run& run : Runs(r, [](const Frame& f) { return f.side; })) {
            if (!run.complete) {
                continue;
            }
            const double ms = run.n * 50.0;
            if (run.val != 0) {
                A.V("movement.side_hold.all", ms);
                if (run.ctx == CTX_LOS_FIRE) {
                    A.V("movement.side_hold.los_fire", ms);
                }
            } else if (run.ctx == CTX_LOS_FIRE) {
                A.V("movement.neutral_gap.los_fire", ms);
            }
        }
        for (const Run& run : Runs(r, [](const Frame& f) { return f.attack ? 1 : 0; })) {
            if (!run.complete) {
                continue;
            }
            const double ms = run.n * 50.0;
            if (run.val) {
                A.V("trigger.attack_hold", ms);
                A.R("trigger.tap_share", ms <= 100.0);
                // holds begun loaded, by what was on screen when they began (perception.py)
                const Frame& h0 = r[run.first];
                if (h0.clip > 0 && !h0.reloading) {
                    A.R(h0.visParts > 0 ? "perception.tap_share.in_sight" : "perception.tap_share.no_part", ms <= 100.0);
                }
            } else {
                A.V("trigger.attack_gap", ms);
            }
        }

        // turn main sequence: runs of same-sign yaw steps of 3 deg or more
        {
            int    sign = 0;
            double amp = 0.0, peak = 0.0;
            auto   endRun = [&]() {
                if (sign != 0) {
                    const char *lab = amp > 90 && amp <= 135 ? "90-135" : amp > 135 && amp <= 180 ? "135-180" : amp > 180 && amp <= 360 ? "180-360"
                                                                                                                                      : nullptr;
                    if (lab) {
                        A.V(std::string("view.turn_peak.") + lab, peak);
                    }
                }
                sign = 0;
                amp = peak = 0.0;
            };
            for (int i = 0; i < n; i++) {
                const float yd = r[i].yawD;
                if (!std::isfinite(yd)) {
                    endRun();
                    continue;
                }
                const int s = std::fabs(yd) >= 3.0f ? (yd > 0 ? 1 : -1) : 0;
                if (s != sign) {
                    endRun();
                    sign = s;
                }
                if (s != 0) {
                    amp += std::fabs(yd);
                    peak = std::max(peak, std::fabs(yd) * 20.0);
                }
            }
            endRun();
        }

        // large turns (hb_replay's definition): >= 45 deg within 300 ms while hidden, not firing, eligible
        {
            bool prevBig = false;
            for (int i = 6; i < n; i++) {
                float sum = 0.0f;
                bool  ok  = true;
                for (int k = i - 5; k <= i && ok; k++) {
                    ok = std::isfinite(r[k].yawD);
                    sum += ok ? r[k].yawD : 0.0f;
                }
                const bool big = ok && std::fabs(sum) >= 45.0f && !r[i].los && !r[i].attack && r[i].eligible;
                largeTurns += big && !prevBig;
                prevBig = big;
            }
        }

        // acquisitions (acquisition.py: 500 ms occluded before, 300 ms visible after, a 1500 ms window)
        const int PRE = 10, POST = 30;
        if (n >= PRE + 6) {
            for (int i = PRE; i < n; i++) {
                if (!r[i].los || r[i - 1].los || !r[i].eligible) {
                    continue;
                }
                bool ok = true;
                for (int k = i - PRE; k < i && ok; k++) {
                    ok = !r[k].los;
                }
                for (int k = i; k < i + 6 && ok; k++) {
                    ok = k < n && r[k].los;
                }
                if (!ok) {
                    continue;
                }
                const bool pre = r[i - 1].attack;
                acqErrByMode[r[i - 1].viewMode].push_back(r[i].aimErr);
                A.V("diag.acq_belief_err", r[i - 1].beliefErr);
                A.R("acquisition.attacking_before", pre);
                A.R("acquisition.on_target_at_0", r[i].onBody);
                const int end = std::min(n, i + POST);
                for (int t : {-500, -200, 0, 100, 200, 300, 500, 1000}) {
                    const int k = i + t / 50;
                    if (k >= i - PRE && k < end) {
                        A.V("acquisition.error_curve." + std::to_string(t) + "ms", r[k].aimErr);
                    }
                }
                for (int t : {-500, -200, 0, 100, 200, 400, 700, 1000, 1450}) {
                    const int k = i + t / 50;
                    if (k >= i - PRE && k < end) {
                        A.R("acquisition.attack_curve." + std::to_string(t) + "ms", r[k].attack);
                    }
                }
                if (!pre && r[i].aimErr > 2.0f * r[i].halfW) {
                    int press = -1, closeAt = -1;
                    for (int k = i; k < end && r[k].los; k++) {
                        if (press < 0 && r[k].attack) {
                            press = k - i;
                        }
                        if (closeAt < 0 && r[k].aimErr <= r[k].halfW + 1.5f) {
                            closeAt = k - i;
                        }
                    }
                    if (press >= 0) {
                        A.V("acquisition.clean_first_press", press * 50.0);
                    }
                    if (closeAt >= 0) {
                        A.V("acquisition.clean_t_close", closeAt * 50.0);
                    }
                }
            }
        }

        // shots: bursts of consecutive 100 ms shots and accuracy (eligible shots)
        int prevT = INT_MIN, len = 0;
        for (const Shot& s : L.shots) {
            if (!s.eligible) {
                continue;
            }
            A.R("combat.accuracy", s.hit);
            if (prevT != INT_MIN && s.t - prevT == 100) {
                len++;
            } else {
                if (len > 0) {
                    A.V("trigger.burst_length", len);
                }
                len = 1;
            }
            prevT = s.t;
        }
        if (len > 0) {
            A.V("trigger.burst_length", len);
        }
    }

    Perception(lives, A, sight, tanH, tanV);

    Metrics M;
    auto    share = [](long long a, long long d) { return d > 0 ? static_cast<double>(a) / static_cast<double>(d) : NAN; };
    long long stillAll = 0, leanAll = 0;
    for (int c = 0; c < NCTX; c++) {
        const std::string cn = CTX_NAMES[c];
        M["movement.context_share." + cn] = share(ctxN[c], elig);
        for (int g = 0; g < 5; g++) {
            M["movement.chord." + cn + "." + GROUP_NAMES[g]] = share(chordN[c][g], ctxN[c]);
        }
        M["movement.still." + cn]         = share(stillN[c], ctxN[c]);
        M["view.mouse_still." + cn]       = share(mouseStill[c], yawN[c]);
        M["view.yaw_speed." + cn + ".p50"] = Quantile(yaw[c], 0.5);
        M["view.yaw_speed." + cn + ".p99"] = Quantile(yaw[c], 0.99);
        M["diag." + cn + ".flick"]        = share(flickN[c], ctxN[c]);
        M["diag." + cn + ".still"]        = share(planStill[c], ctxN[c]);
        M["diag." + cn + ".detected"]     = share(detN[c], ctxN[c]);
        M["diag." + cn + ".yaw_free_p50"] = Quantile(yawFree[c], 0.5);
        M["diag." + cn + ".lean"]         = share(leanN[c], ctxN[c]);
        M["diag." + cn + ".aim_h_p50"]    = Quantile(aimH[c], 0.5);
        M["diag." + cn + ".err_pitch_p50"] = Quantile(errPitch[c], 0.5);
        M["diag." + cn + ".err_yaw_p50"]  = Quantile(errYaw[c], 0.5);
        for (const auto& kv : modeN[c]) {
            M["diag." + cn + ".mode." + std::to_string(kv.first)] = share(kv.second, ctxN[c]);
        }
        for (const auto& kv : intentN[c]) {
            M["diag." + cn + ".intent." + std::to_string(kv.first)] = share(kv.second, ctxN[c]);
        }
        stillAll += stillN[c];
        leanAll += leanN[c];
    }
    M["movement.still.all"]      = share(stillAll, elig);
    M["movement.lean.all"]       = share(leanAll, elig);
    M["movement.lean.los_fire"]  = share(leanN[CTX_LOS_FIRE], ctxN[CTX_LOS_FIRE]);
    const double minutes         = elig / 1200.0;
    M["movement.jump_per_min"]   = minutes > 0 ? jumpS / minutes : NAN;
    M["movement.crouch_per_min"] = minutes > 0 ? crouchS / minutes : NAN;
    M["movement.walk_per_min"]   = minutes > 0 ? walkS / minutes : NAN;
    M["diag.flicks_per_min"]     = minutes > 0 ? flickStarts / minutes : NAN;
    M["view.large_turns_per_min.hidden"] = hiddenNoFire > 0 ? largeTurns / (hiddenNoFire / 1200.0) : NAN;
    for (const auto& kv : acqErrByMode) {
        M["diag.acq_err0.mode." + std::to_string(kv.first)] = Quantile(kv.second, 0.5);
        M["diag.acq_n.mode." + std::to_string(kv.first)]    = static_cast<double>(kv.second.size());
    }
    M["diag.eligible_minutes"]   = minutes;
    for (const auto& kv : A.ratio) {
        M[kv.first] = kv.second.second > 0 ? kv.second.first / kv.second.second : NAN;
    }
    auto quants = [&](const std::string& base, std::initializer_list<std::pair<const char *, double>> qs) {
        const auto it = A.vals.find(base);
        for (const auto& q : qs) {
            const std::string key = q.first[0] ? base + "." + q.first : base;
            M[key]                = it == A.vals.end() ? NAN : Quantile(it->second, q.second);
        }
    };
    quants("movement.side_hold.los_fire", {{"p25", .25}, {"p50", .5}, {"p75", .75}, {"p90", .9}});
    quants("movement.side_hold.all", {{"p50", .5}, {"p90", .9}});
    quants("movement.neutral_gap.los_fire", {{"p50", .5}, {"p90", .9}});
    quants("trigger.attack_hold", {{"p50", .5}, {"p90", .9}});
    quants("trigger.attack_gap", {{"p50", .5}});
    quants("trigger.burst_length", {{"p50", .5}, {"p90", .9}});
    quants("aim.height.firing", {{"", .5}});
    quants("aim.height.idle", {{"", .5}});
    quants("acquisition.clean_first_press", {{"p25", .25}, {"p50", .5}, {"p75", .75}, {"p90", .9}});
    quants("acquisition.clean_t_close", {{"p50", .5}});
    quants("diag.acq_belief_err", {{"p50", .5}});
    for (const char *cn : CTX_NAMES) {
        quants(std::string("diag.") + cn + ".hdiff", {{"p25", .25}, {"p50", .5}, {"p75", .75}});
        quants(std::string("diag.") + cn + ".hdiff_settled", {{"p25", .25}, {"p50", .5}, {"p75", .75}});
    }
    for (const std::string& lab : aimLabels) {
        quants("aim.error_firing." + lab, {{"", .5}});
    }
    for (const char *lab : SINCE_LABELS) {
        quants(std::string("belief.hidden_yaw_error.") + lab, {{"", .5}});
    }
    for (int t : {-500, -200, 0, 100, 200, 300, 500, 1000}) {
        quants("acquisition.error_curve." + std::to_string(t) + "ms", {{"", .5}});
    }
    for (const char *lab : {"90-135", "135-180", "180-360"}) {
        quants(std::string("view.turn_peak.") + lab, {{"p50", .5}});
    }

    // perception.py: pre-aim, the corner and the reaction from the first visible part
    for (int t : {-500, -200, -100, 0, 100, 200}) {
        quants("perception.error_from_part." + std::to_string(t) + "ms", {{"", .5}});
    }
    quants("perception.part_lead", {{"p50", .5}});
    quants("perception.reaction.clean_first_press", {{"p50", .5}});
    quants("perception.reaction.clean_t_close", {{"p50", .5}});
    quants("perception.reaction.first_hit", {{"p50", .5}});
    for (const char *k : {"yaw_to_enemy.m500", "yaw_to_appearance.m500", "view_turn_toward", "view_turn_abs", "speed_m400_m200",
                          "speed_m2000_m1000"}) {
        quants(std::string("perception.preaim.") + k, {{"", .5}});
    }
    for (const char *k : {"yaw.m500", "angle.m500", "pitch.m500", "lead.m500", "yaw_to_appearance.m500", "yaw_to_enemy.m500",
                          "angle.0ms"}) {
        quants(std::string("perception.corner.") + k, {{"", .5}});
    }
    quants("perception.corner.distance", {{"p50", .5}});
    quants("diag.preaim.view_to_target", {{"p25", .25}, {"p50", .5}, {"p75", .75}, {"p90", .9}});
    for (int mm : {0, 1, 2, 3, 4, 5, 6, 7, 8}) {
        quants("diag.perception.err0.mode." + std::to_string(mm), {{"", .5}});
        quants("diag.perception.view_to_target.mode." + std::to_string(mm), {{"", .5}});
        quants("diag.perception.yaw0.mode." + std::to_string(mm), {{"", .5}});
        quants("diag.perception.lead_m500.mode." + std::to_string(mm), {{"", .5}});
        quants("diag.perception.cornyaw_m500.mode." + std::to_string(mm), {{"", .5}});
        quants("diag.perception.pitch0.mode." + std::to_string(mm), {{"", .5}});
        quants("diag.perception.target_to_enemy.mode." + std::to_string(mm), {{"", .5}});
    }
    for (int t : {-2000, -1000, -500, 0}) {
        quants("perception.corner_2s.yaw." + std::to_string(t) + "ms", {{"", .5}});
    }
    quants("perception.encounter.hidden_spell", {{"p50", .5}, {"p75", .75}, {"p90", .9}});
    quants("perception.encounter.first_sight", {{"p50", .5}, {"p75", .75}});
    quants("perception.encounter.travel_2s_hidden", {{"p50", .5}, {"p25", .25}});

    // the style dials (fit_styles.py definitions)
    M["dial.fwd_diag_fight"] = M["movement.chord.los_fire.fwd_diag"];
    quants("movement.side_hold.all", {{"p50", .5}});
    M["dial.side_hold_ms"]     = M["movement.side_hold.all.p50"];
    M["dial.lean_fight"]       = M["movement.lean.los_fire"];
    M["dial.jumps_per_min"]    = minutes > 0 ? jumpFs / minutes : NAN;
    M["dial.crouch_per_min"]   = minutes > 0 ? crouchFs / minutes : NAN;
    M["dial.walk_hidden"]      = M["movement.walk.hidden_nofire"];
    // bursts that start in sight: the dial shifts the release in sight only (hb_trigger.cpp)
    quants("perception.burst_in_sight", {{"p50", .5}, {"p90", .9}});
    M["dial.burst_median"]     = M["perception.burst_in_sight.p50"];
    quants("dial.aim_height_firing", {{"", .5}});
    quants("skill.aim_error_fight_deg", {{"", .5}});
    M["skill.reaction_ms"] = M["acquisition.clean_first_press.p50"];
    M["dial.hold_angle"]   = M["perception.preaim.parked.m500"];
    return M;
}

} // namespace arena
