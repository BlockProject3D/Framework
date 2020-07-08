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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/Math/Quaternion.hpp>
#include <Framework/Math/Stringifier.Quaternion.hpp>

namespace bpf
{
    using Quatld = math::Quaternion<long double>;
    using Vector3ld = math::Vector3<long double>;
}

TEST(Quat, Construct)
{
    bpf::Quatld q = bpf::Quatld::Identity;
    bpf::Quatld expected = bpf::Quatld(1, 0, 0, 0);

    EXPECT_EQ(q, expected);
}

TEST(Quat, EulerAngles)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld(bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 4.0));
    bpf::Quatld expected = bpf::Quatld(0.6532815, 0.2705981, 0.6532815, -0.2705981);

    EXPECT_EQ(q, expected);
}

TEST(Quat, AxisAngle)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld::Forward, bpf::math::Mathld::Pi / 2);
    bpf::Quatld expected = bpf::Quatld(0.7071068, 0, 0.7071068, 0);

    EXPECT_EQ(q, expected);
}

TEST(Quat, Angle)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld::Forward, bpf::math::Mathld::Pi / 2);
    auto angle = q.Angle(bpf::Quatld::Identity);

    EXPECT_LE(bpf::math::Mathld::Abs(angle * bpf::math::Mathld::RadToDeg - 90), bpf::math::Mathld::Epsilon);
}

TEST(Quat, Multiply)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld::Forward, bpf::math::Mathld::Pi / 2);
    bpf::Quatld q1 = bpf::Quatld(bpf::Vector3ld::Forward, bpf::math::Mathld::Pi / 2);
    bpf::Quatld expected = bpf::Quatld(0, 0, 1, 0);

    EXPECT_EQ(q * q1, expected);
}

TEST(Quat, Add)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld::Forward, bpf::math::Mathld::Pi / 2);
    bpf::Quatld q1 = bpf::Quatld(bpf::Vector3ld::Forward, bpf::math::Mathld::Pi / 2);
    bpf::Quatld expected = bpf::Quatld(1.4142136, 0, 1.4142136, 0);

    EXPECT_EQ(q + q1, expected);
}

TEST(Quat, Invert_Test1)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld::Forward, bpf::math::Mathld::Pi / 2);
    bpf::Quatld res = q * q.Inverse();
    bpf::Quatld expected = bpf::Quatld::Identity;

    EXPECT_EQ(res, expected);
}

TEST(Quat, Invert_Test2)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld(bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 4.0));
    bpf::Quatld res = q * q.Inverse();
    bpf::Quatld expected = bpf::Quatld::Identity;

    EXPECT_EQ(res, expected);
}

TEST(Quat, Interpolation)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld(bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 4.0));
    bpf::Quatld q1 = bpf::Quatld(bpf::Vector3ld(1, 0, 0), bpf::math::Mathld::Pi / 2.0);
    bpf::Quatld q2 = bpf::Quatld(bpf::Vector3ld(-1, -1, 1), bpf::math::Mathld::Pi / 2.0);

    EXPECT_EQ(bpf::Quatld::Lerp(q, q, 1.0f), q);
    EXPECT_EQ(bpf::Quatld::Lerp(q, q, 0.1f), q);
    EXPECT_EQ(bpf::Quatld::Slerp(q, q, 1.0f), q);
    EXPECT_EQ(bpf::Quatld::Slerp(q, q, 0.1f), q);
    EXPECT_EQ(bpf::Quatld::Slerp(q, q1, 1.0f), q1);
    EXPECT_EQ(bpf::Quatld::Slerp(q, q2, 1.0f), q2);
}

TEST(Quat, Norm)
{
    bpf::Quatld q = bpf::Quatld(bpf::Vector3ld(bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 2.0, bpf::math::Mathld::Pi / 4.0));

    EXPECT_LE(bpf::math::Mathld::Abs(q.Norm()) - 1, bpf::math::Mathld::Epsilon);
    q = q + q + q + q;
    EXPECT_GT(bpf::math::Mathld::Abs(q.Norm()) - 1, bpf::math::Mathld::Epsilon);
    q = q.Normalize();
    EXPECT_LE(bpf::math::Mathld::Abs(q.Norm()) - 1, bpf::math::Mathld::Epsilon);
}

TEST(Quat, Stringifier)
{
    bpf::Quatld q = bpf::Quatld(1, 2, 3, 4);

    EXPECT_STREQ(*bpf::String::ValueOf(q), "Quaternion(1 + 2i + 3j + 4k)");
}
