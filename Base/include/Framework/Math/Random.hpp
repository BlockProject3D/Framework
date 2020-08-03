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
#include "Framework/Scalar.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Random number generator utility
         */
        class BPF_API Random
        {
        private:
            static fint IntBounds(fint min, fint max);

        public:
            /**
             * Constructs a random with a default seed
             */
            Random();

            /**
             * Constructs a random from a custom seed
             * @param seed random seed to use
             */
            explicit Random(long seed);

            /**
             * Generate a new random integer
             * @param max maximum value
             * @return new 32 bit integer
             */
            fint NextInt(fint max = bpf::Int::MaxValue);

            /**
             * Generate a new random integer
             * @param min minimum value
             * @param max maximum value
             * @return new 32 bit integer
             */
            fint NextInt(fint min, fint max);

            /**
             * Generate a new random integer
             * @param max maximum value
             * @return new 8 bit integer
             */
            uint8 NextByte(uint8 max = bpf::UInt8::MaxValue);

            /**
             * Generate a new random integer
             * @param min minimum value
             * @param max maximum value
             * @return new 8 bit integer
             */
            uint8 NextByte(uint8 min, uint8 max);

            /**
             * Generate a new random integer
             * @param max maximum value
             * @return new 16 bit integer
             */
            uint16 NextShort(uint16 max = bpf::UInt16::MaxValue);

            /**
             * Generate a new random integer
             * @param min minimum value
             * @param max maximum value
             * @return new 16 bit integer
             */
            uint16 NextShort(uint16 min, uint16 max);

            /**
             * Generate a new random float
             * @param min minimum value
             * @return random float between 0 and 1 added with min
             */
            float NextFloat(float min = 0);

            /**
             * Generate a new random double
             * @return random double between 0 and 1
             */
            double NextDouble();
        };
    }
}
