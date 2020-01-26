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
    void ArrayList<T>::RemoveAt(const fsize pos)
    {
        if (pos >= _curid)
            return;
        if (_curid == 0 || _curid == 1)
        {
            _curid = 0;
            return;
        }
        for (fsize i = pos; i < _curid; ++i)
            _arr[i] = _arr[i + 1];
        --_curid;
    }

    template <typename T>
    template <template <typename> class Equal>
    void ArrayList<T>::Remove(const T &elem, const bool all)
    {
        for (fsize i = 0; i < _curid; ++i)
        {
            if (Equal<T>::Eval(_arr[i], elem))
            {
                RemoveAt(i);
                if (!all)
                    return;
            }
        }
    }

    template <typename T>
    void ArrayList<T>::Insert(const fsize pos, const T &elem)
    {
        if (_curid + 1 >= _arr.Size())
            _arr.Resize(_arr.Size() * 2);
        for (fsize i = _curid; i > pos; --i)
            _arr[i] = _arr[i - 1];
        _arr[pos] = elem;
        ++_curid;
    }

    template <typename T>
    void ArrayList<T>::Insert(const fsize pos, T &&elem)
    {
        if (_curid + 1 >= _arr.Size())
            _arr.Resize(_arr.Size() * 2);
        for (fsize i = _curid; i > pos; --i)
            _arr[i] = _arr[i - 1];
        _arr[pos] = std::move(elem);
        ++_curid;
    }

    template <typename T>
    void ArrayList<T>::Insert(const Iterator &pos, const T &elem)
    {
        if (_curid + 1 >= _arr.Size())
            _arr.Resize(_arr.Size() * 2);
        for (fsize i = _curid; i > pos.ArrayPos(); --i)
            _arr[i] = _arr[i - 1];
        _arr[pos.ArrayPos()] = elem;
        ++_curid;
    }

    template <typename T>
    void ArrayList<T>::Insert(const Iterator &pos, T &&elem)
    {
        if (_curid + 1 >= _arr.Size())
            _arr.Resize(_arr.Size() * 2);
        for (fsize i = _curid; i > pos.ArrayPos(); --i)
            _arr[i] = _arr[i - 1];
        _arr[pos.ArrayPos()] = std::move(elem);
        ++_curid;
    }

    template <typename T>
    template <template <typename> class Comparator>
    void ArrayList<T>::Merge(const Array<T> &a, Array<T> &c, const fsize start, const fsize mid, const fsize end)
    {
        fsize n1 = mid - start + 1;
        fsize n2 = end - mid;
        fsize i = 0;
        fsize j = 0;
        fsize k = 0;

        while (i < n1 || j < n2)
        {
            if (i < n1 && j < n2)
            {
                if (Comparator<T>::Eval(a[mid + 1 + j], a[start + i]))
                {
                    c[k + start] = std::move(a[mid + 1 + j]);
                    ++j;
                }
                else
                {
                    c[k + start] = std::move(a[start + i]);
                    ++i;
                }
                ++k;
            }
            else if (i < n1)
            {
                c[k + start] = std::move(a[start + i]);
                ++k;
                ++i;
            }
            else if (j < n2)
            {
                c[k + start] = std::move(a[mid + 1 + j]);
                ++k;
                ++j;
            }
        }
    }

    template <typename T>
    template <template <typename> class Comparator>
    void ArrayList<T>::MergeSort()
    {
        fsize n = _curid;
        Array<T> c(_curid);
        Array<T> &a = _arr;
        fsize sz = 1;

        while (sz <= n - 1)
        {
            for (fsize i = 0; i < n - 1; i += sz * 2)
            {
                fsize mid = (n - 1 < i + sz - 1) ? n - 1 : i + sz - 1;
                fsize end = (n - 1 < i + 2 * sz - 1) ? n - 1 : i + 2 * sz - 1;

                Merge<Comparator>(a, c, i, mid, end);
            }
            auto tmp = std::move(c);
            c = std::move(a);
            a = std::move(tmp);
            sz *= 2;
        }
    }

    template <typename T>
    template <template <typename> class Comparator>
    fsize ArrayList<T>::Partition(fsize start, fsize end)
    {
        fsize x = end;
        fsize iter = start;
        for (fsize j = start; j < end; ++j)
        {
            if (Comparator<T>::Eval(_arr[j], _arr[x]))
            {
                T tmp = std::move(_arr[iter]);
                _arr[iter] = std::move(_arr[j]);
                _arr[j] = std::move(tmp);
                ++iter;
            }
        }
        T tmp = std::move(_arr[iter]);
        _arr[iter] = std::move(_arr[end]);
        _arr[end] = std::move(tmp);
        return (iter);
    }

    template <typename T>
    template <template <typename> class Comparator>
    void ArrayList<T>::QuickSort(fsize start, fsize end)
    {
        if (start < _curid && end < _curid && start < end)
        {
            fsize res = Partition<Comparator>(start, end);
            QuickSort<Comparator>(start, res - 1);
            QuickSort<Comparator>(res + 1, end);
        }
    }

    template <typename T>
    template <template <typename> class Comparator>
    void ArrayList<T>::Sort(const bool stable)
    {
        if (_curid == 0 || _curid == 1)
            return;
        if (!stable)
            QuickSort<Comparator>(0, _curid - 1);
        else
            MergeSort<Comparator>();
    }
}
