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
#include "Framework/String.hpp"

namespace bpf
{
    namespace system
    {
        /**
         * Utility class to represent a cross platform thread
         */
        class BPF_API Thread
        {
        public:
            /**
             * Enumeration for thread state
             */
            enum EState
            {
                /**
                 * The thread is pending start
                 */
                PENDING,

                /**
                 * The thread is running
                 */
                RUNNING,

                /**
                 * The thread is marked for shut down
                 */
                EXITING,

                /**
                 * The thread has terminated unexpectedly
                 */
                STOPPED,

                /**
                 * The thread has finished
                 */
                FINISHED
            };

        private:
            EState _state;
            String _name;
            void *_handle;
            bool _special;

        public:
            /**
             * Constructs a Thread
             * @param name the thread name
             */
            Thread(const String &name);

            virtual ~Thread();

            /**
             * Move constructor
             */
            Thread(Thread &&other) noexcept;

            /**
             * Move assignment operator
             */
            Thread &operator=(Thread &&other) noexcept;

            /**
             * Starts the thread
             */
            void Start();

            /**
             * Kills the thread
             * @param force if true a hard kill will be performed and memory may leak, otherwise the thread is marked
             * for shut down
             */
            void Kill(bool force = false);

            /**
             * Joins the thread with the current thread
             */
            void Join() noexcept;

            /**
             * Returns the state
             * @return copy of the current thread state
             */
            inline EState GetState() const noexcept
            {
                return (_state);
            }

            /**
             * Checks whether this thread is still running
             * @return true if this thread is running, false otherwise
             */
            inline bool IsRunning() const noexcept
            {
                return (_state == RUNNING);
            }

            /**
             * Returns the thread name
             * @return high-level string
             */
            inline const String &GetName() const noexcept
            {
                return (_name);
            }

            /**
             * The actual threaded function
             */
            virtual void Run() = 0;

            /**
             * Yields the current thread for a certain amount of time
             * @param milliseconds number of milliseconds to wait
             */
            static void Sleep(uint32 milliseconds);

            friend void _bpf_internal_state(Thread &ptr, EState state);
            friend bool _bpf_internal_special(Thread &ptr);
        };
    }
}
