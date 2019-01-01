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
        Quat(const Vector3<T> &euler);

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
}

#include "Framework/Math/Quaternion.impl.hpp"

namespace Framework
{
    class ENGINE_API FQuat
    {
    private:
        float X;
        float Y;
        float Z;
        float W;
    public:
        /**
         * Constructs an identity quaternion (zero rotation)
         */
        FQuat();
      
        /**
         * Constructs a quaternion by specifying directly components
         */
        FQuat(const float w, const float x, const float y, const float z);

        /**
         * Constructs a quaternion from euler angles (X Pitch, Y Yaw, Z Roll)
         */
        FQuat(const FVector &eulerangles);

        /**
         * Constructs a quaternion from shortest arc between vector from and to
         */
        FQuat(const FVector &from, const FVector &to);
      
        /**
         * Constructs a quaternion from an axis and a rotation around that axis
         */
        FQuat(const FVector &axis, const float rot);
      
        FQuat(const FQuat &other);
      
        /**
         * Returns the Up pointing vector of that quaternion
         */
        inline FVector Up() const
        {
            return (FVector::Up * *this);
            //return (FVector(-2 * (X * Y - W * Z), 1 - 2 * (X * X + Z * Z), 2 * (Y * Z + W * X)));
        }
      
        /**
         * Returns the forward pointing vector of that quaternion
         */
        inline FVector Forward() const
        {
            return (FVector::Forward * *this);
            //return (FVector(2 * (X * Z - W * Y), 2 * (Y * Z + W * X), 1 - 2 * (X * X + Y * Y)));
        }
      
        /**
         * Returns the right pointing vector of that quaternion
         */
        inline FVector Right() const
        {
            return (FVector::Right * *this);
            //return (FVector(1 - 2 * (Y * Y + Z * Z), 2 * (Y * Z - W * X), 2 * (X * Z + W * Y)));
        }

        /**
         * Returns a rotation matrix from that quaternion
         */
        FMatrix ToMatrix() const;
      
        /**
         * Conjugates that quaternions, that means invert it
         */
        inline FQuat Conjugate() const
        {
            return (FQuat(W, -X, -Y, -Z));
        }
      
        /**
         * Returns true if this quaternion is equal to other
         */
        bool operator==(const FQuat &other) const;
      
        /**
         * Normalizes this quaternion
         */
        void Normalize();
      
        FQuat &operator=(const FQuat &other);

        /**
         * Multiplies two quaternions, returns the cumulation of the rotation of this and other
         */
        FQuat operator*(const FQuat &other) const;

        /**
         * Returns a copy of this quaternion raised to the power of n
         */
        FQuat operator*(const float n) const;
      
        FQuat Exp() const;
        FQuat Ln() const;
      
        /**
         * Returns the angle in degrees between two quaternions
         */
        float Angle(const FQuat &other) const;
      
        /**
         * Returns the euler angles representation of that quaternion
         */
        FVector ToEulerAngles() const;

        /**
         * Returns the axis of that quaternion
         */
        inline FVector GetAxis() const
        {
            return (FVector(X, Y, Z));
        }

        /**
         * Returns the W component of that quaternion
         */
        inline float GetW() const
        {
            return (W);
        }
        
        /**
         * Linear Interpolation between two quaternions
         */
        static FQuat Lerp(const FQuat &q, const FQuat &q1, const float t);
      
        /**
         * Spherical linear interpolation between two quaternions (WARNING : produces garbage after short period of animation time)
         */
        static FQuat Slerp(const FQuat &q, const FQuat &q1, const float t);

        /**
         * Zero quaternion
         */
        static const FQuat	Zero;
        
        /**
         * Identity quaternion
         */
        static const FQuat	Identity;
    };
};
