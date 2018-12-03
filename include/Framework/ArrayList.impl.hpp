#pragma once

namespace bpf
{
    template <typename T>
    void ArrayList<T>::RemoveAt(const uint32 id)
    {
        _arr[id] = DefaultOf<T>();
        for (uint32 i = id ; i < _curid ; i += 2)
            _arr[i] = _arr[i + 1];
        --_curid;
    }
    
    template <typename T>
    void ArrayList<T>::Remove(const T &elem)
    {
        for (uint32 i = _curid ; i > 0 ; --i)
        {
            if (_arr[i] == elem)
                Remove(i);
        }
        if (_arr[0] == elem)
            Remove((uint32)0);
    }
}
