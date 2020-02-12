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
#include <utility>
#undef minor //FUCK YOU LINUX
#undef major //FUCK YOU LINUX

#include "Framework/Math/IncompatibleMatrixSizeException.hpp"
#include "Framework/Math/NonInvertibleMatrixException.hpp"
#include "Framework/Math/NonSquareMatrixException.hpp"

namespace bpf
{
    template <typename T, fsize N>
    Matrix<T, N, N> Matrix<T, N, N>::GenIdentity()
    {
        Matrix<T, N, N> res;

        for (fsize y = 0; y != N; ++y)
        {
            for (fsize x = 0; x != N; ++x)
            {
                if (x == y)
                    res(x, y) = (T)1;
            }
        }
        return (res);
    }

    template <typename T, fsize N, fsize M>
    template <fsize P>
    Matrix<T, N, P> Matrix<T, N, M>::operator*(const Matrix<T, M, P> &other) const
    {
        Matrix<T, N, P> mat;

        for (fsize i = 0; i != N; ++i)
        {
            for (fsize j = 0; j != P; ++j)
            {
                T res = 0;
                for (fsize k = 0; k != M; ++k)
                    res += _arr[i * M + k] * other._arr[k * P + j];
                mat._arr[i * P + j] = res;
            }
        }
        return (mat);
    }

    template <typename T, fsize N, fsize M>
    Vector<T, N> Matrix<T, N, M>::operator*(const Vector<T, N> &other) const
    {
        Vector<T, N> res;

        for (fsize i = 0; i < N; ++i)
        {
            T res = 0;
            for (fsize k = 0; k < N; ++k)
                res += other(k) * _arr[i * N + k];
            res(i) = res;
        }
        return (res);

    }

    template <typename T, fsize N>
    Matrix<T, N, N> Matrix<T, N, N>::operator*(const Matrix<T, N, N> &other) const
    {
        Matrix<T, N, N> mat;

        for (fsize i = 0; i != N; ++i)
        {
            for (fsize j = 0; j != N; ++j)
            {
                T res = 0;
                for (fsize k = 0; k != N; ++k)
                    res += _arr[k * N + j] * other._arr[i * N + k];
                mat._arr[i * N + j] = res;
            }
        }
        return (mat);
    }

    template <typename T, fsize N>
    Vector<T, N> Matrix<T, N, N>::operator*(const Vector<T, N> &other) const
    {
        Vector<T, N> asda;

        for (fsize i = 0; i < N; ++i)
        {
            T res = 0;
            for (fsize k = 0; k < N; ++k)
                res += other(k) * _arr[i * N + k];
            asda(i) = res;
        }
        return (asda);

    }

    template <typename T, fsize N, fsize M>
    Matrix<T, M, N> Matrix<T, N, M>::Transpose() const
    {
        Matrix<T, M, N> res;

        for (fsize x = 0; x != M; ++x)
        {
            for (fsize y = 0; y != N; ++y)
                res(x, y) = operator()(y, x);
        }
        return (res);
    }

    template <typename T, fsize N>
    Matrix<T, N, N> Matrix<T, N, N>::Transpose() const
    {
        Matrix<T, N, N> res;

        for (fsize x = 0; x != N; ++x)
        {
            for (fsize y = 0; y != N; ++y)
                res(x, y) = operator()(y, x);
        }
        return (res);
    }

