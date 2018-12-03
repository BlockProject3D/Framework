#include "framework/framework.h"
#include "framework/object.h"

using namespace Framework;

FObject::~FObject()
{
    for (auto it = Refs.Begin() ; it ; ++it)
    {
        void **ref = *it;
        *ref = NULL;
    }
}
