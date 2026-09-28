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
// hb_style.cpp: style sampling and dial offsets.

#include "hb_style.h"
#include "hb_math.h"
#include "hb_rng.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace hb
{

StyleDials SampleStyle(const StyleModel& model, int family, uint32_t seed, const float *familyWeights)
{
    StyleDials d;
    Rng        rng(0x5b1e5eedULL ^ (static_cast<uint64_t>(seed) << 16));
    Rng        famRng = rng.Derive(1);
    Rng        dialRng = rng.Derive(2);
    Rng        skillRng = rng.Derive(3);
    Rng        weaponRng = rng.Derive(4);

    if (family < 0 || family >= FAMILY_COUNT) {
        double w[FAMILY_COUNT];
        for (int f = 0; f < FAMILY_COUNT; f++) {
            w[f] = familyWeights ? familyWeights[f] : model.families[f].weight;
        }
        family = famRng.Categorical(w, FAMILY_COUNT);
    }
    d.family = family;
    d.seed   = seed;

    const FamilyDist& fd = model.families[family];
    for (int i = 0; i < DIAL_COUNT; i++) {
        const float v = static_cast<float>(dialRng.Normal(fd.centre[i], fd.spread[i]));
        d.dial[i]     = Clamp(v, model.dialMin[i], model.dialMax[i]);
    }
    for (int i = 0; i < SKILL_COUNT; i++) {
        d.skill[i] = static_cast<float>(skillRng.Uniform(model.skillMin[i], model.skillMax[i]));
    }
    std::vector<double> mixw;
    for (const WeaponMixComponent& c : model.weaponMix) {
        mixw.push_back(c.weight);
    }
    const WeaponMixComponent& c = model.weaponMix[weaponRng.Categorical(mixw)];
    d.mp40Share = Clamp(static_cast<float>(weaponRng.Normal(c.mean, c.sd)), 0.0f, 1.0f);
    return d;
}

StyleDials PooledStyle(const StyleModel& model)
{
    StyleDials d;
    d.family = -1;
    for (int i = 0; i < DIAL_COUNT; i++) {
        d.dial[i] = model.pooled[i];
    }
    for (int i = 0; i < SKILL_COUNT; i++) {
        d.skill[i] = model.pooledSkill[i];
    }
    d.mp40Share = 0.5f;
    return d;
}

StyleOffsets ComputeOffsets(const StyleDials& dials, const Calibration& calib, const SharedModel& shared)
{
    StyleOffsets o;
    o.diagLogit       = calib.dial[DIAL_FWD_DIAG].Eval(dials.dial[DIAL_FWD_DIAG]);
    o.reverseLogit    = calib.dial[DIAL_REVERSE].Eval(dials.dial[DIAL_REVERSE]);
    o.holdScale       = Clamp(calib.dial[DIAL_SIDE_HOLD].Eval(dials.dial[DIAL_SIDE_HOLD]), 0.4f, 2.5f);
    o.leanLogit       = calib.dial[DIAL_LEAN].Eval(dials.dial[DIAL_LEAN]);
    o.jumpMult        = Clamp(calib.dial[DIAL_JUMPS].Eval(dials.dial[DIAL_JUMPS]), 0.0f, 10.0f);
    o.crouchMult      = Clamp(calib.dial[DIAL_CROUCH].Eval(dials.dial[DIAL_CROUCH]), 0.0f, 10.0f);
    o.walkMult        = Clamp(calib.dial[DIAL_WALK].Eval(dials.dial[DIAL_WALK]), 0.0f, 10.0f);
    o.releaseLogit    = calib.dial[DIAL_BURST].Eval(dials.dial[DIAL_BURST]);
    o.aimHeightFiring = Clamp(calib.dial[DIAL_AIM_HEIGHT].Eval(dials.dial[DIAL_AIM_HEIGHT]), 0.2f, 0.8f);
    o.noiseScale      = Clamp(calib.skill[SKILL_AIM_ERROR].Eval(dials.skill[SKILL_AIM_ERROR]), 0.3f, 3.0f);
    // the time to the first shot after a sighting is set by the trigger, not by detection
    // (a detection sweep in the arena moves it by less than 30 ms)
    o.reactionLogit   = Clamp(calib.skill[SKILL_REACTION].Eval(dials.skill[SKILL_REACTION]), -3.0f, 3.0f);
    if (dials.family < 0) {
        // pooled style: no dial shift at all
        StyleOffsets p;
        p.aimHeightFiring = shared.view.aimHeightFiring;
        return p;
    }
    return o;
}

std::string StyleKey(const StyleDials& dials)
{
    char buf[64];
    const int f = dials.family >= 0 && dials.family < FAMILY_COUNT ? dials.family : 0;
    std::snprintf(buf, sizeof(buf), "%s:%u", FAMILY_NAMES[f], static_cast<unsigned>(dials.seed));
    return buf;
}

int FamilyFromName(const char *name)
{
    if (!name) {
        return -1;
    }
    for (int f = 0; f < FAMILY_COUNT; f++) {
        if (!std::strcmp(name, FAMILY_NAMES[f])) {
            return f;
        }
    }
    return -1;
}

bool ParseStyleKey(const char *key, int& family, uint32_t& seed)
{
    if (!key || !*key) {
        return false;
    }
    const char *colon = std::strchr(key, ':');
    if (!colon) {
        return false;
    }
    std::string fam(key, colon - key);
    family = FamilyFromName(fam.c_str());
    if (family < 0) {
        return false;
    }
    char         *end = nullptr;
    unsigned long v   = std::strtoul(colon + 1, &end, 10);
    if (end == colon + 1) {
        return false;
    }
    seed = static_cast<uint32_t>(v);
    return true;
}

std::string DialsJson(const StyleDials& dials)
{
    std::string s = "{";
    char        buf[96];
    const char *fam = dials.family >= 0 && dials.family < FAMILY_COUNT ? FAMILY_NAMES[dials.family] : "pooled";
    std::snprintf(buf, sizeof(buf), "\"family\":\"%s\",\"seed\":%u", fam, static_cast<unsigned>(dials.seed));
    s += buf;
    for (int i = 0; i < DIAL_COUNT; i++) {
        std::snprintf(buf, sizeof(buf), ",\"%s\":%.4g", DIAL_NAMES[i], dials.dial[i]);
        s += buf;
    }
    for (int i = 0; i < SKILL_COUNT; i++) {
        std::snprintf(buf, sizeof(buf), ",\"%s\":%.4g", SKILL_NAMES[i], dials.skill[i]);
        s += buf;
    }
    std::snprintf(buf, sizeof(buf), ",\"mp40_share\":%.4g}", dials.mp40Share);
    s += buf;
    return s;
}

} // namespace hb
