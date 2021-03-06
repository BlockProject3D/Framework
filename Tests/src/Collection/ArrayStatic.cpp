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

#include <Framework/Collection/Array.hpp>
#include <Framework/Collection/Stringifier.Array.hpp>
#include <Framework/Memory/Utility.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

using namespace bpf::memory;
using namespace bpf::collection;
using namespace bpf;

TEST(ArrayStatic, Creation)
{
    auto arr = Array<int, 3>();

    arr[0] = 1;
    arr[1] = 3;
    arr[2] = 6;
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 3);
    EXPECT_EQ(arr[2], 6);
}

TEST(ArrayStatic, Creation_List_1)
{
    Array<int, 3> lst = {0, 3, 7};

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(ArrayStatic, Creation_List_2)
{
    Array<int, 3> lst = {0, 3, 7, 8};

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_THROW(lst[3], IndexException);
}

TEST(ArrayStatic, Indexer)
{
    Array<int, 3> lst = {0, 3, 7};

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_THROW(lst[3], IndexException);
    EXPECT_THROW(lst[678], IndexException);
    EXPECT_THROW(lst[(fsize)-1], IndexException);
    EXPECT_THROW(lst[(fsize)-465], IndexException);
    const auto &ref = lst;
    EXPECT_THROW(ref[(fsize)-465], IndexException);
}

TEST(ArrayStatic, FindByKey)
{
    Array<int, 3> lst = {0, 3, 7};

    EXPECT_EQ(lst.begin(), lst.FindByKey(0));
    EXPECT_EQ(--lst.end(), lst.FindByKey(2));
    EXPECT_EQ(lst.end(), lst.FindByKey(3));
}

TEST(ArrayStatic, FindByValue)
{
    Array<int, 3> lst = {0, 3, 7};

    EXPECT_EQ(lst.begin(), lst.FindByValue(0));
    EXPECT_EQ(--lst.end(), lst.FindByValue(7));
    EXPECT_EQ(--lst.end(), lst.FindByValue<ops::Greater>(3));
    EXPECT_EQ(lst.end(), lst.FindByValue(42));
}

TEST(ArrayStatic, Find)
{
    Array<int, 3> lst = {0, 3, 7};

    EXPECT_EQ(++lst.begin(), lst.Find([](fsize, const int &val) { return (val == 3); }));
    EXPECT_EQ(lst.end(), lst.Find([](fsize, const int &val) { return (val == 42); }));
}

TEST(ArrayStatic, Equal)
{
    Array<int, 3> lst = {0, 3, 7};
    Array<int, 3> lst1 = {0, 3, 7};
    Array<int, 3> lst2 = {0, 3};

    EXPECT_TRUE(lst == lst1);
    EXPECT_FALSE(lst != lst1);
    EXPECT_FALSE(lst == lst2);
    EXPECT_TRUE(lst != lst2);
}

TEST(ArrayStatic, FirstLast_1)
{
    Array<int, 3> lst = {0, 3, 7};

    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 7);
    lst = {};
    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 0);
}

TEST(ArrayStatic, FirstLast_2)
{
    Array<int, 2> lst = {0, 1};
    const auto &cref = lst;
    Array<int, 0> lst1;
    const auto &cref1 = lst1;

    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 1);
    EXPECT_EQ(cref.First(), 0);
    EXPECT_EQ(cref.Last(), 1);
    EXPECT_THROW(lst1.First(), IndexException);
    EXPECT_THROW(lst1.Last(), IndexException);
    EXPECT_THROW(cref1.First(), IndexException);
    EXPECT_THROW(cref1.Last(), IndexException);
}

TEST(ArrayStatic, Copy_1)
{
    Array<int, 3> lst = {0, 3, 7};

    auto copy = lst;
    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_EQ(copy[0], 0);
    EXPECT_EQ(copy[1], 3);
    EXPECT_EQ(copy[2], 7);
}

TEST(ArrayStatic, Move)
{
    Array<String, 3> lst = {"0", "3", "7"};

    auto mv = std::move(lst);
    EXPECT_STREQ(*mv[0], "0");
    EXPECT_STREQ(*mv[1], "3");
    EXPECT_STREQ(*mv[2], "7");
    EXPECT_TRUE(lst[0].IsEmpty());
    EXPECT_TRUE(lst[1].IsEmpty());
    EXPECT_TRUE(lst[2].IsEmpty());
}

TEST(ArrayStatic, Copy_2)
{
    auto arr = Array<int, 3>();

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
    Array<int, 4> lst = {0, 3, 7, 0};

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
    Array<String, 3> lst = {"a", "b", "c"};

    EXPECT_EQ(lst.begin()->Size(), 1);
    EXPECT_EQ(lst.begin()->ByteAt(0), 'a');
}

TEST(ArrayStatic, ReverseIterator_1)
{
    Array<int, 4> lst = {0, 3, 7, 0};

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
    Array<String, 3> lst = {"a", "b", "c"};

    EXPECT_EQ(lst.rbegin()->Size(), 1);
    EXPECT_EQ(lst.rbegin()->ByteAt(0), 'c');
}

