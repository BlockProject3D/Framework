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
    namespace system
    {
        /**
         * Represents a time span
         */
        class BPF_API TimeSpan
        {
        private:
            void SetupValues(uint64 seconds) noexcept;

        public:
            /**
             * Day count
             */
            fint Days;

            /**
             * Hour count
             */
            fint Hours;

            /**
             * Minute count
             */
            fint Minutes;

            /**
             * Second count
             */
            fint Seconds;

            /**
             * Total time contained in this span expressed as a number of seconds
             */
            uint64 TotalSeconds;

            /**
             * Constructs a TimeSpan from a number of seconds
             * @param seconds number of seconds, use 0 to denote empty TimeSpan
             */
            explicit TimeSpan(uint64 seconds = 0) noexcept;

            /**
             * Constructs a TimeSpan from individual components
             * @param days day count
             * @param hours hour count
             * @param minutes minute count
             * @param seconds second count
             */
            explicit TimeSpan(fint days, fint hours, fint minutes, fint seconds) noexcept;

            /**
             * Adds two time spans
             * @param other operand
             * @return new TimeSpan
             */
            TimeSpan operator+(const TimeSpan &other) const noexcept;

            /**
             * Subtracts two time spans
             * @param other operand
             * @return new TimeSpan
             */
            TimeSpan operator-(const TimeSpan &other) const noexcept;

            /**
             * Compare TimeSpan
             * @param other operand
             * @return true if this is greater than other, false otherwise
             */
            bool operator>(const TimeSpan &other) const noexcept;

            /**
             * Compare TimeSpan
             * @param other operand
             * @return true if this is less than other, false otherwise
             */
            bool operator<(const TimeSpan &other) const noexcept;

            /**
             * Compare TimeSpan
             * @param other operand
             * @return true if this is less than or equal to other, false otherwise
             */
            inline bool operator<=(const TimeSpan &other) const noexcept
            {
                return (*this < other || *this == other);
            }

            /**
             * Compare TimeSpan
             * @param other operand
             * @return true if this is less than or equal to other, false otherwise
             */
            inline bool operator>=(const TimeSpan &other) const noexcept
            {
                return (*this > other || *this == other);
            }

            /**
             * Compare TimeSpan
             * @param other operand
             * @return true if this is equal to other, false otherwise
             */
            bool operator==(const TimeSpan &other) const noexcept;

            /**
             * Compare TimeSpan
             * @param other operand
             * @return true if this is not equal to other, false otherwise
             */
            bool operator!=(const TimeSpan &other) const noexcept;

            /**
             * Adds a TimeSpan
             * @param other the other TimeSpan to add
             */
            void operator+=(const TimeSpan &other) noexcept;

            /**
             * Subtracts a TimeSpan
             * @param other the other TimeSpan to subtract
             */
            void operator-=(const TimeSpan &other) noexcept;
        };
    }
}