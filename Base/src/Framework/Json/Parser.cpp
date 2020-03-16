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

#include "Framework/Json/Parser.hpp"
#include "Framework/Scalar.hpp"

using namespace bpf::json;
using namespace bpf;

Json Parser::CheckJson()
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
            + String::ValueOf(_tokens.First().Line)
            + ": Unexpected token '" + _tokens.First().Data + "'");
    return (j);
}

bool Parser::CheckObject(Json &j)
{
    if (_tokens.First().Type == Lexer::ETokenType::BASIC
        && _tokens.First().Data == "{")
    {
        _tokens.RemoveAt(0);
        Json::Object obj;

        while (_tokens.Size() > 0 && _tokens.First().Data != "}")
        {
            Json key;
            if (!CheckString(key))
                throw JsonException(String("Line ")
                    + String::ValueOf(_tokens.First().Line)
                    + ": Expected object key");
            CheckColon();
            Json value = CheckJson();
            obj[key] = value;
            if (_tokens.Size() > 0 && _tokens.First().Data != "}")
                CheckComa();
        }
        if (_tokens.Size() == 0)
            throw JsonException("Unexpected EOF");
        else
            _tokens.RemoveAt(0);
        j = obj;
        return (true);
    }
    return (false);
}

void Parser::CheckColon()
{
    if (_tokens.Size() == 0)
        throw JsonException("Unexpected EOF");
    if (_tokens.First().Type != Lexer::ETokenType::BASIC
        || _tokens.First().Data != ":")
        throw JsonException(String("Line ")
            + String::ValueOf(_tokens.First().Line)
            + ": Expected colon");
    _tokens.RemoveAt(0);
}

void Parser::CheckComa()
{
    if (_tokens.First().Type != Lexer::ETokenType::BASIC
        || _tokens.First().Data != ",")
        throw JsonException(String("Line ")
            + String::ValueOf(_tokens.First().Line)
            + ": Expected coma");
    _tokens.RemoveAt(0);
}

bool Parser::CheckArray(Json &j)
{
    if (_tokens.First().Type == Lexer::ETokenType::BASIC
        && _tokens.First().Data == "[")
    {
        _tokens.RemoveAt(0);
        Json::Array arr;

        while (_tokens.Size() > 0 && _tokens.First().Data != "]")
        {
            arr.Add(CheckJson());
            if (_tokens.Size() > 0 && _tokens.First().Data != "]")
                CheckComa();
        }
        if (_tokens.Size() == 0)
            throw JsonException("Unexpected EOF");
        else
            _tokens.RemoveAt(0);
        j = arr;
        return (true);
    }
    return (false);
}

bool Parser::CheckNumber(Json &j)
{
    if (_tokens.First().Type != Lexer::ETokenType::NUMBER)
        return (false);
    j = Double::Parse(_tokens.First().Data);
    _tokens.RemoveAt(0);
    return (true);
}

bool Parser::CheckString(Json &j)
{
    if (_tokens.First().Type != Lexer::ETokenType::STRING)
        return (false);
    j = _tokens.First().Data;
    _tokens.RemoveAt(0);
    return (true);
}

bool Parser::CheckBasic(Json &j)
{
    if (_tokens.First().Type != Lexer::ETokenType::BASIC)
        return (false);
    Lexer::Token tok = _tokens[0];
    if (tok.Data == "false")
    {
        j = false;
        _tokens.RemoveAt(0);
        return (true);
    }
    else if (tok.Data == "true")
    {
        j = true;
        _tokens.RemoveAt(0);
        return (true);
    }
    else if (tok.Data == "null")
    {
        j = Json();
        _tokens.RemoveAt(0);
        return (true);
    }
    return (false);
}

Json Parser::Parse()
{
    Json val = CheckJson();
    if (_tokens.Size() > 0)
        throw JsonException(String("Line ")
            + String::ValueOf(_tokens.First().Line)
            + ": Unexpected token '" + _tokens.First().Data + "'");
    return (val);
}