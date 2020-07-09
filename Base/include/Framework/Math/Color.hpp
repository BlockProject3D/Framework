// Copyright (c) 2020, BlockProject 3D
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
//     * Neither the name of BlockProject 3D nor the names of its contributors
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
#include "Framework/String.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Represents a color
         */
        class BPF_API Color
        {
        public:
            /**
             * R component
             */
            uint8 R;

            /**
             * G component
             */
            uint8 G;

            /**
             * B component
             */
            uint8 B;

            /**
             * A component
             */
            uint8 A;

            /**
             * Constructs a Black Color
             */
            inline Color()
                : R(0)
                , G(0)
                , B(0)
                , A(255)
            {
            }

            /**
             * Constructs a Color from individual RGBA components
             * @param r the red component
             * @param g the green component
             * @param b the blue component
             * @param a the alpha component
             */
            inline Color(const uint8 r, const uint8 g, const uint8 b, const uint8 a = 255)
                : R(r)
                , G(g)
                , B(b)
                , A(a)
            {
            }

            /**
             * Constructs a Color
             * @param other the other Color to copy RGB components from
             * @param a the new alpha value
             */
            inline Color(const Color &other, const uint8 a)
                : R(other.R)
                , G(other.G)
                , B(other.B)
                , A(a)
            {
            }

            /**
             * Constructs a grey scale Color
             * @param grey the grey scale value
             */
            explicit inline Color(const uint8 grey)
                : R(grey)
                , G(grey)
                , B(grey)
                , A(255)
            {
            }

            /**
             * Constructs a Color from a packed rgb int
             * @param rgb the packed rgb integer
             */
            explicit Color(fint rgb); // TODO : Implement

            /**
             * Pack this color into a single integer
             * @return the packed color as integer
             */
            fint Pack() const noexcept;

            /**
             * Blends two colors by addition
             * @param other the color to blend with
             * @return new Color
             */
            Color operator+(const Color &other) const;

            /**
             * Blends two colors by multiplication
             * @param other the color to blend with
             * @return new Color
             */
            Color operator*(const Color &other) const;

            /**
             * Compare two colors
             * @param other the color to compare with
             * @return true if the two colors are equal, false otherwise
             */
            inline bool operator==(const Color &other) const
            {
                return (R == other.R && G == other.G && B == other.B && A == other.A);
            }

            /**
             * Compare two colors
             * @param other the color to compare with
             * @return false if the two colors are equal, true otherwise
             */
            inline bool operator!=(const Color &other) const
            {
                return (!(R == other.R && G == other.G && B == other.B && A == other.A));
            }

            /**
             * Normalizes this color taking only R, G and B channels
             * @return normalized RGB color as an order 3 float vector
             */
            inline Vector3f Normalize() const noexcept
            {
                return (Vector3f((float)R / 255.0f, (float)G / 255.0f, (float)B / 255.0f));
            }

            /**
             * Normalizes this color taking all channels (supports alpha)
             * @return normalized RGBA color as an order 4 float vector
             */
            inline Vector4f NormalizeAlpha() const noexcept
            {
                return (Vector4f((float)R / 255.0f, (float)G / 255.0f, (float)B / 255.0f, (float)A / 255.0f));
            }

            /**
             * The Red color
             */
            static const Color Red;

            /**
             * The Green color
             */
            static const Color Green;

            /**
             * The Blue color
             */
            static const Color Blue;

            /**
             * The White color
             */
            static const Color White;

            /**
             * The Black color
             */
            static const Color Black;

            /**
             * The Yellow color
             */
            static const Color Yellow;

            /**
             * The Cyan color
             */
            static const Color Cyan;
        };
    }
}
