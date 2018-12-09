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
