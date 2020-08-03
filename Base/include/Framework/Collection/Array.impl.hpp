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
#include "Framework/Memory/MemUtils.hpp"
#include <utility>

namespace bpf
{
    namespace collection
    {
        template <typename T>
        inline Array<T>::Array()
            : _size(0)
            , _arr(nullptr)
        {
        }

        template <typename T>
        inline Array<T>::Array(Array<T> &&arr) noexcept
            : _size(arr._size)
            , _arr(arr._arr)
        {
            arr._arr = nullptr;
            arr._size = 0;
        }

        template <typename T>
        Array<T>::Array(const Array<T> &arr)
            : _size(arr._size)
            , _arr(nullptr)
        {
            if (_size > 0)
            {
                _arr = memory::MemUtils::NewArray<T>(_size);
                for (fsize i = 0; i < _size; ++i)
                    _arr[i] = arr[i];
            }
        }

        template <typename T>
        Array<T>::Array(const fsize size)
            : _size(size)
            , _arr(nullptr)
        {
            if (size > 0)
            {
                _arr = memory::MemUtils::NewArray<T>(_size);
            }
        }

        template <typename T>
        Array<T>::Array(const std::initializer_list<T> &lst)
            : _size(lst.size())
            , _arr(nullptr)
        {
            if (_size > 0)
            {
                _arr = memory::MemUtils::NewArray<T>(_size);
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
            while (i != I)
            {
                _arr[i] = T();
                ++i;
            }
        }

        template <typename T>
        Array<T> &Array<T>::operator=(Array<T> &&arr) noexcept
        {
            memory::MemUtils::DeleteArray(_arr, _size);
            _size = arr._size;
            _arr = arr._arr;
            arr._arr = nullptr;
            arr._size = 0;
            return (*this);
        }

        template <typename T>
        Array<T> &Array<T>::operator=(const Array<T> &arr)
        {
            if (this == &arr)
                return (*this);
            memory::MemUtils::DeleteArray(_arr, _size);
            _size = arr._size;
            _arr = memory::MemUtils::NewArray<T>(_size);
            for (fsize i = 0; i < _size; ++i)
                _arr[i] = arr[i];
            return (*this);
        }

        template <typename T>
        inline Array<T>::~Array()
        {
            memory::MemUtils::DeleteArray(_arr, _size);
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
            _arr = memory::MemUtils::ResizeArray(_arr, _size, newSize);
            _size = newSize;
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
            if (a.Position() >= I || b.Position() >= I)
                return;
            T tmp = std::move(_arr[a.Position()]);
            _arr[a.Position()] = std::move(_arr[b.Position()]);
            _arr[b.Position()] = std::move(tmp);
        }

        template <typename T>
        void Array<T>::Swap(const Iterator &a, const Iterator &b)
        {
            if (a.Position() >= _size || b.Position() >= _size)
                return;
            T tmp = std::move(_arr[a.Position()]);
            _arr[a.Position()] = std::move(_arr[b.Position()]);
            _arr[b.Position()] = std::move(tmp);
        }

        template <typename T, fsize I>
        typename Array<T, I>::Iterator Array<T, I>::FindByKey(const fsize pos)
        {
            if (pos >= I)
                return (Iterator(_arr, I, I));
            return (Iterator(_arr, I, pos));
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
                    return (Iterator(_arr, I, pos));
                ++pos;
            }
            return (Iterator(_arr, I, I));
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
                    return (Iterator(_arr, I, pos));
                ++pos;
            }
            return (Iterator(_arr, I, I));
        }

        template <typename T>
        typename Array<T>::Iterator Array<T>::Find(const std::function<bool(const fsize pos, const T &val)> &comparator)
        {
            fsize pos = 0;

            for (auto &elem : *this)
            {
                if (comparator(pos, elem))
                    return (Iterator(_arr, _size, pos));
                ++pos;
            }
            return (Iterator(_arr, _size, _size));
        }

        template <typename T>
        bool Array<T>::operator==(const Array<T> &other)
        {
            if (_size != other._size)
                return (false);
            for (fsize i = 0; i != _size; ++i)
            {
                if (_arr[i] != other._arr[i])
                    return (false);
            }
            return (true);
        }

        template <typename T, fsize I>
        bool Array<T, I>::operator==(const Array<T, I> &other)
        {
            for (fsize i = 0; i != I; ++i)
            {
                if (_arr[i] != other._arr[i])
                    return (false);
            }
            return (true);
        }
    }
}
