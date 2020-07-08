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

namespace bpf
{
    namespace collection
    {
        template <typename K, typename V, template <typename> class HeapFunc>
        void PriorityQueue<K, V, HeapFunc>::BubbleUp(fsize i)
        {
            fsize parent = i / 2;

            while (i > 1 && HeapFunc<K>::Eval(_dataK[i], _dataK[parent]))
            {
                K key = std::move(_dataK[i]);
                V value = std::move(_dataV[i]);
                _dataK[i] = std::move(_dataK[parent]);
                _dataV[i] = std::move(_dataV[parent]);
                _dataK[parent] = std::move(key);
                _dataV[parent] = std::move(value);
                i = parent;
                parent = i / 2;
            }
        }

        template <typename K, typename V, template <typename> class HeapFunc>
        void PriorityQueue<K, V, HeapFunc>::SinkDown(fsize i)
        {
            fsize left = i * 2;
            fsize right = i * 2 + 1;
            fsize k = i;

            while (left < _tailPtr + 1 && right < _tailPtr + 1 && (HeapFunc<K>::Eval(_dataK[left], _dataK[i]) || HeapFunc<K>::Eval(_dataK[right], _dataK[i])))
            {
                if (HeapFunc<K>::Eval(_dataK[left], _dataK[right]))
                    k = left;
                else if (HeapFunc<K>::Eval(_dataK[right], _dataK[left]))
                    k = right;
                if (k != i)
                {
                    K key = std::move(_dataK[i]);
                    V value = std::move(_dataV[i]);
                    _dataK[i] = std::move(_dataK[k]);
                    _dataV[i] = std::move(_dataV[k]);
                    _dataK[k] = std::move(key);
                    _dataV[k] = std::move(value);
                    i = k;
                    left = i * 2;
                    right = i * 2 + 1;
                }
                else
                    break;
            }
        }

        template <typename K, typename V, template <typename> class HeapFunc>
        inline PriorityQueue<K, V, HeapFunc>::PriorityQueue(const fsize maxsize)
            : _maxSize(maxsize)
            , _tailPtr(0)
            , _count(0)
            , _dataK(maxsize > 0 ? (maxsize + 1) : 8)
            , _dataV(maxsize > 0 ? (maxsize + 1) : 8)
        {
        }

        template <typename K, typename V, template <typename> class HeapFunc>
        PriorityQueue<K, V, HeapFunc>::PriorityQueue(const std::initializer_list<Entry> &lst)
            : _maxSize(0)
            , _tailPtr(0)
            , _count(0)
            , _dataK(8)
            , _dataV(8)
        {
            for (auto &elem : lst)
                Push(elem.Key, elem.Value);
        }

        template <typename K, typename V, template <typename> class HeapFunc>
        void PriorityQueue<K, V, HeapFunc>::Clear()
        {
            _tailPtr = 0;
            _count = 0;
        }

        template <typename K, typename V, template <typename> class HeapFunc>
        void PriorityQueue<K, V, HeapFunc>::Push(const K &key, const V &value)
        {
            if (_maxSize == 0)
            {
                if (_tailPtr + 1 >= _dataK.Size())
                {
                    _dataK.Resize(_dataK.Size() * 2);
                    _dataV.Resize(_dataV.Size() * 2);
                }
            }
            else
            {
                if (_tailPtr + 1 >= _dataK.Size())
                    throw IndexException(_tailPtr);
            }
            _dataK[_tailPtr + 1] = key;
            _dataV[_tailPtr + 1] = value;
            //Bubble up
            BubbleUp(_tailPtr + 1);
            ++_tailPtr;
            ++_count;
        }

        template <typename K, typename V, template <typename> class HeapFunc>
        void PriorityQueue<K, V, HeapFunc>::Push(const K &key, V &&value)
        {
            if (_maxSize == 0)
            {
                if (_tailPtr + 1 >= _dataK.Size())
                {
                    _dataK.Resize(_dataK.Size() * 2);
                    _dataV.Resize(_dataV.Size() * 2);
                }
            }
            else
            {
                if (_tailPtr + 1 >= _dataK.Size())
                    throw IndexException(_tailPtr);
            }
            _dataK[_tailPtr + 1] = key;
            _dataV[_tailPtr + 1] = std::move(value);
            //Bubble up
            BubbleUp(_tailPtr + 1);
            ++_tailPtr;
            ++_count;
        }

        template <typename K, typename V, template <typename> class HeapFunc>
        V PriorityQueue<K, V, HeapFunc>::Pop()
        {
            if (_count == 0)
                throw IndexException(0);
            V v = std::move(_dataV[1]);
            _dataV[1] = std::move(_dataV[_tailPtr]);
            _dataK[1] = std::move(_dataK[_tailPtr]);
            //Sink down
            SinkDown(1);
            --_tailPtr;
            --_count;
#ifdef WINDOWS
            return (std::move(v));
#else
            return (v); //Unix wants to be potentially slower (copy instead of move)
#endif
        }
    }
}
