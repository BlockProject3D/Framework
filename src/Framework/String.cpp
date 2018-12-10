#include <locale>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "Framework/Framework.hpp"

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

String::String(const char c)
    : Data(Null), StrLen(1), UnicodeLen(1)
{
    Data = static_cast<char *>(Memory::Malloc(sizeof(char) * (StrLen + 1)));
    Data[0] = c;
    Data[1] = '\0';
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

fchar String::operator[](const int id) const
{
    if (id < 0 || id > Len())
        throw IndexException(id);
    return (String::UTF32(Data + CalcStartFromUnicode(id)));
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
    for (unsigned int i = 0; i < StrLen; i++)
    {
        if (other.Data[i] != Data[i])
            return (false);
    }
    return (true);
}

bool String::operator<(const String &other) const
{
    for (unsigned int i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] != other.Data[i])
            return (Data[i] < other.Data[i]);
    }
    return (false);
}

bool String::operator>(const String &other) const
{
    for (unsigned int i = 0 ; i < StrLen ; i++)
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

String String::operator+(const char other) const
{
  String str;

  MakeSized(str, StrLen + 1);
  CopyString(Data, str.Data, StrLen);
  str.Data[str.StrLen - 1] = other;
  str.UnicodeLen = UnicodeLen + 1;
  str.Data[str.StrLen] = '\0';
  return (str);
}

String &String::operator+=(const String &other)
{
    Data = static_cast<char *>(Memory::Realloc(Data, sizeof(char) * (StrLen + other.StrLen + 1)));
    CopyString(other.Data, Data + StrLen, other.StrLen);
    StrLen += other.StrLen;
    UnicodeLen += other.UnicodeLen;
    return (*this);
}

String &String::operator+=(const char other)
{
    Data = static_cast<char *>(Memory::Realloc(Data, sizeof(char) * (StrLen + 2)));
    Data[StrLen++] = other;
    Data[StrLen] = '\0';
    ++UnicodeLen;
    return (*this);
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

void String::Print()
{
    std::cout << Data;
}

String::~String()
{
    Memory::Free(Data);
}

void String::CopyString(const char *src, char *dest, const uint32 len) const
{
    size_t const *fsrc = reinterpret_cast<size_t const *>(src);
    size_t *fdest = reinterpret_cast<size_t *>(dest);
    uint32 flen = len / sizeof(size_t);

    for (uint32 i = 0; i < flen; ++i, ++fsrc, ++fdest)
        *fdest = *fsrc;
    dest = reinterpret_cast<char *>(fdest);
    src = reinterpret_cast<char const *>(fsrc);
    for (uint32 i = flen * sizeof(size_t); i < len; ++i, ++dest, ++src)
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

char *String::Duplicate() const
{
    char *res = new char[StrLen + 1];

    CopyString(Data, res, StrLen);
    res[StrLen] = '\0';
    return (res);
}

uint32 String::CalcStartFromUnicode(const uint32 start) const
{
    uint32 i = 0;

    for (uint32 j = 0 ; i < StrLen && j != start; ++i, ++j)
        i += CalcCharIncrement(Data[i]);
    return (i);
}

String String::Sub(const int begin, const int end) const
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

String String::Sub(const int begin) const
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

static bool my_strstr(const char *pathern, const char *str)
{
    while (*pathern && *str)
        if (*pathern++ != *str++)
            return (false);
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

void String::Explode(List<String> &l, const char c) const
{
    String cur;

    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] != c)
            cur += Data[i];
        else if (cur != String::Empty)
        {
            l.Add(cur);
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
        l.Add(cur);
}

void String::ExplodeIgnoreChar(List<String> &l, const char c, const char ignore) const
{
    String cur;
    bool ign = false;

    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (Data[i] == ignore)
            ign = !ign;
        if (Data[i] != c || ign)
            cur += Data[i];
        else if (cur != String::Empty)
        {
            l.Add(cur);
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
        l.Add(cur);
}

void String::Explode(List<String> &l, const String &str) const
{
    String cur;

    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (!my_strstr(str.Data, Data + i))
            cur += Data[i];
        else if (cur != String::Empty)
        {
            l.Add(cur);
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
        l.Add(cur);
}

void String::ExplodeIgnoreChar(List<String> &l, const String &str, const String &ignore) const
{
    String cur;
    bool ign = false;

    for (uint32 i = 0 ; i < StrLen ; i++)
    {
        if (my_strstr(ignore.Data, Data + i))
            ign = !ign;
        if (!my_strstr(str.Data, Data + i) || ign)
            cur += Data[i];
        else if (cur != String::Empty)
        {
            l.Add(cur);
            cur = String::Empty;
        }
    }
    if (cur != String::Empty)
        l.Add(cur);
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

int String::IndexOf(const String &str) const
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

int String::LastIndexOf(const String &str) const
{
    int i;
    int charid = UnicodeLen - 1;

    for (i = StrLen - 1 ; i >= 0 ; --i)
    {
        if (Data[i] == str.Data[0] && my_strstr(str.Data, Data + i))
            return (charid);
        if ((Data[i] & 0xC0) != 0x80)
            --charid;
    }
    return (-1);
}

int String::IndexOf(const char c) const
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

int String::LastIndexOf(const char c) const
{
    int i;
    int charid = UnicodeLen - 1;

    for (i = StrLen - 1 ; i >= 0 ; --i)
    {
        if (Data[i] == c)
            return (charid);
        if ((Data[i] & 0xC0) != 0x80)
            --charid;
    }
    return (-1);
}

int String::ToInt() const
{
    return (atoi(Data));
}

float String::ToFloat() const
{
    return ((float)atof(Data));
}

double String::ToDouble() const
{
    return (atof(Data));
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

/* BEGIN Eval expr */
namespace FEvalExpr
{
    enum EEvalExprOperator
    {
        EVAL_EXPR_ADD = 0,
        EVAL_EXPR_SUB,
        EVAL_EXPR_MUL,
        EVAL_EXPR_DIV,
        EVAL_EXPR_MOD,
        EVAL_EXPR_NONE,
    };

    static EEvalExprCode Operation(char const **expr, double &res, bool const parenthesis);

    static unsigned char IsOperator(char const expr)
    {
        return (expr == '+' || expr == '-' || expr == '*' || expr == '/');
    }

    static void DeleteSpace(char const **expr)
    {
        while (**expr && (**expr == ' ' || **expr == '\t'))
            ++(*expr);
    }

    static EEvalExprCode Calc(double const left, double const right, double &res, EEvalExprOperator const op)
    {
        EEvalExprCode status = EVAL_EXPR_SUCCESS;

        switch (op)
        {
        case EVAL_EXPR_ADD:
            res = left + right;
            break;
        case EVAL_EXPR_SUB:
            res = left - right;
            break;
        case EVAL_EXPR_MUL:
            res = left * right;
            break;
        case EVAL_EXPR_DIV:
            if (right)
                res = left / right;
            else
                status = EVAL_EXPR_DIVIDE_ZERO;
            break;
        case EVAL_EXPR_MOD:
            if (right != 0.0)
                res = ((uint64_t)left) % ((uint64_t)right);
            else
                status = EVAL_EXPR_MODULO_ZERO;
            break;
        default:
            status = EVAL_EXPR_SYNTHAX_INCORRECT;
            break;
        }
        return status;
    }

    static EEvalExprCode Number(char const **expr, double &num)
    {
        EEvalExprCode status = EVAL_EXPR_SUCCESS;

        DeleteSpace(expr);
        if (**expr)
        {
            num = ::strtod(*expr, (char **)expr);
            if (**expr == '(')
            {
                if (num == 0.0)
                {
                    ++(*expr);
                    status = Operation(expr, num, true);
                }
            }
            else if (!(!**expr || IsOperator(**expr) || **expr == ' '
                || **expr == '\t' || **expr == ')'))
                status = EVAL_EXPR_INVALID_NUMBER;
        }
        return status;
    }

    static EEvalExprCode Product(char const **expr, double &res)
    {
        double leftOperand = 0;
        double rightOperand = 0;
        EEvalExprOperator op = EVAL_EXPR_MUL;
        EEvalExprCode status = EVAL_EXPR_SUCCESS;

        if ((status = Number(expr, leftOperand)) == EVAL_EXPR_SUCCESS)
        {
            DeleteSpace(expr);
            switch (**expr)
            {
            case '*':
            case '/':
                if (**expr == '/')
                    op = EVAL_EXPR_DIV;
            case '%':
                if (**expr == '%')
                    op = EVAL_EXPR_MOD;
                ++(*expr);
                DeleteSpace(expr);
                status = (IsOperator(**expr)) ? EVAL_EXPR_SYNTHAX_INCORRECT : Product(expr, rightOperand);
                break;
            case '(':
                status = EVAL_EXPR_SYNTHAX_INCORRECT;
                break;
            case '+':
            case '-':
            case ')':
            case '\0':
                rightOperand = 1.0;
                break;
            default:
                status = EVAL_EXPR_SYNTHAX_INCORRECT;
                break;
            }
            if (status == EVAL_EXPR_SUCCESS)
            {
                if (rightOperand != 1.0 || op == EVAL_EXPR_MOD)
                    status = Calc(leftOperand, rightOperand, res, op);
                else
                    res = leftOperand;
            }
        }
        return status;
    }

    static EEvalExprCode Sum(char const **expr, double &res)
    {
        double leftOperand = 0;
        double rightOperand = 0;
        EEvalExprOperator op = EVAL_EXPR_ADD;
        EEvalExprCode status = EVAL_EXPR_SUCCESS;

        if ((status = Product(expr, leftOperand)) == EVAL_EXPR_SUCCESS)
        {
            switch (**expr)
            {
            case '-':
                op = EVAL_EXPR_SUB;
            case '+':
                ++(*expr);
                DeleteSpace(expr);
                status = (IsOperator(**expr)) ? EVAL_EXPR_SYNTHAX_INCORRECT : Sum(expr, rightOperand);
                if (status == EVAL_EXPR_SUCCESS)
                    status = Calc(leftOperand, rightOperand, res, op);
                break;
            default:
                res = leftOperand;
                break;
            }
        }
        return status;
    }

    static EEvalExprCode Operation(char const **expr, double &res, bool const parenthesis)
    {
        double operand = 0;
        EEvalExprCode status = EVAL_EXPR_SUCCESS;

        DeleteSpace(expr);
        switch (**expr)
        {
        case '\0':
            break;
        default:
            if (((status = Sum(expr, res)) == EVAL_EXPR_SUCCESS) && **expr == '(')
            {
                ++(*expr);
                status = Operation(expr, operand, true);
            }
            break;
        }
        if (parenthesis)
        {
            if (**expr == ')')
                ++(*expr);
            else
                status = EVAL_EXPR_PARENTHESIS;
        }
        res += operand;
        return status;
    }
}

EEvalExprCode String::Evaluate(char const *expr, double &res)
{
    res = 0;
    return FEvalExpr::Operation(&expr, res, false);
}
/* END Eval expr */

String String::ValueOf(EEvalExprCode cde)
{
    String res;

    switch (cde)
    {
    case EVAL_EXPR_DIVIDE_ZERO:
        res = "Division by zero";
        break;
    case EVAL_EXPR_INVALID_NUMBER:
        res = "Number expected";
        break;
    case EVAL_EXPR_MODULO_ZERO:
        res = "Modulo by zero";
        break;
    case EVAL_EXPR_SYNTHAX_INCORRECT:
        res = "Syntax error";
        break;
    case EVAL_EXPR_PARENTHESIS:
        res = "Missing parenthesis";
        break;
    default:
        res = "NULL";
        break;
    }
    return (res);
}

String String::ValueOf(int i)
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

    strs << std::hex << ptr;
    return (strs.str().c_str());
}
