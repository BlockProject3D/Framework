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

#include <Framework/Collection/List.hpp>
#include <Framework/Collection/Stringifier.Array.hpp>
#include <Framework/Collection/Stringifier.List.hpp>
#include <Framework/String.hpp>
#include <Framework/TypeInfo.hpp>
#include <gtest/gtest.h>
#include <iostream>

TEST(String, Create)
{
    bpf::String str = "This is a test !";
    bpf::String str1;
    bpf::String str2 = nullptr;
}

TEST(String, Empty)
{
    bpf::String str;
    bpf::String str1 = "";
    bpf::String str2 = "This is a test";
    EXPECT_TRUE(str.IsEmpty());
    EXPECT_TRUE(str1.IsEmpty());
    EXPECT_FALSE(str2.IsEmpty());
}

TEST(String, CreateUnicode)
{
    bpf::String str = " é è à ù € This is a test !";
}

TEST(String, Copy)
{
    bpf::String str = " é è à ù € This is a test !";

    bpf::String ss = str;
    bpf::String *ss1 = &ss;
    ss = *ss1;
    for (int i = 0; i < 100; ++i)
        ss = str;
    EXPECT_STREQ(*ss, " é è à ù € This is a test !");
}

TEST(String, From_TypeName)
{
    bpf::String s = bpf::TypeName<int>();
    s += bpf::TypeName<float>();
    s += bpf::TypeName<double>();
    EXPECT_STREQ(*s, "intfloatdouble");
}

TEST(String, UTF8)
{
    bpf::String str = " é è à ù € This is a test !\U0001F4E0";

    EXPECT_EQ(str[0], bpf::String::UTF32(" "));
    EXPECT_EQ(str[1], bpf::String::UTF32("é"));
    EXPECT_EQ(str[3], bpf::String::UTF32("è"));
    EXPECT_EQ(str[5], bpf::String::UTF32("à"));
    EXPECT_EQ(str[7], bpf::String::UTF32("ù"));
    EXPECT_EQ(str[9], bpf::String::UTF32("€"));
    EXPECT_EQ(str[11], bpf::String::UTF32("T"));
    EXPECT_EQ(str[str.Len() - 1], bpf::String::UTF32("\U0001F4E0"));
}

TEST(String, Compare)
{
    bpf::String s = "abc";
    bpf::String s1 = "def";
    std::string s2 = "abc";
    std::string s3 = "def";

    EXPECT_EQ(s == s1, s2 == s3);
    EXPECT_EQ(s1 == s, s3 == s2);
    EXPECT_EQ(s != s1, s2 != s3);
    EXPECT_EQ(s1 != s, s3 != s2);
    EXPECT_EQ(s < s1, s2 < s3);
    EXPECT_EQ(s1 > s, s3 > s2);
}

TEST(String, Basic)
{
    bpf::String s("test");
    ASSERT_STREQ(*s, "test");
    ASSERT_TRUE(s.Len() == 4);
    ASSERT_TRUE(s.Size() == 4);
    s = 'A';
    ASSERT_STREQ(*s, "A");
}

TEST(String, UTF8CharToUTF32)
{
    //Source : FileFormat.info
    EXPECT_TRUE(bpf::String::UTF32("É") == 201);
    EXPECT_TRUE(bpf::String::UTF32("é") == 233);
    EXPECT_TRUE(bpf::String::UTF32("€") == 8364);
    EXPECT_TRUE(bpf::String::UTF32("¥") == 165);
    EXPECT_TRUE(bpf::String::UTF32("▦") == 9638);
    EXPECT_TRUE(bpf::String::UTF32("a") == 'a');
    EXPECT_TRUE(bpf::String::UTF32("\U0001F4E0") == 128224);
}

TEST(String, UTF32CharToUTF8)
{
    //Source : FileFormat.info
    EXPECT_TRUE(bpf::String::UTF8(201) == "É");
    EXPECT_TRUE(bpf::String::UTF8(233) == "é");
    EXPECT_TRUE(bpf::String::UTF8(8364) == "€");
    EXPECT_TRUE(bpf::String::UTF8(165) == "¥");
    EXPECT_TRUE(bpf::String::UTF8(9638) == "▦");
    EXPECT_TRUE(bpf::String::UTF8('a') == "a");
    EXPECT_TRUE(bpf::String::UTF8(128224) == "\U0001F4E0");
}

