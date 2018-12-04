#include <iostream>
#include "Framework/Logging/DefaultLogger.hpp"

using namespace bpf;

void DefaultLogger::LogMessage(ELogLevel level, const String &category, const String &msg)
{
    String lvl = "";

    switch (level)
    {
    case ELogLevel::DEBUG:
        lvl = "DEBUG";
        break;
    case ELogLevel::INFO:
        lvl = "INFO";
        break;
    case ELogLevel::WARNING:
        lvl = "WARNING";
        break;
    case ELogLevel::ERROR:
        lvl = "ERROR";
        break;
    }
    String res = String('[') + category + "][" + lvl + "] " + msg;
    if (level == ELogLevel::ERROR)
        std::cerr << *res << std::endl;
    else
        std::cout << *res << std::endl;
}
