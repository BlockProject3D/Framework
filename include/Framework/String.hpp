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

#pragma once
#include <type_traits>
#include "Framework/Types.hpp"
#include "Framework/TypeInfo.hpp"
#include "Framework/List.hpp"
#include "Framework/Array.hpp"
#include "Framework/Hash.hpp"
#include "Framework/IndexException.hpp"

namespace bpf
{
    class BPF_API String
    {
    private:
        char *Data;
        uint32 StrLen;
        uint32 UnicodeLen;

        void CopyString(const char *src, char *dest, const uint32 len) const;
        uint32 CalcUnicodeLen(const char *str, const uint32 len) const;
        uint32 CalcStartFromUnicode(const uint32 start) const;
        static uint8 CalcCharIncrement(const char c);
        void MakeSized(String &str, const uint32 len) const;
    public:
        template <typename T>
        class Stringifier
        {
        public:
            inline static String Stringify(const T &obj)
            {
                return (obj.ToString());
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
        inline char ByteAt(const uint32 id) const
        {
            if (id >= StrLen)
                throw IndexException((int)id);
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
        inline fchar operator[](const int id) const
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

        Array<char> ToArray() const;

        /**
         * Returns the number of characters in this string
         */
        inline int Len() const
	{
	    return (UnicodeLen);
	}

        /**
         * Returns the size in bytes of this string
         */
        inline int Size() const
        {
            return (StrLen);
        }

        /**
         * Is this string only composed of numeric chracters
         */
        bool IsNumeric() const;

        /**
         * Returns a copy of this string in all upper case
         * Not working with non ascii characters
         */
        String ToUpper() const;

        /**
         * Returns a copy of this string in all lower case
         * Not working with non ascii characters
         */
        String ToLower() const;

        /**
         * Returns the index of the first occurence of str in this
         */
        int IndexOf(const String &str) const;

        /**
         * Returns the index of the last occurence of str in this
         */
        int LastIndexOf(const String &str) const;

        /**
         * Returns the index of the first occurence of c in this
         */
        int IndexOf(const char c) const;

        /**
         * Returns the index of the last occurence of c in this
         */
        int LastIndexOf(const char c) const;

        /**
         * Splits this string using a delimiter
         * @param c the sperator char
         * @return list of tokens
         */
        List<String> Explode(const char c) const;

        /**
         * Splits this string using a delimiter
         * @param c the seperator char
         * @param ignore character to identify escape sequences (all characters between two of this ignore char will be interpreted as a single token)
         * @return list of tokens
         */
        List<String> ExplodeIgnore(const char c, const char ignore) const;

        /**
         * Splits this string using a delimiter
         * @param str the separator string
         * @return list of tokens
         */
        List<String> Explode(const String &str) const;

        /**
         * Splits this string using a delimiter
         * @param str the separator string
         * @param ignore string to identify escape sequences (all characters between two of this ignore string will be interpreted as a single token)
         * @return list of tokens
         */
        List<String> ExplodeIgnore(const String &str, const String &ignore) const;

        /**
         * Splits this string using multiple delimiters
         * @param str the delimiters
         * @return list of tokens
         */
        List<String> ExplodeOr(const String &str) const;

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
        inline String SubLen(const int begin, const int count = -1) const
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
        String Sub(const int begin, const int end) const;

        /**
         * Returns a substring where the end is the end of this string
         * @param begin the begin index in characters (inclusive)
         */
        String Sub(const int begin) const;

        /**
         * Converts this string to an integer
         */
        int ToInt() const;

        /**
         * Converts this string to a float
         */
        float ToFloat() const;

        /**
         * Converts this string to a double
         */
        double ToDouble() const;

        /**
         * Evaluate a mathematical expression
         * @throws EvalException
         */
        static double Evaluate(const char *expr);

        /**
         * Evaluate a mathematical expression from the contents of the FString
         * @throws EvalException
         */
        inline double Evaluate()
        {
            return (String::Evaluate(Data));
        }

        /**
         * Builds a string using the following formating syntax : '[]' for no format and [<num chars padding>,<allignment (left / right)>,<characters to serve as padding>]
         * @tparam Args objects / scalar types to format
         * @param format the given format
         */
        template <typename ...Args>
        inline static String Format(const String &format, Args&&...)
      	{
            return (format);
      	}

        //TODO : Add precision support
        //TODO : Add support for center alignment
        /**
         * Builds a string using the following formating syntax :
         * '[]' for no format and [<num chars padding>,<allignment (left / right)>,<characters to serve as padding>]
         * @tparam Args objects / scalar types to format
         * @param format the given format
         */
        template <typename T, typename ...Args>
        static String Format(const String &format, T &&t, Args&&... args)
        {
            String res;

            int i = format.IndexOf('[');
            int j = format.IndexOf(']');
            if (i > -1 && j > -1 && format.Sub(i - 1, i) != "\\")
            {
                res += format.Sub(0, i);
                String pattern = format.Sub(i + 1, j);
                List<String> tokens = pattern.Explode(',');
                if (tokens.Size() < 3)
                    return (res + ValueOf(t) + Format(format.Sub(j + 1), args...));
                int maxn = tokens[0].ToInt();
                bool left = tokens[1] == "left";
                String leading = tokens[2];
                String data = ValueOf(t);
                if (data.Len() > maxn)
                    data = data.Sub(0, maxn);
                else if (data.Len() < maxn)
                {
                    int remain = maxn - data.Len();
                    for (int k = 0 ; k < remain ; ++k)
                    {
                        if (left)
                            data += leading;
                        else
                            data = leading + data;
                    }
                }
                res += data;
                return (res + Format(format.Sub(j + 1), args...));
            }
            else
            {
                res = format.Sub(0, format.IndexOf('\\'));
                res += format.Sub(format.IndexOf('\\') + 1);
                return (res);
            }
        }

        inline String ToString() const
        {
            return (*this);
        }

        inline uint32 Hash() const
        {
            return (bpf::Hash<const char *>(Data));
        }

        /**
         * Converts any object to it's string representation by calling ToString on it
         */
        template <typename T, typename std::enable_if<std::is_class<T>::value>::type * = nullptr>
        inline static String ValueOf(const T &val)
        {
            return (Stringifier<T>::Stringify(val));
        }

        /**
         * Converts any pointer type to it's string representation
         */
        template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type * = nullptr>
        inline static String ValueOf(T val)
        {
            return (ValueOf((void *)val));
        }
        
        /**
         * Converts a signed integer 32 bits to it's string representation
         */
        static String ValueOf(int i);
        
        /**
         * Converts an unsigned integer 32 bits to it's string representation
         */
        static String ValueOf(uint32 i);
        
        /**
         * Converts an unsigned integer 64 bits to it's string representation
         */
        static String ValueOf(uint64 i);
        
        /**
         * Converts a signed integer 64 bits to it's string representation
         */
        static String ValueOf(int64 i);
        
        /**
         * Converts a float to it's string representation
         */
        static String ValueOf(float f);
        
        /**
         * Converts a double to it's string representation
         */
        static String ValueOf(double d);
        
        /**
         * Converts a raw pointer to it's string representation
         */
        static String ValueOf(void *ptr);
        
        /**
         * Converts a signed integer 8 bits to it's string representation
         */
        inline static String ValueOf(int8 i)
        {
            return (ValueOf(static_cast<int>(i)));
        }
        
        /**
         * Converts an unsigned integer 8 bits to it's string representation
         */
        inline static String ValueOf(uint8 i)
        {
            return (ValueOf(static_cast<int>(i)));
        }
        
        /**
         * Converts a signed integer 16 bits to it's string representation
         */
        inline static String ValueOf(int16 i)
        {
            return (ValueOf(static_cast<int>(i)));
        }
        
        /**
         * Converts an unsigned integer 16 bits to it's string representation
         */
        inline static String ValueOf(uint16 i)
        {
            return (ValueOf(static_cast<int>(i)));
        }
        
        /**
         * Converts a low-level C string to a high level string
         */
        inline static String ValueOf(const char *s)
        {
            return (String(s));
        }

        /**
         * Converts a bool to it's string representation
         */
        inline static String ValueOf(bool b)
        {
            return (b ? "TRUE" : "FALSE");
        }

        /**
         * Empty string constant
         */
        static const String Empty;
    };
}

DEFINE_DEFAULT(bpf::String, bpf::String());

#include "Framework/Stringifier.hpp"
