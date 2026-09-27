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
// hb_bundle.h: loading the model bundle (embedded JSON, optional overrides).

#pragma once

#include "hb_model.h"

#include <map>
#include <string>

namespace hb
{

class MapPrior;

// Names of the embedded files: "shared.json", "styles.json", "calibration.json",
// "maps/dm_main.json", ...
std::string EmbeddedText(const std::string& name);
std::vector<std::string> EmbeddedNames();
const char *EmbeddedSha256();

// Builds the bundle from the embedded JSON, merge-patching every override text
// (RFC 7386) whose name matches an embedded file. On any parse or validation
// error the embedded bundle is used unchanged and `error` says why.
bool LoadBundle(const std::map<std::string, std::string>& overrides, ModelBundle& out, std::string& error);

// Parses a map prior ("maps/dm_main.json" style text) with optional override.
bool LoadMapPrior(const std::string& text, const std::string& overrideText, MapPrior& out, std::string& error);

// SHA-256 of a byte string, lowercase hex.
std::string Sha256Hex(const std::string& data);

} // namespace hb
