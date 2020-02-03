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

TEST(ArrayStatic, Creation)
{
    auto arr = bpf::Array<int, 3>();

    arr[0] = 1;
    arr[1] = 3;
    arr[2] = 6;
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 3);
    EXPECT_EQ(arr[2], 6);
}

TEST(ArrayStatic, Creation_List_1)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(ArrayStatic, Creation_List_2)
{
    bpf::Array<int, 3> lst = { 0, 3, 7, 8 };

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_THROW(lst[3], bpf::IndexException);
}

TEST(ArrayStatic, Indexer)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_THROW(lst[3], bpf::IndexException);
    EXPECT_THROW(lst[678], bpf::IndexException);
    EXPECT_THROW(lst[(bpf::fsize) - 1], bpf::IndexException);
    EXPECT_THROW(lst[(bpf::fsize) - 465], bpf::IndexException);
    const auto &ref = lst;
    EXPECT_THROW(ref[(bpf::fsize) - 465], bpf::IndexException);
}

TEST(ArrayStatic, FindByKey)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    EXPECT_EQ(lst.begin(), lst.FindByKey(0));
    EXPECT_EQ(--lst.end(), lst.FindByKey(2));
    EXPECT_EQ(lst.end(), lst.FindByKey(3));
}

TEST(ArrayStatic, FindByValue)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    EXPECT_EQ(lst.begin(), lst.FindByValue(0));
    EXPECT_EQ(--lst.end(), lst.FindByValue(7));
    EXPECT_EQ(--lst.end(), lst.FindByValue<bpf::ops::Greater>(3));
    EXPECT_EQ(lst.end(), lst.FindByValue(42));
}

TEST(ArrayStatic, Find)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    EXPECT_EQ(++lst.begin(), lst.Find([](bpf::fsize pos, const int &val) { return (val == 3); }));
}

TEST(ArrayStatic, Equal)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };
    bpf::Array<int, 3> lst1 = { 0, 3, 7 };
    bpf::Array<int, 3> lst2 = { 0, 3 };

    EXPECT_TRUE(lst == lst1);
    EXPECT_FALSE(lst != lst1);
    EXPECT_FALSE(lst == lst2);
    EXPECT_TRUE(lst != lst2);
}

TEST(ArrayStatic, FirstLast_1)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 7);
    lst = {};
    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 0);
}

TEST(ArrayStatic, FirstLast_2)
{
    bpf::Array<int, 2> lst = { 0, 1 };
    const auto &cref = lst;
    bpf::Array<int, 0> lst1;
    const auto &cref1 = lst1;

    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 1);
    EXPECT_EQ(cref.First(), 0);
    EXPECT_EQ(cref.Last(), 1);
    EXPECT_THROW(lst1.First(), bpf::IndexException);
    EXPECT_THROW(lst1.Last(), bpf::IndexException);
    EXPECT_THROW(cref1.First(), bpf::IndexException);
    EXPECT_THROW(cref1.Last(), bpf::IndexException);
}

TEST(ArrayStatic, Copy_1)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    auto copy = lst;
    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_EQ(copy[0], 0);
    EXPECT_EQ(copy[1], 3);
    EXPECT_EQ(copy[2], 7);
}

