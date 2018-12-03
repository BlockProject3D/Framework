#ifndef HASH_H_
# define HASH_H_

namespace bpf
{
    /**
     * Hashes a given tyoe
     * @tparam T the type to hash
     */
    template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type * = nullptr>
    inline uint32 Hash(T t)
    {
        return (t.Hash());
    }

    /**
     * Hashes a given tyoe
     * @tparam T the type to hash
     */
    template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type * = nullptr>
    inline uint32 Hash(T t)
    {
        return ((uint32)t);
    }

    template <>
    inline uint32 Hash<const char *>(const char *str)
    {
        uint32 res = 5381;

        for (uint32 i = 0 ; str[i] ; ++i)
            res = ((res << 5) + res) + str[i];
        return (res);
    }
}

#endif /* HASH_H_ */
