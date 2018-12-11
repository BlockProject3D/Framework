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

#ifndef BMATH_H_
# define BMATH_H_

# include <cmath>
# include "Framework/API.hpp"
# include "Framework/Math/Vector.hpp"
# include "Framework/Math/Matrix.hpp"
# include "Framework/Math/Quaternion.hpp"
# include "Framework/Math/Transform.hpp"
# include "Framework/Math/Vertex.hpp"
# include "Framework/Math/Triangle.hpp"
# include "Framework/Math/Viewport.hpp"

namespace Framework
{
    class ENGINE_API FMath
    {
    public:
        /**
         * Radians to degrees multiplier constant
         */
        static constexpr float RadToDeg = 57.2957795f;

        /**
         * Degrees to radians multiplier constant
         */
        static constexpr float DegToRad = 0.0174533f;

        /**
         * PI constant
         */
        static constexpr float PI = 3.141592654f;

        template <typename T>
        inline static T Abs(T val)
        {
            return ((val < 0) ? (-val) : (val));
        }

        /**
         * Performs a linear interpolation
         * @param a the initial value
         * @param b the target value
         * @param alpha interpolation float
         */
        inline static float Lerp(const float a, const float b, const float alpha)
        {
            return ((a * (1.0f - alpha)) + (b * alpha));
        }

        /**
         * Maps a value between a min and a max value
         * @param val the value to map
         * @param srcmin the source minimum
         * @param srcmax the source maximum
         * @param dstmin the target minimum
         * @param dstmax the target maximum
         */
        inline static float Map(const float val, const float srcmin, const float srcmax, const float dstmin, const float dstmax)
        {
            return ((val - srcmin) * (dstmax - dstmin) / (srcmax - srcmin) + dstmin);
        }
      
        /**
         * Clamps a value between a min and a max value
         * @param val the value to map
         * @param min the minimum
         * @param max the maximum
         */
        template <typename T>
        inline static T Clamp(const T val, const T min, const T max)
        {
            return ((val < min) ? min : (val > max) ? max : val);
        }

        /**
         * Rounds a value at a defined precision
         * @param val the value to round
         * @param prec the number of decimals
         */
        inline static float Round(const float val, const int prec = 0)
        {
            return ((float)(floor(val * pow(10, prec) + 0.5) / pow(10, prec)));
        }

        /**
         * Creates a perspective projection matrix
         * @param fovy the field of view in the Y direction
         * @param aspect aspect ratio
         * @param znear near clipping plane
         * @param zfar far clipping plane
         */
        static FMatrix PerspectiveProjectionMatrix(const float fovy, const float aspect, const float znear, const float zfar);

        /**
         * Creates a projection to draw 2D elements
         * @param w half the width of the target viewport
         * @param h half the height of the target viewport
         */
        static FMatrix ScreenProjectionMatrix(const float w, const float h);

        /**
         * Creates a rotation matrix for a given axis
         * @param out output matrix
         * @param axis the axis the rotation will be on
         * @param angle the angle of the rotation
         */
        static void RotationMatrix(FMatrix &out, const ERotationAxis axis, const float angle);

        /**
         * Returns true if a given number is prime
         * @param n the number to check
         */
        static bool IsPrime(const int n);

        /**
         * Finds the next prime starting at n + 1
         * @param n number to start at
         */
        static int FindNextPrime(const int n);
    };
};

#endif /* !BMATH_H_ */
