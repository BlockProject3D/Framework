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
#include "Framework/IO/Console.hpp"
#include "Framework/IO/EConsoleStream.hpp"
#include "Framework/IO/TextWriter.hpp"

namespace bpf
{
    namespace io
    {
        /**
         * High-level console serializer
         */
        class BPF_API ConsoleWriter final : public IDataOutputStream
        {
        private:
#ifdef WINDOWS
            void *_handle;
            bool _file;
#else
            int _handle;
#endif
            TextWriter _writer;
            EConsoleStream _stream;
#ifdef WINDOWS
            void *GetHandle(const EConsoleStream type);
#else
            static int GetHandle(EConsoleStream type);
#endif

        public:
            /**
             * Constructs a ConsoleWrite
             * @param type which stream to write to (ERROR/OUTPUT)
             */
            explicit ConsoleWriter(EConsoleStream type = EConsoleStream::OUTPUT);

            inline void Flush() final
            {
                _writer.Flush();
            }

            /**
             * This function performs a low level write to console
             * On Windows it expects bufsize to be a multiple of 2 and expects buf to contain UTF-16 code points
             * On Linux it expects buf to contain UTF-8 bytes (last UTF-8 code can be cut)
             * @param buf buffer to write to console
             * @param bufsize the size in bytes of the buffer
             * @return number of bytes written, 0 if no bytes could be written
             */
            fsize Write(const void *buf, fsize bufsize) final;

            /**
             * Writes a line of text
             * @param str text to write
             */
            inline void WriteLine(const String &str)
            {
                _writer.WriteLine(str);
            }

            /**
             * Writes a string (without appending newline character)
             * @param str string to write
             */
            void Write(const String &str)
            {
                _writer.Write(str);
            }

            /**
             * Writes a platform dependent newline character
             */
            inline void NewLine()
            {
                _writer.NewLine();
            }

            inline ConsoleWriter &operator<<(uint8 u) final
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(uint16 u) final
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(uint32 u) final
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(uint64 u) final
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(int8 i) final
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(int16 i) final
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(fint i) final
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(int64 i) final
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(float f) final
            {
                _writer << f;
                return (*this);
            }

            inline ConsoleWriter &operator<<(double d) final
            {
                _writer << d;
                return (*this);
            }

            inline ConsoleWriter &operator<<(bool b) final
            {
                _writer << b;
                return (*this);
            }

            inline ConsoleWriter &operator<<(const bpf::String &str) final
            {
                _writer << str;
                return (*this);
            }

            inline ConsoleWriter &operator<<(const char *str) final
            {
                _writer << str;
                return (*this);
            }

            /**
             * Sets the text style for the subsequent write operations
             * @param style the new text style
             * @return ConsoleWriter& 
             */
            inline ConsoleWriter &operator<<(const Console::TextStyle &style)
            {
                Flush();
                Console::SetTextStyle(style, _stream);
                return (*this);
            }

            /**
             * Resets all previous text styles
             * @return ConsoleWriter& 
             */
            inline ConsoleWriter &operator<<(const Console::ClearTextStyle &)
            {
                Flush();
                Console::ResetTextStyle(_stream);
                return (*this);
            }
        };
    }
}