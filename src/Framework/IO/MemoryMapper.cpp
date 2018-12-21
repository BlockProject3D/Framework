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
    #include <iostream>
#else
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/mman.h>
    #include <errno.h>
    #include <cstring>
#endif
#include "Framework/IO/MemoryMapper.hpp"
#include "Framework/IO/IOException.hpp"
#include "Framework/IO/FileStream.hpp"

using namespace bpf;

#ifdef WINDOWS
String MemoryMapper::ObtainErrorString()
{
    String res = "Unknown";
    LPTSTR errtxt = Null;

    std::cout << GetLastError() << std::endl;
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

MemoryMapper::MemoryMapper(const File &file, int mode)
    : _file(file)
    , _mem(Null)
    , _mode(mode)
    , _memoff(Null)
{
#ifdef WINDOWS
    DWORD md = 0;

    if (mode & FILE_MODE_READ)
        md |= FILE_MAP_READ;
    if (mode & FILE_MODE_WRITE)
        md |= FILE_MAP_WRITE;
    _handle = OpenFileMapping(md, FALSE, *file.GetAbsolutePath().GetPath());
    if (_handle == Null)
        throw IOException(String("Could not open file '")
            + file.GetAbsolutePath().GetPath() + "' : "
            + ObtainErrorString());
#else
    int md = 0;
    if ((mode & FILE_MODE_READ) && (mode & FILE_MODE_WRITE))
        md = O_RDWR | O_CREAT;
    else if (mode & FILE_MODE_READ)
        md = O_RDONLY;
    else
        md = O_WRONLY | O_CREAT;
    if (mode & FILE_MODE_TRUNCATE)
        md |= O_TRUNC;
    _handle = open(*file.GetAbsolutePath().GetPath(), md, 0644);
    if (_handle == -1)
        throw IOException(String("Could not open file '")
            + file.GetAbsolutePath().GetPath() + "' : "
            + String(std::strerror(errno)));
#endif
}

MemoryMapper::~MemoryMapper()
{
#ifdef WINDOWS
    if (_mem != Null)
        UnmapViewOfFile(_mem);
    CloseHandle(_handle);
#else
    if (_mem != Null)
        munmap(_mem, _size);
    close(_handle);
#endif
}

#ifdef WINDOWS
union Unpack64
{
    uint64 _data;
    DWORD _parts[2];
};
#endif

void MemoryMapper::Map(uint64 pos, fsize size)
{
    if ((pos + size) > _file.GetSizeBytes())
        throw IOException(String("Could not map file '")
            + _file.GetAbsolutePath().GetPath()
            + "' : Mapped region is outside file boundarries");
#ifdef WINDOWS
    SYSTEM_INFO inf;
    GetSystemInfo(&inf);
    DWORD psize = inf.dwAllocationGranularity;
    uint64 nearestpsize = (pos / psize) * psize;
    _memoff = Null;
    if (_mem != Null)
        UnmapViewOfFile(_mem);
    DWORD md = 0;
    if (_mode & FILE_MODE_WRITE)
        md |= FILE_MAP_WRITE;
    if (_mode & FILE_MODE_READ)
        md |= FILE_MAP_READ;
    DWORD offsetLow;
    DWORD offsetHeigh;
    Unpack64 up;
    up._data = nearestpsize;
    offsetLow = up._parts[0];
    offsetHeigh = up._parts[1];
    _mem = MapViewOfFile(_handle, md, offsetHeigh, offsetLow, size);
    if (_mem == Null)
        throw IOException(String("Could not map file '")
            + _file.GetAbsolutePath().GetPath() + "' : "
            + ObtainErrorString());
    uint8 *addr = reinterpret_cast<uint8 *>(_mem);
    addr += pos - nearestpsize;
    _memoff = addr;
#else
    long psize = sysconf(_SC_PAGE_SIZE);
    uint64 nearestpsize = (pos / psize) * psize;
    _memoff = Null;
    if (_mem != Null)
        munmap(_mem, _size);
    int md = 0;
    if (_mode & FILE_MODE_WRITE)
        md |= PROT_WRITE;
    if (_mode & FILE_MODE_READ)
        md |= PROT_READ;
    _mem = mmap(Null, size, md, MAP_SHARED, _handle, nearestpsize);
    _size = size;
    if (_mem == MAP_FAILED)
        throw IOException(String("Could not map file '")
            + _file.GetAbsolutePath().GetPath() + "' : "
            + String(std::strerror(errno)));
    uint8 *addr = reinterpret_cast<uint8 *>(_mem);
    addr += pos - nearestpsize;
    _memoff = addr;
#endif
}
