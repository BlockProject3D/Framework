#ifndef LIST_IMPL_H_
# define LIST_IMPL_H_

template <typename T>
inline FList<T>::FList()
    : First(Null), Last(Null), Count(0)
{
}

template <typename T>
inline FList<T>::~FList()
{
    this->Clear();
}

template <typename T>
void FList<T>::Insert(const T &elem, uint32 pos)
{
    FListNode<T> *nd = GetNode(pos);
    FListNode<T> *newi = new FListNode<T>(elem);

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
void FList<T>::Add(const T &elem)
{
    FListNode<T> *newi = new FListNode<T>(elem);

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
void FList<T>::Insert(T &&elem, uint32 pos)
{
    FListNode<T> *nd = GetNode(pos);
    FListNode<T> *newi = new FListNode<T>(std::move(elem));

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
void FList<T>::Add(T &&elem)
{
    FListNode<T> *newi = new FListNode<T>(std::move(elem));

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
inline void FList<T>::Clear()
{
    while (Count > 0)
        RemoveLast();
}

/*template<typename T>
void	FList<T>::Sort(void)
{
	T		*tmp = Null;

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
FListNode<T> *FList<T>::Partition(FListNode<T> *start, FListNode<T> *end)
{
    FListNode<T> *tmp = (start != Null) ? (start->Prev) : Null;

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
void FList<T>::QuickSort(FListNode<T> *start, FListNode<T> *end) //QuickSort seem to be a crap : does not do anything at all
{
    FListNode<T> *res = Partition(First, Last);
    if (res != Null)
    {
        QuickSort(start, res->Prev);
        QuickSort(res->Next, end);
    }
}

template <typename T>
void FList<T>::Sort()
{
    if (Size() <= 0)
        return;
    QuickSort(First, Last);
}

template <typename T>
FListNode<T> *FList<T>::GetNode(uint32 id) const
{
    if (id < Count)
    {
        if (id <= (Count / 2))
        {
            FListNode<T> *cur = First;

            while (id-- > 0)
                cur = cur->Next;
            return (cur);
        }
        else
        {
            FListNode<T> *cur = Last;

            while (++id < Count)
                cur = cur->Prev;
            return cur;
        }
    }
    return Null;
}

template <typename T>
inline T *FList<T>::Get(uint32 const id) const
{
    FListNode<T> *cur = this->GetNode(id);

    return ((cur) ? &cur->Data : Null);
}

template <typename T>
inline T *FList<T>::GetFirst() const
{
    return ((First) ? &First->Data : Null);
}

template <typename T>
inline T *FList<T>::GetLast() const
{
    return ((Last) ? &Last->Data : Null);
}

template <typename T>
void FList<T>::RemoveAt(uint32 const id)
{
    FListNode<T> *toRM = GetNode(id);

    if (toRM)
        RemoveNode(toRM);
}

/* Private Remove */
template <typename T>
void FList<T>::RemoveNode(FListNode<T> *toRM)
{
    if (toRM == Last)
        RemoveLast();
    else
    {
        FListNode<T> *prev = toRM->Prev;
        FListNode<T> *next = toRM->Next;

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
/* End Private */

template <typename T>
void FList<T>::Remove(const T &elem, const bool all)
{
    FListNode<T> *cur = First;

    while (cur)
    {
        if (cur->Data == elem)
        {
            FListNode<T> *toRM = cur;

            cur = (all) ? cur->Next : Null;
            RemoveNode(toRM);
        }
        else
            cur = cur->Next;
    }
}

template <typename T>
void FList<T>::RemoveLast()
{
    if (Last)
    {
        FListNode<T> *lst = Last->Prev;

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
inline uint32 FList<T>::Size() const
{
    return (Count);
}

template <typename T>
inline T FList<T>::operator[](uint32 const id) const
{
    T *elem = Get(id);

    if (elem == Null)
        throw FIndexException(id);
    return (*elem);
}

template <typename T>
inline void FList<T>::ForEach(FDelegateFunction<void, const T &> &fnc) const
{
    for (auto it = Begin(); it; ++it)
        fnc(*it);
}

#endif /* !LIST_IMPL_H_ */
