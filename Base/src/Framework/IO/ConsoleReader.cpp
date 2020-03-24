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

#include "Framework/IO/ConsoleReader.hpp"
#include "Framework/IO/IOException.hpp"
#include "OSPrivate.hpp"
#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

using namespace bpf::io;
using namespace bpf;

ConsoleReader::ConsoleReader()
#ifdef WINDOWS
    : _handle(GetStdHandle(STD_INPUT_HANDLE))
    , _file(GetFileType(reinterpret_cast<HANDLE>(_handle)) != FILE_TYPE_CHAR ? true : false)
    , _reader(*this, _file ? EStringEncoder::UTF8 : EStringEncoder::UTF16)
#else
    : _handle(0)
    , _reader(*this, EStringEncoder::UTF8)
#endif
{
}

fsize ConsoleReader::Read(void *buf, fsize bufsize)
{
#ifdef WINDOWS
    if (_file)
    {
        DWORD out;
        if (!ReadFile(reinterpret_cast<HANDLE>(_handle), reinterpret_cast<LPVOID>(buf), (DWORD)bufsize, &out, NULL))
            throw IOException(String("Console write error: ") + OSPrivate::ObtainLastErrorString());
        return ((fsize)out);
    }
    else
    {
        DWORD out;
        if (!ReadConsoleW(reinterpret_cast<HANDLE>(_handle), reinterpret_cast<LPVOID>(buf), (DWORD)(bufsize / 2), &out, NULL))
            throw IOException(String("Console read error: ") + OSPrivate::ObtainLastErrorString());
        return ((fsize)(out * 2));
    }
#else
    int len = read(_handle, buf, bufsize);
    if (len < 0)
        throw IOException(String("Console read error: ") + OSPrivate::ObtainLastErrorString());
    return ((fsize)len);
#endif
}
