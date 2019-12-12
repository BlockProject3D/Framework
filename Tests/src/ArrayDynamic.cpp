// Copyright (c) 2019, BlockProject
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
#include <Framework/Stringifier.Container.hpp>
#include <Framework/Array.hpp>
#include <Framework/Memory/Memory.hpp>

TEST(ArrayDynamic, Creation)
{
	auto arr = bpf::Array<int>(3);

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 6);
}

TEST(ArrayDynamic, Creation_List)
{
	bpf::Array<int> lst = { 0, 3, 7 };

	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
}

TEST(ArrayDynamic, Indexer)
{
	bpf::Array<int> lst1 = { 0, 3, 7 };

	EXPECT_EQ(lst1[0], 0);
	EXPECT_EQ(lst1[1], 3);
	EXPECT_EQ(lst1[2], 7);
	const auto &lst = lst1;
	EXPECT_THROW(lst[3], bpf::IndexException);
	EXPECT_THROW(lst[678], bpf::IndexException);
	EXPECT_THROW(lst[(bpf::fsize) - 1], bpf::IndexException);
	EXPECT_THROW(lst[(bpf::fsize) - 465], bpf::IndexException);
}

TEST(ArrayDynamic, FirstLast)
{
	bpf::Array<int> lst = { 0, 3, 7 };

	EXPECT_EQ(lst.First(), 0);
	EXPECT_EQ(lst.Last(), 7);
	lst = {};
	EXPECT_THROW(lst.First(), bpf::IndexException);
	EXPECT_THROW(lst.Last(), bpf::IndexException);
}

TEST(ArrayDynamic, Copy_1)
{
	bpf::Array<int> lst = { 0, 3, 7 };

	auto copy = lst;
	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
	EXPECT_EQ(copy[0], 0);
	EXPECT_EQ(copy[1], 3);
	EXPECT_EQ(copy[2], 7);
}

TEST(ArrayDynamic, Move_1)
{
	bpf::Array<int> lst = { 0, 3, 7 };

	auto mv = std::move(lst);
	EXPECT_EQ(mv[0], 0);
	EXPECT_EQ(mv[1], 3);
	EXPECT_EQ(mv[2], 7);
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_EQ(lst.begin(), lst.end());
}

TEST(ArrayDynamic, Copy_2)
{
	auto arr = bpf::Array<int>(3);

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 6);
	auto copy = arr;
	EXPECT_EQ(copy[0], 1);
	EXPECT_EQ(copy[1], 3);
	EXPECT_EQ(copy[2], 6);
}

TEST(ArrayDynamic, Move_2)
{
	auto arr = bpf::Array<int>(3);

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 6);
	auto mv = std::move(arr);
	EXPECT_EQ(mv[0], 1);
	EXPECT_EQ(mv[1], 3);
	EXPECT_EQ(mv[2], 6);
	EXPECT_EQ(arr.Size(), 0);
	EXPECT_EQ(arr.Release(), Null);
}

