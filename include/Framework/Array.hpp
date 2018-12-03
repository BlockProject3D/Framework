#ifndef ARRAY_H_
# define ARRAY_H_

namespace Framework
{
    template <typename T>
    class ENGINE_API FArray
    {
    public:
        class ENGINE_API Iterator final : public IIterator<typename FArray<T>::Iterator, T>
        {
        private:
            int CurID;
            int Max;
            T *Array;
        public:
            inline Iterator(T *lowlevel, const int size, const int start)
                : CurID(start), Max(size), Array(lowlevel)
            {
            }
            inline void operator++()
            {
                if (CurID < Max)
                    CurID++;
            }
            inline void operator--()
            {
                if (CurID > -1)
                    CurID--;
            }
            inline const T &operator*() const
            {
                return (Array[CurID]);
            }
            inline const T &operator->() const
            {
                return (Array[CurID]);
            }
            inline operator bool() const
            {
                return (CurID != Max && CurID != -1);
            }
            inline bool operator==(const Iterator &other) const
            {
                return (CurID == other.CurID);
            }
            inline bool operator!=(const Iterator &other) const
            {
                return (CurID != other.CurID);
            }
        };
    private:
        uint32 FixedSize;
        T *Array;
    public:
        /**
         * Constructs an empty array
         */
        FArray();

        /**
         * Constructs an array of given size
         * @param size the size of the new array
         */
        FArray(const uint32 size);

        FArray(FArray<T> &&arr);
        ~FArray();
        FArray<T> &operator=(FArray<T> &&arr);

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

        FString ToString() const;

        inline T *operator*() const
        {
            return (Array);
        }

        /**
         * Returns an iterator to the begining of the array
         */
        inline Iterator Begin() const
        {
            return (Iterator(Array, FixedSize, 0));
        }

        /**
         * Returns an iterator to the end of the array
         */
        inline Iterator End() const
        {
            return (Iterator(Array, FixedSize, FixedSize - 1));
        }
    };
    
    template <typename T>
    class ENGINE_API FArrayList
    {
    private:
        uint32 CurID;
        FArray<T> Array;
    public:
        inline FArrayList()
            : CurID(0), Array(16)
        {
        }

        inline void Add(const T &elem)
        {
            Array[CurID++] = elem;
        }

        inline void Remove(const uint32 id)
        {
            Array[id] = DefaultOf<T>();
            for (uint32 i = id ; i < CurID ; i += 2)
                Array[i] = Array[i + 1];
            --CurID;
        }
        
        inline void Remove(const T &elem)
        {
            for (uint32 i = CurID ; i > 0 ; --i)
            {
                if (Array[i] == elem)
                    Remove(i);
            }
            if (Array[0] == elem)
                Remove((uint32)0);
        }
        
        inline T *GetLast() const
        {
            if (CurID == 0)
                return (Null);
            return (&Array[CurID]);
        }
        
        inline void RemoveLast()
        {
            Remove(CurID);
        }
        
        inline uint32 Size() const
        {
            return (CurID);
        }
        
        inline FString ToString() const
        {
            return (Array.ToString());
        }
        
        /**
         * Returns an iterator to the begining of the array
         */
        inline typename FArray<T>::Iterator Begin() const
        {
            return (typename FArray<T>::Iterator(*Array, CurID, 0));
        }

        /**
         * Returns an iterator to the end of the array
         */
        inline typename FArray<T>::Iterator End() const
        {
            return (typename FArray<T>::Iterator(*Array, CurID, CurID));
        }
    };
};

#endif /* !ARRAY_H_ */
