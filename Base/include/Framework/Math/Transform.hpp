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
#include "Framework/Math/Vector.hpp"
#include "Framework/Math/Matrix.hpp"
#include "Framework/Math/Quaternion.hpp"

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
        inline Transform()
            : _pos(Vector3f::Zero)
            , _scale(Vector3f::Unit)
            , _quat(Quatf::Identity)
            , _mat(Matrix4f::Identity)
        {
        }
        inline Transform(const Transform &other)
            : _pos(other._pos)
            , _scale(other._scale)
            , _quat(other._quat)
        {
            RebuildMatrix();
        }
        explicit inline Transform(const Vector3f &pos, const Vector3f &scale = Vector3f::Unit,
                         const Quatf &q = Quatf::Identity)
            : _pos(pos)
            , _scale(scale)
            , _quat(q)
        {
        }

        Vector3f TransformPoint(const Vector3f &pt) const;
        
        Vector3f WorldToLocal(const Vector3f &worldpt) const;
        Vector3f LocalToWorld(const Vector3f &localpt) const;

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
