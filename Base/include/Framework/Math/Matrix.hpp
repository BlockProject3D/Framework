// Copyright (c) 2020, BlockProject 3D
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
//     * Neither the name of BlockProject 3D nor the names of its contributors
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
#include "Framework/IndexException.hpp"
#include "Framework/Math/Vector.hpp"
#include "Framework/Types.hpp"
#include <cstring>
#include <initializer_list>

namespace bpf
{
    namespace math
    {
        /**
         * Static matrix class. Memory layout is based on rows
         * @tparam T the number type
         * @tparam N the number of rows
         * @tparam M the number of columns
         */
        template <typename T, fsize N = 0, fsize M = 0>
        class BP_TPL_API Matrix
        {
        private:
            T _arr[N * M];

        public:
            /**
             * Constructs an empty matrix
             */
            inline Matrix()
            {
                for (fsize i = 0; i != N * M; ++i)
                    _arr[i] = T();
            }

            /**
             * Constructs a matrix from a single value
             * @param val the value to initialize all cells
             */
            explicit inline Matrix(const T val)
            {
                for (fsize i = 0; i != N * M; ++i)
                    _arr[i] = val;
            }

            /**
             * Constructs a matrix from an initializer list
             * @param lst the list to copy values from
             */
            inline Matrix(const std::initializer_list<T> &lst)
            {
                fsize i = 0;

                for (auto &elem : lst)
                {
                    if (i >= N * M)
                        break;
                    _arr[i++] = elem;
                }
            }

            /**
             * Copy constructor
             */
            inline Matrix(const Matrix<T, N, M> &other)
            {
                for (fsize i = 0; i != N * M; ++i)
                    _arr[i] = other._arr[i];
            }

            /**
             * Move constructor
             */
            inline Matrix(Matrix<T, N, M> &&other) noexcept
            {
                for (fsize i = 0; i != N * M; ++i)
                    _arr[i] = std::move(other._arr[i]);
            }

            /**
             * Copy assignment operator
             */
            inline Matrix<T, N, M> &operator=(const Matrix<T, N, M> &other)
            {
                if (this == &other)
                    return (*this);
                for (fsize i = 0; i != N * M; ++i)
                    _arr[i] = other._arr[i];
                return (*this);
            }

            /**
             * Move assignment operator
             */
            inline Matrix<T, N, M> &operator=(Matrix<T, N, M> &&other) noexcept
            {
                for (fsize i = 0; i != N * M; ++i)
                    _arr[i] = std::move(other._arr[i]);
                return (*this);
            }

            /**
             * Obtains the value of a cell for modification
             * @param l the row number
             * @param c the column number
             * @return mutable cell value
             */
            inline T &operator()(const fsize l, const fsize c)
            {
                if (l >= N || c >= M)
                    throw IndexException((fisize)(l * M + c));
                return (_arr[l * M + c]);
            }

            /**
             * Obtains the value of a cell for reading
             * @param l the row number
             * @param c the column number
             * @return immutable cell value
             */
            inline T operator()(const fsize l, const fsize c) const
            {
                if (l >= N || c >= M)
                    throw IndexException((fisize)(l * M + c));
                return (_arr[l * M + c]);
            }

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return immutable low-level C array
             */
            inline const T *operator*() const noexcept
            {
                return (_arr);
            }

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return mutable low-level C array
             */
            inline T *operator*() noexcept
            {
                return (_arr);
            }

            /**
             * Compare two matrices
             * @param other matrix to compare with
             * @return true if the two matrices are equal, false otherwise
             */
            bool operator==(const Matrix<T, N, M> &other) const;

            /**
             * Performs matrix-matrix multiplication
             * @tparam P number of columns of the other matrix
             * @param other operand
             * @return new matrix
             */
            template <fsize P>
            Matrix<T, N, P> operator*(const Matrix<T, M, P> &other) const;

            /**
             * Performs matrix addition
             * @param other operand
             * @return new matrix
             */
            Matrix<T, N, M> operator+(const Matrix<T, N, M> &other) const;

            /**
             * Performs matrix-vector multiplication
             * @param other operand
             * @return new vector
             */
            Vector<T, N> operator*(const Vector<T, N> &other) const;

