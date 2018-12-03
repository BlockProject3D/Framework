#pragma once

namespace bpf
{
    template <typename T>
    class BPF_API ArrayList
    {
    private:
        uint32 _curid;
        Array<T> _arr;

    public:
        inline ArrayList()
            : _curid(0), _arr(16)
        {
        }

        inline void Add(const T &elem)
        {
            _arr[_curid++] = elem;
        }

        void RemoveAt(const uint32 id);
        
        void Remove(const T &elem);
        
        inline T *GetLast() const
        {
            if (_curid == 0)
                return (Null);
            return (&_arr[_curid]);
        }
        
        inline void RemoveLast()
        {
            Remove(_curid);
        }
        
        inline uint32 Size() const
        {
            return (_curid);
        }
        
        inline String ToString() const
        {
            return (_arr.ToString());
        }
        
        /**
         * Returns an iterator to the begining of the array
         */
        inline typename Array<T>::Iterator Begin() const
        {
            return (typename Array<T>::Iterator(*_arr, _curid, 0));
        }

        /**
         * Returns an iterator to the end of the array
         */
        inline typename Array<T>::Iterator End() const
        {
            return (typename Array<T>::Iterator(*_arr, _curid, _curid));
        }
    };
}
