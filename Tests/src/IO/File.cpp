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
#include <Framework/IO/File.hpp>
#include <Framework/IO/FileStream.hpp>
#include <Framework/IO/IOException.hpp>

TEST(File, Basics)
{
    bpf::io::File f("./doesnotexist.txt");

    EXPECT_FALSE(f.Exists());
    f.CreateDir();
    EXPECT_TRUE(f.Exists());
    EXPECT_TRUE(f.IsDirectory());
    EXPECT_TRUE(f.HasAccess(bpf::io::FILE_ACCESS_READ | bpf::io::FILE_ACCESS_WRITE));
    f.Delete();
    EXPECT_FALSE(f.Exists());
    EXPECT_FALSE(f.IsDirectory());
}

TEST(File, Hide)
{
    bpf::io::File f("./doesnotexist.txt");

    f.CreateDir();
    EXPECT_FALSE(f.IsHidden());
    f.Hide(true);
    EXPECT_TRUE(f.IsHidden());
    f.Hide(false);
    EXPECT_FALSE(f.IsHidden());
    f.Delete();
}

TEST(File, Abs)
{
    bpf::io::File f("./doesnotexist.txt");

    EXPECT_FALSE(f.Exists());
    f.CreateDir();
    EXPECT_TRUE(f.Exists());
    EXPECT_TRUE(f.IsDirectory());
    bpf::io::File abs = f.GetAbsolutePath();
    std::cout << *abs.Path() << std::endl;
    EXPECT_TRUE(abs.Exists());
    EXPECT_TRUE(abs.IsDirectory());
    f.Delete();
    EXPECT_FALSE(f.Exists());
}

TEST(File, Move)
{
    bpf::io::File f("./doesnotexist.txt");
    bpf::io::File dest("./doesexist.txt");

    EXPECT_FALSE(f.Exists());
    f.CreateDir();
    EXPECT_TRUE(f.Exists());
    EXPECT_FALSE(dest.Exists());
    EXPECT_TRUE(f.MoveTo(dest));
    EXPECT_TRUE(dest.Exists());
    EXPECT_FALSE(f.Exists());
    f.Delete();
    dest.Delete();
}

#ifdef WINDOWS
TEST(File, AutoBackSlash)
{
    bpf::io::File f("./");

    EXPECT_STREQ(*f.PlatformPath(), ".");
    bpf::io::File f1 = f + "../";
    EXPECT_STREQ(*f1.PlatformPath(), ".\\..");
    f = bpf::io::File("./users////var//cleanup/test");
    EXPECT_STREQ(*f.PlatformPath(), ".\\users\\var\\cleanup\\test");
    EXPECT_STREQ(*f.Path(), "./users/var/cleanup/test");

}
#else
TEST(File, AutoSlash)
{
    bpf::io::File f("./");

    EXPECT_STREQ(*f.PlatformPath(), ".");
    bpf::io::File f1 = f + "../";
    EXPECT_STREQ(*f1.PlatformPath(), "./..");
    f = bpf::io::File("./users////var//cleanup/test");
    EXPECT_STREQ(*f.PlatformPath(), "./users/var/cleanup/test");
    EXPECT_STREQ(*f.Path(), "./users/var/cleanup/test");
}
#endif

TEST(File, List_Test1)
{
    bpf::io::File f("./");
    bpf::collection::List<bpf::io::File> fls = f.ListFiles();

    EXPECT_GT(fls.Size(), static_cast<bpf::uint32>(0));
}

TEST(File, List_Test2)
{
    bpf::io::File f(".");
    bpf::collection::List<bpf::io::File> fls = f.ListFiles();

    EXPECT_GT(fls.Size(), static_cast<bpf::uint32>(0));
}

TEST(File, List_Test3)
{
    bpf::io::File f("..");
    bpf::collection::List<bpf::io::File> fls = f.ListFiles();

    EXPECT_GT(fls.Size(), static_cast<bpf::uint32>(0));
}

TEST(File, List_Test4)
{
    bpf::io::File f("../");
    bpf::collection::List<bpf::io::File> fls = f.ListFiles();

    EXPECT_GT(fls.Size(), static_cast<bpf::uint32>(0));
}

static void SetupTestFile(bpf::io::File &f)
{
    bpf::io::FileStream stream(f, bpf::io::FILE_MODE_WRITE | bpf::io::FILE_MODE_TRUNCATE);
    EXPECT_EQ(stream.Write("This is a test", 14), (bpf::fsize)14);
}

TEST(File, List_Test_Err_1)
{
    bpf::io::File f("DoesNotExist");

    EXPECT_THROW(f.ListFiles(), bpf::io::IOException);
}

TEST(File, List_Test_Err_2)
{
    bpf::io::File f("DoesNotExist");

    SetupTestFile(f);
    EXPECT_THROW(f.ListFiles(), bpf::io::IOException);
    f.Delete();
}

TEST(File, GetSizeBytes)
{
    bpf::io::File f("./test_me.txt");
    SetupTestFile(f);
    EXPECT_EQ(f.GetSizeBytes(), 14U);
    f.Delete();
    EXPECT_THROW(f.GetSizeBytes(), bpf::io::IOException);
}
