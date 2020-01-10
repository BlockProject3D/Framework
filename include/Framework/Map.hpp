// Copyright (c) 2019, BlockProject
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
#include "Framework/Stack.hpp"
#include "Framework/Iterator.hpp"

namespace bpf
{
    template <typename K, typename V, template <typename T> typename Greater = >
    class BP_TPL_API Map
    {
    public:
        struct BP_TPL_API Entry
        {
            K Key;
            V Value;
        };

    private:
        struct BP_TPL_API Node
        {
            Node *Left;
            Node *Right;
            Node *Parent;
            Entry KeyVal;
            fisize Height;
        };

    public:
        class BP_TPL_API Iterator : public IIterator<typename Map<K, V>::Iterator, Entry>
        {
        private:
            Node *_root;
            Node *_fixedRoot;
            Node *_curNode;
            Stack<Node *> _stack;
            Stack<Node *> _backStack;
            void ResetIterator();

        public:
            Iterator(Node *root, bool begin);
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
        };

        class BP_TPL_API ReverseIterator : public IIterator<typename Map<K, V>::Iterator, Entry>
        {
        private:
            Node *_root;
            Node *_fixedRoot;
            Node *_curNode;
            Stack<Node *> _stack;
            Stack<Node *> _backStack;
            void ResetIterator();

        public:
            ReverseIterator(Node *root, bool begin);
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
            inline bool operator==(const Iterator &other) const
            {
                return (_curNode == other._curNode);
            }
            inline bool operator!=(const Iterator &other) const
            {
                return (_curNode != other._curNode);
            }
        };

    private:
        Node *_root;
        fsize _count;

    public:
        Map();
        Map(const Map &other);
        Map(Map &&other);
        Map(const std::initializer_list<Entry> &entries);
        ~Map();

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

        Map &operator=(const Map &other);
        Map &operator=(Map &&other);

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
            return (_count);
        }

        inline Iterator begin() const
        {
            return (Iterator(_root, true));
        }
        inline Iterator end() const
        {
            return (Iterator(_root, false));
        }

        inline ReverseIterator rbegin() const
        {
            return (ReverseIterator(_root, true));
        }
        inline ReverseIterator rend() const
        {
            return (ReverseIterator(_root, false));
        }
    };
}

#include "Framework/Map.impl.hpp"