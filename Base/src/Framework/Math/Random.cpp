// Copyright (c) 2020, BlockProject 3D
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
//     * Neither the name of BlockProject 3D nor the names of its contributors
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

#include <cstdlib>
#include <ctime>
#include "Framework/Math/Random.hpp"
#include "Framework/Math/Math.hpp"

using namespace bpf::math;
using namespace bpf;

fint Random::IntBounds(const fint min, const fint max)
{
    fint rd = rand() % max;

    while (rd < min || rd > max)
        rd = rand() % max;
    return (rd);
}

Random::Random(const long seed)
{
    srand((uint32)seed);
}

Random::Random()
{
    srand((unsigned int)time(Null));
}

fint Random::NextInt(const fint max)
{
    return (rand() % max);
}

fint Random::NextInt(const fint min, const fint max)
{
    return (IntBounds(min, max));
}

uint8 Random::NextByte(const uint8 max)
{
    return ((uint8)IntBounds(0, max));
}

uint8 Random::NextByte(const uint8 min, const uint8 max)
{
    return ((uint8)IntBounds(min, max));
}

uint16 Random::NextShort(const uint16 max)
{
    return ((uint16)IntBounds(0, max));
}

uint16 Random::NextShort(const uint16 min, const uint16 max)
{
    return ((uint16)IntBounds(min, max));
}

float Random::NextFloat(const float min)
{
    fint start = Math<fint>::Abs(rand() % 256);

    return (((float)start / 256.0f) + min);
}

double Random::NextDouble()
{
    fint start = Math<fint>::Abs(rand() % 256);

    return ((double)start / 256.0f);
}
