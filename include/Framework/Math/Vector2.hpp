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

namespace bpf
{
    template <typename T>
    class BP_TPL_API Vector2
    {
    public:
        T X;
        T Y;

        Vector2(const T x, const T y)
            : X(x)
            , Y(y)
        {
        }

        Vector2(const Vector2<T> &other)
            : X(other.X)
            , Y(other.Y)
        {
        }

        Vector2()
            : X(0)
            , Y(0)
        {
        }

        inline T Dot(const Vector2<T> &other) const
        {
            return (X * other.X + Y * other.Y);
        }

        inline T Length() const
        {
            return (Math::Sqrt(X * X + Y * Y));
        }

        inline T Distance(const Vector2<T> &other) const
        {
            Vector2<T> v(Math::Abs(other.X - X), Math::Abs(other.Y - Y));
            return (v.Length());
        }

        inline T DistanceSquared(const Vector2<T> &other) const
        {
            Vector2<T> v(Math::Abs(other.X - X), Math::Abs(other.Y - Y));
            return (v.X * v.X + v.Y * v.Y);
        }

        inline void Normalize()
        {
            X = X / Length();
            Y = Y / Length();
        }
        
        //TODO : Rotate function

        inline Vector2<T> &operator=(const Vector2<T> &other)
        {
            X = other.X;
            Y = other.Y;
            return (*this);
        }

        inline Vector2<T> operator*(const T other) const
        {
            return (Vector2<T>(X * other, Y * other));
        }

        inline Vector2<T> operator/(const T other) const
        {
            return (Vector2<T>(X / other, Y / other));
        }

        inline Vector2<T> operator+(const T other) const
        {
            return (Vector2<T>(X + other, Y + other));
        }

        inline Vector2<T> operator-(const T other) const
        {
            return (Vector2<T>(X - other, Y - other));
        }

        inline void operator*=(const T other)
        {
            X *= other;
            Y *= other;
        }

        inline void operator/=(const T other)
        {
            X /= other;
            Y /= other;
        }

        inline void operator+=(const T other)
        {
            X += other;
            Y += other;
        }

        inline void operator-=(const T other)
        {
            X -= other;
            Y -= other;
        }

        inline Vector2<T> operator+(const Vector2<T> &other) const
        {
            return (Vector2<T>(X + other.X, Y + other.Y));
        }

        inline Vector2<T> operator-(const Vector2<T> &other) const
        {
            return (Vector2<T>(X - other.X, Y - other.Y));
        }

        inline Vector2<T> operator*(const Vector2<T> &other) const
        {
            return (Vector2<T>(X * other.X, Y * other.Y));
        }

        inline Vector2<T> operator/(const Vector2<T> &other) const
        {
            return (Vector2<T>(X / other.X, Y / other.Y));
        }

        inline void operator+=(const Vector2<T> &other)
        {
            X += other.X;
            Y += other.Y;
        }

        inline void operator-=(const Vector2<T> &other)
        {
            X -= other.X;
            Y -= other.Y;
        }

        inline void operator*=(const Vector2<T> &other)
        {
            X *= other.X;
            Y *= other.Y;
        }

        inline void operator/=(const Vector2<T> &other)
        {
            X /= other.X;
            Y /= other.Y;
        }

        inline Vector2<T> operator-() const
        {
            return (Vector2<T>(-X, -Y));
        }

        inline bool operator==(const Vector2<T> &other) const
        {
            return (X == other.X && Y == other.Y);
        }

        inline static Vector2<T> Lerp(const Vector2<T> &v, const Vector2<T> &v1, const T t)
        {
            Vector2<T> res(Math::Lerp(v.X, v1.X, t), Math::Lerp(v.Y, v1.Y, t));

            return (res);
        }

        static const Vector2<T> Zero;
        static const Vector2<T> Unit;
    };

    template <typename T>
    const Vector2<T> Vector2<T>::Zero = Vector2<T>();
    template <typename T>
    const Vector2<T> Vector2<T>::Unit = Vector2<T>(1, 1);
}
