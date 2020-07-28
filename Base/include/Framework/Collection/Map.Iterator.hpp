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
#include "Framework/Collection/Stack.hpp"
#include "Framework/Collection/Iterator.hpp"

namespace bpf
{
    namespace collection
    {
        template <typename EntryType, typename NodeType>
        class BP_TPL_API MapConstIterator : public ConstIterator<MapConstIterator<EntryType, NodeType>, EntryType>
        {
        private:
            NodeType *_root;
            NodeType *_fixedRoot;
            NodeType *_curNode;
            Stack<NodeType *> _stack;
            Stack<NodeType *> _backStack;

            void ResetIterator()
            {
                _fixedRoot = nullptr;
                _curNode = nullptr;
                _stack.Clear();
                _backStack.Clear();
                NodeType *nd = _root;
                while (nd != nullptr)
                {
                    _stack.Push(nd);
                    nd = nd->Left;
                }
                operator++();
                if (_curNode == _root)
                    _fixedRoot = _curNode;
                else
                    _fixedRoot = _curNode->Parent;
            }

        public:
            MapConstIterator(NodeType *root, NodeType *start)
                : _root(root)
            {
                ResetIterator();
                if (start != reinterpret_cast<NodeType *>(1))
                {
                    while (_curNode != start)
                        operator++();
                }
            }

            MapConstIterator &operator++()
            {
                if (_curNode != nullptr)
                    _backStack.Push(_curNode);
                if (_stack.Size() == 0)
                {
                    _curNode = nullptr;
                    return (*this);
                }
                NodeType *cpy = _stack.Pop();
                _curNode = cpy;
                cpy = cpy->Right;
                while (cpy != nullptr)
                {
                    _stack.Push(cpy);
                    cpy = cpy->Left;
                }
                return (*this);
            }

            MapConstIterator &operator--()
            {
                if (_backStack.Size() == 0)
                    return (*this);
                if (_curNode == _fixedRoot)
                {
                    ResetIterator();
                    return (*this);
                }
                if (_curNode != nullptr)
                    _stack.Push(_curNode);
                _curNode = _backStack.Pop();
                return (*this);
            }

            inline const EntryType &operator*() const
            {
                return (_curNode->KeyVal);
            }

            inline const EntryType *operator->() const
            {
                return (&_curNode->KeyVal);
            }

            inline bool operator==(const MapConstIterator &other) const
            {
                return (_curNode == other._curNode);
            }

            inline bool operator!=(const MapConstIterator &other) const
            {
                return (_curNode != other._curNode);
            }
        };

        template <typename EntryType, typename NodeType>
        class BP_TPL_API MapConstReverseIterator : public ConstIterator<MapConstReverseIterator<EntryType, NodeType>, EntryType>
        {
        private:
            NodeType *_root;
            NodeType *_fixedRoot;
            NodeType *_curNode;
            Stack<NodeType *> _stack;
            Stack<NodeType *> _backStack;

            void ResetIterator()
            {
                _fixedRoot = nullptr;
                _curNode = nullptr;
                _stack.Clear();
                _backStack.Clear();
                NodeType *nd = _root;
                while (nd != nullptr)
                {
                    _stack.Push(nd);
                    nd = nd->Right;
                }
                operator++();
                if (_curNode == _root)
                    _fixedRoot = _curNode;
                else
                    _fixedRoot = _curNode->Parent;
            }

        public:
            MapConstReverseIterator(NodeType *root, NodeType *start)
                : _root(root)
            {
                ResetIterator();
                if (start != reinterpret_cast<NodeType *>(1))
                {
                    while (_curNode != start)
                        operator++();
                }
            }

            MapConstReverseIterator &operator++()
            {
                if (_curNode != nullptr)
                    _backStack.Push(_curNode);
                if (_stack.Size() == 0)
                {
                    _curNode = nullptr;
                    return (*this);
                }
                NodeType *cpy = _stack.Pop();
                _curNode = cpy;
                cpy = cpy->Left;
                while (cpy != nullptr)
                {
                    _stack.Push(cpy);
                    cpy = cpy->Right;
                }
                return (*this);
            }

            MapConstReverseIterator &operator--()
            {
                if (_backStack.Size() == 0)
                    return (*this);
                if (_curNode == _fixedRoot)
                {
                    ResetIterator();
                    return (*this);
                }
                if (_curNode != nullptr)
                    _stack.Push(_curNode);
                _curNode = _backStack.Pop();
                return (*this);
            }

            inline const EntryType &operator*() const
            {
                return (_curNode->KeyVal);
            }

            inline const EntryType *operator->() const
            {
                return (&_curNode->KeyVal);
            }

            inline bool operator==(const MapConstReverseIterator &other) const
            {
                return (_curNode == other._curNode);
            }

