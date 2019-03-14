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
#include "Framework/Types.hpp"
#include "Framework/String.hpp"

namespace bpf
{
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
         * Initializes a FColor at BLACK
         */
        inline Color()
            : R(0), G(0), B(0), A(255)
        {
        }

        /**
         * Initializes a FColor from individual components
         * @param r the r component
         * @param g the g component
         * @param b the b component
         * @param a the a component
         */
        inline Color(const uint8 r, const uint8 g, const uint8 b, const uint8 a = 255)
            : R(r), G(g), B(b), A(a)
        {
        }

        /**
         * Initializes a FColor from an RGB and an alpha
         * @param other the other FColor to copy RGB from
         * @param a the new alpha value
         */
        inline Color(const Color &other, const uint8 a)
            : R(other.R), G(other.G), B(other.B), A(a)
        {
        }
        
        /**
         * Initializes a FColor from a grey scale value
         * @param grey the grey scale value
         */
        explicit inline Color(const uint8 grey)
            : R(grey), G(grey), B(grey), A(255)
        {
        }

        /**
         * Initializes a FColor from a packed rgb int
         * @param rgb the packed rgb integer
         */
        explicit Color(const fint rgb); //TODO : Implement

        /**
         * Blends two colors by addition
         */
        Color operator+(const Color &other) const;

        /**
         * Blends two colors by multiplication
         */
        Color operator*(const Color &other) const;
        
        inline bool operator==(const Color &other)
        {
            return (R == other.R && G == other.G && B == other.B && A == other.A);
        }

        inline bool operator!=(const Color &other)
        {
            return (!(R == other.R && G == other.G && B == other.B && A == other.A));
        }

        inline String ToString() const
        {
            return (String("Color(") + R + ", " + G + ", " + B + ", " + A + ")");
        }

        /**
         * RED constant
         */
        static const Color Red;

        /**
         * GREEN constant
         */
        static const Color Green;

        /**
         * BLUE constant
         */
        static const Color Blue;

        /**
         * WHITE constant
         */
        static const Color White;

        /**
         * BLACK constant
         */
        static const Color Black;

        /**
         * YELLOW constant
         */
        static const Color Yellow;

        /**
         * CYAN constant
         */
        static const Color Cyan;
    };
};
