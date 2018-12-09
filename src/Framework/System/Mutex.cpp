#include "Framework/Memory/Memory.hpp"
#include "Framework/System/Mutex.hpp"
#ifdef WINDOWS
    #include <Windows.h>
    using MutexType = CRITICAL_SECTION;
#else
    #include <pthread.h>
    using MutexType = pthread_mutex_t;
#endif

using namespace bpf;

Mutex::Mutex()
    : _handle(malloc(sizeof(MutexType)))
{
    if (_handle == Null)
        throw MemoryException();
#ifdef WINDOWS
    InitializeCriticalSection(reinterpret_cast<MutexType *>(_handle));
#else
    pthread_mutex_init(reinterpret_cast<MutexType *>(_handle), Null);
#endif
}

Mutex::~Mutex()
{
#ifdef WINDOWS
    DeleteCriticalSection(reinterpret_cast<MutexType *>(_handle));
#else
    pthread_mutex_destroy(reinterpret_cast<MutexType *>(_handle));
#endif
    free(_handle);
}

void Mutex::Lock()
{
#ifdef WINDOWS
    EnterCriticalSection(reinterpret_cast<MutexType *>(_handle));
#else
    pthread_mutex_lock(reinterpret_cast<MutexType *>(_handle));
#endif
}

void Mutex::Unlock()
{
#ifdef WINDOWS
    LeaveCriticalSection(reinterpret_cast<MutexType *>(_handle));
#else
    pthread_mutex_unlock(reinterpret_cast<MutexType *>(_handle));
#endif
}
