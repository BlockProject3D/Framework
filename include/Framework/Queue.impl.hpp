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
    template <typename T>
    inline Queue<T>::Queue(const fsize maxsize)
        : _maxSize(maxsize)
        , _headPtr(0)
        , _tailPtr(0)
        , _count(0)
        , _data(maxsize > 0 ? maxsize : 8)
    {
    }

    template <typename T>
    Queue<T>::Queue(const std::initializer_list<T>& lst)
        : _maxSize(0)
        , _headPtr(0)
        , _tailPtr(0)
        , _count(0)
        , _data(8)
    {
        for (auto& elem : lst)
            Push(elem);
    }

    template <typename T>
    void Queue<T>::Push(const T &element)
    {
        if (_maxSize == 0)
        {
            if (_count == 0)
            {
                _headPtr = 0;
                _tailPtr = 0;
            }
            if (_tailPtr + 1 >= _data.Size())
                _data.Resize(_data.Size() * 2);
            _data[_tailPtr++] = element;
            ++_count;
        }
        else
        {
            if (_tailPtr >= _maxSize)
            {
                _tailPtr = 0;
                --_count;
            }
            _data[_tailPtr++] = element;
            ++_count;
        }
    }

	template <typename T>
	void Queue<T>::Push(T &&element)
	{
        if (_maxSize == 0)
        {
            if (_count == 0)
            {
                _headPtr = 0;
                _tailPtr = 0;
            }
            if (_tailPtr + 1 >= _data.Size())
                _data.Resize(_data.Size() * 2);
            _data[_tailPtr++] = std::move(element);
            ++_count;
        }
        else
        {
            if (_tailPtr >= _maxSize)
            {
                _tailPtr = 0;
                _count = 0;
            }
            _data[_tailPtr++] = std::move(element);
            ++_count;
        }
    }

    template <typename T>
    T Queue<T>::Pop()
    {
        if (_count == 0)
            throw IndexException(0);
        auto elem = std::move(_data[_headPtr++]);
        --_count;
        if (_maxSize > 0 && _headPtr >= _maxSize)
            _headPtr = 0;
        return (std::move(elem));
    }
}