            /**
             * Performs matrix-scalar multiplication
             * @param other operand
             * @return new matrix
             */
            Matrix<T, N, M> operator*(const T &other) const;

            /**
             * Computes the transpose
             * @return new matrix transpose of this
             */
            Matrix<T, M, N> Transpose() const;

            /**
             * Swaps two rows of this matrix
             * @param rowa first row number
             * @param rowb second row number
             */
            void SwapRows(fsize rowa, fsize rowb);

            /**
             * Swaps two columns of this matrix
             * @param cola first column number
             * @param colb second column number
             */
            void SwapColumns(fsize cola, fsize colb);

            /**
             * Zero-matrix constant
             */
            static const Matrix Zero;

            template <typename TT, fsize P, fsize Q>
            friend class Matrix;
        };

        /**
         * Static square matrix class. Memory layout is based on rows
         * @tparam T the number type
         * @tparam N order of matrix
         */
        template <typename T, fsize N>
        class BP_TPL_API Matrix<T, N, N>
        {
        private:
            T _arr[N * N];

            static Matrix<T, N, N> GenIdentity() noexcept;

        public:
            /**
             * Constructs an empty matrix
             */
            inline Matrix()
            {
                for (fsize i = 0; i != N * N; ++i)
                    _arr[i] = T();
            }

            /**
             * Constructs a matrix from a single value
             * @param val the value to initialize all cells
             */
            explicit inline Matrix(const T val)
            {
                for (fsize i = 0; i != N * N; ++i)
                    _arr[i] = val;
            }

            /**
             * Constructs a matrix from an initializer list
             * @param lst the list to copy values from
             */
            inline Matrix(const std::initializer_list<T> &lst)
            {
                fsize i = 0;

                for (auto &elem : lst)
                {
                    if (i >= N * N)
                        break;
                    _arr[i++] = elem;
                }
            }

            /**
             * Copy constructor
             */
            inline Matrix(const Matrix<T, N, N> &other)
            {
                for (fsize i = 0; i != N * N; ++i)
                    _arr[i] = other._arr[i];
            }

            /**
             * Move constructor
             */
            inline Matrix(Matrix<T, N, N> &&other) noexcept
            {
                for (fsize i = 0; i != N * N; ++i)
                    _arr[i] = std::move(other._arr[i]);
            }

            /**
             * Returns the order of this matrix
             * @return unsigned constant
             */
            inline constexpr fsize Order() const noexcept
            {
                return (N);
            }

            /**
             * Copy assignment operator
             */
            inline Matrix<T, N, N> &operator=(const Matrix<T, N, N> &other)
            {
                if (this == &other)
                    return (*this);
                for (fsize i = 0; i != N * N; ++i)
                    _arr[i] = other._arr[i];
                return (*this);
            }

            /**
             * Move assignment operator
             */
            inline Matrix<T, N, N> &operator=(Matrix<T, N, N> &&other) noexcept
            {
                for (fsize i = 0; i != N * N; ++i)
                    _arr[i] = std::move(other._arr[i]);
                return (*this);
            }

            /**
             * Obtains the value of a cell for modification
             * @param l the row number
             * @param c the column number
             * @return mutable cell value
             */
            inline T &operator()(const fsize l, const fsize c)
            {
                if (l >= N || c >= N)
                    throw IndexException((fisize)(l * N + c));
                return (_arr[l * N + c]);
            }

            /**
             * Obtains the value of a cell for reading
             * @param l the row number
             * @param c the column number
             * @return immutable cell value
             */
            inline T operator()(const fsize l, const fsize c) const
            {
                if (l >= N || c >= N)
                    throw IndexException((fisize)(l * N + c));
                return (_arr[l * N + c]);
            }

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return immutable low-level C array
             */
            inline const T *operator*() const noexcept
            {
                return (_arr);
            }

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return mutable low-level C array
             */
            inline T *operator*() noexcept
            {
                return (_arr);
            }

            /**
             * Computes the inverse of this matrix
             * @throw NonInvertibleMatrixException if the matrix is not invertible (ie determinant is null)
             * @return new inversed matrix
             */
            Matrix<T, N, N> Inverse() const;

