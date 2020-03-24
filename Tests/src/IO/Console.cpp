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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/IO/Console.hpp>

TEST(Console, Standard)
{
    bpf::io::Console::SetTitle("BPF Unit Tests");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::BLACK, true));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::BLUE, false));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::CYAN, true));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::GREEN, false));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::MAGENTA, true));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::RED, false));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::WHITE, true));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::YELLOW, false));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::YELLOW, true));
    bpf::io::Console::WriteLine("This is a test");
    bpf::io::Console::ResetTextStyle();
    bpf::io::Console::WriteLine("Back to normal");
}

TEST(Console, Error)
{
    bpf::io::Console::SetTitle("BPF Unit Tests");
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::BLACK, true), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::BLUE, false), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::CYAN, true), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::GREEN, false), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::MAGENTA, true), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::RED, false), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::WHITE, true), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::YELLOW, false), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::SetTextStyle(bpf::io::Console::TextStyle(bpf::io::EConsoleColor::YELLOW, true), bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::WriteLine("This is a test", bpf::io::EConsoleStream::ERROR);
    bpf::io::Console::ResetTextStyle();
    bpf::io::Console::WriteLine("Back to normal", bpf::io::EConsoleStream::ERROR);
}