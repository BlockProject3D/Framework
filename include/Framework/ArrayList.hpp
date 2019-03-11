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

//TODO : Use bpf::fsize for sizes
namespace bpf
{
    template <typename T>
    class BP_TPL_API ArrayList
    {
    private:
        uint32 _curid;
        Array<T> _arr;

    public:
        inline ArrayList()
            : _curid(0), _arr(16)
        {
        }

        inline void Add(const T &elem)
        {
            _arr[_curid++] = elem;
        }

        void RemoveAt(const uint32 id);
        
        void Remove(const T &elem);

        ListNode<T> *GetNode(uint32 id) const noexcept;

        T *Get(const uint32 id) const noexcept;

        const T &operator[](const uint32 id) const;

        T &operator[](const uint32 id);

        T *GetFirst() const noexcept
        
        inline T *GetLast() const noexcept
        {
            if (_curid == 0)
                return (Null);
            return (&_arr[_curid]);
        }
        
        inline void RemoveLast()
        {
            Remove(_curid);
        }
        
        inline uint32 Size() const
        {
            return (_curid);
        }
        
        inline String ToString() const
        {
            return (_arr.ToString());
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
