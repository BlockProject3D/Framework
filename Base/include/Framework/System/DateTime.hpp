// Copyright (c) 2020, BlockProject
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
#include "Framework/System/TimeSpan.hpp"

namespace bpf
{
    namespace system
    {
        class BPF_API DateTime
        {
        private:
            time_t _curtm;
            uint32 _year;
            fint _day;
            fint _dayweek;
            fint _month;
            fint _hour;
            fint _minute;
            fint _second;

            void RecalcLocal();
            void RecalcUTC();

        public:
            explicit DateTime(uint64 seconds);
            DateTime(fint month, fint day, uint32 year, fint hour = 0, fint minute = 0, fint second = 0);
            DateTime();

            inline uint32 GetYear() const noexcept
            {
                return (_year + 1900);
            }

            inline fint GetDay() const noexcept
            {
                return (_day);
            }

            inline fint GetMonth() const noexcept
            {
                return (_month + 1);
            }

            inline fint GetHours() const noexcept
            {
                return (_hour);
            }

            inline fint GetMinutes() const noexcept
            {
                return (_minute);
            }

            inline fint GetSeconds() const noexcept
            {
                return (_second);
            }

            String GetMonthName() const;
            String GetDayName() const;
            DateTime operator+(const TimeSpan &other) const;
            TimeSpan operator-(const DateTime &other) const;
            DateTime operator-(const TimeSpan &other) const;
            bool operator>(const DateTime &other) const;
            bool operator<(const DateTime &other) const;
            bool operator==(const DateTime &other) const;
            bool operator!=(const DateTime &other) const;
            DateTime ToUTCTime() const;
            DateTime ToLocalTime() const;

            static DateTime UTCNow();
            static DateTime Now();

            static DateTime Parse(const String &str);
            static bool TryParse(const String &str, DateTime &date);
        };
    }
}
