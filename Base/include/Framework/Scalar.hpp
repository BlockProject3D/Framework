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
#include <limits>
#include "Framework/String.hpp"
#include "Framework/ParseException.hpp"

namespace bpf
{
    class BPF_API Int
    {
    public:

        /**
         * 32 bits int max value
         */
        static constexpr fint MaxValue = 2147483647;

        /**
         * 32 bits int min value
         */
        //Fuck you MSVC, @Microsoft learn that the min value of 32 bits int is -2147483648!
        static constexpr fint MinValue = -2147483647 - 1;

        /**
         * Parse a 32 bits int from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 32 bits int parsed from string
         */
        static fint Parse(const String &str);

        /**
         * Parse a 32 bits int from a string
         * @param str the string to parse
         * @param res 32 bits int parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, fint &res);
    };

    class BPF_API UInt
    {
    public:

        /**
         * 32 bits uint max value
         */
        static constexpr uint32 MaxValue = 4294967295;

        /**
         * 32 bits uint min value
         */
        static constexpr uint32 MinValue = 0;

        /**
         * Parse a 32 bits uint from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 32 bits uint parsed from string
         */
        static uint32 Parse(const String &str);

        /**
         * Parse a 32 bits uint from a string
         * @param str the string to parse
         * @param res 32 bits uint parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, uint32 &res);
    };

    class BPF_API Int8
    {
    public:

        /**
         * 8 bits int max value
         */
        static constexpr int8 MaxValue = 127;

        /**
         * 8 bits int min value
         */
        static constexpr int8 MinValue = -128;

        /**
         * Parse an 8 bits int from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 8 bits int parsed from string
         */
        static int8 Parse(const String &str);

        /**
         * Parse an 8 bits int from a string
         * @param str the string to parse
         * @param res 8 bits int parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, int8 &res);
    };

    class BPF_API UInt8
    {
    public:

        /**
         * 8 bits uint max value
         */
        static constexpr uint8 MaxValue = 255;

        /**
         * 8 bits uint min value
         */
        static constexpr uint8 MinValue = 0;

        /**
         * Parse an 8 bits uint from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 8 bits uint parsed from string
         */
        static uint8 Parse(const String &str);

        /**
         * Parse an 8 bits uint from a string
         * @param str the string to parse
         * @param res 8 bits uint parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, uint8 &res);
    };

    class BPF_API Int16
    {
    public:

        /**
         * 16 bits int max value
         */
        static constexpr int16 MaxValue = 32767;

        /**
         * 16 bits int min value
         */
        static constexpr int16 MinValue = -32768;

        /**
         * Parse a 16 bits int from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 16 bits int parsed from string
         */
        static int16 Parse(const String &str);

        /**
         * Parse a 16 bits int from a string
         * @param str the string to parse
         * @param res 16 bits int parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, int16 &res);
    };

    class BPF_API UInt16
    {
    public:

        /**
         * 16 bits uint max value
         */
        static constexpr uint16 MaxValue = 65535;

        /**
         * 16 bits uint min value
         */
        static constexpr uint16 MinValue = 0;

        /**
         * Parse a 16 bits uint from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 16 bits uint parsed from string
         */
        static uint16 Parse(const String &str);

        /**
         * Parse a 16 bits uint from a string
         * @param str the string to parse
         * @param res 16 bits uint parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, uint16 &res);
    };

    class BPF_API Int64
    {
    public:

        /**
         * 64 bits int max value
         */
        static constexpr int64 MaxValue = 9223372036854775807LL;

        /**
         * 64 bits int min value
         */
        static constexpr int64 MinValue = -9223372036854775807LL - 1;

        /**
         * Parse a 64 bits int from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 64 bits int parsed from string
         */
        static int64 Parse(const String &str);

        /**
         * Parse a 64 bits int from a string
         * @param str the string to parse
         * @param res 64 bits int parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, int64 &res);
    };

    class BPF_API UInt64
    {
    public:

        /**
         * 64 bits uint max value
         */
        static constexpr uint64 MaxValue = 18446744073709551615ULL;

        /**
         * 64 bits uint min value
         */
        static constexpr uint64 MinValue = 0;

        /**
         * Parse a 64 bits uint from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 64 bits uint parsed from string
         */
        static uint64 Parse(const String &str);

        /**
         * Parse a 64 bits uint from a string
         * @param str the string to parse
         * @param res 64 bits uint parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, uint64 &res);
    };

    class BPF_API Float
    {
    public:

        /**
         * 32 bits float max value
         */
        static constexpr float MaxValue = std::numeric_limits<float>::max();

        /**
         * 32 bits float min value
         */
        static constexpr float MinValue = std::numeric_limits<float>::min();

        /**
         * Parse a 32 bits float from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 32 bits float parsed from string
         */
        static float Parse(const String &str);

        /**
         * Parse a 32 bits float from a string
         * @param str the string to parse
         * @param res 32 bits float parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, float &res);
    };

    class BPF_API Double
    {
    public:

        /**
         * 64 bits double max value
         */
        static constexpr double MaxValue = std::numeric_limits<double>::max();

        /**
         * 64 bits double min value
         */
        static constexpr double MinValue = std::numeric_limits<double>::max();

        /**
         * Parse a 64 bits double from a string
         * @param str the string to parse
         * @throw ParseException when the string cannot be converted to a number
         * @return 64 bits double parsed from string
         */
        static double Parse(const String &str);

        /**
         * Parse a 64 bits double from a string
         * @param str the string to parse
         * @param res 64 bits double parsed from string
         * @return true if successfull, false otherwise
         */
        static bool TryParse(const String &str, double &res);
    };
}
