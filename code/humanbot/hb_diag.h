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
// hb_diag.h: per-tick diagnostics of the human-imitation bot brain.
//
// The telemetry logger writes one bot_* column per field, in this order, for
// every player (humans get the defaults). The list is the single source of
// truth for the column names; load_captures.py splits every bot_* column off.

#pragma once

#include <cstdint>

//
// X(type, member, default, column)
//
#define HB_DIAG_FIELDS(X)                                                                         \
    X(int, family, -1, "bot_family")                 /* -1 not a human bot, 0 presser, 1 strafer, 2 stopper */ \
    X(int, style_seed, 0, "bot_style_seed")                                                        \
    X(int, owner, -1, "bot_owner")                   /* hb::Owner */                               \
    X(int, ctx, -1, "bot_ctx")                       /* hb::Context */                             \
    X(int, focus_id, -1, "bot_focus_id")             /* entity number of the focus enemy */        \
    X(int, detected, 0, "bot_detected")              /* focus enemy perceived this tick */         \
    X(int, vis_parts, 0, "bot_vis_parts")            /* visible body parts of the focus enemy */   \
    X(float, detect_p, 0.0f, "bot_detect_p")         /* per-tick detection probability */          \
    X(int, los, 0, "bot_los")                        /* logger-equivalent centroid LOS, fov gated */ \
    X(int, lage_ms, 0, "bot_lage_ms")                /* time since that LOS changed */             \
    X(float, belief_x, 0.0f, "bot_belief_x")                                                       \
    X(float, belief_y, 0.0f, "bot_belief_y")                                                       \
    X(float, belief_z, 0.0f, "bot_belief_z")                                                       \
    X(float, belief_spread, 0.0f, "bot_belief_spread")                                             \
    X(float, belief_ess, 0.0f, "bot_belief_ess")                                                   \
    X(float, belief_err, -1.0f, "bot_eval_belief_err") /* evaluation only; never fed back */       \
    X(float, exposure_x, 0.0f, "bot_exposure_x")                                                   \
    X(float, exposure_y, 0.0f, "bot_exposure_y")                                                   \
    X(float, exposure_z, 0.0f, "bot_exposure_z")                                                   \
    X(float, exposure_mass, 0.0f, "bot_exposure_mass")                                             \
    X(int, view_mode, -1, "bot_view_mode")           /* hb::ViewMode */                            \
    X(float, view_target_yaw, 0.0f, "bot_view_target_yaw")                                         \
    X(float, view_target_pitch, 0.0f, "bot_view_target_pitch")                                     \
    X(float, view_err_yaw, 0.0f, "bot_view_err_yaw")                                               \
    X(float, view_err_pitch, 0.0f, "bot_view_err_pitch")                                           \
    X(int, flick, 0, "bot_flick")                                                                  \
    X(float, flick_amp, 0.0f, "bot_flick_amp")                                                     \
    X(int, still, 0, "bot_still")                                                                  \
    X(float, aim_height, 0.0f, "bot_aim_height")                                                   \
    X(float, noise_yaw, 0.0f, "bot_noise_yaw")                                                     \
    X(float, p_press, 0.0f, "bot_p_press")                                                         \
    X(float, p_release, 0.0f, "bot_p_release")                                                     \
    X(int, want_fire, 0, "bot_want_fire")                                                          \
    X(float, err_halfwidths, 0.0f, "bot_err_halfwidths")                                           \
    X(int, burst_shots, 0, "bot_burst_shots")                                                      \
    X(int, chord, -1, "bot_chord")                   /* (fwd + 1) * 3 + (side + 1) */              \
    X(int, chord_age_ms, 0, "bot_chord_age_ms")                                                    \
    X(float, p_switch, 0.0f, "bot_p_switch")                                                       \
    X(int, veto_mask, 0, "bot_veto_mask")            /* bit per chord */                           \
    X(int, lean, 0, "bot_lean")                      /* -1 left, 0 none, 1 right */                \
    X(int, lean_age_ms, 0, "bot_lean_age_ms")                                                      \
    X(int, crouch, 0, "bot_crouch")                                                                \
    X(int, jump, 0, "bot_jump")                                                                    \
    X(int, walk, 0, "bot_walk")                                                                    \
    X(int, nav_intent, -1, "bot_nav_intent")         /* hb::Intent */                              \
    X(float, nav_goal_x, 0.0f, "bot_nav_goal_x")                                                   \
    X(float, nav_goal_y, 0.0f, "bot_nav_goal_y")                                                   \
    X(float, nav_goal_z, 0.0f, "bot_nav_goal_z")                                                   \
    X(float, nav_dir_yaw, 0.0f, "bot_nav_dir_yaw")                                                 \
    X(float, nav_urgency, 0.0f, "bot_nav_urgency")                                                 \
    X(float, nav_misalign, 0.0f, "bot_nav_misalign")                                               \
    X(int, wall_pressure_ms, 0, "bot_wall_pressure_ms")                                            \
    X(int, stuck_ms, 0, "bot_stuck_ms")                                                            \
    X(int, reload_intent, 0, "bot_reload_intent")                                                  \
    X(int, substeps, 0, "bot_substeps")                                                            \
    X(int, kbd_ok, 1, "bot_kbd_ok")                  /* 0 when a usercmd broke the keyboard contract */ \
    X(int, think_us, 0, "bot_think_us")

namespace hb
{

struct Diag {
#define HB_DIAG_MEMBER(type, member, def, column) type member = def;
    HB_DIAG_FIELDS(HB_DIAG_MEMBER)
#undef HB_DIAG_MEMBER
};

// Number of bot_* columns.
enum {
#define HB_DIAG_COUNT(type, member, def, column) +1
    DIAG_COLUMN_COUNT = 0 HB_DIAG_FIELDS(HB_DIAG_COUNT)
#undef HB_DIAG_COUNT
};

} // namespace hb
