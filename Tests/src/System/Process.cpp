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

#include <Framework/IO/TextReader.hpp>
#include <Framework/IO/IOException.hpp>
#include <Framework/System/Process.hpp>
#include <Framework/System/OSException.hpp>
#include <gtest/gtest.h>

#ifdef WINDOWS
constexpr const char *SAMPLE_EXE_NAME = "help.exe";
#else
constexpr const char *SAMPLE_EXE_NAME = "ls";
#endif

TEST(Process, SimpleErr)
{
    EXPECT_THROW(bpf::system::Process::Builder().SetApplication("does not exist").Build(), bpf::io::IOException);
}

TEST(Process, Simple)
{
    auto proc = bpf::system::Process::Builder().SetApplication(SAMPLE_EXE_NAME).Build();
    proc.Wait();
    EXPECT_THROW(proc.GetStandardError(), bpf::system::OSException);
    EXPECT_THROW(proc.GetStandardInput(), bpf::system::OSException);
    EXPECT_THROW(proc.GetStandardOutput(), bpf::system::OSException);
    EXPECT_EQ(proc.GetExitCode(), 0);
}

#ifdef WINDOWS
#else
TEST(Process, RedirectOutput)
{
    auto proc = bpf::system::Process::Builder().SetApplication("uname").RedirectOutput().Build();
    bpf::io::TextReader reader(proc.GetStandardOutput());
    bpf::String text;
    EXPECT_EQ(proc.GetExitCode(), 0);
    text += reader.ReadAll();
    EXPECT_GT(text.Size(), 0);
    EXPECT_GT(text.Len(), 0);
}
#endif