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
#include "Framework/Graphics/EPixelFormat.hpp"
#include "Framework/IO/ByteBuf.hpp"
#include "Framework/Math/Color.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace graphics
    {
        class PixelArray
        {
        private:
            io::ByteBuf _buffer;
            const fsize _width;
            const fsize _height;
            const uint8 _bspp;
            const EPixelFormat _pixelFormat;

        public:
            PixelArray(const fsize w, const fsize h, const uint8 bpp, const EPixelFormat pformat);

            math::Color GetPixel(fsize x, fsize y);

            void SetPixel(fsize x, fsize y, const math::Color &color);

            PixelArray Convert(EPixelFormat target) const;

            inline uint8 &operator()(fsize x, fsize y, uint8 c = 0)
            {
                return (_buffer[y * _width * _bspp + (x + c)]);
            }

            inline uint8 operator()(fsize x, fsize y, uint8 c = 0) const
            {
                return (_buffer[y * _width * _bspp + (x + c)]);
            }

            inline const uint8 *operator*() const
            {
                return (*_buffer);
            }

            inline uint8 *operator*()
            {
                return (*_buffer);
            }
        };
    }
}
