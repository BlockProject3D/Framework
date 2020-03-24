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
#include "Framework/IO/IDataInputStream.hpp"

namespace bpf
{
    namespace io
    {
        class BPF_API TextReader final : public IDataInputStream
        {
        private:
            IInputStream &_stream;
            ByteBuf _buf;
            bool _buffered;
            String _seps;
            EStringEncoder _encoder;

            bool ReadByte2(uint8 &out);
            bool ReadSubBuf(void *out, const fsize size);
            bool CheckIsSeparator(uint8 byte);

        public:
            explicit inline TextReader(IInputStream &stream, const EStringEncoder encoder = EStringEncoder::UTF8, bool buffered = true)
                : _stream(stream)
                , _buf(READ_BUF_SIZE)
                , _buffered(buffered)
                , _seps("\r\n\t ")
                , _encoder(encoder)
            {
            }

            /**
             * Sets the possible characters to find that marks the end of a token.
             * We assume the separators can only be ASCII characters
             * @param str a string where each characters represents a character to seach for the end of a token
             */
            inline void SetTokenSeparators(const char *str)
            {
                _seps = str;
            }

            fsize Read(void *buf, fsize bufsize);

            bool ReadLine(String &out);

            bool Read(String &out);

            String ReadAll();

            IDataInputStream &operator>>(uint8 &u);

            IDataInputStream &operator>>(uint16 &u);

            IDataInputStream &operator>>(uint32 &u);

            IDataInputStream &operator>>(uint64 &u);

            IDataInputStream &operator>>(int8 &i);

            IDataInputStream &operator>>(int16 &i);

            IDataInputStream &operator>>(fint &i);

            IDataInputStream &operator>>(int64 &i);

            IDataInputStream &operator>>(float &f);

            IDataInputStream &operator>>(double &d);

            IDataInputStream &operator>>(bool &b);

            IDataInputStream &operator>>(bpf::String &str);
        };
    }
}