TEST(ArrayStatic, Copy_2)
{
    auto arr = bpf::Array<int, 3>();

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

TEST(ArrayStatic, Iterator_1)
{
    bpf::Array<int, 4> lst = { 0, 3, 7, 0 };

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

TEST(ArrayStatic, Iterator_2)
{
    bpf::Array<bpf::String, 3> lst = { "a", "b", "c" };

    EXPECT_EQ(lst.begin()->Size(), 1);
    EXPECT_EQ(lst.begin()->ByteAt(0), 'a');
}

TEST(ArrayStatic, ReverseIterator_1)
{
    bpf::Array<int, 4> lst = { 0, 3, 7, 0 };

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

TEST(ArrayStatic, ReverseIterator_2)
{
    bpf::Array<bpf::String, 3> lst = { "a", "b", "c" };

    EXPECT_EQ(lst.rbegin()->Size(), 1);
    EXPECT_EQ(lst.rbegin()->ByteAt(0), 'c');
}

TEST(ArrayStatic, IterateForward_Test1)
{
    int res = 0;
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    for (auto &i : lst)
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(ArrayStatic, IterateForward_Test2)
{
    bpf::String res = bpf::String::Empty;
    bpf::Array<bpf::String, 5> lst = { "a", "b", "c", "d", "e" };

    for (auto &i : lst)
        res += i;
    EXPECT_STREQ(*res, "abcde");
}

TEST(ArrayStatic, IterateBackward_Test1)
{
    int res = 0;
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    for (auto &i : bpf::Reverse(lst))
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(ArrayStatic, IterateBackward_Test2)
{
    bpf::String res = bpf::String::Empty;
    bpf::Array<bpf::String, 5> lst = { "a", "b", "c", "d", "e" };

    for (auto &i : bpf::Reverse(lst))
        res += i;
    EXPECT_STREQ(*res, "edcba");
}

TEST(ArrayStatic, ReadWrite_NonCopy_1)
{
    bpf::Array<bpf::UniquePtr<int>, 1> lst;

    lst[0] = bpf::MakeUnique<int>(32);
    for (auto &it : lst)
        EXPECT_EQ(*it, 32);
    EXPECT_EQ(*lst[0], 32);
    *lst[0] = 42;
    EXPECT_EQ(*lst[0], 42);
}

TEST(ArrayStatic, Swap_1)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    lst.Swap(lst.begin(), --(--lst.end()));
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 0, 7]");
}

TEST(ArrayStatic, Swap_2)
{
    bpf::Array<int, 3> lst = { 0, 3, 7 };

    lst.Swap(++lst.begin(), --lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7, 3]");
}

TEST(ArrayStatic, Swap_3)
{
    bpf::Array<int, 2> lst = { 0, 7 };

    lst.Swap(lst.begin(), --lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[7, 0]");
}

TEST(ArrayStatic, Swap_4)
{
    bpf::Array<bpf::UniquePtr<int>, 2> lst;

    lst[0] = bpf::MakeUnique<int>(0);
    lst[1] = bpf::MakeUnique<int>(7);
    lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayStatic, Swap_Err_1)
{
    bpf::Array<bpf::UniquePtr<int>, 1> lst;

    lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayStatic, Swap_Err_2)
{
    bpf::Array<int, 2> lst = { 0, 7 };

    lst.Swap(lst.begin(), lst.begin());
    lst.Swap(lst.end(), lst.end());
    lst.Swap(--lst.end(), --lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7]");
}

TEST(ArrayStatic, ReadWrite_NonCopy_2)
{
    bpf::Array<bpf::UniquePtr<int>, 2> arr;
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
    bpf::Array<bpf::UniquePtr<int>, 2> arr;
    arr[0] = bpf::MakeUnique<int>(32);
    arr[1] = bpf::MakeUnique<int>(42);
    EXPECT_EQ(*arr[0], 32);
    EXPECT_EQ(*arr[1], 42);
    *arr[0] = 42;
    for (auto &it : arr)
        EXPECT_EQ(*it, 42);
}

TEST(ArrayStatic, ReadWrite_NonCopy_MemLeak)
{
    bpf::fsize cur = bpf::Memory::GetAllocCount();

    Test_ReadWrite_NonCopy_MemLeak();
    EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}

static void Test_CopyMoveObj_MemLeak()
{
    bpf::Array<bpf::String, 5> lst = { "a", "b", "c" };

    lst[3] = "d";
    bpf::Array<bpf::String, 5> cpy = lst;
    EXPECT_EQ(lst.Size(), cpy.Size());
    EXPECT_EQ(cpy.Size(), 5);
    cpy[4] = "e";
    EXPECT_EQ(cpy.Size(), 5);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[a, b, c, d, ]");
    EXPECT_STREQ(*bpf::String::ValueOf(cpy), "[a, b, c, d, e]");
}

TEST(ArrayStatic, Test_CopyMoveObj_MemLeak)
{
    bpf::fsize cur = bpf::Memory::GetAllocCount();

    Test_CopyMoveObj_MemLeak();
    EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}
