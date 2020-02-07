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
#include "Framework/Math/Math.hpp"
#include "Framework/Math/Vector.hpp"
#include "Framework/Math/Matrix.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API Quat
    {
    private:
        T X;
        T Y;
        T Z;
        T W;

    public:
        inline Quat()
            : X(0)
            , Y(0)
            , Z(0)
            , W(1)
        {
        }
        
        inline Quat(const T w, const T x, const T y, const T z)
            : X(x)
            , Y(y)
            , Z(z)
            , W(w)
        {
        }

        inline Quat(const Quat<T> &other)
            : X(other.X)
            , Y(other.Y)
            , Z(other.Z)
            , W(other.W)
        {
        }

        /**
         * Constructs a quaternion from euler angles
         * WARNING : Angles are expressed in radians
         * @param euler euler angles X = Pitch, Y = Yaw, Z = Roll
         */
        explicit Quat(const Vector3<T> &euler);

        /**
         * Constructs a quaternion from the rotation between two vectors
         * @param from first vector
         * @param to second vector
         */
        Quat(const Vector3<T> &from, const Vector3<T> &to);

        /**
         * Constructs a quaternion from axis angle
         * WARNING : Angle is expressed in radians
         */
        Quat(const Vector3<T> &axis, const T ang);

        /**
         * Rotates the given vector by this quaternion
         * @param v the vector to rotate
         */
        Vector3<T> Rotate(const Vector3<T> &v) const;

        inline Vector3<T> Up() const
        {
            return (Rotate(Vector3<T>::Up));
        }

        inline Vector3<T> Forward() const
        {
            return (Rotate(Vector3<T>::Forward));
        }

        inline Vector3<T> Right() const
        {
            return (Rotate(Vector3<T>::Right));
        }

        Matrix4<T> ToMatrix() const;

        inline Quat<T> Invert() const
        {
            return (Quat<T>(W, -X, -Y, -Z));
        }

        inline T Length() const
        {
            return (Math::Sqrt(W * W + X * X + Y * Y + Z * Z));
        }

        inline void Normalize()
        {
            T mag = Length();

            X /= mag;
            Y /= mag;
            Z /= mag;
            W /= mag;
        }

        bool operator==(const Quat<T> &other) const;
        
        inline Quat<T> &operator=(const Quat<T> &other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
            W = other.W;
            return (*this);
        }

        /**
         * Multiplies two quaternions, returns the cumulation of the rotation of this and other
         */
        Quat<T> operator*(const Quat<T> &other) const;

        inline Quat<T> operator+(const Quat<T> &other) const
        {
            return (Quat<T>(W + other.W, X + other.X, Y + other.Y, Z + other.Z));
        }

        /**
         * Returns a copy of this quaternion raised to the power of n
         */
        Quat<T> Pow(const T n) const;
        Quat<T> Exp() const;
        Quat<T> Ln() const;

        /**
         * Returns the angle in degrees between two quaternions
         * WARNING : Angle is expressed in radians
         */
        T Angle(const Quat<T> &other) const;

        /**
         * Returns the euler angles representation of that quaternion
         * WARNING : Angles are expressed in radians
         */
        Vector3<T> ToEulerAngles() const;

        /**
         * Returns the axis of that quaternion
         */
        inline Vector3<T> GetAxis() const
        {
            return (Vector3<T>(X, Y, Z));
        }

        /**
         * Returns the W component of that quaternion
         */
        inline T GetW() const
        {
            return (W);
        }

        /**
         * Linear Interpolation between two quaternions
         */
        static Quat<T> Lerp(const Quat<T> &q, const Quat<T> &q1, const T t);

        /**
         * Spherical linear interpolation between two quaternions
         * WARNING : produces garbage after short period of animation time
         * Never could figure why it did this
         * TODO : Figure out why Slerp does not work as intended, and fix it
         */
        static Quat<T> Slerp(const Quat<T> &q, const Quat<T> &q1, const T t);

        /**
         * Zero quaternion
         */
        static const Quat<T> Zero;

        /**
         * Identity quaternion
         */
        static const Quat<T> Identity;
    };

    template <typename T>
    const Quat<T> Quat<T>::Zero = Quat<T>(0, 0, 0, 0);
    template <typename T>
    const Quat<T> Quat<T>::Identity = Quat<T>(1, 0, 0, 0);
    
    using Quatf = Quat<float>;
}

#include "Framework/Math/Quaternion.impl.hpp"
