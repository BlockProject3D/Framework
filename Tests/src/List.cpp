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
#include <Framework/List.hpp>
#include <Framework/String.hpp>
#include <Framework/Stringifier.Container.hpp>
#include <Framework/Memory/Memory.hpp>

TEST(List, Creation)
{
    bpf::List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
}

TEST(List, Copy)
{
	bpf::List<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	auto copy = lst;
	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
	EXPECT_EQ(copy[0], 0);
	EXPECT_EQ(copy[1], 3);
	EXPECT_EQ(copy[2], 7);
}

TEST(List, Move)
{
	bpf::List<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	auto mv = std::move(lst);
	EXPECT_EQ(mv[0], 0);
	EXPECT_EQ(mv[1], 3);
	EXPECT_EQ(mv[2], 7);
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_EQ(lst.FirstNode(), Null);
	EXPECT_EQ(lst.LastNode(), Null);
}

TEST(List, IterateForward_Test1)
{
    int res = 0;
    bpf::List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    for (auto &i : lst)
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(List, IterateForward_Test2)
{
    bpf::String res = bpf::String::Empty;
    bpf::List<bpf::String> lst;

    lst.Add("a");
    lst.Add("b");
    lst.Add("c");
    lst.Add("d");
    lst.Add("e");
    for (auto &i : lst)
        res += i;
    EXPECT_STREQ(*res, "abcde");
}

TEST(List, IterateBackward_Test1)
{
    int res = 0;
    bpf::List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    for (auto &i : bpf::Reverse(lst))
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(List, IterateBackward_Test2)
{
    bpf::String res = bpf::String::Empty;
    bpf::List<bpf::String> lst;

    lst.Add("a");
    lst.Add("b");
    lst.Add("c");
    lst.Add("d");
    lst.Add("e");
    for (auto &i : bpf::Reverse(lst))
        res += i;
    EXPECT_STREQ(*res, "edcba");
}

TEST(List, ReadWrite_NonCopy)
{
    bpf::List<bpf::UniquePtr<int>> lst;

    lst.Add(bpf::MakeUnique<int>(32));
    for (auto &it : lst)
        EXPECT_EQ(*it, 32);
    EXPECT_EQ(*lst[0], 32);
    *lst[0] = 42;
    EXPECT_EQ(*lst[0], 42);
}

static void Test_ReadWrite_NonCopy_MemLeak()
{
    bpf::List<bpf::UniquePtr<int>> lst;

    lst.Add(bpf::MakeUnique<int>(32));
    for (auto &it : lst)
        EXPECT_EQ(*it, 32);
    EXPECT_EQ(*lst[0], 32);
}

TEST(List, ReadWrite_NonCopy_MemLeak)
{
    bpf::fsize cur = bpf::Memory::GetAllocCount();

    Test_ReadWrite_NonCopy_MemLeak();
    EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}

TEST(List, Swap_1)
{
	bpf::List<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	lst.Swap(lst.FirstNode(), lst.LastNode());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[7, 3, 0]");
}

TEST(List, Swap_2)
{
	bpf::List<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	lst.Swap(lst.FirstNode()->Next, lst.LastNode());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7, 3]");
}

TEST(List, Swap_3)
{
	bpf::List<int> lst;

	lst.Add(0);
	lst.Add(7);
	lst.Swap(lst.FirstNode(), lst.LastNode());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[7, 0]");
}

TEST(List, Swap_4)
{
	bpf::List<bpf::UniquePtr<int>> lst;

	lst.Add(bpf::MakeUnique<int>(0));
	lst.Add(bpf::MakeUnique<int>(7));
	lst.Swap(lst.FirstNode(), lst.LastNode());
}

TEST(List, Sort_1)
{
	bpf::List<int> lst;

	lst.Add(0);
	lst.Add(7);
	lst.Add(3);
	lst.Add(9);
	lst.Add(1);
	lst.Add(-67);
	lst.Add(-1);
	lst.Add(-5);
	lst.Add(0);
	lst.Sort();
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-67, -5, -1, 0, 0, 1, 3, 7, 9]");
}