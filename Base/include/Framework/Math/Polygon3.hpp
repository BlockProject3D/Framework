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
#include "Framework/Collection/ArrayList.hpp"
#include "Framework/Math/Transform3.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T>
        class BP_TPL_API Polygon3
        {
        public:
            ArrayList<Vector3> Vertices;

            explicit inline Polygon3(const ArrayList<Vector3> &verts)
                : Vertices(verts)
            {
            }

            explicit inline Polygon3(ArrayList<Vector3> &&verts)
                : Vertices(std::move(verts))
            {
            }

            inline void Transform(const Transform3 &transform)
            {
                Transform(transform.ToMatrix());
            }

            Vector3 GetNormal() const noexcept;
            Vector3 GetBarycenter() const noexcept;
            void Transform(const Matrix4 &matrix);
        };

        using Polygon3f = Polygon3<float>;
    }
}

#include "Framework/Math/Polygon3.impl.hpp"