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

fint Int::Parse(const String &str)
{
	errno = 0;
	char* ptr;
    long l = std::strtol(*str, &ptr, 0);

    if (errno != 0 || ptr != *str + str.Size())
        throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
    return ((fint)l);
}

bool Int::TryParse(const String &str, fint &res)
{
	errno = 0;
	char* ptr;
	long l = std::strtol(*str, &ptr, 0);

    res = (fint)l;
    if (errno != 0 || ptr != *str + str.Size())
        return (false);
    return (true);
}

uint32 UInt::Parse(const String &str)
{
	errno = 0;
	char* ptr;
    unsigned long l = std::strtoul(*str, &ptr, 0);

    if (errno != 0 || ptr != *str + str.Size())
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	return ((uint32)l);
}

bool UInt::TryParse(const String &str, uint32 &res)
{
	errno = 0;
	char* ptr;
    unsigned long l = std::strtoul(*str, &ptr, 0);

    res = (uint32)l;
    if (errno != 0 || ptr != *str + str.Size())
        return (false);
    return (true);
}

int8 Int8::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	long l = std::strtol(*str, &ptr, 0);

	if (errno != 0)
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	if (l < MinValue || l > MaxValue || ptr != *str + str.Size())
		throw ParseException("Out of range");
	return ((int8)l);
}

bool Int8::TryParse(const String &str, int8 &res)
{
	errno = 0;
	char* ptr;
	long l = std::strtol(*str, &ptr, 0);

	res = (int8)l;
	if (errno != 0 || ptr != *str + str.Size() || l < MinValue || l > MaxValue)
		return (false);
	return (true);
} 

uint8 UInt8::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	unsigned long l = std::strtoul(*str, &ptr, 0);

	if (errno != 0)
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	if (l < MinValue || l > MaxValue || ptr != *str + str.Size())
		throw ParseException("Out of range");
	return ((uint8)l);
}

bool UInt8::TryParse(const String &str, uint8 &res)
{
	errno = 0;
	char* ptr;
	unsigned long l = std::strtoul(*str, &ptr, 0);

	res = (uint8)l;
	if (errno != 0 || ptr != *str + str.Size() || l < MinValue || l > MaxValue)
		return (false);
	return (true);
}

int16 Int16::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	long l = std::strtol(*str, &ptr, 0);

	if (errno != 0)
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	if (l < MinValue || l > MaxValue || ptr != *str + str.Size())
		throw ParseException("Out of range");
	return ((int16)l);
}

bool Int16::TryParse(const String &str, int16 &res)
{
	errno = 0;
	char* ptr;
	long l = std::strtol(*str, &ptr, 0);

	res = (int16)l;
	if (errno != 0 || ptr != *str + str.Size() || l < MinValue || l > MaxValue)
		return (false);
	return (true);
}

uint16 UInt16::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	unsigned long l = std::strtoul(*str, &ptr, 0);

	if (errno != 0)
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	if (l < MinValue || l > MaxValue || ptr != *str + str.Size())
		throw ParseException("Out of range");
	return ((uint16)l);
}

bool UInt16::TryParse(const String &str, uint16 &res)
{
	errno = 0;
	char* ptr;
	unsigned long l = std::strtoul(*str, &ptr, 0);

	res = (uint16)l;
	if (errno != 0 || ptr != *str + str.Size() || l < MinValue || l > MaxValue)
		return (false);
	return (true);
}

int64 Int64::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	int64 l = std::strtoll(*str, &ptr, 0);

	if (errno != 0 || ptr != *str + str.Size())
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	return (l);
}

bool Int64::TryParse(const String &str, int64 &res)
{
	errno = 0;
	char* ptr;
	int64 l = std::strtoll(*str, &ptr, 0);

	res = l;
	if (errno != 0 || ptr != *str + str.Size())
		return (false);
	return (true);
}

uint64 UInt64::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	uint64 l = std::strtoull(*str, &ptr, 0);

	if (errno != 0 || ptr != *str + str.Size())
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	return (l);
}

bool UInt64::TryParse(const String &str, uint64 &res)
{
	errno = 0;
	char* ptr;
	uint64 l = std::strtoull(*str, &ptr, 0);

	res = l;
	if (errno != 0 || ptr != *str + str.Size())
		return (false);
	return (true);
}

float Float::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	float f = std::strtof(*str, &ptr);

	if (errno != 0 || ptr != *str + str.Size())
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	return (f);
}

bool Float::TryParse(const String &str, float &res)
{
	errno = 0;
	char* ptr;
	float f = std::strtof(*str, &ptr);

	res = f;
	if (errno != 0 || ptr != *str + str.Size())
		return (false);
	return (true);
}

double Double::Parse(const String &str)
{
	errno = 0;
	char* ptr;
	double d = std::strtod(*str, &ptr);

	if (errno != 0 || ptr != *str + str.Size())
		throw ParseException(errno == ERANGE ? "Out of range" : "Invalid string format");
	return (d);
}

bool Double::TryParse(const String &str, double &res)
{
	errno = 0;
	char* ptr;
	double d = std::strtod(*str, &ptr);

	res = d;
	if (errno != 0 || ptr != *str + str.Size())
		return (false);
	return (true);
}
