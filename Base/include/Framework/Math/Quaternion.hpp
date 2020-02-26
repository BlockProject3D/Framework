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
    class BP_TPL_API Quaternion
    {
    private:
        T X;
        T Y;
        T Z;
        T W;

    public:
        inline Quaternion()
            : X(0)
            , Y(0)
            , Z(0)
            , W(1)
        {
        }
        
        inline Quaternion(const T w, const T x, const T y, const T z)
            : X(x)
            , Y(y)
            , Z(z)
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
         */
        Quaternion(const Vector<T, 3> &axis, const T ang);

        /**
         * Rotates the given vector by this quaternion
         * @param v the vector to rotate
         */
        Vector<T, 3> Rotate(const Vector<T, 3> &v) const;

        inline Vector<T, 3> Up() const
        {
            return (Rotate(Vector3<T>::Up));
        }

        inline Vector<T, 3> Forward() const
        {
            return (Rotate(Vector3<T>::Forward));
        }

        inline Vector<T, 3> Right() const
        {
            return (Rotate(Vector3<T>::Right));
        }

        Matrix<T, 4, 4> ToMatrix() const;

        inline Quaternion<T> Invert() const
        {
            return (Quaternion<T>(W, -X, -Y, -Z));
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

        bool operator==(const Quaternion<T> &other) const;

        /**
         * Multiplies two quaternions, returns the cumulation of the rotation of this and other
         */
        Quaternion<T> operator*(const Quaternion<T> &other) const;

        inline Quaternion<T> operator+(const Quaternion<T> &other) const
        {
            return (Quaternion<T>(W + other.W, X + other.X, Y + other.Y, Z + other.Z));
        }

        /**
         * Returns a copy of this quaternion raised to the power of n
         */
        Quaternion<T> Pow(const T n) const;
        Quaternion<T> Exp() const;
        Quaternion<T> Log() const;

        /**
         * Returns the angle in degrees between two quaternions
         * WARNING : Angle is expressed in radians
         */
        T Angle(const Quaternion<T> &other) const;

        /**
         * Returns the euler angles representation of that quaternion
         * WARNING : Angles are expressed in radians
         */
        Vector<T, 3> ToEulerAngles() const;

        /**
         * Returns the axis of that quaternion
         */
        inline Vector<T, 3> GetAxis() const
        {
            return (Vector<T, 3>(X, Y, Z));
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
        static Quaternion<T> Lerp(const Quaternion<T> &q, const Quaternion<T> &q1, const T t);

        /**
         * Spherical linear interpolation between two quaternions
         * WARNING : produces garbage after short period of animation time
         * Never could figure why it did this
         * TODO : Figure out why Slerp does not work as intended, and fix it
         */
        static Quaternion<T> Slerp(const Quaternion<T> &q, const Quaternion<T> &q1, const T t);

        /**
         * Zero quaternion
         */
        static const Quaternion<T> Zero;

        /**
         * Identity quaternion
         */
        static const Quaternion<T> Identity;
    };

    template <typename T>
    const Quaternion<T> Quaternion<T>::Zero = Quaternion<T>(0, 0, 0, 0);
    template <typename T>
    const Quaternion<T> Quaternion<T>::Identity = Quaternion<T>(1, 0, 0, 0);
    
    using Quaternionf = Quaternion<float>;
}

#include "Framework/Math/Quaternion.impl.hpp"
