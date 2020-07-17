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

namespace bpf
{
    namespace math
    {
        /**
         * Representation of a bounding box defined by an origin and a vector from the origin to one extremity
         * @tparam T the number type
         */
        template <typename T>
        class BP_TPL_API BoundingBox
        {
        public:
            /**
             * Origin position
             */
            Vector3<T> Origin;

            /**
             * Extremity vector
             */
            Vector3<T> Extent;

            /**
             * Constructs a BoundingBox
             * @param origin the origin 
             * @param ext a vector to one extremity
             */
            inline BoundingBox(const Vector3<T> &origin, const Vector3<T> &ext) noexcept
                : Origin(origin)
                , Extent(ext)
            {
            }

            /**
             * Constructs a BoundingBox centered at the origin
             */
            inline BoundingBox() noexcept
                : Origin(Vector3<T>::Zero)
                , Extent(Vector3<T>::Identity)
            {
            }

            /**
             * Calculates a minimum point
             * @return Origin - Extent
             */
            inline Vector3<T> GetMin() const noexcept
            {
                return (Origin - Extent);
            }

            /**
             * Calculates a maximum point
             * @return Origin + Extent
             */
            inline Vector3<T> GetMax() const noexcept
            {
                return (Origin + Extent);
            }

            /**
             * Computes a BoundingBox from a min and a max point
             * @param min the minimum point
             * @param max the maximum point
             * @return new BoundingBox
             */
            inline static BoundingBox FromMinMax(const Vector3<T> &min, const Vector3<T> &max) noexcept
            {
                Vector3<T> origin = (min + max) / 2;
                Vector3<T> ext = max - origin;
                return (BoundingBox(origin, ext));
            }
        };
    }
}