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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/Scalar.hpp>

TEST(Int, Parse)
{
	EXPECT_EQ(bpf::Int::Parse("42"), 42);
	EXPECT_EQ(bpf::Int::Parse("0"), 0);
	EXPECT_EQ(bpf::Int::Parse("-1"), -1);
	EXPECT_EQ(bpf::Int::Parse("-42"), -42);
	EXPECT_EQ(bpf::Int::Parse(bpf::String::ValueOf(bpf::Int::MaxValue)), bpf::Int::MaxValue);
	EXPECT_EQ(bpf::Int::Parse(bpf::String::ValueOf(bpf::Int::MinValue)), bpf::Int::MinValue);
	EXPECT_THROW(bpf::Int::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::Int::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::Int::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(Int, TryParse)
{
	int res;
	EXPECT_TRUE(bpf::Int::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(UInt, Parse)
{
	EXPECT_EQ(bpf::UInt::Parse("0"), 0U);
	EXPECT_EQ(bpf::UInt::Parse("42"), 42U);
	EXPECT_EQ(bpf::UInt::Parse("1"), 1U);
	EXPECT_EQ(bpf::UInt::Parse("4268902"), 4268902U);
	EXPECT_EQ(bpf::UInt::Parse(bpf::String::ValueOf(bpf::UInt::MaxValue)), bpf::UInt::MaxValue);
	EXPECT_EQ(bpf::UInt::Parse(bpf::String::ValueOf(bpf::UInt::MinValue)), bpf::UInt::MinValue);
	EXPECT_THROW(bpf::UInt::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(UInt, TryParse)
{
	bpf::uint32 res;
	EXPECT_TRUE(bpf::UInt::TryParse("42", res));
	EXPECT_EQ(res, 42U);
}

TEST(Int8, Parse)
{
	EXPECT_EQ(bpf::Int8::Parse("42"), 42);
	EXPECT_EQ(bpf::Int8::Parse("0"), 0);
	EXPECT_EQ(bpf::Int8::Parse("-1"), -1);
	EXPECT_EQ(bpf::Int8::Parse("-128"), -128);
	EXPECT_EQ(bpf::Int8::Parse("12"), 12);
	EXPECT_EQ(bpf::Int8::Parse(bpf::String::ValueOf(bpf::Int8::MaxValue)), bpf::Int8::MaxValue);
	EXPECT_EQ(bpf::Int8::Parse(bpf::String::ValueOf(bpf::Int8::MinValue)), bpf::Int8::MinValue);
	EXPECT_THROW(bpf::Int8::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::Int8::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::Int8::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(Int8, TryParse)
{
	bpf::int8 res;
	EXPECT_TRUE(bpf::Int8::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(UInt8, Parse)
{
	EXPECT_EQ(bpf::UInt8::Parse("42"), 42U);
	EXPECT_EQ(bpf::UInt8::Parse("0"), 0U);
	EXPECT_EQ(bpf::UInt8::Parse("1"), 1U);
	EXPECT_EQ(bpf::UInt8::Parse("128"), 128U);
	EXPECT_EQ(bpf::UInt8::Parse("12"), 12U);
	EXPECT_EQ(bpf::UInt8::Parse(bpf::String::ValueOf(bpf::UInt8::MaxValue)), bpf::UInt8::MaxValue);
	EXPECT_EQ(bpf::UInt8::Parse(bpf::String::ValueOf(bpf::UInt8::MinValue)), bpf::UInt8::MinValue);
	EXPECT_THROW(bpf::UInt8::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt8::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt8::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(UInt8, TryParse)
{
	bpf::uint8 res;
	EXPECT_TRUE(bpf::UInt8::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(Int16, Parse)
{
	EXPECT_EQ(bpf::Int16::Parse("42"), 42);
	EXPECT_EQ(bpf::Int16::Parse("0"), 0);
	EXPECT_EQ(bpf::Int16::Parse("-1"), -1);
	EXPECT_EQ(bpf::Int16::Parse("-128"), -128);
	EXPECT_EQ(bpf::Int16::Parse("-12"), -12);
	EXPECT_EQ(bpf::Int16::Parse(bpf::String::ValueOf(bpf::Int16::MaxValue)), bpf::Int16::MaxValue);
	EXPECT_EQ(bpf::Int16::Parse(bpf::String::ValueOf(bpf::Int16::MinValue)), bpf::Int16::MinValue);
	EXPECT_THROW(bpf::Int16::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::Int16::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::Int16::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(Int16, TryParse)
{
	bpf::int16 res;
	EXPECT_TRUE(bpf::Int16::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(UInt16, Parse)
{
	EXPECT_EQ(bpf::UInt16::Parse("42"), 42U);
	EXPECT_EQ(bpf::UInt16::Parse("0"), 0U);
	EXPECT_EQ(bpf::UInt16::Parse("1"), 1U);
	EXPECT_EQ(bpf::UInt16::Parse("128"), 128U);
	EXPECT_EQ(bpf::UInt16::Parse("12"), 12U);
	EXPECT_EQ(bpf::UInt16::Parse(bpf::String::ValueOf(bpf::UInt16::MaxValue)), bpf::UInt16::MaxValue);
	EXPECT_EQ(bpf::UInt16::Parse(bpf::String::ValueOf(bpf::UInt16::MinValue)), bpf::UInt16::MinValue);
	EXPECT_THROW(bpf::UInt16::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt16::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt16::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(UInt16, TryParse)
{
	bpf::uint16 res;
	EXPECT_TRUE(bpf::UInt16::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(Int64, Parse)
{
	EXPECT_EQ(bpf::Int64::Parse("42"), 42);
	EXPECT_EQ(bpf::Int64::Parse("0"), 0);
	EXPECT_EQ(bpf::Int64::Parse("-1"), -1);
	EXPECT_EQ(bpf::Int64::Parse("-128"), -128);
	EXPECT_EQ(bpf::Int64::Parse("-12"), -12);
	EXPECT_EQ(bpf::Int64::Parse(bpf::String::ValueOf(bpf::Int64::MaxValue)), bpf::Int64::MaxValue);
	EXPECT_EQ(bpf::Int64::Parse(bpf::String::ValueOf(bpf::Int64::MinValue)), bpf::Int64::MinValue);
	EXPECT_THROW(bpf::Int64::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::Int64::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::Int64::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(Int64, TryParse)
{
	bpf::int64 res;
	EXPECT_TRUE(bpf::Int64::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(UInt64, Parse)
{
	EXPECT_EQ(bpf::UInt64::Parse("42"), 42U);
	EXPECT_EQ(bpf::UInt64::Parse("0"), 0U);
	EXPECT_EQ(bpf::UInt64::Parse("1"), 1U);
	EXPECT_EQ(bpf::UInt64::Parse("128"), 128U);
	EXPECT_EQ(bpf::UInt64::Parse("12"), 12U);
	EXPECT_EQ(bpf::UInt64::Parse(bpf::String::ValueOf(bpf::UInt64::MaxValue)), bpf::UInt64::MaxValue);
	EXPECT_EQ(bpf::UInt64::Parse(bpf::String::ValueOf(bpf::UInt64::MinValue)), bpf::UInt64::MinValue);
	EXPECT_THROW(bpf::UInt64::Parse("4.2"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt64::Parse("-64262727648623784326834632"), bpf::ParseException);
	EXPECT_THROW(bpf::UInt64::Parse("64262727648623784326834632"), bpf::ParseException);
}

TEST(UInt64, TryParse)
{
	bpf::uint64 res;
	EXPECT_TRUE(bpf::UInt64::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(Float, Parse)
{
	EXPECT_EQ(bpf::Float::Parse("42"), 42);
	EXPECT_EQ(bpf::Float::Parse("0"), 0);
	EXPECT_EQ(bpf::Float::Parse("-1"), -1);
	EXPECT_EQ(bpf::Float::Parse("-128"), -128);
	EXPECT_EQ(bpf::Float::Parse("-12"), -12);
	EXPECT_LE(bpf::Float::Parse("42.2") - 42.2f, 0.0000001f);
	EXPECT_LE(bpf::Float::Parse("42.42") - 42.42f, 0.0000001f);
	EXPECT_EQ(bpf::Float::Parse("0.5"), 0.5f);
	EXPECT_LE(bpf::Float::Parse(bpf::String::ValueOf(bpf::Float::MaxValue)) - bpf::Float::MaxValue, 0.0000001f);
	EXPECT_LE(bpf::Float::Parse(bpf::String::ValueOf(bpf::Float::MinValue)) - bpf::Float::MinValue, 0.0000001f);
}

TEST(Float, TryParse)
{
	float res;
	EXPECT_TRUE(bpf::Float::TryParse("42", res));
	EXPECT_EQ(res, 42);
}

TEST(Double, Parse)
{
	EXPECT_EQ(bpf::Double::Parse("42"), 42);
	EXPECT_EQ(bpf::Double::Parse("0"), 0);
	EXPECT_EQ(bpf::Double::Parse("-1"), -1);
	EXPECT_EQ(bpf::Double::Parse("-128"), -128);
	EXPECT_EQ(bpf::Double::Parse("-12"), -12);
	EXPECT_EQ(bpf::Double::Parse("42.2"), 42.2);
	EXPECT_EQ(bpf::Double::Parse("42.42"), 42.42);
	EXPECT_EQ(bpf::Double::Parse("0.5"), 0.5);
	EXPECT_LE(bpf::Double::Parse(bpf::String::ValueOf(bpf::Double::MaxValue)) - bpf::Double::MaxValue, 0.0000001);
	EXPECT_LE(bpf::Double::Parse(bpf::String::ValueOf(bpf::Double::MinValue)) - bpf::Double::MinValue, 0.0000001);
}

TEST(Double, TryParse)
{
	float res;
	EXPECT_TRUE(bpf::Float::TryParse("42", res));
	EXPECT_EQ(res, 42);
}