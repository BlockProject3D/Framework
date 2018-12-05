#pragma once
#include "Framework/Framework.hpp"
#include "Framework/Logging/ILogHandler.hpp"
#include "Framework/Logging/ELogLevel.hpp"

namespace bpf
{
    class BPF_API Logger
    {
    private:
        List<UniquePtr<ILogHandler>> _handlers;
        String _name;

        String Format(const String &format)
        {
            return (format);
        }

        template <typename ...Args>
        String Format(const String &format, Args &&...args)
        {
            for (size_t i = 0; i < format.size(); ++i)
            {
                if (format[i] == '@' && ((i + 1 == format.Size()) || (i + 1 != format.Size() && format[i + 1] != '@')))
                    return (res + String::ValueOf(std::forward<Args>(arg))
                        + Format(format.Sub(i + 1, format.Size() - i), std::forward<Args>(args)...));
                else
                    res += format[i];
            }
            return (res);
        }
    public:
        inline Logger(const String &name)
            : _name(name)
        {
        }

        inline void AddHandler(UniquePtr<ILogHandler> &&ptr)
        {
            _handlers.Add(std::move(ptr));
        }

        template <typename ...Args>
        inline void Info(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::INFO, _name, Format(format, std::forward<Args>(args)...));
        }

        template <typename ...Args>
        inline void Debug(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::DEBUG, _name, Format(format, std::forward<Args>(args)...));
        }
        
        template <typename ...Args>
        inline void Warning(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::WARNING, _name, Format(format, std::forward<Args>(args)...));
        }

        template <typename ...Args>
        inline void Error(const String &format, Args &&...args)
        {
            //_out.LogMessage(ELogLevel::ERROR, _name, Format(format, std::forward<Args>(args)...));
        }
    };
}
