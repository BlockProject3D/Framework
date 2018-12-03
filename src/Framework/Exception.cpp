#include <iostream>
#include "Framework/Framework.hpp"

using namespace Framework;

void FException::Log(FLogger &) const
{
    std::cerr << "Low-level Exception thrown : " << GetType() << std::endl;
}
