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
// hb_math.h: small vector and angle helpers for the bot brain.
//
// Angles follow the engine: degrees, yaw grows counter-clockwise (to the
// left), positive pitch looks down.

#pragma once

#include <cmath>

namespace hb
{

constexpr float PI_F       = 3.14159265358979323846f;
constexpr float DEG2RAD    = PI_F / 180.0f;
constexpr float RAD2DEG    = 180.0f / PI_F;
constexpr int   TICK_MS    = 50;
constexpr float TICK_S     = 0.05f;

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3() = default;
    Vec3(float ax, float ay, float az)
        : x(ax)
        , y(ay)
        , z(az)
    {}

    Vec3  operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3  operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3  operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    Vec3  operator-() const { return Vec3(-x, -y, -z); }
    Vec3& operator+=(const Vec3& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& o)
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }

    float dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    float lengthXY() const { return std::sqrt(x * x + y * y); }
    float lengthSq() const { return x * x + y * y + z * z; }
};

inline float Clamp(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

inline int ClampI(int v, int lo, int hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// Wrap an angle to [-180, 180).
inline float Wrap180(float a)
{
    a = std::fmod(a + 180.0f, 360.0f);
    if (a < 0.0f) {
        a += 360.0f;
    }
    return a - 180.0f;
}

// Yaw of a horizontal direction, degrees.
inline float YawOf(const Vec3& d)
{
    if (d.x == 0.0f && d.y == 0.0f) {
        return 0.0f;
    }
    return std::atan2(d.y, d.x) * RAD2DEG;
}

// Pitch of a direction (positive = down, like the engine).
inline float PitchOf(const Vec3& d)
{
    const float h = d.lengthXY();
    if (h == 0.0f && d.z == 0.0f) {
        return 0.0f;
    }
    return -std::atan2(d.z, h) * RAD2DEG;
}

inline Vec3 YawDir(float yaw)
{
    return Vec3(std::cos(yaw * DEG2RAD), std::sin(yaw * DEG2RAD), 0.0f);
}

inline Vec3 AnglesForward(float pitch, float yaw)
{
    const float cp = std::cos(pitch * DEG2RAD);
    return Vec3(cp * std::cos(yaw * DEG2RAD), cp * std::sin(yaw * DEG2RAD), -std::sin(pitch * DEG2RAD));
}

inline float Sigmoid(float z)
{
    if (z >= 0.0f) {
        const float e = std::exp(-z);
        return 1.0f / (1.0f + e);
    }
    const float e = std::exp(z);
    return e / (1.0f + e);
}

inline float Logit(float p)
{
    p = Clamp(p, 1e-6f, 1.0f - 1e-6f);
    return std::log(p / (1.0f - p));
}

// Index of the bin whose lower edge is <= x (edges ascending, clamped).
template<typename T, typename E>
inline int BinIndex(T x, const E& edges)
{
    const int n = static_cast<int>(edges.size());
    int       i = 0;
    while (i + 1 < n && static_cast<double>(x) >= static_cast<double>(edges[i + 1])) {
        ++i;
    }
    return i;
}

// Linear interpolation in a monotone table (x ascending), clamped at the ends.
template<typename V>
inline float Interp(float x, const V& xs, const V& ys)
{
    const int n = static_cast<int>(xs.size());
    if (n == 0) {
        return 0.0f;
    }
    if (x <= xs[0]) {
        return ys[0];
    }
    if (x >= xs[n - 1]) {
        return ys[n - 1];
    }
    int i = 0;
    while (i + 1 < n && x > xs[i + 1]) {
        ++i;
    }
    const float t = (x - xs[i]) / (xs[i + 1] - xs[i]);
    return ys[i] + t * (ys[i + 1] - ys[i]);
}

} // namespace hb
