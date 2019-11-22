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

#include <locale>
#include <sstream>
#include <cstdlib>
#include "Framework/Memory/Memory.hpp"
#include "Framework/EvalException.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/BinUtils.hpp"

using namespace bpf;

const String String::Empty = String();

String::String()
    : Data(Null), StrLen(0), UnicodeLen(0)
{
}

fchar String::UTF32(const char *utf8char)
{
    fchar res = 0;

    switch (CalcCharIncrement(utf8char[0]))
    {
    case 0:
        res = utf8char[0];
        break;
    case 1: //2 byte sequence
        res = (uint8)(utf8char[0] & 0x1F) << 6;
        res += utf8char[1] & 0x3F;
        break;
    case 2: //3 byte sequence
        res = (uint8)(utf8char[0] & 0xF) << 12;
        res += (uint8)(utf8char[1] & 0x3F) << 6;
        res += utf8char[2] & 0x3F;
        break;
    case 3: //4 byte sequence
        res = (uint8)(utf8char[0] & 0x7) << 20;
        res += (uint8)(utf8char[1] & 0x3F) << 12;
        res += (uint8)(utf8char[2] & 0x3F) << 6;
        res += utf8char[3] & 0x3F;
        break;
    }
    return (res);
}

String String::UTF8(const fchar utf32char)
{
    if (utf32char <= 0x7F)
        return (String((char)utf32char));
    else if (utf32char >= 0x80 && utf32char <= 0x7FF)
    {
        char b[3] = { 0 };

        SetBit(b[1], 0, GetBit(utf32char, 0));
        SetBit(b[1], 1, GetBit(utf32char, 1));
        SetBit(b[1], 2, GetBit(utf32char, 2));
        SetBit(b[1], 3, GetBit(utf32char, 3));
        SetBit(b[1], 4, GetBit(utf32char, 4));
        SetBit(b[1], 5, GetBit(utf32char, 5));
        SetBit(b[1], 6, false);
        SetBit(b[1], 7, true);

        SetBit(b[0], 0, GetBit(utf32char, 6));
        SetBit(b[0], 1, GetBit(utf32char, 7));
        SetBit(b[0], 2, GetBit(utf32char, 8));
        SetBit(b[0], 3, GetBit(utf32char, 9));
        SetBit(b[0], 4, GetBit(utf32char, 10));
        SetBit(b[0], 5, false);
        SetBit(b[0], 6, true);
        SetBit(b[0], 7, true);
        return (String(b));
    }
    else if (utf32char >= 0x800 && utf32char <= 0xFFFF)
    {
        char b[4] = { 0 };

        SetBit(b[2], 0, GetBit(utf32char, 0));
        SetBit(b[2], 1, GetBit(utf32char, 1));
        SetBit(b[2], 2, GetBit(utf32char, 2));
        SetBit(b[2], 3, GetBit(utf32char, 3));
        SetBit(b[2], 4, GetBit(utf32char, 4));
        SetBit(b[2], 5, GetBit(utf32char, 5));
        SetBit(b[2], 6, false);
        SetBit(b[2], 7, true);

        SetBit(b[1], 0, GetBit(utf32char, 6));
        SetBit(b[1], 1, GetBit(utf32char, 7));
        SetBit(b[1], 2, GetBit(utf32char, 8));
        SetBit(b[1], 3, GetBit(utf32char, 9));
        SetBit(b[1], 4, GetBit(utf32char, 10));
        SetBit(b[1], 5, GetBit(utf32char, 11));
        SetBit(b[1], 6, false);
        SetBit(b[1], 7, true);

        SetBit(b[0], 0, GetBit(utf32char, 12));
        SetBit(b[0], 1, GetBit(utf32char, 13));
        SetBit(b[0], 2, GetBit(utf32char, 14));
        SetBit(b[0], 3, GetBit(utf32char, 15));
        SetBit(b[0], 4, false);
        SetBit(b[0], 5, true);
        SetBit(b[0], 6, true);
        SetBit(b[0], 7, true);
        return (String(b));
    }
    else
    {
        char b[5] = { 0 };

        SetBit(b[3], 0, GetBit(utf32char, 0));
        SetBit(b[3], 1, GetBit(utf32char, 1));
        SetBit(b[3], 2, GetBit(utf32char, 2));
        SetBit(b[3], 3, GetBit(utf32char, 3));
        SetBit(b[3], 4, GetBit(utf32char, 4));
        SetBit(b[3], 5, GetBit(utf32char, 5));
        SetBit(b[3], 6, false);
        SetBit(b[3], 7, true);

        SetBit(b[2], 0, GetBit(utf32char, 6));
        SetBit(b[2], 1, GetBit(utf32char, 7));
        SetBit(b[2], 2, GetBit(utf32char, 8));
        SetBit(b[2], 3, GetBit(utf32char, 9));
        SetBit(b[2], 4, GetBit(utf32char, 10));
        SetBit(b[2], 5, GetBit(utf32char, 11));
        SetBit(b[2], 6, false);
        SetBit(b[2], 7, true);

        SetBit(b[1], 0, GetBit(utf32char, 12));
        SetBit(b[1], 1, GetBit(utf32char, 13));
        SetBit(b[1], 2, GetBit(utf32char, 14));
        SetBit(b[1], 3, GetBit(utf32char, 15));
        SetBit(b[1], 4, GetBit(utf32char, 16));
        SetBit(b[1], 5, GetBit(utf32char, 17));
        SetBit(b[1], 6, false);
        SetBit(b[1], 7, true);

        SetBit(b[0], 0, GetBit(utf32char, 18));
        SetBit(b[0], 1, GetBit(utf32char, 19));
        SetBit(b[0], 2, GetBit(utf32char, 20));
        SetBit(b[0], 3, false);
        SetBit(b[0], 4, true);
        SetBit(b[0], 5, true);
        SetBit(b[0], 6, true);
        SetBit(b[0], 7, true);
        return (String(b));
    }
}

