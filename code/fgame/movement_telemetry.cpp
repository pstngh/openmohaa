/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

Opt-in, server-side movement and aim telemetry.  The recorder deliberately
does not mutate game state and never calls a random-number function.
===========================================================================
*/

#include "movement_telemetry.h"

#include "g_local.h"
#include "g_main.h"
#include "g_utils.h"
#include "g_bot.h"
#include "game.h"
#include "gamecvars.h"
#include "player.h"
#include "playerbot.h"
#include "sentient.h"
#include "weapon.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace
{
constexpr int         MOVELOG_SCHEMA          = 7;
constexpr int         MOVELOG_SAMPLE_MSEC     = 50;
constexpr int         MOVELOG_FLUSH_MSEC      = 1000;
constexpr size_t      MOVELOG_BUFFER_LIMIT    = 64 * 1024;
constexpr unsigned long long MOVELOG_SEGMENT_LIMIT =
    1536ULL * 1024ULL * 1024ULL;
constexpr float       MOVELOG_CLEARANCE_RANGE = 128.0f;
constexpr float       MOVELOG_AIM_RANGE       = 8192.0f;
constexpr float       RAD_TO_DEG              = 57.29577951308232f;
constexpr const char *MOVELOG_FRAMES_PATH = "telemetry/movement_frames.csv";
constexpr const char *MOVELOG_EVENTS_PATH = "telemetry/movement_events.csv";
constexpr const char *MOVELOG_META_PATH   = "telemetry/movement_meta.txt";

fileHandle_t framesFile = 0;
fileHandle_t eventsFile = 0;
std::string  framesBuffer;
std::string  eventsBuffer;
std::string  framesPath(MOVELOG_FRAMES_PATH);
std::string  eventsPath(MOVELOG_EVENTS_PATH);
std::string  metaPath(MOVELOG_META_PATH);
std::string  sessionId;
unsigned long long framesBytesWritten = 0;
int          sessionStartMsec = 0;
int          nextSampleMsec   = 0;
int          nextFlushMsec    = 0;

static void SelectFreshSegmentPaths()
{
    std::ostringstream prefix;
    prefix << "telemetry/segments/"
           << static_cast<long long>(std::time(NULL)) << '_'
           << gi.Milliseconds();
    framesPath = prefix.str() + "_movement_frames.csv";
    eventsPath = prefix.str() + "_movement_events.csv";
    metaPath   = prefix.str() + "_movement_meta.txt";
}

struct VisibilityState
{
    int  targetEntity;
    bool initialized;
    bool visible;

    VisibilityState() : targetEntity(ENTITYNUM_NONE), initialized(false), visible(false) {}
};

VisibilityState visibilityStates[MAX_CLIENTS];

struct ClearanceMetrics
{
    float  distance;
    int    entity;
    Vector normal;
    bool   startSolid;

    ClearanceMetrics() : distance(-1.0f), entity(ENTITYNUM_NONE), normal(vec_zero), startSolid(false) {}
};

struct AimMetrics
{
    Player *target;
    float   pitchError;
    float   yawError;
    float   totalError;
    float   dot;
    float   closestMiss;
    float   heightFraction;
    bool    lineOfSight;
    int     crosshairEntity;
    float   crosshairDistance;
    Vector  crosshairNormal;
    str     crosshairClass;
    bool    crosshairOnTarget;
    int     sightBlockerEntity;
    float   sightFraction;
    float   sightDistance;
    Vector  sightNormal;
    str     sightBlockerClass;

    AimMetrics()
        : target(NULL),
          pitchError(0.0f),
          yawError(0.0f),
          totalError(0.0f),
          dot(0.0f),
          closestMiss(0.0f),
          heightFraction(0.0f),
          lineOfSight(false),
          crosshairEntity(ENTITYNUM_NONE),
          crosshairDistance(0.0f),
          crosshairNormal(vec_zero),
          crosshairOnTarget(false),
          sightBlockerEntity(ENTITYNUM_NONE),
          sightFraction(1.0f),
          sightDistance(0.0f),
          sightNormal(vec_zero)
    {}
};

static Player *AsPlayer(Entity *entity)
{
    if (!entity || !entity->IsSubclassOfPlayer()) {
        return NULL;
    }

    return static_cast<Player *>(entity);
}

static Player *AsPlayer(Sentient *sentient)
{
    return AsPlayer(static_cast<Entity *>(sentient));
}

static bool IsRecordablePlayer(Player *player)
{
    return player && player->edict && player->edict->inuse && player->client;
}

static const char *PlayerName(Player *player)
{
    return IsRecordablePlayer(player) ? player->client->pers.netname : "";
}

static bool PlayerIsBot(Player *player)
{
    return IsRecordablePlayer(player) && G_IsBot(player->edict);
}

static const char *TraceEntityClass(const trace_t& trace)
{
    if (trace.fraction >= 1.0f && !trace.startsolid) {
        return "";
    }
    if (trace.ent && trace.ent->entity) {
        return trace.ent->entity->getClassname();
    }
    return trace.entityNum == ENTITYNUM_WORLD ? "world" : "";
}

static std::string CsvQuote(const char *value)
{
    std::string result("\"");
    const char *cursor = value ? value : "";

    for (; *cursor; ++cursor) {
        if (*cursor == '"') {
            result += "\"\"";
        } else if (*cursor == '\r' || *cursor == '\n') {
            result += ' ';
        } else {
            result += *cursor;
        }
    }

    result += '"';
    return result;
}

static std::string SanitizeFilename(const char *value)
{
    std::string result;
    const char *cursor = value ? value : "unknown";

    for (; *cursor; ++cursor) {
        const unsigned char character = static_cast<unsigned char>(*cursor);
        if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z')
            || (character >= '0' && character <= '9') || character == '-' || character == '_') {
            result += static_cast<char>(character);
        } else {
            result += '_';
        }
    }

    return result.empty() ? "unknown" : result;
}

static int SessionMsec()
{
    return level.inttime - sessionStartMsec;
}

static void FlushBuffers(bool force)
{
    if (framesFile && !framesBuffer.empty() && (force || framesBuffer.size() >= MOVELOG_BUFFER_LIMIT)) {
        framesBytesWritten += gi.FS_Write(
            framesBuffer.data(), framesBuffer.size(), framesFile
        );
        framesBuffer.clear();
    }

    if (eventsFile && !eventsBuffer.empty() && (force || eventsBuffer.size() >= MOVELOG_BUFFER_LIMIT)) {
        gi.FS_Write(eventsBuffer.data(), eventsBuffer.size(), eventsFile);
        eventsBuffer.clear();
    }

    if (force) {
        if (framesFile) {
            gi.FS_Flush(framesFile);
        }
        if (eventsFile) {
            gi.FS_Flush(eventsFile);
        }
    }
}

