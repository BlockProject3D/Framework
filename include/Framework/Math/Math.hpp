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
    class BPF_API Math
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
         * Pi constant
         */
        static constexpr float Pi = 3.141592654f;

        /**
         * Absolute value
         * @param val value to convert to absolute value
         */
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
        template <typename T>
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
        template <typename T>
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
        template <typename T>
        inline static T Round(const T val, const int prec = 0)
        {
            return ((T)(floor(val * pow(10, prec) + 0.5) / pow(10, prec)));
        }

        /**
         * Returns true if nb is a power of two
         * @param nb the number to check
         */
        inline static bool IsPowerOfTwo(const uint32 nb)
        {
            return (nb & (nb - 1)) == 0;
        }

        /**
         * Finds the next power of two starting at nb + 1
         * @param nb the number to find the next power of two
         */
        static uint32 FindNextPowerOfTwo(uint32 nb);

        /**
         * Returns true if a given number is prime
         * @param nb the number to check
         */
        static bool IsPrime(const int nb);

        /**
         * Finds the next prime number starting at nb + 1
         * @param nb the number to find the next prime
         */
        static int FindNextPrime(const int nb);

        /**
         * Shortcut to the system's square root function
         * @param val number to calculate square root of
         */
        template <typename T>
        inline static T Sqrt(const T val)
        {
            return (sqrt(val));
        }

        /**
         * Shortcut to the system's exponential function
         * @param val number for e power val
         */
        template <typename T>
        inline static T Exp(const T val)
        {
            return (exp(val));
        }

        /**
         * Shortcut to the system's natural logarithm function
         * @param val number for Ln(val)
         */
        template <typename T>
        inline static T Ln(const T val)
        {
            return (log(val));
        }

        /**
         * Shortcut to the system's logarithm base 10 function
         * @param val number for Log(val)
         */
        template <typename T>
        inline static T Log(const T val)
        {
            return (log10(val));
        }

        /**
         * Returns the inverse square root of a floating point number
         * Uses the Fast Inverse Square Root
         * @param nb the number to find the inverse square root
         */
        static float InvSqrt(const float nb);

        template <typename T>
        inline static T Cos(const T val)
        {
            return (cos(val));
        }

        template <typename T>
        inline static T Sin(const T val)
        {
            return (sin(val));
        }

        template <typename T>
        inline static T Tan(const T val)
        {
            return (tan(val));
        }

        template <typename T>
        inline static T ArcCos(const T val)
        {
            return (acos(val));
        }

        template <typename T>
        inline static T ArcSin(const T val)
        {
            return (asin(val));
        }

        template <typename T>
        inline static T ArcTan(const T val)
        {
            return (atan(val));
        }

        template <typename T>
        inline static T ArcTan2(const T y, const T x)
        {
            return (atan2(y, x));
        }
    };

    template <>
    inline float Math::Sqrt(const float val)
    {
        return (sqrtf(val));
    }

    template <>
    inline long double Math::Sqrt(const long double val)
    {
        return (sqrtl(val));
    }

    template <>
    inline float Math::Exp(const float val)
    {
        return (expf(val));
    }

    template <>
    inline long double Math::Exp(const long double val)
    {
        return (expl(val));
    }

    template <>
    inline float Math::Ln(const float val)
    {
        return (logf(val));
    }

    template <>
    inline long double Math::Ln(const long double val)
    {
        return (logl(val));
    }

    template <>
    inline float Math::Log(const float val)
    {
        return (log10f(val));
    }

    template <>
    inline long double Math::Log(const long double val)
    {
        return (log10l(val));
    }

    template <>
    inline float Math::Cos(const float val)
    {
        return (cosf(val));
    }

    template <>
    inline long double Math::Cos(const long double val)
    {
        return (cosl(val));
    }

    template <>
    inline float Math::Sin(const float val)
    {
        return (sinf(val));
    }

    template <>
    inline long double Math::Sin(const long double val)
    {
        return (sinl(val));
    }

    template <>
    inline float Math::Tan(const float val)
    {
        return (tanf(val));
    }

    template <>
    inline long double Math::Tan(const long double val)
    {
        return (tanl(val));
    }

    template <>
    inline float Math::ArcCos(const float val)
    {
        return (acosf(val));
    }

    template <>
    inline long double Math::ArcCos(const long double val)
    {
        return (acosl(val));
    }

    template <>
    inline float Math::ArcSin(const float val)
    {
        return (asinf(val));
    }

    template <>
    inline long double Math::ArcSin(const long double val)
    {
        return (asinl(val));
    }

    template <>
    inline float Math::ArcTan(const float val)
    {
        return (atanf(val));
    }

    template <>
    inline long double Math::ArcTan(const long double val)
    {
        return (atanl(val));
    }

    template <>
    inline float Math::ArcTan2(const float y, const float x)
    {
        return (atan2f(y, x));
    }

    template <>
    inline long double Math::ArcTan2(const long double y, const long double x)
    {
        return (atan2l(y, x));
    }
}
