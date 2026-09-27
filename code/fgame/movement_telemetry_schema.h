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
// movement_telemetry_schema.h: CSV column contract of the movement telemetry
// logger (g_movelog).
//
// Header-only and engine-independent. The logger writes its header lines with
// the helpers below and code/tests/humanbot/test_telemetry_headers.cpp checks
// the lists against golden files, so the header text has one source of truth.
//
// A frame row is MOVELOG_FRAME_CORE_COLUMNS, then MOVELOG_FRAME_EXT_COLUMNS,
// then one bot_* column per HB_DIAG_FIELDS entry. The core and event lists are
// the movement lab logger's (schema 12) and existing analysis reads them by
// name: never rename, reorder or remove one of those columns. Bump
// MOVELOG_SCHEMA whenever any list changes (including HB_DIAG_FIELDS); the
// logger refuses to append to files written with other columns.

#pragma once

#include "../humanbot/hb_diag.h"

#include <cstddef>
#include <string>

// Written to every row and to the metadata.
inline constexpr int MOVELOG_SCHEMA = 13;

inline constexpr const char *MOVELOG_FRAME_CORE_COLUMNS[] = {
    // 0: sample identity and player state
    "schema", "session_id", "session_ms", "server_ms", "frame", "frame_ms", "map", "client_id", "name", "model",
    "is_bot", "team", "alive", "spectator", "ping", "health", "max_health",
    // 17: position, legacy eye (origin + viewheight), velocity, view angles
    "origin_x", "origin_y", "origin_z", "eye_x", "eye_y", "eye_z", "velocity_x", "velocity_y", "velocity_z", "speed_xy",
    "speed_xyz", "view_pitch", "view_yaw", "view_roll",
    // 31: last usercmd
    "cmd_server_ms", "cmd_msec", "cmd_angle_pitch", "cmd_angle_yaw", "cmd_angle_roll", "cmd_forward", "cmd_right",
    "cmd_up", "buttons", "attack_primary", "attack_secondary", "run", "use", "lean_left", "lean_right",
    // 46: movement state and bounding box
    "pm_flags", "move_result", "on_ground", "on_ladder", "zoomed", "bbox_min_x", "bbox_min_y", "bbox_min_z",
    "bbox_max_x", "bbox_max_y", "bbox_max_z",
    // 57: held weapon
    "weapon", "weapon_state", "clip_ammo", "clip_size", "reserve_ammo", "fire_spread_mult",
    // 63: nearest living enemy
    "opponent_id", "opponent_name", "opponent_bot", "opponent_origin_x", "opponent_origin_y", "opponent_origin_z",
    "opponent_eye_x", "opponent_eye_y", "opponent_eye_z", "opponent_velocity_x", "opponent_velocity_y",
    "opponent_velocity_z",
    // 75: relative geometry
    "distance_xy", "distance_xyz", "body_gap_xy", "body_contact", "height_delta", "relative_bearing",
    "self_approach_speed", "self_tangential_speed", "opponent_approach_speed", "closing_speed",
    // 85: aim at the opponent centroid and crosshair trace
    "aim_pitch_error", "aim_yaw_error", "aim_total_error", "aim_dot", "aim_closest_miss", "aim_height_fraction",
    "line_of_sight", "crosshair_entity", "crosshair_distance", "crosshair_on_opponent",
    // 95: clearance probes around the view direction
    "clear_front", "clear_back", "clear_left", "clear_right", "clear_front_left", "clear_front_right",
    "clear_back_left", "clear_back_right",
    // 103: centroid sight trace
    "sight_blocker_entity", "sight_fraction", "sight_distance", "sight_normal_x", "sight_normal_y", "sight_normal_z",
    // 109: directional clearance: command direction, reverse, toward and away from the opponent
    "clear_move", "clear_move_entity", "clear_move_normal_x", "clear_move_normal_y", "clear_move_normal_z",
    "clear_move_startsolid", "clear_reverse", "clear_reverse_entity", "clear_reverse_normal_x",
    "clear_reverse_normal_y", "clear_reverse_normal_z", "clear_reverse_startsolid", "clear_toward_opponent",
    "clear_toward_opponent_entity", "clear_toward_opponent_normal_x", "clear_toward_opponent_normal_y",
    "clear_toward_opponent_normal_z", "clear_toward_opponent_startsolid", "clear_away_opponent",
    "clear_away_opponent_entity", "clear_away_opponent_normal_x", "clear_away_opponent_normal_y",
    "clear_away_opponent_normal_z", "clear_away_opponent_startsolid",
};

inline constexpr const char *MOVELOG_FRAME_EXT_COLUMNS[] = {
    "ext_ping",       // client->ps.ping (the legacy ping column reads client->ping, which is always 0)
    "ext_usercmds",   // usercmds Player::ClientThink accepted since the player's previous frame row
    "ext_lean_angle", // client->ps.fLeanAngle
    "ext_eye_ofs_x",  // EyePosition() - origin: the usereyes eye, which includes lean
    "ext_eye_ofs_y",
    "ext_eye_ofs_z",
    "ext_vis_parts",  // G_HumanBotObserveParts toward the logged opponent (0 without one)
    "ext_in_fov",
};

