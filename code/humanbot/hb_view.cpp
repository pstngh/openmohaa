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
static constexpr float DIFFUSE_SPREAD = 600.0f;  // belief spread (units) beyond which it no longer points anywhere
// A look along a direction (route, hold, look-around, damage) aims at a point this far away, so the
// bot's own motion does not turn the view (people keep a direction while running, not a point 10 m away).
static constexpr float DIR_LOOK_DIST  = 4000.0f;
// The chest, as a fraction of the body height from the feet (the head starts near 0.83).
static constexpr float CHEST_AIM_H = 0.5f;
// On the move, a route this far off the view is behind it (routeTurnHazard).
static constexpr float ROUTE_TURN_DEG = 100.0f;
// A watched corner follows its own refinements (re-traced from a moved eye) up to this far; a bigger change is
// another edge, which waits for the next look decision.
static constexpr float PREAIM_FOLLOW_DEG = 4.0f;

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
    m_refractory     = 0;
    m_preaimCell     = -1;
    m_beliefDead     = false;
}

// The crosshair's place for a corner: the corner's direction moved onto the cover side and below, like people.
Vec3 ViewControl::CornerAim(const Vec3& eye, const Vec3& corner, float open) const
{
    const Vec3  d    = corner - eye;
    const float dist = std::max(1.0f, d.length());
    const float yaw  = YawOf(d) - open * m_p->preaimCoverDeg;
    const float pit  = PitchOf(d) + m_p->preaimBelowDeg;
    return eye + AnglesForward(pit, yaw) * dist;
}

