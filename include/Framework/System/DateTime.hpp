// Copyright (c) 2018, BlockProject
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
//     * Neither the name of BlockProject nor the names of its contributors
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
#include <ctime>
#include "Framework/Types.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    class BPF_API DateTime
    {
    private:
        time_t _curtm;
        uint32 _year;
        uint8 _day;
        uint8 _dayweek;
        uint8 _month;
        uint8 _hour;
        uint8 _minute;
        uint8 _second;

        void RecalcLocal();
        void RecalcUTC();

    public:
        DateTime(uint64 seconds);
        DateTime();

        inline uint32 GetYear() const noexcept
        {
            return (_year + 1900);
        }

        inline uint8 GetDay() const noexcept
        {
            return (_day);
        }

        inline uint8 GetMonth() const noexcept
        {
            return (_month + 1);
        }

        inline uint8 GetHours() const noexcept
        {
            return (_hour);
        }

        inline uint8 GetMinutes() const noexcept
        {
            return (_minute);
        }

        inline uint8 GetSeconds() const noexcept
        {
            return (_second);
        }

        String GetMonthName() const;
        String GetDayName() const;
        DateTime operator+(const DateTime &other) const;
        DateTime operator-(const DateTime &other) const;
        bool operator>(const DateTime &other) const;
        bool operator<(const DateTime &other) const;
        bool operator==(const DateTime &other) const;
        bool operator!=(const DateTime &other) const;
        void operator+=(const DateTime &other);
        void operator-=(const DateTime &other);
        String ToString() const;
        DateTime ToUTCTime() const;
        DateTime ToLocalTime() const;

        static DateTime UTCTime();
        static DateTime LocalTime();
    };
}
