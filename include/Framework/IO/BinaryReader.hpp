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

namespace bpf
{
    class BPF_API BinaryReader final : public IDataInputStream
    {
    private:
        IInputStream &_stream;
        ByteBuf _buf;

        
    public:
        inline BinaryReader(IInputStream &stream, bool buf)
            : _stream(stream), _buf(READ_BUF_SIZE)
        {
        }

        inline fsize Read(void *buf, fsize bufsize)
        {
            return (_stream.Read(buf, bufsize));
        }

        IDataInputStream &operator>>(uint8 &u);
        IDataInputStream &operator>>(uint16 &u);
        IDataInputStream &operator>>(uint32 &u);
        IDataInputStream &operator>>(uint64 &u);
        IDataInputStream &operator>>(int8 &i);
        IDataInputStream &operator>>(int16 &i);
        IDataInputStream &operator>>(int &i);
        IDataInputStream &operator>>(int64 &i);
        IDataInputStream &operator>>(float &f);
        IDataInputStream &operator>>(double &d);
        IDataInputStream &operator>>(bool &b);
        IDataInputStream &operator>>(bpf::String &str);
    };
}
