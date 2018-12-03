#include "Framework/Framework.hpp"
#include "Framework/Color.hpp"
#include "Framework/Math/BMath.hpp"

using namespace bpf;

const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::Cyan = Color(0, 255, 255);

Color Color::operator+(const Color &other) const
{
    uint8 newr = (uint8)Framework::FMath::Clamp(R + other.R, 0, 255);
    uint8 newg = (uint8)Framework::FMath::Clamp(G + other.G, 0, 255);
    uint8 newb = (uint8)Framework::FMath::Clamp(B + other.B, 0, 255);
    
    return (Color(newr, newg, newb));
}

Color Color::operator*(const Color &other) const
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

    return (Color(newr * 255, newg * 255, newb * 255));
}
