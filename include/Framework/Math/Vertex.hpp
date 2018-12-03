#ifndef VERTEX_H_
# define VERTEX_H_

# include "Framework/API.hpp"
# include "Framework/Math/Vector.hpp"

namespace Framework
{
    /**
     * Standard vertex class (Position, Normal, UV
     */
    class ENGINE_API FVertex
    {
    public:
        FVertex(const float x, const float y, const float z, const float u, const float v);
        FVertex(const FVector &pos, const FVector &normal, float u, float v);
        FVertex();
        FVertex(const FVertex &other);

        /**
         * Applies a transformation to both the position and normal of that vertex
         * @param tr the transform to apply
         */
        void ApplyTransform(const FTransform &tr);

        /**
         * Sets the normal of that vertex
         * @param vec the new normal for that vertex
         */
        void SetNormal(const FVector &vec);

        bool operator==(const FVertex &other);

        FVertex operator-();
        
        /**
         * X coord
         */
        float X;
        
        /**
         * Y coord
         */
        float Y;
        
        /**
         * Z coord
         */
        float Z;
        
        /**
         * U texture coord
         */
        float U;
        
        /**
         * V texture coord
         */
        float V;

        /**
         * Normal X coord
         */
        float NX;

        /**
         * Normal Y coord
         */
        float NY;
        
        /**
         * Normal Z coord
         */
        float NZ;
    };
};

#endif /* !VERTEX_H_ */
