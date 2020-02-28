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

using Vectorf = bpf::math::Vector<float>;

TEST(VectorDynamic, Create)
{
    Vectorf v = { 0, 0, 0, 0, 0 };
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vectorf(5, 0);
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vectorf(Vectorf(4, 0), 0);
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vectorf::Zero(5);
    EXPECT_EQ(v(0), 0);
    EXPECT_EQ(v(1), 0);
    EXPECT_EQ(v(2), 0);
    EXPECT_EQ(v(3), 0);
    EXPECT_EQ(v(4), 0);

    v = Vectorf::Identity(5);
    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(v(1), 1);
    EXPECT_EQ(v(2), 1);
    EXPECT_EQ(v(3), 1);
    EXPECT_EQ(v(4), 1);

    EXPECT_EQ(v.Dim(), 5);
}

TEST(VectorDynamic, Indexer)
{
    auto v = Vectorf::Identity(5);
    const auto &c = v;

    EXPECT_EQ(v(0), 1);
    EXPECT_EQ(c(4), 1);

    EXPECT_THROW(c(5), bpf::IndexException);
    EXPECT_THROW(v(5), bpf::IndexException);
}

TEST(VectorDynamic, Operators)
{
    auto identity = Vectorf::Identity(5);
    auto zero = Vectorf::Zero(5);

    EXPECT_EQ(identity * zero, zero);
    EXPECT_THROW(identity * Vectorf(4), bpf::IncompatibleMatrixSizeException);
    EXPECT_EQ(identity * 0, zero);
    EXPECT_EQ(identity / 1, identity);
    EXPECT_EQ(identity - zero, identity);
    EXPECT_THROW(identity - Vectorf(4), bpf::IncompatibleMatrixSizeException);
    EXPECT_EQ(identity + zero, identity);
    EXPECT_THROW(identity + Vectorf(4), bpf::IncompatibleMatrixSizeException);
    EXPECT_EQ(identity + identity, Vectorf(5, 2));
    EXPECT_EQ(zero / identity, zero);
    EXPECT_THROW(identity / Vectorf(4), bpf::IncompatibleMatrixSizeException);
    identity -= zero;
    EXPECT_THROW(identity -= Vectorf(4), bpf::IncompatibleMatrixSizeException);
    identity += zero;
    EXPECT_THROW(identity += Vectorf(4), bpf::IncompatibleMatrixSizeException);
    identity /= identity;
    EXPECT_THROW(identity /= Vectorf(4), bpf::IncompatibleMatrixSizeException);
    EXPECT_EQ(identity, Vectorf::Identity(5));
    identity *= zero;
    EXPECT_THROW(identity *= Vectorf(4), bpf::IncompatibleMatrixSizeException);
    EXPECT_EQ(identity, Vectorf::Zero(5));
    identity += Vectorf::Identity(5);
    identity += Vectorf::Identity(5);
    EXPECT_EQ(identity, Vectorf(5, 2));
    identity /= 2;
    EXPECT_EQ(identity, Vectorf::Identity(5));
    identity *= 2;
    EXPECT_EQ(identity, Vectorf(5, 2));
    EXPECT_EQ(-identity, Vectorf(5, -2));
    EXPECT_FALSE(Vectorf::Identity(5) == Vectorf::Zero(5));
    EXPECT_THROW((void)(identity == Vectorf(4)), bpf::IncompatibleMatrixSizeException);
}

TEST(VectorDynamic, Norm)
{
    auto vec = Vectorf(5, 2);

    EXPECT_EQ(vec.NormSquared(), 20);
    EXPECT_GT(vec.Norm(), 4.4);
    EXPECT_LT(vec.Norm(), 4.5);
}

TEST(VectorDynamic, Distance)
{
    auto v1 = Vectorf::Identity(5);
    auto v2 = Vectorf(5, 2);

    EXPECT_EQ(v1.Distance(v1), 0);
    EXPECT_GT(v1.Distance(v2), 2.2);
    EXPECT_LT(v1.Distance(v2), 2.3);
    EXPECT_EQ(v1.DistanceSquared(v1), 0);
    EXPECT_EQ(v1.DistanceSquared(v2), 5);
    EXPECT_THROW(v1.Distance(Vectorf(4)), bpf::IncompatibleMatrixSizeException);
    EXPECT_THROW(v1.DistanceSquared(Vectorf(4)), bpf::IncompatibleMatrixSizeException);
}

TEST(VectorDynamic, Normalize)
{
    auto v1 = Vectorf::Identity(5);

    v1.Normalize();
    EXPECT_LT(1 - v1.Norm(), bpf::math::Math::Epsilon<float>());
}

TEST(VectorDynamic, Dot)
{
    auto v1 = Vectorf::Zero(5);
    auto v2 = Vectorf::Identity(5);
    auto v3 = Vectorf(5, 2);

    EXPECT_EQ(v1.Dot(v2), 0);
    EXPECT_EQ(v2.Dot(v3), 10);
    EXPECT_THROW(v1.Dot(Vectorf(4)), bpf::IncompatibleMatrixSizeException);
}

TEST(VectorDynamic, Lerp)
{
    auto v1 = Vectorf::Zero(5);
    auto v2 = Vectorf::Identity(5);
    auto lerp = Vectorf::Lerp(v1, v2, 0.5f);

    EXPECT_EQ(lerp, Vectorf(5, 0.5f));
    EXPECT_THROW(Vectorf::Lerp(v1, Vectorf(4), 0.5f), bpf::IncompatibleMatrixSizeException);
}

TEST(VectorDynamic, CopyMove_4)
{
    Vectorf v = Vectorf::Identity(5);
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
