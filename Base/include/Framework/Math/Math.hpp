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
#include <cmath>
#include "Framework/Types.hpp"

namespace bpf
{
    namespace math
    {
        namespace __internal_math
        {
            template <typename T>
            inline constexpr T GetEpsilon()
            {
                return ((T)0);
            }

            template <>
            inline constexpr float GetEpsilon()
            {
                return (0.00001f);
            }

            template <>
            inline constexpr double GetEpsilon()
            {
                return (0.0000001);
            }

            template <>
            inline constexpr long double GetEpsilon()
            {
                return (0.0000001);
            }
        }

        template <typename T>
        class BP_TPL_API Math
        {
        public:
            /**
             * Radians to degrees multiplier constant
             */
            static constexpr T RadToDeg = T(57.2957795);

            /**
             * Degrees to radians multiplier constant
             */
            static constexpr T DegToRad = T(0.0174533);

            /**
             * Pi constant
             */
            static constexpr T Pi = T(3.141592654);

            /**
             * 2 * Pi constant
             */
            static constexpr T TwoPi = T(2) * Pi;

            /**
             * Returns a very small value to represent acceptable rounding errors for various types
             */
            static constexpr T Epsilon = __internal_math::GetEpsilon<T>();

            /**
             * Absolute value
             * @param val value to convert to absolute value
             */
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
            inline static T Lerp(const T a, const T b, const T alpha)
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
            inline static T Map(const T val, const T srcmin, const T srcmax, const T dstmin, const T dstmax)
            {
                return ((val - srcmin) * (dstmax - dstmin) / (srcmax - srcmin) + dstmin);
            }

            /**
             * Clamps a value between a min and a max value
             * @param val the value to map
             * @param min the minimum
             * @param max the maximum
             */
            inline static T Clamp(const T val, const T min, const T max)
            {
                return ((val < min) ? min : (val > max) ? max : val);
            }

            /**
             * Rounds a value at a defined precision
             * @param val the value to round
             * @param prec the number of decimals
             */
            inline static T Round(const T val, const fsize prec = 0)
            {
                return ((T)(floor(val * pow(10, prec) + 0.5) / pow(10, prec)));
            }

            /**
             * Shortcut to the system's square root function
             * @param val number to calculate square root of
             */
            inline static T Sqrt(const T val)
            {
                return ((T)sqrt(val));
            }

            /**
             * Shortcut to the system's exponential function
             * @param val number for e power val
             */
            inline static T Exp(const T val)
            {
                return ((T)exp(val));
            }

            /**
             * Shortcut to the system's natural logarithm function
             * @param val number for Ln(val)
             */
            inline static T Log(const T val)
            {
                return ((T)log(val));
            }

            /**
             * Shortcut to the system's logarithm base 10 function
             * @param val number for Log(val)
             */
            inline static T Log10(const T val)
            {
                return ((T)log10(val));
            }

            /**
             * Shortcut to calculate log base n
             * @param val number for Log(val)
             * @param base the logarythm base to use
             */
            inline static T Log(const T base, const T val)
            {
                return (Log(val) / Log(base));
            }

            inline static T Cos(const T val)
            {
                return ((T)cos(val));
            }

            inline static T Sin(const T val)
            {
                return ((T)sin(val));
            }

            inline static T Tan(const T val)
            {
                return ((T)tan(val));
            }

            inline static T ArcCos(const T val)
            {
                return ((T)acos(val));
            }

            inline static T ArcSin(const T val)
            {
                return ((T)asin(val));
            }

            inline static T ArcTan(const T val)
            {
                return ((T)atan(val));
            }

            inline static T ArcTan2(const T y, const T x)
            {
                return ((T)atan2(y, x));
            }
        };

        template <typename T>
        constexpr T Math<T>::RadToDeg;
        template <typename T>
        constexpr T Math<T>::DegToRad;
        template <typename T>
        constexpr T Math<T>::Pi;
        template <typename T>
        constexpr T Math<T>::Epsilon;

        template <>
        inline float Math<float>::Sqrt(const float val)
        {
            return (sqrtf(val));
        }

        template <>
        inline long double Math<long double>::Sqrt(const long double val)
        {
            return (sqrtl(val));
        }

        template <>
        inline float Math<float>::Exp(const float val)
        {
            return (expf(val));
        }

        template <>
        inline long double Math<long double>::Exp(const long double val)
        {
            return (expl(val));
        }

        template <>
        inline float Math<float>::Log(const float val)
        {
            return (logf(val));
        }

        template <>
        inline long double Math<long double>::Log(const long double val)
        {
            return (logl(val));
        }

        template <>
        inline float Math<float>::Log10(const float val)
        {
            return (log10f(val));
        }

        template <>
        inline long double Math<long double>::Log10(const long double val)
        {
            return (log10l(val));
        }

        template <>
        inline float Math<float>::Cos(const float val)
        {
            return (cosf(val));
        }

        template <>
        inline long double Math<long double>::Cos(const long double val)
        {
            return (cosl(val));
        }

        template <>
        inline float Math<float>::Sin(const float val)
        {
            return (sinf(val));
        }

        template <>
        inline long double Math<long double>::Sin(const long double val)
        {
            return (sinl(val));
        }

        template <>
        inline float Math<float>::Tan(const float val)
        {
            return (tanf(val));
        }

        template <>
        inline long double Math<long double>::Tan(const long double val)
        {
            return (tanl(val));
        }

        template <>
        inline float Math<float>::ArcCos(const float val)
        {
            return (acosf(val));
        }

        template <>
        inline long double Math<long double>::ArcCos(const long double val)
        {
            return (acosl(val));
        }

        template <>
        inline float Math<float>::ArcSin(const float val)
        {
            return (asinf(val));
        }

        template <>
        inline long double Math<long double>::ArcSin(const long double val)
        {
            return (asinl(val));
        }

        template <>
        inline float Math<float>::ArcTan(const float val)
        {
            return (atanf(val));
        }

        template <>
        inline long double Math<long double>::ArcTan(const long double val)
        {
            return (atanl(val));
        }

        template <>
        inline float Math<float>::ArcTan2(const float y, const float x)
        {
            return (atan2f(y, x));
        }

        template <>
        inline long double Math<long double>::ArcTan2(const long double y, const long double x)
        {
            return (atan2l(y, x));
        }

        using Mathf = Math<float>;
        using Mathd = Math<double>;
        using Mathld = Math<long double>;
    }
}
