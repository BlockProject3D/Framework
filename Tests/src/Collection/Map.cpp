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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/Memory/Utility.hpp>
#include <Framework/Collection/Map.hpp>
#include <Framework/Collection/Stringifier.Map.hpp>

using namespace bpf::memory;
using namespace bpf::collection;
using namespace bpf;

TEST(Map, Creation_1)
{
    Map<String, int> map;

    map["test1"] = 0;
    map["test2"] = 3;
    map["test3"] = 7;
}

TEST(Map, Creation_2)
{
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 3);
    lst.Add(2, 7);
    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(Map, Creation_List)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(Map, Add_1)
{
    const int i = 12;
    Map<int, int> lst = { { 0, i }, { 1, 2 } };

    EXPECT_EQ(lst.Size(), 2U);
    lst.Add(2, 3);
    EXPECT_EQ(lst.Size(), 3U);
    lst.Add(3, i);
    EXPECT_EQ(lst.Size(), 4U);
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 12, '1': 2, '2': 3, '3': 12}");
}

TEST(Map, Add_2)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 1 } };

    lst.Add(2, 2);
    lst.Add(3, 3);
    lst.Add(-2, -2);
    lst.Add(-1, -1);
    EXPECT_EQ(lst.Size(), 6U);
    EXPECT_STREQ(*String::ValueOf(lst), "{'-2': -2, '-1': -1, '0': 0, '1': 1, '2': 2, '3': 3}");
}

TEST(Map, Add_3)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 1 } };

    lst.Add(2, 2);
    lst.Add(3, 3);
    lst.Add(-2, -2);
    lst.Add(-1, -1);
    lst.Add(5, 5);
    lst.Add(4, 4);
    lst.Add(7, 7);
    lst.Add(6, 6);
    EXPECT_EQ(lst.Size(), 10U);
    EXPECT_STREQ(*String::ValueOf(lst), "{'-2': -2, '-1': -1, '0': 0, '1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7}");
}

TEST(Map, Indexer)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
}

TEST(Map, FindByKey)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

    EXPECT_EQ(lst.begin(), lst.FindByKey(0));
    EXPECT_EQ(--lst.end(), lst.FindByKey(2));
    EXPECT_EQ(lst.end(), lst.FindByKey(3));
}

TEST(Map, FindByValue)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

    EXPECT_EQ(lst.begin(), lst.FindByValue(0));
    EXPECT_EQ(--lst.end(), lst.FindByValue(7));
    EXPECT_EQ(--lst.end(), lst.FindByValue<ops::Greater>(3));
    EXPECT_EQ(lst.end(), lst.FindByValue(42));
}

TEST(Map, Find)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

    EXPECT_EQ(++lst.begin(), lst.Find([](const Map<int, int>::Node &nd) { return (1 - nd.KeyVal.Key); }));
    EXPECT_EQ(lst.end(), lst.Find([](const Map<int, int>::Node &nd) { return (42 - nd.KeyVal.Key); }));
}

TEST(Map, FindMinMax)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

    EXPECT_EQ(lst.FindMin()->Key, 0);
    EXPECT_EQ(lst.FindMax()->Key, 2);
}

TEST(Map, Equal)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };
    Map<int, int> lst1 = { { 0, 0 }, { 1, 3 }, { 2, 7 } };
    Map<int, int> lst2 = { { 0, 0 }, { 1, 3 } };
    Map<int, int> lst3 = { { 0, 0 }, { 1, 3 }, { 2, 4 } };

    EXPECT_TRUE(lst == lst1);
    EXPECT_FALSE(lst != lst1);
    EXPECT_FALSE(lst == lst2);
    EXPECT_TRUE(lst != lst2);
    EXPECT_FALSE(lst == lst3);
    EXPECT_TRUE(lst != lst3);
}

