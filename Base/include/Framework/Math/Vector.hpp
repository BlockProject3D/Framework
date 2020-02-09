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
#include <utility>
#include <initializer_list>
#include "Framework/Types.hpp"
#include "Framework/IndexException.hpp"

namespace bpf
{
    template <typename T, fsize I = 0>
    class Vector
    {
    private:
        T _arr[I];

    public:
        explicit inline Vector(T &&... args)
            : _arr(std::forward<Args &&>(args)...)
        {
        }

        Vector(const std::initializer_list<T> &lst);

        inline T &operator()(const fsize l)
        {
            if (l > I)
                throw IndexException((fisize)l);
            return (_arr[l]);
        }

        inline const T &operator()(const fsize l) const
        {
            if (l > I)
                throw IndexException((fisize)l);
            return (_arr[l]);
        }

        Vector operator+(const Vector &other);
        Vector operator-(const Vector &other);
        Vector operator*(const Vector &other);
        Vector operator*(const T &other);
        Vector operator/(const Vector &other);
    };
}

#include "Framework/Math/Vector2.hpp"
#include "Framework/Math/Vector3.hpp"
#include "Framework/Math/Vector4.hpp"

namespace bpf
{
    using Vector2f = Vector2<float>;
    using Vector3f = Vector3<float>;
    using Vector4f = Vector4<float>;
}
