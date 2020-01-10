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
#include <Framework/Queue.hpp>
#include <Framework/Memory/Memory.hpp>

TEST(Queue, Creation)
{
    bpf::Queue<int> queue(8);

    queue.Push(0);
    queue.Push(42);
    queue.Push(-1);

    EXPECT_EQ(queue.Top(), 0);
    EXPECT_EQ(queue.Size(), 3);
}

TEST(Queue, Creation_List)
{
    bpf::Queue<int> queue = { 0, 42, -1 };

    EXPECT_EQ(queue.Top(), 0);
    EXPECT_EQ(queue.Size(), 3);
}

TEST(Queue, Push_Pop_Limited)
{
    bpf::Queue<int> queue(3);

    queue.Push(0);
    queue.Push(42);
    queue.Push(-1);

    EXPECT_EQ(queue.Top(), 0);
    EXPECT_EQ(queue.Size(), 3);
    queue.Push(1);
    EXPECT_EQ(queue.Pop(), 1);
    EXPECT_THROW(queue.Pop(), bpf::IndexException);
}

TEST(Queue, Push_Pop_Unlimited)
{
    bpf::Queue<int> queue;

    queue.Push(0);
    queue.Push(42);
    queue.Push(-1);

    EXPECT_EQ(queue.Top(), 0);
    EXPECT_EQ(queue.Size(), 3);
    queue.Push(1);
    EXPECT_EQ(queue.Size(), 4);
    EXPECT_EQ(queue.Top(), 0);
    EXPECT_EQ(queue.Pop(), 0);
    EXPECT_EQ(queue.Pop(), 42);
    EXPECT_EQ(queue.Pop(), -1);
    EXPECT_EQ(queue.Pop(), 1);
    EXPECT_THROW(queue.Pop(), bpf::IndexException);
}

TEST(Queue, Push_Pop_NonCopy)
{
    bpf::Queue<bpf::UniquePtr<int>> queue;

    queue.Push(bpf::MakeUnique<int>(0));
    queue.Push(bpf::MakeUnique<int>(42));
    queue.Push(bpf::MakeUnique<int>(-1));

    EXPECT_EQ(*queue.Top(), 0);
    EXPECT_EQ(queue.Size(), 3);
    queue.Push(bpf::MakeUnique<int>(1));
    EXPECT_EQ(queue.Size(), 4);
    EXPECT_EQ(*queue.Top(), 0);
    EXPECT_EQ(*queue.Pop(), 0);
    EXPECT_EQ(*queue.Pop(), 42);
    EXPECT_EQ(*queue.Pop(), -1);
    EXPECT_EQ(*queue.Pop(), 1);
    EXPECT_THROW(queue.Pop(), bpf::IndexException);
}