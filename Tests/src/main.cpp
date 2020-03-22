// Copyright (c) 2018, BlockProject
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
#include <Framework/System/IApplication.hpp>
#include <Framework/Collection/Stringifier.Array.hpp>
#include <Framework/IO/ConsoleWriter.hpp>
#include <Framework/System/Platform.hpp>

int Main(bpf::system::IApplication &app, const bpf::collection::Array<bpf::String> &args, const bpf::system::Paths &paths)
{
    bpf::io::ConsoleWriter console;
    const auto &newLine = bpf::system::Platform::GetOSInfo().NewLine;
    assert(app.GetEnvironment().Size() > 0);
    assert(app.GetArguments().Size() > 0); //At least we need the file name passed to our arguments
    assert(app.GetExeFileName().Len() > 0);
    assert(paths.AppRoot().Path().Len() > 0);
    assert(paths.ThirdParty().Path().Len() > 0);
    assert(paths.CacheDir().Path().Len() > 0);
    assert(paths.TempDir().Path().Len() > 0);
    assert(paths.UserHome().Path().Len() > 0);
    console << "<==== Initializing high-level main ====>" << newLine;
    console << "WorkDir: " << bpf::io::File(".").GetAbsolutePath().PlatformPath() << newLine;
    console << "AppRoot: " << paths.AppRoot().Path() << newLine;
    console << "ThirdParty: " << paths.ThirdParty().Path() << newLine;
    console << "CacheDir: " << paths.CacheDir().Path() << newLine;
    console << "TempDir: " << paths.TempDir().Path() << newLine;
    console << "UserHome: " << paths.UserHome().Path() << newLine;
    console << "Program args: " << bpf::String::ValueOf(args) << newLine;
    console << "Program file name: " << app.GetExeFileName() << newLine;
    console << "Number of environment variables: " << app.GetEnvironment().Size() << newLine;
    console << "<======================================>" << newLine;
    console << newLine;
    console << "<==== Initializing GoogleTest ====>" << newLine;
    return (0);
}
