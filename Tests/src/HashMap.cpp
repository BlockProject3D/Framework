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
#include <Framework/Memory/Memory.hpp>
#include <Framework/HashMap.hpp>
#include <Framework/Stringifier.Container.hpp>

TEST(HashMap, Creation_1)
{
    bpf::HashMap<bpf::String, int> map;

    map["test1"] = 0;
    map["test2"] = 3;
    map["test3"] = 7;
}

TEST(HashMap, Creation_2)
{
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 3);
	lst.Add(2, 7);
	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
}

TEST(HashMap, PreHash)
{
	EXPECT_NE(bpf::Hash<bpf::String>::HashCode("B"), bpf::Hash<bpf::String>::HashCode("C"));
	EXPECT_NE(bpf::Hash<bpf::String>::HashCode("a"), bpf::Hash<bpf::String>::HashCode("b"));
	EXPECT_NE(bpf::Hash<bpf::String>::HashCode("a"), bpf::Hash<bpf::String>::HashCode("aa"));
	EXPECT_NE(bpf::Hash<bpf::String>::HashCode("a"), bpf::Hash<bpf::String>::HashCode("a\a"));
}

TEST(HashMap, Creation_List)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
}

TEST(HashMap, Add)
{
	const int i = 12;
	bpf::HashMap<int, int> lst = { { 0, i }, { 1, 2 } };

	EXPECT_EQ(lst.Size(), 2);
	lst.Add(2, 3);
	EXPECT_EQ(lst.Size(), 3);
	lst.Add(3, i);
	EXPECT_EQ(lst.Size(), 4);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 12, '1': 2, '2': 3, '3': 12}");
}

TEST(HashMap, Indexer)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
}

TEST(HashMap, HasKey)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 } };

	EXPECT_EQ(lst[0], 0);
	EXPECT_EQ(lst[1], 3);
	EXPECT_EQ(lst[2], 7);
	EXPECT_TRUE(lst.HasKey(0));
	EXPECT_TRUE(lst.HasKey(1));
	EXPECT_TRUE(lst.HasKey(2));
}

TEST(HashMap, Copy)
{
	bpf::HashMap<int, int> lst;

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
}

TEST(HashMap, Move)
{
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 3);
	lst.Add(2, 7);
	auto mv = std::move(lst);
	EXPECT_EQ(mv[0], 0);
	EXPECT_EQ(mv[1], 3);
	EXPECT_EQ(mv[2], 7);
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_EQ(lst.begin(), lst.end());
}

TEST(HashMap, Remove)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

	lst.Remove(0, false);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'1': 3, '2': 7, '3': 0}");
	lst.Add(0, 0);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 0, '1': 3, '2': 7, '3': 0}");
	lst.Remove(0);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'1': 3, '2': 7}");
}

TEST(HashMap, RemoveAt)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

	lst.RemoveAt(2);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 0, '1': 3, '3': 0}");
	lst.RemoveAt(++lst.begin());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 0, '3': 0}");
	auto it = lst.begin();
	lst.RemoveAt(it);
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'3': 0}");
	EXPECT_NE(it, lst.end());
	lst.RemoveAt(--lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{}");
	lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };
	lst.RemoveAt(--(--lst.end()));
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 0, '1': 3, '3': 0}");
	lst.RemoveAt(lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 0, '1': 3, '3': 0}");
}

TEST(HashMap, Iterator_1)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(HashMap, Iterator_2)
{
	bpf::HashMap<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(HashMap, ReverseIterator_1)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(HashMap, ReverseIterator_2)
{
	bpf::HashMap<int, int> lst = { { 1, 3 }, { 2, 7 }, { 3, 0 } };

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

TEST(HashMap, Clear)
{
	bpf::HashMap<int, int> lst = { { 0, 0 }, { 1, 3 }, { 2, 7 }, { 3, 0 } };

	EXPECT_EQ(lst.Size(), 4);
	lst.Clear();
	EXPECT_EQ(lst.Size(), 0);
	EXPECT_EQ(lst.begin(), lst.end());
}

TEST(HashMap, IterateForward_Test1)
{
	int res = 0;
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 3);
	lst.Add(2, 7);
	for (auto &i : lst)
		res += i.Key + i.Value;
	EXPECT_EQ(res, 13);
}

TEST(HashMap, IterateForward_Test2)
{
	bpf::String res = bpf::String::Empty;
	bpf::HashMap<bpf::String, int> map;

	map["test1"] = 0;
	map["test2"] = 3;
	map["test3"] = 7;
	for (auto &i : map)
		res += i.Key + bpf::String::ValueOf(i.Value) + ";";
	EXPECT_STREQ(*res, "test37;test10;test23;");
}

TEST(HashMap, IterateBackward_Test1)
{
	int res = 0;
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 3);
	lst.Add(2, 7);
	for (auto &i : bpf::Reverse(lst))
		res += i.Key + i.Value;
	EXPECT_EQ(res, 13);
}

