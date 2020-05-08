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

namespace bpf
{
    namespace math
    {
        template <typename T>
        Quaternion<T>::Quaternion(const Vector<T, 3> &euler)
        {
            T cr = Math<T>::Cos(euler.X / (T)2);
            T cp = Math<T>::Cos(euler.Y / (T)2);
            T cy = Math<T>::Cos(euler.Z / (T)2);
            T sr = Math<T>::Sin(euler.X / (T)2);
            T sp = Math<T>::Sin(euler.Y / (T)2);
            T sy = Math<T>::Sin(euler.Z / (T)2);

            W = cr * cp * cy + sr * sp * sy;
            X = sr * cp * cy - cr * sp * sy;
            Y = cr * sp * cy + sr * cp * sy;
            Z = cr * cp * sy - sr * sp * cy;
        }

        template <typename T>
        Quaternion<T>::Quaternion(const Vector<T, 3> &from, const Vector<T, 3> &to)
        {
            Vector3<T> a = from.Cross(to);

            X = a.X;
            Y = a.Y;
            Z = a.Z;
            W = Math<T>::Sqrt((from.Norm() * from.Norm()) * (to.Norm() * to.Norm())) + from.Dot(to);
        }

        template <typename T>
        Quaternion<T>::Quaternion(const Vector<T, 3> &axis, const T ang)
            : X(axis.X *Math<T>::Sin((ang / (T)2)))
            , Y(axis.Y *Math<T>::Sin((ang / (T)2)))
            , Z(axis.Z *Math<T>::Sin((ang / (T)2)))
            , W(Math<T>::Cos((ang / (T)2)))
        {
            Normalize();
        }

        template <typename T>
        Vector<T, 3> Quaternion<T>::Rotate(const Vector<T, 3> &v) const
        {
            Quaternion<T> p(0, v.X, v.Y, v.Z);
            Quaternion<T> pprime = Inverse() * p * *this;

            return (Vector<T, 3>(pprime.X, pprime.Y, pprime.Z));
        }

        template <typename T>
        Matrix<T, 4, 4> Quaternion<T>::ToMatrix() const
        {
            Matrix<T, 4, 4> mat = {
                W * W + X * X - Y * Y - Z * Z, 2 * X * Y - 2 * W * Z, 2 * X * Z + 2 * W * Y, 0,
                2 * X * Y + 2 * W * Z, W * W - X * X + Y * Y - Z * Z, 2 * Y * Z - 2 * W * X, 0,
                2 * X * Z - 2 * W * Y, 2 * Y * Z + 2 * W * X, W * W - X * X - Y * Y + Z * Z, 0,
                0, 0, 0, 1
            };

            return (mat);
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::operator*(const Quaternion<T> &other) const
        {
            Quaternion<T> res;

            res.W = W * other.W - X * other.X - Y * other.Y - Z * other.Z;
            res.X = W * other.X + X * other.W + Y * other.Z - Z * other.Y;
            res.Y = W * other.Y + Y * other.W + Z * other.X - X * other.Z;
            res.Z = W * other.Z + Z * other.W + X * other.Y - Y * other.X;
            return (res);
        }

        template <typename T>
        bool Quaternion<T>::operator==(const Quaternion<T> &other) const
        {
            T diffx = Math<T>::Abs(X - other.X);
            T diffy = Math<T>::Abs(Y - other.Y);
            T diffz = Math<T>::Abs(Z - other.Z);
            T diffw = Math<T>::Abs(W - other.W);

            return (diffx <= Math<T>::Epsilon && diffy <= Math<T>::Epsilon
                && diffz <= Math<T>::Epsilon && diffw <= Math<T>::Epsilon);
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::Pow(const T n) const
        {
            Quaternion<T> q = Log();

            q.W *= n;
            q.X *= n;
            q.Y *= n;
            q.Z *= n;
            return (q.Exp());
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::Exp() const
        {
            T r = Math<T>::Sqrt(X * X + Y * Y + Z * Z);
            T et = Math<T>::Exp(W);
            T s = r >= (T)0.00001 ? et * Math<T>::Sin(r) / r : 0;

            return (Quaternion<T>(et * Math<T>::Cos(r), X * s, Y * s, Z * s));
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::Log() const
        {
            T r = Math<T>::Sqrt(X * X + Y * Y + Z * Z);
            T et = r > (T)0.00001 ? Math<T>::ArcTan2(r, W) / r : 0;

            return (Quaternion<T>((T)0.5 * Math<T>::Log(W * W + X * X + Y * Y + Z * Z), X * et, Y * et, Z * et));
        }

        template <typename T>
        T Quaternion<T>::Angle(const Quaternion<T> &other) const
        {
            Quaternion<T> q = other * Inverse();
            T a = Math<T>::ArcCos(q.W) * 2;

            return (a > Math<T>::Pi ? Math<T>::TwoPi - a : a);
        }

        template <typename T>
        Vector<T, 3> Quaternion<T>::ToEulerAngles() const
        {
            T roll = Math<T>::ArcTan2(2 * Y * W + 2 * X * Z, 1 - 2 * Y * Y - 2 * Z * Z);
            T pitch = Math<T>::ArcTan2(2 * X * W + 2 * Y * Z, 1 - 2 * X * X - 2 * Z * Z);
            T yaw = Math<T>::ArcSin(2 * X * Y + 2 * Z * W);

            return (Vector<T, 3>(pitch, yaw, roll));
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T> &q, const Quaternion<T> &q1, const T t)
        {
            Quaternion<T> res(Math<T>::Lerp(q.W, q1.W, t), Math<T>::Lerp(q.X, q1.X, t),
                Math<T>::Lerp(q.Y, q1.Y, t), Math<T>::Lerp(q.Z, q1.Z, t));

            return (res);
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T> &q, const Quaternion<T> &q1, const T t)
        {
            Quaternion<T> res = ((q1 * q.Inverse()).Pow(t)) * q;

            return (res);
        }
    }
}
