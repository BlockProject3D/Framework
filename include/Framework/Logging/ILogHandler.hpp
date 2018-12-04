#pragma once
#include "Framework/Framework.hpp"
#include "Framework/Logging/ELogLevel.hpp"

namespace bpf
{
    class BPF_API ILogHandler
    {
    public:
        virtual ~ILogHandler() {}
        virtual void LogMessage(ELogLevel level, const String &category, const String &msg) = 0;
    };
}
