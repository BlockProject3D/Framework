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

#include <iostream>
#include <gtest/gtest.h>
#include <Framework/BaseConvert.hpp>

TEST(BaseConvert, FromStringBin)
{
	bpf::BaseConvert<bpf::fint> bin("01");

	EXPECT_EQ(bin.FromString("111"), 7);
	EXPECT_EQ(bin.FromString("110"), 6);
	EXPECT_EQ(bin.FromString("101"), 5);
	EXPECT_EQ(bin.FromString("100"), 4);
	EXPECT_EQ(bin.FromString("011"), 3);
	EXPECT_EQ(bin.FromString("010"), 2);
	EXPECT_EQ(bin.FromString("001"), 1);
	EXPECT_EQ(bin.FromString("000"), 0);
}

TEST(BaseConvert, FromStringHex)
{
	bpf::BaseConvert<bpf::fint> hex("0123456789ABCDEF");

	EXPECT_EQ(hex.FromString("100"), 256);
	EXPECT_EQ(hex.FromString("FF"), 255);
	EXPECT_EQ(hex.FromString("0"), 0);
	EXPECT_EQ(hex.FromString("A"), 10);
	EXPECT_EQ(hex.FromString("F"), 15);
	EXPECT_EQ(hex.FromString("B"), 11);
	EXPECT_EQ(hex.FromString("2"), 2);
	EXPECT_EQ(hex.FromString("5"), 5);
	EXPECT_EQ(hex.FromString("AF"), 175);
}

TEST(BaseConvert, ToStringBin)
{
	bpf::BaseConvert<bpf::fint> bin("01");

	EXPECT_STREQ(*bin.ToString(7), "111");
	EXPECT_STREQ(*bin.ToString(6), "110");
	EXPECT_STREQ(*bin.ToString(5), "101");
	EXPECT_STREQ(*bin.ToString(4), "100");
	EXPECT_STREQ(*bin.ToString(3), "11");
	EXPECT_STREQ(*bin.ToString(2), "10");
	EXPECT_STREQ(*bin.ToString(1), "1");
	EXPECT_STREQ(*bin.ToString(0), "0");
}

TEST(BaseConvert, ToStringHex)
{
	bpf::BaseConvert<bpf::fint> bin("0123456789ABCDEF");

	EXPECT_STREQ(*bin.ToString(256), "100");
	EXPECT_STREQ(*bin.ToString(255), "FF");
	EXPECT_STREQ(*bin.ToString(0), "0");
	EXPECT_STREQ(*bin.ToString(10), "A");
	EXPECT_STREQ(*bin.ToString(15), "F");
	EXPECT_STREQ(*bin.ToString(11), "B");
	EXPECT_STREQ(*bin.ToString(2), "2");
	EXPECT_STREQ(*bin.ToString(5), "5");
	EXPECT_STREQ(*bin.ToString(175), "AF");
}
