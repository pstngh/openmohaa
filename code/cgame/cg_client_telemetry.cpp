/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

Client-local movement telemetry. This recorder is deliberately passive: it
observes commands, predicted state, and collision data already available to
the client and never changes commands or sends additional data to the server.
===========================================================================
*/

#include "cg_client_telemetry.h"
#include "cg_local.h"

#include <algorithm>
#include <cmath>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace
{

    constexpr int         TELEMETRY_SCHEMA       = 1;
    constexpr int         SAMPLE_MSEC            = 20;
    constexpr int         FLUSH_MSEC             = 1000;
    constexpr std::size_t BUFFER_LIMIT           = 64 * 1024;
    constexpr float       CLEARANCE_RANGE        = 256.0f;
    constexpr float       CROSSHAIR_RANGE        = 8192.0f;
    constexpr float       ALIGNED_TARGET_DEGREES = 12.0f;

    cvar_t *clMoveLog        = nullptr;
    cvar_t *clMoveLogSession = nullptr;

    struct BufferedFile {
        fileHandle_t handle = 0;
        std::string  buffer;
    };

    struct TraceInfo {
        float  distance   = CLEARANCE_RANGE;
        int    entity     = ENTITYNUM_NONE;
        int    entityType = -1;
        int    kind       = 0; // 0 none, 1 world, 2 player, 3 other entity
        int    startSolid = 0;
        vec3_t normal     = {0.0f, 0.0f, 0.0f};
    };

    struct TargetInfo {
        int    entity             = -1;
        int    confidence         = 0; // 0 none, 1 aligned visible, 2 direct trace
        int    visible            = 0;
        float  distance           = -1.0f;
        float  angularError       = -1.0f;
        float  relativeForward    = 0.0f;
        float  relativeRight      = 0.0f;
        float  relativeUp         = 0.0f;
        vec3_t velocity           = {0.0f, 0.0f, 0.0f};
        int    nearestVisible     = -1;
        float  nearestVisibleDist = -1.0f;
    };

    BufferedFile frameFile;
    BufferedFile inputFile;
    bool         recording      = false;
    bool         writeFailed    = false;
    int          recordingStart = 0;
    int          nextSampleTime = 0;
    int          nextFlushTime  = 0;
    int          lastCmdNumber  = -1;
    bool         haveLastInput  = false;
    usercmd_t    lastInput      = {};
    std::string  sessionId;
    std::string  fileStem;

    void Flush(BufferedFile& file)
    {
        if (!file.handle || file.buffer.empty()) {
            return;
        }

        if (cgi.FS_Write(file.buffer.data(), file.buffer.size(), file.handle) != file.buffer.size()) {
            writeFailed = true;
        }
        file.buffer.clear();
    }

    void Append(BufferedFile& file, const std::string& row)
    {
        if (!file.handle || writeFailed) {
            return;
        }

        file.buffer += row;
        if (file.buffer.size() >= BUFFER_LIMIT) {
            Flush(file);
        }
    }

    void Close(BufferedFile& file)
    {
        Flush(file);
        if (file.handle) {
            cgi.FS_FCloseFile(file.handle);
            file.handle = 0;
        }
        file.buffer.clear();
    }

    std::string SafeComponent(const char *value, const char *fallback)
    {
        std::string result;
        if (value) {
            for (const unsigned char ch : std::string(value)) {
                if (std::isalnum(ch) || ch == '-' || ch == '_') {
                    result.push_back(static_cast<char>(ch));
                } else if (ch == '/' || ch == '\\' || ch == '.') {
                    result.push_back('_');
                }
            }
        }

        if (result.empty()) {
            result = fallback;
        }
        if (result.size() > 72) {
            result.resize(72);
        }
        return result;
    }

    std::string CsvQuote(const char *value)
    {
        std::string result = "\"";
        if (value) {
            for (const char ch : std::string(value)) {
                if (ch == '"') {
                    result += "\"\"";
                } else if (ch != '\r' && ch != '\n') {
                    result.push_back(ch);
                }
            }
        }
        result += '"';
        return result;
    }

    int EntityType(int entityNum)
    {
        if (entityNum < 0 || entityNum >= MAX_GENTITIES) {
            return -1;
        }
        return cg_entities[entityNum].currentState.eType;
    }

    int HitKind(int entityNum)
    {
        if (entityNum == ENTITYNUM_NONE) {
            return 0;
        }
        if (entityNum == ENTITYNUM_WORLD) {
            return 1;
        }
        if (entityNum >= 0 && entityNum < MAX_GENTITIES && cg_entities[entityNum].currentState.eType == ET_PLAYER) {
            return 2;
        }
        return 3;
    }

    void PlayerBounds(vec3_t mins, vec3_t maxs)
    {
        const playerState_t& ps = cg.predicted_player_state;

        VectorSet(mins, MINS_X, MINS_Y, MINS_Z);
        VectorSet(maxs, MAXS_X, MAXS_Y, MAXS_Z);

        if (ps.pm_type == PM_DEAD) {
            maxs[2] = DEAD_MINS_Z;
        } else if ((ps.pm_flags & (PMF_DUCKED | PMF_VIEW_PRONE)) == (PMF_DUCKED | PMF_VIEW_PRONE)) {
            maxs[2] = CROUCH_MAXS_Z;
        } else if (ps.pm_flags & PMF_DUCKED) {
            maxs[2] = cg_protocol >= PROTOCOL_MOHTA_MIN ? CROUCH_MAXS_Z : CROUCH_RUN_MAXS_Z;
        } else if (ps.pm_flags & PMF_VIEW_PRONE) {
            maxs[2] = PRONE_MAXS_Z;
        } else if (ps.pm_flags & PMF_VIEW_DUCK_RUN) {
            mins[2] = CROUCH_MAXS_Z;
        }
    }

    TraceInfo TraceDirection(const vec3_t origin, const vec3_t direction, float range, bool fullBody, int mask)
    {
        vec3_t  end;
        vec3_t  mins = {0.0f, 0.0f, 0.0f};
        vec3_t  maxs = {0.0f, 0.0f, 0.0f};
        trace_t trace;

        if (fullBody) {
            PlayerBounds(mins, maxs);
        }
        VectorMA(origin, range, direction, end);
        CG_Trace(
            &trace, origin, mins, maxs, end, cg.predicted_player_state.clientNum, mask, qtrue, qtrue, "client telemetry"
        );

        TraceInfo result;
        result.distance   = trace.startsolid ? 0.0f : trace.fraction * range;
        result.entity     = trace.entityNum;
        result.entityType = EntityType(trace.entityNum);
        result.kind       = HitKind(trace.entityNum);
        result.startSolid = trace.startsolid ? 1 : 0;
        VectorCopy(trace.plane.normal, result.normal);
        return result;
    }

    bool IsEnemy(int entityNum)
    {
        if (entityNum < 0 || entityNum >= MAX_CLIENTS || entityNum == cg.predicted_player_state.clientNum) {
            return false;
        }

        const centity_t& entity = cg_entities[entityNum];
        if (!entity.currentValid || entity.currentState.eType != ET_PLAYER || (entity.currentState.eFlags & EF_DEAD)) {
            return false;
        }

        if (cgs.gametype <= GT_FFA) {
            return true;
        }

        const int localTeam = cg.predicted_player_state.stats[STAT_TEAM];
        int       otherTeam = cg.clientinfo[entityNum].team;
        if (otherTeam != TEAM_ALLIES && otherTeam != TEAM_AXIS) {
            if (entity.currentState.eFlags & EF_ALLIES) {
                otherTeam = TEAM_ALLIES;
            } else if (entity.currentState.eFlags & EF_AXIS) {
                otherTeam = TEAM_AXIS;
            }
        }

        return (localTeam != TEAM_ALLIES && localTeam != TEAM_AXIS) || otherTeam != localTeam;
    }

    bool VisibleEnemy(int entityNum, const vec3_t targetPoint)
    {
        trace_t trace;
        CG_Trace(
            &trace,
            cg.refdef.vieworg,
            vec3_origin,
            vec3_origin,
            targetPoint,
            cg.predicted_player_state.clientNum,
            MASK_SHOT,
            qfalse,
            qtrue,
            "client telemetry sight"
        );
        return trace.fraction == 1.0f || trace.entityNum == entityNum;
    }

    float AngularError(const vec3_t direction, const vec3_t viewForward)
    {
        const float dot = std::clamp(DotProduct(direction, viewForward), -1.0f, 1.0f);
        return std::acos(dot) * 180.0f / 3.14159265358979323846f;
    }

    TargetInfo FindTarget(const TraceInfo& crosshair, const vec3_t viewForward, const vec3_t viewRight)
    {
        TargetInfo result;
        int        alignedEntity = -1;
        float      alignedAngle  = 360.0f;

        for (int entityNum = 0; entityNum < std::min(cgs.maxclients, MAX_CLIENTS); ++entityNum) {
            if (!IsEnemy(entityNum)) {
                continue;
            }

            const centity_t& enemy = cg_entities[entityNum];
            vec3_t           delta;
            vec3_t           direction;
            vec3_t           targetPoint;
            VectorCopy(enemy.lerpOrigin, targetPoint);
            targetPoint[2] += 48.0f;
            VectorSubtract(targetPoint, cg.refdef.vieworg, delta);
            const float distance = VectorLength(delta);
            if (distance <= 0.0f) {
                continue;
            }

            VectorScale(delta, 1.0f / distance, direction);
            if (!VisibleEnemy(entityNum, targetPoint)) {
                continue;
            }

            if (result.nearestVisibleDist < 0.0f || distance < result.nearestVisibleDist) {
                result.nearestVisible     = entityNum;
                result.nearestVisibleDist = distance;
            }

            const float angle = AngularError(direction, viewForward);
            if (angle < alignedAngle) {
                alignedAngle  = angle;
                alignedEntity = entityNum;
            }
        }

        if (crosshair.kind == 2 && IsEnemy(crosshair.entity)) {
            result.entity     = crosshair.entity;
            result.confidence = 2;
        } else if (alignedEntity >= 0 && alignedAngle <= ALIGNED_TARGET_DEGREES) {
            result.entity     = alignedEntity;
            result.confidence = 1;
        }

        if (result.entity >= 0) {
            const centity_t& enemy = cg_entities[result.entity];
            vec3_t           targetPoint;
            vec3_t           delta;
            vec3_t           direction;
            VectorCopy(enemy.lerpOrigin, targetPoint);
            targetPoint[2] += 48.0f;
            VectorSubtract(targetPoint, cg.refdef.vieworg, delta);
            result.distance = VectorLength(delta);
            if (result.distance > 0.0f) {
                VectorScale(delta, 1.0f / result.distance, direction);
                result.angularError    = AngularError(direction, viewForward);
                result.relativeForward = DotProduct(delta, viewForward);
                result.relativeRight   = DotProduct(delta, viewRight);
                result.relativeUp      = delta[2];
            }
            result.visible = VisibleEnemy(result.entity, targetPoint) ? 1 : 0;
            VectorCopy(enemy.currentState.pos.trDelta, result.velocity);
        }

        return result;
    }

    void WriteTrace(std::ostringstream& row, const TraceInfo& trace)
    {
        row << ',' << trace.distance << ',' << trace.entity << ',' << trace.entityType << ',' << trace.kind << ','
            << trace.startSolid;
    }

    void ProcessInputTransitions()
    {
        const int current = cgi.GetCurrentCmdNumber();
        int       first   = lastCmdNumber + 1;
        if (lastCmdNumber < 0 || current - first >= CMD_BACKUP) {
            first = std::max(0, current - CMD_BACKUP + 1);
        }

        for (int cmdNumber = first; cmdNumber <= current; ++cmdNumber) {
            usercmd_t cmd;
            if (!cgi.GetUserCmd(cmdNumber, &cmd)) {
                continue;
            }

            const bool changed = !haveLastInput || cmd.forwardmove != lastInput.forwardmove
                              || cmd.rightmove != lastInput.rightmove || cmd.upmove != lastInput.upmove
                              || cmd.buttons != lastInput.buttons;
            if (changed) {
                std::ostringstream row;
                row << TELEMETRY_SCHEMA << ',' << sessionId << ',' << (cgi.Milliseconds() - recordingStart) << ','
                    << cg.time << ',' << cmdNumber << ',' << cmd.serverTime << ',' << static_cast<int>(cmd.msec) << ','
                    << static_cast<int>(cmd.forwardmove) << ',' << static_cast<int>(cmd.rightmove) << ','
                    << static_cast<int>(cmd.upmove) << ',' << cmd.buttons << ','
                    << ((cmd.buttons & BUTTON_ATTACKLEFT) ? 1 : 0) << ','
                    << ((cmd.buttons & BUTTON_ATTACKRIGHT) ? 1 : 0) << ',' << ((cmd.buttons & BUTTON_USE) ? 1 : 0)
                    << ',' << ((cmd.buttons & BUTTON_LEAN_LEFT) ? 1 : 0) << ','
                    << ((cmd.buttons & BUTTON_LEAN_RIGHT) ? 1 : 0) << ','
                    << GetWeaponCommand(
                           cmd.buttons,
                           cg_protocol >= PROTOCOL_MOHTA_MIN ? WEAPON_COMMAND_MAX_VER17 : WEAPON_COMMAND_MAX_VER6
                       )
                    << ',' << SHORT2ANGLE(cmd.angles[0]) << ',' << SHORT2ANGLE(cmd.angles[1]) << ','
                    << SHORT2ANGLE(cmd.angles[2]) << '\n';
                Append(inputFile, row.str());
            }

            lastInput     = cmd;
            haveLastInput = true;
        }
        lastCmdNumber = current;
    }

    void WriteFrame()
    {
        const playerState_t& ps        = cg.predicted_player_state;
        const int            cmdNumber = cgi.GetCurrentCmdNumber();
        usercmd_t            cmd       = {};
        cgi.GetUserCmd(cmdNumber, &cmd);

        vec3_t forward;
        vec3_t right;
        vec3_t up;
        AngleVectors(cg.refdefViewAngles, forward, right, up);
        forward[2] = 0.0f;
        right[2]   = 0.0f;
        VectorNormalize(forward);
        VectorNormalize(right);

        vec3_t directions[8];
        VectorCopy(forward, directions[0]);
        VectorScale(forward, -1.0f, directions[1]);
        VectorScale(right, -1.0f, directions[2]);
        VectorCopy(right, directions[3]);
        VectorAdd(forward, directions[2], directions[4]);
        VectorAdd(forward, right, directions[5]);
        VectorAdd(directions[1], directions[2], directions[6]);
        VectorAdd(directions[1], right, directions[7]);

        TraceInfo clearances[8];
        for (int i = 0; i < 8; ++i) {
            VectorNormalize(directions[i]);
            clearances[i] = TraceDirection(ps.origin, directions[i], CLEARANCE_RANGE, true, MASK_PLAYERSOLID);
        }

        vec3_t commandDirection;
        VectorScale(forward, static_cast<float>(cmd.forwardmove), commandDirection);
        VectorMA(commandDirection, static_cast<float>(cmd.rightmove), right, commandDirection);
        TraceInfo commandTrace;
        if (VectorNormalize(commandDirection) > 0.0f) {
            commandTrace = TraceDirection(ps.origin, commandDirection, CLEARANCE_RANGE, true, MASK_PLAYERSOLID);
        }

        vec3_t motionDirection;
        VectorCopy(ps.velocity, motionDirection);
        motionDirection[2] = 0.0f;
        TraceInfo motionTrace;
        if (VectorNormalize(motionDirection) > 0.0f) {
            motionTrace = TraceDirection(ps.origin, motionDirection, CLEARANCE_RANGE, true, MASK_PLAYERSOLID);
        }

        TraceInfo crosshair =
            TraceDirection(cg.refdef.vieworg, cg.refdef.viewaxis[0], CROSSHAIR_RANGE, false, MASK_SHOT);
        TargetInfo target = FindTarget(crosshair, cg.refdef.viewaxis[0], right);

        const int   weapon     = ps.activeItems[ITEM_WEAPON];
        const char *weaponName = weapon >= 0 && weapon < MAX_WEAPONS ? CG_ConfigString(CS_WEAPONS + weapon) : "";

        std::ostringstream row;
        row << std::fixed << std::setprecision(3);
        row << TELEMETRY_SCHEMA << ',' << sessionId << ',' << (cgi.Milliseconds() - recordingStart) << ',' << cg.time
            << ',' << cmdNumber << ',' << cmd.serverTime << ',' << static_cast<int>(cmd.msec) << ','
            << static_cast<int>(cmd.forwardmove) << ',' << static_cast<int>(cmd.rightmove) << ','
            << static_cast<int>(cmd.upmove) << ',' << cmd.buttons << ',' << ((cmd.buttons & BUTTON_ATTACKLEFT) ? 1 : 0)
            << ',' << ((cmd.buttons & BUTTON_ATTACKRIGHT) ? 1 : 0) << ',' << ((cmd.buttons & BUTTON_USE) ? 1 : 0) << ','
            << ((cmd.buttons & BUTTON_LEAN_LEFT) ? 1 : 0) << ',' << ((cmd.buttons & BUTTON_LEAN_RIGHT) ? 1 : 0) << ','
            << GetWeaponCommand(
                   cmd.buttons, cg_protocol >= PROTOCOL_MOHTA_MIN ? WEAPON_COMMAND_MAX_VER17 : WEAPON_COMMAND_MAX_VER6
               )
            << ',' << ps.origin[0] << ',' << ps.origin[1] << ',' << ps.origin[2] << ',' << ps.velocity[0] << ','
            << ps.velocity[1] << ',' << ps.velocity[2] << ','
            << std::sqrt(ps.velocity[0] * ps.velocity[0] + ps.velocity[1] * ps.velocity[1]) << ','
            << cg.refdefViewAngles[0] << ',' << cg.refdefViewAngles[1] << ',' << cg.refdefViewAngles[2] << ','
            << ps.fLeanAngle << ',' << ps.viewheight << ',' << ps.pm_type << ',' << ps.pm_flags << ','
            << ps.groundEntityNum << ',' << (ps.walking ? 1 : 0) << ',' << ps.stats[STAT_HEALTH] << ','
            << ps.stats[STAT_TEAM] << ',' << weapon << ',' << CsvQuote(weaponName) << ',' << ps.stats[STAT_AMMO] << ','
            << ps.stats[STAT_CLIPAMMO];

        for (const TraceInfo& clearance : clearances) {
            WriteTrace(row, clearance);
        }
        WriteTrace(row, commandTrace);
        row << ',' << commandTrace.normal[0] << ',' << commandTrace.normal[1] << ',' << commandTrace.normal[2];
        WriteTrace(row, motionTrace);
        row << ',' << motionTrace.normal[0] << ',' << motionTrace.normal[1] << ',' << motionTrace.normal[2];
        WriteTrace(row, crosshair);
        row << ',' << target.entity << ',' << target.confidence << ',' << target.visible << ',' << target.distance
            << ',' << target.angularError << ',' << target.relativeForward << ',' << target.relativeRight << ','
            << target.relativeUp << ',' << target.velocity[0] << ',' << target.velocity[1] << ',' << target.velocity[2]
            << ',' << target.nearestVisible << ',' << target.nearestVisibleDist << '\n';
        Append(frameFile, row.str());
    }

    bool OpenOutput()
    {
        const std::string token = SafeComponent(clMoveLogSession ? clMoveLogSession->string : "", "manual");
        const std::string map   = SafeComponent(cgs.mapname, "unknown_map");
        recordingStart          = cgi.Milliseconds();
        // The wall-clock time keeps names unique across launches; the engine's
        // millisecond counter restarts with every process.
        sessionId = token + "_" + map + "_" + std::to_string(static_cast<long long>(std::time(nullptr))) + "_"
                  + std::to_string(recordingStart);
        fileStem = "client_telemetry/" + sessionId;

        frameFile.handle        = cgi.FS_FOpenFileWrite((fileStem + "_frames.csv").c_str());
        inputFile.handle        = cgi.FS_FOpenFileWrite((fileStem + "_inputs.csv").c_str());
        const fileHandle_t meta = cgi.FS_FOpenFileWrite((fileStem + "_meta.txt").c_str());
        if (!frameFile.handle || !inputFile.handle || !meta) {
            if (meta) {
                cgi.FS_FCloseFile(meta);
            }
            Close(frameFile);
            Close(inputFile);
            cgi.Printf("cl_movelog: could not create client_telemetry output files\n");
            return false;
        }

        Append(
            frameFile,
            "schema,session_id,client_msec,server_msec,cmd_number,cmd_server_msec,cmd_msec,forwardmove,rightmove,"
            "upmove,buttons,attack_primary,attack_secondary,use,lean_left,lean_right,weapon_command,origin_x,origin_y,"
            "origin_z,velocity_x,velocity_y,velocity_z,horizontal_speed,view_pitch,view_yaw,view_roll,lean_angle,"
            "viewheight,pm_type,pm_flags,ground_entity,walking,health,team,weapon,weapon_name,ammo,clip_ammo,"
            "front_distance,front_entity,front_entity_type,front_kind,front_startsolid,back_distance,back_entity,"
            "back_entity_type,back_kind,back_startsolid,left_distance,left_entity,left_entity_type,left_kind,"
            "left_startsolid,right_distance,right_entity,right_entity_type,right_kind,right_startsolid,"
            "front_left_distance,front_left_entity,front_left_entity_type,front_left_kind,front_left_startsolid,"
            "front_right_distance,front_right_entity,front_right_entity_type,front_right_kind,front_right_startsolid,"
            "back_left_distance,back_left_entity,back_left_entity_type,back_left_kind,back_left_startsolid,"
            "back_right_distance,back_right_entity,back_right_entity_type,back_right_kind,back_right_startsolid,"
            "command_distance,command_entity,command_entity_type,command_kind,command_startsolid,command_normal_x,"
            "command_normal_y,command_normal_z,motion_distance,motion_entity,motion_entity_type,motion_kind,"
            "motion_startsolid,motion_normal_x,motion_normal_y,motion_normal_z,crosshair_distance,crosshair_entity,"
            "crosshair_entity_type,crosshair_kind,crosshair_startsolid,target_entity,target_confidence,target_visible,"
            "target_distance,target_angular_error,target_relative_forward,target_relative_right,target_relative_up,"
            "target_velocity_x,target_velocity_y,target_velocity_z,nearest_visible_enemy,nearest_visible_enemy_"
            "distance\n"
        );
        Append(
            inputFile,
            "schema,session_id,client_msec,server_msec,cmd_number,cmd_server_msec,cmd_msec,forwardmove,rightmove,"
            "upmove,buttons,attack_primary,attack_secondary,use,lean_left,lean_right,weapon_command,cmd_pitch,cmd_yaw,"
            "cmd_roll\n"
        );

        std::ostringstream metadata;
        metadata << "schema=" << TELEMETRY_SCHEMA << '\n'
                 << "session_id=" << sessionId << '\n'
                 << "source=client_predicted\n"
                 << "map=" << cgs.mapname << '\n'
                 << "map_checksum=" << cgs.mapChecksum << '\n'
                 << "gametype=" << cgs.gametype << '\n'
                 << "target_game=" << cg_target_game << '\n'
                 << "sample_hz=" << (1000 / SAMPLE_MSEC) << '\n'
                 << "clearance_range=" << CLEARANCE_RANGE << '\n'
                 << "crosshair_range=" << CROSSHAIR_RANGE << '\n'
                 << "target_confidence_0=none_or_ambiguous\n"
                 << "target_confidence_1=aim_aligned_visible\n"
                 << "target_confidence_2=direct_crosshair_trace\n"
                 << "hit_kind_0=none\n"
                 << "hit_kind_1=world\n"
                 << "hit_kind_2=player\n"
                 << "hit_kind_3=other_entity\n"
                 << "privacy=no_names_chat_or_network_addresses\n"
                 << "limitations=predicted_local_state_and_client-visible_entities_only\n";
        const std::string metaText    = metadata.str();
        const bool        metaWritten = cgi.FS_Write(metaText.data(), metaText.size(), meta) == metaText.size();
        cgi.FS_FCloseFile(meta);
        if (!metaWritten) {
            Close(frameFile);
            Close(inputFile);
            cgi.Printf("cl_movelog: could not write the client_telemetry metadata file\n");
            return false;
        }
        return true;
    }

    void Start()
    {
        // A demo shows someone else's movement, which would be recorded next to
        // the viewer's own inputs.
        if (recording || cg.demoPlayback || !cg.snap || !cg.validPPS) {
            return;
        }
        writeFailed = false;
        if (!OpenOutput()) {
            cgi.Cvar_Set("cl_movelog", "0");
            return;
        }

        recording      = true;
        nextSampleTime = cg.time;
        nextFlushTime  = cg.time + FLUSH_MSEC;
        lastCmdNumber  = cgi.GetCurrentCmdNumber() - 1;
        haveLastInput  = false;
        cgi.Printf("cl_movelog: recording %s_[frames.csv|inputs.csv|meta.txt]\n", fileStem.c_str());
    }

    void Stop()
    {
        if (!recording && !frameFile.handle && !inputFile.handle) {
            return;
        }

        Close(frameFile);
        Close(inputFile);
        if (writeFailed) {
            cgi.Printf("cl_movelog: stopped after a telemetry write failure\n");
        } else {
            cgi.Printf("cl_movelog: stopped recording %s\n", sessionId.c_str());
        }
        recording = false;
    }

} // namespace

extern "C" void CG_ClientTelemetryInit(void)
{
    clMoveLog        = cgi.Cvar_Get("cl_movelog", "0", 0);
    clMoveLogSession = cgi.Cvar_Get("cl_movelog_session", "", 0);
}

extern "C" void CG_ClientTelemetryFrame(void)
{
    if (!clMoveLog) {
        return;
    }
    if (!clMoveLog->integer) {
        Stop();
        return;
    }
    if (!recording) {
        Start();
    }
    if (!recording) {
        return;
    }

    ProcessInputTransitions();
    if (cg.time >= nextSampleTime) {
        WriteFrame();
        // Stay on the fixed sample grid, skipping slots already missed, so the
        // rate averages 50 Hz whenever the frame rate allows it.
        do {
            nextSampleTime += SAMPLE_MSEC;
        } while (nextSampleTime <= cg.time);
    }
    if (cg.time >= nextFlushTime) {
        Flush(frameFile);
        Flush(inputFile);
        nextFlushTime = cg.time + FLUSH_MSEC;
    }
    if (writeFailed) {
        cgi.Cvar_Set("cl_movelog", "0");
        Stop();
    }
}

extern "C" void CG_ClientTelemetryShutdown(void)
{
    Stop();
}