static void AppendEventRow(
    const char   *eventName,
    Player       *actor,
    Player       *target,
    const char   *weaponName,
    int           fireMode,
    float         damage,
    float         healthBefore,
    float         healthAfter,
    int           meansOfDeath,
    int           hitLocation,
    const Vector& position,
    const Vector& direction,
    const Vector& viewAngles,
    const AimMetrics *aim,
    const char       *chatMessage = "",
    int               chatMode    = 0
)
{
    if (!eventsFile) {
        return;
    }

    std::ostringstream row;
    row << std::fixed << std::setprecision(3)
        << MOVELOG_SCHEMA << ',' << CsvQuote(sessionId.c_str()) << ',' << SessionMsec() << ',' << level.inttime << ','
        << level.framenum << ',' << CsvQuote(eventName) << ',' << CsvQuote(chatMessage) << ',' << chatMode << ','
        << (actor ? actor->entnum : -1) << ',' << CsvQuote(PlayerName(actor)) << ',' << (PlayerIsBot(actor) ? 1 : 0)
        << ',' << (target ? target->entnum : -1) << ',' << CsvQuote(PlayerName(target)) << ','
        << (PlayerIsBot(target) ? 1 : 0) << ',' << CsvQuote(weaponName) << ',' << fireMode << ',' << damage << ','
        << healthBefore << ',' << healthAfter << ',' << meansOfDeath << ',' << hitLocation << ',' << position.x << ','
        << position.y << ',' << position.z << ',' << direction.x << ',' << direction.y << ',' << direction.z << ','
        << viewAngles.x << ',' << viewAngles.y << ',' << viewAngles.z << ','
        << (aim && aim->target ? aim->target->entnum : -1) << ',' << (aim ? aim->pitchError : 0.0f) << ','
        << (aim ? aim->yawError : 0.0f) << ',' << (aim ? aim->totalError : 0.0f) << ','
        << (aim && aim->lineOfSight ? 1 : 0) << ',' << (aim ? aim->crosshairEntity : ENTITYNUM_NONE) << ','
        << (aim && aim->crosshairOnTarget ? 1 : 0) << ',' << (aim ? aim->crosshairDistance : -1.0f) << ','
        << CsvQuote(aim ? aim->crosshairClass.c_str() : "") << ',' << (aim ? aim->crosshairNormal.x : 0.0f) << ','
        << (aim ? aim->crosshairNormal.y : 0.0f) << ',' << (aim ? aim->crosshairNormal.z : 0.0f) << ','
        << (aim ? aim->sightBlockerEntity : ENTITYNUM_NONE) << ','
        << CsvQuote(aim ? aim->sightBlockerClass.c_str() : "") << ',' << (aim ? aim->sightFraction : 1.0f) << ','
        << (aim ? aim->sightDistance : -1.0f) << ',' << (aim ? aim->sightNormal.x : 0.0f) << ','
        << (aim ? aim->sightNormal.y : 0.0f) << ',' << (aim ? aim->sightNormal.z : 0.0f) << '\n';

    eventsBuffer += row.str();
    FlushBuffers(false);
}

static Player *FindNearestOpponent(Player *player)
{
    if (!IsRecordablePlayer(player) || player->IsSpectator() || player->deadflag != DEAD_NO || player->health <= 0) {
        return NULL;
    }

    Player *nearest         = NULL;
    float   nearestDistance = 0.0f;
    const teamtype_t team   = player->GetTeam();
    const bool hasTeam      = team == TEAM_ALLIES || team == TEAM_AXIS;

    for (int i = 0; i < game.maxclients; ++i) {
        gentity_t *edict = &g_entities[i];
        if (!edict->inuse || !edict->entity || !edict->entity->IsSubclassOfPlayer()) {
            continue;
        }

        Player *candidate = static_cast<Player *>(edict->entity);
        if (candidate == player || candidate->IsSpectator() || candidate->deadflag != DEAD_NO || candidate->health <= 0) {
            continue;
        }
        if (hasTeam && candidate->GetTeam() == team) {
            continue;
        }

        const float distance = (candidate->origin - player->origin).lengthSquared();
        if (!nearest || distance < nearestDistance) {
            nearest         = candidate;
            nearestDistance = distance;
        }
    }

    return nearest;
}

static AimMetrics CalculateAimMetrics(Player *player, const Vector& start, const Vector& suppliedForward)
{
    AimMetrics result;
    result.target = FindNearestOpponent(player);

    Vector forward = suppliedForward;
    if (forward.normalize() == 0.0f) {
        return result;
    }

    const Vector traceEnd = start + forward * MOVELOG_AIM_RANGE;
    const Vector zero(0.0f, 0.0f, 0.0f);
    const int shotMask = MASK_SHOT & ~CONTENTS_TRIGGER;
    const trace_t crosshairTrace = G_Trace(start, zero, zero, traceEnd, player, shotMask, qfalse, "G_MoveLog aim");
    result.crosshairEntity   = crosshairTrace.entityNum;
    result.crosshairDistance = MOVELOG_AIM_RANGE * crosshairTrace.fraction;
    result.crosshairNormal   = crosshairTrace.plane.normal;
    result.crosshairClass    = TraceEntityClass(crosshairTrace);

    if (!result.target) {
        return result;
    }

    Vector targetDelta = result.target->centroid - start;
    const float targetDistance = targetDelta.length();
    if (targetDistance <= 0.001f) {
        return result;
    }

    Vector targetDirection = targetDelta / targetDistance;
    const Vector targetAngles = targetDirection.toAngles();
    const Vector forwardAngles = forward.toAngles();
    result.pitchError = AngleSubtract(targetAngles[PITCH], forwardAngles[PITCH]);
    result.yawError   = AngleSubtract(targetAngles[YAW], forwardAngles[YAW]);
    result.dot        = std::max(-1.0f, std::min(1.0f, forward * targetDirection));
    result.totalError = std::acos(result.dot) * RAD_TO_DEG;

    const float projection = targetDelta * forward;
    const Vector closest = start + forward * std::max(0.0f, projection);
    result.closestMiss = (result.target->centroid - closest).length();

    const float forwardXY = forward.x * forward.x + forward.y * forward.y;
    if (forwardXY > 0.000001f) {
        const float horizontalTime = (targetDelta.x * forward.x + targetDelta.y * forward.y) / forwardXY;
        const float rayHeight      = start.z + horizontalTime * forward.z;
        const float targetMinZ     = result.target->absmin.z;
        const float targetHeight   = result.target->absmax.z - targetMinZ;
        if (targetHeight > 0.001f) {
            result.heightFraction = (rayHeight - targetMinZ) / targetHeight;
        }
    }

    const trace_t sightTrace = G_Trace(
        start,
        zero,
        zero,
        result.target->centroid,
        player,
        shotMask,
        qfalse,
        "G_MoveLog sight"
    );
    result.lineOfSight = sightTrace.fraction >= 0.999f || sightTrace.entityNum == result.target->entnum;
    result.crosshairOnTarget = crosshairTrace.entityNum == result.target->entnum;
    result.sightFraction      = sightTrace.fraction;
    result.sightDistance      = targetDistance * sightTrace.fraction;
    if (!result.lineOfSight) {
        result.sightBlockerEntity = sightTrace.entityNum;
        result.sightNormal        = sightTrace.plane.normal;
        result.sightBlockerClass  = TraceEntityClass(sightTrace);
    }
    return result;
}

