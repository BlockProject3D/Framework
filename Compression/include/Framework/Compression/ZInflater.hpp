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
#include <Framework/IO/ByteBuf.hpp>

namespace bpf
{
    namespace compression
    {
        /**
         * Represents data de-compression using ZLib
         */
        class BPF_COMPRESSION_API ZInflater
        {
        private:
            void *_handle;
            io::ByteBuf _input;
            bool _end;

        public:
            /**
             * Constructs a ZInflater
             */
            ZInflater();

            ~ZInflater();

            /**
             * Sets the input buffer
             * @param deflated buffer of data to de-compress
             */
            void SetInput(const io::ByteBuf &deflated);

            /**
             * Sets the input buffer
             * @param deflated buffer of data to de-compress
             */
            void SetInput(io::ByteBuf &&deflated);

            /**
             * Returns the Adler32 checksum value of the compressed data
             * @return 32 bits unsigned
             */
            uint32 GetAdler32() const;

            /**
             * Inflates a part of the input buffer into the given output buffer
             * @param out output buffer
             * @return unsigned number of bytes written to output buffer
             */
            fsize Inflate(io::ByteBuf &out);

            /**
             * Deflates a part of the input buffer into the given output buffer
             * @param out output buffer
             * @param size the size of the output buffer
             * @return unsigned number of bytes written to output buffer
             */
            fsize Inflate(void *out, fsize size);
        };
    }
}
