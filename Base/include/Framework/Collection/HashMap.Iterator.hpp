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
        template <class HashMap, typename EntryType, typename NodeType, template <class, typename, typename> class IType, template <typename, typename> class Base>
        class BP_TPL_API HashMapIteratorBase : public Base<IType<HashMap, EntryType, NodeType>, EntryType>
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
            HashMapIteratorBase(NodeType *data, fsize start, fsize size)
                : _data(data)
                , MaxSize(size)
                , CurID(start)
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

            inline bool operator==(const HashMapIteratorBase &other) const
            {
                return (CurID == other.CurID);
            }

            inline bool operator!=(const HashMapIteratorBase &other) const
            {
                return (CurID != other.CurID);
            }
        };

        template <class HashMap, typename EntryType, typename NodeType>
        class BP_TPL_API HashMapConstIterator : public HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstIterator, ConstIterator>
        {
        private:
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstIterator, ConstIterator>::SearchPrevEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstIterator, ConstIterator>::SearchNextEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstIterator, ConstIterator>::CurID;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstIterator, ConstIterator>::MaxSize;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstIterator, ConstIterator>::MinSize;

        public:
            HashMapConstIterator(NodeType *data, fsize start, fsize size)
                : HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstIterator, ConstIterator>(data, start, size)
            {
                fsize old = CurID;
                CurID = 0;
                SearchNextEntry();
                MinSize = CurID;
                CurID = old;
                SearchNextEntry();
            }

            HashMapConstIterator &operator++()
            {
                if (CurID < MaxSize)
                {
                    ++CurID;
                    SearchNextEntry();
                }
                return (*this);
            }

            HashMapConstIterator &operator--()
            {
                if (CurID > MinSize)
                {
                    --CurID;
                    SearchPrevEntry();
                }
                return (*this);
            }

        };

        template <class HashMap, typename EntryType, typename NodeType>
        class BP_TPL_API HashMapConstReverseIterator : public HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstReverseIterator, ConstIterator>
        {
        private:
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstReverseIterator, ConstIterator>::SearchPrevEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstReverseIterator, ConstIterator>::SearchNextEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstReverseIterator, ConstIterator>::CurID;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstReverseIterator, ConstIterator>::MaxSize;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstReverseIterator, ConstIterator>::MinSize;

        public:
            HashMapConstReverseIterator(NodeType *data, fsize start, fsize size)
                : HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapConstReverseIterator, ConstIterator>(data, start, size)
            {
                SearchPrevEntry();
                MaxSize = CurID;
            }

            HashMapConstReverseIterator &operator++()
            {
                if (CurID != (fsize)-1)
                {
                    --CurID;
                    SearchPrevEntry();
                }
                return (*this);
            }

            HashMapConstReverseIterator &operator--()
            {
                if (CurID < MaxSize || CurID == (fsize)-1)
                {
                    ++CurID;
                    SearchNextEntry();
                }
                return (*this);
            }
        };

        template <class HashMap, typename EntryType, typename NodeType>
        class BP_TPL_API HashMapIterator : public HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>
        {
        private:
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>::SearchPrevEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>::SearchNextEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>::CurID;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>::MaxSize;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>::MinSize;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>::_data;

        public:
            HashMapIterator(NodeType *data, fsize start, fsize size)
                : HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapIterator, Iterator>(data, start, size)
            {
                fsize old = CurID;
                CurID = 0;
                SearchNextEntry();
                MinSize = CurID;
                CurID = old;
                SearchNextEntry();
            }

            HashMapIterator &operator++()
            {
                if (CurID < MaxSize)
                {
                    ++CurID;
                    SearchNextEntry();
                }
                return (*this);
            }

            HashMapIterator &operator--()
            {
                if (CurID > MinSize)
                {
                    --CurID;
                    SearchPrevEntry();
                }
                return (*this);
            }

            inline EntryType &operator*()
            {
                return (_data[CurID].KeyVal);
            }

            inline EntryType *operator->()
            {
                return (&_data[CurID].KeyVal);
            }

            friend HashMap;
        };

        template <class HashMap, typename EntryType, typename NodeType>
        class BP_TPL_API HashMapReverseIterator : public HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>
        {
        private:
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>::SearchPrevEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>::SearchNextEntry;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>::CurID;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>::MaxSize;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>::MinSize;
            using HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>::_data;

        public:
            HashMapReverseIterator(NodeType *data, fsize start, fsize size)
                : HashMapIteratorBase<HashMap, EntryType, NodeType, HashMapReverseIterator, Iterator>(data, start, size)
            {
                SearchPrevEntry();
                MaxSize = CurID;
            }

            HashMapReverseIterator &operator++()
            {
                if (CurID != (fsize)-1)
                {
                    --CurID;
                    SearchPrevEntry();
                }
                return (*this);
            }

            HashMapReverseIterator &operator--()
            {
                if (CurID < MaxSize || CurID == (fsize)-1)
                {
                    ++CurID;
                    SearchNextEntry();
                }
                return (*this);
            }

            inline EntryType &operator*()
            {
                return (_data[CurID].KeyVal);
            }

            inline EntryType *operator->()
            {
                return (&_data[CurID].KeyVal);
            }
        };
    }
}
