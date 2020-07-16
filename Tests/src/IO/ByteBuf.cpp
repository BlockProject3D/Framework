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

#include <iostream>
#include <gtest/gtest.h>
#include <Framework/IO/ByteBuf.hpp>

TEST(ByteBuf, Construct)
{
    bpf::io::ByteBuf buf(1);
    bpf::io::ByteBuf buf1(0);

    EXPECT_EQ(buf.Size(), 1U);
    EXPECT_EQ(buf1.Size(), 0U);
}

TEST(ByteBuf, Index)
{
    bpf::io::ByteBuf buf(1);
    const auto &b = buf;
    EXPECT_THROW(buf[1], bpf::IndexException);
    EXPECT_THROW(b[1], bpf::IndexException);
    buf[0] = 0;
    EXPECT_EQ(buf[0], 0);
    buf[0] = 5;
    EXPECT_EQ(buf[0], 5);
}

TEST(ByteBuf, Move)
{
    bpf::io::ByteBuf buf(128);
    bpf::io::ByteBuf buf1(std::move(buf));

    buf1.Clear();
    EXPECT_EQ(buf1.Size(), 128U);
    EXPECT_EQ(buf.Size(), 0U);
    buf = std::move(buf1);
    EXPECT_EQ(buf.Size(), 128U);
    EXPECT_EQ(buf1.Size(), 0U);
}

TEST(ByteBuf, Copy)
{
    bpf::io::ByteBuf buf(128);
    bpf::io::ByteBuf buf1(buf);

    buf1.Clear();
    EXPECT_EQ(buf1.Size(), 128U);
    EXPECT_EQ(buf.Size(), 128U);
    buf = buf1;
    EXPECT_EQ(buf1.Size(), 128U);
    EXPECT_EQ(buf.Size(), 128U);

    auto buf2 = &buf;
    buf = *buf2;
    EXPECT_EQ(buf1.Size(), 128U);
    EXPECT_EQ(buf.Size(), 128U);
}

TEST(ByteBuf, Clear)
{
    bpf::io::ByteBuf buf(128);

    buf.Clear();
    for (bpf::fsize i = 0 ; i < buf.Size() ; ++i)
        EXPECT_EQ(buf[i], 0);
}

TEST(ByteBuf, ReadWrite_Test1)
{
    bpf::io::ByteBuf buf(128);
    char res[15];

    buf.Clear();
    for (bpf::fsize i = 0 ; i < buf.Size() ; ++i)
        EXPECT_EQ(buf[i], 0);
    EXPECT_EQ(buf.Write("This is a test", 15), (bpf::fsize)15);
    buf.Seek(0);
    EXPECT_EQ(buf.Read(res, 15), (bpf::fsize)15);
    EXPECT_STREQ("This is a test", res);
}

TEST(ByteBuf, ReadWrite_Test2)
{
    bpf::io::ByteBuf buf(4);
    char res[15];

    EXPECT_EQ(buf.Size(), (bpf::fsize)4);
    buf.Clear();
    for (bpf::fsize i = 0 ; i < buf.Size() ; ++i)
        EXPECT_EQ(buf[i], 0);
    EXPECT_EQ(buf.Write("This is a test", 15), (bpf::fsize)4);
    buf.Seek(0);
    EXPECT_EQ(buf.Read(res, 15), (bpf::fsize)4);
    res[4] = '\0';
    EXPECT_STREQ("This", res);
}

TEST(ByteBuf, ReadWrite_Test3)
{
    bpf::io::ByteBuf buf(128);
    char res[15];

    EXPECT_EQ(buf.Size(), (bpf::fsize)128);
    buf.Clear();
    for (bpf::fsize i = 0 ; i < buf.Size() ; ++i)
        EXPECT_EQ(buf[i], 0);
    EXPECT_EQ(buf.Write("This", 4), (bpf::fsize)4);
    EXPECT_EQ(buf.GetCursor(), (bpf::fsize)4);
    EXPECT_EQ(buf.Write("This", 5), (bpf::fsize)5);
    EXPECT_EQ(buf.GetCursor(), (bpf::fsize)9);
    buf.Seek(0);
    EXPECT_EQ(buf.Read(res, 9), (bpf::fsize)9);
    EXPECT_STREQ("ThisThis", res);
}

TEST(ByteBuf, ReadWrite_Test4)
{
    bpf::io::ByteBuf buf(128);
    char res[15];

    EXPECT_EQ(buf.Size(), (bpf::fsize)128);
    buf.Clear();
    for (bpf::fsize i = 0; i < buf.Size(); ++i)
        EXPECT_EQ(buf[i], 0);
    EXPECT_EQ(buf.Write("This", 4), (bpf::fsize)4);
    EXPECT_EQ(buf.GetCursor(), (bpf::fsize)4);
    EXPECT_EQ(buf.Write("This", 5), (bpf::fsize)5);
    EXPECT_EQ(buf.GetCursor(), (bpf::fsize)9);
    buf.Seek(0);
    EXPECT_EQ(buf.Read(res, 9), (bpf::fsize)9);
    EXPECT_STREQ("ThisThis", res);
    buf.Clear();
    EXPECT_EQ(buf.GetCursor(), (bpf::fsize)0);
    EXPECT_EQ(buf.GetWrittenBytes(), (bpf::fsize)0);
    EXPECT_EQ(buf.Size(), (bpf::fsize)128);
    EXPECT_EQ(buf.Write(Null, 0), (bpf::fsize)0);
    EXPECT_EQ(buf.GetCursor(), (bpf::fsize)0);
    EXPECT_EQ(buf.GetWrittenBytes(), (bpf::fsize)0);
    EXPECT_EQ(buf.Read(Null, 0), (bpf::fsize)0);
    EXPECT_EQ(buf.GetCursor(), (bpf::fsize)0);
    EXPECT_EQ(buf.GetWrittenBytes(), (bpf::fsize)0);
}

TEST(ByteBuf, Shift)
{
    bpf::io::ByteBuf buf(5);
    buf.Write("TEST", 5);
    EXPECT_STREQ(reinterpret_cast<const char *>(*buf), "TEST");
    buf.Shift(2);
    buf[4] = '\0';
    EXPECT_STREQ(reinterpret_cast<const char *>(*buf), "ST");
}