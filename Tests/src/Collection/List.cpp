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

#include <Framework/Collection/List.hpp>
#include <Framework/Collection/Stringifier.List.hpp>
#include <Framework/Memory/Utility.hpp>
#include <Framework/String.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

using namespace bpf::memory;
using namespace bpf::collection;
using namespace bpf;

TEST(List, Creation)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(List, Creation_List)
{
    List<int> lst = {0, 3, 7};

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(List, Add)
{
    const int i = 12;
    List<int> lst = {i, 2};

    EXPECT_EQ(lst.Size(), 2U);
    lst.Add(3);
    EXPECT_EQ(lst.Size(), 3U);
    lst.Add(i);
    EXPECT_EQ(lst.Size(), 4U);
    EXPECT_STREQ(*String::ValueOf(lst), "[12, 2, 3, 12]");
}

TEST(List, Indexer)
{
    List<int> lst = {0, 3, 7};

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    lst[1] = 42;
    EXPECT_EQ(lst[1], 42);
    EXPECT_THROW(lst[3], IndexException);
    EXPECT_THROW(lst[678], IndexException);
    EXPECT_THROW(lst[(fsize)-1], IndexException);
    EXPECT_THROW(lst[(fsize)-465], IndexException);
    const auto &ref = lst;
    EXPECT_THROW(ref[(fsize)-465], IndexException);
}

TEST(List, FindByKey)
{
    List<int> lst = {0, 3, 7};

    EXPECT_EQ(lst.begin(), lst.FindByKey(0));
    EXPECT_EQ(--lst.end(), lst.FindByKey(2));
    EXPECT_EQ(lst.end(), lst.FindByKey(3));
}

TEST(List, FindByValue)
{
    List<int> lst = {0, 3, 7};

    EXPECT_EQ(lst.begin(), lst.FindByValue(0));
    EXPECT_EQ(--lst.end(), lst.FindByValue(7));
    EXPECT_EQ(--lst.end(), lst.FindByValue<ops::Greater>(3));
    EXPECT_EQ(lst.end(), lst.FindByValue(42));
}

TEST(List, Find)
{
    List<int> lst = {0, 3, 7};

    EXPECT_EQ(++lst.begin(), lst.Find([](fsize, const int &val) { return (val == 3); }));
    EXPECT_EQ(lst.end(), lst.Find([](fsize, const int &val) { return (val == 42); }));
}

TEST(List, Equal)
{
    List<int> lst = {0, 3, 7};
    List<int> lst1 = {0, 3, 7};
    List<int> lst2 = {0, 3};
    List<int> lst3 = {0, 3, 4};

    EXPECT_TRUE(lst == lst1);
    EXPECT_FALSE(lst != lst1);
    EXPECT_FALSE(lst == lst2);
    EXPECT_TRUE(lst != lst2);
    EXPECT_FALSE(lst == lst3);
    EXPECT_TRUE(lst != lst3);
}

TEST(List, Concatenate)
{
    List<int> lst = {0, 3, 7};
    List<int> lst1 = {0, 3, 7};

    auto concatenated = lst + lst1;
    EXPECT_STREQ(*String::ValueOf(concatenated), "[0, 3, 7, 0, 3, 7]");
    lst1 += lst;
    EXPECT_STREQ(*String::ValueOf(lst1), "[0, 3, 7, 0, 3, 7]");
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 7]");
}

TEST(List, FirstLast_1)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    EXPECT_EQ(lst.First(), 0);
    EXPECT_EQ(lst.Last(), 7);
    lst.Clear();
    EXPECT_THROW(lst.First(), IndexException);
    EXPECT_THROW(lst.Last(), IndexException);
}

