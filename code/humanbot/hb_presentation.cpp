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
// hb_presentation.cpp: disguise names and ping.

#include "hb_presentation.h"
#include "hb_math.h"

#include <cmath>
#include <cstdio>

namespace hb
{

void PingModel::Init(const PresentationModel *params, const Rng& rng)
{
    m_p     = params;
    m_rng   = rng;
    m_base  = m_rng.LogNormal(params->pingMedianMs, params->pingSigma);
    m_drift = 0.0;
}

int PingModel::Step()
{
    // AR(1) drift on the log scale plus per-frame jitter
    const double ar = Clamp(m_p->pingDriftAr, 0.0f, 0.9999f);
    m_drift         = ar * m_drift + std::sqrt(1.0 - ar * ar) * 0.15 * m_rng.Normal();
    const double v  = m_base * std::exp(m_drift) + std::fabs(m_rng.Normal(0.0, m_p->pingJitterMs));
    return static_cast<int>(Clamp(static_cast<float>(v), 5.0f, 400.0f));
}

std::vector<std::string> ParseNames(const std::string& text)
{
    std::vector<std::string> out;
    size_t                   i = 0;
    while (i < text.size()) {
        size_t j = text.find('\n', i);
        if (j == std::string::npos) {
            j = text.size();
        }
        std::string line = text.substr(i, j - i);
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ' || line.back() == '\t')) {
            line.pop_back();
        }
        size_t s = 0;
        while (s < line.size() && (line[s] == ' ' || line[s] == '\t')) {
            s++;
        }
        line = line.substr(s);
        if (!line.empty() && line[0] != '#' && line.size() < 32 && line.find('"') == std::string::npos
            && line.find('\\') == std::string::npos && line.find(';') == std::string::npos) {
            out.push_back(line);
        }
        i = j + 1;
    }
    return out;
}

std::string PickName(const std::vector<std::string>& names, const std::vector<std::string>& used, Rng& rng)
{
    std::vector<std::string> free;
    for (const std::string& n : names) {
        bool taken = false;
        for (const std::string& u : used) {
            taken |= u == n;
        }
        if (!taken) {
            free.push_back(n);
        }
    }
    if (!free.empty()) {
        return free[rng.UniformInt(static_cast<int>(free.size()))];
    }
    char buf[32];
    std::snprintf(buf, sizeof(buf), "player%d", 100 + rng.UniformInt(900));
    return buf;
}

} // namespace hb
