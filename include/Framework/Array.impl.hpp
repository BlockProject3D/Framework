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
    template <typename T, fsize I>
    String Array<T, I>::ToString() const
    {
        String res = "[";

        for (fsize i = 0 ; i != I ; ++i)
        {
            res += String::ValueOf(_arr[i]);
            if (i < I - 1)
                res += ", ";
        }
        res += "]";
        return (res);
    }

    template <typename T>
    inline Array<T>::Array()
        : _size(0)
        , _arr(Null)
    {
    }

    template <typename T>
    inline Array<T>::Array(Array<T> &&arr)
        : _size(arr._size)
        , _arr(arr._arr)
    {
        arr._arr = Null;
        arr._size = 0;
    }

    template <typename T>
    Array<T>::Array(const uint32 size)
        : _size(size)
        , _arr(Null)
    {
        if (size > 0)
        {
            _arr = new T[_size];
            for (uint32 i = 0 ; i < _size ; ++i)
                _arr[i] = DefaultOf<T>();
        }
    }

    template <typename T>
    Array<T> &Array<T>::operator=(Array<T> &&arr)
    {
        _size = arr._size;
        _arr = arr._arr;
        arr._arr = Null;
        arr._size = 0;
        return (*this);
    }

    template <typename T>
    inline Array<T>::~Array()
    {
        delete[] _arr;
    }

    template <typename T>
    String Array<T>::ToString() const
    {
        String res = "[";

        for (uint32 i = 0 ; i < _size ; ++i)
        {
            res += String::ValueOf(_arr[i]);
            if (i < _size - 1)
                res += ", ";
        }
        res += "]";
        return (res);
    }

    template <typename T>
    inline T &Array<T>::operator[](const uint32 id) const
    {
        if (id >= _size)
            throw IndexException(static_cast<int>(id));
        return (_arr[id]);
    }

    template <typename T>
    T &Array<T>::operator[](const uint32 id)
    {
        if (id >= _size)
        {
            T *tmp = _arr;
            _arr = new T[id + 1];
            for (uint32 i = 0 ; i < _size ; ++i)
                _arr[i] = tmp[i];
            for (uint32 i = _size ; i < id + 1 ; ++i)
                _arr[i] = DefaultOf<T>();
            _size = id + 1;
            delete[] tmp;
        }
        return (_arr[id]);
    }

    template <typename T>
    uint32 Array<T>::Length() const
    {
        return (_size);
    }
}
