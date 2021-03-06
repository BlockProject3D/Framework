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

#include "Framework/IO/ByteBuf.hpp"
#include "Framework/Memory/Memory.hpp"
#include <cstring>

using namespace bpf::memory;
using namespace bpf::io;
using namespace bpf;

ByteBuf::ByteBuf(const fsize size)
    : _buf(static_cast<uint8 *>(Memory::Malloc(size)))
    , _cursor(0)
    , _size(size)
    , _written(0)
{
}

ByteBuf::ByteBuf(ByteBuf &&other) noexcept
    : _buf(other._buf)
    , _cursor(other._cursor)
    , _size(other._size)
    , _written(other._written)
{
    other._buf = nullptr;
    other._cursor = 0;
    other._size = 0;
    other._written = 0;
}

ByteBuf::ByteBuf(const ByteBuf &other)
    : _buf(static_cast<uint8 *>(Memory::Malloc(other._size)))
    , _cursor(other._cursor)
    , _size(other._size)
    , _written(other._written)
{
    std::memcpy(_buf, other._buf, _size);
}

ByteBuf &ByteBuf::operator=(const ByteBuf &other)
{
    if (this == &other)
        return (*this);
    if (_buf != nullptr)
        Memory::Free(_buf);
    _size = other._size;
    _cursor = other._cursor;
    _written = other._written;
    _buf = static_cast<uint8 *>(Memory::Malloc(other._size));
    std::memcpy(_buf, other._buf, _size);
    return (*this);
}

ByteBuf &ByteBuf::operator=(ByteBuf &&other) noexcept
{
    if (_buf != nullptr)
        Memory::Free(_buf);
    _size = other._size;
    _cursor = other._cursor;
    _written = other._written;
    _buf = other._buf;
    other._buf = nullptr;
    other._size = 0;
    other._cursor = 0;
    other._written = 0;
    return (*this);
    
}

ByteBuf::~ByteBuf()
{
    Memory::Free(_buf);
}

void ByteBuf::Reset()
{
    _written = 0;
    _cursor = 0;
}

void ByteBuf::Clear()
{
    _written = 0;
    _cursor = 0;
    std::memset(_buf, 0, _size);
}

void ByteBuf::Shift(fsize count)
{
    if (_cursor < count)
        count = _cursor;
    std::memmove(_buf, _buf + count, _written - count);
    _cursor -= count;
    _written -= count;
}

fsize ByteBuf::Write(const void *buf, fsize bufsize)
{
    if (_cursor + bufsize > _size)
        bufsize = _size - _cursor;
    std::memcpy(_buf + _cursor, buf, bufsize);
    _written = _cursor + bufsize;
    _cursor += bufsize;
    return (bufsize);
}

fsize ByteBuf::Read(void *buf, fsize bufsize)
{
    if (_cursor + bufsize > _written)
        bufsize = _written - _cursor;
    std::memcpy(buf, _buf + _cursor, bufsize);
    _cursor += bufsize;
    return (bufsize);
}
