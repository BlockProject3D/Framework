#include <iostream>
#include "framework/framework.h"

using namespace Framework;

void FException::Log(FLogger &) const
{
    std::cerr << "Low-level Exception thrown : " << GetType() << std::endl;
}
