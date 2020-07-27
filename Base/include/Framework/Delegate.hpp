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
#include "Framework/Memory/ObjectPtr.hpp"
#include "Framework/RuntimeException.hpp"
#include <type_traits>

namespace bpf
{
    template <typename Fn>
    class BP_TPL_API Delegate;

    template <typename R, typename... Args>
    class BP_TPL_API Delegate<R(Args...)>
    {
    private:
        using FuncType = R (memory::Object::*)(Args...);
        FuncType _member;
        memory::ObjectPtr<memory::Object> _object;

    public:
        template <typename T>
        inline Delegate(R (T::*func)(Args...), memory::ObjectPtr<T> &&thisptr) noexcept
            : _member(static_cast<FuncType>(func))
            , _object(std::move(thisptr))
        {
        }

        template <typename T>
        inline Delegate(R (T::*func)(Args...), const memory::ObjectPtr<T> &thisptr)
            : _member(static_cast<FuncType>(func))
            , _object(thisptr)
        {
        }

        template <typename T>
        inline Delegate(R (T::*func)(Args...), T *thisptr)
            : _member(static_cast<FuncType>(func))
            , _object(thisptr)
        {
        }

        inline Delegate() noexcept
            : _member(nullptr)
            , _object(nullptr)
        {
        }

        inline operator bool() const noexcept
        {
            return (_member != nullptr && _object != nullptr);
        }

        inline R operator()(Args &&... args)
        {
            if (_member == nullptr || _object == nullptr)
                throw RuntimeException("Delegate", "Attempt to call null");
            return ((_object.Raw()->*_member)(std::forward<Args>(args)...));
        }
    };

    template <typename R, typename... Args>
    class BP_TPL_API Delegate<R(Args...) const>
    {
    private:
        using FuncType = R (memory::Object::*)(Args...) const;
        FuncType _member;
        memory::ObjectPtr<memory::Object> _object;

    public:
        template <typename T>
        inline Delegate(R (T::*func)(Args...) const, memory::ObjectPtr<T> &&thisptr) noexcept
            : _member(static_cast<FuncType>(func))
            , _object(std::move(thisptr))
        {
        }

        template <typename T>
        inline Delegate(R (T::*func)(Args...) const, const memory::ObjectPtr<T> &thisptr)
            : _member(static_cast<FuncType>(func))
            , _object(thisptr)
        {
        }

        template <typename T>
        inline Delegate(R (T::*func)(Args...) const, T *thisptr)
            : _member(static_cast<FuncType>(func))
            , _object(thisptr)
        {
        }

        inline Delegate() noexcept
            : _member(nullptr)
            , _object(nullptr)
        {
        }

        inline operator bool() const noexcept
        {
            return (_member != nullptr && _object != nullptr);
        }

        inline R operator()(Args &&... args)
        {
            if (_member == nullptr || _object == nullptr)
                throw RuntimeException("Delegate", "Attempt to call null");
            return ((_object.Raw()->*_member)(std::forward<Args>(args)...));
        }
    };
}
