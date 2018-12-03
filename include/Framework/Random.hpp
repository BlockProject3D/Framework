#ifndef RANDOM_H_
# define RANDOM_H_

# include <limits.h>

namespace bpf
{
    class BPF_API Random
    {
    private:
        int IntBounds(const int min, const int max);

    public:
        Random();
        Random(const long seed);
        int NextInt(const int max = INT_MAX);
        int NextInt(const int min, const int max);
        uint8 NextByte(const uint8 max = 255);
        uint8 NextByte(const uint8 min, const uint8 max);
        uint16 NextShort(const uint16 max = 65535);
        uint16 NextShort(const uint16 min, const uint16 max);

        /**
         * Returns random float between 0 and 1
         */
        float NextFloat(const float min = 0);

        /**
         * Returns random double between 0 and 1
         */
        double NextDouble();
    };
};

#endif /* !RANDOM_H_ */
