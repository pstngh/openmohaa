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
// hb_bundle.cpp: JSON parsing and validation of the model bundle.
//
// This is the only translation unit that includes json.hpp.

#include "hb_bundle.h"
#include "hb_map.h"

#if defined(__GNUC__) || defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wshadow"
#    pragma GCC diagnostic ignored "-Wconversion"
#    pragma GCC diagnostic ignored "-Wsign-compare"
#elif defined(_MSC_VER)
#    pragma warning(push, 0)
#endif
#include "../thirdparty/nlohmann-json-3.7.3/json.hpp"
#if defined(__GNUC__) || defined(__clang__)
#    pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#    pragma warning(pop)
#endif

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <stdexcept>

using json = nlohmann::json;

namespace hb
{

const char *const DIAL_NAMES[DIAL_COUNT] = {"fwd_diag_fight", "reverse_share", "side_hold_ms", "lean_fight", "jumps_per_min",
                                            "crouch_per_min", "walk_hidden", "burst_median", "aim_height_firing",
                                            "hold_angle", "counter_strafe", "lean_switch",
                                            "lean_drop"};
const char *const SKILL_NAMES[SKILL_COUNT]   = {"aim_error_fight_deg", "reaction_ms"};
const char *const FAMILY_NAMES[FAMILY_COUNT] = {"presser", "strafer", "stopper"};

float Curve::Eval(float t) const
{
    if (!Valid()) {
        return 0.0f;
    }
    return Interp(t, x, y);
}

//
// SHA-256 (FIPS 180-4), small and portable
//
namespace
{
const uint32_t K256[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
    0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
    0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

inline uint32_t Rotr(uint32_t x, int n)
{
    return (x >> n) | (x << (32 - n));
}

void Sha256Block(uint32_t h[8], const unsigned char *p)
{
    uint32_t w[64];
    for (int i = 0; i < 16; i++) {
        w[i] = (uint32_t(p[i * 4]) << 24) | (uint32_t(p[i * 4 + 1]) << 16) | (uint32_t(p[i * 4 + 2]) << 8) | uint32_t(p[i * 4 + 3]);
    }
    for (int i = 16; i < 64; i++) {
        const uint32_t s0 = Rotr(w[i - 15], 7) ^ Rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        const uint32_t s1 = Rotr(w[i - 2], 17) ^ Rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i]              = w[i - 16] + s0 + w[i - 7] + s1;
    }
    uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
    for (int i = 0; i < 64; i++) {
        const uint32_t S1  = Rotr(e, 6) ^ Rotr(e, 11) ^ Rotr(e, 25);
        const uint32_t ch  = (e & f) ^ (~e & g);
        const uint32_t t1  = hh + S1 + ch + K256[i] + w[i];
        const uint32_t S0  = Rotr(a, 2) ^ Rotr(a, 13) ^ Rotr(a, 22);
        const uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        const uint32_t t2  = S0 + maj;
        hh = g;
        g  = f;
        f  = e;
        e  = d + t1;
        d  = c;
        c  = b;
        b  = a;
        a  = t1 + t2;
    }
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += hh;
}
} // namespace

std::string Sha256Hex(const std::string& data)
{
    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    const size_t n = data.size();
    size_t       i = 0;
    for (; i + 64 <= n; i += 64) {
        Sha256Block(h, reinterpret_cast<const unsigned char *>(data.data()) + i);
    }
    unsigned char tail[128] = {0};
    const size_t  rem       = n - i;
    for (size_t k = 0; k < rem; k++) {
        tail[k] = static_cast<unsigned char>(data[i + k]);
    }
    tail[rem]              = 0x80;
    const size_t   tlen    = rem < 56 ? 64 : 128;
    const uint64_t bits    = static_cast<uint64_t>(n) * 8;
    for (int k = 0; k < 8; k++) {
        tail[tlen - 1 - k] = static_cast<unsigned char>(bits >> (8 * k));
    }
    Sha256Block(h, tail);
    if (tlen == 128) {
        Sha256Block(h, tail + 64);
    }
    static const char *hex = "0123456789abcdef";
    std::string        out;
    for (int k = 0; k < 8; k++) {
        for (int s = 28; s >= 0; s -= 4) {
            out += hex[(h[k] >> s) & 15];
        }
    }
    return out;
}

//
// Parsing helpers
//
namespace
{
struct ParseError : std::runtime_error {
    explicit ParseError(const std::string& what)
        : std::runtime_error(what)
    {}
};

void Require(bool cond, const std::string& what)
{
    if (!cond) {
        throw ParseError(what);
    }
}

const json& Get(const json& j, const char *key, const char *path)
{
    Require(j.is_object() && j.contains(key), std::string("missing ") + path + "." + key);
    return j.at(key);
}

float Num(const json& j, const std::string& path)
{
    Require(j.is_number(), path + " is not a number");
    const double v = j.get<double>();
    Require(std::isfinite(v), path + " is not finite");
    return static_cast<float>(v);
}

float NumOr(const json& j, const char *key, float def)
{
    if (!j.is_object() || !j.contains(key) || !j.at(key).is_number()) {
        return def;
    }
    const double v = j.at(key).get<double>();
    return std::isfinite(v) ? static_cast<float>(v) : def;
}

std::vector<float> Floats(const json& j, const std::string& path)
{
    Require(j.is_array(), path + " is not an array");
    std::vector<float> out;
    out.reserve(j.size());
    for (size_t i = 0; i < j.size(); i++) {
        out.push_back(Num(j[i], path));
    }
    return out;
}

std::vector<double> Doubles(const json& j, const std::string& path)
{
    std::vector<float>  f = Floats(j, path);
    return std::vector<double>(f.begin(), f.end());
}

std::vector<int> Ints(const json& j, const std::string& path)
{
    Require(j.is_array(), path + " is not an array");
    std::vector<int> out;
    for (size_t i = 0; i < j.size(); i++) {
        out.push_back(static_cast<int>(std::lround(Num(j[i], path))));
    }
    return out;
}

// Fills a Table from nested arrays with the given dimensions.
void FillTable(const json& j, Table& t, std::initializer_list<int> dims, const std::string& path)
{
    t.Resize(dims);
    std::vector<int> d(dims);
    std::vector<float>& v = t.v;
    size_t              k = 0;
    std::function<void(const json&, size_t)> rec = [&](const json& node, size_t level) {
        Require(node.is_array() && static_cast<int>(node.size()) == d[level],
                path + " has the wrong shape at depth " + std::to_string(level));
        for (size_t i = 0; i < node.size(); i++) {
            if (level + 1 == d.size()) {
                v[k++] = Num(node[i], path);
            } else {
                rec(node[i], level + 1);
            }
        }
    };
    rec(j, 0);
}

void RequireAscending(const std::vector<float>& e, const std::string& path)
{
    Require(!e.empty(), path + " is empty");
    for (size_t i = 1; i < e.size(); i++) {
        Require(e[i] > e[i - 1], path + " is not ascending");
    }
}

void RequireProb(const std::vector<float>& p, const std::string& path)
{
    for (float x : p) {
        Require(x >= 0.0f && x <= 1.0f, path + " has a value outside [0, 1]");
    }
}

void ParseKey(const json& j, KeyModel& k, int nAge, int nClear, int nCtxAge, int changeRows, const char *path)
{
    const std::string p(path);
    FillTable(Get(j, "switch_logit", path), k.switchLogit, {CTX_COUNT, 3, 3, nAge}, p + ".switch_logit");
    k.wallLogit = Floats(Get(j, "wall_logit", path), p + ".wall_logit");
    Require(static_cast<int>(k.wallLogit.size()) == nClear, p + ".wall_logit size");
    // optional: models fitted before these terms leave walls out of diagonals and choices
    k.diagWallLogit.clear();
    k.choiceWallLogit.clear();
    if (j.contains("diag_wall_logit")) {
        k.diagWallLogit = Floats(j.at("diag_wall_logit"), p + ".diag_wall_logit");
        Require(static_cast<int>(k.diagWallLogit.size()) == nClear, p + ".diag_wall_logit size");
    }
    if (j.contains("choice_wall_logit")) {
        k.choiceWallLogit = Floats(j.at("choice_wall_logit"), p + ".choice_wall_logit");
        Require(static_cast<int>(k.choiceWallLogit.size()) == nClear, p + ".choice_wall_logit size");
    }
    k.losChangeLogit = Num(Get(j, "los_change_logit", path), p + ".los_change_logit");
    FillTable(Get(j, "ctx_change_logit", path), k.ctxChangeLogit, {CTX_COUNT, changeRows, nCtxAge - 1}, p + ".ctx_change_logit");
}

void ParseMovement(const json& j, MovementModel& m)
{
    const json& kj = Get(j, "keys", "movement");
    m.ageEdges     = Ints(Get(kj, "age_edges", "movement.keys"), "movement.keys.age_edges");
    Require(!m.ageEdges.empty() && m.ageEdges[0] == 1, "movement.keys.age_edges must start at 1");
    m.clearEdges  = Floats(Get(kj, "clear_edges", "movement.keys"), "movement.keys.clear_edges");
    m.distEdges   = Floats(Get(kj, "dist_edges", "movement.keys"), "movement.keys.dist_edges");
    m.ctxAgeEdges = Ints(Get(kj, "ctx_age_edges", "movement.keys"), "movement.keys.ctx_age_edges");
    RequireAscending(m.clearEdges, "movement.keys.clear_edges");
    RequireAscending(m.distEdges, "movement.keys.dist_edges");
    Require(m.ctxAgeEdges.size() >= 2, "movement.keys.ctx_age_edges size");
    const int nAge = static_cast<int>(m.ageEdges.size());
    const int nClr = static_cast<int>(m.clearEdges.size());
    const int nCa  = static_cast<int>(m.ctxAgeEdges.size());
    const json& sj = Get(kj, "side", "movement.keys");
    const json& fj = Get(kj, "fwd", "movement.keys");
    ParseKey(sj, m.side, nAge, nClr, nCa, 2, "movement.keys.side");
    ParseKey(fj, m.fwd, nAge, nClr, nCa, 3, "movement.keys.fwd");
    // optional: models fitted before 2026-10-04 have no distance term with the enemy hidden
    m.hidDistEdges.clear();
    m.side.hidDistLogit = Table();
    m.fwd.hidDistLogit  = Table();
    if (kj.contains("hid_dist_edges")) {
        m.hidDistEdges = Floats(kj.at("hid_dist_edges"), "movement.keys.hid_dist_edges");
        RequireAscending(m.hidDistEdges, "movement.keys.hid_dist_edges");
        const int nHd = static_cast<int>(m.hidDistEdges.size());
        FillTable(Get(sj, "hid_dist_logit", "movement.keys.side"), m.side.hidDistLogit, {2, 3, nHd},
                  "movement.keys.side.hid_dist_logit");
        FillTable(Get(fj, "hid_dist_logit", "movement.keys.fwd"), m.fwd.hidDistLogit, {3, nHd},
                  "movement.keys.fwd.hid_dist_logit");
    }
    FillTable(Get(sj, "reverse_p", "movement.keys.side"), m.reverseP, {CTX_COUNT, 3, nAge}, "movement.keys.side.reverse_p");
    FillTable(Get(sj, "right_p", "movement.keys.side"), m.rightP, {CTX_COUNT, 3}, "movement.keys.side.right_p");
    RequireProb(m.reverseP.v, "movement.keys.side.reverse_p");
    RequireProb(m.rightP.v, "movement.keys.side.right_p");
    if (sj.contains("opposite_p")) {
        m.oppositeGapEdges = Ints(Get(sj, "opposite_gap_edges", "movement.keys.side"), "movement.keys.side.opposite_gap_edges");
        FillTable(sj.at("opposite_p"), m.oppositeP, {CTX_COUNT, static_cast<int>(m.oppositeGapEdges.size())},
                  "movement.keys.side.opposite_p");
        RequireProb(m.oppositeP.v, "movement.keys.side.opposite_p");
    }
    FillTable(Get(fj, "next_logit", "movement.keys.fwd"), m.fwdNext, {CTX_COUNT, 3, 3, 3}, "movement.keys.fwd.next_logit");
    FillTable(Get(fj, "approach", "movement.keys.fwd"), m.approach, {CTX_COUNT, static_cast<int>(m.distEdges.size())},
              "movement.keys.fwd.approach");
    m.approachEnemyReload = NumOr(fj, "approach_enemy_reload", 0.0f);

    const json& ln  = Get(j, "lean", "movement");
    m.leanAgeEdges  = Ints(Get(ln, "age_edges", "movement.lean"), "movement.lean.age_edges");
    // relations: 3, or 4 with the tick the strafe turns against a lean apart from the ticks it stays against
    const json& lnx = Get(ln, "next", "movement.lean");
    Require(lnx.is_array() && !lnx.empty() && lnx[0].is_array() && !lnx[0].empty() && lnx[0][0].is_array()
                && !lnx[0][0].empty() && lnx[0][0][0].is_array(),
            "movement.lean.next has the wrong shape");
    m.leanRels = static_cast<int>(lnx[0][0][0].size());
    Require(m.leanRels == 3 || m.leanRels == 4, "movement.lean.next: 3 or 4 strafe relations");
    FillTable(lnx, m.leanNext, {2, LEAN_CTX_COUNT, static_cast<int>(m.leanAgeEdges.size()), m.leanRels, 3},
              "movement.lean.next");
    RequireProb(m.leanNext.v, "movement.lean.next");
    m.leanCtxAgeEdges = Ints(Get(ln, "ctx_age_edges", "movement.lean"), "movement.lean.ctx_age_edges");
    Require(m.leanCtxAgeEdges.size() >= 2, "movement.lean.ctx_age_edges size");
    FillTable(Get(ln, "ctx_change_logit", "movement.lean"), m.leanCtxChangeLogit,
              {CTX_COUNT, 2, static_cast<int>(m.leanCtxAgeEdges.size()) - 1}, "movement.lean.ctx_change_logit");
    m.leanCtxLogit.assign(CTX_COUNT, 0.0f);
    m.sideCtxLogit.assign(CTX_COUNT, 0.0f);
    m.fwdCtxLogit.assign(CTX_COUNT, 0.0f);
    if (j.contains("habit")) {
        const json& h = j.at("habit");
        if (h.contains("side_ctx_logit")) {
            m.sideCtxLogit = Floats(h.at("side_ctx_logit"), "movement.habit.side_ctx_logit");
            Require(m.sideCtxLogit.size() == CTX_COUNT, "movement.habit.side_ctx_logit size");
        }
        m.reverseLogit = NumOr(h, "reverse_logit", m.reverseLogit);
        m.walkMult     = NumOr(h, "walk_mult", m.walkMult);
        if (h.contains("fwd_ctx_logit")) {
            m.fwdCtxLogit = Floats(h.at("fwd_ctx_logit"), "movement.habit.fwd_ctx_logit");
            Require(m.fwdCtxLogit.size() == CTX_COUNT, "movement.habit.fwd_ctx_logit size");
        }
    }
    if (ln.contains("wall")) {
        const json& w   = ln.at("wall");
        m.leanWallRange = NumOr(w, "range", m.leanWallRange);
        m.leanEdgeOpen  = NumOr(w, "edge_open", m.leanEdgeOpen);
        m.leanWallLogit = NumOr(w, "wall_logit", m.leanWallLogit);
        m.leanEdgeLogit = NumOr(w, "edge_logit", m.leanEdgeLogit);
        Require(m.leanWallRange > 0.0f, "movement.lean.wall.range out of range");
    }
    if (ln.contains("ctx_logit")) {
        m.leanCtxLogit = Floats(ln.at("ctx_logit"), "movement.lean.ctx_logit");
        Require(m.leanCtxLogit.size() == CTX_COUNT, "movement.lean.ctx_logit size");
    }

    const json& st = Get(j, "stance", "movement");
    StanceKeyModel *keys[3] = {&m.crouch, &m.jump, &m.walk};
    const char     *names[3] = {"crouch", "jump", "walk"};
    for (int i = 0; i < 3; i++) {
        const json& k        = Get(st, names[i], "movement.stance");
        keys[i]->pressHazard = Floats(Get(k, "press_hazard", "movement.stance"), "movement.stance.press_hazard");
        keys[i]->holdPmf     = Floats(Get(k, "hold_pmf", "movement.stance"), "movement.stance.hold_pmf");
        keys[i]->releaseAgeEdges = Ints(Get(k, "release_age_edges", "movement.stance"), "movement.stance.release_age_edges");
        keys[i]->releaseHazard   = Floats(Get(k, "release_hazard", "movement.stance"), "movement.stance.release_hazard");
        Require(keys[i]->releaseAgeEdges.size() == keys[i]->releaseHazard.size() && !keys[i]->releaseHazard.empty(),
                std::string("movement.stance.") + names[i] + ".release sizes");
        RequireProb(keys[i]->releaseHazard, "movement.stance.release_hazard");
        Require(keys[i]->pressHazard.size() == CTX_COUNT, std::string("movement.stance.") + names[i] + ".press_hazard size");
        RequireProb(keys[i]->pressHazard, "movement.stance.press_hazard");
        if (k.contains("press_hazard_fire")) {
            keys[i]->pressHazardFire = Floats(k.at("press_hazard_fire"), "movement.stance.press_hazard_fire");
            Require(keys[i]->pressHazardFire.size() == CTX_COUNT,
                    std::string("movement.stance.") + names[i] + ".press_hazard_fire size");
            RequireProb(keys[i]->pressHazardFire, "movement.stance.press_hazard_fire");
        }
        if (k.contains("up_hazard")) {
            keys[i]->upAgeEdges = Ints(Get(k, "up_age_edges", "movement.stance"), "movement.stance.up_age_edges");
            keys[i]->upHazard   = Floats(Get(k, "up_hazard", "movement.stance"), "movement.stance.up_hazard");
            Require(keys[i]->upAgeEdges.size() == keys[i]->upHazard.size() && !keys[i]->upHazard.empty(),
                    std::string("movement.stance.") + names[i] + ".up sizes");
            RequireProb(keys[i]->upHazard, "movement.stance.up_hazard");
        }
    }
    m.vetoClearance = NumOr(j, "veto_clearance", 0.0f);
    if (j.contains("coupling")) {
        const json& c       = j.at("coupling");
        m.navSwitchLogit    = NumOr(c, "nav_switch_logit", m.navSwitchLogit);
        m.navChoiceLogit    = NumOr(c, "nav_choice_logit", m.navChoiceLogit);
        m.navFarMult        = NumOr(c, "nav_far_mult", m.navFarMult);
        m.navFarNear        = NumOr(c, "nav_far_near", m.navFarNear);
        m.navFarDist        = NumOr(c, "nav_far_dist", m.navFarDist);
        m.wallPressureLogit = NumOr(c, "wall_pressure_logit", m.wallPressureLogit);
        m.wallReflexMs      = NumOr(c, "wall_reflex_ms", m.wallReflexMs);
        m.wallReflexLogit   = NumOr(c, "wall_reflex_logit", m.wallReflexLogit);
    }
}

void ParseSpawn(const json& j, SpawnModel& m)
{
    m.deadTicksPmf = Floats(Get(j, "dead_ticks_pmf", "spawn"), "spawn.dead_ticks_pmf");
    m.chordP       = Floats(Get(j, "chord_p", "spawn"), "spawn.chord_p");
    m.ageEdges     = Ints(Get(j, "age_edges", "spawn"), "spawn.age_edges");
    Require(!m.deadTicksPmf.empty() && m.deadTicksPmf.size() <= 16, "spawn.dead_ticks_pmf size");
    Require(m.chordP.size() == NUM_CHORDS, "spawn.chord_p size");
    Require(!m.ageEdges.empty() && m.ageEdges[0] == 1, "spawn.age_edges must start at 1");
    RequireProb(m.deadTicksPmf, "spawn.dead_ticks_pmf");
    RequireProb(m.chordP, "spawn.chord_p");
    const int nAge = static_cast<int>(m.ageEdges.size());
    FillTable(Get(j, "side_switch_p", "spawn"), m.sideSwitchP, {2, nAge}, "spawn.side_switch_p");
    FillTable(Get(j, "fwd_switch_p", "spawn"), m.fwdSwitchP, {3, nAge}, "spawn.fwd_switch_p");
    RequireProb(m.sideSwitchP.v, "spawn.side_switch_p");
    RequireProb(m.fwdSwitchP.v, "spawn.fwd_switch_p");
    m.clickFirstP = Num(Get(j, "click_first_p", "spawn"), "spawn.click_first_p");
    m.clickStayP  = Floats(Get(j, "click_stay_p", "spawn"), "spawn.click_stay_p");
    m.clickPressP = Floats(Get(j, "click_press_p", "spawn"), "spawn.click_press_p");
    Require(m.clickStayP.size() == m.clickPressP.size(), "spawn click sizes");
    RequireProb(m.clickStayP, "spawn.click_stay_p");
    RequireProb(m.clickPressP, "spawn.click_press_p");
}

void ParseNoise(const json& j, NoiseModel& n)
{
    n.medianAbsUnits  = NumOr(j, "median_abs_units", n.medianAbsUnits);
    n.floorDeg        = NumOr(j, "median_abs_floor_deg", n.floorDeg);
    n.tNu             = NumOr(j, "t_nu", n.tNu);
    n.tScalePerMedian = NumOr(j, "t_scale_per_median", n.tScalePerMedian);
    n.ar1             = NumOr(j, "ar1", n.ar1);
    Require(n.tNu > 1.0f && n.ar1 > -1.0f && n.ar1 < 1.0f && n.medianAbsUnits >= 0.0f, "view noise out of range");
}

void ParseYaw(const json& j, YawController& c, const std::string& path)
{
    c.rho   = Num(Get(j, "rho", path.c_str()), path + ".rho");
    c.Kp    = Num(Get(j, "Kp", path.c_str()), path + ".Kp");
    c.Kself = Num(Get(j, "Kself", path.c_str()), path + ".Kself");
    c.Kopp  = Num(Get(j, "Kopp", path.c_str()), path + ".Kopp");
    c.bias  = NumOr(j, "bias", 0.0f);
    Require(c.rho > -1.0f && c.rho < 1.0f, path + ".rho out of range");
    if (j.contains("noise")) {
        ParseNoise(j.at("noise"), c.noise);
    }
}

void ParsePitch(const json& j, PitchController& c, const std::string& path)
{
    c.rho  = Num(Get(j, "rho", path.c_str()), path + ".rho");
    c.Kp   = Num(Get(j, "Kp", path.c_str()), path + ".Kp");
    c.Kt   = Num(Get(j, "Kt", path.c_str()), path + ".Kt");
    c.bias = NumOr(j, "bias", 0.0f);
    Require(c.rho > -1.0f && c.rho < 1.0f, path + ".rho out of range");
    if (j.contains("noise")) {
        ParseNoise(j.at("noise"), c.noise);
    }
}

void ParseView(const json& j, ViewModel& v)
{
    ParseYaw(Get(j, "firing", "view"), v.firing, "view.firing");
    ParseYaw(Get(j, "idle", "view"), v.idle, "view.idle");
    ParsePitch(Get(j, "pitch_firing", "view"), v.pitchFiring, "view.pitch_firing");
    ParsePitch(Get(j, "pitch_idle", "view"), v.pitchIdle, "view.pitch_idle");
    v.flickDeg = NumOr(j, "flick_deg", v.flickDeg);
    const json& st = Get(j, "still", "view");
    v.stillEnter   = Floats(Get(st, "enter", "view.still"), "view.still.enter");
    v.stillStay    = Floats(Get(st, "stay", "view.still"), "view.still.stay");
    Require(v.stillEnter.size() == CTX_COUNT && v.stillStay.size() == CTX_COUNT, "view.still size");
    RequireProb(v.stillEnter, "view.still.enter");
    RequireProb(v.stillStay, "view.still.stay");
    if (st.contains("enter_standing")) {
        v.stillEnterStanding = Floats(Get(st, "enter_standing", "view.still"), "view.still.enter_standing");
        v.stillStayStanding  = Floats(Get(st, "stay_standing", "view.still"), "view.still.stay_standing");
        Require(v.stillEnterStanding.size() == CTX_COUNT && v.stillStayStanding.size() == CTX_COUNT, "view.still standing size");
        RequireProb(v.stillEnterStanding, "view.still.enter_standing");
        RequireProb(v.stillStayStanding, "view.still.stay_standing");
        v.standingSpeed = NumOr(st, "standing_speed", v.standingSpeed);
    }
    const json& ms = Get(j, "main_sequence", "view");
    Require(ms.is_array() && !ms.empty(), "view.main_sequence empty");
    v.mainSequence.clear();
    for (size_t i = 0; i < ms.size(); i++) {
        MainSequenceRow r;
        r.ampLo      = NumOr(ms[i], "amp_lo", 0.0f);
        r.ampMed     = NumOr(ms[i], "amp_med", 1.0f);
        r.ticksMed   = NumOr(ms[i], "ticks_med", 1.0f);
        r.ticksSigma = NumOr(ms[i], "ticks_sigma", 0.3f);
        Require(r.ticksMed >= 1.0f && r.ampMed > 0.0f, "view.main_sequence row out of range");
        v.mainSequence.push_back(r);
    }
    if (j.contains("flick_gain")) {
        v.flickGainMedian = NumOr(j.at("flick_gain"), "median", v.flickGainMedian);
        v.flickGainSigma  = NumOr(j.at("flick_gain"), "sigma", v.flickGainSigma);
    }
    if (j.contains("aim_height")) {
        v.aimHeightIdle   = NumOr(j.at("aim_height"), "idle", v.aimHeightIdle);
        v.aimHeightFiring = NumOr(j.at("aim_height"), "firing", v.aimHeightFiring);
        v.aimHeightSd     = NumOr(j.at("aim_height"), "sd", v.aimHeightSd);
    }
    if (j.contains("tuning")) {
        const json& t       = j.at("tuning");
        v.noiseScale        = NumOr(t, "noise_scale", v.noiseScale);
        v.biasScale         = NumOr(t, "bias_scale", v.biasScale);
        v.acquireMinHalfW   = NumOr(t, "acquire_min_halfw", v.acquireMinHalfW);
        v.acquireHazard     = NumOr(t, "acquire_hazard", v.acquireHazard);
        v.trackFlickHazard  = NumOr(t, "track_flick_hazard", v.trackFlickHazard);
        v.lookaroundPerMin  = NumOr(t, "lookaround_per_min", v.lookaroundPerMin);
        v.preaimShare       = NumOr(t, "preaim_share", v.preaimShare);
        v.preaimCoverDeg    = NumOr(t, "preaim_cover_deg", v.preaimCoverDeg);
        v.preaimBelowDeg    = NumOr(t, "preaim_below_deg", v.preaimBelowDeg);
        v.preaimHazard      = NumOr(t, "preaim_hazard", v.preaimHazard);
        v.preaimWeight      = NumOr(t, "preaim_weight", v.preaimWeight);
        v.preaimHorizonMs   = NumOr(t, "preaim_horizon_ms", v.preaimHorizonMs);
        v.preaimFlickDeg    = NumOr(t, "preaim_flick_deg", v.preaimFlickDeg);
        v.preaimSelfComp    = NumOr(t, "preaim_self_comp", v.preaimSelfComp);
        v.beliefLookShare   = NumOr(t, "belief_look_share", v.beliefLookShare);
        v.travelShare       = NumOr(t, "travel_share", v.travelShare);
        v.respawnRelook     = NumOr(t, "respawn_relook", v.respawnRelook);
        v.lostAimLastSeen   = NumOr(t, "lost_aim_last_seen", v.lostAimLastSeen);
        v.holdBeliefDeg     = NumOr(t, "hold_belief_deg", v.holdBeliefDeg);
        v.preaimFollowGeom  = NumOr(t, "preaim_follow_geom", v.preaimFollowGeom);
        v.beliefLookCorner  = NumOr(t, "belief_look_corner", v.beliefLookCorner);
        v.routeTurnHazard   = NumOr(t, "route_turn_hazard", v.routeTurnHazard);
        v.preaimPassDps     = NumOr(t, "preaim_pass_dps", v.preaimPassDps);
        v.travelFollowDeg   = NumOr(t, "travel_follow_deg", v.travelFollowDeg);
        v.lookDwellMedianMs = NumOr(t, "look_dwell_median_ms", v.lookDwellMedianMs);
        v.lookDwellSigma    = NumOr(t, "look_dwell_sigma", v.lookDwellSigma);
        v.damageTurnDelayMs = NumOr(t, "damage_turn_delay_ms", v.damageTurnDelayMs);
        v.soundTurnP        = NumOr(t, "sound_turn_p", v.soundTurnP);
        v.soundTurnDeg      = NumOr(t, "sound_turn_deg", v.soundTurnDeg);
        v.soundTurnDelayMs  = NumOr(t, "sound_turn_delay_ms", v.soundTurnDelayMs);
        v.soundTurnHoldMs   = NumOr(t, "sound_turn_hold_ms", v.soundTurnHoldMs);
        v.pitchOffsetFiring = NumOr(t, "pitch_offset_firing", v.pitchOffsetFiring);
        v.pitchOffsetIdle   = NumOr(t, "pitch_offset_idle", v.pitchOffsetIdle);
        v.pitchGainScale    = NumOr(t, "pitch_gain_scale", v.pitchGainScale);
        v.hiddenNoiseScale  = NumOr(t, "hidden_noise_scale", v.hiddenNoiseScale);
        v.trackGainScale    = NumOr(t, "track_gain_scale", v.trackGainScale);
        v.beliefFollowDeg   = NumOr(t, "belief_follow_deg", v.beliefFollowDeg);
        v.hiddenReaimDeg    = NumOr(t, "hidden_reaim_deg", v.hiddenReaimDeg);
        v.hiddenReaimHazard = NumOr(t, "hidden_reaim_hazard", v.hiddenReaimHazard);
        if (t.contains("still_logit")) {
            const std::vector<float> sl = Floats(t.at("still_logit"), "view.tuning.still_logit");
            Require(sl.size() == CTX_COUNT, "view.tuning.still_logit size");
            for (int c = 0; c < CTX_COUNT; c++) {
                v.stillLogit[c] = sl[c];
            }
        }
        v.flickRefractoryTicks = static_cast<int>(NumOr(t, "flick_refractory_ticks", static_cast<float>(v.flickRefractoryTicks)));
    }
}

void ParseTriggerSide(const json& j, TriggerSide& s, size_t nErr, size_t nLage, size_t nAge, const std::string& path)
{
    s.bias = Num(Get(j, "bias", path.c_str()), path + ".bias");
    s.err  = Floats(Get(j, "err", path.c_str()), path + ".err");
    s.lage = Floats(Get(j, "lage", path.c_str()), path + ".lage");
    s.age  = Floats(Get(j, "age", path.c_str()), path + ".age");
    Require(s.err.size() == nErr && s.lage.size() == nLage && s.age.size() == nAge, path + " table sizes");
    s.damaged = NumOr(j, "damaged", 0.0f);
    if (j.contains("clip")) {
        s.clip = Floats(j.at("clip"), path + ".clip");
    }
}

void ParseTrigger(const json& j, TriggerModel& t)
{
    t.enEdges         = Floats(Get(j, "en_edges", "trigger"), "trigger.en_edges");
    t.yawEdges        = Floats(Get(j, "yaw_edges", "trigger"), "trigger.yaw_edges");
    t.lageLosEdges    = Floats(Get(j, "lage_los_edges", "trigger"), "trigger.lage_los_edges");
    t.lageHiddenEdges = Floats(Get(j, "lage_hidden_edges", "trigger"), "trigger.lage_hidden_edges");
    t.holdEdges       = Ints(Get(j, "hold_edges", "trigger"), "trigger.hold_edges");
    t.gapEdges        = Ints(Get(j, "gap_edges", "trigger"), "trigger.gap_edges");
    RequireAscending(t.enEdges, "trigger.en_edges");
    RequireAscending(t.yawEdges, "trigger.yaw_edges");
    RequireAscending(t.lageLosEdges, "trigger.lage_los_edges");
    RequireAscending(t.lageHiddenEdges, "trigger.lage_hidden_edges");
    ParseTriggerSide(Get(j, "press_los", "trigger"), t.pressLos, t.enEdges.size(), t.lageLosEdges.size(), t.gapEdges.size(),
                     "trigger.press_los");
    ParseTriggerSide(Get(j, "release_los", "trigger"), t.releaseLos, t.enEdges.size(), t.lageLosEdges.size(), t.holdEdges.size(),
                     "trigger.release_los");
    ParseTriggerSide(Get(j, "press_hidden", "trigger"), t.pressHidden, t.yawEdges.size(), t.lageHiddenEdges.size(),
                     t.gapEdges.size(), "trigger.press_hidden");
    ParseTriggerSide(Get(j, "release_hidden", "trigger"), t.releaseHidden, t.yawEdges.size(), t.lageHiddenEdges.size(),
                     t.holdEdges.size(), "trigger.release_hidden");
    if (j.contains("clip_edges")) {
        t.clipEdges = Floats(j.at("clip_edges"), "trigger.clip_edges");
        RequireAscending(t.clipEdges, "trigger.clip_edges");
    }
    Require(t.pressLos.clip.empty() || t.pressLos.clip.size() == t.clipEdges.size(), "trigger.press_los.clip size");
    Require(t.pressHidden.clip.empty() || t.pressHidden.clip.size() == t.clipEdges.size(), "trigger.press_hidden.clip size");
    if (j.contains("tuning")) {
        t.anticipationLogit = NumOr(j.at("tuning"), "anticipation_logit", t.anticipationLogit);
        t.hiddenFireLogit   = NumOr(j.at("tuning"), "hidden_fire_logit", t.hiddenFireLogit);
        t.hiddenLateLogit   = NumOr(j.at("tuning"), "hidden_late_logit", t.hiddenLateLogit);
        t.pressLosLogit     = NumOr(j.at("tuning"), "press_los_logit", t.pressLosLogit);
        t.releaseLosLogit   = NumOr(j.at("tuning"), "release_los_logit", t.releaseLosLogit);
        t.releaseNearLogit  = NumOr(j.at("tuning"), "release_near_logit", t.releaseNearLogit);
        t.releaseFarLogit   = NumOr(j.at("tuning"), "release_far_logit", t.releaseFarLogit);
        t.releaseTapLogit   = NumOr(j.at("tuning"), "release_tap_logit", t.releaseTapLogit);
    }
}

void ParseQuantiles(const json& j, std::vector<double>& probs, std::vector<double>& vals, const std::string& path)
{
    probs = Doubles(Get(j, "probs", path.c_str()), path + ".probs");
    vals  = Doubles(Get(j, "ms", path.c_str()), path + ".ms");
    Require(probs.size() == vals.size() && probs.size() >= 2, path + " sizes");
    for (size_t i = 1; i < probs.size(); i++) {
        Require(probs[i] >= probs[i - 1] && vals[i] >= vals[i - 1], path + " is not monotone");
    }
}

void ParseWeapon(const json& j, WeaponModel& w)
{
    w.postKillRoundEdges = Ints(Get(j, "post_kill_round_edges", "weapon"), "weapon.post_kill_round_edges");
    w.postKillReloadP    = Floats(Get(j, "post_kill_reload_p", "weapon"), "weapon.post_kill_reload_p");
    Require(w.postKillRoundEdges.size() == w.postKillReloadP.size(), "weapon.post_kill tables");
    RequireProb(w.postKillReloadP, "weapon.post_kill_reload_p");
    ParseQuantiles(Get(j, "post_kill_delay", "weapon"), w.postKillDelayProbs, w.postKillDelayMs, "weapon.post_kill_delay");
    ParseQuantiles(Get(j, "respawn", "weapon"), w.respawnProbs, w.respawnMs, "weapon.respawn");
    w.tacticalHazard     = NumOr(j, "tactical_hazard", w.tacticalHazard);
    w.tacticalClipFrac   = NumOr(j, "tactical_clip_frac", w.tacticalClipFrac);
    if (j.contains("tactical")) {
        const json& t       = j.at("tactical");
        w.tacticalClipEdges = Floats(Get(t, "clip_edges", "weapon.tactical"), "weapon.tactical.clip_edges");
        w.tacticalSeenEdges = Floats(Get(t, "seen_edges", "weapon.tactical"), "weapon.tactical.seen_edges");
        RequireAscending(w.tacticalClipEdges, "weapon.tactical.clip_edges");
        RequireAscending(w.tacticalSeenEdges, "weapon.tactical.seen_edges");
        FillTable(Get(t, "hazard", "weapon.tactical"), w.tacticalTable,
                  {static_cast<int>(w.tacticalClipEdges.size()), static_cast<int>(w.tacticalSeenEdges.size())},
                  "weapon.tactical.hazard");
        RequireProb(w.tacticalTable.v, "weapon.tactical.hazard");
    }
    w.pistolSwitchPerMin = NumOr(j, "pistol_switch_per_min", w.pistolSwitchPerMin);
}

void ParsePerception(const json& j, PerceptionModel& p)
{
    p.detectRate         = NumOr(j, "detect_rate", p.detectRate);
    p.eccScaleDeg        = NumOr(j, "ecc_scale_deg", p.eccScaleDeg);
    p.distScale          = NumOr(j, "dist_scale", p.distScale);
    p.partExponent       = NumOr(j, "part_exponent", p.partExponent);
    p.lossMemoryTicks    = static_cast<int>(NumOr(j, "loss_memory_ticks", static_cast<float>(p.lossMemoryTicks)));
    p.gunfireSigmaDeg    = NumOr(j, "gunfire_sigma_deg", p.gunfireSigmaDeg);
    p.gunfireRange       = NumOr(j, "gunfire_range", p.gunfireRange);
    p.footstepSigmaDeg   = NumOr(j, "footstep_sigma_deg", p.footstepSigmaDeg);
    p.footstepRange      = NumOr(j, "footstep_range", p.footstepRange);
    p.frontBackConfusion = NumOr(j, "front_back_confusion", p.frontBackConfusion);
    p.reloadRange        = NumOr(j, "reload_range", p.reloadRange);
    p.reloadSigmaDeg     = NumOr(j, "reload_sigma_deg", p.reloadSigmaDeg);
    p.distanceLogSd      = NumOr(j, "distance_log_sd", p.distanceLogSd);
    p.damageSigmaDeg     = NumOr(j, "damage_sigma_deg", p.damageSigmaDeg);
    Require(p.detectRate > 0.0f && p.eccScaleDeg > 0.0f && p.distScale > 0.0f, "perception out of range");
}

void ParseBelief(const json& j, BeliefModel& b)
{
    b.particles       = static_cast<int>(NumOr(j, "particles", static_cast<float>(b.particles)));
    b.negDetect       = NumOr(j, "neg_detect", b.negDetect);
    b.essResample     = NumOr(j, "ess_resample", b.essResample);
    b.jitter          = NumOr(j, "jitter", b.jitter);
    b.moveBoost       = NumOr(j, "move_boost", b.moveBoost);
    b.soundSigmaScale = NumOr(j, "sound_sigma_scale", b.soundSigmaScale);
    b.spawnMinDist    = NumOr(j, "spawn_min_dist", b.spawnMinDist);
    b.momentum        = NumOr(j, "momentum", b.momentum);
    b.injectMax       = NumOr(j, "inject_max", b.injectMax);
    Require(b.particles >= 16 && b.particles <= 4096, "belief.particles out of range");
    Require(b.negDetect >= 0.0f && b.negDetect <= 1.0f, "belief.neg_detect out of range");
}

void ParseNav(const json& j, NavModel& n)
{
    n.holdHazard    = NumOr(j, "hold_hazard", n.holdHazard);
    n.holdMedianMs  = NumOr(j, "hold_median_ms", n.holdMedianMs);
    n.holdSigma     = NumOr(j, "hold_sigma", n.holdSigma);
    n.spawnPushMs   = NumOr(j, "spawn_push_ms", n.spawnPushMs);
    n.huntUrgency   = NumOr(j, "hunt_urgency", n.huntUrgency);
    n.engageUrgency = NumOr(j, "engage_urgency", n.engageUrgency);
    n.reloadUrgency = NumOr(j, "reload_urgency", n.reloadUrgency);
    n.waypointReach = NumOr(j, "waypoint_reach", n.waypointReach);
    n.repathMs      = NumOr(j, "repath_ms", n.repathMs);
    n.postKillMs    = NumOr(j, "post_kill_ms", n.postKillMs);
}

void ParsePresentation(const json& j, PresentationModel& p)
{
    p.pingMedianMs      = NumOr(j, "ping_median_ms", p.pingMedianMs);
    p.pingSigma         = NumOr(j, "ping_sigma", p.pingSigma);
    p.pingDriftAr       = NumOr(j, "ping_drift_ar", p.pingDriftAr);
    p.pingJitterMs      = NumOr(j, "ping_jitter_ms", p.pingJitterMs);
    p.joinDelayMedianMs = NumOr(j, "join_delay_median_ms", p.joinDelayMedianMs);
    p.joinDelaySigma    = NumOr(j, "join_delay_sigma", p.joinDelaySigma);
}

void ParseShared(const json& j, SharedModel& m)
{
    m.version = static_cast<int>(Num(Get(j, "version", ""), "version"));
    Require(m.version == 1, "unsupported shared model version");
    ParseSpawn(Get(j, "spawn", ""), m.spawn);
    ParseMovement(Get(j, "movement", ""), m.movement);
    ParseView(Get(j, "view", ""), m.view);
    ParseTrigger(Get(j, "trigger", ""), m.trigger);
    ParseWeapon(Get(j, "weapon", ""), m.weapon);
    if (j.contains("perception")) {
        ParsePerception(j.at("perception"), m.perception);
    }
    if (j.contains("belief")) {
        ParseBelief(j.at("belief"), m.belief);
    }
    if (j.contains("nav")) {
        ParseNav(j.at("nav"), m.nav);
    }
    if (j.contains("presentation")) {
        ParsePresentation(j.at("presentation"), m.presentation);
    }
}

void ParseStyles(const json& j, StyleModel& s)
{
    const json& fams = Get(j, "families", "styles");
    Require(fams.is_array() && fams.size() == FAMILY_COUNT, "styles.families must list the 3 families");
    for (int f = 0; f < FAMILY_COUNT; f++) {
        const json& fj = fams[f];
        FamilyDist& fd = s.families[f];
        fd.name        = Get(fj, "name", "styles.families").get<std::string>();
        Require(fd.name == FAMILY_NAMES[f], "styles.families order must be presser, strafer, stopper");
        fd.weight = Num(Get(fj, "weight", "styles.families"), "styles.families.weight");
        for (int d = 0; d < DIAL_COUNT; d++) {
            fd.centre[d] = Num(Get(Get(fj, "centre", "styles.families"), DIAL_NAMES[d], "styles.families.centre"), "centre");
            fd.spread[d] = Num(Get(Get(fj, "spread", "styles.families"), DIAL_NAMES[d], "styles.families.spread"), "spread");
            Require(fd.spread[d] >= 0.0f, "styles spread must be >= 0");
        }
    }
    const json& mn = Get(j, "min", "styles");
    const json& mx = Get(j, "max", "styles");
    for (int d = 0; d < DIAL_COUNT; d++) {
        s.dialMin[d] = Num(Get(mn, DIAL_NAMES[d], "styles.min"), "styles.min");
        s.dialMax[d] = Num(Get(mx, DIAL_NAMES[d], "styles.max"), "styles.max");
        Require(s.dialMax[d] >= s.dialMin[d], "styles min > max");
    }
    for (int d = 0; d < SKILL_COUNT; d++) {
        s.skillMin[d] = Num(Get(mn, SKILL_NAMES[d], "styles.min"), "styles.min");
        s.skillMax[d] = Num(Get(mx, SKILL_NAMES[d], "styles.max"), "styles.max");
        Require(s.skillMax[d] >= s.skillMin[d], "styles skill min > max");
    }
    if (j.contains("pooled")) {
        for (int d = 0; d < DIAL_COUNT; d++) {
            s.pooled[d] = NumOr(j.at("pooled"), DIAL_NAMES[d], 0.5f * (s.dialMin[d] + s.dialMax[d]));
        }
        for (int d = 0; d < SKILL_COUNT; d++) {
            s.pooledSkill[d] = NumOr(j.at("pooled"), SKILL_NAMES[d], 0.5f * (s.skillMin[d] + s.skillMax[d]));
        }
    }
    s.weaponMix.clear();
    if (j.contains("weapon_mix")) {
        for (const json& c : j.at("weapon_mix")) {
            WeaponMixComponent w;
            w.weight = NumOr(c, "weight", 0.0f);
            w.mean   = NumOr(c, "mean", 0.5f);
            w.sd     = NumOr(c, "sd", 0.1f);
            if (w.weight > 0.0f) {
                s.weaponMix.push_back(w);
            }
        }
    }
    if (s.weaponMix.empty()) {
        s.weaponMix.push_back(WeaponMixComponent());
    }
}

void ParseCurve(const json& j, Curve& c, const std::string& path)
{
    c.x = Floats(Get(j, "x", path.c_str()), path + ".x");
    c.y = Floats(Get(j, "y", path.c_str()), path + ".y");
    Require(c.Valid(), path + " needs >= 2 matching points");
    RequireAscending(c.x, path + ".x");
}

void ParseCalibration(const json& j, Calibration& c)
{
    const json& d = Get(j, "dial", "calibration");
    for (int i = 0; i < DIAL_COUNT; i++) {
        ParseCurve(Get(d, DIAL_NAMES[i], "calibration.dial"), c.dial[i], std::string("calibration.dial.") + DIAL_NAMES[i]);
    }
    const json& s = Get(j, "skill", "calibration");
    for (int i = 0; i < SKILL_COUNT; i++) {
        ParseCurve(Get(s, SKILL_NAMES[i], "calibration.skill"), c.skill[i], std::string("calibration.skill.") + SKILL_NAMES[i]);
    }
}

json MergePatch(const json& target, const json& patch)
{
    if (!patch.is_object()) {
        return patch;
    }
    json out = target.is_object() ? target : json::object();
    for (auto it = patch.begin(); it != patch.end(); ++it) {
        if (it.value().is_null()) {
            out.erase(it.key());
        } else {
            out[it.key()] = MergePatch(out.contains(it.key()) ? out[it.key()] : json(), it.value());
        }
    }
    return out;
}

json ParseText(const std::string& text, const std::string& name)
{
    try {
        return json::parse(text);
    } catch (const std::exception& e) {
        throw ParseError(name + ": " + e.what());
    }
}

json Patched(const std::string& name, const std::map<std::string, std::string>& overrides)
{
    const std::string base = EmbeddedText(name);
    Require(!base.empty(), "embedded " + name + " is missing");
    json j  = ParseText(base, name);
    auto it = overrides.find(name);
    if (it != overrides.end() && !it->second.empty()) {
        j = MergePatch(j, ParseText(it->second, "override " + name));
    }
    return j;
}

bool BuildBundle(const std::map<std::string, std::string>& overrides, ModelBundle& out)
{
    ModelBundle b;
    ParseShared(Patched("shared.json", overrides), b.shared);
    ParseStyles(Patched("styles.json", overrides), b.style);
    ParseCalibration(Patched("calibration.json", overrides), b.calib);
    b.sha256 = EmbeddedSha256();
    if (!overrides.empty()) {
        std::string all;
        for (const auto& kv : overrides) {
            all += kv.first;
            all += '\n';
            all += kv.second;
        }
        b.sha256 = Sha256Hex(std::string(EmbeddedSha256()) + all);
        b.source = "override";
    } else {
        b.source = "embedded";
    }
    out = std::move(b);
    return true;
}

} // namespace

bool LoadBundle(const std::map<std::string, std::string>& overrides, ModelBundle& out, std::string& error)
{
    error.clear();
    try {
        return BuildBundle(overrides, out);
    } catch (const std::exception& e) {
        error = e.what();
    }
    if (!overrides.empty()) {
        try {
            BuildBundle(std::map<std::string, std::string>(), out);
            return false;
        } catch (const std::exception& e) {
            error += std::string("; embedded bundle also failed: ") + e.what();
        }
    }
    return false;
}

bool LoadMapPrior(const std::string& text, const std::string& overrideText, MapPrior& out, std::string& error)
{
    error.clear();
    try {
        json j = ParseText(text, "map prior");
        if (!overrideText.empty()) {
            j = MergePatch(j, ParseText(overrideText, "map prior override"));
        }
        MapPrior m;
        m.name     = Get(j, "map", "map").get<std::string>();
        // as an integer: a float keeps 24 bits, and most map checksums need all 32 (sv_mapChecksum is compared
        // exactly, and a rounded checksum sends the bots to the navmesh-derived map)
        const json& sum = Get(j, "checksum", "map");
        Require(sum.is_number_integer(), "map.checksum must be an integer");
        m.checksum = static_cast<int32_t>(sum.get<int64_t>());
        m.cellSize = Num(Get(j, "cell_size", "map"), "map.cell_size");
        Require(m.cellSize >= 8.0f, "map.cell_size too small");
        const json&        cj   = Get(j, "cells", "map");
        std::vector<int>   ix   = Ints(Get(cj, "ix", "map.cells"), "map.cells.ix");
        std::vector<int>   iy   = Ints(Get(cj, "iy", "map.cells"), "map.cells.iy");
        std::vector<float> z    = Floats(Get(cj, "z", "map.cells"), "map.cells.z");
        std::vector<float> occ  = Floats(Get(cj, "occ", "map.cells"), "map.cells.occ");
        std::vector<float> lv   = Floats(Get(cj, "leave", "map.cells"), "map.cells.leave");
        std::vector<float> stl  = Floats(Get(cj, "still", "map.cells"), "map.cells.still");
        const size_t       n    = ix.size();
        Require(n > 0 && iy.size() == n && z.size() == n && lv.size() == n && stl.size() == n && occ.size() == n * CTX_COUNT,
                "map.cells sizes");
        m.cells.resize(n);
        for (size_t i = 0; i < n; i++) {
            MapCell& c = m.cells[i];
            c.ix       = ix[i];
            c.iy       = iy[i];
            c.z        = z[i];
            c.center   = Vec3((ix[i] + 0.5f) * m.cellSize, (iy[i] + 0.5f) * m.cellSize, z[i]);
            c.occTotal = 0.0f;
            for (int k = 0; k < CTX_COUNT; k++) {
                c.occ[k] = occ[i * CTX_COUNT + k];
                c.occTotal += c.occ[k];
            }
            c.leave = Clamp(lv[i], 0.01f, 0.95f);
            c.still = Clamp(stl[i], 0.0f, 1.0f);
        }
        const json&        ej  = Get(j, "edges", "map");
        std::vector<int>   ef  = Ints(Get(ej, "from", "map.edges"), "map.edges.from");
        std::vector<int>   et  = Ints(Get(ej, "to", "map.edges"), "map.edges.to");
        std::vector<float> ec  = Floats(Get(ej, "count", "map.edges"), "map.edges.count");
        Require(ef.size() == et.size() && ef.size() == ec.size(), "map.edges sizes");
        std::vector<std::vector<std::pair<int, float>>> out_edges(n);
        for (size_t k = 0; k < ef.size(); k++) {
            Require(ef[k] >= 0 && static_cast<size_t>(ef[k]) < n && et[k] >= 0 && static_cast<size_t>(et[k]) < n, "map edge index");
            out_edges[ef[k]].push_back(std::make_pair(et[k], ec[k]));
        }
        m.kStart.assign(n + 1, 0);
        for (size_t a = 0; a < n; a++) {
            m.kStart[a] = static_cast<int>(m.kTo.size());
            for (const auto& e : out_edges[a]) {
                m.kTo.push_back(e.first);
                m.kCount.push_back(e.second);
            }
        }
        m.kStart[n] = static_cast<int>(m.kTo.size());
        const json& kj    = Get(j, "kernel", "map");
        m.kernelDistEdges = Floats(Get(kj, "dist_edges", "map.kernel"), "map.kernel.dist_edges");
        m.kernelBeta      = Floats(Get(kj, "beta", "map.kernel"), "map.kernel.beta");
        Require(m.kernelBeta.size() == m.kernelDistEdges.size(), "map.kernel sizes");
        if (j.contains("spawns")) {
            for (const json& s : j.at("spawns")) {
                Require(s.is_array() && s.size() >= 4, "map.spawns entry");
                MapSpawn sp;
                sp.pos   = Vec3(Num(s[0], "spawn"), Num(s[1], "spawn"), Num(s[2], "spawn"));
                sp.yaw   = Num(s[3], "spawn");
                sp.count = s.size() > 4 ? Num(s[4], "spawn") : 1.0f;
                m.spawns.push_back(sp);
            }
        }
        m.hiddenMoveSpeed = NumOr(j, "hidden_move_speed", m.hiddenMoveSpeed);
        m.AddCellIndex();
        m.BuildRouteGraph();
        if (j.contains("los")) {
            const json&        lj = j.at("los");
            std::vector<int>   la = Ints(Get(lj, "a", "map.los"), "map.los.a");
            std::vector<int>   lb = Ints(Get(lj, "b", "map.los"), "map.los.b");
            std::vector<float> ln = Floats(Get(lj, "n", "map.los"), "map.los.n");
            std::vector<float> lvv = Floats(Get(lj, "vis", "map.los"), "map.los.vis");
            Require(la.size() == lb.size() && la.size() == ln.size() && la.size() == lvv.size(), "map.los sizes");
            for (size_t k = 0; k < la.size(); k++) {
                if (ln[k] > 0.0f) {
                    m.SetEmpiricalVisibility(la[k], lb[k], lvv[k] / ln[k]);
                }
            }
        }
        out = std::move(m);
        return true;
    } catch (const std::exception& e) {
        error = e.what();
    }
    return false;
}

} // namespace hb
