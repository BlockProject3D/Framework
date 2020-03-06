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

#include "Framework/System/WindowsApp.hpp"
#include <Windows.h>
#undef SetConsoleTitle

using namespace bpf::system;
using namespace bpf::collection;
using namespace bpf::io;
using namespace bpf;

WindowsApp::WindowsApp(void *hinstance, bool hasConsole)
    : _hInstance(hinstance)
    , _hasConsole(hasConsole)
    , _paths(File(), File(), File(), File())
{
    SetupEnvironment();
    SetupArgs();
    SetupFileName();
    SetupPaths();
}

void WindowsApp::SetConsoleTitle(const String &title)
{
    if (!_hasConsole)
        return;
    SetConsoleTitleW(reinterpret_cast<LPCWSTR>(*title.ToUTF16()));
}

void WindowsApp::SetupEnvironment()
{
    LPWCH ptr = GetEnvironmentStringsW();
    LPWCH wptr = ptr;
    fsize i = 0;

    //We ignore first env var cause it's the weird "=::=::\"
    while (true)
    {
        if (ptr[i] == 0 && ptr[i + 1] == 0)
            break;
        if (ptr[i] == 0)
        {
            wptr = ptr + i + 1;
            String str = String::FromUTF16(reinterpret_cast<const fchar16 *>(wptr));
            if (str[0] != '=')
                _env.Add(str.Sub(0, str.IndexOf('=')), str.Sub(str.IndexOf('=') + 1));
        }
        ++i;
    }
    FreeEnvironmentStringsW(ptr);
}

void WindowsApp::SetupArgs()
{
    int nArgs;
    LPWSTR *args = CommandLineToArgvW(GetCommandLineW(), &nArgs);

    _args = Array<String>(nArgs);
    for (int i = 0; i != nArgs; ++i)
        _args[i] = String::FromUTF16(reinterpret_cast<const fchar16 *>(args[i]));
}

void WindowsApp::SetupFileName()
{
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(reinterpret_cast<HMODULE>(_hInstance), path, MAX_PATH);
    _fileName = String::FromUTF16(reinterpret_cast<const fchar16 *>(path));
}

void WindowsApp::SetupPaths()
{
    File appRoot(_fileName.Sub(0, _fileName.LastIndexOf('\\')));
    File userHome(_env["USERPROFILE"]);
    File cacheDir = appRoot + "Cache";
    WCHAR path[MAX_PATH];
    GetTempPathW(MAX_PATH, path);
    File tmpDir = File(String::FromUTF16(reinterpret_cast<const fchar16 *>(path)));
    _paths = Paths(appRoot, userHome, tmpDir, cacheDir);
}

void WindowsApp::CreateConsole()
{
    if (_hasConsole)
        return;
    //TODO: AllocConsole
    _hasConsole = true;
}