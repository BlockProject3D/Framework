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
        class BPF_API TimeSpan
        {
        private:
            void SetupValues(uint64 seconds) noexcept;

        public:
            fint Days;
            fint Hours;
            fint Minutes;
            fint Seconds;
            uint64 TotalSeconds;

            explicit TimeSpan(uint64 seconds = 0) noexcept;
            explicit TimeSpan(fint days, fint hours, fint minutes, fint seconds) noexcept;

            TimeSpan operator+(const TimeSpan &other) const noexcept;
            TimeSpan operator-(const TimeSpan &other) const noexcept;
            bool operator>(const TimeSpan &other) const noexcept;
            bool operator<(const TimeSpan &other) const noexcept;
            bool operator==(const TimeSpan &other) const noexcept;
            bool operator!=(const TimeSpan &other) const noexcept;
            void operator+=(const TimeSpan &other) noexcept;
            void operator-=(const TimeSpan &other) noexcept;
        };
    }
}