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
        Vector2<T> Polygon2<T>::GetBarycenter() const noexcept
        {
            Vector2<T> res;

            for (const auto &v : Vertices)
                res += v;
            return (res / Vertices.Size());
        }

        template <typename T>
        void Polygon2<T>::Transform(const Matrix3<T> &matrix)
        {
            for (auto &v : Vertices)
            {
                Vector3<T> vec(v, 1.0f);
                auto res = matrix * vec;
                v = Vector2<T>(res.X, res.Y);
            }
        }

        template <typename T>
        ArrayList<Polygon2<T>> Polygon2<T>::Triangulate() const noexcept
        {
            if (Vertices.Size <= 3)
                return ({ *this });
            auto lst = ArrayList<Polygon2>();
            auto v1 = Vertices[0];
            for (fsize i = 1; i + 1 < Vertices.Size(); i += 2)
                lst.Add(Polygon2({ v1, Vertices[i], Vertices[i + 1] }));
            return (lst);
        }
    }
}
