#ifndef QUATERNION_H_
# define QUATERNION_H_

namespace Framework
{
    class ENGINE_API FQuat
    {
    private:
        float X;
        float Y;
        float Z;
        float W;
    public:
        /**
         * Constructs an identity quaternion (zero rotation)
         */
        FQuat();
      
        /**
         * Constructs a quaternion by specifying directly components
         */
        FQuat(const float w, const float x, const float y, const float z);

        /**
         * Constructs a quaternion from euler angles (X Pitch, Y Yaw, Z Roll)
         */
        FQuat(const FVector &eulerangles);

        /**
         * Constructs a quaternion from shortest arc between vector from and to
         */
        FQuat(const FVector &from, const FVector &to);
      
        /**
         * Constructs a quaternion from an axis and a rotation around that axis
         */
        FQuat(const FVector &axis, const float rot);
      
        FQuat(const FQuat &other);
      
        /**
         * Returns the Up pointing vector of that quaternion
         */
        inline FVector Up() const
        {
            return (FVector::Up * *this);
            //return (FVector(-2 * (X * Y - W * Z), 1 - 2 * (X * X + Z * Z), 2 * (Y * Z + W * X)));
        }
      
        /**
         * Returns the forward pointing vector of that quaternion
         */
        inline FVector Forward() const
        {
            return (FVector::Forward * *this);
            //return (FVector(2 * (X * Z - W * Y), 2 * (Y * Z + W * X), 1 - 2 * (X * X + Y * Y)));
        }
      
        /**
         * Returns the right pointing vector of that quaternion
         */
        inline FVector Right() const
        {
            return (FVector::Right * *this);
            //return (FVector(1 - 2 * (Y * Y + Z * Z), 2 * (Y * Z - W * X), 2 * (X * Z + W * Y)));
        }

        /**
         * Returns a rotation matrix from that quaternion
         */
        FMatrix ToMatrix() const;
      
        /**
         * Conjugates that quaternions, that means invert it
         */
        inline FQuat Conjugate() const
        {
            return (FQuat(W, -X, -Y, -Z));
        }
      
        /**
         * Returns true if this quaternion is equal to other
         */
        bool operator==(const FQuat &other) const;
      
        /**
         * Normalizes this quaternion
         */
        void Normalize();
      
        FQuat &operator=(const FQuat &other);

        /**
         * Multiplies two quaternions, returns the cumulation of the rotation of this and other
         */
        FQuat operator*(const FQuat &other) const;

        /**
         * Returns a copy of this quaternion raised to the power of n
         */
        FQuat operator*(const float n) const;
      
        FQuat Exp() const;
        FQuat Ln() const;
      
        /**
         * Returns the angle in degrees between two quaternions
         */
        float Angle(const FQuat &other) const;
      
        /**
         * Returns the euler angles representation of that quaternion
         */
        FVector ToEulerAngles() const;

        /**
         * Returns the axis of that quaternion
         */
        inline FVector GetAxis() const
        {
            return (FVector(X, Y, Z));
        }

        /**
         * Returns the W component of that quaternion
         */
        inline float GetW() const
        {
            return (W);
        }
        
        /**
         * Linear Interpolation between two quaternions
         */
        static FQuat Lerp(const FQuat &q, const FQuat &q1, const float t);
      
        /**
         * Spherical linear interpolation between two quaternions (WARNING : produces garbage after short period of animation time)
         */
        static FQuat Slerp(const FQuat &q, const FQuat &q1, const float t);

        /**
         * Zero quaternion
         */
        static const FQuat	Zero;
        
        /**
         * Identity quaternion
         */
        static const FQuat	Identity;
    };
};

#endif /* !QUATERNION_H_ */
