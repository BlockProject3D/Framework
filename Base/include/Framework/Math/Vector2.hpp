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
        class BP_TPL_API Vector<T, 2>
        {
        public:
            T X;
            T Y;

            inline Vector(const T x, const T y)
                : X(x)
                , Y(y)
            {
            }

            inline Vector()
                : X(DefaultOf<T>())
                , Y(DefaultOf<T>())
            {
            }

            explicit inline Vector(const T val)
                : X(val)
                , Y(val)
            {
            }

            Vector(const std::initializer_list<T> &lst);

            inline Vector(const Vector &other)
                : X(other.X)
                , Y(other.Y)
            {
            }

            inline Vector(Vector &&other)
                : X(std::move(other.X))
                , Y(std::move(other.Y))
            {
            }

            constexpr inline fsize Dim() const noexcept
            {
                return (2);
            }

            T &operator()(const fsize l);

            const T &operator()(const fsize l) const;

            inline Vector &operator=(const Vector &other)
            {
                X = other.X;
                Y = other.Y;
                return (*this);
            }

            inline Vector &operator=(Vector &&other)
            {
                X = std::move(other.X);
                Y = std::move(other.Y);
                return (*this);
            }

            inline Vector operator+(const Vector &other) const
            {
                return (Vector(X + other.X, Y + other.Y));
            }

            inline Vector operator-(const Vector &other) const
            {
                return (Vector(X - other.X, Y - other.Y));
            }

            inline Vector operator*(const Vector &other) const
            {
                return (Vector(X * other.X, Y * other.Y));
            }

            inline Vector operator/(const Vector &other) const
            {
                return (Vector(X / other.X, Y / other.Y));
            }

            inline Vector operator*(const T other) const
            {
                return (Vector(X * other, Y * other));
            }

            inline Vector operator/(const T other) const
            {
                return (Vector(X / other, Y / other));
            }

            inline void operator+=(const Vector &other)
            {
                X += other.X;
                Y += other.Y;
            }

            inline void operator-=(const Vector &other)
            {
                X -= other.X;
                Y -= other.Y;
            }

            inline void operator*=(const Vector &other)
            {
                X *= other.X;
                Y *= other.Y;
            }

            inline void operator/=(const Vector &other)
            {
                X /= other.X;
                Y /= other.Y;
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

            inline Vector operator-() const
            {
                return (Vector(-X, -Y));
            }

            inline T Dot(const Vector &other) const
            {
                return (X * other.X + Y * other.Y);
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
                return (Math::Sqrt(X * X + Y * Y));
            }

            inline T NormSquared() const
            {
                return (X * X + Y * Y);
            }

            inline void Normalize()
            {
                auto v = Norm();
                X /= v;
                Y /= v;
            }

            inline bool operator==(const Vector &other) const
            {
                T diffx = Math::Abs(X - other.X);
                T diffy = Math::Abs(Y - other.Y);

                return (diffx <= Math::Epsilon<T>() && diffy <= Math::Epsilon<T>());
            }

            inline static Vector Lerp(const Vector &v, const Vector &v1, const T t)
            {
                return (Vector(Math::Lerp(v.X, v1.X, t), Math::Lerp(v.Y, v1.Y, t)));
            }

            static const Vector Zero;
            static const Vector Identity;
        };

        template <typename T>
        const Vector<T, 2> Vector<T, 2>::Zero = Vector((T)0);
        template <typename T>
        const Vector<T, 2> Vector<T, 2>::Identity = Vector((T)1);
    }
}

#include "Framework/Math/Vector2.impl.hpp"
