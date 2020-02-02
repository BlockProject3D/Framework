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
#include <utility>

namespace bpf
{
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
    Array<T>::Array(const Array<T> &arr)
        : _size(arr._size)
        , _arr(Null)
    {
        if (_size > 0)
        {
            _arr = new T[_size];
            for (fsize i = 0; i < _size; ++i)
                _arr[i] = arr[i];
        }
    }

    template <typename T>
    Array<T>::Array(const fsize size)
        : _size(size)
        , _arr(Null)
    {
        if (size > 0)
        {
            _arr = new T[_size];
            for (fsize i = 0; i < _size; ++i)
                _arr[i] = DefaultOf<T>();
        }
    }

    template <typename T>
    Array<T>::Array(const std::initializer_list<T> &lst)
        : _size(lst.size())
        , _arr(Null)
    {
        if (_size > 0)
        {
            _arr = new T[_size];
            fsize i = 0;
            for (auto &elem : lst)
                _arr[i++] = elem;
        }
    }

    template <typename T, fsize I>
    Array<T, I>::Array(const std::initializer_list<T> &lst)
    {
        fsize i = 0;
        for (auto &elem : lst)
        {
            _arr[i++] = elem;
            if (i >= I)
                break;
        }
    }

    template <typename T>
    Array<T> &Array<T>::operator=(Array<T> &&arr)
    {
        delete[] _arr;
        _size = arr._size;
        _arr = arr._arr;
        arr._arr = Null;
        arr._size = 0;
        return (*this);
    }

    template <typename T>
    Array<T> &Array<T>::operator=(const Array<T> &arr)
    {
        delete[] _arr;
        _size = arr._size;
        _arr = new T[_size];
        for (fsize i = 0; i < _size; ++i)
            _arr[i] = arr[i];
        return (*this);
    }

    template <typename T>
    inline Array<T>::~Array()
    {
        delete[] _arr;
    }

    template <typename T>
    inline const T &Array<T>::operator[](const fsize id) const
    {
        if (id >= _size)
            throw IndexException(static_cast<fisize>(id));
        return (_arr[id]);
    }

    template <typename T>
    void Array<T>::Resize(const fsize newSize)
    {
        if (_size == newSize)
            return;
        T *tmp = _arr;
        _arr = new T[newSize];
        for (fsize i = 0; i < ((newSize < _size) ? newSize : _size); ++i)
            _arr[i] = std::move(tmp[i]);
        for (fsize i = _size; i < newSize; ++i)
            _arr[i] = DefaultOf<T>();
        _size = newSize;
        delete[] tmp;
    }

    template <typename T>
    T &Array<T>::operator[](const fsize id)
    {
        if (id >= _size)
            throw IndexException(static_cast<fisize>(id));
        return (_arr[id]);
    }

    template <typename T, fsize I>
    void Array<T, I>::Swap(const Iterator &a, const Iterator &b)
    {
        if (a.ArrayPos() >= I || b.ArrayPos() >= I)
            return;
        T tmp = std::move(_arr[a.ArrayPos()]);
        _arr[a.ArrayPos()] = std::move(_arr[b.ArrayPos()]);
        _arr[b.ArrayPos()] = std::move(tmp);
    }

    template <typename T>
    void Array<T>::Swap(const Iterator &a, const Iterator &b)
    {
        if (a.ArrayPos() >= _size || b.ArrayPos() >= _size)
            return;
        T tmp = std::move(_arr[a.ArrayPos()]);
        _arr[a.ArrayPos()] = std::move(_arr[b.ArrayPos()]);
        _arr[b.ArrayPos()] = std::move(tmp);
    }

    template <typename T, fsize I>
    typename Array<T, I>::Iterator Array<T, I>::FindByKey(const fsize pos)
    {
        if (pos >= I)
            return (Iterator(_arr, I));
        return (Iterator(_arr, pos));
    }

    template <typename T>
    typename Array<T>::Iterator Array<T>::FindByKey(const fsize pos)
    {
        if (pos >= _size)
            return (Iterator(_arr, _size, _size));
        return (Iterator(_arr, _size, pos));
    }

    template <typename T, fsize I>
    template <template <typename> class Comparator>
    typename Array<T, I>::Iterator Array<T, I>::FindByValue(const T &val)
    {
        fsize pos = 0;

        for (auto &elem : *this)
        {
            if (Comparator<T>::Eval(elem, val))
                return (Iterator(_arr, pos));
            ++pos;
        }
        return (Iterator(_arr, I));
    }

    template <typename T>
    template <template <typename> class Comparator>
    typename Array<T>::Iterator Array<T>::FindByValue(const T &val)
    {
        fsize pos = 0;

        for (auto &elem : *this)
        {
            if (Comparator<T>::Eval(elem, val))
                return (Iterator(_arr, _size, pos));
            ++pos;
        }
        return (Iterator(_arr, _size, _size));
    }

    template <typename T, fsize I>
    typename Array<T, I>::Iterator Array<T, I>::Find(const std::function<bool(const fsize pos, const T &val)> &comparator)
    {
        fsize pos = 0;

        for (auto &elem : *this)
        {
            if (comparator(pos, elem))
                return (Iterator(*_arr, pos));
            ++pos;
        }
        return (Iterator(*_arr, I));
    }

    template <typename T>
    typename Array<T>::Iterator Array<T>::Find(const std::function<bool(const fsize pos, const T &val)> &comparator)
    {
        fsize pos = 0;

        for (auto &elem : *this)
        {
            if (comparator(pos, elem))
                return (Iterator(*_arr, _size, pos));
            ++pos;
        }
        return (Iterator(*_arr, _size, _size));
    }
}
