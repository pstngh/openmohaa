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
static constexpr int   AIM_ARRIVE_CLOCK_MS = 200;   // the press hazard's clock on a late aim arrival (its peak: 200-250 ms)
// The owner's rule: no two headshots in a row. After a head hit the bot aims at the chest for
// HEAD_HIT_CHEST_MS and holds its fire for the first HEAD_HIT_PAUSE_MS, while the view comes down
// (the next round would leave within 100 ms, before the view moved).
static constexpr int   HEAD_HIT_CHEST_MS = 300;
static constexpr int   HEAD_HIT_PAUSE_MS = 100;
// people crouch three times as readily in the half second after the enemy fired (fit_movement.py)
static constexpr int   FIRE_HEARD_MS     = 500;
// Out of ammunition the bot closes in and bashes with the pistol (DM reach 96 u from its middle, plus the box of
// the victim): within this distance, centre to centre, and with the crosshair this near the body (half-widths).
static constexpr float BASH_REACH           = 100.0f;
static constexpr float BASH_AIM_HALF_WIDTHS = 1.5f;
// g_humanbot_skill, per unit: aim noise x e^-0.5, press logit +0.7 (reaction), detection rate x e^0.5,
// and a round the crosshair would send off the body skipped with chance 0.5 (people fire when on target:
// 30% of their rounds leave on the body against 20% of their frames; the bots' rounds do not)
static constexpr float BOOST_NOISE    = 0.5f;
static constexpr float BOOST_GATE     = 0.5f;   // chance per unit to skip a round with the crosshair off the body
static constexpr float BOOST_REACTION = 0.7f;
static constexpr float BOOST_DETECT   = 0.5f;
// The route the view looks along is smoothed (time constant ~0.3 s): the next path corner's direction swings round
// as a corner is passed, and people look down the corridor, not at the next corner.
static constexpr float ROUTE_LOOK_ALPHA = 0.15f;

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
    // a human client's first 2-4 ticks after the respawn are empty: the spawn tick, then as many as its usercmds take
    // to be stamped after the respawn (the server took none for 1 tick at 48 ms ping, 3 at 98 ms: openmohaa-movement
    // usercmds.py). With its own ping known the bot sends nothing in those ticks
    const int drawn = DrawIndex(sp.deadTicksPmf, m_rngLife);
    m_liveTick      = m_pingMs > 0 ? -(1 + std::max(1, static_cast<int>(std::lround(1.0 + (m_pingMs - 48) / 25.0)))) : -drawn;
    m_spawnLiveTick = m_liveTick;
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
    m_lookRouteValid = false;
    m_los       = false;
    m_lageMs    = 100000;
    m_vis       = false;
    m_vageMs    = 100000;
    m_tclockMs   = 100000;
    m_aimArrived = false;
    m_sightFirst = false;
    m_detected  = false;
    m_acqTicks  = 1000;
    m_clickDown = false;
    m_bashPrev  = false;
}

void Brain::OnDeath(const Observation& obs)
{
    m_deathMs     = obs.self.timeMs;
    m_respawnAtMs = m_deathMs + m_weapon.RespawnDelayMs(m_rngLife);
    m_clickDown   = false;
    m_attackPrev  = false;
}

