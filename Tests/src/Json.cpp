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
#include <Framework/Json/Json.hpp>
#include <Framework/Json/Parser.hpp>
#include <Framework/Json/Stringifier.Json.hpp>

using J = bpf::json::Json;

TEST(Json, API_1)
{
    J::Object obj{
        {"Test", 0.0},
        {"Test1", true},
        {"TestArray", J::Array {"A", "B", "TrouDuCul"}},
        {"TestObject", J::Object
            {
                {"a", 0.0},
                {"b", 0.1},
                {"c", true}
            }
        }
    };

    EXPECT_EQ(obj["Test"], 0.0);
    EXPECT_FALSE(obj["Test"] == true);
    EXPECT_FALSE(obj["Test"] == "test");
    EXPECT_FALSE(obj["Test"] == bpf::String("test"));
    EXPECT_EQ(obj["Test1"], true);
    EXPECT_FALSE(obj["Test1"] == 0.0);
    const J::Object &objConst = obj;
    const J::Array &carr = objConst["TestArray"];
    EXPECT_EQ(carr[0], "A");
    EXPECT_STREQ(*carr[0].AsString(), "A");
    J::Array &arr = obj["TestArray"];
    J v = 42.42;
    arr.Add(42.42);
    arr.Add(v);
    EXPECT_EQ(carr[3], 42.42);
    EXPECT_EQ(carr[4], 42.42);
    arr.RemoveAt(4);
    arr.RemoveAt(0);
    EXPECT_EQ(carr.Size(), 3);
    EXPECT_EQ(arr.Size(), 3);
    arr.Items().Clear();
    EXPECT_EQ(carr.Size(), 0);
    EXPECT_EQ(arr.Size(), 0);
    const J::Object &cobj = objConst["TestObject"];
    EXPECT_EQ(cobj["a"], 0.0);
    EXPECT_EQ(cobj["b"], 0.1);
    EXPECT_EQ(cobj["c"], true);
    J::Object &mobj = obj["TestObject"];
    mobj.Add("test", 42.42);
    mobj.Add("test1", v);
    EXPECT_EQ(cobj["test"], 42.42);
    EXPECT_EQ(mobj["test"], 42.42);
    EXPECT_EQ(cobj["test1"], 42.42);
    EXPECT_EQ(mobj["test1"], 42.42);
    mobj.RemoveAt("test1");
    mobj.RemoveAt("a");
    EXPECT_EQ(mobj.Size(), 3);
    EXPECT_EQ(cobj.Size(), 3);
    mobj.Properties().Clear();
    EXPECT_EQ(mobj.Size(), 0);
    EXPECT_EQ(cobj.Size(), 0);
}

TEST(Json, API_2)
{
    bpf::collection::Map<bpf::String, J> vals = { {"Test", "a"}, {"Test1", "b"} };
    J::Array arr{
        J::Object
        {
            {"Test", "a"},
            {"Test1", "b"}
        },
        J::Object(bpf::collection::Map<bpf::String, J> { {"Test", "a"}, {"Test1", "b"} }),
        J::Object(vals)
    };

    const J::Object &obj = arr[0];
    EXPECT_EQ(obj["Test"], "a");
    EXPECT_EQ(obj["Test1"], "b");
    EXPECT_EQ(obj["Test1"], bpf::String("b"));
    EXPECT_STREQ(*arr[0].AsObject()["Test"].AsString(), "a");
    EXPECT_STREQ(*arr[1].AsObject()["Test"].AsString(), "a");
    EXPECT_STREQ(*arr[2].AsObject()["Test"].AsString(), "a");
    EXPECT_STREQ(*arr[0].AsObject()["Test1"].AsString(), "b");
    for (auto &json : arr)
    {
        EXPECT_EQ(json.Type(), J::EType::OBJECT);
        EXPECT_EQ(json.AsObject()["Test"], "a");
        EXPECT_EQ(json.AsObject()["Test1"], "b");
        const J::Object &jobj = json;
        for (auto &sjson : jobj)
        {
            EXPECT_EQ(sjson.Value.Type(), J::EType::STRING);
            EXPECT_TRUE(sjson.Key == "Test" || sjson.Key == "Test1");
        }
    }
    for (auto &json : bpf::collection::Reverse(arr))
    {
        EXPECT_EQ(json.Type(), J::EType::OBJECT);
        EXPECT_EQ(json.AsObject()["Test"], "a");
        EXPECT_EQ(json.AsObject()["Test1"], "b");
        const J::Object &jobj = json;
        for (auto &sjson : bpf::collection::Reverse(jobj))
        {
            EXPECT_EQ(sjson.Value.Type(), J::EType::STRING);
            EXPECT_TRUE(sjson.Key == "Test" || sjson.Key == "Test1");
        }
    }
}

