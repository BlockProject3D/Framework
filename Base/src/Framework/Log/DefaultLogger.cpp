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

#include <iostream>
#include "Framework/Log/DefaultLogger.hpp"
#include "Framework/System/DateTime.hpp"
#include "Framework/System/Stringifier.DateTime.hpp"

using namespace bpf::log;
using namespace bpf;

void DefaultLogger::LogMessage(ELogLevel level, const String &category, const String &msg)
{
    String lvl = "";

    switch (level)
    {
    case ELogLevel::DEBUG:
        lvl = "DEBUG";
        break;
    case ELogLevel::INFO:
        lvl = "INFO";
        break;
    case ELogLevel::WARNING:
        lvl = "WARNING";
        break;
    case ELogLevel::ERROR:
        lvl = "ERROR";
        break;
    }
    String res = String('(') + String::ValueOf(system::DateTime::Now()) + ")[" + category + "][" + lvl + "] " + msg;
    if (level == ELogLevel::ERROR)
        std::cerr << *res << std::endl;
    else
        std::cout << *res << std::endl;
}
