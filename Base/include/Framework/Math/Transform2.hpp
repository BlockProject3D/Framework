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
#include "Framework/Math/Matrix.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T>
        class BP_TPL_API Transform2
        {
        public:
            class BP_TPL_API MatrixBuilder
            {
            private:
                Matrix3<T> _matrix;

            public:
                inline MatrixBuilder()
                    : _matrix(Matrix3<T>::Identity)
                {
                }
                MatrixBuilder &Translate(const Vector2<T> &translation) noexcept;
                MatrixBuilder &Scale(const Vector2<T> &scale) noexcept;
                MatrixBuilder &ShearX(const T &shear) noexcept;
                MatrixBuilder &ShearY(const T &shear) noexcept;
                MatrixBuilder &Rotate(const T &rotation) noexcept;
                inline Matrix3<T> Build() const noexcept
                {
                    return (_matrix);
                }
            };

        public:
            Vector2<T> Position;
            Vector2<T> Scale;
            T Rotation;

            explicit inline Transform2(const Vector2<T> &pos = Vector2<T>::Zero, const Vector2<T> &scale = Vector2<T>::Identity, const T &rotation = 0)
                : Position(pos)
                , Scale(scale)
                , Rotation(rotation)
            {
            }

            Vector2<T> LocalToWorld(const Vector2<T> &local);
            Vector2<T> WorldToLocal(const Vector2<T> &world);

            inline Matrix3<T> ToMatrix() const noexcept
            {
                return (MatrixBuilder().Translate(Position).Rotate(Rotation).Scale(Scale).Build());
            }

            void RotateArround(const Vector2<T> &pivot, const T &rotation);

            Transform2 operator+(const Transform2 &other) const noexcept;
            void operator+=(const Transform2 &other);
        };

        using Transform2f = Transform2<float>;
    }
}

#include "Framework/Math/Transform2.impl.hpp"