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

#pragma once
#include "Framework/Memory/Utility.hpp"
#include "Framework/Collection/List.hpp"
#include "Framework/Log/ILogAdapter.hpp"
#include "Framework/Log/ELogLevel.hpp"

namespace bpf
{
    namespace log
    {
        /**
         * Utility to handle message logging
         */
        class BPF_API Logger
        {
        private:
            collection::List<memory::UniquePtr<ILogAdapter>> _handlers;
            String _name;
            ELogLevel _level;

            template <typename ...Args>
            inline void LogMessage(const ELogLevel level, const String &format, Args &&...args)
            {
                if (level > _level)
                    return;
                for (auto &ptr : _handlers)
                    ptr->LogMessage(level, _name, String::Format(format, std::forward<Args &&>(args)...));
            }
        public:
            /**
             * Constructs a Logger
             * @param name the category name
             */
            explicit Logger(String name);

            /**
             * Sets the maximum log level. All log messages that are greater than this level are ignored.
             * The default is ELogLevel::DEBUG is debug builds and INFO in release builds
             * @param level the new maximum log level
             */
            inline void SetLevel(const ELogLevel level)
            {
                _level = level;
            }

            /**
             * Move constructor
             */
            Logger(Logger &&other) noexcept;

            /**
             * Move assignment operator
             */
            Logger &operator=(Logger &&other) noexcept;

            /**
             * Explicit deleted copy constructor (MSVC Fix: for some reasons MSVC is unable to identify this class cannot be coppied)
             */
            Logger(const Logger &other) = delete;

            /**
             * Explicit deleted copy assignment operator (MSVC Fix: for some reasons MSVC is unable to identify this class cannot be coppied)
             */
            Logger &operator=(const Logger &other) = delete;

            /**
             * Adds a new log adapter
             * @param ptr UniquePtr to the new ILogAdapter
             */
            void AddHandler(memory::UniquePtr<ILogAdapter> &&ptr);

            /**
             * Logs an information message
             * @tparam Args the type of arguments
             * @param format the format (see bpf::String::Format for more information)
             * @param args the argument values
             */
            template <typename ...Args>
            inline void Info(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::INFO, format, std::forward<Args &>(args)...);
            }

            /**
             * Logs a debug message
             * @tparam Args the type of arguments
             * @param format the format (see bpf::String::Format for more information)
             * @param args the argument values
             */
            template <typename ...Args>
            inline void Debug(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::DEBUG, format, std::forward<Args &&>(args)...);
            }

            /**
             * Logs a warning message
             * @tparam Args the type of arguments
             * @param format the format (see bpf::String::Format for more information)
             * @param args the argument values
             */
            template <typename ...Args>
            inline void Warning(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::WARNING, format, std::forward<Args &&>(args)...);
            }

            /**
             * Logs an error message
             * @tparam Args the type of arguments
             * @param format the format (see bpf::String::Format for more information)
             * @param args the argument values
             */
            template <typename ...Args>
            inline void Error(const String &format, Args &&...args)
            {
                LogMessage(ELogLevel::ERROR, format, std::forward<Args &&>(args)...);
            }
        };
    }
}
