#ifndef ARRAY_IMPL_H_
# define ARRAY_IMPL_H_

namespace bpf
{
    template <typename T>
    inline Array<T>::Array()
        : _size(0)
        , _arr(Null)
    {
    }

    template <typename T>
    inline Array<T>::Array(Array<T> &&arr)
        : _size(arr._size)
        , _arr(arr._arr)
    {
        arr._arr = Null;
        arr._size = 0;
    }

    template <typename T>
    Array<T>::Array(const uint32 size)
        : _size(size)
        , _arr(Null)
    {
        if (size > 0)
        {
            _arr = new T[_size];
            for (uint32 i = 0 ; i < _size ; ++i)
                _arr[i] = DefaultOf<T>();
        }
    }

    template <typename T>
    Array<T> &Array<T>::operator=(Array<T> &&arr)
    {
        _size = arr._size;
        _arr = arr._arr;
        arr._arr = Null;
        arr._size = 0;
        return (*this);
    }

    template <typename T>
    inline Array<T>::~Array()
    {
        delete[] _arr;
    }

    template <typename T>
    String Array<T>::ToString() const
    {
        String res = "[";

        for (uint32 i = 0 ; i < _size ; ++i)
        {
            res += String::ValueOf(_arr[i]);
            if (i < _size - 1)
                res += ", ";
        }
        res += "]";
        return (res);
    }

    template <typename T>
    inline T &Array<T>::operator[](const uint32 id) const
    {
        if (id >= _size)
            throw IndexException(static_cast<int>(id));
        return (_arr[id]);
    }

    template <typename T>
    T &Array<T>::operator[](const uint32 id)
    {
        if (id >= _size)
        {
            T *tmp = _arr;
            _arr = new T[id + 1];
            for (uint32 i = 0 ; i < _size ; ++i)
                _arr[i] = tmp[i];
            for (uint32 i = _size ; i < id + 1 ; ++i)
                _arr[i] = DefaultOf<T>();
            _size = id + 1;
            delete[] tmp;
        }
        return (_arr[id]);
    }

    template <typename T>
    uint32 Array<T>::Length() const
    {
        return (_size);
    }
}

#endif /* !ARRAY_IMPL_H_ */
