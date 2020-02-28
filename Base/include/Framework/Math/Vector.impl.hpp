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
#include "Framework/Math/IncompatibleMatrixSizeException.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T, fsize I>
        Vector<T, I>::Vector(const std::initializer_list<T> &lst)
        {
            fsize i = 0;

            for (auto &elem : lst)
                _arr[i++] = elem;
        }

        template <typename T, fsize I>
        Vector<T, I> &Vector<T, I>::operator=(const Vector &other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] = other._arr[i];
            return (*this);
        }

        template <typename T, fsize I>
        Vector<T, I> &Vector<T, I>::operator=(Vector &&other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] = std::move(other._arr[i]);
            return (*this);
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::operator+(const Vector &other) const
        {
            auto res = *this;

            for (fsize i = 0; i != I; ++i)
                res(i) += other(i);
            return (res);
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::operator-(const Vector &other) const
        {
            auto res = *this;

            for (fsize i = 0; i != I; ++i)
                res(i) -= other(i);
            return (res);
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::operator*(const Vector &other) const
        {
            auto res = *this;

            for (fsize i = 0; i != I; ++i)
                res(i) *= other(i);
            return (res);
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::operator/(const Vector &other) const
        {
            auto res = *this;

            for (fsize i = 0; i != I; ++i)
                res(i) /= other(i);
            return (res);
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::operator*(const T other) const
        {
            auto res = *this;

            for (fsize i = 0; i != I; ++i)
                res(i) *= other;
            return (res);
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::operator/(const T other) const
        {
            auto res = *this;

            for (fsize i = 0; i != I; ++i)
                res(i) /= other;
            return (res);
        }

        template <typename T, fsize I>
        void Vector<T, I>::operator+=(const Vector &other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] += other(i);
        }

        template <typename T, fsize I>
        void Vector<T, I>::operator-=(const Vector &other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] -= other(i);
        }

        template <typename T, fsize I>
        void Vector<T, I>::operator*=(const Vector &other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] *= other(i);
        }

        template <typename T, fsize I>
        void Vector<T, I>::operator/=(const Vector &other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] /= other(i);
        }

        template <typename T, fsize I>
        void Vector<T, I>::operator*=(const T other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] *= other;
        }

        template <typename T, fsize I>
        void Vector<T, I>::operator/=(const T other)
        {
            for (fsize i = 0; i != I; ++i)
                _arr[i] /= other;
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::operator-() const
        {
            auto res = *this;

            for (fsize i = 0; i != I; ++i)
                res(i) *= -1;
            return (res);
        }

        template <typename T, fsize I>
        bool Vector<T, I>::operator==(const Vector &other) const
        {
            for (fsize i = 0; i != I; ++i)
            {
                T diff = Math::Abs(_arr[i] - other(i));
                if (diff >= Math::Epsilon<T>())
                    return (false);
            }
            return (true);
        }

        template <typename T, fsize I>
        T Vector<T, I>::Dot(const Vector &other) const
        {
            T res = DefaultOf<T>();

            for (fsize i = 0; i != I; ++i)
                res += _arr[i] * other(i);
            return (res);
        }

        template <typename T, fsize I>
        T Vector<T, I>::Distance(const Vector &other) const
        {
            auto dist = other - *this;

            return (dist.Norm());
        }

        template <typename T, fsize I>
        T Vector<T, I>::DistanceSquared(const Vector &other) const
        {
            auto dist = other - *this;

            return (dist.NormSquared());
        }

        template <typename T, fsize I>
        T Vector<T, I>::Norm() const
        {
            T res = DefaultOf<T>();

            for (fsize i = 0; i != I; ++i)
                res += _arr[i] * _arr[i];
            return (Math::Sqrt(res));
        }

        template <typename T, fsize I>
        T Vector<T, I>::NormSquared() const
        {
            T res = DefaultOf<T>();

            for (fsize i = 0; i != I; ++i)
                res += _arr[i] * _arr[i];
            return (res);
        }

        template <typename T, fsize I>
        void Vector<T, I>::Normalize()
        {
            auto norm = Norm();

            for (fsize i = 0; i != I; ++i)
                _arr[i] /= norm;
        }

        template <typename T, fsize I>
        Vector<T, I> Vector<T, I>::Lerp(const Vector &v, const Vector &v1, const T t)
        {
            auto res = Vector();

            for (fsize i = 0; i != I; ++i)
                res(i) = Math::Lerp(v(i), v1(i), t);
            return (res);
        }

        template <typename T>
        Vector<T>::Vector(const std::initializer_list<T> &lst)
            : _arr(new T[lst.size()])
            , _l(lst.size())
        {
            fsize i = 0;

            for (auto &elem : lst)
                _arr[i++] = elem;
        }

        template <typename T>
        Vector<T> &Vector<T>::operator=(const Vector &other)
        {
            delete[] _arr;
            _l = other._l;
            _arr = new T[_l];
            for (fsize i = 0; i != _l; ++i)
                _arr[i] = other._arr[i];
            return (*this);
        }

        template <typename T>
        Vector<T> &Vector<T>::operator=(Vector &&other)
        {
            delete[] _arr;
            _arr = other._arr;
            _l = other._l;
            other._arr = Null;
            other._l = 0;
            return (*this);
        }

        template <typename T>
        Vector<T> Vector<T>::operator+(const Vector &other) const
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            auto res = *this;

            for (fsize i = 0; i != _l; ++i)
                res(i) += other(i);
            return (res);
        }

        template <typename T>
        Vector<T> Vector<T>::operator-(const Vector &other) const
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            auto res = *this;

            for (fsize i = 0; i != _l; ++i)
                res(i) -= other(i);
            return (res);
        }

        template <typename T>
        Vector<T> Vector<T>::operator*(const Vector &other) const
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            auto res = *this;

            for (fsize i = 0; i != _l; ++i)
                res(i) *= other(i);
            return (res);
        }

        template <typename T>
        Vector<T> Vector<T>::operator/(const Vector &other) const
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            auto res = *this;

            for (fsize i = 0; i != _l; ++i)
                res(i) /= other(i);
            return (res);
        }

        template <typename T>
        Vector<T> Vector<T>::operator*(const T other) const
        {
            auto res = *this;

            for (fsize i = 0; i != _l; ++i)
                res(i) *= other;
            return (res);
        }

        template <typename T>
        Vector<T> Vector<T>::operator/(const T other) const
        {
            auto res = *this;

            for (fsize i = 0; i != _l; ++i)
                res(i) /= other;
            return (res);
        }

        template <typename T>
        void Vector<T>::operator+=(const Vector &other)
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            for (fsize i = 0; i != _l; ++i)
                _arr[i] += other(i);
        }

        template <typename T>
        void Vector<T>::operator-=(const Vector &other)
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            for (fsize i = 0; i != _l; ++i)
                _arr[i] -= other(i);
        }

        template <typename T>
        void Vector<T>::operator*=(const Vector &other)
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            for (fsize i = 0; i != _l; ++i)
                _arr[i] *= other(i);
        }

        template <typename T>
        void Vector<T>::operator/=(const Vector &other)
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            for (fsize i = 0; i != _l; ++i)
                _arr[i] /= other(i);
        }

        template <typename T>
        void Vector<T>::operator*=(const T other)
        {
            for (fsize i = 0; i != _l; ++i)
                _arr[i] *= other;
        }

        template <typename T>
        void Vector<T>::operator/=(const T other)
        {
            for (fsize i = 0; i != _l; ++i)
                _arr[i] /= other;
        }

        template <typename T>
        Vector<T> Vector<T>::operator-() const
        {
            auto res = *this;

            for (fsize i = 0; i != _l; ++i)
                res(i) *= -1;
            return (res);
        }

        template <typename T>
        bool Vector<T>::operator==(const Vector &other) const
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            for (fsize i = 0; i != _l; ++i)
            {
                T diff = Math::Abs(_arr[i] - other(i));
                if (diff >= Math::Epsilon<T>())
                    return (false);
            }
            return (true);
        }

        template <typename T>
        T Vector<T>::Dot(const Vector &other) const
        {
            if (_l != other._l)
                throw IncompatibleMatrixSizeException((fisize)_l, (fisize)other._l);
            T res = DefaultOf<T>();

            for (fsize i = 0; i != _l; ++i)
                res += _arr[i] * other(i);
            return (res);
        }

        template <typename T>
        T Vector<T>::Distance(const Vector &other) const
        {
            auto dist = other - *this;

            return (dist.Norm());
        }

        template <typename T>
        T Vector<T>::DistanceSquared(const Vector &other) const
        {
            auto dist = other - *this;

            return (dist.NormSquared());
        }

        template <typename T>
        T Vector<T>::Norm() const
        {
            T res = DefaultOf<T>();

            for (fsize i = 0; i != _l; ++i)
                res += _arr[i] * _arr[i];
            return (Math::Sqrt(res));
        }

        template <typename T>
        T Vector<T>::NormSquared() const
        {
            T res = DefaultOf<T>();

            for (fsize i = 0; i != _l; ++i)
                res += _arr[i] * _arr[i];
            return (res);
        }

        template <typename T>
        void Vector<T>::Normalize()
        {
            auto norm = Norm();

            for (fsize i = 0; i != _l; ++i)
                _arr[i] /= norm;
        }

        template <typename T>
        Vector<T> Vector<T>::Lerp(const Vector &v, const Vector &v1, const T t)
        {
            if (v._l != v1._l)
                throw IncompatibleMatrixSizeException((fisize)v._l, (fisize)v1._l);
            auto res = Vector(v._l);

            for (fsize i = 0; i != v._l; ++i)
                res(i) = Math::Lerp(v(i), v1(i), t);
            return (res);
        }
    }
}
