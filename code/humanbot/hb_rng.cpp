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
// hb_rng.cpp: xoshiro256** and distributions.

#include "hb_rng.h"

#include <cmath>

namespace hb
{

static inline uint64_t Rotl(uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

uint64_t SplitMix64(uint64_t& state)
{
    uint64_t z = (state += 0x9e3779b97f4a7c15ULL);
    z          = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z          = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

uint64_t HashString(const char *s)
{
    uint64_t h = 0xcbf29ce484222325ULL;
    for (; s && *s; ++s) {
        h ^= static_cast<unsigned char>(*s);
        h *= 0x100000001b3ULL;
    }
    return h;
}

Rng::Rng()
{
    Seed(0);
}

Rng::Rng(uint64_t seed)
{
    Seed(seed);
}

void Rng::Seed(uint64_t seed)
{
    uint64_t st = seed;
    m_seed      = seed;
    m_hasSpare  = false;
    for (int i = 0; i < 4; i++) {
        m_s[i] = SplitMix64(st);
    }
}

uint64_t Rng::Next()
{
    const uint64_t result = Rotl(m_s[1] * 5, 7) * 9;
    const uint64_t t      = m_s[1] << 17;

    m_s[2] ^= m_s[0];
    m_s[3] ^= m_s[1];
    m_s[1] ^= m_s[2];
    m_s[0] ^= m_s[3];
    m_s[2] ^= t;
    m_s[3] = Rotl(m_s[3], 45);

    return result;
}

Rng Rng::Derive(uint64_t stream) const
{
    uint64_t st = m_seed ^ (stream * 0xd1342543de82ef95ULL);
    return Rng(SplitMix64(st));
}

double Rng::Uniform()
{
    return static_cast<double>(Next() >> 11) * (1.0 / 9007199254740992.0);
}

double Rng::Uniform(double lo, double hi)
{
    return lo + (hi - lo) * Uniform();
}

int Rng::UniformInt(int n)
{
    if (n <= 1) {
        return 0;
    }
    return static_cast<int>(Uniform() * n);
}

bool Rng::Bernoulli(double p)
{
    return Uniform() < p;
}

double Rng::Normal()
{
    if (m_hasSpare) {
        m_hasSpare = false;
        return m_spare;
    }
    double u1 = Uniform();
    double u2 = Uniform();
    if (u1 < 1e-300) {
        u1 = 1e-300;
    }
    const double r = std::sqrt(-2.0 * std::log(u1));
    const double a = 6.283185307179586 * u2;
    m_spare        = r * std::sin(a);
    m_hasSpare     = true;
    return r * std::cos(a);
}

double Rng::Normal(double mean, double sd)
{
    return mean + sd * Normal();
}

double Rng::LogNormal(double median, double sigma)
{
    return median * std::exp(sigma * Normal());
}

double Rng::Gamma(double shape)
{
    if (shape <= 0.0) {
        return 0.0;
    }
    if (shape < 1.0) {
        // boost: Gamma(a) = Gamma(a + 1) * U^(1/a)
        const double g = Gamma(shape + 1.0);
        double       u = Uniform();
        if (u < 1e-300) {
            u = 1e-300;
        }
        return g * std::pow(u, 1.0 / shape);
    }
    const double d = shape - 1.0 / 3.0;
    const double c = 1.0 / std::sqrt(9.0 * d);
    for (int guard = 0; guard < 1000; guard++) {
        double x, v;
        do {
            x = Normal();
            v = 1.0 + c * x;
        } while (v <= 0.0);
        v            = v * v * v;
        const double u = Uniform();
        if (u < 1.0 - 0.0331 * x * x * x * x) {
            return d * v;
        }
        if (u > 0.0 && std::log(u) < 0.5 * x * x + d * (1.0 - v + std::log(v))) {
            return d * v;
        }
    }
    return shape;
}

double Rng::Beta(double a, double b)
{
    const double x = Gamma(a);
    const double y = Gamma(b);
    if (x + y <= 0.0) {
        return 0.5;
    }
    return x / (x + y);
}

double Rng::StudentT(double nu)
{
    const double z = Normal();
    if (nu <= 0.0 || nu > 1e6) {
        return z;
    }
    const double chi2 = 2.0 * Gamma(0.5 * nu);
    if (chi2 <= 0.0) {
        return z;
    }
    return z / std::sqrt(chi2 / nu);
}

double Rng::StudentTUnitVar(double nu)
{
    const double t = StudentT(nu);
    if (nu <= 2.05) {
        // variance undefined; use the scale that matches the median absolute value of a unit normal
        return t * 0.6745 / 0.8165;
    }
    return t * std::sqrt((nu - 2.0) / nu);
}

int Rng::Categorical(const double *weights, int n)
{
    double total = 0.0;
    for (int i = 0; i < n; i++) {
        if (weights[i] > 0.0) {
            total += weights[i];
        }
    }
    if (total <= 0.0) {
        return UniformInt(n);
    }
    double r = Uniform() * total;
    for (int i = 0; i < n; i++) {
        if (weights[i] > 0.0) {
            r -= weights[i];
            if (r < 0.0) {
                return i;
            }
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        if (weights[i] > 0.0) {
            return i;
        }
    }
    return 0;
}

int Rng::Categorical(const std::vector<double>& weights)
{
    return Categorical(weights.data(), static_cast<int>(weights.size()));
}

int Rng::CategoricalCdf(const std::vector<double>& cdf)
{
    const int n = static_cast<int>(cdf.size());
    if (n == 0) {
        return 0;
    }
    if (cdf.back() <= 0.0) {
        return UniformInt(n);
    }
    const double r  = Uniform() * cdf.back();
    int          lo = 0, hi = n - 1;
    // the first index whose cumulative weight exceeds r (zero weights are never picked)
    while (lo < hi) {
        const int mid = (lo + hi) / 2;
        if (cdf[mid] > r) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    while (lo > 0 && cdf[lo] <= cdf[lo - 1]) {
        lo--;
    }
    return lo;
}

double Rng::FromQuantiles(const std::vector<double>& probs, const std::vector<double>& values)
{
    const size_t n = probs.size();
    if (n == 0 || values.size() != n) {
        return 0.0;
    }
    const double u = Uniform();
    if (u <= probs[0]) {
        return values[0];
    }
    for (size_t i = 1; i < n; i++) {
        if (u <= probs[i]) {
            const double t = (u - probs[i - 1]) / (probs[i] - probs[i - 1]);
            return values[i - 1] + t * (values[i] - values[i - 1]);
        }
    }
    return values[n - 1];
}

} // namespace hb