TEST(List, FirstLast_2)
{
    List<int> lst = {0, 1};
    const auto &cref = lst;
    List<int> lst1;
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

TEST(List, Copy)
{
    List<int> lst;

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
    List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    auto mv = std::move(lst);
    EXPECT_EQ(mv[0], 0);
    EXPECT_EQ(mv[1], 3);
    EXPECT_EQ(mv[2], 7);
    EXPECT_EQ(lst.Size(), 0U);
    EXPECT_EQ(lst.begin(), lst.end());
}

TEST(List, Remove)
{
    List<int> lst = {0, 3, 7, 0};

    lst.Remove(0, false);
    EXPECT_STREQ(*String::ValueOf(lst), "[3, 7, 0]");
    lst.Insert(0, 0);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 7, 0]");
    lst.Remove(0);
    EXPECT_STREQ(*String::ValueOf(lst), "[3, 7]");
    lst.Remove<ops::Less>(7);
    EXPECT_STREQ(*String::ValueOf(lst), "[7]");
}

TEST(List, RemoveAt_1)
{
    List<int> lst = {0, 3, 7, 0};

    lst.RemoveAt(2);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(++lst.begin());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 0]");
    auto it = lst.begin();
    lst.RemoveAt(it);
    EXPECT_STREQ(*String::ValueOf(lst), "[0]");
    EXPECT_NE(it, lst.end());
    lst.RemoveAt(--lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[]");
    lst = {0, 3, 7, 0};
    lst.RemoveAt(--(--lst.end()));
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
    it = lst.end();
    lst.RemoveAt(it);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
}

TEST(List, RemoveAt_2)
{
    List<int> lst = {0, 3, 7, 0};

    lst.RemoveAt(2);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(++lst.begin());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 0]");
    lst.RemoveAt(lst.begin());
    EXPECT_STREQ(*String::ValueOf(lst), "[0]");
    EXPECT_NE(lst.begin(), lst.end());
    lst.RemoveAt(--lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[]");
    lst = {0, 3, 7, 0};
    lst.RemoveAt(--(--lst.end()));
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
    lst.RemoveAt(lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 0]");
}

TEST(List, RemoveLast)
{
    List<int> lst = {0, 3, 7, 0};

    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 7, 0]");
    EXPECT_NE(lst.begin(), lst.end());
    lst.RemoveLast();
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3, 7]");
    EXPECT_NE(lst.begin(), lst.end());
    lst.RemoveLast();
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 3]");
    EXPECT_NE(lst.begin(), lst.end());
    lst.RemoveLast();
    EXPECT_STREQ(*String::ValueOf(lst), "[0]");
    EXPECT_NE(lst.begin(), lst.end());
    lst.RemoveLast();
    EXPECT_STREQ(*String::ValueOf(lst), "[]");
    EXPECT_EQ(lst.begin(), lst.end());
}

TEST(List, Insert_1)
{
    List<int> lst = {0, 3, 7, 0};

    lst.Insert(1, 4);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), 5);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), -1);
    EXPECT_STREQ(*String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    const int i = -2;
    lst.Insert(lst.begin(), i);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), 2);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 2]");
    lst.Insert(--lst.end(), 10);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 2]");
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2]");
    lst.Insert(lst.Size(), i);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2, -2]");
}

TEST(List, Insert_2)
{
    List<int> lst = {0, 3, 7, 0};

    lst.Insert(1, 4);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), 5);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), -1);
    EXPECT_STREQ(*String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    const int i = -2;
    lst.Insert(lst.begin(), 2);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), i);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, -2]");
    lst.Insert(--lst.end(), 10);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, -2]");
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2]");
    lst.Insert(lst.Size(), -2);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2, -2]");
}

TEST(List, Insert_3)
{
    List<int> lst = {0, 3, 7, 0};

    lst.Insert(1, 4);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), 5);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), -1);
    EXPECT_STREQ(*String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    const int i = -2;
    lst.Insert(0, 2);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), i);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, -2]");
    lst.Insert(--lst.end(), 10);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, -2]");
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2]");
    lst.Insert(lst.Size(), i);
    EXPECT_STREQ(*String::ValueOf(lst), "[2, -1, 0, 5, 4, 3, 7, 0, 10, 11, -2, -2]");
}

