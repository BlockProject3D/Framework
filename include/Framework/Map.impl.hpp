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

namespace bpf
{
    template <typename K, typename V>
    Map<K, V>::Map()
        : _root(Null)
        , _count(0)
    {
    }

    template <typename K, typename V>
    Map<K, V>::Map(const Map &other)
        : _root(Null)
        , _count(0)
    {
        for (auto &entry : other)
            Add(entry.Key, entry.Value);
    }

    template <typename K, typename V>
    Map<K, V>::Map(Map &&other)
        : _root(other._root)
        , _count(other._count)
    {
        other._root = Null;
        other._count = 0;
    }

    template <typename K, typename V>
    Map<K, V>::Map(const std::initializer_list<Entry> &entries)
        : _root(Null)
        , _count(0)
    {
        for (auto &entry : entries)
            Add(entry.Key, entry.Value);
    }

    template <typename K, typename V>
    Map<K, V>::~Map()
    {
        Clear();
    }

    template <typename K, typename V>
    void Map<K, V>::Clear()
    {
        if (_root == Null)
            return;
        Stack<Node *> stack = { _root };

        while (stack.Size() > 0)
        {
            Node *elem = stack.Pop();

            if (elem->Left != Null)
                stack.Push(elem->Left);
            if (elem->Right != Null)
                stack.Push(elem->Right);
            delete elem;
        }
        _root = Null;
        _count = 0;
    }

    template <typename K, typename V>
    Map<K, V> &Map<K, V>::operator=(const Map &other)
    {
        Clear();
        for (auto &entry : other)
            Add(entry.Key, entry.Value);
    }

    template <typename K, typename V>
    Map<K, V> &Map<K, V>::operator=(Map &&other)
    {
        Clear();
        _root = other._root;
        _count = other._count;
        other._root = Null;
        other._count = 0;
    }

    template <typename K, typename V>
    void Map<K, V>::Add(const K &key, const V &value)
    {
        Node *newNode = new Node();

        newNode->KeyVal.Key = key;
        newNode->KeyVal.Value = value;
        newNode->Height = 1;
        //BST standard add
        if (_root == Null)
        {
            newNode->Left = Null;
            newNode->Right = Null;
            _root = newNode;
            return;
        }
        Node *cur = _root;
        int lastdir = 0; //0 is left 1 is right
        Node *parent = Null;
        while (cur != Null)
        {

        }
    }

    template <typename K, typename V>
    void Map<K, V>::Add(const K &key, V &&value)
    {

    }
}