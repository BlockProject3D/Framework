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
#include "Framework/IO/TextReader.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * High-level console deserializer
         */
        class BPF_API ConsoleReader final : public IDataInputStream
        {
        private:
#ifdef WINDOWS
            void *_handle;
            bool _file;
#else
            int _handle;
#endif
            TextReader _reader;

        public:
            /**
             * Constructs a ConsoleReader
             */
            ConsoleReader();

            /**
             * Sets the possible characters to find that marks the end of a token.
             * We assume the separators can only be ASCII characters
             * @param str a string where each characters represents a character to seach for the end of a token
             */
            inline void SetTokenSeparators(const char *str)
            {
                _reader.SetTokenSeparators(str);
            }

            /**
             * This function performs a low level read from standard input
             * On Windows it fills buf with UTF-16 code points
             * On Linux it fills buf with UTF-8 bytes (last UTF-8 code might be cut)
             * @param buf the buffer to receive the characters
             * @param bufsize size in bytes of the buffer
             * @throw IOException in case of system error
             * @return fsize number of bytes read
             */
            fsize Read(void *buf, fsize bufsize) final;

            /**
             * Reads a line of text
             * @param out the output text without the line separator character
             * @return true if anything was read, false otherwise
             */
            inline bool ReadLine(String &out)
            {
                return (_reader.ReadLine(out));
            }

            /**
             * Reads a single token
             * @param out the output token text
             * @return true if anything was read, false otherwise
             */
            inline bool Read(String &out)
            {
                return (_reader.Read(out));
            }

            inline IDataInputStream &operator>>(uint8 &u) final
            {
                _reader >> u;
                return (*this);
            }

            inline IDataInputStream &operator>>(uint16 &u) final
            {
                _reader >> u;
                return (*this);
            }

            inline IDataInputStream &operator>>(uint32 &u) final
            {
                _reader >> u;
                return (*this);
            }

            inline IDataInputStream &operator>>(uint64 &u) final
            {
                _reader >> u;
                return (*this);
            }

            inline IDataInputStream &operator>>(int8 &i) final
            {
                _reader >> i;
                return (*this);
            }

            inline IDataInputStream &operator>>(int16 &i) final
            {
                _reader >> i;
                return (*this);
            }

            inline IDataInputStream &operator>>(fint &i) final
            {
                _reader >> i;
                return (*this);
            }

            inline IDataInputStream &operator>>(int64 &i) final
            {
                _reader >> i;
                return (*this);
            }

            inline IDataInputStream &operator>>(float &f) final
            {
                _reader >> f;
                return (*this);
            }

            inline IDataInputStream &operator>>(double &d) final
            {
                _reader >> d;
                return (*this);
            }

            inline IDataInputStream &operator>>(bool &b) final
            {
                _reader >> b;
                return (*this);
            }

            inline IDataInputStream &operator>>(bpf::String &str) final
            {
                _reader >> str;
                return (*this);
            }
        };
    }
}