static float TraceClearance(Player *player, const Vector& direction)
{
    const Vector end = player->origin + direction * MOVELOG_CLEARANCE_RANGE;
    const int worldMask = MASK_PLAYERSOLID & ~CONTENTS_BODY & ~CONTENTS_TRIGGER;
    const trace_t trace = G_Trace(
        player->origin,
        player->mins,
        player->maxs,
        end,
        player,
        worldMask,
        qfalse,
        "G_MoveLog clearance"
    );

    return trace.startsolid ? 0.0f : trace.fraction * MOVELOG_CLEARANCE_RANGE;
}

static ClearanceMetrics TraceDirectionalClearance(Player *player, Vector direction)
{
    ClearanceMetrics result;
    direction.z = 0.0f;
    if (!player || direction.normalize() <= 0.0f) {
        return result;
    }

    const Vector end = player->origin + direction * MOVELOG_CLEARANCE_RANGE;
    const int collisionMask = MASK_PLAYERSOLID & ~CONTENTS_TRIGGER;
    const trace_t trace = G_Trace(
        player->origin,
        player->mins,
        player->maxs,
        end,
        player,
        collisionMask,
        qfalse,
        "G_MoveLog directional clearance"
    );

    result.startSolid = trace.startsolid;
    result.distance   = trace.startsolid ? 0.0f : trace.fraction * MOVELOG_CLEARANCE_RANGE;
    if (trace.startsolid || trace.fraction < 1.0f) {
        result.entity = trace.entityNum;
        result.normal = trace.plane.normal;
    }
    return result;
}

static void AppendClearanceColumns(std::ostringstream& row, const ClearanceMetrics& clearance)
{
    row << ',' << clearance.distance << ',' << clearance.entity << ',' << clearance.normal.x << ','
        << clearance.normal.y << ',' << clearance.normal.z << ',' << (clearance.startSolid ? 1 : 0);
}

static void AppendBotColumns(std::ostringstream& row, Player *player)
{
    bot_controller_telemetry_t controllerTelemetry;
    bot_movement_telemetry_t   movementTelemetry;

    if (PlayerIsBot(player)) {
        BotController *controller = botManager.getControllerManager().findController(player);
        if (controller) {
            controller->GetTelemetry(controllerTelemetry);
            controller->GetMovement().GetTelemetry(movementTelemetry);
        }
    }

    row << ',' << controllerTelemetry.stateFlags << ',' << controllerTelemetry.enemyEntity << ','
        << (controllerTelemetry.enemyVisible ? 1 : 0) << ',' << (controllerTelemetry.canAttack ? 1 : 0) << ','
        << static_cast<int>(controllerTelemetry.fireDecision) << ',' << (controllerTelemetry.wantsFire ? 1 : 0) << ','
        << (controllerTelemetry.noMove ? 1 : 0) << ',' << controllerTelemetry.reactionRemainingMsec << ','
        << controllerTelemetry.enemyDistance << ',' << controllerTelemetry.aimAcquireMsec << ','
        << controllerTelemetry.aimHeightFraction << ',' << controllerTelemetry.aimErrorFraction << ','
        << controllerTelemetry.aimErrorUnits << ',' << controllerTelemetry.aimLatencyMsec << ','
        << controllerTelemetry.aimTarget.x << ',' << controllerTelemetry.aimTarget.y << ','
        << controllerTelemetry.aimTarget.z << ',' << controllerTelemetry.aimPoint.x << ','
        << controllerTelemetry.aimPoint.y << ',' << controllerTelemetry.aimPoint.z << ','
        << controllerTelemetry.aimErrorDirection.x << ',' << controllerTelemetry.aimErrorDirection.y << ','
        << controllerTelemetry.aimErrorDirection.z << ',' << controllerTelemetry.targetAngles.x << ','
        << controllerTelemetry.targetAngles.y << ',' << static_cast<int>(controllerTelemetry.contactSource) << ','
        << controllerTelemetry.contactEnemy << ',' << controllerTelemetry.contactReporter << ','
        << controllerTelemetry.contactAgeMsec << ',' << (controllerTelemetry.contactResponder ? 1 : 0) << ','
        << controllerTelemetry.contactPosition.x << ',' << controllerTelemetry.contactPosition.y << ','
        << controllerTelemetry.contactPosition.z << ',' << static_cast<int>(controllerTelemetry.objectiveState) << ','
        << controllerTelemetry.objectiveSite << ',' << controllerTelemetry.objectiveRound << ','
        << static_cast<int>(controllerTelemetry.objectiveUsePhase) << ','
        << (controllerTelemetry.objectiveAttacker ? 1 : 0) << ','
        << (controllerTelemetry.objectiveCritical ? 1 : 0) << ',' << controllerTelemetry.objectivePosition.x << ','
        << controllerTelemetry.objectivePosition.y << ',' << controllerTelemetry.objectivePosition.z << ','
        << (movementTelemetry.hasCombatTarget ? 1 : 0) << ','
        << (movementTelemetry.pathing ? 1 : 0) << ',' << (movementTelemetry.blockedRecovery ? 1 : 0) << ','
        << (movementTelemetry.pathCollisionAvoidance ? 1 : 0) << ','
        << (movementTelemetry.movementSuppressed ? 1 : 0) << ',' << movementTelemetry.strafeDirection << ','
        << movementTelemetry.strafeChangeMsec << ',' << (movementTelemetry.isLeaning ? 1 : 0) << ','
        << (movementTelemetry.strafeApplied ? 1 : 0) << ','
        << (movementTelemetry.strafeClearanceFlip ? 1 : 0) << ',' << movementTelemetry.strafeClearance << ','
        << movementTelemetry.strafeOtherClearance << ',' << movementTelemetry.strafeProbeFraction << ','
        << movementTelemetry.strafeIntensity << ',' << movementTelemetry.radialDirection << ','
        << movementTelemetry.radialChangeMsec << ',' << (movementTelemetry.radialActive ? 1 : 0) << ','
        << (movementTelemetry.radialForcedCloseRetreat ? 1 : 0) << ',' << movementTelemetry.radialDistance << ','
        << movementTelemetry.radialDesiredMove << ',' << movementTelemetry.radialBeforeMove << ','
        << (movementTelemetry.guardTriggered ? 1 : 0) << ',' << (movementTelemetry.guardHitSentient ? 1 : 0) << ','
        << (movementTelemetry.guardHitWorld ? 1 : 0) << ','
        << (movementTelemetry.guardRemovedComponent ? 1 : 0) << ',' << movementTelemetry.guardFraction << ','
        << movementTelemetry.guardEntity;
}

