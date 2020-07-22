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

/**
 * Shortcut to C++ nullptr
 */
//#define Null nullptr

// Check windows
#ifdef WINDOWS
    #if _WIN64
        #define PLATFORM_64
    #else
        #define PLATFORM_32
    #endif
#else
    #if __x86_64__ || __ppc64__ || __aarch64__
        #define PLATFORM_64
    #else
        #define PLATFORM_32
    #endif
#endif

namespace bpf
{
    /**
     * 32 bits unsigned integer
     */
    using uint32 = unsigned int;

    /**
     * 8 bits unsigned integer
     */
    using uint8 = unsigned char;

    /**
     * 32 bits signed integer
     */
    using int32 = int;

    /**
     * 64 bits signed integer
     */
    using int64 = long long signed int;

    /**
     * 64 bits unsigned integer
     */
    using uint64 = long long unsigned int;

    /**
     * 8 bits signed integer
     */
    using int8 = signed char;

    /**
     * 16 bits signed integer
     */
    using int16 = signed short;

    /**
     * 16 bits unsigned integer
     */
    using uint16 = unsigned short;

    /**
     * UTF32 character type
     */
    using fchar = uint32;

    /**
     * UTF16 character type
     */
    using fchar16 = uint16;

    /**
     * Custom int type guarenteed to be ALWAYS 32bits no matter the platform/architecture combination
     */
    using fint = int32;

#ifdef PLATFORM_64
    /**
     * Unsigned type variant of the size of a register on the current system
     */
    using uintptr = uint64;

    /**
     * Signed type variant of the size of a register on the current system
     */
    using intptr = int64;
#else
    /**
     * Unsigned type variant of the size of a register on the current system
     */
    using uintptr = uint32;

    /**
     * Signed type variant of the size of a register on the current system
     */
    using intptr = int32;
#endif

    /**
     * Unsigned type variant used for collections and hash values
     */
    using fsize = uintptr;

    /**
     * Signed type variant used for certain collections
     */
    using fisize = intptr;
}
