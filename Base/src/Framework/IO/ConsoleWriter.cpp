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

#include "Framework/IO/ConsoleWriter.hpp"
#include "Framework/IO/IOException.hpp"
#include "OSPrivate.hpp"
#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#undef ERROR

using namespace bpf::io;
using namespace bpf;

#ifdef WINDOWS
void *ConsoleWriter::GetHandle(const EConsoleStream type)
#else
int ConsoleWriter::GetHandle(const EConsoleStream type)
#endif
{
    switch (type)
    {
    case EConsoleStream::ERROR:
#ifdef WINDOWS
        return (GetStdHandle(STD_ERROR_HANDLE));
#else
        return (2);
#endif
    case EConsoleStream::OUTPUT:
#ifdef WINDOWS
        return (GetStdHandle(STD_OUTPUT_HANDLE));
#else
        return (1);
#endif
    }
#ifdef WINDOWS
    return (GetStdHandle(STD_OUTPUT_HANDLE));
#else
    return (1);
#endif
}

ConsoleWriter::ConsoleWriter(const EConsoleStream type)
    : _handle(GetHandle(type))
#ifdef WINDOWS
    , _file(Console::IsRedirected(type))
    , _writer(*this, _file ? ECharacterEncoding::UTF8 : ECharacterEncoding::UTF16)
#else
    , _writer(*this, ECharacterEncoding::UTF8)
#endif
    , _stream(type)
{
}

fsize ConsoleWriter::Write(const void *buf, fsize bufsize)
{
#ifdef WINDOWS
    if (_file)
    {
        DWORD out;
        if (!WriteFile(reinterpret_cast<HANDLE>(_handle), buf, (DWORD)bufsize, &out, nullptr))
            return (0);
        return ((fsize)out);
    }
    else
    {
        DWORD out;
        if (!WriteConsoleW(reinterpret_cast<HANDLE>(_handle), reinterpret_cast<LPCVOID>(buf), (DWORD)(bufsize / 2),
                           &out, nullptr))
            return (0);
        return ((fsize)(out * 2));
    }
#else
    int len = write(_handle, buf, bufsize);
    if (len < 0)
        return (0);
    return ((fsize)len);
#endif
}
