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

// launcher_settings.cpp: Load and save launcher configuration

#include "launcher.h"

#include <fstream>
#include <sstream>

#ifdef _WIN32
#    include <Windows.h>
#else
#    include <unistd.h>
#    include <limits.h>
#endif

std::string GetSettingsPath()
{
    std::string dir;

#ifdef _WIN32
    wchar_t path[MAX_PATH] = {0};
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    std::wstring wpath(path);
    size_t       pos = wpath.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        wpath = wpath.substr(0, pos);
    }
    dir = std::string(wpath.begin(), wpath.end());
#else
    char    path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count > 0) {
        dir = std::string(path, count);
        size_t pos = dir.find_last_of('/');
        if (pos != std::string::npos) {
            dir = dir.substr(0, pos);
        }
    } else {
        dir = ".";
    }
#endif

    return dir + "/launcher.cfg";
}

LauncherSettings LoadSettings()
{
    LauncherSettings settings;
    settings.gameType = 0;

    std::ifstream file(GetSettingsPath());
    if (!file.is_open()) {
        return settings;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) {
            continue;
        }

        std::string key   = line.substr(0, eq);
        std::string value = line.substr(eq + 1);

        if (key == "ip") {
            settings.ip = value;
        } else if (key == "password") {
            settings.password = value;
        } else if (key == "rcon") {
            settings.rconPassword = value;
        } else if (key == "game") {
            int g = atoi(value.c_str());
            if (g >= 0 && g <= 2) {
                settings.gameType = g;
            }
        } else {
            // Bookmark keys: bookmark_name_0, bookmark_ip_0, etc.
            for (int i = 0; i < MAX_BOOKMARKS; i++) {
                char suffix[4];
                snprintf(suffix, sizeof(suffix), "_%d", i);
                std::string s(suffix);

                if (key == "bookmark_name" + s) {
                    settings.bookmarks[i].name = value;
                } else if (key == "bookmark_ip" + s) {
                    settings.bookmarks[i].ip = value;
                } else if (key == "bookmark_pass" + s) {
                    settings.bookmarks[i].password = value;
                } else if (key == "bookmark_rcon" + s) {
                    settings.bookmarks[i].rconPassword = value;
                }
            }
        }
    }

    return settings;
}

void SaveSettings(const LauncherSettings& settings)
{
    std::ofstream file(GetSettingsPath());
    if (!file.is_open()) {
        return;
    }

    file << "ip=" << settings.ip << "\n";
    file << "password=" << settings.password << "\n";
    file << "rcon=" << settings.rconPassword << "\n";
    file << "game=" << settings.gameType << "\n";

    for (int i = 0; i < MAX_BOOKMARKS; i++) {
        if (!settings.bookmarks[i].name.empty()) {
            file << "bookmark_name_" << i << "=" << settings.bookmarks[i].name << "\n";
            file << "bookmark_ip_" << i << "=" << settings.bookmarks[i].ip << "\n";
            file << "bookmark_pass_" << i << "=" << settings.bookmarks[i].password << "\n";
            file << "bookmark_rcon_" << i << "=" << settings.bookmarks[i].rconPassword << "\n";
        }
    }
}
