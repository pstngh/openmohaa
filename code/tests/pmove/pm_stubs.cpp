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
// pm_stubs.cpp: the engine services that bg_pmove.cpp, q_shared.c and
// q_math.c call. The game module forwards them to the engine (gi.Printf,
// gi.Error); here they print, and an error aborts the process.

#include "pm_world.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

void QDECL Com_Printf(const char *msg, ...)
{
    va_list argptr;
    char    text[4096];

    va_start(argptr, msg);
    Q_vsnprintf(text, sizeof(text), msg, argptr);
    va_end(argptr);

    fputs(text, stdout);
}

void QDECL Com_Error(int level, const char *error, ...)
{
    va_list argptr;
    char    text[4096];

    va_start(argptr, error);
    Q_vsnprintf(text, sizeof(text), error, argptr);
    va_end(argptr);

    fflush(stdout);
    fprintf(stderr, "Com_Error (level %d): %s\n", level, text);
    fflush(stderr);
    abort();
}
