#include "Framework/Framework.hpp"
#include "Framework/Math/BMath.hpp"

using namespace Framework;

FMatrix	FMatrix::Identity = FMatrix({1, 0, 0, 0,
                                     0, 1, 0, 0,
                                     0, 0, 1, 0,
                                     0, 0, 0, 1});

FMatrix	FMatrix::Zero = FMatrix({0, 0, 0, 0,
                                 0, 0, 0, 0,
                                 0, 0, 0, 0,
                                 0, 0, 0, 0});

FMatrix::FMatrix(std::initializer_list<float> lst)
{
    uint8 i = 0;

    for (auto it = lst.begin() ; it != lst.end() ; ++it)
        Data[i++] = *it;
}

FMatrix::FMatrix()
{
    for (uint8 i = 0 ; i < 16 ; i++)
        Data[i] = 0;
}

FMatrix::FMatrix(const float *mat)
{
    for (uint8 i = 0 ; i < 16 ; i++)
        Data[i] = mat[i];
}

FMatrix::FMatrix(const FMatrix &other)
{
    for (uint8 i = 0 ; i < 16 ; i++)
        Data[i] = other.Data[i];
}

FMatrix &FMatrix::operator=(const FMatrix &other)
{
    for (uint8 i = 0 ; i < 16 ; i++)
        Data[i] = other.Data[i];
    return (*this);
}

FMatrix FMatrix::operator*(const FMatrix &other) const
{
    FMatrix mat;

    for (uint8 i = 0 ; i < 4 ; ++i)
    {
        for (uint8 j = 0 ; j < 4 ; ++j)
        {
            float res = 0;
            for (uint8 k = 0 ; k < 4 ; ++k)
                res += Data[k * 4 + j] * other.Data[i * 4 + k];
            mat.Data[i * 4 + j] = res;
        }
    }
    return (mat);
}

