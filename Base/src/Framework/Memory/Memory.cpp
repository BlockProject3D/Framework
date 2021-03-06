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

#include <cstdlib>
#include "Framework/Memory/Memory.hpp"

using namespace bpf::memory;
using namespace bpf;

#ifdef BUILD_DEBUG
fsize Memory::Allocs = 0;
fsize Memory::CurUsedMem = 0;
system::Mutex Memory::MemMutex;

struct Metadata
{
    fsize MemSize;
};
#endif

void *Memory::Malloc(fsize size)
{
    if (size == 0)
        return (nullptr);
#ifdef BUILD_DEBUG
    void *data = malloc(size + sizeof(Metadata));
#else
    void *data = malloc(size);
#endif

    if (data == nullptr)
        throw MemoryException();
#ifdef BUILD_DEBUG
    MemMutex.Lock();
    ++Allocs;
    CurUsedMem += size;
    MemMutex.Unlock();
    auto *meta = static_cast<Metadata *>(data);
    meta->MemSize = size;
    return (static_cast<char *>(data) + sizeof(Metadata));
#else
    return (data);
#endif
}

void Memory::Free(void *addr) noexcept
{
#ifdef BUILD_DEBUG
    if (addr == nullptr)
        return;
    MemMutex.Lock();
    --Allocs;
    CurUsedMem -= reinterpret_cast<Metadata *>(static_cast<char *>(addr) - sizeof(Metadata))->MemSize;
    MemMutex.Unlock();
    free(static_cast<char *>(addr) - sizeof(Metadata));
#else
    free(addr);
#endif
}

void *Memory::Realloc(void *addr, fsize newsize)
{
#ifdef BUILD_DEBUG
    void *data;
    if (addr != nullptr)
    {
        MemMutex.Lock();
        CurUsedMem -= reinterpret_cast<Metadata *>(static_cast<char *>(addr) - sizeof(Metadata))->MemSize;
        MemMutex.Unlock();
        data = realloc(static_cast<char *>(addr) - sizeof(Metadata), newsize + sizeof(Metadata));
    }
    else
    {
        ++Allocs;
        data = realloc(addr, newsize + sizeof(Metadata));
    }
    if (data == nullptr)
        throw MemoryException();
    auto *meta = static_cast<Metadata *>(data);
    meta->MemSize = newsize;
    MemMutex.Lock();
    CurUsedMem += newsize;
    MemMutex.Unlock();
    return (static_cast<char *>(data) + sizeof(Metadata));
#else
    void *data = realloc(addr, newsize);
    if (data == nullptr)
        throw MemoryException();
    return (data);
#endif
}

