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
#include <Framework/Event.hpp>
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

    int TestFunc2() const
    {
        return (_val);
    }
};

TEST(Event, Basic_NonConst)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(42);
    bpf::Event<int()> ev;
    ev += bpf::Delegate<int()>(&MyObject::TestFunc, ptr.Raw());
    EXPECT_EQ(ev.GetEventCount(), 1u);
    ev.Invoke();
    EXPECT_EQ(ev.GetEventCount(), 1u);
}

TEST(Event, Basic_Const)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(42);
    bpf::Event<int() const> ev;
    ev += bpf::Delegate<int() const>(&MyObject::TestFunc2, ptr.Raw());
    EXPECT_EQ(ev.GetEventCount(), 1u);
    ev.Invoke();
    EXPECT_EQ(ev.GetEventCount(), 1u);
}

TEST(Event, Safety_NonConst)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(42);
    bpf::Event<int()> ev;
    ev += bpf::Delegate<int()>(&MyObject::TestFunc, ptr.Raw());
    ptr = nullptr;
    EXPECT_EQ(ev.GetEventCount(), 1u);
    ev.Invoke();
    EXPECT_EQ(ev.GetEventCount(), 0u);
}

TEST(Event, Safety_Const)
{
    auto ptr = bpf::memory::MakeUnique<MyObject>(42);
    bpf::Event<int() const> ev;
    ev += bpf::Delegate<int() const>(&MyObject::TestFunc2, ptr.Raw());
    ptr = nullptr;
    EXPECT_EQ(ev.GetEventCount(), 1u);
    ev.Invoke();
    EXPECT_EQ(ev.GetEventCount(), 0u);
}
