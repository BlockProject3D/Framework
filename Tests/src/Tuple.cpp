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

#include <Framework/Memory/Utility.hpp>
#include <Framework/String.hpp>
#include <Framework/Tuple.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

TEST(Tuple, Basic)
{
    bpf::Tuple<int, float, double> tp;

    tp.Get<0>() = 42;
    tp.Get<1>() = 42.42f;
    tp.Get<2>() = 423.423;

    EXPECT_EQ(tp.Size(), 3);
    EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<float>(), 42.42f);
    EXPECT_EQ(tp.Get<double>(), 423.423);

    tp.Get<int>() = -42;
    EXPECT_EQ(tp.Size(), 3);
    EXPECT_EQ(tp.Get<int>(), -42);
    EXPECT_EQ(tp.Get<float>(), 42.42f);
    EXPECT_EQ(tp.Get<double>(), 423.423);
}

TEST(Tuple, Copy)
{
    bpf::Tuple<int, float, double> tp;

    tp.Get<0>() = 42;
    tp.Get<1>() = 42.42f;
    tp.Get<2>() = 423.423;

    EXPECT_EQ(tp.Size(), 3);
    EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<float>(), 42.42f);
    EXPECT_EQ(tp.Get<double>(), 423.423);

    auto copy = tp;

    EXPECT_EQ(copy.Size(), 3);
    EXPECT_EQ(copy.Get<int>(), 42);
    EXPECT_EQ(copy.Get<float>(), 42.42f);
    EXPECT_EQ(copy.Get<double>(), 423.423);

    copy.Get<0>() = -1;
    EXPECT_EQ(copy.Size(), 3);
    EXPECT_EQ(copy.Get<int>(), -1);
    EXPECT_EQ(copy.Get<float>(), 42.42f);
    EXPECT_EQ(copy.Get<double>(), 423.423);

    EXPECT_EQ(tp.Size(), 3);
    EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<float>(), 42.42f);
    EXPECT_EQ(tp.Get<double>(), 423.423);
}

TEST(Tuple, Move)
{
    bpf::Tuple<int, float, double> tp;

    tp.Get<0>() = 42;
    tp.Get<1>() = 42.42f;
    tp.Get<2>() = 423.423;

    EXPECT_EQ(tp.Size(), 3);
    EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<float>(), 42.42f);
    EXPECT_EQ(tp.Get<double>(), 423.423);

    auto mv = std::move(tp);

    EXPECT_EQ(mv.Size(), 3);
    EXPECT_EQ(mv.Get<int>(), 42);
    EXPECT_EQ(mv.Get<float>(), 42.42f);
    EXPECT_EQ(mv.Get<double>(), 423.423);

    mv.Get<0>() = -1;
    EXPECT_EQ(mv.Size(), 3);
    EXPECT_EQ(mv.Get<int>(), -1);
    EXPECT_EQ(mv.Get<float>(), 42.42f);
    EXPECT_EQ(mv.Get<double>(), 423.423);
}

TEST(Tuple, Move_1)
{
    bpf::Tuple<int, float, double, bpf::String> tp;

    tp.Get<0>() = 42;
    tp.Get<1>() = 42.42f;
    tp.Get<2>() = 423.423;
    tp.Get<3>() = "test";

    EXPECT_EQ(tp.Size(), 4);
    EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<float>(), 42.42f);
    EXPECT_EQ(tp.Get<double>(), 423.423);

    auto mv = std::move(tp);

    EXPECT_EQ(mv.Size(), 4);
    EXPECT_EQ(mv.Get<int>(), 42);
    EXPECT_EQ(mv.Get<float>(), 42.42f);
    EXPECT_EQ(mv.Get<double>(), 423.423);
    EXPECT_STREQ(*mv.Get<3>(), "test");

    mv.Get<0>() = -1;
    EXPECT_EQ(mv.Size(), 4);
    EXPECT_EQ(mv.Get<int>(), -1);
    EXPECT_EQ(mv.Get<float>(), 42.42f);
    EXPECT_EQ(mv.Get<double>(), 423.423);
    EXPECT_STREQ(*mv.Get<3>(), "test");

    EXPECT_EQ(tp.Get<3>().Len(), 0);
}

