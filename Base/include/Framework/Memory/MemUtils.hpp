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
#include "Framework/Memory/Memory.hpp"

namespace bpf
{
    namespace memory
    {
        class MemUtils
        {
        public:
            /**
             * Allocates a new C++ object.
             * WARNING: Never mix allocators
             * @tparam T type of object to allocate
             * @tparam Args argument types to the constructor
             * @param args arguments to the constructor
             * @throw MemoryException in case allocation is impossible
             * @return pointer to new allocated object
             */
            template <typename T, typename... Args>
            inline static T *New(Args &&... args)
            {
                T *obj = static_cast<T *>(Memory::Malloc(sizeof(T)));

                new (obj) T(std::forward<Args &&>(args)...);
                return (obj);
            }

            /**
             * Frees a C++ object.
             * WARNING: Never mix allocators
             * @tparam T type of object to free
             * @param obj pointer to object
             */
            template <typename T>
            inline static void Delete(T *obj) noexcept
            {
                if (obj == nullptr)
                    return;
                obj->~T();
                Memory::Free(obj);
            }

            /**
             * Allocates an array of C++ objects.
             * WARNING: Never mix allocators
             * @tparam T type of object to allocate
             * @tparam Args argument types to the constructor
             * @param count number of objects to store in the array
             * @param args arguments to the constructor
             * @throw MemoryException in case allocation is impossible
             * @return pointer to new allocated array
             */
            template <typename T, typename... Args>
            inline static T *NewArray(const fsize count, Args &&... args)
            {
                T *mem = static_cast<T *>(Memory::Malloc(count * sizeof(T)));
                for (fsize i = 0; i != count; ++i)
                    new (mem + i) T(std::forward<Args &&>(args)...);
                return (mem);
            }

            /**
             * Frees an array of C++ objects.
             * WARNING: Never mix allocators
             * @tparam T type of object to free
             * @param mem pointer to allocated array
             * @param count number of objects stored in the array
             */
            template <typename T>
            inline static void DeleteArray(T *mem, const fsize count) noexcept
            {
                for (fsize i = 0; i != count; ++i)
                    mem[i].~T();
                Memory::Free(mem);
            }

            /**
             * Resize an array of C++ objects.
             * WARNING: Never mix allocators
             * @tparam T type of object stored in the array
             * @tparam Args argument types to the constructor
             * @param mem pointer to allocated array
             * @param oldCount current number of objects stored in the array
             * @param newCount new wanted number objects to store in the array
             * @param args arguments to the constructor
             * @throw MemoryException in case allocation is impossible
             * @return pointer same array pointer or pointer to a newly allocated array
             */
            template <typename T, typename... Args>
            inline static T *ResizeArray(T *mem, const fsize oldCount, const fsize newCount, Args &&... args)
            {
                if (newCount == oldCount)
                    return (mem);
                if (newCount < oldCount)
                {
                    for (fsize i = newCount; i != oldCount; ++i)
                        mem[i].~T();
                    return (reinterpret_cast<T *>(
                        Memory::Realloc(reinterpret_cast<void *>(mem), (newCount + 1) * sizeof(T))));
                }
                else
                {
                    mem = reinterpret_cast<T *>(Memory::Realloc(reinterpret_cast<void *>(mem), newCount * sizeof(T)));
                    for (fsize i = oldCount; i != newCount; ++i)
                        new (mem + i) T(std::forward<Args &&>(args)...);
                    return (mem);
                }
            }
        };
    }
}