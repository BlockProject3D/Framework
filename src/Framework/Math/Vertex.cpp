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

#include "Framework/Framework.hpp"
#include "Framework/Math/BMath.hpp"
#include "Framework/Math/Vector.hpp"
#include "Framework/Math/Vertex.hpp"

using namespace Framework;

FVertex::FVertex(float x, float y, float z, float u, float v)
    : X(x), Y(y), Z(z), U(u), V(v)
{
    SetNormal(FVector::Zero);
}

FVertex::FVertex(const FVector &pos, const FVector &normal, float u, float v)
    : X(pos.X), Y(pos.Y), Z(pos.Z), U(u), V(v)
{
    SetNormal(normal);
}

FVertex::FVertex(const FVertex &other)
    : X(other.X), Y(other.Y), Z(other.Z), U(other.U), V(other.V)
{
    SetNormal(FVector::Zero);
}

FVertex::FVertex()
    : X(0), Y(0), Z(0), U(0), V(0)
{
    SetNormal(FVector::Zero);
}

void FVertex::SetNormal(const FVector &vec)
{
    NX = vec.X;
    NY = vec.Y;
    NZ = vec.Z;
}

FVertex FVertex::operator-()
{
    return (FVertex(FVector(-X, -Y, -Z), FVector(NX, NY, NZ), U, V));
}

bool FVertex::operator==(const FVertex &other)
{
    return (X == other.X && Y == other.Y && Z == other.Z && U == other.U && V == other.V);
}

void FVertex::ApplyTransform(const FTransform &tr)
{
    FMatrix mat = tr.GetMatrix();
    FVector4D v(X, Y, Z, 1.0f);
    FVector4D res = v * mat;
    FMatrix rot = tr.GetQuat().ToMatrix();
    FVector4D v1(NX, NY, NZ, 1.0f);
    FVector4D res1 = v1 * rot;

    X = res.X;
    Y = res.Y;
    Z = res.Z;
    NX = res1.X;
    NY = res1.Y;
    NZ = res1.Z;
}
