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
#include "Framework/ClassCastException.hpp"
#include "Framework/Memory/SharedPtr.hpp"
#include "Framework/TypeInfo.hpp"

namespace bpf
{
    namespace memory
    {
        /**
         * Weak smart pointer
         * @tparam T the type of the underlying instance
         */
        template <typename T>
        class BP_TPL_API WeakPtr
        {
        private:
            fint *Count;
            fint *WCount;
            T *RawPtr;

            inline WeakPtr(fint *c, T *raw, fint *w)
                : Count(c), WCount(w), RawPtr(raw)
            {
                if (Count != nullptr)
                    ++ *WCount;
            }

        public:
            /**
             * Constructs a WeakPtr from a SharedPtr
             * @param other shared ptr to build from
             */
            inline WeakPtr(const SharedPtr<T> &other) noexcept
                : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
            {
                if (Count != nullptr)
                    ++ *WCount;
            }

            /**
             * Copy constructor
             */
            inline WeakPtr(const WeakPtr<T> &other) noexcept
                : Count(other.Count), WCount(other.WCount), RawPtr(other.RawPtr)
            {
                if (Count != nullptr)
                    ++ *WCount;
            }

            ~WeakPtr();

            /**
             * Obtains a shared pointer from this weak pointer
             * @return new SharedPtr
             */
            SharedPtr<T> Lock() noexcept;

            /**
             * Quick casting function
             * @tparam T1 the type to cast to
             * @throw ClassCastException in debug only if the class cannot be casted
             * @return new casted WeakPtr
             */
            template <typename T1>
            inline WeakPtr<T1> Cast() const
            {
#ifdef BUILD_DEBUG
                if (RawPtr == nullptr)
                    return (nullptr);
                else
                {
                    auto ptr = dynamic_cast<T1 *>(RawPtr);
                    if (ptr == nullptr)
                        throw ClassCastException(String("Cannot cast from ") + TypeName<T>() + " to " + TypeName<T1>());
                    return (WeakPtr<T1>(Count, ptr, WCount));
                }
#else
                return (WeakPtr<T1>(Count, static_cast<T1 *>(RawPtr), WCount));
#endif
            }

            template <typename T1>
            friend class WeakPtr;
        };
    }
}

#include "Framework/Memory/WeakPtr.impl.hpp"