static float AxisGap(float firstMin, float firstMax, float secondMin, float secondMax)
{
    if (firstMax < secondMin) {
        return secondMin - firstMax;
    }
    if (secondMax < firstMin) {
        return firstMin - secondMax;
    }
    return 0.0f;
}

static std::string CvarLine(const char *name, cvar_t *cvar)
{
    std::ostringstream line;
    line << name << '=' << (cvar ? cvar->string : "") << '\n';
    return line.str();
}

static int LastMetadataSchema(const char *path)
{
    char *buffer = NULL;
    const long length = gi.FS_ReadFile(path, reinterpret_cast<void **>(&buffer), qtrue);
    if (length <= 0 || !buffer) {
        return 0;
    }

    const std::string metadata(buffer, static_cast<size_t>(length));
    gi.FS_FreeFile(buffer);

    const size_t marker = metadata.rfind("schema=");
    if (marker == std::string::npos) {
        return 0;
    }
    return std::atoi(metadata.c_str() + marker + 7);
}

static bool EnsureOpen()
{
    if (!g_movelog || !g_movelog->integer) {
        return false;
    }
    if (framesFile && eventsFile) {
        return true;
    }

    long framesLength = gi.FS_ReadFile(framesPath.c_str(), NULL, qtrue);
    if (framesLength >= static_cast<long>(MOVELOG_SEGMENT_LIMIT)) {
        SelectFreshSegmentPaths();
        framesLength = gi.FS_ReadFile(framesPath.c_str(), NULL, qtrue);
    }

    const long eventsLength = gi.FS_ReadFile(eventsPath.c_str(), NULL, qtrue);
    const long metaLength   = gi.FS_ReadFile(metaPath.c_str(), NULL, qtrue);
    const bool framesNeedHeader = framesLength <= 0;
    const bool eventsNeedHeader = eventsLength <= 0;
    const bool metaNeedsHeader   = metaLength <= 0;
    const bool anyExistingLog   = !framesNeedHeader || !eventsNeedHeader || !metaNeedsHeader;
    const bool allExistingLogs  = !framesNeedHeader && !eventsNeedHeader && !metaNeedsHeader;

    if ((anyExistingLog && !allExistingLogs) || (allExistingLogs && LastMetadataSchema(metaPath.c_str()) != MOVELOG_SCHEMA)) {
        gi.Printf(
            "g_movelog: existing telemetry files do not use schema %d; archive/delete all three movement_* files "
            "before recording\n",
            MOVELOG_SCHEMA
        );
        gi.cvar_set("g_movelog", "0");
        return false;
    }

    const std::string mapName = SanitizeFilename(level.current_map);
    std::ostringstream id;
    id << mapName << '_' << static_cast<long long>(std::time(NULL)) << '_' << gi.Milliseconds();
    sessionId = id.str();

    framesFile = gi.FS_FOpenFileAppend(framesPath.c_str());
    eventsFile = gi.FS_FOpenFileAppend(eventsPath.c_str());
    if (!framesFile || !eventsFile) {
        if (framesFile) {
            gi.FS_FCloseFile(framesFile);
        }
        if (eventsFile) {
            gi.FS_FCloseFile(eventsFile);
        }
        framesFile = eventsFile = 0;
        gi.Printf("g_movelog: could not open telemetry output files\n");
        gi.cvar_set("g_movelog", "0");
        return false;
    }

    sessionStartMsec = level.inttime;
    nextSampleMsec   = level.inttime;
    nextFlushMsec    = level.inttime + MOVELOG_FLUSH_MSEC;
    framesBytesWritten = framesLength > 0 ? static_cast<unsigned long long>(framesLength) : 0;

    framesBuffer.clear();
    eventsBuffer.clear();
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        visibilityStates[i] = VisibilityState();
    }
    if (framesNeedHeader) {
        framesBuffer =
            "schema,session_id,session_ms,server_ms,frame,frame_ms,map,client_id,name,model,is_bot,team,alive,spectator,"
            "ping,health,max_health,origin_x,origin_y,origin_z,eye_x,eye_y,eye_z,velocity_x,velocity_y,velocity_z,"
            "speed_xy,speed_xyz,view_pitch,view_yaw,view_roll,cmd_server_ms,cmd_msec,cmd_angle_pitch,cmd_angle_yaw,"
            "cmd_angle_roll,cmd_forward,cmd_right,cmd_up,buttons,attack_primary,attack_secondary,run,use,lean_left,"
            "lean_right,pm_flags,move_result,on_ground,on_ladder,zoomed,bbox_min_x,bbox_min_y,bbox_min_z,bbox_max_x,"
            "bbox_max_y,bbox_max_z,weapon,weapon_state,clip_ammo,clip_size,reserve_ammo,fire_spread_mult,opponent_id,"
            "opponent_name,opponent_bot,opponent_origin_x,opponent_origin_y,opponent_origin_z,opponent_eye_x,"
            "opponent_eye_y,opponent_eye_z,opponent_velocity_x,opponent_velocity_y,opponent_velocity_z,distance_xy,"
            "distance_xyz,body_gap_xy,body_contact,height_delta,relative_bearing,self_approach_speed,"
            "self_tangential_speed,opponent_approach_speed,closing_speed,aim_pitch_error,aim_yaw_error,aim_total_error,"
            "aim_dot,aim_closest_miss,aim_height_fraction,line_of_sight,crosshair_entity,crosshair_distance,"
            "crosshair_on_opponent,clear_front,clear_back,clear_left,clear_right,clear_front_left,clear_front_right,"
            "clear_back_left,clear_back_right,sight_blocker_entity,sight_fraction,sight_distance,sight_normal_x,"
            "sight_normal_y,sight_normal_z,clear_move,clear_move_entity,clear_move_normal_x,clear_move_normal_y,"
            "clear_move_normal_z,clear_move_startsolid,clear_reverse,clear_reverse_entity,clear_reverse_normal_x,"
            "clear_reverse_normal_y,clear_reverse_normal_z,clear_reverse_startsolid,clear_toward_opponent,"
            "clear_toward_opponent_entity,clear_toward_opponent_normal_x,clear_toward_opponent_normal_y,"
            "clear_toward_opponent_normal_z,clear_toward_opponent_startsolid,clear_away_opponent,"
            "clear_away_opponent_entity,clear_away_opponent_normal_x,clear_away_opponent_normal_y,"
            "clear_away_opponent_normal_z,clear_away_opponent_startsolid,bot_state_flags,bot_enemy_id,"
            "bot_enemy_visible,bot_can_attack,bot_fire_decision,bot_wants_fire,bot_no_move,"
            "bot_reaction_remaining_ms,bot_enemy_distance,bot_aim_acquire_ms,bot_aim_height_fraction,"
            "bot_aim_error_fraction,bot_aim_error_units,bot_aim_latency_ms,bot_aim_target_x,bot_aim_target_y,"
            "bot_aim_target_z,bot_aim_point_x,bot_aim_point_y,bot_aim_point_z,bot_aim_error_dir_x,"
            "bot_aim_error_dir_y,bot_aim_error_dir_z,bot_target_pitch,bot_target_yaw,bot_contact_source,"
            "bot_contact_enemy_id,bot_contact_reporter_id,bot_contact_age_ms,bot_contact_responder,"
            "bot_contact_x,bot_contact_y,bot_contact_z,bot_objective_state,bot_objective_site,"
            "bot_objective_round,bot_objective_use_phase,bot_objective_attacker,bot_objective_critical,"
            "bot_objective_x,bot_objective_y,bot_objective_z,bot_has_combat_target,"
            "bot_pathing,bot_blocked_recovery,bot_path_collision_avoidance,bot_movement_suppressed,"
            "bot_strafe_direction,bot_strafe_change_ms,bot_is_leaning,bot_strafe_applied,"
            "bot_strafe_clearance_flip,bot_strafe_clearance,bot_strafe_other_clearance,"
            "bot_strafe_probe_fraction,bot_strafe_intensity,bot_radial_direction,bot_radial_change_ms,"
            "bot_radial_active,bot_radial_forced_close_retreat,bot_radial_distance,bot_radial_desired_move,"
            "bot_radial_before_move,bot_guard_triggered,bot_guard_hit_sentient,bot_guard_hit_world,"
            "bot_guard_removed_component,bot_guard_fraction,bot_guard_entity\n";
    }
    if (eventsNeedHeader) {
        eventsBuffer =
            "schema,session_id,session_ms,server_ms,frame,event,chat_message,chat_mode,actor_id,actor_name,actor_bot,"
            "target_id,target_name,target_bot,weapon,fire_mode,damage,health_before,health_after,means_of_death,"
            "hit_location,position_x,"
            "position_y,position_z,direction_x,direction_y,direction_z,view_pitch,view_yaw,view_roll,aim_target_id,"
            "aim_pitch_error,aim_yaw_error,aim_total_error,line_of_sight,crosshair_entity,crosshair_on_target,"
            "crosshair_distance,crosshair_hit_class,crosshair_normal_x,crosshair_normal_y,crosshair_normal_z,"
            "sight_blocker_entity,sight_blocker_class,sight_fraction,sight_distance,sight_normal_x,sight_normal_y,"
            "sight_normal_z\n";
    }

    std::ostringstream meta;
    if (metaNeedsHeader) {
        meta << "# OpenMoHAA movement and aim telemetry sessions\n\n";
    }
    meta << "[session " << sessionId << "]\n"
         << "schema=" << MOVELOG_SCHEMA << '\n'
         << "session_id=" << sessionId << '\n'
         << "created_epoch=" << static_cast<long long>(std::time(NULL)) << '\n'
         << "map=" << (level.current_map ? level.current_map : "") << '\n'
         << "game_dir=" << (gi.GameDir() ? gi.GameDir() : "") << '\n'
         << "target_game=" << static_cast<int>(g_target_game) << '\n'
         << "protocol=" << g_protocol << '\n'
         << "sample_hz=" << (1000 / MOVELOG_SAMPLE_MSEC) << '\n'
         << "telemetry_profile=human_and_bot_decisions\n"
         << "clearance_probe_units=" << MOVELOG_CLEARANCE_RANGE << '\n'
         << CvarLine("sv_mapChecksum", gi.Cvar_Get("sv_mapChecksum", "", 0))
         << CvarLine("g_gametype", g_gametype)
         << CvarLine("sv_fps", sv_fps)
         << CvarLine("sv_runspeed", sv_runspeed)
         << CvarLine("sv_dmspeedmult", sv_dmspeedmult)
         << CvarLine("sv_gravity", sv_gravity)
         << CvarLine("sv_bots", sv_bots)
         << CvarLine("g_playerdmhealth", gi.Cvar_Get("g_playerdmhealth", "100", 0))
         << CvarLine("g_bot_difficulty", g_bot_difficulty)
         << CvarLine("g_bot_attack_react_min_delay", g_bot_attack_react_min_delay)
         << CvarLine("g_bot_aim_height_min", g_bot_aim_height_min)
         << CvarLine("g_bot_aim_height_max", g_bot_aim_height_max)
         << CvarLine("g_bot_aim_error", g_bot_aim_error)
         << CvarLine("g_bot_aim_settle_time", g_bot_aim_settle_time)
         << CvarLine("g_bot_aim_latency", g_bot_aim_latency)
         << CvarLine("g_bot_reload_pistol", g_bot_reload_pistol)
         << CvarLine("g_bot_turn_speed", g_bot_turn_speed)
         << CvarLine("g_bot_turn_accel", g_bot_turn_accel)
         << CvarLine("g_bot_strafe_intensity", g_bot_strafe_intensity)
         << CvarLine("g_bot_strafe_min_interval", g_bot_strafe_min_interval)
         << CvarLine("g_bot_strafe_max_interval", g_bot_strafe_max_interval)
         << CvarLine("g_bot_peek_min_interval", g_bot_peek_min_interval)
         << CvarLine("g_bot_peek_max_interval", g_bot_peek_max_interval)
         << CvarLine("g_bot_peek_distance", g_bot_peek_distance)
         << CvarLine("g_bot_spread", g_bot_spread)
         << CvarLine("g_bot_team", g_bot_team)
         << CvarLine("g_bot_sniper", g_bot_sniper)
         << CvarLine("g_bot_rifle", g_bot_rifle)
         << CvarLine("g_bot_shotgun", g_bot_shotgun)
         << CvarLine("g_bot_stg", g_bot_stg)
         << CvarLine("g_bot_bar", g_bot_bar)
         << CvarLine("g_accuracy", g_accuracy)
         << '\n';
    const std::string metadata = meta.str();
    fileHandle_t metaFile = gi.FS_FOpenFileAppend(metaPath.c_str());
    if (!metaFile) {
        gi.FS_FCloseFile(framesFile);
        gi.FS_FCloseFile(eventsFile);
        framesFile = eventsFile = 0;
        gi.Printf("g_movelog: could not open telemetry metadata file\n");
        gi.cvar_set("g_movelog", "0");
        return false;
    }
    gi.FS_Write(metadata.data(), metadata.size(), metaFile);
    gi.FS_FCloseFile(metaFile);

    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow("session_start", NULL, NULL, "", -1, 0.0f, 0.0f, 0.0f, -1, -1, zero, zero, zero, NULL);
    FlushBuffers(true);
    gi.Printf(
        "g_movelog: recording session %s to %s, %s, and %s\n",
        sessionId.c_str(),
        framesPath.c_str(),
        eventsPath.c_str(),
        metaPath.c_str()
    );
    return true;
}

