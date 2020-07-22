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
#include "Framework/String.hpp"

namespace bpf
{
    /**
     * High-level exception
     */
    class BPF_API RuntimeException : public Exception
    {
    private:
        String *_type;
        String *_message;
        int *_refs;

    public:
        ~RuntimeException() override;

        /**
         * Constructs a high-level exception
         * @param type exception type name suffixed with "Exception"
         * @param message exception message string
         */
        RuntimeException(const String &type, const String &message);

        /**
         * Copy constructor
         */
        RuntimeException(const RuntimeException &other) noexcept;

        /**
         * Copy assignment operator
         */
        RuntimeException &operator=(const RuntimeException &other) noexcept;

        /**
         * Returns the exception's message
         */
        inline const String &Message() const noexcept
        {
            return (*_message);
        }

        inline const char *Type() const noexcept override
        {
            return (**_type);
        }

        void Print() const noexcept override;
    };
}
