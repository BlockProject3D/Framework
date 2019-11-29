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
#include <Framework/String.hpp>

TEST(HashMap, Creation)
{
    bpf::HashMap<bpf::String, int> map;

    map["test1"] = 0;
    map["test2"] = 3;
    map["test3"] = 7;
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

TEST(HashMap, IterateForward)
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

TEST(HashMap, IterateBackward)
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
