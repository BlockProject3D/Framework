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

#include <Framework/System/ThreadPool.hpp>
#include <gtest/gtest.h>

TEST(ThreadPool, Mutex)
{
    auto mu = bpf::system::Mutex();
    mu.Lock();
    mu.Unlock();
    auto mv = std::move(mu);
    mv.Lock();
    mv.Unlock();
}

TEST(ThreadPool, Basic)
{
    auto pool = bpf::system::ThreadPool();
    EXPECT_TRUE(pool.IsIdle());
    auto mv = std::move(pool);
    EXPECT_TRUE(mv.IsIdle());
}

TEST(ThreadPool, Run)
{
    auto pool = bpf::system::ThreadPool();
    bpf::fsize res;
    for (bpf::fsize i = 0; i != 6; ++i)
    {
        pool.Run(
            [] {
                bpf::fsize i = 1;
                while (i < 16384)
                    i *= 2;
                return (i);
            },
            [&res](bpf::Dynamic &dyn) { res = (bpf::fsize)dyn; });
    }
    while (!pool.IsIdle())
        pool.Poll();
    EXPECT_EQ(res, 16384);
}
