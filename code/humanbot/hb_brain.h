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
// hb_brain.h: one human-imitation bot.
//
// Each 50 ms tick runs belief -> trigger -> view -> movement and stance ->
// nav -> weapon on the noisy Observation only. Decisions for tick n use the
// state observed at the end of tick n-1, the one-tick lag the recordings show.

#pragma once

#include "hb_belief.h"
#include "hb_diag.h"
#include "hb_map.h"
#include "hb_model.h"
#include "hb_movement.h"
#include "hb_nav.h"
#include "hb_rng.h"
#include "hb_style.h"
#include "hb_trigger.h"
#include "hb_types.h"
#include "hb_view.h"
#include "hb_weapon.h"

namespace hb
{

class Brain
{
public:
    void Init(const ModelBundle *bundle, const MapPrior *map, const StyleDials& dials, uint64_t seed, int substeps);
    void SetMap(const MapPrior *map);
    void SetFov(float hfovDeg, float vfovDeg);
    // The bot's own ping (ms; 0 = unknown): after a respawn the server takes no usercmd for as many ticks as a
    // client at that ping takes to see the respawn (else the fitted dead ticks are drawn).
    void SetPingMs(int ms) { m_pingMs = ms; }
    // The map's geometry for the corners of the believed paths (nullptr: exposures without corners).
    void SetWorld(const WorldQuery *world) { m_belief.SetWorld(world); }

    void Think(const Observation& obs, TickPlan& plan, Diag *diag);
    // The lean the engine held this tick instead of the planned one (going through a door): the lean chain goes on from it.
    void SetLean(int lean) { m_mover.SetLean(lean); }

    const StyleDials&   Dials() const { return m_dials; }
    const StyleOffsets& Offsets() const { return m_off; }

    // The server's difficulty (g_humanbot_skill): 0 = as fitted to the recorded players; higher is
    // sharper than they were (less aim noise, tighter tracking, quicker reactions and spotting).
    void  SetSkillBoost(float boost) { m_skillBoost = boost < 0.0f ? 0.0f : (boost > 3.0f ? 3.0f : boost); }
    float DetectMult() const;
    // Test harnesses only: replace the offsets derived from the dials (dial calibration sweeps).
    void                SetOffsets(const StyleOffsets& off) { m_off = off; }
    const BeliefFilter& Belief() const { return m_belief; }
    uint64_t            Seed() const { return m_seed; }

private:
    void OnSpawn(const Observation& obs);
    void OnDeath(const Observation& obs);

    const ModelBundle *m_bundle = nullptr;
    const MapPrior    *m_map    = nullptr;
    StyleDials         m_dials;
    StyleOffsets       m_off;
    float              m_skillBoost = 0.0f;
    uint64_t           m_seed = 0;
    int                m_substeps = 4;
    float              m_hfov = 96.4f;
    float              m_vfov = 64.4f;

    Rng m_rngBelief, m_rngView, m_rngTrigger, m_rngMove, m_rngStance, m_rngNav, m_rngWeapon, m_rngLife;

    BeliefFilter m_belief;
    ViewControl  m_view;
    Trigger      m_trigger;
    Mover        m_mover;
    Navigator    m_nav;
    WeaponLogic  m_weapon;
    NavOutput    m_navOut;
    float        m_lookRouteYaw   = 0.0f;   // the route direction the view looks along: smoothed (ROUTE_LOOK_ALPHA)
    bool         m_lookRouteValid = false;

    bool m_alive         = false;
    int  m_spawnMs       = 0;
    int  m_liveTick      = 0;      // ticks since the first live tick of this life (< 0 during the dead time)
    int  m_spawnChord    = 4;      // keys already held at the first live tick
    int  m_spawnLiveTick = 0;      // m_liveTick at the spawn tick
    bool m_click         = false;  // the respawn click still held or repeated after the spawn
    bool m_clickOver     = false;
    bool m_attackPrev    = false;  // attack actually sent last tick
    int  m_deathMs       = 0;
    int  m_respawnAtMs   = 0;
    bool m_clickDown     = false;
    bool m_bashPrev      = false;  // a bash tap went out last tick (out of ammunition)
    int  m_killMs        = -1000000;
    int  m_headHitMs     = -1000000;   // our last bullet in an enemy's head
    int  m_fireHeardMs   = -1000000;   // another player's gunfire last heard
    int  m_pingMs        = 0;
    int  m_viewMode      = -1;     // the view mode of the last tick
    bool m_los           = false;
    int  m_lageMs        = 100000;
    bool m_vis           = false;  // the trigger's sight: a body part of the focus enemy perceived
    int  m_vageMs        = 100000; // since that changed; a sighting counts from when the parts came on screen
    int  m_tclockMs      = 100000; // the press hazard's clock in sight: m_vageMs, restarted when the aim arrives late
    bool m_aimArrived    = false;  // the crosshair came near the enemy in this sighting
    bool m_sightFirst    = false;  // the first tick of a sighting
    bool m_firedInSight  = false;  // the trigger pressed in this sighting (the reaction dial acts until then)
    bool m_detected      = false;
    int  m_acqTicks      = 1000;
    int  m_focusId       = -1;
};

} // namespace hb