TEST(String, ByteAt)
{
    bpf::String s = "testÉé€test¥▦test";
    EXPECT_TRUE(s.ByteAt(3) == 't');
    EXPECT_TRUE((bpf::uint8)s.ByteAt(4) == 0xC3);
    EXPECT_TRUE(s.ByteAt(s.Size() - 1) == 't');
    EXPECT_THROW(s.ByteAt(42), bpf::IndexException);
}

TEST(String, OperatorPlus)
{
    bpf::String s("init");
    s = s + ' ' + bpf::String::ValueOf(42) + ',' + " the test, " + bpf::String::ValueOf(42.42f);
    EXPECT_STREQ(*s, "init 42, the test, 42.42");
    bpf::String s1("");
    s1 = s1 + 'T' + ' ' + bpf::String::UTF32("¥") + ' ' + 128224;
    EXPECT_STREQ(*s1, "T ¥ \U0001F4E0");
    bpf::String ss = bpf::String("test") + bpf::String(bpf::String::UTF32("¥"));
    EXPECT_STREQ(*ss, "test¥");
}

TEST(String, OperatorPlusEqual)
{
    bpf::String s("init");
    s += ' ';
    s += bpf::String::ValueOf(42);
    s += ',';
    s += " the test, ";
    s += bpf::String::ValueOf(42.42f);
    EXPECT_STREQ(*s, "init 42, the test, 42.42");
    bpf::String s1("");
    s1 += 'T';
    s1 += ' ';
    s1 += bpf::String::UTF32("¥");
    s1 += ' ';
    s1 += 128224;
    EXPECT_STREQ(*s1, "T ¥ \U0001F4E0");
}

TEST(String, OperatorEquall)
{
    bpf::String s("init");
    s += ' ';
    s += bpf::String::ValueOf(42);
    s += ',';
    s += " the test, ";
    s += bpf::String::ValueOf(42.42f);
    bpf::String s1("init");
    s1 = s1 + ' ' + bpf::String::ValueOf(42) + ',' + " the test, " + bpf::String::ValueOf(42.42f);
    EXPECT_STREQ(*s, "init 42, the test, 42.42");
    EXPECT_STREQ(*s1, "init 42, the test, 42.42");
    EXPECT_TRUE(s == s1);
    EXPECT_TRUE(!(s != s1));
}

TEST(String, OperatorGreaterLess_1)
{
    bpf::String s("init");
    s += ' ';
    s += bpf::String::ValueOf(42);
    s += ',';
    s += " the test, ";
    s += bpf::String::ValueOf(42.42f);
    bpf::String s1("init");
    s1 = s1 + ' ' + bpf::String::ValueOf(42) + ',' + " the test, " + bpf::String::ValueOf(42.42f);
    EXPECT_STREQ(*s, "init 42, the test, 42.42");
    EXPECT_STREQ(*s1, "init 42, the test, 42.42");
    EXPECT_TRUE(!(s > s1));
    EXPECT_TRUE(!(s < s1));
    EXPECT_TRUE(bpf::String("TT") < bpf::String("qq"));
    EXPECT_TRUE(bpf::String("qq") > bpf::String("TT"));
    bpf::String test = "Test";
    bpf::String test1 = "Test1";
    EXPECT_GT(test1, test);
    EXPECT_LT(test, test1);
}

TEST(String, OperatorGreaterLess_2)
{
    auto a = bpf::String("");
    auto b = bpf::String("some very long string");

    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
}

TEST(String, Replace_1)
{
    bpf::String s = bpf::String("      test      ,     \t    \t  \t\t\t              yay").Replace(" ", "").Replace("\t", "");
    EXPECT_STREQ(*s, "test,yay");

    bpf::String s1 = bpf::String("a/b/c/d/e/f/").Replace("/", "\\");
    EXPECT_STREQ(*s1, "a\\b\\c\\d\\e\\f\\");
    s1 = bpf::String("a/ b/ c/ d/ e/ f/").Replace("/", "\\");
    EXPECT_STREQ(*s1, "a\\ b\\ c\\ d\\ e\\ f\\");
    EXPECT_STRNE(*s1, "a\\b\\c\\d\\e\\f\\");
}

TEST(String, Replace_2)
{
    bpf::String s1 = bpf::String("test\r\nt\ntest2").Replace("\r\n", "\n");

    EXPECT_STREQ(*s1, "test\nt\ntest2");
}

