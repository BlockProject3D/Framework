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
#include "Framework/IO/IInputStream.hpp"
#include "Framework/IO/IOutputStream.hpp"
#include "Framework/IO/File.hpp"

namespace bpf
{
    constexpr fint FILE_MODE_READ = 0x10;
    constexpr fint FILE_MODE_WRITE = 0x20;
    constexpr fint FILE_MODE_APPEND = 0x40;
    constexpr fint FILE_MODE_TRUNCATE = 0x80;

    class BPF_API FileStream : public IInputStream, public IOutputStream
    {
    private:
        fint _mode;
#ifdef WINDOWS
        void *_handle;
#else
        int _handle;
#endif
    
#ifdef WINDOWS
        String ObtainErrorString();
#endif
    public:
        /**
         * Creates a new FileStream from the given file and mode
         * @param file file to open
         * @param mode file mode ored (ex : FILE_MODE_READ | FILE_MODE_WRITE)
         * @throws IOException
         */
        FileStream(const File &file, fint mode);
        ~FileStream();
        
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
         * @throws IOException
         */
        void SeekOffset(int64 offset);
        
        /**
         * Sets the file cursor position to pos
         * @param pos new cursor position
         * @throws IOException
         */
        void Seek(uint64 pos);
        
        /**
         * Closes this FileStream, calling any more IO functions will throw IOException
         */
        void Close();
        
        /**
         * @throws IOException
         */
        fsize Read(void *buf, fsize bufsize);

        /**
         * @throws IOException
         */
        fsize Write(const void *buf, fsize bufsize);
    };
}
