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
#include <functional>
#include <initializer_list>
#include "Framework/Collection/Utility.hpp"
#include "Framework/Collection/Iterator.hpp"
#include "Framework/Types.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/IndexException.hpp"

namespace bpf
{
    namespace collection
    {
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
                    : _curid(start), _arr(lowlevel)
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
                    : _curid(start), _arr(lowlevel)
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
            inline Array()
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = T();
            }

            Array(const std::initializer_list<T> &lst);

            inline Array(const Array<T> &arr)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = arr._arr[i];
            }

            inline Array<T, I> &operator=(const Array<T, I> &arr)
            {
                for (fsize i = 0; i != I; ++i)
                    _arr[i] = arr._arr[i];
                return (*this);
            }

            void Swap(const Iterator &a, const Iterator &b);

            constexpr inline fsize Size() const
            {
                return (I);
            }

            inline T &First()
            {
                return (_arr[0]);
            }

            inline T &Last()
            {
                return (_arr[I - 1]);
            }

            inline const T &First() const
            {
                return (_arr[0]);
            }

            inline const T &Last() const
            {
                return (_arr[I - 1]);
            }

            inline const T *operator*() const
            {
                return (_arr);
            }

            inline T *operator*()
            {
                return (_arr);
            }

            inline T &operator[](const fsize id)
            {
                if (id >= I)
                    throw IndexException(static_cast<fisize>(id));
                return (_arr[id]);
            }

            inline const T &operator[](const fsize id) const
            {
                if (id >= I)
                    throw IndexException(static_cast<fisize>(id));
                return (_arr[id]);
            }

            bool operator==(const Array<T, I> &other);

            inline bool operator!=(const Array<T, I> &other)
            {
                return (!operator==(other));
            }

            Iterator FindByKey(const fsize pos);

            template <template <typename> class Comparator = ops::Equal>
            Iterator FindByValue(const T &val);

            Iterator Find(const std::function<bool(const fsize pos, const T & val)> &comparator);

            /**
             * Returns an iterator to the begining of the array
             */
            inline Iterator begin() const
            {
                return (Iterator(_arr, 0));
            }

            /**
             * Returns an iterator to the end of the array
             */
            inline Iterator end() const
            {
                return (Iterator(_arr, I));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             */
            inline ReverseIterator rbegin() const
            {
                return (ReverseIterator(_arr, I - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             */
            inline ReverseIterator rend() const
            {
                return (ReverseIterator(_arr, (fsize)-1));
            }
        };

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
                    : _curid(start), _max(size), _arr(lowlevel)
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
                    : _curid(start), _max(size), _arr(lowlevel)
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
            explicit Array(const fsize size);

            Array(const std::initializer_list<T> &lst);
            Array(Array<T> &&arr);
            Array(const Array<T> &arr);
            ~Array();

            Array<T> &operator=(Array<T> &&arr);
            Array<T> &operator=(const Array<T> &arr);

            void Swap(const Iterator &a, const Iterator &b);

            /**
             * Returns an element const mode
             * @param id the index of the element, in case of out of bounds, throws
             */
            const T &operator[](const fsize id) const;

            /**
             * Returns an element non-const mode
             * @param id the index of the element, in case of out of bounds, throws
             */
            T &operator[](const fsize id);

            /**
             * Resizes the array to a different size
             * @param newSize the new array size
             */
            void Resize(const fsize newSize);

            bool operator==(const Array<T> &other);

            inline bool operator!=(const Array<T> &other)
            {
                return (!operator==(other));
            }

            Iterator FindByKey(const fsize pos);

            template <template <typename> class Comparator = ops::Equal>
            Iterator FindByValue(const T &val);

            Iterator Find(const std::function<bool(const fsize pos, const T & val)> &comparator);

            inline T &First()
            {
                if (_arr == Null || _size == 0)
                    throw IndexException(0);
                return (_arr[0]);
            }

            inline T &Last()
            {
                if (_arr == Null || _size == 0)
                    throw IndexException(0);
                return (_arr[_size - 1]);
            }

            inline const T &First() const
            {
                if (_arr == Null || _size == 0)
                    throw IndexException(0);
                return (_arr[0]);
            }

            inline const T &Last() const
            {
                if (_arr == Null || _size == 0)
                    throw IndexException(0);
                return (_arr[_size - 1]);
            }

            /**
             * Returns the length of the array
             */
            inline fsize Size() const
            {
                return (_size);
            }

            inline const T *operator*() const
            {
                return (_arr);
            }

            inline T *operator*()
            {
                return (_arr);
            }

            /**
             * Terminates management of the raw pointer and returns it
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
             */
            inline Iterator begin() const
            {
                return (Iterator(_arr, _size, 0));
            }

            /**
             * Returns an iterator to the end of the array
             */
            inline Iterator end() const
            {
                return (Iterator(_arr, _size, _size));
            }

            /**
             * Returns a reverse iterator to the begining of the array
             */
            inline ReverseIterator rbegin() const
            {
                return (ReverseIterator(_arr, _size, _size - 1));
            }

            /**
             * Returns a reverse iterator to the end of the array
             */
            inline ReverseIterator rend() const
            {
                return (ReverseIterator(_arr, _size, (fsize)-1));
            }
        };
    }
};

#include "Framework/Collection/Array.impl.hpp"
