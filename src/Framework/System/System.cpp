#include "Framework/Framework.hpp"
#include "Framework/System/System.hpp"

using namespace Framework;

bpf::String FTimeDate::Format(const bpf::String &format)
{
    bpf::String res;

    for (int i = 0 ; i < format.Size() ; i++)
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == 'h')
                res += bpf::String::ValueOf(Hours);
            else if (format[i] == 'n')
                res += bpf::String::ValueOf(Minutes);
            else if (format[i] == 's')
                res += bpf::String::ValueOf(Seconds);
            else if (format[i] == 'm')
                res += bpf::String::ValueOf(Month);
            else if (format[i] == 'd')
                res += bpf::String::ValueOf(Day);
            else if (format[i] == 'y')
                res += bpf::String::ValueOf(Year);
        }
        else
            res += format[i];
    }
    return (res);
}
