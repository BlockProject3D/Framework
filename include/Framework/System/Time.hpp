#pragma once
#include <ctime>
#include "Framework/Types.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    class Time
    {
    private:
        time_t _curtm;
        uint32 _year;
        uint8 _day;
        uint8 _month;
        uint8 _hour;
        uint8 _minute;
        uint8 _second;

        void RecalcLocal();
        void RecalcUTC();

    public:
        Time(uint64 seconds);
        Time();

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
        Time operator+(const Time &other) const;
        Time operator-(const Time &other) const;
        void operator+=(const Time &other);
        void operator-=(const Time &other);
        String ToString() const;
        Time ToUTCTime() const;
        Time ToLocalTime() const;

        static Time UTCTime();
        static Time LocalTime();
    };
}
