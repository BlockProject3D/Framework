#include <iostream>
#include "framework/framework.h"

using namespace Framework;

FString FLogHandler::FormatLevel(const ELogLevel level)
{
    switch (level)
    {
    case LOG_INFO:
        return ("INFO");
    case LOG_WARN:
        return ("WARNING");
    case LOG_ERR:
        return ("ERROR");
    case LOG_FATAL:
        return ("FATAL");
    }
    return ("NULL");
}

FString FLogHandler::HandleFormating(const char *format, va_list *lst)
{
    FString res;

    int i = 0;
    while (format[i]) {
        if (format[i] == '%')
        {
            switch (format[++i])
            {
	    case 'i':
	        res += FString::ValueOf(va_arg(*lst, int));
	        break;
            case 'c':
	        res += (char) va_arg(*lst, int);
	        break;
	    case 's':
	        res += FString(va_arg(*lst, char *));
	        break;
	    case 'f':
	        res += FString::ValueOf(va_arg(*lst, double));
	        break;
	    case 'd':
	        res += FString::ValueOf(va_arg(*lst, double));
	        break;
            case 'b':
                {
                    bool b = (bool)va_arg(*lst, int);
                    if (b)
                        res += "true";
                    else
                        res += "false";
                    break;
                }
	    }
        }
        else
            res += format[i];
        i++;
    }
    return (res);
}

void FDefaultLogger::LogMessage(const char *name, const FString &level, const FString &msg)
{
    FString tim;
    FTimeDate date;

    if (FPlatform::GetBaseSystem() != NULL)
        date = FPlatform::GetBaseSystem()->GetSystemTime();
    if (FPlatform::GetBaseSystem() == NULL)
        tim = "NULL";
    else
        tim = date.Format("%d-%m-%y %h:%n:%s");
    const char *a = msg.GetData();
    const char *b = level.GetData();
    std::cout << '[' << tim.GetData() << ']';
    std::cout << '[' << name << ']';
    std::cout << '[' << b << "] ";
    std::cout << a << '\n';
}

FLogger::FLogger(const char *logname)
{
    Name = logname;
    Loggers = new FList<ILogBase*>();
}

FLogger::~FLogger()
{
    for (uint32 i = 0; i < Loggers->Size(); i++)
        delete *(Loggers->Get(i));
    delete Loggers;
}

void FLogger::Log(const ELogLevel level, const char *format, ...)
{
    va_list	lst;

    va_start(lst, format);
    for (uint32 i = 0; i < Loggers->Size(); i++)
    {
        ILogBase *logger = *Loggers->Get(i);
        FString msg = logger->HandleFormating(format, &lst);
        FString lvl = logger->FormatLevel(level);
        logger->LogMessage(Name, lvl, msg);
    }
    va_end(lst);
}

void FLogger::AddLogHandler(ILogBase *hdl)
{
    Loggers->Add(hdl);
}
