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
#include <Framework/ArrayList.hpp>
#include <Framework/String.hpp>
#include <Framework/Stringifier.Container.hpp>
#include <Framework/Memory/Memory.hpp>

TEST(ArrayList, Creation)
{
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
}

TEST(ArrayList, Creation_List)
{
	bpf::ArrayList<int> lst = { 0, 3, 7 };

	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
}

TEST(ArrayList, Add)
{
	const int i = 12;
	bpf::ArrayList<int> lst = { i, 2 };

	EXPECT_EQ(lst.Size(), 2);
	lst.Add(3);
	EXPECT_EQ(lst.Size(), 3);
	lst.Add(i);
	EXPECT_EQ(lst.Size(), 4);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[12, 2, 3, 12]");
}

TEST(ArrayList, Indexer)
{
	bpf::ArrayList<int> lst = { 0, 3, 7 };

	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
	EXPECT_THROW(lst[3], bpf::IndexException);
	EXPECT_THROW(lst[678], bpf::IndexException);
	EXPECT_THROW(lst[(bpf::fsize) - 1], bpf::IndexException);
	EXPECT_THROW(lst[(bpf::fsize) - 465], bpf::IndexException);
}

TEST(ArrayList, FirstLast)
{
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	EXPECT_EQ(lst.First(), 0);
	EXPECT_EQ(lst.Last(), 7);
	lst.Clear();
	EXPECT_THROW(lst.First(), bpf::IndexException);
	EXPECT_THROW(lst.Last(), bpf::IndexException);
}

TEST(ArrayList, Copy)
{
	bpf::ArrayList<int> lst;

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

TEST(ArrayList, Move)
{
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	auto mv = std::move(lst);
	EXPECT_EQ(mv[0], 0);
	EXPECT_EQ(mv[1], 3);
	EXPECT_EQ(mv[2], 7);
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_EQ(lst.begin(), lst.end());
}

TEST(ArrayList, Remove)
{
	bpf::ArrayList<int> lst = { 0, 3, 7, 0 };

	lst.Remove(0, false);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 7, 0]");
	lst.Insert(0, 0);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 7, 0]");
	lst.Remove(0);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 7]");
}

TEST(ArrayList, RemoveAt)
{
	bpf::ArrayList<int> lst = { 0, 3, 7, 0 };

	lst.RemoveAt(2);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
	lst.RemoveAt(++lst.begin());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 0]");
	auto it = lst.begin();
	lst.RemoveAt(it);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0]");
	EXPECT_NE(it, lst.end());
	lst.RemoveAt(--lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[]");
	lst = { 0, 3, 7, 0 };
	lst.RemoveAt(--(--lst.end()));
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
	lst.RemoveAt(lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
}

TEST(ArrayList, RemoveLast)
{
	bpf::ArrayList<int> lst = { 0, 3, 7, 0 };

	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 7, 0]");
	EXPECT_NE(lst.begin(), lst.end());
	lst.RemoveLast();
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 7]");
	EXPECT_NE(lst.begin(), lst.end());
	lst.RemoveLast();
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3]");
	EXPECT_NE(lst.begin(), lst.end());
	lst.RemoveLast();
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0]");
	EXPECT_NE(lst.begin(), lst.end());
	lst.RemoveLast();
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[]");
	EXPECT_EQ(lst.begin(), lst.end());
}

TEST(ArrayList, Insert)
{
	bpf::ArrayList<int> lst = { 0, 3, 7, 0 };

	lst.Insert(1, 4);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 4, 3, 7, 0]");
	lst.Insert(++lst.begin(), 5);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
	lst.Insert(lst.begin(), -1);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
	const int i = -2;
	lst.Insert(lst.begin(), i);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0]");
	lst.Insert(lst.end(), 2);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 2]");
	lst.Insert(--lst.end(), 10);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 2]");
}

TEST(ArrayList, Iterator)
{
	bpf::ArrayList<int> lst = { 0, 3, 7, 0 };

	auto it = lst.begin();
	++it;
	--it;
	EXPECT_EQ(it, lst.begin());
	--it;
	++it;
	EXPECT_EQ(it, ++lst.begin());
	it = lst.end();
	--it;
	++it;
	EXPECT_EQ(it, lst.end());
	++it;
	--it;
	EXPECT_EQ(it, --lst.end());
}

