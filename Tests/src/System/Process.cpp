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

#include <Framework/IO/IOException.hpp>
#include <Framework/IO/TextReader.hpp>
#include <Framework/System/OSException.hpp>
#include <Framework/System/Process.hpp>
#include <Framework/Collection/Stringifier.HashMap.hpp>
#include <gtest/gtest.h>

extern bpf::system::Application *g_app;

#ifdef WINDOWS
constexpr const char *SAMPLE_EXE_NAME = "help.exe";
#else
constexpr const char *SAMPLE_EXE_NAME = "ls";
#endif

TEST(Process, SimpleErr)
{
    EXPECT_THROW(bpf::system::Process::Builder().SetApplication("does not exist").Build(), bpf::io::IOException);
    EXPECT_THROW(bpf::system::Process::Builder().SetApplication("./does not exist").Build(), bpf::io::IOException);
    EXPECT_THROW(bpf::system::Process::Builder().SetWorkingDirectory(bpf::io::File("does not exist")).Build(), bpf::io::IOException);
}

TEST(Process, Simple)
{
    auto proc = bpf::system::Process::Builder().SetApplication(SAMPLE_EXE_NAME).Build();
    proc.Wait();
    EXPECT_THROW(proc.GetStandardError(), bpf::system::OSException);
    EXPECT_THROW(proc.GetStandardInput(), bpf::system::OSException);
    EXPECT_THROW(proc.GetStandardOutput(), bpf::system::OSException);
#ifdef WINDOWS
    EXPECT_EQ(proc.GetExitCode(), 1);
#else
    EXPECT_EQ(proc.GetExitCode(), 0);
#endif
}

#ifdef WINDOWS
TEST(Process, RedirectOutput)
{
    auto proc = bpf::system::Process::Builder(*g_app)
        .SetApplication("where.exe")
        .SetArguments({"help.exe"})
        .RedirectOutput()
        .Build();
    bpf::io::TextReader reader(proc.GetStandardOutput());
    bpf::String text;
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 0);
    text += reader.ReadAll();
    EXPECT_GT(text.Size(), 0);
    EXPECT_GT(text.Len(), 0);
    EXPECT_TRUE(text.Contains("help.exe"));
}
#else
TEST(Process, RedirectOutput_1)
{
    auto proc = bpf::system::Process::Builder()
        .SetApplication("uname")
        .RedirectOutput()
        .Build();
    bpf::io::TextReader reader(proc.GetStandardOutput());
    bpf::String text;
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 0);
    text += reader.ReadAll();
    EXPECT_GT(text.Size(), 0);
    EXPECT_GT(text.Len(), 0);
}

TEST(Process, RedirectOutput_2)
{
    auto proc = bpf::system::Process::Builder()
        .SetApplication("uname")
        .SetArguments({"-a"})
        .RedirectOutput()
        .Build();
    bpf::io::TextReader reader(proc.GetStandardOutput());
    bpf::String text;
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 0);
    text += reader.ReadAll();
    EXPECT_GT(text.Size(), 0);
    EXPECT_GT(text.Len(), 0);
}

TEST(Process, RedirectOutput_3)
{
    auto proc = bpf::system::Process::Builder(*g_app)
        .SetApplication("uname")
        .SetArguments({"-a"})
        .RedirectOutput()
        .Build();
    bpf::io::TextReader reader(proc.GetStandardOutput());
    bpf::String text;
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 0);
    text += reader.ReadAll();
    EXPECT_GT(text.Size(), 0);
    EXPECT_GT(text.Len(), 0);
}

TEST(Process, RedirectOutput_4)
{
    auto args = bpf::collection::Array<bpf::String>({"-a"});
    auto proc = bpf::system::Process::Builder()
        .SetApplication("uname")
        .SetArguments(args)
        .RedirectOutput()
        .SetWorkingDirectory(bpf::io::File("."))
        .Build();
    bpf::io::TextReader reader(proc.GetStandardOutput());
    bpf::String text;
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 0);
    text += reader.ReadAll();
    EXPECT_GT(text.Size(), 0);
    EXPECT_GT(text.Len(), 0);
}
#endif
