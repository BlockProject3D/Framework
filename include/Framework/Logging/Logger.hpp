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

#pragma once
#include "Framework/Framework.hpp"
#include "Framework/Logging/ILogHandler.hpp"
#include "Framework/Logging/ELogLevel.hpp"

namespace bpf
{
    class BPF_API Logger
    {
    private:
        List<UniquePtr<ILogHandler>> _handlers;
        String _name;

        String Format(const String &format)
        {
            return (format);
        }

        template <typename Arg, typename ...Args>
        String Format(const String &format, Arg &&arg, Args &&...args)
        {
            String res = "";

            for (size_t i = 0; i < format.Size(); ++i)
            {
                if (format[i] == '@' && ((i + 1 == format.Size()) || (i + 1 != format.Size() && format[i + 1] != '@')))
                    return (res + String::ValueOf(std::forward<Arg>(arg))
                        + Format(format.Sub(i + 1, format.Size() - i), std::forward<Args>(args)...));
                else
                    res += format[i];
            }
            return (res);
        }
    public:
        inline Logger(const String &name)
            : _name(name)
        {
        }

        inline void AddHandler(UniquePtr<ILogHandler> &&ptr)
        {
            _handlers.Add(std::move(ptr));
        }

        template <typename ...Args>
        inline void Info(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::INFO, _name, Format(format, std::forward<Args>(args)...));
        }

        template <typename ...Args>
        inline void Debug(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::DEBUG, _name, Format(format, std::forward<Args>(args)...));
        }
        
        template <typename ...Args>
        inline void Warning(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::WARNING, _name, Format(format, std::forward<Args>(args)...));
        }

        template <typename ...Args>
        inline void Error(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::ERROR, _name, Format(format, std::forward<Args>(args)...));
        }
    };
}
