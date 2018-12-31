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
#include "Framework/Math/Matrix.hpp"
#include "Framework/Math/Vector2.hpp"
#include "Framework/Math/Vector3.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API Matrix3 : public Matrix<3, 3, T>
    {
    public:
        inline Matrix3()
            : Matrix<3, 3, T>()
        {
        }

        inline Matrix3(const std::initializer_list<T> &lst)
            : Matrix<3, 3, T>(lst)
        {
        }

        inline Matrix3(const T *mat)
            : Matrix<3, 3, T>(mat)
        {
        }

        inline Matrix3(const Matrix3<T> &other)
            : Matrix<3, 3, T>(other)
        {
        }

        inline Matrix3(const Matrix<3, 3, T> &other)
            : Matrix<3, 3, T>(other)
        {
        }

        inline Matrix3<T> Invert() const
        {
            return (Matrix<3, 3, T>::Invert());
        }

        inline Matrix3<T> operator*(const Matrix3<T> &other) const
        {
            return (Matrix<3, 3, T>::operator*(other));
        }

        inline Matrix3<T> Transpose() const
        {
            return (Matrix<3, 3, T>::Transpose());
        }

        void Translate(const Vector2<T> &v);

        /**
         * WARNING : In radians !
         */
        void Rotate(const T ang);
        void Scale(const Vector2<T> &v);
        Vector3<T> operator*(const Vector3<T> &other);
    };
}

#include "Framework/Math/Matrix3.impl.hpp"
