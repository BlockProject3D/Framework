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

#include <Framework/Memory/Utility.hpp>
#include <Framework/Memory/Object.hpp>
#include <Framework/Memory/ObjectPtr.hpp>
#include <gtest/gtest.h>

class Parent : public bpf::memory::Object
{
public:
    virtual ~Parent() {}
};

class MyClass1 : public Parent
{
public:
    int Val;

    explicit MyClass1(int a)
        : Val(a)
    {
    }

    MyClass1()
        : Val(0)
    {
    }
};

TEST(UniquePtr, Cast)
{
    bpf::memory::UniquePtr<Parent> ptr = bpf::memory::MakeUnique<MyClass1>(42);
    auto p = dynamic_cast<MyClass1 *>(ptr.Raw());
    EXPECT_EQ(p->Val, 42);
    auto ptr1 = bpf::Cast<MyClass1>(ptr);
    EXPECT_EQ(ptr, nullptr);
    EXPECT_NE(ptr1, nullptr);
    EXPECT_EQ(ptr1->Val, 42);
}

TEST(SharedPtr, Cast_1)
{
    bpf::memory::SharedPtr<Parent> ptr = bpf::memory::MakeShared<MyClass1>(42);
    auto p = dynamic_cast<MyClass1 *>(ptr.Raw());
    EXPECT_EQ(p->Val, 42);
    auto ptr1 = bpf::Cast<MyClass1>(ptr);
    EXPECT_NE(ptr, nullptr);
    EXPECT_NE(ptr1, nullptr);
    EXPECT_EQ(ptr1->Val, 42);
}

TEST(SharedPtr, Cast_2)
{
    bpf::memory::SharedPtr<Parent> ptr = bpf::memory::MakeShared<MyClass1>(42);
    auto p = dynamic_cast<MyClass1 *>(ptr.Raw());
    EXPECT_EQ(p->Val, 42);
    const auto &pp = ptr;
    auto ptr1 = bpf::Cast<MyClass1>(pp);
    EXPECT_NE(ptr, nullptr);
    EXPECT_NE(ptr1, nullptr);
    EXPECT_EQ(ptr1->Val, 42);
}

TEST(ObjectPtr, Cast)
{
    bpf::memory::UniquePtr<Parent> ptr = bpf::memory::MakeUnique<MyClass1>(42);
    auto p = bpf::memory::ObjectPtr<Parent>(ptr.Raw());
    EXPECT_NE(ptr, nullptr);
    EXPECT_NE(p, nullptr);
    bpf::memory::ObjectPtr<MyClass1> p1 = bpf::Cast<MyClass1>(p);
    EXPECT_NE(p1, nullptr);
    EXPECT_EQ(p1->Val, 42);
}

TEST(CustomCast, Builtins_1)
{
    float f = 123.123f;
    EXPECT_EQ(static_cast<bpf::uint32>(f), 123);
    EXPECT_EQ(bpf::Cast<bpf::uint32>(f), 123);
}