TEST(Json, API_3)
{
    J::Object obj{
        {"Property1", true},
        {"Property2", 0.0},
        {"Property1", false},
        {"Property2", 42.0},
    };

    EXPECT_EQ(obj["Property1"], false);
    EXPECT_EQ(obj["Property2"], 42.0);
}

TEST(Json, API_4_1)
{
    J::Object myObj = J::Object
    {
        {"MyBool", true},
        {"MyVec", J::Array {0.0, 1.0, 0.0}}
    };

    EXPECT_EQ(myObj["MyBool"], true);
    EXPECT_EQ(myObj["MyVec"].AsArray()[0], 0.0);
    EXPECT_EQ(myObj["MyVec"].AsArray()[1], 1.0);
    EXPECT_EQ(myObj["MyVec"].AsArray()[2], 0.0);
}

TEST(Json, API_4_2)
{
    J::Object myObj = J::Object
    {
        {"MyVec", J::Array(bpf::collection::List<J> { 0.0, 1.0, 0.0})}
    };

    EXPECT_EQ(myObj["MyVec"].AsArray()[0], 0.0);
    EXPECT_EQ(myObj["MyVec"].AsArray()[1], 1.0);
    EXPECT_EQ(myObj["MyVec"].AsArray()[2], 0.0);
}

TEST(Json, API_4_3)
{
    bpf::collection::List<J> lst = { 0.0, 1.0, 0.0 };
    J::Object myObj = J::Object
    {
        {"MyVec", J::Array(lst)}
    };

    EXPECT_EQ(myObj["MyVec"].AsArray()[0], 0.0);
    EXPECT_EQ(myObj["MyVec"].AsArray()[1], 1.0);
    EXPECT_EQ(myObj["MyVec"].AsArray()[2], 0.0);
}

TEST(Json, API_5)
{
    auto s = bpf::String("Test");
    J::Object myObj = J::Object
    {
        {"MyStr", bpf::String("Test")},
        {"MyStr1", s}
    };

    EXPECT_STREQ(*myObj["MyStr"].AsString(), "Test");
    EXPECT_STREQ(*myObj["MyStr1"].AsString(), "Test");
}

TEST(Json, API_6)
{
    J::Object myObj = J::Object
    {
        {"MyStr", "Test"},
    };
    J::Array myArr = J::Array{ 1.0, true };
    J val = myObj;
    J val1 = myArr;
    const J &valConst = val;
    const J &val1Const = val1;

    EXPECT_THROW((void)((const bpf::String &)val), bpf::json::JsonException);
    EXPECT_THROW(val.AsString(), bpf::json::JsonException);
    EXPECT_THROW((void)((const J::Array &)valConst), bpf::json::JsonException);
    EXPECT_THROW(val.AsArray(), bpf::json::JsonException);
    EXPECT_THROW((void)((J::Array &)val), bpf::json::JsonException);
    EXPECT_THROW((void)((const J::Object &)val1Const), bpf::json::JsonException);
    EXPECT_THROW((void)((J::Object &)val1), bpf::json::JsonException);
    EXPECT_THROW(val1.AsObject(), bpf::json::JsonException);
    EXPECT_THROW((void)((double)val), bpf::json::JsonException);
    EXPECT_THROW(val.AsNumber(), bpf::json::JsonException);
    EXPECT_THROW((void)((bool)val), bpf::json::JsonException);
    EXPECT_THROW(val.AsBool(), bpf::json::JsonException);
    EXPECT_STREQ(*val.AsObject()["MyStr"].AsString(), "Test");
    EXPECT_EQ(val1.AsArray()[0], 1.0);
    val = "this is a test";
    EXPECT_STREQ(*val.AsString(), "this is a test");
    double d = myArr[0];
    EXPECT_EQ(d, 1.0);
    bool b = myArr[1];
    EXPECT_EQ(b, true);
}

