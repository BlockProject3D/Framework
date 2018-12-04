#pragma once

namespace bpf
{
    template <typename T>
    WeakPtr<T>::~WeakPtr()
    {
        if (Count != Null)
            --*WCount;
        if (*WCount <= 0 && *Count <= 0)
        {
            Memory::Free(WCount);
            Memory::Free(Count);
        }
    }

    template <typename T>
    SharedPtr<T> WeakPtr<T>::Lock() noexcept
    {
        if (Count == Null || *Count <= 0)
            return (SharedPtr<T>());
        SharedPtr<T> res;
        res.WCount = WCount;
        res.Count = Count;
        res.RawPtr = RawPtr;
        ++*res.Count;
        return (res);
    }
}
