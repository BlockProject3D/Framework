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
#include <Framework/System/Application.hpp>
#include <Framework/Collection/Stringifier.Array.hpp>
#include <Framework/IO/ConsoleWriter.hpp>
#include <Framework/IO/ConsoleReader.hpp>
#include <Framework/System/Platform.hpp>

bpf::system::Application *g_app;

int Main(bpf::system::Application &app, const bpf::collection::Array<bpf::String> &args)
{
    bpf::io::Console::SetTitle("BPF Unit Tests");
    bpf::io::Console::ResetTextStyle(bpf::io::EConsoleStream::OUTPUT);
    bpf::io::Console::ResetTextStyle(bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::ResetTextStyle();
    if (app.Environment.HasKey("__BPF_PARSE__"))
    {
        bpf::io::ConsoleReader reader;
        bpf::String str;
#ifdef BUILD_DEBUG
        assert(reader.ReadLine(str));
#else
        reader.ReadLine(str);
#endif
        bpf::io::Console::WriteLine(str);
        bpf::io::Console::WriteLine(bpf::String("TestError: ") + str, bpf::io::EConsoleStream::ERROR);
        return (1);
    }
#ifndef WINDOWS
    app.CreateConsole(); //Dummy call for coverage
#endif
    g_app = &app;
    bpf::io::ConsoleWriter console;
    const auto &newLine = bpf::system::Platform::GetOSInfo().NewLine;
    assert(app.Environment.Size() > 0);
    assert(args.Size() > 0); //At least we need the file name passed to our arguments
    assert(app.FileName.Len() > 0);
    assert(app.Props.AppRoot.Path().Len() > 0);
    assert(app.Props.ThirdParty.Path().Len() > 0);
    assert(app.Props.CacheDir.Path().Len() > 0);
    assert(app.Props.TempDir.Path().Len() > 0);
    assert(app.Props.UserHome.Path().Len() > 0);
    assert(app.Props.DataDir.Path().Len() > 0);
    console << bpf::io::Console::TextStyle(bpf::io::EConsoleColor::CYAN) << "<==== Initializing high-level main ====>" << bpf::io::Console::ClearTextStyle() << newLine;
    console.WriteLine("您好!");
    console << "WorkDir: " << app.GetWorkingDirectory().Path() << newLine;
    console << "AppRoot: " << app.Props.AppRoot.Path() << newLine;
    console << "DataDir: " << app.Props.DataDir.Path() << newLine;
    console << "ThirdParty: " << app.Props.ThirdParty.Path() << newLine;
    console << "CacheDir: " << app.Props.CacheDir.Path() << newLine;
    console << "TempDir: " << app.Props.TempDir.Path() << newLine;
    console << "UserHome: " << app.Props.UserHome.Path() << newLine;
    console << "Program args: " << bpf::String::ValueOf(args) << newLine;
    console << "Program file name: " << app.FileName << newLine;
    console << "Number of environment variables: " << app.Environment.Size() << newLine;
    console << bpf::io::Console::TextStyle(bpf::io::EConsoleColor::CYAN) << "<======================================>" << bpf::io::Console::ClearTextStyle() << newLine;
    console.NewLine();
    console << "<==== Initializing GoogleTest ====>" << newLine;
    return (0);
}
