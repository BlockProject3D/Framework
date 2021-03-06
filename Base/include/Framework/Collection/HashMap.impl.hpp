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
#include "Framework/Memory/MemUtils.hpp"

namespace bpf
{
    namespace collection
    {
        template <typename K, typename V, typename HashOp>
        HashMap<K, V, HashOp>::HashMap()
            : _data(memory::MemUtils::NewArray<Node>(HASH_MAP_INIT_BUF_SIZE))
            , CurSize(HASH_MAP_INIT_BUF_SIZE)
            , ElemCount(0)
        {
            _data[0].State = ENTRY_STATE_NON_EXISTANT;
            _data[1].State = ENTRY_STATE_NON_EXISTANT;
        }

        template <typename K, typename V, typename HashOp>
        HashMap<K, V, HashOp>::HashMap(const std::initializer_list<Entry> &entries)
            : _data(memory::MemUtils::NewArray<Node>(HASH_MAP_INIT_BUF_SIZE))
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
            : _data(memory::MemUtils::NewArray<Node>(HASH_MAP_INIT_BUF_SIZE))
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
            if (this == &other)
                return (*this);
            memory::MemUtils::DeleteArray(_data, CurSize);
            _data = memory::MemUtils::NewArray<Node>(HASH_MAP_INIT_BUF_SIZE);
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
        HashMap<K, V, HashOp>::HashMap(HashMap &&other) noexcept
            : _data(other._data)
            , CurSize(other.CurSize)
            , ElemCount(other.ElemCount)
        {
            other._data = nullptr;
            other.CurSize = 0;
            other.ElemCount = 0;
        }

        template <typename K, typename V, typename HashOp>
        HashMap<K, V, HashOp> &HashMap<K, V, HashOp>::operator=(HashMap &&other) noexcept
        {
            memory::MemUtils::DeleteArray(_data, CurSize);
            _data = other._data;
            CurSize = other.CurSize;
            ElemCount = other.ElemCount;
            other._data = nullptr;
            other.CurSize = 0;
            other.ElemCount = 0;
            return (*this);
        }

        template <typename K, typename V, typename HashOp>
        HashMap<K, V, HashOp>::~HashMap()
        {
            memory::MemUtils::DeleteArray(_data, CurSize);
        }

        template <typename K, typename V, typename HashOp>
        void HashMap<K, V, HashOp>::TryExtend()
        {
            if ((float)ElemCount / (float)CurSize >= HASH_MAP_LIMIT_UNTIL_EXTEND)
            {
                Node *olddata = _data;
                CurSize <<= 1;
                _data = memory::MemUtils::NewArray<Node>(CurSize);
                for (fsize i = 0; i < CurSize; ++i)
                    _data[i].State = ENTRY_STATE_NON_EXISTANT;
                for (fsize i = 0; i<CurSize>> 1; ++i)
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
                memory::MemUtils::DeleteArray(olddata, CurSize >> 1);
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
                if (_data[index].State == ENTRY_STATE_NON_EXISTANT || _data[index].State == ENTRY_STATE_INSTANCE_DELETE)
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
            fsize hkey = HashOp::ValueOf(key);

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
            fsize hkey = HashOp::ValueOf(key);

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
            fsize idx = QuadraticSearch(HashOp::ValueOf(key));

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
            if (a.CurID >= CurSize || b.CurID >= CurSize || _data[a.CurID].State != ENTRY_STATE_OCCUPIED || _data[b.CurID].State != ENTRY_STATE_OCCUPIED)
                return;
            auto v = std::move(_data[a.CurID].KeyVal.Value);
            _data[a.CurID].KeyVal.Value = std::move(_data[b.CurID].KeyVal.Value);
            _data[b.CurID].KeyVal.Value = std::move(v);
        }

        template <typename K, typename V, typename HashOp>
        void HashMap<K, V, HashOp>::Clear()
        {
            memory::MemUtils::DeleteArray(_data, CurSize);
            _data = memory::MemUtils::NewArray<Node>(HASH_MAP_INIT_BUF_SIZE);
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
            fsize idx = QuadraticSearch(HashOp::ValueOf(key));

            if (idx == (fsize)-1)
                return (Iterator(_data, CurSize, CurSize));
            return (Iterator(_data, idx, CurSize));
        }

        template <typename K, typename V, typename HashOp>
        template <template <typename> class Comparator>
        typename HashMap<K, V, HashOp>::Iterator HashMap<K, V, HashOp>::FindByValue(const V &val)
        {
            for (auto it = begin(); it != end(); ++it)
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
        bool HashMap<K, V, HashOp>::operator==(const HashMap<K, V, HashOp> &other) const noexcept
        {
            if (ElemCount != other.ElemCount)
                return (false);
            auto it = begin();

            while (it != end())
            {
                if (!other.HasKey(it->Key) || other[it->Key] != it->Value)
                    return (false);
                ++it;
            }
            return (true);
        }

        template <typename K, typename V, typename HashOp>
        const V &HashMap<K, V, HashOp>::operator[](const K &key) const
        {
            fsize idx = QuadraticSearch(HashOp::ValueOf(key));

            if (idx == (fsize)-1)
                throw bpf::IndexException((fint)idx);
            return (_data[idx].KeyVal.Value);
        }

        template <typename K, typename V, typename HashOp>
        V &HashMap<K, V, HashOp>::operator[](const K &key)
        {
            fsize idx = QuadraticSearch(HashOp::ValueOf(key));

            if (idx == (fsize)-1)
            {
                TryExtend();
                idx = QuadraticInsert(HashOp::ValueOf(key));
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
            return (QuadraticSearch(HashOp::ValueOf(key)) != (fsize)-1);
        }
    }
}
