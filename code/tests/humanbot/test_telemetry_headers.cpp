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
// test_telemetry_headers.cpp: column contract of the movement telemetry logger.
//
// Engine-independent. Checks movement_telemetry_schema.h against the golden
// headers (extracted from the movement lab logger's schema 12 header literals)
// and against HB_DIAG_FIELDS.
//
// usage: test_telemetry_headers <golden_dir>

#include "movement_telemetry_schema.h"

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

namespace
{

int failures = 0;

void Fail(const std::string& message)
{
    std::fprintf(stderr, "FAIL: %s\n", message.c_str());
    ++failures;
}

std::vector<std::string> Split(const std::string& line)
{
    std::vector<std::string> names;
    std::string::size_type   start = 0;

    for (;;) {
        const std::string::size_type comma = line.find(',', start);
        names.push_back(line.substr(start, comma == std::string::npos ? std::string::npos : comma - start));
        if (comma == std::string::npos) {
            return names;
        }
        start = comma + 1;
    }
}

std::string Join(const std::vector<std::string>& names)
{
    std::string line;

    for (std::size_t i = 0; i < names.size(); ++i) {
        if (i) {
            line += ',';
        }
        line += names[i];
    }
    return line;
}

std::vector<std::string> ToVector(const char *const *columns, std::size_t count)
{
    return std::vector<std::string>(columns, columns + count);
}

// Reads a golden file holding one comma-separated line (optionally newline
// terminated) and splits it.
std::vector<std::string> ReadGolden(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        Fail("cannot read " + path);
        return std::vector<std::string>();
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (text.size() >= 1 && text[text.size() - 1] == '\n') {
        text.erase(text.size() - 1);
    }
    if (text.size() >= 1 && text[text.size() - 1] == '\r') {
        text.erase(text.size() - 1);
    }
    if (text.empty() || text.find_first_of("\r\n") != std::string::npos) {
        Fail(path + " must hold exactly one non-empty line");
        return std::vector<std::string>();
    }
    return Split(text);
}

void CheckSameList(const std::string& what, const std::vector<std::string>& actual, const std::vector<std::string>& expected)
{
    if (actual.size() != expected.size()) {
        Fail(what + ": " + std::to_string(actual.size()) + " columns, expected " + std::to_string(expected.size()));
    }

    int reported = 0;
    for (std::size_t i = 0; i < actual.size() && i < expected.size() && reported < 5; ++i) {
        if (actual[i] != expected[i]) {
            Fail(what + " column " + std::to_string(i) + ": \"" + actual[i] + "\", expected \"" + expected[i] + "\"");
            ++reported;
        }
    }
}

bool StartsWith(const std::string& text, const char *prefix)
{
    return text.compare(0, std::char_traits<char>::length(prefix), prefix) == 0;
}

// Plain lower-case identifiers: never need CSV quoting and survive the analysis
// loader's name-based column access.
bool IsPlainName(const std::string& name)
{
    if (name.empty()) {
        return false;
    }
    for (const char character : name) {
        if (!((character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') || character == '_')) {
            return false;
        }
    }
    return true;
}

void CheckUnique(const std::string& what, const std::vector<std::string>& names)
{
    std::set<std::string> seen;

    for (const std::string& name : names) {
        if (!seen.insert(name).second) {
            Fail(what + ": duplicate column \"" + name + "\"");
        }
    }
}

} // namespace

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::fprintf(stderr, "usage: %s <golden_dir>\n", argc > 0 ? argv[0] : "test_telemetry_headers");
        return 2;
    }

    const std::string golden(argv[1]);

    //
    // Core frame columns and event columns: byte-identical to the lab logger.
    //
    const std::vector<std::string> goldenCore   = ReadGolden(golden + "/movement_frames_core_header.txt");
    const std::vector<std::string> goldenEvents = ReadGolden(golden + "/movement_events_header.txt");
    const std::vector<std::string> core = ToVector(MOVELOG_FRAME_CORE_COLUMNS, MOVELOG_FRAME_CORE_COLUMN_COUNT);
    const std::vector<std::string> events = ToVector(MOVELOG_EVENT_COLUMNS, MOVELOG_EVENT_COLUMN_COUNT);

    if (goldenCore.size() != 133) {
        Fail("golden core frame header has " + std::to_string(goldenCore.size()) + " columns, expected 133");
    }
    if (goldenEvents.size() != 49) {
        Fail("golden event header has " + std::to_string(goldenEvents.size()) + " columns, expected 49");
    }
    CheckSameList("MOVELOG_FRAME_CORE_COLUMNS", core, goldenCore);
    CheckSameList("MOVELOG_EVENT_COLUMNS", events, goldenEvents);

    //
    // Ext columns, exactly as specified.
    //
    const std::vector<std::string> ext = ToVector(MOVELOG_FRAME_EXT_COLUMNS, MOVELOG_FRAME_EXT_COLUMN_COUNT);
    const std::vector<std::string> expectedExt {
        "ext_ping",
        "ext_usercmds",
        "ext_lean_angle",
        "ext_eye_ofs_x",
        "ext_eye_ofs_y",
        "ext_eye_ofs_z",
        "ext_vis_parts",
        "ext_in_fov",
    };
    CheckSameList("MOVELOG_FRAME_EXT_COLUMNS", ext, expectedExt);

