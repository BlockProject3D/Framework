#ifndef LOGGER_H_
# define LOGGER_H_

# include <stdarg.h>

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
        virtual FString HandleFormating(const char *format, va_list *lst) = 0;
        virtual FString FormatLevel(const ELogLevel level) = 0;
        virtual void LogMessage(const char *name, const FString &level, const FString &msg) = 0;
    };

    class ENGINE_API FLogHandler : public ILogBase
    {
    public:
        virtual FString HandleFormating(const char *format, va_list *lst);
        virtual FString FormatLevel(const ELogLevel level);
        virtual void LogMessage(const char *name, const FString &level, const FString &msg) = 0;
    };

    class ENGINE_API FDefaultLogger : public FLogHandler
    {
    public:
        virtual void LogMessage(const char *name, const FString &level, const FString &msg);
    };

    class ENGINE_API FLogger
    {
    public:
        FLogger(const char *logname);
        ~FLogger();
        void Log(const ELogLevel level, const char *format, ...);
        void AddLogHandler(ILogBase *hdl);
    private:
        FList<ILogBase *> *Loggers;
        const char *Name;
    };
};

#endif /* !LOGGER_H_ */
