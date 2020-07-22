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
#include "Framework/Memory/Memory.hpp"
#include "Framework/System/OSException.hpp"
#include <iostream>

#include <cstdlib>
#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <ctime>
    #include <pthread.h>
using ThreadType = pthread_t;
#endif

// Attempt at re-implementing what Google is apparently unable to implement
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
        bool _bpf_internal_special(Thread &ptr)
        {
            return (ptr._special);
        }
    }
}

#ifdef WINDOWS
DWORD WINAPI ThreadRoutine(void *ptr)
{
    auto thread = reinterpret_cast<bpf::system::Thread *>(ptr);
    try
    {
        //Hack in order to prevent a C++ defect: virtual methods are incorrectly called from destructors
        if (!_bpf_internal_special(*thread))
            thread->Run();
        _bpf_internal_state(*thread, Thread::FINISHED);
    }
    catch (const bpf::Exception &ex)
    {
        ex.Print();
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
        //Hack in order to prevent a C++ defect: virtual methods are incorrectly called from destructors
        if (!_bpf_internal_special(*thread))
            thread->Run();
        _bpf_internal_state(*thread, Thread::FINISHED);
    }
    catch (const bpf::Exception &ex)
    {
        ex.Print();
        _bpf_internal_state(*thread, Thread::STOPPED);
    }
    return (nullptr);
}
#endif

Thread::Thread(const String &name)
    : _state(PENDING)
    , _name(name)
    , _handle(nullptr)
    , _special(false)
{
}

Thread::Thread(Thread &&other) noexcept
{
    other.Join();
    _state = other._state;
    _name = std::move(other._name);
    _handle = other._handle;
    _special = false;
    other._handle = nullptr;
}

Thread::~Thread()
{
    _special = true; //Trigger the special flag that prevents the Thread from calling any virtual function while destructing
    Join(); //Join the thread will essentially cause the C++ virtual function to be broken
}

Thread &Thread::operator=(Thread &&other) noexcept
{
    other.Join();
    Join();
#ifndef WINDOWS
    free(_handle);
#endif
    _state = other._state;
    _handle = other._handle;
    _name = std::move(other._name);
    _special = false;
    other._handle = nullptr;
    return (*this);
}

void Thread::Start()
{
    if (_state == RUNNING)
        return;
    if (_handle != nullptr)
        Join();
    _bpf_internal_state(*this, RUNNING);
#ifdef WINDOWS
    _handle = CreateThread(nullptr, 0, &ThreadRoutine, this, 0, nullptr);
    if (_handle == nullptr)
        throw OSException("Failed to create thread");
#else
    _handle = malloc(sizeof(ThreadType));
    if (_handle == nullptr)
        throw memory::MemoryException();
    if (pthread_create(reinterpret_cast<ThreadType *>(_handle), nullptr, &ThreadRoutine, this) != 0)
        throw OSException("Failed to create thread");
#endif
}

void Thread::Join() noexcept
{
    if (_handle == nullptr)
        return;
#ifdef WINDOWS
    WaitForSingleObject(_handle, INFINITE);
#else
    pthread_join(*reinterpret_cast<ThreadType *>(_handle), nullptr);
    free(_handle);
#endif
    _handle = nullptr;
}

void Thread::Kill(const bool force)
{
    if (_handle == nullptr)
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
    nanosleep(&ts, nullptr);
#endif
}