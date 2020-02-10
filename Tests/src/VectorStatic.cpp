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
#include <Framework/Math/Vector.hpp>

TEST(VectorStatic, Create_1)
{
    bpf::Vector2f v(0, 0);

    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);

    v = { 0, 0 };
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);

    v = bpf::Vector2f(0);
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);

    v = bpf::Vector2f::Zero;
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);

    v = bpf::Vector2f::Identity;
    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);
}

TEST(VectorStatic, Create_2)
{
    bpf::Vector3f v(0, 0, 0);

    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);

    v = { 0, 0, 0 };
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);

    v = bpf::Vector3f(0);
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);

    v = bpf::Vector3f(bpf::Vector2f(0), 0);
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);

    v = bpf::Vector3f::Zero;
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);

    v = bpf::Vector3f::Identity;
    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);
    EXPECT_EQ(v.Z, 1);
}

TEST(VectorStatic, Create_3)
{
    bpf::Vector4f v(0, 0, 0, 0);

    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);
    EXPECT_EQ(v.W, 0);

    v = { 0, 0, 0, 0 };
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);
    EXPECT_EQ(v.W, 0);

    v = bpf::Vector4f(0);
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);
    EXPECT_EQ(v.W, 0);

    v = bpf::Vector4f(bpf::Vector3f(0), 0);
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);
    EXPECT_EQ(v.W, 0);

    v = bpf::Vector4f::Zero;
    EXPECT_EQ(v.X, 0);
    EXPECT_EQ(v.Y, 0);
    EXPECT_EQ(v.Z, 0);
    EXPECT_EQ(v.W, 0);

    v = bpf::Vector4f::Identity;
    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);
    EXPECT_EQ(v.Z, 1);
    EXPECT_EQ(v.W, 1);
}

TEST(VectorStatic, Create_4)
{
    using Vector5f = bpf::Vector<float, 5>;

    Vector5f v = { 0, 0, 0, 0, 0 };
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vector5f(0);
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vector5f(bpf::Vector4f(0), 0);
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vector5f::Zero;
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vector5f::Identity;
    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(v(1), 1);
    EXPECT_EQ(v(2), 1);
    EXPECT_EQ(v(3), 1);
    EXPECT_EQ(v(4), 1);
}
