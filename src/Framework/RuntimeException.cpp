#include "Framework/Framework.hpp"
#include "Framework/RuntimeException.hpp"

using namespace bpf;

RuntimeException::RuntimeException(const String &type, const String &message) noexcept
    : Type(type + "Exception"), Message(message)
{
}

void RuntimeException::Log(Framework::FLogger &logger) const
{
    logger.Log(Framework::LOG_FATAL, "Exception thrown (%s) : %s", *Type, *Message);
}
