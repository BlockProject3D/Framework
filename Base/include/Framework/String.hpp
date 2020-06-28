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

#pragma once
#include <type_traits>
#include <cstdlib>
#include "Framework/Types.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/Collection/Array.hpp"
#include "Framework/IndexException.hpp"
#include "Framework/Api.hpp"

namespace bpf
{
    class BPF_API String
    {
    private:
        char *Data;
        fsize StrLen;
        fsize UnicodeLen;

        void CopyString(const char *src, char *dest, const fsize len) const;
        fsize CalcUnicodeLen(const char *str, const fsize len) const;
        fsize CalcStartFromUnicode(const fsize start) const;
        static uint8 CalcCharIncrement(const char c);
        void MakeSized(String &str, const fsize len) const;

        //* '[]' for no format and [(.precision,)(<num chars padding>,<allignment (left / right)>,<characters to serve as padding>)]
        template <typename T>
        static String FormatSingle(const collection::Array<String> &tokens, T &&t)
        {
            fsize tokId = 0;
            if (tokens.Size() == 0)
                return (ValueOf(t));
            fsize prec = 0;
            if (tokens[0][0] == '.')
            {
                prec = std::atoi(*tokens[0].Sub(1));
                ++tokId;
            }
            if (tokens.Size() - tokId < 3)
                return (ValueOf(t, prec));
            fint maxn = std::atoi(*tokens[tokId]);
            bool left = tokens[tokId + 1] == "left";
            String leading = tokens[tokId + 2];
            String data = ValueOf(t, prec);
            if (data.Len() > maxn)
                data = data.Sub(0, maxn);
            else if (data.Len() < maxn)
            {
                fisize remain = maxn - data.Len();
                for (int k = 0; k < remain; ++k)
                {
                    if (left)
                        data += leading;
                    else
                        data = leading + data;
                }
            }
            return (data);
        }
    public:
        template <typename T>
        class Stringifier
        {
        public:
            inline static String Stringify(const T &obj, const fsize = 0)
            {
                return (obj);
            }
        };

        /**
         * Constructs a new empty string
         */
        String();

        /**
         * Constructs a new string from a string literal
         * @param str pointer to an array of bytes containing UTF-8 data
         */
        String(const char *str);

        /**
         * Constructs a new string from a single character
         * @param c the character to copy in the string
         */
        String(const fchar c);

        /**
         * Copy constructor
         */
        String(const String &s);

        /**
         * Move constructor
         */
        String(String &&s);

        ~String();

        /**
         * Low level function to adds a single byte at the end of this string
         * This function ignores wether or not byte is part of a UTF8 code
         * StrLen will be incremented of 1 and UnicodeLen will be recalculated accordingly
         */
        void AddSingleByte(const char byte);

        /**
         * Returns a UTF32 character from a single UTF8 code
         * @param utf8char the utf8 null-terminated byte sequence to convert to a single UTF32 character
         */
        static fchar UTF32(const char *utf8char);

        /**
         * Returns a UTF8 high level String from a single UTF32 code
         * @param utf32char the utf32 character to convert to a UTF8 String
         */
        static String UTF8(const fchar utf32char); // TODO : Implement

        /**
         * Returns a byte at a given index
         * @param id the index of the byte to return
         * @throws IndexException
         */
        inline char ByteAt(const fisize id) const
        {
            if (id < 0 || id >= Size())
                throw IndexException(id);
            return ((char)(Data[id]));
        }

        /**
         * Returns a new string from concatenation of this + other
         */
        String operator+(const String &other) const;

        /**
         * Returns a new string from concatenation of this + other
         */
        String operator+(const fchar other) const;

        /**
         * Appends a string at the end of this string
         */
        String &operator+=(const String &other);

        /**
         * Appends a UTF32 code at the end of this string
         */
        String &operator+=(const fchar other);

        /**
         * Copy assignment operator
         */
        String &operator=(const String &other);

        /**
         * Move assignment operator
         */
        String &operator=(String &&other);

        /**
         * Checks if this string is empty
         * @return true if this string is null or 0 bytes long, false otherwise
         */
        inline bool IsEmpty() const noexcept
        {
            return (Data == Null || StrLen == 0);
        }

        /**
         * Returns true if this string is equal to other
         * @param other the other string to compare with
         */
        bool operator==(const String &other) const;

        /**
         * Returns true if this string is not equal to other
         * @param other the other string to compare with
         */
        inline bool operator!=(const String &other) const
        {
            return (!operator==(other));
        }

