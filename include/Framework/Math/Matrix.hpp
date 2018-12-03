#ifndef MATRIX_H_
# define MATRIX_H_

# include <initializer_list>

namespace Framework
{
    enum ERotationAxis
    {
        ROT_AXIS_X,
        ROT_AXIS_Y,
        ROT_AXIS_Z
    };

    class ENGINE_API FMatrix
    {
    private:
        float Data[16];
    public:
        FMatrix();
        FMatrix(std::initializer_list<float> lst);
        FMatrix(const FMatrix &other);
        FMatrix(const float *mat);
        FMatrix &operator=(const FMatrix &other);
        void Invert();
        void Translate(const FVector &v);
        void Rotate(const ERotationAxis axis, const float ang);
        void RotateYaw(const float ang);
        void RotatePitch(const float ang);
        void Scale(const FVector &v);
        void Set(const int i, const int j, const float d);
        float Get(const int i, const int j) const;
        FMatrix operator*(const FMatrix &other) const;
        inline const float *operator*() const
        {
            return (Data);
        }
      
        static FMatrix Identity;
        static FMatrix Zero;
    };
};

#endif /* !MATRIX_H_ */
