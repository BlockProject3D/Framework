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
    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::Iterator::SearchNextEntry()
    {
        while (CurID < MaxSize && _data[CurID].Empty)
            ++CurID;
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::Iterator::SearchPrevEntry()
    {
        while (CurID != (fsize)-1 && _data[CurID].Empty)
            --CurID;
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::ReverseIterator::operator++()
    {
        if (Iterator::CurID != (fsize)-1)
            --Iterator::CurID;
        Iterator::SearchPrevEntry();
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::ReverseIterator::operator--()
    {
        if (Iterator::CurID < Iterator::MaxSize)
            ++Iterator::CurID;
        Iterator::SearchNextEntry();
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::Iterator::operator++()
    {
        if (CurID < MaxSize)
            ++CurID;
        SearchNextEntry();
    }
    
    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::Iterator::operator--()
    {
        if (CurID > 0)
            --CurID;
        SearchPrevEntry();
    }

    template <typename K, typename V, typename HashOp>
    Map<K, V, HashOp>::Map()
        : _data(new Data[MAP_INIT_BUF_SIZE])
        , CurSize(MAP_INIT_BUF_SIZE)
        , ElemCount(0)
    {
        _data[0].Empty = true;
        _data[1].Empty = true;
    }

    template <typename K, typename V, typename HashOp>
    Map<K, V, HashOp>::~Map()
    {
        delete[] _data;
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::TryExtend()
    {
        if ((float)ElemCount / (float)CurSize >= MAP_LIMIT_UNTIL_EXTEND)
        {
            Data *olddata = _data;
            CurSize <<= 1;
            _data = new Data[CurSize];
            for (fsize i = 0 ; i < CurSize; ++i)
                _data[i].Empty = true;
            for (fsize i = 0 ; i < CurSize >> 1 ; ++i)
            {
                if (!olddata[i].Empty)
                {
                    fsize id = QuadraticInsert(olddata[i].Hash);
                    if (id != (fsize)-1)
                    {
		        _data[id].KeyVal.Key = std::move(olddata[i].KeyVal.Key);
                        _data[id].KeyVal.Value = std::move(olddata[i].KeyVal.Value);
                    }
                }
            }
            delete[] olddata;
        }
    }

    template <typename K, typename V, typename HashOp>
    fsize Map<K, V, HashOp>::QuadraticSearch(fsize hkey) const
    {
        for (fsize i = 0 ; i < CurSize ; ++i)
        {
            fsize index = (hkey + ((i * i + i) / 2)) % CurSize;
            if (!_data[index].Empty && _data[index].Hash == hkey)
                return (index);
        }
        return ((fsize)-1);
    }

    template <typename K, typename V, typename HashOp>
    fsize Map<K, V, HashOp>::QuadraticInsert(fsize hkey)
    {
        for (fsize i = 0 ; i < CurSize ; ++i)
        {
            fsize index = (hkey + ((i * i + i) / 2)) % CurSize;
            if (_data[index].Empty)
            {
                _data[index].Hash = hkey;
                _data[index].Empty = false;
                ++ElemCount;
                return ((int)index);
            }
        }
        return ((fsize)-1);
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::Add(const K &key, const V &value)
    {
        fsize hkey = HashOp::HashCode(key);

        TryExtend();
        if (QuadraticSearch(hkey) != (fsize)-1)
            Remove(key);
        fsize idx = QuadraticInsert(hkey);
        if (idx != (fsize)-1)
        {
            _data[idx].KeyVal.Value = value;
            _data[idx].KeyVal.Key = key;
        }
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::Add(const K &key, V &&value)
    {
        fsize hkey = HashOp::HashCode(key);

        TryExtend();
        if (QuadraticSearch(hkey) != (fsize)-1)
            Remove(key);
        fsize idx = QuadraticInsert(hkey);
        if (idx != (fsize)-1)
        {
            _data[idx].KeyVal.Value = std::move(value);
            _data[idx].KeyVal.Key = key;
        }
    }

    template <typename K, typename V, typename HashOp>
    void Map<K, V, HashOp>::Remove(const K &key)
    {
        fsize idx = QuadraticSearch(HashOp::HashCode(key));

        if (idx != (fsize)-1)
        {
            --ElemCount;
            _data[idx].Empty = true;
        }
    }

    template <typename K, typename V, typename HashOp>
    const V &Map<K, V, HashOp>::operator[](const K &key) const
    {
        fsize idx = QuadraticSearch(HashOp::HashCode(key));

        if (idx == (fsize)-1)
            throw bpf::IndexException((fint)idx);
        return (_data[idx].KeyVal.Value);
    }

    template <typename K, typename V, typename HashOp>
    V &Map<K, V, HashOp>::operator[](const K &key)
    {
        fsize idx = QuadraticSearch(HashOp::HashCode(key));

        if (idx == (fsize)-1)
        {
            TryExtend();
            idx = QuadraticInsert(HashOp::HashCode(key));
            if (idx == (fsize)-1)
                throw bpf::IndexException((fint)idx);
            _data[idx].KeyVal.Key = key;
        }
        return (_data[idx].KeyVal.Value);
    }

    template <typename K, typename V, typename HashOp>
    inline bool Map<K, V, HashOp>::HasKey(const K &key) const
    {
        return (QuadraticSearch(HashOp::HashCode(key)) != (fsize)-1);
    }
}