TEST(ArrayList, ReverseIterator)
{
	bpf::ArrayList<int> lst = { 0, 3, 7, 0 };

	auto it = lst.rbegin();
	++it;
	--it;
	EXPECT_EQ(it, lst.rbegin());
	--it;
	++it;
	EXPECT_EQ(it, ++lst.rbegin());
	it = lst.rend();
	--it;
	++it;
	EXPECT_EQ(it, lst.rend());
	++it;
	--it;
	EXPECT_EQ(it, --lst.rend());
}

TEST(ArrayList, Clear)
{
	bpf::ArrayList<int> lst = { 0, 3, 7, 0 };

	EXPECT_EQ(lst.Size(), 4);
	lst.Clear();
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_EQ(lst.begin(), lst.end());
}

TEST(ArrayList, IterateForward_Test1)
{
	int res = 0;
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	for (auto &i : lst)
		res += i;
	EXPECT_EQ(res, 10);
}

TEST(ArrayList, IterateForward_Test2)
{
	bpf::String res = bpf::String::Empty;
	bpf::ArrayList<bpf::String> lst;

	lst.Add("a");
	lst.Add("b");
	lst.Add("c");
	lst.Add("d");
	lst.Add("e");
	for (auto &i : lst)
		res += i;
	EXPECT_STREQ(*res, "abcde");
}

TEST(ArrayList, IterateBackward_Test1)
{
	int res = 0;
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	for (auto &i : bpf::Reverse(lst))
		res += i;
	EXPECT_EQ(res, 10);
}

TEST(ArrayList, IterateBackward_Test2)
{
	bpf::String res = bpf::String::Empty;
	bpf::ArrayList<bpf::String> lst;

	lst.Add("a");
	lst.Add("b");
	lst.Add("c");
	lst.Add("d");
	lst.Add("e");
	for (auto &i : bpf::Reverse(lst))
		res += i;
	EXPECT_STREQ(*res, "edcba");
}

TEST(ArrayList, ReadWrite_NonCopy)
{
	bpf::ArrayList<bpf::UniquePtr<int>> lst;

	lst.Add(bpf::MakeUnique<int>(32));
	for (auto &it : lst)
		EXPECT_EQ(*it, 32);
	EXPECT_EQ(*lst[0], 32);
	*lst[0] = 42;
	EXPECT_EQ(*lst[0], 42);
}

static void Test_ReadWrite_NonCopy_MemLeak()
{
	bpf::ArrayList<bpf::UniquePtr<int>> lst;

	lst.Add(bpf::MakeUnique<int>(32));
	for (auto &it : lst)
		EXPECT_EQ(*it, 32);
	EXPECT_EQ(*lst[0], 32);
}

TEST(ArrayList, ReadWrite_NonCopy_MemLeak)
{
	bpf::fsize cur = bpf::Memory::GetAllocCount();

	Test_ReadWrite_NonCopy_MemLeak();
	EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}

TEST(ArrayList, Swap_1)
{
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	lst.Swap(lst.begin(), --(--lst.end()));
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 0, 7]");
}

TEST(ArrayList, Swap_2)
{
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(3);
	lst.Add(7);
	lst.Swap(++lst.begin(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7, 3]");
}

TEST(ArrayList, Swap_3)
{
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(7);
	lst.Swap(lst.begin(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[7, 0]");
}

TEST(ArrayList, Swap_4)
{
	bpf::ArrayList<bpf::UniquePtr<int>> lst;

	lst.Add(bpf::MakeUnique<int>(0));
	lst.Add(bpf::MakeUnique<int>(7));
	lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayList, Swap_Err_1)
{
	bpf::ArrayList<bpf::UniquePtr<int>> lst;

	lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayList, Swap_Err_2)
{
	bpf::ArrayList<int> lst;

	lst.Add(0);
	lst.Add(7);
	lst.Swap(lst.begin(), lst.begin());
	lst.Swap(lst.end(), lst.end());
	lst.Swap(--lst.end(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7]");
}

/*TEST(ArrayList, Sort_1)
{
	bpf::ArrayList<int> lst;

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
}*/