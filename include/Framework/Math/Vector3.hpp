// Copyright (c) 2018, BlockProject
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
#include "Framework/Math/Math.hpp"
#include "Framework/Math/Vector2.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API Vector3
    {
    public:
        T X;
        T Y;
        T Z;

        Vector3(const T x, const T y, const T z)
            : X(x)
            , Y(y)
            , Z(z)
        {
        }

        Vector3(const Vector3<T> &other)
            : X(other.X)
            , Y(other.Y)
            , Z(other.Z)
        {
        }

        Vector3(const Vector2<T> &other, const T z = 0)
            : X(other.X)
            , Y(other.Y)
            , Z(z)
        {
        }

        Vector3()
            : X(0)
            , Y(0)
            , Z(0)
        {
        }

        inline T Dot(const Vector3<T> &other) const
        {
            return (X * other.X + Y * other.Y + Z * other.Z);
        }

        inline Vector3<T> Cross(const Vector3<T> &other) const
        {
            return (Vector3<T>(Y * other.Z - Z * other.Y,
                               Z * other.X - X * other.Z,
                               X * other.Y - Y * other.X));
        }

        inline T Length() const
        {
            return (Math::Sqrt(X * X + Y * Y + Z * Z));
        }

        inline T Distance(const Vector3<T> &other) const
        {
            Vector3<T> v(Math::Abs(other.X - X), Math::Abs(other.Y - Y), Math::Abs(other.Z - Z));
            return (v.Length());
        }

        inline T DistanceSquared(const Vector3<T> &other) const
        {
            Vector3<T> v(Math::Abs(other.X - X), Math::Abs(other.Y - Y), Math::Abs(other.Z - Z));
            return (v.X * v.X + v.Y * v.Y + v.Z * v.Z);
        }

        inline void Normalize()
        {
            X = X / Length();
            Y = Y / Length();
            Z = Z / Length();
        }

        inline Vector3<T> &operator=(const Vector3<T> &other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
            return (*this);
        }

        inline Vector3<T> operator*(const T other) const
        {
            return (Vector3<T>(X * other, Y * other, Z * other));
        }

        inline Vector3<T> operator/(const T other) const
        {
            return (Vector3<T>(X / other, Y / other, Z / other));
        }

        inline Vector3<T> operator+(const T other) const
        {
            return (Vector3<T>(X + other, Y + other, Z + other));
        }

        inline Vector3<T> operator-(const T other) const
        {
            return (Vector3<T>(X - other, Y - other, Z - other));
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

        inline void operator+=(const T other)
        {
            X += other;
            Y += other;
            Z += other;
        }

        inline void operator-=(const T other)
        {
            X -= other;
            Y -= other;
            Z -= other;
        }

        inline Vector3<T> operator+(const Vector3<T> &other) const
        {
            return (Vector3<T>(X + other.X, Y + other.Y, Z + other.Z));
        }

        inline Vector3<T> operator-(const Vector3<T> &other) const
        {
            return (Vector3<T>(X - other.X, Y - other.Y, Z - other.Z));
        }

        inline Vector3<T> operator*(const Vector3<T> &other) const
        {
            return (Vector3<T>(X * other.X, Y * other.Y, Z * other.Z));
        }

        inline Vector3<T> operator/(const Vector3<T> &other) const
        {
            return (Vector3<T>(X / other.X, Y / other.Y, Z / other.Z));
        }

        inline void operator+=(const Vector3<T> &other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;
        }

        inline void operator-=(const Vector3<T> &other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;
        }

        inline void operator*=(const Vector3<T> &other)
        {
            X *= other.X;
            Y *= other.Y;
            Z *= other.Z;
        }

        inline void operator/=(const Vector3<T> &other)
        {
            X /= other.X;
            Y /= other.Y;
            Z /= other.Z;
        }

        inline Vector3<T> operator-() const
        {
            return (Vector3<T>(-X, -Y, -Z));
        }

        inline bool operator==(const Vector3<T> &other) const
        {
            return (X == other.X && Y == other.Y && Z == other.Z);
        }

        inline static Vector3<T> Lerp(const Vector3<T> &v, const Vector3<T> &v1, const T t)
        {
            Vector3<T> res(Math::Lerp(v.X, v1.X, t), Math::Lerp(v.Y, v1.Y, t), Math::Lerp(v.Z, v1.Z, t));

            return (res);
        }

        static const Vector3<T> Zero;
        static const Vector3<T> Unit;
        static const Vector3<T> Right;
        static const Vector3<T> Up;
        static const Vector3<T> Forward;
        static const Vector3<T> Backward;
        static const Vector3<T> Left;
        static const Vector3<T> Down;
    };

    template <typename T>
    const Vector3<T> Vector3<T>::Zero = Vector3<T>();
    template <typename T>
    const Vector3<T> Vector3<T>::Unit = Vector3<T>(1, 1, 1);
    template <typename T>
    const Vector3<T> Vector3<T>::Right = Vector3<T>(1, 0, 0);
    template <typename T>
    const Vector3<T> Vector3<T>::Up = Vector3<T>(0, 0, 1);
    template <typename T>
    const Vector3<T> Vector3<T>::Forward = Vector3<T>(0, -1, 0);
    template <typename T>
    const Vector3<T> Vector3<T>::Backward = Vector3<T>(0, 1, 0);
    template <typename T>
    const Vector3<T> Vector3<T>::Left = Vector3<T>(-1, 0, 0);
    template <typename T>
    const Vector3<T> Vector3<T>::Down = Vector3<T>(0, 0, -1);
}
