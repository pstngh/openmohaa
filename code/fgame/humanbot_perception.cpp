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
// humanbot_perception.cpp: what a bot can see of the game, read from the engine.
//
// Everything here is deterministic and leaves the game state alone. The only
// facts that leave this file about another player are the ones a human at the
// same screen would have: which body parts are on screen and unoccluded (and
// where they are), and the logger's centroid ray for the trigger features.

#include "humanbot_internal.h"
#include "weapon.h"

#include <cmath>

cvar_t *g_humanbot_fov;
cvar_t *g_humanbot_aspect;

static const char *const PART_TAGS[hb::NUM_PARTS] = {
    "Bip01 Head", "Bip01 Spine2", "Bip01 Spine1", "Bip01 Pelvis", "Bip01 L Foot", "Bip01 R Foot",
};

// bounding-box stand-ins when a model has no such bone (fractions of the box height)
static const float PART_BOX_HEIGHT[hb::NUM_PARTS] = {0.9f, 0.72f, 0.6f, 0.48f, 0.04f, 0.04f};

static const int   SIGHT_MASK       = MASK_SHOT & ~CONTENTS_TRIGGER;
static const float CLEARANCE_RANGE  = 128.0f;  // the logger's clearance probe
static const float DROP_PROBE_AHEAD = 32.0f;
static const float DROP_PROBE_DEPTH = 320.0f;

void HB_Fov(float& hfov, float& vfov)
{
    const float fov    = g_humanbot_fov ? Q_clamp_float(g_humanbot_fov->value, 60.0f, 120.0f) : 80.0f;
    const float aspect = g_humanbot_aspect ? Q_clamp_float(g_humanbot_aspect->value, 1.0f, 3.0f) : 16.0f / 9.0f;
    // fov is horizontal at 4:3; the vertical field is kept and the width grows (Hor+)
    const float tv = std::tan(DEG2RAD(fov) * 0.5f) * 0.75f;
    vfov           = RAD2DEG(2.0f * std::atan(tv));
    hfov           = RAD2DEG(2.0f * std::atan(tv * aspect));
}

HbView HB_ViewOf(Player *player)
{
    HbView view;
    Vector angles;
    player->GetPlayerView(&view.logEye, &angles);
    view.eye   = player->EyePosition();
    view.yaw   = AngleNormalize180(angles[YAW]);
    view.pitch = AngleNormalize180(angles[PITCH]);
    return view;
}

//
// Body parts, computed once per frame per player
//
struct PartCache {
    int    frame = -1;
    bool   valid = false;
    Vector pos[hb::NUM_PARTS];
};

static PartCache s_parts[MAX_CLIENTS];

bool HB_PartPositions(Player *target, Vector parts[hb::NUM_PARTS])
{
    if (!target || target->entnum < 0 || target->entnum >= MAX_CLIENTS) {
        return false;
    }
    PartCache& c = s_parts[target->entnum];
    if (c.frame != level.framenum) {
        c.frame = level.framenum;
        c.valid = true;
        const float height = target->maxs.z - target->mins.z;
        for (int i = 0; i < hb::NUM_PARTS; i++) {
            Vector pos;
            if (target->edict->tiki && target->GetTag(PART_TAGS[i], &pos)) {
                c.pos[i] = pos;
            } else {
                c.pos[i] = target->origin + Vector(0, 0, target->mins.z + height * PART_BOX_HEIGHT[i]);
            }
        }
    }
    for (int i = 0; i < hb::NUM_PARTS; i++) {
        parts[i] = c.pos[i];
    }
    return c.valid;
}

// Projects a world point into the view: inside the frustum and its angular offset.
static bool InFrustum(const Vector& eye, const Vector& fwd, const Vector& left, const Vector& up, float tanH, float tanV, const Vector& p)
{
    const Vector d = p - eye;
    const float  x = d * fwd;
    if (x <= 1.0f) {
        return false;
    }
    return std::fabs(d * left) <= x * tanH && std::fabs(d * up) <= x * tanV;
}

