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
            I = sr * cp * cy - cr * sp * sy;
            J = cr * sp * cy + sr * cp * sy;
            K = cr * cp * sy - sr * sp * cy;
        }

        template <typename T>
        Quaternion<T>::Quaternion(const Vector<T, 3> &from, const Vector<T, 3> &to)
        {
            Vector3<T> a = from.Cross(to);

            I = a.X;
            J = a.Y;
            K = a.Z;
            W = Math<T>::Sqrt((from.Norm() * from.Norm()) * (to.Norm() * to.Norm())) + from.Dot(to);
            T norm = Norm();
            I /= norm;
            J /= norm;
            K /= norm;
            W /= norm;
        }

        template <typename T>
        Quaternion<T>::Quaternion(const Vector<T, 3> &axis, const T ang)
        {
            //Not using initializer to allow performance improvement
            T v = Math<T>::Sin(ang) / (T)2;
            I = axis.X * v;
            J = axis.Y * v;
            K = axis.Z * v;
            W = Math<T>::Cos((ang / (T)2));
        }

        template <typename T>
        T Quaternion<T>::Dot(const Quaternion<T> &other) const
        {
            return (I * other.I + J * other.J + K * other.K + W * other.W);
        }

        template <typename T>
        Vector<T, 3> Quaternion<T>::Rotate(const Vector<T, 3> &v) const
        {
            Quaternion<T> p(0, v.X, v.Y, v.Z);
            Quaternion<T> pprime = Inverse() * p * *this;

            return (Vector<T, 3>(pprime.I, pprime.J, pprime.K));
        }

        template <typename T>
        Matrix<T, 4, 4> Quaternion<T>::ToMatrix() const
        {
            Matrix<T, 4, 4> mat = {
                W * W + I * I - J * J - K * K, 2 * I * J - 2 * W * K, 2 * I * K + 2 * W * J, 0,
                2 * I * J + 2 * W * K, W * W - I * I + J * J - K * K, 2 * J * K - 2 * W * I, 0,
                2 * I * K - 2 * W * J, 2 * J * K + 2 * W * I, W * W - I * I - J * J + K * K, 0,
                0, 0, 0, 1
            };

            return (mat);
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::operator*(const Quaternion<T> &other) const
        {
            Quaternion<T> res;

            res.W = W * other.W - I * other.I - J * other.J - K * other.K;
            res.I = W * other.I + I * other.W + J * other.K - K * other.J;
            res.J = W * other.J + J * other.W + K * other.I - I * other.K;
            res.K = W * other.K + K * other.W + I * other.J - J * other.I;
            return (res);
        }

        template <typename T>
        bool Quaternion<T>::operator==(const Quaternion<T> &other) const
        {
            T diffx = Math<T>::Abs(I - other.I);
            T diffy = Math<T>::Abs(J - other.J);
            T diffz = Math<T>::Abs(K - other.K);
            T diffw = Math<T>::Abs(W - other.W);

            return (diffx <= Math<T>::Epsilon && diffy <= Math<T>::Epsilon
                && diffz <= Math<T>::Epsilon && diffw <= Math<T>::Epsilon);
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
            T roll = Math<T>::ArcTan2(2 * J * W + 2 * I * K, 1 - 2 * J * J - 2 * K * K);
            T pitch = Math<T>::ArcTan2(2 * I * W + 2 * J * K, 1 - 2 * I * I - 2 * K * K);
            T yaw = Math<T>::ArcSin(2 * I * J + 2 * K * W);

            return (Vector<T, 3>(pitch, yaw, roll));
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T> &q, const Quaternion<T> &q1, const T t)
        {
            Quaternion<T> res(Math<T>::Lerp(q.W, q1.W, t), Math<T>::Lerp(q.I, q1.I, t),
                Math<T>::Lerp(q.J, q1.J, t), Math<T>::Lerp(q.K, q1.K, t));

            return (res);
        }

        template <typename T>
        Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T> &q, const Quaternion<T> &q1, const T t)
        {
            auto quat = q;
            T dot = q.Dot(q1);
            if (dot < T(0)) {
                quat = quat.Inverse();
                dot = -dot;
            }
            if (dot > (T)0.9995)
            {
                return ((q + (q1 - q) * t).Normalize());
            }
            T theta0 = Math<T>::ArcCos(dot);
            T theta = theta0 * t;
            T stheta = Math<T>::Sin(theta);
            T stheta0 = Math<T>::Sin(theta0);
            T s0 = Math<T>::Cos(theta) - dot * stheta / stheta0;
            T s1 = stheta / stheta0;
            return ((q * s0) + (q1 * s1)));
        }
    }
}