float Brain::DetectMult() const
{
    return m_off.detectMult * std::exp(BOOST_DETECT * m_skillBoost);
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
        // the dead time after a respawn: no keys, no run bit, no mouse, no fire (and with the ping known, the
        // server's view of it: no usercmd at all after the spawn tick)
        m_belief.Update(obs, m_hfov, m_vfov);
        plan.owner     = OWNER_BRAIN;
        plan.walk      = true;
        plan.viewStill = true;
        plan.send      = m_pingMs <= 0 || liveTick == m_spawnLiveTick;
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
    const bool dry      = OutOfAmmo(self);
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
    if (obs.headHit) {
        m_headHitMs = now;
    }
    for (const SoundObs& s : obs.sounds) {
        if (s.type == SOUND_GUNFIRE) {
            m_fireHeardMs = now;
        }
    }

    // logger-equivalent LOS with the focus enemy (frustum gated) and its age: the contexts of the
    // movement and view models
    const bool los = detected && fe->centroidLos;
    if (los != m_los) {
        m_los    = los;
        m_lageMs = 0;
    } else {
        m_lageMs = std::min(m_lageMs + TICK_MS, 100000);
    }
    // the trigger's sight: any body part perceived. Its clock starts when the parts came on screen, as
    // people's reaction is timed from the first visible part (REPORT section 14), not when the bot noticed
    if (detected != m_vis) {
        m_vis        = detected;
        m_vageMs     = detected ? std::max(0, fe->visibleMs) : 0;
        m_tclockMs   = m_vageMs;
        m_aimArrived = false;
        m_sightFirst = detected;
    } else {
        m_sightFirst = false;
        m_vageMs   = std::min(m_vageMs + TICK_MS, 100000);
        m_tclockMs = std::min(m_tclockMs + TICK_MS, 100000);
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
    ti.los    = m_vis;
    ti.lageMs = m_vageMs;
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
        // the press hazard falls with the time since the enemy came on screen, but people press as readily once
        // their crosshair gets there late (0.3-1.5 s in: 21-37% a tick in the next 250 ms); the bot, slower to get
        // there, tracked the owner for seconds at 1-2% a tick. A late arrival restarts the clock at the press peak
        const float arrive = S.trigger.aimArriveHalfWidths;
        if (m_vis && !m_aimArrived && arrive > 0.0f && ti.errHalfWidths < arrive) {
            // there from the start of the sighting: the clock runs on from the parts
            m_aimArrived = true;
            if (!m_sightFirst) {
                m_tclockMs = std::min(m_tclockMs, AIM_ARRIVE_CLOCK_MS);
            }
        }
        if (m_vis) {
            ti.lageMs = m_tclockMs;
        }
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
    // people spare the last rounds of a clip (and run dry in sight a quarter as often as the bots did)
    ti.clipFill = self.clipSize > 0 ? static_cast<float>(self.clipAmmo) / self.clipSize : 1.0f;
    if (fb && !detected && fb->valid && !fb->dead && fb->visibleSoon > 0.25f) {
        for (int i = 0; i < fb->nExposure; i++) {
            // the corner it comes out from when known, else where it would stand
            const Vec3 d = (fb->cornerValid[i] ? fb->corner[i] : fb->exposure[i] + Vec3(0.0f, 0.0f, 56.0f)) - self.eye;
            if (fb->exposureEtaMs[i] < 400.0f && std::fabs(Wrap180(YawOf(d) - self.viewYaw)) < 8.0f) {
                ti.anticipate = true;
            }
        }
    }
    ti.blocked      = obs.teammateInCrosshair || now - m_headHitMs < HEAD_HIT_PAUSE_MS;
    ti.releaseLogit = m_off.releaseLogit;
    ti.pressLogit   = m_off.reactionLogit + BOOST_REACTION * m_skillBoost;
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
    const int  cmd        = m_weapon.Step(self, obs.gotKillOf >= 0, enemyAlive, detected, attack, m_vis ? 0 : m_vageMs, m_rngWeapon);
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
        if (!fe->centroidLos && fe->partMask) {
            // the body's middle is behind cover: aim at the visible part nearest the wanted height
            const float want = (attackPrev ? m_off.aimHeightFiring : S.view.aimHeightIdle) * fe->bodyHeight;
            float       best = 1e9f;
            for (int k = 0; k < NUM_PARTS; k++) {
                if (fe->partMask & (1 << k)) {
                    const float dz = std::fabs(fe->partPos[k].z - enemyFeet.z - want);
                    if (dz < best) {
                        best         = dz;
                        vi.aimPart   = fe->partPos[k];
                        vi.aimPartValid = true;
                    }
                }
            }
        }
    } else if (recentlyLost) {
        // people hold where the enemy went out of sight (their view 4-5 deg from it over the next 400 ms while the
        // enemy moved on to 10 deg); the believed position runs on with the last seen velocity
        const bool hold = S.view.lostAimLastSeen > 0.5f;
        vi.enemyFeet = hold ? fb->lastSeenPos : fb->mode;
        vi.enemyVel  = hold ? Vec3() : fb->lastSeenVel;
    }
    vi.belief          = fb;
    vi.moving          = self.velocity.lengthXY() > 50.0f;
    vi.navValid        = m_navOut.valid && m_navOut.urgency > 0.2f;
    if (!vi.navValid) {
        m_lookRouteValid = false;
    } else if (!m_lookRouteValid) {
        m_lookRouteYaw   = m_navOut.desiredYaw;
        m_lookRouteValid = true;
    } else {
        m_lookRouteYaw = Wrap180(m_lookRouteYaw + ROUTE_LOOK_ALPHA * Wrap180(m_navOut.desiredYaw - m_lookRouteYaw));
    }
    vi.navYaw          = m_lookRouteValid ? m_lookRouteYaw : m_navOut.desiredYaw;
    vi.sounds          = &obs.sounds;
    vi.damage          = &obs.damage;
    vi.aimHeightFiring = m_off.aimHeightFiring;
    // the owner's rule (people land two headshots in a row in 3% of their kills; the bots should not)
    vi.chestOnly = now - m_headHitMs < HEAD_HIT_CHEST_MS;
    vi.noiseScale      = m_off.noiseScale * std::exp(-BOOST_NOISE * m_skillBoost);
    vi.angleHold       = m_off.angleHold;
    ViewOutput vo;
    m_view.Step(self, vi, m_rngView, vo);
    if (vo.mode == VIEW_SOUND && m_viewMode != VIEW_SOUND) {
        // turned to an enemy heard behind: hunt toward where it is now believed to be, not on along the old route
        m_nav.Replan();
    }
    m_viewMode = vo.mode;

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
    mi.travelling     = m_navOut.intent == INTENT_HUNT || m_navOut.intent == INTENT_SPAWN_PUSH;
    for (int c = 0; c < NUM_CHORDS; c++) {
        mi.clearance[c] = self.clearance[c];
        mi.drop[c]      = self.drop[c];
    }
    mi.wallPressMs = self.wallPressMs;
    {
        const float y = self.viewYaw * DEG2RAD;
        mi.velFwd     = self.velocity.x * std::cos(y) + self.velocity.y * std::sin(y);
        mi.velRight   = self.velocity.x * std::sin(y) - self.velocity.y * std::cos(y);
    }
    mi.ducked      = self.ducked;
    mi.enemyDead   = !enemyAlive;
    mi.fireHeard   = now - m_fireHeardMs <= FIRE_HEARD_MS;
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
    ni.angleHold    = m_off.angleHold;
    ni.outOfAmmo    = dry;
    ni.clipFill     = self.clipSize > 0 ? static_cast<float>(self.clipAmmo) / self.clipSize : 1.0f;
    m_nav.Step(self, ni, m_rngNav, m_navOut);

    //
    // Plan
    //
    plan.owner      = OWNER_BRAIN;
    plan.chord      = mo.chord;
    plan.attack     = attack;
    // out of ammunition (people never are: they die first) the pistol bashes, a tap at a time, with the enemy in
    // reach and the crosshair on it
    plan.bash = dry && detected && self.weaponClass == WEAPON_CLASS_PISTOL && !self.switching && !m_bashPrev
             && (fe->pos - self.origin).lengthXY() < BASH_REACH && ti.errHalfWidths < BASH_AIM_HALF_WIDTHS;
    m_bashPrev = plan.bash;
    plan.lean       = mo.lean;
    plan.crouch     = mo.crouch;
    plan.jump       = mo.jump;
    plan.walk       = mo.walk;
    plan.yawDelta   = vo.yawDelta;
    plan.pitchDelta = vo.pitchDelta;
    plan.viewStill  = vo.still;
    plan.flickShaped = vo.flick;
    if (vo.aimValid && m_skillBoost > 0.0f) {
        plan.fireGate    = std::min(1.0f, BOOST_GATE * m_skillBoost);
        plan.gateYaw     = vo.targetYaw;
        plan.gateYawRate = vo.targetYawRate;
        plan.gatePitch   = vo.targetPitch;
        plan.gateHalfW   = vo.halfWidthDeg;
        plan.gateHalfH   = vo.halfHeightDeg;
        // never past a block (a teammate in the crosshair, the pause after a head hit) or an empty weapon
        plan.gateMayPress = ti.canFire && !ti.blocked && los;
    }
    for (int k = 0; k < MAX_SUBSTEPS; k++) {
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
                // the corner of the heaviest exposure when traced, else its cell
                const Vec3& x   = fb->cornerValid[0] ? fb->corner[0] : fb->exposure[0];
                d.exposure_x    = x.x;
                d.exposure_y    = x.y;
                d.exposure_z    = x.z;
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
