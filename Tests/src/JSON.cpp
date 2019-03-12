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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/JSON/Json.hpp>
#include <Framework/JSON/JsonParser.hpp>

TEST(Json, API_1)
{
    bpf::Json::Object obj = {
        {"Test", 0.0},
        {"Test1", true},
        {"TestArray", bpf::Json::Array {"A", "B", "TrouDuCul"}},
        {"TestObject", bpf::Json::Object
            {
                {"a", 0.0},
                {"b", 0.1},
                {"c", true}
            }
        }
    };
    EXPECT_EQ(obj["Test"], 0.0);
    EXPECT_EQ(obj["Test1"], true);
    const bpf::Json::Array &arr = obj["TestArray"];
    EXPECT_EQ(arr[0], "A");
}

TEST(Json, API_2)
{
    bpf::Json::Array arr = {
        bpf::Json::Object
        {
            {"Test", "a"},
            {"Test1", "b"}
        },
        bpf::Json::Object
        {
            {"Test", "a"},
            {"Test1", "b"}
        },
        bpf::Json::Object
        {
            {"Test", "a"},
            {"Test1", "b"}
        }
    };
}

TEST(Json, LexerParser)
{
    bpf::String str =
        "{"
        "   \"test\": true,"
        "   \"TheNull\": null,"
        "   \"Array\": [ true, false, null, 0.1, 1, 42, 42.42e2, 42.42e-2 ],"
        "   \"MyString\": \"This is a true test of false and null containing 0.1 42.42 numbers and even \\\"strings\\\"\""
        "}";
    bpf::JsonLexer lexer;
    lexer.Push(str);
    bpf::Json testObj = bpf::JsonParser(std::move(lexer)).Parse();
    EXPECT_EQ(testObj.Type(), bpf::Json::EType::OBJECT);
    bpf::Json::Object testObject = testObj;
    EXPECT_EQ(testObject["test"], true);
    EXPECT_EQ(testObject["test"], true);
    EXPECT_EQ(testObject["TheNull"], bpf::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::Json());
    bpf::String str1 = testObject["MyString"];
    EXPECT_EQ(str1, "This is a true test of false and null containing 0.1 42.42 numbers and even \"strings\"");
    EXPECT_EQ(testObject["MyString"], "This is a true test of false and null containing 0.1 42.42 numbers and even \"strings\"");
}