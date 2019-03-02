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
    template <typename T>
    inline List<T>::List()
        : First(Null)
        , Last(Null)
        , Count(0)
    {
    }
    
    template <typename T>
    inline List<T>::List(List<T> &&other)
        : First(other.First)
        , Last(other.Last)
        , Count(other.Count)
    {
        other.First = Null;
        other.Last = Null;
        other.Count = 0;
    }

    template <typename T>
    inline List<T>::~List()
    {
        this->Clear();
    }

    template <typename T>
    void List<T>::Insert(const T &elem, fsize pos)
    {
        ListNode<T> *nd = GetNode(pos);
        ListNode<T> *newi = new ListNode<T>(elem);

        newi->Next = nd;
        if (nd != Null)
        {
            newi->Prev = nd->Prev;
            nd->Prev = newi;
            if (newi->Prev != Null)
                newi->Prev->Next = newi;
            if (pos == 0)
                First = newi;
        }
        else if (Last)
        {
            newi->Prev = Last;
            Last->Next = newi;
            Last = newi;
        }
        else
            First = Last = newi;
        ++Count;
    }

    template <typename T>
    void List<T>::Add(const T &elem)
    {
        ListNode<T> *newi = new ListNode<T>(elem);

        if (Last == Null)
            First = newi;
        else
        {
            newi->Prev = Last;
            Last->Next = newi;
        }
        Last = newi;
        ++Count;
    }

    template <typename T>
    void List<T>::Insert(T &&elem, fsize pos)
    {
        ListNode<T> *nd = GetNode(pos);
        ListNode<T> *newi = new ListNode<T>(std::move(elem));

        newi->Next = nd;
        if (nd != Null)
        {
            newi->Prev = nd->Prev;
            nd->Prev = newi;
            if (newi->Prev != Null)
                newi->Prev->Next = newi;
            if (pos == 0)
                First = newi;
        }
        else if (Last)
        {
            newi->Prev = Last;
            Last->Next = newi;
            Last = newi;
        }
        else
            First = Last = newi;
        ++Count;
    }

    template <typename T>
    void List<T>::Add(T &&elem)
    {
        ListNode<T> *newi = new ListNode<T>(std::move(elem));

        if (Last == Null)
            First = newi;
        else
        {
            newi->Prev = Last;
            Last->Next = newi;
        }
        Last = newi;
        ++Count;
    }

    template <typename T>
    inline void List<T>::Clear()
    {
        while (Count > 0)
            RemoveLast();
    }

/*template<typename T>
void    FList<T>::Sort(void)
{
    T        *tmp = Null;

    for (int i = 0 ; i < Count ; i++)
    {
        for (int j = 0 ; j < (Count - i) ; j++)
        {
            if (Get(j) != Null && Get(j + 1) != Null
                && *Get(j) > *Get(j + 1))
            {
                tmp = GetNode(j)->Data;
                GetNode(j)->Data = GetNode(j + 1)->Data;
                GetNode(j + 1)->Data = tmp;
            }
        }
    }
}*/

    template <typename T>
    ListNode<T> *List<T>::Partition(ListNode<T> *start, ListNode<T> *end)
    {
        ListNode<T> *tmp = (start != Null) ? (start->Prev) : Null;

        while (start != Null && end != Null && start != end->Prev)
        {
            if (start->Data < end->Data && tmp != Null)
            {
                tmp = tmp->Next;
                T tmpdata = tmp->Data;
                tmp->Data = start->Data;
                start->Data = tmpdata;
            }
            start = start->Next;
        }
        if (tmp != Null && tmp->Next != Null)
        {
            T tmpdata = tmp->Next->Data;
            tmp->Next->Data = end->Data;
            end->Data = tmpdata;
        }
        return (tmp == Null ? Null : tmp->Next);
    }

    template <typename T>
    void List<T>::QuickSort(ListNode<T> *start, ListNode<T> *end) //QuickSort seem to be a crap : does not do anything at all
    {
        ListNode<T> *res = Partition(First, Last);

        if (res != Null)
        {
            QuickSort(start, res->Prev);
            QuickSort(res->Next, end);
        }
    }

    template <typename T>
    void List<T>::Sort()
    {
        if (Size() <= 0)
            return;
        QuickSort(First, Last);
    }

    template <typename T>
    ListNode<T> *List<T>::GetNode(fsize id) const
    {
        if (id < Count)
        {
            if (id <= (Count / 2))
            {
                ListNode<T> *cur = First;
                while (id-- > 0)
                    cur = cur->Next;
                return (cur);
            }
            else
            {
                ListNode<T> *cur = Last;
                while (++id < Count)
                    cur = cur->Prev;
                return cur;
            }
        }
        return Null;
    }

    template <typename T>
    inline T *List<T>::Get(fsize const id) const
    {
        ListNode<T> *cur = this->GetNode(id);

        return ((cur) ? &cur->Data : Null);
    }

    template <typename T>
    inline T *List<T>::GetFirst() const
    {
        return ((First) ? &First->Data : Null);
    }

    template <typename T>
    inline T *List<T>::GetLast() const
    {
        return ((Last) ? &Last->Data : Null);
    }

    template <typename T>
    void List<T>::RemoveAt(fsize const id)
    {
        ListNode<T> *toRM = GetNode(id);

        if (toRM)
            RemoveNode(toRM);
    }

    template <typename T>
    void List<T>::RemoveNode(ListNode<T> *toRM)
    {
        if (toRM == Last)
            RemoveLast();
        else
        {
            ListNode<T> *prev = toRM->Prev;
            ListNode<T> *next = toRM->Next;

            if (prev)
                prev->Next = next;
            else
                First = next;
            if (next)
                next->Prev = prev;
            delete toRM;
            --Count;
        }
    }

    template <typename T>
    void List<T>::Remove(const T &elem, const bool all)
    {
        ListNode<T> *cur = First;

        while (cur)
        {
            if (cur->Data == elem)
            {
                ListNode<T> *toRM = cur;
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
        if (Last)
        {
            ListNode<T> *lst = Last->Prev;

            if (lst)
                lst->Next = Null;
            else
                First = Null;
            delete Last;
            Last = lst;
            --Count;
        }
    }

    template <typename T>
    inline fsize List<T>::Size() const
    {
        return (Count);
    }

    template <typename T>
    inline T List<T>::operator[](fsize const id) const
    {
        T *elem = Get(id);

        if (elem == Null)
            throw IndexException((fint)id);
        return (*elem);
    }
}