TEST(String, GetUTF32CodePoint)
{
    bpf::String s = "testÉé€test¥▦test";
    EXPECT_TRUE(s.Len() == 17);
    EXPECT_TRUE(s[0] == bpf::String::UTF32("t"));
    EXPECT_TRUE(s[4] == bpf::String::UTF32("É"));
    EXPECT_TRUE(s[12] == bpf::String::UTF32("▦"));
    EXPECT_TRUE(s[11] == bpf::String::UTF32("¥"));
    EXPECT_TRUE(s[s.Len() - 1] == bpf::String::UTF32("t"));
}

TEST(String, LenAndSize)
{
    bpf::String s = "testÉé€test¥▦test\U0001F4E0";
    EXPECT_TRUE(s.Len() == 18);
    EXPECT_TRUE(s.Size() == (int)strlen("testÉé€test¥▦test\U0001F4E0"));
}

TEST(String, IsNumeric)
{
    bpf::String s = "1234567890";
    bpf::String s1 = "42.42";
    bpf::String s2 = "42";
    bpf::String s3 = "42.";
    bpf::String s4 = ".42";
    bpf::String s5 = "a42";
    bpf::String s6 = "qq";
    bpf::String s7 = "";
    bpf::String s8 = "\n";
    bpf::String s9 = "éèù";
    bpf::String s10 = "42.42.42";
    bpf::String s11 = "42.-42.42";
    EXPECT_TRUE(s.IsNumeric());
    EXPECT_TRUE(s1.IsNumeric());
    EXPECT_TRUE(s2.IsNumeric());
    EXPECT_TRUE(s3.IsNumeric());
    EXPECT_TRUE(s4.IsNumeric());
    EXPECT_TRUE(!s5.IsNumeric());
    EXPECT_TRUE(!s6.IsNumeric());
    EXPECT_TRUE(!s7.IsNumeric());
    EXPECT_TRUE(!s8.IsNumeric());
    EXPECT_TRUE(!s9.IsNumeric());
    EXPECT_TRUE(!s10.IsNumeric());
    EXPECT_TRUE(!s11.IsNumeric());
}

TEST(String, ToUpper)
{
    bpf::String s("test");
    EXPECT_STREQ(*s, "test");
    s = s.ToUpper();
    EXPECT_STREQ(*s, "TEST");
    s = "test.1";
    EXPECT_STREQ(*s, "test.1");
    s = s.ToUpper();
    EXPECT_STREQ(*s, "TEST.1");
}

TEST(String, ToLower)
{
    bpf::String s("TEST");
    EXPECT_STREQ(*s, "TEST");
    s = s.ToLower();
    EXPECT_STREQ(*s, "test");
    s = "TEST.1";
    EXPECT_STREQ(*s, "TEST.1");
    s = s.ToLower();
    EXPECT_STREQ(*s, "test.1");
}

TEST(String, IndexOfSingleChar)
{
    bpf::String utf8 = "testÉé€test¥▦testa";
    bpf::String ascii = "youhou";
    EXPECT_TRUE(utf8.IndexOf('t') == 0);
    EXPECT_TRUE(utf8.IndexOf('a') == utf8.Len() - 1);
    EXPECT_TRUE(ascii.IndexOf('y') == 0);
    EXPECT_TRUE(ascii.IndexOf('u') == 2);
    EXPECT_TRUE(ascii.IndexOf('A') == -1);
    EXPECT_TRUE(utf8.IndexOf('G') == -1);
}

TEST(String, IndexOfString)
{
    bpf::String utf8 = "testÉé€test¥▦testabc";
    bpf::String ascii = "youhou";
    EXPECT_TRUE(utf8.IndexOf("t") == 0);
    EXPECT_TRUE(utf8.IndexOf("É") == 4);
    EXPECT_TRUE(utf8.IndexOf("▦") == 12);
    EXPECT_TRUE(utf8.IndexOf("abc") == utf8.Len() - 3);
    EXPECT_TRUE(utf8.IndexOf("doesnotexist") == -1);
    EXPECT_TRUE(ascii.IndexOf("doesnotexist") == -1);
    EXPECT_TRUE(ascii.IndexOf("hou") == 3);
}

