#ifndef LOGGER_H_
# define LOGGER_H_

# include <stdarg.h>

namespace bpf
{
    enum class ELogLevel
    {
        INFO,
        WARNING,
        ERROR,
        DEBUG
    };

    class BPF_API ILogHandler
    {
    public:
        virtual ~ILogHandler() {}
        virtual void LogMessage(ELogLevel level, const String &category, const String &msg) = 0;
    };

    class BPF_API Logger
    {
    private:
        ILogHandler &_out;
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
                if (format[i] == '@' && ((i + 1 == format.size()) || (i + 1 != format.size() && format[i + 1] != '@')))
                    return (res + String::ValueOf(std::forward<Arg>(arg))
                        + Format(format.substr(i + 1, format.size() - i), std::forward<Args>(args)...));
                else
                    res += format[i];
            }
            return (res);
        }
    public:
        inline Logger(ILogHandler &hdl, const String &name)
            : _out(hdl)
            , _name(name)
        {
        }

        template <typename ...Args>
        inline void Info(const String &format, Args &&...args)
        {
            _out.LogMessage(ELogLevel::INFO, _name, Format(format, std::forward<Args>(args)...));
        }

        template <typename ...Args>
        inline void Debug(const String &format, Args &&...args)
        {
            _out.LogMessage(ELogLevel::DEBUG, _name, Format(format, std::forward<Args>(args)...));
        }
        
        template <typename ...Args>
        inline void Warning(const String &format, Args &&...args)
        {
            _out.LogMessage(ELogLevel::WARNING, _name, Format(format, std::forward<Args>(args)...));
        }

        template <typename ...Args>
        inline void Error(const String &format, Args &&...args)
        {
            _out.LogMessage(ELogLevel::ERROR, _name, Format(format, std::forward<Args>(args)...));
        }
    };
}

namespace Framework
{
    enum ELogLevel
    {
        LOG_INFO,
        LOG_ERR,
        LOG_WARN,
        LOG_FATAL
    };

    class ENGINE_API ILogBase
    {
    public:
        virtual ~ILogBase() {}
        virtual bpf::String HandleFormating(const char *format, va_list *lst) = 0;
        virtual bpf::String FormatLevel(const ELogLevel level) = 0;
        virtual void LogMessage(const char *name, const bpf::String &level, const bpf::String &msg) = 0;
    };

    class ENGINE_API FLogHandler : public ILogBase
    {
    public:
        virtual bpf::String HandleFormating(const char *format, va_list *lst);
        virtual bpf::String FormatLevel(const ELogLevel level);
        virtual void LogMessage(const char *name, const bpf::String &level, const bpf::String &msg) = 0;
    };

    class ENGINE_API FDefaultLogger : public FLogHandler
    {
    public:
        virtual void LogMessage(const char *name, const bpf::String &level, const bpf::String &msg);
    };

    class ENGINE_API FLogger
    {
    public:
        FLogger(const char *logname);
        ~FLogger();
        void Log(const ELogLevel level, const char *format, ...);
        void AddLogHandler(ILogBase *hdl);
    private:
        bpf::List<ILogBase *> *Loggers;
        const char *Name;
    };
};

#endif /* !LOGGER_H_ */
