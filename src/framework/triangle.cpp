#include "framework/framework.h"
#include "framework/bmath.h"

using namespace Framework;

FTriangle::FTriangle(const FVertex &p1, const FVertex &p2, const FVertex &p3)
    : VertA(p1), VertB(p2), VertC(p3)
{
}

void FTriangle::Move(const FVector &offset)
{
    VertA.X += offset.X;
    VertA.Y += offset.Y;
    VertA.Z += offset.Z;

    VertB.X += offset.X;
    VertB.Y += offset.Y;
    VertB.Z += offset.Z;

    VertC.X += offset.X;
    VertC.Y += offset.Y;
    VertC.Z += offset.Z;
}

void FTriangle::ApplyTransform(const FTransform &tr)
{
    VertA.ApplyTransform(tr);
    VertB.ApplyTransform(tr);
    VertC.ApplyTransform(tr);
}