static void RecordVisibilityState(
    Player *player, const AimMetrics& aim, const Vector& eye, const Vector& forward, const Vector& viewAngles
)
{
    if (!player || player->entnum < 0 || player->entnum >= MAX_CLIENTS) {
        return;
    }

    VisibilityState& state = visibilityStates[player->entnum];
    if (!aim.target) {
        state = VisibilityState();
        return;
    }

    const bool newTarget = !state.initialized || state.targetEntity != aim.target->entnum;
    const bool changed   = !newTarget && state.visible != aim.lineOfSight;
    const char *eventName = NULL;

    if (newTarget) {
        eventName = aim.lineOfSight ? "los_initial_visible" : "los_initial_hidden";
    } else if (changed) {
        eventName = aim.lineOfSight ? "los_gain" : "los_loss";
    }

    state.initialized  = true;
    state.targetEntity = aim.target->entnum;
    state.visible      = aim.lineOfSight;

    if (eventName) {
        AppendEventRow(
            eventName,
            player,
            aim.target,
            "",
            -1,
            0.0f,
            player->health,
            player->health,
            -1,
            -1,
            eye,
            forward,
            viewAngles,
            &aim
        );
    }
}

static void AppendFrame(Player *player)
{
    Vector eye;
    Vector viewAngles;
    player->GetPlayerView(&eye, &viewAngles);
    Vector forward;
    Vector right;
    viewAngles.AngleVectors(&forward, &right, NULL);

    const AimMetrics aim = CalculateAimMetrics(player, eye, forward);
    RecordVisibilityState(player, aim, eye, forward, viewAngles);
    Player *opponent = aim.target;
    const usercmd_t& command = player->GetLastUsercmd();
    Weapon *weapon = player->GetActiveWeapon(WEAPON_MAIN);

    Vector opponentEye(0.0f, 0.0f, 0.0f);
    Vector relative(0.0f, 0.0f, 0.0f);
    Vector horizontalDirection(0.0f, 0.0f, 0.0f);
    float distanceXY = 0.0f;
    float distanceXYZ = 0.0f;
    float bodyGapXY = 0.0f;
    float heightDelta = 0.0f;
    float relativeBearing = 0.0f;
    float selfApproach = 0.0f;
    float selfTangential = 0.0f;
    float opponentApproach = 0.0f;
    float closingSpeed = 0.0f;

    if (opponent) {
        Vector ignoredAngles;
        opponent->GetPlayerView(&opponentEye, &ignoredAngles);
        relative         = opponent->origin - player->origin;
        distanceXY       = relative.lengthXY();
        distanceXYZ      = relative.length();
        heightDelta      = opponent->origin.z - player->origin.z;
        relativeBearing  = AngleSubtract(relative.toYaw(), viewAngles[YAW]);

        if (distanceXY > 0.001f) {
            horizontalDirection = Vector(relative.x / distanceXY, relative.y / distanceXY, 0.0f);
            const Vector tangent(-horizontalDirection.y, horizontalDirection.x, 0.0f);
            selfApproach       = player->velocity * horizontalDirection;
            selfTangential     = player->velocity * tangent;
            opponentApproach   = opponent->velocity * -horizontalDirection;
            closingSpeed       = (player->velocity - opponent->velocity) * horizontalDirection;
        }

        const float gapX = AxisGap(player->absmin.x, player->absmax.x, opponent->absmin.x, opponent->absmax.x);
        const float gapY = AxisGap(player->absmin.y, player->absmax.y, opponent->absmin.y, opponent->absmax.y);
        bodyGapXY = std::sqrt(gapX * gapX + gapY * gapY);
    }

    Vector front(forward.x, forward.y, 0.0f);
    Vector side(right.x, right.y, 0.0f);
    front.normalize();
    side.normalize();
    Vector frontLeft = front - side;
    Vector frontRight = front + side;
    Vector backLeft = -front - side;
    Vector backRight = -front + side;
    frontLeft.normalize();
    frontRight.normalize();
    backLeft.normalize();
    backRight.normalize();

    Vector commandDirection = front * (float)command.forwardmove + side * (float)command.rightmove;
    commandDirection.z      = 0.0f;

    const ClearanceMetrics clearMove     = TraceDirectionalClearance(player, commandDirection);
    const ClearanceMetrics clearReverse  = TraceDirectionalClearance(player, -commandDirection);
    const ClearanceMetrics clearToward   = TraceDirectionalClearance(player, horizontalDirection);
    const ClearanceMetrics clearAway     = TraceDirectionalClearance(player, -horizontalDirection);

    const char *weaponName = weapon && weapon->GetItemName() ? weapon->GetItemName() : "";
    const int weaponState  = weapon ? static_cast<int>(weapon->GetState()) : -1;
    const int clipAmmo     = weapon ? weapon->ClipAmmo(FIRE_PRIMARY) : -1;
    const int clipSize     = weapon ? weapon->GetClipSize(FIRE_PRIMARY) : -1;
    const int reserveAmmo  = weapon ? weapon->AmmoAvailable(FIRE_PRIMARY) : -1;
    const float spreadMult = weapon ? weapon->GetCurrentFireSpreadMult(FIRE_PRIMARY) : 0.0f;

    std::ostringstream row;
    row << std::fixed << std::setprecision(3)
        << MOVELOG_SCHEMA << ',' << CsvQuote(sessionId.c_str()) << ',' << SessionMsec() << ',' << level.inttime << ','
        << level.framenum << ',' << level.intframetime << ',' << CsvQuote(level.current_map) << ',' << player->entnum << ','
        << CsvQuote(PlayerName(player)) << ',' << CsvQuote(player->client->pers.dm_playermodel) << ','
        << (PlayerIsBot(player) ? 1 : 0) << ',' << static_cast<int>(player->GetTeam()) << ','
        << (player->deadflag == DEAD_NO && player->health > 0 ? 1 : 0) << ',' << (player->IsSpectator() ? 1 : 0)
        << ',' << player->client->ping << ',' << player->health << ',' << player->max_health << ','
        << player->origin.x << ',' << player->origin.y << ',' << player->origin.z << ',' << eye.x << ',' << eye.y << ','
        << eye.z << ',' << player->velocity.x << ',' << player->velocity.y << ',' << player->velocity.z << ','
        << player->velocity.lengthXY() << ',' << player->velocity.length() << ',' << viewAngles.x << ',' << viewAngles.y
        << ',' << viewAngles.z << ',' << command.serverTime << ',' << static_cast<int>(command.msec) << ','
        << command.angles[PITCH] << ',' << command.angles[YAW] << ',' << command.angles[ROLL] << ','
        << static_cast<int>(command.forwardmove) << ',' << static_cast<int>(command.rightmove) << ','
        << static_cast<int>(command.upmove) << ',' << command.buttons << ','
        << ((command.buttons & BUTTON_ATTACKLEFT) ? 1 : 0) << ',' << ((command.buttons & BUTTON_ATTACKRIGHT) ? 1 : 0)
        << ',' << ((command.buttons & BUTTON_RUN) ? 1 : 0) << ',' << ((command.buttons & BUTTON_USE) ? 1 : 0) << ','
        << ((command.buttons & BUTTON_LEAN_LEFT) ? 1 : 0) << ',' << ((command.buttons & BUTTON_LEAN_RIGHT) ? 1 : 0)
        << ',' << player->client->ps.pm_flags << ',' << player->GetMoveResult() << ','
        << (player->groundentity ? 1 : 0) << ',' << (player->GetLadder() ? 1 : 0) << ','
        << (player->IsZoomed() ? 1 : 0) << ',' << player->mins.x << ',' << player->mins.y << ',' << player->mins.z
        << ',' << player->maxs.x << ',' << player->maxs.y << ',' << player->maxs.z << ',' << CsvQuote(weaponName) << ','
        << weaponState << ',' << clipAmmo << ',' << clipSize << ',' << reserveAmmo << ',' << spreadMult << ','
        << (opponent ? opponent->entnum : -1) << ',' << CsvQuote(PlayerName(opponent)) << ','
        << (PlayerIsBot(opponent) ? 1 : 0) << ',' << (opponent ? opponent->origin.x : 0.0f) << ','
        << (opponent ? opponent->origin.y : 0.0f) << ',' << (opponent ? opponent->origin.z : 0.0f) << ','
        << opponentEye.x << ',' << opponentEye.y << ',' << opponentEye.z << ','
        << (opponent ? opponent->velocity.x : 0.0f) << ',' << (opponent ? opponent->velocity.y : 0.0f) << ','
        << (opponent ? opponent->velocity.z : 0.0f) << ',' << distanceXY << ',' << distanceXYZ << ',' << bodyGapXY
        << ',' << (opponent && bodyGapXY <= 0.001f ? 1 : 0) << ',' << heightDelta << ',' << relativeBearing << ','
        << selfApproach << ',' << selfTangential << ',' << opponentApproach << ',' << closingSpeed << ','
        << aim.pitchError << ',' << aim.yawError << ',' << aim.totalError << ',' << aim.dot << ',' << aim.closestMiss
        << ',' << aim.heightFraction << ',' << (aim.lineOfSight ? 1 : 0) << ',' << aim.crosshairEntity << ','
        << aim.crosshairDistance << ',' << (aim.crosshairOnTarget ? 1 : 0) << ',' << TraceClearance(player, front) << ','
        << TraceClearance(player, -front) << ',' << TraceClearance(player, -side) << ',' << TraceClearance(player, side)
        << ',' << TraceClearance(player, frontLeft) << ',' << TraceClearance(player, frontRight) << ','
        << TraceClearance(player, backLeft) << ',' << TraceClearance(player, backRight) << ','
        << aim.sightBlockerEntity << ',' << aim.sightFraction << ',' << aim.sightDistance << ',' << aim.sightNormal.x
        << ',' << aim.sightNormal.y << ',' << aim.sightNormal.z;

    AppendClearanceColumns(row, clearMove);
    AppendClearanceColumns(row, clearReverse);
    AppendClearanceColumns(row, clearToward);
    AppendClearanceColumns(row, clearAway);
    AppendBotColumns(row, player);
    row << '\n';

    framesBuffer += row.str();
}

