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
#include "Framework/Types.hpp"
#include <functional>

namespace bpf
{
    namespace collection
    {
        /**
         * A simple double linked list
         * @tparam T the type of element to store
         */
        template <typename T>
        class BP_TPL_API List
        {
        private:
            class Node
            {
            public:
                Node *Next;
                Node *Prev;
                T Data;

                explicit Node(const T &data)
                    : Next(Null)
                    , Prev(Null)
                    , Data(data)
                {
                }
                explicit Node(T &&data)
                    : Next(Null)
                    , Prev(Null)
                    , Data(std::move(data))
                {
                }
            };

        public:
            class BP_TPL_API Iterator final : public IIterator<typename List<T>::Iterator, T>
            {
            private:
                Node *_cur;
                Node *_reset;

            public:
                inline Iterator(Node *start, Node *reset)
                    : _cur(start)
                    , _reset(reset)
                {
                }
                inline Iterator &operator++()
                {
                    if (_cur)
                        _cur = _cur->Next;
                    return (*this);
                }
                inline Iterator &operator--()
                {
                    if (_cur)
                        _cur = _cur->Prev ? _cur->Prev : _cur;
                    else
                        _cur = _reset;
                    return (*this);
                }
                inline const T *operator->() const
                {
                    return (&_cur->Data);
                }
                inline const T &operator*() const
                {
                    return (_cur->Data);
                }
                inline bool operator==(const Iterator &it) const
                {
                    return (_cur == it._cur);
                }
                inline bool operator!=(const Iterator &it) const
                {
                    return (_cur != it._cur);
                }

                friend class List<T>;
            };

            class BP_TPL_API ReverseIterator final : public IIterator<typename List<T>::ReverseIterator, T>
            {
            private:
                Node *_cur;
                Node *_reset;

            public:
                inline ReverseIterator(Node *start, Node *reset)
                    : _cur(start)
                    , _reset(reset)
                {
                }
                inline ReverseIterator &operator++()
                {
                    if (_cur)
                        _cur = _cur->Prev;
                    return (*this);
                }
                inline ReverseIterator &operator--()
                {
                    if (_cur)
                        _cur = _cur->Next ? _cur->Next : _cur;
                    else
                        _cur = _reset;
                    return (*this);
                }
                inline const T *operator->() const
                {
                    return (&_cur->Data);
                }
                inline const T &operator*() const
                {
                    return (_cur->Data);
                }
                inline bool operator==(const ReverseIterator &it) const
                {
                    return (_cur == it._cur);
                }
                inline bool operator!=(const ReverseIterator &it) const
                {
                    return (_cur != it._cur);
                }
            };

        private:
            Node *_first;
            Node *_last;
            fsize _count;

            template <template <typename> class Comparator>
            Node *Partition(Node *start, Node *end);
            template <template <typename> class Comparator>
            void QuickSort(Node *start, Node *end);
            template <template <typename> class Comparator>
            void Merge(Node **startl, Node **endl, Node **startr, Node **endr);
            template <template <typename> class Comparator>
            void MergeSort();
            void RemoveNode(Node *toRM);
            void Swap(Node *a, Node *b);
            Node *GetNode(fsize id) const;

        public:
            /**
             * Constructs an empty List
             */
            List<T>();

            /**
             * Constructs a List from an existing initializer list
             * @param lst the initial list of items to add to this new List
             */
            List<T>(const std::initializer_list<T> &lst);

            /**
             * Copy constructor
             */
            List<T>(const List<T> &other);

            /**
             * Move constructor
             */
            List<T>(List<T> &&other);

            ~List<T>();

            /**
             * Move assignment operator
             */
            List<T> &operator=(List<T> &&other);

            /**
             * Copy assignment operator
             */
            List<T> &operator=(const List<T> &other);

            /**
             * Adds an item at the end of this list
             * @param elem the element to add
             */
            void Add(const T &elem);

            /**
             * Adds an item at the end of this list
             * @param elem the element to add
             */
            void Add(T &&elem);

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
             * Returns an element const mode
             * @param id the index of the element, in case of out of bounds, throws
             * @throw IndexException if id is out of bounds
             * @return immutable item at index id
             */
            const T &operator[](const fsize id) const;

