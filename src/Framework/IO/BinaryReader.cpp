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

#include "Framework/IO/BinaryReader.hpp"

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
    }
    _buf.Read(&out, 1);
    return (out);
}

void BinaryReader::ReadSubBuf(void *out, const fsize size)
{
    uint8 *res = reinterpret_cast<uint8 *>(out);

    for (fsize i = 0; i != size; ++i)
        res[i] = ReadByte();
    if (Platform::GetEndianess() != _targetorder)
        Platform::ReverseBuffer(res, size);
}

IDataInputStream &BinaryReader::operator>>(bpf::String &str)
{
    uint32 size = 0;
    str = String::Empty;

    ReadSubBuf(&size, 4);
    for (uint32 i = 0; i < size; ++i)
        str += (char)ReadByte();
    return (*this);
}
