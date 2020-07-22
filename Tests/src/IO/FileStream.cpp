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

#include <Framework/IO/FileStream.hpp>
#include <Framework/IO/IOException.hpp>
#include <Framework/Memory/Memory.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

TEST(FileStream, OpenExcept)
{
    EXPECT_THROW(bpf::io::FileStream(bpf::io::File("./doesnotexist.txt"), bpf::io::FILE_MODE_READ), bpf::io::IOException);
}

#ifdef BUILD_DEBUG
static void Test_OpenExcept_MemLeak()
{
    EXPECT_THROW(bpf::io::FileStream(bpf::io::File("./doesnotexist.txt"), bpf::io::FILE_MODE_READ), bpf::io::IOException);
}

TEST(FileStream, OpenExcept_MemLeak)
{
    bpf::fsize cur = bpf::memory::Memory::GetAllocCount();

    Test_OpenExcept_MemLeak();
    EXPECT_EQ(cur, bpf::memory::Memory::GetAllocCount());
}
#endif

TEST(FileStream, Open_ReadWrite)
{
    bpf::io::File f("./doesnotexist.txt");
    bpf::io::FileStream stream(f, bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
    EXPECT_THROW(stream.Read(nullptr, 0), bpf::io::IOException);
    EXPECT_EQ(stream.Write("This is a test", 14), (bpf::fsize)14);
    stream.Close();
    stream.Close();
    bpf::io::FileStream stream1(f, bpf::io::FILE_MODE_READ);
    EXPECT_THROW(stream1.Write(nullptr, 0), bpf::io::IOException);
    char buf[15];
    EXPECT_EQ(stream1.Read(buf, 14), (bpf::fsize)14);
    buf[14] = '\0';
    EXPECT_STREQ(buf, "This is a test");
    stream1.Close();
    stream1.Close();
    f.Delete();
}

TEST(FileStream, Open_Append)
{
    {
        bpf::io::FileStream stream(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        stream.SeekOffset(0);
        EXPECT_EQ(stream.Write("This is a test\n", 15), (bpf::fsize)15);
    }
    {
        bpf::io::FileStream stream(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_APPEND);
        EXPECT_EQ(stream.Write("3.141592654", 11), (bpf::fsize)11);
        EXPECT_THROW(stream.Seek(0), bpf::io::IOException);
        EXPECT_THROW(stream.SeekOffset(0), bpf::io::IOException);
    }
    {
        bpf::io::FileStream stream(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        char text[27];
        EXPECT_EQ(stream.Read(text, 26), 26U);
        text[26] = 0;
        EXPECT_STREQ(text, "This is a test\n3.141592654");
        stream.SeekOffset(-11);
        EXPECT_EQ(stream.Read(text, 11), 11U);
        text[11] = 0;
        EXPECT_STREQ(text, "3.141592654");
    }
    bpf::io::File("./edit_me.txt").Delete();
}

TEST(FileStream, Unicode)
{
    bpf::io::File f("./doesnotexist.txt");
    bpf::io::FileStream stream(f, bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
    EXPECT_THROW(stream.Read(nullptr, 0), bpf::io::IOException);
    EXPECT_EQ(stream.Write("This is a test你好", 21), (bpf::fsize)21);
    stream.Close();
    stream.Close();
    bpf::io::FileStream stream1(f, bpf::io::FILE_MODE_READ);
    EXPECT_THROW(stream1.Write(nullptr, 0), bpf::io::IOException);
    char buf[22];
    EXPECT_EQ(stream1.Read(buf, 21), (bpf::fsize)21);
    buf[21] = '\0';
    EXPECT_STREQ(buf, "This is a test你好");
    stream1.Close();
    stream1.Close();
    f.Delete();
}
