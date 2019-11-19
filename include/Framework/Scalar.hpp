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
#include <limits>
#include "Framework/String.hpp"

namespace bpf
{
    class Int
    {
    public:
        static constexpr fint MaxValue = 2147483647;
		//Fuck you MSVC, @Microsoft learn that the min value of 32 bits int is -2147483648!
        static constexpr fint MinValue = -2147483647 - 1;
        static fint Parse(const String &str);
        static bool TryParse(const String &str, fint &res);
    };

    class UInt
    {
    public:
        static constexpr uint32 MaxValue = 4294967295;
        static constexpr uint32 MinValue = 0;
        static uint32 Parse(const String &str);
        static bool TryParse(const String &str, uint32 &res);
    };

    class Int8
    {
    public:
        static constexpr int8 MaxValue = 127;
        static constexpr int8 MinValue = -128;
        static int8 Parse(const String &str);
        static bool TryParse(const String &str, int8 &res);
    };

    class UInt8
    {
    public:
        static constexpr uint8 MaxValue = 255;
        static constexpr uint8 MinValue = 0;
        static uint8 Parse(const String &str);
        static bool TryParse(const String &str, uint8 &res);
    };

    class Int16
    {
    public:
        static constexpr int16 MaxValue = 32767;
        static constexpr int16 MinValue = -32768;
        static int16 Parse(const String &str);
        static bool TryParse(const String &str, int16 &res);
    };

    class UInt16
    {
    public:
        static constexpr uint16 MaxValue = 65535;
        static constexpr uint16 MinValue = 0;
        static uint16 Parse(const String &str);
        static bool TryParse(const String &str, uint16 &res);
    };

    class Int64
    {
    public:
        static constexpr int64 MaxValue = 9223372036854775807LL;
        static constexpr int64 MinValue = -9223372036854775808LL;
        static int64 Parse(const String &str);
        static bool TryParse(const String &str, int64 &res);
    };

    class UInt64
    {
    public:
        static constexpr uint64 MaxValue = 18446744073709551615ULL;
        static constexpr uint64 MinValue = 0;
        static uint64 Parse(const String &str);
        static bool TryParse(const String &str, uint64 &res);
    };

    class Float
    {
    public:
        static constexpr float MaxValue = std::numeric_limits<float>::max();
        static constexpr float MinValue = std::numeric_limits<float>::min();
        static float Parse(const String &str);
        static bool TryParse(const String &str, float &res);
    };

    class Double
    {
    public:
        static constexpr double MaxValue = std::numeric_limits<double>::max();
        static constexpr double MinValue = std::numeric_limits<double>::max();
        static double Parse(const String &str);
        static bool TryParse(const String &str, double &res);
    };
}
