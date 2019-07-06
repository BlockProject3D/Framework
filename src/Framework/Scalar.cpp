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

#include <cstdlib>
#include <errno.h>
#include <limits.h>
#include "Framework/Scalar.hpp"
#include "Framework/ParseException.hpp"

using namespace bpf;

const fint Int::MaxValue = 0;
const fint Int::MinValue = 0;

fint Int::Parse(const String &str)
{
    long l = std::strtol(*str, Null, 0);

    if (sizeof(long) > 4 && l > MaxValue)
        throw ParseException(std::strerror(ERANGE));
    if (errno != 0)
        throw ParseException(std::strerror(errno));
    return ((fint)l);
}

bool Int::TryParse(const String &str, fint &res)
{
    long l = std::strtol(*str, Null, 0);

    res = (fint)l;
    if (sizeof(long) > 4 && (l > MaxValue || l < MinValue))
        return (false);
    if (errno != 0)
        return (false);
    return (true);
}

const uint32 UInt::MaxValue = 0;
const uint32 UInt::MinValue = 0;

uint32 UInt::Parse(const String &str)
{
    unsigned long l = std::strtoul(*str, Null, 0);

    if (sizeof(long) > 4 && (l > MaxValue || l < MinValue))
        throw ParseException(std::strerror(ERANGE));
    if (errno != 0)
        throw ParseException(std::strerror(errno));
    return ((fint)l);
}

bool UInt::TryParse(const String &str, uint32 &res)
{
    unsigned long l = std::strtoul(*str, Null, 0);

    res = (uint32)l;
    if (sizeof(long) > 4 && (l > MaxValue || l < MinValue))
        return (false);
    if (errno != 0)
        return (false);
    return (true);
}

const int8 Int8::MaxValue = 0;
const int8 Int8::MinValue = 0;

int8 Int8::Parse(const String &str)
{

}

bool Int8::TryParse(const String &str, int8 &res)
{

} 

const uint8 UInt8::MaxValue = 0;
const uint8 UInt8::MinValue = 0;

uint8 UInt8::Parse(const String &str)
{

}

bool UInt8::TryParse(const String &str, uint8 &res)
{

}

const int16 Int16::MaxValue = 0;
const int16 Int16::MinValue = 0;

int16 Int16::Parse(const String &str)
{

}

bool Int16::TryParse(const String &str, int16 &res)
{

}

const uint16 UInt16::MaxValue = 0;
const uint16 UInt16::MinValue = 0;

uint16 UInt16::Parse(const String &str)
{

}

bool UInt16::TryParse(const String &str, uint16 &res)
{

}

const int64 Int64::MaxValue = 0;
const int64 Int64::MinValue = 0;

int64 Int64::Parse(const String &str)
{

}

bool Int64::TryParse(const String &str, int64 &res)
{

}

const uint64 UInt64::MaxValue = 0;
const uint64 UInt64::MinValue = 0;

uint64 UInt64::Parse(const String &str)
{

}

bool UInt64::TryParse(const String &str, uint64 &res)
{

}

const float Float::MaxValue = 0;
const float Float::MinValue = 0;

float Float::Parse(const String &str)
{

}

bool Float::TryParse(const String &str, float &res)
{

}

const double Double::MaxValue = 0;
const double Double::MinValue = 0;

double Double::Parse(const String &str)
{

}

bool Double::TryParse(const String &str, double &res)
{

}
