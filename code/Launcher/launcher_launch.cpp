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

// launcher_launch.cpp: Spawn the game process with appropriate arguments

#include "launcher.h"
#include <string>
#include <vector>

#ifdef _WIN32
#    include <Windows.h>
#else
#    include <unistd.h>
#    include <limits.h>
#    include <stdlib.h>

extern "C" char **environ;
#endif

static std::string GetProgramDir()
{
#ifdef _WIN32
    wchar_t path[MAX_PATH] = {0};
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    std::wstring wpath(path);
    size_t       pos = wpath.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        wpath = wpath.substr(0, pos);
    }
    return std::string(wpath.begin(), wpath.end());
#else
    char    path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count > 0) {
        std::string dir(path, count);
        size_t      pos = dir.find_last_of('/');
        if (pos != std::string::npos) {
            dir = dir.substr(0, pos);
        }
        return dir;
    }
    return ".";
#endif
}

void LaunchGame(const LauncherSettings& settings)
{
    std::string programDir  = GetProgramDir();
#ifdef _WIN32
    std::string programName = "openmohaa.exe";
#else
    std::string programName = "openmohaa";
#endif
    std::string programPath = programDir + "/" + programName;

    std::vector<std::string> args;
    args.push_back("+set");
    args.push_back("fs_homepath");
    args.push_back(".");
    args.push_back("+set");
    args.push_back("com_target_game");
    args.push_back(std::to_string(settings.gameType));

    if (!settings.ip.empty()) {
        args.push_back("+connect");
        args.push_back(settings.ip);
    }

    if (!settings.password.empty()) {
        args.push_back("+set");
        args.push_back("password");
        args.push_back(settings.password);
    }

    if (!settings.rconPassword.empty()) {
        args.push_back("+set");
        args.push_back("rconpassword");
        args.push_back(settings.rconPassword);
    }

    if (!settings.nickname.empty()) {
        args.push_back("+set");
        args.push_back("name");
        args.push_back(settings.nickname);
    }

#ifdef _WIN32
    std::wstring cmdLine = L"\"" + std::wstring(programPath.begin(), programPath.end()) + L"\"";
    for (size_t i = 0; i < args.size(); i++) {
        cmdLine += L" \"";
        cmdLine += std::wstring(args[i].begin(), args[i].end());
        cmdLine += L"\"";
    }

    PROCESS_INFORMATION processInfo;
    STARTUPINFOW        startupInfo;
    memset(&processInfo, 0, sizeof(processInfo));
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    std::wstring wProgramPath(programPath.begin(), programPath.end());
    std::wstring wProgramDir(programDir.begin(), programDir.end());

    CreateProcessW(
        wProgramPath.c_str(),
        (LPWSTR)cmdLine.c_str(),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        wProgramDir.c_str(),
        &startupInfo,
        &processInfo
    );

    if (processInfo.hProcess) {
        CloseHandle(processInfo.hProcess);
    }
    if (processInfo.hThread) {
        CloseHandle(processInfo.hThread);
    }
#else
    size_t argc = args.size();
    char **argv = new char *[argc + 2];
    argv[0]     = (char *)programPath.c_str();
    for (size_t i = 0; i < argc; i++) {
        argv[i + 1] = (char *)args[i].c_str();
    }
    argv[argc + 1] = NULL;

    execve(programPath.c_str(), argv, environ);

    // execve only returns on error
    delete[] argv;
#endif
}
