// Copyright (c) 2020, BlockProject
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

namespace bpf
{
    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::Translate(const Vector3<T> &translation) noexcept
    {
        _matrix *= {
            1, 0, 0, translation.X,
            0, 1, 0, translation.Y,
            0, 0, 1, translation.Z,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::Scale(const Vector3<T> &scale) noexcept
    {
        _matrix *= {
            scale.X, 0, 0, 0,
            0, scale.Y, 0, 0,
            0, 0, scale.Z, 0,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::ShearX(const Vector3<T> &shear) noexcept
    {
        _matrix *= {
            1, 0, 0, 0,
            shear.Y, 1, 0, 0,
            shear.Z, 0, 1, 0,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::ShearY(const Vector3<T> &shear) noexcept
    {
        _matrix *= {
            1, shear.X, 0, 0,
            0, 1, 0, 0,
            0, shear.Z, 1, 0,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::ShearZ(const Vector3<T> &shear) noexcept
    {
        _matrix *= {
            1, 0, shear.X, 0,
            0, 1, shear.Z, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::RotateX(const T &rotation) noexcept
    {
        _matrix *= {
            1, 0, 0, 0,
            0, Math::Cos(xyz.x), -Math::Sin(xyz.x), 0,
            0, Math::Sin(xyz.x), Math::Cos(xyz.x), 0,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::RotateY(const T &rotation) noexcept
    {
        _matrix *= {
            Math::Cos(xyz.y), 0, Math::Sin(xyz.y), 0,
            0, 1, 0, 0,
            -Math::Sin(xyz.y), 0, Math::Cos(xyz.y), 0,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Transform3D<T>::MatrixBuilder &Transform3D<T>::MatrixBuilder::RotateZ(const T &rotation) noexcept
    {
        _matrix *= {
            Math::Cos(xyz.z), -Math::Sin(xyz.z), 0, 0,
            Math::Sin(xyz.z), Math::Cos(xyz.z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        return (*this);
    }

    template <typename T>
    Vector3<T> Transform3D<T>::LocalToWorld(const Vector3<T> &local)
    {
        auto res = ToMatrix() * Vector4<T>(local, (T)1);

        return (Vector3<T>(res.X, res.Y, res.Z));
    }

    template <typename T>
    Vector3<T> Transform3D<T>::WorldToLocal(const Vector3<T> &world)
    {
        auto res = ToMatrix().Invert() * Vector4<T>(world, (T)1);

        return (Vector3<T>(res.X, res.Y, res.Z));
    }

    template <typename T>
    Transform3D<T> Transform3D<T>::operator+(const Transform3D &other) const noexcept
    {
        Transform3D<T> res(Position + other.Position, Scale * other.Scale, Angle * other.Angle);

        return (res);
    }

    template <typename T>
    void Transform3D<T>::operator+=(const Transform3D &other)
    {
        Position += other.Position;
        Scale *= other.Scale;
        Angle *= other.Angle;
    }

    template <typename T>
    void Transform3D<T>::RotateArround(const Vector3<T> &pivot, const Quaternion<T> &rotation) noexcept
    {
        auto dir = Position - pivot;

        rotation.Rotate(dir);
        Position = dir + pivot;
    }
}
