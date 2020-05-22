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
#include "Framework/IO/IInputStream.hpp"
#include "Framework/String.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * Recommended buffer size for reading
         */
        constexpr fsize READ_BUF_SIZE = 128;

        /**
         * Represents an arbitary stream with deserialization capability
         */
        class BPF_API IDataInputStream : public IInputStream
        {
        public:
            virtual ~IDataInputStream() {}

            /**
             * Deserialize a number
             * @param u deserialized uint8
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(uint8 &u) = 0;

            /**
             * Deserialize a number
             * @param u deserialized uint16
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(uint16 &u) = 0;

            /**
             * Deserialize a number
             * @param u deserialized uint21
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(uint32 &u) = 0;

            /**
             * Deserialize a number
             * @param u deserialized uint64
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(uint64 &u) = 0;

            /**
             * Deserialize a number
             * @param i deserialized int8
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(int8 &i) = 0;

            /**
             * Deserialize a number
             * @param i deserialized int16
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(int16 &i) = 0;

            /**
             * Deserialize a number
             * @param i deserialized int32
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(fint &i) = 0;

            /**
             * Deserialize a number
             * @param i deserialized int64
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(int64 &i) = 0;

            /**
             * Deserialize a number
             * @param f deserialized float
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(float &f) = 0;

            /**
             * Deserialize a number
             * @param d deserialized double
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(double &d) = 0;

            /**
             * Deserialize a boolean
             * @param b deserialized bool
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(bool &b) = 0;

            /**
             * Deserialize a string
             * @param str deserialized high-level string
             * @return IDataInputStream&
             */
            virtual IDataInputStream &operator>>(bpf::String &str) = 0;
        };
    }
}
