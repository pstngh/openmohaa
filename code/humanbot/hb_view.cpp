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
// hb_view.cpp: view controller, flicks, still gate and look policy.

#include "hb_view.h"

#include <algorithm>
#include <cmath>

namespace hb
{

static constexpr float HEAD_HEIGHT   = 62.0f;   // ~0.66 of a standing body: where people pre-aim
static constexpr float MAX_RATE_DEG  = 60.0f;   // per tick outside flicks
static constexpr float PITCH_LIMIT   = 85.0f;
static constexpr float BODY_HALF_W   = 15.0f;

static float MinJerk(float tau)
{
    tau = Clamp(tau, 0.0f, 1.0f);
    return tau * tau * tau * (10.0f + tau * (-15.0f + 6.0f * tau));
}

void ViewControl::Init(const ViewModel *params)
{
    m_p = params;
    SelfState s;
    Reset(s);
}

void ViewControl::Reset(const SelfState& self)
{
    m_rate  = 0.0f;
    m_prate = 0.0f;
    for (float& e : m_err) {
        e = 0.0f;
    }
    for (float& e : m_perr) {
        e = 0.0f;
    }
    for (float& w : m_wopp) {
        w = 0.0f;
    }
    for (float& t : m_tpitch) {
        t = 0.0f;
    }
    m_noise          = 0.0f;
    m_pnoise         = 0.0f;
    m_still          = false;
    m_histValid      = false;
    m_aimH           = m_p ? m_p->aimHeightIdle : 0.66f;
    m_flick          = Flick();
    m_lookMode       = VIEW_TRAVEL;
    m_lookPointValid = false;
    m_lookYaw        = self.viewYaw;
    m_lookPitch      = 0.0f;
    m_dwellMs        = 0.0f;
    m_damagePending  = false;
    m_wasTracking    = false;
}

float ViewControl::FlickDurationMs(float amplitude) const
{
    const std::vector<MainSequenceRow>& ms = m_p->mainSequence;
    if (ms.empty()) {
        return 150.0f;
    }
    std::vector<float> xs, ys;
    for (const MainSequenceRow& r : ms) {
        xs.push_back(r.ampMed);
        ys.push_back(r.ticksMed);
    }
    // a flick spanning n ticks of >= 3 deg steps lasts about n ticks plus a partial one
    return 50.0f * Interp(amplitude, xs, ys) + 25.0f;
}

void ViewControl::StartFlick(float errYaw, float errPitch, float gainMedian, float gainSigma, Rng& rng)
{
    const float g  = static_cast<float>(rng.LogNormal(gainMedian, gainSigma));
    const float gp = static_cast<float>(rng.LogNormal(0.85, 0.2));
    m_flick.active   = true;
    m_flick.ampYaw   = errYaw * g;
    m_flick.ampPitch = errPitch * gp;
    const float amp  = std::sqrt(m_flick.ampYaw * m_flick.ampYaw + m_flick.ampPitch * m_flick.ampPitch);
    // duration: main sequence with the recorded spread
    float sigma = 0.3f;
    for (const MainSequenceRow& r : m_p->mainSequence) {
        if (amp >= r.ampLo) {
            sigma = r.ticksSigma;
        }
    }
    m_flick.durMs = std::max(40.0f, FlickDurationMs(amp) * static_cast<float>(rng.LogNormal(1.0, Clamp(sigma, 0.1f, 0.7f))));
    // it starts somewhere inside this tick
    m_flick.tMs = -static_cast<float>(rng.Uniform(0.0, 25.0));
}

float ViewControl::NoiseStep(const NoiseModel& nm, float dist, float& state, float scale, Rng& rng)
{
    const float d        = std::max(32.0f, dist);
    const float scaleDeg = std::atan(nm.medianAbsUnits / d) * RAD2DEG + nm.floorDeg;
    const float t        = static_cast<float>(rng.StudentT(nm.tNu));
    const float phi      = Clamp(nm.ar1, -0.95f, 0.95f);
    const float innov    = std::sqrt(1.0f - phi * phi) * nm.tScalePerMedian * scaleDeg * t * scale;
    state                = phi * state + innov;
    return state;
}

void ViewControl::ChooseLook(const SelfState& self, const ViewInput& in, Rng& rng)
{
    const ViewModel& p   = *m_p;
    const Vec3&      eye = self.eye;
    double           w[4] = {0.0, 0.0, 0.0, 0.0};   // preaim, travel, lookaround, sound
    const BeliefEstimate *b = in.belief;
    const bool hasExposure  = b && b->valid && b->nExposure > 0;
    const bool heard        = in.sounds && !in.sounds->empty();
    if (hasExposure) {
        w[0] = p.preaimShare;
    }
    if (in.moving && in.navValid) {
        w[1] = p.travelShare;
    }
    w[2] = std::max(0.05, 1.0 - w[0] - w[1]);
    if (heard) {
        w[3] = 1.5;
    }
    const int pick = rng.Categorical(w, 4);
    const double dwellMul = rng.LogNormal(1.0, p.lookDwellSigma);
    m_dwellMs = static_cast<float>(p.lookDwellMedianMs * dwellMul);
    switch (pick) {
    case 0:
        {
            double ew[MAX_EXPOSURE];
            for (int i = 0; i < b->nExposure; i++) {
                ew[i] = b->exposureMass[i] * std::exp(-b->exposureEtaMs[i] / 2500.0);
            }
            const int i      = rng.Categorical(ew, b->nExposure);
            m_lookPoint      = b->exposure[i] + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
            m_lookPointValid = true;
            m_lookMode       = VIEW_PREAIM;
            break;
        }
    case 1:
        m_lookPoint      = eye + YawDir(in.navYaw) * 400.0f;
        m_lookPointValid = true;
        m_lookMode       = VIEW_TRAVEL;
        m_dwellMs *= 0.7f;
        break;
    case 3:
        {
            const SoundObs& s = in.sounds->back();
            const float     d = Clamp(s.dist, 128.0f, 900.0f);
            m_lookPoint       = self.origin + YawDir(s.yaw) * d + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
            m_lookPointValid  = true;
            m_lookMode        = VIEW_SOUND;
            break;
        }
    default:
        {
            const float mag  = static_cast<float>(rng.Uniform(45.0, 150.0));
            const float sign = rng.Bernoulli(0.5) ? 1.0f : -1.0f;
            const float yaw  = self.viewYaw + sign * mag;
            m_lookPoint      = eye + YawDir(yaw) * 400.0f;
            m_lookPointValid = true;
            m_lookMode       = VIEW_LOOKAROUND;
            m_dwellMs *= 0.6f;
            break;
        }
    }
}

void ViewControl::Step(const SelfState& self, const ViewInput& in, Rng& rng, ViewOutput& out)
{
    const ViewModel& p = *m_p;
    out                = ViewOutput();
    // fixed draws per tick
    const double uStill = rng.Uniform();
    const double uFlick = rng.Uniform();

    const Vec3& eye = self.eye;
    Vec3        aim;
    Vec3        tvel;
    bool        newLook = false;

    if (in.track) {
        const float hWant = in.firing ? in.aimHeightFiring : p.aimHeightIdle;
        m_aimH += (hWant - m_aimH) * 0.35f;
        aim  = in.enemyFeet + Vec3(0.0f, 0.0f, m_aimH * in.bodyHeight);
        tvel = in.enemyVel;
        out.mode      = VIEW_TRACK;
        m_wasTracking = true;
        m_damagePending = false;
    } else {
        // turn toward hits from outside the central view
        if (in.damage) {
            for (const DamageObs& d : *in.damage) {
                if (std::fabs(Wrap180(d.yaw - self.viewYaw)) > 60.0f && !m_damagePending) {
                    m_damagePending = true;
                    m_damageAtMs    = self.timeMs + static_cast<int>(p.damageTurnDelayMs);
                    m_damageYaw     = d.yaw;
                }
            }
        }
        if (m_damagePending && self.timeMs >= m_damageAtMs) {
            m_damagePending  = false;
            m_lookPoint      = eye + YawDir(m_damageYaw) * 400.0f;
            m_lookPointValid = true;
            m_lookMode       = VIEW_DAMAGE;
            m_dwellMs        = 900.0f;
            newLook          = true;
        } else {
            m_dwellMs -= TICK_MS;
            const bool heardNew = in.sounds && !in.sounds->empty() && self.timeMs - m_lastSoundMs > 600;
            if (m_dwellMs <= 0.0f || m_wasTracking || heardNew || !m_lookPointValid) {
                ChooseLook(self, in, rng);
                newLook = true;
            }
        }
        if (in.sounds && !in.sounds->empty()) {
            m_lastSoundMs = self.timeMs;
        }
        m_wasTracking = false;
        aim           = m_lookPoint;
        m_aimH += (p.aimHeightIdle - m_aimH) * 0.35f;
        out.mode = m_lookMode;
    }

    const Vec3  d      = aim - eye;
    const float dist   = std::max(1.0f, d.length());
    const float dxy2   = std::max(1.0f, d.x * d.x + d.y * d.y);
    const float tYaw   = YawOf(d);
    const float tPitch = PitchOf(d);
    const float errYaw   = Wrap180(tYaw - self.viewYaw);
    const float errPitch = tPitch - self.viewPitch;
    // angular velocity of the target direction caused by my motion and by the target's
    const float wself = (d.y * self.velocity.x - d.x * self.velocity.y) / dxy2 * RAD2DEG * TICK_S;
    const float wopp  = (d.x * tvel.y - d.y * tvel.x) / dxy2 * RAD2DEG * TICK_S;

    if (!m_histValid) {
        for (float& e : m_err) {
            e = errYaw;
        }
        for (float& e : m_perr) {
            e = errPitch;
        }
        for (float& w : m_wopp) {
            w = wopp;
        }
        for (float& t : m_tpitch) {
            t = tPitch;
        }
        m_histValid = true;
    }
    for (int i = 3; i > 0; i--) {
        m_err[i] = m_err[i - 1];
    }
    m_err[0]  = errYaw;
    m_perr[1] = m_perr[0];
    m_perr[0] = errPitch;
    for (int i = 4; i > 0; i--) {
        m_wopp[i] = m_wopp[i - 1];
    }
    m_wopp[0] = wopp;
    for (int i = 3; i > 0; i--) {
        m_tpitch[i] = m_tpitch[i - 1];
    }
    m_tpitch[0] = tPitch;

    out.targetYaw   = tYaw;
    out.targetPitch = tPitch;
    out.errYaw      = errYaw;
    out.errPitch    = errPitch;
    out.aimHeight   = m_aimH;

    // flick decisions
    if (!m_flick.active) {
        const float angErr = std::sqrt(errYaw * errYaw + errPitch * errPitch);
        if (in.track) {
            const float halfW = std::atan(BODY_HALF_W / dist) * RAD2DEG;
            if (in.acquisition && angErr > p.acquireMinHalfW * halfW && uFlick < p.acquireHazard) {
                StartFlick(errYaw, errPitch, p.flickGainMedian, p.flickGainSigma, rng);
            } else if (std::fabs(errYaw) > p.flickDeg && uFlick < p.trackFlickHazard) {
                StartFlick(errYaw, errPitch, p.flickGainMedian, p.flickGainSigma, rng);
            }
        } else if (newLook && angErr > p.flickDeg) {
            StartFlick(errYaw, errPitch, 0.95f, 0.12f, rng);
        }
    }

    const int K = ClampI(in.substeps, 1, 8);
    if (m_flick.active) {
        const float t0 = m_flick.tMs;
        const float t1 = t0 + TICK_MS;
        const float s0 = MinJerk(t0 / m_flick.durMs);
        const float s1 = MinJerk(t1 / m_flick.durMs);
        out.yawDelta   = m_flick.ampYaw * (s1 - s0);
        out.pitchDelta = m_flick.ampPitch * (s1 - s0);
        out.flick      = true;
        out.flickAmp   = m_flick.ampYaw;
        if (s1 - s0 > 1e-6f) {
            float prev = s0;
            for (int k = 0; k < K; k++) {
                const float sk   = MinJerk((t0 + TICK_MS * (k + 1) / static_cast<float>(K)) / m_flick.durMs);
                out.flickFrac[k] = (sk - prev) / (s1 - s0);
                prev             = sk;
            }
        } else {
            for (int k = 0; k < K; k++) {
                out.flickFrac[k] = 1.0f / K;
            }
        }
        m_flick.tMs = t1;
        if (t1 >= m_flick.durMs) {
            m_flick.active = false;
            m_rate         = 0.0f;
            m_prate        = 0.0f;
            m_noise        = 0.0f;
            m_pnoise       = 0.0f;
        } else {
            m_rate  = out.yawDelta;
            m_prate = out.pitchDelta;
        }
        m_still = false;
    } else {
        // still gate
        const int ctx = ClampI(in.ctx, 0, CTX_COUNT - 1);
        if (m_still) {
            m_still = uStill < p.stillStay[ctx];
        } else {
            m_still = uStill < p.stillEnter[ctx];
        }
        if (m_still) {
            out.still      = true;
            out.yawDelta   = 0.0f;
            out.pitchDelta = 0.0f;
            m_rate         = 0.0f;
            m_prate        = 0.0f;
        } else {
            const YawController&   c  = in.firing ? p.firing : p.idle;
            const PitchController& pc = in.firing ? p.pitchFiring : p.pitchIdle;
            const float            ns = in.noiseScale * p.noiseScale;
            const float noise  = NoiseStep(c.noise, dist, m_noise, ns, rng);
            const float pnoise = NoiseStep(pc.noise, dist, m_pnoise, ns, rng);
            float rate = c.rho * m_rate + c.Kp * m_err[1] + c.Kself * wself + c.Kopp * m_wopp[3] + c.bias * p.biasScale + noise;
            const float tpRate = m_tpitch[2] - m_tpitch[3];
            float prate = pc.rho * m_prate + pc.Kp * m_perr[0] + pc.Kt * tpRate + pc.bias * p.biasScale + pnoise;
            rate  = Clamp(rate, -MAX_RATE_DEG, MAX_RATE_DEG);
            prate = Clamp(prate, -0.5f * MAX_RATE_DEG, 0.5f * MAX_RATE_DEG);
            out.yawDelta   = rate;
            out.pitchDelta = prate;
            out.noiseYaw   = noise;
            m_rate         = rate;
            m_prate        = prate;
        }
    }
    // keep the pitch inside the engine's range
    const float newPitch = Clamp(self.viewPitch + out.pitchDelta, -PITCH_LIMIT, PITCH_LIMIT);
    out.pitchDelta       = newPitch - self.viewPitch;
}

} // namespace hb
