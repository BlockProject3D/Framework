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

#include "Framework/System/WindowsApp.hpp"
#include "Framework/IO/IOException.hpp"
#include "Framework/System/OSException.hpp"
#include <Windows.h>
#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#undef SetConsoleTitle

using namespace bpf::system;
using namespace bpf::collection;
using namespace bpf::io;
using namespace bpf;

namespace bpf
{
    using _internal_AddDllDirectory = void *(WINAPI *)(PCWSTR str);
    using _internal_RemoveDllDirectory = BOOL (WINAPI *)(void *handle);
    using _internal_SetDefaultDllDirectories = BOOL (WINAPI *)(DWORD flags);
}

WindowsApp::WindowsApp(void *hinstance, bool hasConsole)
    : Application(_env, _fileName, _props)
    , _hInstance(hinstance)
    , _hasConsole(hasConsole)
    , _env(SetupEnvironment())
    , _fileName(SetupFileName())
    , _props(SetupPaths())
{
    _kernel = LoadLibrary("Kernel32.dll");
    if (_kernel != nullptr)
    {
        _addDllDir = reinterpret_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(_kernel), "AddDllDirectory"));
        _rmDllDir = reinterpret_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(_kernel), "RemoveDllDirectory"));
        auto setdlldirs = reinterpret_cast<_internal_SetDefaultDllDirectories>(
            GetProcAddress(reinterpret_cast<HMODULE>(_kernel), "SetDefaultDllDirectories"));
        if (setdlldirs != nullptr)
            setdlldirs(0x00001000); //Set the recommended search path
    }
    else
    {
        _addDllDir = nullptr;
        _rmDllDir = nullptr;
    }
}

WindowsApp::~WindowsApp()
{
    if (_kernel != nullptr)
        FreeModule(reinterpret_cast<HMODULE>(_kernel));
}

HashMap<String, String> WindowsApp::SetupEnvironment()
{
    auto env = HashMap<String, String>();
    LPWCH ptr = GetEnvironmentStringsW();
    LPWCH wptr;
    fsize i = 0;

    // We ignore first env var cause it's the weird "=::=::\"
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
                if (key.IsEmpty())
                    continue;
                auto value = str.Sub(str.IndexOf('=') + 1);
                if (*value == nullptr)
                    value = ""; // Attempt to fix Travis bug of having null vars...
                env.Add(key, value);
            }
        }
        ++i;
    }
    FreeEnvironmentStringsW(ptr);
    if (env.HasKey("Path"))
        env["PATH"] =
            env["Path"]; // Allow apps to use "PATH" to refer to the environment path on both Unix and Windows systems
    return (env);
}

Array<String> WindowsApp::GetArguments()
{
    int nArgs;
    LPWSTR *args = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    auto astyudf = Array<String>(nArgs);

    for (int i = 0; i != nArgs; ++i)
        astyudf[i] = String::FromUTF16(reinterpret_cast<const fchar16 *>(args[i]));
    return (astyudf);
}

String WindowsApp::SetupFileName()
{
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(reinterpret_cast<HMODULE>(_hInstance), path, MAX_PATH);
    return (String::FromUTF16(reinterpret_cast<const fchar16 *>(path)));
}

Paths WindowsApp::SetupPaths()
{
    SetupEnvironment();
    File appRoot(_fileName.Sub(0, _fileName.LastIndexOf('\\')));
    File userHome(_env["USERPROFILE"]);
    WCHAR path[MAX_PATH];
    GetTempPathW(MAX_PATH, path);
    File tmpDir = File(String::FromUTF16(reinterpret_cast<const fchar16 *>(path)));
    if (appRoot.HasAccess(FILE_MODE_READ | FILE_MODE_WRITE))
        return (Paths(appRoot, appRoot, userHome, tmpDir));
    else
        return (Paths(appRoot, userHome + File(_fileName).Name(), userHome, tmpDir));
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
    coninfo.dwSize.Y = (SHORT)rows;
    coninfo.dwSize.X = (SHORT)columns;
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
    if (freopen_s(&fp, "CONERR$", "w", stderr) != 0)
        throw OSException("Could not redirect console error output");
    std::ios::sync_with_stdio();
    _hasConsole = true;
}

File WindowsApp::GetWorkingDirectory() const
{
    WCHAR path[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, path) > 0)
        return (File(String::FromUTF16(reinterpret_cast<const fchar16 *>(path))));
    else
        return (File(".").GetAbsolutePath());
}

bool WindowsApp::SetWorkingDirectory(const File &file)
{
    return (SetCurrentDirectoryW(reinterpret_cast<LPCWSTR>(*file.PlatformPath().ToUTF16())));
}

void WindowsApp::DisableErrorDialogs() noexcept
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _set_abort_behavior(0, _WRITE_ABORT_MSG);
}

void WindowsApp::SetModuleDirectories(const collection::Array<io::File> &directories)
{
    if (directories.Size() > 1 &&
        _addDllDir == nullptr) // We have more than 1 directory and no support for AddDllDirectory
        throw OSException("Multi-module directory not supported on this system");
    for (auto &dir : directories)
    {
        if (!dir.IsDirectory())
            throw io::IOException(String("Path '") + dir.Path() + "' is not a directory");
    }
    if (_addDllDir != nullptr)
    {
        if (_dlls.Size() > 0)
        {
            auto rmdir = reinterpret_cast<_internal_RemoveDllDirectory>(_rmDllDir);
            for (auto dir : _dlls)
                rmdir(dir);
            _dlls.Clear();
        }
        for (auto &dir : directories)
        {
            auto adddir = reinterpret_cast<_internal_AddDllDirectory>(_addDllDir);
            auto u16 = dir.PlatformPath().ToUTF16();
            auto handle = adddir(reinterpret_cast<PCWSTR>(*u16));
            if (handle != nullptr)
                _dlls.Add(handle);
        }
    }
    else
    {
        auto u16 = directories[0].PlatformPath().ToUTF16();
        if (!SetDllDirectoryW(reinterpret_cast<LPCWSTR>(*u16)))
            throw OSException("Failed to set module directories");
    }
}