HbSight HB_SightOf(Player *viewer, const HbView& view, Player *target, bool fullCheck)
{
    HbSight out;
    if (!viewer || !target || target == viewer) {
        return out;
    }
    float hfov, vfov;
    HB_Fov(hfov, vfov);
    const float tanH = std::tan(DEG2RAD(hfov) * 0.5f);
    const float tanV = std::tan(DEG2RAD(vfov) * 0.5f);
    Vector      fwd, left, up;
    Vector(view.pitch, view.yaw, 0).AngleVectorsLeft(&fwd, &left, &up);

    Vector parts[hb::NUM_PARTS];
    HB_PartPositions(target, parts);
    int inside = 0;
    for (int i = 0; i < hb::NUM_PARTS; i++) {
        if (InFrustum(view.eye, fwd, left, up, tanH, tanV, parts[i])) {
            inside |= 1 << i;
        }
    }
    const bool centroidIn = InFrustum(view.logEye, fwd, left, up, tanH, tanV, target->centroid);
    out.inFov             = inside != 0 || centroidIn;
    if (!out.inFov) {
        return out;
    }

    // one sight trace per part on screen (the torso parts only when not doing the full check)
    for (int i = 0; i < hb::NUM_PARTS; i++) {
        if (!(inside & (1 << i))) {
            continue;
        }
        if (!fullCheck && i != hb::PART_HEAD && i != hb::PART_CHEST && i != hb::PART_PELVIS) {
            continue;
        }
        out.traces++;
        if (G_SightTrace(view.eye, vec_zero, vec_zero, parts[i], viewer, target, SIGHT_MASK, qfalse, "HumanBot part")) {
            out.partMask |= 1 << i;
        }
    }
    // the logger's centroid ray (from the unleaned eye), gated by the frustum
    if (centroidIn) {
        out.traces++;
        const trace_t tr =
            G_Trace(view.logEye, vec_zero, vec_zero, target->centroid, viewer, SIGHT_MASK, qfalse, "HumanBot centroid");
        out.centroidLos = tr.fraction >= 0.999f || tr.entityNum == target->entnum;
    }
    return out;
}

static int WeaponClassOf(Weapon *weapon)
{
    if (!weapon) {
        return hb::WEAPON_CLASS_NONE;
    }
    const int wc = weapon->GetWeaponClass();
    if (wc & ::WEAPON_CLASS_PISTOL) {
        return hb::WEAPON_CLASS_PISTOL;
    }
    if (wc & ::WEAPON_CLASS_SMG) {
        return hb::WEAPON_CLASS_SMG;
    }
    if (wc & ::WEAPON_CLASS_RIFLE) {
        return weapon->GetZoom() ? hb::WEAPON_CLASS_SNIPER : hb::WEAPON_CLASS_RIFLE;
    }
    if (wc & (::WEAPON_CLASS_MG | ::WEAPON_CLASS_HEAVY)) {
        return hb::WEAPON_CLASS_HEAVY;
    }
    if (wc & ::WEAPON_CLASS_GRENADE) {
        return hb::WEAPON_CLASS_GRENADE;
    }
    return hb::WEAPON_CLASS_OTHER;
}

