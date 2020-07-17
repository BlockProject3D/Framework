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

#pragma once
#include "Framework/IO/ByteBuf.hpp"
#include "Framework/IO/ECharacterEncoding.hpp"
#include "Framework/IO/IDataOutputStream.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * Text serializer
         */
        class BPF_API TextWriter final : public IDataOutputStream
        {
        private:
            IOutputStream &_stream;
            ByteBuf _buf;
            bool _buffered;
            ECharacterEncoding _encoder;

            void WriteByte(uint8 byte);
            void WriteSubBuf(const void *in, fsize size);

        public:
            /**
             * Creates a TextWriter
             * @param stream the stream to write to
             * @param encoder the string encoding to use
             * @param buffered true to allow buffering, false otherwise
             */
            explicit inline TextWriter(IOutputStream &stream, const ECharacterEncoding encoder = ECharacterEncoding::UTF8, bool buffered = true)
                : _stream(stream)
                , _buf(WRITE_BUF_SIZE)
                , _buffered(buffered)
                , _encoder(encoder)
            {
            }

            ~TextWriter() final;

            void Flush() final;

            /**
             * Writes raw bytes to this stream, taking into account buffering
             * @param buf the buffer with the bytes to write
             * @param bufsize the size of the buffer
             * @return number of bytes written
             */
            fsize Write(const void *buf, fsize bufsize) final;

            /**
             * Writes a line of text
             * @param str text to write
             */
            void WriteLine(const String &str);

            /**
             * Writes a string (without appending newline character)
             * @param str string to write
             */
            void Write(const String &str);

            /**
             * Writes a platform dependent newline character
             */
            void NewLine();

            inline IDataOutputStream &operator<<(uint8 u) final
            {
                Write(String::ValueOf(u));
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint16 u) final
            {
                Write(String::ValueOf(u));
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint32 u) final
            {
                Write(String::ValueOf(u));
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint64 u) final
            {
                Write(String::ValueOf(u));
                return (*this);
            }

            inline IDataOutputStream &operator<<(int8 i) final
            {
                Write(String::ValueOf(i));
                return (*this);
            }

            inline IDataOutputStream &operator<<(int16 i) final
            {
                Write(String::ValueOf(i));
                return (*this);
            }

            inline IDataOutputStream &operator<<(fint i) final
            {
                Write(String::ValueOf(i));
                return (*this);
            }

            inline IDataOutputStream &operator<<(int64 i) final
            {
                Write(String::ValueOf(i));
                return (*this);
            }

            inline IDataOutputStream &operator<<(float f) final
            {
                Write(String::ValueOf(f));
                return (*this);
            }

            inline IDataOutputStream &operator<<(double d) final
            {
                Write(String::ValueOf(d));
                return (*this);
            }

            inline IDataOutputStream &operator<<(bool b) final
            {
                Write(String::ValueOf(b));
                return (*this);
            }

            inline IDataOutputStream &operator<<(const bpf::String &str) final
            {
                Write(str);
                return (*this);
            }

            inline IDataOutputStream &operator<<(const char *str) final
            {
                Write(String(str));
                return (*this);
            }
        };
    }
}
