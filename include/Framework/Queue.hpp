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

#pragma once
#include "Framework/Array.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API Queue
    {
    private:
        fsize _maxSize;
        fsize _headPtr;
        fsize _tailPtr;
        fsize _count;
        Array<T> _data;

    public:
        explicit Queue(const fsize maxsize);
        Queue(const std::initializer_list<T> &lst);

		/**
		 * Pushes an element on the queue
		 */
		void Push(const T &element);

        /**
         * Pushes an element on the queue
         */
        void Push(T &&element);

        /**
         * Extracts the top of the queue
         */
        T Pop();

        /**
         * Returns the top of the queue
         */
        inline T &Top() const
        {
            if (Size() <= 0)
                throw IndexException(0);
            return (_data[_ptr]);
        }

        inline fsize Size() const
        {
            return (_count);
        }
    };
};

#include "Framework/Queue.impl.hpp"