            /**
             * Returns an element non-const mode
             * @param id the index of the element, in case of out of bounds, throws
             * @throw IndexException if id is out of bounds
             * @return mutable item at index id
             */
            T &operator[](const fsize id);

            /**
             * Create a new list from concatenation of two lists
             * @param other list to concatenate with
             * @return new list
             */
            List<T> operator+(const List<T> &other) const;

            /**
             * Appends the content of a list at the end of this list
             * @param other list to append
             */
            void operator+=(const List<T> &other);

            /**
             * Compare List by performing a per-element check
             * @param other List to compare with
             * @return true if the two lists are equal, false otherwise
             */
            bool operator==(const List<T> &other);

            /**
             * Compare List by performing a per-element check
             * @param other ArrayList to compare with
             * @return false if the two lists are equal, true otherwise
             */
            inline bool operator!=(const List<T> &other)
            {
                return (!operator==(other));
            }

            /**
             * Removes an item at an arbitary position in the list
             * @param pos item position
             */
            void RemoveAt(const fsize pos);

            /**
             * Removes an item at an arbitary position in the list
             * @param pos item position
             */
            void RemoveAt(Iterator &pos);

            /**
             * Removes an item at an arbitary position in the list
             * @param pos item position
             */
            void RemoveAt(Iterator &&pos);

            /**
             * Removes occurences of an element fron the list
             * @param elem the element to search for
             * @param all wether or not to remove all occurences or just the first one
             * @tparam Comparator the comparision operator to use for comparing values
             */
            template <template <typename> class Comparator = ops::Equal>
            void Remove(const T &elem, const bool all = true);

            /**
             * Locate an item by index inside this list
             * @param pos the index of the item to search for
             * @return iterator to the found item or end() if none
             */
            Iterator FindByKey(const fsize pos);

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
             * Swap two elements by iterator in the List
             * @param a first element
             * @param b second element
             */
            void Swap(const Iterator &a, const Iterator &b);

            /**
             * Removes the last item in this list
             */
            void RemoveLast();

            /**
             * Sorts this collection in place
             * @param stable if true this function will apply a Merge-Sort algorithm, otherwise this function uses the Quick-Sort algorithm
             * @tparam Comparator comparision operator
             */
            template <template <typename> class Comparator = ops::Less>
            void Sort(const bool stable = false);

            /**
             * Returns the first element in this List
             * @throw IndexException if none
             * @return mutable item
             */
            inline T &First()
            {
                if (_first == Null)
                    throw IndexException(0);
                return (_first->Data);
            }

            /**
             * Returns the last element in this List
             * @throw IndexException if none
             * @return mutable item
             */
            inline T &Last()
            {
                if (_last == Null)
                    throw IndexException(0);
                return (_last->Data);
            }

            /**
             * Returns the first element in this List
             * @throw IndexException if none
             * @return immutable item
             */
            inline const T &First() const
            {
                if (_first == Null)
                    throw IndexException(0);
                return (_first->Data);
            }

            /**
             * Returns the last element in this List
             * @throw IndexException if none
             * @return immutable item
             */
            inline const T &Last() const
            {
                if (_last == Null)
                    throw IndexException(0);
                return (_last->Data);
            }

            /**
             * Returns the number of items in this list
             * @return number of items as unsigned
             */
            inline fsize Size() const noexcept
            {
                return (_count);
            }

            /**
             * Clears the content of this List
             */
            void Clear();

            /**
             * Returns an iterator to the begining of the collection
             * @return new iterator
             */
            inline Iterator begin() const
            {
                return (Iterator(_first, _last));
            }

            /**
             * Returns an iterator to the end of the collection
             * @return new iterator
             */
            inline Iterator end() const
            {
                return (Iterator(Null, _last));
            }

            /**
             * Returns a reverse iterator to the begining of the collection
             * @return new iterator
             */
            inline ReverseIterator rbegin() const
            {
                return (ReverseIterator(_last, _first));
            }

            /**
             * Returns a reverse iterator to the end of the collection
             * @return new iterator
             */
            inline ReverseIterator rend() const
            {
                return (ReverseIterator(Null, _first));
            }
        };
    }
};

#include "Framework/Collection/List.impl.hpp"
