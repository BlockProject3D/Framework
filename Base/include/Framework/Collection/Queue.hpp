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
#include "Framework/Collection/Array.hpp"

namespace bpf
{
    namespace collection
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
            /**
             * Constructs an empty Queue
             * @param maxsize maximum size of queue (0 = infinity/unlimited)
             */
            explicit Queue(fsize maxsize = 0);

            /**
             * Constructs a Queue from an existing initializer list
             * @param lst the initial list of items to push to this new queue
             */
            Queue(const std::initializer_list<T> &lst);

            /**
             * Copy constructor
             */
            inline Queue(const Queue<T> &other)
                : _maxSize(other._maxSize)
                , _headPtr(other._headPtr)
                , _tailPtr(other._tailPtr)
                , _count(other._count)
                , _data(other._data)
            {
            }

            /**
             * Move constructor
             */
            inline Queue(Queue<T> &&other) noexcept
                : _maxSize(other._maxSize)
                , _headPtr(other._headPtr)
                , _tailPtr(other._tailPtr)
                , _count(other._count)
                , _data(std::move(other._data))
            {
                other._headPtr = 0;
                other._tailPtr = 0;
                other._count = 0;
            }

            /**
             * Copy assignment operator
             */
            inline Queue<T> &operator=(const Queue<T> &other)
            {
                if (this == &other)
                    return (*this);
                _maxSize = other._maxSize;
                _headPtr = other._headPtr;
                _tailPtr = other._tailPtr;
                _count = other._count;
                _data = other._data;
                return (*this);
            }

            /**
             * Move assignment operator
             */
            inline Queue<T> &operator=(Queue<T> &&other) noexcept
            {
                _maxSize = other._maxSize;
                _headPtr = other._headPtr;
                _tailPtr = other._tailPtr;
                _count = other._count;
                _data = std::move(other._data);
                other._headPtr = 0;
                other._tailPtr = 0;
                other._count = 0;
                return (*this);
            }

            /**
             * Clears this queue
             */
            void Clear();

            /**
             * Pushes an element on the queue
             * @param element the element to push
             */
            void Push(const T &element);

            /**
             * Pushes an element on the queue
             * @param element the element to push
             */
            void Push(T &&element);

            /**
             * Extracts the top of the queue
             * @throw IndexException if the queue is empty
             * @return the extracted/removed item
             */
            T Pop();

            /**
             * Returns the top of the queue
             * @throw IndexException if the queue is empty
             * @return mutable item
             */
            inline T &Top()
            {
                if (Size() <= 0)
                    throw IndexException(0);
                return (_data[_headPtr]);
            }

            /**
             * Returns the top of the queue
             * @throw IndexException if the queue is empty
             * @return immutable item
             */
            inline const T &Top() const
            {
                if (Size() <= 0)
                    throw IndexException(0);
                return (_data[_headPtr]);
            }

            /**
             * Returns the number of items in this queue
             * @return number of items as unsigned
             */
            inline fsize Size() const
            {
                return (_count);
            }
        };
    }
}

#include "Framework/Collection/Queue.impl.hpp"
