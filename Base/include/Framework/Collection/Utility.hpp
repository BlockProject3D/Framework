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

namespace bpf
{
    namespace collection
    {
        /**
         * Usefull operators used by various collection types
         */
        namespace ops
        {
            /**
             * Default less than operator
             * @tparam T the type of value to compare
             */
            template <typename T>
            class Less
            {
            public:
                inline static bool Eval(const T &a, const T &b)
                {
                    return (a < b);
                }
            };

            /**
             * Default greater than operator
             * @tparam T the type of value to compare
             */
            template <typename T>
            class Greater
            {
            public:
                inline static bool Eval(const T &a, const T &b)
                {
                    return (a > b);
                }
            };

            /**
             * Default less-or-equal operator
             * @tparam T the type of value to compare
             */
            template <typename T>
            class LessOrEqual
            {
            public:
                inline static bool Eval(const T &a, const T &b)
                {
                    return (a <= b);
                }
            };

            /**
             * Default greater-or-equal operator
             * @tparam T the type of value to compare
             */
            template <typename T>
            class GreaterOrEqual
            {
            public:
                inline static bool Eval(const T &a, const T &b)
                {
                    return (a >= b);
                }
            };

            /**
             * Default equal operator
             * @tparam T the type of value to compare
             */
            template <typename T>
            class Equal
            {
            public:
                inline static bool Eval(const T &a, const T &b)
                {
                    return (a == b);
                }
            };
        }

        /**
         * Default minimizing operator for PriorityQueue
         * @tparam T the type of value to compare
         */
        template <typename T>
        using MinHeap = ops::LessOrEqual<T>;

        /**
         * Default maximizing operator for PriorityQueue
         * @tparam T the type of value to compare
         */
        template <typename T>
        using MaxHeap = ops::GreaterOrEqual<T>;
    }
}
