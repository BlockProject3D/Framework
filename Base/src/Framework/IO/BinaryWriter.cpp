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

#include "Framework/IO/BinaryWriter.hpp"
#include "Framework/IO/IOException.hpp"

using namespace bpf::io;
using namespace bpf;

void BinaryWriter::WriteByte(uint8 byte)
{
    if (!_buffered)
    {
        _stream.Write(&byte, 1);
        return;
    }
    if (_buf.GetWrittenBytes() >= _buf.Size())
    {
        _stream.Write(*_buf, WRITE_BUF_SIZE);
        _buf.Reset();
    }
    _buf.Write(&byte, 1);
}

void BinaryWriter::WriteSubBuf(void *out, const fsize size)
{
    auto *res = reinterpret_cast<uint8 *>(out);

    if (system::Platform::GetEndianess() != _targetorder)
        system::Platform::ReverseBuffer(res, size);
    for (fsize i = 0; i != size; ++i)
        WriteByte(res[i]);
}

IDataOutputStream &BinaryWriter::operator<<(const bpf::String &str)
{
    fisize size = str.Size();

    switch (_serializer)
    {
    case EStringSerializer::VARCHAR_32:
        WriteSubBuf(&size, 4);
        for (fisize i = 0; i < size; ++i)
            WriteByte((uint8)str.ByteAt(i));
        break;
    case EStringSerializer::VARCHAR_16:
        WriteSubBuf(&size, 2);
        for (fisize i = 0; i < (uint16)size; ++i)
            WriteByte((uint8)str.ByteAt(i));
        break;
    case EStringSerializer::VARCHAR_8:
        WriteSubBuf(&size, 1);
        for (fisize i = 0; i < (uint8)size; ++i)
            WriteByte((uint8)str.ByteAt(i));
        break;
    case EStringSerializer::CSTYLE:
        for (fisize i = 0; i < size; ++i)
            WriteByte((uint8)str.ByteAt(i));
        WriteByte(0);
        break;
    }
    return (*this);
}

IDataOutputStream &BinaryWriter::operator<<(const char *str)
{
    fisize size = 0;

    for (; str[size]; ++size)
        ;
    switch (_serializer)
    {
    case EStringSerializer::VARCHAR_32:
        WriteSubBuf(&size, 4);
        for (fisize i = 0; i < size; ++i)
            WriteByte((uint8)str[i]);
        break;
    case EStringSerializer::VARCHAR_16:
        WriteSubBuf(&size, 2);
        for (fisize i = 0; i < (uint16)size; ++i)
            WriteByte((uint8)str[i]);
        break;
    case EStringSerializer::VARCHAR_8:
        WriteSubBuf(&size, 1);
        for (fisize i = 0; i < (uint8)size; ++i)
            WriteByte((uint8)str[i]);
        break;
    case EStringSerializer::CSTYLE:
        for (fisize i = 0; i < size; ++i)
            WriteByte((uint8)str[i]);
        WriteByte(0);
        break;
    }
    return (*this);
}

fsize BinaryWriter::Write(const void *buf, fsize bufsize)
{
    if (_buffered)
    {
        auto *data = reinterpret_cast<const uint8 *>(buf);
        for (fsize i = 0; i != bufsize; ++i)
            WriteByte(data[i]);
        return (bufsize);
    }
    else
        return (_stream.Write(buf, bufsize));
}

void BinaryWriter::Flush()
{
    if (_buffered)
    {
        // Inverse logic to avoid re-throwing the same exception
        auto size = _buf.GetWrittenBytes();
        _buf.Reset();
        _stream.Write(*_buf, size);
    }
}

BinaryWriter::~BinaryWriter()
{
    try
    {
        if (_buffered)
            _stream.Write(*_buf, _buf.GetWrittenBytes());
    }
    catch (const IOException &e)
    {
        e.Print();
    }
}
