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
#include <typeinfo>
#include "Framework/Types.hpp"

namespace bpf
{
    /**
     * Returns the cross-platform type name of a given type.
     * Defaults to implementation defined typeid
     * @tparam T the type to search the name of
     * @return low-level null-terminated c-string
     */
    template <typename T>
    inline const char *TypeName() noexcept
    {
        return (typeid(T).name());
    }

    /**
     * Returns a type hash in order to compare two types
     * @tparam T type to find hash for
     * @return unsigned hash code
     */
    template <typename T>
    inline fsize TypeIndex() noexcept
    {
        return (typeid(T).hash_code());
    }
}

/**
 * Defines the type name for a given type
 * @param T the type to define the cross platform type name for
 */
#define BP_DEFINE_TYPENAME(T)                                                                                          \
    namespace bpf                                                                                                      \
    {                                                                                                                  \
        template <>                                                                                                    \
        inline const char *TypeName<T>() noexcept                                                                      \
        {                                                                                                              \
            return (#T);                                                                                               \
        }                                                                                                              \
    }

BP_DEFINE_TYPENAME(int)
BP_DEFINE_TYPENAME(float)
BP_DEFINE_TYPENAME(double)