TEST(String, LastIndexOfSingleChar)
{
    bpf::String utf8 = "testÉé€test¥▦testa";
    bpf::String ascii = "youhou";
    EXPECT_TRUE(utf8.LastIndexOf('t') == utf8.Len() - 2);
    EXPECT_TRUE(utf8.LastIndexOf('a') == utf8.Len() - 1);
    EXPECT_TRUE(ascii.LastIndexOf('y') == 0);
    EXPECT_TRUE(ascii.LastIndexOf('u') == ascii.Len() - 1);
    EXPECT_TRUE(ascii.LastIndexOf('A') == -1);
    EXPECT_TRUE(utf8.LastIndexOf('G') == -1);
    EXPECT_EQ(utf8.LastIndexOf(bpf::String::UTF32("▦")), 12);
}

TEST(String, LastIndexOfString)
{
    bpf::String utf8 = "testÉé€test¥▦testabc";
    bpf::String ascii = "youhou";
    EXPECT_TRUE(utf8.LastIndexOf("t") == utf8.Len() - 4);
    EXPECT_TRUE(utf8.LastIndexOf("É") == 4);
    EXPECT_TRUE(utf8.LastIndexOf("▦") == 12);
    EXPECT_TRUE(utf8.LastIndexOf("abc") == utf8.Len() - 3);
    EXPECT_TRUE(utf8.LastIndexOf("doesnotexist") == -1);
    EXPECT_TRUE(ascii.LastIndexOf("doesnotexist") == -1);
    EXPECT_TRUE(ascii.LastIndexOf("hou") == 3);
}

TEST(String, ExplodeSingleChar)
{
    bpf::String s = "this is a test";
    bpf::String s1 = "this      is   a                 test";
    bpf::String s2 = "         this      is   a                 test         ";
    bpf::collection::Array<bpf::String> data = s.Explode(' ');
    bpf::collection::Array<bpf::String> data1 = s1.Explode(' ');
    bpf::collection::Array<bpf::String> data2 = s2.Explode(' ');

    EXPECT_TRUE(data.Size() == data1.Size());
    EXPECT_TRUE(data.Size() == data2.Size());
    EXPECT_TRUE(data.Size() == 4);
    EXPECT_STREQ("this", *data[0]);
    EXPECT_STREQ("is", *data[1]);
    EXPECT_STREQ("a", *data[2]);
    EXPECT_STREQ("test", *data[3]);
    EXPECT_STREQ("this", *data1[0]);
    EXPECT_STREQ("is", *data1[1]);
    EXPECT_STREQ("a", *data1[2]);
    EXPECT_STREQ("test", *data1[3]);
    EXPECT_STREQ("this", *data2[0]);
    EXPECT_STREQ("is", *data2[1]);
    EXPECT_STREQ("a", *data2[2]);
    EXPECT_STREQ("test", *data2[3]);
}

TEST(String, ExplodeOr)
{
    bpf::String s = "this is a, test";
    bpf::collection::Array<bpf::String> data = s.ExplodeOr(" ,");

    EXPECT_TRUE(data.Size() == 4);
    EXPECT_STREQ("this", *data[0]);
    EXPECT_STREQ("is", *data[1]);
    EXPECT_STREQ("a", *data[2]);
    EXPECT_STREQ("test", *data[3]);
}

TEST(String, ExplodeString)
{
    bpf::String s = "this  is  a  test";
    bpf::String s1 = "this      is    a                  test";
    bpf::String s2 = "        this      is    a                  test          ";
    bpf::String s3 = "this is a test";
    bpf::collection::Array<bpf::String> data = s.Explode("  ");
    bpf::collection::Array<bpf::String> data1 = s1.Explode("  ");
    bpf::collection::Array<bpf::String> data2 = s2.Explode("  ");
    bpf::collection::Array<bpf::String> data3 = s3.Explode(" ");

    EXPECT_TRUE(data.Size() == data1.Size());
    EXPECT_TRUE(data.Size() == data2.Size());
    EXPECT_TRUE(data3.Size() == data.Size());
    EXPECT_TRUE(data.Size() == 4);
    EXPECT_STREQ("this", *data[0]);
    EXPECT_STREQ("is", *data[1]);
    EXPECT_STREQ("a", *data[2]);
    EXPECT_STREQ("test", *data[3]);
    EXPECT_STREQ("this", *data1[0]);
    EXPECT_STREQ("is", *data1[1]);
    EXPECT_STREQ("a", *data1[2]);
    EXPECT_STREQ("test", *data1[3]);
    EXPECT_STREQ("this", *data2[0]);
    EXPECT_STREQ("is", *data2[1]);
    EXPECT_STREQ("a", *data2[2]);
    EXPECT_STREQ("test", *data2[3]);
    EXPECT_STREQ("this", *data3[0]);
    EXPECT_STREQ("is", *data3[1]);
    EXPECT_STREQ("a", *data3[2]);
    EXPECT_STREQ("test", *data3[3]);
}

