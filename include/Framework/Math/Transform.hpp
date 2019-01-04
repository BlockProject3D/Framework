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

namespace bpf
{
    class BPF_API Transform
    {
    private:
        Vector3f _pos;
        Vector3f _scale;
        Quatf _quat;
        Matrix4f _mat;
        
        void RebuildMatrix();

    public:
        Transform();
        Transform(const Transform &other)
            : _pos(other._pos)
            , _scale(other._scale)
            , _quat(other._quat)
        {
            RebuildMatrix();
        }
        Transform(const Vector3f &pos, const Vector3f &scale = Vector3f::Unit,
            const Quatf &q = Quatf::Identity)
            : _pos(pos)
            , _scale(scale)
            , _quat(q)
        {
        }
        
        Vector3f WorldToLocal(const Vector3f &worldpt);
        Vector3f LocalToWorld(const Vector3f &localpt);

        inline Transform &Move(const Vector3f &vec)
        {
            _pos += vec;
            return (*this);
        }
        
        inline Transform &Scale(const Vector3f &scale)
        {
            _scale *= scale;
            return (*this);
        }
        
        inline Transform &SetPos(const Vector3f &pos)
        {
            _pos = pos;
            return (*this);
        }
        
        inline Transform &SetScale(const Vector3f &scale)
        {
            _scale = scale;
            return (*this);
        }
        
        inline Transform &SetQuat(const Quatf &q)
        {
            _quat = q;
            return (*this);
        }
        
        inline const Vector3f &GetPos() const
        {
            return (_pos);
        }

        inline const Vector3f &GetScale() const
        {
            return (_scale);
        }

        inline const Quatf &GetQuat() const
        {
            return (_quat);
        }
        
        inline Transform operator+(const Transform &other) const
        {
            return (Transform(_pos + other._pos,
                              _scale * other._scale,
                              _quat * other._quat));
        }
        
        inline void operator+=(const Transform &other)
        {
            _pos += other._pos;
            _scale *= other._scale;
            _quat = _quat * other._quat;
        }

        inline bool operator==(const Transform &other) const
        {
            return (_pos == other._pos && _quat == other._quat && _scale == other._scale);
        }
    };
}

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
