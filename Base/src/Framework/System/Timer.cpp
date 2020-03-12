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

#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <time.h> 
    #include <sys/time.h>   
    #include <sys/resource.h> 
#endif

#include "Framework/System/OSException.hpp"
#include "Framework/System/Timer.hpp"

using namespace bpf::system;
using namespace bpf;

Timer::Timer()
{
#ifdef WINDOWS
    LARGE_INTEGER li;
    if (QueryPerformanceFrequency(&li) == FALSE)
        throw OSException("QueryPerformanceFrequency/Counter is not available on your system");
    _perfCounterFreq = (double)li.QuadPart / 1000000.0;
    QueryPerformanceCounter(&li);
    _curCounter = li.QuadPart;
#else
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    _sec = t.tv_sec;
    _nsec = t.tv_nsec;
#endif
}

double Timer::Reset()
{
#ifdef WINDOWS
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    int64 diff = t.QuadPart - _curCounter;
    double delta = ((double)diff / _perfCounterFreq) / 1000000.0;
    _curCounter = t.QuadPart;
    return (delta);
#else
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    time_t seconds = t.tv_sec - _sec;
    long ns = t.tv_nsec - _nsec;
    double delta = (double)seconds + (double)ns / 1000000000.0;
    _sec = t.tv_sec;
    _nsec = t.tv_nsec;
    return (delta);
#endif
}