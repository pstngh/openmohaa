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
// hb_test.h: minimal assertions for the human-bot unit tests.

#pragma once

#include <cmath>
#include <cstdio>
#include <string>

namespace hbtest
{

inline int& Failures()
{
    static int n = 0;
    return n;
}

inline void Fail(const char *file, int line, const std::string& what)
{
    std::printf("FAIL %s:%d: %s\n", file, line, what.c_str());
    Failures()++;
}

inline int Finish(const char *name)
{
    if (Failures()) {
        std::printf("%s: %d failure(s)\n", name, Failures());
        return 1;
    }
    std::printf("%s: all checks passed\n", name);
    return 0;
}

} // namespace hbtest

#define HB_CHECK(cond)                                                  \
    do {                                                                \
        if (!(cond)) {                                                  \
            hbtest::Fail(__FILE__, __LINE__, #cond);                    \
        }                                                               \
    } while (0)

#define HB_CHECK_NEAR(a, b, tol)                                                                     \
    do {                                                                                             \
        const double hb_a_ = (a), hb_b_ = (b), hb_t_ = (tol);                                        \
        if (!(std::fabs(hb_a_ - hb_b_) <= hb_t_)) {                                                  \
            char hb_buf_[256];                                                                       \
            std::snprintf(hb_buf_, sizeof(hb_buf_), "%s = %.6g, expected %s = %.6g +- %.3g", #a, hb_a_, #b, \
                          hb_b_, hb_t_);                                                             \
            hbtest::Fail(__FILE__, __LINE__, hb_buf_);                                               \
        }                                                                                            \
    } while (0)

#define HB_REPORT(fmt, ...) std::printf("  " fmt "\n", __VA_ARGS__)
