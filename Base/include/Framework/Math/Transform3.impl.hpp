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

namespace bpf
{
    namespace math
    {
        template <typename T>
        typename Transform3<T>::MatrixBuilder &Transform3<T>::MatrixBuilder::Translate(const Vector3<T> &translation) noexcept
        {
            _matrix = _matrix * Matrix4<T>({
                1, 0, 0, translation.X,
                    0, 1, 0, translation.Y,
                    0, 0, 1, translation.Z,
                    0, 0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform3<T>::MatrixBuilder &Transform3<T>::MatrixBuilder::Scale(const Vector3<T> &scale) noexcept
        {
            _matrix = _matrix * Matrix4<T>({
                scale.X, 0, 0, 0,
                    0, scale.Y, 0, 0,
                    0, 0, scale.Z, 0,
                    0, 0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform3<T>::MatrixBuilder &Transform3<T>::MatrixBuilder::ShearX(const Vector3<T> &shear) noexcept
        {
            _matrix = _matrix * Matrix4<T>({
                1, 0, 0, 0,
                    shear.Y, 1, 0, 0,
                    shear.Z, 0, 1, 0,
                    0, 0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform3<T>::MatrixBuilder &Transform3<T>::MatrixBuilder::ShearY(const Vector3<T> &shear) noexcept
        {
            _matrix = _matrix * Matrix4<T>({
                1, shear.X, 0, 0,
                    0, 1, 0, 0,
                    0, shear.Z, 1, 0,
                    0, 0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform3<T>::MatrixBuilder &Transform3<T>::MatrixBuilder::ShearZ(const Vector3<T> &shear) noexcept
        {
            _matrix = _matrix * Matrix4<T>({
                1, 0, shear.X, 0,
                    0, 1, shear.Z, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform3<T>::MatrixBuilder &Transform3<T>::MatrixBuilder::Rotate(const Vector3<T> &axis, const T angle) noexcept
        {
            T c = Math<T>::Cos(angle);
            T s = Math<T>::Sin(angle);
            T t = (T)1 - c;
            Vector3<T> a = axis.Normalize();

            _matrix = _matrix * Matrix4<T>({
                t * a.X * a.X + c, t * a.X * a.Y - a.Z * s, t * a.X * a.Z + a.Y * s, 0,
                t * a.X * a.Y + a.Z * s, t * a.Y * a.Y + c, t * a.Y * a.Z - a.X * s, 0,
                t * a.X * a.Z - a.Y * s, t * a.Y * a.Z + a.X * s, t * a.Z * a.Z + c, 0,
                0, 0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        Vector3<T> Transform3<T>::LocalToWorld(const Vector3<T> &local)
        {
            auto res = ToMatrix() * Vector4<T>(local, (T)1);

            return (Vector3<T>(res.X, res.Y, res.Z));
        }

        template <typename T>
        Vector3<T> Transform3<T>::WorldToLocal(const Vector3<T> &world)
        {
            auto res = ToMatrix().Invert() * Vector4<T>(world, (T)1);

            return (Vector3<T>(res.X, res.Y, res.Z));
        }

        template <typename T>
        Transform3<T> Transform3<T>::operator+(const Transform3 &other) const noexcept
        {
            Transform3<T> res(Position + other.Position, Scale * other.Scale, Rotation * other.Rotation);

            return (res);
        }

        template <typename T>
        void Transform3<T>::operator+=(const Transform3 &other)
        {
            Position += other.Position;
            Scale *= other.Scale;
            Rotation *= other.Rotation;
        }

        template <typename T>
        void Transform3<T>::RotateArround(const Vector3<T> &pivot, const Quaternion<T> &rotation) noexcept
        {
            auto dir = Position - pivot;

            dir = rotation.Rotate(dir);
            Position = dir + pivot;
        }
    }
}
