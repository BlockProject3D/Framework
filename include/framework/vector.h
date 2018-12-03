#ifndef VECTOR_H_
# define VECTOR_H_

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

#endif /* !VECTOR_H_ */
