#include "Framework/Math/BMath.hpp"

using namespace Framework;

FTransform::FTransform()
    : PosVec(FVector::Zero), ScaleVec(FVector::Unit), Rot(FQuat::Identity)
{
    RebuildMatrix();
}

FTransform::FTransform(const FTransform &other)
    : PosVec(other.PosVec), ScaleVec(other.ScaleVec), Rot(other.Rot)
{
    RebuildMatrix();
}

FTransform::FTransform(const FVector &pos, const FVector &scale, const FQuat &rot)
    : PosVec(pos), ScaleVec(scale), Rot(rot)
{
    RebuildMatrix();
}

FTransform::FTransform(const FVector &pos, const FVector &scale)
    : PosVec(pos), ScaleVec(scale), Rot(FQuat::Identity)
{
    RebuildMatrix();
}

FTransform::FTransform(const FVector &pos)
    : PosVec(pos), ScaleVec(FVector::Unit), Rot(FQuat::Identity)
{
    RebuildMatrix();
}

FVector FTransform::WorldToLocal(const FVector &vec)
{
    FVector4D v(vec, 1.0f);
    FMatrix worldinv = Matrix;

    worldinv.Invert();
    FVector4D res = v * worldinv;
    return (FVector(res.X, res.Y, res.Z));
}

FVector FTransform::LocalToWorld(const FVector &vec)
{
    FVector4D v(vec, 1.0f);
    FMatrix world = Matrix;

    FVector4D res = v * world;
    return (FVector(res.X, res.Y, res.Z));
}

void FTransform::RebuildMatrix()
{
    FMatrix mat = Rot.ToMatrix();

    Matrix = FMatrix::Identity;
    Matrix.Scale(ScaleVec);
    Matrix = Matrix * mat;
    Matrix.Translate(PosVec);
}

FTransform FTransform::operator+(const FTransform &other) const
{
    FTransform res;

    res.PosVec = (PosVec * ScaleVec) + (other.PosVec * other.ScaleVec);
    res.ScaleVec = ScaleVec * other.ScaleVec;
    res.Rot = Rot * other.Rot;
    res.RebuildMatrix();
    return (res);
}
