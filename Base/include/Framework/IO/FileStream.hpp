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
#include "Framework/IO/File.hpp"
#include "Framework/IO/IInputStream.hpp"
#include "Framework/IO/IOutputStream.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * Open a file for always appending data at the end
         */
        constexpr fint FILE_MODE_APPEND = 0x40;

        /**
         * If this flag is specified when opening a file, the file is always emptied
         */
        constexpr fint FILE_MODE_TRUNCATE = 0x80;

        /**
         * Class to represent a file stream open as read, write or random access
         */
        class BPF_API FileStream final : public IInputStream, public IOutputStream
        {
        private:
            fint _mode;
#ifdef WINDOWS
            void *_handle;
#else
            int _handle;
#endif

        public:
            /**
             * Creates a new FileStream from the given file and mode
             * @param file file to open
             * @param mode file mode and flags or'ed (ex : FILE_MODE_READ | FILE_MODE_WRITE)
             * @throw IOException in case of system error
             */
            FileStream(const File &file, fint mode);

            ~FileStream() final;

            /**
             * Cannot copy a FileStream
             */
            FileStream(const FileStream &other) = delete;

            /**
             * Cannot copy a FileStream
             */
            FileStream operator=(const FileStream &other) = delete;

            /**
             * Sets the file cursor position to an offset relative to the current position
             * @param offset relative offset
             * @throw IOException in case of system error
             */
            void SeekOffset(int64 offset) const;

            /**
             * Sets the file cursor position to pos
             * @param pos new cursor position
             * @throw IOException in case of system error
             */
            void Seek(uint64 pos) const;

            /**
             * Closes this FileStream, calling any more IO functions will throw IOException
             */
            void Close();

            /**
             * Reads bytes from this stream, no buffering is performed
             * @param buf buffer to receive the read bytes
             * @param bufsize the size of the receiving buffer
             * @throw IOException in case of system error
             * @return number of bytes read
             */
            fsize Read(void *buf, fsize bufsize) final;

            /**
             * Writes bytes to this stream, no buffering is performed
             * @param buf the buffer with the bytes to write
             * @param bufsize the size of the buffer
             * @throw IOException in case of system error
             * @return number of bytes written
             */
            fsize Write(const void *buf, fsize bufsize) final;
        };
    }
}
