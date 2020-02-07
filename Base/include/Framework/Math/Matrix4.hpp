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
#include "Framework/Math/Vector3.hpp"
#include "Framework/Math/Vector4.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API Matrix4 : public Matrix<T, 4, 4>
    {
    public:
        inline Matrix4()
            : Matrix<T, 4, 4>()
        {
        }

        inline Matrix4(const std::initializer_list<T> &lst)
            : Matrix<T, 4, 4>(lst)
        {
        }

        explicit inline Matrix4(const T *mat)
            : Matrix<T, 4, 4>(mat)
        {
        }

        inline Matrix4(const Matrix4<T> &other)
            : Matrix<T, 4, 4>(other)
        {
        }

        inline Matrix4(const Matrix<T, 4, 4> &other)
            : Matrix<T, 4, 4>(other)
        {
        }

        inline Matrix4<T> Invert() const
        {
            return (Matrix<T, 4, 4>::Invert());
        }

        inline Matrix4<T> operator*(const Matrix4<T> &other) const
        {
            return (Matrix<T, 4, 4>::operator*(other));
        }

        inline Matrix4<T> Transpose() const
        {
            return (Matrix<T, 4, 4>::Transpose());
        }

        void Translate(const Vector3<T> &v);
        
        /**
         * WARNING : In radians !
         */
        void RotateYaw(const T ang);

        /**
         * WARNING : In radians !
         */
        void RotatePitch(const T ang);
        
        /**
         * WARNING : In radians !
         */
        void RotateRoll(const T ang);
        void Scale(const Vector3<T> &v);
        Vector4<T> operator*(const Vector4<T> &other) const;
    };
}

#include "Framework/Math/Matrix4.impl.hpp"
