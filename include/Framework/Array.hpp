#ifndef ARRAY_H_
# define ARRAY_H_

namespace bpf
{
    template <typename T>
    class BP_TPL_API Array
    {
    public:
        class BP_TPL_API Iterator final : public IIterator<typename Array<T>::Iterator, T>
        {
        private:
            int _curid;
            int _max;
            T *_arr;

        public:
            inline Iterator(T *lowlevel, const int size, const int start)
                : _curid(start), _max(size), _arr(lowlevel)
            {
            }
            inline void operator++()
            {
                if (_curid < _max)
                    _curid++;
            }
            inline void operator--()
            {
                if (_curid > -1)
                    _curid--;
            }
            inline const T &operator*() const
            {
                return (_arr[_curid]);
            }
            inline const T &operator->() const
            {
                return (_arr[_curid]);
            }
            inline operator bool() const
            {
                return (_curid != _max && _curid != -1);
            }
            inline bool operator==(const Iterator &other) const
            {
                return (_curid == other._curid);
            }
            inline bool operator!=(const Iterator &other) const
            {
                return (_curid != other._curid);
            }
        };

    private:
        uint32 _size;
        T *_arr;

    public:
        /**
         * Constructs an empty array
         */
        Array();

        /**
         * Constructs an array of given size
         * @param size the size of the new array
         */
        Array(const uint32 size);

        Array(Array<T> &&arr);
        ~Array();
        Array<T> &operator=(Array<T> &&arr);

        /**
         * Returns an element const mode
         * @param id the index of the element, in case of out of bounds, throws
         */
        T &operator[](const uint32 id) const;

        /**
         * Returns an element non-const mode
         * @param id the index of the element, in case of out of bounds, re-sizes the array
         */
        T &operator[](const uint32 id);

        /**
         * Returns the length of the array
         */
        uint32 Length() const;

        String ToString() const;

        inline T *operator*() const
        {
            return (_arr);
        }

        /**
         * Returns an iterator to the begining of the array
         */
        inline Iterator Begin() const
        {
            return (Iterator(_arr, _size, 0));
        }

        /**
         * Returns an iterator to the end of the array
         */
        inline Iterator End() const
        {
            return (Iterator(_arr, _size, _size - 1));
        }
    };
};

#endif /* !ARRAY_H_ */
