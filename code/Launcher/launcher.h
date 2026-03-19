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

// launcher.h: GUI launcher for OpenMoHAA

#pragma once

#include <string>

#define MAX_BOOKMARKS 4

struct Bookmark {
    std::string name;
    std::string ip;
    std::string password;
    std::string rconPassword;
};

struct ResolutionEntry {
    const char *label;
    int         rMode;      // r_mode value, -1 for custom
    int         customWidth; // only used when rMode == -1
    int         customHeight;
};

static const ResolutionEntry resolutionList[] = {
    {"800x600",   4, 0,    0},
    {"960x720",   5, 0,    0},
    {"1024x768",  6, 0,    0},
    {"1152x864",  7, 0,    0},
    {"1280x720",  -1, 1280, 720},
    {"1280x1024", 8, 0,    0},
    {"1600x1200", 9, 0,    0},
    {"1920x1080", -1, 1920, 1080},
    {"3840x2160", -1, 3840, 2160},
};

static const int resolutionCount = sizeof(resolutionList) / sizeof(resolutionList[0]);

struct LauncherSettings {
    std::string ip;
    std::string password;
    std::string rconPassword;
    std::string nickname;
    int         gameType          = 0; // 0=Base (AA), 1=Spearhead (SH), 2=Breakthrough (BT)
    bool        overrideResolution = false; // Whether resolution override is enabled
    int         resolutionIndex   = 0; // Index into resolutionList
    Bookmark    bookmarks[MAX_BOOKMARKS];
};

// Settings persistence (launcher_settings.cpp)
std::string      GetSettingsPath();
LauncherSettings LoadSettings();
void             SaveSettings(const LauncherSettings& settings);

// Game launching (launcher_launch.cpp)
void LaunchGame(const LauncherSettings& settings);
