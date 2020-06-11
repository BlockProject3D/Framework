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

#include "Framework/System/UnixApp.hpp"
#include <limits.h>
#include <unistd.h>

using namespace bpf::system;
using namespace bpf::collection;
using namespace bpf::io;
using namespace bpf;

UnixApp::UnixApp(char **argv, char **env)
    : Application(_env, _fileName, _props)
    , _env(SetupEnvironment(env))
    , _fileName(String(argv[0]))
    , _props(SetupPaths())
{
}

HashMap<String, String> UnixApp::SetupEnvironment(char **env)
{
    auto menv = HashMap<String, String>();

    for (int i = 0; env[i]; ++i)
    {
        String str(env[i]);
        auto key = str.Sub(0, str.IndexOf('='));
        auto value = str.Sub(str.IndexOf('=') + 1);
        if (key != String::Empty && value != String::Empty)
            menv.Add(key, value);
    }
    return (menv);
}

Array<String> UnixApp::GetArguments(char **argv, int argc)
{
    auto args = Array<String>(argc);
    for (int i = 0; i != argc; ++i)
        args[i] = String(argv[i]);
    return (args);
}

Paths UnixApp::SetupPaths()
{
    File home = File(_env["HOME"]);
    File root = File(_fileName.Sub(0, _fileName.LastIndexOf('/')));
    File tmp = _env.HasKey("TMPDIR") ? File(_env["TMPDIR"]) : File("/tmp");
    return (Paths(root, home, tmp));
}

File UnixApp::GetWorkingDirectory() const
{
    char path[PATH_MAX];
    if (getcwd(path, PATH_MAX) != Null)
        return (File(path));
    else
        return (File("."));
}

void UnixApp::SetWorkingDirectory(const File &file) const
{
    chdir(*file.PlatformPath());
}
