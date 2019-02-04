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

#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <cstring>
#endif
#include "Framework/IO/FileStream.hpp"
#include "Framework/IO/IOException.hpp"

using namespace bpf;

#ifdef WINDOWS
String FileStream::ObtainErrorString()
{
    String res = "Unknown";
    LPTSTR errtxt = Null;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_IGNORE_INSERTS,
        Null, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPTSTR>(&errtxt), 0, Null);
    if (errtxt != Null)
    {
        res = errtxt;
        LocalFree(errtxt);
    }
    return (res);
}
#endif

FileStream::FileStream(const File &file, int mode)
    : _mode(mode)
{
#ifdef WINDOWS
    DWORD md = 0;
    DWORD md1 = 0;

    if (mode & FILE_MODE_READ)
    {
        md |= GENERIC_READ;
        md1 |= OPEN_EXISTING;
    }
    if ((mode & FILE_MODE_WRITE) || (mode & FILE_MODE_APPEND))
    {
        md |= GENERIC_WRITE;
        if (mode & FILE_MODE_TRUNCATE)
            md1 |= CREATE_ALWAYS;
        else
            md1 |= OPEN_ALWAYS;
    }
    _handle = CreateFile(*file.GetAbsolutePath().Path(), md, FILE_SHARE_READ, Null, md1, FILE_ATTRIBUTE_NORMAL, Null);
    if (_handle == INVALID_HANDLE_VALUE)
        throw IOException(String("Could not open file '")
            + file.GetAbsolutePath().Path() + "' : "
            + ObtainErrorString());
    if (mode & FILE_MODE_APPEND)
    {
        LARGE_INTEGER pos;
        pos.QuadPart = 0;
        SetFilePointerEx(_handle, pos, Null, FILE_END);
    }
#else
    int md = 0;
    
    if ((mode & FILE_MODE_READ) && (mode & FILE_MODE_WRITE))
        md |= O_RDWR | O_CREAT;
    else if (mode & FILE_MODE_APPEND)
        md |= O_APPEND | O_CREAT;
    else if (mode & FILE_MODE_READ)
        md |= O_RDONLY;
    else if (mode & FILE_MODE_WRITE)
        md |= O_WRONLY | O_CREAT;
    if (mode & FILE_MODE_TRUNCATE)
        md |= O_TRUNC;
    _handle = open(*file.GetAbsolutePath().Path(), md, 0644);
    if (_handle == -1)
        throw IOException(String("Could not open file '")
            + file.GetAbsolutePath().Path() + "' : "
            + String(std::strerror(errno)));
#endif
}

FileStream::~FileStream()
{
#ifdef WINDOWS
    if (_handle != INVALID_HANDLE_VALUE)
        CloseHandle(_handle);
#else
    if (_handle != -1)
        close(_handle);
#endif
}

void FileStream::SeekOffset(int64 offset)
{
    if (_mode & FILE_MODE_APPEND)
        throw IOException("Cannot Seek in append mode");
#ifdef WINDOWS
    if (_handle == INVALID_HANDLE_VALUE)
        throw IOException("File is closed");
    LARGE_INTEGER pos;
    pos.QuadPart = offset;
    SetFilePointerEx(_handle, pos, Null, FILE_CURRENT);
#else
    if (_handle == -1)
        throw IOException("File is closed");
    lseek(_handle, offset, SEEK_CUR);
#endif
}

void FileStream::Seek(uint64 pos)
{
    if (_mode & FILE_MODE_APPEND)
        throw IOException("Cannot Seek in append mode");
#ifdef WINDOWS
    if (_handle == INVALID_HANDLE_VALUE)
        throw IOException("File is closed");
    LARGE_INTEGER pos1;
    pos1.QuadPart = pos;
    SetFilePointerEx(_handle, pos1, Null, FILE_BEGIN);
#else
    if (_handle == -1)
        throw IOException("File is closed");
    lseek(_handle, pos, SEEK_SET);
#endif
}

void FileStream::Close()
{
#ifdef WINDOWS
    if (_handle == INVALID_HANDLE_VALUE)
        return;
    CloseHandle(_handle);
    _handle = INVALID_HANDLE_VALUE;
#else
    if (_handle == -1)
        return;
    close(_handle);
    _handle = -1;
#endif
}

fsize FileStream::Read(void *buf, fsize bufsize)
{
    if (!(_mode & FILE_MODE_READ))
        throw IOException("File has not been oppened with read mode");
#ifdef WINDOWS
    DWORD readsize;
    if (ReadFile(_handle, buf, bufsize, &readsize, Null) == FALSE)
        return (0);
    return (readsize);
#else
    return (read(_handle, buf, bufsize));
#endif
}

fsize FileStream::Write(const void *buf, fsize bufsize)
{
    if (!(_mode & FILE_MODE_WRITE))
        throw IOException("File has not been oppened with write mode");
#ifdef WINDOWS
    DWORD writesize;
    if (WriteFile(_handle, buf, bufsize, &writesize, Null) == FALSE)
        return (0);
    return (writesize);
#else
    return (write(_handle, buf, bufsize));
#endif
}
