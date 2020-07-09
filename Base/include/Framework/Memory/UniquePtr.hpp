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
#include "Framework/Memory/MemUtils.hpp"
#include "Framework/Memory/ClassCastException.hpp"
#include "Framework/TypeInfo.hpp"

namespace bpf
{
    namespace memory
    {
        template <typename T>
        class BP_TPL_API UniquePtr
        {
        private:
            T *RawPtr;

        public:
            inline UniquePtr() noexcept
                : RawPtr(Null)
            {
            }

            inline UniquePtr(T *raw) noexcept
                : RawPtr(raw)
            {
            }

            inline UniquePtr(UniquePtr<T> &&other) noexcept
                : RawPtr(other.RawPtr)
            {
                other.RawPtr = Null;
            }

            template <typename T1>
            inline UniquePtr(UniquePtr<T1> &&other) noexcept
                : RawPtr(other.RawPtr)
            {
                other.RawPtr = Null;
            }

            inline ~UniquePtr()
            {
                MemUtils::Delete(RawPtr);
            }

            UniquePtr<T> &operator=(UniquePtr<T> &&other) noexcept;

            inline T &operator*() const noexcept
            {
                return (*RawPtr);
            }

            inline T *operator->() const noexcept
            {
                return (RawPtr);
            }

            inline T *Raw() const noexcept
            {
                return (RawPtr);
            }

            inline bool operator==(const T *other) const noexcept
            {
                return (RawPtr == other);
            }

            inline bool operator!=(const T *other) const noexcept
            {
                return (RawPtr != other);
            }

            template <typename T1>
            inline bool operator==(const UniquePtr<T1> &other) const noexcept
            {
                return (RawPtr == other.RawPtr);
            }

            template <typename T1>
            inline bool operator!=(const UniquePtr<T1> &other) const noexcept
            {
                return (RawPtr != other.RawPtr);
            }

            /**
             * Return a new UniquePtr containing casted pointer from T to T1,
             * WARNING: old UniquePtr will be reset to Null
             */
            template <typename T1>
            inline UniquePtr<T1> Cast()
            {
#ifdef BUILD_DEBUG
                if (RawPtr == Null)
                    return (Null);
                else
                {
                    auto ptr = dynamic_cast<T1 *>(RawPtr);
                    if (ptr == Null)
                        throw ClassCastException(String("Cannot cast from ") + TypeName<T>() + " to " + TypeName<T1>());
                    RawPtr = Null;
                    return (UniquePtr<T1>(ptr));
                }
#else
                auto ptr = static_cast<T1 *>(RawPtr);
                RawPtr = Null;
                return (UniquePtr<T1>(ptr));
#endif
            }

            template <typename T1>
            friend class UniquePtr;

            static const UniquePtr<T> NullPtr;
        };

        template <typename T>
        const UniquePtr<T> UniquePtr<T>::NullPtr = UniquePtr<T>(Null);
    }
}

#include "Framework/Memory/UniquePtr.impl.hpp"
