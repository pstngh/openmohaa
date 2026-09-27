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
// hb_eye_math.h: the first-person eye a human client would report.
//
// Port of the eye placement in CG_OffsetFirstPersonView (code/cgame/cg_view.c):
// view-height smoothing, the pitch pivot, the lean roll around a point 28.7
// units below the eye and the view bob. The engine glue then applies the same
// two clip traces as the client and rounds like CL_EyeInfo, so bots peek and
// shoot from the eye position a human would have.

#pragma once

#include "hb_math.h"

#include <cmath>

namespace hb
{

struct EyeState {
    float currentViewHeight = 0.0f;  // absolute z of the smoothed eye
    float bobPhase          = 0.0f;
    float bobAmp            = 0.0f;
};

struct EyeInput {
    Vec3  origin;
    Vec3  velocity;
    float viewheight = 82.0f;
    float pitch      = 0.0f;
    float yaw        = 0.0f;
    float leanAngle  = 0.0f;   // playerState fLeanAngle, degrees
    bool  walking    = true;   // on the ground
    bool  climbWall  = false;  // PM_CLIMBWALL (ladders)
    bool  frozen     = false;  // PMF_FROZEN / PMF_NO_MOVE
    float frametimeMs = 12.5f;
};

// Rotate v around the unit axis k by `degrees` (right-hand rule, like
// RotatePointAroundVector).
inline Vec3 RotateAround(const Vec3& v, const Vec3& k, float degrees)
{
    const float r = degrees * DEG2RAD;
    const float c = std::cos(r);
    const float s = std::sin(r);
    const Vec3  kxv(k.y * v.z - k.z * v.y, k.z * v.x - k.x * v.z, k.x * v.y - k.y * v.x);
    const float kd = k.dot(v);
    return v * c + kxv * s + k * (kd * (1.0f - c));
}

// Forward and left unit vectors for (pitch, yaw), roll 0 (AngleVectorsLeft).
inline void ForwardLeft(float pitch, float yaw, Vec3& forward, Vec3& left)
{
    const float sy = std::sin(yaw * DEG2RAD);
    const float cy = std::cos(yaw * DEG2RAD);
    const float sp = std::sin(pitch * DEG2RAD);
    const float cp = std::cos(pitch * DEG2RAD);
    forward        = Vec3(cp * cy, cp * sy, -sp);
    left           = Vec3(-sy, cy, 0.0f);
}

// Desired eye position before the clip traces; updates the smoothing and bob state.
inline Vec3 ComputeEye(EyeState& st, const EyeInput& in)
{
    const float ft     = in.frametimeMs / 1000.0f;
    const float target = in.origin.z + in.viewheight;
    float       delta  = target - st.currentViewHeight;

    if (std::fabs(delta) < 0.1f || st.currentViewHeight == 0.0f) {
        st.currentViewHeight = target;
    } else {
        if (delta > 32.0f) {
            delta                = 32.0f;
            st.currentViewHeight = target - 32.0f;
        } else if (delta < -32.0f) {
            delta                = -32.0f;
            st.currentViewHeight = target + 32.0f;
        }
        float change = ft * delta * 12.5f;
        if (!in.walking) {
            change += change;
        }
        if (std::fabs(delta) < std::fabs(change)) {
            change = delta;
        }
        st.currentViewHeight += change;
    }

    Vec3 eye(in.origin.x, in.origin.y, st.currentViewHeight);
    Vec3 forward, left;
    ForwardLeft(in.pitch, in.yaw, forward, left);

    // pitch pivot: the head tilts around a point below the eye
    Vec3        start = eye;
    const float h     = st.currentViewHeight - in.origin.z;
    if (!in.climbWall) {
        start.z -= h * (in.pitch > 0.0f ? 0.4f : 0.2f);
    } else {
        start.z -= h * 0.15f;
    }
    eye = start + RotateAround(eye - start, left, in.pitch * 0.4f);

    // lean: roll around a point 28.7 units below the eye
    if (in.leanAngle != 0.0f) {
        const Vec3 pivot = eye - Vec3(0.0f, 0.0f, 28.7f);
        eye              = pivot + RotateAround(eye - pivot, forward, in.leanAngle);
    }

    // view bob
    const Vec3 vel = in.frozen ? Vec3() : in.velocity;
    if (in.walking) {
        const float speed = vel.length();
        const float phase = speed * 0.0015f + 0.9f;
        st.bobPhase += (ft + ft) * PI_F * phase;
        if (st.bobAmp != 0.0f) {
            st.bobAmp = speed;
        } else {
            st.bobAmp = speed * 0.5f;
        }
        if (in.leanAngle != 0.0f) {
            st.bobAmp *= 0.75f;
        }
        st.bobAmp *= (1.0f - std::fabs(in.pitch) * (1.0f / 90.0f) * 0.5f) * 0.5f;
    } else if (st.bobAmp > 0.0f) {
        st.bobAmp -= ft * st.bobAmp + ft * st.bobAmp;
        if (st.bobAmp < 0.1f) {
            st.bobAmp = 0.0f;
        }
    }
    if (st.bobAmp > 0.0f) {
        const float lat = Clamp(std::sin(st.bobPhase) * st.bobAmp * 0.03f, -16.0f, 16.0f);
        eye += left * lat;
        const float vert = Clamp((std::fabs(std::sin(st.bobPhase - 0.94f)) - 0.5f) * st.bobAmp * 0.06f, -16.0f, 16.0f);
        eye.z += vert;
    }
    return eye;
}

// CL_EyeInfo rounding of the offset from the origin to a signed char.
inline signed char EyeOffsetByte(float ofs)
{
    ofs += 0.5f;
    if (ofs < -127.0f) {
        ofs = -127.0f;
    } else if (ofs > 127.0f) {
        ofs = 127.0f;
    }
    return static_cast<signed char>(static_cast<int>(ofs));
}

} // namespace hb
