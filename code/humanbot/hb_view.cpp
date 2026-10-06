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
// a noise behind is turned to on the corner nearest its direction when one lies this close (the sound itself is
// 10-20 deg off)
static constexpr float SOUND_CORNER_DEG = 45.0f;
// Travel mode, left for the corners once the enemy is expected soon, comes back below this share of the threshold.
static constexpr float TRAVEL_REENTER = 0.5f;
// The enemy believed near (ViewModel::travelFar) stays near until believed this much further off.
static constexpr float NEAR_LEAVE = 1.15f;
// A sound's direction is kept at least this long before it may give way to where the enemy is believed to be.
static constexpr float NEAR_SOUND_MIN_MS = 600.0f;

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
    m_travel         = false;
    m_doorLook       = false;
    m_near           = false;
    m_lookOwed       = false;
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

// Running past a corner people keep a direction rather than the corner: the bots held corners while they ran
// past them (a third of their corner time was on corners within 120 u, the view turning at 33 deg/s there) and
// turned their hidden view twice as fast as people. A corner straight ahead stays put in the view; one beside
// the way sweeps past at v_perp / dist.
bool ViewControl::PassingCorner(const SelfState& self, const ViewInput& in, const Vec3& corner) const
{
    if (!in.moving || m_p->preaimPassDps <= 0.0f) {
        return false;
    }
    const Vec3  d     = corner - self.eye;
    const float sweep = std::fabs(d.y * self.velocity.x - d.x * self.velocity.y) / std::max(1.0f, d.x * d.x + d.y * d.y) * RAD2DEG;
    return sweep > m_p->preaimPassDps;
}

// People watch the corner a hidden enemy will come out of rather than its believed position through the wall: of
// the corners the belief offered 500 ms before a sighting, the heaviest and the one nearest the believed position's
// direction were within 5 deg of the corner the enemy came out of in 56-57% of the sightings, the view in 20%.
int ViewControl::CornerNearBelief(const SelfState& self, const ViewInput& in) const
{
    const BeliefEstimate *b = in.belief;
    if (!b || !b->valid || b->dead) {
        return -1;
    }
    const float my   = YawOf(b->mode - self.eye);
    int         best = -1;
    double      bw   = 0.0;
    for (int i = 0; i < b->nExposure; i++) {
        if (!b->cornerValid[i] || PassingCorner(self, in, b->corner[i])) {
            continue;
        }
        const double dy = Wrap180(YawOf(b->corner[i] - self.eye) - my) / 30.0;
        const double w  = b->exposureMass[i] * std::exp(-dy * dy);
        if (w > bw) {
            bw   = w;
            best = i;
        }
    }
    return best;
}

