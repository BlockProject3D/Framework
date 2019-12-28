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
        for (fsize i = pos ; i < _curid ; ++i)
            _arr[i] = _arr[i + 1];
        --_curid;
    }
    
    template <typename T>
    template <typename Equal>
    void ArrayList<T>::Remove(const T &elem, const bool all)
    {
        for (fsize i = 0 ; i < _curid ; ++i)
        {
            if (Equal::Eval(_arr[i], elem))
            {
                RemoveAt(i);
                if (!all)
                    return;
            }
        }
        if (_arr[0] == elem)
            RemoveAt((fsize)0);
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
}