static void CloseTelemetrySession()
{
    if (!framesFile && !eventsFile) {
        return;
    }

    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow("session_end", NULL, NULL, "", -1, 0.0f, 0.0f, 0.0f, -1, -1, zero, zero, zero, NULL);
    FlushBuffers(true);

    if (framesFile) {
        gi.FS_FCloseFile(framesFile);
    }
    if (eventsFile) {
        gi.FS_FCloseFile(eventsFile);
    }
    gi.Printf("g_movelog: stopped recording session %s\n", sessionId.c_str());

    framesFile = eventsFile = 0;
    framesBuffer.clear();
    eventsBuffer.clear();
    sessionId.clear();
    framesBytesWritten = 0;
}

static void RotateTelemetryIfNeeded()
{
    if (framesBytesWritten
        + static_cast<unsigned long long>(framesBuffer.size())
        < MOVELOG_SEGMENT_LIMIT) {
        return;
    }

    const std::string priorFramesPath = framesPath;
    CloseTelemetrySession();
    SelectFreshSegmentPaths();
    if (EnsureOpen()) {
        gi.Printf(
            "g_movelog: rotated full frame segment %s to fresh triplet %s\n",
            priorFramesPath.c_str(),
            framesPath.c_str()
        );
    }
}

} // namespace

