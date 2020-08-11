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
#include "Framework/Memory/MemUtils.hpp"
#include "Framework/Memory/RawMemberFunction.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/Cast.hpp"

namespace bpf
{
    namespace memory
    {
        template <typename T>
        class BP_TPL_API WeakPtr;

        /**
         * Shared smart pointer
         * @tparam T the type of the underlying instance
         */
        template <typename T>
        class BP_TPL_API SharedPtr
        {
        private:
            fint *Count;
            fint *WCount;
            T *RawPtr;

            inline SharedPtr(fint *c, fint *w, T *raw)
                : Count(c)
                , WCount(w)
                , RawPtr(raw)
            {
                if (Count != nullptr)
                    ++*Count;
            }

        public:
            /**
             * Constructs a null SharedPtr
             */
            inline SharedPtr() noexcept
                : Count(nullptr)
                , WCount(nullptr)
                , RawPtr(nullptr)
            {
            }

            /**
             * Constructs a SharedPtr from a raw pointer
             * @param raw pointer to wrap
             */
            inline SharedPtr(T *raw)
                : Count(static_cast<fint *>(Memory::Malloc(sizeof(int))))
                , WCount(static_cast<fint *>(Memory::Malloc(sizeof(int))))
                , RawPtr(raw)
            {
                *Count = 1;
                *WCount = 0;
            }

            /**
             * Move constructor
             */
            inline SharedPtr(SharedPtr<T> &&other) noexcept
                : Count(other.Count)
                , WCount(other.WCount)
                , RawPtr(other.RawPtr)
            {
                other.Count = nullptr;
                other.WCount = nullptr;
                other.RawPtr = nullptr;
            }

            /**
             * Copy constructor
             */
            template <typename T1>
            inline SharedPtr(const SharedPtr<T1> &other) noexcept
                : Count(other.Count)
                , WCount(other.WCount)
                , RawPtr(other.RawPtr)
            {
                if (Count != nullptr)
                    ++*Count;
            }

            /**
             * Copy constructor
             */
            inline SharedPtr(const SharedPtr<T> &other) noexcept
                : Count(other.Count)
                , WCount(other.WCount)
                , RawPtr(other.RawPtr)
            {
                if (Count != nullptr)
                    ++*Count;
            }

            ~SharedPtr();

            /**
             * Move assignment operator
             */
            SharedPtr<T> &operator=(SharedPtr<T> &&other) noexcept;

            /**
             * Copy assignment operator
             */
            SharedPtr<T> &operator=(const SharedPtr<T> &other);

            /**
             * Access the wrapped object
             * @return reference to T
             */
            inline T &operator*() const noexcept
            {
                return (*RawPtr);
            }

            /**
             * Access the wrapped object
             * @return pointer to T
             */
            inline T *operator->() const noexcept
            {
                return (RawPtr);
            }

            /**
             * Call a pointer to a member function on this smart pointer
             * @tparam R the function return type
             * @tparam U the object type which must be the same as T
             * @tparam Args the types of the function arguments
             * @param fn the function pointer itself
             * @return RawMemberFunction
             */
            template <typename R, typename U, typename... Args>
            inline typename std::enable_if<std::is_class<U>::value && std::is_same<T, U>::value,
                                           RawMemberFunction<U, R, R (U::*)(Args...)>>::type
            operator->*(R (U::*fn)(Args...)) const
            {
                return (RawMemberFunction<U, R, R (U::*)(Args...)>(RawPtr, fn));
            }

            /**
             * Call a pointer to a member function on this smart pointer
             * @tparam R the function return type
             * @tparam U the object type which must be the same as T
             * @tparam Args the types of the function arguments
             * @param fn the function pointer itself
             * @return RawMemberFunction
             */
            template <typename R, typename U, typename... Args>
            inline typename std::enable_if<std::is_class<U>::value && std::is_same<T, U>::value,
                                           RawMemberFunction<U, R, R (U::*)(Args...) const>>::type
            operator->*(R (U::*fn)(Args...) const) const
            {
                return (RawMemberFunction<U, R, R (U::*)(Args...) const>(RawPtr, fn));
            }

            /**
             * Returns the raw pointer
             * @return low-level raw pointer
             */
            inline T *Raw() const noexcept
            {
                return (RawPtr);
            }

            /**
             * Compare SharedPtr
             * @param other operand
             * @return true if this equal other, false otherwise
             */
            inline bool operator==(const T *other) const noexcept
            {
                return (RawPtr == other);
            }

            /**
             * Compare SharedPtr
             * @param other operand
             * @return false if this equal other, true otherwise
             */
            inline bool operator!=(const T *other) const noexcept
            {
                return (RawPtr != other);
            }

            /**
             * Compare SharedPtr
             * @tparam T1 type to compare with
             * @param other operand
             * @return true if this equal other, false otherwise
             */
            template <typename T1>
            inline bool operator==(const SharedPtr<T1> &other) const noexcept
            {
                return (RawPtr == other.RawPtr);
            }

            /**
             * Compare SharedPtr
             * @tparam T1 type to compare with
             * @param other operand
             * @return false if this equal other, true otherwise
             */
            template <typename T1>
            inline bool operator!=(const SharedPtr<T1> &other) const noexcept
            {
                return (RawPtr != other.RawPtr);
            }

            /**
             * Quick casting function
             * @tparam T1 the type to cast to
             * @throw ClassCastException in debug only if the class cannot be casted
             * @return new casted SharedPtr
             */
            template <typename T1>
            inline SharedPtr<T1> Cast() const
            {
#ifdef BUILD_DEBUG
                if (RawPtr == nullptr)
                    return (nullptr);
                else
                {
                    auto ptr = dynamic_cast<T1 *>(RawPtr);
                    if (ptr == nullptr)
                        throw ClassCastException(String("Cannot cast from ") + TypeName<T>() + " to " + TypeName<T1>());
                    return (SharedPtr<T1>(Count, WCount, ptr));
                }
#else
                return (SharedPtr<T1>(Count, WCount, static_cast<T1 *>(RawPtr)));
#endif
            }

            friend class WeakPtr<T>;

            template <typename T1>
            friend class SharedPtr;
        };
    }

    template <typename Target, typename Source>
    class CastOperator<Target, memory::SharedPtr<Source>>
    {
    public:
        using Return = memory::SharedPtr<Target>;

        inline static Return Cast(const memory::SharedPtr<Source> &source)
        {
            return (source.template Cast<Target>());
        }
    };
}

#include "Framework/Memory/SharedPtr.impl.hpp"
