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
    inline Stack<T>::Stack(int maxsize)
        : Content(new T[maxsize])
        , MaxSize(maxsize)
        , CurSize(0)
        , CurPtr(MaxSize - 1)
    {
    }

    template <typename T>
    inline Stack<T>::~Stack()
    {
        delete[] Content;
    }

    template <typename T>
    void Stack<T>::Push(const T &element)
    {
        if (CurSize >= MaxSize)
            return;
        Content[CurPtr] = T(element);
        --CurPtr;
        ++CurSize;
    }

    template <typename T>
    const T &Stack<T>::Pop()
    {
        if (CurSize > 0)
        {
            CurSize--;
            CurPtr++;
        }
        return (Content[CurPtr]);
    }

    template <typename T>
    inline const T &Stack<T>::Get(const int id) const
    {
        return (Content[CurPtr + 1 + id]);
    }

    template <typename T>
    inline const T &Stack<T>::GetLast() const
    {
        return (Content[CurPtr + 1]);
    }

    template <typename T>
    inline int Stack<T>::Size() const
    {
        return (CurSize);
    }

    template <typename T>
    inline bool Stack<T>::IsEmpty() const
    {
        return (CurSize == 0);
    }
}
