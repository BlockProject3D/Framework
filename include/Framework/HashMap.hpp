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
#include <initializer_list>
#include "Framework/Types.hpp"
#include "Framework/Iterator.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/Hash.hpp"

namespace bpf
{
    constexpr fint HASH_MAP_INIT_BUF_SIZE = 2;
    constexpr float HASH_MAP_LIMIT_UNTIL_EXTEND = 0.5f;

    template <typename K, typename V, typename HashOp = Hash<K>>
    class BP_TPL_API HashMap
    {
    public:
        struct BP_TPL_API Entry
        {
            K Key;
            V Value;
        };

		enum EntryState
		{
			ENTRY_STATE_INSTANCE_DELETE,
			ENTRY_STATE_NON_EXISTANT,
			ENTRY_STATE_OCCUPIED
		};

        struct BP_TPL_API Data
        {
            fsize Hash;
            EntryState State;
            Entry KeyVal;
        };

        class BP_TPL_API Iterator : public IIterator<typename HashMap<K, V, HashOp>::Iterator, Entry>
        {
        protected:
            Data *_data;
            fsize MaxSize;
			fsize MinSize;
            fsize CurID;
            void SearchNextEntry();
            void SearchPrevEntry();

        public:
			Iterator(Data *data, fsize start, fsize size, const bool reverse = false);
            Iterator &operator++();
			Iterator &operator--();
            inline const Entry &operator*() const
            {
                return (_data[CurID].KeyVal);
            }
            inline const Entry *operator->() const
            {
                return (&_data[CurID].KeyVal);
            }
            inline bool operator==(const Iterator &other) const
            {
                return (CurID == other.CurID);
            }
            inline bool operator!=(const Iterator &other) const
            {
                return (CurID != other.CurID);
            }

			friend class HashMap<K, V, HashOp>;
        };

        class BP_TPL_API ReverseIterator final : public Iterator
        {
        public:
            inline ReverseIterator(Data *data, fsize start, fsize size)
                : Iterator(data, start, size, true)
            {
            }
			ReverseIterator &operator++();
			ReverseIterator &operator--();

			friend class HashMap<K, V, HashOp>;
        };

    private:
        Data *_data;
        fsize CurSize;
        fsize ElemCount;

        void TryExtend(); //Checks and extends the hash table by the multiplier
        fsize QuadraticSearch(fsize hkey) const;
        fsize QuadraticInsert(fsize hkey);

    public:
        HashMap();
		HashMap(const HashMap &other);
		HashMap(HashMap &&other);
		HashMap(const std::initializer_list<Entry> &entries);
        ~HashMap();
      
        /**
         * Adds a new element in this hash map, replaces if key already exists
         * @param key the key of the element
         * @param value the value to insert
         */
        void Add(const K &key, const V &value);

        void Add(const K &key, V &&value);
      
        /**
         * Removes an element from the hash table
         * @param key the key of the element to remove
         */
        void RemoveAt(const K &key);
		void RemoveAt(Iterator &pos);
		void RemoveAt(Iterator &&pos);

		void Swap(const Iterator &a, const Iterator &b);

		void Clear();

		/**
		 * Removes an element by value
		 * @param value the value to search for
		 * @param all wether to remove all occurances or just the first one
		 */
		void Remove(const V &value, const bool all = true);

        /**
         * Returns the element at the specified key, if no key exists in this hash table, throws
         * @param key the key of the element to return
         */
        const V &operator[](const K &key) const;
        
        V &operator[](const K &key);

		HashMap &operator=(const HashMap &other);
		HashMap &operator=(HashMap &&other);
      
        /**
         * Returns true if the specified key exists, false otherwise
         * @param key the key to check
         */
        bool HasKey(const K &key) const;
      
        /**
         * Returns the size of this hash table, that means the element count
         */
        inline fsize Size() const
        {
            return (ElemCount);
        }
        
        inline Iterator begin() const
        {
            return (Iterator(_data, 0, CurSize));
        }
        inline Iterator end() const
        {
            return (Iterator(_data, CurSize, CurSize));
        }

        inline ReverseIterator rbegin() const
        {
            return (ReverseIterator(_data, CurSize - 1, CurSize));
        }
        inline ReverseIterator rend() const
        {
            return (ReverseIterator(_data, (fsize)-1, CurSize));
        }
    };
};

#include "Framework/HashMap.impl.hpp"