inline constexpr const char *MOVELOG_FRAME_BOT_COLUMNS[] = {
#define MOVELOG_BOT_COLUMN(type, member, def, column) column,
    HB_DIAG_FIELDS(MOVELOG_BOT_COLUMN)
#undef MOVELOG_BOT_COLUMN
};

inline constexpr const char *MOVELOG_EVENT_COLUMNS[] = {
    // 0: sample identity
    "schema", "session_id", "session_ms", "server_ms", "frame", "event", "chat_message", "chat_mode",
    // 8: actor and target
    "actor_id", "actor_name", "actor_bot", "target_id", "target_name", "target_bot",
    // 14: weapon and damage
    "weapon", "fire_mode", "damage", "health_before", "health_after", "means_of_death", "hit_location",
    // 21: position, direction and actor view
    "position_x", "position_y", "position_z", "direction_x", "direction_y", "direction_z", "view_pitch", "view_yaw",
    "view_roll",
    // 30: actor aim at its nearest living enemy
    "aim_target_id", "aim_pitch_error", "aim_yaw_error", "aim_total_error", "line_of_sight", "crosshair_entity",
    "crosshair_on_target", "crosshair_distance",
    // 38: crosshair trace surface
    "crosshair_hit_class", "crosshair_normal_x", "crosshair_normal_y", "crosshair_normal_z",
    // 42: centroid sight trace
    "sight_blocker_entity", "sight_blocker_class", "sight_fraction", "sight_distance", "sight_normal_x",
    "sight_normal_y", "sight_normal_z",
};

template<typename T, std::size_t N>
constexpr std::size_t MoveLogArrayCount(const T (&)[N])
{
    return N;
}

inline constexpr std::size_t MOVELOG_FRAME_CORE_COLUMN_COUNT = MoveLogArrayCount(MOVELOG_FRAME_CORE_COLUMNS);
inline constexpr std::size_t MOVELOG_FRAME_EXT_COLUMN_COUNT  = MoveLogArrayCount(MOVELOG_FRAME_EXT_COLUMNS);
inline constexpr std::size_t MOVELOG_FRAME_BOT_COLUMN_COUNT  = MoveLogArrayCount(MOVELOG_FRAME_BOT_COLUMNS);
inline constexpr std::size_t MOVELOG_FRAME_COLUMN_COUNT =
    MOVELOG_FRAME_CORE_COLUMN_COUNT + MOVELOG_FRAME_EXT_COLUMN_COUNT + MOVELOG_FRAME_BOT_COLUMN_COUNT;
inline constexpr std::size_t MOVELOG_EVENT_COLUMN_COUNT = MoveLogArrayCount(MOVELOG_EVENT_COLUMNS);

static_assert(MOVELOG_FRAME_CORE_COLUMN_COUNT == 133, "the core frame columns are the lab logger's 133");
static_assert(MOVELOG_FRAME_EXT_COLUMN_COUNT == 8, "ext frame columns changed: bump MOVELOG_SCHEMA");
static_assert(
    MOVELOG_FRAME_BOT_COLUMN_COUNT == static_cast<std::size_t>(hb::DIAG_COLUMN_COUNT),
    "one bot_* column per HB_DIAG_FIELDS entry"
);
static_assert(MOVELOG_EVENT_COLUMN_COUNT == 49, "the event columns are the lab logger's 49");

// Appends `count` column names to `line`, separated by commas.
inline void MoveLogAppendColumns(std::string& line, const char *const *columns, std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i) {
        if (!line.empty()) {
            line += ',';
        }
        line += columns[i];
    }
}

// Header line of movement_frames.csv: core, ext and bot columns, newline terminated.
inline std::string MoveLogFrameHeaderLine()
{
    std::string line;
    MoveLogAppendColumns(line, MOVELOG_FRAME_CORE_COLUMNS, MOVELOG_FRAME_CORE_COLUMN_COUNT);
    MoveLogAppendColumns(line, MOVELOG_FRAME_EXT_COLUMNS, MOVELOG_FRAME_EXT_COLUMN_COUNT);
    MoveLogAppendColumns(line, MOVELOG_FRAME_BOT_COLUMNS, MOVELOG_FRAME_BOT_COLUMN_COUNT);
    line += '\n';
    return line;
}

// Header line of movement_events.csv, newline terminated.
inline std::string MoveLogEventHeaderLine()
{
    std::string line;
    MoveLogAppendColumns(line, MOVELOG_EVENT_COLUMNS, MOVELOG_EVENT_COLUMN_COUNT);
    line += '\n';
    return line;
}

// Number of fields in the first CSV record of `text`. Quoted fields may hold
// commas and doubled quotes; the record ends at the first line break outside
// quotes.
inline std::size_t MoveLogCsvFieldCount(const std::string& text)
{
    std::size_t fields = 1;
    bool        quoted = false;

    for (const char character : text) {
        if (character == '"') {
            quoted = !quoted;
        } else if (!quoted && character == ',') {
            ++fields;
        } else if (!quoted && (character == '\n' || character == '\r')) {
            break;
        }
    }

    return fields;
}