// The exposure whose corner lies nearest a direction (within maxDeg), by mass (-1: none).
int ViewControl::CornerNearYaw(const SelfState& self, const ViewInput& in, float yaw, float maxDeg) const
{
    const BeliefEstimate *b = in.belief;
    if (!b || !b->valid || b->dead) {
        return -1;
    }
    int    best = -1;
    double bw   = 0.0;
    for (int i = 0; i < b->nExposure; i++) {
        if (!b->cornerValid[i] || PassingCorner(self, in, b->corner[i])) {
            continue;
        }
        const float dy = std::fabs(Wrap180(YawOf(b->corner[i] - self.eye) - yaw));
        if (dy > maxDeg) {
            continue;
        }
        const double z = dy / 30.0;
        const double w = b->exposureMass[i] * std::exp(-z * z);
        if (w > bw) {
            bw   = w;
            best = i;
        }
    }
    return best;
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
    if (in.moving && in.navValid && !m_near) {
        // (with the enemy believed near his side is watched, not the route)
        w[2] = p.travelShare;
    }
    w[3] = std::max(0.02, 1.0 - w[0] - w[1] - w[2]);
    const int    pick     = rng.Categorical(w, 4);
    const double dwellMul = rng.LogNormal(1.0, p.lookDwellSigma);
    m_dwellMs             = static_cast<float>(p.lookDwellMedianMs * dwellMul);
    const float  lookYaw  = m_lookPointValid ? YawOf(m_lookPoint - eye) : self.viewYaw;
    switch (pick) {
    case 0:
        {
            const int c = p.beliefLookCorner > 0.5f ? CornerNearBelief(self, in) : -1;
            if (c >= 0) {
                m_lookPoint  = CornerAim(eye, b->corner[c], b->cornerOpen[c]);
                m_lookMode   = VIEW_PREAIM;
                m_preaimCell = b->exposureCell[c];
            } else {
                m_lookPoint = b->mode + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
                m_lookMode  = VIEW_BELIEF;
            }
            m_lookPointValid = true;
            break;
        }
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
                if (b->cornerValid[i] && PassingCorner(self, in, b->corner[i])) {
                    ew[i] = 0.0;
                }
                if (m_near && p.nearAwayDeg > 0.0f && focused && std::fabs(Wrap180(ey - beliefYaw)) > p.nearAwayDeg) {
                    ew[i] = 0.0;
                }
            }
            double ewSum = 0.0;
            for (int k = 0; k < b->nExposure; k++) {
                ewSum += ew[k];
            }
            if (ewSum <= 0.0 && m_near && focused) {
                // the enemy near and no corner his way: watch where he is believed to be
                const int c = p.beliefLookCorner > 0.5f ? CornerNearBelief(self, in) : -1;
                if (c >= 0 && !(p.nearAwayDeg > 0.0f && std::fabs(Wrap180(YawOf(b->corner[c] - eye) - beliefYaw)) > p.nearAwayDeg)) {
                    m_lookPoint  = CornerAim(eye, b->corner[c], b->cornerOpen[c]);
                    m_lookMode   = VIEW_PREAIM;
                    m_preaimCell = b->exposureCell[c];
                } else {
                    m_lookPoint = b->mode + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
                    m_lookMode  = VIEW_BELIEF;
                }
                m_lookPointValid = true;
                break;
            }
            if (ewSum <= 0.0) {
                // only corners being passed: keep looking where the view points
                m_lookPoint      = eye + AnglesForward(self.viewPitch, self.viewYaw) * DIR_LOOK_DIST;
                m_lookPointValid = true;
                m_lookMode       = VIEW_HOLD;
                break;
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
    if (m_near && focused && p.nearAwayDeg > 0.0f && std::fabs(Wrap180(YawOf(m_lookPoint - eye) - YawOf(b->mode - eye))) > p.nearAwayDeg) {
        // the enemy near: not a look away from him
        m_lookPoint = b->mode + Vec3(0.0f, 0.0f, HEAD_HEIGHT);
        m_lookMode  = VIEW_BELIEF;
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
    const double uSound = rng.Uniform();

    // People watch the side of a hidden enemy they know is near, whichever way they go: within 600 u (dm/brownffa,
    // dm/flag) their view is a median 7-25 deg off his direction even when their way goes 60-180 deg elsewhere, and
    // only further off do they look along the way more than at him. The bots led the view along the way whenever no
    // exposure was expected soon (a spread belief of a still enemy expects none), so on dm/flag they came through the
    // doors to the owner, who held the flag room, looking down the corridor (17 of the 40 lives he saw first)
    const auto *bel   = in.belief;
    const bool  focus = bel && bel->valid && !bel->dead && bel->spread < DIFFUSE_SPREAD;
    if (p.travelFar > 0.0f && focus) {
        const float dist = (bel->mode - self.origin).lengthXY();
        m_near           = dist < (m_near ? NEAR_LEAVE * p.travelFar : p.travelFar);
    } else {
        m_near = false;
    }
    const float beliefYaw = focus ? YawOf(bel->mode - self.eye) : 0.0f;
    // the imminence of the exposures with a corner: belief mass expected to come out there soon (near, on his side)
    double     preW[MAX_EXPOSURE] = {};
    if (bel && bel->valid && !bel->dead && !in.track) {
        const double horizon = std::max(50.0, static_cast<double>(p.preaimHorizonMs) * in.angleHold);
        for (int i = 0; i < bel->nExposure; i++) {
            if (bel->cornerValid[i] && !PassingCorner(self, in, bel->corner[i])) {
                preW[i] = bel->exposureMass[i] * std::exp(-bel->exposureEtaMs[i] / horizon);
                if (m_near) {
                    const float dy = Wrap180(YawOf(bel->corner[i] - self.eye) - beliefYaw);
                    if (p.nearSideDeg > 0.0f) {
                        const double z = dy / p.nearSideDeg;
                        preW[i] *= std::exp(-z * z);
                    }
                    if (p.nearAwayDeg > 0.0f && std::fabs(dy) > p.nearAwayDeg) {
                        preW[i] = 0.0;
                    }
                }
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
        m_travel      = false;
    } else {
        // Travelling with no enemy expected soon, people look where they go: their view is a median 20 deg off the way
        // to where they are a second later (the bots' 24-33 deg) and within 45 deg of their motion 75% of the time (the
        // bots' 42-45%), and they hold forward or a forward diagonal. The bots looked at corners and the believed
        // position, steered with the keys alone, ran past turns of the path and back (a third of their 3 s runs ended
        // where they began, people's a tenth). The view leads along the way, looking at a point ahead on the path. It
        // gives way to the corners once the enemy is expected soon, and comes back only once it is expected half as
        // soon (near the threshold the view flipped between a corner and the way 16 times a minute)
        const float immMax = m_travel ? p.travelImminence : TRAVEL_REENTER * p.travelImminence;
        const bool  travel = p.travelLead > 0.0f && in.travelling && in.aheadValid && out.imminence < immMax && !m_near;
        // turn toward a hit from an enemy not in sight (one in sight is tracked), wherever the hit is felt to come
        // from. Only hits felt more than 60 deg off used to turn the view: the felt direction is +-20 deg off, and the
        // bot sees 48 deg to each side, so a shooter just outside the view often went unanswered (the owner shot a
        // bot from 61 deg off and its view did not move for 1.3 s). People hit by an enemy with no part of it on
        // screen 48-75 deg off face it within half a second 87-91% of the time
        if (in.damage) {
            for (const DamageObs& d : *in.damage) {
                if (!m_damagePending || m_pendingMode != VIEW_DAMAGE) {
                    m_damagePending = true;
                    m_damageAtMs    = self.timeMs + static_cast<int>(p.damageTurnDelayMs);
                    m_damageYaw     = d.yaw;
                    m_pendingMode   = VIEW_DAMAGE;
                }
            }
        }
        // and toward an enemy heard behind: people with an unseen enemy running or firing behind them within 1000 u
        // face it within a second 63% of the time and are hit first 7% (the bots, without this, 38% and 20%)
        if (!m_damagePending && in.sounds && p.soundTurnP > 0.0f) {
            for (const SoundObs& s : *in.sounds) {
                if ((s.type == SOUND_FOOTSTEP || s.type == SOUND_GUNFIRE) && std::fabs(Wrap180(s.yaw - self.viewYaw)) > p.soundTurnDeg) {
                    if (uSound < p.soundTurnP) {
                        m_damagePending = true;
                        m_damageAtMs    = self.timeMs + static_cast<int>(p.soundTurnDelayMs);
                        m_damageYaw     = s.yaw;
                        m_pendingMode   = VIEW_SOUND;
                    }
                    break;
                }
            }
        }
        if (m_damagePending && self.timeMs >= m_damageAtMs) {
            m_damagePending  = false;
            m_lookPoint      = eye + YawDir(m_damageYaw) * DIR_LOOK_DIST;
            if (m_pendingMode == VIEW_SOUND) {
                // a noise behind: the corner it will come out of, when the belief offers one that way. The sound moved the
                // belief onto the ground it can come from (its direction is 10-20 deg off): the corner nearest the
                // believed position, when that lies the sound's way
                int c = -1;
                if (p.soundBeliefCorner > 0.5f && focus && std::fabs(Wrap180(beliefYaw - m_damageYaw)) < SOUND_CORNER_DEG) {
                    c = CornerNearBelief(self, in);
                    if (c < 0) {
                        m_lookPoint = eye + YawDir(beliefYaw) * DIR_LOOK_DIST;
                    }
                }
                if (c < 0) {
                    c = CornerNearYaw(self, in, m_damageYaw, SOUND_CORNER_DEG);
                }
                if (c >= 0) {
                    m_lookPoint = CornerAim(eye, bel->corner[c], bel->cornerOpen[c]);
                }
            }
            m_lookPointValid = true;
            m_lookMode       = m_pendingMode;
            m_dwellMs        = m_pendingMode == VIEW_SOUND ? p.soundTurnHoldMs : 900.0f;
            newLook          = true;
            m_travel         = false;
        } else if (in.firing) {
            // spraying at a hidden enemy: the view stays where it points, no new look starts
            if (m_wasTracking || !m_lookPointValid || m_travel) {
                m_travel         = false;
                m_lookPoint      = eye + AnglesForward(self.viewPitch, self.viewYaw) * DIR_LOOK_DIST;
                m_lookPointValid = true;
                m_lookMode       = VIEW_HOLD;
                m_dwellMs        = p.lookDwellMedianMs;
            }
        } else {
            m_dwellMs -= TICK_MS;
            if (m_lookMode == VIEW_PREAIM && bel) {
                // the watched corner is traced again as the eye moves: follow it, but not onto another edge
                const float lookYaw = YawOf(m_lookPoint - eye);
                int         follow  = -1;
                float       best    = PREAIM_FOLLOW_DEG;
                Vec3        fa;
                for (int i = 0; i < bel->nExposure; i++) {
                    // by geometry the edge is followed whichever exposure offers it (the exposure cells change as
                    // the cloud and the bot move: a fifth of the sightings found the view on a corner no longer offered)
                    if (bel->cornerValid[i] && (p.preaimFollowGeom > 0.5f || bel->exposureCell[i] == m_preaimCell)) {
                        const Vec3  a  = CornerAim(eye, bel->corner[i], bel->cornerOpen[i]);
                        const float dy = std::fabs(Wrap180(YawOf(a - eye) - lookYaw));
                        if (dy < best) {
                            best   = dy;
                            follow = i;
                            fa     = a;
                        }
                    }
                }
                if (follow >= 0) {
                    m_lookPoint  = fa;
                    m_preaimCell = bel->exposureCell[follow];
                }
            }
            if (m_lookMode == VIEW_PREAIM && PassingCorner(self, in, m_lookPoint)) {
                // running past the watched corner: keep its direction, not the point
                m_lookPoint = eye + AnglesForward(self.viewPitch, YawOf(m_lookPoint - eye)) * DIR_LOOK_DIST;
                m_lookMode  = VIEW_HOLD;
            }
            // a held direction gives way once the enemy is believed out of view of it: people look where they think
            // the enemy is (first seen after their spawn, their view is 5 deg off it at the median), and a direction kept
            // after running past a corner left the bots looking 60 deg away from a belief right within 30 deg
            const float holdDeg = m_near && p.nearHoldDeg > 0.0f ? p.nearHoldDeg : p.holdBeliefDeg;
            const bool holdOff = m_lookMode == VIEW_HOLD && holdDeg > 0.0f && bel && bel->valid && !bel->dead
                                 && bel->spread < DIFFUSE_SPREAD
                                 && std::fabs(Wrap180(YawOf(bel->mode - eye) - YawOf(m_lookPoint - eye))) > holdDeg;
            // the enemy near: a corner, a sound's direction, a route or a held direction that ended up far off where he is
            // believed to be gives way (a sound's direction is held while the bot runs on, so it drifts off him: at the
            // sightings where the bots' view was over 90 deg off the enemy their belief was within 30 deg of him 75% of
            // the time). A sound is turned to first
            const float soundAge = m_lookMode == VIEW_SOUND ? p.soundTurnHoldMs - m_dwellMs : 1e9f;
            // (a sound's direction gives way sooner: it is 10-20 deg off where he is, and the belief the sound moved goes
            // on refining while the view held the direction for 3 s; a third of the bots seen with the view over 20 deg off
            // the one holding dm/flag's flag room were holding such a direction)
            const float awayDeg  = m_lookMode == VIEW_SOUND && p.nearSoundDeg > 0.0f ? p.nearSoundDeg : p.nearAwayDeg;
            const bool  awayOff  = m_near && awayDeg > 0.0f && m_lookPointValid && !m_wasTracking
                                  && (m_lookMode == VIEW_PREAIM || m_lookMode == VIEW_HOLD || (m_lookMode == VIEW_TRAVEL && !m_travel)
                                      || (m_lookMode == VIEW_SOUND && soundAge > NEAR_SOUND_MIN_MS))
                                  && std::fabs(Wrap180(beliefYaw - YawOf(m_lookPoint - eye))) > awayDeg;
            // on the way (travel): a turn to a sound or a hit and a look-around run their course
            const bool busy   = (m_lookMode == VIEW_SOUND || m_lookMode == VIEW_DAMAGE || m_lookMode == VIEW_LOOKAROUND)
                              && m_dwellMs > 0.0f && !m_wasTracking && m_lookPointValid;
            if (p.travelLead > 0.0f && in.doorValid && !(busy && m_lookMode != VIEW_LOOKAROUND)) {
                // a closed door across the way: look at it (the use key opens what the view is on). Its direction, not the
                // point: the point is found afresh each tick along the way from the eye, so as a near fixed point the
                // view's own-motion term swung the view off it (a bot strafing through dm/flag's flag door turned
                // 105 deg away from the room in half a second)
                m_lookPoint      = eye + AnglesForward(PitchOf(in.door - eye), YawOf(in.door - eye)) * DIR_LOOK_DIST;
                m_travelYaw      = YawOf(in.door - eye);
                m_lookPointValid = true;
                m_lookMode       = VIEW_TRAVEL;
                newLook          = !m_travel || !m_doorLook;
                m_travel         = true;
                m_doorLook       = true;
            } else if (travel && !busy) {
                m_doorLook = false;
                if (m_travel && uLook < p.lookaroundPerMin / 1200.0f) {
                    LookAround(self, rng);
                    m_travel = false;
                    newLook  = true;
                } else {
                    Vec3 a = in.ahead;
                    a.z += eye.z - self.origin.z;
                    const Vec3 da = a - eye;
                    m_travelYaw   = m_travel && m_lookMode == VIEW_TRAVEL
                                      ? Wrap180(m_travelYaw + p.travelSmooth * Wrap180(YawOf(da) - m_travelYaw))
                                      : YawOf(da);
                    m_lookPoint      = eye + AnglesForward(PitchOf(da), m_travelYaw) * DIR_LOOK_DIST;
                    m_lookPointValid = true;
                    m_lookMode       = VIEW_TRAVEL;
                    newLook          = !m_travel;
                    m_travel         = true;
                }
            } else if (m_dwellMs <= 0.0f || m_wasTracking || !m_lookPointValid || reborn || holdOff || awayOff || m_travel) {
                // (the way led the view until now: the enemy is expected soon, or the bot stopped travelling)
                m_travel = false;
                ChooseLook(self, in, preW, out.imminence, rng);
                newLook = true;
            } else if (m_lookMode != VIEW_PREAIM && m_lookMode != VIEW_SOUND && out.imminence > 0.0f
                       && uPre < p.preaimHazard * out.imminence) {
                // an exposure is coming up: turn onto its corner
                PreaimCorner(self, in, preW, rng);
                newLook = true;
            } else if (m_lookMode != VIEW_TRAVEL && m_lookMode != VIEW_SOUND && in.moving && in.navValid && p.routeTurnHazard > 0.0f
                       && !(m_near && p.nearRouteTurn < 0.5f)
                       && std::fabs(Wrap180(in.navYaw - self.viewYaw)) > ROUTE_TURN_DEG && rng.Uniform() < p.routeTurnHazard) {
                // the route turned away behind the view: people turn to it rather than walk backwards (not with the enemy
                // believed near, whose side they watch whichever way they go: the turn to the route, then back to him once
                // it was 90 deg off him, kept the bots' view swinging near doorways)
                m_lookPoint      = eye + YawDir(in.navYaw) * DIR_LOOK_DIST;
                m_lookPointValid = true;
                m_lookMode       = VIEW_TRAVEL;
                m_dwellMs        = static_cast<float>(0.7 * p.lookDwellMedianMs * rng.LogNormal(1.0, p.lookDwellSigma));
                newLook          = true;
            } else if (m_lookMode != VIEW_LOOKAROUND && m_lookMode != VIEW_SOUND && !(m_near && p.nearLookaround < 0.5f)
                       && uLook < p.lookaroundPerMin / 1200.0f) {
                // (no look-around with the enemy believed near: the bots' hidden view turns 90 deg or more twice as often
                // as people's)
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
    out.travel = m_travel;
    out.near   = m_near && !in.track;
    if (!m_travel) {
        m_doorLook = false;
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
    // with the enemy near, a hidden look is corrected from closer (people's crosshair is a median 4-5 deg off an enemy
    // the moment he first has them on screen; the bots' 9-14, their quick turns landing 5-10 deg short or wide and the
    // slow controller and the still mouse leaving it there)
    const float reaimDeg = m_near && p.nearReaimDeg > 0.0f ? p.nearReaimDeg : p.hiddenReaimDeg;
    const float reaimHz  = m_near && p.nearReaimHazard > 0.0f ? p.nearReaimHazard : p.hiddenReaimHazard;
    const float halfW      = std::atan(BODY_HALF_W / dist) * RAD2DEG;
    const bool  offTarget  = in.track && angErr > p.acquireMinHalfW * halfW;
    if (m_refractory > 0) {
        m_refractory--;
    }
    // a look that begins while the view is still turning to the last one (or just after) is turned to once that turn
    // is done: a quarter of the bots' hidden looks began so and were never turned to (the view 21 deg off the new
    // target when the old turn ended, the idle controller drifting there over seconds)
    if (in.track) {
        m_lookOwed = false;
    } else if (newLook && p.lookChain > 0.5f && (m_flick.active || m_refractory > 0)) {
        m_lookOwed = true;
    }
    if (!m_flick.active && m_refractory == 0) {
        const bool owed = m_lookOwed;
        m_lookOwed      = false;
        if (in.track) {
            // corrective submovements toward a seen target: fast right after the sighting
            const float hz = in.acquisition ? p.acquireHazard : p.trackFlickHazard;
            if ((offTarget || std::fabs(errYaw) > p.flickDeg) && uFlick < hz) {
                StartFlick(errYaw, errPitch, p.flickGainMedian, p.flickGainSigma, rng);
            }
        } else if ((newLook || owed) && angErr > (m_lookMode == VIEW_PREAIM ? p.preaimFlickDeg : p.flickDeg)) {
            StartFlick(errYaw, errPitch, 0.95f, 0.12f, rng);
        } else if (m_travel && std::fabs(errYaw) > p.travelFlickDeg && uFlick < p.travelFlickHazard) {
            // the way turned: one sweep onto it
            StartFlick(errYaw, errPitch, 0.95f, 0.12f, rng);
        } else if (!m_travel && reaimHz > 0.0f && angErr > reaimDeg && uFlick < reaimHz) {
            // the view drifted off what it watches (own motion, the believed position moved): re-aim in one turn
            StartFlick(errYaw, errPitch, 0.95f, 0.12f, rng);
        }
    }

    const int K = ClampI(in.substeps, 1, MAX_SUBSTEPS);
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
        if (m_travel && std::fabs(errYaw) > p.travelStillDeg) {
            // the mouse does not rest while the way turns off the view
            m_still = false;
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
