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
#include <Framework/String.hpp>
#include <Framework/Array.hpp>
#include <Framework/Memory/Memory.hpp>

TEST(Array, Init)
{
	auto arr = bpf::Array<int>(3);

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 6);
}

TEST(Array, Copy)
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

TEST(Array, Move)
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

TEST(Array, Swap_1)
{
	auto arr = bpf::Array<int>(3);

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	arr.Swap(0, 2);
	EXPECT_EQ(arr[0], 6);
	EXPECT_EQ(arr[1], 3);
	EXPECT_EQ(arr[2], 1);
}

TEST(Array, Swap_2)
{
	auto arr = bpf::Array<int>(3);

	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 6;
	arr.Swap(1, 2);
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 6);
	EXPECT_EQ(arr[2], 3);
}

TEST(Array, Realloc)
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

TEST(Array, ReallocForbidThrow)
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

TEST(Array, ReadWrite_NonCopy)
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

TEST(Array, ReadWrite_NonCopy_MemLeak)
{
    bpf::fsize cur = bpf::Memory::GetAllocCount();

    Test_ReadWrite_NonCopy_MemLeak();
    EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}
