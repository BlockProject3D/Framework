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
#include "Framework/Math/Math.hpp"
#include "Framework/Math/Matrix.hpp"
#include "Framework/Math/Vector.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Represents a quaternion
         * @tparam T the number type
         */
        template <typename T>
        class BP_TPL_API Quaternion
        {
        public:
            /**
             * Imaginary I component
             */
            T I;

            /**
             * Imaginary J component
             */
            T J;

            /**
             * Imaginary K component
             */
            T K;

            /**
             * Real component
             */
            T W;

            /**
             * Constructs an empty quaternion
             */
            inline Quaternion()
                : I(0)
                , J(0)
                , K(0)
                , W(1)
            {
            }

            /**
             * Constructs a quaternion from individual components
             * @param w real component
             * @param i imaginary I component
             * @param j imaginary J component
             * @param k imaginary K component
             */
            inline Quaternion(const T w, const T i, const T j, const T k)
                : I(i)
                , J(j)
                , K(k)
                , W(w)
            {
            }

            /**
             * Constructs a quaternion from euler angles
             * WARNING : Angles are expressed in radians
             * @param euler euler angles X = Pitch, Y = Yaw, Z = Roll
             */
            explicit Quaternion(const Vector<T, 3> &euler);

            /**
             * Constructs a quaternion from the rotation between two vectors
             * @param from first vector
             * @param to second vector
             */
            Quaternion(const Vector<T, 3> &from, const Vector<T, 3> &to);

            /**
             * Constructs a quaternion from axis angle
             * WARNING : Angle is expressed in radians
             * @param axis the rotation axis
             * @param ang the rotation angle
             */
            Quaternion(const Vector<T, 3> &axis, T ang);

            /**
             * Rotates the given vector by this quaternion
             * @param v the vector to rotate
             * @return new rotated vector
             */
            Vector<T, 3> Rotate(const Vector<T, 3> &v) const;

            /**
             * Computes an Up vector by rotating the Framework's default up vector by this quaternion
             * @return up vector from rotation of Vector3<T>::Up
             */
            inline Vector<T, 3> Up() const
            {
                return (Rotate(Vector3<T>::Up));
            }

            /**
             * Computes a Forward vector by rotating the Framework's default forward vector by this quaternion
             * @return forward vector from rotation of Vector3<T>::Forward
             */
            inline Vector<T, 3> Forward() const
            {
                return (Rotate(Vector3<T>::Forward));
            }

            /**
             * Computes a Right vector by rotating the Framework's default right vector by this quaternion
             * @return right vector from rotation of Vector3<T>::Right
             */
            inline Vector<T, 3> Right() const
            {
                return (Rotate(Vector3<T>::Right));
            }

            /**
             * Converts this quaternion to it's rotation matrix representation.
             * Result is undefined if this quaternion is not a unit quaternion
             * @return new matrix
             */
            Matrix<T, 4, 4> ToMatrix() const;

            /**
             * Computes the inverse of this quaternion
             * @return new quaternion
             */
            inline Quaternion<T> Inverse() const
            {
                return (Quaternion<T>(W, -I, -J, -K));
            }

            /**
             * Computes the norm of this quaternion
             * @return number
             */
            inline T Norm() const noexcept
            {
                return (Math<T>::Sqrt(W * W + I * I + J * J + K * K));
            }

            /**
             * Normalizes this quaternion to a unit quaternion
             * @return new unit quaternion
             */
            inline Quaternion<T> Normalize() const noexcept
            {
                T v = Norm();
                return (Quaternion<T>(W / v, I / v, J / v, K / v));
            }

            /**
             * Compare two quaternions
             * @param other quaternion to compare with
             * @return true if the two quaternions are equal, false otherwise
             */
            bool operator==(const Quaternion<T> &other) const;

            /**
             * Compare two quaternions
             * @param other quaternion to compare with
             * @return true if the two quaternions are not equal, false otherwise
             */
            inline bool operator!=(const Quaternion<T> &other) const
            {
                return (!operator==(other));
            }

            /**
             * Performs quaternion-quaternion multiplication
             * @param other operand
             * @return new quaternion
             */
            Quaternion<T> operator*(const Quaternion<T> &other) const;

            /**
             * Performs quaternion-scalar multiplication
             * @param other operand
             * @return new quaternion
             */
            inline Quaternion<T> operator*(const T &other) const
            {
                return (Quaternion<T>(W * other, I * other, J * other, K * other));
            }

            /**
             * Performs quaternion-quaternion addition
             * @param other operand
             * @return new quaternion
             */
            inline Quaternion<T> operator+(const Quaternion<T> &other) const
            {
                return (Quaternion<T>(W + other.W, I + other.I, J + other.J, K + other.K));
            }

            /**
             * Performs quaternion-quaternion subtraction
             * @param other operand
             * @return new quaternion
             */
            inline Quaternion<T> operator-(const Quaternion<T> &other) const
            {
                return (Quaternion<T>(W - other.W, I - other.I, J - other.J, K - other.K));
            }

            /**
             * Computes a dot product between two quaternions
             * @param other operand
             * @return number
             */
            T Dot(const Quaternion<T> &other) const;

            /**
             * Returns the angle between two quaternions.
             * WARNING : Angle is expressed in radians
             * @param other operand
             * @return number
             */
            T Angle(const Quaternion<T> &other) const;

            /**
             * Returns the euler angles representation of that quaternion.
             * WARNING : Angles are expressed in radians
             * @return 3D vector, X = Pitch, Y = Yaw, Z = Roll
             */
            Vector<T, 3> ToEulerAngles() const;

            /**
             * Performs linear interpolation between two quaternions
             * @param q first quaternion
             * @param q1 second quaternion
             * @param t interpolation factor
             * @return new quaternion
             */
            static Quaternion<T> Lerp(const Quaternion<T> &q, const Quaternion<T> &q1, T t);

            /**
             * Performs spherical linear interpolation between two quaternions
             * @param q first quaternion
             * @param q1 second quaternion
             * @param t interpolation factor
             * @return new quaternion
             */
            static Quaternion<T> Slerp(const Quaternion<T> &q, const Quaternion<T> &q1, T t);

            /**
             * Computes a quaternion to point a vector in a given direction
             * @param dir the direction to look at
             * @param forward the world's forward vector
             * @param up the world's up vector
             * @return new quaternion
             */
            static Quaternion<T> LookAt(const Vector3<T> &dir, const Vector3<T> &forward = Vector3<T>::Forward,
                                        const Vector3<T> &up = Vector3<T>::Up);

            /**
             * Zero quaternion constant
             */
            static const Quaternion<T> Zero;

            /**
             * Identity quaternion constant
             */
            static const Quaternion<T> Identity;
        };

        template <typename T>
        const Quaternion<T> Quaternion<T>::Zero = Quaternion<T>(0, 0, 0, 0);
        template <typename T>
        const Quaternion<T> Quaternion<T>::Identity = Quaternion<T>(1, 0, 0, 0);

        using Quaternionf = Quaternion<float>;
    }
}

#include "Framework/Math/Quaternion.impl.hpp"