            inline bool operator!=(const MapConstReverseIterator &other) const
            {
                return (_curNode != other._curNode);
            }
        };

        template <typename Map, typename EntryType, typename NodeType>
        class BP_TPL_API MapIterator : public Iterator<MapIterator<Map, EntryType, NodeType>, EntryType>
        {
        private:
            NodeType *_root;
            NodeType *_fixedRoot;
            NodeType *_curNode;
            Stack<NodeType *> _stack;
            Stack<NodeType *> _backStack;

            void ResetIterator()
            {
                _fixedRoot = nullptr;
                _curNode = nullptr;
                _stack.Clear();
                _backStack.Clear();
                NodeType *nd = _root;
                while (nd != nullptr)
                {
                    _stack.Push(nd);
                    nd = nd->Left;
                }
                operator++();
                if (_curNode == _root)
                    _fixedRoot = _curNode;
                else
                    _fixedRoot = _curNode->Parent;
            }

        public:
            MapIterator(NodeType *root, NodeType *start)
                : _root(root)
            {
                ResetIterator();
                if (start != reinterpret_cast<NodeType *>(1))
                {
                    while (_curNode != start)
                        operator++();
                }
            }

            MapIterator &operator++()
            {
                if (_curNode != nullptr)
                    _backStack.Push(_curNode);
                if (_stack.Size() == 0)
                {
                    _curNode = nullptr;
                    return (*this);
                }
                NodeType *cpy = _stack.Pop();
                _curNode = cpy;
                cpy = cpy->Right;
                while (cpy != nullptr)
                {
                    _stack.Push(cpy);
                    cpy = cpy->Left;
                }
                return (*this);
            }

            MapIterator &operator--()
            {
                if (_backStack.Size() == 0)
                    return (*this);
                if (_curNode == _fixedRoot)
                {
                    ResetIterator();
                    return (*this);
                }
                if (_curNode != nullptr)
                    _stack.Push(_curNode);
                _curNode = _backStack.Pop();
                return (*this);
            }

            inline EntryType &operator*()
            {
                return (_curNode->KeyVal);
            }

            inline EntryType *operator->()
            {
                return (&_curNode->KeyVal);
            }

            inline bool operator==(const MapIterator &other) const
            {
                return (_curNode == other._curNode);
            }

            inline bool operator!=(const MapIterator &other) const
            {
                return (_curNode != other._curNode);
            }

            friend Map;
        };

        template <typename EntryType, typename NodeType>
        class BP_TPL_API MapReverseIterator : public ConstIterator<MapReverseIterator<EntryType, NodeType>, EntryType>
        {
        private:
            NodeType *_root;
            NodeType *_fixedRoot;
            NodeType *_curNode;
            Stack<NodeType *> _stack;
            Stack<NodeType *> _backStack;

            void ResetIterator()
            {
                _fixedRoot = nullptr;
                _curNode = nullptr;
                _stack.Clear();
                _backStack.Clear();
                NodeType *nd = _root;
                while (nd != nullptr)
                {
                    _stack.Push(nd);
                    nd = nd->Right;
                }
                operator++();
                if (_curNode == _root)
                    _fixedRoot = _curNode;
                else
                    _fixedRoot = _curNode->Parent;
            }

        public:
            MapReverseIterator(NodeType *root, NodeType *start)
                : _root(root)
            {
                ResetIterator();
                if (start != reinterpret_cast<NodeType *>(1))
                {
                    while (_curNode != start)
                        operator++();
                }
            }

            MapReverseIterator &operator++()
            {
                if (_curNode != nullptr)
                    _backStack.Push(_curNode);
                if (_stack.Size() == 0)
                {
                    _curNode = nullptr;
                    return (*this);
                }
                NodeType *cpy = _stack.Pop();
                _curNode = cpy;
                cpy = cpy->Left;
                while (cpy != nullptr)
                {
                    _stack.Push(cpy);
                    cpy = cpy->Right;
                }
                return (*this);
            }

            MapReverseIterator &operator--()
            {
                if (_backStack.Size() == 0)
                    return (*this);
                if (_curNode == _fixedRoot)
                {
                    ResetIterator();
                    return (*this);
                }
                if (_curNode != nullptr)
                    _stack.Push(_curNode);
                _curNode = _backStack.Pop();
                return (*this);
            }

            inline EntryType &operator*()
            {
                return (_curNode->KeyVal);
            }

            inline EntryType *operator->()
            {
                return (&_curNode->KeyVal);
            }

            inline bool operator==(const MapReverseIterator &other) const
            {
                return (_curNode == other._curNode);
            }

            inline bool operator!=(const MapReverseIterator &other) const
            {
                return (_curNode != other._curNode);
            }
        };
    }
}