TEST(List, Insert_4)
{
    List<int> lst = {0, 3, 7, 0};

    lst.Insert(1, 4);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), 5);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), -1);
    EXPECT_STREQ(*String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    const int i = -2;
    lst.Insert(0, i);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), 2);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 2]");
    lst.Insert(--lst.end(), 10);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 2]");
    lst.Insert(lst.Size() - 1, 11);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2]");
    lst.Insert(lst.Size(), i);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2, -2]");
}

TEST(List, Insert_5)
{
    List<int> lst = {0, 3, 7, 0};
    const int a = 4;
    const int b = 5;
    const int c = -1;
    const int d = -2;
    const int e = 2;
    const int f = 10;
    const int g = 11;

    lst.Insert(1, a);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 4, 3, 7, 0]");
    lst.Insert(++lst.begin(), b);
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.begin(), c);
    EXPECT_STREQ(*String::ValueOf(lst), "[-1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(0, d);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0]");
    lst.Insert(lst.end(), e);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 2]");
    lst.Insert(--lst.end(), f);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 2]");
    lst.Insert(lst.Size() - 1, g);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2]");
    lst.Insert(lst.Size(), d);
    EXPECT_STREQ(*String::ValueOf(lst), "[-2, -1, 0, 5, 4, 3, 7, 0, 10, 11, 2, -2]");
}

TEST(List, Insert_6)
{
    List<int> lst;
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
    List<int> lst = {0, 3, 7, 0};

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
    List<String> lst = {"a", "b", "c"};

    EXPECT_EQ(lst.begin()->Size(), 1);
    EXPECT_EQ(lst.begin()->ByteAt(0), 'a');
}

TEST(List, ReverseIterator_1)
{
    List<int> lst = {0, 3, 7, 0};

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
    List<String> lst = {"a", "b", "c"};

    EXPECT_EQ(lst.rbegin()->Size(), 1);
    EXPECT_EQ(lst.rbegin()->ByteAt(0), 'c');
}

TEST(List, Clear)
{
    List<int> lst = {0, 3, 7, 0};

    EXPECT_EQ(lst.Size(), 4U);
    lst.Clear();
    EXPECT_EQ(lst.Size(), 0U);
    EXPECT_EQ(lst.begin(), lst.end());
}

TEST(List, IterateForward_Test1)
{
    int res = 0;
    List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    for (auto &i : lst)
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(List, IterateForward_Test2)
{
    String res = String::Empty;
    List<String> lst;

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
    List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    for (auto &i : Reverse(lst))
        res += i;
    EXPECT_EQ(res, 10);
}

TEST(List, IterateBackward_Test2)
{
    String res = String::Empty;
    List<String> lst;

    lst.Add("a");
    lst.Add("b");
    lst.Add("c");
    lst.Add("d");
    lst.Add("e");
    for (auto &i : Reverse(lst))
        res += i;
    EXPECT_STREQ(*res, "edcba");
}

TEST(List, ReadWrite_NonCopy)
{
    List<UniquePtr<int>> lst;

    lst.Add(MakeUnique<int>(32));
    for (auto &it : lst)
        EXPECT_EQ(*it, 32);
    EXPECT_EQ(*lst[0], 32);
    *lst[0] = 42;
    EXPECT_EQ(*lst[0], 42);
}

#ifdef BUILD_DEBUG
static void Test_ReadWrite_NonCopy_MemLeak()
{
    List<UniquePtr<int>> lst;

    lst.Add(MakeUnique<int>(32));
    for (auto &it : lst)
        EXPECT_EQ(*it, 32);
    EXPECT_EQ(*lst[0], 32);
}

TEST(List, ReadWrite_NonCopy_MemLeak)
{
    fsize cur = Memory::GetAllocCount();

    Test_ReadWrite_NonCopy_MemLeak();
    EXPECT_EQ(cur, Memory::GetAllocCount());
}
#endif

TEST(List, Swap_1)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    lst.Swap(lst.begin(), --(--lst.end()));
    EXPECT_STREQ(*String::ValueOf(lst), "[3, 0, 7]");
}