        /**
         * Compares this string with another : returns true if this is less than other
         * @param other the other string to compare with
         */
        bool operator<(const String &other) const;

        /**
         * Compares this string with another : returns true if this is greater than other
         * @param other the other string to compare with
         */
        bool operator>(const String &other) const;

        /**
         * Returns a new string with all occurenses of search replaced by repby
         * @param search the string to search for
         * @param repby the string to replace occurenses of search with
         */
        String Replace(const String &search, const String &repby) const;

        /**
         * Returns a character of this string at offset id
         * @param id the offset of the character to get
         * @throws IndexException
         */
        inline fchar operator[](const fisize id) const
        {
            if (id < 0 || id > Len())
                throw IndexException(id);
            return (String::UTF32(Data + CalcStartFromUnicode(id)));
        }

        /**
         * Returns the data of this string ready for printing
         */
        inline const char *operator*() const
        {
            return (Data);
        }

        collection::Array<char> ToArray() const;
        collection::Array<fchar> ToUTF32() const;
        collection::Array<fchar16> ToUTF16() const;
        static String FromUTF16(const fchar16 *str);
        static String FromUTF32(const fchar *str);

        /**
         * Returns the number of characters in this string
         */
        inline fisize Len() const
        {
            return (UnicodeLen);
        }

        /**
         * Returns the size in bytes of this string
         */
        inline fisize Size() const
        {
            return (StrLen);
        }

        /**
         * Is this string only composed of numeric chracters
         */
        bool IsNumeric() const;

        /**
         * Returns a copy of this string in all upper case
         * Unicode not supported
         */
        String ToUpper() const;

        /**
         * Returns a copy of this string in all lower case
         * Unicode not supported
         */
        String ToLower() const;

        /**
         * Returns a copy of this string reversed
         */
        String Reverse() const;

        /**
         * Returns the index of the first occurence of str in this
         */
        fisize IndexOf(const String &str) const;

        /**
         * Returns the index of the last occurence of str in this
         */
        fisize LastIndexOf(const String &str) const;

        /**
         * Returns the index of the first occurence of c in this
         */
        fisize IndexOf(const char c) const;

        /**
         * Returns the index of the last occurence of c in this
         */
        fisize LastIndexOf(const char c) const;

        /**
         * Returns the index of the first occurence of c in this
         */
        inline fisize IndexOf(const fchar c) const
        {
            return (IndexOf(UTF8(c)));
        }

        /**
         * Returns the index of the last occurence of c in this
         */
        inline fisize LastIndexOf(const fchar c) const
        {
            return (LastIndexOf(UTF8(c)));
        }

        /**
         * Splits this string using a delimiter
         * @param c the sperator char
         * @return array of tokens
         */
        collection::Array<String> Explode(const char c) const;

        /**
         * Splits this string using a delimiter
         * @param c the seperator char
         * @param ignore character to identify escape sequences (all characters between two of this ignore char will be interpreted as a single token)
         * @return array of tokens
         */
        collection::Array<String> ExplodeIgnore(const char c, const char ignore) const;

        /**
         * Splits this string using a delimiter
         * @param str the separator string
         * @return array of tokens
         */
        collection::Array<String> Explode(const String &str) const;

        /**
         * Splits this string using a delimiter
         * @param str the separator string
         * @param ignore string to identify escape sequences (all characters between two of this ignore string will be interpreted as a single token)
         * @return array of tokens
         */
        collection::Array<String> ExplodeIgnore(const String &str, const String &ignore) const;

        /**
         * Splits this string using multiple delimiters
         * @param str the delimiters
         * @return array of tokens
         */
        collection::Array<String> ExplodeOr(const String &str) const;

        /**
         * Returns true if this string starts with other
         */
        bool StartsWith(const String &other) const;

        /**
         * Returns true if this string ends with other
         */
        bool EndsWith(const String &other) const;

        /**
         * Returns true if this string contains other
         */
        bool Contains(const String &other) const;

        bool Contains(const fchar other) const;

        /**
         * Returns a substring
         * @param begin the begin index in characters (inclusive)
         * @param count the amount of characters to read
         */
        inline String SubLen(const fisize begin, const fisize count = -1) const
        {
            if (count < 0)
                return (Sub(begin));
            return (Sub(begin, begin + count));
        }

