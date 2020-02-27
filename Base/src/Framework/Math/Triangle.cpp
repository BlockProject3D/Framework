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

#include "Framework/Math/Triangle.hpp"

/*using namespace bpf;

Vector3f Triangle::GetNormal() const
{
    Vector3f v = B - A;
    Vector3f w = C - A;

    return (v.Cross(w));
}

void Triangle::ApplyTransform(const Transform &tr)
{
    A = tr.TransformPoint(A);
    B = tr.TransformPoint(B);
    C = tr.TransformPoint(C);
}

Vector3f Triangle::GetBarycentricCoordinates(const Vector3f &p) const
{
    Vector3f normal = GetNormal();
    Vector3f res;

    float abc = normal.Dot((B - A).Cross(C - A));
    float pbc = normal.Dot((B - p).Cross(C - p));
    float pca = normal.Dot((C - p).Cross(A - p));
    res.X = pbc / abc;
    res.Y = pca / abc;
    res.Z = 1.0f - res.X - res.Y;
    return (res);
}*/
