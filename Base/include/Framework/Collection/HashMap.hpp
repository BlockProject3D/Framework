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
#include "Framework/Collection/HashMap.Iterator.hpp"
#include "Framework/Collection/Utility.hpp"
#include "Framework/Hash.Base.hpp"
#include "Framework/Hash.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/Types.hpp"
#include <functional>
#include <initializer_list>

namespace bpf
{
    namespace collection
    {
        constexpr fint HASH_MAP_INIT_BUF_SIZE = 2;
        constexpr float HASH_MAP_LIMIT_UNTIL_EXTEND = 0.5f;

        /**
         * Hash table using open-hashing algorithm
         * @tparam K the key type
         * @tparam V the value type
         * @tparam HashOp the hash operator
         */
        template <typename K, typename V, typename HashOp = Hash<K>>
        class BP_TPL_API HashMap
        {
        private:
            struct Entry
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
            struct Node
            {
                fsize Hash;
                EntryState State;
                Entry KeyVal;
            };

        public:
            using Iterator = HashMapIterator<HashMap<K, V, HashOp>, Entry, Node>;
            using CIterator = HashMapConstIterator<HashMap<K, V, HashOp>, Entry, Node>;
            using ReverseIterator = HashMapReverseIterator<HashMap<K, V, HashOp>, Entry, Node>;
            using CReverseIterator = HashMapConstReverseIterator<HashMap<K, V, HashOp>, Entry, Node>;

            friend Iterator;
            friend CIterator;
            friend ReverseIterator;
            friend CReverseIterator;

        private:
            Node *_data;
            fsize CurSize;
            fsize ElemCount;

            void TryExtend(); //Checks and extends the hash table by the multiplier
            fsize QuadraticSearch(fsize hkey) const;
            fsize QuadraticInsert(fsize hkey);

        public:
            /**
             * Constructs an empty HashMap
             */
            HashMap();

            /**
             * Copy constructor
             */
            HashMap(const HashMap &other);

            /**
             * Move constructor
             */
            HashMap(HashMap &&other) noexcept;

            /**
             * Constructs a HashMap from an existing initializer list
             * @param entries the initial list of key-value pairs to add to this new HashMap
             */
            HashMap(const std::initializer_list<Entry> &entries);
            ~HashMap();

            /**
             * Adds a new element in this hash map, replaces if key already exists
             * @param key the key of the element
             * @param value the value to insert
             */
            void Add(const K &key, const V &value);

            /**
             * Adds a new element in this hash map, replaces if key already exists
             * @param key the key of the element
             * @param value the value to insert
             */
            void Add(const K &key, V &&value);

            /**
             * Removes an element from the hash table
             * @param key the key of the element to remove
             */
            void RemoveAt(const K &key);

            /**
             * Removes an element from the hash table
             * @param pos iterator of the element to remove
             */
            void RemoveAt(Iterator &pos);

            /**
             * Removes an element from the hash table
             * @param pos iterator of the element to remove
             */
            void RemoveAt(Iterator &&pos);

            /**
             * Swap two elements by iterator in the HashMap
             * @param a first element
             * @param b second element
             */
            void Swap(const Iterator &a, const Iterator &b);

            /**
             * Clears the content of this HashMap
             * WARNING: Does not automatically deallocate existing items in this map
             */
            void Clear();

            /**
             * Removes an element by value
             * @param value the value to search for
             * @param all wether to remove all occurances or just the first one
             * @tparam Comparator the comparision operator to use for comparing values
             */
            template <template <typename> class Comparator = ops::Equal>
            void Remove(const V &value, bool all = true);

            /**
             * Compare HashMap by performing a per-element check
             * @param other HashMap to compare with
             * @return true if the two maps are equal, false otherwise
             */
            bool operator==(const HashMap<K, V, HashOp> &other);

            /**
             * Compare HashMap by performing a per-element check
             * @param other HashMap to compare with
             * @return false if the two maps are equal, true otherwise
             */
            inline bool operator!=(const HashMap<K, V, HashOp> &other)
            {
                return (!operator==(other));
            }

            /**
             * Locate an item by key inside this map
             * @param key the key of the item to search for
             * @return iterator to the found item or end() if none
             */
            Iterator FindByKey(const K &key);

            /**
             * Locate an item by performing per-element check
             * @tparam Comparator comparision operator to use
             * @param val the value to search for
             * @return iterator to the found item or end() if none
             */
            template <template <typename> class Comparator = ops::Equal>
            Iterator FindByValue(const V &val);

            /**
             * Locate an item by performing per-element check
             * @param comparator the comparision function to use
             * @return iterator to the found item or end() if none
             */
            Iterator Find(const std::function<bool(Iterator it)> &comparator);

            /**
             * Returns an element const mode
             * @param key the key of the element
             * @throw IndexException if key is not in this map
             * @return immutable item
             */
            const V &operator[](const K &key) const;

            /**
             * Returns an element non-const mode
             * @param key the key of the element
             * @throw IndexException if key is not in this map and cannot be created
             * @return mutable item
             */
            V &operator[](const K &key);

            /**
             * Copy assignment operator
             */
            HashMap &operator=(const HashMap &other);

            /**
             * Move assignment operator
             */
            HashMap &operator=(HashMap &&other) noexcept;

            /**
             * Create a new HashMap from concatenation of two maps
             * @param other map to concatenate with
             * @return new HashMap
             */
            HashMap operator+(const HashMap &other) const;

            /**
             * Appends the content of a HashMap at the end of this map
             * @param other map to append
             */
            void operator+=(const HashMap &other);

            /**
             * Check if a particular key exists
             * @param key the key to check
             * @return true if the specified key exists, false otherwise
             */
            bool HasKey(const K &key) const;

            /**
             * Returns the number of items in this map
             * @return number of items as unsigned
             */
            inline fsize Size() const
            {
                return (ElemCount);
            }

            /**
             * Returns an iterator to the begining of the collection
             * @return new iterator
             */
            inline CIterator begin() const
            {
                return (CIterator(_data, 0, CurSize));
            }

            /**
             * Returns an iterator to the end of the collection
             * @return new iterator
             */
            inline CIterator end() const
            {
                return (CIterator(_data, CurSize, CurSize));
            }

            /**
             * Returns an iterator to the begining of the collection
             * @return new iterator
             */
            inline Iterator begin()
            {
                return (Iterator(_data, 0, CurSize));
            }

            /**
             * Returns an iterator to the end of the collection
             * @return new iterator
             */
            inline Iterator end()
            {
                return (Iterator(_data, CurSize, CurSize));
            }

            /**
             * Returns a reverse iterator to the begining of the collection
             * @return new iterator
             */
            inline CReverseIterator rbegin() const
            {
                return (CReverseIterator(_data, CurSize - 1, CurSize));
            }

            /**
             * Returns a reverse iterator to the end of the collection
             * @return new iterator
             */
            inline CReverseIterator rend() const
            {
                return (CReverseIterator(_data, (fsize)-1, CurSize));
            }

            /**
             * Returns a reverse iterator to the begining of the collection
             * @return new iterator
             */
            inline ReverseIterator rbegin()
            {
                return (ReverseIterator(_data, CurSize - 1, CurSize));
            }

            /**
             * Returns a reverse iterator to the end of the collection
             * @return new iterator
             */
            inline ReverseIterator rend()
            {
                return (ReverseIterator(_data, (fsize)-1, CurSize));
            }
        };
    }
}

#include "Framework/Collection/HashMap.impl.hpp"
