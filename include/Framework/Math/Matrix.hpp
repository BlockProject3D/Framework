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
#include <cstring>
#include <initializer_list>
#include "Framework/Types.hpp"
#include "Framework/IndexException.hpp"

namespace bpf
{
    template <fsize M, fsize N, typename T>
    class BP_TPL_API Matrix
    {
    private:
        T _arr[N * M];

    public:
        inline Matrix()
        {
            std::memset(_arr, 0, N * M * sizeof(T));
        }

        inline Matrix(const std::initializer_list<T> &lst)
        {
            std::memcpy(_arr, lst.begin(), N * M * sizeof(T));
        }

        inline Matrix(const T *mat)
        {
            std::memcpy(_arr, mat, N * M * sizeof(T));
        }

        inline Matrix(const Matrix<M, N, T> &other)
        {
            std::memcpy(_arr, other._arr, N * M * sizeof(T));
        }

        inline Matrix<M, N, T> &operator=(const Matrix<M, N, T> &other)
        {
            std::memcpy(_arr, other._arr, N * M * sizeof(T));
            return (*this);
        }

        inline T &operator()(const fsize x, const fsize y)
        {
            if (x >= N || y >= M)
                throw IndexException(x);
            return (_arr[y * N + x]);
        }

        inline T operator()(const fsize x, const fsize y) const
        {
            if (x >= N || y >= M)
                throw IndexException(x);
            return (_arr[y * N + x]);
        }

        template <fsize P>
        Matrix<M, P, T> operator*(const Matrix<N, P, T> &other) const;
        Matrix<N, M, T> Transpose() const;
        void SwapRows(const fsize rowa, const fsize rowb);

        static const Matrix Zero;
        
        template <fsize P, fsize Q, typename T>
        friend class Matrix;
    };

    template <fsize N, typename T>
    class BP_TPL_API Matrix<N, N, T>
    {
    private:
        T _arr[N * N];

        static Matrix<N, N, T> GenIdentity();
    public:
        inline Matrix()
        {
            std::memset(_arr, 0, N * N * sizeof(T));
        }

        inline Matrix(const std::initializer_list<T> &lst)
        {
            std::memcpy(_arr, lst.begin(), N * N * sizeof(T));
        }

        inline Matrix(const T *mat)
        {
            std::memcpy(_arr, mat, N * N * sizeof(T));
        }

        inline Matrix(const Matrix<N, N, T> &other)
        {
            std::memcpy(_arr, other._arr, N * N * sizeof(T));
        }

        inline Matrix<N, N, T> &operator=(const Matrix<N, N, T> &other)
        {
            std::memcpy(_arr, other._arr, N * N * sizeof(T));
            return (*this);
        }

        inline T &operator()(const fsize x, const fsize y)
        {
            if (x >= N || y >= N)
                throw IndexException(x);
            return (_arr[y * N + x]);
        }

        inline T operator()(const fsize x, const fsize y) const
        {
            if (x >= N || y >= N)
                throw IndexException(x);
            return (_arr[y * N + x]);
        }

        Matrix<N, N, T> Invert() const;
        Matrix<N, N, T> operator*(const Matrix<N, N, T> &other) const;
        Matrix<N, N, T> Transpose() const;
        void SwapRows(const fsize rowa, const fsize rowb);

        static const Matrix Zero;
        static const Matrix Identity;

        template <fsize P, fsize Q, typename T>
        friend class Matrix;
    };

    template <fsize N, fsize M, typename T>
    const Matrix<N, M, T> Matrix<N, M, T>::Zero = Matrix<N, M, T>();

    template <fsize N, typename T>
    const Matrix<N, N, T> Matrix<N, N, T>::Identity = Matrix<N, N, T>::GenIdentity();
}

#include "Framework/Math/Matrix.impl.hpp"

namespace Framework
{
    enum ERotationAxis
    {
        ROT_AXIS_X,
        ROT_AXIS_Y,
        ROT_AXIS_Z
    };

    class ENGINE_API FMatrix
    {
    private:
        float Data[16];
    public:
        FMatrix();
        FMatrix(std::initializer_list<float> lst);
        FMatrix(const FMatrix &other);
        FMatrix(const float *mat);
        FMatrix &operator=(const FMatrix &other);
        void Invert();
        void Translate(const FVector &v);
        void Rotate(const ERotationAxis axis, const float ang);
        void RotateYaw(const float ang);
        void RotatePitch(const float ang);
        void Scale(const FVector &v);
        void Set(const int i, const int j, const float d);
        float Get(const int i, const int j) const;
        FMatrix operator*(const FMatrix &other) const;
        inline const float *operator*() const
        {
            return (Data);
        }
      
        static FMatrix Identity;
        static FMatrix Zero;
    };
};
