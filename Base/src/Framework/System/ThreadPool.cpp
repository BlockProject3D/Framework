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
#include "Framework/System/OSException.hpp"

using namespace bpf::system;
using namespace bpf::memory;
using namespace bpf;

class ThreadRuntime final : public Thread
{
private:
    ThreadPool *_pool;

public:
    explicit ThreadRuntime(const String &name, ThreadPool *pool)
        : Thread(name)
        , _pool(pool)
    {
    }

    void ReLink(ThreadPool *newpool)
    {
        _pool = newpool;
    }

    void Run() final
    {
        while (_pool->_sharedInputQueue.Size() > 0)
        {
            ThreadPool::Task task;
            {
                auto lock = ScopeLock(_pool->_inputMutex);
                task = _pool->_sharedInputQueue.Pop();
            }
            if (task.Processing1)
                task.Processing1();
            else
            {
                task.Output = task.Processing();
                {
                    auto lock = ScopeLock(_pool->_outputMutex);
                    _pool->_sharedOutputQueue.Push(std::move(task));
                }
            }
        }
    }
};

ThreadPool::ThreadPool(const fsize tcount, const String &name)
    : _tasks(0)
    , _tcount(tcount)
{
    if (name.IsEmpty())
    {
        auto rname = String("Pool_") + String::ValueOf(this);
        _threads = MemUtils::NewArray<ThreadRuntime>(_tcount, rname, this);
    }
    else
        _threads = MemUtils::NewArray<ThreadRuntime>(_tcount, name, this);
}

ThreadPool::ThreadPool(ThreadPool &&other) noexcept
    : _tasks(other._tasks)
    , _tcount(other._tcount)
    , _inputMutex(std::move(other._inputMutex))
    , _sharedInputQueue(std::move(other._sharedInputQueue))
    , _outputMutex(std::move(other._outputMutex))
    , _sharedOutputQueue(std::move(other._sharedOutputQueue))
    , _threads(other._threads)
{
    other._threads = nullptr;
    other._tcount = 0;
    other._tasks = 0;
    for (fsize i = 0; i != _tcount; ++i)
    {
        _threads[i].Join();
        _threads[i].ReLink(this);
    }
}

ThreadPool::~ThreadPool()
{
    for (fsize i = 0; i != _tcount; ++i)
        _threads[i].Join();
    MemUtils::DeleteArray(_threads, _tcount);
}

ThreadPool &ThreadPool::operator=(ThreadPool &&other) noexcept
{
    MemUtils::DeleteArray(_threads, _tcount);
    _tasks = other._tasks;
    _tcount = other._tcount;
    _inputMutex = std::move(other._inputMutex);
    _sharedInputQueue = std::move(other._sharedInputQueue);
    _outputMutex = std::move(other._outputMutex);
    _sharedOutputQueue = std::move(other._sharedOutputQueue);
    _threads = other._threads;
    other._threads = nullptr;
    other._tcount = 0;
    other._tasks = 0;
    for (fsize i = 0; i != _tcount; ++i)
    {
        _threads[i].Join();
        _threads[i].ReLink(this);
    }
    return (*this);
}

void ThreadPool::Run(std::function<Dynamic()> processing, std::function<void(Dynamic &)> callback)
{
    if (!processing || !callback)
        throw OSException("Invalid argument in call to Run");
    ++_tasks;
    Task t;
    t.Callback = std::move(callback);
    t.Processing = std::move(processing);
    {
        auto lock = ScopeLock(_inputMutex);
        _sharedInputQueue.Push(std::move(t));
    }
    for (fsize i = 0; i != _tcount; ++i)
    {
        if (!_threads[i].IsRunning())
            _threads[i].Start();
    }
}

void ThreadPool::Run(std::function<void()> processing)
{
    if (!processing)
        throw OSException("Invalid argument in call to Run");
    Task t;
    t.Processing1 = std::move(processing);
    {
        auto lock = ScopeLock(_inputMutex);
        _sharedInputQueue.Push(std::move(t));
    }
    for (fsize i = 0; i != _tcount; ++i)
    {
        if (!_threads[i].IsRunning())
            _threads[i].Start();
    }
}

void ThreadPool::Poll()
{
    if (IsIdle())
        return;
    while (_sharedOutputQueue.Size() > 0)
    {
        Task task;
        {
            auto lock = ScopeLock(_outputMutex);
            task = _sharedOutputQueue.Pop();
        }
        task.Callback(task.Output);
        --_tasks;
    }
}
