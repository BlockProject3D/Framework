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
#include "Framework/String.hpp"

namespace bpf
{
    /**
     * Utility class to represent a hashed string.
     * By default this class uses the register size of the current system to store the hash number
     */
    class BPF_API Name
    {
    private:
        fsize _hash;

    public:

        /**
         * Initialize an empty Name
         */
        inline Name() noexcept
            : _hash(0)
        {
        }

        /**
         * Construct a Name from a low-level string
         * @param str the low-level null-terminated c-string to build a hash from
         */
        explicit inline Name(const char *str) noexcept
#ifdef PLATFORM_64
        : _hash(Hash64(str))
#else
        : _hash(Hash32(str))
#endif
        {
        }

        /**
         * Constructs a Name from a high-level string
         * @param str the high-level string to build a hash from
         */
        explicit inline Name(const String &str) noexcept
#ifdef PLATFORM_64
            : _hash(Hash64(str))
#else
            : _hash(Hash32(str))
#endif
        {
        }

        /**
         * Constructs a Name from an existing hash
         * @param hash the hash to copy
         */        
        explicit inline Name(fsize hash) noexcept
            : _hash(hash)
        {
        }

        /**
         * Returns the hash value
         * @return the hash value for the current platform register size
         */
        inline fsize Hash() const noexcept
        {
            return (_hash);
        }

        /**
         * Computes 32 bits string hash
         * @param str string to hash
         * @return 32 bits unsigned
         */
        static uint32 Hash32(const char *str) noexcept;

        /**
         * Computes 32 bits string hash
         * @param str high-level string to hash
         * @return 32 bits unsigned
         */
        static uint32 Hash32(const String &str) noexcept;

        /**
         * Computes 64 bits string hash
         * @param str string to hash
         * @return 64 bits unsigned
         */
        static uint64 Hash64(const char *str) noexcept;

        /**
         * Computes 64 bits string hash
         * @param str high-level string to hash
         * @return 64 bits unsigned
         */
        static uint64 Hash64(const String &str) noexcept;

        /**
         * Compare Name
         * @return true if the two Names are equal false otherwise
         */
        inline bool operator==(const Name &other) const noexcept
        {
            return (_hash == other._hash);
        }

        /**
         * Compare Name
         * @return false if the two Names are equal true otherwise
         */
        inline bool operator!=(const Name &other) const noexcept
        {
            return (_hash != other._hash);
        }
    };
}
