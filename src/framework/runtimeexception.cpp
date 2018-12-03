#include "Framework/Framework.hpp"
#include "Framework/RuntimeException.hpp"

using namespace Framework;

FRuntimeException::FRuntimeException(const FString &type, const FString &message) noexcept
    : Type(type + "Exception"), Message(message)
{
}

void FRuntimeException::Log(FLogger &logger) const
{
    logger.Log(LOG_FATAL, "Exception thrown (%s) : %s", *Type, *Message);
}

void FIndexException::Log(FLogger &logger) const
{
    FString msg = FString::Format("Invalid index ([])", ID);
    logger.Log(LOG_FATAL, "Exception thrown (%s) : %s", *GetType(), *msg);
}
