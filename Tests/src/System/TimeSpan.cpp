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

#include <Framework/System/TimeSpan.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

TEST(TimeSpan, Creation)
{
    bpf::system::TimeSpan ts(1, 2, 50, 40);

    EXPECT_EQ(ts.Days, 1);
    EXPECT_EQ(ts.Hours, 2);
    EXPECT_EQ(ts.Minutes, 50);
    EXPECT_EQ(ts.Seconds, 40);
    EXPECT_EQ(ts.TotalSeconds, 96840U);
}

TEST(TimeSpan, Addition)
{
    bpf::system::TimeSpan ts(1, 0, 0, 0);
    auto res = ts + bpf::system::TimeSpan(1, 0, 0, 0);

    EXPECT_EQ(res.Days, 2);
    EXPECT_EQ(res.Hours, 0);
    EXPECT_EQ(res.Minutes, 0);
    EXPECT_EQ(res.Seconds, 0);
    EXPECT_EQ(res.TotalSeconds, 172800U);
    EXPECT_EQ(ts.TotalSeconds, 86400U);
    ts += bpf::system::TimeSpan(1, 0, 0, 0);
    EXPECT_EQ(ts.TotalSeconds, 172800U);
}

TEST(TimeSpan, Subtraction)
{
    bpf::system::TimeSpan ts(3, 0, 0, 0);
    auto res = ts - bpf::system::TimeSpan(1, 0, 0, 0);

    EXPECT_EQ(res.Days, 2);
    EXPECT_EQ(res.Hours, 0);
    EXPECT_EQ(res.Minutes, 0);
    EXPECT_EQ(res.Seconds, 0);
    EXPECT_EQ(res.TotalSeconds, 172800U);
    EXPECT_EQ(ts.TotalSeconds, 259200U);
    ts -= bpf::system::TimeSpan(1, 0, 0, 0);
    EXPECT_EQ(ts.TotalSeconds, 172800U);
}

TEST(TimeSpan, Compare)
{
    bpf::system::TimeSpan ts(1, 0, 0, 0);
    bpf::system::TimeSpan ts1(2, 0, 0, 0);

    EXPECT_GT(ts1, ts);
    EXPECT_LT(ts, ts1);
    EXPECT_EQ(ts, bpf::system::TimeSpan(1, 0, 0, 0));
    EXPECT_NE(ts, ts1);
}