TEST(HashMap, IterateBackward_Test2)
{
	bpf::String res = bpf::String::Empty;
	bpf::HashMap<bpf::String, int> map;

	map["test1"] = 0;
	map["test2"] = 3;
	map["test3"] = 7;
	for (auto &i : bpf::Reverse(map))
		res += i.Key + bpf::String::ValueOf(i.Value) + ";";
	EXPECT_STREQ(*res, "test23;test10;test37;");
}

TEST(HashMap, ReadWrite)
{
    bpf::HashMap<bpf::String, int> map;

    map["test1"] = 0;
    map["test2"] = 3;
    map["test3"] = 7;
    EXPECT_EQ(map["test1"], 0);
    EXPECT_EQ(map["test2"], 3);
    EXPECT_EQ(map["test3"], 7);
}

TEST(HashMap, ReadWrite_NonCopy)
{
    bpf::HashMap<bpf::String, bpf::UniquePtr<int>> map;

    map["test1"] = Null;
    map["test2"] = Null;
    map["test3"] = Null;
    EXPECT_EQ(map["test1"], Null);
    EXPECT_EQ(map["test2"], Null);
    EXPECT_EQ(map["test3"], Null);
    map["test1"] = bpf::MakeUnique<int>(0);
    map["test2"] = bpf::MakeUnique<int>(5);
    map["test3"] = bpf::MakeUnique<int>(9);
    EXPECT_EQ(*map["test1"], 0);
    EXPECT_EQ(*map["test2"], 5);
    EXPECT_EQ(*map["test3"], 9);
}

static void RunLeakCheckBody()
{
    bpf::HashMap<bpf::String, bpf::UniquePtr<int>> map;

    map["test1"] = Null;
    map["test2"] = Null;
    map["test3"] = Null;
    EXPECT_EQ(map["test1"], Null);
    EXPECT_EQ(map["test2"], Null);
    EXPECT_EQ(map["test3"], Null);
    map["test1"] = bpf::MakeUnique<int>(0);
    map["test2"] = bpf::MakeUnique<int>(5);
    map["test3"] = bpf::MakeUnique<int>(9);
    EXPECT_EQ(*map["test1"], 0);
    EXPECT_EQ(*map["test2"], 5);
    EXPECT_EQ(*map["test3"], 9);
    map["test3"] = Null;
}

TEST(HashMap, ReadWrite_LeakCheck)
{
    bpf::fsize count = bpf::Memory::GetAllocCount();
    RunLeakCheckBody();
    EXPECT_EQ(count, bpf::Memory::GetAllocCount());
}

TEST(HashMap, Swap_1)
{
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 3);
	lst.Add(2, 7);
	lst.Swap(lst.begin(), --(--lst.end()));
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 3, '1': 0, '2': 7}");
}

TEST(HashMap, Swap_2)
{
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 3);
	lst.Add(2, 7);
	lst.Swap(++lst.begin(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 0, '1': 7, '2': 3}");
}

TEST(HashMap, Swap_3)
{
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 7);
	lst.Swap(lst.begin(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 7, '1': 0}");
}

TEST(HashMap, Swap_4)
{
	bpf::HashMap<int, bpf::UniquePtr<int>> lst;

	lst.Add(0, bpf::MakeUnique<int>(0));
	lst.Add(1, bpf::MakeUnique<int>(7));
	lst.Swap(lst.begin(), --lst.end());
}

TEST(HashMap, Swap_Err_1)
{
	bpf::HashMap<int, bpf::UniquePtr<int>> lst;

	lst.Swap(lst.begin(), --lst.end());
}

TEST(HashMap, Swap_Err_2)
{
	bpf::HashMap<int, int> lst;

	lst.Add(0, 0);
	lst.Add(1, 7);
	lst.Swap(lst.begin(), lst.begin());
	lst.Swap(lst.end(), lst.end());
	lst.Swap(--lst.end(), --lst.end());
	EXPECT_STREQ(*bpf::String::ValueOf(lst), "{'0': 0, '1': 7}");
}
