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
#include "Framework/IO/EStringSerializer.hpp"
#include "Framework/IO/IDataOutputStream.hpp"
#include "Framework/System/Platform.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * Binary serializer
         */
        class BPF_API BinaryWriter final : public IDataOutputStream
        {
        private:
            IOutputStream &_stream;
            ByteBuf _buf;
            system::EPlatformEndianess _targetorder;
            bool _buffered;
            EStringSerializer _serializer;

            void WriteByte(uint8 byte);
            void WriteSubBuf(void *in, fsize size);

        public:
            /**
             * Creates a BinaryWriter
             * @param stream the stream to write to
             * @param order what byte order to use when writing data to this stream
             * @param buffered true to allow buffering, false otherwise
             */
            explicit inline BinaryWriter(IOutputStream &stream, system::EPlatformEndianess order = system::PLATFORM_LITTLEENDIAN, bool buffered = true)
                : _stream(stream)
                , _buf(WRITE_BUF_SIZE)
                , _targetorder(order)
                , _buffered(buffered)
                , _serializer(EStringSerializer::VARCHAR_32)
            {
            }

            /**
             * Sets the string serializing function to use for reading strings
             * @param ser the new string serializing function
             */
            inline void SetStringSerializer(EStringSerializer ser)
            {
                _serializer = ser;
            }

            ~BinaryWriter() final;

            void Flush() final;

            /**
             * Writes raw bytes to this stream, taking into account buffering
             * @param buf the buffer with the bytes to write
             * @param bufsize the size of the buffer
             * @return number of bytes written
             */
            fsize Write(const void *buf, fsize bufsize) final;

            inline IDataOutputStream &operator<<(uint8 u) final
            {
                WriteByte(u);
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint16 u) final
            {
                WriteSubBuf(&u, 2);
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint32 u) final
            {
                WriteSubBuf(&u, 4);
                return (*this);
            }

            inline IDataOutputStream &operator<<(uint64 u) final
            {
                WriteSubBuf(&u, 8);
                return (*this);
            }

            inline IDataOutputStream &operator<<(int8 i) final
            {
                WriteByte((uint8)i);
                return (*this);
            }

            inline IDataOutputStream &operator<<(int16 i) final
            {
                WriteSubBuf(&i, 2);
                return (*this);
            }

            inline IDataOutputStream &operator<<(fint i) final
            {
                WriteSubBuf(&i, 4);
                return (*this);
            }

            inline IDataOutputStream &operator<<(int64 i) final
            {
                WriteSubBuf(&i, 8);
                return (*this);
            }

            inline IDataOutputStream &operator<<(float f) final
            {
                WriteSubBuf(&f, 4);
                return (*this);
            }

            inline IDataOutputStream &operator<<(double d) final
            {
                WriteSubBuf(&d, 8);
                return (*this);
            }

            inline IDataOutputStream &operator<<(bool b) final
            {
                if (b)
                    WriteByte(1);
                else
                    WriteByte(0);
                return (*this);
            }

            IDataOutputStream &operator<<(const bpf::String &str) final;
            IDataOutputStream &operator<<(const char *str) final;
        };
    }
}
