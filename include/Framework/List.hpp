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
#include "Framework/ContainerUtilities.hpp"

//TODO: Finish ALL TESTS for List and then copy these tests for ArrayList and copy relevant tests to other containers

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
            ListNode<T> *_cur;
			ListNode<T> *_reset;

        public:
            inline Iterator(ListNode<T> *start, ListNode<T> *reset)
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
			ListNode<T> *_cur;
			ListNode<T> *_reset;

		public:
			inline ReverseIterator(ListNode<T> *start, ListNode<T> *reset)
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
        ListNode<T> *_first;
        ListNode<T> *_last;
        fsize _count;

        ListNode<T> *Partition(ListNode<T> *start, ListNode<T> *end);
        void QuickSort(ListNode<T> *start, ListNode<T> *end);
		void MergeSort();
        void RemoveNode(ListNode<T> *toRM);
		void Swap(ListNode<T>* a, ListNode<T>* b);
		ListNode<T>* GetNode(fsize id) const;
		ListNode<T>* FirstNode() const;
		ListNode<T>* LastNode() const;
    public:
        List<T>();
		List<T>(const std::initializer_list<T> &lst);
		List<T>(const List<T> &other);
        List<T>(List<T> &&other);
        ~List<T>();

		List<T> &operator=(List<T> &&other);
		List<T> &operator=(const List<T> &other);

        /**
         * Adds an element at the end of the list
         * @param elem element to add
         */
        void Add(const T &elem);
        void Add(T &&elem);

        /**
         * Inserts an element at a given position
         * @param elem element to insert
         * @param pos position
         */
		void Insert(fsize pos, const T &elem);
		void Insert(fsize pos, T &&elem);
		void Insert(const Iterator &pos, const T &elem);
		void Insert(const Iterator &pos, T &&elem);

        /**
         * Returns an element
         * @param id index of the element
		 * @throws IndexException if the position is outside the bounds of the list
         */
        const T &operator[](const fsize id) const;

        /**
		 * Removes an element at specified position in the list
		 * @param pos the position of the element
		 */
		void RemoveAt(const fsize pos);
		void RemoveAt(Iterator &pos);
		void RemoveAt(Iterator &&pos);

		/**
		 * Removes occurences of an element fron the list
		 * @param elem the element to search for
		 * @param all wether or not to remove all occurences or just the first one
         * @tparam Equal the equal operator to use for comparing values
		 */
        template <template <typename> typename Equal = bpf::ops::Equal>
        void Remove(const T &elem, const bool all = true);

		void Swap(const Iterator &a, const Iterator &b);

        void RemoveLast();

        void Sort(const bool stable = false);

		inline T &First()
		{
			if (FirstNode() == Null)
				throw IndexException(0);
			return (FirstNode()->Data);
		}

		inline T &Last()
		{
			if (LastNode() == Null)
				throw IndexException(0);
			return (LastNode()->Data);
		}

        inline const T &First() const
        {
            if (FirstNode() == Null)
                throw IndexException(0);
            return (FirstNode()->Data);
        }

        inline const T &Last() const
        {
            if (LastNode() == Null)
                throw IndexException(0);
            return (LastNode()->Data);
        }

        fsize Size() const;

        void Clear();

        /**
         * Returns an iterator to the begining of the list
         */
        inline Iterator begin() const
        {
            return (Iterator(_first, _last));
        }

        /**
         * Returns an iterator to the end of the list
         */
        inline Iterator end() const
        {
            return (Iterator(Null, _last));
        }

        /**
         * Returns a reverse iterator to the begining of the list
         */
        inline ReverseIterator rbegin() const
        {
            return (ReverseIterator(_last, _first));
        }

        /**
         * Returns a reverse iterator to the end of the list
         */
        inline ReverseIterator rend() const
        {
            return (ReverseIterator(Null, _first));
        }
    };
};

#include "Framework/List.impl.hpp"