void G_MoveLogFrame()
{
    if (!g_movelog || !g_movelog->integer) {
        if (framesFile || eventsFile) {
            G_MoveLogShutdown();
        }
        return;
    }
    if (!EnsureOpen() || level.inttime < nextSampleMsec) {
        return;
    }

    do {
        nextSampleMsec += MOVELOG_SAMPLE_MSEC;
    } while (nextSampleMsec <= level.inttime);

    for (int i = 0; i < game.maxclients; ++i) {
        gentity_t *edict = &g_entities[i];
        if (!edict->inuse || !edict->entity || !edict->entity->IsSubclassOfPlayer()) {
            continue;
        }
        AppendFrame(static_cast<Player *>(edict->entity));
    }

    const bool timedFlush = level.inttime >= nextFlushMsec;
    FlushBuffers(timedFlush);
    if (timedFlush) {
        nextFlushMsec = level.inttime + MOVELOG_FLUSH_MSEC;
    }
    RotateTelemetryIfNeeded();
}

void G_MoveLogShutdown()
{
    CloseTelemetrySession();
}

void G_MoveLogShot(Sentient *owner, Weapon *weapon, int mode, const Vector& position, const Vector& forward)
{
    Player *actor = AsPlayer(owner);
    if (!actor || !EnsureOpen()) {
        return;
    }

    const AimMetrics aim = CalculateAimMetrics(actor, position, forward);
    Vector viewAngles;
    actor->GetPlayerView(NULL, &viewAngles);
    AppendEventRow(
        "shot",
        actor,
        aim.target,
        weapon && weapon->GetItemName() ? weapon->GetItemName() : "",
        mode,
        0.0f,
        actor->health,
        actor->health,
        weapon ? static_cast<int>(weapon->GetMeansOfDeath(static_cast<firemode_t>(mode))) : -1,
        -1,
        position,
        forward,
        viewAngles,
        &aim
    );
}

