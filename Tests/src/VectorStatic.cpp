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

using Vector5f = bpf::Vector<float, 5>;

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

    EXPECT_EQ(v.Dim(), 2);
}

TEST(VectorStatic, Indexer_1)
{
    auto v = bpf::Vector2f::Identity;
    const auto &c = v;

    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(c(1), 1);

    EXPECT_THROW(c(2), bpf::IndexException);
    EXPECT_THROW(v(2), bpf::IndexException);
}

TEST(VectorStatic, Operators_1)
{
    auto identity = bpf::Vector2f::Identity;
    auto zero = bpf::Vector2f::Zero;

    EXPECT_EQ(identity * zero, zero);
    EXPECT_EQ(identity * 0, zero);
    EXPECT_EQ(identity / 1, identity);
    EXPECT_EQ(identity - zero, identity);
    EXPECT_EQ(identity + zero, identity);
    EXPECT_EQ(identity + identity, bpf::Vector2f(2));
    EXPECT_EQ(zero / identity, zero);
    identity -= zero;
    identity += zero;
    identity /= identity;
    EXPECT_EQ(identity, bpf::Vector2f::Identity);
    identity *= zero;
    EXPECT_EQ(identity, bpf::Vector2f::Zero);
    identity += bpf::Vector2f::Identity;
    identity += bpf::Vector2f::Identity;
    EXPECT_EQ(identity, bpf::Vector2f(2));
    identity /= 2;
    EXPECT_EQ(identity, bpf::Vector2f::Identity);
    identity *= 2;
    EXPECT_EQ(identity, bpf::Vector2f(2));
    EXPECT_EQ(-identity, bpf::Vector2f(-2));
}

TEST(VectorStatic, Norm_1)
{
    auto vec = bpf::Vector2f(2);

    EXPECT_EQ(vec.NormSquared(), 8);
    EXPECT_GT(vec.Norm(), 2.8);
    EXPECT_LT(vec.Norm(), 2.9);
}

TEST(VectorStatic, Distance_1)
{
    auto v1 = bpf::Vector2f::Identity;
    auto v2 = bpf::Vector2f(2);

    EXPECT_EQ(v1.Distance(v1), 0);
    EXPECT_GT(v1.Distance(v2), 1.4);
    EXPECT_LT(v1.Distance(v2), 1.5);
}

TEST(VectorStatic, CopyMove_1)
{
    bpf::Vector2f v = bpf::Vector2f::Identity;
    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);

    auto copy = v;
    EXPECT_EQ(copy.X, 1);
    EXPECT_EQ(copy.Y, 1);

    copy.X = 0;
    EXPECT_EQ(copy.X, 0);
    EXPECT_EQ(copy.Y, 1);

    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);

    auto mv = std::move(v);
    EXPECT_EQ(mv.X, 1);
    EXPECT_EQ(mv.Y, 1);
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

    EXPECT_EQ(v.Dim(), 3);
}

TEST(VectorStatic, Indexer_2)
{
    auto v = bpf::Vector3f::Identity;
    const auto &c = v;

    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(c(2), 1);

    EXPECT_THROW(c(3), bpf::IndexException);
    EXPECT_THROW(v(3), bpf::IndexException);
}

TEST(VectorStatic, Operators_2)
{
    auto identity = bpf::Vector3f::Identity;
    auto zero = bpf::Vector3f::Zero;

    EXPECT_EQ(identity * zero, zero);
    EXPECT_EQ(identity * 0, zero);
    EXPECT_EQ(identity / 1, identity);
    EXPECT_EQ(identity - zero, identity);
    EXPECT_EQ(identity + zero, identity);
    EXPECT_EQ(identity + identity, bpf::Vector3f(2));
    EXPECT_EQ(zero / identity, zero);
    identity -= zero;
    identity += zero;
    identity /= identity;
    EXPECT_EQ(identity, bpf::Vector3f::Identity);
    identity *= zero;
    EXPECT_EQ(identity, bpf::Vector3f::Zero);
    identity += bpf::Vector3f::Identity;
    identity += bpf::Vector3f::Identity;
    EXPECT_EQ(identity, bpf::Vector3f(2));
    identity /= 2;
    EXPECT_EQ(identity, bpf::Vector3f::Identity);
    identity *= 2;
    EXPECT_EQ(identity, bpf::Vector3f(2));
    EXPECT_EQ(-identity, bpf::Vector3f(-2));
}

TEST(VectorStatic, CopyMove_2)
{
    bpf::Vector3f v = bpf::Vector3f::Identity;
    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);
    EXPECT_EQ(v.Z, 1);

    auto copy = v;
    EXPECT_EQ(copy.X, 1);
    EXPECT_EQ(copy.Y, 1);
    EXPECT_EQ(copy.Z, 1);

    copy.X = 0;
    EXPECT_EQ(copy.X, 0);
    EXPECT_EQ(copy.Y, 1);
    EXPECT_EQ(copy.Z, 1);

    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);
    EXPECT_EQ(v.Z, 1);

    auto mv = std::move(v);
    EXPECT_EQ(mv.X, 1);
    EXPECT_EQ(mv.Y, 1);
    EXPECT_EQ(mv.Z, 1);
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

    EXPECT_EQ(v.Dim(), 4);
}

