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
#include "Framework/Types.hpp"
#include "Framework/Exception.hpp"

namespace bpf
{
    namespace collection
    {
        /**
         * Exception thrown when stack size is exceeded
         */
        class BPF_API StackOverflowException final : public Exception
        {
        private:
            fisize _size;

        public:
            /**
             * Constructs a StackOverflowException
             * @param size the size that was exceeded 
             */
            explicit inline StackOverflowException(const fisize size) noexcept
                : Exception()
                , _size(size)
            {
            }

            /**
             * Returns the maximum size of that stack that was exceeded
             * @return fisize maximum size of the stack as unsigned
             */
            fisize Size() const noexcept
            {
                return (_size);
            }

            const char *Type() const noexcept
            {
                return ("StackOverflow");
            }
        };

        /**
         * Exception thrown when trying to Pop an empty stack
         */
        class BPF_API StackUnderflowException final : public Exception
        {
        public:
            explicit inline StackUnderflowException() noexcept
                : Exception()
            {
            }

            const char *Type() const noexcept
            {
                return ("StackUnderflow");
            }
        };
    }
}