void HB_FillSelf(Player *player, const HbView& view, hb::SelfState& self)
{
    self.timeMs    = level.inttime;
    self.entnum    = player->entnum;
    self.team      = static_cast<int>(player->GetTeam());
    self.alive     = !player->IsDead() && player->health > 0.0f;
    self.spectator = player->IsSpectator();
    self.origin    = hb::Vec3(player->origin.x, player->origin.y, player->origin.z);
    self.velocity  = hb::Vec3(player->velocity.x, player->velocity.y, player->velocity.z);
    self.viewYaw   = view.yaw;
    self.viewPitch = view.pitch;
    self.eye       = hb::Vec3(view.eye.x, view.eye.y, view.eye.z);
    self.aimEye    = hb::Vec3(view.logEye.x, view.logEye.y, view.logEye.z);
    self.aimEyeValid = true;
    self.health    = player->health;
    self.maxHealth = player->max_health;
    self.onGround  = player->groundentity != NULL || player->client->ps.walking;
    self.ducked    = (player->client->ps.pm_flags & PMF_DUCKED) != 0;
    self.onLadder  = player->GetLadder() != NULL;

    Weapon *weapon   = player->GetActiveWeapon(WEAPON_MAIN);
    self.weaponClass = WeaponClassOf(weapon);
    self.weaponState = weapon ? static_cast<int>(weapon->GetState()) : -1;
    self.clipAmmo    = weapon ? weapon->ClipAmmo(FIRE_PRIMARY) : 0;
    self.clipSize    = weapon ? weapon->GetClipSize(FIRE_PRIMARY) : 0;
    self.reserveAmmo = weapon ? weapon->AmmoAvailable(FIRE_PRIMARY) : 0;
    self.switching   = player->GetNewActiveWeapon() != NULL && player->GetNewActiveWeapon() != weapon;
    self.hasPistol   = false;
    const Container<int>& inventory = player->getInventory();
    for (int i = 1; i <= inventory.NumObjects(); i++) {
        Entity *item = G_GetEntity(inventory.ObjectAt(i));
        if (item && item->IsSubclassOfWeapon() && (static_cast<Weapon *>(item)->GetWeaponClass() & ::WEAPON_CLASS_PISTOL)) {
            self.hasPistol = true;
            break;
        }
    }
}

// Chord directions in the view frame (index = (fwd + 1) * 3 + (side + 1), side +1 = right).
static void ChordDirection(int chord, float viewYaw, Vector& dir)
{
    const int   f = hb::ChordFwd(chord);
    const int   s = hb::ChordSide(chord);
    const float y = DEG2RAD(viewYaw);
    // forward (cos, sin); right = forward rotated -90 degrees
    const Vector forward(std::cos(y), std::sin(y), 0.0f);
    const Vector right(std::sin(y), -std::cos(y), 0.0f);
    dir = forward * static_cast<float>(f) + right * static_cast<float>(s);
    dir.normalize();
}

void HB_FillClearance(Player *player, float viewYaw, float clearance[hb::NUM_CHORDS], float drop[hb::NUM_CHORDS], int dropPhase)
{
    const int worldMask = MASK_PLAYERSOLID & ~CONTENTS_BODY & ~CONTENTS_TRIGGER;
    for (int c = 0; c < hb::NUM_CHORDS; c++) {
        if (c == hb::CHORD_NEUTRAL) {
            clearance[c] = CLEARANCE_RANGE;
            drop[c]      = 0.0f;
            continue;
        }
        Vector dir;
        ChordDirection(c, viewYaw, dir);
        const Vector  end = player->origin + dir * CLEARANCE_RANGE;
        const trace_t tr  = G_Trace(player->origin, player->mins, player->maxs, end, player, worldMask, qfalse, "HumanBot clear");
        clearance[c]      = tr.startsolid ? 0.0f : tr.fraction * CLEARANCE_RANGE;
        if (dropPhase < 0 || (c & 1) != dropPhase) {
            continue;
        }
        // how far the floor falls just ahead in this direction (0 = level or blocked)
        const float ahead = std::min(DROP_PROBE_AHEAD, clearance[c]);
        if (ahead < 4.0f) {
            drop[c] = 0.0f;
            continue;
        }
        const Vector  start = player->origin + dir * ahead + Vector(0, 0, 8);
        const Vector  down  = start - Vector(0, 0, DROP_PROBE_DEPTH);
        const trace_t tf    = G_Trace(start, player->mins, player->maxs, down, player, worldMask, qfalse, "HumanBot drop");
        drop[c]             = tf.startsolid ? 0.0f : std::max(0.0f, player->origin.z - tf.endpos[2]);
    }
}
