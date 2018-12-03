#ifndef STRING_H_
# define STRING_H_

# include <type_traits>

namespace Framework
{
    enum EEvalExprCode
    {
        EVAL_EXPR_PARENTHESIS = 0,
        EVAL_EXPR_DIVIDE_ZERO,
        EVAL_EXPR_MODULO_ZERO,
        EVAL_EXPR_SYNTHAX_INCORRECT,
        EVAL_EXPR_INVALID_NUMBER,
        EVAL_EXPR_SUCCESS,
    };

    class ENGINE_API FString
    {
    private:
        char *Data;
        uint32 StrLen;
        uint32 UnicodeLen;

        void CopyString(const char *src, char *dest, const uint32 len) const;
        uint32 CalcUnicodeLen(const char *str, const uint32 len) const;
        uint32 CalcStartFromUnicode(const uint32 start) const;
        static uint8 CalcCharIncrement(const char c);
        void MakeSized(FString &str, const uint32 len) const;
    public:
        /**
         * Constructs a new empty string
         */
        FString();

        /**
         * Constructs a new string from a string literal
         * @param str pointer to an array of bytes containing UTF-8 data
         */
        FString(const char *str);

        /**
         * Constructs a new string from a single character
         * @param c the character to copy in the string
         */
        FString(const char c);

        /**
         * Copy constructor
         */
        FString(const FString &s);

        /**
         * Move constructor
         */
        FString(FString &&s);

        ~FString();

        /**
         * Returns a UTF32 character from a null-terminated UTF8 byte sequence
         * @param utf8char the utf8 null-terminated byte sequence to convert to a single UTF32 character
         */
        static fchar UTF32(const char *utf8char);

        /**
         * Returns a byte at a given index
         * @param id the index of the byte to return
         */
        inline char ByteAt(const uint32 id) const
        {
            return ((char)((id >= StrLen) ? '\0' : Data[id]));
        }

        /**
         * Returns a new string from concatenation of this + other
         */
        FString operator+(const FString &other) const;

        /**
         * Returns a new string from concatenation of this + other
         */
        FString operator+(const char other) const;
        
        /**
         * Returns a new string from concatenation of this + other
         */
        inline FString operator+(const int other) const
        {
            return (operator+(ValueOf(other)));
        }

        /**
         * Returns a new string from concatenation of this + other
         */
        inline FString operator+(const float other) const
        {
            return (operator+(ValueOf(other)));
        }

        /**
         * Returns a new string from concatenation of this + other
         */
        inline FString operator+(const double other) const
        {
            return (operator+(ValueOf(other)));
        }

        /**
         * Returns a new string from concatenation of this + other
         */
        inline FString operator+(const uint32 other) const
        {
            return (operator+(ValueOf(other)));
        }

        /**
         * Returns a new string from concatenation of this + other
         */
        inline FString operator+(const uint64 other) const
        {
            return (operator+(ValueOf(other)));
        }
        
        /**
         * Appends a string at the end of this string
         */
        FString &operator+=(const FString &other);

        /**
         * Appends a single byte at the end of this string
         */
        FString &operator+=(const char other);

        /**
         * Appends an int at the end of this string
         */
        inline FString &operator+=(const int other)
        {
            return (operator+=(ValueOf(other)));
        }

        /**
         * Appends a float at the end of this string
         */
        inline FString &operator+=(const float other)
        {
            return (operator+=(ValueOf(other)));
        }

        /**
         * Appends a double at the end of this string
         */
        inline FString &operator+=(const double other)
        {
            return (operator+=(ValueOf(other)));
        }

        /**
         * Appends a uint32 number at the end of this string
         */
        inline FString &operator+=(const uint32 other)
        {
            return (operator+=(ValueOf(other)));
        }

        /**
         * Appends a uint64 number at the end of this string
         */
        inline FString &operator+=(const uint64 other)
        {
            return (operator+=(ValueOf(other)));
        }

        /**
         * Copy assignment operator
         */
        FString &operator=(const FString &other);