TEST(String, ExplodeIgnoreSingleChar)
{
    bpf::String s = "this 'is a' test";
    bpf::String s1 = "this      'is   a'                 test";
    bpf::String s2 = "         this      'is   a'                 test         ";
    bpf::collection::Array<bpf::String> data = s.ExplodeIgnore(' ', '\'');
    bpf::collection::Array<bpf::String> data1 = s1.ExplodeIgnore(' ', '\'');
    bpf::collection::Array<bpf::String> data2 = s2.ExplodeIgnore(' ', '\'');

    EXPECT_TRUE(data.Size() == data1.Size());
    EXPECT_TRUE(data.Size() == data2.Size());
    EXPECT_TRUE(data.Size() == 3);
    EXPECT_STREQ("this", *data[0]);
    EXPECT_STREQ("'is a'", *data[1]);
    EXPECT_STREQ("test", *data[2]);
    EXPECT_STREQ("this", *data1[0]);
    EXPECT_STREQ("'is   a'", *data1[1]);
    EXPECT_STREQ("test", *data1[2]);
    EXPECT_STREQ("this", *data2[0]);
    EXPECT_STREQ("'is   a'", *data2[1]);
    EXPECT_STREQ("test", *data2[2]);
}

TEST(String, ExplodeIgnoreString_Test1)
{
    bpf::String s = "this  'is  a'  test";
    bpf::String s1 = "this      'is   a'                  test";
    bpf::String s2 = "        this      'is   a'                test         ";
    bpf::collection::Array<bpf::String> data = s.ExplodeIgnore("  ", "'");
    bpf::collection::Array<bpf::String> data1 = s1.ExplodeIgnore("  ", "'");
    bpf::collection::Array<bpf::String> data2 = s2.ExplodeIgnore("  ", "'");

    EXPECT_TRUE(data.Size() == data1.Size());
    EXPECT_TRUE(data.Size() == data2.Size());
    EXPECT_TRUE(data.Size() == 3);
    EXPECT_STREQ("this", *data[0]);
    EXPECT_STREQ("'is  a'", *data[1]);
    EXPECT_STREQ("test", *data[2]);
    EXPECT_STREQ("this", *data1[0]);
    EXPECT_STREQ("'is   a'", *data1[1]);
    EXPECT_STREQ("test", *data1[2]);
    EXPECT_STREQ("this", *data2[0]);
    EXPECT_STREQ("'is   a'", *data2[1]);
    EXPECT_STREQ("test", *data2[2]);
}

TEST(String, ExplodeIgnoreString_Test2)
{
    bpf::String s = "this  !'is  a!'  test";
    bpf::String s1 = "this      !'is   a!'                  test";
    bpf::String s2 = "          this      !'is   a!'                  test          ";
    bpf::collection::Array<bpf::String> data = s.ExplodeIgnore("  ", "!'");
    bpf::collection::Array<bpf::String> data1 = s1.ExplodeIgnore("  ", "!'");
    bpf::collection::Array<bpf::String> data2 = s2.ExplodeIgnore("  ", "!'");

    EXPECT_TRUE(data.Size() == data1.Size());
    EXPECT_TRUE(data.Size() == data2.Size());
    EXPECT_TRUE(data.Size() == 3);
    EXPECT_STREQ("this", *data[0]);
    EXPECT_STREQ("!'is  a!'", *data[1]);
    EXPECT_STREQ("test", *data[2]);
    EXPECT_STREQ("this", *data1[0]);
    EXPECT_STREQ("!'is   a!'", *data1[1]);
    EXPECT_STREQ("test", *data1[2]);
    EXPECT_STREQ("this", *data2[0]);
    EXPECT_STREQ("!'is   a!'", *data2[1]);
    EXPECT_STREQ("test", *data2[2]);
}

TEST(String, StartsWith)
{
    bpf::String s = "this is a test";
    EXPECT_TRUE(s.StartsWith("this"));
    EXPECT_TRUE(!s.StartsWith("is a"));
    EXPECT_TRUE(s.StartsWith(""));
    EXPECT_TRUE(!s.StartsWith("this is a test "));
}

