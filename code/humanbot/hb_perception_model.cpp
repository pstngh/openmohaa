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
// hb_perception_model.cpp: detection, hearing and damage direction.

#include "hb_perception_model.h"

#include <cmath>

namespace hb
{

void Perceiver::Init(const PerceptionModel *params, const Rng& rng)
{
    m_p   = params;
    m_rng = rng;
    Reset();
}

void Perceiver::Reset()
{
    m_tracks.clear();
    m_lastDetectP = 0.0f;
}

Perceiver::Track& Perceiver::TrackFor(int id)
{
    for (Track& t : m_tracks) {
        if (t.id == id) {
            return t;
        }
    }
    Track t;
    t.id = id;
    m_tracks.push_back(t);
    return m_tracks.back();
}

static int PopCount(int mask)
{
    int n = 0;
    for (; mask; mask &= mask - 1) {
        n++;
    }
    return n;
}

void Perceiver::Process(const RawInput& raw, float hfovDeg, float vfovDeg, float detectMult, Observation& out)
{
    const PerceptionModel& p = *m_p;
    out.self                 = raw.self;
    out.enemies.clear();
    out.sounds.clear();
    out.damage.clear();
    out.deaths              = raw.deaths;
    out.gotKillOf           = raw.gotKillOf;
    out.teammateInCrosshair = raw.teammateInCrosshair;
    m_lastDetectP           = 0.0f;

    const Vec3& eye = raw.self.eye;

    //
    // Vision: one uniform per listed enemy per tick, whatever its state
    //
    for (const RawEnemy& e : raw.enemies) {
        const double u = m_rng.Uniform();
        Track&       t = TrackFor(e.id);
        if (!e.alive) {
            t.detected  = false;
            t.lostTicks = 1000;
            continue;
        }
        // Defensive: positions are only meaningful for visible parts.
        const bool visible = e.partMask != 0 && e.inFov;
        EnemyObs   o;
        o.id         = e.id;
        o.bodyHeight = e.bodyHeight;
        if (visible) {
            const Vec3  d      = e.centroid - eye;
            const float dist   = d.length();
            const float dyaw   = std::fabs(Wrap180(YawOf(d) - raw.self.viewYaw));
            const float dpitch = std::fabs(PitchOf(d) - raw.self.viewPitch);
            const float ecc    = std::sqrt(dyaw * dyaw + dpitch * dpitch);
            const int   parts  = PopCount(e.partMask);
            float       rate   = p.detectRate * detectMult * std::pow(parts / static_cast<float>(NUM_PARTS), p.partExponent)
                        * std::exp(-ecc / p.eccScaleDeg) * std::exp(-dist / p.distScale);
            // outside the fovea the frustum edge makes detection much slower
            if (dyaw > 0.5f * hfovDeg || dpitch > 0.5f * vfovDeg) {
                rate *= 0.1f;
            }
            const float pDet = 1.0f - std::exp(-rate);
            m_lastDetectP    = pDet;
            if (!t.detected && t.lostTicks <= p.lossMemoryTicks) {
                // brief occlusion: the target is picked up again at once
                t.detected = true;
            } else if (!t.detected && u < pDet) {
                t.detected = true;
            }
            t.lostTicks = 0;
            if (t.detected) {
                o.detected    = true;
                o.visParts    = parts;
                o.partMask    = e.partMask;
                for (int k = 0; k < NUM_PARTS; k++) {
                    if (e.partMask & (1 << k)) {
                        o.partPos[k] = e.partPos[k];
                    }
                }
                o.pos         = e.centroid;
                o.vel         = e.velocity;
                o.centroidLos = e.centroidLos;
                o.reloading   = e.reloading;
                o.firing      = e.firing;
                o.detectP     = pDet;
                t.lastPos     = e.centroid;
                t.lastTime    = raw.self.timeMs;
            }
        } else {
            if (t.lostTicks < 1000) {
                t.lostTicks++;
            }
            t.detected = false;
        }
        out.enemies.push_back(o);
    }

    //
    // Hearing: three draws per sound event
    //
    for (const RawSound& s : raw.sounds) {
        const double u1 = m_rng.Uniform();
        const double n1 = m_rng.Normal();
        const double n2 = m_rng.Normal();
        const Vec3   d  = s.origin - raw.self.origin;
        const float  dist = d.length();
        float        range, sigma;
        switch (s.type) {
        case SOUND_FOOTSTEP:
            range = p.footstepRange;
            sigma = p.footstepSigmaDeg;
            break;
        case SOUND_GUNFIRE:
            range = p.gunfireRange;
            sigma = p.gunfireSigmaDeg;
            break;
        case SOUND_RELOAD:
            range = p.reloadRange;
            sigma = p.reloadSigmaDeg;
            break;
        default:
            range = 0.5f * p.gunfireRange;
            sigma = p.footstepSigmaDeg;
            break;
        }
        if (dist > range) {
            continue;
        }
        SoundObs o;
        o.type      = s.type;
        o.sourceId  = -1;
        o.yawSigma  = sigma;
        o.yaw       = Wrap180(YawOf(d) + static_cast<float>(n1) * sigma);
        o.dist      = std::max(16.0f, dist * std::exp(static_cast<float>(n2) * p.distanceLogSd));
        o.distLogSd = p.distanceLogSd;
        // mirror across the listener's left-right axis
        o.mirrorYaw = Wrap180(2.0f * raw.self.viewYaw + 180.0f - o.yaw);
        if (s.type == SOUND_FOOTSTEP && u1 < p.frontBackConfusion) {
            o.frontBack = true;
            const float y = o.yaw;
            o.yaw         = o.mirrorYaw;
            o.mirrorYaw   = y;
        } else if (s.type == SOUND_FOOTSTEP) {
            o.frontBack = true;  // ambiguous either way; the belief weighs both
        }
        out.sounds.push_back(o);
    }

    //
    // Damage direction: one draw per hit
    //
    for (const RawDamage& dmg : raw.damage) {
        const double n1 = m_rng.Normal();
        DamageObs    o;
        const Vec3   d = dmg.attackerPos - raw.self.origin;
        o.yaw          = Wrap180(YawOf(d) + static_cast<float>(n1) * p.damageSigmaDeg);
        o.yawSigma     = p.damageSigmaDeg;
        o.damage       = dmg.damage;
        o.attackerId   = dmg.attackerId;
        out.damage.push_back(o);
    }
}

} // namespace hb
