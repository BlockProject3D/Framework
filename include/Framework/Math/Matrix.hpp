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
    class BPF_API MatrixException : public Exception
    {
    public:
        inline virtual const char *GetType() const noexcept
        {
            return ("MatrixException");
        }
    };

    template <typename T, fsize M = 0, fsize N = 0>
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

        inline Matrix(const Matrix<T, M, N> &other)
        {
            std::memcpy(_arr, other._arr, N * M * sizeof(T));
        }

        inline Matrix<T, M, N> &operator=(const Matrix<T, M, N> &other)
        {
            std::memcpy(_arr, other._arr, N * M * sizeof(T));
            return (*this);
        }

        inline T &operator()(const fsize x, const fsize y)
        {
            if (x >= N || y >= M)
                throw IndexException((fint)(y * N + x));
            return (_arr[y * N + x]);
        }

        inline T operator()(const fsize x, const fsize y) const
        {
            if (x >= N || y >= M)
                throw IndexException((fint)(y * N + x));
            return (_arr[y * N + x]);
        }

        inline const T *operator*() const
        {
            return (_arr);
        }

        template <fsize P>
        Matrix<T, M, P> operator*(const Matrix<T, N, P> &other) const;
        Matrix<T, N, M> Transpose() const;
        void SwapRows(const fsize rowa, const fsize rowb);
        void SwapColumns(const fsize cola, const fsize colb);

        static const Matrix Zero;
        
        template <typename TT, fsize P, fsize Q>
        friend class Matrix;
    };

    template <typename T, fsize N>
    class BP_TPL_API Matrix<T, N, N>
    {
    private:
        T _arr[N * N];

        static Matrix<T, N, N> GenIdentity();
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

        inline Matrix(const Matrix<T, N, N> &other)
        {
            std::memcpy(_arr, other._arr, N * N * sizeof(T));
        }

        inline Matrix<T, N, N> &operator=(const Matrix<T, N, N> &other)
        {
            std::memcpy(_arr, other._arr, N * N * sizeof(T));
            return (*this);
        }

        inline T &operator()(const fsize x, const fsize y)
        {
            if (x >= N || y >= N)
                throw IndexException((fint)(y * N + x));
            return (_arr[y * N + x]);
        }

        inline T operator()(const fsize x, const fsize y) const
        {
            if (x >= N || y >= N)
                throw IndexException((fint)(y * N + x));
            return (_arr[y * N + x]);
        }

        inline const T *operator*() const
        {
            return (_arr);
        }

        /**
         * Inverts this matrix
         * @throws MatrixException if the matrix is not invertible (ie determinant is null)
         * @return the inverse of this matrix
         */
        Matrix<T, N, N> Invert() const;
        T GetDeterminant() const;
        template <fsize P>
        void GetMinor(Matrix<T, P, P> &dest, fsize row, fsize col) const;
        Matrix<T, N, N> operator*(const Matrix<T, N, N> &other) const;
        Matrix<T, N, N> Transpose() const;
        void SwapRows(const fsize rowa, const fsize rowb);
        void SwapColumns(const fsize cola, const fsize colb);

        static const Matrix Zero;
        static const Matrix Identity;

        template <typename TT, fsize P, fsize Q>
        friend class Matrix;
    };

    template <typename T>
    class BP_TPL_API Matrix<T, 1, 1>
    {
    private:
        T _arr;

    public:
        inline Matrix()
            : _arr(0)
        {
        }

        inline Matrix(const std::initializer_list<T> &lst)
            : _arr(*lst.begin())
        {
        }

        inline Matrix(const T *mat)
            : _arr(*mat)
        {
        }

        inline Matrix(const Matrix<T, 1, 1> &other)
            : _arr(other._arr)
        {
        }

        inline Matrix<T, 1, 1> &operator=(const Matrix<T, 1, 1> &other)
        {
            _arr = other._arr;
            return (*this);
        }

        inline T &operator()(const fsize, const fsize)
        {
            return (_arr);
        }

        inline T operator()(const fsize, const fsize) const
        {
            return (_arr);
        }

        inline T GetDeterminant() const
        {
            return (_arr);
        }

        template <typename TT, fsize P, fsize Q>
        friend class Matrix;
    };
    
    template <typename T>
    class BP_TPL_API Matrix<T, 0, 0>
    {
    private:
        T *_arr;
        fsize _m;
        fsize _n;
    
    public:
        Matrix(const fsize m, const fsize n);
        Matrix(const fsize m, const fsize n, const std::initializer_list<T> &lst);
        Matrix(const fsize m, const fsize n, const T *mat);
        Matrix(const Matrix<T> &other);
        Matrix(Matrix<T> &&other);
        ~Matrix();
        Matrix<T> &operator=(const Matrix<T> &other);
        Matrix<T> &operator=(Matrix<T> &&other);
        
        fsize Rows() const noexcept
        {
            return (_m);
        }

        fsize Columns() const noexcept
        {
            return (_n);
        }

        /**
         * Sets this matrix to identity
         * @throws MatrixException in case this is not a square matrix
         */
        void SetIdentity();
        
        inline T &operator()(const fsize x, const fsize y)
        {
            if (x >= _n || y >= _m)
                throw IndexException((fint)(y * _n + x));
            return (_arr[y * _n + x]);
        }

        inline T operator()(const fsize x, const fsize y) const
        {
            if (x >= _n || y >= _m)
                throw IndexException((fint)(y * _n + x));
            return (_arr[y * _n + x]);
        }

        inline const T *operator*() const
        {
            return (_arr);
        }

        /**
         * Inverts this matrix
         * @throws MatrixException if the matrix is not invertible (ie determinant is null) or if it is not a square matrix
         * @return the inverse of this matrix
         */
        Matrix<T> Invert() const;
        
        /**
         * Calculates the determinant of this matrix
         * @throws MatrixException if the matrix is not squared
         * @return the determinant of this matrix
         */
        T GetDeterminant() const;
        
        /**
         * Calculates a minor matrix of a square matrix
         * @throws MatrixException if the matrix is not squared
         * @return the minor matrix
         */
        void GetMinor(Matrix<T> &dest, fsize row, fsize col) const;
        
        /**
         * Multiplies two matrices
         * @throws MatrixException if the multiplication is impossible
         * @return the product of this by other
         */
        Matrix<T> operator*(const Matrix<T> &other) const;
        Matrix<T> Transpose() const;
        void SwapRows(const fsize rowa, const fsize rowb);
        void SwapColumns(const fsize cola, const fsize colb);
    };

    template <typename T, fsize N, fsize M>
    const Matrix<T, N, M> Matrix<T, N, M>::Zero = Matrix<T, N, M>();

    template <typename T, fsize N>
    const Matrix<T, N, N> Matrix<T, N, N>::Identity = Matrix<T, N, N>::GenIdentity();
}

#include "Framework/Math/Matrix.impl.hpp"
#include "Framework/Math/Matrix2.hpp"
#include "Framework/Math/Matrix3.hpp"
#include "Framework/Math/Matrix4.hpp"

namespace bpf
{
    using Matrix2f = Matrix2<float>;
    using Matrix3f = Matrix3<float>;
    using Matrix4f = Matrix4<float>;
}

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