    //
    // Bot columns: the column strings of HB_DIAG_FIELDS, in order.
    //
    const std::vector<std::string> bot = ToVector(MOVELOG_FRAME_BOT_COLUMNS, MOVELOG_FRAME_BOT_COLUMN_COUNT);
    const std::vector<std::string> expectedBot {
#define TEST_DIAG_COLUMN(type, member, def, column) column,
        HB_DIAG_FIELDS(TEST_DIAG_COLUMN)
#undef TEST_DIAG_COLUMN
    };
    CheckSameList("MOVELOG_FRAME_BOT_COLUMNS", bot, expectedBot);
    if (bot.size() != static_cast<std::size_t>(hb::DIAG_COLUMN_COUNT)) {
        Fail("bot column count differs from hb::DIAG_COLUMN_COUNT");
    }

    // The analysis loader splits every bot_* column off the frame table.
    for (const std::string& name : bot) {
        if (!StartsWith(name, "bot_")) {
            Fail("bot column \"" + name + "\" does not start with bot_");
        }
    }
    for (const std::vector<std::string> *list : {&core, &ext}) {
        for (const std::string& name : *list) {
            if (StartsWith(name, "bot_")) {
                Fail("non-bot frame column \"" + name + "\" starts with bot_");
            }
        }
    }

    //
    // Names: plain identifiers, unique across the frame header and the event header.
    //
    std::vector<std::string> frame = core;
    frame.insert(frame.end(), ext.begin(), ext.end());
    frame.insert(frame.end(), bot.begin(), bot.end());

    const std::vector<std::string> *const headers[] = {&frame, &events};
    for (const std::vector<std::string> *list : headers) {
        for (const std::string& name : *list) {
            if (!IsPlainName(name)) {
                Fail("column name \"" + name + "\" is not a plain [a-z0-9_] identifier");
            }
        }
    }
    CheckUnique("frame header", frame);
    CheckUnique("event header", events);
    if (frame.size() != MOVELOG_FRAME_COLUMN_COUNT) {
        Fail("MOVELOG_FRAME_COLUMN_COUNT is not core + ext + bot");
    }

    //
    // Header lines built by the schema helpers.
    //
    std::vector<std::string> expectedFrame = goldenCore;
    expectedFrame.insert(expectedFrame.end(), expectedExt.begin(), expectedExt.end());
    expectedFrame.insert(expectedFrame.end(), expectedBot.begin(), expectedBot.end());

    const std::string frameHeader = MoveLogFrameHeaderLine();
    const std::string eventHeader = MoveLogEventHeaderLine();
    if (frameHeader != Join(expectedFrame) + "\n") {
        Fail("MoveLogFrameHeaderLine() is not core + ext + bot joined by commas");
    }
    if (eventHeader != Join(goldenEvents) + "\n") {
        Fail("MoveLogEventHeaderLine() is not the golden event header");
    }

    //
    // Field counter used by the logger's first-row guard.
    //
    if (MoveLogCsvFieldCount(frameHeader) != MOVELOG_FRAME_COLUMN_COUNT) {
        Fail("MoveLogCsvFieldCount(frame header) is not MOVELOG_FRAME_COLUMN_COUNT");
    }
    if (MoveLogCsvFieldCount(eventHeader) != MOVELOG_EVENT_COLUMN_COUNT) {
        Fail("MoveLogCsvFieldCount(event header) is not MOVELOG_EVENT_COLUMN_COUNT");
    }
    if (MoveLogCsvFieldCount("13,\"a,b\",\"say \"\"hi, there\"\"\",,-1.000\n9,9") != 5) {
        Fail("MoveLogCsvFieldCount miscounts quoted fields");
    }
    if (MoveLogCsvFieldCount("") != 1 || MoveLogCsvFieldCount("x\n") != 1) {
        Fail("MoveLogCsvFieldCount miscounts a single field");
    }

    // Schema numbers up to 12 belong to the lab logger's layouts.
    if (MOVELOG_SCHEMA < 13) {
        Fail("MOVELOG_SCHEMA must be at least 13");
    }

    if (failures) {
        std::fprintf(stderr, "test_telemetry_headers: %d failure(s)\n", failures);
        return 1;
    }

    std::printf(
        "test_telemetry_headers: OK (schema %d, %d frame columns = %d core + %d ext + %d bot, %d event columns)\n",
        MOVELOG_SCHEMA,
        static_cast<int>(MOVELOG_FRAME_COLUMN_COUNT),
        static_cast<int>(MOVELOG_FRAME_CORE_COLUMN_COUNT),
        static_cast<int>(MOVELOG_FRAME_EXT_COLUMN_COUNT),
        static_cast<int>(MOVELOG_FRAME_BOT_COLUMN_COUNT),
        static_cast<int>(MOVELOG_EVENT_COLUMN_COUNT)
    );
    return 0;
}