        /**
         * Move assignment operator
         */
        FString &operator=(FString &&other);

        /**
         * Returns true if this string is equal to other
         * @param other the other string to compare with
         */
        bool operator==(const FString &other) const;

        /**
         * Returns true if this string is not equal to other
         * @param other the other string to compare with
         */
        inline bool operator!=(const FString &other) const
        {
            return (!operator==(other));
        }

        /**
         * Compares this string with another : returns true if this is less than other
         * @param other the other string to compare with
         */
        bool operator<(const FString &other) const;

        /**
         * Compares this string with another : returns true if this is greater than other
         * @param other the other string to compare with
         */
        bool operator>(const FString &other) const;

        /**
         * Returns a new string with all occurenses of search replaced by repby
         * @param search the string to search for
         * @param repby the string to replace occurenses of search with
         */
        FString Replace(const FString &search, const FString &repby) const;

        /**
         * Returns a character of this string at offset id
         * @param id the offset of the character to get
         */
        fchar operator[](const int id) const;

        /**
         * Prints this string on standard output
         */
        void Print();

        /**
         * Returns the data of this string ready for printing
         */
        inline const char *operator*() const
        {
            return (Data);
        }

        /**
         * Same as operator*
         */
        inline const char *GetData() const
	{
	    return (Data);
	}

        /**
         * Duplicates this string and returns a low level c array
         */
        char *Duplicate() const;

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
        FString ToUpper() const;

        /**
         * Returns a copy of this string in all lower case
         * Not working with non ascii characters
         */
        FString ToLower() const;

        /**
         * Appends a string to this string
         * @param other the other string to copy characters from
         * @param n how many characters should be copied
         */
        FString &Append(const FString &other, const uint32 n);

        /**
         * Appends a string to this string
         * @param data an arry of characetrs to copy from
         * @param n how many characters should be copied
         */
        FString &Append(const char *data, const uint32 n);

        /**
         * Returns the index of the first occurence of str in this
         */
        int IndexOf(const FString &str) const;

        /**
         * Returns the index of the last occurence of str in this
         */
        int LastIndexOf(const FString &str) const;

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
         * @param l the list to load of tokens
         * @param c the sperator char
         */
        void Explode(FList<FString> &l, const char c) const;

        /**
         * Splits this string using a delimiter
         * @param l the list to load of tokens
         * @param c the seperator char
         * @param ignore character to identify escape sequences (all characters between two of this ignore char will be interpreted as a single token)
         */
        void ExplodeIgnoreChar(FList<FString> &l, const char c, const char ignore) const;

        /**
         * Splits this string using a delimiter
         * @param l the list to load of tokens
         * @param str the separator string
         */
        void Explode(FList<FString> &l, const FString &str) const;

        /**
         * Splits this string using a delimiter
         * @param l the list to load of tokens
         * @param str the separator string
         * @param ignore string to identify escape sequences (all characters between two of this ignore string will be interpreted as a single token)
         */
        void ExplodeIgnoreChar(FList<FString> &l, const FString &str, const FString &ignore) const;

        /**
         * Returns true if this string starts with other
         */
        bool StartsWith(const FString &other) const;

        /**
         * Returns true if this string ends with other
         */
        bool EndsWith(const FString &other) const;

        /**
         * Returns true if this string contains other
         */
        bool Contains(const FString &other) const;

        /**
         * Returns a substring
         * @param begin the begin index in characters included
         * @param end the end index in characters included
         */
        FString Sub(const int begin, const int end) const;

        /**
         * Returns a substring where the end is the end of this string
         * @param begin the begin index in characters included
         */
        FString Sub(const int begin) const;

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
         */
        static EEvalExprCode Evaluate(char const *expr, double &res);

        /**
         * Evaluate a mathematical expression from the contents of the FString
         */
        inline EEvalExprCode Evaluate(double &res)
        {
            return (FString::Evaluate(Data, res));
        }

