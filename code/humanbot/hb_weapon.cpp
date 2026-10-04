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
// hb_weapon.cpp: reload and weapon decisions.

#include "hb_weapon.h"
#include "hb_math.h"
#include <algorithm>

namespace hb
{

static constexpr int WEAPON_STATE_READY     = 0;
static constexpr int WEAPON_STATE_FIRING    = 1;
static constexpr int WEAPON_STATE_RELOADING = 5;
static constexpr int UNARMED_DRAW_MS         = 1000;   // nothing in hand this long: draw a weapon

void WeaponLogic::Init(const WeaponModel *params)
{
    m_p = params;
    Reset();
}

void WeaponLogic::Reset()
{
    m_reloadAtMs   = -1;
    m_lastCmdMs    = -100000;
    m_unarmedSince = -1;
}

bool OutOfAmmo(const SelfState& self)
{
    // no round in the hand, and none for the primary or the pistol (people never get here: they die first)
    const bool held = self.weaponClass != WEAPON_CLASS_NONE && (self.clipAmmo > 0 || self.reserveAmmo > 0);
    return self.alive && !held && self.primaryAmmo <= 0 && self.pistolAmmo <= 0;
}

int WeaponLogic::RespawnDelayMs(Rng& rng) const
{
    return static_cast<int>(rng.FromQuantiles(m_p->respawnProbs, m_p->respawnMs));
}

int WeaponLogic::Step(const SelfState& self, bool gotKill, bool enemyAlive, bool enemyDetected, bool attackHeld, int msSinceSeen,
                      Rng& rng)
{
    const WeaponModel& p   = *m_p;
    const int          now = self.timeMs;
    // fixed draws per tick
    const double uKill = rng.Uniform();
    const double uTac  = rng.Uniform();
    const double uDly  = rng.Uniform();

    if (!self.alive) {
        m_reloadAtMs   = -1;
        m_unarmedSince = -1;
        return CMD_NONE;
    }
    if (self.weaponClass == WEAPON_CLASS_NONE) {
        // nothing in hand for a while (not the moment of a respawn, nor a ladder, which puts it away): draw the
        // primary when it has rounds, else the pistol, loaded or not (it still bashes)
        m_reloadAtMs = -1;
        if (m_unarmedSince < 0 || self.onLadder || self.switching) {
            m_unarmedSince = self.onLadder || self.switching ? -1 : now;
            return CMD_NONE;
        }
        if (now - m_unarmedSince >= UNARMED_DRAW_MS && now - m_lastCmdMs > 1000) {
            if (self.primaryAmmo > 0 || self.hasPistol) {
                m_lastCmdMs = now;
                return self.primaryAmmo > 0 ? CMD_PRIMARY : CMD_PISTOL;
            }
        }
        return CMD_NONE;
    }
    m_unarmedSince = -1;
    if (self.weaponState == WEAPON_STATE_RELOADING || self.switching) {
        m_reloadAtMs = -1;
        return CMD_NONE;
    }
    const bool canReload = self.clipAmmo < self.clipSize && self.reserveAmmo > 0;

    // the held weapon is out of rounds: the pistol (empty too, it still bashes: OutOfAmmo)
    if (self.clipAmmo == 0 && self.reserveAmmo == 0 && self.hasPistol && self.weaponClass != WEAPON_CLASS_PISTOL
        && now - m_lastCmdMs > 1000) {
        m_lastCmdMs = now;
        return CMD_PISTOL;
    }
    // back to the primary once it has rounds again (a weapon or ammunition picked up): at once when the pistol is
    // empty, else out of a fight
    if (self.weaponClass == WEAPON_CLASS_PISTOL && self.primaryAmmo > 0 && now - m_lastCmdMs > 1000
        && ((self.clipAmmo == 0 && self.reserveAmmo == 0) || (!enemyDetected && !attackHeld))) {
        m_lastCmdMs = now;
        return CMD_PRIMARY;
    }
    if (gotKill && canReload) {
        const int   bin = BinIndex(self.clipAmmo, p.postKillRoundEdges);
        const float pr  = p.postKillReloadP[bin];
        if (uKill < pr) {
            // inverse-CDF draw of the delay, using this tick's own uniform
            double delay = p.postKillDelayMs.back();
            for (size_t i = 1; i < p.postKillDelayProbs.size(); i++) {
                if (uDly <= p.postKillDelayProbs[i]) {
                    const double t = (uDly - p.postKillDelayProbs[i - 1])
                                   / std::max(1e-9, p.postKillDelayProbs[i] - p.postKillDelayProbs[i - 1]);
                    delay = p.postKillDelayMs[i - 1] + t * (p.postKillDelayMs[i] - p.postKillDelayMs[i - 1]);
                    break;
                }
            }
            m_reloadAtMs = now + static_cast<int>(delay);
        }
    }
    if (self.clipAmmo == 0 && self.reserveAmmo > 0 && m_reloadAtMs < 0) {
        // dry clip: reload right after letting go of the trigger
        m_reloadAtMs = now + (attackHeld ? 150 : 50) + static_cast<int>(100.0 * uDly);
    }
    if (m_reloadAtMs < 0 && canReload && enemyAlive && !enemyDetected && self.clipAmmo > 0 && self.clipSize > 0) {
        // early, with the enemy out of sight: people reload readily with little left, seldom with most of the clip
        float h;
        if (!p.tacticalTable.v.empty()) {
            const float fill = static_cast<float>(self.clipAmmo) / self.clipSize;
            h = p.tacticalTable.At(BinIndex(fill, p.tacticalClipEdges), BinIndex(static_cast<float>(msSinceSeen), p.tacticalSeenEdges));
        } else {
            h = self.clipAmmo < p.tacticalClipFrac * self.clipSize ? p.tacticalHazard : 0.0f;
        }
        if (uTac < h) {
            m_reloadAtMs = now + 100;
        }
    }
    if (m_reloadAtMs >= 0 && now >= m_reloadAtMs) {
        m_reloadAtMs = -1;
        if (canReload && (self.weaponState == WEAPON_STATE_READY || self.weaponState == WEAPON_STATE_FIRING)
            && now - m_lastCmdMs > 300) {
            m_lastCmdMs = now;
            return CMD_RELOAD;
        }
    }
    return CMD_NONE;
}

} // namespace hb
