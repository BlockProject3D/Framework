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
    template <typename T>
    Quat<T>::Quat(const Vector3<T> &euler)
    {
        T cr = Math::Cos(euler.X / (T)2);
        T cp = Math::Cos(euler.Y / (T)2);
        T cy = Math::Cos(euler.Z / (T)2);
        T sr = Math::Sin(euler.X / (T)2);
        T sp = Math::Sin(euler.Y / (T)2);
        T sy = Math::Sin(euler.Z / (T)2);

        W = cr * cp * cy + sr * sp * sy;
        X = sr * cp * cy - cr * sp * sy;
        Y = cr * sp * cy + sr * cp * sy;
        Z = cr * cp * sy - sr * sp * cy;
    }

    template <typename T>
    Quat<T>::Quat(const Vector3<T> &from, const Vector3<T> &to)
    {
        Vector3<T> a = from.Cross(to);

        X = a.X;
        Y = a.Y;
        Z = a.Z;
        W = Math::Sqrt((from.Length() * from.Length()) * (to.Length() * to.Length())) + from.Dot(to);
    }

    template <typename T>
    Quat<T>::Quat(const Vector3<T> &axis, const T ang)
        : X(axis.X * Math::Sin((ang / (T)2)))
        , Y(axis.Y * Math::Sin((ang / (T)2)))
        , Z(axis.Z * Math::Sin((ang / (T)2)))
        , W(Math::Cos((ang / (T)2)))
    {
        Normalize();
    }

    template <typename T>
    Vector3<T> Quat<T>::Rotate(const Vector3<T> &v) const
    {
        Quat<T> p(0, X, Y, Z);
        Quat<T> pprime = Invert() * p * *this;

        return (pprime.GetAxis());
    }

    template <typename T>
    Matrix4<T> Quat<T>::ToMatrix() const
    {
        Matrix4<T> mat = {
            W * W + X * X - Y * Y - Z * Z, 2 * X * Y - 2 * W * Z, 2 * X * Z + 2 * W * Y, 0,
            2 * X * Y + 2 * W * Z, W * W - X * X + Y * Y - Z * Z, 2 * Y * Z - 2 * W * X, 0,
            2 * X * Z - 2 * W * Y, 2 * Y * Z + 2 * W * X, W * W - X * X - Y * Y + Z * Z, 0,
            0, 0, 0, 1
        };

        return (mat);
    }

    template <typename T>
    Quat<T> Quat<T>::operator*(const Quat<T> &other) const
    {
        Quat<T> res;

        res.W = W * other.W - X * other.X - Y * other.Y - Z * other.Z;
        res.X = W * other.X + X * other.W + Y * other.Z - Z * other.Y;
        res.Y = W * other.Y + Y * other.W + Z * other.X - X * other.Z;
        res.Z = W * other.Z + Z * other.W + X * other.Y - Y * other.X;
        return (res);
    }

    template <typename T>
    bool Quat<T>::operator==(const Quat<T> &other) const
    {
        T diffx = X - other.X;
        T diffy = Y - other.Y;
        T diffz = Z - other.Z;
        T diffw = W - other.W;

        return (diffx < (T)0.00001 && diffy < (T)0.00001
                && diffz < (T)0.00001 && diffw < (T)0.00001);
    }

    template <typename T>
    Quat<T> Quat<T>::Pow(const T n) const
    {
        Quat<T> q = Ln();

        q.W *= n;
        q.X *= n;
        q.Y *= n;
        q.Z *= n;
        return (q.Exp());
    }

    template <typename T>
    Quat<T> Quat<T>::Exp() const
    {
        T r = Math::Sqrt(X * X + Y * Y + Z * Z);
        T et = Math::Exp(W);
        T s = r >= (T)0.00001 ? et * Math::Sin(r) / r : 0;

        return (Quat<T>(et * Math::Cos(r), X * s, Y * s, Z * s));
    }

    template <typename T>
    Quat<T> Quat<T>::Ln() const
    {
        T r = Math::Sqrt(X * X + Y * Y + Z * Z);
        T et = r > (T)0.00001 ? Math::ArcTan2(r, W) / r : 0;

        return (Quat<T>((T)0.5 * Math::Ln(W * W + X * X + Y * Y + Z * Z), X * et, Y * et, Z * et));
    }

    template <typename T>
    T Quat<T>::Angle(const Quat<T> &other) const
    {
        Quat<T> res = other * Invert();

        return (Math::ArcCos(res.W) * 2);
    }

    template <typename T>
    Vector3<T> Quat<T>::ToEulerAngles() const
    {
        T roll = Math::ArcTan2(2 * Y * W + 2 * X * Z, 1 - 2 * Y * Y - 2 * Z * Z);
        T pitch = Math::ArcTan2(2 * X * W + 2 * Y * Z, 1 - 2 * X * X - 2 * Z * Z);
        T yaw = Math::ArcSin(2 * X * Y + 2 * Z * W);

        return (Vector3<T>(pitch, yaw, roll));
    }

    template <typename T>
    Quat<T> Quat<T>::Lerp(const Quat<T> &q, const Quat<T> &q1, const T t)
    {
        Quat<T> res(Math::Lerp(q.W, q1.W, t), Math::Lerp(q.X, q1.X, t),
                    Math::Lerp(q.Y, q1.Y, t), Math::Lerp(q.Z, q1.Z, t));

        return (res);
    }

    template <typename T>
    Quat<T> Quat<T>::Slerp(const Quat<T> &q, const Quat<T> &q1, const T t)
    {
        Quat<T> res = ((q1 * q.Invert()).Pow(t)) * q;

        return (res);
    }
}
