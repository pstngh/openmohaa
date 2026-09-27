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
// hb_rng.h: portable random numbers for the bot brain.
//
// xoshiro256** seeded through splitmix64, plus hand-written distributions, so
// the integer and uniform streams are identical on every platform and no bot
// code touches rand() or the engine's generator. Each module of each bot owns
// its own stream (see Rng::Derive), so one module's draws never shift another's.

#pragma once

#include <cstdint>
#include <vector>

namespace hb
{

enum RngStream : uint64_t {
    STREAM_STYLE      = 1,
    STREAM_PERCEPTION = 2,
    STREAM_BELIEF     = 3,
    STREAM_VIEW       = 4,
    STREAM_TRIGGER    = 5,
    STREAM_MOVEMENT   = 6,
    STREAM_STANCE     = 7,
    STREAM_NAV        = 8,
    STREAM_WEAPON     = 9,
    STREAM_SUBSTEP    = 10,
    STREAM_PRESENT    = 11,
    STREAM_LIFE       = 12,
};

uint64_t SplitMix64(uint64_t& state);

// Stable 64-bit hash of a string (FNV-1a), for seeding from names.
uint64_t HashString(const char *s);

class Rng
{
public:
    Rng();
    explicit Rng(uint64_t seed);

    void     Seed(uint64_t seed);
    uint64_t Next();

    // An independent generator for a sub-stream of this seed.
    Rng Derive(uint64_t stream) const;

    uint64_t SeedValue() const { return m_seed; }

    double Uniform();                      // [0, 1)
    double Uniform(double lo, double hi);  // [lo, hi)
    int    UniformInt(int n);              // [0, n)
    bool   Bernoulli(double p);
    double Normal();                       // standard normal (Box-Muller, 2 uniforms)
    double Normal(double mean, double sd);
    double LogNormal(double median, double sigma);
    double Gamma(double shape);            // scale 1 (Marsaglia-Tsang)
    double Beta(double a, double b);
    double StudentT(double nu);            // unit scale
    double StudentTUnitVar(double nu);     // scaled to unit variance (nu > 2)
    int    Categorical(const double *weights, int n);
    int    Categorical(const std::vector<double>& weights);
    // Inverse-CDF draw from monotone quantile knots (probabilities ascending).
    double FromQuantiles(const std::vector<double>& probs, const std::vector<double>& values);

private:
    uint64_t m_s[4];
    uint64_t m_seed;
};

} // namespace hb
