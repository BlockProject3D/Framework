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

#include "Framework/IO/TextReader.hpp"
#include "Framework/Scalar.hpp"

using namespace bpf::io;
using namespace bpf;

bool TextReader::ReadByte2(uint8 &out)
{
    out = 0;

    if (!_buffered)
    {
        if (_stream.Read(&out, 1) == 1)
            return (true);
        return (false);
    }
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

bool TextReader::ReadSubBuf(void *out, const fsize size)
{
    uint8 *res = reinterpret_cast<uint8 *>(out);

    for (fsize i = 0; i != size; ++i)
    {
        if (!ReadByte2(res[i]))
            return (false);
    }
    return (true);
}

bool TextReader::CheckIsSeparator(uint8 byte)
{
    for (fisize i = 0; i != _seps.Size(); ++i)
    {
        if (_seps.ByteAt(i) == (char)byte)
            return (true);
    }
    return (false);
}

bool TextReader::Read(String &out)
{
    out = String::Empty;
    uint8 byte;

    while (ReadByte2(byte))
    {
        switch (_encoder)
        {
        case EStringEncoder::UTF8:
        {
            if (CheckIsSeparator(byte))
            {
                if (out.Size() > 0)
                    return (true);
            }
            else
                out.AddSingleByte((char)byte);
            break;
        }
        case EStringEncoder::UTF16:
        {
            uint8 bytes[4];
            if (!ReadByte2(bytes[1]))
                return (false);
            bytes[2] = 0;
            bytes[3] = 0;
            bytes[0] = byte;
            String str = String::FromUTF16(reinterpret_cast<const fchar16 *>(&bytes));
            if (str.Size() == 1 && CheckIsSeparator(str.ByteAt(0)))
            {
                if (out.Size() > 0)
                    return (true);
            }
            else
                out += str;
            break;
        }
        case EStringEncoder::UTF32:
        {
            uint8 bytes[8];
            if (!ReadSubBuf(bytes + 1, 3))
                return (false);
            bytes[4] = 0;
            bytes[5] = 0;
            bytes[6] = 0;
            bytes[7] = 0;
            bytes[0] = byte;
            String str = String::FromUTF32(reinterpret_cast<const fchar *>(&bytes));
            if (str.Size() == 1 && CheckIsSeparator(str.ByteAt(0)))
            {
                if (out.Size() > 0)
                    return (true);
            }
            else
                out += str;
            break;
        }
        }
    }
    return (out.Size() > 0 ? true : false);
}

bool TextReader::ReadLine(String &out)
{
    auto copy = _seps;
    _seps = "\r\n";
    bool flag = Read(out);
    _seps = std::move(copy);
    return (flag);
}

String TextReader::ReadAll()
{
    String str;
    uint8 byte;

    while (ReadByte2(byte))
        str.AddSingleByte(byte);
    return (str);
}

fsize TextReader::Read(void *buf, fsize bufsize)
{
    if (_buffered)
    {
        fsize read = 0;
        uint8 *data = reinterpret_cast<uint8 *>(buf);
        for (fsize i = 0; i != bufsize; ++i)
        {
            if (ReadByte2(data[i]))
                ++read;
        }
        return (read);
    }
    return (_stream.Read(buf, bufsize));
}

IDataInputStream &TextReader::operator>>(uint8 &u)
{
    String token;
    if (!Read(token))
        u = 0;
    else
        u = UInt8::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(uint16 &u)
{
    String token;
    if (!Read(token))
        u = 0;
    else
        u = UInt16::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(uint32 &u)
{
    String token;
    if (!Read(token))
        u = 0;
    else
        u = UInt::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(uint64 &u)
{
    String token;
    if (!Read(token))
        u = 0;
    else
        u = UInt64::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(int8 &i)
{
    String token;
    if (!Read(token))
        i = 0;
    else
        i = Int8::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(int16 &i)
{
    String token;
    if (!Read(token))
        i = 0;
    else
        i = Int16::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(fint &i)
{
    String token;
    if (!Read(token))
        i = 0;
    else
        i = Int::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(int64 &i)
{
    String token;
    if (!Read(token))
        i = 0;
    else
        i = Int64::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(float &f)
{
    String token;
    if (!Read(token))
        f = 0.0f;
    else
        f = Float::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(double &d)
{
    String token;
    if (!Read(token))
        d = 0.0;
    else
        d = Double::Parse(token);
    return (*this);
}

IDataInputStream &TextReader::operator>>(bool &b)
{
    String token;
    if (!Read(token))
        b = false;
    else
        b = token == "TRUE" ? true : false;
    return (*this);
}

IDataInputStream &TextReader::operator>>(bpf::String &str)
{
    String token;
    if (!Read(token))
        str = String::Empty;
    else
        str = token;
    return (*this);
}
