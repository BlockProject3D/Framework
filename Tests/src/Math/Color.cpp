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

#include <gtest/gtest.h>
#include <Framework/Math/Color.hpp>

TEST(Color, Basic)
{
    auto c = bpf::math::Color(255, 1, 6);
    auto c1 = bpf::math::Color();
    auto c2 = bpf::math::Color(c, 128);
    auto c3 = bpf::math::Color((bpf::uint8)1);

    EXPECT_EQ(c.R, 255);
    EXPECT_EQ(c.G, 1);
    EXPECT_EQ(c.B, 6);
    EXPECT_EQ(c.A, 255);
    EXPECT_EQ(c1, bpf::math::Color::Black);
    EXPECT_NE(c1, bpf::math::Color::Red);
    EXPECT_NE(c1, bpf::math::Color::White);
    EXPECT_EQ(c2.R, 255);
    EXPECT_EQ(c2.G, 1);
    EXPECT_EQ(c2.B, 6);
    EXPECT_EQ(c2.A, 128);
    EXPECT_EQ(c3.R, 1);
    EXPECT_EQ(c3.G, 1);
    EXPECT_EQ(c3.B, 1);
    EXPECT_EQ(c3.A, 255);
}

TEST(Color, Normalize)
{
    auto c = bpf::math::Color(255, 128, 128);

    EXPECT_EQ(c.Normalize(), bpf::math::Vector3f(1.0f, 0.501961f, 0.501961f));
    EXPECT_EQ(c.NormalizeAlpha(), bpf::math::Vector4f(1.0f, 0.501961f, 0.501961f, 1.0f));
}

TEST(Color, Packed)
{
    auto c = bpf::math::Color(0x99CCFF);

    EXPECT_EQ(c.R, 153);
    EXPECT_EQ(c.G, 204);
    EXPECT_EQ(c.B, 255);
    EXPECT_EQ(c.A, 255);
}

TEST(Color, Pack)
{
    auto c = bpf::math::Color(153, 204, 255);

    EXPECT_EQ(c.R, 153);
    EXPECT_EQ(c.G, 204);
    EXPECT_EQ(c.B, 255);
    EXPECT_EQ(c.A, 255);
    EXPECT_EQ(c.Pack(), 0x99CCFF);
}

TEST(Color, Add)
{
    auto c = bpf::math::Color::Red + bpf::math::Color::Green;

    EXPECT_EQ(c, bpf::math::Color::Yellow);
}

TEST(Color, Multiply)
{
    auto c = bpf::math::Color::Red * bpf::math::Color::White;
    auto c1 = bpf::math::Color::Yellow * bpf::math::Color::Red;

    EXPECT_EQ(c, bpf::math::Color::Red);
    EXPECT_EQ(c1, bpf::math::Color::Red);
}
