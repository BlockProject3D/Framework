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

#pragma once
#include "Framework/Math/Vector.hpp"
#include "Framework/Collection/ArrayList.hpp"
#include "Framework/Math/Transform3.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Utility to represent a 3D polygon
         * @tparam T the number type
         */
        template <typename T>
        class BP_TPL_API Polygon3
        {
        public:
            /**
             * Polygon vertices
             */
            collection::ArrayList<Vector3<T>> Vertices;

            /**
             * Constructs a polygon from a set of vertices
             * @param verts the vertices to copy from
             */
            explicit inline Polygon3(const collection::ArrayList<Vector3<T>> &verts)
                : Vertices(verts)
            {
            }

            /**
             * Constructs a polygon from a set of vertices
             * @param verts the vertices to move from
             */
            explicit inline Polygon3(collection::ArrayList<Vector3<T>> &&verts)
                : Vertices(std::move(verts))
            {
            }

            /**
             * Apply a 3D transform to each vertex sequentially
             * @param transform the transform to apply
             */
            inline void Transform(const Transform3<T> &transform)
            {
                Transform(transform.ToMatrix());
            }

            /**
             * Computes a normal of this polygon.
             * If this polygon is more than 3 points, the first 3 points are chosen
             * @return 3D vector
             */
            Vector3<T> GetNormal() const noexcept;

            /**
             * Computes the barycenter of this polygon
             * @return 3D vector
             */
            Vector3<T> GetBarycenter() const noexcept;

            /**
             * Apply a matrix to each vertex sequentially
             * @param matrix the matrix to apply
             */
            void Transform(const Matrix4<T> &matrix);
        };

        using Polygon3f = Polygon3<float>;
    }
}

#include "Framework/Math/Polygon3.impl.hpp"