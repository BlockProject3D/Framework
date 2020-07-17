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
#include "Framework/Collection/Iterator.hpp"
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
            class BP_TPL_API Iterator final : public IIterator<typename Array<T, I>::Iterator, T>
            {
            private:
                fsize _curid;
                const T *_arr;

            public:
                inline Iterator(const T *lowlevel, const fsize start)
                    : _curid(start)
                    , _arr(lowlevel)
                {
                }
                inline Iterator &operator++()
                {
                    if (_curid != I)
                        _curid++;
                    return (*this);
                }
                inline Iterator &operator--()
                {
                    if (_curid > 0)
                        _curid--;
                    return (*this);
                }
                inline const T &operator*() const
                {
                    return (_arr[_curid]);
                }
                inline const T *operator->() const
                {
                    return (&_arr[_curid]);
                }
                inline bool operator==(const Iterator &other) const
                {
                    return (_curid == other._curid);
                }
                inline bool operator!=(const Iterator &other) const
                {
                    return (_curid != other._curid);
                }
                inline fsize ArrayPos() const noexcept
                {
                    return (_curid);
                }
            };

            class BP_TPL_API ReverseIterator final : public IIterator<typename Array<T, I>::ReverseIterator, T>
            {
            private:
                fsize _curid;
                const T *_arr;

            public:
                inline ReverseIterator(const T *lowlevel, const fsize start)
                    : _curid(start)
                    , _arr(lowlevel)
                {
                }
                inline ReverseIterator &operator++()
                {
                    if (_curid != (fsize)-1)
                        _curid--;
                    return (*this);
                }
                inline ReverseIterator &operator--()
                {
                    if (_curid < I - 1)
                        _curid++;
                    return (*this);
                }
                inline const T &operator*() const
                {
                    return (_arr[_curid]);
                }
                inline const T *operator->() const
                {
                    return (&_arr[_curid]);
                }
                inline bool operator==(const ReverseIterator &other) const
                {
                    return (_curid == other._curid);
                }
                inline bool operator!=(const ReverseIterator &other) const
                {
                    return (_curid != other._curid);
                }
                inline fsize ArrayPos() const noexcept
                {
                    return (_curid);
                }
            };

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
            inline Iterator begin() const
            {
                return (Iterator(_arr, 0));
            }

            /**
             * Returns an iterator to the end of the array
             * @return new iterator
             */
            inline Iterator end() const
            {
                return (Iterator(_arr, I));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             * @return new iterator
             */
            inline ReverseIterator rbegin() const
            {
                return (ReverseIterator(_arr, I - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             * @return new iterator
             */
            inline ReverseIterator rend() const
            {
                return (ReverseIterator(_arr, (fsize)-1));
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
            class BP_TPL_API Iterator final : public IIterator<typename Array<T, 0>::Iterator, T>
            {
            private:
                fsize _curid;
                fsize _max;
                const T *_arr;

            public:
                inline Iterator(const T *lowlevel, const fsize size, const fsize start)
                    : _curid(start)
                    , _max(size)
                    , _arr(lowlevel)
                {
                }
                inline Iterator &operator++()
                {
                    if (_curid != _max)
                        _curid++;
                    return (*this);
                }
                inline Iterator &operator--()
                {
                    if (_curid > 0)
                        _curid--;
                    return (*this);
                }
                inline const T &operator*() const
                {
                    return (_arr[_curid]);
                }
                inline const T *operator->() const
                {
                    return (&_arr[_curid]);
                }
                inline bool operator==(const Iterator &other) const
                {
                    return (_curid == other._curid);
                }
                inline bool operator!=(const Iterator &other) const
                {
                    return (_curid != other._curid);
                }

                inline fsize ArrayPos() const noexcept
                {
                    return (_curid);
                }
            };

            class BP_TPL_API ReverseIterator final : public IIterator<typename Array<T, 0>::ReverseIterator, T>
            {
            private:
                fsize _curid;
                fsize _max;
                const T *_arr;

            public:
                inline ReverseIterator(const T *lowlevel, const fsize size, const fsize start)
                    : _curid(start)
                    , _max(size)
                    , _arr(lowlevel)
                {
                }
                inline ReverseIterator &operator++()
                {
                    if (_curid != (fsize)-1)
                        _curid--;
                    return (*this);
                }
                inline ReverseIterator &operator--()
                {
                    if (_curid < _max - 1)
                        _curid++;
                    return (*this);
                }
                inline const T &operator*() const
                {
                    return (_arr[_curid]);
                }
                inline const T *operator->() const
                {
                    return (&_arr[_curid]);
                }
                inline bool operator==(const ReverseIterator &other) const
                {
                    return (_curid == other._curid);
                }
                inline bool operator!=(const ReverseIterator &other) const
                {
                    return (_curid != other._curid);
                }

                inline fsize ArrayPos() const noexcept
                {
                    return (_curid);
                }
            };

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
                if (_arr == Null || _size == 0)
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
                if (_arr == Null || _size == 0)
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
                if (_arr == Null || _size == 0)
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
                if (_arr == Null || _size == 0)
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
                _arr = Null;
                _size = 0;
                return (tmp);
            }

            /**
             * Returns an iterator to the begining of the array
             * @return new iterator
             */
            inline Iterator begin() const
            {
                return (Iterator(_arr, _size, 0));
            }

            /**
             * Returns an iterator to the end of the array
             * @return new iterator
             */
            inline Iterator end() const
            {
                return (Iterator(_arr, _size, _size));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             * @return new iterator
             */
            inline ReverseIterator rbegin() const
            {
                return (ReverseIterator(_arr, _size, _size - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             * @return new iterator
             */
            inline ReverseIterator rend() const
            {
                return (ReverseIterator(_arr, _size, (fsize)-1));
            }
        };
    }
}

#include "Framework/Collection/Array.impl.hpp"
