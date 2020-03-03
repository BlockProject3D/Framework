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
#include "Framework/Memory/Memory.hpp"

namespace bpf
{
    namespace memory
    {
        class MemUtils
        {
        public:
            template <typename T, typename ...Args>
            inline static T *New(Args &&... args)
            {
                T *obj = static_cast<T *>(Memory::Malloc(sizeof(T)));

                new (obj) T(std::forward<Args>(args)...);
                return (obj);
            }

            template <typename T>
            inline static void Delete(T *obj)
            {
                if (obj == Null)
                    return;
                obj->~T();
                Memory::Free(obj);
            }

            template <typename T, typename ...Args>
            inline static T *NewArray(const fsize count, Args &&... args)
            {
                T *mem = static_cast<T *>(Memory::Malloc(count * sizeof(T)));
                for (fsize i = 0; i != count; ++i)
                    new (mem + i) T(std::forward<Args>(args)...);
                return (mem);
            }

            template <typename T>
            inline static void DeleteArray(T *mem, const fsize count)
            {
                for (fsize i = 0; i != count; ++i)
                    mem[i].~T();
                Memory::Free(mem);
            }

            template <typename T, typename ...Args>
            inline static T *ResizeArray(T *mem, const fsize oldCount, const fsize newCount, Args &&... args)
            {
                if (newCount == oldCount)
                    return (mem);
                if (newCount < oldCount)
                {
                    for (fsize i = newCount; i != oldCount; ++i)
                        mem[i].~T();
                    return (reinterpret_cast<T *>(Memory::Realloc(reinterpret_cast<void *>(mem), newCount * sizeof(T))));
                }
                else
                {
                    mem = reinterpret_cast<T *>(Memory::Realloc(reinterpret_cast<void *>(mem), newCount * sizeof(T)));
                    for (fsize i = oldCount; i != newCount; ++i)
                        new (mem + i) T(std::forward<Args>(args)...);
                    return (mem);
                }
            }
        };
    }
}