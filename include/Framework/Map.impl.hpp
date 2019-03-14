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

namespace bpf
{
    template <typename K, typename V>
    void Map<K, V>::Iterator::SearchNextEntry()
    {
        while (CurID < MaxSize && EmptyKeys[CurID])
            ++CurID;
        if (CurID < MaxSize && !EmptyKeys[CurID])
        {
            Entry.Key = KeyData[CurID];
            Entry.Value = Data[CurID];
        }
    }

    template <typename K, typename V>
    void Map<K, V>::Iterator::SearchPrevEntry()
    {
        while (CurID != (fsize)-1 && EmptyKeys[CurID])
            --CurID;
        if (CurID != (fsize)-1 && !EmptyKeys[CurID])
        {
            Entry.Key = KeyData[CurID];
            Entry.Value = Data[CurID];
        }
    }

    template <typename K, typename V>
    void Map<K, V>::ReverseIterator::operator++()
    {
        if (Iterator::CurID != (fsize)-1)
            --Iterator::CurID;
        Iterator::SearchPrevEntry();
    }

    template <typename K, typename V>
    void Map<K, V>::ReverseIterator::operator--()
    {
        if (Iterator::CurID < Iterator::MaxSize)
            ++Iterator::CurID;
        Iterator::SearchNextEntry();
    }

    template <typename K, typename V>
    void Map<K, V>::Iterator::operator++()
    {
        if (CurID < MaxSize)
            ++CurID;
        SearchNextEntry();
    }
    
    template <typename K, typename V>
    void Map<K, V>::Iterator::operator--()
    {
        if (CurID > 0)
            --CurID;
        SearchPrevEntry();
    }

    template <typename K, typename V>
    Map<K, V>::Map()
        : Data(new V[MAP_INIT_BUF_SIZE])
        , KeyData(new K[MAP_INIT_BUF_SIZE])
        , HashTable(new fsize[MAP_INIT_BUF_SIZE])
        , EmptyKeys(new bool[MAP_INIT_BUF_SIZE])
        , CurSize(MAP_INIT_BUF_SIZE), ElemCount(0)
    {
        EmptyKeys[0] = true;
        EmptyKeys[1] = true;
    }

    template <typename K, typename V>
    Map<K, V>::~Map()
    {
        delete[] Data;
        delete[] KeyData;
        delete[] HashTable;
        delete[] EmptyKeys;
    }

    template <typename K, typename V>
    void Map<K, V>::TryExtend()
    {
        if ((float)ElemCount / (float)CurSize >= MAP_LIMIT_UNTIL_EXTEND)
        {
            V *olddata = Data;
            K *oldkeydata = KeyData;
            fsize *oldhash = HashTable;
            bool *oldempty = EmptyKeys;
            CurSize <<= 1;
            EmptyKeys = new bool[CurSize];
            for (fsize i = 0 ; i < CurSize; ++i)
                EmptyKeys[i] = true;
            Data = new V[CurSize];
            KeyData = new K[CurSize];
            HashTable = new fsize[CurSize];
            for (fsize i = 0 ; i < CurSize >> 1 ; ++i)
            {
                if (!oldempty[i])
                {
                    fsize id = QuadraticInsert(oldhash[i]);
                    if (id != (fsize)-1)
                    {
		        Data[id] = std::move(olddata[i]);
		        KeyData[id] = std::move(oldkeydata[i]);
                    }
                }
            }
            delete[] olddata;
            delete[] oldkeydata;
            delete[] oldhash;
            delete[] oldempty;
        }
    }

    template <typename K, typename V>
    fsize Map<K, V>::QuadraticSearch(fsize hkey) const
    {
        for (fsize i = 0 ; i < CurSize ; ++i)
        {
            fsize index = (hkey + ((i * i + i) / 2)) % CurSize;
            if (!EmptyKeys[index] && HashTable[index] == hkey)
                return (index);
        }
        return ((fsize)-1);
    }

    template <typename K, typename V>
    fsize Map<K, V>::QuadraticInsert(fsize hkey)
    {
        for (fsize i = 0 ; i < CurSize ; ++i)
        {
            fsize index = (hkey + ((i * i + i) / 2)) % CurSize;
            if (EmptyKeys[index])
            {
                HashTable[index] = hkey;
                EmptyKeys[index] = false;
                ++ElemCount;
                return ((int)index);
            }
        }
        return ((fsize)-1);
    }

    template <typename K, typename V>
    void Map<K, V>::Add(const K &key, const V &value)
    {
        fsize hkey = Hash(key);

        TryExtend();
        if (QuadraticSearch(hkey) != (fsize)-1)
            Remove(key);
        fsize idx = QuadraticInsert(hkey);
        if (idx != (fsize)-1)
        {
            Data[idx] = value;
            KeyData[idx] = key;
        }
    }

    template <typename K, typename V>
    void Map<K, V>::Remove(const K &key)
    {
        fsize idx = QuadraticSearch(Hash(key));

        if (idx != (fsize)-1)
        {
            --ElemCount;
            EmptyKeys[idx] = true;
        }
    }

    template <typename K, typename V>
    const V &Map<K, V>::operator[](const K &key) const
    {
        fsize idx = QuadraticSearch(Hash(key));

        if (idx == (fsize)-1)
            throw bpf::IndexException((fint)idx);
        return (Data[idx]);
    }

    template <typename K, typename V>
    V &Map<K, V>::operator[](const K &key)
    {
        fsize idx = QuadraticSearch(Hash(key));

        if (idx == (fsize)-1)
        {
            TryExtend();
            idx = QuadraticInsert(Hash(key));
            if (idx == (fsize)-1)
                throw bpf::IndexException((fint)idx);
            KeyData[idx] = key;
        }
        return (Data[idx]);
    }

    template <typename K, typename V>
    inline bool Map<K, V>::HasKey(const K &key) const
    {
        return (QuadraticSearch(Hash(key)) != (fsize)-1);
    }
}
