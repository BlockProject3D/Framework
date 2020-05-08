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
#include <Framework/Math/MathUtils.hpp>
#include <Framework/Math/Math.hpp>

TEST(Math, Epislon)
{
    EXPECT_EQ(bpf::math::__internal_math::GetEpsilon<float>(), bpf::math::Mathf::Epsilon);
    EXPECT_EQ(bpf::math::__internal_math::GetEpsilon<double>(), bpf::math::Mathd::Epsilon);
    EXPECT_EQ(bpf::math::__internal_math::GetEpsilon<long double>(), bpf::math::Mathld::Epsilon);
    EXPECT_EQ(bpf::math::__internal_math::GetEpsilon<int>(), bpf::math::Math<int>::Epsilon);
    EXPECT_EQ(bpf::math::Math<int>::Epsilon, 0);
}

TEST(Math, Map)
{
    EXPECT_TRUE(bpf::math::Mathf::Map(3000, 1000, 4000, 0, 360) >= 0 && bpf::math::Mathf::Map(3000, 1000, 4000, 0, 360) <= 360);
}

TEST(Math, Clamp)
{
    EXPECT_EQ(bpf::math::Mathf::Clamp(3000, 0, 360), 360);
    EXPECT_EQ(bpf::math::Mathf::Clamp(-2, 0, 360), 0);
    EXPECT_EQ(bpf::math::Mathf::Clamp(2, 0, 360), 2);
}

TEST(MathUtils, IsPowerOfTwo)
{
    EXPECT_TRUE(bpf::math::MathUtils::IsPowerOfTwo(2));
    EXPECT_TRUE(bpf::math::MathUtils::IsPowerOfTwo(1));
    EXPECT_TRUE(bpf::math::MathUtils::IsPowerOfTwo(4));
    EXPECT_TRUE(bpf::math::MathUtils::IsPowerOfTwo(8));
    EXPECT_FALSE(bpf::math::MathUtils::IsPowerOfTwo(7));
    EXPECT_FALSE(bpf::math::MathUtils::IsPowerOfTwo(6));
}

TEST(MathUtils, NextPowerOfTwo)
{
    EXPECT_EQ(bpf::math::MathUtils::FindNextPowerOfTwo(2), 4U);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPowerOfTwo(1), 2U);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPowerOfTwo(7), 8U);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPowerOfTwo(9), 16U);
}

TEST(MathUtils, IsPrime)
{
    EXPECT_TRUE(bpf::math::MathUtils::IsPrime(3));
    EXPECT_TRUE(bpf::math::MathUtils::IsPrime(13));
    EXPECT_TRUE(bpf::math::MathUtils::IsPrime(2));
    EXPECT_TRUE(bpf::math::MathUtils::IsPrime(23));
    EXPECT_FALSE(bpf::math::MathUtils::IsPrime(1));
    EXPECT_FALSE(bpf::math::MathUtils::IsPrime(0));
}

TEST(MathUtils, NextPrime)
{
    EXPECT_EQ(bpf::math::MathUtils::FindNextPrime(2), 3);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPrime(1), 2);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPrime(0), 2);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPrime(3), 5);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPrime(4), 5);
    EXPECT_EQ(bpf::math::MathUtils::FindNextPrime(5), 7);
}