void G_MoveLogReload(Sentient *owner, Weapon *weapon)
{
    Player *actor = AsPlayer(owner);
    if (!actor || !EnsureOpen()) {
        return;
    }

    Vector viewAngles;
    Vector forward;
    actor->GetPlayerView(NULL, &viewAngles);
    viewAngles.AngleVectors(&forward, NULL, NULL);
    const AimMetrics aim = CalculateAimMetrics(actor, actor->EyePosition(), forward);
    AppendEventRow(
        "reload",
        actor,
        aim.target,
        weapon && weapon->GetItemName() ? weapon->GetItemName() : "",
        static_cast<int>(FIRE_PRIMARY),
        0.0f,
        actor->health,
        actor->health,
        -1,
        -1,
        actor->origin,
        forward,
        viewAngles,
        &aim
    );
}

void G_MoveLogBotEvent(
    const char *eventName, Player *actor, Player *target, int detail, const Vector& position
)
{
    if (!eventName || !EnsureOpen()) {
        return;
    }

    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow(
        eventName,
        actor,
        target,
        "",
        detail,
        0.0f,
        actor ? actor->health : 0.0f,
        actor ? actor->health : 0.0f,
        -1,
        -1,
        position,
        zero,
        zero,
        NULL
    );
}

void G_MoveLogDamage(
    Sentient     *victim,
    Sentient     *attacker,
    float         damage,
    float         healthBefore,
    float         healthAfter,
    int           meansOfDeath,
    int           location,
    const Vector& position,
    const Vector& direction
)
{
    Player *target = AsPlayer(victim);
    Player *actor  = AsPlayer(attacker);
    if ((!target && !actor) || !EnsureOpen()) {
        return;
    }

    Weapon *weapon = actor ? actor->GetActiveWeapon(WEAPON_MAIN) : NULL;
    Vector viewAngles(0.0f, 0.0f, 0.0f);
    AimMetrics aim;
    if (actor) {
        Vector eye;
        Vector forward;
        actor->GetPlayerView(&eye, &viewAngles);
        viewAngles.AngleVectors(&forward, NULL, NULL);
        aim = CalculateAimMetrics(actor, eye, forward);
    }
    AppendEventRow(
        "damage",
        actor,
        target,
        weapon && weapon->GetItemName() ? weapon->GetItemName() : "",
        -1,
        damage,
        healthBefore,
        healthAfter,
        meansOfDeath,
        location,
        position,
        direction,
        viewAngles,
        actor ? &aim : NULL
    );
}

void G_MoveLogDeath(Player *victim, Entity *attacker, int meansOfDeath, int location)
{
    Player *actor = AsPlayer(attacker);
    if (!victim || !EnsureOpen()) {
        return;
    }

    Weapon *weapon = actor ? actor->GetActiveWeapon(WEAPON_MAIN) : NULL;
    Vector viewAngles(0.0f, 0.0f, 0.0f);
    AimMetrics aim;
    if (actor) {
        Vector forward;
        actor->GetPlayerView(NULL, &viewAngles);
        viewAngles.AngleVectors(&forward, NULL, NULL);
        aim = CalculateAimMetrics(actor, actor->EyePosition(), forward);
    }
    AppendEventRow(
        "death",
        actor,
        victim,
        weapon && weapon->GetItemName() ? weapon->GetItemName() : "",
        -1,
        0.0f,
        victim->health,
        victim->health,
        meansOfDeath,
        location,
        victim->origin,
        Vector(0.0f, 0.0f, 0.0f),
        viewAngles,
        actor ? &aim : NULL
    );
}

void G_MoveLogSpawn(Player *player)
{
    if (!player || !EnsureOpen()) {
        return;
    }

    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow(
        "spawn",
        player,
        NULL,
        "",
        -1,
        0.0f,
        player->health,
        player->health,
        -1,
        -1,
        player->origin,
        zero,
        zero,
        NULL
    );
}

static void G_MoveLogClientMarker(const char *eventName, Player *player)
{
    if (!IsRecordablePlayer(player) || !EnsureOpen()) {
        return;
    }

    Vector eye;
    Vector viewAngles;
    player->GetPlayerView(&eye, &viewAngles);
    Vector forward;
    viewAngles.AngleVectors(&forward, NULL, NULL);
    const AimMetrics aim = CalculateAimMetrics(player, eye, forward);

    AppendEventRow(
        eventName,
        player,
        aim.target,
        "",
        -1,
        0.0f,
        player->health,
        player->health,
        -1,
        -1,
        player->origin,
        forward,
        viewAngles,
        &aim
    );
    FlushBuffers(true);
}

void G_MoveLogClientBegin(Player *player)
{
    G_MoveLogClientMarker("client_begin", player);
}

void G_MoveLogClientDisconnect(Player *player)
{
    G_MoveLogClientMarker("client_disconnect", player);
    if (player && player->entnum >= 0 && player->entnum < MAX_CLIENTS) {
        visibilityStates[player->entnum] = VisibilityState();
    }
}

void G_MoveLogChat(Player *player, int mode, const char *message)
{
    if (!IsRecordablePlayer(player) || !message || !*message || !EnsureOpen()) {
        return;
    }

    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow(
        "chat",
        player,
        NULL,
        "",
        -1,
        0.0f,
        player->health,
        player->health,
        -1,
        -1,
        player->origin,
        zero,
        zero,
        NULL,
        message,
        mode
    );
}
