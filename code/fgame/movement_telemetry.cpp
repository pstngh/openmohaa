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
// movement_telemetry.cpp: opt-in (g_movelog), server-side movement and aim
// telemetry, ported from the movement lab logger (schema 12).
//
// Output goes below the active game directory: telemetry/movement_frames.csv
// (one row per player per 50 ms server frame), telemetry/movement_events.csv
// and telemetry/movement_meta.txt (one INI section per session). With
// g_movelog_max_mb or g_movelog_max_seconds set, a capture goes to fresh files
// under telemetry/segments/ and stops at the limit.
//
// The recorder deliberately does not mutate game state and never calls a
// random-number function.

#include "movement_telemetry.h"

#include "g_local.h"
#include "g_main.h"
#include "g_utils.h"
#include "g_bot.h"
#include "game.h"
#include "gamecvars.h"
#include "humanbot_adapter.h"
#include "movement_telemetry_schema.h"
#include "player.h"
#include "sentient.h"
#include "weapon.h"
#include "../humanbot/hb_diag.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace
{
constexpr int                MOVELOG_SAMPLE_MSEC     = 50;
constexpr int                MOVELOG_FLUSH_MSEC      = 1000;
constexpr size_t             MOVELOG_BUFFER_LIMIT    = 64 * 1024;
constexpr unsigned long long MOVELOG_SEGMENT_LIMIT   = 1536ULL * 1024ULL * 1024ULL;
constexpr float              MOVELOG_CLEARANCE_RANGE = 128.0f;
constexpr float              MOVELOG_AIM_RANGE       = 8192.0f;
constexpr float              RAD_TO_DEG              = 57.29577951308232f;
constexpr const char        *MOVELOG_FRAMES_PATH     = "telemetry/movement_frames.csv";
constexpr const char        *MOVELOG_EVENTS_PATH     = "telemetry/movement_events.csv";
constexpr const char        *MOVELOG_META_PATH       = "telemetry/movement_meta.txt";

fileHandle_t       framesFile = 0;
fileHandle_t       eventsFile = 0;
std::string        framesBuffer;
std::string        eventsBuffer;
std::string        framesPath(MOVELOG_FRAMES_PATH);
std::string        eventsPath(MOVELOG_EVENTS_PATH);
std::string        metaPath(MOVELOG_META_PATH);
std::string        sessionId;
unsigned long long framesBytesWritten  = 0;
unsigned long long captureBytesWritten = 0;
int                sessionStartMsec    = 0;
int                captureStartMsec    = 0;
int                nextSampleMsec      = 0;
int                nextFlushMsec       = 0;
bool               captureActive       = false;

//
// Row shape guard: the first frame row and the first event row of every session
// are checked against the width of their header. On a mismatch recording is
// disabled and no further row of that kind is written.
//
enum RowShape {
    ROW_SHAPE_UNCHECKED,
    ROW_SHAPE_OK,
    ROW_SHAPE_BROKEN,
};

RowShape frameRowShape = ROW_SHAPE_UNCHECKED;
RowShape eventRowShape = ROW_SHAPE_UNCHECKED;

static bool BoundedCaptureRequested()
{
    return (g_movelog_max_mb && g_movelog_max_mb->integer > 0)
        || (g_movelog_max_seconds && g_movelog_max_seconds->integer > 0);
}

static void SelectFreshSegmentPaths()
{
    std::ostringstream prefix;
    prefix << "telemetry/segments/" << static_cast<long long>(std::time(NULL)) << '_' << gi.Milliseconds();
    framesPath = prefix.str() + "_movement_frames.csv";
    eventsPath = prefix.str() + "_movement_events.csv";
    metaPath   = prefix.str() + "_movement_meta.txt";
}

struct VisibilityState {
    int  targetEntity;
    bool initialized;
    bool visible;

    VisibilityState()
        : targetEntity(ENTITYNUM_NONE)
        , initialized(false)
        , visible(false)
    {}
};

VisibilityState visibilityStates[MAX_CLIENTS];

//
// ext_usercmds: Player::GetUsercmdCount() when the slot's previous frame row
// was written, or when counting (re)started on client begin or session start.
// No player means no baseline.
//
struct UsercmdBaseline {
    const Player *player;
    unsigned int  count;

    UsercmdBaseline()
        : player(NULL)
        , count(0)
    {}
};

UsercmdBaseline usercmdBaselines[MAX_CLIENTS];

struct ClearanceMetrics {
    float  distance;
    int    entity;
    Vector normal;
    bool   startSolid;

    ClearanceMetrics()
        : distance(-1.0f)
        , entity(ENTITYNUM_NONE)
        , normal(vec_zero)
        , startSolid(false)
    {}
};

struct AimMetrics {
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
        : target(NULL)
        , pitchError(0.0f)
        , yawError(0.0f)
        , totalError(0.0f)
        , dot(0.0f)
        , closestMiss(0.0f)
        , heightFraction(0.0f)
        , lineOfSight(false)
        , crosshairEntity(ENTITYNUM_NONE)
        , crosshairDistance(0.0f)
        , crosshairNormal(vec_zero)
        , crosshairOnTarget(false)
        , sightBlockerEntity(ENTITYNUM_NONE)
        , sightFraction(1.0f)
        , sightDistance(0.0f)
        , sightNormal(vec_zero)
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

static bool IsClientSlot(int entnum)
{
    return entnum >= 0 && entnum < MAX_CLIENTS;
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

static void AccountCaptureWrite(size_t bytes)
{
    captureBytesWritten += static_cast<unsigned long long>(bytes);
}

static void FlushBuffers(bool force)
{
    if (framesFile && !framesBuffer.empty() && (force || framesBuffer.size() >= MOVELOG_BUFFER_LIMIT)) {
        const size_t bytesWritten = gi.FS_Write(framesBuffer.data(), framesBuffer.size(), framesFile);
        framesBytesWritten += static_cast<unsigned long long>(bytesWritten);
        AccountCaptureWrite(bytesWritten);
        framesBuffer.clear();
    }

    if (eventsFile && !eventsBuffer.empty() && (force || eventsBuffer.size() >= MOVELOG_BUFFER_LIMIT)) {
        const size_t bytesWritten = gi.FS_Write(eventsBuffer.data(), eventsBuffer.size(), eventsFile);
        AccountCaptureWrite(bytesWritten);
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

// Checks the first row of each kind in a session against its header width.
// On a mismatch it disables g_movelog and returns false for every later row.
static bool RowMatchesHeader(RowShape& shape, const std::string& row, size_t headerColumns, const char *kind)
{
    if (shape == ROW_SHAPE_OK) {
        return true;
    }
    if (shape == ROW_SHAPE_BROKEN) {
        return false;
    }

    const size_t fields = MoveLogCsvFieldCount(row);
    if (fields != headerColumns) {
        gi.Printf(
            "g_movelog: ERROR: %s row has %d fields but the %s header has %d columns; disabling recording\n",
            kind,
            static_cast<int>(fields),
            kind,
            static_cast<int>(headerColumns)
        );
        gi.cvar_set("g_movelog", "0");
        shape = ROW_SHAPE_BROKEN;
        return false;
    }

    shape = ROW_SHAPE_OK;
    return true;
}

static void AppendEventRow(
    const char       *eventName,
    Player           *actor,
    Entity           *target,
    const char       *weaponName,
    int               fireMode,
    float             damage,
    float             healthBefore,
    float             healthAfter,
    int               meansOfDeath,
    int               hitLocation,
    const Vector&     position,
    const Vector&     direction,
    const Vector&     viewAngles,
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
        << ',' << (target ? target->entnum : -1) << ',' << CsvQuote(PlayerName(AsPlayer(target))) << ','
        << (PlayerIsBot(AsPlayer(target)) ? 1 : 0) << ',' << CsvQuote(weaponName) << ',' << fireMode << ',' << damage << ','
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

    const std::string text = row.str();
    if (!RowMatchesHeader(eventRowShape, text, MOVELOG_EVENT_COLUMN_COUNT, "event")) {
        return;
    }

    eventsBuffer += text;
    FlushBuffers(false);
}

static Player *FindNearestOpponent(Player *player)
{
    if (!IsRecordablePlayer(player) || player->IsSpectator() || player->deadflag != DEAD_NO || player->health <= 0) {
        return NULL;
    }

    Player          *nearest         = NULL;
    float            nearestDistance = 0.0f;
    const teamtype_t team            = player->GetTeam();
    const bool       hasTeam         = team == TEAM_ALLIES || team == TEAM_AXIS;

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

    const Vector  traceEnd = start + forward * MOVELOG_AIM_RANGE;
    const Vector  zero(0.0f, 0.0f, 0.0f);
    const int     shotMask = MASK_SHOT & ~CONTENTS_TRIGGER;
    const trace_t crosshairTrace =
        G_Trace(start, zero, zero, traceEnd, player, shotMask, qfalse, "G_MoveLog aim");
    result.crosshairEntity   = crosshairTrace.entityNum;
    result.crosshairDistance = MOVELOG_AIM_RANGE * crosshairTrace.fraction;
    result.crosshairNormal   = crosshairTrace.plane.normal;
    result.crosshairClass    = TraceEntityClass(crosshairTrace);

    if (!result.target) {
        return result;
    }

    Vector      targetDelta    = result.target->centroid - start;
    const float targetDistance = targetDelta.length();
    if (targetDistance <= 0.001f) {
        return result;
    }

    Vector       targetDirection = targetDelta / targetDistance;
    const Vector targetAngles    = targetDirection.toAngles();
    const Vector forwardAngles   = forward.toAngles();
    result.pitchError            = AngleSubtract(targetAngles[PITCH], forwardAngles[PITCH]);
    result.yawError              = AngleSubtract(targetAngles[YAW], forwardAngles[YAW]);
    result.dot                   = std::max(-1.0f, std::min(1.0f, forward * targetDirection));
    result.totalError            = std::acos(result.dot) * RAD_TO_DEG;

    const float  projection = targetDelta * forward;
    const Vector closest    = start + forward * std::max(0.0f, projection);
    result.closestMiss      = (result.target->centroid - closest).length();

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

    const trace_t sightTrace =
        G_Trace(start, zero, zero, result.target->centroid, player, shotMask, qfalse, "G_MoveLog sight");
    result.lineOfSight       = sightTrace.fraction >= 0.999f || sightTrace.entityNum == result.target->entnum;
    result.crosshairOnTarget = crosshairTrace.entityNum == result.target->entnum;
    result.sightFraction     = sightTrace.fraction;
    result.sightDistance     = targetDistance * sightTrace.fraction;
    if (!result.lineOfSight) {
        result.sightBlockerEntity = sightTrace.entityNum;
        result.sightNormal        = sightTrace.plane.normal;
        result.sightBlockerClass  = TraceEntityClass(sightTrace);
    }
    return result;
}

static float TraceClearance(Player *player, const Vector& direction)
{
    const Vector  end       = player->origin + direction * MOVELOG_CLEARANCE_RANGE;
    const int     worldMask = MASK_PLAYERSOLID & ~CONTENTS_BODY & ~CONTENTS_TRIGGER;
    const trace_t trace =
        G_Trace(player->origin, player->mins, player->maxs, end, player, worldMask, qfalse, "G_MoveLog clearance");

    return trace.startsolid ? 0.0f : trace.fraction * MOVELOG_CLEARANCE_RANGE;
}

static ClearanceMetrics TraceDirectionalClearance(Player *player, Vector direction)
{
    ClearanceMetrics result;
    direction.z = 0.0f;
    if (!player || direction.normalize() <= 0.0f) {
        return result;
    }

    const Vector  end           = player->origin + direction * MOVELOG_CLEARANCE_RANGE;
    const int     collisionMask = MASK_PLAYERSOLID & ~CONTENTS_TRIGGER;
    const trace_t trace         = G_Trace(
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

// Starts counting ext_usercmds for `player` from its current usercmd count.
static void ResetUsercmdBaseline(Player *player)
{
    if (!player || !IsClientSlot(player->entnum)) {
        return;
    }

    usercmdBaselines[player->entnum].player = player;
    usercmdBaselines[player->entnum].count  = player->GetUsercmdCount();
}

// Session start: every connected player counts from now, empty slots have no baseline.
static void ResetUsercmdBaselines()
{
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        usercmdBaselines[i] = UsercmdBaseline();
    }

    for (int i = 0; i < game.maxclients && i < MAX_CLIENTS; ++i) {
        gentity_t *edict = &g_entities[i];
        if (edict->inuse && edict->entity && edict->entity->IsSubclassOfPlayer()) {
            ResetUsercmdBaseline(static_cast<Player *>(edict->entity));
        }
    }
}

// Usercmds the player's ClientThink accepted since its previous frame row. A
// player without a baseline in its slot writes 0 and starts one.
static unsigned int UsercmdsSinceLastRow(Player *player)
{
    if (!IsClientSlot(player->entnum)) {
        return 0;
    }

    UsercmdBaseline&   baseline = usercmdBaselines[player->entnum];
    const unsigned int count    = player->GetUsercmdCount();
    const unsigned int accepted = (baseline.player == player && count >= baseline.count) ? count - baseline.count : 0;

    baseline.player = player;
    baseline.count  = count;
    return accepted;
}

static void AppendExtColumns(std::ostringstream& row, Player *player, Player *opponent)
{
    const Vector eyeOffset    = player->EyePosition() - player->origin;
    int          visibleParts = 0;
    int          inFov        = 0;

    if (opponent) {
        G_HumanBotObserveParts(player, opponent, &visibleParts, &inFov);
    }

    row << ',' << player->client->ps.ping << ',' << UsercmdsSinceLastRow(player) << ','
        << player->client->ps.fLeanAngle << ',' << eyeOffset.x << ',' << eyeOffset.y << ',' << eyeOffset.z << ','
        << visibleParts << ',' << inFov;
}

// HB_DIAG_FIELDS members are written as integers or with the row's fixed
// 3-decimal formatting; any other member type fails to compile here.
static void AppendDiagValue(std::ostringstream& row, int value)
{
    row << ',' << value;
}

static void AppendDiagValue(std::ostringstream& row, float value)
{
    row << ',' << value;
}

static void AppendBotColumns(std::ostringstream& row, Player *player)
{
    hb::Diag diag;
    if (!G_HumanBotGetDiag(player, &diag)) {
        diag = hb::Diag();
    }

#define MOVELOG_APPEND_DIAG(type, member, def, column) AppendDiagValue(row, diag.member);
    HB_DIAG_FIELDS(MOVELOG_APPEND_DIAG)
#undef MOVELOG_APPEND_DIAG
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

// G_HumanBotMetaLines() ("key=value" lines), newline terminated.
static std::string HumanBotMetaLines()
{
    const char *lines  = G_HumanBotMetaLines();
    std::string result = lines ? lines : "";

    if (!result.empty() && result[result.size() - 1] != '\n') {
        result += '\n';
    }
    return result;
}

// Whether one of the "key=value" lines sets `key`. The analysis reads the
// metadata with a strict INI parser, which rejects repeated keys.
static bool HasMetaKey(const std::string& lines, const char *key)
{
    const std::string prefix = std::string(key) + '=';

    for (size_t lineStart = 0; lineStart < lines.size();) {
        if (lines.compare(lineStart, prefix.size(), prefix) == 0) {
            return true;
        }

        const size_t lineEnd = lines.find('\n', lineStart);
        if (lineEnd == std::string::npos) {
            break;
        }
        lineStart = lineEnd + 1;
    }
    return false;
}

static int LastMetadataSchema(const char *path)
{
    char      *buffer = NULL;
    const long length = gi.FS_ReadFile(path, reinterpret_cast<void **>(&buffer), qtrue);
    if (length <= 0 || !buffer) {
        return 0;
    }

    const std::string metadata(buffer, static_cast<size_t>(length));
    gi.FS_FreeFile(buffer);

    // The last "schema=" line; keys that merely end in "schema" do not count.
    const size_t marker = metadata.rfind("\nschema=");
    if (marker == std::string::npos) {
        return 0;
    }
    return std::atoi(metadata.c_str() + marker + 8);
}

// Whether the file at `path` starts with exactly `headerLine` (newline included).
static bool FileStartsWith(const char *path, const std::string& headerLine)
{
    fileHandle_t file   = 0;
    const long   length = gi.FS_FOpenFile(path, &file, qfalse, qtrue);
    if (!file) {
        return false;
    }

    bool matches = false;
    if (length >= static_cast<long>(headerLine.size())) {
        std::string header(headerLine.size(), '\0');
        matches = gi.FS_Read(&header[0], header.size(), file) == header.size() && header == headerLine;
    }

    gi.FS_FCloseFile(file);
    return matches;
}

static bool EnsureOpen()
{
    if (!g_movelog || !g_movelog->integer) {
        return false;
    }
    if (framesFile && eventsFile) {
        return true;
    }

    if (!captureActive) {
        captureActive       = true;
        captureStartMsec    = level.inttime;
        captureBytesWritten = 0;
        if (BoundedCaptureRequested()) {
            SelectFreshSegmentPaths();
        }
    }

    long framesLength = gi.FS_ReadFile(framesPath.c_str(), NULL, qtrue);
    if (framesLength >= static_cast<long>(MOVELOG_SEGMENT_LIMIT)) {
        SelectFreshSegmentPaths();
        framesLength = gi.FS_ReadFile(framesPath.c_str(), NULL, qtrue);
    }

    const long eventsLength     = gi.FS_ReadFile(eventsPath.c_str(), NULL, qtrue);
    const long metaLength       = gi.FS_ReadFile(metaPath.c_str(), NULL, qtrue);
    const bool framesNeedHeader = framesLength <= 0;
    const bool eventsNeedHeader = eventsLength <= 0;
    const bool metaNeedsHeader  = metaLength <= 0;
    const bool anyExistingLog   = !framesNeedHeader || !eventsNeedHeader || !metaNeedsHeader;
    const bool allExistingLogs  = !framesNeedHeader && !eventsNeedHeader && !metaNeedsHeader;

    if ((anyExistingLog && !allExistingLogs)
        || (allExistingLogs && LastMetadataSchema(metaPath.c_str()) != MOVELOG_SCHEMA)) {
        gi.Printf(
            "g_movelog: existing telemetry files do not use schema %d; archive/delete all three movement_* files "
            "before recording\n",
            MOVELOG_SCHEMA
        );
        gi.cvar_set("g_movelog", "0");
        return false;
    }

    const std::string frameHeader = MoveLogFrameHeaderLine();
    const std::string eventHeader = MoveLogEventHeaderLine();

    if (allExistingLogs
        && (!FileStartsWith(framesPath.c_str(), frameHeader) || !FileStartsWith(eventsPath.c_str(), eventHeader))) {
        gi.Printf(
            "g_movelog: existing telemetry files use schema %d with other columns than this build; archive/delete "
            "all three movement_* files before recording\n",
            MOVELOG_SCHEMA
        );
        gi.cvar_set("g_movelog", "0");
        return false;
    }

    const std::string  mapName = SanitizeFilename(level.current_map);
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

    sessionStartMsec   = level.inttime;
    nextSampleMsec     = level.inttime;
    nextFlushMsec      = level.inttime + MOVELOG_FLUSH_MSEC;
    framesBytesWritten = framesLength > 0 ? static_cast<unsigned long long>(framesLength) : 0;
    frameRowShape      = ROW_SHAPE_UNCHECKED;
    eventRowShape      = ROW_SHAPE_UNCHECKED;

    framesBuffer.clear();
    eventsBuffer.clear();
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        visibilityStates[i] = VisibilityState();
    }
    ResetUsercmdBaselines();

    if (framesNeedHeader) {
        framesBuffer = frameHeader;
    }
    if (eventsNeedHeader) {
        eventsBuffer = eventHeader;
    }

    const std::string  botMeta  = HumanBotMetaLines();
    const char        *hbSha256 = G_HumanBotModelSha256();
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
         << "telemetry_profile=human_and_bot_brain\n"
         << "clearance_probe_units=" << MOVELOG_CLEARANCE_RANGE << '\n'
         << CvarLine("sv_mapChecksum", gi.Cvar_Get("sv_mapChecksum", "", 0)) << CvarLine("g_gametype", g_gametype)
         << CvarLine("sv_fps", sv_fps) << CvarLine("sv_runspeed", sv_runspeed)
         << CvarLine("sv_dmspeedmult", sv_dmspeedmult) << CvarLine("sv_gravity", sv_gravity)
         // Player::InitHealth creates this cvar on the first spawn; use its default.
         << CvarLine("g_playerdmhealth", gi.Cvar_Get("g_playerdmhealth", "100", 0))
         << CvarLine("sv_maxbots", sv_maxbots) << CvarLine("sv_numbots", sv_numbots)
         << CvarLine("sv_minPlayers", sv_minPlayers) << CvarLine("dmflags", dmflags)
         << CvarLine("com_target_game", gi.Cvar_Get("com_target_game", "", 0))
         << CvarLine("g_movelog_max_mb", g_movelog_max_mb)
         << CvarLine("g_movelog_max_seconds", g_movelog_max_seconds) << botMeta;
    if (!HasMetaKey(botMeta, "hb_model_sha256")) {
        meta << "hb_model_sha256=" << (hbSha256 ? hbSha256 : "") << '\n';
    }
    meta << '\n';

    const std::string metadata = meta.str();
    fileHandle_t      metaFile = gi.FS_FOpenFileAppend(metaPath.c_str());
    if (!metaFile) {
        gi.FS_FCloseFile(framesFile);
        gi.FS_FCloseFile(eventsFile);
        framesFile = eventsFile = 0;
        gi.Printf("g_movelog: could not open telemetry metadata file\n");
        gi.cvar_set("g_movelog", "0");
        return false;
    }
    AccountCaptureWrite(gi.FS_Write(metadata.data(), metadata.size(), metaFile));
    gi.FS_FCloseFile(metaFile);

    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow("session_start", NULL, NULL, "", -1, 0.0f, 0.0f, 0.0f, -1, -1, zero, zero, zero, NULL);
    if (eventRowShape == ROW_SHAPE_BROKEN) {
        // The event writer does not match the header; recording is already disabled.
        G_MoveLogShutdown();
        return false;
    }
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
    if (!player || !IsClientSlot(player->entnum)) {
        return;
    }

    VisibilityState& state = visibilityStates[player->entnum];
    if (!aim.target) {
        state = VisibilityState();
        return;
    }

    const bool  newTarget = !state.initialized || state.targetEntity != aim.target->entnum;
    const bool  changed   = !newTarget && state.visible != aim.lineOfSight;
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

// Appends the player's frame row. Returns false when the row does not match
// the frame header (recording is then disabled).
static bool AppendFrame(Player *player)
{
    Vector eye;
    Vector viewAngles;
    player->GetPlayerView(&eye, &viewAngles);
    Vector forward;
    Vector right;
    viewAngles.AngleVectors(&forward, &right, NULL);

    const AimMetrics aim = CalculateAimMetrics(player, eye, forward);
    RecordVisibilityState(player, aim, eye, forward, viewAngles);
    Player          *opponent = aim.target;
    const usercmd_t& command  = player->GetLastUsercmd();
    Weapon          *weapon   = player->GetActiveWeapon(WEAPON_MAIN);

    Vector opponentEye(0.0f, 0.0f, 0.0f);
    Vector relative(0.0f, 0.0f, 0.0f);
    Vector horizontalDirection(0.0f, 0.0f, 0.0f);
    float  distanceXY       = 0.0f;
    float  distanceXYZ      = 0.0f;
    float  bodyGapXY        = 0.0f;
    float  heightDelta      = 0.0f;
    float  relativeBearing  = 0.0f;
    float  selfApproach     = 0.0f;
    float  selfTangential   = 0.0f;
    float  opponentApproach = 0.0f;
    float  closingSpeed     = 0.0f;

    if (opponent) {
        Vector ignoredAngles;
        opponent->GetPlayerView(&opponentEye, &ignoredAngles);
        relative        = opponent->origin - player->origin;
        distanceXY      = relative.lengthXY();
        distanceXYZ     = relative.length();
        heightDelta     = opponent->origin.z - player->origin.z;
        relativeBearing = AngleSubtract(relative.toYaw(), viewAngles[YAW]);

        if (distanceXY > 0.001f) {
            horizontalDirection = Vector(relative.x / distanceXY, relative.y / distanceXY, 0.0f);
            const Vector tangent(-horizontalDirection.y, horizontalDirection.x, 0.0f);
            selfApproach     = player->velocity * horizontalDirection;
            selfTangential   = player->velocity * tangent;
            opponentApproach = opponent->velocity * -horizontalDirection;
            closingSpeed     = (player->velocity - opponent->velocity) * horizontalDirection;
        }

        const float gapX = AxisGap(player->absmin.x, player->absmax.x, opponent->absmin.x, opponent->absmax.x);
        const float gapY = AxisGap(player->absmin.y, player->absmax.y, opponent->absmin.y, opponent->absmax.y);
        bodyGapXY        = std::sqrt(gapX * gapX + gapY * gapY);
    }

    Vector front(forward.x, forward.y, 0.0f);
    Vector side(right.x, right.y, 0.0f);
    front.normalize();
    side.normalize();
    Vector frontLeft  = front - side;
    Vector frontRight = front + side;
    Vector backLeft   = -front - side;
    Vector backRight  = -front + side;
    frontLeft.normalize();
    frontRight.normalize();
    backLeft.normalize();
    backRight.normalize();

    Vector commandDirection = front * (float)command.forwardmove + side * (float)command.rightmove;
    commandDirection.z      = 0.0f;

    const ClearanceMetrics clearMove    = TraceDirectionalClearance(player, commandDirection);
    const ClearanceMetrics clearReverse = TraceDirectionalClearance(player, -commandDirection);
    const ClearanceMetrics clearToward  = TraceDirectionalClearance(player, horizontalDirection);
    const ClearanceMetrics clearAway    = TraceDirectionalClearance(player, -horizontalDirection);

    const char *weaponName  = weapon && weapon->GetItemName() ? weapon->GetItemName() : "";
    const int   weaponState = weapon ? static_cast<int>(weapon->GetState()) : -1;
    const int   clipAmmo    = weapon ? weapon->ClipAmmo(FIRE_PRIMARY) : -1;
    const int   clipSize    = weapon ? weapon->GetClipSize(FIRE_PRIMARY) : -1;
    const int   reserveAmmo = weapon ? weapon->AmmoAvailable(FIRE_PRIMARY) : -1;
    const float spreadMult  = weapon ? weapon->GetCurrentFireSpreadMult(FIRE_PRIMARY) : 0.0f;

    // Core columns: MOVELOG_FRAME_CORE_COLUMNS, same values as the lab logger.
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
    // MOVELOG_FRAME_EXT_COLUMNS, then the bot_* columns in HB_DIAG_FIELDS order.
    AppendExtColumns(row, player, opponent);
    AppendBotColumns(row, player);
    row << '\n';

    const std::string text = row.str();
    if (!RowMatchesHeader(frameRowShape, text, MOVELOG_FRAME_COLUMN_COUNT, "frame")) {
        return false;
    }

    framesBuffer += text;
    return true;
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

static void ResetCaptureState()
{
    captureActive       = false;
    captureStartMsec    = 0;
    captureBytesWritten = 0;
    framesPath          = MOVELOG_FRAMES_PATH;
    eventsPath          = MOVELOG_EVENTS_PATH;
    metaPath            = MOVELOG_META_PATH;
}

static const char *CaptureLimitEvent()
{
    if (g_movelog_max_seconds && g_movelog_max_seconds->integer > 0
        && level.inttime - captureStartMsec >= g_movelog_max_seconds->integer * 1000) {
        return "capture_time_limit";
    }

    if (g_movelog_max_mb && g_movelog_max_mb->integer > 0) {
        const unsigned long long maxBytes = static_cast<unsigned long long>(g_movelog_max_mb->integer) * 1024ULL * 1024ULL;
        const unsigned long long pendingBytes = static_cast<unsigned long long>(framesBuffer.size() + eventsBuffer.size());
        if (captureBytesWritten + pendingBytes >= maxBytes) {
            return "capture_size_limit";
        }
    }

    return NULL;
}

static void StopCaptureAtLimit(const char *eventName)
{
    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow(eventName, NULL, NULL, "", -1, 0.0f, 0.0f, 0.0f, -1, -1, zero, zero, zero, NULL);
    gi.Printf("g_movelog: reached configured telemetry limit (%s); disabling recording\n", eventName);
    CloseTelemetrySession();
    ResetCaptureState();
    gi.cvar_set("g_movelog", "0");
}

static void RotateTelemetryIfNeeded()
{
    if (framesBytesWritten + static_cast<unsigned long long>(framesBuffer.size()) < MOVELOG_SEGMENT_LIMIT) {
        return;
    }

    const std::string priorFramesPath = framesPath;
    CloseTelemetrySession();
    SelectFreshSegmentPaths();
    if (EnsureOpen()) {
        gi.Printf(
            "g_movelog: rotated full frame segment %s to fresh triplet %s\n", priorFramesPath.c_str(), framesPath.c_str()
        );
    }
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

} // namespace

void G_MoveLogFrame()
{
    if (!g_movelog || !g_movelog->integer) {
        if (framesFile || eventsFile || captureActive) {
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
        if (!AppendFrame(static_cast<Player *>(edict->entity))) {
            // The row did not match the header and recording is disabled.
            G_MoveLogShutdown();
            return;
        }
    }

    const bool timedFlush = level.inttime >= nextFlushMsec;
    FlushBuffers(timedFlush);
    if (timedFlush) {
        nextFlushMsec = level.inttime + MOVELOG_FLUSH_MSEC;
    }
    const char *limitEvent = CaptureLimitEvent();
    if (limitEvent) {
        StopCaptureAtLimit(limitEvent);
        return;
    }
    RotateTelemetryIfNeeded();
}

void G_MoveLogShutdown()
{
    CloseTelemetrySession();
    ResetCaptureState();
}

void G_MoveLogShot(Sentient *owner, Weapon *weapon, int mode, const Vector& position, const Vector& forward)
{
    Player *actor = AsPlayer(owner);
    if (!actor || !EnsureOpen()) {
        return;
    }

    const AimMetrics aim = CalculateAimMetrics(actor, position, forward);
    Vector           viewAngles;
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

void G_MoveLogBotEvent(const char *eventName, Player *actor, Entity *target, int eventDetail, const Vector& position)
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
        eventDetail,
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

void G_MoveLogBotStyle(Player *bot, const char *dials)
{
    if (!IsRecordablePlayer(bot) || !EnsureOpen()) {
        return;
    }

    const Vector zero(0.0f, 0.0f, 0.0f);
    AppendEventRow(
        "bot_style",
        bot,
        NULL,
        "",
        -1,
        0.0f,
        bot->health,
        bot->health,
        -1,
        -1,
        bot->origin,
        zero,
        zero,
        NULL,
        dials ? dials : "",
        0
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

    Weapon    *weapon = actor ? actor->GetActiveWeapon(WEAPON_MAIN) : NULL;
    Vector     viewAngles(0.0f, 0.0f, 0.0f);
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

    Weapon    *weapon = actor ? actor->GetActiveWeapon(WEAPON_MAIN) : NULL;
    Vector     viewAngles(0.0f, 0.0f, 0.0f);
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
        "spawn", player, NULL, "", -1, 0.0f, player->health, player->health, -1, -1, player->origin, zero, zero, NULL
    );
}

void G_MoveLogClientBegin(Player *player)
{
    ResetUsercmdBaseline(player);
    G_MoveLogClientMarker("client_begin", player);
}

void G_MoveLogClientDisconnect(Player *player)
{
    G_MoveLogClientMarker("client_disconnect", player);
    if (player && IsClientSlot(player->entnum)) {
        visibilityStates[player->entnum] = VisibilityState();
        usercmdBaselines[player->entnum] = UsercmdBaseline();
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
