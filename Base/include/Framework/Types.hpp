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

#define Null nullptr

// Check windows
#ifdef WINDOWS
    #if _WIN64
        #define X86_64
    #else
        #define X86
    #endif
#else
    #if __x86_64__ || __ppc64__
        #define X86_64
    #else
        #define X86
    #endif
#endif

namespace bpf
{
    using uint32 = unsigned int;
    using uint8 = unsigned char;
    using int32 = int;
    using int64 = long long signed int;
    using uint64 = long long unsigned int;
    using int8 = signed char;
    using int16 = signed short;
    using uint16 = unsigned short;

    using fchar = uint32;
    using fchar16 = uint16;

    /**
     * Custom int type guarenteed to be ALWAYS 32bits no matter the platform
     */
    using fint = int32;

#ifdef X86_64
    using uintptr = uint64;
    using intptr = int64;
#else
    using uintptr = uint32;
    using intptr = int32;
#endif

    using fsize = uintptr;
	using fisize = intptr;
}
