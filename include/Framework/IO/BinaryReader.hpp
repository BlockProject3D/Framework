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

#pragma once
#include "Framework/IO/IDataInputStream.hpp"
#include "Framework/IO/ByteBuf.hpp"
#include "Framework/System/Platform.hpp"

namespace bpf
{
    class BPF_API BinaryReader final : public IDataInputStream
    {
    private:
        IInputStream &_stream;
        ByteBuf _buf;
        EPlatformEndianess _targetorder;
        bool _buffered;

        uint8 ReadByte();
        bool ReadByte2(uint8 &out);
        void ReadSubBuf(void *out, const fsize size);
    public:
        inline BinaryReader(IInputStream &stream, EPlatformEndianess order = PLATFORM_LITTLEENDIAN, bool buffered = true)
            : _stream(stream)
            , _buf(READ_BUF_SIZE)
            , _targetorder(order)
            , _buffered(buffered)
        {
        }

        fsize Read(void *buf, fsize bufsize);

        inline IDataInputStream &operator>>(uint8 &u)
        {
            u = ReadByte();
            return (*this);
        }

        inline IDataInputStream &operator>>(uint16 &u)
        {
            ReadSubBuf(&u, 2);
            return (*this);
        }

        inline IDataInputStream &operator>>(uint32 &u)
        {
            ReadSubBuf(&u, 4);
            return (*this);
        }

        inline IDataInputStream &operator>>(uint64 &u)
        {
            ReadSubBuf(&u, 8);
            return (*this);
        }

        inline IDataInputStream &operator>>(int8 &i)
        {
            i = (int8)ReadByte();
            return (*this);
        }

        inline IDataInputStream &operator>>(int16 &i)
        {
            ReadSubBuf(&i, 2);
            return (*this);
        }

        inline IDataInputStream &operator>>(int &i)
        {
            ReadSubBuf(&i, 4);
            return (*this);
        }

        inline IDataInputStream &operator>>(int64 &i)
        {
            ReadSubBuf(&i, 8);
            return (*this);
        }

        inline IDataInputStream &operator>>(float &f)
        {
            ReadSubBuf(&f, 4);
            return (*this);
        }

        inline IDataInputStream &operator>>(double &d)
        {
            ReadSubBuf(&d, 8);
            return (*this);
        }

        inline IDataInputStream &operator>>(bool &b)
        {
            uint8 u = ReadByte();
            b = (u == 1 ? true : false);
            return (*this);
        }

        IDataInputStream &operator>>(bpf::String &str);
    };
}
