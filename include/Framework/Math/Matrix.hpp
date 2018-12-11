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
