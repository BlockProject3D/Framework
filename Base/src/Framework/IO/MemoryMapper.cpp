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
    #include <iostream>
#else
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <unistd.h>
#endif
#include "Framework/IO/FileStream.hpp"
#include "Framework/IO/IOException.hpp"
#include "Framework/IO/MemoryMapper.hpp"
#include "OSPrivate.hpp"

using namespace bpf::io;
using namespace bpf;

MemoryMapper::MemoryMapper(const File &file, fint mode)
    : _file(file)
    , _mem(nullptr)
    , _mode(mode)
    , _memoff(nullptr)
{
#ifdef WINDOWS
    DWORD md = 0;
    DWORD md1 = 0;
    DWORD md2 = 0;

    if (mode & FILE_MODE_READ)
    {
        md |= GENERIC_READ;
        md1 |= OPEN_EXISTING;
    }
    if (mode & FILE_MODE_WRITE)
    {
        md |= GENERIC_WRITE;
        if (mode & FILE_MODE_TRUNCATE)
            md1 |= CREATE_ALWAYS;
        else
            md1 |= OPEN_EXISTING;
    }
    if ((mode & FILE_MODE_READ) && (mode & FILE_MODE_WRITE))
        md2 = PAGE_READWRITE;
    else
        md2 = PAGE_READONLY;
    _handle = CreateFileW(reinterpret_cast<LPCWSTR>(*file.PlatformPath().ToUTF16()), md, FILE_SHARE_READ, nullptr, md1, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (_handle == INVALID_HANDLE_VALUE)
        throw IOException(String("Could not open file '") + file.PlatformPath() + "' : " + OSPrivate::ObtainLastErrorString());
    _mapper = CreateFileMappingW(_handle, nullptr, md2, 0, 0, nullptr);
    if (_mapper == nullptr)
    {
        CloseHandle(_handle);
        throw IOException(String("Could not create mapper for file '") + file.PlatformPath() + "' : " + OSPrivate::ObtainLastErrorString());
    }
#else
    int md;
    if ((mode & FILE_MODE_READ) && (mode & FILE_MODE_WRITE))
        md = O_RDWR | O_CREAT;
    else if (mode & FILE_MODE_READ)
        md = O_RDONLY;
    else
        md = O_WRONLY;
    if (mode & FILE_MODE_TRUNCATE)
        md |= O_TRUNC;
    _handle = open(*file.PlatformPath(), md, 0644);
    if (_handle == -1)
        throw IOException(String("Could not open file '") + file.PlatformPath() + "' : " + OSPrivate::ObtainLastErrorString());
#endif
}

MemoryMapper::MemoryMapper(MemoryMapper &&other) noexcept
    : _file(std::move(other._file))
    , _mem(other._mem)
    , _mode(other._mode)
    , _memoff(other._memoff)
#ifdef WINDOWS
    , _handle(other._handle)
    , _mapper(other._mapper)
#else
    , _handle(other._handle)
    , _size(other._size)
#endif
{
    other._mem = nullptr;
    other._memoff = nullptr;
#ifdef WINDOWS
    other._handle = INVALID_HANDLE_VALUE;
    other._mapper = nullptr;
#else
    other._handle = -1;
    other._size = 0;
#endif
}

MemoryMapper::~MemoryMapper()
{
    Close();
}

void MemoryMapper::Close()
{
#ifdef WINDOWS
    if (_mem != nullptr)
        UnmapViewOfFile(_mem);
    if (_mapper != nullptr)
        CloseHandle(_mapper);
    if (_handle != INVALID_HANDLE_VALUE)
        CloseHandle(_handle);
    _handle = INVALID_HANDLE_VALUE;
    _mapper = nullptr;
#else
    if (_mem != nullptr)
        munmap(_mem, _size);
    if (_handle != -1)
        close(_handle);
    _handle = -1;
#endif
    _mem = nullptr;
    _memoff = nullptr;
}

MemoryMapper &MemoryMapper::operator=(MemoryMapper &&other) noexcept
{
    Close();
    _file = std::move(other._file);
    _mem = other._mem;
    _mode = other._mode;
    _memoff = other._memoff;
#ifdef WINDOWS
    _handle = other._handle;
    _mapper = other._mapper;
    other._handle = INVALID_HANDLE_VALUE;
    other._mapper = nullptr;
#else
    _handle = other._handle;
    _size = other._size;
    other._handle = -1;
    other._size = 0;
#endif
    return (*this);
}

#ifdef WINDOWS
union Unpack64 {
    uint64 _data;
    DWORD _parts[2];
};
#endif

void MemoryMapper::Map(uint64 pos, fsize size)
{
    if ((pos + size) > _file.GetSizeBytes())
        throw IOException(String("Could not map file '") + _file.PlatformPath() + "' : Mapped region is outside file boundarries");
#ifdef WINDOWS
    SYSTEM_INFO inf;
    GetSystemInfo(&inf);
    DWORD psize = inf.dwAllocationGranularity;
    uint64 nearestpsize = (pos / psize) * psize;
    _memoff = nullptr;
    if (_mem != nullptr)
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
    _mem = MapViewOfFile(_mapper, md, offsetHeigh, offsetLow, size);
    if (_mem == nullptr)
        throw IOException(String("Could not map file '") + _file.PlatformPath() + "' : " + OSPrivate::ObtainLastErrorString());
    uint8 *addr = reinterpret_cast<uint8 *>(_mem);
    addr += pos - nearestpsize;
    _memoff = addr;
#else
    long psize = sysconf(_SC_PAGE_SIZE);
    uint64 nearestpsize = (pos / psize) * psize;
    _memoff = nullptr;
    if (_mem != nullptr)
        munmap(_mem, _size);
    int md = 0;
    if (_mode & FILE_MODE_WRITE)
        md |= PROT_WRITE;
    if (_mode & FILE_MODE_READ)
        md |= PROT_READ;
    _mem = mmap(nullptr, size, md, MAP_SHARED, _handle, nearestpsize);
    _size = size;
    if (_mem == MAP_FAILED)
        throw IOException(String("Could not map file '") + _file.PlatformPath() + "' : " + OSPrivate::ObtainLastErrorString());
    auto *addr = reinterpret_cast<uint8 *>(_mem);
    addr += pos - nearestpsize;
    _memoff = addr;
#endif
}

void MemoryMapper::Unmap()
{
#ifdef WINDOWS
    if (_mem != nullptr)
        UnmapViewOfFile(_mem);
#else
    if (_mem != nullptr)
        munmap(_mem, _size);
#endif
    _mem = nullptr;
}
