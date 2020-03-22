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

#include "Framework/IO/ConsoleWriter.hpp"
#include "Framework/IO/IOException.hpp"
#include "OSPrivate.hpp"
#include <iostream>
#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#undef ERROR

using namespace bpf::io;
using namespace bpf;

ConsoleWriter::ConsoleWriter(const EConsoleStream type)
    : _type(type)
{
    switch (type)
    {
    case EConsoleStream::ERROR:
#ifdef WINDOWS
        _handle = GetStdHandle(STD_ERROR_HANDLE);
#else
        _handle = 2;
#endif
        break;
    case EConsoleStream::INPUT:
#ifdef WINDOWS
        _handle = GetStdHandle(STD_INPUT_HANDLE);
#else
        _handle = 0;
#endif
        break;
    case EConsoleStream::OUTPUT:
#ifdef WINDOWS
        _handle = GetStdHandle(STD_OUTPUT_HANDLE);
#else
        _handle = 1;
#endif
        break;
    }
}

void ConsoleWriter::Flush()
{
#ifdef WINDOWS
    switch (_type)
    {
    case EConsoleStream::ERROR:
        std::wcerr.flush();
        break;
    case EConsoleStream::OUTPUT:
        std::wcout.flush();
        break;
    }
#else
    switch (_type)
    {
    case EConsoleStream::ERROR:
        std::cerr.flush();
        break;
    case EConsoleStream::OUTPUT:
        std::cout.flush();
        break;
    }
#endif
}

fsize ConsoleWriter::Write(const void *buf, fsize bufsize)
{
#ifdef WINDOWS
    DWORD out;
    if (!WriteConsoleW(reinterpret_cast<HANDLE>(_handle), reinterpret_cast<LPCVOID>(buf), (DWORD)(bufsize / 2), &out, NULL))
        throw IOException(String("Console write error: ") + OSPrivate::ObtainLastErrorString());
    return (out * 2);
#else
    int len = write(_handle, buf, bufsize);
    if (len < 0)
        throw IOException(String("Console write error: ") + OSPrivate::ObtainLastErrorString());
    return ((fsize)len);
#endif
}

void ConsoleWriter::WriteLine(const String &str)
{
    Write(str);
    NewLine();
}

void ConsoleWriter::Write(const String &str)
{
#ifdef WINDOWS
    auto utf16 = str.ToUTF16();
    switch (_type)
    {
    case EConsoleStream::ERROR:
        std::wcerr << reinterpret_cast<const wchar_t *>(*utf16);
        break;
    case EConsoleStream::OUTPUT:
        std::wcout << reinterpret_cast<const wchar_t *>(*utf16);
        break;
    }
#else
    switch (_type)
    {
    case EConsoleStream::ERROR:
        std::cerr << *str;
        break;
    case EConsoleStream::OUTPUT:
        std::cout << *str;
        break;
    }
#endif
}

void ConsoleWriter::NewLine()
{
#ifdef WINDOWS
    switch (_type)
    {
    case EConsoleStream::ERROR:
        std::wcerr << std::endl;
        break;
    case EConsoleStream::OUTPUT:
        std::wcout << std::endl;
        break;
    }
#else
    switch (_type)
    {
    case EConsoleStream::ERROR:
        std::cerr << std::endl;
        break;
    case EConsoleStream::OUTPUT:
        std::cout << std::endl;
        break;
    }
#endif
}
