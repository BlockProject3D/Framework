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
        template <typename T>
        class BP_TPL_API List;

        template <typename T, typename NodeType>
        class BP_TPL_API ListConstIterator : public ConstIterator<ListConstIterator<T, NodeType>, T>
        {
        protected:
            NodeType *_cur;
            NodeType *_reset;

        public:
            inline ListConstIterator(NodeType *start, NodeType *reset)
                : _cur(start)
                , _reset(reset)
            {
            }

            inline ListConstIterator &operator++()
            {
                if (_cur)
                    _cur = _cur->Next;
                return (*this);
            }

            inline ListConstIterator &operator--()
            {
                if (_cur)
                    _cur = _cur->Prev ? _cur->Prev : _cur;
                else
                    _cur = _reset;
                return (*this);
            }

            inline const T *operator->() const
            {
                return (&_cur->Data);
            }

            inline const T &operator*() const
            {
                return (_cur->Data);
            }

            inline bool operator==(const ListConstIterator &it) const noexcept
            {
                return (_cur == it._cur);
            }

            inline bool operator!=(const ListConstIterator &it) const noexcept
            {
                return (_cur != it._cur);
            }

            friend class List<T>;
        };

        template <typename T, typename NodeType>
        class BP_TPL_API ListConstReverseIterator : public ListConstIterator<T, NodeType>
        {
        private:
            using ListConstIterator<T, NodeType>::_cur;
            using ListConstIterator<T, NodeType>::_reset;

        public:
            inline ListConstReverseIterator(NodeType *start, NodeType *reset)
                : ListConstIterator<T, NodeType>(start, reset)
            {
            }

            inline ListConstReverseIterator &operator++()
            {
                if (_cur)
                    _cur = _cur->Prev;
                return (*this);
            }

            inline ListConstReverseIterator &operator--()
            {
                if (_cur)
                    _cur = _cur->Next ? _cur->Next : _cur;
                else
                    _cur = _reset;
                return (*this);
            }
        };

        template <typename T, typename NodeType>
        class BP_TPL_API ListIterator : public Iterator<ListIterator<T, NodeType>, T>
        {
        protected:
            NodeType *_cur;
            NodeType *_reset;

        public:
            inline ListIterator(NodeType *start, NodeType *reset)
                : _cur(start)
                , _reset(reset)
            {
            }

            inline ListIterator &operator++()
            {
                if (_cur)
                    _cur = _cur->Next;
                return (*this);
            }

            inline ListIterator &operator--()
            {
                if (_cur)
                    _cur = _cur->Prev ? _cur->Prev : _cur;
                else
                    _cur = _reset;
                return (*this);
            }

            inline T *operator->()
            {
                return (&_cur->Data);
            }

            inline T &operator*()
            {
                return (_cur->Data);
            }

            inline const T *operator->() const
            {
                return (&_cur->Data);
            }

            inline const T &operator*() const
            {
                return (_cur->Data);
            }

            inline bool operator==(const ListIterator &it) const noexcept
            {
                return (_cur == it._cur);
            }

            inline bool operator!=(const ListIterator &it) const noexcept
            {
                return (_cur != it._cur);
            }

            friend class List<T>;
        };

        template <typename T, typename NodeType>
        class BP_TPL_API ListReverseIterator : public ListIterator<T, NodeType>
        {
        private:
            using ListIterator<T, NodeType>::_cur;
            using ListIterator<T, NodeType>::_reset;

        public:
            inline ListReverseIterator(NodeType *start, NodeType *reset)
                : ListIterator<T, NodeType>(start, reset)
            {
            }

            inline ListReverseIterator &operator++()
            {
                if (_cur)
                    _cur = _cur->Prev;
                return (*this);
            }

            inline ListReverseIterator &operator--()
            {
                if (_cur)
                    _cur = _cur->Next ? _cur->Next : _cur;
                else
                    _cur = _reset;
                return (*this);
            }
        };
    }
}
