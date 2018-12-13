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

#include "Framework/Framework.hpp"

using namespace bpf;

#ifdef BUILD_DEBUG
int Memory::Allocs = 0;
size_t Memory::CurUsedMem = 0;
Mutex Memory::MemMutex;

struct Metadata
{
    size_t MemSize;
};
#endif

void *Memory::Malloc(size_t size)
{
#ifdef BUILD_DEBUG
    void *data = malloc(size + sizeof(Metadata));
#else
    void *data = malloc(size);
#endif

    if (data == Null)
        throw MemoryException();
#ifdef BUILD_DEBUG
    MemMutex.Lock();
    ++Allocs;
    CurUsedMem += size;
    MemMutex.Unlock();
    Metadata *meta = static_cast<Metadata *>(data);
    meta->MemSize = size;
    return (static_cast<char *>(data) + sizeof(Metadata));
#else
    return (data);
#endif
}

void Memory::Free(void *addr)
{
#ifdef BUILD_DEBUG
    if (addr == Null)
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

void *Memory::Realloc(void *addr, size_t newsize)
{
#ifdef BUILD_DEBUG
    void *data;
    if (addr != Null)
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
    if (data == Null)
        throw MemoryException();
    Metadata *meta = static_cast<Metadata *>(data);
    meta->MemSize = newsize;
    MemMutex.Lock();
    CurUsedMem += newsize;
    MemMutex.Unlock();
    return (static_cast<char *>(data) + sizeof(Metadata));
#else
    void *data = realloc(addr, newsize);
    if (data == Null)
        throw MemoryException();
    return (data);
#endif
}

