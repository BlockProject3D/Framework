#include <time.h>
#include "Framework/Framework.hpp"
#include "Framework/Math/BMath.hpp"
#include "Framework/Random.hpp"

using namespace Framework;

int FRandom::IntBounds(const int min, const int max)
{
    int rd = rand() % max;

    while (rd < min || rd > max)
        rd = rand() % max;
    return (rd);
}

FRandom::FRandom(const long seed)
{
    srand((uint32)seed);
}

FRandom::FRandom()
{
    srand((unsigned int)time(NULL));
}

int FRandom::NextInt(const int max)
{
    return (rand() % max);
}

int FRandom::NextInt(const int min, const int max)
{
    return (IntBounds(min, max));
}

uint8 FRandom::NextByte(const uint8 max)
{
    return ((uint8)IntBounds(0, max));
}

uint8 FRandom::NextByte(const uint8 min, const uint8 max)
{
    return ((uint8)IntBounds(min, max));
}

uint16 FRandom::NextShort(const uint16 max)
{
    return ((uint16)IntBounds(0, max));
}

uint16 FRandom::NextShort(const uint16 min, const uint16 max)
{
    return ((uint16)IntBounds(min, max));
}

float FRandom::NextFloat(const float min)
{
    int start = FMath::Abs(rand() % 256);

    return (((float)start / 256.0f) + min);
}

double FRandom::NextDouble()
{
    int start = FMath::Abs(rand() % 256);

    return ((double)start / 256.0f);
}

FVector FRandom::NextVector(const float mult)
{
    return (FVector(NextFloat() * mult, NextFloat() * mult, NextFloat() * mult));
}

FColor FRandom::NextColor()
{
    return (FColor(NextByte(), NextByte(), NextByte()));
}
