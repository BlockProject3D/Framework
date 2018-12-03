#include "framework/bmath.h"

using namespace Framework;

const FQuat FQuat::Zero = FQuat(0, 0, 0, 0);
const FQuat FQuat::Identity = FQuat();

FQuat::FQuat(const FVector &eulerangles)
{
    float cr = cosf(eulerangles.X * FMath::DegToRad / 2.0f);
    float cp = cosf(eulerangles.Y * FMath::DegToRad / 2.0f);
    float cy = cosf(eulerangles.Z * FMath::DegToRad / 2.0f);
    float sr = sinf(eulerangles.X * FMath::DegToRad / 2.0f);
    float sp = sinf(eulerangles.Y * FMath::DegToRad / 2.0f);
    float sy = sinf(eulerangles.Z * FMath::DegToRad / 2.0f);
    W = cr * cp * cy + sr * sp * sy;
    X = sr * cp * cy - cr * sp * sy;
    Y = cr * sp * cy + sr * cp * sy;
    Z = cr * cp * sy - sr * sp * cy;
}

FQuat::FQuat(const FVector &from, const FVector &to)
{
    FVector a = from.Cross(to);

    X = a.X;
    Y = a.Y;
    Z = a.Z;
    W = sqrt((from.Length() * from.Length()) * (to.Length() * to.Length())) + from.Dot(to);
}

FQuat::FQuat(const float w, const float x, const float y, const float z)
    : X(x), Y(y), Z(z), W(w)
{
}

FQuat::FQuat(const FVector &axis, const float rot)
    : X(axis.X * sinf((rot / 2.0f) * FMath::DegToRad)), Y(axis.Y * sinf((rot / 2.0f) * FMath::DegToRad)),
    Z(axis.Z * sinf((rot / 2.0f) * FMath::DegToRad)), W(cosf((rot / 2.0f) * FMath::DegToRad))
{
    Normalize();
}

FQuat::FQuat()
    : X(0), Y(0), Z(0), W(1)
{
}

FQuat::FQuat(const FQuat &other)
    : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
{
}

FQuat FQuat::operator*(const float n) const
{
    FQuat q = Ln();

    q.W *= n;
    q.X *= n;
    q.Y *= n;
    q.Z *= n;
    return (q.Exp());
}

FQuat FQuat::Exp() const
{
    float r = sqrtf(X * X + Y * Y + Z * Z);
    float et = expf(W);
    float s = r >= 0.00001f ? et * sinf(r) / r : 0.0f;

    return (FQuat(et * cosf(r), X * s, Y * s, Z * s));
}

FQuat FQuat::Ln() const
{
    float r = sqrtf(X * X + Y * Y + Z * Z);
    float t = r > 0.00001f ? atan2f(r, W) / r : 0.0f;

    return (FQuat(0.5f * logf(W * W + X * X + Y * Y + Z * Z), X * t, Y * t, Z * t));
}


FQuat FQuat::Lerp(const FQuat &q, const FQuat &q1, const float t)
{
    FQuat res(FMath::Lerp(q.W, q1.W, t), FMath::Lerp(q.X, q1.X, t), FMath::Lerp(q.Y, q1.Y, t), FMath::Lerp(q.Z, q1.Z, t));

    return (res);
}

FQuat FQuat::Slerp(const FQuat &q, const FQuat &q1, const float t)
{
    FQuat a = ((q1 * q.Conjugate()) * t) * q;

    return (a);
}

FMatrix FQuat::ToMatrix() const
{
    FMatrix mat = {
        W * W + X * X - Y * Y - Z * Z, 2 * X * Y - 2 * W * Z, 2 * X * Z + 2 * W * Y, 0,
        2 * X * Y + 2 * W * Z, W * W - X * X + Y * Y - Z * Z, 2 * Y * Z - 2 * W * X, 0,
        2 * X * Z - 2 * W * Y, 2 * Y * Z + 2 * W * X, W * W - X * X - Y * Y + Z * Z, 0,
        0, 0, 0, 1
    };

    return (mat);
}

void FQuat::Normalize()
{
    float mag = sqrtf(W * W + X * X + Y * Y + Z * Z);

    X /= mag;
    Y /= mag;
    Z /= mag;
    W /= mag;
}

bool FQuat::operator==(const FQuat &other) const
{
    return (X == other.X && Y == other.Y && Z == other.Z);
}

FQuat &FQuat::operator=(const FQuat &other)
{
    X = other.X;
    Y = other.Y;
    Z = other.Z;
    W = other.W;
    return (*this);
}

FQuat FQuat::operator*(const FQuat &other) const
{
    FQuat res;

    res.W = W * other.W - X * other.X - Y * other.Y - Z * other.Z;
    res.X = W * other.X + X * other.W + Y * other.Z - Z * other.Y;
    res.Y = W * other.Y + Y * other.W + Z * other.X - X * other.Z;
    res.Z = W * other.Z + Z * other.W + X * other.Y - Y * other.X;
    return (res);
}

FVector FQuat::ToEulerAngles() const
{
    float roll = atan2f(2 * Y * W + 2 * X * Z, 1 - 2 * Y * Y - 2 * Z * Z) * FMath::RadToDeg;
    float pitch = atan2f(2 * X * W + 2 * Y * Z, 1 - 2 * X * X - 2 * Z * Z) * FMath::RadToDeg;
    float yaw = asinf(2 * X * Y + 2 * Z * W) * FMath::RadToDeg;

    return (FVector(pitch, yaw, roll));
}

float FQuat::Angle(const FQuat &other) const
{
    FQuat res = other * Conjugate();

    return (acosf(res.W) * 2.0f * FMath::RadToDeg);
}
