#ifndef STACK_IMPL_H_
# define STACK_IMPL_H_

template <typename T>
inline FStack<T>::FStack(int maxsize)
    : Content(new T[maxsize]), MaxSize(maxsize),
    CurSize(0), CurPtr(MaxSize - 1)
{
}

template <typename T>
inline FStack<T>::~FStack()
{
    delete[] Content;
}

template <typename T>
void FStack<T>::Push(const T &element)
{
    if (CurSize >= MaxSize)
        return;
    Content[CurPtr] = T(element);
    --CurPtr;
    ++CurSize;
}

template <typename T>
const T &FStack<T>::Pop()
{
    if (CurSize > 0)
    {
        CurSize--;
        CurPtr++;
    }
    return (Content[CurPtr]);
}

template <typename T>
inline const T &FStack<T>::Get(const int id) const
{
    return (Content[CurPtr + 1 + id]);
}

template <typename T>
inline const T &FStack<T>::GetLast() const
{
    return (Content[CurPtr + 1]);
}

template <typename T>
inline int FStack<T>::Size() const
{
    return (CurSize);
}

template <typename T>
inline bool FStack<T>::IsEmpty() const
{
    return (CurSize == 0);
}

#endif /* !STACK_IMPL_H_ */
