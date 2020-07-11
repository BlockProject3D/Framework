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
#include "Framework/Math/Vector.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Static 3D vector class
         * @tparam T the number type
         * @tparam I the dimension
         */
        template <typename T>
        class BP_TPL_API Vector<T, 3>
        {
        public:
            /**
             * X coordinate
             */
            T X;

            /**
             * Y coordinate
             */
            T Y;

            /**
             * Z coordinate
             */
            T Z;

            /**
             * Constructs a vector by individual coordinates
             * @param x coordinate
             * @param y coordinate
             * @param z coordinate
             */
            inline Vector(T x, T y, T z)
                : X(std::move(x))
                , Y(std::move(y))
                , Z(std::move(z))
            {
            }

            /**
             * Constructs a vector by extension of a lower-dimension vector
             * @param other the lower dimension vector
             * @param val the additional scalar
             */
            inline Vector(const Vector<T, 2> &other, const T val)
                : X(other.X)
                , Y(other.Y)
                , Z(val)
            {
            }

            /**
             * Constructs a empty vector
             */
            inline Vector()
                : X(T())
                , Y(T())
                , Z(T())
            {
            }

            /**
             * Constructs a vector from a scalar
             * @param val number to copy from
             */
            explicit inline Vector(const T val)
                : X(val)
                , Y(val)
                , Z(val)
            {
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
                : X(other.X)
                , Y(other.Y)
                , Z(other.Z)
            {
            }

            /**
             * Move constructor
             */
            inline Vector(Vector &&other) noexcept
                : X(std::move(other.X))
                , Y(std::move(other.Y))
                , Z(std::move(other.Z))
            {
            }

            /**
             * Returns the dimension of this vector
             * @return unsigned constant expression
             */
            constexpr inline fsize Dim() const noexcept
            {
                return (3);
            }

            /**
             * Obtains the value of a cell for modification
             * @param l the row number
             * @throw IndexException if out of bounds
             * @return mutable cell value
             */
            T &operator()(fsize l);

            /**
             * Obtains the value of a cell for reading
             * @param l the row number
             * @throw IndexException if out of bounds
             * @return immutable cell value
             */
            const T &operator()(fsize l) const;

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return immutable low-level C array
             */
            inline const T *operator*() const noexcept
            {
                return (&X);
            }

            /**
             * Returns a low-level C array, used to transfer to lower-level APIs
             * @return mutable low-level C array
             */
            inline T *operator*() noexcept
            {
                return (&X);
            }

            /**
             * Copy assignment operator
             */
            inline Vector &operator=(const Vector &other)
            {
                if (this == &other)
                    return (*this);
                X = other.X;
                Y = other.Y;
                Z = other.Z;
                return (*this);
            }

            /**
             * Move assignment operator
             */
            inline Vector &operator=(Vector &&other) noexcept
            {
                X = std::move(other.X);
                Y = std::move(other.Y);
                Z = std::move(other.Z);
                return (*this);
            }

            /**
             * Performs element-wise addition
             * @param other operand
             * @return new vector
             */
            inline Vector operator+(const Vector &other) const
            {
                return (Vector(X + other.X, Y + other.Y, Z + other.Z));
            }

            /**
             * Performs element-wise subtraction
             * @param other operand
             * @return new vector
             */
            inline Vector operator-(const Vector &other) const
            {
                return (Vector(X - other.X, Y - other.Y, Z - other.Z));
            }

            /**
             * Performs element-wise multiplication
             * @param other operand
             * @return new vector
             */
            inline Vector operator*(const Vector &other) const
            {
                return (Vector(X * other.X, Y * other.Y, Z * other.Z));
            }

            /**
             * Performs element-wise division
             * @param other operand
             * @return new vector
             */
            inline Vector operator/(const Vector &other) const
            {
                return (Vector(X / other.X, Y / other.Y, Z / other.Z));
            }

            /**
             * Performs vector-scalar multiplication
             * @param other operand
             * @return new vector
             */
            inline Vector operator*(const T other) const
            {
                return (Vector(X * other, Y * other, Z * other));
            }

            /**
             * Performs vector-scalar division
             * @param other operand
             * @return new vector
             */
            inline Vector operator/(const T other) const
            {
                return (Vector(X / other, Y / other, Z / other));
            }

            /**
             * Performs element-wise addition
             * @param other operand
             */
            inline void operator+=(const Vector &other)
            {
                X += other.X;
                Y += other.Y;
                Z += other.Z;
            }

            /**
             * Performs element-wise subtraction
             * @param other operand
             */
            inline void operator-=(const Vector &other)
            {
                X -= other.X;
                Y -= other.Y;
                Z -= other.Z;
            }

            /**
             * Performs element-wise multiplication
             * @param other operand
             */
            inline void operator*=(const Vector &other)
            {
                X *= other.X;
                Y *= other.Y;
                Z *= other.Z;
            }

            /**
             * Performs element-wise division
             * @param other operand
             */
            inline void operator/=(const Vector &other)
            {
                X /= other.X;
                Y /= other.Y;
                Z /= other.Z;
            }

            /**
             * Performs vector-scalar multiplication
             * @param other operand
             */
            inline void operator*=(const T other)
            {
                X *= other;
                Y *= other;
                Z *= other;
            }

            /**
             * Performs vector-scalar division
             * @param other operand
             */
            inline void operator/=(const T other)
            {
                X /= other;
                Y /= other;
                Z /= other;
            }

            /**
             * Negates this vector
             * @return negated vector
             */
            inline Vector operator-() const
            {
                return (Vector(-X, -Y, -Z));
            }

            /**
             * Computes the dot product
             * @param other operand
             * @return number
             */
            inline T Dot(const Vector &other) const
            {
                return (X * other.X + Y * other.Y + Z * other.Z);
            }

            /**
             * Computes the distance between two vectors
             * @param other operand
             * @return number
             */
            inline T Distance(const Vector &other) const
            {
                auto dist = other - *this;
                return (dist.Norm());
            }

            /**
             * Computes the squared distance between two vectors
             * @param other operand
             * @return number
             */
            inline T DistanceSquared(const Vector &other) const
            {
                auto dist = other - *this;
                return (dist.NormSquared());
            }

            /**
             * Computes the norm of this vector
             * @return number
             */
            inline T Norm() const
            {
                return (Math<T>::Sqrt(X * X + Y * Y + Z * Z));
            }

            /**
             * Computes the squared norm of this vector
             * @return number
             */
            inline T NormSquared() const
            {
                return (X * X + Y * Y + Z * Z);
            }

            /**
             * Normalizes this vector (by dividing each component by Norm())
             * @return normalized vector
             */
            inline Vector Normalize() const
            {
                auto v = Norm();
                return (Vector(X / v, Y / v, Z / v));
            }

            /**
             * Performs cross product
             * @param other operand
             * @return new vector
             */
            inline Vector Cross(const Vector &other) const
            {
                return (Vector(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X));
            }

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if the two vectors are equal, false otherwise
             */
            inline bool operator==(const Vector &other) const
            {
                T diffx = Math<T>::Abs(X - other.X);
                T diffy = Math<T>::Abs(Y - other.Y);
                T diffz = Math<T>::Abs(Z - other.Z);

                return (diffx <= Math<T>::Epsilon && diffy <= Math<T>::Epsilon && diffz <= Math<T>::Epsilon);
            }

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
            inline bool operator<(const Vector &other) const
            {
                return (X < other.X && Y < other.Y && Z < other.Z);
            }

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if this is greater than other, false otherwise
             */
            inline bool operator>(const Vector &other) const
            {
                return (X > other.X &&Y > other.Y && Z > other.Z);
            }

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if this is less or equal than other, false otherwise
             */
            inline bool operator<=(const Vector &other) const
            {
                return (X <= other.X && Y <= other.Y && Z <= other.Z);
            }

            /**
             * Compare two vectors
             * @param other vector to compare with
             * @return true if this is greater or equal than other, false otherwise
             */
            inline bool operator>=(const Vector &other) const
            {
                return (X >= other.X && Y >= other.Y && Z >= other.Z);
            }

            /**
             * Performs linear interpolation
             * @param v first vector
             * @param v1 second vector
             * @param t interpolation factor
             * @return new interpolated vector
             */
            inline static Vector Lerp(const Vector &v, const Vector &v1, const T t)
            {
                return (Vector(Math<T>::Lerp(v.X, v1.X, t), Math<T>::Lerp(v.Y, v1.Y, t), Math<T>::Lerp(v.Z, v1.Z, t)));
            }

            /**
             * Zero vector constant
             */
            static const Vector Zero;

            /**
             * Identity vector constant (1, 1, 1)
             */
            static const Vector Identity;

            /**
             * Right vector constant
             */
            static const Vector Right;

            /**
             * Up vector constant
             */
            static const Vector Up;

            /**
             * Forward vector constant
             */
            static const Vector Forward;
        };

        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Zero = Vector((T)0);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Identity = Vector((T)1);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Right = Vector((T)1, (T)0, (T)0);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Up = Vector((T)0, (T)0, (T)1);
        template <typename T>
        const Vector<T, 3> Vector<T, 3>::Forward = Vector((T)0, (T)1, (T)0);
    }
}

#include "Framework/Math/Vector3.impl.hpp"
