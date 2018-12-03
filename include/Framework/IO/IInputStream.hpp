#pragma once
#include "Framework/Types.hpp"

namespace bpf
{
    class IInputStream
    {
    public:
        virtual ~IInputStream() {}
        virtual size Read(void *buf, size bufsize) = 0;
    };
}
