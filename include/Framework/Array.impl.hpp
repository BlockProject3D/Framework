#ifndef ARRAY_IMPL_H_
# define ARRAY_IMPL_H_

template <typename T>
inline FArray<T>::FArray()
    : FixedSize(0), Array(Null)
{
}

template <typename T>
inline FArray<T>::FArray(FArray<T> &&arr)
    : FixedSize(arr.FixedSize), Array(arr.Array)
{
    arr.Array = Null;
    arr.FixedSize = 0;
}

template <typename T>
FArray<T>::FArray(const uint32 size)
{
    FixedSize = size;
    if (size == 0)
        Array = Null;
    Array = new T[FixedSize];
    for (uint32 i = 0 ; i < FixedSize ; ++i)
        Array[i] = DefaultOf<T>();
}

template <typename T>
FArray<T> &FArray<T>::operator=(FArray<T> &&arr)
{
    FixedSize = arr.FixedSize;
    Array = arr.Array;
    arr.Array = NULL;
    arr.FixedSize = 0;
    return (*this);
}

template <typename T>
inline FArray<T>::~FArray()
{
    delete[] Array;
}

template <typename T>
FString FArray<T>::ToString() const
{
    FString res = "[";

    for (uint32 i = 0 ; i < FixedSize ; ++i)
    {
        res += FString::ValueOf(Array[i]);
        if (i < FixedSize - 1)
            res += ", ";
    }
    res += "]";
    return (res);
}

template <typename T>
inline T &FArray<T>::operator[](const uint32 id) const
{
    if (id >= FixedSize)
        throw FIndexException((int)id);
    return (Array[id]);
}

template <typename T>
T &FArray<T>::operator[](const uint32 id)
{
    if (id >= FixedSize)
    {
        T *tmp = Array;
        Array = new T[id + 1];
        for (uint32 i = 0 ; i < FixedSize ; ++i)
            Array[i] = tmp[i];
        for (uint32 i = FixedSize ; i < id + 1 ; ++i)
            Array[i] = DefaultOf<T>();
        FixedSize = id + 1;
        delete[] tmp;
    }
    return (Array[id]);
}

template <typename T>
uint32 FArray<T>::Length() const
{
    return (FixedSize);
}

#endif /* !ARRAY_IMPL_H_ */