        /**
         * Returns a substring
         * @param begin the begin index in characters (inclusive)
         * @param end the end index in characters (exclusive)
         */
        String Sub(const fisize begin, const fisize end) const;

        /**
         * Returns a substring where the end is the end of this string
         * @param begin the begin index in characters (inclusive)
         */
        String Sub(const fisize begin) const;

        /**
         * Builds a string using the following formating syntax : '[]' for no format and [<num chars padding>,<allignment (left / right)>,<characters to serve as padding>]
         * @tparam Args objects / scalar types to format
         * @param format the given format
         */
        template <typename ...Args>
        inline static String Format(const String &format, Args &&...)
        {
            return (format);
        }

        //TODO : Add support for center alignment
        /**
         * Builds a string using the following formating syntax :
         * '[]' for no format and [(.precision,)(<num chars padding>,<allignment (left / right)>,<characters to serve as padding>)]
         * @tparam Args objects / scalar types to format
         * @param format the given format
         */
        template <typename T, typename ...Args>
        static String Format(const String &format, T &&t, Args &&... args)
        {
            String res;

            fisize i = format.IndexOf('[');
            fisize j = format.IndexOf(']');
            if (i > -1 && j > -1 && format.Sub(i - 1, i) != "\\")
            {
                res += format.Sub(0, i);
                String pattern = format.Sub(i + 1, j);
                collection::Array<String> tokens = pattern.Explode(',');
                res += FormatSingle(tokens, std::forward<T>(t));
                return (res + Format(format.Sub(j + 1), args...));
            }
            else
            {
                res = format.Sub(0, format.IndexOf('\\'));
                res += format.Sub(format.IndexOf('\\') + 1);
                return (res);
            }
        }

        /**
         * Converts any object to it's string representation by calling ToString on it
         * @param val value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        template <typename T, typename std::enable_if<std::is_class<T>::value>::type * = nullptr>
        inline static String ValueOf(const T &val, const fsize prec = 0)
        {
            return (Stringifier<T>::Stringify(val, prec));
        }

        /**
         * Converts any pointer type to it's string representation
         * @param val value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type * = nullptr>
        inline static String ValueOf(T val, const fsize prec = 0)
        {
            return (ValueOf((void *)val));
        }

        /**
         * Converts a signed integer 32 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(fint i, const fsize prec = 0);

        /**
         * Converts an unsigned integer 32 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(uint32 i, const fsize prec = 0);

        /**
         * Converts an unsigned integer 64 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(uint64 i, const fsize prec = 0);

        /**
         * Converts a signed integer 64 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(int64 i, const fsize prec = 0);

        /**
         * Converts a float to it's string representation
         * @param f value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(float f, const fsize prec = 0);

        /**
         * Converts a double to it's string representation
         * @param d value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(double d, const fsize prec = 0);

        /**
         * Converts a double to it's string representation
         * @param d value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(long double d, const fsize prec = 0);

        /**
         * Converts a raw pointer to it's string representation
         * @param ptr value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        static String ValueOf(void *ptr, const fsize prec = 0);

        /**
         * Converts a signed integer 8 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        inline static String ValueOf(int8 i, const fsize = 0)
        {
            return (ValueOf(static_cast<int>(i)));
        }

        /**
         * Converts an unsigned integer 8 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        inline static String ValueOf(uint8 i, const fsize = 0)
        {
            return (ValueOf(static_cast<int>(i)));
        }

        /**
         * Converts a signed integer 16 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        inline static String ValueOf(int16 i, const fsize = 0)
        {
            return (ValueOf(static_cast<int>(i)));
        }

        /**
         * Converts an unsigned integer 16 bits to it's string representation
         * @param i value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        inline static String ValueOf(uint16 i, const fsize = 0)
        {
            return (ValueOf(static_cast<int>(i)));
        }

        /**
         * Converts a low-level C string to a high level string
         * @param s value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        inline static String ValueOf(const char *s, const fsize = 0)
        {
            return (String(s));
        }

        /**
         * Converts a bool to it's string representation
         * @param b value to convert to a string
         * @param prec precision for numeric types (0 means max precision)
         */
        inline static String ValueOf(bool b, const fsize = 0)
        {
            return (b ? "TRUE" : "FALSE");
        }

        /**
         * Empty string constant
         */
        BP_DEPRECATED("To check for empty string use String.IsEmpty, to create an empty string use litteral \"\". This constant will be removed in a future version.") static const String Empty;
    };
}
