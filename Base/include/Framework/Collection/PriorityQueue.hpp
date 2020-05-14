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
#include "Framework/Collection/Utility.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace collection
    {
        /**
         * Priority queue implemented using min/max heap
         * @tparam K the key type
         * @tparam V the value type
         * @tparam HeapFunc heap comparision operator usually set to either MaxHeap or MinHeap
         */
        template <typename K, typename V, template <typename> class HeapFunc = MaxHeap>
        class BP_TPL_API PriorityQueue
        {
        public:
            struct Entry
            {
                K Key;
                V Value;
            };

        private:
            fsize _maxSize;
            fsize _tailPtr;
            fsize _count;
            // MSVC absolutely wants to instantiate unused C++ template member functions !
            // Seriously Microsoft, please fix your fucking non standard compliant compiler !!
            Array<K> _dataK;
            Array<V> _dataV;

            void BubbleUp(fsize i);
            void SinkDown(fsize i);

        public:
            /**
             * Constructs an empty PriorityQueue
             * @param maxsize maximum size of queue (0 = infinity/unlimited)
             */
            explicit PriorityQueue(const fsize maxsize = 0);

            /**
             * Constructs a PriorityQueue from an existing initializer list
             * @param lst the initial list of items to push to this new queue
             */
            PriorityQueue(const std::initializer_list<Entry> &lst);

            /**
             * Copy constructor
             */
            inline PriorityQueue(const PriorityQueue<K, V, HeapFunc> &other)
                : _maxSize(other._maxSize)
                , _tailPtr(other._tailPtr)
                , _count(other._count)
                , _dataK(other._dataK)
                , _dataV(other._dataV)
            {
            }

            /**
             * Move constructor
             */
            inline PriorityQueue(PriorityQueue<K, V, HeapFunc> &&other)
                : _maxSize(other._maxSize)
                , _tailPtr(other._tailPtr)
                , _count(other._count)
                , _dataK(std::move(other._dataK))
                , _dataV(std::move(other._dataV))
            {
                other._tailPtr = 0;
                other._count = 0;
            }

            /**
             * Copy assignment operator
             */
            inline PriorityQueue<K, V, HeapFunc> &operator=(const PriorityQueue<K, V, HeapFunc> &other)
            {
                _maxSize = other._maxSize;
                _tailPtr = other._tailPtr;
                _count = other._count;
                _dataK = other._dataK;
                _dataV = other._dataV;
                return (*this);
            }

            /**
             * Move assignment operator
             */
            inline PriorityQueue<K, V, HeapFunc> &operator=(PriorityQueue<K, V, HeapFunc> &&other)
            {
                _maxSize = other._maxSize;
                _tailPtr = other._tailPtr;
                _count = other._count;
                _dataK = std::move(other._dataK);
                _dataV = std::move(other._dataV);
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
             * @param key the key for that element
             * @param value the value for that element
             */
            void Push(const K &key, const V &value);

            /**
             * Pushes an element on the queue
             * @param key the key for that element
             * @param value the value for that element
             */
            void Push(const K &key, V &&value);

            /**
             * Extracts the top of the queue
             * @throw IndexException if the queue is empty
             * @return the extracted/removed item
             */
            V Pop();

            /**
             * Returns the top of the queue
             * @throw IndexException if the queue is empty
             * @return mutable item
             */
            inline V &Top()
            {
                if (Size() <= 0)
                    throw IndexException(0);
                return (_dataV[1]);
            }

            /**
             * Returns the top of the queue
             * @throw IndexException if the queue is empty
             * @return immutable item
             */
            inline const V &Top() const
            {
                if (Size() <= 0)
                    throw IndexException(0);
                return (_dataV[1]);
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
};

#include "Framework/Collection/PriorityQueue.impl.hpp"