    template <typename T, fsize N, fsize M>
    void Matrix<T, N, M>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != M; ++x)
        {
            T tmp = std::move(operator()(rowa, x));
            operator()(rowa, x) = std::move(operator()(rowb, x));
            operator()(rowb, x) = std::move(tmp);
        }
    }

    template <typename T, fsize N>
    void Matrix<T, N, N>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T tmp = std::move(operator()(rowa, x));
            operator()(rowa, x) = std::move(operator()(rowb, x));
            operator()(rowb, x) = std::move(tmp);
        }
    }

    template <typename T, fsize N, fsize M>
    void Matrix<T, N, M>::SwapColumns(const fsize cola, const fsize colb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T tmp = std::move(operator()(x, cola));
            operator()(x, cola) = std::move(operator()(x, colb));
            operator()(x, colb) = std::move(tmp);
        }
    }
    
    template <typename T, fsize N>
    void Matrix<T, N, N>::SwapColumns(const fsize cola, const fsize colb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T tmp = std::move(operator()(x, cola));
            operator()(x, cola) = std::move(operator()(x, colb));
            operator()(x, colb) = std::move(tmp);
        }
    }

    template <typename T, fsize N>
    Matrix<T, N, N> Matrix<T, N, N>::Invert() const
    {
        T det = GetDeterminant();
        Matrix<T, N - 1, N - 1> minor;
        Matrix<T, N, N> res;

        if (det == 0)
            throw NonInvertibleMatrixException();
        det = (T)1 / det;
        for (fsize j = 0; j != N; ++j)
        {
            for (fsize i = 0; i != N; ++i)
            {
                GetMinor(minor, j, i);
                res(j, i) = det * minor.GetDeterminant();
                if ((i + j) % 2 == 1)
                    res(j, i) = -res(j, i);
            }
        }
        return (res);
    }

    template <typename T, fsize N>
    template <fsize P>
    void Matrix<T, N, N>::GetMinor(Matrix<T, P, P> &dest, fsize row, fsize col) const
    {
        fsize coli = 0;
        fsize rowi = 0;

        for (fsize i = 0; i != N; ++i)
        {
            if (i != row)
            {
                coli = 0;
                for (fsize j = 0; j != N; ++j)
                {
                    if (j != col)
                    {
                        dest(coli, rowi) = operator()(j, i);
                        ++coli;
                    }
                }
                ++rowi;
            }
        }
    }

    template <typename T, fsize N>
    T Matrix<T, N, N>::GetDeterminant() const
    {
        T det = 0;
        Matrix<T, N - 1, N - 1> minor;
        
        for (fsize i = 0; i != N; ++i)
        {
            GetMinor(minor, 0, i);
            det += (i % 2 == 1 ? -1 : 1) * operator()(i, 0) * minor.GetDeterminant();
        }
        return (det);
    }
    
    template <typename T>
    Matrix<T>::Matrix(const fsize n, const fsize m)
        : _arr(new T[n * m])
        , _n(n)
        , _m(m)
    {
        for (fsize i = 0; i != n * m; ++i)
            _arr[i] = DefaultOf<T>();
    }

    template <typename T>
    Matrix<T>::Matrix(const fsize n, const fsize m, const T val)
        : _arr(new T[n * m])
        , _n(n)
        , _m(m)
    {
        for (fsize i = 0; i != n * m; ++i)
            _arr[i] = val;
    }

    template <typename T>
    Matrix<T>::Matrix(const fsize n, const fsize m, const std::initializer_list<T> &lst)
        : _arr(new T[n * m])
        , _n(n)
        , _m(m)
    {
        fsize i = 0;

        for (auto &elem : lst)
        {
            if (i >= m * n)
                break;
            _arr[i++] = elem;
        }
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix<T> &other)
        : _arr(new T[other._n * other._m])
        , _n(other._n)
        , _m(other._m)
    {
        for (fsize i = 0; i != _n * _m; ++i)
            _arr[i] = other._arr[i];
    }

    template <typename T>
    Matrix<T>::Matrix(Matrix<T> &&other)
        : _arr(other._arr)
        , _n(other._n)
        , _m(other._m)
    {
        other._arr = Null;
        other._n = 0;
        other._m = 0;
    }
    
    template <typename T>
    Matrix<T>::~Matrix()
    {
        delete[] _arr;
    }

    template <typename T>
    Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other)
    {
        if (_arr != Null)
            delete[] _arr;
        _n = other._n;
        _m = other._m;
        _arr = new T[_n * _m];
        for (fsize i = 0; i != _m * _n; ++i)
            _arr[i] = other._arr[i];
    }

    template <typename T>
    Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other)
    {
        if (_arr != Null)
            delete[] _arr;
        _n = other._n;
        _m = other._m;
        _arr = other._arr;
        other._arr = Null;
        other._n = 0;
        other._m = 0;
    }

    template <typename T>
    Matrix<T> Matrix<T>::Identity(const fsize n)
    {
        Matrix<T> m(n, n);

        for (fsize y = 0; y != n; ++y)
        {
            for (fsize x = 0; x != n; ++x)
            {
                if (x == y)
                    m(y, x) = (T)1;
            }
        }
        return (m);
    }

    template <typename T>
    Matrix<T> Matrix<T>::Zero(const fsize n, const fsize m)
    {
        return (Matrix<T>(n, m == 0 ? n : m, (T)0));
    }

    template <typename T>
    Matrix<T> Matrix<T>::Invert() const
    {
        if (_m != _n)
            throw NonSquareMatrixException();
        T det = GetDeterminant();
        Matrix<T> minor(_n - 1, _n - 1);
        Matrix<T> res(_n, _n);

        if (det == 0)
            throw NonInvertibleMatrixException();
        det = (T)1 / det;
        for (fsize j = 0; j != _n; ++j)
        {
            for (fsize i = 0; i != _n; ++i)
            {
                GetMinor(minor, j, i);
                res(j, i) = det * minor.GetDeterminant();
                if ((i + j) % 2 == 1)
                    res(j, i) = -res(j, i);
            }
        }
        return (res);
    }
    
    template <typename T>
    T Matrix<T>::GetDeterminant() const
    {
        if (_m != _n)
            throw NonSquareMatrixException();
        if (_n == 1)
            return (operator()(0, 0));
        T det = 0;
        Matrix<T> minor(_n - 1, _n - 1);
        
        for (fsize i = 0; i != _n; ++i)
        {
            GetMinor(minor, 0, i);
            det += (i % 2 == 1 ? -1 : 1) * operator()(i, 0) * minor.GetDeterminant();
        }
        return (det);
    }
    
    template <typename T>
    void Matrix<T>::GetMinor(Matrix<T> &dest, fsize row, fsize col) const
    {
        fsize coli = 0;
        fsize rowi = 0;

        if (_m != _n)
            throw NonSquareMatrixException();
        for (fsize i = 0; i != _n; ++i)
        {
            if (i != row)
            {
                coli = 0;
                for (fsize j = 0; j != _n; ++j)
                {
                    if (j != col)
                    {
                        dest(coli, rowi) = operator()(j, i);
                        ++coli;
                    }
                }
                ++rowi;
            }
        }
    }
    
    template <typename T>
    Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const
    {
        if (_m != other._n)
            throw IncompatibleMatrixSizeException((fisize)_m, (fisize)other._n);
        Matrix<T> mat(_n, other._m);

        for (fsize i = 0; i != _n; ++i)
        {
            for (fsize j = 0; j != other._m; ++j)
            {
                T res = 0;
                for (fsize k = 0; k != _m; ++k)
                    res += _arr[i * _m + k] * other._arr[k * other._m + j];
                mat._arr[i * other._m + j] = res;
            }
        }
        return (mat);
    }

    template <typename T>
    Vector<T> Matrix<T>::operator*(const Vector<T> &other) const
    {
        if (_n != other.Dim())
            throw IncompatibleMatrixSizeException((fisize)_n, (fisize)other.Dim());
        Vector<T> res;

        for (fsize i = 0; i < _n; ++i)
        {
            T res = 0;
            for (fsize k = 0; k < _n; ++k)
                res += other(k) * _arr[i * _n + k];
            res(i) = res;
        }
        return (res);
    }

    template <typename T>
    Matrix<T> Matrix<T>::Transpose() const
    {
        Matrix<T> res(_m, _n);

        for (fsize x = 0; x != _m; ++x)
        {
            for (fsize y = 0; y != _n; ++y)
                res(x, y) = operator()(y, x);
        }
        return (res);
    }
    
    template <typename T>
    void Matrix<T>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != _m; ++x)
        {
            T tmp = std::move(operator()(rowa, x));
            operator()(rowa, x) = std::move(operator()(rowb, x));
            operator()(rowb, x) = std::move(tmp);
        }
    }
    
    template <typename T>
    void Matrix<T>::SwapColumns(const fsize cola, const fsize colb)
    {
        for (fsize x = 0; x != _n; ++x)
        {
            T tmp = std::move(operator()(x, cola));
            operator()(x, cola) = std::move(operator()(x, colb));
            operator()(x, colb) = std::move(tmp);
        }
    }
}
