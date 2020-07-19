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

#include "Framework/System/ThreadPool.hpp"
#include "Framework/System/ScopeLock.hpp"
#include "Framework/Memory/Utility.hpp"

using namespace bpf::system;
using namespace bpf::memory;
using namespace bpf;

class ThreadRuntime final : public IThreadRunnable
{
private:
    ThreadPool &_pool;

public:
    ThreadRuntime(ThreadPool &pool)
        : _pool(pool)
    {
    }

    void Run() final
    {

    }
};

ThreadPool::ThreadPool(const fsize tcount)
    : _tcount(tcount)
    , _threadRuntime(MakeUnique<ThreadRuntime>(*this))
{
    _threads = MemUtils::NewArray<Thread>(_tcount, "ThreadPool", *_threadRuntime);
}

ThreadPool::ThreadPool(ThreadPool &&other) noexcept
    : _tcount(other._tcount)
    , _threadRuntime(MakeUnique<ThreadRuntime>(*this))
    , _inputMutex(std::move(other._inputMutex))
    , _sharedInputQueue(std::move(other._sharedInputQueue))
    , _outputMutex(std::move(other._outputMutex))
    , _sharedOutputQueue(std::move(other._sharedOutputQueue))
    , _threads(other._threads)
{
    other._threads = Null;
    other._tcount = 0;
    other._threadRuntime = Null;
}

ThreadPool::~ThreadPool()
{
    MemUtils::DeleteArray(_threads, _tcount);
}

ThreadPool &ThreadPool::operator=(ThreadPool &&other) noexcept
{
    MemUtils::DeleteArray(_threads, _tcount);
    _tcount = other._tcount;
    _threadRuntime = MakeUnique<ThreadRuntime>(*this);
    _inputMutex = std::move(other._inputMutex);
    _sharedInputQueue = std::move(other._sharedInputQueue);
    _outputMutex = std::move(other._outputMutex);
    _sharedOutputQueue = std::move(other._sharedOutputQueue);
    _threads = other._threads;
    other._threads = Null;
    other._tcount = 0;
    other._threadRuntime = Null;
    return (*this);
}

void ThreadPool::Run(std::function<Dynamic()> processing, std::function<void(Dynamic &)> callback)
{
    Task t;
    t.Callback = std::move(callback);
    t.Processing = std::move(processing);
    {
        auto lock = ScopeLock(_inputMutex);
        _sharedInputQueue.Push(std::move(t));
    }
}

void ThreadPool::Poll()
{

}
