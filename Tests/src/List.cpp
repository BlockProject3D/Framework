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

TEST(List, Creation_List)
{
    bpf::List<int> lst = { 0, 3, 7 };

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(List, Add)
{
    const int i = 12;
    bpf::List<int> lst = { i, 2 };

    EXPECT_EQ(lst.Size(), 2);
    lst.Add(3);
    EXPECT_EQ(lst.Size(), 3);
    lst.Add(i);
    EXPECT_EQ(lst.Size(), 4);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[12, 2, 3, 12]");
}

TEST(List, Indexer)
{
    bpf::List<int> lst = { 0, 3, 7 };

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

TEST(List, FindByKey)
{
    bpf::List<int> lst = { 0, 3, 7 };

    EXPECT_EQ(lst.begin(), lst.FindByKey(0));
    EXPECT_EQ(--lst.end(), lst.FindByKey(2));
    EXPECT_EQ(lst.end(), lst.FindByKey(3));
}

TEST(List, FindByValue)
{
    bpf::List<int> lst = { 0, 3, 7 };

    EXPECT_EQ(lst.begin(), lst.FindByValue(0));
    EXPECT_EQ(--lst.end(), lst.FindByValue(7));
    EXPECT_EQ(--lst.end(), lst.FindByValue<bpf::ops::Greater>(3));
    EXPECT_EQ(lst.end(), lst.FindByValue(42));
}

TEST(List, Find)
{
    bpf::List<int> lst = { 0, 3, 7 };

    EXPECT_EQ(++lst.begin(), lst.Find([](bpf::fsize pos, const int &val) { return (val == 3); }));
}

TEST(List, Concatenate)
{
    bpf::List<int> lst = { 0, 3, 7 };
    bpf::List<int> lst1 = { 0, 3, 7 };

    auto concatenated = lst + lst1;
    EXPECT_STREQ(*bpf::String::ValueOf(concatenated), "[0, 3, 7, 0, 3, 7]");
    lst1 += lst;
    EXPECT_STREQ(*bpf::String::ValueOf(lst1), "[0, 3, 7, 0, 3, 7]");
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 7]");
}

TEST(List, FirstLast_1)
{
    bpf::List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 7);
    lst.Clear();
    EXPECT_THROW(lst.First(), bpf::IndexException);
    EXPECT_THROW(lst.Last(), bpf::IndexException);
}

TEST(List, FirstLast_2)
{
    bpf::List<int> lst = { 0, 1 };
    const auto &cref = lst;
    bpf::List<int> lst1;
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
    EXPECT_EQ(lst.begin(), lst.end());
}

TEST(List, Remove)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

    lst.Remove(0, false);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 7, 0]");
    lst.Insert(0, 0);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 7, 0]");
    lst.Remove(0);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 7]");
    lst.Remove<bpf::ops::Less>(7);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[7]");
}

TEST(List, RemoveAt_1)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

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
    it = lst.end();
    lst.RemoveAt(it);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
}

TEST(List, RemoveAt_2)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

    lst.RemoveAt(2);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(++lst.begin());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 0]");
    lst.RemoveAt(lst.begin());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0]");
    EXPECT_NE(lst.begin(), lst.end());
    lst.RemoveAt(--lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[]");
    lst = { 0, 3, 7, 0 };
    lst.RemoveAt(--(--lst.end()));
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 3, 0]");
}

TEST(List, RemoveLast)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

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

TEST(List, Insert_1)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

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
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2]");
    lst.Insert(lst.Size(), i);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2, -2]");
}

TEST(List, Insert_2)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

    lst.Insert(1, 4);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), 5);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), -1);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    const int i = -2;
    lst.Insert(lst.begin(), 2);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), i);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, -2]");
    lst.Insert(--lst.end(), 10);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, -2]");
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2]");
    lst.Insert(lst.Size(), -2);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2, -2]");
}

TEST(List, Insert_3)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

    lst.Insert(1, 4);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), 5);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), -1);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    const int i = -2;
    lst.Insert(0, 2);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), i);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, -2]");
    lst.Insert(--lst.end(), 10);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, -2]");
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2]");
    lst.Insert(lst.Size(), i);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2, -2]");
}

TEST(List, Insert_4)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

    lst.Insert(1, 4);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), 5);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), -1);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    const int i = -2;
    lst.Insert(0, i);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), 2);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 2]");
    lst.Insert(--lst.end(), 10);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 2]");
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2]");
    lst.Insert(lst.Size(), i);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2, -2]");
}

TEST(List, Insert_5)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };
    const int a = 4;
    const int b = 5;
    const int c = -1;
    const int d = -2;
    const int e = 2;
    const int f = 10;
    const int g = 11;

    lst.Insert(1, a);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), b);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), c);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(0, d);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), e);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 2]");
    lst.Insert(--lst.end(), f);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 2]");
    lst.Insert(lst.Size() - 1, g);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2]");
    lst.Insert(lst.Size(), d);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2, -2]");
}

