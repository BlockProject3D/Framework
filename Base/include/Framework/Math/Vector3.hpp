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
        template <typename T>
        class BP_TPL_API Vector<T, 3>
        {
        public:
            T X;
            T Y;
            T Z;

            inline Vector(const T x, const T y, const T z)
                : X(x)
                , Y(y)
                , Z(z)
            {
            }

            inline Vector(const Vector<T, 2> &other, const T val)
                : X(other.X)
                , Y(other.Y)
                , Z(val)
            {
            }

            inline Vector()
                : X(T())
                , Y(T())
                , Z(T())
            {
            }

            explicit inline Vector(const T val)
                : X(val)
                , Y(val)
                , Z(val)
            {
            }

            Vector(const std::initializer_list<T> &lst);

            inline Vector(const Vector &other)
                : X(other.X)
                , Y(other.Y)
                , Z(other.Z)
            {
            }

            inline Vector(Vector &&other) noexcept
                : X(std::move(other.X))
                , Y(std::move(other.Y))
                , Z(std::move(other.Z))
            {
            }

            constexpr inline fsize Dim() const noexcept
            {
                return (3);
            }

            T &operator()(fsize l);

            const T &operator()(fsize l) const;

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
                if (this == &other)
                    return (*this);
                X = other.X;
                Y = other.Y;
                Z = other.Z;
                return (*this);
            }

            inline Vector &operator=(Vector &&other) noexcept
            {
                X = std::move(other.X);
                Y = std::move(other.Y);
                Z = std::move(other.Z);
                return (*this);
            }

            inline Vector operator+(const Vector &other) const
            {
                return (Vector(X + other.X, Y + other.Y, Z + other.Z));
            }

            inline Vector operator-(const Vector &other) const
            {
                return (Vector(X - other.X, Y - other.Y, Z - other.Z));
            }

            inline Vector operator*(const Vector &other) const
            {
                return (Vector(X * other.X, Y * other.Y, Z * other.Z));
            }

            inline Vector operator/(const Vector &other) const
            {
                return (Vector(X / other.X, Y / other.Y, Z / other.Z));
            }

            inline Vector operator*(const T other) const
            {
                return (Vector(X * other, Y * other, Z * other));
            }

            inline Vector operator/(const T other) const
            {
                return (Vector(X / other, Y / other, Z / other));
            }

            inline void operator+=(const Vector &other)
            {
                X += other.X;
                Y += other.Y;
                Z += other.Z;
            }

            inline void operator-=(const Vector &other)
            {
                X -= other.X;
                Y -= other.Y;
                Z -= other.Z;
            }

            inline void operator*=(const Vector &other)
            {
                X *= other.X;
                Y *= other.Y;
                Z *= other.Z;
            }

            inline void operator/=(const Vector &other)
            {
                X /= other.X;
                Y /= other.Y;
                Z /= other.Z;
            }

            inline void operator*=(const T other)
            {
                X *= other;
                Y *= other;
                Z *= other;
            }

            inline void operator/=(const T other)
            {
                X /= other;
                Y /= other;
                Z /= other;
            }

            inline Vector operator-() const
            {
                return (Vector(-X, -Y, -Z));
            }

            inline T Dot(const Vector &other) const
            {
                return (X * other.X + Y * other.Y + Z * other.Z);
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
                return (Math<T>::Sqrt(X * X + Y * Y + Z * Z));
            }

            inline T NormSquared() const
            {
                return (X * X + Y * Y + Z * Z);
            }

            inline Vector Normalize() const
            {
                auto v = Norm();
                return (Vector(X / v, Y / v, Z / v));
            }

            inline Vector Cross(const Vector &other) const
            {
                return (Vector(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X));
            }

            inline bool operator==(const Vector &other) const
            {
                T diffx = Math<T>::Abs(X - other.X);
                T diffy = Math<T>::Abs(Y - other.Y);
                T diffz = Math<T>::Abs(Z - other.Z);

                return (diffx <= Math<T>::Epsilon && diffy <= Math<T>::Epsilon && diffz <= Math<T>::Epsilon);
            }

            inline bool operator!=(const Vector &other) const
            {
                return (!operator==(other));
            }

            inline bool operator<(const Vector &other) const
            {
                return (X < other.X && Y < other.Y && Z < other.Z);
            }

            inline bool operator>(const Vector &other) const
            {
                return (X > other.X &&Y > other.Y && Z > other.Z);
            }

            inline bool operator<=(const Vector &other) const
            {
                return (X <= other.X && Y <= other.Y && Z <= other.Z);
            }

            inline bool operator>=(const Vector &other) const
            {
                return (X >= other.X && Y >= other.Y && Z >= other.Z);
            }

            inline static Vector Lerp(const Vector &v, const Vector &v1, const T t)
            {
                return (Vector(Math<T>::Lerp(v.X, v1.X, t), Math<T>::Lerp(v.Y, v1.Y, t), Math<T>::Lerp(v.Z, v1.Z, t)));
            }

            static const Vector Zero;
            static const Vector Identity;
            static const Vector Right;
            static const Vector Up;
            static const Vector Forward;
        };

        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Zero = Vector((T)0);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Identity = Vector((T)1);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Right = Vector((T)1, (T)0, (T)0);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Up = Vector((T)0, (T)0, (T)1);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Forward = Vector((T)0, (T)1, (T)0);
    }
}

#include "Framework/Math/Vector3.impl.hpp"
