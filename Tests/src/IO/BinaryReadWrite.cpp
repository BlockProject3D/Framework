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
#include <Framework/IO/BinaryReader.hpp>
#include <Framework/IO/BinaryWriter.hpp>
#include <Framework/IO/FileStream.hpp>
#include <Framework/Scalar.hpp>

TEST(BinaryReadWrite, Construct_Writer)
{
    bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE);
    bpf::io::BinaryWriter w(f);
}

TEST(BinaryReadWrite, Construct_Reader)
{
    bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
    bpf::io::BinaryReader r(f);
}

TEST(BinaryReadWrite, ReadWrite_Test_1)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w << 42 << 42.42f << 42.4242 << true;
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        int i;
        float ff;
        double d;
        bool b;

        r >> i >> ff >> d >> b;
        EXPECT_EQ(i, 42);
        EXPECT_EQ(ff, 42.42f);
        EXPECT_EQ(d, 42.4242);
        EXPECT_EQ(b, true);
    }
}

TEST(BinaryReadWrite, ReadWrite_Test_2)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w << bpf::Int64::MaxValue << bpf::Int16::MaxValue << bpf::Int8::MaxValue << bpf::UInt64::MaxValue << bpf::UInt::MaxValue << bpf::UInt16::MaxValue << bpf::UInt8::MaxValue << false;
        w.Flush();
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
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
    }
}

TEST(BinaryReadWrite, ReadWrite_Test_3)
{
    int i;
    float ff;
    double d;
    bool b;
    bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_READ | bpf::io::FILE_MODE_TRUNCATE);
    bpf::io::BinaryWriter w(f);
    bpf::io::BinaryReader r(f);

    w << 42 << 42.42f << 42.4242 << true;
    w.Flush();

    f.Seek(0);
    r >> i >> ff >> d >> b;
    EXPECT_EQ(i, 42);
    EXPECT_EQ(ff, 42.42f);
    EXPECT_EQ(d, 42.4242);
    EXPECT_EQ(b, true);
}

TEST(BinaryReadWrite, ReadWrite_Test_4)
{
    int i;
    float ff;
    double d;
    bool b;
    bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_READ | bpf::io::FILE_MODE_TRUNCATE);
    bpf::io::BinaryWriter w(f, bpf::system::PLATFORM_BIGENDIAN);
    bpf::io::BinaryReader r(f, bpf::system::PLATFORM_BIGENDIAN);

    w << 42 << 42.42f << 42.4242 << true;
    w.Flush();

    f.Seek(0);
    r >> i >> ff >> d >> b;
    EXPECT_EQ(i, 42);
    EXPECT_EQ(ff, 42.42f);
    EXPECT_EQ(d, 42.4242);
    EXPECT_EQ(b, true);
}

TEST(BinaryReadWrite, ReadWrite_Unbuffered)
{
    int i;
    float ff;
    double d;
    bool b;
    bpf::io::ByteBuf buf(4096);
    bpf::io::BinaryWriter w(buf, bpf::system::PLATFORM_BIGENDIAN, false);
    bpf::io::BinaryReader r(buf, bpf::system::PLATFORM_BIGENDIAN, false);

    w << 42 << 42.42f << 42.4242 << true;
    w.Flush();

    buf.Seek(0);
    r >> i >> ff >> d >> b;
    EXPECT_EQ(i, 42);
    EXPECT_EQ(ff, 42.42f);
    EXPECT_EQ(d, 42.4242);
    EXPECT_EQ(b, true);
    buf.Seek(0);
    EXPECT_EQ(w.Write("test", 5), 5U);
    buf.Seek(0);
    char test[5];
    EXPECT_EQ(r.Read(test, 5), 5U);
    EXPECT_STREQ(test, "test");
}

TEST(BinaryReadWrite, ReadWrite_String_Test_1_1)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w << "This is a test";
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_String_Test_1_2)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w << bpf::String("This is a test");
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_String_Test_2_1)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_16);
        w << "This is a test";
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_16);
        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_String_Test_2_2)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_16);
        w << bpf::String("This is a test");
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_16);
        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_String_Test_3_1)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_8);
        w << "This is a test";
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_8);
        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_String_Test_3_2)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_8);
        w << bpf::String("This is a test");
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r.SetStringSerializer(bpf::io::EStringSerializer::VARCHAR_8);
        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_String_Test_4_1)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w.SetStringSerializer(bpf::io::EStringSerializer::CSTYLE);
        w << "This is a test";
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r.SetStringSerializer(bpf::io::EStringSerializer::CSTYLE);
        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_String_Test_4_2)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w.SetStringSerializer(bpf::io::EStringSerializer::CSTYLE);
        w << bpf::String("This is a test");
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        bpf::String str;

        r.SetStringSerializer(bpf::io::EStringSerializer::CSTYLE);
        r >> str;
        EXPECT_STREQ(*str, "This is a test");
    }
}

TEST(BinaryReadWrite, ReadWrite_Raw)
{
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
        bpf::io::BinaryWriter w(f);

        w.Write("this is a test", 14);
    }
    {
        bpf::io::FileStream f(bpf::io::File("./edit_me.txt"), bpf::io::FILE_MODE_READ);
        bpf::io::BinaryReader r(f);
        char out[15];
        out[14] = 0;
        r.Read(out, 14);
        EXPECT_STREQ(out, "this is a test");
    }
}
