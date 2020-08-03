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
#include <ctime>
#include "Framework/Types.hpp"
#include "Framework/String.hpp"
#include "Framework/ParseException.hpp"
#include "Framework/System/TimeSpan.hpp"

namespace bpf
{
    namespace system
    {
        /**
         * Container to represent a date time across platforms
         */
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
            /**
             * Constructs a DateTime from individual components
             * @param month the month number
             * @param day the day number in the month
             * @param year the year number
             * @param hour the hours count
             * @param minute the minutes count
             * @param second the seconds count
             */
            DateTime(fint month, fint day, uint32 year, fint hour = 0, fint minute = 0, fint second = 0);

            /**
             * Constructs an empty DateTime
             */
            DateTime();

            /**
             * Returns the year number
             * @return unsigned 32 bits
             */
            inline uint32 GetYear() const noexcept
            {
                return (_year + 1900);
            }

            /**
             * Returns the day number in the month
             * @return signed 32 bits
             */
            inline fint GetDay() const noexcept
            {
                return (_day);
            }

            /**
             * Returns the month number
             * @return signed 32 bits
             */
            inline fint GetMonth() const noexcept
            {
                return (_month + 1);
            }

            /**
             * Returns the hours count
             * @return signed 32 bits
             */
            inline fint GetHours() const noexcept
            {
                return (_hour);
            }

            /**
             * Returns the minutes count
             * @return signed 32 bits
             */
            inline fint GetMinutes() const noexcept
            {
                return (_minute);
            }

            /**
             * Returns the seconds count
             * @return signed 32 bits
             */
            inline fint GetSeconds() const noexcept
            {
                return (_second);
            }

            /**
             * Returns the month name
             * @return high-level string
             */
            String GetMonthName() const;

            /**
             * Returns the day name
             * @return high-level string
             */
            String GetDayName() const;

            /**
             * Adds a DateTime with a TimeSpan
             * @param other TimeSpan to add with
             * @return new DateTime
             */
            DateTime operator+(const TimeSpan &other) const;

            /**
             * Subtract two DateTime
             * @param other DateTime to subtract with
             * @return new TimeSpan
             */
            TimeSpan operator-(const DateTime &other) const;

            /**
             * Subtract a DateTime with a TimeSpan
             * @param other TimeSpan to subtract with
             * @return new DateTime
             */
            DateTime operator-(const TimeSpan &other) const;

            /**
             * Compare DateTime
             * @param other operand
             * @return true if this is greater than other, false otherwise
             */
            bool operator>(const DateTime &other) const;

            /**
             * Compare DateTime
             * @param other operand
             * @return true if this is less than other, false otherwise
             */
            bool operator<(const DateTime &other) const;

            /**
             * Compare DateTime
             * @param other operand
             * @return true if this is less than or equal to other, false otherwise
             */
            inline bool operator<=(const DateTime &other) const noexcept
            {
                return (*this < other || *this == other);
            }

            /**
             * Compare DateTime
             * @param other operand
             * @return true if this is greater than or equal to other, false otherwise
             */
            inline bool operator>=(const DateTime &other) const noexcept
            {
                return (*this > other || *this == other);
            }

            /**
             * Compare DateTime
             * @param other operand
             * @return true if this is equal to other, false otherwise
             */
            bool operator==(const DateTime &other) const;

            /**
             * Compare DateTime
             * @param other operand
             * @return true if this is not equal to other, false otherwise
             */
            bool operator!=(const DateTime &other) const;

            /**
             * Converts this DateTime to a UTC time
             * @return new DateTime
             */
            DateTime ToUTCTime() const;

            /**
             * Converts this DateTime to a system localized time
             * @return new DateTime
             */
            DateTime ToLocalTime() const;

            /**
             * Gets the current system time as UTC time
             * @return new DateTime
             */
            static DateTime UTCNow();

            /**
             * Gets the current local system time
             * @return new DateTime
             */
            static DateTime Now();

            /**
             * Parse a DateTime from a string
             * @param str the string to parse
             * @throw ParseException when the string cannot be converted to a DateTime
             * @return new DateTime
             */
            static DateTime Parse(const String &str);

            /**
             * Parse a DateTime from a string
             * @param str the string to parse
             * @param date DateTime parsed from string
             * @return true if successfull, false otherwise
             */
            static bool TryParse(const String &str, DateTime &date);
        };
    }
}
