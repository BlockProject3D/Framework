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

#include <Framework/Collection/Stringifier.Array.hpp>
#include <Framework/IO/ConsoleReader.hpp>
#include <Framework/IO/ConsoleWriter.hpp>
#include <Framework/RuntimeException.hpp>
#include <Framework/System/Application.hpp>
#include <Framework/System/Platform.hpp>

int Main(bpf::system::Application &, const bpf::collection::Array<bpf::String> &)
{
    bpf::io::ConsoleWriter writer;
    bpf::io::ConsoleReader reader;
    const auto &newLine = bpf::system::Platform::GetOSInfo().NewLine;
    bpf::uint8 u8;
    bpf::uint16 u16;
    bpf::uint32 u32;
    bpf::uint64 u64;
    bpf::int8 i8;
    bpf::int16 i16;
    bpf::int32 i32;
    bpf::int64 i64;
    float f;
    double d;
    bool b;
    bpf::String word;

    try
    {
        writer << "Please enter a uint8: ";
        writer.Flush();
        reader >> u8;
        writer << "Please enter a uint16: ";
        writer.Flush();
        reader >> u16;
        writer << "Please enter a uint32: ";
        writer.Flush();
        reader >> u32;
        writer << "Please enter a uint64: ";
        writer.Flush();
        reader >> u64;
        writer << "Please enter a int8: ";
        writer.Flush();
        reader >> i8;
        writer << "Please enter a int16: ";
        writer.Flush();
        reader >> i16;
        writer << "Please enter a int32: ";
        writer.Flush();
        reader >> i32;
        writer << "Please enter a int64: ";
        writer.Flush();
        reader >> i64;
        writer << "Please enter a float: ";
        writer.Flush();
        reader >> f;
        writer << "Please enter a double: ";
        writer.Flush();
        reader >> d;
        writer << "Please enter a bool: ";
        writer.Flush();
        reader >> b;
        writer << "Please enter a single word: ";
        writer.Flush();
        reader >> word;
        writer << "You have entered: " << u8 << ", " << u16 << ", " << u32 << ", " << u64 << ", "
               << i8 << ", " << i16 << ", " << i32 << ", " << i64 << ", " << f << ", " << d << ", "
               << b << ", " << word << newLine;
        writer.WriteLine("Please enter a line of text:");
        writer.Flush();
        bpf::String line;
        if (!reader.ReadLine(line))
            throw 1; // Intended to cause the application to abort
        writer.WriteLine(bpf::String("Text line is: ") + line);
        writer.Flush();
        writer.WriteLine("Checking single token read: ");
        writer.Flush();
        reader.SetTokenSeparators("\r\n");
        bpf::String token;
        if (!reader.Read(token))
            throw 1; // Intended to cause the application to abort
        writer.WriteLine(bpf::String("Single token is: ") + token);
    }
    catch (const bpf::RuntimeException &e)
    {
        bpf::io::Console::WriteLine(bpf::String("An unhandled exception of type ") + e.Type() + " has occured:");
        bpf::io::Console::WriteLine(e.Message());
    }
    return (0);
}
