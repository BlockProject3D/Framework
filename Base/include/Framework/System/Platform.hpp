// Copyright (c) 2018, BlockProject
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
//     * Neither the name of BlockProject nor the names of its contributors
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
        enum EPlatformEndianess
        {
            PLATFORM_BIGENDIAN,
            PLATFORM_LITTLEENDIAN
        };

        struct OS
        {
            String Name;
            String Version;
            String ModuleExt;
            String PathSep;
            String NewLine;
        };

        struct CPU
        {
            String Name;
            fint NumCores;
            fint Freq;
        };

        struct RAM
        {
            uint64 MaxPhysical;
            uint64 MaxVirtual;
        };

        struct Env
        {
            String ShortName;
            String Name;
            String Version;
            fint VersionInt;
        };

        class BPF_API Platform
        {
        private:
            static Env InitEnvInfo();
            static OS InitOSInfo();
            static String IdentifyCPUBranding();
            static String CPUIDIntToStr(fint data);

        public:
            static Env &GetEnvInfo();
            static OS &GetOSInfo();
            static CPU &GetCPUInfo();
            static RAM GetRAMInfo();
            static EPlatformEndianess GetEndianess();

            /**
             * Reverse a buffer byte per byte
             * WARNING : This function modifies the input buffer
             * @param buf the buffer to reverse
             * @param size the size of the buffer to reverse
             */
            static void ReverseBuffer(void *buf, const fsize size);

            /**
             * Reverse a buffer group per group
             * WARNING : This function modifies the input buffer
             * @param buf the buffer to reverse
             * @param size the size of the buffer to reverse
             * @param groupsize the amount of bytes per group
             */
            static void ReverseBuffer(void *buf, const fsize size, const fsize groupsize);
        };
    }
}
