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
#include "Framework/Types.hpp"
#include "Framework/Iterator.hpp"
#include "Framework/IndexException.hpp"

//TODO : Use bpf::fsize for sizes
namespace bpf
{
    template <typename T>
    class BP_TPL_API ListNode
    {
    public:
        ListNode *Next;
        ListNode *Prev;
        T Data;

	explicit ListNode(const T &data)
            : Next(Null), Prev(Null), Data(data)
        {
        }
        explicit ListNode(T &&data)
            : Next(Null), Prev(Null), Data(std::move(data))
        {
        }
    };

    /**
     * A simple double chained linked list
     * @tparam T the type of element in that list
     */
    template <typename T>
    class BP_TPL_API List
    {
    public:
        class BP_TPL_API Iterator final : public IIterator<typename List<T>::Iterator, T>
        {
        private:
            ListNode<T> *Cur;

        public:
            explicit inline Iterator(ListNode<T> *cur)
                : Cur(cur)
            {
            }
            inline void operator++()
            {
                if (Cur)
                    Cur = Cur->Next;
            }
            inline void operator--()
            {
                if (Cur)
                    Cur = Cur->Prev;
            }
            inline const T &operator->() const
            {
                return Cur->Data;
            }
            inline const T &operator*() const
            {
                return Cur->Data;
            }
            inline bool operator==(const Iterator &it) const
            {
                return (Cur == it.Cur);
            }
            inline bool operator!=(const Iterator &it) const
            {
                return (Cur != it.Cur);
            }
            inline operator bool() const
            {
                return (Cur != Null);
            }
        };
    private:
        ListNode<T> *First;
        ListNode<T> *Last;
        uint32 Count;

        ListNode<T> *Partition(ListNode<T> *start, ListNode<T> *end);
        void QuickSort(ListNode<T> *start, ListNode<T> *end);
        void RemoveNode(ListNode<T> *toRM);
    public:
        List<T>();
        List<T>(List<T> &&other);
        List<T>(const List<T> &other);
        ~List<T>();

        List<T> &operator=(const List<T> &other);
        List<T> &operator=(List<T> &&other);

        /**
         * Adds an element at the end of the list
         * @param elem element to add
         */
        void Add(const T &elem);

        /**
         * Inserts an element at a given position
         * @param elem element to insert
         * @param pos position
         */
        void Insert(const T &elem, uint32 pos);

        /**
         * Adds an element at the end of the list
         * @param elem element to add
         */
        void Add(T &&elem);

        /**
         * Inserts an element at a given position
         * @param elem element to insert
         * @param pos position
         */
        void Insert(T &&elem, uint32 pos);

        /**
         * Returns an element (safe), returns Null whenever element could not be found
         * @param id index of the element
         */
        ListNode<T> *GetNode(uint32 id) const noexcept;

        T *Get(const uint32 id) const noexcept;

        const T &operator[](const uint32 id) const;

        T &operator[](const uint32 id);

        void RemoveAt(const uint32 id);

        void Remove(const T &elem, const bool all = true);

        void RemoveLast();

        void Sort();

        T *GetFirst() const noexcept;

        T *GetLast() const noexcept;

        uint32 Size() const;

        void Clear();

        void ForEach(const std::function<void(const T &)> &fnc) const;

        /**
         * Returns an iterator to the begining of the list
         */
        inline Iterator Begin() const
        {
            return (Iterator(First));
        }

        /**
         * Returns an iterator to the end of the list
         */
        inline Iterator End() const
        {
            return (Iterator(Last));
        }
    };
};

#include "Framework/List.impl.hpp"