TEST(Map, Concatenate)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };
    Map<int, int> lst1 = { { 3, 0 }, { 1, 5 }, { 4, 7 } };

    auto concatenated = lst + lst1;
    EXPECT_STREQ(*String::ValueOf(concatenated), "{'0': 0, '1': 5, '2': 7, '3': 0, '4': 7}");
    lst1 += lst;
    EXPECT_STREQ(*String::ValueOf(lst1), "{'0': 0, '1': 3, '2': 7, '3': 0, '4': 7}");
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '1': 3, '2': 7}");
}

TEST(Map, HasKey)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_TRUE(lst.HasKey(0));
    EXPECT_TRUE(lst.HasKey(1));
    EXPECT_TRUE(lst.HasKey(2));
}

TEST(Map, Copy)
{
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 3);
    lst.Add(2, 7);
    auto copy = lst;
    EXPECT_EQ(lst[0], 0);
    EXPECT_EQ(lst[1], 3);
    EXPECT_EQ(lst[2], 7);
    EXPECT_EQ(copy[0], 0);
    EXPECT_EQ(copy[1], 3);
    EXPECT_EQ(copy[2], 7);
    copy = lst;
    EXPECT_EQ(copy[0], 0);
    EXPECT_EQ(copy[1], 3);
    EXPECT_EQ(copy[2], 7);
}

TEST(Map, Move)
{
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 3);
    lst.Add(2, 7);
    auto mv = std::move(lst);
    EXPECT_EQ(mv[0], 0);
    EXPECT_EQ(mv[1], 3);
    EXPECT_EQ(mv[2], 7);
    EXPECT_EQ(lst.Size(), 0u);
    EXPECT_EQ(lst.begin(), lst.end());
}

TEST(Map, Remove)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

    lst.Remove(0, false);
    EXPECT_STREQ(*String::ValueOf(lst), "{'1': 3, '2': 7, '3': 0}");
    lst.Add(0, 0);
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '1': 3, '2': 7, '3': 0}");
    lst.Remove(0);
    EXPECT_STREQ(*String::ValueOf(lst), "{'1': 3, '2': 7}");
    lst.Remove<ops::Less>(7);
    EXPECT_STREQ(*String::ValueOf(lst), "{'2': 7}");
}

TEST(Map, RemoveAt)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

    lst.RemoveAt(2);
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '1': 3, '3': 0}");
    lst.RemoveAt(++lst.begin());
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '3': 0}");
    auto it = lst.begin();
    lst.RemoveAt(it);
    EXPECT_STREQ(*String::ValueOf(lst), "{'3': 0}");
    EXPECT_NE(it, lst.end());
    lst.RemoveAt(--lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "{}");
    lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };
    lst.RemoveAt(--(--lst.end()));
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '1': 3, '3': 0}");
    lst.RemoveAt(lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '1': 3, '3': 0}");
}

