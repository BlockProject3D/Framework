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
#include "Framework/Collection/List.hpp"
#include "Framework/Delegate.hpp"

namespace bpf
{
    /**
     * Represents an event
     * @tparam Fn delegate signature (using functional-like notation)
     */
    template <typename Fn>
    class BP_TPL_API Event;

    /**
     * Represents an event (non const mode)
     * @tparam R the return type
     * @tparam Args the argument types
     */
    template <typename R, typename... Args>
    class BP_TPL_API Event<R(Args...)>
    {
    private:
        collection::List<Delegate<R(Args...)>> _lst;

    public:
        /**
         * Subscribes a new delegate to this event
         * @param delegate the delegate to register
         */
        inline void operator+=(Delegate<R(Args...)> &&delegate)
        {
            _lst.Add(std::move(delegate));
        }

        /**
         * Invokes this event. Delegates are automatically removed
         * @param args the arguments to pass to the delegate
         */
        inline void Invoke(Args &&... args)
        {
            for (auto it = _lst.begin(); it != _lst.end(); ++it)
            {
                if (!*it)
                    _lst.RemoveAt(it);
                if (it != _lst.end() && *it)
                    (*it)(std::forward<Args>(args)...);
            }
        }
    };

    /**
     * Represents an event (const mode)
     * @tparam R the return type
     * @tparam Args the argument types
     */
    template <typename R, typename... Args>
    class BP_TPL_API Event<R(Args...) const>
    {
    private:
        collection::List<Delegate<R(Args...) const>> _lst;

    public:
        /**
         * Subscribes a new delegate to this event
         * @param delegate the delegate to register
         */
        inline void operator+=(Delegate<R(Args...) const> &&delegate)
        {
            _lst.Add(std::move(delegate));
        }

        /**
         * Invokes this event. Delegates are automatically removed
         * @param args the arguments to pass to the delegate
         */
        inline void Invoke(Args &&... args)
        {
            for (auto &it = _lst.begin(); it != _lst.end(); ++it)
            {
                if (!*it)
                    _lst.RemoveAt(it);
                *it(std::forward<Args>(args)...);
            }
        }
    };
}
