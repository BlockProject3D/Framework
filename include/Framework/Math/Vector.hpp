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
