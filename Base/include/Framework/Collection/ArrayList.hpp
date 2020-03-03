// Copyright (c) 2020, BlockProject
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
#include "Framework/Collection/Array.hpp"

namespace bpf
{
    namespace collection
    {
        template <typename T>
        class BP_TPL_API ArrayList
        {
        private:
            fsize _curid;
            Array<T> _arr;
            template <template <typename> class Comparator>
            fsize Partition(fsize start, fsize end);
            template <template <typename> class Comparator>
            void QuickSort(fsize start, fsize end);
            template <template <typename> class Comparator>
            void Merge(const Array<T> &a, Array<T> &c, const fsize start, const fsize mid, const fsize end);
            template <template <typename> class Comparator>
            void MergeSort();

        public:
            using Iterator = typename Array<T>::Iterator;
            using ReverseIterator = typename Array<T>::ReverseIterator;

            /**
             * Constructs a new ArrayList
             * @param preallocsize initial number of items to pre-allocate
             */
            inline ArrayList(const fsize preallocsize = 8)
                : _curid(0), _arr(preallocsize)
            {
            }

            /**
             * Constructs an ArrayList from an existing initializer list
             * @param lst the initial list of items to add to this new ArrayList
             */
            inline ArrayList(const std::initializer_list<T> &lst)
                : _curid(0), _arr(8)
            {
                for (auto &elem : lst)
                    Add(elem);
            }

            inline ArrayList(const ArrayList<T> &other)
                : _curid(other._curid), _arr(other._arr)
            {
            }

            inline ArrayList(ArrayList<T> &&other)
                : _curid(other._curid), _arr(std::move(other._arr))
            {
                other._curid = 0;
            }

            inline ArrayList<T> &operator=(const ArrayList<T> &other)
            {
                _curid = other._curid;
                _arr = other._arr;
                return (*this);
            }

            inline ArrayList<T> &operator=(ArrayList<T> &&other)
            {
                _curid = other._curid;
                _arr = std::move(other._arr);
                other._curid = 0;
                return (*this);
            }

            bool operator==(const ArrayList<T> &other);

            inline bool operator!=(const ArrayList<T> &other)
            {
                return (!operator==(other));
            }

            ArrayList<T> operator+(const ArrayList<T> &other) const;

            void operator+=(const ArrayList<T> &other);

            inline T &First()
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr.First());
            }

            inline T &Last()
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr[_curid - 1]);
            }

            inline const T &First() const
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr.First());
            }

            inline const T &Last() const
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr[_curid - 1]);
            }

            inline T &operator[](const fsize id)
            {
                if (id >= _curid)
                    throw IndexException(static_cast<fisize>(id));
                return (_arr[id]);
            }

            inline const T &operator[](const fsize id) const
            {
                if (id >= _curid)
                    throw IndexException(static_cast<fisize>(id));
                return (_arr[id]);
            }

            inline void Add(const T &elem)
            {
                if (_curid + 1 >= _arr.Size())
                    _arr.Resize(_arr.Size() * 2);
                _arr[_curid++] = elem;
            }

            inline void Add(T &&elem)
            {
                if (_curid + 1 >= _arr.Size())
                    _arr.Resize(_arr.Size() * 2);
                _arr[_curid++] = std::move(elem);
            }

            inline void Swap(const Iterator &a, const Iterator &b)
            {
                _arr.Swap(a, b);
            }

            void Insert(const fsize pos, const T &elem);
            void Insert(const fsize pos, T &&elem);
            void Insert(const Iterator &pos, const T &elem);
            void Insert(const Iterator &pos, T &&elem);

            void RemoveAt(const fsize pos);
            void RemoveAt(Iterator &pos)
            {
                RemoveAt(pos.ArrayPos());
            }
            void RemoveAt(Iterator &&pos)
            {
                RemoveAt(pos.ArrayPos());
            }

            /**
             * Removes occurences of an element fron the list
             * @param elem the element to search for
             * @param all wether or not to remove all occurences or just the first one
             * @tparam Comparator the comparision operator to use for comparing values
             */
            template <template <typename> class Comparator = ops::Equal>
            void Remove(const T &elem, const bool all = true);

            Iterator FindByKey(const fsize pos);

            template <template <typename> class Comparator = ops::Equal>
            Iterator FindByValue(const T &val);

            Iterator Find(const std::function<bool(const fsize pos, const T & val)> &comparator);

            inline void Clear()
            {
                _curid = 0;
            }

            inline void RemoveLast()
            {
                --_curid;
            }

            inline fsize Size() const
            {
                return (_curid);
            }

            inline Array<T> ToArray() const
            {
                Array<T> arr = _arr;
                arr.Resize(_curid);
                return (arr);
            }

            template <template <typename> class Comparator = ops::Less>
            void Sort(const bool stable = false);

            /**
             * Returns an iterator to the begining of the array
             */
            inline Iterator begin() const
            {
                return (Iterator(*_arr, _curid, 0));
            }

            /**
             * Returns an iterator to the end of the array
             */
            inline Iterator end() const
            {
                return (Iterator(*_arr, _curid, _curid));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             */
            inline ReverseIterator rbegin() const
            {
                return (ReverseIterator(*_arr, _curid, _curid - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             */
            inline ReverseIterator rend() const
            {
                return (ReverseIterator(*_arr, _curid, (fsize)-1));
            }
        };
    }
}

#include "Framework/Collection/ArrayList.impl.hpp"
