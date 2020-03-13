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
#include <Framework/Json/Json.hpp>
#include <Framework/Json/Parser.hpp>

TEST(Json, API_1)
{
    bpf::json::Json::Object obj = {
        {"Test", 0.0},
        {"Test1", true},
        {"TestArray", bpf::json::Json::Array {"A", "B", "TrouDuCul"}},
        {"TestObject", bpf::json::Json::Object
            {
                {"a", 0},
                {"b", 0.1},
                {"c", true}
            }
        }
    };
    EXPECT_EQ((int)obj["Test"], 0);
    EXPECT_EQ(obj["Test1"], true);
    const bpf::json::Json::Array &arr = obj["TestArray"];
    EXPECT_EQ(arr[0], "A");
    EXPECT_STREQ(*arr[0].AsString(), "A");
}

TEST(Json, API_2)
{
    bpf::json::Json::Array arr = {
        bpf::json::Json::Object
        {
            {"Test", "a"},
            {"Test1", "b"}
        },
        bpf::json::Json::Object
        {
            {"Test", "a"},
            {"Test1", "b"}
        },
        bpf::json::Json::Object
        {
            {"Test", "a"},
            {"Test1", "b"}
        }
    };
    const bpf::json::Json::Object &obj = arr[0];
    EXPECT_EQ(obj["Test"], "a");
    EXPECT_EQ(obj["Test1"], "b");
    EXPECT_STREQ(*arr[0].AsObject()["Test"].AsString(), "a");
    EXPECT_STREQ(*arr[1].AsObject()["Test"].AsString(), "a");
    EXPECT_STREQ(*arr[2].AsObject()["Test"].AsString(), "a");
    EXPECT_STREQ(*arr[0].AsObject()["Test1"].AsString(), "b");
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
    bpf::json::Lexer lexer;
    lexer.LoadString(str);
    bpf::json::Json testObj = bpf::json::Parser(std::move(lexer)).Parse();
    EXPECT_EQ(testObj.Type(), bpf::json::Json::EType::OBJECT);
    bpf::json::Json::Object testObject = testObj;
    EXPECT_EQ(testObject["test"], true);
    EXPECT_EQ(testObject["test"], true);
    EXPECT_EQ(testObject["TheNull"], bpf::json::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::json::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::json::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::json::Json());
    EXPECT_EQ(testObject["TheNull"], bpf::json::Json());
    bpf::String str1 = testObject["MyString"];
    EXPECT_STREQ(*str1, "This is a true test of false and null containing 0.1 42.42 numbers and even \"strings\"");
    EXPECT_STREQ(*testObject["MyString"].AsString(), "This is a true test of false and null containing 0.1 42.42 numbers and even \"strings\"");
}