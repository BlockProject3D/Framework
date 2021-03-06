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
#include <Framework/IO/TextWriter.hpp>
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
#ifdef WINDOWS
    EXPECT_THROW(bpf::system::Process::Builder().SetWorkingDirectory(g_app->Props.AppRoot + "BPF.Tests.exe"), bpf::io::IOException);
#else
    EXPECT_THROW(bpf::system::Process::Builder().SetWorkingDirectory(g_app->Props.AppRoot + "BPF.Tests"), bpf::io::IOException);
#endif
}

TEST(Process, ArgsEnvErr)
{
    auto args = bpf::collection::Array<bpf::String>({bpf::String()}); //If not properly handled will crash the application
    auto env = bpf::collection::HashMap<bpf::String, bpf::String>({{bpf::String(), "Value"}}); //If not properly handled will crash the application
    auto env1 = bpf::collection::HashMap<bpf::String, bpf::String>({{"Key", bpf::String()}}); //If not properly handled will crash the application
    EXPECT_THROW(bpf::system::Process::Builder().SetArguments(args).SetApplication(SAMPLE_EXE_NAME).Build(), bpf::system::OSException);
    EXPECT_THROW(bpf::system::Process::Builder().SetEnvironment(env).SetApplication(SAMPLE_EXE_NAME).Build(), bpf::system::OSException);
    EXPECT_THROW(bpf::system::Process::Builder().SetEnvironment(env1).SetApplication(SAMPLE_EXE_NAME).Build(), bpf::system::OSException);
}

#ifndef WINDOWS
TEST(Process, ExecErr)
{
    EXPECT_THROW(bpf::system::Process::Builder().SetApplication("/etc/group").Build(), bpf::system::OSException);
}
#endif

TEST(Process, Simple_1)
{
    auto proc = bpf::system::Process::Builder().SetApplication(SAMPLE_EXE_NAME).Build();
    proc.Wait();
    EXPECT_THROW(proc.GetStandardError(), bpf::system::OSException);
    EXPECT_THROW(proc.GetStandardInput(), bpf::system::OSException);
    EXPECT_THROW(proc.GetStandardOutput(), bpf::system::OSException);
    EXPECT_FALSE(proc.IsRunning());
    EXPECT_FALSE(proc.IsCrashed());
#ifdef WINDOWS
    EXPECT_EQ(proc.GetExitCode(), 1U);
#else
    EXPECT_EQ(proc.GetExitCode(), 0U);
#endif
}

TEST(Process, Simple_2)
{
    auto proc = bpf::system::Process::Builder().SetApplication(SAMPLE_EXE_NAME).Build();
    auto p1 = std::move(proc);
    p1.Wait();
    EXPECT_THROW(p1.GetStandardError(), bpf::system::OSException);
    EXPECT_THROW(p1.GetStandardInput(), bpf::system::OSException);
    EXPECT_THROW(p1.GetStandardOutput(), bpf::system::OSException);
#ifdef WINDOWS
    EXPECT_EQ(p1.GetExitCode(), 1U);
#else
    EXPECT_EQ(p1.GetExitCode(), 0U);
#endif
}

TEST(Process, Simple_3)
{
    auto proc = bpf::system::Process::Builder().SetApplication(SAMPLE_EXE_NAME).Build();
    auto p1 = bpf::system::Process::Builder().SetApplication(SAMPLE_EXE_NAME).Build();
    p1 = std::move(proc);
    p1.Wait();
    EXPECT_THROW(p1.GetStandardError(), bpf::system::OSException);
    EXPECT_THROW(p1.GetStandardInput(), bpf::system::OSException);
    EXPECT_THROW(p1.GetStandardOutput(), bpf::system::OSException);
#ifdef WINDOWS
    EXPECT_EQ(p1.GetExitCode(), 1U);
#else
    EXPECT_EQ(p1.GetExitCode(), 0U);
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
    EXPECT_EQ(proc.GetExitCode(), 0U);
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
    EXPECT_EQ(proc.GetExitCode(), 0U);
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
    EXPECT_EQ(proc.GetExitCode(), 0U);
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
    EXPECT_EQ(proc.GetExitCode(), 0U);
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
    EXPECT_EQ(proc.GetExitCode(), 0U);
    text += reader.ReadAll();
    EXPECT_GT(text.Size(), 0);
    EXPECT_GT(text.Len(), 0);
}
#endif

