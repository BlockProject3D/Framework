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

namespace bpf
{
    namespace collection
    {
        template <class HashMap, typename EntryType, typename NodeType>
        class BP_TPL_API HashMapConstIterator : public ConstIterator<HashMapConstIterator<K, V, HashOp, EntryType, NodeType>, EntryType>
        {
        protected:
            NodeType *_data;
            fsize MaxSize;
            fsize MinSize;
            fsize CurID;

            void SearchNextEntry()
            {
                while (CurID < MaxSize && _data[CurID].State != HashMap::ENTRY_STATE_OCCUPIED)
                    ++CurID;
            }

            void SearchPrevEntry()
            {
                while (CurID != (fsize)-1 && _data[CurID].State != HashMap::ENTRY_STATE_OCCUPIED)
                    --CurID;
            }

        public:
            HashMapConstIterator(NodeType *data, fsize start, fsize size, bool reverse = false)
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

            HashMapConstIterator &operator++()
            {

            }

            HashMapConstIterator &operator--()
            {

            }

            inline const EntryType &operator*() const
            {
                return (_data[CurID].KeyVal);
            }
            inline const EntryType *operator->() const
            {
                return (&_data[CurID].KeyVal);
            }
            inline bool operator==(const HashMapConstIterator &other) const
            {
                return (CurID == other.CurID);
            }
            inline bool operator!=(const HashMapConstIterator &other) const
            {
                return (CurID != other.CurID);
            }

            friend class typename HashMap;
        };

        class BP_TPL_API ReverseIterator final : public Iterator
        {
        public:
            inline ReverseIterator(Node *data, fsize start, fsize size)
                : Iterator(data, start, size, true)
            {
            }
            ReverseIterator &operator++();
            ReverseIterator &operator--();

            friend class HashMap<K, V, HashOp>;
        };
    }
}
