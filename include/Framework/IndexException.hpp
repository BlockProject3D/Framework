#pragma once

namespace bpf
{
    class BPF_API IndexException : public RuntimeException
    {
    private:
        int ID;

    public:
        inline IndexException(const int id) noexcept
            : RuntimeException("Index", "")
            , ID(id)
        {
        }
        void Log(Framework::FLogger &logger) const;
    };
}
