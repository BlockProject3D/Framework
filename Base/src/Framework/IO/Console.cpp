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

#include "Framework/IO/Console.hpp"
#include "Framework/IO/IOException.hpp"
#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <unistd.h>
#endif
#undef ERROR

using namespace bpf::io;
using namespace bpf;

#ifdef WINDOWS
// Apparently Microsoft loves globals; yeah!!
WORD g_Console_OldAttributes_Out = (WORD)-1;
WORD g_Console_OldAttributes_Err = (WORD)-1;
#endif

void Console::WriteLine(const String &str, const EConsoleStream type) noexcept
{
    if (type == EConsoleStream::ERROR)
    {
#ifdef WINDOWS
        HANDLE hdl = GetStdHandle(STD_ERROR_HANDLE);
        if (IsRedirected(EConsoleStream::ERROR))
        {
            auto s = str + "\r\n";
            WriteFile(hdl, reinterpret_cast<const void *>(*s), (DWORD)s.Size(), nullptr, nullptr);
        }
        else
        {
            auto utf16 = (str + "\r\n").ToUTF16();
            WriteConsoleW(hdl, reinterpret_cast<const void *>(*utf16), (DWORD)utf16.Size() - 1, nullptr, nullptr);
        }
#else
        BP_IGNORE(write(2, *(str + "\n"), str.Size() + 1))
#endif
    }
    else
    {
#ifdef WINDOWS
        HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
        if (IsRedirected())
        {
            auto s = str + "\r\n";
            WriteFile(hdl, reinterpret_cast<const void *>(*s), (DWORD)s.Size(), nullptr, nullptr);
        }
        else
        {
            auto utf16 = (str + "\r\n").ToUTF16();
            WriteConsoleW(hdl, reinterpret_cast<const void *>(*utf16), (DWORD)utf16.Size() - 1, nullptr, nullptr);
        }
#else
        BP_IGNORE(write(1, *(str + "\n"), str.Size() + 1))
#endif
    }
}

void Console::SetTextStyle(const TextStyle &style, const EConsoleStream type) noexcept
{
    if (IsRedirected(type))
        return;
#ifdef WINDOWS
    HANDLE hdl;
    if (type == EConsoleStream::ERROR)
    {
        hdl = GetStdHandle(STD_ERROR_HANDLE);
        if (g_Console_OldAttributes_Err == (WORD)-1)
        {
            CONSOLE_SCREEN_BUFFER_INFO Info;
            GetConsoleScreenBufferInfo(hdl, &Info);
            g_Console_OldAttributes_Err = Info.wAttributes;
        }
    }
    else
    {
        hdl = GetStdHandle(STD_OUTPUT_HANDLE);
        if (g_Console_OldAttributes_Out == (WORD)-1)
        {
            CONSOLE_SCREEN_BUFFER_INFO Info;
            GetConsoleScreenBufferInfo(hdl, &Info);
            g_Console_OldAttributes_Out = Info.wAttributes;
        }
    }
    WORD color = 0;
    switch (style.TextColor)
    {
    case EConsoleColor::BLACK:
        color = 0;
        break;
    case EConsoleColor::RED:
        color = FOREGROUND_INTENSITY | FOREGROUND_RED;
        break;
    case EConsoleColor::GREEN:
        color = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
        break;
    case EConsoleColor::YELLOW:
        color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case EConsoleColor::BLUE:
        color = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
        break;
    case EConsoleColor::MAGENTA:
        color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    case EConsoleColor::CYAN:
        color = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case EConsoleColor::WHITE:
        color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    }
    SetConsoleTextAttribute(hdl, color);
    // Bold is not supported > ask why Microsoft does not know how to code a terminal!
#else
    int fd;
    if (type == EConsoleStream::ERROR)
        fd = 2;
    else
        fd = 1;
    String str = String("\033[") + (style.Bold ? "1;" : "0;");
    switch (style.TextColor)
    {
    case EConsoleColor::BLACK:
        str += "30m";
        break;
    case EConsoleColor::RED:
        str += "31m";
        break;
    case EConsoleColor::GREEN:
        str += "32m";
        break;
    case EConsoleColor::YELLOW:
        str += "33m";
        break;
    case EConsoleColor::BLUE:
        str += "34m";
        break;
    case EConsoleColor::MAGENTA:
        str += "35m";
        break;
    case EConsoleColor::CYAN:
        str += "36m";
        break;
    case EConsoleColor::WHITE:
        str += "37m";
        break;
    }
    BP_IGNORE(write(fd, *str, str.Size()))
#endif
}

void Console::ResetTextStyle(const EConsoleStream type) noexcept
{
    if (IsRedirected(type))
        return;
    if (type == EConsoleStream::ERROR)
    {
#ifdef WINDOWS
        if (g_Console_OldAttributes_Err == (WORD)-1)
            return;
        HANDLE hdl = GetStdHandle(STD_ERROR_HANDLE);
        SetConsoleTextAttribute(hdl, g_Console_OldAttributes_Err);
#else
        BP_IGNORE(write(2, "\033[0m", 4))
#endif
    }
    else
    {
#ifdef WINDOWS
        if (g_Console_OldAttributes_Out == (WORD)-1)
            return;
        HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hdl, g_Console_OldAttributes_Out);
#else
        BP_IGNORE(write(1, "\033[0m", 4))
#endif
    }
}

void Console::SetTitle(const String &title) noexcept
{
#ifdef WINDOWS
    SetConsoleTitleW(reinterpret_cast<LPCWSTR>(*title.ToUTF16()));
#else
    if (IsRedirected())
        return;
    auto str = String("\e]0;") + title + "\007";
    BP_IGNORE(write(1, *str, str.Size()))
#endif
}

bool Console::IsRedirected(const EConsoleStream type) noexcept
{
#ifdef WINDOWS
    HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    if (type == EConsoleStream::ERROR)
        hdl = GetStdHandle(STD_ERROR_HANDLE);
    return (GetFileType(reinterpret_cast<HANDLE>(hdl)) != FILE_TYPE_CHAR ? true : false);
#else
    int fd = 1;
    if (type == EConsoleStream::ERROR)
        fd = 2;
    return (isatty(fd) == 0);
#endif
}
