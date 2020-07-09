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

#include <iostream>
#include <gtest/gtest.h>
#include <Framework/Json/Json.hpp>
#include <Framework/Json/JsonParseException.hpp>
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
    EXPECT_STREQ(*carr[0].ToString(), "A");
    J::Array &arr = obj["TestArray"];
    J v = 42.42;
    arr.Add(42.42);
    arr.Add(v);
    EXPECT_EQ(carr[3], 42.42);
    EXPECT_EQ(carr[4], 42.42);
    arr.RemoveAt(4);
    arr.RemoveAt(0);
    EXPECT_EQ(carr.Size(), 3U);
    EXPECT_EQ(arr.Size(), 3U);
    arr.Items.Clear();
    EXPECT_EQ(carr.Size(), 0U);
    EXPECT_EQ(arr.Size(), 0U);
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
    EXPECT_EQ(mobj.Size(), 3U);
    EXPECT_EQ(cobj.Size(), 3U);
    mobj.Properties.Clear();
    EXPECT_EQ(mobj.Size(), 0U);
    EXPECT_EQ(cobj.Size(), 0U);
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
    EXPECT_STREQ(*arr[0].ToObject()["Test"].ToString(), "a");
    EXPECT_STREQ(*arr[1].ToObject()["Test"].ToString(), "a");
    EXPECT_STREQ(*arr[2].ToObject()["Test"].ToString(), "a");
    EXPECT_STREQ(*arr[0].ToObject()["Test1"].ToString(), "b");
    for (auto &json : arr)
    {
        EXPECT_EQ(json.Type(), J::OBJECT);
        EXPECT_EQ(json.ToObject()["Test"], "a");
        EXPECT_EQ(json.ToObject()["Test1"], "b");
        const J::Object &jobj = json;
        for (auto &sjson : jobj)
        {
            EXPECT_EQ(sjson.Value.Type(), J::STRING);
            EXPECT_TRUE(sjson.Key == "Test" || sjson.Key == "Test1");
        }
    }
    for (auto &json : bpf::collection::Reverse(arr))
    {
        EXPECT_EQ(json.Type(), J::OBJECT);
        EXPECT_EQ(json.ToObject()["Test"], "a");
        EXPECT_EQ(json.ToObject()["Test1"], "b");
        const J::Object &jobj = json;
        for (auto &sjson : bpf::collection::Reverse(jobj))
        {
            EXPECT_EQ(sjson.Value.Type(), J::STRING);
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
    EXPECT_EQ(myObj["MyVec"].ToArray()[0], 0.0);
    EXPECT_EQ(myObj["MyVec"].ToArray()[1], 1.0);
    EXPECT_EQ(myObj["MyVec"].ToArray()[2], 0.0);
}

TEST(Json, API_4_2)
{
    J::Object myObj = J::Object
    {
        {"MyVec", J::Array(bpf::collection::List<J> { 0.0, 1.0, 0.0})}
    };

    EXPECT_EQ(myObj["MyVec"].ToArray()[0], 0.0);
    EXPECT_EQ(myObj["MyVec"].ToArray()[1], 1.0);
    EXPECT_EQ(myObj["MyVec"].ToArray()[2], 0.0);
}

TEST(Json, API_4_3)
{
    bpf::collection::List<J> lst = { 0.0, 1.0, 0.0 };
    J::Object myObj = J::Object
    {
        {"MyVec", J::Array(lst)}
    };

    EXPECT_EQ(myObj["MyVec"].ToArray()[0], 0.0);
    EXPECT_EQ(myObj["MyVec"].ToArray()[1], 1.0);
    EXPECT_EQ(myObj["MyVec"].ToArray()[2], 0.0);
}

TEST(Json, API_5)
{
    auto s = bpf::String("Test");
    J::Object myObj = J::Object
    {
        {"MyStr", bpf::String("Test")},
        {"MyStr1", s}
    };

    EXPECT_STREQ(*myObj["MyStr"].ToString(), "Test");
    EXPECT_STREQ(*myObj["MyStr1"].ToString(), "Test");
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
    EXPECT_THROW(val.ToString(), bpf::json::JsonException);
    EXPECT_THROW((void)((const J::Array &)valConst), bpf::json::JsonException);
    EXPECT_THROW(val.ToArray(), bpf::json::JsonException);
    EXPECT_THROW((void)((J::Array &)val), bpf::json::JsonException);
    EXPECT_THROW((void)((const J::Object &)val1Const), bpf::json::JsonException);
    EXPECT_THROW((void)((J::Object &)val1), bpf::json::JsonException);
    EXPECT_THROW(val1.ToObject(), bpf::json::JsonException);
    EXPECT_THROW((void)((double)val), bpf::json::JsonException);
    EXPECT_THROW(val.ToNumber(), bpf::json::JsonException);
    EXPECT_THROW((void)((bool)val), bpf::json::JsonException);
    EXPECT_THROW(val.ToBoolean(), bpf::json::JsonException);
    EXPECT_STREQ(*val.ToObject()["MyStr"].ToString(), "Test");
    EXPECT_EQ(val1.ToArray()[0], 1.0);
    val = "this is a test";
    EXPECT_STREQ(*val.ToString(), "this is a test");
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
        "   \"StrArray\": [\"test\",\"test1\",\"test2\"],"
        "   \"AdvTestStr\": \"String with special \b\t characters\b\t.\","
        "   \"Unicode\": \"\\u233\","
        "   \"Special\": \"\\t\\b\","
        "   \"MyString\": \"This is a true test of false and null containing 0.1 42.42 numbers and even \\\"strings\\\"\""
        "}";

    bpf::json::Lexer lexer;
    lexer.LoadString(str);
    J testObj = bpf::json::Parser(std::move(lexer)).Parse();
    EXPECT_EQ(testObj.Type(), bpf::json::Json::OBJECT);
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
    EXPECT_STREQ(*testObject["MyString"].ToString(), "This is a true test of false and null containing 0.1 42.42 numbers and even \"strings\"");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Array"].ToArray()), "[TRUE, FALSE, null, 0.1, 1, 42, 4242, 0.4242]");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["StrArray"].ToArray()), "[test, test1, test2]");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["AdvTestStr"]), "String with special \b\t characters\b\t.");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Unicode"]), "é");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Special"]), "\t\b");
}

