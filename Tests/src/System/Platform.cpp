// Copyright (c) 2020, BlockProject
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
//     * Neither the name of BlockProject nor the names of its contributors
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
#include <Framework/System/Platform.hpp>
#include <Framework/System/ModuleInterface.hpp>

TEST(Platform, OS)
{
    auto var = bpf::system::Platform::GetOSInfo();
    bool executed = false;

#ifdef WINDOWS
    EXPECT_TRUE(var.Name.Contains("Windows"));
    EXPECT_STREQ(*var.NewLine, "\r\n");
    EXPECT_STREQ(*var.PathSep, "\\");
    EXPECT_STREQ(*var.ModuleExt, "dll");
    EXPECT_GT(var.Version.Len(), 0);
    executed = true;
#elif MAC
    EXPECT_TRUE(var.Name.Contains("Mac"));
    EXPECT_STREQ(*var.NewLine, "\n");
    EXPECT_STREQ(*var.PathSep, "/");
    EXPECT_STREQ(*var.ModuleExt, "dylib");
    EXPECT_GT(var.Version.Len(), 0);
    executed = true;
#else
    EXPECT_TRUE(var.Name.Contains("Linux"));
    EXPECT_STREQ(*var.NewLine, "\n");
    EXPECT_STREQ(*var.PathSep, "/");
    EXPECT_STREQ(*var.ModuleExt, "so");
    EXPECT_GT(var.Version.Len(), 0);
    executed = true;
#endif
    EXPECT_TRUE(executed);
}

TEST(Platform, Env)
{
    auto var = bpf::system::Platform::GetEnvInfo();

    EXPECT_STREQ(*var.ShortName, "BPF");
    EXPECT_STREQ(*var.Name, "BlockProject Framework");
    EXPECT_STREQ(*var.Version, "2.0");
    EXPECT_EQ(var.VersionInt, BP_MODULE_VERSION_INT);

}

TEST(Platform, CPU)
{
    auto var = bpf::system::Platform::GetCPUInfo();

    EXPECT_GT(var.Name.Len(), 0);
    EXPECT_GT(var.NumCores, 0); //There has to be at least 1 core
    EXPECT_GT(var.Freq, 0); //Frequency has to be at least greater than 0Mhz
}

TEST(Platform, RAM)
{
    auto var = bpf::system::Platform::GetRAMInfo();

    EXPECT_GT(var.MaxPhysical, 0U);
    EXPECT_GT(var.MaxVirtual, 0U);
}

TEST(Platform, ReverseBuffer_1)
{
    char data[] = "abcd";

    bpf::system::Platform::ReverseBuffer(data, 4);
    EXPECT_STREQ(data, "dcba");
}

TEST(Platform, ReverseBuffer_2)
{
    char data[] = "abcdefghijklmnop";

    bpf::system::Platform::ReverseBuffer(data, 16, 4);
    EXPECT_STREQ(data, "mnopijklefghabcd");
}