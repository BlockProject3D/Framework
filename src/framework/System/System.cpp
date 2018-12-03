#include "Framework/Framework.hpp"
#include "Framework/System/System.hpp"

using namespace Framework;

FString FTimeDate::Format(const FString &format)
{
    FString res;

    for (int i = 0 ; i < format.Size() ; i++)
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == 'h')
                res += FString::ValueOf(Hours);
            else if (format[i] == 'n')
                res += FString::ValueOf(Minutes);
            else if (format[i] == 's')
                res += FString::ValueOf(Seconds);
            else if (format[i] == 'm')
                res += FString::ValueOf(Month);
            else if (format[i] == 'd')
                res += FString::ValueOf(Day);
            else if (format[i] == 'y')
                res += FString::ValueOf(Year);
        }
        else
            res += format[i];
    }
    return (res);
}
