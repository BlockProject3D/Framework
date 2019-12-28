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

namespace bpf
{
    template <typename K, typename V, template <typename> typename HeapFunc>
    void PriorityQueue<K, V, HeapFunc>::BubbleUp(fsize i)
    {
        fsize parent = i / 2;

        while (i > 1 && HeapFunc<K>::Eval(_data[i].Key, _data[parent].Key))
        {
            Entry tmp = std::move(_data[i]);
            _data[i] = std::move(_data[parent]);
            _data[parent] = std::move(tmp);
            i = parent;
            parent = i / 2;
        }
    }

    template <typename K, typename V, template <typename> typename HeapFunc>
    void PriorityQueue<K, V, HeapFunc>::SinkDown(fsize i)
    {
        fsize left = i * 2;
        fsize right = i * 2 + 1;
        fsize k = i;

        while (left < _tailPtr + 1 && right < _tailPtr + 1
            && (HeapFunc<K>::Eval(_data[left].Key, _data[i].Key) || HeapFunc<K>::Eval(_data[right].Key, _data[i].Key)))
        {
            if (HeapFunc<K>::Eval(_data[left].Key, _data[right].Key))
                k = left;
            else if (HeapFunc<K>::Eval(_data[right].Key, _data[left].Key))
                k = right;
            if (k != i)
            {
                Entry tmp = std::move(_data[i]);
                _data[i] = std::move(_data[k]);
                _data[k] = std::move(tmp);
                i = k;
                left = i * 2;
                right = i * 2 + 1;
            }
            else
                break;
        }
    }

    template <typename K, typename V, template <typename> typename HeapFunc>
    inline PriorityQueue<K, V, HeapFunc>::PriorityQueue(const fsize maxsize)
        : _maxSize(maxsize)
        , _tailPtr(1)
        , _count(0)
        , _data(maxsize > 0 ? (maxsize + 1) : 8)
    {
    }

    template <typename K, typename V, template <typename> typename HeapFunc>
    PriorityQueue<K, V, HeapFunc>::PriorityQueue(const std::initializer_list<Entry>& lst)
        : _maxSize(0)
        , _tailPtr(1)
        , _count(0)
        , _data(8)
    {
        for (auto& elem : lst)
            Push(elem.Key, elem.Value);
    }

    template <typename K, typename V, template <typename> typename HeapFunc>
    void PriorityQueue<K, V, HeapFunc>::Push(const K &key, const V &value)
    {
        if (_maxSize == 0)
        {
            if (_tailPtr + 1 >= _data.Size())
                _data.Resize(_data.Size() * 2);
        }
        else
        {
            if (_tailPtr + 1 >= _data.Size())
                --_tailPtr;
        }
        Entry newVal;
        newVal.Priority = priority;
        newVal.Value = element;
        _data[_tailPtr] = newVal;
        //Bubble up
        ++_tailPtr;
        ++_count;
    }

    template <typename K, typename V, template <typename> typename HeapFunc>
	void PriorityQueue<K, V, HeapFunc>::Push(const K &key, V &&value)
	{
        if (_maxSize == 0)
        {
            if (_tailPtr + 1 >= _data.Size())
                _data.Resize(_data.Size() * 2);
        }
        else
        {
            if (_tailPtr + 1 >= _data.Size())
                --_tailPtr;
        }
        Entry newVal;
        newVal.Priority = priority;
        newVal.Value = std::move(element);
        _data[_tailPtr] = newVal;
        //Bubble up
        ++_tailPtr;
        ++_count;
    }

    template <typename K, typename V, template <typename> typename HeapFunc>
    V PriorityQueue<K, V, HeapFunc>::Pop()
    {
        T v = std::move(_data[1]);
        _data[1] = std::move(_data[_tailPtr]);
        //Sink down
        --_tailPtr;
        return (std::move(v));
    }
}
