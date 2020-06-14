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
#include <Framework/System/Timer.hpp>
#include <Framework/System/Thread.hpp>

TEST(Timer, Test_1)
{
    bpf::system::Timer timer;

    //Pump CPU
    int i;
    int j = 0;
    for (i = 0; i < 100000000; i++)
        j += i * i;
#ifdef RELEASE //Too unreliable under OSX and Linux in Release mode
    EXPECT_GE(timer.Reset(), 0);
#else
    EXPECT_GT(timer.Reset(), 0);
#endif
}

TEST(Timer, Test_2)
{
    bpf::system::Timer timer;

    bpf::system::Thread::Sleep(1000);
    EXPECT_LE(timer.Reset() - 1, 0.1); //Allow a difference of 100ms for Sleep precision (Travis Windows and Mac are highly inaccurate in times...)
}