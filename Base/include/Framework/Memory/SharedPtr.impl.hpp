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
    SharedPtr<T>::~SharedPtr()
    {
        if (Count == Null)
            return;
        --*Count;
        if (*Count <= 0)
        {
            Memory::Delete(RawPtr);
            if (*WCount <= 0)
            {
                Memory::Free(Count);
                Memory::Free(WCount);
            }
        }
    }

    template <typename T>
    SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&other)
    {
        if (Count != Null)
        {
            --*Count;
            if (*Count <= 0)
            {
                Memory::Delete(RawPtr);
                if (*WCount <= 0)
                {
                    Memory::Free(Count);
                    Memory::Free(WCount);
                }
            }
        }
        Count = other.Count;
        WCount = other.WCount;
        RawPtr = other.RawPtr;
        other.Count = Null;
        other.WCount = Null;
        other.RawPtr = Null;
        return (*this);
    }

    template <typename T>
    SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &other)
    {
        if (Count != Null)
        {
            --*Count;
            if (*Count <= 0)
            {
                Memory::Delete(RawPtr);
                if (*WCount <= 0)
                {
                    Memory::Free(Count);
                    Memory::Free(WCount);
                }
            }
        }
        Count = other.Count;
        WCount = other.WCount;
        RawPtr = other.RawPtr;
        if (Count != Null)
            ++*Count;
        return (*this);
    }
}