#include <time.h>
#include "Framework/Framework.hpp"
#include "Framework/Random.hpp"
#include "Framework/Math/BMath.hpp"

using namespace bpf;

int Random::IntBounds(const int min, const int max)
{
    int rd = rand() % max;

    while (rd < min || rd > max)
        rd = rand() % max;
    return (rd);
}

Random::Random(const long seed)
{
    srand((uint32)seed);
}

Random::Random()
{
    srand((unsigned int)time(NULL));
}

int Random::NextInt(const int max)
{
    return (rand() % max);
}

int Random::NextInt(const int min, const int max)
{
    return (IntBounds(min, max));
}

uint8 Random::NextByte(const uint8 max)
{
    return ((uint8)IntBounds(0, max));
}

uint8 Random::NextByte(const uint8 min, const uint8 max)
{
    return ((uint8)IntBounds(min, max));
}

uint16 Random::NextShort(const uint16 max)
{
    return ((uint16)IntBounds(0, max));
}

uint16 Random::NextShort(const uint16 min, const uint16 max)
{
    return ((uint16)IntBounds(min, max));
}

float Random::NextFloat(const float min)
{
    int start = Framework::FMath::Abs(rand() % 256);

    return (((float)start / 256.0f) + min);
}

double Random::NextDouble()
{
    int start = Framework::FMath::Abs(rand() % 256);

    return ((double)start / 256.0f);
}
