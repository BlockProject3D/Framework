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
    class BP_TPL_API Stack
    {
    private:
        T *Content;
        fsize MaxSize;
        fsize CurSize;
        fsize CurPtr;
        
    public:
        explicit Stack(fsize maxsize);
        ~Stack();
        void Push(const T &element);
        const T &Pop();

        inline const T &operator[](const fsize id) const
        {
            if (id >= MaxSize)
                throw IndexException((fint)id);
            return (Content[CurPtr + 1 + id]);
        }

        inline T &Last() const
        {
			if (Size() <= 0)
				throw IndexException(0);
            return (Content[CurPtr + 1]);
        }

        inline T &First() const
        {
			if (Size() <= 0)
				throw IndexException(0);
            return (Content[0]);
        }

        inline fsize Size() const
        {
            return (CurSize);
        }

        inline typename Array<T>::Iterator Begin() const
        {
            return (typename Array<T>::Iterator(Content, MaxSize, 0));
        }

        inline typename Array<T>::Iterator End() const
        {
            return (typename Array<T>::Iterator(Content, MaxSize, MaxSize - 1));
        }
    };
};

#include "Framework/Stack.impl.hpp"
