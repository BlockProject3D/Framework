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
#include "Framework/String.hpp"
#include "Framework/Math/Matrix.hpp"

namespace bpf
{
    template <typename T, fsize N, fsize M>
    class String::Stringifier<math::Matrix<T, N, M>>
    {
    public:
        inline static String Stringify(const math::Matrix<T, N, M> &mat, const fsize prec = 0)
        {
            String res = "Matrix(\n\t";

            for (fsize i = 0; i != N; ++i)
            {
                for (fsize j = 0; j != M; ++j)
                {
                    res += String::ValueOf(mat(i, j), prec);
                    if (j < M - 1)
                        res += "\t";
                }
                if (i < N - 1)
                    res += "\n\t";
            }
            return (res + "\n)");
        }
    };

    template <typename T>
    class String::Stringifier<math::Matrix<T>>
    {
    public:
        inline static String Stringify(const math::Matrix<T> &mat, const fsize prec = 0)
        {
            String res = "Matrix(\n\t";

            for (fsize i = 0; i != mat.Rows(); ++i)
            {
                for (fsize j = 0; j != mat.Columns(); ++j)
                {
                    res += String::ValueOf(mat(i, j), prec);
                    if (j < mat.Rows() - 1)
                        res += "\t";
                }
                if (i < mat.Columns() - 1)
                    res += "\n\t";
            }
            return (res + "\n)");
        }
    };
}
