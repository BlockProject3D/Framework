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

#include "Framework/Json/JsonException.hpp"
#include "Framework/Json/Writer.hpp"

using namespace bpf::json;
using namespace bpf;

String Writer::SerializeObject(const Json::Object &json)
{
    String res = '{';

    for (auto &prop : json)
    {
        if (!_ignoreNulls || (_ignoreNulls && prop.Value.Type() != Json::NONE))
        {
            res += String('"') + prop.Key + "\":" + Serialize(prop.Value) + ',';
        }
    }
    res = res.Sub(0, res.Len() - 1);
    res += '}';
    return (res);
}

String Writer::SerializeArray(const Json::Array &json)
{
    String res = '[';

    for (auto &prop : json)
        res += Serialize(prop) + ',';
    res = res.Sub(0, res.Len() - 1);
    res += ']';
    return (res);
}

String Writer::Indent()
{
    String res;

    for (int i = 0; i != _stack; ++i)
        res += "    ";
    return (res);
}

String Writer::SerializeObjectPretty(const Json::Object &json)
{
    String res = "{\n";

    ++_stack;
    String prefix = Indent();
    for (auto &prop : json)
    {
        if (!_ignoreNulls || prop.Value.Type() != Json::NONE)
            res += prefix + '"' + prop.Key + "\": " + Serialize(prop.Value) + ",\n";
    }
    --_stack;
    res = res.Sub(0, res.Len() - 2);
    res += String('\n') + Indent() + '}';
    return (res);
}

String Writer::SerializeArrayPretty(const Json::Array &json)
{
    String res = '[';

    for (auto &prop : json)
        res += Serialize(prop) + ", ";
    res = res.Sub(0, res.Len() - 2);
    res += ']';
    return (res);
}

String Writer::Serialize(const Json &json)
{
    switch (json.Type())
    {
    case Json::ARRAY:
        return (_pretty ? SerializeArrayPretty(json) : SerializeArray(json));
    case Json::OBJECT:
        return (_pretty ? SerializeObjectPretty(json) : SerializeObject(json));
    case Json::NUMBER:
        return (String::ValueOf(json.AsNumber()));
    case Json::BOOLEAN:
        return (json == true ? "true" : "false");
    case Json::STRING:
        return (String('"') + json.AsString() + '"');
    case Json::NONE:
        return ("null");
    }
    throw JsonException("Unknown value type");
}
