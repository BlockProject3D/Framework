#ifndef MAP_IMPL_H_
# define MAP_IMPL_H_

template <typename K, typename V>
FMap<K, V>::FMap()
    : Data(new V[MAP_INIT_BUF_SIZE]),
    KeyData(new K[MAP_INIT_BUF_SIZE]),
    HashTable(new uint32[MAP_INIT_BUF_SIZE]),
    EmptyKeys(new bool[MAP_INIT_BUF_SIZE]),
    CurSize(MAP_INIT_BUF_SIZE), ElemCount(0)
{
    EmptyKeys[0] = true;
    EmptyKeys[1] = true;
}

template <typename K, typename V>
FMap<K, V>::~FMap()
{
    delete[] Data;
    delete[] KeyData;
    delete[] HashTable;
    delete[] EmptyKeys;
}

template <typename K, typename V>
void FMap<K, V>::TryExtend()
{
    if ((float)ElemCount / (float)CurSize >= MAP_LIMIT_UNTIL_EXTEND)
    {
        V *olddata = Data;
        K *oldkeydata = KeyData;
        uint32 *oldhash = HashTable;
        bool *oldempty = EmptyKeys;
        CurSize <<= 1;
        EmptyKeys = new bool[CurSize];
        for (uint32 i = 0 ; i < CurSize; ++i)
            EmptyKeys[i] = true;
        Data = new V[CurSize];
        KeyData = new K[CurSize];
        HashTable = new uint32[CurSize];
        for (uint32 i = 0 ; i < CurSize >> 1 ; ++i)
        {
            if (!oldempty[i])
            {
                int id = QuadraticInsert(oldhash[i]);
                if (id != -1)
                {
		    Data[id] = std::move(olddata[i]);
		    KeyData[id] = std::move(oldkeydata[i]);
                }
            }
        }
        delete[] olddata;
        delete[] oldkeydata;
        delete[] oldhash;
        delete[] oldempty;
    }
}

template <typename K, typename V>
int FMap<K, V>::QuadraticSearch(uint32 hkey) const
{
    for (uint32 i = 0 ; i < CurSize ; ++i)
    {
        uint32 index = (hkey + ((i * i + i) / 2)) % CurSize;
        if (!EmptyKeys[index] && HashTable[index] == hkey)
            return ((int)index);
    }
    return (-1);
}

template <typename K, typename V>
int FMap<K, V>::QuadraticInsert(uint32 hkey)
{
    for (uint32 i = 0 ; i < CurSize ; ++i)
    {
        uint32 index = (hkey + ((i * i + i) / 2)) % CurSize;
        if (EmptyKeys[index])
        {
            HashTable[index] = hkey;
            EmptyKeys[index] = false;
            ++ElemCount;
            return ((int)index);
        }
    }
    return (-1);
}

template <typename K, typename V>
void FMap<K, V>::Add(const K &key, const V &value)
{
    uint32 hkey = Hash(key);

    TryExtend();
    if (QuadraticSearch(hkey) != -1)
        Remove(key);
    int idx = QuadraticInsert(hkey);
    if (idx != -1)
    {
        Data[idx] = value;
        KeyData[idx] = key;
    }
}

template <typename K, typename V>
void FMap<K, V>::Remove(const K &key)
{
    int idx = QuadraticSearch(Hash(key));

    if (idx != -1)
    {
        --ElemCount;
        EmptyKeys[idx] = true;
    }
}

template <typename K, typename V>
const V &FMap<K, V>::operator[](const K &key) const
{
    int idx = QuadraticSearch(Hash(key));

    if (idx == -1)
        throw FIndexException(idx);
    return (Data[idx]);
}

template <typename K, typename V>
V &FMap<K, V>::operator[](const K &key)
{
    int idx = QuadraticSearch(Hash(key));

    if (idx == -1) {
        TryExtend();
        idx = QuadraticInsert(Hash(key));
        if (idx == -1)
            throw FIndexException(idx);
        KeyData[idx] = key;
    }
    return (Data[idx]);
}

template <typename K, typename V>
inline bool FMap<K, V>::HasKey(const K &key) const
{
    return (QuadraticSearch(Hash(key)) != -1);
}

#endif /* !MAP_IMPL_H_ */
