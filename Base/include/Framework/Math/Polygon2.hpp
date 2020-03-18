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
#include "Framework/Math/Vector.hpp"
#include "Framework/Collection/ArrayList.hpp"
#include "Framework/Math/Transform2.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T>
        class BP_TPL_API Polygon2
        {
        public:
            collection::ArrayList<Vector2<T>> Vertices;

            explicit inline Polygon2(const collection::ArrayList<Vector2<T>> &verts)
                : Vertices(verts)
            {
            }

            explicit inline Polygon2(collection::ArrayList<Vector2<T>> &&verts)
                : Vertices(std::move(verts))
            {
            }

            inline void Transform(const Transform2<T> &transform)
            {
                Transform(transform.ToMatrix());
            }

            Vector2<T> GetBarycenter() const noexcept;
            void Transform(const Matrix3<T> &matrix);
            collection::ArrayList<Polygon2> Triangulate() const noexcept;
        };

        using Polygon2f = Polygon2<float>;
    }
}

#include "Framework/Math/Polygon2.impl.hpp"