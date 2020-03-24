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

#pragma once
#include "Framework/IO/Console.hpp"
#include "Framework/IO/EConsoleStream.hpp"
#include "Framework/IO/TextWriter.hpp"

namespace bpf
{
    namespace io
    {
        class BPF_API ConsoleWriter : public IDataOutputStream
        {
        private:
#ifdef WINDOWS
            void *_handle;
            bool _file;
#else
            int _handle;
#endif
            TextWriter _writer;
#ifdef WINDOWS
            void *GetHandle(const EConsoleStream type);
#else
            int GetHandle(const EConsoleStream type);
#endif

        public:
            explicit ConsoleWriter(const EConsoleStream type = EConsoleStream::OUTPUT);

            inline void Flush()
            {
                _writer.Flush();
            }

            /**
             * This function performs a low level write to console
             * On Windows it expects bufsize to be a multiple of 2 and expects buf to contain UTF-16 code points
             * On Linux it expects buf to contain UTF-8 bytes (last UTF-8 code can be cut)
             */
            fsize Write(const void *buf, fsize bufsize);

            inline void WriteLine(const String &str)
            {
                _writer.WriteLine(str);
            }

            void Write(const String &str)
            {
                _writer.Write(str);
            }

            inline void NewLine()
            {
                _writer.NewLine();
            }

            inline ConsoleWriter &operator<<(uint8 u)
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(uint16 u)
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(uint32 u)
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(uint64 u)
            {
                _writer << u;
                return (*this);
            }

            inline ConsoleWriter &operator<<(int8 i)
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(int16 i)
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(fint i)
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(int64 i)
            {
                _writer << i;
                return (*this);
            }

            inline ConsoleWriter &operator<<(float f)
            {
                _writer << f;
                return (*this);
            }

            inline ConsoleWriter &operator<<(double d)
            {
                _writer << d;
                return (*this);
            }

            inline ConsoleWriter &operator<<(bool b)
            {
                _writer << b;
                return (*this);
            }

            inline ConsoleWriter &operator<<(const bpf::String &str)
            {
                _writer << str;
                return (*this);
            }

            inline ConsoleWriter &operator<<(const char *str)
            {
                _writer << str;
                return (*this);
            }

            inline ConsoleWriter &operator<<(const Console::TextStyle &style)
            {
                Flush();
                Console::SetTextStyle(style);
                return (*this);
            }

            inline ConsoleWriter &operator<<(const Console::ClearTextStyle &style)
            {
                Flush();
                Console::ResetTextStyle();
                return (*this);
            }
        };
    }
}