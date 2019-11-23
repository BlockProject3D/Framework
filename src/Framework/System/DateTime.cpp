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

#include <cstring>
#include <ctime>
#include "Framework/System/DateTime.hpp"

using namespace bpf;

DateTime::DateTime(uint64 seconds)
    : _curtm(0)
    , _year(0)
    , _day(0)
    , _dayweek(0)
    , _month(0)
    , _hour(0)
    , _minute(0)
    , _second(0)
{
    _day = (fint)(seconds / (3600 * 24));
    seconds -= _day * 3600 * 24;
    _hour = (fint)(seconds / 3600);
    seconds -= _hour * 3600;
    _minute = (fint)(seconds / 60);
    seconds -= _minute * 60;
    _second = (fint)seconds;
}

DateTime::DateTime()
    : _curtm(0)
    , _year(0)
    , _day(0)
    , _dayweek(0)
    , _month(0)
    , _hour(0)
    , _minute(0)
    , _second(0)
{
}

void DateTime::RecalcUTC()
{
    struct tm *tres;
#ifdef WINDOWS
    struct tm wtt;
    gmtime_s(&wtt, &_curtm);
    tres = &wtt;
#else
    tres = std::gmtime(&_curtm);
#endif
    _year = tres->tm_year;
    _day = tres->tm_mday;
    _month = tres->tm_mon;
    _hour = tres->tm_hour;
    _minute = tres->tm_min;
    _second = tres->tm_sec;
    _dayweek = tres->tm_wday;
}

void DateTime::RecalcLocal()
{
    struct tm *tres;
#ifdef WINDOWS
    struct tm wtt;
    localtime_s(&wtt, &_curtm);
    tres = &wtt;
#else
    tres = std::localtime(&_curtm);
#endif
    _year = tres->tm_year;
    _day = tres->tm_mday;
    _month = tres->tm_mon;
    _hour = tres->tm_hour;
    _minute = tres->tm_min;
    _second = tres->tm_sec;
    _dayweek = tres->tm_wday;
}

String DateTime::GetMonthName() const
{
    String month[] = {
        "Jan", "Feb", "Mar", "Apr",
        "May", "Jun", "Jul", "Aug",
        "Sep", "Nov", "Oct", "Dec",
    };

    return (month[_month]);
}

String DateTime::GetDayName() const
{
    String days[] = {
        "Sun",
        "Mon",
        "Tue",
        "Thu",
        "Fri",
        "Sat"
    };

    return (days[_dayweek]);
}

DateTime DateTime::operator+(const DateTime &other) const
{
    DateTime res;
    struct tm t;

    std::memset(&t, 0, sizeof(t));
    t.tm_hour = _hour + other._hour;
    t.tm_min = _minute + other._minute;
    t.tm_sec = _second + other._second;
    t.tm_mon = _month + other._month;
    t.tm_mday = _day + other._day;
    t.tm_year = _year + other._year;
    res._curtm = std::mktime(&t);
    res.RecalcLocal();
    return (res);
}

DateTime DateTime::operator-(const DateTime &other) const
{
    DateTime res;
    struct tm t;

    std::memset(&t, 0, sizeof(t));
    t.tm_hour = _hour - other._hour;
    t.tm_min = _minute - other._minute;
    t.tm_sec = _second - other._second;
    t.tm_mon = _month - other._month;
    t.tm_mday = _day - other._day;
    t.tm_year = _year - other._year;
    res._curtm = std::mktime(&t);
    res.RecalcLocal();
    return (res);
}

bool DateTime::operator>(const DateTime &other) const
{
    double val = std::difftime(_curtm, other._curtm);
    return (val > 0);
}

bool DateTime::operator<(const DateTime &other) const
{
    double val = std::difftime(_curtm, other._curtm);
    return (val < 0);
}

bool DateTime::operator==(const DateTime &other) const
{
    double val = std::difftime(_curtm, other._curtm);
    return (val == 0);
}

bool DateTime::operator!=(const DateTime &other) const
{
    double val = std::difftime(_curtm, other._curtm);
    return (val == 0);
}

void DateTime::operator+=(const DateTime &other)
{
    struct tm t;

    std::memset(&t, 0, sizeof(t));
    t.tm_hour = _hour + other._hour;
    t.tm_min = _minute + other._minute;
    t.tm_sec = _second + other._second;
    t.tm_mon = _month + other._month;
    t.tm_mday = _day + other._day;
    t.tm_year = _year + other._year;
    _curtm = std::mktime(&t);
    RecalcLocal();
}

void DateTime::operator-=(const DateTime &other)
{
    struct tm t;

    std::memset(&t, 0, sizeof(t));
    t.tm_hour = _hour - other._hour;
    t.tm_min = _minute - other._minute;
    t.tm_sec = _second - other._second;
    t.tm_mon = _month - other._month;
    t.tm_mday = _day - other._day;
    t.tm_year = _year - other._year;
    _curtm = std::mktime(&t);
    RecalcLocal();
}

DateTime DateTime::ToUTCTime() const
{
    DateTime res;

    res._curtm = _curtm;
    res.RecalcUTC();
    return (res);
}

DateTime DateTime::ToLocalTime() const
{
    DateTime res;

    res._curtm = _curtm;
    res.RecalcLocal();
    return (res);
}

DateTime DateTime::UTCTime()
{
    DateTime res;

    res._curtm = std::time(Null);
    res.RecalcUTC();
    return (res);
}

DateTime DateTime::LocalTime()
{
    DateTime res;

    res._curtm = std::time(Null);
    res.RecalcLocal();
    return (res);
}
