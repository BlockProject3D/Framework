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

#include "Framework/String.hpp"
#include "Framework/BinUtils.hpp"
#include "Framework/Collection/ArrayList.hpp"
#include "Framework/EvalException.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/Memory/Memory.hpp"
#include "Framework/Scalar.hpp"
#include <sstream>

using namespace bpf::memory;
using namespace bpf::collection;
using namespace bpf;

const String String::Empty = String();

String::String() noexcept
    : Data(nullptr)
    , StrLen(0)
    , UnicodeLen(0)
{
}

fint String::InternalToInt(const String &str)
{
    return (bpf::Int::Parse((str)));
}

fchar String::UTF32(const char *utf8char)
{
    if (utf8char[0] == '\0')
        return (0u);
    else if (utf8char[1] == '\0')
        return ((fchar)utf8char[0]); //Slight patch to make function run faster on ASCII codes
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
        char b[3] = {0};

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
        char b[4] = {0};

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
        char b[5] = {0};

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

Array<fchar> String::ToUTF32() const
{
    Array<fchar> arr(Len() + 1);

    for (fisize i = 0; i != Len(); ++i)
        arr[i] = operator[](i);
    return (arr);
}

String String::FromUTF32(const fchar *str)
{
    String res;
    fisize i = 0;

    while (str[i])
        res += str[i++];
    return (res);
}

Array<fchar16> String::ToUTF16() const
{
    ArrayList<fchar16> arr;

    for (fisize i = 0; i != Len(); ++i)
    {
        fchar u = operator[](i);
        if (u > 0x10FFFF)
            throw EvalException(String("Cannot represent U+") + ValueOf((void *)((intptr)u)) + " in UTF-16");
        if (u < 0x10000)
            arr.Add((fchar16)u);
        else
        {
            fchar up = u - 0x10000;
            fchar16 w1 = 0xD800;
            fchar16 w2 = 0xDC00;
            for (fint j = 0; j != 10; ++j)
            {
                SetBit(w1, 9 - j, GetBit(up, 19 - j));
                SetBit(w2, 9 - j, GetBit(up, 9 - j));
            }
            arr.Add(w1);
            arr.Add(w2);
        }
    }
    arr.Add(0);
    return (arr.ToArray());
}

String String::FromUTF16(const fchar16 *str)
{
    String res;
    fisize i = 0;

    while (str[i])
    {
        fchar16 w1 = str[i++];
        if (w1 < 0xD800 || w1 > 0xDFFF)
            res += (fchar)w1;
        else if (w1 >= 0xD800 && w1 <= 0xDBFF)
        {
            fchar16 w2 = str[i++];
            if (w2 == 0 || w2 < 0xDC00 || w2 > 0xDFFF)
                throw EvalException("Ill-formed UTF-16 code");
            fchar u = 0;
            for (fint j = 0; j != 10; ++j)
            {
                SetBit(u, 19 - j, GetBit(w1, 9 - j));
                SetBit(u, 9 - j, GetBit(w2, 9 - j));
            }
            res += (u + 0x10000);
        }
        else
            throw EvalException("Ill-formed UTF-16 code");
    }
    return (res);
}

String::String(const char *str)
    : Data(nullptr)
    , StrLen(0)
    , UnicodeLen(0)
{
    if (str == nullptr)
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
    : Data(nullptr)
    , StrLen(1)
    , UnicodeLen(1)
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
        str.Data = nullptr;
        str.StrLen = 0;
        str.UnicodeLen = 0;
    }
}

String::String(const String &s)
    : Data(static_cast<char *>(Memory::Malloc(sizeof(char) * (s.StrLen + 1))))
    , StrLen(s.StrLen)
    , UnicodeLen(s.UnicodeLen)
{
    CopyString(s.Data, Data, s.StrLen);
}

String::String(String &&s) noexcept
    : Data(s.Data)
    , StrLen(s.StrLen)
    , UnicodeLen(s.UnicodeLen)
{
    s.Data = nullptr;
    s.StrLen = 0;
    s.UnicodeLen = 0;
}

