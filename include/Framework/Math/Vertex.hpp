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
