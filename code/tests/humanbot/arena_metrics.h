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
// diagnostics with no human counterpart.

#pragma once

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

namespace arena
{

const int NCTX = 5;
static const char *const CTX_NAMES[NCTX] = {"hidden_nofire", "hidden_fire", "los_nofire", "los_fire", "reload"};
enum { CTX_HIDDEN_NOFIRE, CTX_HIDDEN_FIRE, CTX_LOS_NOFIRE, CTX_LOS_FIRE, CTX_RELOAD };

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

inline Metrics Compute(const std::vector<Life>& lives)
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

    // the style dials (fit_styles.py definitions)
    M["dial.fwd_diag_fight"] = M["movement.chord.los_fire.fwd_diag"];
    quants("movement.side_hold.all", {{"p50", .5}});
    M["dial.side_hold_ms"]     = M["movement.side_hold.all.p50"];
    M["dial.lean_fight"]       = M["movement.lean.los_fire"];
    M["dial.jumps_per_min"]    = minutes > 0 ? jumpFs / minutes : NAN;
    M["dial.crouch_per_min"]   = minutes > 0 ? crouchFs / minutes : NAN;
    M["dial.walk_hidden"]      = M["movement.walk.hidden_nofire"];
    M["dial.burst_median"]     = M["trigger.burst_length.p50"];
    quants("dial.aim_height_firing", {{"", .5}});
    quants("skill.aim_error_fight_deg", {{"", .5}});
    M["skill.reaction_ms"] = M["acquisition.clean_first_press.p50"];
    return M;
}

} // namespace arena
