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
		for (auto& elem : lst)
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
    void List<T>::Insert(fsize pos, const T &elem)
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
		ListNode<T> *nd = pos._cur;
		ListNode<T> *newi = new ListNode<T>(elem);

		newi->Next = nd;
		if (nd != Null)
		{
			newi->Prev = nd->Prev;
			nd->Prev = newi;
			if (newi->Prev != Null)
				newi->Prev->Next = newi;
			if (pos._cur == FirstNode())
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
		ListNode<T> *nd = pos._cur;
		ListNode<T> *newi = new ListNode<T>(std::move(elem));

		newi->Next = nd;
		if (nd != Null)
		{
			newi->Prev = nd->Prev;
			nd->Prev = newi;
			if (newi->Prev != Null)
				newi->Prev->Next = newi;
			if (pos._cur == FirstNode())
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
        ListNode<T> *newi = new ListNode<T>(elem);

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
        ListNode<T> *newi = new ListNode<T>(std::move(elem));

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
	inline void List<T>::Swap(ListNode<T> *a, ListNode<T> *b)
	{
		T tmpdata = std::move(a->Data);

		a->Data = std::move(b->Data);
		b->Data = std::move(tmpdata);
	}

    template <typename T>
    ListNode<T> *List<T>::Partition(ListNode<T> *start, ListNode<T> *end)
    {
		ListNode<T> *x = end;
		ListNode<T> *iter = start->Prev;
		for (ListNode<T>* j = start; j != end; j = j->Next)
		{
			if (j->Data <= x->Data)
			{
				iter = (iter == Null) ? start : iter->Next;
				Swap(iter, j);
			}
		}
		Swap(iter->Next, end);
		return (iter->Next);
    }

    template <typename T>
    void List<T>::QuickSort(ListNode<T> *start, ListNode<T> *end)
    {
		if (end != Null && start != end && start != end->Next)
		{
			ListNode<T> *res = Partition(start, end);
			QuickSort(start, res->Prev);
			QuickSort(res->Next, end);
		}
    }

    template <typename T>
	void List<T>::Sort(const bool stable) //TODO: Adapt to use different sorting functions
    {
        if (Size() <= 0)
            return;
		if (!stable)
			QuickSort(_first, _last);
    }

	template <typename T>
	void List<T>::Swap(const Iterator &a, const Iterator &b)
	{
		ListNode<T> *an = a._cur;
		ListNode<T> *bn = b._cur;
		if (an == Null || bn == Null || an == bn)
			return;
		Swap(an, bn);
	}

	template <typename T>
	void List<T>::RemoveAt(Iterator &pos)
	{
		if (pos._cur == Null)
			return;
		ListNode<T> *next = pos._cur->Next;
		RemoveNode(pos._cur);
		pos._cur = next;
	}

	template <typename T>
	void List<T>::RemoveAt(Iterator &&pos)
	{
		if (pos._cur == Null)
			return;
		ListNode<T> *next = pos._cur->Next;
		RemoveNode(pos._cur);
		pos._cur = next;
	}

    template <typename T>
    ListNode<T> *List<T>::GetNode(fsize id) const
    {
        if (id < _count)
        {
            if (id <= (_count / 2))
            {
                ListNode<T> *cur = _first;
                while (id-- > 0)
                    cur = cur->Next;
                return (cur);
            }
            else
            {
                ListNode<T> *cur = _last;
                while (++id < _count)
                    cur = cur->Prev;
                return cur;
            }
        }
        return Null;
    }

    template <typename T>
    inline ListNode<T> *List<T>::FirstNode() const
    {
        return (_first);
    }

    template <typename T>
    inline ListNode<T> *List<T>::LastNode() const
    {
        return (_last);
    }

    template <typename T>
    void List<T>::RemoveAt(fsize const pos)
    {
        ListNode<T> *toRM = GetNode(pos);

        if (toRM)
            RemoveNode(toRM);
    }

    template <typename T>
    void List<T>::RemoveNode(ListNode<T> *toRM)
    {
        if (toRM == _last)
            RemoveLast();
        else
        {
            ListNode<T> *prev = toRM->Prev;
            ListNode<T> *next = toRM->Next;

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
    template <template <typename> typename Equal>
    void List<T>::Remove(const T &elem, const bool all)
    {
        ListNode<T> *cur = _first;

        while (cur)
        {
            if (Equal<T>::Eval(cur->Data, elem))
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
        if (_last)
        {
            ListNode<T> *lst = _last->Prev;

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
    inline fsize List<T>::Size() const
    {
        return (_count);
    }

    template <typename T>
    inline const T &List<T>::operator[](fsize const id) const
    {
        ListNode<T> *elem = GetNode(id);

        if (elem == Null)
            throw IndexException((fint)id);
        return (elem->Data);
    }
}
