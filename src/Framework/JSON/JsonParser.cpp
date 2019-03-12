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

#include "Framework/JSON/JsonParser.hpp"

using namespace bpf;

Json JsonParser::CheckJson()
{
    Json j;

    if (_tokens.Size() <= 0)
        return (Json());
    if (!CheckBasic(j)
        && !CheckNumber(j)
        && !CheckString(j)
        && !CheckObject(j)
        && !CheckArray(j))
        throw JsonException(String("Line ")
            + String::ValueOf(_tokens.GetFirst()->line)
            + ": Unexpected token '" + _tokens.GetFirst()->data + "'");
    return (j);
}

bool JsonParser::CheckObject(Json &j)
{
    if (_tokens.GetFirst()->type == JsonLexer::ETokenType::BASIC
        && _tokens.GetFirst()->data == "{")
    {
        _tokens.RemoveAt(0);
        Json::Object obj;

        while (_tokens.GetFirst() != Null && _tokens.GetFirst()->data != "}")
        {
            Json key;
            if (!CheckString(key))
                throw JsonException(String("Line ")
                    + String::ValueOf(_tokens.GetFirst()->line)
                    + ": Expected object key");
            CheckColon();
            Json value = CheckJson();
            obj[key] = value;
            if (_tokens.GetFirst() != Null && _tokens.GetFirst()->data != "}")
                CheckComa();
        }
        if (_tokens.GetFirst() == Null)
            throw JsonException("Unexpected EOF");
        else
            _tokens.RemoveAt(0);
        j = obj;
        return (true);
    }
    return (false);
}

void JsonParser::CheckColon()
{
    if (_tokens.GetFirst() == Null)
        throw JsonException("Unexpected EOF");
    if (_tokens.GetFirst()->type != JsonLexer::ETokenType::BASIC
        || _tokens.GetFirst()->data != ":")
        throw JsonException(String("Line ")
            + String::ValueOf(_tokens.GetFirst()->line)
            + ": Expected colon");
    _tokens.RemoveAt(0);
}

void JsonParser::CheckComa()
{
    if (_tokens.GetFirst() == Null)
        throw JsonException("Unexpected EOF");
    if (_tokens.GetFirst()->type != JsonLexer::ETokenType::BASIC
        || _tokens.GetFirst()->data != ",")
        throw JsonException(String("Line ")
            + String::ValueOf(_tokens.GetFirst()->line)
            + ": Expected coma");
    _tokens.RemoveAt(0);
}

bool JsonParser::CheckArray(Json &j)
{
    if (_tokens.GetFirst()->type == JsonLexer::ETokenType::BASIC
        && _tokens.GetFirst()->data == "[")
    {
        _tokens.RemoveAt(0);
        Json::Array arr;

        while (_tokens.GetFirst() != Null && _tokens.GetFirst()->data != "]")
        {
            arr.Add(CheckJson());
            if (_tokens.GetFirst() != Null && _tokens.GetFirst()->data != "]")
                CheckComa();
        }
        if (_tokens.GetFirst() == Null)
            throw JsonException("Unexpected EOF");
        else
            _tokens.RemoveAt(0);
        j = arr;
        return (true);
    }
    return (false);
}

bool JsonParser::CheckNumber(Json &j)
{
    if (_tokens.GetFirst()->type != JsonLexer::ETokenType::NUMBER)
        return (false);
    j = _tokens.GetFirst()->data.ToDouble();
    _tokens.RemoveAt(0);
    return (true);
}

bool JsonParser::CheckString(Json &j)
{
    if (_tokens.GetFirst()->type != JsonLexer::ETokenType::STRING)
        return (false);
    j = _tokens.GetFirst()->data;
    _tokens.RemoveAt(0);
    return (true);
}

bool JsonParser::CheckBasic(Json &j)
{
    if (_tokens.GetFirst()->type != JsonLexer::ETokenType::BASIC)
        return (false);
    JsonLexer::Token tok = _tokens[0];
    if (tok.data == "false")
    {
        j = false;
        _tokens.RemoveAt(0);
        return (true);
    }
    else if (tok.data == "true")
    {
        j = true;
        _tokens.RemoveAt(0);
        return (true);
    }
    else if (tok.data == "null")
    {
        j = Json();
        _tokens.RemoveAt(0);
        return (true);
    }
    return (false);
}

Json JsonParser::Parse()
{
    return (CheckJson());
}