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
#undef minor //FUCK YOU LINUX
#undef major //FUCK YOU LINUX

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
                    res(x, y) = 1;
            }
        }
        return (res);
    }

    template <typename T, fsize M, fsize N>
    template <fsize P>
    Matrix<T, M, P> Matrix<T, M, N>::operator*(const Matrix<T, N, P> &other) const
    {
        Matrix<T, M, P> mat;

        for (fsize i = 0; i != M; ++i)
        {
            for (fsize j = 0; j != P; ++j)
            {
                T res = 0;
                for (fsize k = 0; k != N; ++k)
                    res += _arr[i * N + k] * other._arr[k * P + j];
                mat._arr[i * P + j] = res;
            }
        }
        return (mat);
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
    
    template <typename T, fsize M, fsize N>
    Matrix<T, N, M> Matrix<T, M, N>::Transpose() const
    {
        Matrix<T, N, M> res;

        for (fsize x = 0; x != N; ++x)
        {
            for (fsize y = 0; y != M; ++y)
                res(y, x) = operator()(x, y);
        }
        return (res);
    }

    template <typename T, fsize N>
    Matrix<T, N, N> Matrix<T, N, N>::Transpose() const
    {
        Matrix<T, N, N> res = *this;

        for (fsize x = 0; x != N; ++x)
        {
            for (fsize y = 0; y != N; ++y)
            {
                T copy = res(y, x);
                res(y, x) = res(x, y);
                res(x, y) = copy;
            }
        }
        return (res);
    }

    template <typename T, fsize M, fsize N>
    void Matrix<T, M, N>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T copy = operator()(x, rowa);
            operator()(x, rowa) = operator()(x, rowb);
            operator()(x, rowb) = copy;
        }
    }

    template <typename T, fsize N>
    void Matrix<T, N, N>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T copy = operator()(x, rowa);
            operator()(x, rowa) = operator()(x, rowb);
            operator()(x, rowb) = copy;
        }
    }
    
    template <typename T, fsize M, fsize N>
    void Matrix<T, M, N>::SwapColumns(const fsize cola, const fsize colb)
    {
        for (fsize x = 0; x != M; ++x)
        {
            T copy = operator()(cola, x);
            operator()(cola, x) = operator()(colb, x);
            operator()(colb, x) = copy;
        }
    }
    
    template <typename T, fsize N>
    void Matrix<T, N, N>::SwapColumns(const fsize cola, const fsize colb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T copy = operator()(cola, x);
            operator()(cola, x) = operator()(colb, x);
            operator()(colb, x) = copy;
        }
    }

    template <typename T, fsize N>
    Matrix<T, N, N> Matrix<T, N, N>::Invert() const
    {
        T det = GetDeterminant();
        Matrix<T, N - 1, N - 1> minor;
        Matrix<T, N, N> res;

        if (det == 0)
            throw MatrixException();
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
    Matrix<T>::Matrix(const fsize m, const fsize n)
        : _arr(new T[m * n])
        , _m(m)
        , _n(n)
    {
        std::memset(_arr, 0, m * n * sizeof(T));
    }
    
    template <typename T>
    Matrix<T>::Matrix(const fsize m, const fsize n, const std::initializer_list<T> &lst)
        : _arr(new T[m * n])
        , _m(m)
        , _n(n)
    {
        std::memcpy(_arr, lst.begin(), m * n * sizeof(T));
    }
    
    template <typename T>
    Matrix<T>::Matrix(const fsize m, const fsize n, const T *mat)
        : _arr(new T[m * n])
        , _m(m)
        , _n(n)
    {
        std::memcpy(_arr, mat, m * n * sizeof(T));
    }
    
    template <typename T>
    Matrix<T>::Matrix(const Matrix<T> &other)
        : _arr(new T[other._m * other._n])
        , _m(other._m)
        , _n(other._n)
    {
        std::memcpy(_arr, other._arr, _m * _n * sizeof(T));
    }
    
    template <typename T>
    Matrix<T>::Matrix(Matrix<T> &&other)
        : _arr(other._arr)
        , _m(other._m)
        , _n(other._n)
    {
        other._arr = Null;
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
        _m = other._m;
        _n = other._n;
        _arr = new T[_m * _n];
        std::memcpy(_arr, other._arr, _m * _n * sizeof(T));
    }
    
    template <typename T>
    Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other)
    {
        if (_arr != Null)
            delete[] _arr;
        _m = other._m;
        _n = other._n;
        _arr = other._arr;
        other._arr = Null;
    }
    
    template <typename T>
    void Matrix<T>::SetIdentity()
    {
        if (_m != _n)
            throw MatrixException();
        for (fsize y = 0; y != _m; ++y)
        {
            for (fsize x = 0; x != _n; ++x)
            {
                if (x == y)
                    operator()(x, y) = 1;
            }
        }
    }
    
    template <typename T>
    Matrix<T> Matrix<T>::Invert() const
    {
        if (_m != _n)
            throw MatrixException();
        T det = GetDeterminant();
        Matrix<T> minor(_n - 1, _n - 1);
        Matrix<T> res(_n, _n);

        if (det == 0)
            throw MatrixException();
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
            throw MatrixException();
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
            throw MatrixException();
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
        if (_n != other._m)
            throw MatrixException();
        Matrix<T> mat(_m, other._n);

        for (fsize i = 0; i != _m; ++i)
        {
            for (fsize j = 0; j != other._n; ++j)
            {
                T res = 0;
                for (fsize k = 0; k != _n; ++k)
                    res += _arr[i * _n + k] * other._arr[k * other._n + j];
                mat._arr[i * other._n + j] = res;
            }
        }
        return (mat);
    }
    
    template <typename T>
    Matrix<T> Matrix<T>::Transpose() const
    {
        Matrix<T> res(_n, _m);

        for (fsize x = 0; x != _n; ++x)
        {
            for (fsize y = 0; y != _m; ++y)
                res(y, x) = operator()(x, y);
        }
        return (res);
    }
    
    template <typename T>
    void Matrix<T>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != _n; ++x)
        {
            T copy = operator()(x, rowa);
            operator()(x, rowa) = operator()(x, rowb);
            operator()(x, rowb) = copy;
        }
    }
    
    template <typename T>
    void Matrix<T>::SwapColumns(const fsize cola, const fsize colb)
    {
        for (fsize x = 0; x != _m; ++x)
        {
            T copy = operator()(cola, x);
            operator()(cola, x) = operator()(colb, x);
            operator()(colb, x) = copy;
        }
    }
}
