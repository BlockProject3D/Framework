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
#include "Framework/System/OSException.hpp"
#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
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

void WindowsApp::SetupEnvironment()
{
    LPWCH ptr = GetEnvironmentStringsW();
    LPWCH wptr;
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
            {
                auto key = str.Sub(0, str.IndexOf('='));
                auto value = str.Sub(str.IndexOf('=') + 1);
                if (key != String::Empty && value != String::Empty)
                    _env.Add(key, value);
            }
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

void WindowsApp::CreateConsole(const fint rows, const fint columns)
{
    if (_hasConsole)
        return;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;
    int hConHandle;
    HANDLE lStdHandle;

    if (AllocConsole() == FALSE)
        throw OSException("Could not allocate console");
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = rows;
    coninfo.dwSize.X = columns;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
    lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
        throw OSException("Could not redirect console output");
    lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "r");
    if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
        throw OSException("Could not redirect console input");
    lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    if (freopen_s(&fp, "CONERR$", "r", stderr) != 0)
        throw OSException("Could not redirect console error output");
    std::ios::sync_with_stdio();
    _hasConsole = true;
}