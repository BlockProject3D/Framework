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
#include <utility>
#include <initializer_list>
#include "Framework/Types.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/Math/Math.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T, fsize I = 0>
        class BP_TPL_API Vector
        {
        private:
            T _arr[I];

        public:
            static const Vector Zero;
            static const Vector Identity;

            inline Vector()
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = DefaultOf<T>();
            }

            explicit inline Vector(const T val)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = val;
            }

            inline Vector(const Vector<T, I - 1> &other, const T val)
            {
                for (fsize i = 0; i != I - 1; ++i)
                    _arr[i] = other(i);
                _arr[I - 1] = val;
            }

            Vector(const std::initializer_list<T> &lst);

            inline Vector(const Vector &other)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = other._arr[i];
            }

            inline Vector(Vector &&other)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = std::move(other._arr[i]);
            }

            constexpr inline fsize Dim() const noexcept
            {
                return (I);
            }

            inline T &operator()(const fsize l)
            {
                if (l >= I)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            inline const T &operator()(const fsize l) const
            {
                if (l >= I)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            Vector &operator=(const Vector &other);
            Vector &operator=(Vector &&other);

            Vector operator+(const Vector &other) const;
            Vector operator-(const Vector &other) const;
            Vector operator*(const Vector &other) const;
            Vector operator/(const Vector &other) const;

            Vector operator*(const T other) const;
            Vector operator/(const T other) const;

            void operator+=(const Vector &other);
            void operator-=(const Vector &other);
            void operator*=(const Vector &other);
            void operator/=(const Vector &other);

            void operator*=(const T other);
            void operator/=(const T other);

            Vector operator-() const;

            bool operator==(const Vector &other) const;

            T Dot(const Vector &other) const;
            T Distance(const Vector &other) const;
            T DistanceSquared(const Vector &other) const;
            T Norm() const;
            T NormSquared() const;
            void Normalize();

            inline const T *operator*() const noexcept
            {
                return (_arr);
            }

            inline T *operator*() noexcept
            {
                return (_arr);
            }

            static Vector Lerp(const Vector &v, const Vector &v1, const T t);
        };

        template <typename T>
        class BP_TPL_API Vector<T, 0>
        {
        private:
            T *_arr;
            fsize _l;

        public:
            inline static Vector Zero(const fsize l)
            {
                return (Vector(l, (T)0));
            }

            inline static Vector Identity(const fsize l)
            {
                return (Vector(l, (T)1));
            }

            inline Vector(const Vector &other, const T val)
                : _arr(new T[other._l + 1])
                , _l(other._l + 1)
            {
                for (fsize i = 0; i != other._l; ++i)
                    _arr[i] = other(i);
                _arr[_l - 1] = val;
            }

            explicit inline Vector(const fsize l)
                : _arr(new T[l])
                , _l(l)
            {
                for (fsize i = 0; i != l; ++i)
                    _arr[i] = DefaultOf<T>();
            }

            inline Vector(const fsize l, const T val)
                : _arr(new T[l])
                , _l(l)
            {
                for (fsize i = 0; i != l; ++i)
                    _arr[i] = val;
            }

            Vector(const std::initializer_list<T> &lst);

            inline Vector(const Vector &other)
                : _arr(new T[other._l])
                , _l(other._l)
            {
                for (fsize i = 0; i != _l; ++i)
                    _arr[i] = other._arr[i];
            }

            inline Vector(Vector &&other)
                : _arr(other._arr)
                , _l(other._l)
            {
                other._arr = Null;
                other._l = 0;
            }

            inline fsize Dim() const noexcept
            {
                return (_l);
            }

            inline T &operator()(const fsize l)
            {
                if (l >= _l)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            inline const T &operator()(const fsize l) const
            {
                if (l >= _l)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            Vector &operator=(const Vector &other);
            Vector &operator=(Vector &&other);

            Vector operator+(const Vector &other) const;
            Vector operator-(const Vector &other) const;
            Vector operator*(const Vector &other) const;
            Vector operator/(const Vector &other) const;

            Vector operator*(const T other) const;
            Vector operator/(const T other) const;

            void operator+=(const Vector &other);
            void operator-=(const Vector &other);
            void operator*=(const Vector &other);
            void operator/=(const Vector &other);

            void operator*=(const T other);
            void operator/=(const T other);

            Vector operator-() const;

            bool operator==(const Vector &other) const;

            T Dot(const Vector &other) const;
            T Distance(const Vector &other) const;
            T DistanceSquared(const Vector &other) const;
            T Norm() const;
            T NormSquared() const;
            void Normalize();

            inline const T *operator*() const noexcept
            {
                return (_arr);
            }

            inline T *operator*() noexcept
            {
                return (_arr);
            }

            static Vector Lerp(const Vector &v, const Vector &v1, const T t);
        };

        template <typename T, fsize I>
        const Vector<T, I> Vector<T, I>::Zero = Vector<T, I>((T)0);

        template <typename T, fsize I>
        const Vector<T, I> Vector<T, I>::Identity = Vector<T, I>((T)1);
    }
}

#include "Framework/Math/Vector.impl.hpp"
#include "Framework/Math/Vector2.hpp"
#include "Framework/Math/Vector3.hpp"
#include "Framework/Math/Vector4.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T>
        using Vector2 = Vector<T, 2>;
        template <typename T>
        using Vector3 = Vector<T, 3>;
        template <typename T>
        using Vector4 = Vector<T, 4>;
        using Vector2f = Vector2<float>;
        using Vector3f = Vector3<float>;
        using Vector4f = Vector4<float>;
    }
}
