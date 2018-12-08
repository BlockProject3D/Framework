#include <ctime>
#include "Framework/Framework.hpp"
#include "Framework/System/Time.hpp"

using namespace bpf;

Time::Time(uint64 seconds)
{
    struct tm t;
    memset(&t, 0, sizeof(t));
    t.tm_sec = seconds;
    _curtm = std::mktime(&t);
    RecalcLocal();
}

Time::Time()
    : _curtm(0)
    , _year(0)
    , _day(0)
    , _month(0)
    , _hour(0)
    , _minute(0)
    , _second(0)
{
}

void Time::RecalcUTC()
{
    struct tm *tres;
    tres = std::gmtime(&_curtm);
    _year = tres->tm_year;
    _day = tres->tm_mday;
    _month = tres->tm_mon;
    _hour = tres->tm_hour;
    _minute = tres->tm_min;
    _second = tres->tm_sec;
}

void Time::RecalcLocal()
{
    struct tm *tres;
    tres = std::localtime(&_curtm);
    _year = tres->tm_year;
    _day = tres->tm_mday;
    _month = tres->tm_mon;
    _hour = tres->tm_hour;
    _minute = tres->tm_min;
    _second = tres->tm_sec;
}

String Time::GetMonthName() const
{

}

String Time::GetDayName() const
{

}

Time Time::operator+(const Time &other) const
{
    Time res;
    struct tm t;

    memset(&t, 0, sizeof(t));
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

Time Time::operator-(const Time &other) const
{
    Time res;
    struct tm t;

    memset(&t, 0, sizeof(t));
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

void Time::operator+=(const Time &other)
{
    struct tm t;

    memset(&t, 0, sizeof(t));
    t.tm_hour = _hour + other._hour;
    t.tm_min = _minute + other._minute;
    t.tm_sec = _second + other._second;
    t.tm_mon = _month + other._month;
    t.tm_mday = _day + other._day;
    t.tm_year = _year + other._year;
    _curtm = std::mktime(&t);
    RecalcLocal();
}

void Time::operator-=(const Time &other)
{
    struct tm t;

    memset(&t, 0, sizeof(t));
    t.tm_hour = _hour - other._hour;
    t.tm_min = _minute - other._minute;
    t.tm_sec = _second - other._second;
    t.tm_mon = _month - other._month;
    t.tm_mday = _day - other._day;
    t.tm_year = _year - other._year;
    _curtm = std::mktime(&t);
    RecalcLocal();
}

Time Time::ToUTCTime() const
{
    Time res;

    res._curtm = _curtm;
    res.RecalcUTC();
    return (res);
}

Time Time::ToLocalTime() const
{
    Time res;

    res._curtm = _curtm;
    res.RecalcLocal();
    return (res);
}

String Time::ToString() const
{
    return ("");
}

Time Time::UTCTime()
{
    Time res;

    res._curtm = std::time(Null);
    res.RecalcUTC();
    return (res);
}

Time Time::LocalTime()
{
    Time res;

    res._curtm = std::time(Null);
    res.RecalcLocal();
    return (res);
}
