#pragma once
#include <utility>

namespace bpf
{
    template <typename T, typename ...Args>
    inline BPF_API UniquePtr<T> MakeUnique(Args &&...args)
    {
        return (UniquePtr<T>(Memory::New<T>(std::forward<Args>(args)...)));
    }

    template <typename T, typename ...Args>
    inline BPF_API SharedPtr<T> MakeShared(Args &&...args)
    {
        return (SharedPtr<T>(Memory::New<T>(std::forward<Args>(args)...)));
    }
    
    template <typename T, typename ...Args>
    inline BPF_API T *MakeRaw(Args &&...args)
    {
        return (Memory::New<T>(std::forward<Args>(args)...));
    }
}
