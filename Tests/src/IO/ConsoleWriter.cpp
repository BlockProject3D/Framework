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

#include <Framework/IO/ConsoleWriter.hpp>
#include <Framework/Scalar.hpp>
#include <Framework/System/Platform.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

TEST(ConsoleWriter, Output)
{
    const auto &newLine = bpf::system::Platform::GetOSInfo().NewLine;
    bpf::io::ConsoleWriter console;

    console << "Max(UInt8)=" << bpf::UInt8::MaxValue << newLine
            << "Max(UInt16)=" << bpf::UInt16::MaxValue << newLine
            << "Max(UInt32)=" << bpf::UInt::MaxValue << newLine
            << "Max(Int8)=" << bpf::Int8::MaxValue << newLine
            << "Max(Int16)=" << bpf::Int16::MaxValue << newLine
            << "Max(Int32)=" << bpf::Int::MaxValue << newLine
            << "Max(Int64)=" << bpf::Int64::MaxValue << newLine;
    console.NewLine();
    console << 0.42f << ", " << 42.42 << ", " << true << newLine;
    console.Write("A test");
    console.Write(newLine);
}

TEST(ConsoleWriter, Error)
{
    const auto &newLine = bpf::system::Platform::GetOSInfo().NewLine;
    bpf::io::ConsoleWriter console(bpf::io::EConsoleStream::ERROR);

    console << "Max(UInt8)=" << bpf::UInt8::MaxValue << newLine
            << "Max(UInt16)=" << bpf::UInt16::MaxValue << newLine
            << "Max(UInt32)=" << bpf::UInt::MaxValue << newLine
            << "Max(Int8)=" << bpf::Int8::MaxValue << newLine
            << "Max(Int16)=" << bpf::Int16::MaxValue << newLine
            << "Max(Int32)=" << bpf::Int::MaxValue << newLine
            << "Max(Int64)=" << bpf::Int64::MaxValue << newLine;
    console.NewLine();
    console << 0.42f << ", " << 42.42 << ", " << true << newLine;
    console.Write("A test");
    console.Write(newLine);
}
