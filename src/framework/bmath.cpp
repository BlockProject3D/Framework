#include <stdlib.h>
#include <time.h>
#include "framework/bmath.h"

using namespace Framework;

void FMath::RotationMatrix(FMatrix &out, const ERotationAxis axis, const float angle)
{
    switch (axis)
    {
    case ROT_AXIS_Y:
        {
            out.Set(1, 1, cosf(angle * DegToRad));
            out.Set(1, 2, -sinf(angle * DegToRad));
            out.Set(2, 1, sinf(angle * DegToRad));
            out.Set(2, 2, cosf(angle * DegToRad));
            break;
        }
    case ROT_AXIS_X:
        {
            out.Set(0, 0, cosf(angle * DegToRad));
            out.Set(0, 2, sinf(angle * DegToRad));
            out.Set(2, 0, -sinf(angle * DegToRad));
            out.Set(2, 2, cosf(angle * DegToRad));
            break;
        }
    case ROT_AXIS_Z:
        {
            out.Set(0, 0, cosf(angle * DegToRad));
            out.Set(0, 1, -sinf(angle * DegToRad));
            out.Set(1, 0, sinf(angle * DegToRad));
            out.Set(1, 1, cosf(angle * DegToRad));
            break;
        }
    }
}

FMatrix FMath::PerspectiveProjectionMatrix(const float fovy, const float aspect, const float znear, const float zfar)
{
    FMatrix out = FMatrix::Zero;
    float f = 1 / tanf((fovy / 2.0f) * DegToRad);

    out = FMatrix({
                  1 / (aspect * tanf((fovy / 2.0f) * DegToRad)), 0, 0, 0,
                  0, f, 0, 0,
                  0, 0, -(zfar + znear) / (zfar - znear), -((2 * zfar * znear) / (zfar - znear)),
                  0, 0, -1, 0
                });
    return (out);
}

FMatrix FMath::ScreenProjectionMatrix(const float w, const float h)
{
    FMatrix out = FMatrix::Identity;

    out.Scale(FVector(1.0f / w, -(1.0f / h), 0));
    out.Translate(FVector(-1, 1, 0));
    return (out);
}

bool FMath::IsPrime(const int n)
{
    int a = 2;

    while (a < n)
    {
        if ((n % a) == 0)
            return (false);
        ++a;
    }
    return (true);
}

int FMath::FindNextPrime(const int n)
{
    int a = n + 1;

    while (!IsPrime(a))
        ++a;
    return (a);
}
