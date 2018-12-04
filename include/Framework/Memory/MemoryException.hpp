#pragma once

namespace bpf
{
    class BPF_API MemoryException final : public Exception
    {
    public:
        inline MemoryException() {}
        inline const char *GetType() const
        {
            return ("MemoryException");
        }
    };
}
