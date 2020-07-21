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

#pragma once
#include "Framework/Collection/Queue.hpp"
#include "Framework/Dynamic.hpp"
#include "Framework/Memory/UniquePtr.hpp"
#include "Framework/System/Mutex.hpp"
#include "Framework/System/Thread.hpp"
#include <functional>

class ThreadRuntime;

namespace bpf
{
    namespace system
    {
        /**
         * Represents a thread pool
         */
        class BPF_API ThreadPool
        {
        private:
            struct Task
            {
                std::function<Dynamic()> Processing;
                std::function<void(Dynamic &)> Callback;
                Dynamic Output;
            };

            fsize _tcount;
            memory::UniquePtr<IThreadRunnable> _threadRuntime;
            Mutex _inputMutex;
            collection::Queue<Task> _sharedInputQueue;
            Mutex _outputMutex;
            collection::Queue<Task> _sharedOutputQueue;
            Thread *_threads; // Raw pointer cause Thread does not have a default constructor

        public:
            /**
             * Constructs a ThreadPool
             * @param tcount maximum number of threads
             * @param name the name of the ThreadPool
             */
            explicit ThreadPool(fsize tcount = 2, const String &name = "");

            /**
             * Move constructor
             */
            ThreadPool(ThreadPool &&other) noexcept;

            ~ThreadPool();

            /**
             * Move assignment operator
             */
            ThreadPool &operator=(ThreadPool &&other) noexcept;

            /**
             * Runs a task using this ThreadPool
             * @param processing the actual threaded function
             * @param callback function to call on completion on the thread Poll is called
             */
            void Run(std::function<Dynamic()> processing, std::function<void(Dynamic &)> callback);

            /**
             * Checks if this ThreadPool is idle: it has no tasks anymore
             * @return true if this ThreadPool is idle, false otherwise
             */
            inline bool IsIdle() const
            {
                return (_sharedInputQueue.Size() == 0 && _sharedOutputQueue.Size() == 0);
            }

            /**
             * Call this function (usually on the main thread) in order to update the status of each task and run
             * callbacks when needed
             */
            void Poll();

            friend class ::ThreadRuntime;
        };
    }
}