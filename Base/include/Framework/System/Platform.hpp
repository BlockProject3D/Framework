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
    namespace system
    {
        /**
         * Enumeration for endianess, used with Platform::GetEndianess
         */
        enum EPlatformEndianess
        {
            PLATFORM_BIGENDIAN,
            PLATFORM_LITTLEENDIAN
        };

        /**
         * OS specs structure
         */
        struct OS
        {
            /**
             * Name of operating system
             */
            String Name;

            /**
             * Version of operating system
             */
            String Version;

            /**
             * Extension of a dynamic library
             */
            String ModuleExt;

            /**
             * File path separator character
             */
            String PathSep;

            /**
             * New-line character
             */
            String NewLine;
        };

        /**
         * CPU specs structure
         */
        struct CPU
        {
            /**
             * Brand name.
             * In release builds, a generic brand name is populated under Linux and OSX to prevent a bug in GCC itself
             * On ARM based architectures, a generic brand name is populated as ARM instruction set does not provide any
             * way to know the CPU brand name
             */
            String Name;

            /**
             * Number of cores (may not be accurate on all systems)
             */
            fint NumCores;

            /**
             * CPU frequency (may not be accurate on all systems)
             */
            fint Freq;
        };

        /**
         * RAM specs structure
         */
        struct RAM
        {
            /**
             * Maximum physical RAM installed (may not be accurate on all systems)
             */
            uint64 MaxPhysical;

            /**
             * Maximum virtual RAM usable
             */
            uint64 MaxVirtual;
        };

        /**
         * Environment specs structure
         */
        struct Env
        {
            /**
             * Short name of the Framework
             */
            String ShortName;

            /**
             * Long name of the Framework
             */
            String Name;

            /**
             * Version string of the Framework (used for display)
             */
            String Version;

            /**
             * Version integer of the Framework (used for lading plugins)
             */
            fint VersionInt;
        };

        /**
         * Utility class to represent the current system specs and environment specs.
         * Also provides access to various low-level platform-dependent utilities
         */
        class BPF_API Platform
        {
        private:
            static Env InitEnvInfo();
            static OS InitOSInfo();
            static String IdentifyCPUBranding();
            static String CPUIDIntToStr(fint data);

        public:
            /**
             * Returns environment specs
             * @return Env structure reference
             */
            static const Env &GetEnvInfo();

            /**
             * Returns OS specs
             * @return OS structure reference
             */
            static const OS &GetOSInfo();

            /**
             * Returns CPU specs
             * @return CPU structure reference
             */
            static const CPU &GetCPUInfo();

            /**
             * Returns current RAM specs
             * @return RAM structure reference
             */
            static RAM GetRAMInfo();

            /**
             * Returns the current platform endianess
             * @return enum value (see EPlatformEndianess)
             */
            static EPlatformEndianess GetEndianess();

            /**
             * Check if this application is running as administrator
             * @return true if the application is currently running as administrator, false otherwise
             */
            static bool IsRunningAsAdmin();

            /**
             * Reverse a buffer byte per byte.
             * WARNING : This function modifies the input buffer
             * @param buf the buffer to reverse
             * @param size the size of the buffer to reverse
             */
            static void ReverseBuffer(void *buf, const fsize size);

            /**
             * Reverse a buffer group per group.
             * WARNING : This function modifies the input buffer
             * @param buf the buffer to reverse
             * @param size the size of the buffer to reverse
             * @param groupsize the amount of bytes per group
             */
            static void ReverseBuffer(void *buf, const fsize size, const fsize groupsize);
        };
    }
}
