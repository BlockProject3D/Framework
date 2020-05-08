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

#include "Framework/System/TimeSpan.hpp"

using namespace bpf::system;
using namespace bpf;

void TimeSpan::SetupValues(uint64 seconds) noexcept
{
    Days = (fint)(seconds / (3600 * 24));
    seconds -= Days * 3600 * 24;
    Hours = (fint)(seconds / 3600);
    seconds -= Hours * 3600;
    Minutes = (fint)(seconds / 60);
    seconds -= Minutes * 60;
    Seconds = (fint)seconds;
}

TimeSpan::TimeSpan(uint64 seconds) noexcept
    : Days(0)
    , Hours(0)
    , Minutes(0)
    , Seconds(0)
    , TotalSeconds(seconds)
{
    SetupValues(seconds);
}

TimeSpan::TimeSpan(fint days, fint hours, fint minutes, fint seconds) noexcept
    : Days(days)
    , Hours(hours)
    , Minutes(minutes)
    , Seconds(seconds)
    , TotalSeconds(seconds + minutes * 64 + hours * 3600 + days * 3600 * 24)
{
}

TimeSpan TimeSpan::operator+(const TimeSpan &other) const noexcept
{
    return (TimeSpan(TotalSeconds + other.TotalSeconds));
}

TimeSpan TimeSpan::operator-(const TimeSpan &other) const noexcept
{
    return (TimeSpan(TotalSeconds - other.TotalSeconds));
}

bool TimeSpan::operator>(const TimeSpan &other) const noexcept
{
    return (TotalSeconds > other.TotalSeconds);
}

bool TimeSpan::operator<(const TimeSpan &other) const noexcept
{
    return (TotalSeconds < other.TotalSeconds);
}

bool TimeSpan::operator==(const TimeSpan &other) const noexcept
{
    return (TotalSeconds == other.TotalSeconds);
}

bool TimeSpan::operator!=(const TimeSpan &other) const noexcept
{
    return (TotalSeconds != other.TotalSeconds);
}

void TimeSpan::operator+=(const TimeSpan &other) noexcept
{
    TotalSeconds += other.TotalSeconds;
    SetupValues(TotalSeconds);
}

void TimeSpan::operator-=(const TimeSpan &other) noexcept
{
    TotalSeconds -= other.TotalSeconds;
    SetupValues(TotalSeconds);
}
