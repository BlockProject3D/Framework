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
#include <Framework/System/Thread.hpp>
#include <Framework/System/OSException.hpp>

class MyThread : public bpf::system::Thread
{
public:
    bool except;
    bpf::uint32 _value;
    explicit MyThread(bool except)
        : bpf::system::Thread("MyThread")
        , except(except)
        , _value(0) //Yeah will work better when initialized...
    {
    }
    void Run()
    {
        while (IsRunning())
        {
            ++_value;
            bpf::system::Thread::Sleep(250);
            if (except)
                throw bpf::RuntimeException("UnitTest", "A nice exception");
        }
    }
};

TEST(Thread, Basic)
{
    MyThread thread(false);

    EXPECT_STREQ(*thread.GetName(), "MyThread");
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::PENDING);
    thread.Kill(); //Should just silently return as the thread is not yet running
    thread.Start();
    thread.Start(); //Should just silently return as the thread already runs
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread.Kill();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::EXITING);
    thread.Join();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)2, (bpf::uint32)1);
}

TEST(Thread, Move_1)
{
    MyThread thread(false);

    EXPECT_STREQ(*thread.GetName(), "MyThread");
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::PENDING);
    thread.Start();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::RUNNING);
    EXPECT_THROW(MyThread th(std::move(thread)), bpf::system::OSException);
    bpf::system::Thread::Sleep(500);
    thread.Kill();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::EXITING);
    thread.Join();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)2, (bpf::uint32)1);
}

TEST(Thread, Move_2)
{
    MyThread thread(false);

    EXPECT_STREQ(*thread.GetName(), "MyThread");
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::PENDING);
    MyThread th = std::move(thread);
    th.Start();
    EXPECT_THROW(thread = std::move(th), bpf::system::OSException);
    EXPECT_EQ(th.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    th.Kill();
    EXPECT_EQ(th.GetState(), bpf::system::Thread::EXITING);
    th.Join();
    EXPECT_EQ(th.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(th._value - (bpf::uint32)2, (bpf::uint32)1);
    thread = std::move(th);
}

TEST(Thread, KillForce)
{
    MyThread thread(false);

    EXPECT_EQ(thread.GetState(), bpf::system::Thread::PENDING);
    thread.Start();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread.Kill(true);
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::STOPPED); //As we just force killed the thread it should be marked STOPPED
}

TEST(Thread, Exception)
{
    MyThread thread(true);

    EXPECT_EQ(thread.GetState(), bpf::system::Thread::PENDING);
    thread.Start();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread.Join();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::STOPPED); //As the thread crashed (throw) it should be marked as STOPPED instead of FINISHED
}

TEST(Thread, RunTwice)
{
    MyThread thread(false);

    EXPECT_STREQ(*thread.GetName(), "MyThread");
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::PENDING);
    thread.Start();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread.Kill();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::EXITING);
    thread.Join();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)2, (bpf::uint32)1);
    thread.Start();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread.Kill();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::EXITING);
    thread.Join();
    EXPECT_EQ(thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)4, (bpf::uint32)1);
}
