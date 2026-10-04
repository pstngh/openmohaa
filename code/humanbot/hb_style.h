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
// hb_style.h: per-bot style dials.
//
// A bot first draws a style family (diagonal presser, pure strafer, stopper),
// then its own dials around that family's centre, clipped to the recorded
// human range, so habits that belong together stay together. Skill (aim error,
// reaction) is drawn over the full human range independently of the family.
// Calibration curves turn each dial target into the internal offset that
// realises it.

#pragma once

#include "hb_model.h"

#include <cstdint>
#include <string>

namespace hb
{

struct StyleDials {
    int      family = FAMILY_PRESSER;
    uint32_t seed   = 0;
    float    dial[DIAL_COUNT]   = {};
    float    skill[SKILL_COUNT] = {};
    float    mp40Share          = 0.5f;
};

struct StyleOffsets {
    float diagLogit       = 0.0f;  // transition logit into forward-diagonal chords
    float reverseLogit    = 0.0f;  // transition logit of a direct strafe reversal
    float holdScale       = 1.0f;  // chord ages are divided by this before the hazard lookup
    float leanLogit       = 0.0f;  // lean on (+) / off (-) logit shift
    float jumpMult        = 1.0f;
    float crouchMult      = 1.0f;
    float walkMult        = 1.0f;
    float releaseLogit    = 0.0f;  // trigger release hazard shift (burst length)
    float aimHeightFiring = 0.44f;
    float noiseScale      = 1.0f;  // aim noise multiplier (skill)
    float detectMult      = 1.0f;  // detection-rate multiplier
    float reactionLogit   = 0.0f;  // press hazard shift with the enemy in sight (reaction skill)
    float angleHold       = 1.0f;  // hold or clear an angle: x the corner pre-aim horizon and the hold hazard near exposures
    float counterLogit    = 0.0f;  // shift of the chance a strafe key is pressed the tick after one is let go
};

// Draws a bot's dials. family < 0 draws the family from the recorded mix
// (or from familyWeights when given, three weights summing to > 0).
StyleDials SampleStyle(const StyleModel& model, int family, uint32_t seed, const float *familyWeights = nullptr);

// The pooled (average human) style: used for tests and the replay baseline.
StyleDials PooledStyle(const StyleModel& model);

StyleOffsets ComputeOffsets(const StyleDials& dials, const Calibration& calib, const SharedModel& shared);

// "family:seed", stored in the bot's userinfo so a bot keeps its style across maps.
std::string StyleKey(const StyleDials& dials);
bool        ParseStyleKey(const char *key, int& family, uint32_t& seed);
int         FamilyFromName(const char *name);  // -1 for "random" or unknown

// Compact JSON object with every dial (telemetry bot_style event, humanbot_list).
std::string DialsJson(const StyleDials& dials);

} // namespace hb
