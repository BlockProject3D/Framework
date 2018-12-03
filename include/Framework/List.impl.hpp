#ifndef LIST_IMPL_H_
# define LIST_IMPL_H_

namespace bpf
{
    template <typename T>
    inline List<T>::List()
        : First(Null), Last(Null), Count(0)
    {
    }

    template <typename T>
    inline List<T>::~List()
    {
        this->Clear();
    }

    template <typename T>
    void List<T>::Insert(const T &elem, uint32 pos)
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
    void List<T>::Insert(T &&elem, uint32 pos)
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
    ListNode<T> *List<T>::GetNode(uint32 id) const
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
    inline T *List<T>::Get(uint32 const id) const
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
    void List<T>::RemoveAt(uint32 const id)
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
    inline uint32 List<T>::Size() const
    {
        return (Count);
    }

    template <typename T>
    inline T List<T>::operator[](uint32 const id) const
    {
        T *elem = Get(id);

        if (elem == Null)
            throw IndexException(id);
        return (*elem);
    }

    template <typename T>
    inline void List<T>::ForEach(const std::function<void(const T &)> &fnc) const
    {
        for (auto it = Begin(); it; ++it)
            fnc(*it);
    }
}

#endif /* !LIST_IMPL_H_ */
