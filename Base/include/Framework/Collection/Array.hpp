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
#include "Framework/Collection/Array.Iterator.hpp"
#include "Framework/Collection/Utility.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/Types.hpp"
#include <functional>
#include <initializer_list>

namespace bpf
{
    namespace collection
    {
        /**
         * Static array
         * @tparam T the type of element to store
         * @tparam I the size of the array as a constant expression
         */
        template <typename T, fsize I = 0>
        class BP_TPL_API Array
        {
        public:
            using Iterator = ArrayIterator<T>;
            using CIterator = ArrayConstIterator<T>;
            using ReverseIterator = ArrayReverseIterator<T>;
            using CReverseIterator = ArrayConstReverseIterator<T>;

        private:
            T _arr[I];

        public:
            /**
             * Constructs an empty array
             */
            inline Array()
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = T();
            }

            /**
             * Constructs an array from a list of values
             * @param lst the list of values to copy the values from
             */
            Array(const std::initializer_list<T> &lst);

            /**
             * Copy constructor
             */
            inline Array(const Array<T, I> &arr)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = arr._arr[i];
            }

            /**
             * Move constructor
             */
            inline Array(Array<T, I> &&arr) noexcept
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = std::move(arr._arr[i]);
            }

            /**
             * Copy assignment operator
             */
            inline Array<T, I> &operator=(const Array<T, I> &arr)
            {
                if (this == &arr)
                    return (*this);
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = arr._arr[i];
                return (*this);
            }

            /**
             * Move assignment operator
             */
            inline Array<T, I> &operator=(Array<T, I> &&arr) noexcept
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = std::move(arr._arr[i]);
                return (*this);
            }

            /**
             * Swap two elements by iterator in the Array
             * @param a first element
             * @param b second element
             */
            void Swap(const Iterator &a, const Iterator &b);

            /**
             * Returns the length of the array
             * @return size of collection as an unsigned number
             */
            constexpr inline fsize Size() const
            {
                return (I);
            }

            /**
             * Returns the first element in this Array
             * @return mutable item
             */
            inline T &First()
            {
                return (_arr[0]);
            }

            /**
             * Returns the last element in this Array
             * @return mutable item
             */
            inline T &Last()
            {
                return (_arr[I - 1]);
            }

            /**
             * Returns the first element in this array
             * @return immutable item
             */
            inline const T &First() const
            {
                return (_arr[0]);
            }

            /**
             * Returns the last element in this array
             * @return immutable item
             */
            inline const T &Last() const
            {
                return (_arr[I - 1]);
            }

            /**
             * Returns an immutable low-level c-like arrray
             * @return low-level c-like arrray pointer
             */
            inline const T *operator*() const
            {
                return (_arr);
            }

            /**
             * Returns a mutable low-level c-like arrray
             * @return low-level c-like arrray pointer
             */
            inline T *operator*()
            {
                return (_arr);
            }

            /**
             * Returns an element non-const mode
             * @param id the index of the element, in case of out of bounds, throws
             * @throw IndexException if id is out of bounds
             * @return mutable item at index id
             */
            inline T &operator[](const fsize id)
            {
                if (id >= I)
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
                if (id >= I)
                    throw IndexException(static_cast<fisize>(id));
                return (_arr[id]);
            }

            /**
             * Compare Array by performing a per-element check
             * @param other Array to compare with
             * @return true if the two arrays are equal, false otherwise
             */
            bool operator==(const Array<T, I> &other);

            /**
             * Compare Array by performing a per-element check
             * @param other Array to compare with
             * @return false if the two arrays are equal, true otherwise
             */
            inline bool operator!=(const Array<T, I> &other)
            {
                return (!operator==(other));
            }

            /**
             * Locate an item by index inside this array
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
             * Returns an iterator to the begining of the array
             * @return new iterator
             */
            inline Iterator begin()
            {
                return (Iterator(_arr, 0));
            }

            /**
             * Returns an iterator to the end of the array
             * @return new iterator
             */
            inline Iterator end()
            {
                return (Iterator(_arr, I));
            }

            /**
             * Returns an iterator to the begining of the array
             * @return new iterator
             */
            inline CIterator begin() const
            {
                return (CIterator(_arr, 0));
            }

            /**
             * Returns an iterator to the end of the array
             * @return new iterator
             */
            inline CIterator end() const
            {
                return (CIterator(_arr, I));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             * @return new iterator
             */
            inline ReverseIterator rbegin()
            {
                return (ReverseIterator(_arr, I, I - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             * @return new iterator
             */
            inline ReverseIterator rend()
            {
                return (ReverseIterator(_arr, I, (fsize)-1));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             * @return new iterator
             */
            inline CReverseIterator rbegin() const
            {
                return (CReverseIterator(_arr, I, I - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             * @return new iterator
             */
            inline CReverseIterator rend() const
            {
                return (CReverseIterator(_arr, I, (fsize)-1));
            }
        };

        /**
         * Dynamic array (array which size is defined dynamically at run-time)
         * @tparam T the type of element to store
         */
        template <typename T>
        class BP_TPL_API Array<T, 0>
        {
        public:
            using Iterator = ArrayIterator<T>;
            using CIterator = ArrayConstIterator<T>;
            using ReverseIterator = ArrayReverseIterator<T>;
            using CReverseIterator = ArrayConstReverseIterator<T>;

        private:
            fsize _size;
            T *_arr;

        public:
            /**
             * Constructs an empty array
             */
            Array();

            /**
             * Constructs an array of given size
             * @param size the size of the new array
             */
            explicit Array(fsize size);

            /**
             * Constructs an array from a list of values
             * @param lst the list of values to copy the values from
             */
            Array(const std::initializer_list<T> &lst);

            /**
             * Move constructor
             */
            Array(Array<T> &&arr) noexcept;

            /**
             * Copy constructor
             */
            Array(const Array<T> &arr);
            ~Array();

            /**
             * Move assignment operator
             */
            Array<T> &operator=(Array<T> &&arr) noexcept;

            /**
             * Copy assignment operator
             */
            Array<T> &operator=(const Array<T> &arr);

            /**
             * Swap two elements by iterator in the Array
             * @param a first element
             * @param b second element
             */
            void Swap(const Iterator &a, const Iterator &b);

            /**
             * Returns an element const mode
             * @param id the index of the element, in case of out of bounds, throws
             * @throw IndexException if id is out of bounds
             * @return immutable item at index id
             */
            const T &operator[](fsize id) const;

            /**
             * Returns an element non-const mode
             * @param id the index of the element, in case of out of bounds, throws
             * @throw IndexException if id is out of bounds
             * @return mutable item at index id
             */
            T &operator[](fsize id);

            /**
             * Resizes the array to a different size
             * @param newSize the new array size
             */
            void Resize(fsize newSize);

            /**
             * Compare Array by performing a per-element check
             * @param other Array to compare with
             * @return true if the two arrays are equal, false otherwise
             */
            bool operator==(const Array<T> &other);

            /**
             * Compare Array by performing a per-element check
             * @param other Array to compare with
             * @return false if the two arrays are equal, true otherwise
             */
            inline bool operator!=(const Array<T> &other)
            {
                return (!operator==(other));
            }

            /**
             * Locate an item by index inside this array
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
             * Returns the first element in this Array
             * @throw IndexException if Array is not initialized
             * @return mutable item
             */
            inline T &First()
            {
                if (_arr == nullptr || _size == 0)
                    throw IndexException(0);
                return (_arr[0]);
            }

            /**
             * Returns the last element in this Array
             * @throw IndexException if Array is not initialized
             * @return mutable item
             */
            inline T &Last()
            {
                if (_arr == nullptr || _size == 0)
                    throw IndexException(0);
                return (_arr[_size - 1]);
            }

            /**
             * Returns the first element in this array
             * @throw IndexException if array is not initialized
             * @return immutable item
             */
            inline const T &First() const
            {
                if (_arr == nullptr || _size == 0)
                    throw IndexException(0);
                return (_arr[0]);
            }

            /**
             * Returns the last element in this array
             * @throw IndexException if array is not initialized
             * @return immutable item
             */
            inline const T &Last() const
            {
                if (_arr == nullptr || _size == 0)
                    throw IndexException(0);
                return (_arr[_size - 1]);
            }

            /**
             * Returns the length of the array
             * @return size of collection as an unsigned number
             */
            inline fsize Size() const
            {
                return (_size);
            }

            /**
             * Returns an immutable low-level c-like arrray
             * @return low-level c-like arrray pointer
             */
            inline const T *operator*() const
            {
                return (_arr);
            }

            /**
             * Returns a mutable low-level c-like arrray
             * @return low-level c-like arrray pointer
             */
            inline T *operator*()
            {
                return (_arr);
            }

            /**
             * Terminates management of the raw pointer and returns it
             * WARNING: You are responsible for calling MemUtils::DeleteArray on the raw pointer after calling this
             * function. Do not call this function unless you know what you're doing
             * @return low-level c-like arrray pointer
             */
            inline T *Release()
            {
                T *tmp = _arr;
                _arr = nullptr;
                _size = 0;
                return (tmp);
            }

            /**
             * Returns an iterator to the begining of the array
             * @return new iterator
             */
            inline Iterator begin()
            {
                return (Iterator(_arr, _size, 0));
            }

            /**
             * Returns an iterator to the end of the array
             * @return new iterator
             */
            inline Iterator end()
            {
                return (Iterator(_arr, _size, _size));
            }

            /**
             * Returns an iterator to the begining of the array
             * @return new iterator
             */
            inline CIterator begin() const
            {
                return (CIterator(_arr, _size, 0));
            }

            /**
             * Returns an iterator to the end of the array
             * @return new iterator
             */
            inline CIterator end() const
            {
                return (CIterator(_arr, _size, _size));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             * @return new iterator
             */
            inline ReverseIterator rbegin()
            {
                return (ReverseIterator(_arr, _size, _size - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             * @return new iterator
             */
            inline ReverseIterator rend()
            {
                return (ReverseIterator(_arr, _size, (fsize)-1));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             * @return new iterator
             */
            inline CReverseIterator rbegin() const
            {
                return (CReverseIterator(_arr, _size, _size - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             * @return new iterator
             */
            inline CReverseIterator rend() const
            {
                return (CReverseIterator(_arr, _size, (fsize)-1));
            }
        };
    }
}

#include "Framework/Collection/Array.impl.hpp"
