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

#include "Framework/Graphics/PixelArray.hpp"

using namespace bpf;
using namespace bpf::io;
using namespace bpf::graphics;
using namespace bpf::math;

PixelArray::PixelArray(const fsize w, const fsize h, const uint8 bpp, const EPixelFormat pformat)
    : _buffer(w * h * (bpp / 8))
    , _width(w)
    , _height(h)
    , _bspp(bpp / 8)
    , _pixelFormat(pformat)
{
}

Color PixelArray::GetPixel(fsize x, fsize y)
{
    switch (_pixelFormat)
    {
    case EPixelFormat::RGB_U8:
        return (Color(operator()(x, y, 0), operator()(x, y, 1), operator()(x, y, 2)));
    case EPixelFormat::RGBA_U8:
        return (Color(operator()(x, y, 0), operator()(x, y, 1), operator()(x, y, 2), operator()(x, y, 3)));
    case EPixelFormat::R_U8:
        return (Color(operator()(x, y, 0), operator()(x, y, 0), operator()(x, y, 0)));
    case EPixelFormat::RGB_FLOAT:
        break;
    case EPixelFormat::RGBA_FLOAT:
        break;
    case EPixelFormat::R_FLOAT:
        break;
    case EPixelFormat::BGR_U8:
        break;
    case EPixelFormat::BGRA_U8:
        break;
    case EPixelFormat::BGR_FLOAT:
        break;
    case EPixelFormat::BGRA_FLOAT:
        break;
    }
}

void PixelArray::SetPixel(fsize x, fsize y, const Color &color)
{
}

PixelArray PixelArray::Convert(EPixelFormat target) const
{
}