            /**
             * Computes the determinant of this matrix
             * @return number
             */
            T GetDeterminant() const;

            /**
             * Computes a minor matrix. Used mainly for matrix inversion
             * @tparam P order of the target minor matrix
             * @param dest the destination minor matrix
             * @param row the start row
             * @param col the start column
             */
            template <fsize P>
            void GetMinor(Matrix<T, P, P> &dest, fsize row, fsize col) const;

            /**
             * Compare two matrices
             * @param other matrix to compare with
             * @return true if the two matrices are equal, false otherwise
             */
            bool operator==(const Matrix<T, N, N> &other) const;

            /**
             * Performs matrix-matrix multiplication
             * @tparam P number of columns of the other matrix
             * @param other operand
             * @return new matrix
             */
            template <fsize P>
            Matrix<T, N, P> operator*(const Matrix<T, N, P> &other) const;

            /**
             * Performs matrix addition
             * @param other operand
             * @return new matrix
             */
            Matrix<T, N, N> operator+(const Matrix<T, N, N> &other) const;

            /**
             * Performs matrix-scalar multiplication
             * @param other operand
             * @return new matrix
             */
            Matrix<T, N, N> operator*(const T &other) const;

            /**
             * Performs matrix-vector multiplication
             * @param other operand
             * @return new vector
             */
            Vector<T, N> operator*(const Vector<T, N> &other) const;

            /**
             * Computes the transpose
             * @return new matrix transpose of this
             */
            Matrix<T, N, N> Transpose() const;

            /**
             * Swaps two rows of this matrix
             * @param rowa first row number
             * @param rowb second row number
             */
            void SwapRows(fsize rowa, fsize rowb);

            /**
             * Swaps two columns of this matrix
             * @param cola first column number
             * @param colb second column number
             */
            void SwapColumns(fsize cola, fsize colb);

            /**
             * Zero-matrix constant
             */
            static const Matrix Zero;

            /**
             * Identity matrix constant
             */
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

            explicit inline Matrix(const T val)
                : _arr(val)
            {
            }

            inline Matrix(const Matrix<T, 1, 1> &other)
                : _arr(other._arr)
            {
            }

