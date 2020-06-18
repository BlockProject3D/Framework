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
#include "Framework/IO/File.hpp"

namespace bpf
{
    namespace system
    {
        /**
         * Class to represent various path for the application to work with
         */
        class BPF_API Paths
        {
        public:
            /**
             * The application root folder
             * WARNING: In certain cases this path may be read-only
             */
            const io::File AppRoot;

            /**
             * The application data directory, this directory can be assumed always read-write
             */
            const io::File DataDir;

            /**
             * System defined user's home directory
             * WARNING: Restricive-by-design systems may not allow accessing this folder
             */
            const io::File UserHome;

            /**
             * System defined temporary directory (usually cleared after system reboot)
             */
            const io::File TempDir;

            /**
             * The location for third party dependencies (will be used as fallback to search for non-system dependencies)
             */
            const io::File ThirdParty;

            /**
             * Application's persistent cache directory
             */
            const io::File CacheDir;

            /**
             * Constructs a set of application paths
             * @param root the application root directory (may be read-only)
             * @param data the application data directory (must be read-write)
             * @param home the user's home directory (access not defined on restrictive-by-design systems)
             * @param tmp the system temporary directory for that application (must be read-write)
             */
            Paths(const io::File &root, const io::File &data, const io::File &home, const io::File &tmp);
        };
    }
};