void String::MakeSized(String &str, const fsize len)
{
    str.StrLen = len;
    str.Data = static_cast<char *>(Memory::Malloc(sizeof(char) * (len + 1)));
}

String &String::operator=(String &&other) noexcept
{
    Memory::Free(Data);
    Data = other.Data;
    StrLen = other.StrLen;
    UnicodeLen = other.UnicodeLen;
    other.Data = nullptr;
    other.StrLen = 0;
    other.UnicodeLen = 0;
    return (*this);
}

bool String::operator==(const String &other) const
{
    if (StrLen != other.StrLen)
        return (false);
    for (fsize i = 0; i < StrLen; i++)
    {
        if (other.Data[i] != Data[i])
            return (false);
    }
    return (true);
}

bool String::operator<(const String &other) const
{
    if (StrLen < other.StrLen)
        return (true);
    if (StrLen > other.StrLen)
        return (false);
    for (fsize i = 0; i < StrLen; i++)
    {
        if (Data[i] != other.Data[i])
            return (Data[i] < other.Data[i]);
    }
    return (false);
}

bool String::operator>(const String &other) const
{
    if (StrLen > other.StrLen)
        return (true);
    if (StrLen < other.StrLen)
        return (false);
    for (fsize i = 0; i < StrLen; i++)
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

void String::AddSingleByte(const char byte)
{
    Data = static_cast<char *>(Memory::Realloc(Data, sizeof(char) * (StrLen + 2)));
    Data[StrLen++] = byte;
    Data[StrLen] = '\0';
    UnicodeLen = CalcUnicodeLen(Data, StrLen);
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
    if (this == &other)
        return (*this);
    Memory::Free(Data);
    if (other.Data == nullptr)
    {
        Data = nullptr;
        StrLen = 0;
        UnicodeLen = 0;
        return (*this);
    }
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

void String::CopyString(const char *src, char *dest, const fsize len)
{
    const auto *fsrc = reinterpret_cast<const fsize *>(src);
    auto *fdest = reinterpret_cast<fsize *>(dest);
    fsize flen = len / sizeof(fsize);

    for (fsize i = 0; i < flen; ++i, ++fsrc, ++fdest)
        *fdest = *fsrc;
    dest = reinterpret_cast<char *>(fdest);
    src = reinterpret_cast<const char *>(fsrc);
    for (fsize i = flen * sizeof(fsize); i < len; ++i, ++dest, ++src)
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

fsize String::CalcUnicodeLen(const char *str, const fsize len)
{
    fsize ulen = 0;

    for (fsize i = 0; i < len; i++)
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

fsize String::CalcStartFromUnicode(const fsize start) const
{
    if (UnicodeLen == StrLen)
        return (start); //We are pure ASCII
    fsize i = 0;

    for (fsize j = 0; i < StrLen && j != start; ++i, ++j)
        i += CalcCharIncrement(Data[i]);
    return (i);
}

String String::Sub(const fisize begin, const fisize end) const
{
    String s;
    fsize min;
    fsize max;

    if ((fsize)begin > UnicodeLen || (fsize)end > UnicodeLen)
        return (*this);
    min = CalcStartFromUnicode((fsize)begin);
    max = CalcStartFromUnicode((fsize)end);
    if (max > min)
    {
        MakeSized(s, max - min);
        CopyString(Data + min, s.Data, s.StrLen);
        s.UnicodeLen = CalcUnicodeLen(s.Data, s.StrLen);
    }
    return (s);
}

String String::Sub(const fisize begin) const
{
    String s;
    fsize min;

    if ((fsize)begin > UnicodeLen)
        return (*this);
    min = CalcStartFromUnicode((fsize)begin);
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
    if (other.Data == nullptr)
        return (false);
    for (fsize i = 0; i < StrLen; i++)
    {
        if (Data[i] == other.Data[0] && my_strstr(other.Data, Data + i))
            return (true);
    }
    return (false);
}

bool String::Contains(const fchar other) const
{
    for (fsize i = 0; i < UnicodeLen; i++)
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
    for (fsize i = 0; i < StrLen; i++)
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
    String cur;
    ArrayList<String> l;

    for (fsize i = 0; i < StrLen; i++)
    {
        if (Data[i] != c)
            cur.AddSingleByte(Data[i]);
        else if (!cur.IsEmpty())
        {
            l.Add(cur);
            cur = "";
        }
    }
    if (!cur.IsEmpty())
        l.Add(cur);
    return (l.ToArray());
}

Array<String> String::ExplodeIgnore(const char c, const char ignore) const
{
    String cur;
    bool ign = false;
    ArrayList<String> l;

    for (fsize i = 0; i < StrLen; i++)
    {
        if (Data[i] == ignore)
            ign = !ign;
        if (Data[i] != c || ign)
            cur.AddSingleByte(Data[i]);
        else if (!cur.IsEmpty())
        {
            l.Add(cur);
            cur = "";
        }
    }
    if (!cur.IsEmpty())
        l.Add(cur);
    return (l.ToArray());
}

Array<String> String::Explode(const String &str) const
{
    String cur;
    ArrayList<String> l;

    for (fsize i = 0; i < StrLen; ++i)
    {
        if (Data[i] != str.Data[0])
            cur.AddSingleByte(Data[i]);
        else if (!cur.IsEmpty() && my_strstr(str.Data, Data + i))
        {
            l.Add(cur);
            cur = "";
        }
    }
    if (!cur.IsEmpty())
        l.Add(cur);
    return (l.ToArray());
}

Array<String> String::ExplodeIgnore(const String &str, const String &ignore) const
{
    String cur;
    bool ign = false;
    ArrayList<String> l;

    for (fsize i = 0; i < StrLen; ++i)
    {
        if (my_strstr(ignore.Data, Data + i))
            ign = !ign;
        if (Data[i] != str.Data[0] || ign)
            cur.AddSingleByte(Data[i]);
        else if (!cur.IsEmpty() && my_strstr(str.Data, Data + i))
        {
            l.Add(cur);
            cur = "";
        }
    }
    if (!cur.IsEmpty())
        l.Add(cur);
    return (l.ToArray());
}

Array<String> String::ExplodeOr(const String &str) const
{
    String cur;
    ArrayList<String> l;

    for (fsize i = 0; i < StrLen; i++)
    {
        if (!str.Contains(operator[](i)))
            cur.AddSingleByte(Data[i]);
        else if (!cur.IsEmpty())
        {
            l.Add(cur);
            cur = "";
        }
    }
    if (!cur.IsEmpty())
        l.Add(cur);
    return (l.ToArray());
}

bool String::StartsWith(const String &other) const
{
    if (StrLen < (fsize)other.Size())
        return (false);
    for (fsize i = 0; i < (fsize)other.Size(); i++)
        if (Data[i] != other.Data[i])
            return (false);
    return (true);
}

bool String::EndsWith(const String &other) const
{
    if (other.Size() == 0)
        return (true);
    if (StrLen < (fsize)other.Size())
        return (false);
    fsize i, j;
    for (j = StrLen - 1, i = other.Size() - 1; i > 0 && j > 0; i--, j--)
        if (Data[j] != other.Data[i])
            return (false);
    return (true);
}

String String::Replace(const String &search, const String &repby) const
{
    String str;

    for (fsize i = 0; i < StrLen; i++)
    {
        if (Data[i] == search.ByteAt(0) && my_strstr(Data + i, search.Data))
        {
            str += repby;
            i += search.Size() - 1;
        }
        else
            str.AddSingleByte(Data[i]);
    }
    return (str);
}

fisize String::IndexOf(const String &str) const
{
    fsize i;
    fsize charid = 0;

    for (i = 0; i != StrLen; ++i)
    {
        if (Data[i] == str.Data[0] && my_strstr(str.Data, Data + i))
            return (charid);
        i += CalcCharIncrement(Data[i]);
        ++charid;
    }
    return (-1);
}

fisize String::LastIndexOf(const String &str) const
{
    fisize i;
    fsize charid = UnicodeLen - 1;

    for (i = StrLen - 1; i >= 0; --i)
    {
        if (Data[i] == str.Data[0] && my_strstr(str.Data, Data + i))
            return (charid);
        if ((Data[i] & 0xC0) != 0x80)
            --charid;
    }
    return (-1);
}

fisize String::IndexOf(const char c) const
{
    fsize i;
    fsize charid = 0;

    for (i = 0; i != StrLen; ++i)
    {
        if (Data[i] == c)
            return (charid);
        i += CalcCharIncrement(Data[i]);
        ++charid;
    }
    return (-1);
}

fisize String::LastIndexOf(const char c) const
{
    fisize i;
    fsize charid = UnicodeLen - 1;

    for (i = StrLen - 1; i >= 0; --i)
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

    for (fsize i = 0; i < StrLen; ++i)
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

    for (fsize i = 0; i < StrLen; ++i)
    {
        if (CalcCharIncrement(Data[i]) <= 1 && Data[i] >= 'A' && Data[i] <= 'Z')
            res += (char)(Data[i] + 32);
        else
            res += (char)Data[i];
    }
    return (res);
}

String String::Reverse() const
{
    String res = "";

    if (UnicodeLen == 0)
        return (res);
    for (fsize i = UnicodeLen - 1; i > 0; --i)
        res += this->operator[](i);
    res += this->operator[](0);
    return (res);
}

String String::ValueOf(fint i, const fsize prec)
{
    std::stringstream strs;

    if (prec > 0) //Dev of stringstream you are stupid; let me show you how it's supposed to be done!
    {
        strs << i;
        String str = strs.str().c_str();
        auto len = (fisize)(prec - str.Len());
        if (len < 0)
            return (str);
        if (str[0] == '-')
            str = str.Sub(1);
        for (fisize j = 0; j < len; ++j)
            str = String('0') + str;
        if (i < 0)
            str = String('-') + str;
        return (str);
    }
    else
        strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(uint32 i, const fsize prec)
{
    std::stringstream strs;

    if (prec > 0)
    {
        strs << i;
        String str = strs.str().c_str();
        auto len = (fisize)(prec - str.Len());
        if (len < 0)
            return (str);
        for (fisize j = 0; j < len; ++j)
            str = String('0') + str;
        return (str);
    }
    else
        strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(uint64 i, const fsize prec)
{
    std::stringstream strs;

    if (prec > 0)
    {
        strs << i;
        String str = strs.str().c_str();
        auto len = (fisize)(prec - str.Len());
        if (len < 0)
            return (str);
        for (fisize j = 0; j < len; ++j)
            str = String('0') + str;
        return (str);
    }
    else
        strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(int64 i, const fsize prec)
{
    std::stringstream strs;

    if (prec > 0)
    {
        strs << i;
        String str = strs.str().c_str();
        auto len = (fisize)(prec - str.Len());
        if (len < 0)
            return (str);
        if (str[0] == '-')
            str = str.Sub(1);
        for (fisize j = 0; j < len; ++j)
            str = String('0') + str;
        if (i < 0)
            str = String('-') + str;
        return (str);
    }
    else
        strs << i;
    return (String(strs.str().c_str()));
}

String String::ValueOf(float f, const fsize prec)
{
    std::stringstream strs;

    if (prec > 0)
    {
        strs.precision(prec);
        strs << std::fixed << f;
    }
    else
        strs << f;
    return (String(strs.str().c_str()));
}

String String::ValueOf(double d, const fsize prec)
{
    std::stringstream strs;

    if (prec > 0)
    {
        strs.precision(prec);
        strs << std::fixed << d;
    }
    else
        strs << d;
    return (String(strs.str().c_str()));
}

String String::ValueOf(long double d, const fsize prec)
{
    std::stringstream strs;

    if (prec > 0)
    {
        strs.precision(prec);
        strs << std::fixed << d;
    }
    else
        strs << d;
    return (String(strs.str().c_str()));
}

String String::ValueOf(void *ptr, const fsize)
{
    std::stringstream strs;

    strs << std::hex << (uintptr)ptr;
    return (String("0x") + strs.str().c_str());
}
