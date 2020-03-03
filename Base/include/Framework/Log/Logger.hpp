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
#include "Framework/Memory/Utility.hpp"
#include "Framework/Collection/List.hpp"
#include "Framework/Log/ILogHandler.hpp"
#include "Framework/Log/ELogLevel.hpp"

namespace bpf
{
    namespace log
    {
        class BPF_API Logger
        {
        private:
            collection::List<memory::UniquePtr<ILogHandler>> _handlers;
            String _name;

            template <typename Arg, typename ...Args>
            inline String LogMessage(const ELogLevel level, const String &format, Args &&...args)
            {
                for (auto &ptr : _handlers)
                    ptr->LogMessage(level, _name, String::Format(format, std::forward<Args>(args)...));
            }
        public:
            explicit inline Logger(const String &name)
                : _name(name)
            {
            }

            inline void AddHandler(memory::UniquePtr<ILogHandler> &&ptr)
            {
                _handlers.Add(std::move(ptr));
            }

            template <typename ...Args>
            inline void Info(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::INFO, _name, Format(format, std::forward<Args>(args)...));
            }

            template <typename ...Args>
            inline void Debug(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::DEBUG, _name, Format(format, std::forward<Args>(args)...));
            }

            template <typename ...Args>
            inline void Warning(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::WARNING, _name, Format(format, std::forward<Args>(args)...));
            }

            template <typename ...Args>
            inline void Error(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::ERROR, _name, Format(format, std::forward<Args>(args)...));
            }
        };
    }
}
