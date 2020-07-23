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

#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <fcntl.h>
    #include <unistd.h>
#endif
#include "Framework/IO/FileStream.hpp"
#include "Framework/IO/IOException.hpp"
#include "OSPrivate.hpp"

using namespace bpf::io;
using namespace bpf;

FileStream::FileStream(const File &file, fint mode)
    : _mode(mode)
{
#ifdef WINDOWS
    DWORD md = 0;
    DWORD md1 = 0;

    if (mode & FILE_MODE_READ)
    {
        md |= GENERIC_READ;
        if (!(mode & FILE_MODE_TRUNCATE))
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
    _handle = CreateFileW(reinterpret_cast<LPCWSTR>(*file.PlatformPath().ToUTF16()), md, FILE_SHARE_READ, nullptr, md1, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (_handle == INVALID_HANDLE_VALUE)
        throw IOException(String("Could not open file '") + file.PlatformPath() + "' : " + OSPrivate::ObtainLastErrorString());
    if (mode & FILE_MODE_APPEND)
    {
        LARGE_INTEGER pos;
        pos.QuadPart = 0;
        SetFilePointerEx(_handle, pos, nullptr, FILE_END);
    }
#else
    int md = 0;

    if ((mode & FILE_MODE_READ) && (mode & FILE_MODE_WRITE))
        md |= O_RDWR | O_CREAT;
    else if (mode & FILE_MODE_APPEND)
        md |= O_APPEND | O_CREAT | O_WRONLY;
    else if (mode & FILE_MODE_READ)
        md |= O_RDONLY;
    else if (mode & FILE_MODE_WRITE)
        md |= O_WRONLY | O_CREAT;
    if (mode & FILE_MODE_TRUNCATE)
        md |= O_TRUNC;
    _handle = open(*file.PlatformPath(), md, 0644);
    if (_handle == -1)
        throw IOException(String("Could not open file '") + file.PlatformPath() + "' : " + OSPrivate::ObtainLastErrorString());
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

void FileStream::SeekOffset(int64 offset) const
{
    if (_mode & FILE_MODE_APPEND)
        throw IOException("Cannot Seek in append mode");
#ifdef WINDOWS
    if (_handle == INVALID_HANDLE_VALUE)
        throw IOException("File is closed");
    LARGE_INTEGER pos;
    pos.QuadPart = offset;
    SetFilePointerEx(_handle, pos, nullptr, FILE_CURRENT);
#else
    if (_handle == -1)
        throw IOException("File is closed");
    lseek(_handle, offset, SEEK_CUR);
#endif
}

void FileStream::Seek(uint64 pos) const
{
    if (_mode & FILE_MODE_APPEND)
        throw IOException("Cannot Seek in append mode");
#ifdef WINDOWS
    if (_handle == INVALID_HANDLE_VALUE)
        throw IOException("File is closed");
    LARGE_INTEGER pos1;
    pos1.QuadPart = pos;
    SetFilePointerEx(_handle, pos1, nullptr, FILE_BEGIN);
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
    if (ReadFile(_handle, buf, (DWORD)bufsize, &readsize, nullptr) == FALSE)
        throw IOException(String("Error reading file : ") + OSPrivate::ObtainLastErrorString());
    return (readsize);
#else
    fsize len = read(_handle, buf, bufsize);
    if (len == (fsize)-1)
        throw IOException(String("Error reading file : ") + OSPrivate::ObtainLastErrorString());
    return (len);
#endif
}

fsize FileStream::Write(const void *buf, fsize bufsize)
{
    if (!(_mode & FILE_MODE_WRITE))
        throw IOException("File has not been oppened with write mode");
#ifdef WINDOWS
    DWORD writesize;
    if (WriteFile(_handle, buf, (DWORD)bufsize, &writesize, nullptr) == FALSE)
        throw IOException(String("Error writing file : ") + OSPrivate::ObtainLastErrorString());
    return (writesize);
#else
    fsize len = write(_handle, buf, bufsize);
    if (len == (fsize)-1)
        throw IOException(String("Error writing file : ") + OSPrivate::ObtainLastErrorString());
    return (len);
#endif
}
