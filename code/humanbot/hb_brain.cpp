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
// hb_brain.cpp: the per-tick pipeline of one bot.

#include "hb_brain.h"

#include <algorithm>
#include <cmath>

namespace hb
{

static constexpr int   TRACK_MEMORY_MS   = 350;   // keep tracking a just-lost enemy this long
static constexpr int   ACQUISITION_TICKS = 10;
static constexpr int   CLICK_RETRY_MS    = 600;
static constexpr float BODY_HALF_W       = 15.0f;

void Brain::Init(const ModelBundle *bundle, const MapPrior *map, const StyleDials& dials, uint64_t seed, int substeps)
{
    m_bundle   = bundle;
    m_map      = map;
    m_dials    = dials;
    m_seed     = seed;
    m_substeps = substeps;
    m_off      = ComputeOffsets(dials, bundle->calib, bundle->shared);

    const Rng root(seed);
    m_rngBelief  = root.Derive(STREAM_BELIEF);
    m_rngView    = root.Derive(STREAM_VIEW);
    m_rngTrigger = root.Derive(STREAM_TRIGGER);
    m_rngMove    = root.Derive(STREAM_MOVEMENT);
    m_rngStance  = root.Derive(STREAM_STANCE);
    m_rngNav     = root.Derive(STREAM_NAV);
    m_rngWeapon  = root.Derive(STREAM_WEAPON);
    m_rngLife    = root.Derive(STREAM_LIFE);

    const SharedModel& s = bundle->shared;
    m_belief.Init(&s.belief, &s.perception, map, m_rngBelief);
    m_view.Init(&s.view);
    m_trigger.Init(&s.trigger);
    m_mover.Init(&s.movement, &s.spawn);
    m_nav.Init(&s.nav, map);
    m_weapon.Init(&s.weapon);
    m_navOut = NavOutput();
    m_alive  = false;
}

void Brain::SetMap(const MapPrior *map)
{
    m_map = map;
    m_belief.SetMap(map);
    m_nav.SetMap(map);
}

void Brain::SetFov(float hfovDeg, float vfovDeg)
{
    m_hfov = hfovDeg;
    m_vfov = vfovDeg;
}

static int DrawIndex(const std::vector<float>& pmf, Rng& rng)
{
    std::vector<double> w(pmf.begin(), pmf.end());
    return w.empty() ? 0 : rng.Categorical(w);
}

void Brain::OnSpawn(const Observation& obs)
{
    const SpawnModel& sp = m_bundle->shared.spawn;
    m_spawnMs    = obs.self.timeMs;
    // a human client keeps sending empty usercmds for 2-4 ticks after the respawn
    m_liveTick   = -DrawIndex(sp.deadTicksPmf, m_rngLife);
    m_spawnChord = DrawIndex(sp.chordP, m_rngLife);
    m_click      = false;
    m_clickOver  = false;
    m_attackPrev = false;
    m_view.Reset(obs.self);
    m_trigger.Reset();
    m_mover.Reset();
    m_nav.Reset();
    m_weapon.Reset();
    m_navOut    = NavOutput();
    m_los       = false;
    m_lageMs    = 100000;
    m_detected  = false;
    m_acqTicks  = 1000;
    m_clickDown = false;
}

void Brain::OnDeath(const Observation& obs)
{
    m_deathMs     = obs.self.timeMs;
    m_respawnAtMs = m_deathMs + m_weapon.RespawnDelayMs(m_rngLife);
    m_clickDown   = false;
    m_attackPrev  = false;
}

void Brain::Think(const Observation& obs, TickPlan& plan, Diag *diag)
{
    plan                  = TickPlan();
    const SelfState& self = obs.self;
    const int        now  = self.timeMs;
    const SharedModel& S  = m_bundle->shared;

    if (diag) {
        *diag            = Diag();
        diag->family     = m_dials.family;
        diag->style_seed = static_cast<int>(m_dials.seed);
        diag->substeps   = m_substeps;
    }

    //
    // Dead or spectating: click to respawn after a human delay
    //
    if (!self.alive || self.spectator) {
        if (m_alive) {
            OnDeath(obs);
        }
        m_alive    = false;
        plan.owner = OWNER_DEAD;
        m_belief.Update(obs, m_hfov, m_vfov);
        if (m_clickDown) {
            m_clickDown = false;  // release: the engine needs a fresh press
        } else if (now >= m_respawnAtMs) {
            plan.attack   = true;
            m_clickDown   = true;
            m_respawnAtMs = now + CLICK_RETRY_MS;
        }
        plan.viewStill = true;
        if (diag) {
            diag->owner = OWNER_DEAD;
        }
        return;
    }
    if (!m_alive) {
        OnSpawn(obs);
        m_alive = true;
    }
    const int liveTick = m_liveTick;
    if (m_liveTick < 100000) {
        m_liveTick++;
    }
    if (liveTick < 0) {
        // the dead time after a respawn: no keys, no run bit, no mouse, no fire
        m_belief.Update(obs, m_hfov, m_vfov);
        plan.owner     = OWNER_BRAIN;
        plan.walk      = true;
        plan.viewStill = true;
        if (diag) {
            diag->owner = OWNER_BRAIN;
            diag->walk  = 1;
            diag->still = 1;
            diag->chord = CHORD_NEUTRAL;
        }
        return;
    }

    //
    // Belief and the focus enemy
    //
    m_belief.Update(obs, m_hfov, m_vfov);
    const int             fi = m_belief.Focus();
    const BeliefEstimate *fb = fi >= 0 ? &m_belief.Track(fi) : nullptr;
    const EnemyObs       *fe = nullptr;
    if (fb) {
        for (const EnemyObs& eo : obs.enemies) {
            if (eo.id == fb->enemyId) {
                fe = &eo;
            }
        }
    }
    const bool detected = fe && fe->detected;
    if (detected && (!m_detected || (fb && fb->enemyId != m_focusId))) {
        m_acqTicks = 0;
    } else if (m_acqTicks < 1000) {
        m_acqTicks++;
    }
    m_detected = detected;
    m_focusId  = fb ? fb->enemyId : -1;
    if (obs.gotKillOf >= 0) {
        m_killMs = now;
    }

    // logger-equivalent LOS with the focus enemy (frustum gated) and its age
    const bool los = detected && fe->centroidLos;
    if (los != m_los) {
        m_los    = los;
        m_lageMs = 0;
    } else {
        m_lageMs = std::min(m_lageMs + TICK_MS, 100000);
    }

    //
    // Context of the previous tick (one-tick lag)
    //
    const bool reloading  = self.weaponState == 5;
    const bool attackPrev = m_attackPrev;
    int        ctx;
    if (reloading) {
        ctx = CTX_RELOAD;
    } else if (los) {
        ctx = attackPrev ? CTX_LOS_FIRE : CTX_LOS_NOFIRE;
    } else {
        ctx = attackPrev ? CTX_HIDDEN_FIRE : CTX_HIDDEN_NOFIRE;
    }

    //
    // Trigger
    //
    TriggerInput ti;
    ti.canFire = (self.weaponState == 0 || self.weaponState == 1) && self.clipAmmo > 0 && !self.switching
              && self.weaponClass != WEAPON_CLASS_NONE && self.weaponClass != WEAPON_CLASS_GRENADE;
    ti.los    = los;
    ti.lageMs = m_lageMs;
    float enemyDist = 1000.0f;
    Vec3  enemyFeet;
    bool  enemyKnown = false;
    if (detected) {
        const Vec3  d    = fe->pos - self.eye;
        const float dist = std::max(1.0f, d.length());
        const Vec3  fwd  = AnglesForward(self.viewPitch, self.viewYaw);
        const float c    = Clamp(fwd.dot(d) / dist, -1.0f, 1.0f);
        const float err  = std::acos(c) * RAD2DEG;
        const float hw   = std::atan(BODY_HALF_W / dist) * RAD2DEG;
        ti.errHalfWidths = err / std::max(0.05f, hw);
        enemyFeet        = fe->pos - Vec3(0.0f, 0.0f, 0.5f * fe->bodyHeight);
        enemyKnown       = true;
    } else if (fb && fb->valid && !fb->dead) {
        enemyFeet  = fb->mode;
        enemyKnown = true;
    }
    if (enemyKnown) {
        const Vec3 d    = enemyFeet + Vec3(0.0f, 0.0f, 56.0f) - self.eye;
        ti.hiddenYawErr = std::fabs(Wrap180(YawOf(d) - self.viewYaw));
        enemyDist       = (enemyFeet - self.origin).lengthXY();
    }
    ti.damaged = !obs.damage.empty();
    if (fb && !detected && fb->valid && !fb->dead && fb->visibleSoon > 0.25f) {
        for (int i = 0; i < fb->nExposure; i++) {
            const Vec3 d = fb->exposure[i] + Vec3(0.0f, 0.0f, 56.0f) - self.eye;
            if (fb->exposureEtaMs[i] < 400.0f && std::fabs(Wrap180(YawOf(d) - self.viewYaw)) < 8.0f) {
                ti.anticipate = true;
            }
        }
    }
    ti.blocked      = obs.teammateInCrosshair;
    ti.releaseLogit = m_off.releaseLogit;
    // the respawn click is often still held, or clicked again, in the first live ticks (it never fires);
    // the trigger takes over for good once an enemy is seen or the clicking is over
    const SpawnModel& sp = S.spawn;
    const double      uc = m_rngLife.Uniform();
    if (!m_clickOver && (detected || liveTick > static_cast<int>(sp.clickStayP.size()))) {
        m_clickOver = true;
    }
    bool attack;
    if (!m_clickOver) {
        if (liveTick == 0) {
            m_click = uc < sp.clickFirstP;
        } else {
            m_click = uc < (m_click ? sp.clickStayP[liveTick - 1] : sp.clickPressP[liveTick - 1]);
        }
        attack = m_click;
    } else {
        attack = m_trigger.Step(ti, m_rngTrigger);
    }

    //
    // Weapon (a reload lets go of the trigger)
    //
    const bool enemyAlive = fb && fb->valid && !fb->dead;
    const int  cmd        = m_weapon.Step(self, obs.gotKillOf >= 0, enemyAlive, detected, attack, m_rngWeapon);
    if (cmd == CMD_RELOAD || cmd == CMD_PISTOL) {
        attack = false;
    }

    //
    // View
    //
    ViewInput vi;
    vi.ctx      = ctx;
    vi.firing   = attackPrev;
    vi.substeps = m_substeps;
    const bool recentlyLost = !detected && fb && fb->valid && !fb->dead && fb->seenThisLife && fb->msSinceSeen < TRACK_MEMORY_MS;
    vi.track       = detected || recentlyLost;
    vi.detected    = detected;
    vi.acquisition = detected && m_acqTicks < ACQUISITION_TICKS;
    if (detected) {
        vi.enemyFeet  = enemyFeet;
        vi.enemyVel   = fe->vel;
        vi.bodyHeight = fe->bodyHeight;
    } else if (recentlyLost) {
        vi.enemyFeet = fb->mode;
        vi.enemyVel  = fb->lastSeenVel;
    }
    vi.belief          = fb;
    vi.moving          = self.velocity.lengthXY() > 50.0f;
    vi.navValid        = m_navOut.valid && m_navOut.urgency > 0.2f;
    vi.navYaw          = m_navOut.desiredYaw;
    vi.sounds          = &obs.sounds;
    vi.damage          = &obs.damage;
    vi.aimHeightFiring = m_off.aimHeightFiring;
    vi.noiseScale      = m_off.noiseScale;
    ViewOutput vo;
    m_view.Step(self, vi, m_rngView, vo);

    //
    // Movement, lean and stance
    //
    MoveInput mi;
    mi.ctx        = ctx;
    mi.losChanged = m_lageMs <= TICK_MS;
    if (enemyKnown) {
        mi.enemyKnown   = true;
        mi.enemyBearing = Wrap180(YawOf(enemyFeet - self.origin) - self.viewYaw);
        mi.enemyDist    = enemyDist;
    }
    mi.enemyReloading = detected && fe->reloading;
    mi.navValid       = m_navOut.valid;
    mi.navBearing     = Wrap180(m_navOut.desiredYaw - self.viewYaw);
    mi.urgency        = m_navOut.urgency;
    for (int c = 0; c < NUM_CHORDS; c++) {
        mi.clearance[c] = self.clearance[c];
        mi.drop[c]      = self.drop[c];
    }
    mi.wallPressMs = self.wallPressMs;
    mi.ducked      = self.ducked;
    mi.onGround    = self.onGround;
    MoveOutput mo;
    if (liveTick == 0) {
        m_mover.Spawn(m_spawnChord, mo);
    } else {
        m_mover.Step(mi, m_off, m_rngMove, m_rngStance, mo);
    }

    //
    // Navigation for the next tick
    //
    NavInput ni;
    ni.focus        = fb;
    ni.detected     = detected;
    ni.enemyPos     = enemyFeet;
    ni.reloading    = reloading;
    ni.msSinceSpawn = now - m_spawnMs;
    ni.msSinceKill  = now - m_killMs;
    m_nav.Step(self, ni, m_rngNav, m_navOut);

    //
    // Plan
    //
    plan.owner      = OWNER_BRAIN;
    plan.chord      = mo.chord;
    plan.attack     = attack;
    plan.lean       = mo.lean;
    plan.crouch     = mo.crouch;
    plan.jump       = mo.jump;
    plan.walk       = mo.walk;
    plan.yawDelta   = vo.yawDelta;
    plan.pitchDelta = vo.pitchDelta;
    plan.viewStill  = vo.still;
    plan.flickShaped = vo.flick;
    for (int k = 0; k < 8; k++) {
        plan.flickFrac[k] = vo.flickFrac[k];
    }
    plan.command        = cmd;
    plan.navTargetValid = m_navOut.valid;
    plan.navTarget      = m_navOut.target;
    m_attackPrev        = attack;

    if (diag) {
        Diag& d           = *diag;
        d.owner           = OWNER_BRAIN;
        d.ctx             = ctx;
        d.focus_id        = fb ? fb->enemyId : -1;
        d.detected        = detected ? 1 : 0;
        d.vis_parts       = fe ? fe->visParts : 0;
        d.detect_p        = fe ? fe->detectP : 0.0f;
        d.los             = los ? 1 : 0;
        d.lage_ms         = m_lageMs;
        if (fb) {
            d.belief_x      = fb->mode.x;
            d.belief_y      = fb->mode.y;
            d.belief_z      = fb->mode.z;
            d.belief_spread = fb->spread;
            d.belief_ess    = fb->ess;
            if (fb->nExposure > 0) {
                d.exposure_x    = fb->exposure[0].x;
                d.exposure_y    = fb->exposure[0].y;
                d.exposure_z    = fb->exposure[0].z;
                d.exposure_mass = fb->exposureMass[0];
            }
        }
        d.view_mode         = vo.mode;
        d.view_target_yaw   = vo.targetYaw;
        d.view_target_pitch = vo.targetPitch;
        d.view_err_yaw      = vo.errYaw;
        d.view_err_pitch    = vo.errPitch;
        d.flick             = vo.flick ? 1 : 0;
        d.flick_amp         = vo.flickAmp;
        d.still             = vo.still ? 1 : 0;
        d.aim_height        = vo.aimHeight;
        d.noise_yaw         = vo.noiseYaw;
        d.p_press           = m_trigger.LastPress();
        d.p_release         = m_trigger.LastRelease();
        d.want_fire         = attack ? 1 : 0;
        d.err_halfwidths    = ti.los ? ti.errHalfWidths : -1.0f;
        d.burst_shots       = m_trigger.HoldTicks();
        d.chord             = mo.chord;
        d.chord_age_ms      = m_mover.ChordAgeTicks() * TICK_MS;
        d.p_switch          = mo.pSwitch;
        d.veto_mask         = mo.vetoMask;
        d.lean              = mo.lean;
        d.lean_age_ms       = m_mover.LeanAgeTicks() * TICK_MS;
        d.crouch            = mo.crouch ? 1 : 0;
        d.jump              = mo.jump ? 1 : 0;
        d.walk              = mo.walk ? 1 : 0;
        d.nav_intent        = m_navOut.intent;
        d.nav_goal_x        = m_navOut.target.x;
        d.nav_goal_y        = m_navOut.target.y;
        d.nav_goal_z        = m_navOut.target.z;
        d.nav_dir_yaw       = m_navOut.desiredYaw;
        d.nav_urgency       = m_navOut.urgency;
        d.nav_misalign      = mi.navValid && mo.chord != CHORD_NEUTRAL
                                ? 0.5f * (1.0f - std::cos((Mover::ChordAngle(mo.chord) - mi.navBearing) * DEG2RAD))
                                : 0.0f;
        d.wall_pressure_ms  = static_cast<int>(self.wallPressMs);
        d.stuck_ms          = static_cast<int>(self.stuckMs);
        d.reload_intent     = m_weapon.ReloadPlanned() || cmd == CMD_RELOAD ? 1 : 0;
    }
    (void)S;
}

} // namespace hb
