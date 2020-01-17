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
#include <Framework/Stack.hpp>
#include <Framework/Memory/Memory.hpp>

TEST(Stack, Creation)
{
    bpf::Stack<int> stack(8);

    stack.Push(0);
    stack.Push(42);
    stack.Push(-1);

    EXPECT_EQ(stack.Top(), -1);
    EXPECT_EQ(stack.Size(), 3);
}

TEST(Stack, Creation_List)
{
    bpf::Stack<int> stack = { 0, 42, -1 };

    EXPECT_EQ(stack.Top(), -1);
    EXPECT_EQ(stack.Size(), 3);
}

TEST(Stack, Push_Pop_Limited)
{
    bpf::Stack<int> stack(3);

    stack.Push(0);
    stack.Push(42);
    stack.Push(-1);

    EXPECT_EQ(stack.Top(), -1);
    EXPECT_EQ(stack.Size(), 3);
    EXPECT_THROW(stack.Push(1), bpf::StackOverflowException);
    EXPECT_EQ(stack.Pop(), -1);
    EXPECT_EQ(stack.Pop(), 42);
    EXPECT_EQ(stack.Pop(), 0);
    EXPECT_THROW(stack.Pop(), bpf::StackUnderflowException);
}

TEST(Stack, Push_Pop_Unlimited_1)
{
    bpf::Stack<int> stack;

    stack.Push(0);
    stack.Push(42);
    stack.Push(-1);

    EXPECT_EQ(stack.Top(), -1);
    EXPECT_EQ(stack.Size(), 3);
    stack.Push(1);
    EXPECT_EQ(stack.Size(), 4);
    EXPECT_EQ(stack.Top(), 1);
    EXPECT_EQ(stack.Pop(), 1);
    EXPECT_EQ(stack.Pop(), -1);
    EXPECT_EQ(stack.Pop(), 42);
    EXPECT_EQ(stack.Pop(), 0);
    EXPECT_THROW(stack.Pop(), bpf::StackUnderflowException);
}

TEST(Stack, Push_Pop_Unlimited_2)
{
    bpf::Stack<int> stack;

    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    stack.Push(4);
    stack.Push(5);
    stack.Push(6);
    stack.Push(7);
    stack.Push(8);
    stack.Push(9);
    stack.Push(10);
    EXPECT_EQ(stack.Size(), 10);
}

TEST(Stack, Push_Pop_NonCopy)
{
    bpf::Stack<bpf::UniquePtr<int>> stack;

    stack.Push(bpf::MakeUnique<int>(0));
    stack.Push(bpf::MakeUnique<int>(42));
    stack.Push(bpf::MakeUnique<int>(-1));

    EXPECT_EQ(*stack.Top(), -1);
    EXPECT_EQ(stack.Size(), 3);
    stack.Push(bpf::MakeUnique<int>(1));
    EXPECT_EQ(stack.Size(), 4);
    EXPECT_EQ(*stack.Top(), 1);
    EXPECT_EQ(*stack.Pop(), 1);
    EXPECT_EQ(*stack.Pop(), -1);
    EXPECT_EQ(*stack.Pop(), 42);
    EXPECT_EQ(*stack.Pop(), 0);
    EXPECT_THROW(stack.Pop(), bpf::StackUnderflowException);
}