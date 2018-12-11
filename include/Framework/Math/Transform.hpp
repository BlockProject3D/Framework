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
