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
#include "Framework/Memory/ClassCastException.hpp"
#include "Framework/Memory/Object.hpp"
#include "Framework/Memory/RawMemberFunction.hpp"

namespace bpf
{
    namespace memory
    {
        /**
         * The object smart pointer, allows you to automatically reset all instances of ObjectPtr when a given
         * underlying instance is destroyed This class only works with types extending the Object class
         * @tparam T the type of the underlying instance (must extend bpf::memory::Object)
         */
        template <class T /* extends Object */>
        class BP_TPL_API ObjectPtr
        {
        private:
            T *RawPtr;

        public:
            /**
             * Constructs a null ObjectPtr
             */
            inline ObjectPtr()
                : RawPtr(nullptr)
            {
            }

            /**
             * Constructs an ObjectPtr from a raw pointer
             * @param raw pointer to wrap
             */
            inline ObjectPtr(T *raw)
                : RawPtr(raw)
            {
                if (RawPtr != nullptr)
                    RawPtr->AddRef((void **)&RawPtr);
            }

            /**
             * Copy constructor
             */
            template <typename T1>
            inline ObjectPtr(const ObjectPtr<T1> &other) noexcept
                : RawPtr(other.RawPtr)
            {
                if (RawPtr != nullptr)
                    RawPtr->AddRef((void **)&RawPtr);
            }

            /**
             * Copy constructor
             */
            inline ObjectPtr(const ObjectPtr<T> &other)
                : RawPtr(other.RawPtr)
            {
                if (RawPtr != nullptr)
                    RawPtr->AddRef((void **)&RawPtr);
            }

            /**
             * Move constructor
             */
            inline ObjectPtr(ObjectPtr<T> &&other) noexcept
                : RawPtr(other.RawPtr)
            {
                if (RawPtr != nullptr)
                {
                    RawPtr->AddRef((void **)&RawPtr);
                    RawPtr->RemoveRef((void **)&other.RawPtr);
                    other.RawPtr = nullptr;
                }
            }

            inline ~ObjectPtr()
            {
                if (RawPtr != nullptr)
                    RawPtr->RemoveRef((void **)&RawPtr);
            }

            /**
             * Compare ObjectPtr
             * @param other operand
             * @return true if this equal other, false otherwise
             */
            inline bool operator==(T *other) const
            {
                return (RawPtr == other);
            }

            /**
             * Compare ObjectPtr
             * @param other operand
             * @return false if this equal other, true otherwise
             */
            inline bool operator!=(T *other) const
            {
                return (RawPtr != other);
            }

            /**
             * Compare ObjectPtr
             * @param other operand
             * @return true if this equal other, false otherwise
             */
            inline bool operator==(const ObjectPtr<T> &other) const
            {
                return (RawPtr == other.RawPtr);
            }

            /**
             * Compare ObjectPtr
             * @param other operand
             * @return false if this equal other, true otherwise
             */
            inline bool operator!=(const ObjectPtr<T> &other) const
            {
                return (RawPtr != other.RawPtr);
            }

            /**
             * Compare ObjectPtr
             * @tparam T1 type to compare with
             * @param other operand
             * @return true if this equal other, false otherwise
             */
            template <typename T1>
            inline bool operator==(const ObjectPtr<T1> &other) const noexcept
            {
                return (RawPtr == other.RawPtr);
            }

            /**
             * Compare ObjectPtr
             * @tparam T1 type to compare with
             * @param other operand
             * @return false if this equal other, true otherwise
             */
            template <typename T1>
            inline bool operator!=(const ObjectPtr<T1> &other) const noexcept
            {
                return (RawPtr != other.RawPtr);
            }

            /**
             * Access the wrapped object
             * @return pointer to T
             */
            inline T *operator->() const
            {
                return (RawPtr);
            }

            /**
             * Access the wrapped object
             * @return reference to T
             */
            inline T &operator*() const
            {
                return (*RawPtr);
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
             * Assigns this smart pointer to a raw pointer
             * @param other raw pointer
             * @return reference to this
             */
            ObjectPtr<T> &operator=(T *other)
            {
                if (RawPtr != nullptr)
                    RawPtr->RemoveRef((void **)&RawPtr);
                RawPtr = other;
                if (RawPtr != nullptr)
                    RawPtr->AddRef((void **)&RawPtr);
                return (*this);
            }

            /**
             * Copy assignment operator
             */
            ObjectPtr<T> &operator=(const ObjectPtr<T> &other)
            {
                if (this == &other)
                    return (*this);
                if (RawPtr != nullptr)
                    RawPtr->RemoveRef((void **)&RawPtr);
                RawPtr = other.RawPtr;
                if (RawPtr != nullptr)
                    RawPtr->AddRef((void **)&RawPtr);
                return (*this);
            }

            /**
             * Move assignment operator
             */
            ObjectPtr<T> &operator=(ObjectPtr<T> &&other) noexcept
            {
                if (RawPtr != nullptr)
                    RawPtr->RemoveRef((void **)&RawPtr);
                RawPtr = other.RawPtr;
                if (RawPtr != nullptr)
                {
                    RawPtr->AddRef((void **)&RawPtr);
                    RawPtr->RemoveRef((void **)&other.RawPtr);
                    other.RawPtr = nullptr;
                }
                return (*this);
            }

            /**
             * Quick casting function
             * @tparam T1 the type to cast to
             * @throw ClassCastException in debug only if the class cannot be casted
             * @return new casted ObjectPtr
             */
            template <typename T1>
            inline ObjectPtr<T1> Cast() const
            {
#ifdef BUILD_DEBUG
                if (RawPtr == nullptr)
                    return (nullptr);
                else
                {
                    auto ptr = dynamic_cast<T1 *>(RawPtr);
                    if (ptr == nullptr)
                        throw ClassCastException(String("Cannot cast from ") + TypeName<T>() + " to " + TypeName<T1>());
                    return (ObjectPtr<T1>(ptr));
                }
#else
                return (ObjectPtr<T1>(static_cast<T1 *>(RawPtr)));
#endif
            }
        };
    }
}
