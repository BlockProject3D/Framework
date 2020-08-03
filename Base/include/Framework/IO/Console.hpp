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
#include "Framework/IO/EConsoleColor.hpp"
#include "Framework/IO/EConsoleStream.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * Low-level console IO
         */
        class BPF_API Console
        {
        public:
            /**
             * Structure used by ConsoleWriter to reset the text style
             */
            struct ClearTextStyle
            {
            };

            /**
             * Class used to specify the text style
             */
            class TextStyle
            {
            public:
                bool Bold;
                EConsoleColor TextColor;

                inline TextStyle(EConsoleColor color, bool bold = false)
                    : Bold(bold)
                    , TextColor(color)
                {
                }
            };

            /**
             * Writes a line of text to the console
             * @param str the text to write
             * @param type which stream to write to (ERROR/OUTPUT)
             */
            static void WriteLine(const String &str, EConsoleStream type = EConsoleStream::OUTPUT) noexcept;

            /**
             * Sets the text style for the next write operations
             * @param style the new text style
             * @param type which stream to set the style (ERROR/OUTPUT)
             */
            static void SetTextStyle(const TextStyle &style, EConsoleStream type = EConsoleStream::OUTPUT) noexcept;

            /**
             * Resets the text style to the default for a given stream
             * @param type which stream to set the style (ERROR/OUTPUT)
             */
            static void ResetTextStyle(EConsoleStream type = EConsoleStream::OUTPUT) noexcept;

            /**
             * Sets the console title
             * @param title new title to apply to the console
             */
            static void SetTitle(const String &title) noexcept;

            /**
             * Checks if this console is redirected
             * @param type type of console to check
             * @return true if the given console type is redirected, false otherwise
             */
            static bool IsRedirected(EConsoleStream type = EConsoleStream::OUTPUT) noexcept;
        };
    }
}
