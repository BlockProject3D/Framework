#pragma once
#include "Framework/Framework.hpp"

namespace bpf
{
    class BPF_API ModuleException : public bpf::RuntimeException
    {
    public:
        inline ModuleException(const bpf::String &msg) noexcept
            : bpf::RuntimeException("Module", msg)
        {
        }
    };
}
