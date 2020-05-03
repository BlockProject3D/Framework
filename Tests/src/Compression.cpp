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

#include <Framework/Compression/ZDeflater.hpp>
#include <Framework/Compression/ZInflater.hpp>
#include <Framework/IO/IOException.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

TEST(Compression, InflateDeflate_Simple_1)
{
    bpf::compression::ZDeflater deflater(bpf::compression::ECompressionLevel::HIGH);
    bpf::compression::ZInflater inflater;
    bpf::io::ByteBuf buf(29);
    bpf::io::ByteBuf deflated(128);
    bpf::uint8 inflated[29];
    bpf::uint8 chunk[8];
    bpf::fsize len = 0;

    buf.Write("This is a testThis is a test", 29);
    deflater.SetInput(buf);
    while ((len = deflater.Deflate(chunk, 8)) > 0)
        deflated.Write(chunk, len);
    inflater.SetInput(std::move(deflated));
    EXPECT_EQ(inflater.Inflate(inflated, 29), 29U);
    EXPECT_STREQ(reinterpret_cast<const char *>(inflated), "This is a testThis is a test");
}

TEST(Compression, InflateDeflate_Simple_2)
{
    bpf::compression::ZDeflater deflater(bpf::compression::ECompressionLevel::DEFAULT);
    bpf::compression::ZInflater inflater;
    bpf::io::ByteBuf buf(29);
    bpf::io::ByteBuf deflated(128);
    bpf::uint8 inflated[29];
    bpf::uint8 chunk[8];
    bpf::fsize len = 0;

    buf.Write("This is a testThis is a test", 29);
    deflater.SetInput(std::move(buf));
    while ((len = deflater.Deflate(chunk, 8)) > 0)
        deflated.Write(chunk, len);
    inflater.SetInput(std::move(deflated));
    EXPECT_EQ(inflater.Inflate(inflated, 29), 29U);
    EXPECT_STREQ(reinterpret_cast<const char *>(inflated), "This is a testThis is a test");
}

TEST(Compression, InflateDeflate_Simple_3)
{
    bpf::compression::ZDeflater deflater(bpf::compression::ECompressionLevel::LOW);
    bpf::compression::ZInflater inflater;
    bpf::io::ByteBuf buf(29);
    bpf::io::ByteBuf deflated(128);
    bpf::uint8 inflated[29];
    bpf::uint8 chunk[8];
    bpf::fsize len = 0;

    buf.Write("This is a testThis is a test", 29);
    deflater.SetInput(buf);
    while ((len = deflater.Deflate(chunk, 8)) > 0)
        deflated.Write(chunk, len);
    inflater.SetInput(deflated);
    EXPECT_EQ(inflater.Inflate(inflated, 29), 29U);
    EXPECT_STREQ(reinterpret_cast<const char *>(inflated), "This is a testThis is a test");
}

TEST(Compression, InflateDeflate_Err)
{
    bpf::compression::ZInflater inflater;
    bpf::io::ByteBuf buf(29);
    bpf::uint8 inflated[29];

    buf.Write("This is a testThis is a test", 29);
    inflater.SetInput(buf);
    EXPECT_THROW(inflater.Inflate(inflated, 29), bpf::io::IOException);
}
