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

namespace bpf
{
    namespace io
    {
        class BPF_API MemoryMapper
        {
        private:
            File _file;
            void *_mem;
            fint _mode;
            void *_memoff;
#ifdef WINDOWS
            void *_handle;
            void *_mapper;
#else
            int _handle;
            fsize _size;
#endif

        public:
            /**
             * Creates a new MemoryMapper from the given file and mode
             * @param file file to open
             * @param mode file mode
             * @throw IOException in case of system error
             */
            MemoryMapper(const File &file, fint mode);

            /**
             * Move constructor
             */
            MemoryMapper(MemoryMapper &&other) noexcept;

            ~MemoryMapper();

            /**
             * Cannot copy a MemoryMapper
             */
            MemoryMapper(const MemoryMapper &other) = delete;

            /**
             * Cannot copy a MemoryMapper
             */
            MemoryMapper operator=(const MemoryMapper &other) = delete;

            /**
             * Move assignment operator
             */
            MemoryMapper &operator=(MemoryMapper &&other) noexcept;

            /**
             * Map or re-map the file in virtual memory
             * @param pos position in bytes in the file to start mapping
             * @param size the size in bytes to map
             * @throw IOException in case of system error or if the size is greater than the size of the mapped file
             */
            void Map(uint64 pos, fsize size);

            /**
             * Returns the mapped file
             * @return immutable File reference
             */
            const File &GetFile() const noexcept
            {
                return (_file);
            }

            /**
             * Returns a pointer to the mapped file memory
             * @return raw pointer to the mapped file memory
             */
            inline void *operator*()
            {
                return (_memoff);
            }
        };
    }
}
