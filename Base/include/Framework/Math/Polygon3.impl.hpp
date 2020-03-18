// Copyright (c) 2020, BlockProject
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

#pragma once

namespace bpf
{
    namespace math
    {
        template <typename T>
        Vector3<T> Polygon3<T>::GetNormal() const noexcept
        {
            if (Vertices.Size() >= 3)
            {
                Vector3<T> v = Vertices[1] - Vertices[0];
                Vector3<T> w = Vertices[2] - Vertices[0];
                return (v.Cross(w));
            }
            return (Vector3<T>::Zero);
        }

        template <typename T>
        Vector3<T> Polygon3<T>::GetBarycenter() const noexcept
        {
            Vector3<T> res;

            for (const auto &v : Vertices)
                res += v;
            return (res / (T)Vertices.Size());
        }

        template <typename T>
        void Polygon3<T>::Transform(const Matrix4<T> &matrix)
        {
            for (auto &v : Vertices)
            {
                Vector4<T> vec(v, 1.0f);
                auto res = matrix * vec;
                v = Vector3<T>(res.X, res.Y, res.Z);
            }
        }
    }
}
