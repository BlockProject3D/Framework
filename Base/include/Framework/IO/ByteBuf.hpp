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
        class BPF_API ByteBuf : public IInputStream, public IOutputStream
        {
        private:
            uint8 *_buf;
            fsize _cursor;
            fsize _size;
            fsize _written;

        public:
            explicit ByteBuf(const fsize size);
            ByteBuf(ByteBuf &&other);
            ByteBuf(const ByteBuf &other);
            ~ByteBuf();

            ByteBuf &operator=(const ByteBuf &other);
            ByteBuf &operator=(ByteBuf &&other);

            void Clear();

            void Reset();

            void Shift(fsize count);

            fsize Write(const void *buf, fsize bufsize);
            fsize Read(void *buf, fsize bufsize);

            inline uint8 *operator*() noexcept
            {
                return (_buf);
            }

            inline const uint8 *operator*() const noexcept
            {
                return (_buf);
            }

            inline fsize Size() const noexcept
            {
                return (_size);
            }

            inline fsize GetCursor() const noexcept
            {
                return (_cursor);
            }

            inline fsize GetWrittenBytes() const noexcept
            {
                return (_written);
            }

            inline void Seek(const fsize pos)
            {
                if (pos >= _size)
                    throw IndexException((fint)pos);
                _cursor = pos;
            }

            inline uint8 operator[](const fsize id) const
            {
                if (id >= _size)
                    throw IndexException((fint)id);
                return (_buf[id]);
            }
        };
    }
}
