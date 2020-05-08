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

#include <cstring>
#include <ctime>
#include "Framework/System/DateTime.hpp"
#include "Framework/Scalar.hpp"

using namespace bpf::collection;
using namespace bpf::system;
using namespace bpf;

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
        "Wed",
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
        if (t.Size() != 3)
            throw ParseException(String("Unknown time format '") + subs[4] + '\'');
        res._hour = Int::Parse(t[0]);
        res._minute = Int::Parse(t[1]);
        res._second = Int::Parse(t[2]);
        while (month[res._month] != subs[1])
        {
            ++res._month;
            if (res._month >= 12)
                throw ParseException(String("Unknown month name '") + subs[1] + '\'');
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
                throw ParseException(String("Unknown month name '") + subs[1] + '\'');
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
                throw ParseException(String("Unknown month name '") + subs[1] + '\'');
        }
        res._day = Int::Parse(subs[2]);
        res._year = Now().GetYear() - 1900;
        break;
    }
    case 2:
    {
        Array<String> d = subs[0].Replace('/', '-').Explode('-');
        if (d.Size() != 3)
            throw ParseException(String("Unknown date format '") + subs[0] + '\'');
        res._month = Int::Parse(d[0]) - 1;
        res._day = Int::Parse(d[1]);
        res._year = UInt::Parse(d[2]) - 1900;
        Array<String> t = subs[1].Explode(':');
        if (t.Size() != 3)
            throw ParseException(String("Unknown time format '") + subs[1] + '\'');
        res._hour = Int::Parse(t[0]);
        res._minute = Int::Parse(t[1]);
        res._second = Int::Parse(t[2]);
        break;
    }
    case 1:
    {
        Array<String> d = subs[0].Replace('/', '-').Explode('-');
        if (d.Size() != 2 && d.Size() != 3)
            throw ParseException(String("Unknown date format '") + subs[0] + '\'');
        res._month = Int::Parse(d[0]) - 1;
        res._day = Int::Parse(d[1]);
        if (d.Size() > 2)
            res._year = UInt::Parse(d[2]) - 1900;
        else
            res._year = Now().GetYear() - 1900;
        break;
    }
    default:
        throw ParseException(String("Unknown date/time format '") + str + '\'');
    }
    struct tm t;
    std::memset(&t, 0, sizeof(t));
    t.tm_hour = res._hour;
    t.tm_min = res._minute;
    t.tm_sec = res._second;
    t.tm_mon = res._month;
    t.tm_mday = res._day;
    t.tm_year = res._year;
    res._curtm = std::mktime(&t);
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
        if (t.Size() != 3)
            return (false);
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
        if (d.Size() != 3)
            return (false);
        if (!Int::TryParse(d[0], res._month))
            return (false);
        if (!Int::TryParse(d[1], res._day))
            return (false);
        if (!UInt::TryParse(d[2], res._year))
            return (false);
        res._year -= 1900;
        res._month -= 1;
        Array<String> t = subs[1].Explode(':');
        if (t.Size() != 3)
            return (false);
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
        if (d.Size() != 2 && d.Size() != 3)
            return (false);
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
    default:
        return (false);
    }
    struct tm t;
    std::memset(&t, 0, sizeof(t));
    t.tm_hour = res._hour;
    t.tm_min = res._minute;
    t.tm_sec = res._second;
    t.tm_mon = res._month;
    t.tm_mday = res._day;
    t.tm_year = res._year;
    res._curtm = std::mktime(&t);
    res.RecalcLocal();
    date = res;
    return (true);
}