String::String(const char *str)
    : Data(Null), StrLen(0), UnicodeLen(0)
{
    if (str == Null)
        str = "(NULL)";
    for (char const *strIndex = str; *strIndex; ++UnicodeLen)
    {
        uint8 charLen = CalcCharIncrement(*strIndex) + 1;

        StrLen += charLen;
        strIndex += charLen;
    }
    Data = static_cast<char *>(Memory::Malloc(sizeof(char) * (StrLen + 1)));
    CopyString(str, Data, StrLen);
}

String::String(const fchar c)
    : Data(Null), StrLen(1), UnicodeLen(1)
{
    if (c < 0x7F)
    {
        Data = static_cast<char *>(Memory::Malloc(sizeof(char) * (StrLen + 1)));
        Data[0] = (char)c;
        Data[1] = '\0';
    }
    else
    {
        String str = UTF8(c);
        Data = str.Data;
        StrLen = str.StrLen;
        UnicodeLen = str.UnicodeLen;
        str.Data = Null;
        str.StrLen = 0;
        str.UnicodeLen = 0;
    }
}

String::String(const String &s)
    : Data(static_cast<char *>(Memory::Malloc(sizeof(char) * (s.StrLen + 1)))),
    StrLen(s.StrLen), UnicodeLen(s.UnicodeLen)
{
    CopyString(s.Data, Data, s.StrLen);
}

String::String(String &&s)
    : Data(s.Data), StrLen(s.StrLen), UnicodeLen(s.UnicodeLen)
{
    s.Data = Null;
    s.StrLen = 0;
    s.UnicodeLen = 0;
}

void String::MakeSized(String &str, const uint32 len) const
{
    str.StrLen = len;
    str.Data = static_cast<char *>(Memory::Malloc(sizeof(char) * (len + 1)));
}

String &String::operator=(String &&other)
{
    Memory::Free(Data);
    Data = other.Data;
    StrLen = other.StrLen;
    UnicodeLen = other.UnicodeLen;
    other.Data = NULL;
    other.StrLen = 0;
    other.UnicodeLen = 0;
    return (*this);
}

