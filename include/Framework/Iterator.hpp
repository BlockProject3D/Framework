#ifndef ITERATOR_H_
# define ITERATOR_H_

namespace bpf
{
    /**
     * Abstract iterator
     * @tparam C the target container for this Iterator
     * @tparam T the target iterated object
     */
    template <typename C, typename T>
    class BP_TPL_API IIterator
    {
    public:
        virtual ~IIterator() {}
        virtual void operator++() = 0;
        virtual void operator--() = 0;
        virtual const T &operator*() const = 0;
        virtual const T &operator->() const = 0;
        virtual operator bool() const = 0;
        virtual bool operator==(const C &other) const = 0;
        virtual bool operator!=(const C &other) const = 0;
    };
};

#endif /* !ITERATOR_H_ */
