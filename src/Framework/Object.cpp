#include "Framework/Framework.hpp"
#include "Framework/Object.hpp"

using namespace Framework;

FObject::~FObject()
{
    for (auto it = Refs.Begin() ; it ; ++it)
    {
        void **ref = *it;
        *ref = NULL;
    }
}
