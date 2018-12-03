#ifndef TRIANGLE_H_
# define TRIANGLE_H_

namespace Framework
{
    class ENGINE_API FTriangle
    {
    public:
        FVertex VertA;
        FVertex VertB;
        FVertex VertC;

        FTriangle(const FVertex &p1, const FVertex &p2, const FVertex &p3);
        void Move(const FVector &offset);
        void ApplyTransform(const FTransform &tr);
    };
};

#endif /* !TRIANGLE_H_ */
