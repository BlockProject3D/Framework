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

#include "Framework/System/Thread.hpp"

#include <stdlib.h>
#ifdef WINDOWS
#include <Windows.h>
#else
#include <pthread.h>
using ThreadType = pthread_t;
#endif

#ifdef WINDOWS
DWORD WINAPI ThreadRoutine(void *ptr)
{
    reinterpret_cast<bpf::system::Thread *>(ptr)->Run();
    return (0);
}
#else
void *ThreadRoutine(void *ptr)
{
    reinterpret_cast<bpf::system::Thread *>(ptr)->Run();
    return (Null);
}
#endif

using namespace bpf::system;
using namespace bpf;

Thread::Thread(const String &name)
    : _handle(Null)
    , _exit(false)
    , _name(name)
{
}

Thread::~Thread()
{
#ifndef WINDOWS
    free(_handle);
#endif
}

void Thread::Start()
{
    if (_handle != Null)
        return;
#ifdef WINDOWS
    _handle = CreateThread(Null, 0, &ThreadRoutine, this, 0, Null);
#else
    _handle = malloc(sizeof(ThreadType));
    pthread_create(reinterpret_cast<ThreadType *>(_handle), Null,
                   &ThreadRoutine, this);
#endif
}

void Thread::Join()
{
    if (_handle == Null)
        return;
#ifdef WINDOWS
    WaitForSingleObject(_handle, INFINITE);
#else
    pthread_join(*reinterpret_cast<ThreadType *>(_handle), Null);
#endif
}

void Thread::Kill(const bool force)
{
    if (_handle == Null)
        return;
    if (!force)
    {
        _exit = true;
        Join();
    }
    else
    {
#ifdef WINDOWS
        TerminateThread(_handle, 0);
#else
        pthread_cancel(*reinterpret_cast<ThreadType *>(_handle));
#endif
    }
}
