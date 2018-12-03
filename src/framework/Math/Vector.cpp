#include "framework/framework.h"
#include "framework/bmath.h"

using namespace Framework;

const FVector FVector::Zero = FVector(0, 0, 0);
const FVector FVector::Unit = FVector(1, 1, 1);
const FVector FVector::Right = FVector(1, 0, 0);
const FVector FVector::Up = FVector(0, 1, 0);
const FVector FVector::Forward = FVector(0, 0, -1);
const FVector FVector::Left = FVector(-1, 0, 0);
const FVector FVector::Down = FVector(0, -1, 0);
const FVector FVector::Backward = FVector(0, 0, 1);

FVector::FVector(const float x, const float y, const float z)
    : X(x), Y(y), Z(z)
{
}

float FVector::DistanceSquared(const FVector &other) const
{
    FVector v(FMath::Abs(other.X - X), FMath::Abs(other.Y - Y), FMath::Abs(other.Z - Z));

    return (v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

FVector FVector::Project(const FMatrix &view, const FViewport &viewport)
{
    FMatrix viewproj = view * viewport.Projection;
    FVector4D vec(*this, 1.0f);
    FVector projected;

    vec = vec * viewproj;
    projected = vec.PerspectiveDevide();
    projected.X /= projected.Z;
    projected.Y /= projected.Z;
    projected.X = (((1 + projected.X) / 2.f) * viewport.Width) + 0.5f;
    projected.Y = (((1 - projected.Y) / 2.f) * viewport.Height) + 0.5f;
    if (vec.W < 0)
    {
        if (projected.X > 0)
            projected.X *= -1;
        if (projected.Y > 0)
            projected.Y *= -1;
    }
    return (projected);
}

FVector::FVector(const float x, const float y)
    : X(x), Y(y), Z(0)
{
}

FVector::FVector(const FVector &other)
    : X(other.X), Y(other.Y), Z(other.Y)
{
}

FVector::FVector()
    : X(0), Y(0), Z(0)
{
}

FVector FVector::Cross(const FVector &other) const
{
    float cx = Y * other.Z - Z * other.Y;
    float cy = Z * other.X - X * other.Z;
    float cz = X * other.Y - Y * other.X;

    return (FVector(cx, cy, cz));
}

float FVector::Length() const
{
    return (sqrt(X * X + Y * Y + Z * Z));
}

float FVector::Distance(const FVector &other) const
{
    FVector v(FMath::Abs(other.X - X), FMath::Abs(other.Y - Y), FMath::Abs(other.Z - Z));

    return (v.Length());
}

void FVector::Normalize()
{
    X = X / Length();
    Y = Y / Length();
    Z = Z / Length();
}

void FVector::ApplyTransform(const FTransform &transform)
{
    FMatrix mat = transform.GetMatrix();
    FVector4D v(X, Y, Z, 1.0f);
    FVector4D res = v * mat;

    X = res.X;
    Y = res.Y;
    Z = res.Z;
}

FVector &FVector::operator=(const FVector &other)
{
    X = other.X;
    Y = other.Y;
    Z = other.Z;
    return (*this);
}

FVector FVector::operator+(const FVector &other) const
{
    FVector v(X + other.X, Y + other.Y, Z + other.Z);

    return (v);
}

void FVector::operator+=(const FVector &other)
{
    X += other.X;
    Y += other.Y;
    Z += other.Z;
}

FVector FVector::operator-(const FVector &other) const
{
    FVector v(X - other.X, Y - other.Y, Z - other.Z);

    return (v);
}

void FVector::operator-=(const FVector &other)
{
    X -= other.X;
    Y -= other.Y;
    Z -= other.Z;
}

FVector FVector::operator*(const FVector &other) const
{
    FVector v(X * other.X, Y * other.Y, Z * other.Z);

    return (v);
}

void FVector::operator*=(const FVector &other)
{
    X *= other.X;
    Y *= other.Y;
    Z *= other.Z;
}

FVector FVector::operator*(const float scale) const
{
    FVector v(X * scale, Y * scale, Z * scale);

    return (v);
}

FVector FVector::operator*(const FQuat &other) const
{
    FQuat p(0, X, Y, Z);
    FQuat pprime = other.Conjugate() * p * other;
    return (pprime.GetAxis());
}

void FVector::operator*=(const float scale)
{
    X *= scale;
    Y *= scale;
    Z *= scale;
}

FVector FVector::Lerp(const FVector &v, const FVector &v1, const float t)
{
    FVector res(FMath::Lerp(v.X, v1.X, t), FMath::Lerp(v.Y, v1.Y, t), FMath::Lerp(v.Z, v1.Z, t));

    return (res);
}

FVector4D::FVector4D()
    : X(0), Y(0), Z(0), W(1.0f)
{
}

FVector4D::FVector4D(const FVector4D &other)
    : X(other.X), Y(other.Y), Z(other.Z), W(other.W)
{
}

FVector4D::FVector4D(float x, float y, float z, float w)
    : X(x), Y(y), Z(z), W(w)
{
}

FVector4D::FVector4D(const FVector &vec, float w)
    : X(vec.X), Y(vec.Y), Z(vec.Z), W(w)
{
}

FVector4D FVector4D::operator*(const FMatrix &other) const
{
    const float *data = *other;
    float myvec[4] = { X, Y, Z, W };
    float result[4];

    for (uint8 i = 0 ; i < 4 ; i++)
    {
        float res = 0;
        for (uint8 k = 0 ; k < 4 ; k++)
            res += myvec[k] * data[i * 4 + k];
        result[i] = res;
    }
    return (FVector4D(result[0], result[1], result[2], result[3]));
}

FVector4D &FVector4D::operator=(const FVector4D &other)
{
  X = other.X;
  Y = other.Y;
  Z = other.Z;
  W = other.W;
  return (*this);
}
