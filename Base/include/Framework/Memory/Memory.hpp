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
#include "Framework/Types.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/Memory/MemoryException.hpp"
#ifdef BUILD_DEBUG
    #include "Framework/System/Mutex.hpp"
#endif

namespace bpf
{
    namespace memory
    {
        /**
         * Low-level memory allocator
         */
        class BPF_API Memory
        {
#ifdef BUILD_DEBUG
        private:
            static fsize CurUsedMem;
            static fsize Allocs;
            static system::Mutex MemMutex;
#endif
        public:
            /**
             * Allocate memory.
             * WARNING: Never mix allocators
             * @param size number of bytes
             * @throw MemoryException in case allocation is impossible
             * @return pointer to allocated memory
             */
            static void *Malloc(fsize size);

            /**
             * Free allocated memory.
             * WARNING: Never mix allocators
             * @param addr pointer to allocated memory
             */
            static void Free(void *addr) noexcept;

            /**
             * Resize an already allocated memory.
             * WARNING: Never mix allocators
             * @param addr pointer to allocated memory
             * @param newsize new size to apply
             * @throw MemoryException in case re-allocation is impossible
             * @return either addr pointer or pointer to a newly allocated memory
             */
            static void *Realloc(void *addr, fsize newsize);

#ifdef BUILD_DEBUG
            /**
             * Returns the number of allocations performed currently (DEBUG only)
             * @return unsigned
             */
            inline static fsize GetAllocCount() noexcept
            {
                return (Allocs);
            }

            /**
             * Returns the current amount of memory consumed (DEBUG only)
             * @return unsigned
             */
            inline static fsize GetUsedMem() noexcept
            {
                return (CurUsedMem);
            }
#endif
        };
    }
}
