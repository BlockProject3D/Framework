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
#include "Framework/Math/BMath.hpp"

using namespace Framework;

void FMath::RotationMatrix(FMatrix &out, const ERotationAxis axis, const float angle)
{
    switch (axis)
    {
    case ROT_AXIS_Y:
        {
            out.Set(1, 1, cosf(angle * DegToRad));
            out.Set(1, 2, -sinf(angle * DegToRad));
            out.Set(2, 1, sinf(angle * DegToRad));
            out.Set(2, 2, cosf(angle * DegToRad));
            break;
        }
    case ROT_AXIS_X:
        {
            out.Set(0, 0, cosf(angle * DegToRad));
            out.Set(0, 2, sinf(angle * DegToRad));
            out.Set(2, 0, -sinf(angle * DegToRad));
            out.Set(2, 2, cosf(angle * DegToRad));
            break;
        }
    case ROT_AXIS_Z:
        {
            out.Set(0, 0, cosf(angle * DegToRad));
            out.Set(0, 1, -sinf(angle * DegToRad));
            out.Set(1, 0, sinf(angle * DegToRad));
            out.Set(1, 1, cosf(angle * DegToRad));
            break;
        }
    }
}

FMatrix FMath::PerspectiveProjectionMatrix(const float fovy, const float aspect, const float znear, const float zfar)
{
    FMatrix out = FMatrix::Zero;
    float f = 1 / tanf((fovy / 2.0f) * DegToRad);

    out = FMatrix({
                  1 / (aspect * tanf((fovy / 2.0f) * DegToRad)), 0, 0, 0,
                  0, f, 0, 0,
                  0, 0, -(zfar + znear) / (zfar - znear), -((2 * zfar * znear) / (zfar - znear)),
                  0, 0, -1, 0
                });
    return (out);
}

FMatrix FMath::ScreenProjectionMatrix(const float w, const float h)
{
    FMatrix out = FMatrix::Identity;

    out.Scale(FVector(1.0f / w, -(1.0f / h), 0));
    out.Translate(FVector(-1, 1, 0));
    return (out);
}

bool FMath::IsPrime(const int n)
{
    int a = 2;

    while (a < n)
    {
        if ((n % a) == 0)
            return (false);
        ++a;
    }
    return (true);
}

int FMath::FindNextPrime(const int n)
{
    int a = n + 1;

    while (!IsPrime(a))
        ++a;
    return (a);
}
