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

#include "Framework/IO/TextWriter.hpp"
#include "Framework/IO/IOException.hpp"

using namespace bpf::io;
using namespace bpf;

void TextWriter::WriteByte(uint8 byte)
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

void TextWriter::WriteSubBuf(const void *out, const fsize size)
{
    const uint8 *res = reinterpret_cast<const uint8 *>(out);

    for (fsize i = 0; i != size; ++i)
        WriteByte(res[i]);
}

void TextWriter::Write(const String &str)
{
    switch (_encoder)
    {
    case EStringEncoder::UTF8:
        WriteSubBuf(*str, str.Size());
        break;
    case EStringEncoder::UTF16:
    {
        auto buf = str.ToUTF16();
        WriteSubBuf(*buf, sizeof(bpf::fchar16) * (buf.Size() - 1));
        break;
    }
    case EStringEncoder::UTF32:
    {
        auto buf = str.ToUTF32();
        WriteSubBuf(*buf, sizeof(bpf::fchar) * (buf.Size() - 1));
        break;
    }
    }
}

void TextWriter::WriteLine(const String &str)
{
    Write(str);
    NewLine();
}

void TextWriter::NewLine()
{
#ifdef WINDOWS
    Write("\r\n");
#else
    Write("\n");
#endif
}

fsize TextWriter::Write(const void *buf, fsize bufsize)
{
    if (_buffered)
    {
        const uint8 *data = reinterpret_cast<const uint8 *>(buf);
        for (fsize i = 0; i != bufsize; ++i)
            WriteByte(data[i]);
        return (bufsize);
    }
    else
        return (_stream.Write(buf, bufsize));
}

void TextWriter::Flush()
{
    if (_buffered)
    {
        _stream.Write(*_buf, _buf.GetWrittenBytes());
        _buf.Reset();
    }
}

TextWriter::~TextWriter()
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
