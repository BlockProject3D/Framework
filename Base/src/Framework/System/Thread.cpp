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

#include "Framework/System/Thread.hpp"
#include "Framework/Exception.hpp"
#include "Framework/System/OSException.hpp"
#include "Framework/Memory/Memory.hpp"
#include <iostream>

#include <cstdlib>
#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <pthread.h>
    #include <ctime>
using ThreadType = pthread_t;
#endif

//Attempt at re-implementing what Google is apparently unable to implement
#ifdef ANDROID
int pthread_cancel(pthread_t h)
{
    return pthread_kill(h, 0);
}
#endif

using namespace bpf::system;
using namespace bpf;

namespace bpf
{
    namespace system
    {
        void _bpf_internal_state(Thread &ptr, Thread::EState state)
        {
            ptr._state = state;
        }
    }
}

#ifdef WINDOWS
DWORD WINAPI ThreadRoutine(void *ptr)
{
    auto thread = reinterpret_cast<bpf::system::Thread *>(ptr);
    try
    {
        thread->Run();
        _bpf_internal_state(*thread, Thread::FINISHED);
    }
    catch (const bpf::Exception &)
    {
        //TODO: print ex
        _bpf_internal_state(*thread, Thread::STOPPED);
    }
    return (0);
}
#else
void *ThreadRoutine(void *ptr)
{
    auto thread = reinterpret_cast<bpf::system::Thread *>(ptr);
    try
    {
        thread->Run();
        _bpf_internal_state(*thread, Thread::FINISHED);
    }
    catch (const bpf::Exception &)
    {
        //TODO: print ex
        _bpf_internal_state(*thread, Thread::STOPPED);
    }
    return (Null);
}
#endif

Thread::Thread(const String &name, IThreadRunnable &runnable)
    : _state(PENDING)
    , _runnable(runnable)
    , _name(name)
    , _handle(Null)
{
}

Thread::Thread(Thread &&other) noexcept
    : _state(other._state)
    , _runnable(other._runnable)
    , _name(std::move(other._name))
    , _handle(other._handle)
{
    other._handle = Null;
}

Thread::~Thread()
{
    Join();
#ifndef WINDOWS
    free(_handle);
#endif
}

Thread &Thread::operator=(Thread &&other) noexcept
{
    Join();
#ifndef WINDOWS
    free(_handle);
#endif
    _state = other._state;
    _handle = other._handle;
    _name = std::move(other._name);
    other._handle = Null;
    return (*this);
}

void Thread::Start()
{
    if (_handle != Null)
        return;
    _bpf_internal_state(*this, RUNNING);
#ifdef WINDOWS
    _handle = CreateThread(Null, 0, &ThreadRoutine, this, 0, Null);
    if (_handle == Null)
        throw OSException("Failed to create thread");
#else
    _handle = malloc(sizeof(ThreadType));
    if (_handle == Null)
        throw memory::MemoryException();
    if (pthread_create(reinterpret_cast<ThreadType *>(_handle), Null,
                   &ThreadRoutine, this) != 0)
        throw OSException("Failed to create thread");
#endif
}

void Thread::Join() noexcept
{
    if (_handle == Null)
        return;
#ifdef WINDOWS
    WaitForSingleObject(_handle, INFINITE);
#else
    pthread_join(*reinterpret_cast<ThreadType *>(_handle), Null);
    free(_handle);
#endif
    _handle = Null;
}

void Thread::Run()
{
    _runnable.Run();
}

void Thread::Kill(const bool force)
{
    if (_handle == Null)
        return;
    if (!force)
        _bpf_internal_state(*this, EXITING);
    else
    {
#ifdef WINDOWS
        TerminateThread(_handle, 0);
#else
        pthread_cancel(*reinterpret_cast<ThreadType *>(_handle));
#endif
        _bpf_internal_state(*this, STOPPED);
    }
}

void Thread::Sleep(const uint32 milliseconds)
{
#ifdef WINDOWS
    ::Sleep((DWORD)milliseconds);
#else
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, Null);
#endif
}