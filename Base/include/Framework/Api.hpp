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
#include "Framework/Types.hpp"

#if defined(__GNUC__) || defined(__clang__)
    /**
     * Use this macro to define a declaration as deprecated
     * @param message the deprecation message
     */
    #define BP_DEPRECATED(message) __attribute__((deprecated(message)))

    /**
     * Auto generate base sortcut/alias (cross platform way to achieve CRTP)
     * @param BaseName the name of the base class
     * @param ... the base class declaration
     */
    #define BP_DEFINE_BASE(BaseName, ...)                                                                              \
    private:                                                                                                           \
        using Base = __VA_ARGS__
#elif defined(_MSC_VER)
    /**
     * Use this macro to define a declaration as deprecated
     * @param message the deprecation message
     */
    #define BP_DEPRECATED(message) __declspec(deprecated(message))

    /**
     * Auto generate base sortcut/alias (cross platform way to achieve CRTP)
     * @param BaseName the name of the base class
     * @param ... the base class declaration
     */
    #define BP_DEFINE_BASE(BaseName, ...)                                                                              \
    private:                                                                                                           \
        using Base = BaseName
#else
    #pargma message "WARNING: Your compiler is not supported"

    /**
     * Use this macro to define a declaration as deprecated
     * @param message the deprecation message
     */
    #define BP_DEPRECATED(message)
#endif

#ifdef __GNUC__
    /**
     * Forces ignore of a function return value
     * @param v the function cal to ignore the return value
     */
    #define BP_IGNORE(v)                                                                                               \
        if (v)                                                                                                         \
        {                                                                                                              \
        }
// Because GCC is a peace of shit we must make the code slower, please GCC learn how to code it's FUCKING ANNOYING how
// bad you are at coding Even Microsoft is able to do better
#else
    /**
     * Forces ignore of a function return value
     * @param v the function cal to ignore the return value
     */
    #define BP_IGNORE(v) (void)v
// Yeah everyone else in the world understood how things should be done! MOTHER FUCKER GNU LEARN FROM THAT YOU PEACE OF
// MOTHERSHIT
#endif
