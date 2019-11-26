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
#include "Framework/Array.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API ArrayList
    {
    private:
        fsize _curid;
        Array<T> _arr;

    public:
        inline ArrayList()
            : _curid(0), _arr(16)
        {
        }

		inline ArrayList(const std::initializer_list<T> &lst)
			: _curid(0), _arr(16)
		{
			for (auto& elem : lst)
				Add(elem);
		}

		inline ArrayList(const ArrayList<T> &other)
			: _curid(other._curid), _arr(other._arr)
		{
		}

		inline ArrayList(ArrayList<T> &&other)
			: _curid(other._curid), _arr(std::move(other._arr))
		{
			other._curid = 0;
		}

		inline ArrayList<T> &operator=(const ArrayList<T> &other)
		{
			_curid = other._curid;
			_arr = other._arr;
			return (*this);
		}

		inline ArrayList<T> &operator=(ArrayList<T> &&other)
		{
			_curid = other._curid;
			_arr = std::move(other._arr);
			other._curid = 0;
			return (*this);
		}

		inline T& First()
		{
			return (_arr.First());
		}

		inline T& Last()
		{
			if (_curid == 0)
				throw IndexException(0);
			return (_arr[_curid]);
		}

		inline T &operator[](const fsize id) const
		{
			if (id >= _curid)
				throw IndexException(static_cast<fisize>(id));
			return (_arr[id]);
		}

        inline void Add(const T &elem)
        {
            _arr[_curid++] = elem;
        }

		inline void Add(T &&elem)
		{
			_arr[_curid++] = std::move(elem);
		}

		inline void Swap(const fsize a, const fsize b)
		{
			_arr.Swap(a, b);
		}

		void Insert(const T& elem, const fsize pos);
		
		void Insert(T&& elem, const fsize pos);

        void RemoveAt(const fsize id);
        
        void Remove(const T &elem, const bool all = true);

		inline void Clear()
		{
			while (Size() > 0)
				RemoveLast();
		}

        inline void RemoveLast()
        {
            RemoveAt(_curid);
        }
        
        inline fsize Size() const
        {
            return (_curid);
        }

		inline const Array<T> &ToArray() const
		{
			return (_arr);
		}

        /**
         * Returns an iterator to the begining of the array
         */
        inline typename Array<T>::Iterator Begin() const
        {
            return (typename Array<T>::Iterator(*_arr, _curid, 0));
        }

        /**
         * Returns an iterator to the end of the array
         */
        inline typename Array<T>::Iterator End() const
        {
            return (typename Array<T>::Iterator(*_arr, _curid, _curid));
        }
    };
}

#include "Framework/ArrayList.impl.hpp"