TEST(Map, Iterator_1)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(Map, Iterator_2)
{
    Map<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(Map, Iterator_3)
{
    Map<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

    auto it = lst.begin();
    it += 2;
    EXPECT_EQ(it->Value, 0);
    it -= 2;
    EXPECT_EQ(it->Value, 3);
    it = lst.begin();
    it += 42;
    EXPECT_EQ(it, lst.end());
    it = lst.end();
    it -= 42;
    EXPECT_EQ(it->Value, 3);
}

TEST(Map, Iterator_4)
{
    Map<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

    auto it1 = lst.begin();
    const auto &it = it1;
    EXPECT_EQ((*it).Value, 3);
    EXPECT_EQ(it->Value, 3);
    EXPECT_EQ(it->Key, 1);
    ++it1;
    EXPECT_EQ((*it).Value, 7);
    EXPECT_EQ(it->Value, 7);
    EXPECT_EQ(it->Key, 2);
}

TEST(Map, Iterator_Special)
{
    Map<int, int> lst = { { 1, 3 } };

    auto it = lst.rbegin();
    EXPECT_EQ((*it).Value, 3);
    EXPECT_EQ(it->Value, 3);
    EXPECT_EQ(it->Key, 1);
    const auto &map  = lst;
    auto it1 = map.rbegin();
    EXPECT_EQ((*it1).Value, 3);
    EXPECT_EQ(it1->Value, 3);
    EXPECT_EQ(it1->Key, 1);
}

TEST(Map, ReverseIterator_1)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(Map, ReverseIterator_2)
{
    Map<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(Map, ReverseIterator_3)
{
    Map<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

    auto it = lst.rbegin();
    it += 2;
    EXPECT_EQ(it->Value, 3);
    it -= 2;
    EXPECT_EQ(it->Value, 0);
    it = lst.rbegin();
    it += 42;
    EXPECT_EQ(it, lst.rend());
    it = lst.rend();
    it -= 42;
    EXPECT_EQ(it->Value, 0);
}

TEST(Map, ReverseIterator_4)
{
    Map<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

    auto it1 = lst.rbegin();
    const auto &it = it1;
    EXPECT_EQ((*it).Value, 0);
    EXPECT_EQ(it->Value, 0);
    EXPECT_EQ(it->Key, 3);
    ++it1;
    EXPECT_EQ((*it).Value, 7);
    EXPECT_EQ(it->Value, 7);
    EXPECT_EQ(it->Key, 2);
}

TEST(Map, CIterator_1)
{
    Map<int, int> lst1 = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };
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

TEST(Map, CIterator_2)
{
    Map<int, int> lst1 = { { 1, 3 }, { 2, 7 }, { 3, 0 } };
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

TEST(Map, CIterator_3)
{
    Map<int, int> lst1 = { { 1, 3 }, { 2, 7 }, { 3, 0 } };
    const auto &lst = lst1;

    auto it = lst.begin();
    it += 2;
    EXPECT_EQ(it->Value, 0);
    it -= 2;
    EXPECT_EQ(it->Value, 3);
    it = lst.begin();
    it += 42;
    EXPECT_EQ(it, lst.end());
    it = lst.end();
    it -= 42;
    EXPECT_EQ(it->Value, 3);
}

TEST(Map, CReverseIterator_1)
{
    Map<int, int> lst1 = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };
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

TEST(Map, CReverseIterator_2)
{
    Map<int, int> lst1 = { { 1, 3 }, { 2, 7 }, { 3, 0 } };
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

TEST(Map, CReverseIterator_3)
{
    Map<int, int> lst1 = { { 1, 3 }, { 2, 7 }, { 3, 0 } };
    const auto &lst = lst1;

    auto it = lst.rbegin();
    it += 2;
    EXPECT_EQ(it->Value, 3);
    it -= 2;
    EXPECT_EQ(it->Value, 0);
    it = lst.rbegin();
    it += 42;
    EXPECT_EQ(it, lst.rend());
    it = lst.rend();
    it -= 42;
    EXPECT_EQ(it->Value, 0);
}

TEST(Map, Clear)
{
    Map<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

    EXPECT_EQ(lst.Size(), 4U);
    lst.Clear();
    EXPECT_EQ(lst.Size(), 0U);
    EXPECT_EQ(lst.begin(), lst.end());
}

TEST(Map, IterateForward_Test1)
{
    int res = 0;
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 3);
    lst.Add(2, 7);
    for (auto &i : lst)
        res += i.Key + i.Value;
    EXPECT_EQ(res, 13);
}

TEST(Map, IterateForward_Test2)
{
    String res = "";
    Map<String, int> map;

    map["test1"] = 0;
    map["test2"] = 3;
    map["test3"] = 7;
    for (auto &i : map)
        res += i.Key + String::ValueOf(i.Value) + ";";
    EXPECT_STREQ(*res, "test10;test23;test37;");
}

TEST(Map, IterateBackward_Test1)
{
    int res = 0;
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 3);
    lst.Add(2, 7);
    for (auto &i : Reverse(lst))
        res += i.Key + i.Value;
    EXPECT_EQ(res, 13);
}

TEST(Map, IterateBackward_Test2)
{
    String res = "";
    Map<String, int> map;

    map["test1"] = 0;
    map["test2"] = 3;
    map["test3"] = 7;
    for (auto &i : Reverse(map))
        res += i.Key + String::ValueOf(i.Value) + ";";
    EXPECT_STREQ(*res, "test37;test23;test10;");
}

TEST(Map, ReadWrite)
{
    Map<String, int> map;

    map["test1"] = 0;
    map["test2"] = 3;
    map["test3"] = 7;
    EXPECT_EQ(map["test1"], 0);
    EXPECT_EQ(map["test2"], 3);
    EXPECT_EQ(map["test3"], 7);
}

TEST(Map, ReadWrite_NonCopy)
{
    Map<String, UniquePtr<int>> map;

    map["test1"] = nullptr;
    map["test2"] = nullptr;
    map["test3"] = nullptr;
    EXPECT_EQ(map["test1"], nullptr);
    EXPECT_EQ(map["test2"], nullptr);
    EXPECT_EQ(map["test3"], nullptr);
    map["test1"] = MakeUnique<int>(0);
    map["test2"] = MakeUnique<int>(5);
    map["test3"] = MakeUnique<int>(9);
    EXPECT_EQ(*map["test1"], 0);
    EXPECT_EQ(*map["test2"], 5);
    EXPECT_EQ(*map["test3"], 9);
}

#ifdef BUILD_DEBUG
static void RunLeakCheckBody()
{
    Map<String, UniquePtr<int>> map;

    map["test1"] = nullptr;
    map["test2"] = nullptr;
    map["test3"] = nullptr;
    EXPECT_EQ(map["test1"], nullptr);
    EXPECT_EQ(map["test2"], nullptr);
    EXPECT_EQ(map["test3"], nullptr);
    map["test1"] = MakeUnique<int>(0);
    map["test2"] = MakeUnique<int>(5);
    map["test3"] = MakeUnique<int>(9);
    EXPECT_EQ(*map["test1"], 0);
    EXPECT_EQ(*map["test2"], 5);
    EXPECT_EQ(*map["test3"], 9);
    map["test3"] = nullptr;
}

TEST(Map, ReadWrite_LeakCheck)
{
    fsize count = Memory::GetAllocCount();
    RunLeakCheckBody();
    EXPECT_EQ(count, Memory::GetAllocCount());
}
#endif

TEST(Map, Swap_1)
{
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 3);
    lst.Add(2, 7);
    lst.Swap(lst.begin(), --(--lst.end()));
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 3, '1': 0, '2': 7}");
}

TEST(Map, Swap_2)
{
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 3);
    lst.Add(2, 7);
    lst.Swap(++lst.begin(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '1': 7, '2': 3}");
}

TEST(Map, Swap_3)
{
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 7);
    lst.Swap(lst.begin(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 7, '1': 0}");
}

TEST(Map, Swap_4)
{
    Map<int, UniquePtr<int>> lst;

    lst.Add(0, MakeUnique<int>(0));
    lst.Add(1, MakeUnique<int>(7));
    lst.Swap(lst.begin(), --lst.end());
}

TEST(Map, Swap_Err_1)
{
    Map<int, UniquePtr<int>> lst;

    lst.Swap(lst.begin(), --lst.end());
}

TEST(Map, Swap_Err_2)
{
    Map<int, int> lst;

    lst.Add(0, 0);
    lst.Add(1, 7);
    lst.Swap(lst.begin(), lst.begin());
    lst.Swap(lst.end(), lst.end());
    lst.Swap(--lst.end(), --lst.end());
    EXPECT_STREQ(*String::ValueOf(lst), "{'0': 0, '1': 7}");
}