bool String::operator==(const String &other) const
{
    if (StrLen != other.StrLen)
        return (false);
    for (uint32 i = 0; i < StrLen; i++)
    {
        if (other.Data[i] != Data[i])
            return (false);
    }
    return (true);
}

bool String::operator<(const String &other) const
{
    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] != other.Data[i])
            return (Data[i] < other.Data[i]);
    }
    return (false);
}

bool String::operator>(const String &other) const
{
    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] != other.Data[i])
            return (Data[i] > other.Data[i]);
    }
    return (false);
}

String String::operator+(const String &other) const
{
    String str;

    MakeSized(str, StrLen + other.StrLen);
    CopyString(Data, str.Data, StrLen);
    CopyString(other.Data, str.Data + StrLen, other.StrLen);
    str.UnicodeLen = UnicodeLen + other.UnicodeLen;
    return (str);
}

String String::operator+(const fchar other) const
{
    if (other <= 0x7F)
    {
        String str;

        MakeSized(str, StrLen + 1);
        CopyString(Data, str.Data, StrLen);
        str.Data[str.StrLen - 1] = (char)other;
        str.UnicodeLen = UnicodeLen + 1;
        str.Data[str.StrLen] = '\0';
        return (str);
    }
    else
        return (operator+(UTF8(other)));
}

String &String::operator+=(const String &other)
{
    Data = static_cast<char *>(Memory::Realloc(Data, sizeof(char) * (StrLen + other.StrLen + 1)));
    CopyString(other.Data, Data + StrLen, other.StrLen);
    StrLen += other.StrLen;
    UnicodeLen += other.UnicodeLen;
    return (*this);
}

String &String::operator+=(const fchar other)
{
    if (other <= 0x7F)
    {
        Data = static_cast<char *>(Memory::Realloc(Data, sizeof(char) * (StrLen + 2)));
        Data[StrLen++] = (char)other;
        Data[StrLen] = '\0';
        ++UnicodeLen;
        return (*this);
    }
    else
        return (operator+=(UTF8(other)));
}

String &String::operator=(const String &other)
{
    Memory::Free(Data);
    Data = static_cast<char *>(Memory::Malloc(sizeof(char) * (other.StrLen + 1)));
    StrLen = other.StrLen;
    CopyString(other.Data, Data, StrLen);
    UnicodeLen = other.UnicodeLen;
    return (*this);
}

String::~String()
{
    Memory::Free(Data);
}

void String::CopyString(const char *src, char *dest, const uint32 len) const
{
    fsize const *fsrc = reinterpret_cast<fsize const *>(src);
    fsize *fdest = reinterpret_cast<fsize *>(dest);
    uint32 flen = len / sizeof(fsize);

    for (uint32 i = 0; i < flen; ++i, ++fsrc, ++fdest)
        *fdest = *fsrc;
    dest = reinterpret_cast<char *>(fdest);
    src = reinterpret_cast<char const *>(fsrc);
    for (uint32 i = flen * sizeof(fsize); i < len; ++i, ++dest, ++src)
        *dest = *src;
    *dest = '\0';
}

uint8 String::CalcCharIncrement(const char c) // 1101 & 1111
{
    switch (c & 0xF0) //Identified UTF-8 sequence with 4 first strong bits
    {
    case 0xC0: // 1100
        return 1;
    case 0xE0: // 1110
        return 2;
    case 0xF0: // 1111
        return 3;
    }
    return 0;
}

uint32 String::CalcUnicodeLen(const char *str, const uint32 len) const
{
    uint32 ulen = 0;
    uint32 i = 0;

    for (i = 0; i < len; i++)
    {
        i += CalcCharIncrement(str[i]);
        ++ulen;
    }
    return (ulen);
}

Array<char> String::ToArray() const
{
    Array<char> arr(StrLen + 1);

    CopyString(Data, *arr, StrLen);
    arr[StrLen] = '\0';
    return (arr);
}

