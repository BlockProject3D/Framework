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
#include "Framework/Math/Quaternion.hpp"
#include "Framework/Math/Vector.hpp"
#include "Framework/Math/Matrix.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Utility to represent a transformation in 3D space
         * @tparam T the type of number
         */
        template <typename T>
        class BP_TPL_API Transform3
        {
        public:
            /**
             * Utility to build matrices for 3D transformations in homogeneous coordinate space
             */
            class BP_TPL_API MatrixBuilder
            {
            private:
                Matrix4<T> _matrix;

            public:
                /**
                 * Constructs a MatrixBuilder from identity transform
                 */
                inline MatrixBuilder()
                    : _matrix(Matrix4<T>::Identity)
                {
                }

                /**
                 * Push a new translation to the current matrix
                 * @param translation new translation vector
                 * @return reference for chaining
                 */
                MatrixBuilder &Translate(const Vector3<T> &translation) noexcept;

                /**
                 * Push a new scale to the current matrix
                 * @param scale new scale vector
                 * @return reference for chaining
                 */
                MatrixBuilder &Scale(const Vector3<T> &scale) noexcept;

                /**
                 * Push a new shear to the current matrix
                 * @param shear shear value
                 * @return reference for chaining
                 */
                MatrixBuilder &ShearX(const Vector3<T> &shear) noexcept;

                /**
                 * Push a new shear to the current matrix
                 * @param shear shear value
                 * @return reference for chaining
                 */
                MatrixBuilder &ShearY(const Vector3<T> &shear) noexcept;

                /**
                 * Push a new shear to the current matrix
                 * @param shear shear value
                 * @return reference for chaining
                 */
                MatrixBuilder &ShearZ(const Vector3<T> &shear) noexcept;

                /**
                 * Push a new rotation to the current matrix
                 * @param axis rotation axis
                 * @param angle angle in radians
                 * @return reference for chaining
                 */
                MatrixBuilder &Rotate(const Vector3<T> &axis, T angle) noexcept;

                /**
                 * Push a new rotation to the current matrix
                 * @param quat unit quaternion
                 * @return reference for chaining
                 */
                inline MatrixBuilder &Rotate(const Quaternion<T> &quat) noexcept
                {
                    _matrix = _matrix * quat.ToMatrix();
                    return (*this);
                }

                /**
                 * Returns the built matrix
                 * @return order 4 square matrix
                 */
                inline Matrix4<T> Build() const noexcept
                {
                    return (_matrix);
                }
            };

        public:
            /**
             * Transform position
             */
            Vector3<T> Position;

            /**
             * Transform scale
             */
            Vector3<T> Scale;

            /**
             * Transform rotation as a unit quaternion
             */
            Quaternion<T> Rotation;

            /**
             * Constructs a 3D transform
             * @param pos transform position
             * @param scale transform scale
             * @param rotation transform rotation
             */
            explicit inline Transform3(const Vector3<T> &pos = Vector3<T>::Zero, const Vector3<T> &scale = Vector3<T>::Identity, const Quaternion<T> &rotation = Quaternion<T>::Identity)
                : Position(pos)
                , Scale(scale)
                , Rotation(rotation)
            {
            }

            /**
             * Transforms a point
             * @param local 3D point to transform
             * @return new transformed 3D point
             */
            Vector3<T> LocalToWorld(const Vector3<T> &local);

            /**
             * Revert transformation from a point
             * @param world 3D point to revert transformation
             * @return new de-transformed 3D point
             */
            Vector3<T> WorldToLocal(const Vector3<T> &world);

            /**
             * Computes the matrix of this transform
             * @return order 4 square matrix
             */
            inline Matrix4<T> ToMatrix() const noexcept
            {
                return (MatrixBuilder().Translate(Position).Rotate(Rotation).Scale(Scale).Build());
            }

            /**
             * Adds two transforms
             * @param other transform to add
             * @return new transform
             */
            Transform3 operator+(const Transform3 &other) const noexcept;

            /**
             * Adds a transform to this transform
             * @param other transform to add
             */
            void operator+=(const Transform3 &other);

            /**
             * Rotates this transform around a point
             * @param pivot rotation origin
             * @param rotation unit quaternion
             */
            void RotateArround(const Vector3<T> &pivot, const Quaternion<T> &rotation) noexcept;

            /**
             * Updates this transform to look at a given point
             * @param target the target point to look at
             * @param forward the world's forward vector
             * @param up the world's up vector
             */
            inline void LookAt(const Vector3<T> &target, const Vector3<T> &forward = Vector3<T>::Forward, const Vector3<T> &up = Vector3<T>::Up) noexcept
            {
                Rotation = Quaternion<T>::LookAt(target - Position, forward, up);
            }
        };

        using Transform3f = Transform3<float>;
    }
}

#include "Framework/Math/Transform3.impl.hpp"