TEST(String, EndsWith)
{
    bpf::String s = "this is a test";
    EXPECT_TRUE(s.EndsWith("test"));
    EXPECT_TRUE(!s.EndsWith("a tes"));
    EXPECT_TRUE(s.EndsWith(""));
    EXPECT_TRUE(!s.EndsWith("this is a test "));
}

TEST(String, StartsWithUTF8)
{
    bpf::String s = "thïs ïs à tèst€";
    EXPECT_TRUE(s.StartsWith("thïs"));
    EXPECT_FALSE(s.StartsWith("this"));
    EXPECT_FALSE(s.StartsWith("is a"));
    EXPECT_FALSE(s.StartsWith("ïs à"));
    EXPECT_TRUE(s.StartsWith(""));
}

TEST(String, EndsWithUTF8)
{
    bpf::String s = "this is à tèst€";
    EXPECT_TRUE(s.EndsWith("tèst€"));
    EXPECT_FALSE(s.EndsWith("test€"));
    EXPECT_FALSE(s.EndsWith("à tes"));
    EXPECT_FALSE(s.EndsWith("a tes"));
    EXPECT_TRUE(s.EndsWith(""));
}

TEST(String, ContainsString_1)
{
    bpf::String s = "this is a test";
    EXPECT_TRUE(s.Contains(" is a "));
    EXPECT_TRUE(s.Contains("this is"));
    EXPECT_TRUE(s.Contains("a test"));
    EXPECT_TRUE(!s.Contains("go"));
}

TEST(String, ContainsChar_1)
{
    bpf::String s = "this is a test";
    EXPECT_TRUE(s.Contains('i'));
    EXPECT_TRUE(s.Contains('t'));
    EXPECT_TRUE(s.Contains('a'));
    EXPECT_TRUE(!s.Contains('g'));
}

TEST(String, ContainsString_2)
{
    bpf::String a = "";
    bpf::String b = "Some very long string";

    EXPECT_FALSE(b.ToLower().Contains(a.ToLower()));
}

TEST(String, ContainsChar_2)
{
    bpf::String b = "Some very long string";

    EXPECT_TRUE(b.ToLower().Contains(' '));
}

TEST(String, Sub)
{
    bpf::String s = "test   test  testabc";
    EXPECT_STREQ(*s.Sub(0, 4), "test");
    EXPECT_STREQ(*s.Sub(9, 11), "st");
    EXPECT_STREQ(*s.Sub(12), " testabc");
    EXPECT_STREQ(*s.Sub(s.Len() + 1), *s);
}

TEST(String, SubUTF8)
{
    bpf::String s = "testÉé€test¥▦testabc";
    EXPECT_STREQ(*s.Sub(0, 4), "test");
    EXPECT_STREQ(*s.Sub(11, 13), "¥▦");
    EXPECT_STREQ(*s.Sub(12), "▦testabc");
    EXPECT_STREQ(*s.Sub(s.Len() + 1), *s);
}

TEST(String, SubLen)
{
    bpf::String s = "test   test  testabc";
    EXPECT_STREQ(*s.SubLen(0, 4), "test");
    EXPECT_STREQ(*s.SubLen(9, 2), "st");
    EXPECT_STREQ(*s.SubLen(12), " testabc");
}

TEST(String, SubLenUTF8)
{
    bpf::String s = "testÉé€test¥▦testabc";
    EXPECT_STREQ(*s.SubLen(0, 4), "test");
    EXPECT_STREQ(*s.SubLen(11, 2), "¥▦");
    EXPECT_STREQ(*s.SubLen(12), "▦testabc");
}

TEST(String, Format_Test1)
{
    bpf::collection::Array<bpf::String> arr(2);
    arr[0] = "this is a test";
    arr[1] = "yay";
    bpf::String formatted = bpf::String::Format("[16,right, ]\t[16,right, ]\t[]\t[]", -42, 42.42, 42000, arr);
    const char *expected = "             -42\t           42.42\t42000\t[this is a test, yay]";
    EXPECT_STREQ(*formatted, expected);
    EXPECT_TRUE(strlen(expected) == (size_t)formatted.Size());
}

