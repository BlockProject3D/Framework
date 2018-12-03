#ifndef TRANSFORM_H_
# define TRANSFORM_H_

namespace Framework
{
    class ENGINE_API FTransform
    {
    private:
        FVector PosVec;
        FVector ScaleVec;
        FQuat Rot;
        FMatrix Matrix;

        void RebuildMatrix();
    public:
        FTransform();
        FTransform(const FTransform &other);
        FTransform(const FVector &pos, const FVector &scale, const FQuat &rot);
        FTransform(const FVector &pos, const FVector &scale);
        FTransform(const FVector &pos);

        FVector WorldToLocal(const FVector &vec);

        FVector LocalToWorld(const FVector &vec);

        inline void RotateLocal(const FQuat &q)
        {
            Rot = q * Rot;
            RebuildMatrix();
        }

        inline void RotateWorld(const FQuat &q)
        {
            Rot = Rot * q;
            RebuildMatrix();
        }

        inline void Move(const FVector &vec)
        {
            PosVec += vec;
            RebuildMatrix();
        }

        inline void Scale(const FVector &vec)
        {
            ScaleVec *= vec;
            RebuildMatrix();
        }

        FTransform operator+(const FTransform &other) const;

        inline bool operator==(const FTransform &other) const
        {
            return (PosVec == other.PosVec && Rot == other.Rot && ScaleVec == other.ScaleVec);
        }
        
        inline void SetQuat(const FQuat &quat)
        {
            Rot = quat;
            RebuildMatrix();
        }

        inline void SetPos(const FVector &vec)
        {
            PosVec = vec;
            RebuildMatrix();
        }
        
        inline void SetScale(const FVector &vec)
        {
            ScaleVec = vec;
            RebuildMatrix();
        }
        
        inline const FQuat &GetQuat() const
        {
            return (Rot);
        }
        
        inline const FVector &GetPos() const
        {
            return (PosVec);
        }
      
        inline const FVector &GetScale() const
        {
            return (ScaleVec);
        }
        
        inline const FMatrix &GetMatrix() const
        {
            return (Matrix);
        }
    };
};

#endif /* !TRANSFORM_H_ */