TEST(List, Insert_6)
{
    bpf::List<int> lst;
    const int a = 4;

    lst.Insert(0, a);
    EXPECT_EQ(4, lst[0]);
    lst.Clear();
    lst.Insert(0, 4);
    EXPECT_EQ(4, lst[0]);
    lst.Clear();
    lst.Insert(lst.begin(), a);
    EXPECT_EQ(4, lst[0]);
    lst.Clear();
    lst.Insert(lst.begin(), 4);
    EXPECT_EQ(4, lst[0]);
}

TEST(List, Iterator_1)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

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

TEST(List, Iterator_2)
{
    bpf::List<bpf::String> lst = { "a", "b", "c" };

    EXPECT_EQ(lst.begin()->Size(), 1);
    EXPECT_EQ(lst.begin()->ByteAt(0), 'a');
}

TEST(List, ReverseIterator_1)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

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

TEST(List, ReverseIterator_2)
{
    bpf::List<bpf::String> lst = { "a", "b", "c" };

    EXPECT_EQ(lst.rbegin()->Size(), 1);
    EXPECT_EQ(lst.rbegin()->ByteAt(0), 'c');
}

TEST(List, Clear)
{
    bpf::List<int> lst = { 0, 3, 7, 0 };

    EXPECT_EQ(lst.Size(), 4);
    lst.Clear();
    EXPECT_EQ(lst.Size(), 0);
    EXPECT_EQ(lst.begin(), lst.end());
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
    lst.Swap(lst.begin(), --(--lst.end()));
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[3, 0, 7]");
}

TEST(List, Swap_2)
{
    bpf::List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    lst.Swap(++lst.begin(), --lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7, 3]");
}

TEST(List, Swap_3)
{
    bpf::List<int> lst;

    lst.Add(0);
    lst.Add(7);
    lst.Swap(lst.begin(), --lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[7, 0]");
}

TEST(List, Swap_4)
{
    bpf::List<bpf::UniquePtr<int>> lst;

    lst.Add(bpf::MakeUnique<int>(0));
    lst.Add(bpf::MakeUnique<int>(7));
    lst.Swap(lst.begin(), --lst.end());
}

TEST(List, Swap_Err_1)
{
    bpf::List<bpf::UniquePtr<int>> lst;

    lst.Swap(lst.begin(), --lst.end());
}

TEST(List, Swap_Err_2)
{
    bpf::List<int> lst;

    lst.Add(0);
    lst.Add(7);
    lst.Swap(lst.begin(), lst.begin());
    lst.Swap(lst.end(), lst.end());
    lst.Swap(--lst.end(), --lst.end());
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 7]");
}

TEST(List, Sort_Unstable_1)
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

TEST(List, Sort_Stable_1)
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
    lst.Sort(true);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[-67, -5, -1, 0, 0, 1, 3, 7, 9]");
}

TEST(List, Sort_Unstable_2)
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
    lst.Sort<bpf::ops::Greater>();
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[9, 7, 3, 1, 0, 0, -1, -5, -67]");
}

TEST(List, Sort_Stable_2)
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
    lst.Sort<bpf::ops::Greater>(true);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[9, 7, 3, 1, 0, 0, -1, -5, -67]");
}

TEST(List, Sort_Err)
{
    bpf::List<int> lst;
    lst.Sort();
    EXPECT_EQ(lst.Size(), 0);
    lst = { 0 };
    lst.Sort();
    EXPECT_EQ(lst.Size(), 1);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0]");
}

static void Test_CopyMoveObj_MemLeak()
{
    bpf::List<bpf::String> lst = { "a", "b", "c" };

    lst.Add("d");
    bpf::List<bpf::String> cpy = lst;
    EXPECT_EQ(lst.Size(), cpy.Size());
    EXPECT_EQ(lst.Size(), 4);
    bpf::List<bpf::String> mv = std::move(lst);
    EXPECT_EQ(lst.Size(), 0);
    EXPECT_EQ(mv.Size(), 4);
    EXPECT_EQ(cpy.Size(), 4);
    cpy.Add("e");
    EXPECT_EQ(cpy.Size(), 5);
    EXPECT_EQ(mv.Size(), 4);
    EXPECT_EQ(lst.Size(), 0);
    EXPECT_STREQ(*bpf::String::ValueOf(mv), "[a, b, c, d]");
    EXPECT_STREQ(*bpf::String::ValueOf(cpy), "[a, b, c, d, e]");
}

TEST(List, Test_CopyMoveObj_MemLeak)
{
    bpf::fsize cur = bpf::Memory::GetAllocCount();

    Test_CopyMoveObj_MemLeak();
    EXPECT_EQ(cur, bpf::Memory::GetAllocCount());
}
