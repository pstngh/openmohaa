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
// hb_presentation.h: how a bot looks from the outside in the optional
// disguise test mode (g_humanbot_disguise): a human-looking name and a ping
// that behaves like a real connection. Bots never chat.

#pragma once

#include "hb_model.h"
#include "hb_rng.h"

#include <string>
#include <vector>

namespace hb
{

class PingModel
{
public:
    void Init(const PresentationModel *params, const Rng& rng);
    // One server frame; returns the ping to show, ms.
    int Step();

private:
    const PresentationModel *m_p = nullptr;
    Rng                      m_rng;
    double                   m_base  = 45.0;   // this connection's typical ping
    double                   m_drift = 0.0;    // slow log-scale drift
};

// Parses a names file (one name per line, '#' comments) and drops names in use.
std::vector<std::string> ParseNames(const std::string& text);

// Picks a name not in `used`; falls back to "player<n>".
std::string PickName(const std::vector<std::string>& names, const std::vector<std::string>& used, Rng& rng);

} // namespace hb
