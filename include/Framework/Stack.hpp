#ifndef STACK_H_
# define STACK_H_

namespace bpf
{
    template <typename T>
    class BP_TPL_API Stack
    {
    private:
        T *Content;
        int MaxSize;
        int CurSize;
        int CurPtr;
        
    public:
        Stack(int maxsize);
        ~Stack();
        void Push(const T &element);
        const T &Pop();
        const T &Get(const int id) const;
        const T &GetLast() const;
        bool IsEmpty() const;
        int Size() const;

        inline typename Array<T>::Iterator Begin() const
        {
            return (typename Array<T>::Iterator(Content, MaxSize, 0));
        }

        inline typename Array<T>::Iterator End() const
        {
            return (typename Array<T>::Iterator(Content, MaxSize, MaxSize - 1));
        }
    };
};

#endif /* !STACK_H_ */
