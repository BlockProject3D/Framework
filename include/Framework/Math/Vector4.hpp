// Copyright (c) 2018, BlockProject
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
#include "Framework/Math/BMath.hpp"

namespace bpf
{
    template <typename T>
    class BP_TPL_API Vector2
    {
    public:
        T X;
        T Y;

        Vector2(const T x, const t y)
            : X(x)
            , Y(y)
        {
        }

        Vector2(const Vector2<T> &other)
            : X(other.X)
            , Y(other.Y)
        {
        }

        Vector2()
            : X(0)
            , Y(0)
        {
        }

        inline T Dot(const Vector2<T> &other) const
        {
            return (X * other.X + Y * other.Y);
        }

        inline T Length() const
        {
            return (sqrt(X * X + Y * Y));
        }

        inline T Distance(const Vector2<T> &other) const
        {
            Vector2<T> v(Framework::FMath::Abs(other.X - X), Framework::FMath::Abs(other.Y - Y));
            return (v.Length());
        }

        inline T DistanceSquared(const Vector2<T> &other) const
        {
            Vector2<T> v(Framework::FMath::Abs(other.X - X), Framework::FMath::Abs(other.Y - Y));
            return (v.X * v.X + v.Y * v.Y);
        }

        inline void Normalize()
        {
            X = X / Length();
            Y = Y / Length();
        }
        
        //TODO : Rotate function

        inline Vector2<T> &operator=(const Vector2<T> &other)
        {
            X = other.X;
            Y = other.Y;
        }

        inline Vector2<T> operator+(const Vector2<T> &other) const
        {
            return (Vector2<T>(X + other.X, Y + other.Y));
        }

        inline Vector2<T> operator-(const Vector2<T> &other) const
        {
            return (Vector2<T>(X - other.X, Y - other.Y));
        }

        inline Vector2<T> operator*(const Vector2<T> &other) const
        {
            return (Vector2<T>(X * other.X, Y * other.Y));
        }

        inline Vector2<T> operator/(const Vector2<T> &other) const
        {
            return (Vector2<T>(X / other.X, Y / other.Y));
        }

        inline void operator+=(const Vector2<T> &other) const
        {
            X += other.X;
            Y += other.Y;
        }

        inline void operator-=(const Vector2<T> &other) const
        {
            X -= other.X;
            Y -= other.Y;
        }

        inline void operator*=(const Vector2<T> &other) const
        {
            X *= other.X;
            Y *= other.Y;
        }

        inline void operator/=(const Vector2<T> &other) const
        {
            X /= other.X;
            Y /= other.Y;
        }

        inline Vector2<T> operator-() const
        {
            return (Vector2<T>(-X, -Y));
        }

        inline bool operator==(const Vector2<T> &other) const
        {
            return (X == other.X && Y == other.Y && Z == other.Z);
        }

        static Vector2<T> Lerp(const Vector2<T> &v, const Vector2<T> &v1, const T t);

        static const Vector2<T> Zero;
        static const Vector2<T> Unit;
    };
}

namespace Framework
{
    class FMatrix;
    class FViewport;
    class FTransform;
    class FQuat;

    class ENGINE_API FVector
    {
    public:
        float X;
        float Y;
        float Z;

        FVector(const float x, const float y, const float z);
        FVector();
        FVector(const float x, const float y);
        FVector(const FVector &other);
        float Length() const;
        float Distance(const FVector &other) const;
        float DistanceSquared(const FVector &other) const;
        inline float Dot(const FVector &other) const
        {
            return (X * other.X + Y * other.Y + Z * other.Z);
        }
        FVector Cross(const FVector &other) const;
        FVector Project(const FMatrix &view, const FViewport &viewport);
        void Normalize();
        void ApplyTransform(const FTransform &transform);
        FVector &operator=(const FVector &other);
        FVector operator+(const FVector &other) const;
        void operator+=(const FVector &other);
        FVector operator-(const FVector &other) const;
        inline FVector operator-() const
        {
            return (FVector(-X, -Y, -Z));
        }
        FVector operator*(const FQuat &other) const;
        void operator-=(const FVector &other);
        FVector operator*(const FVector &other) const;
        void operator*=(const FVector &other);
        FVector operator*(const float scale) const;
        void operator*=(const float scale);
        inline bool operator==(const FVector &other) const
        {
            return (X == other.X && Y == other.Y && Z == other.Z);
        }
        static FVector Lerp(const FVector &v, const FVector &v1, const float t);
        static FVector Slerp(const FVector &v, const FVector &v1, const float t); //Failure implementing Slerp formula uses undefined variable called omega
      
        static const FVector	Zero;
        static const FVector	Unit;
        static const FVector	Right;
        static const FVector	Up;
        static const FVector	Forward;
        static const FVector	Backward;
        static const FVector	Left;
        static const FVector	Down;
    };
    
    /**
     * Homogeneous coordinates vector (X, Y, Z, W)
     */
    class ENGINE_API FVector4D
    {
    public:
        float X;
        float Y;
        float Z;
        float W;

        FVector4D();
        FVector4D(const FVector &vec, float w);
        FVector4D(const FVector4D &other);
        FVector4D(float x, float y, float z, float w);
        FVector4D operator*(const FMatrix &other) const;
        FVector4D &operator=(const FVector4D &other);
        inline FVector PerspectiveDevide() const
        {
            return (FVector(X / W, Y / W, Z / W));
        }
    };
};
