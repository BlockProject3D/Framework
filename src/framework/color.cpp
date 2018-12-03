#include "framework/framework.h"
#include "framework/color.h"
#include "framework/bmath.h"

using namespace Framework;

const FColor FColor::Red = FColor(255, 0, 0);
const FColor FColor::Green = FColor(0, 255, 0);
const FColor FColor::Blue = FColor(0, 0, 255);
const FColor FColor::White = FColor(255, 255, 255);
const FColor FColor::Black = FColor(0, 0, 0);
const FColor FColor::Yellow = FColor(255, 255, 0);
const FColor FColor::Cyan = FColor(0, 255, 255);

FColor FColor::operator+(const FColor &other) const
{
    uint8 newr = (uint8)FMath::Clamp(R + other.R, 0, 255);
    uint8 newg = (uint8)FMath::Clamp(G + other.G, 0, 255);
    uint8 newb = (uint8)FMath::Clamp(B + other.B, 0, 255);
    
    return (FColor(newr, newg, newb));
}

FColor FColor::operator*(const FColor &other) const
{
    float r = (float)R / 255.0f;
    float g = (float)G / 255.0f;
    float b = (float)B / 255.0f;
    float r1 = (float)other.R / 255.0f;
    float g1 = (float)other.G / 255.0f;
    float b1 = (float)other.B / 255.0f;
    float newr = r * r1;
    float newg = g * g1;
    float newb = b * b1;

    return (FColor(newr * 255, newg * 255, newb * 255));
}
