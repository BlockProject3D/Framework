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
#include "Framework/Types.hpp"
#include "Framework/Iterator.hpp"
#include "Framework/IndexException.hpp"

//TODO : Use bpf::fsize for sizes
namespace bpf
{
    constexpr int MAP_INIT_BUF_SIZE = 2;
    constexpr float MAP_LIMIT_UNTIL_EXTEND = 0.5f;

    template<typename K, typename V>
    struct BP_TPL_API MapEntry
    {
        K Key;
        V Value;
    };

    template <typename K, typename V>
    class BP_TPL_API Map
    {
    public:
        class BP_TPL_API Iterator final : public IIterator<typename Map<K, V>::Iterator, MapEntry<K, V>>
        {
        private:
            V *Data;
            K *KeyData;
            bool *EmptyKeys;
            uint32 MaxSize;
            uint32 CurID;
            MapEntry<K, V> Entry;

        public:
            inline Iterator(V *data, K *keydata, bool *empty, uint32 start, uint32 size)
                : Data(data), KeyData(keydata), EmptyKeys(empty), MaxSize(size), CurID(start)
            {
                if (start == 0)
                    operator++();
                else
                    operator--();
            }
            void operator++();
            void operator--();
            inline const MapEntry<K, V> &operator*() const
            {
                return (Entry);
            }
            inline const MapEntry<K, V> &operator->() const
            {
                return (Entry);
            }
            inline operator bool() const
            {
                return (CurID != 0 && CurID != MaxSize);
            }
            inline bool operator==(const Iterator &other) const
            {
                return (CurID == other.CurID);
            }
            inline bool operator!=(const Iterator &other) const
            {
                return (CurID != other.CurID);
            }
        };

    private:
        V *Data;
        K *KeyData;
        uint32 *HashTable;
        bool *EmptyKeys;
        uint32 CurSize;
        uint32 ElemCount;

        void TryExtend(); //Checks and extends the hash table by the multiplier
        int QuadraticSearch(uint32 hkey) const;
        int QuadraticInsert(uint32 hkey);

    public:
        Map();
        Map(const Map &other);
        Map(Map &&other);
        ~Map();
      
        Map &operator=(const Map &other);
        Map &operator=(Map &&other);
        /**
         * Adds a new element in this hash map, replaces if key already exists
         * @param key the key of the element
         * @param value the value to insert
         */
        void Add(const K &key, const V &value);
      
        /**
         * Removes an element from the hash table
         * @param key the key of the element to remove
         */
        void Remove(const K &key);
      
        /**
         * Returns the element at the specified key, if no key exists in this hash table, throws
         * @param key the key of the element to return
         */
        const V &operator[](const K &key) const;
        
        V &operator[](const K &key);
      
        /**
         * Returns true if the specified key exists, false otherwise
         * @param key the key to check
         */
        bool HasKey(const K &key) const;
      
        /**
         * Returns the size of this hash table, that means the element count
         */
        inline uint32 Size() const
        {
            return (ElemCount);
        }
        
        inline Iterator Begin() const
        {
            return (Iterator(Data, KeyData, EmptyKeys, 0, CurSize));
        }
        inline Iterator End() const
        {
            return (Iterator(Data, KeyData, EmptyKeys, CurSize - 1, CurSize));
        }
    };
};

#include "Framework/Map.impl.hpp"
