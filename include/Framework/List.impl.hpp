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
    template <typename T>
    inline List<T>::List()
        : _first(Null)
        , _last(Null)
        , _count(0)
    {
    }

    template <typename T>
    inline List<T>::List(List<T> &&other)
        : _first(other._first)
        , _last(other._last)
        , _count(other._count)
    {
        other._first = Null;
        other._last = Null;
        other._count = 0;
    }

    template <typename T>
    List<T>::List(const List<T> &other)
        : _first(Null)
        , _last(Null)
        , _count(0)
    {
        for (auto &elem : other)
            Add(elem);
    }

    template <typename T>
    List<T>::List(const std::initializer_list<T> &lst)
        : _first(Null)
        , _last(Null)
        , _count(0)
    {
        for (auto &elem : lst)
            Add(elem);
    }

    template <typename T>
    inline List<T>::~List()
    {
        Clear();
    }

    template <typename T>
    List<T> &List<T>::operator=(List<T> &&other)
    {
        Clear();
        _first = other._first;
        _last = other._last;
        _count = other._count;
        other._first = Null;
        other._last = Null;
        other._count = 0;
        return (*this);
    }

    template <typename T>
    List<T> &List<T>::operator=(const List<T> &other)
    {
        Clear();
        for (auto &elem : other)
            Add(elem);
        return (*this);
    }

    template <typename T>
    List<T> List<T>::operator+(const List<T> &other) const
    {
        List<T> cpy = *this;

        for (const auto &elem : other)
            cpy.Add(elem);
        return (cpy);
    }

    template <typename T>
    void List<T>::operator+=(const List<T> &other)
    {
        for (const auto &elem : other)
            Add(elem);
    }

    template <typename T>
    void List<T>::Insert(fsize pos, const T &elem)
    {
        Node *nd = GetNode(pos);
        Node *newi = new Node(elem);

        newi->Next = nd;
        if (nd != Null)
        {
            newi->Prev = nd->Prev;
            nd->Prev = newi;
            if (newi->Prev != Null)
                newi->Prev->Next = newi;
            if (pos == 0)
                _first = newi;
        }
        else if (_last)
        {
            newi->Prev = _last;
            _last->Next = newi;
            _last = newi;
        }
        else
            _first = _last = newi;
        ++_count;
    }

    template <typename T>
    void List<T>::Insert(fsize pos, T &&elem)
    {
        Node *nd = GetNode(pos);
        Node *newi = new Node(std::move(elem));

        newi->Next = nd;
        if (nd != Null)
        {
            newi->Prev = nd->Prev;
            nd->Prev = newi;
            if (newi->Prev != Null)
                newi->Prev->Next = newi;
            if (pos == 0)
                _first = newi;
        }
        else if (_last)
        {
            newi->Prev = _last;
            _last->Next = newi;
            _last = newi;
        }
        else
            _first = _last = newi;
        ++_count;
    }

    template <typename T>
    void List<T>::Insert(const Iterator &pos, const T &elem)
    {
        Node *nd = pos._cur;
        Node *newi = new Node(elem);

        newi->Next = nd;
        if (nd != Null)
        {
            newi->Prev = nd->Prev;
            nd->Prev = newi;
            if (newi->Prev != Null)
                newi->Prev->Next = newi;
            if (pos._cur == _first)
                _first = newi;
        }
        else if (_last)
        {
            newi->Prev = _last;
            _last->Next = newi;
            _last = newi;
        }
        else
            _first = _last = newi;
        ++_count;
    }

    template <typename T>
    void List<T>::Insert(const Iterator &pos, T &&elem)
    {
        Node *nd = pos._cur;
        Node *newi = new Node(std::move(elem));

        newi->Next = nd;
        if (nd != Null)
        {
            newi->Prev = nd->Prev;
            nd->Prev = newi;
            if (newi->Prev != Null)
                newi->Prev->Next = newi;
            if (pos._cur == _first)
                _first = newi;
        }
        else if (_last)
        {
            newi->Prev = _last;
            _last->Next = newi;
            _last = newi;
        }
        else
            _first = _last = newi;
        ++_count;
    }

    template <typename T>
    void List<T>::Add(const T &elem)
    {
        Node *newi = new Node(elem);

        if (_last == Null)
            _first = newi;
        else
        {
            newi->Prev = _last;
            _last->Next = newi;
        }
        _last = newi;
        ++_count;
    }

    template <typename T>
    void List<T>::Add(T &&elem)
    {
        Node *newi = new Node(std::move(elem));

        if (_last == Null)
            _first = newi;
        else
        {
            newi->Prev = _last;
            _last->Next = newi;
        }
        _last = newi;
        ++_count;
    }

    template <typename T>
    inline void List<T>::Clear()
    {
        while (_count > 0)
            RemoveLast();
    }

    template <typename T>
    inline void List<T>::Swap(Node *a, Node *b)
    {
        T tmpdata = std::move(a->Data);

        a->Data = std::move(b->Data);
        b->Data = std::move(tmpdata);
    }

    template <typename T>
    template <template <typename> class Comparator>
    void List<T>::Merge(Node **startl1, Node **endl1, Node **startr1, Node **endr1)
    {
        if (Comparator<T>::Eval((*startr1)->Data, (*startl1)->Data))
        {
            std::swap(*startl1, *startr1);
            std::swap(*endl1, *endr1);
        }
        Node *startl = *startl1;
        Node *endl = *endl1;
        Node *startr = *startr1;
        Node *endrnext = (*endr1)->Next;
        while (startl != endl && startr != endrnext)
        {
            if (Comparator<T>::Eval(startr->Data, startl->Next->Data))
            {
                Node *tmp = startr->Next;
                startr->Next = startl->Next;
                startl->Next = startr;
                startr = tmp;
            }
            startl = startl->Next;
        }
        if (startl == endl)
            startl->Next = startr;
        else
            *endr1 = *endl1;
    }

    template <typename T>
    template <template <typename> class Comparator>
    void List<T>::MergeSort()
    {
        Node *curNode;
        Node *startl;
        Node *startr;
        Node *endl;
        Node *endr;

        for (fsize sz = 1; sz < _count; sz *= 2)
        {
            startl = _first;
            while (startl != Null)
            {
                bool first = false;
                if (startl == _first)
                    first = true;
                endl = startl;
                for (fsize i = 0; endl->Next != Null && i != sz - 1; ++i)
                    endl = endl->Next;
                startr = endl->Next;
                if (startr == Null)
                    break;
                endr = startr;
                for (fsize i = 0; endr->Next != Null && i != sz - 1; ++i)
                    endr = endr->Next;
                Node *cpy = endr->Next; // Get a hold on the start for the next partitions before Merge function destroys it...
                Merge<Comparator>(&startl, &endl, &startr, &endr);
                if (first)
                    _first = startl;
                else
                    curNode->Next = startl;
                curNode = endr;
                startl = cpy;
            }
            curNode->Next = startl;
        }
    }

    template <typename T>
    template <template <typename> class Comparator>
    typename List<T>::Node *List<T>::Partition(Node *start, Node *end)
    {
        Node *x = end;
        Node *iter = start;
        for (Node *j = start; j != end; j = j->Next)
        {
            if (Comparator<T>::Eval(j->Data, x->Data))
            {
                Swap(iter, j);
                iter = iter->Next;
            }
        }
        Swap(iter, end);
        return (iter);
    }

    template <typename T>
    template <template <typename> class Comparator>
    void List<T>::QuickSort(Node *start, Node *end)
    {
        if (end != Null && start != end && start != end->Next)
        {
            Node *res = Partition<Comparator>(start, end);
            QuickSort<Comparator>(start, res->Prev);
            QuickSort<Comparator>(res->Next, end);
        }
    }

    template <typename T>
    template <template <typename> class Comparator>
    void List<T>::Sort(const bool stable) //TODO: Adapt to use different sorting functions
    {
        if (Size() == 0 || Size() == 1)
            return;
        if (!stable)
            QuickSort<Comparator>(_first, _last);
        else
        {
            MergeSort<Comparator>();
            /* Re-chain the broken list */
            Node *cur = _first;
            Node *last = Null;
            while (cur != Null)
            {
                cur->Prev = last;
                last = cur;
                cur = cur->Next;
            }
            _last = last;
        }
    }

    template <typename T>
    void List<T>::Swap(const Iterator &a, const Iterator &b)
    {
        Node *an = a._cur;
        Node *bn = b._cur;
        if (an == Null || bn == Null || an == bn)
            return;
        Swap(an, bn);
    }

    template <typename T>
    void List<T>::RemoveAt(Iterator &pos)
    {
        if (pos._cur == Null)
            return;
        Node *next = pos._cur->Next;
        RemoveNode(pos._cur);
        pos._cur = next;
    }

    template <typename T>
    void List<T>::RemoveAt(Iterator &&pos)
    {
        if (pos._cur == Null)
            return;
        Node *next = pos._cur->Next;
        RemoveNode(pos._cur);
        pos._cur = next;
    }

    template <typename T>
    typename List<T>::Node *List<T>::GetNode(fsize id) const
    {
        if (id < _count)
        {
            if (id <= (_count / 2))
            {
                Node *cur = _first;
                while (id-- > 0)
                    cur = cur->Next;
                return (cur);
            }
            else
            {
                Node *cur = _last;
                while (++id < _count)
                    cur = cur->Prev;
                return cur;
            }
        }
        return Null;
    }

    template <typename T>
    void List<T>::RemoveAt(fsize const pos)
    {
        Node *toRM = GetNode(pos);

        if (toRM)
            RemoveNode(toRM);
    }

    template <typename T>
    void List<T>::RemoveNode(Node *toRM)
    {
        if (toRM == _last)
            RemoveLast();
        else
        {
            Node *prev = toRM->Prev;
            Node *next = toRM->Next;

            if (prev)
                prev->Next = next;
            else
                _first = next;
            if (next)
                next->Prev = prev;
            delete toRM;
            --_count;
        }
    }

    template <typename T>
    template <template <typename> class Equal>
    void List<T>::Remove(const T &elem, const bool all)
    {
        Node *cur = _first;

        while (cur)
        {
            if (Equal<T>::Eval(cur->Data, elem))
            {
                Node *toRM = cur;
                cur = (all) ? cur->Next : Null;
                RemoveNode(toRM);
            }
            else
                cur = cur->Next;
        }
    }

    template <typename T>
    void List<T>::RemoveLast()
    {
        if (_last)
        {
            Node *lst = _last->Prev;

            if (lst)
                lst->Next = Null;
            else
                _first = Null;
            delete _last;
            _last = lst;
            --_count;
        }
    }

    template <typename T>
    inline const T &List<T>::operator[](fsize const id) const
    {
        Node *elem = GetNode(id);

        if (elem == Null)
            throw IndexException((fint)id);
        return (elem->Data);
    }
}
