#include "framework/framework.h"

using namespace Framework;

#ifdef BUILD_DEBUG
int FMemory::Allocs = 0;
size_t FMemory::CurUsedMem = 0;
std::mutex FMemory::MemMutex;

struct Metadata
{
    size_t MemSize;
};
#endif

void *FMemory::Malloc(size_t size)
{
#ifdef BUILD_DEBUG
    void *data = malloc(size + sizeof(Metadata));
#else
    void *data = malloc(size);
#endif

    if (data == Null)
        throw FMemoryException();
#ifdef BUILD_DEBUG
    MemMutex.lock();
    ++Allocs;
    CurUsedMem += size;
    MemMutex.unlock();
    Metadata *meta = static_cast<Metadata *>(data);
    meta->MemSize = size;
    return (static_cast<char *>(data) + sizeof(Metadata));
#else
    return (data);
#endif
}

void FMemory::Free(void *addr)
{
#ifdef BUILD_DEBUG
    if (addr == Null)
        return;
    MemMutex.lock();
    --Allocs;
    CurUsedMem -= reinterpret_cast<Metadata *>(static_cast<char *>(addr) - sizeof(Metadata))->MemSize;
    MemMutex.unlock();
    free(static_cast<char *>(addr) - sizeof(Metadata));
#else
    free(addr);
#endif
}

void *FMemory::Realloc(void *addr, size_t newsize)
{
#ifdef BUILD_DEBUG
    void *data;
    if (addr != Null)
    {
        MemMutex.lock();
        CurUsedMem -= reinterpret_cast<Metadata *>(static_cast<char *>(addr) - sizeof(Metadata))->MemSize;
        MemMutex.unlock();
        data = realloc(static_cast<char *>(addr) - sizeof(Metadata), newsize + sizeof(Metadata));
    }
    else
    {
        ++Allocs;
        data = realloc(addr, newsize + sizeof(Metadata));
    }
    if (data == Null)
        throw FMemoryException();
    Metadata *meta = static_cast<Metadata *>(data);
    meta->MemSize = newsize;
    MemMutex.lock();
    CurUsedMem += newsize;
    MemMutex.unlock();
    return (static_cast<char *>(data) + sizeof(Metadata));
#else
    void *data = realloc(addr, newsize);
    if (data == Null)
        throw FMemoryException();
    return (data);
#endif
}

