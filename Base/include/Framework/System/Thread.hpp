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
#include "Framework/System/IThreadRunnable.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace system
    {
        class BPF_API Thread
        {
        public:
            enum EState
            {
                PENDING,
                RUNNING,
                EXITING,
                STOPPED,
                FINISHED
            };

        private:
            EState _state;
            IThreadRunnable &_runnable;
            String _name;
            void *_handle;

        public:
            Thread(const String &name, IThreadRunnable &runnable);
            ~Thread();

            /**
             * Construct a thread by move semantics
             * Never move a running thread
             */
            Thread(Thread &&other) noexcept;

            /**
             * Assigns this thread by move semantics
             * Never move a running thread
             */
            Thread &operator=(Thread &&other) noexcept;

            void Start();
            void Kill(bool force = false);
            void Join();

            inline EState GetState() const noexcept
            {
                return (_state);
            }

            inline bool IsRunning() const noexcept
            {
                return (_state == RUNNING);
            }

            inline const String &GetName() const noexcept
            {
                return (_name);
            }

            void Run();

            static void Sleep(uint32 milliseconds);

            friend void _bpf_internal_state(Thread &ptr, EState state);
        };
    }
}
