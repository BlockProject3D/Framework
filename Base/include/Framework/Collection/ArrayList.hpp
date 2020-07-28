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
        /**
         * Simple array-list class, automatically doubles it's size
         * @tparam T the type of element to store
         */
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
            void Merge(const Array<T> &a, Array<T> &c, fsize start, fsize mid, fsize end);
            template <template <typename> class Comparator>
            void MergeSort();

        public:
            using Iterator = typename Array<T>::Iterator;
            using ReverseIterator = typename Array<T>::ReverseIterator;
            using CIterator = typename Array<T>::CIterator;
            using CReverseIterator = typename Array<T>::CReverseIterator;

            /**
             * Constructs a new ArrayList
             * @param preallocsize initial number of items to pre-allocate
             */
            inline ArrayList(const fsize preallocsize = 8)
                : _curid(0)
                , _arr(preallocsize)
            {
            }

            /**
             * Constructs an ArrayList from an existing initializer list
             * @param lst the initial list of items to add to this new ArrayList
             */
            inline ArrayList(const std::initializer_list<T> &lst)
                : _curid(0)
                , _arr(8)
            {
                for (auto &elem : lst)
                    Add(elem);
            }

            /**
             * Copy constructor
             */
            inline ArrayList(const ArrayList<T> &other)
                : _curid(other._curid)
                , _arr(other._arr)
            {
            }

            /**
             * Move constructor
             */
            inline ArrayList(ArrayList<T> &&other) noexcept
                : _curid(other._curid)
                , _arr(std::move(other._arr))
            {
                other._curid = 0;
            }

            /**
             * Copy assignment operator
             */
            inline ArrayList<T> &operator=(const ArrayList<T> &other)
            {
                if (this == &other)
                    return (*this);
                _curid = other._curid;
                _arr = other._arr;
                return (*this);
            }

            /**
             * Move assignment operator
             */
            inline ArrayList<T> &operator=(ArrayList<T> &&other) noexcept
            {
                _curid = other._curid;
                _arr = std::move(other._arr);
                other._curid = 0;
                return (*this);
            }

            /**
             * Compare ArrayList by performing a per-element check
             * @param other ArrayList to compare with
             * @return true if the two lists are equal, false otherwise
             */
            bool operator==(const ArrayList<T> &other);

            /**
             * Compare ArrayList by performing a per-element check
             * @param other ArrayList to compare with
             * @return false if the two lists are equal, true otherwise
             */
            inline bool operator!=(const ArrayList<T> &other)
            {
                return (!operator==(other));
            }

            /**
             * Create a new list from concatenation of two lists
             * @param other list to concatenate with
             * @return new list
             */
            ArrayList<T> operator+(const ArrayList<T> &other) const;

            /**
             * Appends the content of a list at the end of this list
             * @param other list to append
             */
            void operator+=(const ArrayList<T> &other);

            /**
             * Create a new list from concatenation of a list and an array
             * @param other array to concatenate with
             * @return new list
             */
            ArrayList<T> operator+(const Array<T> &other) const;

            /**
             * Appends the content of an array at the end of this list
             * @param other array to append
             */
            void operator+=(const Array<T> &other);

            /**
             * Returns the first element in this ArrayList
             * @throw IndexException if none
             * @return mutable item
             */
            inline T &First()
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr.First());
            }

            /**
             * Returns the last element in this ArrayList
             * @throw IndexException if none
             * @return mutable item
             */
            inline T &Last()
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr[_curid - 1]);
            }

            /**
             * Returns the first element in this ArrayList
             * @throw IndexException if none
             * @return immutable item
             */
            inline const T &First() const
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr.First());
            }

            /**
             * Returns the last element in this ArrayList
             * @throw IndexException if none
             * @return immutable item
             */
            inline const T &Last() const
            {
                if (_curid == 0)
                    throw IndexException(0);
                return (_arr[_curid - 1]);
            }

            /**
             * Returns an element non-const mode
             * @param id the index of the element, in case of out of bounds, throws
             * @throw IndexException if id is out of bounds
             * @return mutable item at index id
             */
            inline T &operator[](const fsize id)
            {
                if (id >= _curid)
                    throw IndexException(static_cast<fisize>(id));
                return (_arr[id]);
            }

            /**
             * Returns an element const mode
             * @param id the index of the element, in case of out of bounds, throws
             * @throw IndexException if id is out of bounds
             * @return immutable item at index id
             */
            inline const T &operator[](const fsize id) const
            {
                if (id >= _curid)
                    throw IndexException(static_cast<fisize>(id));
                return (_arr[id]);
            }

            /**
             * Adds an item at the end of this list
             * @param elem the element to add
             */
            inline void Add(const T &elem)
            {
                if (_curid + 1 >= _arr.Size())
                    _arr.Resize(_arr.Size() * 2);
                _arr[_curid++] = elem;
            }

            /**
             * Adds an item at the end of this list
             * @param elem the element to add
             */
            inline void Add(T &&elem)
            {
                if (_curid + 1 >= _arr.Size())
                    _arr.Resize(_arr.Size() * 2);
                _arr[_curid++] = std::move(elem);
            }

            /**
             * Swap two elements by iterator in the ArrayList
             * @param a first element
             * @param b second element
             */
            inline void Swap(const Iterator &a, const Iterator &b)
            {
                _arr.Swap(a, b);
            }

            /**
             * Inserts an item at an arbitary position in the list
             * @param pos insert position
             * @param elem element to insert
             */
            void Insert(fsize pos, const T &elem);

            /**
             * Inserts an item at an arbitary position in the list
             * @param pos insert position
             * @param elem element to insert
             */
            void Insert(fsize pos, T &&elem);

            /**
             * Inserts an item at an arbitary position in the list
             * @param pos insert position
             * @param elem element to insert
             */
            void Insert(const Iterator &pos, const T &elem);

            /**
             * Inserts an item at an arbitary position in the list
             * @param pos insert position
             * @param elem element to insert
             */
            void Insert(const Iterator &pos, T &&elem);

            /**
             * Removes an item at an arbitary position in the list
             * @param pos item position
             */
            void RemoveAt(fsize pos);

            /**
             * Removes an item at an arbitary position in the list
             * @param pos item position
             */
            void RemoveAt(Iterator &pos)
            {
                RemoveAt(pos.ArrayPos());
            }

            /**
             * Removes an item at an arbitary position in the list
             * @param pos item position
             */
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
            void Remove(const T &elem, bool all = true);

            /**
             * Locate an item by index inside this list
             * @param pos the index of the item to search for
             * @return iterator to the found item or end() if none
             */
            Iterator FindByKey(fsize pos);

            /**
             * Locate an item by performing per-element check
             * @tparam Comparator comparision operator to use
             * @param val the value to search for
             * @return iterator to the found item or end() if none
             */
            template <template <typename> class Comparator = ops::Equal>
            Iterator FindByValue(const T &val);

            /**
             * Locate an item by performing per-element check
             * @param comparator the comparision function to use
             * @return iterator to the found item or end() if none
             */
            Iterator Find(const std::function<bool(const fsize pos, const T &val)> &comparator);

            /**
             * Clears the content of this ArrayList
             * WARNING: Does not automatically deallocate existing items in this list
             */
            inline void Clear()
            {
                _curid = 0;
            }

            /**
             * Removes the last item in this list
             */
            inline void RemoveLast()
            {
                --_curid;
            }

            /**
             * Returns the number of items in this list
             * @return number of items as unsigned
             */
            inline fsize Size() const
            {
                return (_curid);
            }

            /**
             * Converts this ArrayList to a raw Array
             * WARNING: This function rquires deep-copy of the collection
             * @return new Array containing all the items in this collection at the time of this function call
             */
            inline Array<T> ToArray() const
            {
                Array<T> arr = _arr;
                arr.Resize(_curid);
                return (arr);
            }

            /**
             * Sorts this collection in place
             * @param stable if true this function will apply a Merge-Sort algorithm, otherwise this function uses the Quick-Sort algorithm
             * @tparam Comparator comparision operator
             */
            template <template <typename> class Comparator = ops::Less>
            void Sort(bool stable = false);

            /**
             * Returns an iterator to the begining of the collection
             * @return new iterator
             */
            inline CIterator begin() const
            {
                return (CIterator(*_arr, _curid, 0));
            }

            /**
             * Returns an iterator to the end of the collection
             * @return new iterator
             */
            inline CIterator end() const
            {
                return (CIterator(*_arr, _curid, _curid));
            }

            /**
             * Returns an iterator to the begining of the collection
             * @return new iterator
             */
            inline Iterator begin()
            {
                return (Iterator(*_arr, _curid, 0));
            }

            /**
             * Returns an iterator to the end of the collection
             * @return new iterator
             */
            inline Iterator end()
            {
                return (Iterator(*_arr, _curid, _curid));
            }

            /**
             * Returns a reverse iterator to the begining of the collection
             * @return new iterator
             */
            inline CReverseIterator rbegin() const
            {
                return (CReverseIterator(*_arr, _curid, _curid - 1));
            }

            /**
             * Returns a reverse iterator to the end of the collection
             * @return new iterator
             */
            inline CReverseIterator rend() const
            {
                return (CReverseIterator(*_arr, _curid, (fsize)-1));
            }

            /**
             * Returns a reverse iterator to the begining of the collection
             * @return new iterator
             */
            inline ReverseIterator rbegin()
            {
                return (ReverseIterator(*_arr, _curid, _curid - 1));
            }

            /**
             * Returns a reverse iterator to the end of the collection
             * @return new iterator
             */
            inline ReverseIterator rend()
            {
                return (ReverseIterator(*_arr, _curid, (fsize)-1));
            }
        };
    }
}

#include "Framework/Collection/ArrayList.impl.hpp"
