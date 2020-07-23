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
#include "Framework/Exception.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace math
    {
        /**
         * Exception thrown when operating on two matrices and/or vectors of incompatible size
         */
        class BPF_API IncompatibleMatrixSizeException final : public Exception
        {
        private:
            fisize _sizea;
            fisize _sizeb;

        public:
            /**
             * Constructs an IncompatibleMatrixSizeException
             * @param sizea the incompatible size of the first operand
             * @param sizeb the incompatible size of the second operand
             */
            inline IncompatibleMatrixSizeException(fisize sizea, fisize sizeb) noexcept
                : _sizea(sizea)
                , _sizeb(sizeb)
            {
            }

            /**
             * Returns the incompatible size of the first operand
             * @return unsigned
             */
            inline fsize SizeA() const noexcept
            {
                return (_sizea);
            }

            /**
             * Returns the incompatible size of the first operand
             * @return unsigned
             */
            inline fsize SizeB() const noexcept
            {
                return (_sizeb);
            }

            inline virtual const char *GetType() const noexcept
            {
                return ("IncompatibleMatrixSize");
            }

            void Print() const noexcept final;
        };
    }
}