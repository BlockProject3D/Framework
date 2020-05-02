// Copyright (c) 2020, BlockProject
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

#include "Framework/Compression/ZInflater.hpp"
#include <Framework/IO/IOException.hpp>
#include <Framework/Memory/MemoryException.hpp>
#include <Framework/Memory/Memory.hpp>
#include <zlib.h>

using namespace bpf::compression;
using namespace bpf::memory;
using namespace bpf::io;
using namespace bpf;

ZInflater::ZInflater()
    : _handle(Memory::Malloc(sizeof(z_stream_s)))
    , _input(0)
{
    z_stream_s *stream = reinterpret_cast<z_stream_s *>(_handle);
    stream->zalloc = Z_NULL;
    stream->zfree = Z_NULL;
    stream->opaque = Z_NULL;
    stream->avail_in = 0;
    stream->next_in = Z_NULL;
    auto ret = inflateInit(stream);
    if (ret != Z_OK)
    {
        Memory::Free(stream);
        throw IOException(String("Could not initialize zlib: ") + String::ValueOf(ret));
    }
}

ZInflater::~ZInflater()
{
    z_stream_s *stream = reinterpret_cast<z_stream_s *>(_handle);
    inflateEnd(stream);
    Memory::Free(stream);
}

void ZInflater::SetInput(const io::ByteBuf &deflated)
{
    _input = deflated;
    z_stream_s *stream = reinterpret_cast<z_stream_s *>(_handle);
    stream->avail_in = (uInt)_input.Size();
    stream->next_in = *_input;
    stream->total_in = 0;
    stream->total_out = 0;
}

void ZInflater::SetInput(io::ByteBuf &&deflated)
{
    _input = std::move(deflated);
    z_stream_s *stream = reinterpret_cast<z_stream_s *>(_handle);
    stream->avail_in = (uInt)_input.Size();
    stream->next_in = *_input;
    stream->total_in = 0;
    stream->total_out = 0;
}

fsize ZInflater::Inflate(io::ByteBuf &out)
{
    return (Inflate(*out, out.Size()));
}

fsize ZInflater::Inflate(void *out, const fsize size)
{
    z_stream_s *stream = reinterpret_cast<z_stream_s *>(_handle);
    stream->avail_out = (uInt)size;
    stream->next_out = reinterpret_cast<Bytef *>(out);
    int func = Z_NO_FLUSH;
    if (stream->total_in >= _input.Size())
        func = Z_FINISH;
    auto ret = inflate(stream, func);
    switch (ret)
    {
    case Z_NEED_DICT:
        ret = Z_DATA_ERROR; /* and fall through */
    case Z_DATA_ERROR:
        throw IOException("Inflate failed: Z_DATA_ERROR");
    case Z_MEM_ERROR:
        throw MemoryException();
    }
    return (size - stream->avail_out);
}
