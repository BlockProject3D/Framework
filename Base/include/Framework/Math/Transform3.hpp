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
#include "Framework/Math/Quaternion.hpp"
#include "Framework/Math/Vector.hpp"
#include "Framework/Math/Matrix.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T>
        class BP_TPL_API Transform3
        {
        public:
            class BP_TPL_API MatrixBuilder
            {
            private:
                Matrix4<T> _matrix;

            public:
                inline MatrixBuilder()
                    : _matrix(Matrix4<T>::Identity)
                {
                }
                MatrixBuilder &Translate(const Vector3<T> &translation) noexcept;
                MatrixBuilder &Scale(const Vector3<T> &scale) noexcept;
                MatrixBuilder &ShearX(const Vector3<T> &shear) noexcept;
                MatrixBuilder &ShearY(const Vector3<T> &shear) noexcept;
                MatrixBuilder &ShearZ(const Vector3<T> &shear) noexcept;
                MatrixBuilder &Rotate(const Vector3<T> &axis, const T angle) noexcept;
                inline MatrixBuilder &Rotate(const Quaternion<T> &quat) noexcept
                {
                    _matrix = _matrix * quat.ToMatrix();
                    return (*this);
                }
                inline Matrix4<T> Build() const noexcept
                {
                    return (_matrix);
                }
            };

        public:
            Vector3<T> Position;
            Vector3<T> Scale;
            Quaternion<T> Rotation;

            explicit inline Transform3(const Vector3<T> &pos = Vector3<T>::Zero, const Vector3<T> &scale = Vector3<T>::Identity, const Quaternion<T> &rotation = Quaternion<T>::Identity)
                : Position(pos)
                , Scale(scale)
                , Rotation(rotation)
            {
            }

            Vector3<T> LocalToWorld(const Vector3<T> &local);
            Vector3<T> WorldToLocal(const Vector3<T> &world);

            inline Matrix4<T> ToMatrix() const noexcept
            {
                return (MatrixBuilder().Translate(Position).Rotate(Rotation).Scale(Scale).Build());
            }

            Transform3 operator+(const Transform3 &other) const noexcept;
            void operator+=(const Transform3 &other);

            void RotateArround(const Vector3<T> &pivot, const Quaternion<T> &rotation) noexcept;
        };

        using Transform3f = Transform3<float>;
    }
}

#include "Framework/Math/Transform3.impl.hpp"