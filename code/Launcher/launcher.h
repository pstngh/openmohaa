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

struct LauncherSettings {
    std::string ip;
    std::string password;
    std::string rconPassword;
    int         gameType; // 0=Base (AA), 1=Spearhead (SH), 2=Breakthrough (BT)
    Bookmark    bookmarks[MAX_BOOKMARKS];
};

// Settings persistence (launcher_settings.cpp)
std::string      GetSettingsPath();
LauncherSettings LoadSettings();
void             SaveSettings(const LauncherSettings& settings);

// Game launching (launcher_launch.cpp)
void LaunchGame(const LauncherSettings& settings);
