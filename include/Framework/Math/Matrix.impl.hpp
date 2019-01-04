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

namespace bpf
{
    template <fsize N, typename T>
    Matrix<N, N, T> Matrix<N, N, T>::GenIdentity()
    {
        Matrix<N, N, T> res;

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

    template <fsize M, fsize N, typename T>
    template <fsize P>
    Matrix<M, P, T> Matrix<M, N, T>::operator*(const Matrix<N, P, T> &other) const
    {
        Matrix<M, P, T> mat;

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

    template <fsize N, typename T>
    Matrix<N, N, T> Matrix<N, N, T>::operator*(const Matrix<N, N, T> &other) const
    {
        Matrix<N, N, T> mat;

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
    
    template <fsize M, fsize N, typename T>
    Matrix<N, M, T> Matrix<M, N, T>::Transpose() const
    {
        Matrix<N, M, T> res;

        for (fsize x = 0; x != N; ++x)
        {
            for (fsize y = 0; y != M; ++y)
                res(y, x) = operator()(x, y);
        }
        return (res);
    }

    template <fsize N, typename T>
    Matrix<N, N, T> Matrix<N, N, T>::Transpose() const
    {
        Matrix<N, N, T> res = *this;

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

    template <fsize M, fsize N, typename T>
    void Matrix<M, N, T>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T copy = operator()(x, rowa);
            operator()(x, rowa) = operator()(x, rowb);
            operator()(x, rowb) = copy;
        }
    }

    template <fsize N, typename T>
    void Matrix<N, N, T>::SwapRows(const fsize rowa, const fsize rowb)
    {
        for (fsize x = 0; x != N; ++x)
        {
            T copy = operator()(x, rowa);
            operator()(x, rowa) = operator()(x, rowb);
            operator()(x, rowb) = copy;
        }
    }

    template <fsize N, typename T>
    Matrix<N, N, T> Matrix<N, N, T>::Invert() const
    {
        T det = GetDeterminant();
        Matrix<N - 1, N - 1, T> minor;
        Matrix<N, N, T> res;

        if (det == 0)
            return (0);
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

    template <fsize N, typename T>
    template <fsize P>
    void Matrix<N, N, T>::GetMinor(Matrix<P, P, T> &dest, fsize row, fsize col) const
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

    template <fsize N, typename T>
    T Matrix<N, N, T>::GetDeterminant() const
    {
        T det = 0;
        Matrix<N - 1, N - 1, T> minor;
        
        for (fsize i = 0; i != N; ++i)
        {
            GetMinor(minor, 0, i);
            det += (i % 2 == 1 ? -1 : 1) * operator()(i, 0) * minor.GetDeterminant();
        }
        return (det);
    }
}
