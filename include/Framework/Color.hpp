#ifndef COLOR_H_
# define COLOR_H_

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
        inline Color(const uint8 grey)
            : R(grey), G(grey), B(grey), A(255)
        {
        }

        /**
         * Initializes a FColor from a packed rgb int
         * @param rgb the packed rgb integer
         */
        Color(const int rgb); //TODO : Implement

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

#endif /* !COLOR_H_ */
