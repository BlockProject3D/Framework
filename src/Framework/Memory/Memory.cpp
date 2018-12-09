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
        throw FMemoryException();
    return (data);
#endif
}

