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