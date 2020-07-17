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

#include <Framework/System/OSException.hpp>
#include <Framework/System/Thread.hpp>
#include <Framework/Memory/Utility.hpp>
#include <gtest/gtest.h>
#include <iostream>

class MyThread final : public bpf::system::IThreadRunnable
{
public:
    bpf::system::Thread _thread;
    bool except;
    bpf::uint32 _value;
    explicit MyThread(bool except)
        : _thread("MyThread", *this)
        , except(except)
        , _value(0) // Yeah will work better when initialized...
    {
    }
    void Run() final
    {
        while (_thread.IsRunning())
        {
            ++_value;
            bpf::system::Thread::Sleep(250);
            if (except)
                throw bpf::RuntimeException("UnitTest", "A nice exception");
        }
    }
};

TEST(Thread, Basic_1)
{
    MyThread thread(false);

    EXPECT_STREQ(*thread._thread.GetName(), "MyThread");
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::PENDING);
    thread._thread.Kill(); // Should just silently return as the thread is not yet running
    thread._thread.Start();
    thread._thread.Start(); // Should just silently return as the thread already runs
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread._thread.Kill();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::EXITING);
    thread._thread.Join();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)2, (bpf::uint32)1);
}

TEST(Thread, Basic_2)
{
    MyThread thread(false);

    EXPECT_STREQ(*thread._thread.GetName(), "MyThread");
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::PENDING);
    thread._thread.Start();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread._thread.Kill();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::EXITING);
    thread._thread.Join();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)2, (bpf::uint32)1);
}

TEST(Thread, Move)
{
    bpf::memory::UniquePtr<MyThread> thread(bpf::memory::MakeUnique<MyThread>(false));

    EXPECT_STREQ(*thread->_thread.GetName(), "MyThread");
    EXPECT_EQ(thread->_thread.GetState(), bpf::system::Thread::PENDING);
    auto th = std::move(thread);
    th->_thread.Start();
    EXPECT_EQ(th->_thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    th->_thread.Kill();
    EXPECT_TRUE((th->_thread.GetState() == bpf::system::Thread::EXITING) ||
                (th->_thread.GetState() == bpf::system::Thread::FINISHED)); // It is possible that the thread had time to
                                                                           // check it's new state before we could check
    th->_thread.Join();
    EXPECT_EQ(th->_thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(th->_value - (bpf::uint32)2, (bpf::uint32)1);
    thread = std::move(th);
}

TEST(Thread, KillForce)
{
    MyThread thread(false);

    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::PENDING);
    thread._thread.Start();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread._thread.Kill(true);
    EXPECT_EQ(thread._thread.GetState(),
              bpf::system::Thread::STOPPED); // As we just force killed the thread it should be marked STOPPED
}

TEST(Thread, Exception)
{
    MyThread thread(true);

    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::PENDING);
    thread._thread.Start();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread._thread.Join();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::STOPPED); // As the thread crashed (throw) it should be
                                                                        // marked as STOPPED instead of FINISHED
}

TEST(Thread, RunTwice)
{
    MyThread thread(false);

    EXPECT_STREQ(*thread._thread.GetName(), "MyThread");
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::PENDING);
    thread._thread.Start();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread._thread.Kill();
    EXPECT_TRUE(
        (thread._thread.GetState() == bpf::system::Thread::EXITING) ||
        (thread._thread.GetState() == bpf::system::Thread::FINISHED)); // It is possible that the thread had time to
                                                                       // check it's new state before we could check
    thread._thread.Join();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)2, (bpf::uint32)1);
    thread._thread.Start();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::RUNNING);
    bpf::system::Thread::Sleep(500);
    thread._thread.Kill();
    EXPECT_TRUE(
        (thread._thread.GetState() == bpf::system::Thread::EXITING) ||
        (thread._thread.GetState() == bpf::system::Thread::FINISHED)); // It is possible that the thread had time to
                                                                       // check it's new state before we could check
    thread._thread.Join();
    EXPECT_EQ(thread._thread.GetState(), bpf::system::Thread::FINISHED);
    EXPECT_LE(thread._value - (bpf::uint32)4,
              (bpf::uint32)2); // GitHub Actions so inaccurate in times even worse than Travis
}
