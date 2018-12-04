#pragma once
#include "Framework/Logging/ILogHandler.hpp"

namespace bpf
{
    class DefaultLogger final : public ILogHandler
    {
    public:
        void LogMessage(ELogLevel level, const String &category, const String &msg);
    };
}
