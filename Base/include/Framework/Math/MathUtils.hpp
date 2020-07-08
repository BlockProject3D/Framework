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

namespace bpf
{
    namespace math
    {
        /**
         * Special Math utilities
         */
        class BPF_API MathUtils
        {
        public:
            /**
             * Check if a number is a power of two
             * @param nb the number to check
             * @return true if nb is a power of two, false otherwise
             */
            inline static bool IsPowerOfTwo(const fsize nb)
            {
                return (nb & (nb - 1)) == 0;
            }

            /**
             * Finds the next power of two of a given number
             * @param nb the number to find the next power of two
             * @return next power of two > nb
             */
            static fsize FindNextPowerOfTwo(fsize nb);

            /**
             * Check if a number is prime
             * @param nb the number to check
             * @return true if nb is a prime number, false otherwise
             */
            static bool IsPrime(const fisize nb);

            /**
             * Finds the next prime number of a given number
             * @param nb the number to find the next prime
             * @return next prime number > nb
             */
            static fisize FindNextPrime(const fisize nb);

            /**
             * Computes the fast inverse square root of a number
             * @param nb the number to find the inverse square root
             * @return inverse square root of nb
             */
            static float InvSqrt(const float nb);
        };
    }
}