TEST(Json, LexerParser)
{
    bpf::String str =
        "{"
        "   \"test\": true,\n"
        "   \"TheNull\": null,\r\n"
        "   \"-42\": -42,"
        "   \"Array\": [ true, false, null, 0.1, 1, 42, 42.42e2, 42.42e-2 ],"
        "   \"AdvTestStr\": \"String with special \b\t characters\b\t.\","
        "   \"Unicode\": \"\\u233\","
        "   \"Special\": \"\\t\\b\","
        "   \"MyString\": \"This is a true test of false and null containing 0.1 42.42 numbers and even \\\"strings\\\"\""
        "}";

    bpf::json::Lexer lexer;
    lexer.LoadString(str);
    J testObj = bpf::json::Parser(std::move(lexer)).Parse();
    EXPECT_EQ(testObj.Type(), bpf::json::Json::EType::OBJECT);
    J::Object testObject = testObj;
    EXPECT_EQ(testObject["test"], true);
    EXPECT_EQ(testObject["test"], true);
    EXPECT_EQ(testObject["-42"], -42.0);
    EXPECT_EQ(testObject["TheNull"], Null);
    EXPECT_EQ(testObject["TheNull"], Null);
    EXPECT_EQ(testObject["TheNull"], Null);
    EXPECT_EQ(testObject["TheNull"], Null);
    EXPECT_EQ(testObject["TheNull"], Null);
    bpf::String str1 = testObject["MyString"];
    EXPECT_STREQ(*str1, "This is a true test of false and null containing 0.1 42.42 numbers and even \"strings\"");
    EXPECT_STREQ(*testObject["MyString"].AsString(), "This is a true test of false and null containing 0.1 42.42 numbers and even \"strings\"");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Array"].AsArray()), "[TRUE, FALSE, null, 0.1, 1, 42, 4242, 0.4242]");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["AdvTestStr"]), "String with special \b\t characters\b\t.");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Unicode"]), "é");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Special"]), "\t\b");
}

TEST(Json, Lexer_Err)
{
    bpf::json::Lexer lexer;

    EXPECT_THROW(lexer.LoadString("{\"test"), bpf::json::JsonException);
    EXPECT_THROW(lexer.LoadString("{\"test\": --2}"), bpf::json::JsonException);
    EXPECT_THROW(lexer.LoadString("{\"test\": -2e--2}"), bpf::json::JsonException);
    EXPECT_THROW(lexer.LoadString("{\"test\": 2e3e2}"), bpf::json::JsonException);
    EXPECT_THROW(lexer.LoadString("{\"test\": 2ee2}"), bpf::json::JsonException);
}

TEST(Json, Parser_Err_1)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\": ");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonException);
}

TEST(Json, Parser_Err_2)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\": 2");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonException);
}

TEST(Json, Parser_Err_3)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\" 2}");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonException);
}

TEST(Json, Parser_Err_4)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\"}");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonException);
}

TEST(Json, Stringifier)
{
    J::Object obj{
        {"MyBool", true},
        {"MyNum", 42.42},
        {"MyArr", J::Array {"a", "b"}}
    };
    J val = std::move(obj);

    EXPECT_STREQ(*bpf::String::ValueOf(val), "{'MyArr': [a, b], 'MyNum': 42.42, 'MyBool': TRUE}");
}