// Turn onto the corner of one of the exposures, drawn by weight w (the imminence terms); the corner already
// watched weighs more.
void ViewControl::PreaimCorner(const SelfState& self, const ViewInput& in, const double *w, Rng& rng)
{
    const BeliefEstimate *b = in.belief;
    double                ww[MAX_EXPOSURE];
    for (int i = 0; i < b->nExposure; i++) {
        ww[i] = w[i] * (b->exposureCell[i] == m_preaimCell ? 3.0 : 1.0);
    }
    const int i      = rng.Categorical(ww, b->nExposure);
    m_lookPoint      = CornerAim(self.eye, b->corner[i], b->cornerOpen[i]);
    m_lookPointValid = true;
    m_lookMode       = VIEW_PREAIM;
    m_preaimCell     = b->exposureCell[i];
    m_dwellMs        = static_cast<float>(m_p->lookDwellMedianMs * rng.LogNormal(1.0, m_p->lookDwellSigma));
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

// Where to look without a visible enemy. People keep their view close to where they
// believe the enemy is (5-10 degrees for the first seconds after losing sight, about
// 20 later), mostly by watching that position or the corner it will come out of.
// On the move they also look along their route, belief or not: a third of the time
// they move hidden, they move within 30 degrees of their view, and so walk forward
// down the corridor rather than sideways along it. With nothing better they keep
// looking where they look. Sounds reach the view through the belief; look-arounds
// are a separate hazard.
void ViewControl::ChooseLook(const SelfState& self, const ViewInput& in, const double *preW, double imminence, Rng& rng)
{
    const ViewModel&      p    = *m_p;
    const Vec3&           eye  = self.eye;
    const BeliefEstimate *b    = in.belief;
    const bool            live = b && b->valid && !b->dead;
    const bool            focused     = live && b->spread < DIFFUSE_SPREAD;
    const bool            hasExposure = live && b->nExposure > 0;
    double                w[4] = {0.0, 0.0, 0.0, 0.0};   // belief, preaim, travel, hold
    if (focused) {
        w[0] = p.beliefLookShare;
    }
    if (hasExposure) {
        // the corner of an exposure coming up wins over watching the believed position through the wall
        w[1] = p.preaimShare + p.preaimWeight * imminence;
    }
    if (in.moving && in.navValid) {
        w[2] = p.travelShare;
    }
    w[3] = std::max(0.02, 1.0 - w[0] - w[1] - w[2]);
    const int    pick     = rng.Categorical(w, 4);
    const double dwellMul = rng.LogNormal(1.0, p.lookDwellSigma);
    m_dwellMs             = static_cast<float>(p.lookDwellMedianMs * dwellMul);
    const float  lookYaw  = m_lookPointValid ? YawOf(m_lookPoint - eye) : self.viewYaw;
    switch (pick) {
    case 0:
        m_lookPoint      = b->mode + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
        m_lookPointValid = true;
        m_lookMode       = VIEW_BELIEF;
        break;
    case 1:
        {
            if (imminence > 0.0) {
                // the corners of the exposures coming up, by their imminence
                PreaimCorner(self, in, preW, rng);
                break;
            }
            const float beliefYaw = YawOf(b->mode - eye);
            double      ew[MAX_EXPOSURE];
            for (int i = 0; i < b->nExposure; i++) {
                const float ey = YawOf(b->exposure[i] - eye);
                const float db = focused ? Wrap180(ey - beliefYaw) / 30.0f : 0.0f;
                // the corner already watched weighs more
                const float dl = std::fabs(Wrap180(ey - lookYaw));
                ew[i] = b->exposureMass[i] * std::exp(-b->exposureEtaMs[i] / 2500.0 - db * db) * (dl < 20.0f ? 3.0 : 1.0);
            }
            const int i      = rng.Categorical(ew, b->nExposure);
            m_lookPoint      = b->cornerValid[i] ? CornerAim(eye, b->corner[i], b->cornerOpen[i])
                                                 : b->exposure[i] + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
            m_lookPointValid = true;
            m_lookMode       = VIEW_PREAIM;
            m_preaimCell     = b->exposureCell[i];
            break;
        }
    case 2:
        m_lookPoint      = eye + YawDir(in.navYaw) * DIR_LOOK_DIST;
        m_lookPointValid = true;
        m_lookMode       = VIEW_TRAVEL;
        m_dwellMs *= 0.7f;
        break;
    default:
        m_lookPoint      = eye + AnglesForward(self.viewPitch, self.viewYaw) * DIR_LOOK_DIST;
        m_lookPointValid = true;
        m_lookMode       = VIEW_HOLD;
        break;
    }
}

void ViewControl::LookAround(const SelfState& self, Rng& rng)
{
    const ViewModel& p    = *m_p;
    const float      mag  = static_cast<float>(rng.Uniform(45.0, 150.0));
    const float      sign = rng.Bernoulli(0.5) ? 1.0f : -1.0f;
    m_lookPoint           = self.eye + YawDir(self.viewYaw + sign * mag) * DIR_LOOK_DIST;
    m_lookPointValid      = true;
    m_lookMode            = VIEW_LOOKAROUND;
    m_dwellMs             = static_cast<float>(0.6 * p.lookDwellMedianMs * rng.LogNormal(1.0, p.lookDwellSigma));
}

void ViewControl::Step(const SelfState& self, const ViewInput& in, Rng& rng, ViewOutput& out)
{
    const ViewModel& p = *m_p;
    out                = ViewOutput();
    // fixed draws per tick
    const double uStill = rng.Uniform();
    const double uFlick = rng.Uniform();
    const double uLook  = rng.Uniform();
    const double uPre   = rng.Uniform();

    // the imminence of the exposures with a corner: belief mass expected to come out there soon
    double     preW[MAX_EXPOSURE] = {};
    const auto *bel = in.belief;
    if (bel && bel->valid && !bel->dead && !in.track) {
        const double horizon = std::max(50.0, static_cast<double>(p.preaimHorizonMs) * in.angleHold);
        for (int i = 0; i < bel->nExposure; i++) {
            if (bel->cornerValid[i]) {
                preW[i] = bel->exposureMass[i] * std::exp(-bel->exposureEtaMs[i] / horizon);
                out.imminence += static_cast<float>(preW[i]);
            }
        }
    }

    const Vec3& eye = self.eye;
    Vec3        aim;
    Vec3        tvel;
    bool        newLook = false;
    // the belief of a dead enemy came back (it respawned somewhere): people turn toward where it will come from at
    // once, not when their current look runs out
    const bool reborn = p.respawnRelook > 0.5f && m_beliefDead && bel && bel->valid && !bel->dead;
    m_beliefDead      = bel && bel->valid && bel->dead;

    if (in.track) {
        float hWant = in.firing ? in.aimHeightFiring : p.aimHeightIdle;
        if (in.chestOnly) {
            // at once: the next round leaves within 100 ms
            hWant  = std::min(hWant, CHEST_AIM_H);
            m_aimH = std::min(m_aimH, CHEST_AIM_H);
        }
        m_aimH += (hWant - m_aimH) * 0.35f;
        aim  = in.aimPartValid ? in.aimPart : in.enemyFeet + Vec3(0.0f, 0.0f, m_aimH * in.bodyHeight);
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
            m_lookPoint      = eye + YawDir(m_damageYaw) * DIR_LOOK_DIST;
            m_lookPointValid = true;
            m_lookMode       = VIEW_DAMAGE;
            m_dwellMs        = 900.0f;
            newLook          = true;
        } else if (in.firing) {
            // spraying at a hidden enemy: the view stays where it points, no new look starts
            if (m_wasTracking || !m_lookPointValid) {
                m_lookPoint      = eye + AnglesForward(self.viewPitch, self.viewYaw) * DIR_LOOK_DIST;
                m_lookPointValid = true;
                m_lookMode       = VIEW_HOLD;
                m_dwellMs        = p.lookDwellMedianMs;
            }
        } else {
            m_dwellMs -= TICK_MS;
            if (m_lookMode == VIEW_PREAIM && bel) {
                // the watched corner is traced again as the eye moves: follow it, but not onto another edge
                for (int i = 0; i < bel->nExposure; i++) {
                    if (bel->exposureCell[i] == m_preaimCell && bel->cornerValid[i]) {
                        const Vec3 a = CornerAim(eye, bel->corner[i], bel->cornerOpen[i]);
                        if (std::fabs(Wrap180(YawOf(a - eye) - YawOf(m_lookPoint - eye))) < PREAIM_FOLLOW_DEG) {
                            m_lookPoint = a;
                        }
                    }
                }
            }
            if (m_dwellMs <= 0.0f || m_wasTracking || !m_lookPointValid || reborn) {
                ChooseLook(self, in, preW, out.imminence, rng);
                newLook = true;
            } else if (m_lookMode != VIEW_PREAIM && out.imminence > 0.0f && uPre < p.preaimHazard * out.imminence) {
                // an exposure is coming up: turn onto its corner
                PreaimCorner(self, in, preW, rng);
                newLook = true;
            } else if (m_lookMode != VIEW_TRAVEL && in.moving && in.navValid && p.routeTurnHazard > 0.0f
                       && std::fabs(Wrap180(in.navYaw - self.viewYaw)) > ROUTE_TURN_DEG && rng.Uniform() < p.routeTurnHazard) {
                // the route turned away behind the view: people turn to it rather than walk backwards
                m_lookPoint      = eye + YawDir(in.navYaw) * DIR_LOOK_DIST;
                m_lookPointValid = true;
                m_lookMode       = VIEW_TRAVEL;
                m_dwellMs        = static_cast<float>(0.7 * p.lookDwellMedianMs * rng.LogNormal(1.0, p.lookDwellSigma));
                newLook          = true;
            } else if (m_lookMode != VIEW_LOOKAROUND && uLook < p.lookaroundPerMin / 1200.0f) {
                LookAround(self, rng);
                newLook = true;
            } else if (m_lookMode == VIEW_BELIEF && in.belief && in.belief->valid && !in.belief->dead) {
                // follow the believed position as it moves, in steps once it moved far enough
                const Vec3 want = in.belief->mode + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
                if (p.beliefFollowDeg <= 0.0f || std::fabs(Wrap180(YawOf(want - eye) - YawOf(m_lookPoint - eye))) > p.beliefFollowDeg) {
                    m_lookPoint = want;
                }
            } else if (m_lookMode == VIEW_TRAVEL && in.moving && in.navValid) {
                // look down the route as it turns, in steps, like a person following a corridor
                if (std::fabs(Wrap180(in.navYaw - YawOf(m_lookPoint - eye))) > p.travelFollowDeg) {
                    m_lookPoint = eye + YawDir(in.navYaw) * DIR_LOOK_DIST;
                }
            }
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
    // The recorded aim heights are measured from the unleaned eye (a lean lowers the camera by up
    // to 7 u), so the pitch aims from there; the yaw aims from the camera like people do.
    const Vec3  dp     = self.aimEyeValid ? aim - self.aimEye : d;
    const float tPitch = PitchOf(dp) + (in.track ? (in.firing ? p.pitchOffsetFiring : p.pitchOffsetIdle) : 0.0f);
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
    out.aimValid    = in.track && in.detected;
    out.targetYawRate = wself + wopp;
    out.halfWidthDeg  = std::atan(BODY_HALF_W / dist) * RAD2DEG;
    out.halfHeightDeg = std::atan(0.4f * in.bodyHeight / dist) * RAD2DEG;
    out.aimHeight   = m_aimH;

    // flick decisions
    const float angErr     = std::sqrt(errYaw * errYaw + errPitch * errPitch);
    const float halfW      = std::atan(BODY_HALF_W / dist) * RAD2DEG;
    const bool  offTarget  = in.track && angErr > p.acquireMinHalfW * halfW;
    if (m_refractory > 0) {
        m_refractory--;
    }
    if (!m_flick.active && m_refractory == 0) {
        if (in.track) {
            // corrective submovements toward a seen target: fast right after the sighting
            const float hz = in.acquisition ? p.acquireHazard : p.trackFlickHazard;
            if ((offTarget || std::fabs(errYaw) > p.flickDeg) && uFlick < hz) {
                StartFlick(errYaw, errPitch, p.flickGainMedian, p.flickGainSigma, rng);
            }
        } else if (newLook && angErr > (m_lookMode == VIEW_PREAIM ? p.preaimFlickDeg : p.flickDeg)) {
            StartFlick(errYaw, errPitch, 0.95f, 0.12f, rng);
        } else if (p.hiddenReaimHazard > 0.0f && angErr > p.hiddenReaimDeg && uFlick < p.hiddenReaimHazard) {
            // the view drifted off what it watches (own motion, the believed position moved): re-aim in one turn
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
            m_refractory   = p.flickRefractoryTicks;
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
        // still gate (not while still reacting to a fresh sighting)
        const int ctx = ClampI(in.ctx, 0, CTX_COUNT - 1);
        if (in.acquisition && offTarget) {
            m_still = false;
        } else {
            // people who stop moving mostly stop turning too
            const bool standing = !p.stillEnterStanding.empty() && self.velocity.lengthXY() < p.standingSpeed;
            if (m_still) {
                m_still = uStill < Sigmoid(Logit((standing ? p.stillStayStanding : p.stillStay)[ctx]) + p.stillLogit[ctx]);
            } else {
                m_still = uStill < Sigmoid(Logit((standing ? p.stillEnterStanding : p.stillEnter)[ctx]) + p.stillLogit[ctx]);
            }
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
            // without a visible enemy the hand rests: the noise fitted while tracking is too lively
            const float            ns = in.noiseScale * p.noiseScale * (in.track ? 1.0f : p.hiddenNoiseScale);
            const float noise  = NoiseStep(c.noise, dist, m_noise, ns, rng);
            const float pnoise = NoiseStep(pc.noise, dist, m_pnoise, ns, rng);
            // a watched corner is held like a target as the enemy is expected out of it (the tracking gain closes the
            // gap the bot's motion opens; people's crosshair is a median 5.8 deg from it 500 ms before), else looked at
            // like any point: holding near corners while running past them turned the hidden view 2-3x as fast as
            // people's (corners at a median 170 u sweep past at 22 deg/s)
            float g  = in.track ? p.trackGainScale : 1.0f;
            float ks = c.Kself;
            if (!in.track && m_lookMode == VIEW_PREAIM && bel && bel->valid && !bel->dead) {
                const double horizon = std::max(50.0, static_cast<double>(p.preaimHorizonMs) * in.angleHold);
                for (int i = 0; i < bel->nExposure; i++) {
                    if (bel->exposureCell[i] == m_preaimCell) {
                        const float imm = static_cast<float>(std::exp(-bel->exposureEtaMs[i] / horizon));
                        g  = 1.0f + (p.trackGainScale - 1.0f) * imm;
                        ks = c.Kself + (p.preaimSelfComp - c.Kself) * imm;
                    }
                }
            }
            float rate = c.rho * m_rate + c.Kp * g * m_err[1] + ks * wself + c.Kopp * g * m_wopp[3] + c.bias * p.biasScale + noise;
            const float tpRate = m_tpitch[2] - m_tpitch[3];
            float prate = pc.rho * m_prate + pc.Kp * p.pitchGainScale * m_perr[0] + pc.Kt * tpRate + pc.bias * p.biasScale + pnoise;
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
