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

#include "Framework/IO/BinaryReader.hpp"

using namespace bpf::io;
using namespace bpf;

uint8 BinaryReader::ReadByte()
{
    uint8 out = 0;

    if (!_buffered)
    {
        _stream.Read(&out, 1);
        return (out);
    }
    if ((_buf.GetCursor() + 1) > _buf.GetWrittenBytes())
    {
        _buf.Clear();
        uint8 buf[READ_BUF_SIZE];
        fsize s = _stream.Read(buf, READ_BUF_SIZE);
        _buf.Write(buf, s);
        _buf.Seek(0);
    }
    _buf.Read(&out, 1);
    return (out);
}

bool BinaryReader::ReadByte2(uint8 &out)
{
    out = 0;

    if (_buf.GetCursor() + 1 > _buf.GetWrittenBytes())
    {
        _buf.Clear();
        uint8 buf[READ_BUF_SIZE];
        fsize s = _stream.Read(buf, READ_BUF_SIZE);
        _buf.Write(buf, s);
        _buf.Seek(0);
    }
    if (_buf.GetCursor() + 1 > _buf.GetWrittenBytes())
        return (false);
    _buf.Read(&out, 1);
    return (true);
}

void BinaryReader::ReadSubBuf(void *out, const fsize size)
{
    auto *res = reinterpret_cast<uint8 *>(out);

    for (fsize i = 0; i != size; ++i)
        res[i] = ReadByte();
    if (system::Platform::GetEndianess() != _targetorder)
        system::Platform::ReverseBuffer(res, size);
}

IDataInputStream &BinaryReader::operator>>(bpf::String &str)
{
    uint32 size = 0;

    switch (_serializer)
    {
    case EStringSerializer::VARCHAR_32:
        str = "";
        ReadSubBuf(&size, 4);
        for (uint32 i = 0; i < size; ++i)
            str += (char)ReadByte();
        break;
    case EStringSerializer::VARCHAR_16:
        str = "";
        ReadSubBuf(&size, 2);
        for (uint32 i = 0; i < size; ++i)
            str += (char)ReadByte();
        break;
    case EStringSerializer::VARCHAR_8:
        str = "";
        ReadSubBuf(&size, 1);
        for (uint32 i = 0; i < size; ++i)
            str += (char)ReadByte();
        break;
    case EStringSerializer::CSTYLE:
        uint8 b;
        while ((b = ReadByte()) != 0)
            str += (char)b;
        break;
    }
    return (*this);
}

fsize BinaryReader::Read(void *buf, fsize bufsize)
{
    if (_buffered)
    {
        fsize read = 0;
        auto *data = reinterpret_cast<uint8 *>(buf);
        for (fsize i = 0; i != bufsize; ++i)
        {
            if (ReadByte2(data[i]))
                ++read;
        }
        return (read);
    }
    return (_stream.Read(buf, bufsize));
}