TEST(List, Swap_2)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(3);
    lst.Add(7);
    lst.Swap(++lst.begin(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 7, 3]");
}

TEST(List, Swap_3)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(7);
    lst.Swap(lst.begin(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[7, 0]");
}

TEST(List, Swap_4)
{
    List<UniquePtr<int>> lst;

    lst.Add(MakeUnique<int>(0));
    lst.Add(MakeUnique<int>(7));
    lst.Swap(lst.begin(), --lst.end());
}

TEST(List, Swap_Err_1)
{
    List<UniquePtr<int>> lst;

    lst.Swap(lst.begin(), --lst.end());
}

TEST(List, Swap_Err_2)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(7);
    lst.Swap(lst.begin(), lst.begin());
    lst.Swap(lst.end(), lst.end());
    lst.Swap(--lst.end(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "[0, 7]");
}

TEST(List, Sort_Unstable_1)
{
    List<int> lst;

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
    EXPECT_STREQ(*String::ValueOf(lst), "[-67, -5, -1, 0, 0, 1, 3, 7, 9]");
}

TEST(List, Sort_Stable_1)
{
    List<int> lst;

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
    EXPECT_STREQ(*String::ValueOf(lst), "[-67, -5, -1, 0, 0, 1, 3, 7, 9]");
}

TEST(List, Sort_Unstable_2)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(7);
    lst.Add(3);
    lst.Add(9);
    lst.Add(1);
    lst.Add(-67);
    lst.Add(-1);
    lst.Add(-5);
    lst.Add(0);
    lst.Sort<ops::Greater>();
    EXPECT_STREQ(*String::ValueOf(lst), "[9, 7, 3, 1, 0, 0, -1, -5, -67]");
}

TEST(List, Sort_Stable_2)
{
    List<int> lst;

    lst.Add(0);
    lst.Add(7);
    lst.Add(3);
    lst.Add(9);
    lst.Add(1);
    lst.Add(-67);
    lst.Add(-1);
    lst.Add(-5);
    lst.Add(0);
    lst.Sort<ops::Greater>(true);
    EXPECT_STREQ(*String::ValueOf(lst), "[9, 7, 3, 1, 0, 0, -1, -5, -67]");
}

TEST(List, Sort_Err)
{
    List<int> lst;
    lst.Sort();
    EXPECT_EQ(lst.Size(), 0U);
    lst = {0};
    lst.Sort();
    EXPECT_EQ(lst.Size(), 1U);
    EXPECT_STREQ(*String::ValueOf(lst), "[0]");
}

#ifdef BUILD_DEBUG
static void Test_CopyMoveObj_MemLeak()
{
    List<String> lst = {"a", "b", "c"};

    lst.Add("d");
    List<String> cpy = lst;
    EXPECT_EQ(lst.Size(), cpy.Size());
    EXPECT_EQ(lst.Size(), 4U);
    List<String> mv = std::move(lst);
    EXPECT_EQ(lst.Size(), 0U);
    EXPECT_EQ(mv.Size(), 4U);
    EXPECT_EQ(cpy.Size(), 4U);
    cpy.Add("e");
    EXPECT_EQ(cpy.Size(), 5U);
    EXPECT_EQ(mv.Size(), 4U);
    EXPECT_EQ(lst.Size(), 0U);
    EXPECT_STREQ(*String::ValueOf(mv), "[a, b, c, d]");
    EXPECT_STREQ(*String::ValueOf(cpy), "[a, b, c, d, e]");
}

TEST(List, Test_CopyMoveObj_MemLeak)
{
    fsize cur = Memory::GetAllocCount();

    Test_CopyMoveObj_MemLeak();
    EXPECT_EQ(cur, Memory::GetAllocCount());
}
#endif