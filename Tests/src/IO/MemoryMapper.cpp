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

#include <Framework/IO/FileStream.hpp>
#include <Framework/IO/IOException.hpp>
#include <Framework/IO/MemoryMapper.hpp>
#include <Framework/Memory/Memory.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

TEST(MemoryMapper, OpenExcept)
{
    try
    {
        bpf::io::MemoryMapper mapper(bpf::io::File("./doesnotexist.txt"), bpf::io::FILE_MODE_READ);
    }
    catch (const bpf::io::IOException &)
    {
        return;
    }
    ASSERT_TRUE(false);
}

#ifdef BUILD_DEBUG
static void Test_OpenExcept_MemLeak()
{
    try
    {
        bpf::io::MemoryMapper mapper(bpf::io::File("./doesnotexist.txt"), bpf::io::FILE_MODE_READ);
    }
    catch (const bpf::io::IOException &)
    {
        return;
    }
    ASSERT_TRUE(false);
}

TEST(MemoryMapper, OpenExcept_MemLeak)
{
    bpf::fsize cur = bpf::memory::Memory::GetAllocCount();

    Test_OpenExcept_MemLeak();
    EXPECT_EQ(cur, bpf::memory::Memory::GetAllocCount());
}
#endif

static void SetupTestFile(bpf::io::File &f)
{
    bpf::io::FileStream stream(f, bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
    EXPECT_EQ(stream.Write("This is a test", 14), (bpf::fsize)14);
    stream.Close();
}

TEST(MemoryMapper, Map_Test1)
{
    bpf::io::File f("./map_me.txt");
    SetupTestFile(f);
    bpf::io::MemoryMapper mapper(f, bpf::io::FILE_MODE_READ);
    mapper.Map(0, 14);
    char *txt = reinterpret_cast<char *>(*mapper);
    char buf[15];
    memcpy(buf, txt, 14);
    buf[14] = '\0';
    EXPECT_STREQ(buf, "This is a test");
}

TEST(MemoryMapper, Map_Test2)
{
    bpf::io::File f("./map_me.txt");
    SetupTestFile(f);
    bpf::io::MemoryMapper mapper(f, bpf::io::FILE_MODE_READ | bpf::io::FILE_MODE_WRITE);
    mapper.Map(0, 1);
    char *txt = reinterpret_cast<char *>(*mapper);
    EXPECT_EQ(txt[0], 'T');
    mapper.Map(1, 1);
    txt = reinterpret_cast<char *>(*mapper);
    EXPECT_EQ(txt[0], 'h');
}

TEST(MemoryMapper, Map_Test3)
{
    bpf::io::File f("./map_me.txt");
    SetupTestFile(f);
    bpf::io::MemoryMapper mapper(f, bpf::io::FILE_MODE_READ);
    mapper.Map(13, 1);
    char *txt = reinterpret_cast<char *>(*mapper);
    EXPECT_EQ(txt[0], 't');
}

TEST(MemoryMapper, Map_Test4)
{
    bpf::io::File f("./map_me.txt");
    SetupTestFile(f);
    bpf::io::MemoryMapper mapper(f, bpf::io::FILE_MODE_READ);
    try
    {
        mapper.Map(14, 1);
    }
    catch (const bpf::io::IOException &)
    {
        return;
    }
    ASSERT_TRUE(false);
}

TEST(MemoryMapper, Map_Test5)
{
    bpf::io::File f("./map_me.txt");
    SetupTestFile(f);
    bpf::io::MemoryMapper mapper(f, bpf::io::FILE_MODE_READ);
    try
    {
        mapper.Map(100000, 1); //Obviously it's not gonna work :)
    }
    catch (const bpf::io::IOException &)
    {
        return;
    }
    ASSERT_TRUE(false);
}
