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

namespace bpf
{
    namespace math
    {
        template <typename T>
        class BP_TPL_API Vector<T, 4>
        {
        public:
            T X;
            T Y;
            T Z;
            T W;

            inline Vector(const T x, const T y, const T z, const T w)
                : X(x)
                , Y(y)
                , Z(z)
                , W(w)
            {
            }

            inline Vector(const Vector<T, 3> &other, const T val)
                : X(other.X)
                , Y(other.Y)
                , Z(other.Z)
                , W(val)
            {
            }

            inline Vector()
                : X(T())
                , Y(T())
                , Z(T())
                , W(T())
            {
            }

            explicit inline Vector(const T val)
                : X(val)
                , Y(val)
                , Z(val)
                , W(val)
            {
            }

            Vector(const std::initializer_list<T> &lst);

            inline Vector(const Vector &other)
                : X(other.X)
                , Y(other.Y)
                , Z(other.Z)
                , W(other.W)
            {
            }

            inline Vector(Vector &&other)
                : X(std::move(other.X))
                , Y(std::move(other.Y))
                , Z(std::move(other.Z))
                , W(std::move(other.W))
            {
            }

            constexpr inline fsize Dim() const noexcept
            {
                return (4);
            }

            T &operator()(const fsize l);

            const T &operator()(const fsize l) const;

            inline const T *operator*() const noexcept
            {
                return (&X);
            }

            inline T *operator*() noexcept
            {
                return (&X);
            }

            inline Vector &operator=(const Vector &other)
            {
                X = other.X;
                Y = other.Y;
                Z = other.Z;
                W = other.W;
                return (*this);
            }

            inline Vector &operator=(Vector &&other)
            {
                X = std::move(other.X);
                Y = std::move(other.Y);
                Z = std::move(other.Z);
                W = std::move(other.W);
                return (*this);
            }

            inline Vector operator+(const Vector &other) const
            {
                return (Vector(X + other.X, Y + other.Y, Z + other.Z, W + other.W));
            }

            inline Vector operator-(const Vector &other) const
            {
                return (Vector(X - other.X, Y - other.Y, Z - other.Z, W - other.W));
            }

            inline Vector operator*(const Vector &other) const
            {
                return (Vector(X * other.X, Y * other.Y, Z * other.Z, W * other.W));
            }

            inline Vector operator/(const Vector &other) const
            {
                return (Vector(X / other.X, Y / other.Y, Z / other.Z, W / other.W));
            }

            inline Vector operator*(const T other) const
            {
                return (Vector(X * other, Y * other, Z * other, W * other));
            }

            inline Vector operator/(const T other) const
            {
                return (Vector(X / other, Y / other, Z / other, W / other));
            }

            inline void operator+=(const Vector &other)
            {
                X += other.X;
                Y += other.Y;
                Z += other.Z;
                W += other.W;
            }

            inline void operator-=(const Vector &other)
            {
                X -= other.X;
                Y -= other.Y;
                Z -= other.Z;
                W -= other.W;
            }

            inline void operator*=(const Vector &other)
            {
                X *= other.X;
                Y *= other.Y;
                Z *= other.Z;
                W *= other.W;
            }

            inline void operator/=(const Vector &other)
            {
                X /= other.X;
                Y /= other.Y;
                Z /= other.Z;
                W /= other.W;
            }

            inline void operator*=(const T other)
            {
                X *= other;
                Y *= other;
                Z *= other;
                W *= other;
            }

            inline void operator/=(const T other)
            {
                X /= other;
                Y /= other;
                Z /= other;
                W /= other;
            }

            inline Vector operator-() const
            {
                return (Vector(-X, -Y, -Z, -W));
            }

            inline T Dot(const Vector &other) const
            {
                return (X * other.X + Y * other.Y + Z * other.Z + W * other.W);
            }

            inline T Distance(const Vector &other) const
            {
                auto dist = other - *this;
                return (dist.Norm());
            }

            inline T DistanceSquared(const Vector &other) const
            {
                auto dist = other - *this;
                return (dist.NormSquared());
            }

            inline T Norm() const
            {
                return (Math::Sqrt(X * X + Y * Y + Z * Z + W * W));
            }

            inline T NormSquared() const
            {
                return (X * X + Y * Y + Z * Z + W * W);
            }

            inline Vector Normalize() const
            {
                auto v = Norm();
                return (Vector(X / v, Y / v, Z / v, W / v));
            }

            inline bool operator==(const Vector &other) const
            {
                T diffx = Math::Abs(X - other.X);
                T diffy = Math::Abs(Y - other.Y);
                T diffz = Math::Abs(Z - other.Z);
                T diffw = Math::Abs(W - other.W);

                return (diffx <= Math::Epsilon<T>() && diffy <= Math::Epsilon<T>() && diffz <= Math::Epsilon<T>() && diffw <= Math::Epsilon<T>());
            }

            inline bool operator!=(const Vector &other) const
            {
                return (!operator==(other));
            }

            inline bool operator<(const Vector &other) const
            {
                return (X < other.X && Y < other.Y && Z < other.Z && W < other.W);
            }

            inline bool operator>(const Vector &other) const
            {
                return (X > other.X &&Y > other.Y &&Z > other.Z && W > other.W);
            }

            inline bool operator<=(const Vector &other) const
            {
                return (X <= other.X && Y <= other.Y && Z <= other.Z && W <= other.W);
            }

            inline bool operator>=(const Vector &other) const
            {
                return (X >= other.X && Y >= other.Y && Z >= other.Z && W >= other.W);
            }

            inline static Vector Lerp(const Vector &v, const Vector &v1, const T t)
            {
                return (Vector(Math::Lerp(v.X, v1.X, t), Math::Lerp(v.Y, v1.Y, t), Math::Lerp(v.Z, v1.Z, t), Math::Lerp(v.W, v1.W, t)));
            }

            static const Vector Zero;
            static const Vector Identity;
        };

        template <typename T>
        const Vector<T, 4> Vector<T, 4>::Zero = Vector((T)0);
        template <typename T>
        const Vector<T, 4> Vector<T, 4>::Identity = Vector((T)1);
    }
}

#include "Framework/Math/Vector4.impl.hpp"
