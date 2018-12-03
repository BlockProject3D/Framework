#ifndef MAP_H_
# define MAP_H_

namespace bpf
{
    constexpr int MAP_INIT_BUF_SIZE = 2;
    constexpr float MAP_LIMIT_UNTIL_EXTEND = 0.5f;

    template<typename K, typename V>
    struct MapEntry
    {
        K Key;
        V Value;
    };

    template <typename K, typename V>
    class BPF_API Map
    {
    public:
        class BPF_API Iterator final : public IIterator<typename Map<K, V>::Iterator, MapEntry<K, V>>
        {
        private:
            V *Data;
            K *KeyData;
            bool *EmptyKeys;
            uint32 MaxSize;
            uint32 CurID;
            MapEntry<K, V> Entry;

        public:
            inline Iterator(V *data, K *keydata, bool *empty, uint32 start, uint32 size)
                : Data(data), KeyData(keydata), EmptyKeys(empty), MaxSize(size), CurID(start)
            {
                if (start == 0)
                    operator++();
                else
                    operator--();
            }
            void operator++();
            void operator--();
            inline const MapEntry<K, V> &operator*() const
            {
                return (Entry);
            }
            inline const MapEntry<K, V> &operator->() const
            {
                return (Entry);
            }
            inline operator bool() const
            {
                return (CurID != 0 && CurID != MaxSize);
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
        V *Data;
        K *KeyData;
        uint32 *HashTable;
        bool *EmptyKeys;
        uint32 CurSize;
        uint32 ElemCount;

        void TryExtend(); //Checks and extends the hash table by the multiplier
        int QuadraticSearch(uint32 hkey) const;
        int QuadraticInsert(uint32 hkey);

    public:
        Map();
        ~Map();
      
        /**
         * Adds a new element in this hash map, replaces if key already exists
         * @param key the key of the element
         * @param value the value to insert
         */
        void Add(const K &key, const V &value);
      
        /**
         * Removes an element from the hash table
         * @param key the key of the element to remove
         */
        void Remove(const K &key);
      
        /**
         * Returns the element at the specified key, if no key exists in this hash table, throws
         * @param key the key of the element to return
         */
        const V &operator[](const K &key) const;
        
        V &operator[](const K &key);
      
        /**
         * Returns true if the specified key exists, false otherwise
         * @param key the key to check
         */
        bool HasKey(const K &key) const;
      
        /**
         * Returns the size of this hash table, that means the element count
         */
        inline uint32 Size() const
        {
            return (ElemCount);
        }
        
        inline Iterator Begin() const
        {
            return (Iterator(Data, KeyData, EmptyKeys, 0, CurSize));
        }
        inline Iterator End() const
        {
            return (Iterator(Data, KeyData, EmptyKeys, CurSize - 1, CurSize));
        }
    };
};

#endif /* !MAP_H_ */