TEST(String, Format_Test2)
{
    bpf::String res = bpf::String::Format("[8,left, ] | [4,right, ] | [5,right, ]", "Name", "GPA", "TOEIC");
    std::cout << *res << std::endl;
    res = bpf::String::Format("[8,left,-]---[4,right,-]---[5,right,-]", "", "", "");
    std::cout << *res << std::endl;
    res = bpf::String::Format("[8,left, ] | [4,right, ] | [5,right, ]", "Tsinghua", 3.5, -1);
    std::cout << *res << std::endl;
    res = bpf::String::Format("[8,left, ] | [4,right, ] | [5,right, ]", "UC Berkley", 3.0, 800);
    std::cout << *res << std::endl;
}

TEST(String, ValueOf)
{
    //Int32
    EXPECT_STREQ(*bpf::String::ValueOf(42), "42");
    EXPECT_STREQ(*bpf::String::ValueOf(42, 4), "0042");
    EXPECT_STREQ(*bpf::String::ValueOf(42, 1), "42");
    EXPECT_STREQ(*bpf::String::ValueOf(-42, 1), "-42");
    EXPECT_STREQ(*bpf::String::ValueOf(-42, 4), "-042");
    //UInt32
    EXPECT_STREQ(*bpf::String::ValueOf(42U), "42");
    EXPECT_STREQ(*bpf::String::ValueOf(42U, 4), "0042");
    EXPECT_STREQ(*bpf::String::ValueOf(42U, 1), "42");
    //Int64
    EXPECT_STREQ(*bpf::String::ValueOf(42LL), "42");
    EXPECT_STREQ(*bpf::String::ValueOf(42LL, 4), "0042");
    EXPECT_STREQ(*bpf::String::ValueOf(42LL, 1), "42");
    EXPECT_STREQ(*bpf::String::ValueOf(-42LL, 1), "-42");
    EXPECT_STREQ(*bpf::String::ValueOf(-42LL, 4), "-042");
    //UInt64
    EXPECT_STREQ(*bpf::String::ValueOf(42ULL), "42");
    EXPECT_STREQ(*bpf::String::ValueOf(42ULL, 4), "0042");
    EXPECT_STREQ(*bpf::String::ValueOf(42ULL, 1), "42");
    //Float
    EXPECT_STREQ(*bpf::String::ValueOf(42.42f), "42.42");
    EXPECT_STREQ(*bpf::String::ValueOf(42.423f, 2), "42.42");
    EXPECT_STREQ(*bpf::String::ValueOf(42.42f, 4), "42.4200");
    //Double
    EXPECT_STREQ(*bpf::String::ValueOf(42.42), "42.42");
    EXPECT_STREQ(*bpf::String::ValueOf(42.423, 2), "42.42");
    EXPECT_STREQ(*bpf::String::ValueOf(42.42, 4), "42.4200");
    EXPECT_STREQ(*bpf::String::ValueOf((bpf::uint64)999999999), "999999999");
    EXPECT_STREQ(*bpf::String::ValueOf((bpf::int64)-999999999), "-999999999");
    EXPECT_STREQ(*bpf::String::ValueOf((bpf::uint32)4000000000), "4000000000");
    bpf::collection::Array<bpf::String> arr(2);
    arr[0] = "this is a test";
    arr[1] = "yay";
    EXPECT_STREQ(*bpf::String::ValueOf(arr), "[this is a test, yay]");
    bpf::collection::List<int> lst;
    lst.Add(0);
    lst.Add(2);
    lst.Add(5);
    EXPECT_STREQ(*bpf::String::ValueOf(lst), "[0, 2, 5]");
    bpf::uintptr s = 0x456789F;
    EXPECT_STREQ(*bpf::String::ValueOf((void *)s), "0x456789f");
}

TEST(String, Safety)
{
    bpf::String str = "this is a test";
    bpf::String str1 = "this is a test1";
    EXPECT_EQ(str[-1], bpf::String::UTF32("t")); //MSVC wants slow unoptimized code; fine!
    EXPECT_EQ(str1[-1], bpf::String::UTF32("1")); //MSVC wants slow unoptimized code; fine!
    EXPECT_EQ(str1[-2], bpf::String::UTF32("t")); //MSVC wants slow unoptimized code; fine!
    EXPECT_THROW(str += str[9999], bpf::IndexException);
    EXPECT_THROW(str += str[15], bpf::IndexException);
}

