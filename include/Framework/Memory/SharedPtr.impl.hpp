#pragma once

namespace bpf
{
    template <typename T>
    SharedPtr<T>::~SharedPtr()
    {
        if (Count == Null)
            return;
        --*Count;
        if (*Count <= 0)
        {
            Memory::Delete(RawPtr);
            if (*WCount <= 0)
            {
                Memory::Free(Count);
                Memory::Free(WCount);
            }
        }
    }

    template <typename T>
    SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&other)
    {
        if (Count != Null)
        {
            --*Count;
            if (*Count <= 0)
            {
                FMemory::Delete(RawPtr);
                if (*WCount <= 0)
                {
                    FMemory::Free(Count);
                    FMemory::Free(WCount);
                }
            }
        }
        Count = other.Count;
        WCount = other.WCount;
        RawPtr = other.RawPtr;
        other.Count = Null;
        other.WCount = Null;
        other.RawPtr = Null;
        return (*this);
    }

    template <typename T>
    SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &other)
    {
        if (Count != Null)
        {
            --*Count;
            if (*Count <= 0)
            {
                Memory::Delete(RawPtr);
                if (*WCount <= 0)
                {
                    Memory::Free(Count);
                    Memory::Free(WCount);
                }
            }
        }
        Count = other.Count;
        WCount = other.WCount;
        RawPtr = other.RawPtr;
        if (Count != Null)
            ++*Count;
        return (*this);
    }
}
