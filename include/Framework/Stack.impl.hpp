#ifndef STACK_IMPL_H_
# define STACK_IMPL_H_

namespace bpf
{
    template <typename T>
    inline Stack<T>::Stack(int maxsize)
        : Content(new T[maxsize])
        , MaxSize(maxsize)
        , CurSize(0)
        , CurPtr(MaxSize - 1)
    {
    }

    template <typename T>
    inline Stack<T>::~Stack()
    {
        delete[] Content;
    }

    template <typename T>
    void Stack<T>::Push(const T &element)
    {
        if (CurSize >= MaxSize)
            return;
        Content[CurPtr] = T(element);
        --CurPtr;
        ++CurSize;
    }

    template <typename T>
    const T &Stack<T>::Pop()
    {
        if (CurSize > 0)
        {
            CurSize--;
            CurPtr++;
        }
        return (Content[CurPtr]);
    }

    template <typename T>
    inline const T &Stack<T>::Get(const int id) const
    {
        return (Content[CurPtr + 1 + id]);
    }

    template <typename T>
    inline const T &Stack<T>::GetLast() const
    {
        return (Content[CurPtr + 1]);
    }

    template <typename T>
    inline int Stack<T>::Size() const
    {
        return (CurSize);
    }

    template <typename T>
    inline bool Stack<T>::IsEmpty() const
    {
        return (CurSize == 0);
    }
}

#endif /* !STACK_IMPL_H_ */
