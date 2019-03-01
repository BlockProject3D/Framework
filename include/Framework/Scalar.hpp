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
#include "Framework/String.hpp"

namespace bpf
{
    class Int
    {
    public:
        static const int MaxValue;
        static const int MinValue;
        static int Parse(const String &str);
    };

    class UInt
    {
    public:
        static const uint32 MaxValue;
        static const uint32 MinValue;
        static int Parse(const String &str);
    };

    class Short
    {
    public:
        static const int MaxValue;
        static const int MinValue;
        static int Parse(const String &str);
    };

    class UShort
    {
    public:
        static const uint32 MaxValue;
        static const uint32 MinValue;
        static int Parse(const String &str);
    };

    class Int64
    {
    public:
        static const int64 MaxValue;
        static const int64 MinValue;
        static int64 Parse(const String &str);
    };

    class UInt64
    {
    public:
        static const uint64 MaxValue;
        static const uint64 MinValue;
        static uint64 Parse(const String &str);
    };

    class Float
    {
    public:
        static float Parse(const String &str);
    };

    class Double
    {
    public:
        static double Parse(const String &str);
    };
}