TEST(String, Iterate)
{
    bpf::String str = "this is a test";
    bpf::String res;

    for (int i = 0; i != str.Len(); ++i)
        res += str[i];
    EXPECT_STREQ(*str, *res);
    res = "";
    for (bpf::uint32 i = 0; str[i]; ++i)
        res += str[i];
    EXPECT_STREQ(*str, *res);
}

TEST(String, IterateUTF8)
{
    bpf::String str = "thïs ïs à tést€ducultdbdc";
    bpf::String res;

    for (int i = 0; i != str.Len(); ++i)
        res += str[i];
    EXPECT_STREQ(*str, *res);
    res = "";
    for (bpf::uint32 i = 0; str[i]; ++i)
        res += str[i];
    EXPECT_STREQ(*str, *res);
}

TEST(String, ToArray)
{
    bpf::String str = "this is a test";
    bpf::collection::Array<char> arr = str.ToArray();

    EXPECT_STREQ(*str, *arr);
}

TEST(String, Reverse)
{
    bpf::String str = "abc";
    EXPECT_STREQ(*str.Reverse(), "cba");
    str = "";
    EXPECT_STREQ(*str.Reverse(), "");
    str = "abcd  ef";
    EXPECT_STREQ(*str.Reverse(), "fe  dcba");
}

TEST(String, Chinese_1)
{
    bpf::String str = "你好，我是清华大学的留学生";

    EXPECT_EQ(str.Len(), 13);
    EXPECT_EQ(str.Size(), 39);
    EXPECT_STREQ(*str.Sub(str.IndexOf("我是") + 2), "清华大学的留学生");
}

TEST(String, Chinese_2)
{
    bpf::String str = "你好，我是清华大学的留学生";
    auto arr = str.ToUTF32();
    auto recover = bpf::String::FromUTF32(*arr);

    EXPECT_EQ(str.Len(), 13);
    EXPECT_EQ(str.Size(), 39);
    EXPECT_EQ(arr.Size(), 14U);
    EXPECT_EQ(recover.Len(), 13);
    EXPECT_EQ(recover.Size(), 39);
    EXPECT_STREQ(*recover, "你好，我是清华大学的留学生");
    EXPECT_STREQ(*str, "你好，我是清华大学的留学生");
}

TEST(String, Chinese_3)
{
    bpf::String str = "你好，我是清华大学的留学生";
    auto *expected = reinterpret_cast<const bpf::fchar16 *>(u"你好，我是清华大学的留学生");
    auto arr = str.ToUTF16();
    auto recover = bpf::String::FromUTF16(*arr);

    for (bpf::fsize i = 0; i != arr.Size(); ++i)
        EXPECT_EQ(arr[i], expected[i]);
    EXPECT_EQ(str.Len(), 13);
    EXPECT_EQ(str.Size(), 39);
    EXPECT_EQ(arr.Size(), 14U);
    EXPECT_EQ(recover.Len(), 13);
    EXPECT_EQ(recover.Size(), 39);
    EXPECT_STREQ(*recover, "你好，我是清华大学的留学生");
    EXPECT_STREQ(*str, "你好，我是清华大学的留学生");
}

TEST(String, Convert_U16_1)
{
    bpf::String str = (bpf::fchar)0x10000; //U+10000
    auto arr = str.ToUTF16();
    auto recover = bpf::String::FromUTF16(*arr);

    EXPECT_EQ(str.Len(), 1);
    EXPECT_EQ(str.Size(), 4);
    EXPECT_EQ(arr.Size(), 3U); //Null terminated
    EXPECT_EQ(recover.Len(), 1);
    EXPECT_EQ(recover.Size(), 4);
    EXPECT_EQ(recover[0], (bpf::fchar)0x10000);
    EXPECT_EQ(str[0], (bpf::fchar)0x10000);
}

TEST(String, Convert_U16_2)
{
    bpf::String str = (bpf::fchar)0x100BB; //U+10000
    auto arr = str.ToUTF16();
    auto recover = bpf::String::FromUTF16(*arr);

    EXPECT_EQ(str.Len(), 1);
    EXPECT_EQ(str.Size(), 4);
    EXPECT_EQ(arr.Size(), 3U); //Null terminated
    EXPECT_EQ(recover.Len(), 1);
    EXPECT_EQ(recover.Size(), 4);
    EXPECT_EQ(recover[0], (bpf::fchar)0x100BB);
    EXPECT_EQ(str[0], (bpf::fchar)0x100BB);
}