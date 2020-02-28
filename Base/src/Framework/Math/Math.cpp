// Copyright (c) 2018, BlockProject
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdlib.h>
#include <time.h>
#include "Framework/Math/Math.hpp"

using namespace bpf::math;
using namespace bpf;

constexpr float Math::RadToDeg;
constexpr float Math::DegToRad;
constexpr float Math::Pi;

bool Math::IsPrime(const fisize n)
{
    fisize a = 2;

    while (a < n)
    {
        if ((n % a) == 0)
            return (false);
        ++a;
    }
    return (true);
}

fisize Math::FindNextPrime(const fisize n)
{
    fisize a = n + 1;

    while (!IsPrime(a))
        ++a;
    return (a);
}

fsize Math::FindNextPowerOfTwo(fsize nb)
{
    --nb;
    nb |= nb >> 1;
    nb |= nb >> 2;
    nb |= nb >> 4;
    nb |= nb >> 8;
    nb |= nb >> 16;
    ++nb;
    return (nb);
}

float Math::InvSqrt(const float nb)
{
    float y = nb;
    float x2 = nb * 0.5f;
    uint32 nbu = *(uint32 *)&y;
    const float threehalfs = 1.5f;

    nbu = 0x5F3759DF - (nbu >> 1);
    y = *(float *)&nbu;
    y = y * (threehalfs - (x2 * y * y));
    return (y);
}
