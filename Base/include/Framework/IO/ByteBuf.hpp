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
#include "Framework/IO/IOutputStream.hpp"
#include "Framework/IndexException.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * Low-level fixed size buffer that can store raw bytes
         */
        class BPF_API ByteBuf final : public IInputStream, public IOutputStream
        {
        private:
            uint8 *_buf;
            fsize _cursor;
            fsize _size;
            fsize _written;

        public:
            /**
             * Constructs a ByteBuf
             * WARNING: ByteBuf does not automatically set bytes to 0
             * @param size the maximum size in bytes of the buffer
             */
            explicit ByteBuf(fsize size);

            /**
             * Move constructor
             */
            ByteBuf(ByteBuf &&other) noexcept;

            /**
             * Copy constructor
             */
            ByteBuf(const ByteBuf &other);

            ~ByteBuf() final;

            /**
             * Copy assignment operator
             */
            ByteBuf &operator=(const ByteBuf &other);

            /**
             * Move assignment operator
             */
            ByteBuf &operator=(ByteBuf &&other) noexcept;

            /**
             * Clears this buffer by resetting all bytes inside to the value 0
             */
            void Clear();

            /**
             * Reset the pointers of this buffer to the beginning
             */
            void Reset();

            /**
             * Shift the bytes in this buffer
             * @param count number of bytes for shifting
             */
            void Shift(fsize count);

            fsize Write(const void *buf, fsize bufsize) final;

            fsize Read(void *buf, fsize bufsize) final;

            /**
             * Returns a raw pointer to the beginning of this buffer
             * @return mutable pointer to the buffer start 
             */
            inline uint8 *operator*() noexcept
            {
                return (_buf);
            }

            /**
             * Returns a raw pointer to the beginning of this buffer
             * @return immutable pointer to the buffer start 
             */
            inline const uint8 *operator*() const noexcept
            {
                return (_buf);
            }

            /**
             * Returns the maximum size of this buffer
             * @return size in bytes as unsigned
             */
            inline fsize Size() const noexcept
            {
                return (_size);
            }

            /**
             * Returns the location of the read/write head
             * @return cursor location as unsigned
             */
            inline fsize GetCursor() const noexcept
            {
                return (_cursor);
            }

            /**
             * Returns the number of bytes written
             * @return number of bytes written as unsigned
             */
            inline fsize GetWrittenBytes() const noexcept
            {
                return (_written);
            }

            /**
             * Change location of read/write head
             * @param pos new cursor position as unsigned
             * @throw IndexException if pos out of bounds
             */
            inline void Seek(const fsize pos)
            {
                if (pos >= _size)
                    throw IndexException((fint)pos);
                _cursor = pos;
            }

            /**
             * Returns a byte at a given index
             * @param id the index of the byte to get
             * @throw IndexException if index out of bounds
             * @return copy of the byte at index id
             */
            inline uint8 operator[](const fsize id) const
            {
                if (id >= _size)
                    throw IndexException((fint)id);
                return (_buf[id]);
            }

            inline uint8 &operator[](const fsize id)
            {
                if (id >= _size)
                    throw IndexException((fint)id);
                return (_buf[id]);
            }
        };
    }
}
