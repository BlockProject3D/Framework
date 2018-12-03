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
        inline static float Clamp(const float val, const float min, const float max)
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