        /**
         * Builds a string using the following formating syntax : '[]' for no format and [<num chars padding>,<allignment (left / right)>,<characters to serve as padding>]
         * @tparam Args objects / scalar types to format
         * @param format the given format
         */
        template <typename ...Args>
        inline static FString Format(const FString &format, Args&&...)
      	{
            return (format);
      	}

        /**
         * Builds a string using the following formating syntax : '[]' for no format and [<num chars padding>,<allignment (left / right)>,<characters to serve as padding>]
         * @tparam Args objects / scalar types to format
         * @param format the given format
         */
        template <typename T, typename ...Args>
        static FString Format(const FString &format, T &&t, Args&&... args)
        {
            FString res;

            int i = format.IndexOf('[');
            int j = format.IndexOf(']');
            if (i > -1 && j > -1 && format.Sub(i - 1, i) != "\\")
            {
                res += format.Sub(0, i);
                FString pattern = format.Sub(i + 1, j);
                FList<FString> tokens;
                pattern.Explode(tokens, ',');
                if (tokens.Size() < 3)
                    return (res + ValueOf(t) + Format(format.Sub(j + 1), args...));
                int maxn = tokens[0].ToInt();
                bool left = tokens[1] == "left";
                FString leading = tokens[2];
                FString data = ValueOf(t);
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

        inline FString ToString() const
        {
            return (*this);
        }

        inline uint32 Hash() const
        {
            return (Framework::Hash<const char *>(Data));
        }

        /**
         * Converts any object to it's string representation by calling ToString on it
         */
        template <typename T, typename std::enable_if<std::is_class<T>::value>::type * = nullptr>
        inline static FString ValueOf(const T &val)
        {
            return (val.ToString());
        }

        /**
         * Converts any pointer type to it's string representation
         */
        template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type * = nullptr>
        inline static FString ValueOf(T val)
        {
            return (ValueOf((void *)val));
        }

        /**
         * Converts an EvalExpr result code in string
         */
        static FString ValueOf(EEvalExprCode cde);
        
        /**
         * Converts a signed integer 32 bits to it's string representation
         */
        static FString ValueOf(int i);
        
        /**
         * Converts an unsigned integer 32 bits to it's string representation
         */
        static FString ValueOf(uint32 i);
        
        /**
         * Converts an unsigned integer 64 bits to it's string representation
         */
        static FString ValueOf(uint64 i);
        
        /**
         * Converts a signed integer 64 bits to it's string representation
         */
        static FString ValueOf(int64 i);
        
        /**
         * Converts a float to it's string representation
         */
        static FString ValueOf(float f);
        
        /**
         * Converts a double to it's string representation
         */
        static FString ValueOf(double d);
        
        /**
         * Converts a raw pointer to it's string representation
         */
        static FString ValueOf(void *ptr);
        
        /**
         * Converts a signed integer 8 bits to it's string representation
         */
        inline static FString ValueOf(int8 i)
        {
            return (ValueOf((int)i));
        }
        
        /**
         * Converts an unsigned integer 8 bits to it's string representation
         */
        inline static FString ValueOf(uint8 i)
        {
            return (ValueOf((int)i));
        }
        
        /**
         * Converts a signed integer 16 bits to it's string representation
         */
        inline static FString ValueOf(int16 i)
        {
            return (ValueOf((int)i));
        }
        
        /**
         * Converts an unsigned integer 16 bits to it's string representation
         */
        inline static FString ValueOf(uint16 i)
        {
            return (ValueOf((int)i));
        }
        
        /**
         * Converts a low-level C string to a high level string
         */
        inline static FString ValueOf(const char *s)
        {
            return (FString(s));
        }

        /**
         * Converts a bool to it's string representation
         */
        inline static FString ValueOf(bool b)
        {
            return (b ? "TRUE" : "FALSE");
        }

        /**
         * Empty string constant
         */
        static const FString Empty;
    };
};

DEFINE_DEFAULT(Framework::FString, Framework::FString());

#endif /* !STRING_H_ */
