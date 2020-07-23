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

#include "Framework/Math/Color.hpp"
#include "Framework/Math/Math.hpp"

using namespace bpf::math;
using namespace bpf;

const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::Cyan = Color(0, 255, 255);

Color::Color(fint rgb)
{
    B = rgb & 0xFF;
    rgb = rgb >> 8;
    G = rgb & 0xFF;
    rgb = rgb >> 8;
    R = rgb & 0xFF;
    A = 255;
}

Color Color::operator+(const Color &other) const
{
    auto newr = (uint8)Math<uint32>::Clamp(R + other.R, 0u, 255u);
    auto newg = (uint8)Math<uint32>::Clamp(G + other.G, 0u, 255u);
    auto newb = (uint8)Math<uint32>::Clamp(B + other.B, 0u, 255u);
    auto newa = (uint8)Math<uint32>::Clamp(A + other.A, 0u, 255u);

    return (Color(newr, newg, newb, newa));
}

Color Color::operator*(const Color &other) const
{
    float r = (float)R / 255.0f;
    float g = (float)G / 255.0f;
    float b = (float)B / 255.0f;
    float a = (float)A / 255.0f;
    float r1 = (float)other.R / 255.0f;
    float g1 = (float)other.G / 255.0f;
    float b1 = (float)other.B / 255.0f;
    float a1 = (float)other.A / 255.0f;
    float newr = r * r1;
    float newg = g * g1;
    float newb = b * b1;
    float newa = a * a1;

    return (Color((uint8)(newr * 255), (uint8)(newg * 255), (uint8)(newb * 255), (uint8)(newa * 255)));
}

fint Color::Pack() const noexcept
{
    fint res = 0;

    res += R << 16;
    res += G << 8;
    res += B;
    return (res);
}
