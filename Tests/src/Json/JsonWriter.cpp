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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/Json/Writer.hpp>

using J = bpf::json::Json;

TEST(JsonWriter, Basic)
{
    J::Object obj{
        {"Test", 0.0},
        {"Test1", true},
        {"Test2", Null},
        {"TestArray", J::Array {"A", "B", "TrouDuCul"}},
        {"TestObject", J::Object
            {
                {"a", 0.0},
                {"b", 0.1},
                {"c", true}
            }
        }
    };
    bpf::json::Writer writer(false);
    const char *expected =
    "{"
    "\"Test\":0,"
    "\"Test1\":true,"
    "\"Test2\":null,"
    "\"TestArray\":[\"A\",\"B\",\"TrouDuCul\"],"
    "\"TestObject\":{\"a\":0,\"b\":0.1,\"c\":true}"
    "}";

    EXPECT_STREQ(*writer.Serialize(obj), expected);
}

TEST(JsonWriter, Basic_IgnoreNulls)
{
    J::Object obj{
        {"Test", 0.0},
        {"Test1", true},
        {"Test2", Null},
        {"TestArray", J::Array {"A", "B", "TrouDuCul"}},
        {"TestObject", J::Object
            {
                {"a", 0.0},
                {"b", 0.1},
                {"c", true}
            }
        }
    };
    bpf::json::Writer writer(false, true);
    const char *expected =
    "{"
    "\"Test\":0,"
    "\"Test1\":true,"
    "\"TestArray\":[\"A\",\"B\",\"TrouDuCul\"],"
    "\"TestObject\":{\"a\":0,\"b\":0.1,\"c\":true}"
    "}";

    EXPECT_STREQ(*writer.Serialize(obj), expected);
}

TEST(JsonWriter, Pretty)
{
    J::Object obj{
        {"Test", 0.0},
        {"Test1", true},
        {"Test2", Null},
        {"TestArray", J::Array {"A", "B", "TrouDuCul"}},
        {"TestObject", J::Object
            {
                {"a", 0.0},
                {"b", 0.1},
                {"c", true}
            }
        }
    };
    bpf::json::Writer writer;
    const char *expected =
    "{\n"
    "    \"Test\": 0,\n"
    "    \"Test1\": true,\n"
    "    \"Test2\": null,\n"
    "    \"TestArray\": [\"A\", \"B\", \"TrouDuCul\"],\n"
    "    \"TestObject\": {\n"
    "        \"a\": 0,\n"
    "        \"b\": 0.1,\n"
    "        \"c\": true\n"
    "    }\n"
    "}";

    EXPECT_STREQ(*writer.Serialize(obj), expected);
}

TEST(JsonWriter, Pretty_IgnoreNulls)
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
    bpf::json::Writer writer;
    const char *expected =
    "{\n"
    "    \"Test\": 0,\n"
    "    \"Test1\": true,\n"
    "    \"TestArray\": [\"A\", \"B\", \"TrouDuCul\"],\n"
    "    \"TestObject\": {\n"
    "        \"a\": 0,\n"
    "        \"b\": 0.1,\n"
    "        \"c\": true\n"
    "    }\n"
    "}";

    EXPECT_STREQ(*writer.Serialize(obj), expected);
}
