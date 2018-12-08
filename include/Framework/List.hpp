#ifndef LIST_H_
# define LIST_H_

#include <functional>
#include "Framework/Iterator.hpp"

namespace bpf
{
    template <typename T>
    class BPF_API ListNode
    {
    public:
        ListNode *Next;
        ListNode *Prev;
        T Data;

	ListNode(const T &data)
            : Next(NULL), Prev(NULL), Data(data)
        {
        }
        ListNode(T &&data)
            : Next(NULL), Prev(NULL), Data(std::move(data))
        {
        }
    };

    /**
     * A simple double chained linked list
     * @tparam T the type of element in that list
     */
    template <typename T>
    class BPF_API List
    {
    public:
        class BPF_API Iterator final : public IIterator<typename List<T>::Iterator, T>
        {
        private:
            ListNode<T> *Cur;

        public:
            inline Iterator(ListNode<T> *cur)
                : Cur(cur)
            {
            }
            inline void operator++()
            {
                if (Cur)
                    Cur = Cur->Next;
            }
            inline void operator--()
            {
                if (Cur)
                    Cur = Cur->Prev;
            }
            inline const T &operator->() const
            {
                return Cur->Data;
            }
            inline const T &operator*() const
            {
                return Cur->Data;
            }
            inline bool operator==(const Iterator &it) const
            {
                return (Cur == it.Cur);
            }
            inline bool operator!=(const Iterator &it) const
            {
                return (Cur != it.Cur);
            }
            inline operator bool() const
            {
                return (Cur != Null);
            }
        };
    private:
        ListNode<T> *First;
        ListNode<T> *Last;
        uint32 Count;

        ListNode<T> *Partition(ListNode<T> *start, ListNode<T> *end);
        void QuickSort(ListNode<T> *start, ListNode<T> *end);
        void RemoveNode(ListNode<T> *toRM);
    public:
        List<T>();
        ~List<T>();

        /**
         * Adds an element at the end of the list
         * @param elem element to add
         */
        void Add(const T &elem);

        /**
         * Inserts an element at a given position
         * @param elem element to insert
         * @param pos position
         */
        void Insert(const T &elem, uint32 pos);

        /**
         * Adds an element at the end of the list
         * @param elem element to add
         */
        void Add(T &&elem);

        /**
         * Inserts an element at a given position
         * @param elem element to insert
         * @param pos position
         */
        void Insert(T &&elem, uint32 pos);

        /**
         * Returns an element (safe), returns Null whenever element could not be found
         * @param id index of the element
         */
        ListNode<T> *GetNode(uint32 id) const;

        T *Get(const uint32 id) const;

        T operator[](const uint32 id) const;

        void RemoveAt(const uint32 id);

        void Remove(const T &elem, const bool all = true);

        void RemoveLast();

        void Sort();

        T *GetFirst() const;

        T *GetLast() const;

        uint32 Size() const;

        void Clear();

        void ForEach(const std::function<void(const T &)> &fnc) const;

        /**
         * Returns an iterator to the begining of the list
         */
        inline Iterator Begin() const
        {
            return (Iterator(First));
        }

        /**
         * Returns an iterator to the end of the list
         */
        inline Iterator End() const
        {
            return (Iterator(Last));
        }
    };
};

#endif /* !LIST_H_ */
