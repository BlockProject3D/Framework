// Copyright (c) 2020, BlockProject
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
#include <Framework/IO/TextReader.hpp>
#include <Framework/IO/TextWriter.hpp>
#include <Framework/IO/FileStream.hpp>
#include <Framework/Scalar.hpp>

TEST(TextReadWrite, Construct_Writer)
{
    bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE);
    bpf::io::TextWriter w(f);
}

TEST(TextReadWrite, Construct_Reader)
{
    bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
    bpf::io::TextReader r(f);
}

TEST(TextReadWrite, ReadWrite_Test_1)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::TextWriter w(f);

        w << 42 << 42.42f << 42.4242 << true;
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::TextReader r(f);
        int i;
        float ff;
        double d;
        bool b;

        r >> i >> ff >> d >> b;
        EXPECT_EQ(i, 42);
        EXPECT_EQ(ff, 42.42f);
        EXPECT_EQ(d, 42.4242);
        EXPECT_EQ(b, true);
        r >> i >> ff >> d >> b;
        EXPECT_EQ(i, 0);
        EXPECT_EQ(ff, 0);
        EXPECT_EQ(d, 0);
        EXPECT_EQ(b, false);
    }
}

TEST(TextReadWrite, ReadWrite_Test_2)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::TextWriter w(f);

        w << bpf::Int64::MaxValue << bpf::Int16::MaxValue << bpf::Int8::MaxValue << bpf::UInt64::MaxValue << bpf::UInt::MaxValue << bpf::UInt16::MaxValue << bpf::UInt8::MaxValue << false;
        w.Flush();
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::TextReader r(f);
        bpf::int64 a;
        bpf::int16 b;
        bpf::int8 c;
        bpf::uint64 d;
        bpf::uint32 e;
        bpf::uint16 g;
        bpf::uint8 h;
        bool i;

        r >> a >> b >> c >> d >> e >> g >> h >> i;
        EXPECT_EQ(a, bpf::Int64::MaxValue);
        EXPECT_EQ(b, bpf::Int16::MaxValue);
        EXPECT_EQ(c, bpf::Int8::MaxValue);
        EXPECT_EQ(d, bpf::UInt64::MaxValue);
        EXPECT_EQ(e, bpf::UInt::MaxValue);
        EXPECT_EQ(g, bpf::UInt16::MaxValue);
        EXPECT_EQ(h, bpf::UInt8::MaxValue);
        EXPECT_FALSE(i);
        r >> a >> b >> c >> d >> e >> g >> h >> i;
        EXPECT_EQ(a, 0);
        EXPECT_EQ(b, 0);
        EXPECT_EQ(c, 0);
        EXPECT_EQ(d, 0);
        EXPECT_EQ(e, 0);
        EXPECT_EQ(g, 0);
        EXPECT_EQ(h, 0);
    }
}

TEST(TextReadWrite, ReadWrite_Test_3)
{
    int i;
    float ff;
    double d;
    bool b;
    bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_READ | bpf::io::FILE_MODE_TRUNCATE);
    bpf::io::TextWriter w(f);
    bpf::io::TextReader r(f);

    w << 42 << 42.42f << 42.4242 << true;
    w.Flush();

    f.Seek(0);
    r >> i >> ff >> d >> b;
    EXPECT_EQ(i, 42);
    EXPECT_EQ(ff, 42.42f);
    EXPECT_EQ(d, 42.4242);
    EXPECT_EQ(b, true);
    r >> i >> ff >> d >> b;
    EXPECT_EQ(i, 0);
    EXPECT_EQ(ff, 0);
    EXPECT_EQ(d, 0);
    EXPECT_EQ(b, false);
}

TEST(TextReadWrite, ReadWrite_String_Test_1_1)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::TextWriter w(f);

        w << "Thisisatest";
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::TextReader r(f);
        bpf::String str;

        r >> str;
        EXPECT_STREQ(*str, "Thisisatest");
        r >> str;
        EXPECT_EQ(str.Size(), 0);
    }
}

TEST(TextReadWrite, ReadWrite_String_Test_1_2)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::TextWriter w(f);

        w << bpf::String("Thisisatest");
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::TextReader r(f);
        bpf::String str;

        r >> str;
        EXPECT_STREQ(*str, "Thisisatest");
        r >> str;
        EXPECT_EQ(str.Size(), 0);
    }
}
