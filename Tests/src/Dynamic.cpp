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
#include <Framework/Dynamic.hpp>
#include <Framework/Memory/Utility.hpp>

TEST(Dynamic, Basic_1)
{
    bpf::Dynamic dyn;

    EXPECT_EQ(dyn, Null);
    dyn = -42;
    EXPECT_EQ((int)dyn, -42);
}

TEST(Dynamic, Basic_2)
{
    bpf::Dynamic dyn = 42;

    EXPECT_EQ((int)dyn, 42);
    dyn = -42;
    EXPECT_EQ((int)dyn, -42);
}

TEST(Dynamic, Basic_3)
{
    bpf::Dynamic dyn = Null;

    EXPECT_EQ(dyn, Null);
    EXPECT_THROW((void)((int)dyn), bpf::memory::ClassCastException);
    dyn = -42.42;
    EXPECT_EQ((double)dyn, -42.42);
}

TEST(Dynamic, Copy)
{
    bpf::Dynamic dyn = 42;

    EXPECT_EQ((int)dyn, 42);
    dyn = -42;
    EXPECT_EQ((int)dyn, -42);
    auto copy = dyn;
    EXPECT_EQ((int)copy, -42);
    copy = 42;
    EXPECT_EQ((int)copy, 42);
    dyn = copy;
    EXPECT_EQ((int)dyn, 42);
}

TEST(Dynamic, Move_1)
{
    bpf::Dynamic dyn = 42;

    EXPECT_EQ((int)dyn, 42);
    dyn = -42;
    EXPECT_EQ((int)dyn, -42);
    auto copy = std::move(dyn);
    EXPECT_EQ(dyn, Null); //NOLINT
    EXPECT_NE(copy, Null);
    EXPECT_EQ((int)copy, -42);
    copy = 42;
    EXPECT_EQ((int)copy, 42);
}

TEST(Dynamic, Move_2)
{
    bpf::Dynamic dyn = 42;

    EXPECT_EQ((int)dyn, 42);
    dyn = -42;
    EXPECT_EQ((int)dyn, -42);
    auto copy = std::move(dyn);
    EXPECT_EQ((int)copy, -42);
    copy = 42;
    EXPECT_EQ((int)copy, 42);
    dyn = std::move(copy);
    EXPECT_NE(dyn, Null);
    EXPECT_EQ(copy, Null); //NOLINT
    EXPECT_EQ((int)dyn, 42);
    EXPECT_THROW((void)((float)dyn), bpf::memory::ClassCastException);
    auto copy1 = copy;
    EXPECT_EQ(copy1, Null); //NOLINT
}

TEST(Dynamic, ComplexType)
{
    bpf::Dynamic dyn = bpf::String("My String");
    bpf::String str = dyn;

    EXPECT_STREQ(*str, "My String");
    dyn = Null;
    EXPECT_EQ(dyn, Null);
}

TEST(Dynamic, NonCopy)
{
    bpf::Dynamic mem = bpf::memory::MakeUnique<int>(42);
    auto mv = std::move(mem);

    EXPECT_NE(mv, Null);
    EXPECT_EQ(mem, Null); //NOLINT
    bpf::memory::UniquePtr<int> &ptr = mv;
    EXPECT_EQ(*ptr, 42);
    auto copy = mv;
    EXPECT_EQ(copy, Null);
}
