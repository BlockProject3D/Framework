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

#include <iostream>
#include "Framework/Json/Lexer.hpp"
#include "Framework/Json/JsonParseException.hpp"
#include "Framework/Scalar.hpp"

using namespace bpf::collection;
using namespace bpf::json;
using namespace bpf;

Lexer::Lexer()
    : _line(1)
    , _enableComments(false)
{
}

void Lexer::LoadString(const String &input)
{
    String token;

    _comment = false;
    _lineComment = false;
    _string = false;
    _cursor = 0;
    while (_cursor < input.Len())
    {
        token += input[_cursor];
        if (CheckString(token)
            || CheckBasic(token, _cursor < input.Len() ? input[_cursor + 1] : '\0')
            || CheckNumber(token, _cursor < input.Len() ? input[_cursor + 1] : '\0'))
            token = String::Empty;
        ++_cursor;
    }
    if (token != String::Empty)
        throw JsonParseException(_line, String("Unexpected token '") + token + "'");
}

Queue<Lexer::Token> Lexer::ReadTokens()
{
    return (std::move(_tokens));
}

fchar Lexer::ProcessUnicode(const String &str, int &pos)
{
    String nbr = String::Empty;
    int i = pos + 2;

    for (; i != str.Len(); ++i)
    {
        if (str[i] >= 48 && str[i] <= 57)
            nbr += str[i];
    }
    pos = i;
    return (UInt::Parse(nbr));
}

fchar Lexer::ProcessStandard(const String &str, int &pos)
{
    if (pos + 1 >= str.Len())
    {
        ++pos;
        return (str[pos - 1]);
    }
    ++pos;
    switch (str[pos])
    {
    case 't':
        return ('\t');
    case 'b':
        return ('\b');
    default:
        return (str[pos]);
    }
}

String Lexer::ReProcessString(const String &str)
{
    String copy = str.Sub(1, str.Len() - 1);
    String res = String::Empty;

    for (int i = 0; i < copy.Len(); ++i)
    {
        if (copy[i] == '\\')
        {
            if (i + 1 < copy.Len() && copy[i + 1] == 'u')
                res += ProcessUnicode(copy, i);
            else
                res += ProcessStandard(copy, i);
        }
        else
            res += copy[i];
    }
    return (res);
}

bool Lexer::CheckString(const String &token)
{
    if (_enableComments && (_comment || _lineComment))
        return (false);
    if (token == '"')
        _string = true;
    if (token.Len() > 1 && token[0] == '"' && token[token.Len() - 1] == '"'
        && ((token.Len() > 2 && token[token.Len() - 2] != '\\') || token.Len() <= 2))
    {
        Token tok;
        tok.Data = ReProcessString(token);
        tok.Line = _line;
        tok.Type = ETokenType::STRING;
        _tokens.Push(std::move(tok));
        _string = false;
        return (true);
    }
    return (false);
}

bool Lexer::CheckNumber(const String &token, const fchar next)
{
    if (_enableComments && (_comment || _lineComment))
        return (false);
    if (_string || token.Len() <= 0)
        return (false);
    if (token[0] == '-' && ((_curNbr.Len() > 0 && _curExponent.Len() > 1) || (_curNbr.Len() > 0 && _curExponent.Len() == 0)))
        throw JsonParseException(_line, "Invalid number format");
    else if (token[0] == '-')
    {
        if (_curNbr.Len() > 0)
            _curExponent += '-';
        else
            _curNbr += '-';
        return (true);
    }
    else if (token.IsNumeric() && _curExponent == String::Empty)
    {
        _curNbr += token;
        if ((next < 48 || next > 57) && next != 'e' && next != 'E' && next != '.')
        {
            Token tok;
            tok.Data = _curNbr + _curExponent;
            tok.Line = _line;
            tok.Type = ETokenType::NUMBER;
            _tokens.Push(std::move(tok));
            _curNbr = String::Empty;
            _curExponent = String::Empty;
        }
        return (true);
    }
    else if (token[0] == 'e' || token[0] == 'E')
    {
        if (_curExponent.Len() > 0)
            throw JsonParseException(_line, "Invalid number format");
        _curExponent = 'e';
        return (true);
    }
    else if (token.IsNumeric() && _curExponent.Len() > 0)
    {
        _curExponent += token;
        if ((next < 48 || next > 57) && next != 'e' && next != 'E' && next != '.')
        {
            Token tok;
            tok.Data = _curNbr + _curExponent;
            tok.Line = _line;
            tok.Type = ETokenType::NUMBER;
            _tokens.Push(std::move(tok));
            _curNbr = String::Empty;
            _curExponent = String::Empty;
        }
        return (true);
    }
    return (false);
}

bool Lexer::CheckBasic(const String &token, const fchar next)
{
    if (_string)
        return (false);
    if (_enableComments)
    {
        if (token == "/*")
        {
            _comment = true;
            return (true);
        }
        else if (token == '*' && next == '/' && _comment)
        {
            _comment = false;
            ++_cursor;
            return (true);
        }
        else if (token == "//")
        {
            _lineComment = true;
            return (true);
        }
        else if (((token == '\r' && next == '\n') || token == '\n') && _lineComment)
        {
            if (token == '\r' && next == '\n')
                ++_cursor;
            _lineComment = false;
            ++_line;
            return (true);
        }
        else if (_comment || _lineComment)
            return (true);
    }
    if (token == "false"
        || token == "true"
        || token == "null"
        || token == '['
        || token == ']'
        || token == '{'
        || token == '}'
        || token == ':'
        || token == ',')
    {
        Token tok;
        tok.Data = token;
        tok.Line = _line;
        tok.Type = ETokenType::BASIC;
        _tokens.Push(std::move(tok));
        return (true);
    }
    if (token == ' ' || token == '\t' || token == "\r\n" || token == '\n')
    {
        if (token == "\r\n" || token == '\n')
            ++_line;
        return (true);
    }
    return (false);
}
