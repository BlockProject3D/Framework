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
#include "Framework/Collection/Stack.hpp"
#include "Framework/Collection/Utility.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/Types.hpp"
#include <functional>
#include <initializer_list>

namespace bpf
{
    namespace collection
    {
        /**
         * AVL tree based map
         * @tparam K the key type
         * @tparam V the value type
         * @tparam Greater the greater than operator
         * @tparam Less the less than operator
         */
        template <typename K, typename V, template <typename T> class Greater = ops::Greater, template <typename T> class Less = ops::Less>
        class BP_TPL_API Map
        {
        public:
            struct Entry
            {
                K Key;
                V Value;
            };

            struct Node
            {
                Node *Left;
                Node *Right;
                Node *Parent;
                Entry KeyVal;
                fisize Height;
            };

        public:
            class BP_TPL_API Iterator : public IIterator<typename Map<K, V, Greater, Less>::Iterator, Entry>
            {
            private:
                Node *_root;
                Node *_fixedRoot;
                Node *_curNode;
                Stack<Node *> _stack;
                Stack<Node *> _backStack;
                void ResetIterator();

            public:
                Iterator(Node *root, Node *start);
                Iterator &operator++();
                Iterator &operator--();
                inline const Entry &operator*() const
                {
                    return (_curNode->KeyVal);
                }
                inline const Entry *operator->() const
                {
                    return (&_curNode->KeyVal);
                }
                inline bool operator==(const Iterator &other) const
                {
                    return (_curNode == other._curNode);
                }
                inline bool operator!=(const Iterator &other) const
                {
                    return (_curNode != other._curNode);
                }

                friend class Map<K, V, Greater, Less>;
            };

            class BP_TPL_API ReverseIterator : public IIterator<typename Map<K, V, Greater, Less>::ReverseIterator, Entry>
            {
            private:
                Node *_root;
                Node *_fixedRoot;
                Node *_curNode;
                Stack<Node *> _stack;
                Stack<Node *> _backStack;
                void ResetIterator();

            public:
                ReverseIterator(Node *root, Node *start);
                ReverseIterator &operator++();
                ReverseIterator &operator--();
                inline const Entry &operator*() const
                {
                    return (_curNode->KeyVal);
                }
                inline const Entry *operator->() const
                {
                    return (&_curNode->KeyVal);
                }
                inline bool operator==(const ReverseIterator &other) const
                {
                    return (_curNode == other._curNode);
                }
                inline bool operator!=(const ReverseIterator &other) const
                {
                    return (_curNode != other._curNode);
                }
            };

        private:
            Node *_root;
            fsize _count;
            fisize Height(Node *node);
            fisize Balance(Node *node);
            void LeftRotate(Node *node);
            void RightRotate(Node *node);
            Node *InsertNode(const K &key);
            void RemoveNode(Node *node);
            Node *FindMin(Node *node);
            void SwapKeyVal(Node *a, Node *b);
            void SwapVal(Node *a, Node *b);
            Node *FindNode(const K &key) const;

        public:
            /**
             * Constructs an empty Map
             */
            Map();

            /**
             * Copy constructor
             */
            Map(const Map &other);

            /**
             * Move constructor
             */
            Map(Map &&other) noexcept;

            /**
             * Constructs a HashMap from an existing initializer list
             * @param entries the initial list of key-value pairs to add to this new HashMap
             */
            Map(const std::initializer_list<Entry> &entries);

            ~Map();

            /**
             * Adds a new element in this map, replaces if key already exists
             * @param key the key of the element
             * @param value the value to insert
             */
            void Add(const K &key, const V &value);

            /**
             * Adds a new element in this map, replaces if key already exists
             * @param key the key of the element
             * @param value the value to insert
             */
            void Add(const K &key, V &&value);

            /**
             * Removes an element from the map
             * @param key the key of the element to remove
             */
            void RemoveAt(const K &key);

            /**
             * Removes an element from the map
             * @param pos iterator of the element to remove
             */
            void RemoveAt(Iterator &pos);

            /**
             * Removes an element from the map
             * @param pos iterator of the element to remove
             */
            void RemoveAt(Iterator &&pos);

            /**
             * Swap two elements by iterator in the Map
             * @param a first element
             * @param b second element
             */
            void Swap(const Iterator &a, const Iterator &b);

            /**
             * Clears the content of this Map
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
             * Compare Map by performing a per-element check
             * @param other Map to compare with
             * @return true if the two maps are equal, false otherwise
             */
            bool operator==(const Map<K, V, Greater, Less> &other);

            /**
             * Compare Map by performing a per-element check
             * @param other Map to compare with
             * @return false if the two maps are equal, true otherwise
             */
            inline bool operator!=(const Map<K, V, Greater, Less> &other)
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
             * @param comparator the comparision function to use: return Z- for less than, Z+ for greater than and 0 for equal
             * @return iterator to the found item or end() if none
             */
            Iterator Find(const std::function<int(const Node &node)> &comparator);

            /**
             * Returns an element const mode
             * @param key the key of the element
             * @throw IndexException if key is not in this map
             * @return immutable item
             */
            const V &operator[](const K &key) const;

            /**
             * Returns the element with the minimum key value
             * @return iterator to the minimum element
             */
            Iterator FindMin();

            /**
             * Returns the element with the maximum key value
             * @return iterator to the maximum element
             */
            Iterator FindMax();

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
            Map &operator=(const Map &other);

            /**
             * Move assignment operator
             */
            Map &operator=(Map &&other) noexcept;

            /**
             * Create a new Map from concatenation of two maps
             * @param other map to concatenate with
             * @return new Map
             */
            Map operator+(const Map &other) const;

            /**
             * Appends the content of a Map at the end of this map
             * @param other map to append
             */
            void operator+=(const Map &other);

            /**
             * Check if a particular key exists
             * @param key the key to check
             * @return true if the specified key exists, false otherwise
             */
            inline bool HasKey(const K &key) const
            {
                return (FindNode(key) != Null);
            }

            /**
             * Returns the number of items in this map
             * @return number of items as unsigned
             */
            inline fsize Size() const
            {
                return (_count);
            }

            /**
             * Returns an iterator to the begining of the collection
             * @return new iterator
             */
            inline Iterator begin() const
            {
                return (Iterator(_root, reinterpret_cast<Node *>(1)));
            }

            /**
             * Returns an iterator to the end of the collection
             * @return new iterator
             */
            inline Iterator end() const
            {
                return (Iterator(_root, Null));
            }

            /**
             * Returns a reverse iterator to the begining of the collection
             * @return new iterator
             */
            inline ReverseIterator rbegin() const
            {
                return (ReverseIterator(_root, reinterpret_cast<Node *>(1)));
            }

            /**
             * Returns a reverse iterator to the end of the collection
             * @return new iterator
             */
            inline ReverseIterator rend() const
            {
                return (ReverseIterator(_root, Null));
            }
        };
    }
}

#include "Framework/Collection/Map.impl.hpp"