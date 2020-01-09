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
    void Matrix3<T>::Translate(const Vector2<T> &v)
    {
        Matrix3<T> mat = {
            1, 0, v.X,
            0, 1, v.Y,
            0, 0, 1
        };

        *this = *this * mat;
    }

    template <typename T>
    void Matrix3<T>::Rotate(const T ang)
    {
        Matrix3<T> mat = {
            Math::Cos(ang), -Math::Sin(ang), 0,
            Math::Sin(ang), Math::Cos(ang), 0,
            0, 0, 1
        };

        *this = *this * mat;
    }

    template <typename T>
    void Matrix3<T>::Scale(const Vector2<T> &v)
    {
        Matrix3<T> mat = {
            v.X, 0, 0,
            0, v.Y, 0,
            0, 0, 1
        };

        *this = *this * mat;
    }

    template <typename T>
    Vector3<T> Matrix3<T>::operator*(const Vector3<T> &other)
    {
        const T *data = Matrix<T, 3, 3>::operator*();
        T myvec[3] = { other.X, other.Y, other.Z };
        T result[3];

        for (fsize i = 0; i < 3; ++i)
        {
            T res = 0;
            for (fsize k = 0; k < 3; ++k)
                res += myvec[k] * data[i * 3 + k];
            result[i] = res;
        }
        return (Vector3<T>(result[0], result[1], result[2]));
    }
}