TEST(ArrayStatic, CIterator_1)
{
    Array<int, 4> lst1 = {0, 3, 7, 0};
    const auto &lst = lst1;

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

TEST(ArrayStatic, CIterator_2)
{
    Array<String, 3> lst1 = {"a", "b", "c"};
    const auto &lst = lst1;

    EXPECT_EQ(lst.begin()->Size(), 1);
    EXPECT_EQ(lst.begin()->ByteAt(0), 'a');
}

TEST(ArrayStatic, CReverseIterator_1)
{
    Array<int, 4> lst1 = {0, 3, 7, 0};
    const auto &lst = lst1;

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

TEST(ArrayStatic, CReverseIterator_2)
{
    Array<String, 3> lst1 = {"a", "b", "c"};
    const auto &lst = lst1;

    EXPECT_EQ(lst.rbegin()->Size(), 1);
    EXPECT_EQ(lst.rbegin()->ByteAt(0), 'c');
}

TEST(ArrayStatic, IterateForward_Test1)
{
    int res = 0;
    Array<int, 3> lst = {0, 3, 7};

    for (auto &i : lst)
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(ArrayStatic, IterateForward_Test2)
{
    String res = "";
    Array<String, 5> lst = {"a", "b", "c", "d", "e"};

    for (auto &i : lst)
        res += i;
    EXPECT_STREQ(*res, "abcde");
}

TEST(ArrayStatic, IterateBackward_Test1)
{
    int res = 0;
    Array<int, 3> lst = {0, 3, 7};

    for (auto &i : Reverse(lst))
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(ArrayStatic, IterateBackward_Test2)
{
    String res = "";
    Array<String, 5> lst = {"a", "b", "c", "d", "e"};

    for (auto &i : Reverse(lst))
        res += i;
    EXPECT_STREQ(*res, "edcba");
}

TEST(ArrayStatic, ReadWrite_NonCopy_1)
{
    Array<UniquePtr<int>, 1> lst;

    lst[0] = MakeUnique<int>(32);
    for (auto &it : lst)
        EXPECT_EQ(*it, 32);
    EXPECT_EQ(*lst[0], 32);
    *lst[0] = 42;
    EXPECT_EQ(*lst[0], 42);
}

TEST(ArrayStatic, Swap_1)
{
    Array<int, 3> lst = {0, 3, 7};

    lst.Swap(lst.begin(), --(--lst.end()));
    EXPECT_STREQ(*String::ValueOf(lst), "[3, 0, 7]");
}

TEST(ArrayStatic, Swap_2)
{
    Array<int, 3> lst = {0, 3, 7};

    lst.Swap(++lst.begin(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 7, 3]");
}

TEST(ArrayStatic, Swap_3)
{
    Array<int, 2> lst = {0, 7};

    lst.Swap(lst.begin(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[7, 0]");
}

TEST(ArrayStatic, Swap_4)
{
    Array<UniquePtr<int>, 2> lst;

    lst[0] = MakeUnique<int>(0);
    lst[1] = MakeUnique<int>(7);
    lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayStatic, Swap_Err_1)
{
    Array<UniquePtr<int>, 1> lst;

    lst.Swap(lst.begin(), --lst.end());
}

TEST(ArrayStatic, Swap_Err_2)
{
    Array<int, 2> lst = {0, 7};

    lst.Swap(lst.begin(), lst.begin());
    lst.Swap(lst.end(), lst.end());
    lst.Swap(--lst.end(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 7]");
}

TEST(ArrayStatic, ReadWrite_NonCopy_2)
{
    Array<UniquePtr<int>, 2> arr;
    arr[0] = MakeUnique<int>(32);
    arr[1] = MakeUnique<int>(42);
    EXPECT_EQ(*arr[0], 32);
    EXPECT_EQ(*arr[1], 42);
    *arr[0] = 42;
    for (auto &it : arr)
        EXPECT_EQ(*it, 42);
}

#ifdef BUILD_DEBUG
static void Test_ReadWrite_NonCopy_MemLeak()
{
    Array<UniquePtr<int>, 2> arr;
    arr[0] = MakeUnique<int>(32);
    arr[1] = MakeUnique<int>(42);
    EXPECT_EQ(*arr[0], 32);
    EXPECT_EQ(*arr[1], 42);
    *arr[0] = 42;
    for (auto &it : arr)
        EXPECT_EQ(*it, 42);
}

TEST(ArrayStatic, ReadWrite_NonCopy_MemLeak)
{
    fsize cur = Memory::GetAllocCount();

    Test_ReadWrite_NonCopy_MemLeak();
    EXPECT_EQ(cur, Memory::GetAllocCount());
}

static void Test_CopyMoveObj_MemLeak()
{
    Array<String, 5> lst = {"a", "b", "c"};

    lst[3] = "d";
    Array<String, 5> cpy = lst;
    EXPECT_EQ(lst.Size(), cpy.Size());
    EXPECT_EQ(cpy.Size(), 5U);
    cpy[4] = "e";
    EXPECT_EQ(cpy.Size(), 5U);
    EXPECT_STREQ(*String::ValueOf(lst), "[a, b, c, d, ]");
    EXPECT_STREQ(*String::ValueOf(cpy), "[a, b, c, d, e]");
}

TEST(ArrayStatic, Test_CopyMoveObj_MemLeak)
{
    fsize cur = Memory::GetAllocCount();

    Test_CopyMoveObj_MemLeak();
    EXPECT_EQ(cur, Memory::GetAllocCount());
}
#endif