uint32 String::CalcStartFromUnicode(const uint32 start) const
{
    uint32 i = 0;

    for (uint32 j = 0 ; i < StrLen && j != start; ++i, ++j)
        i += CalcCharIncrement(Data[i]);
    return (i);
}

String String::Sub(const fint begin, const fint end) const
{
    String s;
    uint32 min;
    uint32 max;

    if ((uint32)begin > UnicodeLen || (uint32)end > UnicodeLen)
        return (*this);
    min = CalcStartFromUnicode((uint32)begin);
    max = CalcStartFromUnicode((uint32)end);
    if (max > min)
    {
        MakeSized(s, max - min);
        CopyString(Data + min, s.Data, s.StrLen);
        s.UnicodeLen = CalcUnicodeLen(s.Data, s.StrLen);
    }
    return (s);
}

String String::Sub(const fint begin) const
{
    String s;
    uint32 min;

    if ((uint32)begin > UnicodeLen)
        return (*this);
    min = CalcStartFromUnicode((uint32)begin);
    if (StrLen > min)
    {
        MakeSized(s, StrLen - min);
        CopyString(Data + min, s.Data, s.StrLen);
        s.UnicodeLen = CalcUnicodeLen(s.Data, s.StrLen);
    }
    return (s);
}

static bool my_strstr(char *pathern, char *str)
{
    while (*pathern && *str)
    {
        if (*pathern++ != *str++)
            return (false);
    }
    return (true);
}

bool String::Contains(const String &other) const
{
    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] == other.Data[0] && my_strstr(other.Data, Data + i))
            return (true);
    }
    return (false);
}

bool String::Contains(const fchar other) const
{
    for (uint32 i = 0; i < UnicodeLen; i++)
    {
        if (operator[](i) == other)
            return (true);
    }
    return (false);
}

bool String::IsNumeric() const
{
    bool coma = false;

    if (UnicodeLen <= 0)
        return (false);
    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] == '.' && coma)
            return (false);
        if (Data[i] == '.')
            coma = true;
        if (Data[i] == '-' && i != 0)
            return (false);
        if (!(Data[i] >= 48 && Data[i] <= 57) && Data[i] != '.' && Data[i] != '-')
            return (false);
    }
    return (true);
}

Array<String> String::Explode(const char c) const
{
    Array<String> l;
	int k = 0;
    String cur;

    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] != c)
            cur += Data[i];
        else if (cur != String::Empty)
        {
            l[k++] = cur;
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
		l[k++] = cur;
    return (l);
}

Array<String> String::ExplodeIgnore(const char c, const char ignore) const
{
    String cur;
	int k = 0;
    bool ign = false;
	Array<String> l;

    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] == ignore)
            ign = !ign;
        if (Data[i] != c || ign)
            cur += Data[i];
        else if (cur != String::Empty)
        {
            l[k++] = cur;
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
		l[k++] = cur;
    return (l);
}