void FMatrix::Invert()
{
    float inv[16];
    float det;

    inv[0] = Data[5]  * Data[10] * Data[15] -
             Data[5]  * Data[11] * Data[14] -
             Data[9]  * Data[6]  * Data[15] +
             Data[9]  * Data[7]  * Data[14] +
             Data[13] * Data[6]  * Data[11] -
             Data[13] * Data[7]  * Data[10];

    inv[4] = -Data[4]  * Data[10] * Data[15] +
              Data[4]  * Data[11] * Data[14] +
              Data[8]  * Data[6]  * Data[15] -
              Data[8]  * Data[7]  * Data[14] -
              Data[12] * Data[6]  * Data[11] +
              Data[12] * Data[7]  * Data[10];

    inv[8] = Data[4]  * Data[9] * Data[15] -
             Data[4]  * Data[11] * Data[13] -
             Data[8]  * Data[5] * Data[15] +
             Data[8]  * Data[7] * Data[13] +
             Data[12] * Data[5] * Data[11] -
             Data[12] * Data[7] * Data[9];

    inv[12] = -Data[4]  * Data[9] * Data[14] +
               Data[4]  * Data[10] * Data[13] +
               Data[8]  * Data[5] * Data[14] -
               Data[8]  * Data[6] * Data[13] -
               Data[12] * Data[5] * Data[10] +
               Data[12] * Data[6] * Data[9];

    inv[1] = -Data[1]  * Data[10] * Data[15] +
              Data[1]  * Data[11] * Data[14] +
              Data[9]  * Data[2] * Data[15] -
              Data[9]  * Data[3] * Data[14] -
              Data[13] * Data[2] * Data[11] +
              Data[13] * Data[3] * Data[10];

    inv[5] = Data[0]  * Data[10] * Data[15] -
             Data[0]  * Data[11] * Data[14] -
             Data[8]  * Data[2] * Data[15] +
             Data[8]  * Data[3] * Data[14] +
             Data[12] * Data[2] * Data[11] -
             Data[12] * Data[3] * Data[10];

    inv[9] = -Data[0]  * Data[9] * Data[15] +
              Data[0]  * Data[11] * Data[13] +
              Data[8]  * Data[1] * Data[15] -
              Data[8]  * Data[3] * Data[13] -
              Data[12] * Data[1] * Data[11] +
              Data[12] * Data[3] * Data[9];

    inv[13] = Data[0]  * Data[9] * Data[14] -
              Data[0]  * Data[10] * Data[13] -
              Data[8]  * Data[1] * Data[14] +
              Data[8]  * Data[2] * Data[13] +
              Data[12] * Data[1] * Data[10] -
              Data[12] * Data[2] * Data[9];

    inv[2] = Data[1]  * Data[6] * Data[15] -
             Data[1]  * Data[7] * Data[14] -
             Data[5]  * Data[2] * Data[15] +
             Data[5]  * Data[3] * Data[14] +
             Data[13] * Data[2] * Data[7] -
             Data[13] * Data[3] * Data[6];

    inv[6] = -Data[0]  * Data[6] * Data[15] +
              Data[0]  * Data[7] * Data[14] +
              Data[4]  * Data[2] * Data[15] -
              Data[4]  * Data[3] * Data[14] -
              Data[12] * Data[2] * Data[7] +
              Data[12] * Data[3] * Data[6];

    inv[10] = Data[0]  * Data[5] * Data[15] -
              Data[0]  * Data[7] * Data[13] -
              Data[4]  * Data[1] * Data[15] +
              Data[4]  * Data[3] * Data[13] +
              Data[12] * Data[1] * Data[7] -
              Data[12] * Data[3] * Data[5];

    inv[14] = -Data[0]  * Data[5] * Data[14] +
               Data[0]  * Data[6] * Data[13] +
               Data[4]  * Data[1] * Data[14] -
               Data[4]  * Data[2] * Data[13] -
               Data[12] * Data[1] * Data[6] +
               Data[12] * Data[2] * Data[5];

    inv[3] = -Data[1] * Data[6] * Data[11] +
              Data[1] * Data[7] * Data[10] +
              Data[5] * Data[2] * Data[11] -
              Data[5] * Data[3] * Data[10] -
              Data[9] * Data[2] * Data[7] +
              Data[9] * Data[3] * Data[6];

    inv[7] = Data[0] * Data[6] * Data[11] -
             Data[0] * Data[7] * Data[10] -
             Data[4] * Data[2] * Data[11] +
             Data[4] * Data[3] * Data[10] +
             Data[8] * Data[2] * Data[7] -
             Data[8] * Data[3] * Data[6];

    inv[11] = -Data[0] * Data[5] * Data[11] +
               Data[0] * Data[7] * Data[9] +
               Data[4] * Data[1] * Data[11] -
               Data[4] * Data[3] * Data[9] -
               Data[8] * Data[1] * Data[7] +
               Data[8] * Data[3] * Data[5];

    inv[15] = Data[0] * Data[5] * Data[10] -
              Data[0] * Data[6] * Data[9] -
              Data[4] * Data[1] * Data[10] +
              Data[4] * Data[2] * Data[9] +
              Data[8] * Data[1] * Data[6] -
              Data[8] * Data[2] * Data[5];

    det = Data[0] * inv[0] + Data[1] * inv[4] + Data[2] * inv[8] + Data[3] * inv[12];
    if (det == 0)
        return;
    det = 1.0f / det;
    for (int i = 0; i < 16; i++)
        Data[i] = inv[i] * det;
}

void FMatrix::Translate(const FVector &v)
{
    FMatrix translate = FMatrix({1, 0, 0, v.X,
                                 0, 1, 0, v.Y,
                                 0, 0, 1, v.Z,
                                 0, 0, 0, 1});

    *this = *this * translate;
}

void FMatrix::Rotate(const ERotationAxis axis, const float ang)
{
    FMatrix rotate = FMatrix::Identity;

    FMath::RotationMatrix(rotate, axis, ang);
    *this = *this * rotate;
}

void FMatrix::RotateYaw(const float ang)
{
    Rotate(ROT_AXIS_X, ang);
}

void FMatrix::RotatePitch(const float ang)
{
    Rotate(ROT_AXIS_Y, ang);
}

void FMatrix::Scale(const FVector &v)
{
    FMatrix scale = FMatrix({v.X, 0, 0, 0,
                             0, v.Y, 0, 0,
                             0, 0, v.Z, 0,
                             0, 0, 0, 1});

    *this = *this * scale;
}

void FMatrix::Set(const int i, const int j, const float d)
{
    if ((i < 4 && i >= 0) && (j < 4 && j >= 0))
        Data[i * 4 + j] = d;
}

float FMatrix::Get(const int i, const int j) const
{
    if (i >= 0 && i < 4 && j >= 0 && j < 4)
        return (Data[i * 4 + j]);
    return (-1);
}
