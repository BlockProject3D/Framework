#include "framework/framework.h"
#include "framework/bmath.h"
#include "framework/vector.h"
#include "framework/vertex.h"

using namespace Framework;

FVertex::FVertex(float x, float y, float z, float u, float v)
    : X(x), Y(y), Z(z), U(u), V(v)
{
    SetNormal(FVector::Zero);
}

FVertex::FVertex(const FVector &pos, const FVector &normal, float u, float v)
    : X(pos.X), Y(pos.Y), Z(pos.Z), U(u), V(v)
{
    SetNormal(normal);
}

FVertex::FVertex(const FVertex &other)
    : X(other.X), Y(other.Y), Z(other.Z), U(other.U), V(other.V)
{
    SetNormal(FVector::Zero);
}

FVertex::FVertex()
    : X(0), Y(0), Z(0), U(0), V(0)
{
    SetNormal(FVector::Zero);
}

void FVertex::SetNormal(const FVector &vec)
{
    NX = vec.X;
    NY = vec.Y;
    NZ = vec.Z;
}

FVertex FVertex::operator-()
{
    return (FVertex(FVector(-X, -Y, -Z), FVector(NX, NY, NZ), U, V));
}

bool FVertex::operator==(const FVertex &other)
{
    return (X == other.X && Y == other.Y && Z == other.Z && U == other.U && V == other.V);
}

void FVertex::ApplyTransform(const FTransform &tr)
{
    FMatrix mat = tr.GetMatrix();
    FVector4D v(X, Y, Z, 1.0f);
    FVector4D res = v * mat;
    FMatrix rot = tr.GetQuat().ToMatrix();
    FVector4D v1(NX, NY, NZ, 1.0f);
    FVector4D res1 = v1 * rot;

    X = res.X;
    Y = res.Y;
    Z = res.Z;
    NX = res1.X;
    NY = res1.Y;
    NZ = res1.Z;
}
