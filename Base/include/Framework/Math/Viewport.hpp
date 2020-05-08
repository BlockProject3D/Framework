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
#include "Framework/Types.hpp"
#include "Framework/Math/Vector.hpp"
#include "Framework/Math/Matrix.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T>
        class BP_TPL_API Viewport
        {
        public:
            /**
             * Field of view
             */
            T FOV;

            /**
             * Near clipping plane
             */
            T NearPlane;

            /**
             * Far clipping plane
             */
            T FarPlane;

            /**
             * Viewport width
             */
            fsize Width;

            /**
             * Viewport height
             */
            fsize Height;

            /**
             * Projection matrix
             */
            Matrix4<T> Projection;

            Vector2<T> Project(const Matrix4<T> &view, const Vector3<T> &pt);

            inline Viewport()
                : FOV(0.0f)
                , NearPlane(0.0f)
                , FarPlane(0.0f)
                , Width(0)
                , Height(0)
                , Projection(Matrix4<T>::Identity)
            {
            }
        };

        using Viewportf = Viewport<float>;
    }
};

#include "Framework/Math/Viewport.impl.hpp"
