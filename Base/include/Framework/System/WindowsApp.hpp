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
#include "Framework/System/Application.hpp"
#include "Framework/Collection/List.hpp"

namespace bpf
{
    namespace system
    {
        class BPF_API WindowsApp final : public Application
        {
        private:
            void *_hInstance;
            bool _hasConsole;

            collection::HashMap<String, String> _env;
            String _fileName;
            Paths _props;
            void *_addDllDir;
            void *_rmDllDir;
            void *_kernel;
            collection::List<void *> _dlls;

            static collection::HashMap<String, String> SetupEnvironment();
            String SetupFileName();
            Paths SetupPaths();

        public:
            WindowsApp(void *hinstance, bool hasConsole);
            ~WindowsApp();

            void CreateConsole(fint rows, fint columns) final;

            static collection::Array<String> GetArguments();

            io::File GetWorkingDirectory() const final;
            bool SetWorkingDirectory(const io::File &file) final;
            void DisableErrorDialogs() noexcept final;
            void SetModuleDirectories(const collection::Array<io::File> &directories);
        };
    }
};