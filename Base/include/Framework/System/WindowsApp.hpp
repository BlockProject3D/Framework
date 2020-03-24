// Copyright (c) 2020, BlockProject
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
#include "Framework/System/IApplication.hpp"

namespace bpf
{
    namespace system
    {
        class BPF_API WindowsApp final : public IApplication
        {
        private:
            void *_hInstance;
            bool _hasConsole;
            Paths _paths;
            collection::HashMap<String, String> _env;
            collection::Array<String> _args;
            String _fileName;

            void SetupEnvironment();
            void SetupArgs();
            void SetupFileName();
            void SetupPaths();

        public:
            WindowsApp(void *hinstance, bool hasConsole);

            void CreateConsole(const fint rows, const fint columns);

            const String &GetExeFileName() const noexcept
            {
                return (_fileName);
            }

            const collection::HashMap<String, String> &GetEnvironment() const noexcept
            {
                return (_env);
            }

            const collection::Array<String> &GetArguments() const noexcept
            {
                return (_args);
            }

            const Paths &GetPaths() const noexcept
            {
                return (_paths);
            }
        };
    }
};