            inline Matrix<T, 1, 1> &operator=(const Matrix<T, 1, 1> &other)
            {
                if (this == &other)
                    return (*this);
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

        /**
         * Dynamic matrix class. Memory layout is based on rows
         * @tparam T the number type
         * @tparam N order of matrix
         */
        template <typename T>
        class BP_TPL_API Matrix<T, 0, 0>
        {
        private:
            T *_arr;
            fsize _n;
            fsize _m;

        public:
            /**
             * Generates an identity matrix
             * @param n order of the matrix
             * @return new matrix
             */
            static Matrix Identity(fsize n);

            /**
             * Generates an zero matrix
             * @param n number of rows
             * @param m number of columns
             * @return new matrix
             */
            static Matrix Zero(fsize n, fsize m = 0);

            /**
             * Constructs an empty matrix
             * @param n number of rows
             * @param m number of columns
             */
            Matrix(fsize n, fsize m);

            /**
             * Constructs a matrix from an initializer list
             * @param n number of rows
             * @param m number of columns
             * @param lst the list to copy values from
             */
            Matrix(fsize n, fsize m, const std::initializer_list<T> &lst);

            /**
             * Constructs a matrix from a single value
             * @param n number of rows
             * @param m number of columns
             * @param val the value to initialize all cells
             */
            Matrix(fsize n, fsize m, T val);

            /**
             * Copy constructor
             */
            Matrix(const Matrix<T> &other);

            /**
             * Move constructor
             */
            Matrix(Matrix<T> &&other) noexcept;

            ~Matrix();

            /**
             * Copy assignment operator
             */
            Matrix<T> &operator=(const Matrix<T> &other);

            /**
             * Move assignment operator
             */
            Matrix<T> &operator=(Matrix<T> &&other) noexcept;

            /**
             * Returns the number of rows
             * @return unsigned
             */
            fsize Rows() const noexcept
            {
                return (_n);
            }

            /**
             * Returns the number of columns
             * @return unsigned
             */
            fsize Columns() const noexcept
            {
                return (_m);
            }

            /**
             * Obtains the value of a cell for modification
             * @param l the row number
             * @param c the column number
             * @throw IndexException if out of bounds
             * @return mutable cell value
             */
            inline T &operator()(const fsize l, const fsize c)
            {
                if (l >= _n || c >= _m)
                    throw IndexException((fisize)(l * _m + c));
                return (_arr[l * _m + c]);
            }

            /**
             * Obtains the value of a cell for reading
             * @param l the row number
             * @param c the column number
             * @throw IndexException if out of bounds
             * @return immutable cell value
             */
            inline T operator()(const fsize l, const fsize c) const
            {
                if (l >= _n || c >= _m)
                    throw IndexException((fisize)(l * _m + c));
                return (_arr[l * _m + c]);
            }

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return immutable low-level C array
             */
            inline const T *operator*() const noexcept
            {
                return (_arr);
            }

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return mutable low-level C array
             */
            inline T *operator*() noexcept
            {
                return (_arr);
            }

            /**
             * Computes the inverse of this matrix
             * @throw NonInvertibleMatrixException if the matrix is not invertible (ie determinant is null)
             * @throw NonSquareMatrixException if the matrix is not a square matrix
             * @return new inversed matrix
             */
            Matrix<T> Inverse() const;

            /**
             * Computes the determinant of this matrix
             * @throw NonSquareMatrixException if the matrix is not a square matrix
             * @return number
             */
            T GetDeterminant() const;

            /**
             * Computes a minor matrix. Used mainly for matrix inversion
             * @param dest the destination minor matrix
             * @param row the start row
             * @param col the start column
             * @throw NonSquareMatrixException if the matrix is not a square matrix
             */
            void GetMinor(Matrix<T> &dest, fsize row, fsize col) const;

            /**
             * Compare two matrices
             * @param other matrix to compare with
             * @return true if the two matrices are equal, false otherwise
             */
            bool operator==(const Matrix<T> &other) const;

            /**
             * Performs matrix-matrix multiplication
             * @param other operand
             * @throw IncompatibleMatrixSizeException when the matrices have incompatible size
             * @return new matrix
             */
            Matrix<T> operator*(const Matrix<T> &other) const;

            /**
             * Performs matrix addition
             * @param other operand
             * @throw IncompatibleMatrixSizeException when the matrices have incompatible size
             * @return new matrix
             */
            Matrix<T> operator+(const Matrix<T> &other) const;

            /**
             * Performs matrix-vector multiplication
             * @param other operand
             * @throw IncompatibleMatrixSizeException when the matrices have incompatible size
             * @return new vector
             */
            Vector<T> operator*(const Vector<T> &other) const;

            /**
             * Performs matrix-scalar multiplication
             * @param other operand
             * @return new matrix
             */
            Matrix<T> operator*(const T &other) const;

            /**
             * Computes the transpose
             * @return new matrix transpose of this
             */
            Matrix<T> Transpose() const;

            /**
             * Swaps two rows of this matrix
             * @param rowa first row number
             * @param rowb second row number
             */
            void SwapRows(fsize rowa, fsize rowb);

            /**
             * Swaps two columns of this matrix
             * @param cola first column number
             * @param colb second column number
             */
            void SwapColumns(fsize cola, fsize colb);
        };

        template <typename T, fsize N, fsize M>
        const Matrix<T, N, M> Matrix<T, N, M>::Zero = Matrix<T, N, M>((T)0);

        template <typename T, fsize N>
        const Matrix<T, N, N> Matrix<T, N, N>::Identity = Matrix<T, N, N>::GenIdentity();
    }
}

#include "Framework/Math/Matrix.impl.hpp"

namespace bpf
{
    namespace math
    {
        template <typename T>
        using Matrix2 = Matrix<T, 2, 2>;
        template <typename T>
        using Matrix3 = Matrix<T, 3, 3>;
        template <typename T>
        using Matrix4 = Matrix<T, 4, 4>;
        using Matrix2f = Matrix2<float>;
        using Matrix3f = Matrix3<float>;
        using Matrix4f = Matrix4<float>;
    }
}
