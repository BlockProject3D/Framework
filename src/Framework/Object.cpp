#include "Framework/Framework.hpp"
#include "Framework/Object.hpp"

using namespace bpf;

Object::~Object()
{
    for (auto it = Refs.Begin() ; it ; ++it)
    {
        void **ref = *it;
        *ref = NULL;
    }
}
