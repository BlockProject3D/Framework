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
#include <Framework/Delegate.hpp>
#include <gtest/gtest.h>

class MyObject : public bpf::memory::Object
{
private:
    int _val;

public:
    explicit MyObject(int val)
        : _val(val)
    {
    }

    int TestFunc()
    {
        return (_val);
    }

    void TestFunc1(int &res) //NOLINT
    {
        res = _val;
    }

    int TestFunc2() const //NOLINT
    {
        return (_val);
    }
};

TEST(UniquePtr, MemberCall_1)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(-42);
    auto func = &MyObject::TestFunc;
    auto res = (ptr->*func)();
    EXPECT_EQ(res, -42);
}

TEST(SharedPtr, MemberCall_1)
{
    auto ptr = bpf::memory::MakeShared<MyObject>(-42);
    auto func = &MyObject::TestFunc;
    auto res = (ptr->*func)();
    EXPECT_EQ(res, -42);
}

TEST(ObjectPtr, MemberCall_1)
{
    auto ptr1 = bpf::memory::MakeUnique<MyObject>(-42);
    auto ptr = bpf::memory::ObjectPtr<MyObject>(ptr1.Raw());
    auto func = &MyObject::TestFunc;
    auto res = (ptr->*func)();
    EXPECT_EQ(res, -42);
}

TEST(UniquePtr, MemberCall_2)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(-42);
    auto func = &MyObject::TestFunc1;
    int res = 0;
    (ptr->*func)(res);
    EXPECT_EQ(res, -42);
}

TEST(SharedPtr, MemberCall_2)
{
    auto ptr = bpf::memory::MakeShared<MyObject>(-42);
    auto func = &MyObject::TestFunc1;
    int res = 0;
    (ptr->*func)(res);
    EXPECT_EQ(res, -42);
}

TEST(ObjectPtr, MemberCall_2)
{
    auto ptr1 = bpf::memory::MakeUnique<MyObject>(-42);
    auto ptr = bpf::memory::ObjectPtr<MyObject>(ptr1.Raw());
    auto func = &MyObject::TestFunc1;
    int res = 0;
    (ptr->*func)(res);
    EXPECT_EQ(res, -42);
}

TEST(UniquePtr, MemberCall_3)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(-42);
    auto func = &MyObject::TestFunc2;
    auto res = (ptr->*func)();
    EXPECT_EQ(res, -42);
}

TEST(SharedPtr, MemberCall_3)
{
    auto ptr = bpf::memory::MakeShared<MyObject>(-42);
    auto func = &MyObject::TestFunc2;
    auto res = (ptr->*func)();
    EXPECT_EQ(res, -42);
}

TEST(ObjectPtr, MemberCall_3)
{
    auto ptr1 = bpf::memory::MakeUnique<MyObject>(-42);
    auto ptr = bpf::memory::ObjectPtr<MyObject>(ptr1.Raw());
    auto func = &MyObject::TestFunc2;
    auto res = (ptr->*func)();
    EXPECT_EQ(res, -42);
}

TEST(Delegate, Basic_1)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(-42);
    auto delegate = bpf::Delegate<int()>(&MyObject::TestFunc, ptr.Raw());
    auto delegate1 = bpf::Delegate<void(int &)>(&MyObject::TestFunc1, ptr.Raw());
    auto delegate2 = bpf::Delegate<int() const>(&MyObject::TestFunc2, ptr.Raw());
    auto delegate3 = bpf::Delegate<void()>();
    auto delegate4 = bpf::Delegate<void() const>();
    EXPECT_TRUE(delegate);
    auto val = delegate();
    EXPECT_EQ(val, -42);
    val = -12;
    EXPECT_EQ(val, -12);
    delegate1(val);
    EXPECT_EQ(val, -42);
    val = delegate2();
    EXPECT_EQ(val, -42);
    EXPECT_FALSE(delegate3);
    EXPECT_THROW(delegate3(), bpf::RuntimeException);
    EXPECT_FALSE(delegate4);
    EXPECT_THROW(delegate4(), bpf::RuntimeException);
    auto d = delegate4;
    EXPECT_FALSE(d);
    EXPECT_THROW(d(), bpf::RuntimeException);
    auto d1 = std::move(delegate4);
    EXPECT_FALSE(d1);
    EXPECT_THROW(d1(), bpf::RuntimeException);
}

TEST(Delegate, Basic_2)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(-42);
    auto objptr = bpf::memory::ObjectPtr<MyObject>(ptr.Raw());
    auto delegate = bpf::Delegate<int()>(&MyObject::TestFunc, objptr);
    auto delegate1 = bpf::Delegate<void(int &)>(&MyObject::TestFunc1, objptr);
    auto delegate2 = bpf::Delegate<int() const>(&MyObject::TestFunc2, objptr);
    auto delegate3 = bpf::Delegate<void()>();
    auto delegate4 = bpf::Delegate<void() const>();
    EXPECT_TRUE(delegate);
    auto val = delegate();
    EXPECT_EQ(val, -42);
    val = -12;
    EXPECT_EQ(val, -12);
    delegate1(val);
    EXPECT_EQ(val, -42);
    val = delegate2();
    EXPECT_EQ(val, -42);
    EXPECT_FALSE(delegate3);
    EXPECT_THROW(delegate3(), bpf::RuntimeException);
    EXPECT_FALSE(delegate4);
    EXPECT_THROW(delegate4(), bpf::RuntimeException);
    auto d = delegate4;
    EXPECT_FALSE(d);
    EXPECT_THROW(d(), bpf::RuntimeException);
    auto d1 = std::move(delegate4);
    EXPECT_FALSE(d1);
    EXPECT_THROW(d1(), bpf::RuntimeException);
}

TEST(Delegate, Basic_3)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(-42);
    auto delegate = bpf::Delegate<int()>(&MyObject::TestFunc, bpf::memory::ObjectPtr<MyObject>(ptr.Raw()));
    auto delegate1 = bpf::Delegate<void(int &)>(&MyObject::TestFunc1, bpf::memory::ObjectPtr<MyObject>(ptr.Raw()));
    auto delegate2 = bpf::Delegate<int() const>(&MyObject::TestFunc2, bpf::memory::ObjectPtr<MyObject>(ptr.Raw()));
    auto delegate3 = bpf::Delegate<void()>();
    auto delegate4 = bpf::Delegate<void() const>();
    EXPECT_TRUE(delegate);
    auto val = delegate();
    EXPECT_EQ(val, -42);
    val = -12;
    EXPECT_EQ(val, -12);
    delegate1(val);
    EXPECT_EQ(val, -42);
    val = delegate2();
    EXPECT_EQ(val, -42);
    EXPECT_FALSE(delegate3);
    EXPECT_THROW(delegate3(), bpf::RuntimeException);
    EXPECT_FALSE(delegate4);
    EXPECT_THROW(delegate4(), bpf::RuntimeException);
    auto d = delegate4;
    EXPECT_FALSE(d);
    EXPECT_THROW(d(), bpf::RuntimeException);
    auto d1 = std::move(delegate4);
    EXPECT_FALSE(d1);
    EXPECT_THROW(d1(), bpf::RuntimeException);
}
