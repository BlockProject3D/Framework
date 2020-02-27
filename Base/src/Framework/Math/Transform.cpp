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

#include "Framework/Math/Transform.hpp"

/*namespace bpf
{
    void Transform::RebuildMatrix()
    {
        //_mat = Matrix4f::Identity;
        //_mat.Scale(_scale);
        //_mat = _mat * _quat.ToMatrix();
        //_mat.Translate(_pos);
    }
    
    Vector3f Transform::WorldToLocal(const Vector3f &worldpt) const
    {
        Vector4f v(worldpt, 1.0f);
        Matrix4f worldinv = _mat.Invert();

        Vector4f res = worldinv * v;
        return (Vector3f(res.X, res.Y, res.Z));*/
        //return (worldpt);
    //}

    /*Vector3f Transform::LocalToWorld(const Vector3f &localpt) const
    {
        Vector4f v(localpt, 1.0f);

        Vector4f res = _mat * v;
        return (Vector3f(res.X, res.Y, res.Z));*/
        //return (localpt);
    //}

    /*Vector3f Transform::TransformPoint(const Vector3f &pt) const
    {
        Vector3f res = pt;

        res *= _scale;
        res = _quat.Rotate(res);
        res += _pos;
        return (res);
    }
}*/
