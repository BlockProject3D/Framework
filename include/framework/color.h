#ifndef COLOR_H_
# define COLOR_H_

namespace Framework
{
    class ENGINE_API FColor
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
        inline FColor()
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
        inline FColor(const uint8 r, const uint8 g, const uint8 b, const uint8 a = 255)
            : R(r), G(g), B(b), A(a)
        {
        }

        /**
         * Initializes a FColor from an RGB and an alpha
         * @param other the other FColor to copy RGB from
         * @param a the new alpha value
         */
        inline FColor(const FColor &other, const uint8 a)
            : R(other.R), G(other.G), B(other.B), A(a)
        {
        }
        
        /**
         * Initializes a FColor from a grey scale value
         * @param grey the grey scale value
         */
        inline FColor(const uint8 grey)
            : R(grey), G(grey), B(grey), A(255)
        {
        }

        /**
         * Initializes a FColor from a packed rgb int
         * @param rgb the packed rgb integer
         */
        FColor(const int rgb); //TODO : Implement

        /**
         * Blends two colors by addition
         */
        FColor operator+(const FColor &other) const;

        /**
         * Blends two colors by multiplication
         */
        FColor operator*(const FColor &other) const;
        
        inline bool operator==(const FColor &other)
        {
            return (R == other.R && G == other.G && B == other.B && A == other.A);
        }

        inline bool operator!=(const FColor &other)
        {
            return (!(R == other.R && G == other.G && B == other.B && A == other.A));
        }

        inline FString ToString() const
        {
            return (FString("Color(") + R + ", " + G + ", " + B + ", " + A + ")");
        }

        /**
         * RED constant
         */
        static const FColor Red;

        /**
         * GREEN constant
         */
        static const FColor Green;

        /**
         * BLUE constant
         */
        static const FColor Blue;

        /**
         * WHITE constant
         */
        static const FColor White;

        /**
         * BLACK constant
         */
        static const FColor Black;

        /**
         * YELLOW constant
         */
        static const FColor Yellow;

        /**
         * CYAN constant
         */
        static const FColor Cyan;
    };
};

#endif /* !COLOR_H_ */
