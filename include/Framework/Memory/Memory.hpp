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
#include "Framework/Types.hpp"
#include "Framework/Memory/MemoryException.hpp"
#ifdef BUILD_DEBUG
    #include "Framework/System/Mutex.hpp"
#endif

#define Null nullptr

namespace bpf
{
    class BPF_API Memory
    {
#ifdef BUILD_DEBUG
    private:
        static fsize CurUsedMem;
        static fsize Allocs;
        static Mutex MemMutex;
#endif
    public:
        static void *Malloc(fsize size);
        static void Free(void *addr);
        static void *Realloc(void *addr, fsize newsize);

        template <typename T, typename ...Args>
        inline static T *New(Args&&... args)
        {
            T *obj = static_cast<T *>(Malloc(sizeof(T)));

            new (obj) T(args...);
            return (obj);
        }

        template <typename T>
        inline static void Delete(T *obj)
        {
            if (obj == Null)
                return;
            obj->~T();
            Free(obj);
        }

#ifdef BUILD_DEBUG
        inline static fsize GetAllocCount() noexcept
        {
            return (Allocs);
        }
        inline static fsize GetUsedMem() noexcept
        {
            return (CurUsedMem);
        }
#endif
    };
};

#include "Framework/Memory/UniquePtr.hpp"
#include "Framework/Memory/SharedPtr.hpp"
#include "Framework/Memory/WeakPtr.hpp"
#include "Framework/Memory/Utility.hpp"

#include "Framework/Memory/SharedPtr.impl.hpp"
#include "Framework/Memory/WeakPtr.impl.hpp"
#include "Framework/Memory/UniquePtr.impl.hpp"
