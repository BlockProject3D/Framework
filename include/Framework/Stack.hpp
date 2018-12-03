#ifndef STACK_H_
# define STACK_H_

namespace Framework
{
    template <typename T>
    class ENGINE_API FStack
    {
    private:
        T *Content;
        int MaxSize;
        int CurSize;
        int CurPtr;
    public:
        FStack(int maxsize);
        ~FStack();
        void Push(const T &element);
        const T &Pop();
        const T &Get(const int id) const;
        const T &GetLast() const;
        bool IsEmpty() const;
        int Size() const;

        inline typename FArray<T>::Iterator Begin() const
        {
            return (typename FArray<T>::Iterator(Content, MaxSize, 0));
        }

        inline typename FArray<T>::Iterator End() const
        {
            return (typename FArray<T>::Iterator(Content, MaxSize, MaxSize - 1));
        }
    };
};

#endif /* !STACK_H_ */