TEST(VectorStatic, Indexer_3)
{
    auto v = bpf::Vector4f::Identity;
    const auto &c = v;

    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(c(3), 1);

    EXPECT_THROW(c(4), bpf::IndexException);
    EXPECT_THROW(v(4), bpf::IndexException);
}

TEST(VectorStatic, Operators_3)
{
    auto identity = bpf::Vector4f::Identity;
    auto zero = bpf::Vector4f::Zero;

    EXPECT_EQ(identity * zero, zero);
    EXPECT_EQ(identity * 0, zero);
    EXPECT_EQ(identity / 1, identity);
    EXPECT_EQ(identity - zero, identity);
    EXPECT_EQ(identity + zero, identity);
    EXPECT_EQ(identity + identity, bpf::Vector4f(2));
    EXPECT_EQ(zero / identity, zero);
    identity -= zero;
    identity += zero;
    identity /= identity;
    EXPECT_EQ(identity, bpf::Vector4f::Identity);
    identity *= zero;
    EXPECT_EQ(identity, bpf::Vector4f::Zero);
    identity += bpf::Vector4f::Identity;
    identity += bpf::Vector4f::Identity;
    EXPECT_EQ(identity, bpf::Vector4f(2));
    identity /= 2;
    EXPECT_EQ(identity, bpf::Vector4f::Identity);
    identity *= 2;
    EXPECT_EQ(identity, bpf::Vector4f(2));
    EXPECT_EQ(-identity, bpf::Vector4f(-2));
}

TEST(VectorStatic, CopyMove_3)
{
    bpf::Vector4f v = bpf::Vector4f::Identity;
    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);
    EXPECT_EQ(v.Z, 1);
    EXPECT_EQ(v.W, 1);

    auto copy = v;
    EXPECT_EQ(copy.X, 1);
    EXPECT_EQ(copy.Y, 1);
    EXPECT_EQ(copy.Z, 1);
    EXPECT_EQ(copy.W, 1);

    copy.X = 0;
    EXPECT_EQ(copy.X, 0);
    EXPECT_EQ(copy.Y, 1);
    EXPECT_EQ(copy.Z, 1);
    EXPECT_EQ(copy.W, 1);

    EXPECT_EQ(v.X, 1);
    EXPECT_EQ(v.Y, 1);
    EXPECT_EQ(v.Z, 1);
    EXPECT_EQ(v.W, 1);

    auto mv = std::move(v);
    EXPECT_EQ(mv.X, 1);
    EXPECT_EQ(mv.Y, 1);
    EXPECT_EQ(mv.Z, 1);
    EXPECT_EQ(mv.W, 1);
}

TEST(VectorStatic, Create_4)
{
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

    EXPECT_EQ(v.Dim(), 5);
}

TEST(VectorStatic, Indexer_4)
{
    auto v = Vector5f::Identity;
    const auto &c = v;

    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(c(4), 1);

    EXPECT_THROW(c(5), bpf::IndexException);
    EXPECT_THROW(v(5), bpf::IndexException);
}

TEST(VectorStatic, Operators_4)
{
    auto identity = Vector5f::Identity;
    auto zero = Vector5f::Zero;

    EXPECT_EQ(identity * zero, zero);
    EXPECT_EQ(identity * 0, zero);
    EXPECT_EQ(identity / 1, identity);
    EXPECT_EQ(identity - zero, identity);
    EXPECT_EQ(identity + zero, identity);
    EXPECT_EQ(identity + identity, Vector5f(2));
    EXPECT_EQ(zero / identity, zero);
    identity -= zero;
    identity += zero;
    identity /= identity;
    EXPECT_EQ(identity, Vector5f::Identity);
    identity *= zero;
    EXPECT_EQ(identity, Vector5f::Zero);
    identity += Vector5f::Identity;
    identity += Vector5f::Identity;
    EXPECT_EQ(identity, Vector5f(2));
    identity /= 2;
    EXPECT_EQ(identity, Vector5f::Identity);
    identity *= 2;
    EXPECT_EQ(identity, Vector5f(2));
    EXPECT_EQ(-identity, Vector5f(-2));
}

TEST(VectorStatic, CopyMove_4)
{
    Vector5f v = Vector5f::Identity;
    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(v(1), 1);
    EXPECT_EQ(v(2), 1);
    EXPECT_EQ(v(3), 1);
    EXPECT_EQ(v(4), 1);

    auto copy = v;
    EXPECT_EQ(copy(0), 1);
    EXPECT_EQ(copy(1), 1);
    EXPECT_EQ(copy(2), 1);
    EXPECT_EQ(copy(3), 1);
    EXPECT_EQ(copy(4), 1);

    copy(0) = 0;
    EXPECT_EQ(copy(0), 0);
    EXPECT_EQ(copy(1), 1);
    EXPECT_EQ(copy(2), 1);
    EXPECT_EQ(copy(3), 1);
    EXPECT_EQ(copy(4), 1);

    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(v(1), 1);
    EXPECT_EQ(v(2), 1);
    EXPECT_EQ(v(3), 1);
    EXPECT_EQ(v(4), 1);

    auto mv = std::move(v);
    EXPECT_EQ(mv(0), 1);
    EXPECT_EQ(mv(1), 1);
    EXPECT_EQ(mv(2), 1);
    EXPECT_EQ(mv(3), 1);
    EXPECT_EQ(mv(4), 1);
}
