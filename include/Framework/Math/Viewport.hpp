#ifndef VIEWPORT_H_
# define VIEWPORT_H_

namespace Framework
{
    class ENGINE_API FViewport
    {
    public:
        /**
         * Field of view
         */
        float FOV;
        
        /**
         * Near clipping plane
         */
        float NearPlane;
        
        /**
         * Far clipping plane
         */
        float FarPlane;
        
        /**
         * Viewport width
         */
        unsigned int Width;
        
        /**
         * Viewport height
         */
        unsigned int Height;
        
        /**
         * Projection matrix
         */
        FMatrix Projection;
        
        inline FViewport()
            : FOV(0.0f), NearPlane(0.0f), FarPlane(0.0f),
            Width(0), Height(0), Projection(FMatrix::Identity)
        {
        }
    };
};

#endif /* !VIEWPORT_H_ */
