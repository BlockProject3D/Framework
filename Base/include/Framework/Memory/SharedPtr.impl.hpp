// Copyright (c) 2020, BlockProject 3D
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
//     * Neither the name of BlockProject 3D nor the names of its contributors
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
    namespace memory
    {
        template <typename T>
        SharedPtr<T>::~SharedPtr()
        {
            if (Count == nullptr)
                return;
            -- *Count;
            if (*Count <= 0)
            {
                MemUtils::Delete(RawPtr);
                if (*WCount <= 0)
                {
                    Memory::Free(Count);
                    Memory::Free(WCount);
                }
            }
        }

        template <typename T>
        SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&other) noexcept
        {
            if (Count != nullptr)
            {
                -- *Count;
                if (*Count <= 0)
                {
                    MemUtils::Delete(RawPtr);
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
            other.Count = nullptr;
            other.WCount = nullptr;
            other.RawPtr = nullptr;
            return (*this);
        }

        template <typename T>
        SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &other)
        {
            if (this == &other)
                return (*this);
            if (Count != nullptr)
            {
                -- *Count;
                if (*Count <= 0)
                {
                    MemUtils::Delete(RawPtr);
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
            if (Count != nullptr)
                ++ *Count;
            return (*this);
        }
    }
}
