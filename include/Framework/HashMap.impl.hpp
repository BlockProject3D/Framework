// Copyright (c) 2020, BlockProject
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
    HashMap<K, V, HashOp>::Iterator::Iterator(Node *data, fsize start, fsize size, const bool reverse)
        : _data(data)
        , MaxSize(size)
        , CurID(start)
    {
        if (reverse)
        {
            SearchPrevEntry();
            MaxSize = CurID;
        }
        else
        {
            fsize old = CurID;
            CurID = 0;
            SearchNextEntry();
            MinSize = CurID;
            CurID = old;
            SearchNextEntry();
        }
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::Iterator::SearchNextEntry()
    {
        while (CurID < MaxSize && _data[CurID].State != ENTRY_STATE_OCCUPIED)
            ++CurID;
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::Iterator::SearchPrevEntry()
    {
        while (CurID != (fsize)-1 && _data[CurID].State != ENTRY_STATE_OCCUPIED)
            --CurID;
    }

    template <typename K, typename V, typename HashOp>
    typename HashMap<K, V, HashOp>::ReverseIterator &HashMap<K, V, HashOp>::ReverseIterator::operator++()
    {
        if (Iterator::CurID != (fsize)-1)
        {
            --Iterator::CurID;
            Iterator::SearchPrevEntry();
        }
        return (*this);
    }

    template <typename K, typename V, typename HashOp>
    typename HashMap<K, V, HashOp>::ReverseIterator &HashMap<K, V, HashOp>::ReverseIterator::operator--()
    {
        if (Iterator::CurID < Iterator::MaxSize)
        {
            ++Iterator::CurID;
            Iterator::SearchNextEntry();
        }
        return (*this);
    }

    template <typename K, typename V, typename HashOp>
    typename HashMap<K, V, HashOp>::Iterator &HashMap<K, V, HashOp>::Iterator::operator++()
    {
        if (CurID < MaxSize)
        {
            ++CurID;
            SearchNextEntry();
        }
        return (*this);
    }

    template <typename K, typename V, typename HashOp>
    typename HashMap<K, V, HashOp>::Iterator &HashMap<K, V, HashOp>::Iterator::operator--()
    {
        if (CurID > MinSize)
        {
            --CurID;
            SearchPrevEntry();
        }
        return (*this);
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp>::HashMap()
        : _data(new Node[HASH_MAP_INIT_BUF_SIZE])
        , CurSize(HASH_MAP_INIT_BUF_SIZE)
        , ElemCount(0)
    {
        _data[0].State = ENTRY_STATE_NON_EXISTANT;
        _data[1].State = ENTRY_STATE_NON_EXISTANT;
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp>::HashMap(const std::initializer_list<Entry> &entries)
        : _data(new Node[HASH_MAP_INIT_BUF_SIZE])
        , CurSize(HASH_MAP_INIT_BUF_SIZE)
        , ElemCount(0)
    {
        _data[0].State = ENTRY_STATE_NON_EXISTANT;
        _data[1].State = ENTRY_STATE_NON_EXISTANT;
        for (auto &entry : entries)
            Add(entry.Key, entry.Value);
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp>::HashMap(const HashMap &other)
        : _data(new Node[HASH_MAP_INIT_BUF_SIZE])
        , CurSize(HASH_MAP_INIT_BUF_SIZE)
        , ElemCount(0)
    {
        _data[0].State = ENTRY_STATE_NON_EXISTANT;
        _data[1].State = ENTRY_STATE_NON_EXISTANT;
        for (auto &entry : other)
            Add(entry.Key, entry.Value);
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp> &HashMap<K, V, HashOp>::operator=(const HashMap &other)
    {
        delete[] _data;
        _data = new Node[HASH_MAP_INIT_BUF_SIZE];
        _data[0].State = ENTRY_STATE_NON_EXISTANT;
        _data[1].State = ENTRY_STATE_NON_EXISTANT;
        CurSize = HASH_MAP_INIT_BUF_SIZE;
        ElemCount = 0;
        for (auto &entry : other)
            Add(entry.Key, entry.Value);
        return (*this);
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp> HashMap<K, V, HashOp>::operator+(const HashMap<K, V, HashOp> &other) const
    {
        HashMap<K, V, HashOp> cpy = *this;

        for (const auto &elem : other)
            cpy.Add(elem.Key, elem.Value);
        return (cpy);
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::operator+=(const HashMap<K, V, HashOp> &other)
    {
        for (const auto &elem : other)
            Add(elem.Key, elem.Value);
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp>::HashMap(HashMap &&other)
        : _data(other._data)
        , CurSize(other.CurSize)
        , ElemCount(other.ElemCount)
    {
        other._data = Null;
        other.CurSize = 0;
        other.ElemCount = 0;
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp> &HashMap<K, V, HashOp>::operator=(HashMap &&other)
    {
        delete[] _data;
        _data = other._data;
        CurSize = other.CurSize;
        ElemCount = other.ElemCount;
        other._data = Null;
        other.CurSize = 0;
        other.ElemCount = 0;
        return (*this);
    }

    template <typename K, typename V, typename HashOp>
    HashMap<K, V, HashOp>::~HashMap()
    {
        delete[] _data;
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::TryExtend()
    {
        if ((float)ElemCount / (float)CurSize >= HASH_MAP_LIMIT_UNTIL_EXTEND)
        {
            Node *olddata = _data;
            CurSize <<= 1;
            _data = new Node[CurSize];
            for (fsize i = 0; i < CurSize; ++i)
                _data[i].State = ENTRY_STATE_NON_EXISTANT;
            for (fsize i = 0; i < CurSize >> 1; ++i)
            {
                if (olddata[i].State == ENTRY_STATE_OCCUPIED)
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
    fsize HashMap<K, V, HashOp>::QuadraticSearch(fsize hkey) const
    {
        for (fsize i = 0; i < CurSize; ++i)
        {
            fsize index = (hkey + ((i * i + i) / 2)) % CurSize;
            if (_data[index].State == ENTRY_STATE_NON_EXISTANT)
                break;
            if (_data[index].State == ENTRY_STATE_OCCUPIED && _data[index].Hash == hkey)
                return (index);
        }
        return ((fsize)-1);
    }

    template <typename K, typename V, typename HashOp>
    fsize HashMap<K, V, HashOp>::QuadraticInsert(fsize hkey)
    {
        for (fsize i = 0; i < CurSize; ++i)
        {
            fsize index = (hkey + ((i * i + i) / 2)) % CurSize;
            if (_data[index].State == ENTRY_STATE_NON_EXISTANT
                || _data[index].State == ENTRY_STATE_INSTANCE_DELETE)
            {
                _data[index].Hash = hkey;
                _data[index].State = ENTRY_STATE_OCCUPIED;
                return ((int)index);
            }
        }
        return ((fsize)-1);
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::Add(const K &key, const V &value)
    {
        fsize hkey = HashOp::HashCode(key);

        TryExtend();
        fsize idx = QuadraticSearch(hkey);
        if (idx == (fsize)-1)
        {
            idx = QuadraticInsert(hkey);
            if (idx != (fsize)-1)
                ++ElemCount;
        }
        if (idx != (fsize)-1)
        {
            _data[idx].KeyVal.Value = value;
            _data[idx].KeyVal.Key = key;
        }
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::Add(const K &key, V &&value)
    {
        fsize hkey = HashOp::HashCode(key);

        TryExtend();
        fsize idx = QuadraticSearch(hkey);
        if (idx == (fsize)-1)
        {
            idx = QuadraticInsert(hkey);
            if (idx != (fsize)-1)
                ++ElemCount;
        }
        if (idx != (fsize)-1)
        {
            _data[idx].KeyVal.Value = std::move(value);
            _data[idx].KeyVal.Key = key;
        }
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::RemoveAt(const K &key)
    {
        fsize idx = QuadraticSearch(HashOp::HashCode(key));

        if (idx != (fsize)-1)
        {
            --ElemCount;
            _data[idx].State = ENTRY_STATE_INSTANCE_DELETE;
        }
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::RemoveAt(Iterator &pos)
    {
        auto cur = pos;

        ++pos;
        if (cur.CurID < CurSize && _data[cur.CurID].State == ENTRY_STATE_OCCUPIED)
            RemoveAt(cur->Key);
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::RemoveAt(Iterator &&pos)
    {
        auto cur = pos;

        ++pos;
        if (cur.CurID < CurSize && _data[cur.CurID].State == ENTRY_STATE_OCCUPIED)
            RemoveAt(cur->Key);
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::Swap(const Iterator &a, const Iterator &b)
    {
        if (a.CurID >= CurSize || b.CurID >= CurSize
            || _data[a.CurID].State != ENTRY_STATE_OCCUPIED
            || _data[b.CurID].State != ENTRY_STATE_OCCUPIED)
            return;
        auto v = std::move(this->operator[](a->Key));
        this->operator[](a->Key) = std::move(this->operator[](b->Key));
        this->operator[](b->Key) = std::move(v);
    }

    template <typename K, typename V, typename HashOp>
    void HashMap<K, V, HashOp>::Clear()
    {
        delete[] _data;
        _data = new Node[HASH_MAP_INIT_BUF_SIZE];
        CurSize = HASH_MAP_INIT_BUF_SIZE;
        ElemCount = 0;
        _data[0].State = ENTRY_STATE_NON_EXISTANT;
        _data[1].State = ENTRY_STATE_NON_EXISTANT;
    }

    template <typename K, typename V, typename HashOp>
    template <template <typename> class Comparator>
    void HashMap<K, V, HashOp>::Remove(const V &value, const bool all)
    {
        for (auto &entry : *this)
        {
            if (Comparator<V>::Eval(entry.Value, value))
            {
                RemoveAt(entry.Key);
                if (!all)
                    return;
            }
        }
    }

    template <typename K, typename V, typename HashOp>
    typename HashMap<K, V, HashOp>::Iterator HashMap<K, V, HashOp>::FindByKey(const K &key)
    {
        fsize idx = QuadraticSearch(HashOp::HashCode(key));

        if (idx == (fsize)-1)
            return (Iterator(_data, CurSize, CurSize));
        return (Iterator(_data, idx, CurSize));
    }

    template <typename K, typename V, typename HashOp>
    template <template <typename> class Comparator>
    typename HashMap<K, V, HashOp>::Iterator HashMap<K, V, HashOp>::FindByValue(const V &val)
    {
        for (auto it = begin() ; it != end() ; ++it)
        {
            if (Comparator<V>::Eval(it->Value, val))
                return (it);
        }
        return (Iterator(_data, CurSize, CurSize));
    }

    template <typename K, typename V, typename HashOp>
    typename HashMap<K, V, HashOp>::Iterator HashMap<K, V, HashOp>::Find(const std::function<bool(Iterator it)> &comparator)
    {
        for (auto it = begin(); it != end(); ++it)
        {
            if (comparator(it))
                return (it);
        }
        return (Iterator(_data, CurSize, CurSize));
    }

    template <typename K, typename V, typename HashOp>
    const V &HashMap<K, V, HashOp>::operator[](const K &key) const
    {
        fsize idx = QuadraticSearch(HashOp::HashCode(key));

        if (idx == (fsize)-1)
            throw bpf::IndexException((fint)idx);
        return (_data[idx].KeyVal.Value);
    }

    template <typename K, typename V, typename HashOp>
    V &HashMap<K, V, HashOp>::operator[](const K &key)
    {
        fsize idx = QuadraticSearch(HashOp::HashCode(key));

        if (idx == (fsize)-1)
        {
            TryExtend();
            idx = QuadraticInsert(HashOp::HashCode(key));
            if (idx == (fsize)-1)
                throw bpf::IndexException((fint)idx);
            _data[idx].KeyVal.Key = key;
            ++ElemCount;
        }
        return (_data[idx].KeyVal.Value);
    }

    template <typename K, typename V, typename HashOp>
    inline bool HashMap<K, V, HashOp>::HasKey(const K &key) const
    {
        return (QuadraticSearch(HashOp::HashCode(key)) != (fsize)-1);
    }
}
