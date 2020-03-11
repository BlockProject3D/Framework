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
        typename Transform2D<T>::MatrixBuilder &Transform2D<T>::MatrixBuilder::Translate(const Vector2<T> &translation) noexcept
        {
            _matrix = _matrix * Matrix3<T>({
                1, 0, translation.X,
                    0, 1, translation.Y,
                    0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform2D<T>::MatrixBuilder &Transform2D<T>::MatrixBuilder::Scale(const Vector2<T> &scale) noexcept
        {
            _matrix = _matrix * Matrix3<T>({
                scale.X, 0, 0,
                    0, scale.Y, 0,
                    0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform2D<T>::MatrixBuilder &Transform2D<T>::MatrixBuilder::ShearX(const T &shear) noexcept
        {
            _matrix = _matrix * Matrix3<T>({
                1, 0, 0,
                    shear, 1, 0,
                    0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform2D<T>::MatrixBuilder &Transform2D<T>::MatrixBuilder::ShearY(const T &shear) noexcept
        {
            _matrix = _matrix * Matrix3<T>({
                1, shear, 0,
                    0, 1, 0,
                    0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        typename Transform2D<T>::MatrixBuilder &Transform2D<T>::MatrixBuilder::Rotate(const T &rotation) noexcept
        {
            _matrix = _matrix * Matrix3<T>({
                Math::Cos(rotation), -Math::Sin(rotation), 0,
                    Math::Sin(rotation), Math::Cos(rotation), 0,
                    0, 0, 1
            });
            return (*this);
        }

        template <typename T>
        Vector2<T> Transform2D<T>::LocalToWorld(const Vector2<T> &local)
        {
            auto res = ToMatrix() * Vector3<T>(local, (T)1);

            return (Vector2<T>(res.X, res.Y));
        }

        template <typename T>
        Vector2<T> Transform2D<T>::WorldToLocal(const Vector2<T> &world)
        {
            auto res = ToMatrix().Invert() * Vector3<T>(local, (T)1);

            return (Vector2<T>(res.X, res.Y));
        }

        template <typename T>
        Transform2D<T> Transform2D<T>::operator+(const Transform2D &other) const noexcept
        {
            Transform2D<T> res(Position + other.Position, Scale * other.Scale, Rotation + other.Rotation);

            return (res);
        }

        template <typename T>
        void Transform2D<T>::operator+=(const Transform2D &other)
        {
            Position += other.Position;
            Scale *= other.Scale;
            Rotation += other.Rotation;
        }

        template <typename T>
        void Transform2D<T>::RotateArround(const Vector2<T> &pivot, const T &rotation)
        {
            auto dir = Position - pivot;
            Matrix2<T> mat = {
                Math::Cos(rotation), -Math::Sin(rotation),
                Math::Sin(rotation), Math::Cos(rotation)
            };

            dir = mat * dir;
            Position = dir + pivot;
        }
    }
}
