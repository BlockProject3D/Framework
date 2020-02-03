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
    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less>::Iterator::Iterator(Node *root, Node *start)
        : _root(root)
    {
        ResetIterator();
        if (start != (Node *)1)
        {
            while (_curNode != start)
                operator++();
        }
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::Iterator::ResetIterator()
    {
        _fixedRoot = Null;
        _curNode = Null;
        _stack.Clear();
        _backStack.Clear();
        Node *nd = _root;
        while (nd != Null)
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

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Iterator &Map<K, V, Greater, Less>::Iterator::operator++()
    {
        if (_curNode != Null)
            _backStack.Push(_curNode);
        if (_stack.Size() == 0)
        {
            _curNode = Null;
            return (*this);
        }
        Node *cpy = _stack.Pop();
        _curNode = cpy;
        cpy = cpy->Right;
        while (cpy != Null)
        {
            _stack.Push(cpy);
            cpy = cpy->Left;
        }
        return (*this);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Iterator &Map<K, V, Greater, Less>::Iterator::operator--()
    {
        if (_backStack.Size() == 0)
            return (*this);
        if (_curNode == _fixedRoot)
        {
            ResetIterator();
            return (*this);
        }
        if (_curNode != Null)
            _stack.Push(_curNode);
        _curNode = _backStack.Pop();
        return (*this);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less>::ReverseIterator::ReverseIterator(Node *root, Node *start)
        : _root(root)
    {
        ResetIterator();
        if (start != (Node *)1)
        {
            while (_curNode != start)
                operator++();
        }
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::ReverseIterator::ResetIterator()
    {
        _fixedRoot = Null;
        _curNode = Null;
        _stack.Clear();
        _backStack.Clear();
        Node *nd = _root;
        while (nd != Null)
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

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::ReverseIterator &Map<K, V, Greater, Less>::ReverseIterator::operator++()
    {
        if (_curNode != Null)
            _backStack.Push(_curNode);
        if (_stack.Size() == 0)
        {
            _curNode = Null;
            return (*this);
        }
        Node *cpy = _stack.Pop();
        _curNode = cpy;
        cpy = cpy->Left;
        while (cpy != Null)
        {
            _stack.Push(cpy);
            cpy = cpy->Right;
        }
        return (*this);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::ReverseIterator &Map<K, V, Greater, Less>::ReverseIterator::operator--()
    {
        if (_backStack.Size() == 0)
            return (*this);
        if (_curNode == _fixedRoot)
        {
            ResetIterator();
            return (*this);
        }
        if (_curNode != Null)
            _stack.Push(_curNode);
        _curNode = _backStack.Pop();
        return (*this);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less>::Map()
        : _root(Null)
        , _count(0)
    {
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less>::Map(const Map &other)
        : _root(Null)
        , _count(0)
    {
        for (auto &entry : other)
            Add(entry.Key, entry.Value);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less>::Map(Map &&other)
        : _root(other._root)
        , _count(other._count)
    {
        other._root = Null;
        other._count = 0;
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less>::Map(const std::initializer_list<Entry> &entries)
        : _root(Null)
        , _count(0)
    {
        for (auto &entry : entries)
            Add(entry.Key, entry.Value);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less>::~Map()
    {
        Clear();
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::Clear()
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

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less> &Map<K, V, Greater, Less>::operator=(const Map &other)
    {
        Clear();
        for (auto &entry : other)
            Add(entry.Key, entry.Value);
        return (*this);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less> &Map<K, V, Greater, Less>::operator=(Map &&other)
    {
        Clear();
        _root = other._root;
        _count = other._count;
        other._root = Null;
        other._count = 0;
        return (*this);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    Map<K, V, Greater, Less> Map<K, V, Greater, Less>::operator+(const Map<K, V, Greater, Less> &other) const
    {
        Map<K, V, Greater, Less> cpy = *this;

        for (const auto &elem : other)
            cpy.Add(elem.Key, elem.Value);
        return (cpy);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::operator+=(const Map<K, V, Greater, Less> &other)
    {
        for (const auto &elem : other)
            Add(elem.Key, elem.Value);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    fisize Map<K, V, Greater, Less>::Height(Node *node)
    {
        return (node != Null ? node->Height : 0);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    fisize Map<K, V, Greater, Less>::Balance(Node *node)
    {
        return (node != Null ? Height(node->Left) - Height(node->Right) : 0);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::LeftRotate(Node *node)
    {
        Node *nd = node->Right;
        node->Right = nd->Left;
        if (nd->Left != Null)
            nd->Left->Parent = node;
        nd->Parent = node->Parent;
        if (node->Parent == Null)
            _root = nd;
        else if (node->Parent->Left == node)
            node->Parent->Left = nd;
        else
            node->Parent->Right = nd;
        nd->Left = node;
        node->Parent = nd;
        if (Height(node->Left) > Height(node->Right))
            node->Height = 1 + Height(node->Left);
        else
            node->Height = 1 + Height(node->Right);
        if (Height(nd->Left) > Height(nd->Right))
            nd->Height = 1 + Height(nd->Left);
        else
            nd->Height = 1 + Height(nd->Right);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::RightRotate(Node *node)
    {
        Node *nd = node->Left;
        node->Left = nd->Right;
        if (nd->Right != Null)
            nd->Right->Parent = node;
        nd->Parent = node->Parent;
        if (node->Parent == Null)
            _root = nd;
        else if (node->Parent->Right == node)
            node->Parent->Right = nd;
        else
            node->Parent->Left = nd;
        nd->Right = node;
        node->Parent = nd;
        if (Height(node->Left) > Height(node->Right))
            node->Height = 1 + Height(node->Left);
        else
            node->Height = 1 + Height(node->Right);
        if (Height(nd->Left) > Height(nd->Right))
            nd->Height = 1 + Height(nd->Left);
        else
            nd->Height = 1 + Height(nd->Right);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Node *Map<K, V, Greater, Less>::InsertNode(const K &key)
    {
        Node *newNode;

        /* BST standard add */
        if (_root == Null)
        {
            newNode = new Node();
            newNode->KeyVal.Key = key;
            /* Data structure augmentation */
            newNode->Height = 1; //Every new node is a leaf
            newNode->Left = Null;
            newNode->Right = Null;
            newNode->Parent = Null;
            _root = newNode;
            ++_count;
            return (newNode);
        }
        Node *cur = _root;
        int lastdir = 0; //0 is left 1 is right
        Node *parent = Null;
        while (cur != Null)
        {
            parent = cur;
            if (Greater<K>::Eval(key, cur->KeyVal.Key))
            {
                cur = cur->Right;
                lastdir = 1;
            }
            else if (Less<K>::Eval(key, cur->KeyVal.Key))
            {
                cur = cur->Left;
                lastdir = 0;
            }
            else // Equal = replace node value
                return (cur);
        }
        ++_count;
        newNode = new Node();
        newNode->KeyVal.Key = key;
        /* Data structure augmentation */
        newNode->Height = 1; //Every new node is a leaf
        newNode->Parent = parent;
        if (lastdir == 1)
            parent->Right = newNode;
        else
            parent->Left = newNode;

        /* AVL Tree property */
        while (parent != Null)
        {
            if (Height(parent->Left) > Height(parent->Right))
                parent->Height = 1 + Height(parent->Left);
            else
                parent->Height = 1 + Height(parent->Right);
            if (Balance(parent) > 1 && Less<K>::Eval(key, parent->Left->KeyVal.Key))
                RightRotate(parent);
            if (Balance(parent) > 1 && Greater<K>::Eval(key, parent->Left->KeyVal.Key))
            {
                LeftRotate(parent->Left);
                RightRotate(parent);
            }
            if (Balance(parent) < -1 && Greater<K>::Eval(key, parent->Right->KeyVal.Key))
                LeftRotate(parent);
            if (Balance(parent) < -1 && Less<K>::Eval(key, parent->Right->KeyVal.Key))
            {
                RightRotate(parent->Right);
                LeftRotate(parent);
            }
            parent = parent->Parent;
        }
        return (newNode);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Node *Map<K, V, Greater, Less>::FindMin(Node *node)
    {
        while (node != Null && node->Left != Null)
            node = node->Left;
        return (node);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::SwapKeyVal(Node *a, Node *b)
    {
        K tmp = std::move(a->KeyVal.Key);
        V tmp1 = std::move(a->KeyVal.Value);

        a->KeyVal.Key = std::move(b->KeyVal.Key);
        a->KeyVal.Value = std::move(b->KeyVal.Value);
        b->KeyVal.Key = std::move(tmp);
        b->KeyVal.Value = std::move(tmp1);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::SwapVal(Node *a, Node *b)
    {
        V tmp = std::move(a->KeyVal.Value);

        a->KeyVal.Value = std::move(b->KeyVal.Value);
        b->KeyVal.Value = std::move(tmp);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::RemoveNode(Node *node)
    {
        /* Data structure augmentation */
        Node *parent = node;

        /* BST standard remove */
        if (node->Left == Null && node->Right == Null) // Case 1 node has no children, delete the node
        {
            if (node == _root)
            {
                delete node;
                _root = Null;
                return;
            }
            if (node->Parent->Left == node)
                node->Parent->Left = Null;
            else
                node->Parent->Right = Null;
            parent = parent->Parent;
            delete node;
        }
        else if (node->Left != Null && node->Right != Null) // Case 3 node has two children, find min in right sub tree then swap and finally remove
        {
            Node *nd = FindMin(node->Right);
            SwapKeyVal(nd, node);
            if (nd->Parent->Left == nd)
                nd->Parent->Left = Null;
            else
                nd->Parent->Right = Null;
            delete nd;
        }
        else // Case 2 node has one child
        {
            if (node == _root)
            {
                if (_root->Left != Null)
                    _root = _root->Left;
                else
                    _root = _root->Right;
                parent = _root;
            }
            if (node->Parent->Left == node)
            {
                if (node->Left != Null)
                {
                    node->Parent->Left = node->Left;
                    node->Left->Parent = node->Parent;
                    parent = node->Left;
                }
                else
                {
                    node->Parent->Left = node->Right;
                    node->Right->Parent = node->Parent;
                    parent = node->Right;
                }
            }
            else
            {
                if (node->Left != Null)
                {
                    node->Parent->Right = node->Left;
                    node->Left->Parent = node->Parent;
                    parent = node->Left;
                }
                else
                {
                    node->Parent->Right = node->Right;
                    node->Right->Parent = node->Parent;
                    parent = node->Right;
                }
                delete node;
            }
        }

        /* AVL Tree property */
        while (parent != Null)
        {
            if (Height(parent->Left) > Height(parent->Right))
                parent->Height = 1 + Height(parent->Left);
            else
                parent->Height = 1 + Height(parent->Right);
            if (Balance(parent) > 1 && Balance(parent->Left) >= 0)
                RightRotate(parent);
            if (Balance(parent) > 1 && Balance(parent->Left) < 0)
            {
                LeftRotate(parent->Left);
                RightRotate(parent);
            }
            if (Balance(parent) < -1 && Balance(parent->Right) <= 0)
                LeftRotate(parent);
            if (Balance(parent) < -1 && Balance(parent->Right) > 0)
            {
                RightRotate(parent->Right);
                LeftRotate(parent);
            }
            parent = parent->Parent;
        }
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::Add(const K &key, const V &value)
    {
        Node *newNode = InsertNode(key);

        newNode->KeyVal.Value = value;
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::Add(const K &key, V &&value)
    {
        Node *newNode = InsertNode(key);

        newNode->KeyVal.Value = std::move(value);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Node *Map<K, V, Greater, Less>::FindNode(const K &key) const
    {
        Node *nd = _root;

        while (nd != Null)
        {
            if (Greater<K>::Eval(key, nd->KeyVal.Key))
                nd = nd->Right;
            else if (Less<K>::Eval(key, nd->KeyVal.Key))
                nd = nd->Left;
            else
                break;
        }
        return (nd);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::RemoveAt(const K &key)
    {
        Node *nd = FindNode(key);

        if (nd != Null)
        {
            RemoveNode(nd);
            --_count;
        }
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::RemoveAt(Iterator &pos)
    {
        Iterator cpy = pos;

        if (cpy._curNode != Null)
        {
            if (cpy._curNode == _root)
                pos = Iterator(Null, Null);
            else
                ++pos;
            RemoveNode(cpy._curNode);
            --_count;
            if (pos._root != Null)
                pos = Iterator(_root, pos._curNode);
        }
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::RemoveAt(Iterator &&pos)
    {
        Iterator cpy = pos;

        ++pos;
        if (cpy._curNode != Null)
        {
            if (cpy._curNode == _root)
                pos = Iterator(Null, Null);
            else
                ++pos;
            RemoveNode(cpy._curNode);
            --_count;
            if (pos._root != Null)
                pos = Iterator(_root, pos._curNode);
        }
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    void Map<K, V, Greater, Less>::Swap(const Iterator &a, const Iterator &b)
    {
        if (a._curNode == Null || b._curNode == Null)
            return;
        SwapVal(a._curNode, b._curNode);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    template <template <typename> class Comparator>
    void Map<K, V, Greater, Less>::Remove(const V &value, const bool all)
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

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    const V &Map<K, V, Greater, Less>::operator[](const K &key) const
    {
        Node *nd = FindNode(key);

        if (nd == Null)
            throw bpf::IndexException(0);
        return (nd->KeyVal.Value);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    V &Map<K, V, Greater, Less>::operator[](const K &key)
    {
        Node *nd = FindNode(key);

        if (nd == Null)
        {
            Node *newNode = InsertNode(key);
            return (newNode->KeyVal.Value);
        }
        return (nd->KeyVal.Value);
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Iterator Map<K, V, Greater, Less>::FindByKey(const K &key)
    {
        Node *nd = FindNode(key);

        return (Iterator(_root, nd));
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    template <template <typename> class Comparator>
    typename Map<K, V, Greater, Less>::Iterator Map<K, V, Greater, Less>::FindByValue(const V &val)
    {
        for (auto it = begin(); it != end(); ++it)
        {
            if (Comparator<V>::Eval(it->Value, val))
                return (it);
        }
        return (Iterator(_root, Null));
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Iterator Map<K, V, Greater, Less>::Find(const std::function<int(const Node &node)> &comparator)
    {
        Node *nd = _root;

        while (nd != Null)
        {
            int res = comparator(*nd);
            if (res < 0)
                nd = nd->Left;
            else if (res > 0)
                nd = nd->Right;
            else
                break;
        }
        return (Iterator(_root, nd));
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Iterator Map<K, V, Greater, Less>::FindMin()
    {
        Node *node = _root;

        while (node != Null && node->Left != Null)
            node = node->Left;
        return (Iterator(_root, node));
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    typename Map<K, V, Greater, Less>::Iterator Map<K, V, Greater, Less>::FindMax()
    {
        Node *node = _root;

        while (node != Null && node->Right != Null)
            node = node->Right;
        return (Iterator(_root, node));
    }

    template <typename K, typename V, template <typename T> class Greater, template <typename T> class Less>
    bool Map<K, V, Greater, Less>::operator==(const Map<K, V, Greater, Less> &other)
    {
        if (_count != other._count)
            return (false);
        Iterator it = begin();

        while (it != end())
        {
            if (!other.HasKey(it->Key) || other[it->Key] != it->Value)
                return (false);
            ++it;
        }
        return (true);
    }
}