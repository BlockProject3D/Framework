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

#include "Framework/Json/Parser.hpp"
#include "Framework/Json/JsonParseException.hpp"
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
        throw JsonParseException(_tokens.Top().Line, String("Unexpected token '") + _tokens.Top().Data + "'");
    return (j);
}

bool Parser::CheckObject(Json &j)
{
    if (_tokens.Top().Type == Lexer::ETokenType::BASIC
        && _tokens.Top().Data == "{")
    {
        _tokens.Pop();
        Json::Object obj;

        while (_tokens.Size() > 0 && _tokens.Top().Data != "}")
        {
            Json key;
            if (!CheckString(key))
                throw JsonParseException(_tokens.Top().Line, "Expected object key");
            CheckColon();
            Json value = CheckJson();
            if (!_ignoreNulls || value.Type() != Json::NONE)
                obj[key] = value;
            if (_tokens.Size() > 0 && _tokens.Top().Data != "}")
                CheckComa();
        }
        if (_tokens.Size() == 0)
            throw JsonParseException(_line, "Unexpected EOF");
        else
            _line = _tokens.Pop().Line;
        j = obj;
        return (true);
    }
    return (false);
}

void Parser::CheckColon()
{
    if (_tokens.Size() == 0)
        throw JsonParseException(_line, "Unexpected EOF");
    if (_tokens.Top().Type != Lexer::ETokenType::BASIC
        || _tokens.Top().Data != ":")
        throw JsonParseException(_tokens.Top().Line, "Expected colon");
    _line = _tokens.Pop().Line;
}

void Parser::CheckComa()
{
    if (_tokens.Top().Type != Lexer::ETokenType::BASIC
        || _tokens.Top().Data != ",")
        throw JsonParseException(_tokens.Top().Line, "Expected coma");
    _line = _tokens.Pop().Line;
}

bool Parser::CheckArray(Json &j)
{
    if (_tokens.Top().Type == Lexer::ETokenType::BASIC
        && _tokens.Top().Data == "[")
    {
        _line = _tokens.Pop().Line;
        Json::Array arr;

        while (_tokens.Size() > 0 && _tokens.Top().Data != "]")
        {
            arr.Add(CheckJson());
            if (_tokens.Size() > 0 && _tokens.Top().Data != "]")
                CheckComa();
        }
        if (_tokens.Size() == 0)
            throw JsonParseException(_line, "Unexpected EOF");
        else
            _line = _tokens.Pop().Line;
        j = arr;
        return (true);
    }
    return (false);
}

bool Parser::CheckNumber(Json &j)
{
    if (_tokens.Top().Type != Lexer::ETokenType::NUMBER)
        return (false);
    j = Double::Parse(_tokens.Top().Data);
    _line = _tokens.Pop().Line;
    return (true);
}

bool Parser::CheckString(Json &j)
{
    if (_tokens.Top().Type != Lexer::ETokenType::STRING)
        return (false);
    j = _tokens.Top().Data;
    _line = _tokens.Pop().Line;
    return (true);
}

bool Parser::CheckBasic(Json &j)
{
    if (_tokens.Top().Type != Lexer::ETokenType::BASIC)
        return (false);
    auto tok = _tokens.Top();
    if (tok.Data == "false")
    {
        j = false;
        _line = _tokens.Pop().Line;
        return (true);
    }
    else if (tok.Data == "true")
    {
        j = true;
        _line = _tokens.Pop().Line;
        return (true);
    }
    else if (tok.Data == "null")
    {
        j = Json();
        _line = _tokens.Pop().Line;
        return (true);
    }
    return (false);
}

Json Parser::Parse()
{
    Json val = CheckJson();
    if (_tokens.Size() > 0)
        throw JsonParseException(_tokens.Top().Line, String("Unexpected token '") + _tokens.Top().Data + "'");
    return (val);
}