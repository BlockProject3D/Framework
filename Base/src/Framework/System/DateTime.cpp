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

#include <cstring>
#include <ctime>
#include "Framework/System/DateTime.hpp"
#include "Framework/Scalar.hpp"

using namespace bpf::collection;
using namespace bpf::system;
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
    struct tm t;

    _day = (fint)(seconds / (3600 * 24));
    seconds -= _day * 3600 * 24;
    _hour = (fint)(seconds / 3600);
    seconds -= _hour * 3600;
    _minute = (fint)(seconds / 60);
    seconds -= _minute * 60;
    _second = (fint)seconds;
    std::memset(&t, 0, sizeof(t));
    t.tm_hour = _hour;
    t.tm_min = _minute;
    t.tm_sec = _second;
    t.tm_mon = _month;
    t.tm_mday = _day;
    t.tm_year = _year;
    _curtm = std::mktime(&t);
    RecalcLocal();
}

DateTime::DateTime(fint month, fint day, uint32 year, fint hour, fint minute, fint second)
    : _curtm(0)
    , _year(year - 1900)
    , _day(day)
    , _dayweek(0)
    , _month(month - 1)
    , _hour(hour)
    , _minute(minute)
    , _second(second)
{
    struct tm t;

    std::memset(&t, 0, sizeof(t));
    t.tm_hour = _hour;
    t.tm_min = _minute;
    t.tm_sec = _second;
    t.tm_mon = _month;
    t.tm_mday = _day;
    t.tm_year = _year;
    _curtm = std::mktime(&t);
    RecalcLocal();
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

DateTime DateTime::operator+(const TimeSpan &other) const
{
    DateTime res = *this;

    res._curtm += other.TotalSeconds;
    res.RecalcLocal();
    return (res);
}

TimeSpan DateTime::operator-(const DateTime &other) const
{
    double val = std::difftime(_curtm, other._curtm);

    return (TimeSpan((uint64)val));
}

DateTime DateTime::operator-(const TimeSpan &other) const
{
    DateTime res = *this;

    res._curtm -= other.TotalSeconds;
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
    return (val != 0);
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

DateTime DateTime::UTCNow()
{
    DateTime res;

    res._curtm = std::time(Null);
    res.RecalcUTC();
    return (res);
}

DateTime DateTime::Now()
{
    DateTime res;

    res._curtm = std::time(Null);
    res.RecalcLocal();
    return (res);
}

DateTime DateTime::Parse(const String &str)
{
    String month[] = {
        "Jan", "Feb", "Mar", "Apr",
        "May", "Jun", "Jul", "Aug",
        "Sep", "Nov", "Oct", "Dec",
    };
    DateTime res;
    Array<String> subs = str.Explode(' ');
    switch (subs.Size())
    {
    case 5:
    {
        Array<String> t = subs[4].Explode(':');
        res._hour = Int::Parse(t[0]);
        res._minute = Int::Parse(t[1]);
        res._second = Int::Parse(t[2]);
        while (month[res._month] != subs[1])
        {
            ++res._month;
            if (res._month >= 12)
                throw ParseException(String("Unknown month name ") + subs[1]);
        }
        res._day = Int::Parse(subs[2]);
        res._year = UInt::Parse(subs[3]) - 1900;
        break;
    }
    case 4:
    {
        while (month[res._month] != subs[1])
        {
            ++res._month;
            if (res._month >= 12)
                throw ParseException(String("Unknown month name ") + subs[1]);
        }
        res._day = Int::Parse(subs[2]);
        res._year = UInt::Parse(subs[3]) - 1900;
        break;
    }
    case 3:
    {
        while (month[res._month] != subs[1])
        {
            ++res._month;
            if (res._month >= 12)
                throw ParseException(String("Unknown month name ") + subs[1]);
        }
        res._day = Int::Parse(subs[2]);
        res._year = Now().GetYear() - 1900;
        break;
    }
    case 2:
    {
        Array<String> d = subs[0].Replace('/', '-').Explode('-');
        res._month = Int::Parse(d[0]) - 1;
        res._day = Int::Parse(d[1]);
        res._year = UInt::Parse(d[2]) - 1900;
        Array<String> t = subs[1].Explode(':');
        res._hour = Int::Parse(t[0]);
        res._minute = Int::Parse(t[1]);
        res._second = Int::Parse(t[2]);
        break;
    }
    case 1:
    {
        Array<String> d = subs[0].Replace('/', '-').Explode('-');
        res._month = Int::Parse(d[0]) - 1;
        res._day = Int::Parse(d[1]);
        if (d.Size() > 2)
            res._year = UInt::Parse(d[2]) - 1900;
        else
            res._year = Now().GetYear() - 1900;
        break;
    }
    }
    res.RecalcLocal();
    return (res);
}

bool DateTime::TryParse(const String &str, DateTime &date)
{
    String month[] = {
        "Jan", "Feb", "Mar", "Apr",
        "May", "Jun", "Jul", "Aug",
        "Sep", "Nov", "Oct", "Dec",
    };
    DateTime res;
    Array<String> subs = str.Explode(' ');
    switch (subs.Size())
    {
    case 5:
    {
        Array<String> t = subs[4].Explode(':');
        if (!Int::TryParse(t[0], res._hour))
            return (false);
        if (!Int::TryParse(t[1], res._minute))
            return (false);
        if (!Int::TryParse(t[2], res._second))
            return (false);
        while (month[res._month] != subs[1])
        {
            ++res._month;
            if (res._month >= 12)
                return (false);
        }
        if (!Int::TryParse(subs[2], res._day))
            return (false);
        if (!UInt::TryParse(subs[3], res._year))
            return (false);
        res._year -= 1900;
        break;
    }
    case 4:
    {
        while (month[res._month] != subs[1])
        {
            ++res._month;
            if (res._month >= 12)
                return (false);
        }
        if (!Int::TryParse(subs[2], res._day))
            return (false);
        if (!UInt::TryParse(subs[3], res._year))
            return (false);
        res._year -= 1900;
        break;
    }
    case 3:
    {
        while (month[res._month] != subs[1])
        {
            ++res._month;
            if (res._month >= 12)
                return (false);
        }
        if (!Int::TryParse(subs[2], res._day))
            return (false);
        res._year = Now().GetYear() - 1900;
        break;
    }
    case 2:
    {
        Array<String> d = subs[0].Replace('/', '-').Explode('-');
        if (!Int::TryParse(d[0], res._month))
            return (false);
        if (!Int::TryParse(d[1], res._day))
            return (false);
        if (!UInt::TryParse(d[2], res._year))
            return (false);
        res._year -= 1900;
        res._month -= 1;
        Array<String> t = subs[1].Explode(':');
        if (!Int::TryParse(t[0], res._hour))
            return (false);
        if (!Int::TryParse(t[1], res._minute))
            return (false);
        if (!Int::TryParse(t[2], res._second))
            return (false);
        break;
    }
    case 1:
    {
        Array<String> d = subs[0].Replace('/', '-').Explode('-');
        if (!Int::TryParse(d[0], res._month))
            return (false);
        if (!Int::TryParse(d[1], res._day))
            return (false);
        if (d.Size() > 2)
        {
            if (!UInt::TryParse(d[2], res._year))
                return (false);
        }
        else
            res._year = Now().GetYear();
        res._year -= 1900;
        res._month -= 1;
        break;
    }
    }
    res.RecalcLocal();
    return (true);
}
