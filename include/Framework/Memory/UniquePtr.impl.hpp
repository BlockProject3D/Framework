#pragma once

namespace bpf
{
    template <typename T>
    UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr<T> &&other)
    {
        if (RawPtr != Null)
            Memory::Delete(RawPtr);
        RawPtr = other.RawPtr;
        other.RawPtr = Null;
        return (*this);
    }
}