TEST(Process, MultiRedirection_1)
{
    auto proc = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").Path())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    bpf::io::TextWriter writer(proc.GetStandardInput());
    writer.WriteLine("this is a test");
    writer.Flush();
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 2U);
    bpf::io::TextReader oreader(proc.GetStandardOutput());
    bpf::io::TextReader ereader(proc.GetStandardError());
    bpf::String line;
    bpf::String line1;
    EXPECT_TRUE(oreader.ReadLine(line));
    EXPECT_TRUE(ereader.ReadLine(line1));
    EXPECT_STREQ(*line, "this is a test");
    EXPECT_STREQ(*line1, "TestError: this is a test");
}

TEST(Process, PipeErr)
{
    auto proc = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").Path())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    proc.GetStandardInput().Close();
    proc.GetStandardOutput().Close();
    proc.GetStandardError().Close();
    bpf::io::TextWriter writer(proc.GetStandardInput());
    writer.WriteLine("this is a test");
    EXPECT_THROW(writer.Flush(), bpf::io::IOException);
    proc.Wait();
    EXPECT_TRUE(proc.IsCrashed());
    bpf::io::TextReader oreader(proc.GetStandardOutput());
    bpf::io::TextReader ereader(proc.GetStandardError());
    bpf::String line;
    bpf::String line1;
#ifdef WINDOWS
    // Unfortunatly windows does not distinguish from actual error and empty pipes
    EXPECT_FALSE(oreader.ReadLine(line));
    EXPECT_FALSE(ereader.ReadLine(line1));
#else
    EXPECT_THROW(oreader.ReadLine(line), bpf::io::IOException);
    EXPECT_THROW(ereader.ReadLine(line1), bpf::io::IOException);
#endif
}

TEST(Process, MultiRedirection_2)
{
    auto proc = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").PlatformPath())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    bpf::io::TextWriter writer(proc.GetStandardInput());
    writer.WriteLine("this is a test");
    writer.Flush();
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 2U);
    bpf::io::TextReader oreader(proc.GetStandardOutput());
    bpf::io::TextReader ereader(proc.GetStandardError());
    bpf::String line;
    bpf::String line1;
    EXPECT_TRUE(oreader.ReadLine(line));
    EXPECT_TRUE(ereader.ReadLine(line1));
    EXPECT_STREQ(*line, "this is a test");
    EXPECT_STREQ(*line1, "TestError: this is a test");
}

TEST(Process, MultiRedirection_3)
{
    auto p = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").Path())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    auto proc = std::move(p);
    EXPECT_TRUE(proc.IsRunning());
    bpf::io::TextWriter writer(proc.GetStandardInput());
    writer.WriteLine("this is a test");
    writer.Flush();
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 2U);
    bpf::io::TextReader oreader(proc.GetStandardOutput());
    bpf::io::TextReader ereader(proc.GetStandardError());
    bpf::String line;
    bpf::String line1;
    EXPECT_TRUE(oreader.ReadLine(line));
    EXPECT_TRUE(ereader.ReadLine(line1));
    EXPECT_STREQ(*line, "this is a test");
    EXPECT_STREQ(*line1, "TestError: this is a test");
}

TEST(Process, AutoKill)
{
    auto p = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").Path())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    auto p1 = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").Path())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    p = std::move(p1);
    (void)p;
}

TEST(Process, Kill_NonForce)
{
    auto proc = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").Path())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    proc.Kill(false);
    auto res = proc.GetExitCode();
    EXPECT_FALSE(proc.IsRunning());
    EXPECT_NE(res, 0U);
    proc.Kill(false);
    EXPECT_EQ(res, proc.GetExitCode());
}

TEST(Process, Kill_Force)
{
    auto proc = bpf::system::Process::Builder()
        .SetApplication((g_app->Props.AppRoot + "BPF.Tests").Path())
        .SetEnvironment({{"__BPF_PARSE__", ""}})
        .RedirectInput()
        .RedirectOutput()
        .RedirectError()
        .Build();
    proc.Kill(true);
    auto res = proc.GetExitCode();
    EXPECT_FALSE(proc.IsRunning());
    EXPECT_NE(res, 0U);
    proc.Kill(true);
    EXPECT_EQ(res, proc.GetExitCode());
}

#ifdef LINUX
TEST(Process, SimpleCat)
{
    auto proc = bpf::system::Process::Builder(*g_app)
        .SetApplication("cat")
        .RedirectInput()
        .RedirectOutput()
        .Build();
    bpf::io::TextWriter writer(proc.GetStandardInput());
    writer.WriteLine("this is a test");
    writer.Flush();
    proc.GetStandardInput().Close();
    proc.Wait();
    EXPECT_EQ(proc.GetExitCode(), 0U);
    bpf::io::TextReader oreader(proc.GetStandardOutput());
    bpf::String line;
    EXPECT_TRUE(oreader.ReadLine(line));
    EXPECT_STREQ(*line, "this is a test");
}
#endif