TEST(Tuple, String)
{
    bpf::Tuple<int, float, double, bpf::String> tp(42, 42.42f, 423.423, "test");

    EXPECT_EQ(tp.Size(), 4);
    EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<float>(), 42.42f);
    EXPECT_EQ(tp.Get<double>(), 423.423);
    EXPECT_STREQ(*tp.Get<bpf::String>(), "test");
}

TEST(Tuple, IdenticalTypes)
{
    bpf::Tuple<int, int> tp(42, 43);

    EXPECT_EQ(tp.Size(), 2);
    EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<0>(), 42);
    EXPECT_EQ(tp.Get<1>(), 43);
}

TEST(Tuple, IdenticalTypes_Strings)
{
    bpf::Tuple<bpf::String, bpf::String> tp("t", "t2");

    EXPECT_EQ(tp.Size(), 2);
    EXPECT_STREQ(*tp.Get<bpf::String>(), "t");
    EXPECT_STREQ(*tp.Get<0>(), "t");
    EXPECT_STREQ(*tp.Get<1>(), "t2");
}

TEST(Tuple, NonCopy_Types)
{
    bpf::Tuple<bpf::memory::UniquePtr<int>, bpf::memory::UniquePtr<int>> tp(bpf::memory::MakeUnique<int>(1), bpf::memory::MakeUnique<int>(-1));

    EXPECT_EQ(tp.Size(), 2);
    EXPECT_EQ(*tp.Get<bpf::memory::UniquePtr<int>>(), 1);
    EXPECT_EQ(*tp.Get<0>(), 1);
    EXPECT_EQ(*tp.Get<1>(), -1);
}

TEST(Tuple, Move_NonCopy_Types)
{
    bpf::Tuple<bpf::memory::UniquePtr<int>, bpf::memory::UniquePtr<int>> tp(bpf::memory::MakeUnique<int>(1), bpf::memory::MakeUnique<int>(-1));

    EXPECT_EQ(tp.Size(), 2);
    EXPECT_EQ(*tp.Get<bpf::memory::UniquePtr<int>>(), 1);
    EXPECT_EQ(*tp.Get<0>(), 1);
    EXPECT_EQ(*tp.Get<1>(), -1);

    auto mv = std::move(tp);
    EXPECT_EQ(tp.Get<0>(), Null);
    EXPECT_EQ(mv.Size(), 2);
    EXPECT_EQ(*mv.Get<bpf::memory::UniquePtr<int>>(), 1);
    EXPECT_EQ(*mv.Get<0>(), 1);
    EXPECT_EQ(*mv.Get<1>(), -1);
}

class BP_TPL_API Test1
{
public:
    int _a;
    bool _b;
	Test1(int a, bool b)
        : _a(a)
        , _b(b)
	{
	}
};

class BP_TPL_API Test2
{
public:
    Test1 &_f;
    double _d;
	Test2(Test1 &f, double d)
        : _f(f)
        , _d(d)
	{
	}
};

TEST(Tuple, Special_Preliminary)
{
    auto t = bpf::__internal_tp::IsAllDefaultConstructible<int, float, double>::Value;
    auto t1 = bpf::__internal_tp::IsAllDefaultConstructible<Test1, Test2, int>::Value;
    auto t2 = bpf::__internal_tp::IsAllDefaultConstructible<float, Test1, Test2, int>::Value;

    EXPECT_TRUE(t);
    EXPECT_FALSE(t1);
    EXPECT_FALSE(t2);
}

TEST(Tuple, Special)
{
    Test1 t(42, true);
	bpf::Tuple<Test1, Test2, int> tp(Test1(1, false), Test2(t, 0.42), 42);

	EXPECT_EQ(tp.Get<int>(), 42);
    EXPECT_EQ(tp.Get<Test1>()._a, 1);
    EXPECT_EQ(tp.Get<Test1>()._b, false);
    EXPECT_EQ(tp.Get<Test2>()._f._a, 42);
    EXPECT_EQ(tp.Get<Test2>()._f._b, true);
    EXPECT_EQ(tp.Get<Test2>()._d, 0.42);
}
