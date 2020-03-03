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
#include "Framework/Types.hpp"
#include "Framework/IO/IOutputStream.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace io
    {
        constexpr fsize WRITE_BUF_SIZE = 128;

        class BPF_API IDataOutputStream : public IOutputStream
        {
        public:
            virtual ~IDataOutputStream() {}
            virtual IDataOutputStream &operator<<(uint8 u) = 0;
            virtual IDataOutputStream &operator<<(uint16 u) = 0;
            virtual IDataOutputStream &operator<<(uint32 u) = 0;
            virtual IDataOutputStream &operator<<(uint64 u) = 0;
            virtual IDataOutputStream &operator<<(int8 i) = 0;
            virtual IDataOutputStream &operator<<(int16 i) = 0;
            virtual IDataOutputStream &operator<<(fint i) = 0;
            virtual IDataOutputStream &operator<<(int64 i) = 0;
            virtual IDataOutputStream &operator<<(float f) = 0;
            virtual IDataOutputStream &operator<<(double d) = 0;
            virtual IDataOutputStream &operator<<(bool b) = 0;
            virtual IDataOutputStream &operator<<(const bpf::String &str) = 0;
            virtual IDataOutputStream &operator<<(const char *str) = 0;
            virtual void Flush() = 0;
        };
    }
}
