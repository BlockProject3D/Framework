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
#include "Framework/Math/Matrix.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Utility to represent a transformation in 2D space
         * @tparam T the type of number
         */
        template <typename T>
        class BP_TPL_API Transform2
        {
        public:
            /**
             * Utility to build matrices for 2D transformations in homogeneous coordinate space
             */
            class BP_TPL_API MatrixBuilder
            {
            private:
                Matrix3<T> _matrix;

            public:
                /**
                 * Constructs a MatrixBuilder from identity transform
                 */
                inline MatrixBuilder()
                    : _matrix(Matrix3<T>::Identity)
                {
                }

                /**
                 * Push a new translation to the current matrix
                 * @param translation new translation vector
                 * @return reference for chaining
                 */
                MatrixBuilder &Translate(const Vector2<T> &translation) noexcept;

                /**
                 * Push a new scale to the current matrix
                 * @param scale new scale vector
                 * @return reference for chaining
                 */
                MatrixBuilder &Scale(const Vector2<T> &scale) noexcept;

                /**
                 * Push a new shear to the current matrix
                 * @param shear shear value
                 * @return reference for chaining
                 */
                MatrixBuilder &ShearX(const T &shear) noexcept;

                /**
                 * Push a new shear to the current matrix
                 * @param shear shear value
                 * @return reference for chaining
                 */
                MatrixBuilder &ShearY(const T &shear) noexcept;

                /**
                 * Push a new rotation to the current matrix
                 * @param rotation angle in radians
                 * @return reference for chaining
                 */
                MatrixBuilder &Rotate(const T &rotation) noexcept;

                /**
                 * Returns the built matrix
                 * @return order 3 square matrix
                 */
                inline Matrix3<T> Build() const noexcept
                {
                    return (_matrix);
                }
            };

        public:
            /**
             * Transform position
             */
            Vector2<T> Position;

            /**
             * Transform scale
             */
            Vector2<T> Scale;

            /**
             * Transform rotation
             */
            T Rotation;

            /**
             * Constructs a 2D transform
             * @param pos transform position
             * @param scale transform scale
             * @param rotation transform rotation
             */
            explicit inline Transform2(const Vector2<T> &pos = Vector2<T>::Zero, const Vector2<T> &scale = Vector2<T>::Identity, const T &rotation = 0)
                : Position(pos)
                , Scale(scale)
                , Rotation(rotation)
            {
            }

            /**
             * Transforms a point
             * @param local 2D point to transform
             * @return new transformed 2D point
             */
            Vector2<T> LocalToWorld(const Vector2<T> &local);

            /**
             * Revert transformation from a point
             * @param world 2D point to revert transformation
             * @return new de-transformed 2D point
             */
            Vector2<T> WorldToLocal(const Vector2<T> &world);

            /**
             * Computes the matrix of this transform
             * @return order 3 square matrix
             */
            inline Matrix3<T> ToMatrix() const noexcept
            {
                return (MatrixBuilder().Translate(Position).Rotate(Rotation).Scale(Scale).Build());
            }

            /**
             * Rotates this transform around a point
             * @param pivot rotation origin
             * @param rotation angle in radians
             */
            void RotateArround(const Vector2<T> &pivot, const T &rotation);

            /**
             * Adds two transforms
             * @param other transform to add
             * @return new transform
             */
            Transform2 operator+(const Transform2 &other) const noexcept;

            /**
             * Adds a transform to this transform
             * @param other transform to add
             */
            void operator+=(const Transform2 &other);
        };

        using Transform2f = Transform2<float>;
    }
}

#include "Framework/Math/Transform2.impl.hpp"