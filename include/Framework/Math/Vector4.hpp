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
#include "Framework/Math/Vector3.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API Vector4
    {
    public:
        T X;
        T Y;
        T Z;
        T W;

        Vector4(const T x, const T y, const T z, const T w)
            : X(x)
            , Y(y)
            , Z(z)
            , W(w)
        {
        }

        Vector4(const Vector4<T> &other)
            : X(other.X)
            , Y(other.Y)
            , Z(other.Z)
            , W(other.W)
        {
        }

        Vector4(const Vector3<T> &other, const T w = 0)
            : X(other.X)
            , Y(other.Y)
            , Z(other.Z)
            , W(w)
        {
        }

        Vector4()
            : X(0)
            , Y(0)
            , Z(0)
            , W(0)
        {
        }

        inline T Dot(const Vector4<T> &other) const
        {
            return (X * other.X + Y * other.Y + Z * other.Z + W * other.W);
        }

        inline T Length() const
        {
            return (Math::Sqrt(X * X + Y * Y));
        }

        inline T Distance(const Vector4<T> &other) const
        {
            Vector4<T> v(Math::Abs(other.X - X), Math::Abs(other.Y - Y),
                         Math::Abs(other.Z - Z), Math::Abs(other.W - W));
            return (v.Length());
        }

        inline T DistanceSquared(const Vector4<T> &other) const
        {
            Vector4<T> v(Math::Abs(other.X - X), Math::Abs(other.Y - Y),
                         Math::Abs(other.Z - Z), Math::Abs(other.W - W));
            return (v.X * v.X + v.Y * v.Y + v.Z * v.Z + v.W * v.W);
        }

        inline void Normalize()
        {
            X = X / Length();
            Y = Y / Length();
            Z = Z / Length();
            W = W / Length();
        }
        
        inline Vector4<T> &operator=(const Vector4<T> &other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
            W = other.W;
            return (*this);
        }

        inline Vector4<T> operator*(const T other) const
        {
            return (Vector4<T>(X * other, Y * other, Z * other, W * other));
        }

        inline Vector4<T> operator/(const T other) const
        {
            return (Vector4<T>(X / other, Y / other, Z / other, W / other));
        }

        inline Vector4<T> operator+(const T other) const
        {
            return (Vector4<T>(X + other, Y + other, Z + other, W + other));
        }

        inline Vector4<T> operator-(const T other) const
        {
            return (Vector4<T>(X - other, Y - other, Z - other, W - other));
        }

        inline Vector4<T> &operator*=(const T other)
        {
            X *= other;
            Y *= other;
            Z *= other;
            W *= other;
            return (*this);
        }

        inline Vector4<T> &operator/=(const T other)
        {
            X /= other;
            Y /= other;
            Z /= other;
            W /= other;
            return (*this);
        }

        inline Vector4<T> &operator+=(const T other)
        {
            X += other;
            Y += other;
            Z += other;
            W += other;
            return (*this);
        }

        inline Vector4<T> &operator-=(const T other)
        {
            X -= other;
            Y -= other;
            Z -= other;
            W -= other;
            return (*this);
        }

        inline Vector4<T> operator+(const Vector4<T> &other) const
        {
            return (Vector4<T>(X + other.X, Y + other.Y, Z + other.Z, W + other.W));
        }

        inline Vector4<T> operator-(const Vector4<T> &other) const
        {
            return (Vector4<T>(X - other.X, Y - other.Y, Z - other.Z, W - other.W));
        }

        inline Vector4<T> operator*(const Vector4<T> &other) const
        {
            return (Vector4<T>(X * other.X, Y * other.Y, Z * other.Z, W * other.W));
        }

        inline Vector4<T> operator/(const Vector4<T> &other) const
        {
            return (Vector4<T>(X / other.X, Y / other.Y, Z / other.Z, W / other.W));
        }

        inline void operator+=(const Vector4<T> &other) const
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;
            W += other.W;
        }

        inline void operator-=(const Vector4<T> &other) const
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;
            W -= other.W;
        }

        inline void operator*=(const Vector4<T> &other) const
        {
            X *= other.X;
            Y *= other.Y;
            Z *= other.Z;
            W *= other.W;
        }

        inline void operator/=(const Vector4<T> &other) const
        {
            X /= other.X;
            Y /= other.Y;
            Z /= other.Z;
            W /= other.W;
        }

        inline Vector4<T> operator-() const
        {
            return (Vector4<T>(-X, -Y, -Z, -W));
        }

        inline bool operator==(const Vector4<T> &other) const
        {
            return (X == other.X && Y == other.Y && Z == other.Z && W == other.W);
        }

        inline static Vector4<T> Lerp(const Vector4<T> &v, const Vector4<T> &v1, const T t)
        {
            Vector4<T> res(Math::Lerp(v.X, v1.X, t), Math::Lerp(v.Y, v1.Y, t),
                           Math::Lerp(v.Z, v1.Z, t), Math::Lerp(v.W, v1.W, t));

            return (res);
        }

        static const Vector4<T> Zero;
        static const Vector4<T> Unit;
    };

    template <typename T>
    const Vector4<T> Vector4<T>::Zero = Vector4<T>();
    template <typename T>
    const Vector4<T> Vector4<T>::Unit = Vector4<T>(1, 1, 1, 1);
}
