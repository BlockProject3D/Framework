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
#include "Framework/Types.hpp"
#include "Framework/ContainerUtilities.hpp"

namespace bpf
{
    template <typename K, typename V, template <typename> typename HeapFunc = MaxHeap>
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
        explicit PriorityQueue(const fsize maxsize = 0);
        PriorityQueue(const std::initializer_list<Entry> &lst);

        inline PriorityQueue(const PriorityQueue<K, V, HeapFunc> &other)
            : _maxSize(other._maxSize)
            , _tailPtr(other._tailPtr)
            , _count(other._count)
            , _dataK(other._dataK)
            , _dataV(other._dataV)
        {
        }

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

        inline PriorityQueue<K, V, HeapFunc> &operator=(const PriorityQueue<K, V, HeapFunc> &other)
        {
            _maxSize = other._maxSize;
            _tailPtr = other._tailPtr;
            _count = other._count;
            _dataK = other._dataK;
            _dataV = other._dataV;
            return (*this);
        }

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
		 * Pushes an element on the queue
		 */
		void Push(const K &key, const V &value);

        /**
         * Pushes an element on the queue
         */
        void Push(const K &key, V &&value);

        /**
         * Extracts the top of the queue
         */
        V Pop();

        /**
         * Returns the top of the queue
         */
        inline V &Top()
        {
            if (Size() <= 0)
                throw IndexException(0);
            return (_dataV[1]);
        }

        /**
         * Returns the top of the queue
         */
        inline const V &Top() const
        {
            if (Size() <= 0)
                throw IndexException(0);
            return (_dataV[1]);
        }

        inline fsize Size() const
        {
            return (_count);
        }
    };
};

#include "Framework/PriorityQueue.impl.hpp"
