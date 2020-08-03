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
#include <utility>

namespace bpf
{
    namespace memory
    {
        /**
         * Helper class for implementing operator->* on smart pointer classes
         * @tparam T the type of this pointer
         * @tparam R the member function return type
         * @tparam Fn the member function type
         */
        template <typename T, typename R, typename Fn>
        class RawMemberFunction
        {
        private:
            T *_raw;
            Fn _funcptr;

        public:
            /**
             * Constructs a RawMemberFunction
             * @param raw the raw this pointer
             * @param funcptr the raw member function pointer
             */
            inline RawMemberFunction(T *raw, Fn funcptr)
                : _raw(raw)
                , _funcptr(funcptr)
            {
            }

            /**
             * Calls this member function
             * @tparam Args the type of arguments
             * @param args the arguments to forward
             * @return the return value of the function
             */
            template <typename ...Args>
            inline R operator()(Args &&... args)
            {
                return ((_raw->*_funcptr)(std::forward<Args>(args)...));
            }
        };
    }
}
