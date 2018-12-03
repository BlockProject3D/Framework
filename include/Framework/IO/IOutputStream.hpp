#pragma once
#include "Framework/Types.hpp"

namespace bpf
{
    class IOutputStream
    {
    public:
        virtual ~IOutputStream() {}
        virtual size Write(const void *buf, size bufsize) = 0;
    };
}