Array<String> String::Explode(const String &str) const
{
    String cur;
	int k = 0;
    Array<String> l;

    for (uint32 i = 0 ; i < StrLen ; ++i)
    {
        if (Data[i] != str.Data[0])
            cur += Data[i];
        else if (cur != String::Empty && my_strstr(str.Data, Data + i))
        {
			l[k++] = cur;
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
		l[k++] = cur;
    return (l);
}

Array<String> String::ExplodeIgnore(const String &str, const String &ignore) const
{
    String cur;
	int k = 0;
    bool ign = false;
	Array<String> l;

    for (uint32 i = 0 ; i < StrLen ; ++i)
    {
        if (my_strstr(ignore.Data, Data + i))
            ign = !ign;
        if (Data[i] != str.Data[0] || ign)
            cur += Data[i];
        else if (cur != String::Empty && my_strstr(str.Data, Data + i))
        {
			l[k++] = cur;
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
		l[k++] = cur;
    return (l);
}

Array<String> String::ExplodeOr(const String &str) const
{
    String cur;
	int k = 0;
	Array<String> l;

    for (uint32 i = 0; i < StrLen; i++)
    {
        if (!str.Contains(operator[](i)))
            cur += Data[i];
        else if (cur != String::Empty)
        {
			l[k++] = cur;
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
		l[k++] = cur;
    return (l);
}

bool String::StartsWith(const String &other) const
{
    if (StrLen < (uint32)other.Size())
        return (false);
    for (uint32 i = 0 ; i < (uint32)other.Size() ; i++)
        if (Data[i] != other.Data[i])
            return (false);
    return (true);
}

bool String::EndsWith(const String &other) const
{
    if (other.Size() == 0)
        return (true);
    if (StrLen < (uint32)other.Size())
        return (false);
    uint32 i, j;
    for (j = StrLen - 1, i = other.Size() - 1 ; i > 0 && j > 0 ; i--, j--)
        if (Data[j] != other.Data[i])
            return (false);
    return (true);
}

String String::Replace(const String &search, const String &repby) const
{
    String str;

    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] == search.ByteAt(0) && my_strstr(Data + i, search.Data))
            str += repby;
        else
            str += Data[i];
    }
    return (str);
}

fint String::IndexOf(const String &str) const
{
    uint32 i;
    uint32 charid = 0;

    for (i = 0 ; i < StrLen ; ++i)
    {
        if (Data[i] == str.Data[0] && my_strstr(str.Data, Data + i))
            return (charid);
        i += CalcCharIncrement(Data[i]);
        ++charid;
    }
    return (-1);
}

fint String::LastIndexOf(const String &str) const
{
    fint i;
    fint charid = UnicodeLen - 1;

    for (i = StrLen - 1 ; i >= 0 ; --i)
    {
        if (Data[i] == str.Data[0] && my_strstr(str.Data, Data + i))
            return (charid);
        if ((Data[i] & 0xC0) != 0x80)
            --charid;
    }
    return (-1);
}

fint String::IndexOf(const char c) const
{
    uint32 i;
    uint32 charid = 0;

    for (i = 0 ; i < StrLen ; ++i)
    {
        if (Data[i] == c)
            return (charid);
        i += CalcCharIncrement(Data[i]);
        ++charid;
    }
    return (-1);
}

fint String::LastIndexOf(const char c) const
{
    fint i;
    fint charid = UnicodeLen - 1;
    
    for (i = StrLen - 1 ; i >= 0 ; --i)
    {
        if (Data[i] == c)
            return (charid);
        if ((Data[i] & 0xC0) != 0x80)
            --charid;
    }
    return (-1);
}

String String::ToUpper() const
{
    String res;

    for (uint32 i = 0 ; i < StrLen ; ++i)
    {
        if (CalcCharIncrement(Data[i]) <= 1 && Data[i] >= 'a' && Data[i] <= 'z')
            res += (char)(Data[i] - 32);
        else
            res += (char)Data[i];
    }
    return (res);
}

String String::ToLower() const
{
    String res;

    for (uint32 i = 0 ; i < StrLen ; ++i)
    {
        if (CalcCharIncrement(Data[i]) <= 1 && Data[i] >= 'A' && Data[i] <= 'Z')
            res += (char)(Data[i] + 32);
        else
            res += (char)Data[i];
    }
    return (res);
}

String String::ValueOf(fint i)
{
    std::stringstream    strs;

    strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(uint32 i)
{
    std::stringstream    strs;

    strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(uint64 i)
{
    std::stringstream    strs;

    strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(int64 i)
{
    std::stringstream    strs;

    strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(float f)
{
    std::stringstream    strs;

    strs << f;
    return (String(strs.str().c_str()));
}

String String::ValueOf(double d)
{
    std::stringstream    strs;

    strs << d;
    return (String(strs.str().c_str()));
}

String String::ValueOf(void *ptr)
{
    std::stringstream    strs;

    strs << std::hex << (uintptr)ptr;
    return (strs.str().c_str());
}
