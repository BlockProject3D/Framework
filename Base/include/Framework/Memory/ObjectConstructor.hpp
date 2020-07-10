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
#include "Framework/Memory/Utility.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace memory
    {
        /**
         * Represent a constructor in a C++ class; part of mini-reflection API
         * @tparam C source class extends T
         * @tparam T base class (may be equal to C)
         * @tparam Args arguments to the constructor
         */
        template <class C, class /* C extends */ T, typename... Args>
        class BPF_API ObjectConstructor
        {
        public:
            inline UniquePtr<T> MakeUnique(Args &&... args) const
            {
                return (bpf::memory::MakeUnique<C>(std::forward<Args>(args)...));
            }
            inline SharedPtr<T> MakeShared(Args &&... args) const
            {
                return (bpf::memory::MakeShared<C>(std::forward<Args>(args)...));
            }
        };
    }
}

/**
 * Use in your header to opt in with the automatic constructor reflector
 * @param Class the source class type name (must extend the base class)
 * @param Parent the base class type name
 */
#define BP_USE_CONSTRUCTOR(Class, Parent)                                                                              \
    template <typename... Args>                                                                                        \
    static bpf::memory::ObjectConstructor<Class, Parent, Args...> *GetConstructor()

/**
 * Maps a given constructor for use with constructor reflection API.
 * Will compile error if class does not declare BP_USE_CONSTRUCTOR
 * @param Class the source class type name (must extend the base class)
 * @param Parent the base class type name
 * @param ... arguments to the constructor
 */
#define BP_MAP_CONSTRUCTOR(Class, Parent, ...)                                                                         \
    template <>                                                                                                        \
    bpf::memory::ObjectConstructor<Class, Parent, ##__VA_ARGS__> *Class::GetConstructor()                              \
    {                                                                                                                  \
        static bpf::memory::ObjectConstructor<Class, Parent, ##__VA_ARGS__> crt;                                       \
        return (&crt);                                                                                                 \
    }