TEST(Json, LexerParser_Comments_Err)
{
    bpf::json::Lexer lexer;

    EXPECT_THROW(lexer.LoadString("//This should fail"), bpf::json::JsonParseException);
}

TEST(Json, LexerParser_Comments)
{
    bpf::String str =
        "{"
        "   \"test\": true, /* This is a test comment */\n"
        "   \"TheNull\": /*this is an inline comment*/ null,\r\n"
        "   \"-42\": -42, //Single line comment 1\n"
        "   \"Array\": [ true, false, null, 0.1, 1, 42, 42.42e2, 42.42e-2 ], //Single line comment 2\r\n"
        "   \"StrArray\": [\"test\",\"test1\"/*,\"test2\"*/],"
        "   \"AdvTestStr\": \"String with special \b\t characters\b\t.\","
        "   /*\"Unicode\": \"\\u233\",*///Remove the key for testing\n"
        "   \"Special\": \"\\t\\b\","
        "   \"MyString\": \"This is a true test of false and /*comments*/ with null containing 0.1 42.42 numbers and even \\\"strings\\\"\""
        "}";

    bpf::json::Lexer lexer(true);
    lexer.LoadString(str);
    J testObj = bpf::json::Parser(std::move(lexer)).Parse();
    EXPECT_EQ(testObj.Type(), bpf::json::Json::OBJECT);
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
    EXPECT_STREQ(*str1, "This is a true test of false and /*comments*/ with null containing 0.1 42.42 numbers and even \"strings\"");
    EXPECT_STREQ(*testObject["MyString"].ToString(), "This is a true test of false and /*comments*/ with null containing 0.1 42.42 numbers and even \"strings\"");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Array"].ToArray()), "[TRUE, FALSE, null, 0.1, 1, 42, 4242, 0.4242]");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["StrArray"].ToArray()), "[test, test1]");
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["AdvTestStr"]), "String with special \b\t characters\b\t.");
    EXPECT_FALSE(testObject.Properties.HasKey("Unicode"));
    EXPECT_STREQ(*bpf::String::ValueOf(testObject["Special"]), "\t\b");
}

TEST(Json, LexerParser_MultiLoadString)
{
    bpf::json::Lexer lexer(true);

    lexer.LoadString("//Start our object declaration");
    lexer.LoadString("{");
    lexer.LoadString("  //A number");
    lexer.LoadString("  \"test\": 1,");
    lexer.LoadString("  //A commented number");
    lexer.LoadString("  /*\"test\": 42,*/");
    lexer.LoadString("  //A string");
    lexer.LoadString(R"(  "testStr": "this is the end")");
    lexer.LoadString("}");

    J testObj = bpf::json::Parser(std::move(lexer)).Parse();
    EXPECT_EQ(testObj.Type(), bpf::json::Json::OBJECT);
    J::Object testObject = std::move(testObj);
    EXPECT_EQ(testObject["test"], 1.0);
    EXPECT_EQ(testObject["testStr"], "this is the end");
}

TEST(Json, LexerParser_IgnoreNulls)
{
    bpf::json::Lexer lexer(false, true);

    lexer.LoadString("{");
    lexer.LoadString("  \"test\": 1,");
    lexer.LoadString("  \"testStr\": null");
    lexer.LoadString("}");

    J testObj = bpf::json::Parser(std::move(lexer)).Parse();
    EXPECT_EQ(testObj.Type(), bpf::json::Json::OBJECT);
    J::Object testObject = std::move(testObj);
    EXPECT_EQ(testObject["test"], 1.0);
    EXPECT_FALSE(testObject.Properties.HasKey("testStr"));
}

TEST(Json, Lexer_Err)
{
    bpf::json::Lexer lexer;

    EXPECT_THROW(lexer.LoadString("{\"test"), bpf::json::JsonParseException);
    EXPECT_THROW(lexer.LoadString("{\"test\": --2}"), bpf::json::JsonParseException);
    EXPECT_THROW(lexer.LoadString("{\"test\": -2e--2}"), bpf::json::JsonParseException);
    EXPECT_THROW(lexer.LoadString("{\"test\": 2e3e2}"), bpf::json::JsonParseException);
    EXPECT_THROW(lexer.LoadString("{\"test\": 2ee2}"), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_1)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\": ");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_2)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\": 2");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_3)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\" 2}");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_4)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\"}");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_5)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("[2, 3,");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_6)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("[2, 3");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_7)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\":");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_8)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{:\"test\"");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_9)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("[2\"2\":]");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_10)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{}\"test\"");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_11)
{
    bpf::json::Lexer lexer;

    lexer.LoadString(",{}");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_12)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\"");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
}

TEST(Json, Parser_Err_13)
{
    bpf::json::Lexer lexer;

    lexer.LoadString("{\"test\"");
    EXPECT_THROW(bpf::json::Parser(std::move(lexer)).Parse(), bpf::json::JsonParseException);
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