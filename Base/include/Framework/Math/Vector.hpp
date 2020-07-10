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
#include "Framework/Math/Math.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/Types.hpp"
#include <initializer_list>
#include <utility>

#include "Framework/Memory/MemUtils.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Static vector class
         * @tparam T the number type
         * @tparam I the dimension
         */
        template <typename T, fsize I = 0>
        class BP_TPL_API Vector
        {
        private:
            T _arr[I];

        public:
            /**
             * Zero vector constant
             */
            static const Vector Zero;

            /**
             * Identity vector constant (1, ...)
             */
            static const Vector Identity;

            /**
             * Constructs a empty vector
             */
            inline Vector()
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = T();
            }

            /**
             * Constructs a vector from a scalar
             * @param val number to copy from
             */
            explicit inline Vector(const T val)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = val;
            }

            /**
             * Constructs a vector by extension of a lower-dimension vector
             * @param other the lower dimension vector
             * @param val the additional scalar
             */
            inline Vector(const Vector<T, I - 1> &other, const T val)
            {
                for (fsize i = 0; i != I - 1; ++i)
                    _arr[i] = other(i);
                _arr[I - 1] = val;
            }

            /**
             * Constructs a vector from an initializer list
             * @param lst the list to copy values from
             */
            Vector(const std::initializer_list<T> &lst);

            /**
             * Copy constructor
             */
            inline Vector(const Vector &other)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = other._arr[i];
            }

            /**
             * Move constructor
             */
            inline Vector(Vector &&other) noexcept
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = std::move(other._arr[i]);
            }

            /**
             * Returns the dimension of this vector
             * @return unsigned constant expression
             */
            constexpr inline fsize Dim() const noexcept
            {
                return (I);
            }

            /**
             * Obtains the value of a cell for modification
             * @param l the row number
             * @param c the column number
             * @throw IndexException if out of bounds
             * @return mutable cell value
             */
            inline T &operator()(const fsize l)
            {
                if (l >= I)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            /**
             * Obtains the value of a cell for reading
             * @param l the row number
             * @param c the column number
             * @throw IndexException if out of bounds
             * @return immutable cell value
             */
            inline const T &operator()(const fsize l) const
            {
                if (l >= I)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            /**
             * Copy assignment operator
             */
            Vector &operator=(const Vector &other);

            /**
             * Move assignment operator
             */
            Vector &operator=(Vector &&other) noexcept;

            /**
             * Performs element-wise addition
             * @param other operand
             * @return new vector
             */
            Vector operator+(const Vector &other) const;

            /**
             * Performs element-wise subtraction
             * @param other operand
             * @return new vector
             */
            Vector operator-(const Vector &other) const;

            /**
             * Performs element-wise multiplication
             * @param other operand
             * @return new vector
             */
            Vector operator*(const Vector &other) const;

            /**
             * Performs element-wise division
             * @param other operand
             * @return new vector
             */
            Vector operator/(const Vector &other) const;

            /**
             * Performs vector-scalar multiplication
             * @param other operand
             * @return new vector
             */
            Vector operator*(T other) const;

            /**
             * Performs vector-scalar division
             * @param other operand
             * @return new vector
             */
            Vector operator/(T other) const;

            /**
             * Performs element-wise addition
             * @param other operand
             */
            void operator+=(const Vector &other);

            /**
             * Performs element-wise subtraction
             * @param other operand
             */
            void operator-=(const Vector &other);

            /**
             * Performs element-wise multiplication
             * @param other operand
             */
            void operator*=(const Vector &other);

            /**
             * Performs element-wise division
             * @param other operand
             */
            void operator/=(const Vector &other);

            /**
             * Performs vector-scalar multiplication
             * @param other operand
             */
            void operator*=(T other);

            /**
             * Performs vector-scalar division
             * @param other operand
             */
            void operator/=(T other);

            /**
             * Negates this vector
             * @return negated vector
             */
            Vector operator-() const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if the two vectors are equal, false otherwise
             */
            bool operator==(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return false if the two vectors are equal, true otherwise
             */
            inline bool operator!=(const Vector &other) const
            {
                return (!operator==(other));
            }

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if this is less than other, false otherwise
             */
            bool operator<(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if this is greater than other, false otherwise
             */
            bool operator>(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if this is less or equal than other, false otherwise
             */
            bool operator<=(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if this is greater or equal than other, false otherwise
             */
            bool operator>=(const Vector &other) const;

            /**
             * Computes the dot product
             * @param other operand
             * @return number
             */
            T Dot(const Vector &other) const;

            /**
             * Computes the distance between two vectors
             * @param other operand
             * @return number
             */
            T Distance(const Vector &other) const;

            /**
             * Computes the squared distance between two vectors
             * @param other operand
             * @return number
             */
            T DistanceSquared(const Vector &other) const;

            /**
             * Computes the norm of this vector
             * @return number
             */
            T Norm() const;

            /**
             * Computes the squared norm of this vector
             * @return number
             */
            T NormSquared() const;

            /**
             * Normalizes this vector (by dividing each component by Norm())
             * @return normalized vector
             */
            Vector Normalize() const;

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
             * Performs linear interpolation
             * @param v first vector
             * @param v1 second vector
             * @param t interpolation factor
             * @return new interpolated vector
             */
            static Vector Lerp(const Vector &v, const Vector &v1, T t);
        };

        /**
         * Dynamic vector class
         * @tparam T the number type
         * @tparam I the dimension
         */
        template <typename T>
        class BP_TPL_API Vector<T, 0>
        {
        private:
            T *_arr;
            fsize _l;

        public:
            /**
             * Generates a zero vector
             * @param l vector dimension
             * @return new vector
             */
            inline static Vector Zero(const fsize l)
            {
                return (Vector(l, (T)0));
            }

            /**
             * Generates an identity vector (1, ...)
             * @param l vector dimension
             * @return new vector
             */
            inline static Vector Identity(const fsize l)
            {
                return (Vector(l, (T)1));
            }

            /**
             * Constructs a vector by extension of a lower-dimension vector
             * @param other the lower dimension vector
             * @param val the additional scalar
             */
            inline Vector(const Vector &other, const T val)
                : _arr(memory::MemUtils::NewArray<T>(other._l + 1))
                , _l(other._l + 1)
            {
                for (fsize i = 0; i != other._l; ++i)
                    _arr[i] = other(i);
                _arr[_l - 1] = val;
            }

            /**
             * Constructs a empty vector
             * @param l vector dimension
             */
            explicit inline Vector(const fsize l)
                : _arr(memory::MemUtils::NewArray<T>(l))
                , _l(l)
            {
                for (fsize i = 0; i != l; ++i)
                    _arr[i] = T();
            }

            /**
             * Constructs a vector from a scalar
             * @param l vector dimension
             * @param val number to copy from
             */
            inline Vector(const fsize l, const T val)
                : _arr(memory::MemUtils::NewArray<T>(l))
                , _l(l)
            {
                for (fsize i = 0; i != l; ++i)
                    _arr[i] = val;
            }

            /**
             * Constructs a vector from an initializer list
             * @param lst the list to copy values from
             */
            Vector(const std::initializer_list<T> &lst);

            /**
             * Copy constructor
             */
            inline Vector(const Vector &other)
                : _arr(memory::MemUtils::NewArray<T>(other._l))
                , _l(other._l)
            {
                for (fsize i = 0; i != _l; ++i)
                    _arr[i] = other._arr[i];
            }

            /**
             * Move constructor
             */
            inline Vector(Vector &&other) noexcept
                : _arr(other._arr)
                , _l(other._l)
            {
                other._arr = Null;
                other._l = 0;
            }

            inline ~Vector()
            {
                memory::MemUtils::DeleteArray(_arr, _l);
            }

            /**
             * Returns the dimension of this vector
             * @return unsigned
             */
            inline fsize Dim() const noexcept
            {
                return (_l);
            }

            /**
             * Obtains the value of a cell for modification
             * @param l the row number
             * @param c the column number
             * @throw IndexException if out of bounds
             * @return mutable cell value
             */
            inline T &operator()(const fsize l)
            {
                if (l >= _l)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            /**
             * Obtains the value of a cell for reading
             * @param l the row number
             * @param c the column number
             * @throw IndexException if out of bounds
             * @return immutable cell value
             */
            inline const T &operator()(const fsize l) const
            {
                if (l >= _l)
                    throw IndexException((fisize)l);
                return (_arr[l]);
            }

            /**
             * Copy assignment operator
             */
            Vector &operator=(const Vector &other);

            /**
             * Move assignment operator
             */
            Vector &operator=(Vector &&other) noexcept;

            /**
             * Performs element-wise addition
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return new vector
             */
            Vector operator+(const Vector &other) const;

            /**
             * Performs element-wise subtraction
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return new vector
             */
            Vector operator-(const Vector &other) const;

            /**
             * Performs element-wise multiplication
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return new vector
             */
            Vector operator*(const Vector &other) const;

            /**
             * Performs element-wise division
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return new vector
             */
            Vector operator/(const Vector &other) const;

            /**
             * Performs vector-scalar multiplication
             * @param other operand
             * @return new vector
             */
            Vector operator*(T other) const;

            /**
             * Performs vector-scalar division
             * @param other operand
             * @return new vector
             */
            Vector operator/(T other) const;

            /**
             * Performs element-wise addition
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             */
            void operator+=(const Vector &other);

            /**
             * Performs element-wise subtraction
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             */
            void operator-=(const Vector &other);

            /**
             * Performs element-wise multiplication
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             */
            void operator*=(const Vector &other);

            /**
             * Performs element-wise division
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             */
            void operator/=(const Vector &other);

            /**
             * Performs vector-scalar multiplication
             * @param other operand
             */
            void operator*=(T other);

            /**
             * Performs vector-scalar division
             * @param other operand
             */
            void operator/=(T other);

            /**
             * Negates this vector
             * @return negated vector
             */
            Vector operator-() const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if the two vectors are equal, false otherwise
             */
            bool operator==(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return false if the two vectors are equal, true otherwise
             */
            inline bool operator!=(const Vector &other) const
            {
                return (!operator==(other));
            }

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return true if this is less than other, false otherwise
             */
            bool operator<(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return true if this is greater than other, false otherwise
             */
            bool operator>(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return true if this is less or equal than other, false otherwise
             */
            bool operator<=(const Vector &other) const;

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return true if this is greater or equal than other, false otherwise
             */
            bool operator>=(const Vector &other) const;

            /**
             * Computes the dot product
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return number
             */
            T Dot(const Vector &other) const;

            /**
             * Computes the distance between two vectors
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return number
             */
            T Distance(const Vector &other) const;

            /**
             * Computes the squared distance between two vectors
             * @param other operand
             * @throw IncompatibleMatrixSizeException if the two vector dimensions do not match
             * @return number
             */
            T DistanceSquared(const Vector &other) const;

            /**
             * Computes the norm of this vector
             * @return number
             */
            T Norm() const;

            /**
             * Computes the squared norm of this vector
             * @return number
             */
            T NormSquared() const;

            /**
             * Normalizes this vector (by dividing each component by Norm())
             * @return normalized vector
             */
            Vector Normalize() const;

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
             * Performs linear interpolation
             * @param v first vector
             * @param v1 second vector
             * @param t interpolation factor
             * @return new interpolated vector
             */
            static Vector Lerp(const Vector &v, const Vector &v1, T t);
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
