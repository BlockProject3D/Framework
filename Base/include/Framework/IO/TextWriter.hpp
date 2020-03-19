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

#pragma once
#include "Framework/IO/ByteBuf.hpp"
#include "Framework/IO/EStringEncoder.hpp"
#include "Framework/IO/IDataOutputStream.hpp"

namespace bpf
{
    namespace io
    {
        class BPF_API TextWriter final : public IDataOutputStream
        {
        private:
            IOutputStream &_stream;
            ByteBuf _buf;
            bool _buffered;
            fchar _sep;
            EStringEncoder _encoder;

            void WriteByte(uint8 byte);
            void WriteSubBuf(const void *in, const fsize size);

        public:
            explicit inline TextWriter(IOutputStream &stream, const EStringEncoder encoder = EStringEncoder::UTF8, bool buffered = true)
                : _stream(stream)
                , _buf(WRITE_BUF_SIZE)
                , _buffered(buffered)
                , _sep(' ')
                , _encoder(encoder)
            {
            }

            inline void SetTokenSeparator(const fchar ch)
            {
                _sep = ch;
            }

            inline ~TextWriter()
            {
                if (_buffered)
                    _stream.Write(_buf.GetRawData(), _buf.GetWrittenBytes());
            }

            void Flush();

            fsize Write(const void *buf, fsize bufsize);

            void WriteLine(const String &str);

            void Write(const String &str);

            void NewLine();

            inline IDataOutputStream &operator<<(uint8 u)
            {
                Write(String::ValueOf(u) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint16 u)
            {
                Write(String::ValueOf(u) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint32 u)
            {
                Write(String::ValueOf(u) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint64 u)
            {
                Write(String::ValueOf(u) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(int8 i)
            {
                Write(String::ValueOf(i) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(int16 i)
            {
                Write(String::ValueOf(i) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(fint i)
            {
                Write(String::ValueOf(i) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(int64 i)
            {
                Write(String::ValueOf(i) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(float f)
            {
                Write(String::ValueOf(f) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(double d)
            {
                Write(String::ValueOf(d) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(bool b)
            {
                Write(String::ValueOf(b) + _sep);
                return (*this);
            }

            inline IDataOutputStream &operator<<(const bpf::String &str)
            {
                Write(str + _sep);
                return (*this);
            }

            IDataOutputStream &operator<<(const char *str)
            {
                Write(String(str) + _sep);
                return (*this);
            }
        };
    }
}