TEST(ArrayDynamic, Iterator)
{
	bpf::Array<int> lst = { 0, 3, 7, 0 };

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

TEST(ArrayDynamic, ReverseIterator)
{
	bpf::Array<int> lst = { 0, 3, 7, 0 };

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

TEST(ArrayDynamic, IterateForward_Test1)
{
	int res = 0;
	bpf::Array<int> lst = { 0, 3, 7 };

	for (auto &i : lst)
		res += i;
	EXPECT_EQ(res, 10);
}

TEST(ArrayDynamic, IterateForward_Test2)
{
	bpf::String res = bpf::String::Empty;
	bpf::Array<bpf::String> lst = { "a", "b", "c", "d", "e" };

	for (auto &i : lst)
		res += i;
	EXPECT_STREQ(*res, "abcde");
}

TEST(ArrayDynamic, IterateBackward_Test1)
{
	int res = 0;
	bpf::Array<int> lst = { 0, 3, 7 };

	for (auto &i : bpf::Reverse(lst))
		res += i;
	EXPECT_EQ(res, 10);
}

TEST(ArrayDynamic, IterateBackward_Test2)
{
	bpf::String res = bpf::String::Empty;
	bpf::Array<bpf::String> lst = { "a", "b", "c", "d", "e" };

	for (auto &i : bpf::Reverse(lst))
		res += i;
	EXPECT_STREQ(*res, "edcba");
}

TEST(ArrayDynamic, ReadWrite_NonCopy_1)
{
	bpf::Array<bpf::UniquePtr<int>> lst(1);

	lst[0] = bpf::MakeUnique<int>(32);
	for (auto &it : lst)
		EXPECT_EQ(*it, 32);
	EXPECT_EQ(*lst[0], 32);
	*lst[0] = 42;
	EXPECT_EQ(*lst[0], 42);
}

TEST(ArrayDynamic, Swap_1)
{
	bpf::Array<int> lst = { 0, 3, 7 };

	lst.Swap(lst.begin(), --(--lst.end()));
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 0, 7]");
}

TEST(ArrayDynamic, Swap_2)
{
	bpf::Array<int> lst = { 0, 3, 7 };

	lst.Swap(++lst.begin(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7, 3]");
}

TEST(ArrayDynamic, Swap_3)
{
	bpf::Array<int> lst = { 0, 7 };

	lst.Swap(lst.begin(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[7, 0]");
}

TEST(ArrayDynamic, Swap_4)
{
	bpf::Array<bpf::UniquePtr<int>> lst;

	lst[0] = bpf::MakeUnique<int>(0);
	lst[1] = bpf::MakeUnique<int>(7);
	lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayDynamic, Swap_Err_1)
{
	bpf::Array<bpf::UniquePtr<int>> lst;

	lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayDynamic, Swap_Err_2)
{
	bpf::Array<int> lst = { 0, 7 };

	lst.Swap(lst.begin(), lst.begin());
	lst.Swap(lst.end(), lst.end());
	lst.Swap(--lst.end(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7]");
}

TEST(ArrayDynamic, Realloc)
{
	auto arr = bpf::Array<int>(3);

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	arr[3] = 100;
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 6);
	EXPECT_EQ(arr[3], 100);
}

TEST(ArrayDynamic, ReallocForbidThrow)
{
	auto arr = bpf::Array<int>(3);
	const auto &test = arr;

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 6);

	EXPECT_THROW(test[3], bpf::IndexException);
}

TEST(ArrayDynamic, ReadWrite_NonCopy_2)
{
    bpf::Array<bpf::UniquePtr<int>> arr;
    arr[0] = bpf::MakeUnique<int>(32);
    arr[1] = bpf::MakeUnique<int>(42);
    EXPECT_EQ(*arr[0], 32);
    EXPECT_EQ(*arr[1], 42);
    *arr[0] = 42;
    for (auto &it : arr)
        EXPECT_EQ(*it, 42);
}

static void Test_ReadWrite_NonCopy_MemLeak()
{
    bpf::Array<bpf::UniquePtr<int>> arr;
    arr[0] = bpf::MakeUnique<int>(32);
    arr[1] = bpf::MakeUnique<int>(42);
    EXPECT_EQ(*arr[0], 32);
    EXPECT_EQ(*arr[1], 42);
    *arr[0] = 42;
    for (auto &it : arr)
        EXPECT_EQ(*it, 42);
}

TEST(ArrayDynamic, ReadWrite_NonCopy_MemLeak)
{
    bpf::fsize cur = bpf::Memory::GetAllocCount();

    Test_ReadWrite_NonCopy_MemLeak();
    EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}

static void Test_CopyMoveObj_MemLeak()
{
	bpf::Array<bpf::String> lst = { "a", "b", "c" };

	lst[3] = "d";
	bpf::Array<bpf::String> cpy = lst;
	EXPECT_EQ(lst.Size(), cpy.Size());
	EXPECT_EQ(lst.Size(), 4);
	bpf::Array<bpf::String> mv = std::move(lst);
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_EQ(mv.Size(), 4);
	EXPECT_EQ(cpy.Size(), 4);
	cpy[4] = "e";
	EXPECT_EQ(cpy.Size(), 5);
	EXPECT_EQ(mv.Size(), 4);
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_STREQ(*bpf::String::ValueOf(mv), "[a, b, c, d]");
	EXPECT_STREQ(*bpf::String::ValueOf(cpy), "[a, b, c, d, e]");
}

TEST(ArrayDynamic, Test_CopyMoveObj_MemLeak)
{
	bpf::fsize cur = bpf::Memory::GetAllocCount();

	Test_CopyMoveObj_MemLeak();
	EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}
